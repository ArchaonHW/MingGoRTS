#Requires -Version 5.1

<#
.SYNOPSIS
    MingGoRTS 智能工作流選擇器
    Intelligent Workflow Selector for MingGoRTS

.DESCRIPTION
    根據專案狀態自動選擇最適合的工作流程
    Automatically selects the most appropriate workflow based on project status

.PARAMETER AutoMode
    啟動自動化模式，持續監控並執行最佳工作流

.PARAMETER ManualMode
    手動選擇工作流

.PARAMETER QuickFix
    快速修復模式 - 自動執行所有必要的修復工作流

.PARAMETER ProjectRoot
    專案根目錄路徑

.PARAMETER ScanInterval
    自動模式下掃描間隔（秒）

.EXAMPLE
    .\SmartWorkflowSelector.ps1 -AutoMode
    .\SmartWorkflowSelector.ps1 -QuickFix -ProjectRoot "F:\MingGoRTS"
    .\SmartWorkflowSelector.ps1 -ManualMode
#>

[CmdletBinding()]
param(
    [switch]$AutoMode,
    [switch]$ManualMode,
    [switch]$QuickFix,
    [string]$ProjectRoot = $PWD,
    [int]$ScanInterval = 60
)

# ============================================
# 全局配置
# ============================================

$Global:WorkflowConfig = @{
    ProjectRoot = $ProjectRoot
    WorkflowPath = Join-Path $ProjectRoot "Tools\workflow"
    LogsPath = Join-Path $ProjectRoot "Logs"
    ReportsPath = Join-Path $ProjectRoot "Reports"
    ScanInterval = $ScanInterval
    LastWorkflowRun = $null
    RunHistory = @()
}

# 工作流優先級配置
$Global:WorkflowPriority = @(
    @{ Name = "Debug"; Priority = 1; Condition = "HasBugs"; Script = "Debug-Workflow-EN.ps1" },
    @{ Name = "Compilation"; Priority = 2; Condition = "NeedsCompilation"; Script = "Compilation-Workflow-EN.ps1" },
    @{ Name = "CodeReview"; Priority = 3; Condition = "HasCodeChanges"; Script = "CodeReview-Workflow.ps1" },
    @{ Name = "VersionControl"; Priority = 4; Condition = "HasUncommittedChanges"; Script = "VersionControl-Workflow-EN.ps1" },
    @{ Name = "BMAD"; Priority = 5; Condition = "HasPendingTasks"; Script = "BMAD_WorkflowSystem_Final.ps1" }
)

# ============================================
# 日誌函數
# ============================================

function Write-SmartLog {
    param(
        [string]$Message,
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    
    # 確保日誌目錄存在
    if (-not (Test-Path $Global:WorkflowConfig.LogsPath)) {
        New-Item -ItemType Directory -Path $Global:WorkflowConfig.LogsPath -Force | Out-Null
    }
    
    $LogFile = Join-Path $Global:WorkflowConfig.LogsPath "SmartWorkflow_$(Get-Date -Format 'yyyyMMdd').log"
    Add-Content -Path $LogFile -Value $LogEntry
    
    # 控制台輸出顏色
    $ColorMap = @{
        "INFO" = "White"
        "SUCCESS" = "Green"
        "WARNING" = "Yellow"
        "ERROR" = "Red"
        "WORKFLOW" = "Cyan"
        "DECISION" = "Magenta"
    }
    
    Write-Host $LogEntry -ForegroundColor $ColorMap[$Level]
}

function Show-SmartBanner {
    Write-Host ""
    Write-Host "    ╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "    ║                                                              ║" -ForegroundColor Cyan
    Write-Host "    ║        MingGoRTS 智能工作流選擇器                           ║" -ForegroundColor Cyan
    Write-Host "    ║        Intelligent Workflow Selector                        ║" -ForegroundColor Cyan
    Write-Host "    ║                                                              ║" -ForegroundColor Cyan
    Write-Host "    ║        自動選擇最佳工作流程                                  ║" -ForegroundColor Cyan
    Write-Host "    ║        Auto-Select Best Workflow                            ║" -ForegroundColor Cyan
    Write-Host "    ║                                                              ║" -ForegroundColor Cyan
    Write-Host "    ╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
}

# ============================================
# 專案狀態檢測函數
# ============================================

function Test-ProjectState {
    Write-SmartLog "Analyzing project state..." "DECISION"
    
    $State = @{
        HasBugs = $false
        BugCount = 0
        NeedsCompilation = $false
        HasCodeChanges = $false
        HasUncommittedChanges = $false
        HasPendingTasks = $false
        LastCompilationStatus = "Unknown"
        RecommendedWorkflow = $null
        Priority = 0
    }
    
    # 1. 檢查是否有 Bug (通過掃描已知問題)
    $State.HasBugs = Test-HasBugs
    if ($State.HasBugs) {
        $State.BugCount = Get-BugCount
        Write-SmartLog "Detected $State.BugCount bugs in project" "WARNING"
    }
    
    # 2. 檢查是否需要編譯
    $State.NeedsCompilation = Test-NeedsCompilation
    if ($State.NeedsCompilation) {
        Write-SmartLog "Compilation needed - source files changed" "INFO"
    }
    
    # 3. 檢查是否有代碼變更
    $State.HasCodeChanges = Test-HasCodeChanges
    if ($State.HasCodeChanges) {
        Write-SmartLog "Code changes detected" "INFO"
    }
    
    # 4. 檢查版本控制狀態
    $State.HasUncommittedChanges = Test-UncommittedChanges
    if ($State.HasUncommittedChanges) {
        Write-SmartLog "Uncommitted changes detected" "INFO"
    }
    
    # 5. 檢查待處理任務
    $State.HasPendingTasks = Test-PendingTasks
    if ($State.HasPendingTasks) {
        Write-SmartLog "Pending BMAD tasks detected" "INFO"
    }
    
    return $State
}

function Test-HasBugs {
    # 檢查是否有已知錯誤文件
    $DebugReports = Get-ChildItem -Path (Join-Path $Global:WorkflowConfig.ProjectRoot "Tasks\Debug") -Filter "*.md" -ErrorAction SilentlyContinue
    if ($DebugReports) {
        # 檢查最新的報告是否有錯誤
        $LatestReport = $DebugReports | Sort-Object LastWriteTime -Descending | Select-Object -First 1
        $Content = Get-Content $LatestReport.FullName -Raw
        return $Content -match "Error|Bug|Syntax"
    }
    return $false
}

function Get-BugCount {
    $DebugReports = Get-ChildItem -Path (Join-Path $Global:WorkflowConfig.ProjectRoot "Tasks\Debug") -Filter "*.md" -ErrorAction SilentlyContinue
    if ($DebugReports) {
        $LatestReport = $DebugReports | Sort-Object LastWriteTime -Descending | Select-Object -First 1
        # 嘗試從報告中提取錯誤數
        $Content = Get-Content $LatestReport.FullName -Raw
        if ($Content -match "(\d+)\s*(errors?|bugs?|issues?)") {
            return [int]$Matches[1]
        }
    }
    return 0
}

function Test-NeedsCompilation {
    # 檢查源文件是否比上次編譯更新
    $SourcePath = Join-Path $Global:WorkflowConfig.ProjectRoot "Source"
    $BuildPath = Join-Path $Global:WorkflowConfig.ProjectRoot "Build"
    
    if (-not (Test-Path $BuildPath)) {
        return $true
    }
    
    $LastBuild = (Get-Item $BuildPath).LastWriteTime
    $SourceFiles = Get-ChildItem -Path $SourcePath -Recurse -Include "*.cpp", "*.h" -ErrorAction SilentlyContinue
    
    foreach ($File in $SourceFiles) {
        if ($File.LastWriteTime -gt $LastBuild) {
            return $true
        }
    }
    
    return $false
}

function Test-HasCodeChanges {
    # 檢查是否有未審查的代碼變更
    $SourcePath = Join-Path $Global:WorkflowConfig.ProjectRoot "Source"
    $CodeReviewPath = Join-Path $Global:WorkflowConfig.ProjectRoot "Tasks\CodeReview"
    
    $SourceFiles = Get-ChildItem -Path $SourcePath -Recurse -Include "*.cpp", "*.h", "*.cs" -ErrorAction SilentlyContinue
    $LastCodeReview = if (Test-Path $CodeReviewPath) { 
        (Get-Item $CodeReviewPath).LastWriteTime 
    } else { 
        [DateTime]::MinValue 
    }
    
    foreach ($File in $SourceFiles) {
        if ($File.LastWriteTime -gt $LastCodeReview) {
            return $true
        }
    }
    
    return $false
}

function Test-UncommittedChanges {
    # 檢查 Git 是否有未提交的變更
    $GitPath = Join-Path $Global:WorkflowConfig.ProjectRoot ".git"
    if (Test-Path $GitPath) {
        try {
            $Status = & git -C $Global:WorkflowConfig.ProjectRoot status --short 2>$null
            return -not [string]::IsNullOrWhiteSpace($Status)
        }
        catch {
            return $false
        }
    }
    return $false
}

function Test-PendingTasks {
    # 檢查是否有待處理的 BMAD 任務
    $TasksPath = Join-Path $Global:WorkflowConfig.ProjectRoot "Tasks"
    $PendingFiles = Get-ChildItem -Path $TasksPath -Recurse -Include "*.json", "*.md" -ErrorAction SilentlyContinue | 
        Where-Object { $_.Name -match "pending|todo|task" }
    return $PendingFiles.Count -gt 0
}

# ============================================
# 智能決策引擎
# ============================================

function Select-BestWorkflow {
    param([hashtable]$State)
    
    Write-SmartLog "Selecting best workflow based on project state..." "DECISION"
    
    $Scores = @()
    
    # 評分邏輯
    if ($State.HasBugs -and $State.BugCount -gt 0) {
        $Scores += @{ Name = "Debug"; Score = 100; Reason = "Found $State.BugCount bugs"; Script = "Debug-Workflow-EN.ps1" }
    }
    
    if ($State.NeedsCompilation) {
        $Scores += @{ Name = "Compilation"; Score = 80; Reason = "Source files changed since last build"; Script = "Compilation-Workflow-EN.ps1" }
    }
    
    if ($State.HasCodeChanges) {
        $Scores += @{ Name = "CodeReview"; Score = 60; Reason = "Code changes need review"; Script = "CodeReview-Workflow.ps1" }
    }
    
    if ($State.HasUncommittedChanges) {
        $Scores += @{ Name = "VersionControl"; Score = 40; Reason = "Uncommitted changes detected"; Script = "VersionControl-Workflow-EN.ps1" }
    }
    
    if ($State.HasPendingTasks) {
        $Scores += @{ Name = "BMAD"; Score = 30; Reason = "Pending BMAD tasks"; Script = "BMAD_WorkflowSystem_Final.ps1" }
    }
    
    if ($Scores.Count -eq 0) {
        Write-SmartLog "No workflow needed - project is in good state!" "SUCCESS"
        return $null
    }
    
    # 選擇最高分的工作流
    $BestWorkflow = $Scores | Sort-Object Score -Descending | Select-Object -First 1
    
    Write-SmartLog "Selected workflow: $($BestWorkflow.Name) (Score: $($BestWorkflow.Score))" "DECISION"
    Write-SmartLog "Reason: $($BestWorkflow.Reason)" "INFO"
    
    return $BestWorkflow
}

# ============================================
# 工作流執行函數
# ============================================

function Invoke-SelectedWorkflow {
    param([hashtable]$Workflow)
    
    if (-not $Workflow) {
        Write-SmartLog "No workflow selected" "WARNING"
        return $null
    }
    
    $ScriptPath = Join-Path $Global:WorkflowConfig.WorkflowPath $Workflow.Script
    
    if (-not (Test-Path $ScriptPath)) {
        # 嘗試尋找替代版本
        $AlternativeScripts = Get-ChildItem -Path $Global:WorkflowConfig.WorkflowPath -Filter "*$($Workflow.Name)*.ps1" -ErrorAction SilentlyContinue
        if ($AlternativeScripts) {
            $ScriptPath = $AlternativeScripts | Select-Object -First 1 | Select-Object -ExpandProperty FullName
        }
    }
    
    if (-not (Test-Path $ScriptPath)) {
        Write-SmartLog "Workflow script not found: $ScriptPath" "ERROR"
        return $null
    }
    
    Write-SmartLog "Executing workflow: $($Workflow.Name)" "WORKFLOW"
    Write-SmartLog "Script: $ScriptPath" "INFO"
    
    $StartTime = Get-Date
    $Result = @{
        Workflow = $Workflow.Name
        StartTime = $StartTime
        EndTime = $null
        Success = $false
        Output = ""
        ExitCode = -1
    }
    
    try {
        # 根據工作流類型選擇適當的參數
        $Arguments = switch ($Workflow.Name) {
            "Debug" { @("-DeepScan", "-ReportToBMAD") }
            "Compilation" { @("-AutoMode", "-ScanInterval", "30") }
            "VersionControl" { @("-AutoMode", "-ScanInterval", "60") }
            "CodeReview" { @("-DeepSecurityScan", "-GenerateReport") }
            "BMAD" { @() }
            default { @() }
        }
        
        # 執行工作流
        $Process = Start-Process -FilePath "powershell.exe" -ArgumentList @("-ExecutionPolicy", "Bypass", "-File", "`"$ScriptPath`"", $Arguments) -Wait -PassThru -WorkingDirectory $Global:WorkflowConfig.WorkflowPath
        
        $Result.ExitCode = $Process.ExitCode
        $Result.Success = ($Process.ExitCode -eq 0)
        $Result.EndTime = Get-Date
        
        if ($Result.Success) {
            Write-SmartLog "Workflow completed successfully: $($Workflow.Name)" "SUCCESS"
        }
        else {
            Write-SmartLog "Workflow failed with exit code: $($Process.ExitCode)" "ERROR"
        }
    }
    catch {
        $Result.EndTime = Get-Date
        Write-SmartLog "Workflow execution error: $($_.Exception.Message)" "ERROR"
    }
    
    # 記錄到歷史
    $Global:WorkflowConfig.RunHistory += $Result
    $Global:WorkflowConfig.LastWorkflowRun = $Result
    
    return $Result
}

# ============================================
# 手動模式
# ============================================

function Show-ManualMenu {
    Write-Host ""
    Write-Host "    ╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Yellow
    Write-Host "    ║              手動工作流選擇                                   ║" -ForegroundColor Yellow
    Write-Host "    ║              Manual Workflow Selection                       ║" -ForegroundColor Yellow
    Write-Host "    ╠══════════════════════════════════════════════════════════════╣" -ForegroundColor Yellow
    Write-Host "    ║  1. Debug Workflow      - Bug 檢測與修復                     ║" -ForegroundColor White
    Write-Host "    ║  2. Compilation Workflow - 編譯管理                          ║" -ForegroundColor White
    Write-Host "    ║  3. Code Review Workflow - 代碼審查                         ║" -ForegroundColor White
    Write-Host "    ║  4. Version Control     - 版本控制                          ║" -ForegroundColor White
    Write-Host "    ║  5. BMAD Workflow       - BMAD 任務管理                     ║" -ForegroundColor White
    Write-Host "    ║  6. Quick Fix Mode      - 快速修復 (執行所有必要工作流)    ║" -ForegroundColor Green
    Write-Host "    ║  7. Auto Analysis       - 智能分析當前狀態                  ║" -ForegroundColor Cyan
    Write-Host "    ║  0. Exit                - 退出                             ║" -ForegroundColor Red
    Write-Host "    ╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Yellow
    Write-Host ""
}

function Start-ManualMode {
    Write-SmartLog "Starting manual mode..." "INFO"
    
    do {
        Show-ManualMenu
        $Choice = Read-Host "請選擇工作流 (0-7)"
        
        switch ($Choice) {
            "1" { 
                Write-SmartLog "User selected: Debug Workflow" "INFO"
                Invoke-SelectedWorkflow -Workflow @{ Name = "Debug"; Script = "Debug-Workflow-EN.ps1" }
            }
            "2" { 
                Write-SmartLog "User selected: Compilation Workflow" "INFO"
                Invoke-SelectedWorkflow -Workflow @{ Name = "Compilation"; Script = "Compilation-Workflow-EN.ps1" }
            }
            "3" { 
                Write-SmartLog "User selected: Code Review Workflow" "INFO"
                Invoke-SelectedWorkflow -Workflow @{ Name = "CodeReview"; Script = "CodeReview-Workflow.ps1" }
            }
            "4" { 
                Write-SmartLog "User selected: Version Control Workflow" "INFO"
                Invoke-SelectedWorkflow -Workflow @{ Name = "VersionControl"; Script = "VersionControl-Workflow-EN.ps1" }
            }
            "5" { 
                Write-SmartLog "User selected: BMAD Workflow" "INFO"
                Invoke-SelectedWorkflow -Workflow @{ Name = "BMAD"; Script = "BMAD_WorkflowSystem_Final.ps1" }
            }
            "6" { 
                Write-SmartLog "User selected: Quick Fix Mode" "INFO"
                Start-QuickFixMode
            }
            "7" { 
                Write-SmartLog "User selected: Auto Analysis" "INFO"
                $State = Test-ProjectState
                $BestWorkflow = Select-BestWorkflow -State $State
                if ($BestWorkflow) {
                    Write-Host ""
                    Write-Host "推薦工作流: $($BestWorkflow.Name)" -ForegroundColor Green
                    Write-Host "原因: $($BestWorkflow.Reason)" -ForegroundColor White
                    $Confirm = Read-Host "是否執行此工作流? (Y/N)"
                    if ($Confirm -eq "Y" -or $Confirm -eq "y") {
                        Invoke-SelectedWorkflow -Workflow $BestWorkflow
                    }
                }
                else {
                    Write-Host "專案狀態良好，無需執行任何工作流！" -ForegroundColor Green
                }
            }
            "0" { 
                Write-SmartLog "Exiting manual mode" "INFO"
                return
            }
            default {
                Write-SmartLog "Invalid selection: $Choice" "WARNING"
            }
        }
        
        Write-Host ""
        Write-Host "按 Enter 鍵繼續..." -ForegroundColor Gray
        Read-Host | Out-Null
        
    } while ($Choice -ne "0")
}

# ============================================
# 快速修復模式
# ============================================

function Start-QuickFixMode {
    Write-SmartLog "Starting Quick Fix Mode..." "WORKFLOW"
    Write-SmartLog "Will execute all necessary workflows automatically" "INFO"
    
    $State = Test-ProjectState
    $ExecutionOrder = @()
    
    # 確定執行順序
    if ($State.HasBugs) {
        $ExecutionOrder += @{ Name = "Debug"; Priority = 1; Script = "Debug-Workflow-EN.ps1"; Args = @("-DeepScan", "-ReportToBMAD") }
    }
    
    if ($State.NeedsCompilation) {
        $ExecutionOrder += @{ Name = "Compilation"; Priority = 2; Script = "Compilation-Workflow-EN.ps1"; Args = @() }
    }
    
    if ($State.HasCodeChanges) {
        $ExecutionOrder += @{ Name = "CodeReview"; Priority = 3; Script = "CodeReview-Workflow.ps1"; Args = @("-GenerateReport") }
    }
    
    if ($State.HasUncommittedChanges) {
        $ExecutionOrder += @{ Name = "VersionControl"; Priority = 4; Script = "VersionControl-Workflow-EN.ps1"; Args = @() }
    }
    
    if ($ExecutionOrder.Count -eq 0) {
        Write-SmartLog "No workflows needed - project is already in good state!" "SUCCESS"
        return
    }
    
    Write-SmartLog "Will execute $($ExecutionOrder.Count) workflows in sequence" "INFO"
    
    $SuccessCount = 0
    $FailCount = 0
    
    foreach ($Workflow in $ExecutionOrder | Sort-Object Priority) {
        Write-SmartLog "`n==========================================" "WORKFLOW"
        Write-SmartLog "Executing [$($SuccessCount + $FailCount + 1)/$ExecutionOrder.Count]: $($Workflow.Name)" "WORKFLOW"
        Write-SmartLog "==========================================" "WORKFLOW"
        
        $Result = Invoke-SelectedWorkflow -Workflow $Workflow
        
        if ($Result.Success) {
            $SuccessCount++
        }
        else {
            $FailCount++
            Write-SmartLog "Workflow failed, stopping quick fix mode" "ERROR"
            break
        }
    }
    
    Write-SmartLog "`n==========================================" "SUCCESS"
    Write-SmartLog "Quick Fix Mode Complete!" "SUCCESS"
    Write-SmartLog "Success: $SuccessCount, Failed: $FailCount" $(if ($FailCount -eq 0) { "SUCCESS" } else { "WARNING" })
    Write-SmartLog "==========================================" "SUCCESS"
}

# ============================================
# 自動模式
# ============================================

function Start-AutoMode {
    Write-SmartLog "Starting Auto Mode..." "INFO"
    Write-SmartLog "Scan interval: $($Global:WorkflowConfig.ScanInterval) seconds" "INFO"
    Write-SmartLog "Press Ctrl+C to stop" "WARNING"
    
    $CycleCount = 0
    
    while ($true) {
        try {
            $CycleCount++
            Write-SmartLog "`n--- Auto Mode Cycle #$CycleCount - $(Get-Date -Format 'HH:mm:ss') ---" "INFO"
            
            # 檢測專案狀態
            $State = Test-ProjectState
            
            # 選擇最佳工作流
            $BestWorkflow = Select-BestWorkflow -State $State
            
            if ($BestWorkflow) {
                # 執行工作流
                $Result = Invoke-SelectedWorkflow -Workflow $BestWorkflow
                
                if (-not $Result.Success) {
                    Write-SmartLog "Workflow failed, waiting before retry..." "WARNING"
                    Start-Sleep -Seconds ($Global:WorkflowConfig.ScanInterval * 2)
                }
            }
            else {
                Write-SmartLog "No workflow needed, waiting..." "INFO"
            }
            
            Write-SmartLog "Waiting $($Global:WorkflowConfig.ScanInterval) seconds for next cycle..." "DEBUG"
            Start-Sleep -Seconds $Global:WorkflowConfig.ScanInterval
        }
        catch {
            Write-SmartLog "Auto mode error: $($_.Exception.Message)" "ERROR"
            Write-SmartLog "Waiting 60 seconds before retry..." "WARNING"
            Start-Sleep -Seconds 60
        }
    }
}

# ============================================
# 報告生成
# ============================================

function New-SmartWorkflowReport {
    $ReportFile = Join-Path $Global:WorkflowConfig.ReportsPath "SmartWorkflowReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    if (-not (Test-Path $Global:WorkflowConfig.ReportsPath)) {
        New-Item -ItemType Directory -Path $Global:WorkflowConfig.ReportsPath -Force | Out-Null
    }
    
    $Report = @"
# Smart Workflow Selector Report

**Generated**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Project**: $($Global:WorkflowConfig.ProjectRoot)
**Total Runs**: $($Global:WorkflowConfig.RunHistory.Count)

## Execution Summary

| Workflow | Start Time | End Time | Duration | Success |
|----------|------------|----------|----------|---------
"@

    foreach ($Run in $Global:WorkflowConfig.RunHistory) {
        $Duration = if ($Run.EndTime) { ($Run.EndTime - $Run.StartTime).ToString("hh\:mm\:ss") } else { "N/A" }
        $Status = if ($Run.Success) { "✅" } else { "❌" }
        $Report += "| $($Run.Workflow) | $($Run.StartTime -f 'HH:mm:ss') | $($Run.EndTime -f 'HH:mm:ss') | $Duration | $Status |`n"
    }

    $Report += @"

## System Configuration

- **Scan Interval**: $($Global:WorkflowConfig.ScanInterval) seconds
- **Project Root**: $($Global:WorkflowConfig.ProjectRoot)
- **Workflow Path**: $($Global:WorkflowConfig.WorkflowPath)

---
*Generated by MingGoRTS Smart Workflow Selector*
"@

    $Report | Set-Content -Path $ReportFile
    Write-SmartLog "Report generated: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# 主執行流程
# ============================================

function Main {
    Show-SmartBanner
    
    # 驗證專案路徑
    if (-not (Test-Path $Global:WorkflowConfig.ProjectRoot)) {
        Write-SmartLog "Project path not found: $($Global:WorkflowConfig.ProjectRoot)" "ERROR"
        exit 1
    }
    
    Write-SmartLog "Project Root: $($Global:WorkflowConfig.ProjectRoot)" "INFO"
    Write-SmartLog "Workflow Path: $($Global:WorkflowConfig.WorkflowPath)" "INFO"
    
    # 確保工作流目錄存在
    if (-not (Test-Path $Global:WorkflowConfig.WorkflowPath)) {
        Write-SmartLog "Workflow directory not found!" "ERROR"
        exit 1
    }
    
    # 根據參數選擇模式
    if ($QuickFix) {
        Write-SmartLog "Quick Fix Mode selected" "INFO"
        Start-QuickFixMode
        New-SmartWorkflowReport
    }
    elseif ($AutoMode) {
        Write-SmartLog "Auto Mode selected" "INFO"
        Start-AutoMode
    }
    elseif ($ManualMode) {
        Write-SmartLog "Manual Mode selected" "INFO"
        Start-ManualMode
        New-SmartWorkflowReport
    }
    else {
        # 默認：智能分析一次，然後推薦
        Write-SmartLog "Smart Analysis Mode (default)" "INFO"
        
        $State = Test-ProjectState
        $BestWorkflow = Select-BestWorkflow -State $State
        
        if ($BestWorkflow) {
            Write-Host ""
            Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
            Write-Host "  推薦工作流: $($BestWorkflow.Name)" -ForegroundColor Green
            Write-Host "  原因: $($BestWorkflow.Reason)" -ForegroundColor White
            Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
            Write-Host ""
            
            $Choice = Read-Host "選擇操作: [1]執行 [2]手動選擇 [3]快速修復 [0]退出"
            
            switch ($Choice) {
                "1" { Invoke-SelectedWorkflow -Workflow $BestWorkflow }
                "2" { Start-ManualMode }
                "3" { Start-QuickFixMode }
                "0" { Write-SmartLog "Exiting..." "INFO"; exit 0 }
                default { Write-SmartLog "Invalid choice, exiting" "WARNING" }
            }
        }
        else {
            Write-Host ""
            Write-Host "✅ 專案狀態良好！無需執行任何工作流。" -ForegroundColor Green
            Write-Host ""
            Write-Host "可用操作:" -ForegroundColor Yellow
            Write-Host "  [1] 手動選擇工作流" -ForegroundColor White
            Write-Host "  [2] 啟動自動監控模式" -ForegroundColor White
            Write-Host "  [0] 退出" -ForegroundColor Red
            Write-Host ""
            
            $Choice = Read-Host "請選擇"
            switch ($Choice) {
                "1" { Start-ManualMode }
                "2" { Start-AutoMode }
                "0" { exit 0 }
            }
        }
        
        New-SmartWorkflowReport
    }
    
    Write-SmartLog "Smart Workflow Selector completed" "SUCCESS"
}

# 執行主程序
Main
