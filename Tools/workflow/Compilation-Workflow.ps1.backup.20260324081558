#!/usr/bin/env pwsh

<#
.SYNOPSIS
    MingGoRTS Compilation Workflow System
    自動化編譯工作流程系統

.DESCRIPTION
    1. 檢測需求單任務狀態為"已開發"的任務
    2. 自動認領並標記為"待編譯"
    3. 根據任務數量選擇編譯模式（完整/批量/深度）
    4. 執行編譯並標記狀態
    5. 成功：標記為"CodeReview"，失敗：標記為"Debug"

.PARAMETER AutoMode
    自動化操作模式

.PARAMETER ScanInterval
    掃描間隔（秒）

.PARAMETER MaxConcurrentTasks
    最大並發任務數

.EXAMPLE
    .\Compilation-Workflow.ps1 -AutoMode
    自動化編譯模式

.EXAMPLE
    .\Compilation-Workflow.ps1 -ManualMode
    手動編譯模式
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoMode,
    
    [Parameter()]
    [int]$ScanInterval = 30,
    
    [Parameter()]
    [int]$MaxConcurrentTasks = 3,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TasksPath = "$PWD\Tasks",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\CompilationWorkflow.log"
)

$ErrorActionPreference = "Stop"

# 顏色定義
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
    Compilation = "Blue"
}

# 編譯追蹤器
$CompilationTracker = @{
    CurrentTasks = @()
    CompletedTasks = @()
    FailedTasks = @()
    TotalCompiled = 0
    TotalFailed = 0
    StartTime = Get-Date
}

function Write-CompilationLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG", "COMPILATION")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
        "COMPILATION" { $Colors.Compilation }
        default { $Colors.Info }
    }
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    # 寫入日誌文件
    $LogDir = Split-Path -Parent $LogPath
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
    }
    "[$Timestamp] [$Level] $Message" | Add-Content -Path $LogPath -ErrorAction SilentlyContinue
}

function Show-CompilationBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║            MingGoRTS Compilation Workflow System           ║
    ║              自動化編譯工作流程系統                        ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.Compilation
}

# ============================================
# 階段 1: 需求單掃描與任務檢測
# ============================================

function Find-DevelopedTasks {
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "階段 1: 掃描已開發任務" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $DevelopedTasks = @()
    
    # 掃描所有需求單目錄
    $RequirementDirs = Get-ChildItem -Path $TasksPath -Directory -ErrorAction SilentlyContinue
    
    foreach ($Dir in $RequirementDirs) {
        $RequirementFiles = Get-ChildItem -Path $Dir.FullName -Filter "Requirement_*.json" -ErrorAction SilentlyContinue
        
        foreach ($File in $RequirementFiles) {
            try {
                $RequirementData = Get-Content -Path $File.FullName -Raw | ConvertFrom-Json
                
                # 檢查是否有已開發的任務
                $DevelopedTasksInReq = $RequirementData.Tasks | Where-Object { 
                    $_.Status -eq "Developed" -or 
                    ($_.Status -eq "Done" -and $_.Type -eq "Development") 
                }
                
                if ($DevelopedTasksInReq.Count -gt 0) {
                    foreach ($Task in $DevelopedTasksInReq) {
                        $TaskInfo = @{
                            RequirementFile = $File.FullName
                            RequirementID = $RequirementData.RequirementID
                            TaskID = $Task.ID
                            TaskTitle = $Task.Title
                            TaskType = $Task.Type
                            TaskPriority = $Task.Priority
                            CurrentStatus = $Task.Status
                            EstimatedHours = $Task.EstimatedHours
                            Dependencies = $Task.Dependencies
                        }
                        $DevelopedTasks += $TaskInfo
                    }
                    
                    Write-CompilationLog "  發現已開發任務: $($RequirementData.RequirementID) - $($DevelopedTasksInReq.Count) 個任務" "DEBUG"
                }
            }
            catch {
                Write-CompilationLog "  錯誤讀取需求單: $($File.Name) - $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-CompilationLog "掃描完成: 發現 $($DevelopedTasks.Count) 個已開發任務" $(if ($DevelopedTasks.Count -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $DevelopedTasks
}

# ============================================
# 階段 2: 任務認領與狀態更新
# ============================================

function Request-TasksForCompilation {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "階段 2: 認領編譯任務" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $ClaimedTasks = @()
    
    foreach ($Task in $Tasks) {
        try {
            # 讀取需求單文件
            $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
            
            # 找到對應任務
            $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
            
            if ($TaskToUpdate) {
                # 更新任務狀態
                $TaskToUpdate.Status = "PendingCompilation"
                $TaskToUpdate.ClaimedBy = "CompilationWorkflow"
                $TaskToUpdate.ClaimedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
                
                # 保存更新後的需求單
                $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
                
                $ClaimedTask = $Task.Clone()
                $ClaimedTask.UpdatedStatus = "PendingCompilation"
                $ClaimedTasks += $ClaimedTask
                
                Write-CompilationLog "  [✓] 認領任務: $($Task.TaskID) - $($Task.TaskTitle)" "SUCCESS"
            }
            else {
                Write-CompilationLog "  [✗] 未找到任務: $($Task.TaskID)" "ERROR"
            }
        }
        catch {
            Write-CompilationLog "  [✗] 認領失敗: $($Task.TaskID) - $($_.Exception.Message)" "ERROR"
        }
    }
    
    Write-CompilationLog "認領完成: 成功認領 $($ClaimedTasks.Count) 個任務" "SUCCESS"
    
    return $ClaimedTasks
}

# ============================================
# 階段 3: 編譯模式選擇
# ============================================

function Select-CompilationMode {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "階段 3: 選擇編譯模式" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $TaskCount = $Tasks.Count
    $CompilationMode = ""
    
    if ($TaskCount -eq 0) {
        Write-CompilationLog "沒有任務需要編譯" "INFO"
        return $null
    }
    elseif ($TaskCount -eq 1) {
        $CompilationMode = "Single"
        Write-CompilationLog "選擇模式: 單一任務編譯" "INFO"
    }
    elseif ($TaskCount -le $MaxConcurrentTasks) {
        $CompilationMode = "Batch"
        Write-CompilationLog "選擇模式: 批量編譯 ($TaskCount 個任務)" "INFO"
    }
    else {
        $CompilationMode = "Deep"
        Write-CompilationLog "選擇模式: 深度編譯 (大量任務: $TaskCount)" "INFO"
    }
    
    # 分析任務類型和優先級
    $HighPriorityTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Critical" -or $_.TaskPriority -eq "High" }
    $NormalTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Medium" -or $_.TaskPriority -eq "Low" }
    
    Write-CompilationLog "任務分析:" "DEBUG"
    Write-CompilationLog "  高優先級: $($HighPriorityTasks.Count) 個" "DEBUG"
    Write-CompilationLog "  普通優先級: $($NormalTasks.Count) 個" "DEBUG"
    
    return @{
        Mode = $CompilationMode
        HighPriorityTasks = $HighPriorityTasks
        NormalTasks = $NormalTasks
        TotalTasks = $Tasks
    }
}

# ============================================
# 階段 4: 編譯執行
# ============================================

function Start-CompilationProcess {
    [CmdletBinding()]
    param(
        [string]$Mode,
        [array]$HighPriorityTasks,
        [array]$NormalTasks
    )
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "階段 4: 執行編譯過程" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $AllTasks = $HighPriorityTasks + $NormalTasks
    $CompilationResults = @()
    
    foreach ($Task in $AllTasks) {
        Write-CompilationLog "`n開始編譯任務: $($Task.TaskID) - $($Task.TaskTitle)" "COMPILATION"
        
        # 標記為編譯中
        Update-TaskStatus -Task $Task -Status "Compiling"
        
        # 執行編譯
        $Result = Invoke-TaskCompilation -Task $Task -Mode $Mode
        
        # 更新最終狀態
        if ($Result.Success) {
            Update-TaskStatus -Task $Task -Status "CodeReview" -Result $Result
            $CompilationTracker.TotalCompiled++
            Write-CompilationLog "  [✓] 編譯成功: $($Task.TaskID)" "SUCCESS"
        }
        else {
            Update-TaskStatus -Task $Task -Status "Debug" -Result $Result
            $CompilationTracker.TotalFailed++
            Write-CompilationLog "  [✗] 編譯失敗: $($Task.TaskID) - $($Result.ErrorMessage)" "ERROR"
        }
        
        $CompilationResults += $Result
        $CompilationTracker.CurrentTasks = $CompilationTracker.CurrentTasks | Where-Object { $_.TaskID -ne $Task.TaskID }
    }
    
    Write-CompilationLog "編譯完成: 成功 $($CompilationTracker.TotalCompiled) 個，失敗 $($CompilationTracker.TotalFailed) 個" "SUCCESS"
    
    return $CompilationResults
}

function Invoke-TaskCompilation {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Mode
    )
    
    $Result = @{
        TaskID = $Task.TaskID
        Success = $false
        CompilationTime = 0
        Output = ""
        ErrorMessage = ""
        Warnings = @()
    }
    
    $Stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    
    try {
        Write-CompilationLog "  執行編譯命令..." "DEBUG"
        
        # 執行UE編譯
        $UEProjectFile = Join-Path $ProjectRoot "MingGoRTS.uproject"
        if (Test-Path $UEProjectFile) {
            $CompileArgs = @(
                "-projectfiles",
                "-project=$UEProjectFile",
                "-game",
                "-rocket",
                "-progress"
            )
            
            # 執行編譯
            $Process = Start-Process -FilePath "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -ArgumentList $CompileArgs -Wait -PassThru -RedirectStandardOutput "$($env:TEMP)\Compilation_$($Task.TaskID).log" -RedirectStandardError "$($env:TEMP)\Compilation_$($Task.TaskID)_error.log"
            
            $Result.Output = Get-Content -Path "$($env:TEMP)\Compilation_$($Task.TaskID).log" -Raw -ErrorAction SilentlyContinue
            $ErrorOutput = Get-Content -Path "$($env:TEMP)\Compilation_$($Task.TaskID)_error.log" -Raw -ErrorAction SilentlyContinue
            
            if ($Process.ExitCode -eq 0) {
                $Result.Success = $true
                Write-CompilationLog "    UE編譯成功" "DEBUG"
            }
            else {
                $Result.ErrorMessage = "UE編譯失敗，退出代碼: $($Process.ExitCode)"
                if ($ErrorOutput) {
                    $Result.ErrorMessage += "`n錯誤詳情: $ErrorOutput"
                }
            }
        }
        else {
            # 備用編譯方案 - 使用MSBuild
            $SLNPath = Join-Path $ProjectRoot "MingGoRTS.sln"
            if (Test-Path $SLNPath) {
                $Process = Start-Process -FilePath "msbuild.exe" -ArgumentList "$SLNPath", "/p:Configuration=Shipping", "/p:Platform=x64", "/verbosity:minimal" -Wait -PassThru -RedirectStandardOutput "$($env:TEMP)\Compilation_$($Task.TaskID).log" -RedirectStandardError "$($env:TEMP)\Compilation_$($Task.TaskID)_error.log"
                
                $Result.Output = Get-Content -Path "$($env:TEMP)\Compilation_$($Task.TaskID).log" -Raw -ErrorAction SilentlyContinue
                $ErrorOutput = Get-Content -Path "$($env:TEMP)\Compilation_$($Task.TaskID)_error.log" -Raw -ErrorAction SilentlyContinue
                
                if ($Process.ExitCode -eq 0) {
                    $Result.Success = $true
                    Write-CompilationLog "    MSBuild編譯成功" "DEBUG"
                }
                else {
                    $Result.ErrorMessage = "MSBuild編譯失敗，退出代碼: $($Process.ExitCode)"
                    if ($ErrorOutput) {
                        $Result.ErrorMessage += "`n錯誤詳情: $ErrorOutput"
                    }
                }
            }
            else {
                $Result.ErrorMessage = "找不到編譯項目文件 (.uproject 或 .sln)"
            }
        }
        
        # 清理臨時文件
        Remove-Item -Path "$($env:TEMP)\Compilation_$($Task.TaskID).log" -ErrorAction SilentlyContinue
        Remove-Item -Path "$($env:TEMP)\Compilation_$($Task.TaskID)_error.log" -ErrorAction SilentlyContinue
    }
    catch {
        $Result.ErrorMessage = "編譯過程發生異常: $($_.Exception.Message)"
        Write-CompilationLog "    編譯異常: $($_.Exception.Message)" "ERROR"
    }
    finally {
        $Stopwatch.Stop()
        $Result.CompilationTime = $Stopwatch.Elapsed.TotalSeconds
    }
    
    return $Result
}

function Update-TaskStatus {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Status,
        [hashtable]$Result = $null
    )
    
    try {
        # 讀取需求單文件
        $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
        
        # 找到對應任務
        $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
        
        if ($TaskToUpdate) {
            $TaskToUpdate.Status = $Status
            $TaskToUpdate.UpdatedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
            
            if ($Result) {
                $TaskToUpdate.CompilationResult = @{
                    Success = $Result.Success
                    CompilationTime = $Result.CompilationTime
                    ErrorMessage = $Result.ErrorMessage
                    Warnings = $Result.Warnings
                }
            }
            
            # 保存更新後的需求單
            $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
            
            Write-CompilationLog "    狀態更新: $($Task.TaskID) -> $Status" "DEBUG"
        }
    }
    catch {
        Write-CompilationLog "    狀態更新失敗: $($_.Exception.Message)" "ERROR"
    }
}

# ============================================
# 階段 5: 編譯報告生成
# ============================================

function New-CompilationReport {
    [CmdletBinding()]
    param([array]$Results)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "階段 5: 生成編譯報告" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $ReportFile = Join-Path $TasksPath "Compilation\CompilationReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    # 確保目錄存在
    $ReportDir = Split-Path -Parent $ReportFile
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $TotalTime = (Get-Date) - $CompilationTracker.StartTime
    
    $Report = @"
# 編譯工作流程報告

**執行時間**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**專案路徑**: $ProjectRoot  
**總執行時間**: $($TotalTime.ToString("hh\:mm\:ss"))

## 編譯摘要

| 指標 | 數值 |
|------|------|
| 總任務數 | $($Results.Count) |
| 成功編譯 | $($CompilationTracker.TotalCompiled) |
| 編譯失敗 | $($CompilationTracker.TotalFailed) |
| 成功率 | $([math]::Round(($CompilationTracker.TotalCompiled / [math]::Max($Results.Count, 1)) * 100, 2))% |

## 詳細結果

### ✅ 成功編譯的任務

"@
    
    foreach ($Result in $Results | Where-Object { $_.Success }) {
        $Report += "- **$($Result.TaskID)**: 編譯成功 (耗時: $([math]::Round($Result.CompilationTime, 2))秒)`n"
    }
    
    $Report += @"

### ❌ 編譯失敗的任務

"@
    
    foreach ($Result in $Results | Where-Object { -not $_.Success }) {
        $Report += "- **$($Result.TaskID)**: 編譯失敗 - $($Result.ErrorMessage)`n"
    }
    
    $Report += @"

## 編譯統計

- 平均編譯時間: $([math]::Round(($Results | Measure-Object -Property CompilationTime -Average).Average, 2))秒
- 最長編譯時間: $([math]::Round(($Results | Measure-Object -Property CompilationTime -Maximum).Maximum, 2))秒
- 最短編譯時間: $([math]::Round(($Results | Measure-Object -Property CompilationTime -Minimum).Minimum, 2))秒

## 下一步操作

1. 檢查編譯失敗的任務並修復問題
2. 對成功編譯的任務進行代碼審查
3. 重新運行編譯工作流程驗證修復

---
*由 MingGoRTS Compilation Workflow System 自動生成*
"@
    
    $Report | Set-Content -Path $ReportFile
    Write-CompilationLog "編譯報告已生成: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# 自動化模式監控
# ============================================

function Start-AutoMonitoring {
    Write-CompilationLog "啟動自動化監控模式，掃描間隔: $ScanInterval 秒" "INFO"
    
    while ($true) {
        try {
            Write-CompilationLog "`n--- 開始掃描 $(Get-Date -Format 'HH:mm:ss') ---" "DEBUG"
            
            # 尋找已開發任務
            $DevelopedTasks = Find-DevelopedTasks
            
            if ($DevelopedTasks.Count -gt 0) {
                Write-CompilationLog "發現 $($DevelopedTasks.Count) 個待編譯任務，開始處理..." "INFO"
                
                # 認領任務
                $ClaimedTasks = Request-TasksForCompilation -Tasks $DevelopedTasks
                
                if ($ClaimedTasks.Count -gt 0) {
                    # 選擇編譯模式
                    $CompilationPlan = Select-CompilationMode -Tasks $ClaimedTasks
                    
                    if ($CompilationPlan) {
                        # 執行編譯
                        $Results = Start-CompilationProcess -Mode $CompilationPlan.Mode -HighPriorityTasks $CompilationPlan.HighPriorityTasks -NormalTasks $CompilationPlan.NormalTasks
                        
                        # 生成報告
                        $ReportFile = New-CompilationReport -Results $Results
                        Write-CompilationLog "編譯週期完成，報告: $ReportFile" "SUCCESS"
                    }
                }
            }
            else {
                Write-CompilationLog "未發現待編譯任務，繼續監控..." "INFO"
            }
            
            Write-CompilationLog "等待 $ScanInterval 秒後進行下次掃描..." "DEBUG"
            Start-Sleep -Seconds $ScanInterval
        }
        catch {
            Write-CompilationLog "自動監控發生錯誤: $($_.Exception.Message)" "ERROR"
            Start-Sleep -Seconds 60 # 錯誤時等待較長時間
        }
    }
}

# ============================================
# 主執行流程
# ============================================

function Main {
    Show-CompilationBanner
    
    if ($AutoMode) {
        Write-CompilationLog "啟動自動化編譯模式" "INFO"
        Start-AutoMonitoring
    }
    else {
        Write-CompilationLog "執行單次編譯流程" "INFO"
        
        # 尋找已開發任務
        $DevelopedTasks = Find-DevelopedTasks
        
        if ($DevelopedTasks.Count -eq 0) {
            Write-CompilationLog "未發現待編譯任務" "INFO"
            return
        }
        
        # 認領任務
        $ClaimedTasks = Request-TasksForCompilation -Tasks $DevelopedTasks
        
        if ($ClaimedTasks.Count -eq 0) {
            Write-CompilationLog "沒有成功認領的任務" "WARNING"
            return
        }
        
        # 選擇編譯模式
        $CompilationPlan = Select-CompilationMode -Tasks $ClaimedTasks
        
        if (-not $CompilationPlan) {
            Write-CompilationLog "無法確定編譯模式" "ERROR"
            return
        }
        
        # 執行編譯
        $Results = Start-CompilationProcess -Mode $CompilationPlan.Mode -HighPriorityTasks $CompilationPlan.HighPriorityTasks -NormalTasks $CompilationPlan.NormalTasks
        
        # 生成報告
        $ReportFile = New-CompilationReport -Results $Results
        
        Write-CompilationLog "`n==========================================" "SUCCESS"
        Write-CompilationLog "編譯工作流程執行完成" "SUCCESS"
        Write-CompilationLog "==========================================" "SUCCESS"
        
        Write-CompilationLog "`n📊 執行摘要:" "INFO"
        Write-CompilationLog "  成功編譯: $($CompilationTracker.TotalCompiled) 個任務" "SUCCESS"
        Write-CompilationLog "  編譯失敗: $($CompilationTracker.TotalFailed) 個任務" $(if ($CompilationTracker.TotalFailed -gt 0) { "WARNING" } else { "INFO" })
        Write-CompilationLog "  詳細報告: $ReportFile" "INFO"
    }
}

# 執行主程序
Main
