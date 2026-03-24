#!/usr/bin/env pwsh

<#
.SYNOPSIS
    MingGoRTS Version Control Workflow System
    版控工作流程系統

.DESCRIPTION
    1. 檢測需求單系統任務狀態為"Done"的任務
    2. 啟動智能提交流程
    3. 成功：標記為"已完成"，失敗：標記為"Debug"並回到"未提交"狀態
    4. 必要時開啟自動化操控功能

.PARAMETER AutoMode
    自動化操作模式

.PARAMETER ScanInterval
    掃描間隔（秒）

.PARAMETER MaxConcurrentCommits
    最大並發提交數

.EXAMPLE
    .\VersionControl-Workflow.ps1 -AutoMode
    自動化版控模式

.EXAMPLE
    .\VersionControl-Workflow.ps1 -ManualMode
    手動版控模式
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoMode,
    
    [Parameter()]
    [int]$ScanInterval = 45,
    
    [Parameter()]
    [int]$MaxConcurrentCommits = 2,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TasksPath = "$PWD\Tasks",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\VersionControlWorkflow.log"
)

$ErrorActionPreference = "Stop"

# 顏色定義
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
    VersionControl = "Blue"
}

# 版控追蹤器
$VersionControlTracker = @{
    CurrentTasks = @()
    CompletedTasks = @()
    FailedTasks = @()
    TotalCommitted = 0
    TotalFailed = 0
    StartTime = Get-Date
}

function Write-VersionControlLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG", "VERSIONCONTROL")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
        "VERSIONCONTROL" { $Colors.VersionControl }
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

function Show-VersionControlBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║          MingGoRTS Version Control Workflow System       ║
    ║              版控工作流程系統                            ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.VersionControl
}

# ============================================
# 階段 1: 需求單掃描與任務檢測
# ============================================

function Find-DoneTasks {
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "階段 1: 掃描已完成任務" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $DoneTasks = @()
    
    # 掃描所有需求單目錄
    $RequirementDirs = Get-ChildItem -Path $TasksPath -Directory -ErrorAction SilentlyContinue
    
    foreach ($Dir in $RequirementDirs) {
        $RequirementFiles = Get-ChildItem -Path $Dir.FullName -Filter "Requirement_*.json" -ErrorAction SilentlyContinue
        
        foreach ($File in $RequirementFiles) {
            try {
                $RequirementData = Get-Content -Path $File.FullName -Raw | ConvertFrom-Json
                
                # 檢查狀態為"Done"的任務
                $DoneTasksInReq = $RequirementData.Tasks | Where-Object { 
                    $_.Status -eq "Done" -or 
                    $_.Status -eq "CodeReview" -or
                    $_.Status -eq "Completed"
                }
                
                if ($DoneTasksInReq.Count -gt 0) {
                    foreach ($Task in $DoneTasksInReq) {
                        # 檢查是否已經處理過
                        if ($Task.Committed -ne $true) {
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
                                Changes = $Task.Changes
                                Files = $Task.Files
                            }
                            $DoneTasks += $TaskInfo
                        }
                    }
                    
                    Write-VersionControlLog "  發現已完成任務: $($RequirementData.RequirementID) - $($DoneTasksInReq.Count) 個任務" "DEBUG"
                }
            }
            catch {
                Write-VersionControlLog "  錯誤讀取需求單: $($File.Name) - $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-VersionControlLog "掃描完成: 發現 $($DoneTasks.Count) 個待提交任務" $(if ($DoneTasks.Count -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $DoneTasks
}

# ============================================
# 階段 2: 任務認領與狀態更新
# ============================================

function Request-TasksForCommit {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "階段 2: 認領提交任務" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $ClaimedTasks = @()
    
    foreach ($Task in $Tasks) {
        try {
            # 讀取需求單文件
            $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
            
            # 找到對應任務
            $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
            
            if ($TaskToUpdate) {
                # 更新任務狀態
                $TaskToUpdate | Add-Member -NotePropertyName "Status" -NotePropertyValue "Committing" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedBy" -NotePropertyValue "VersionControlWorkflow" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedAt" -NotePropertyValue (Get-Date -Format "yyyy-MM-dd HH:mm:ss") -Force
                
                # 保存更新後的需求單
                $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
                
                $ClaimedTask = $Task.Clone()
                $ClaimedTask.UpdatedStatus = "Committing"
                $ClaimedTasks += $ClaimedTask
                
                Write-VersionControlLog "  [✓] 認領任務: $($Task.TaskID) - $($Task.TaskTitle)" "SUCCESS"
            }
            else {
                Write-VersionControlLog "  [✗] 未找到任務: $($Task.TaskID)" "ERROR"
            }
        }
        catch {
            Write-VersionControlLog "  [✗] 認領失敗: $($Task.TaskID) - $($_.Exception.Message)" "ERROR"
        }
    }
    
    Write-VersionControlLog "認領完成: 成功認領 $($ClaimedTasks.Count) 個任務" "SUCCESS"
    
    return $ClaimedTasks
}

# ============================================
# 階段 3: 版控模式選擇
# ============================================

function Select-VersionControlMode {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "階段 3: 選擇版控模式" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $TaskCount = $Tasks.Count
    $VersionControlMode = ""
    
    if ($TaskCount -eq 0) {
        Write-VersionControlLog "沒有任務需要提交" "INFO"
        return $null
    }
    elseif ($TaskCount -eq 1) {
        $VersionControlMode = "Single"
        Write-VersionControlLog "選擇模式: 單一任務提交" "INFO"
    }
    elseif ($TaskCount -le $MaxConcurrentCommits) {
        $VersionControlMode = "Batch"
        Write-VersionControlLog "選擇模式: 批量提交 ($TaskCount 個任務)" "INFO"
    }
    else {
        $VersionControlMode = "Sequential"
        Write-VersionControlLog "選擇模式: 順序提交 (大量任務: $TaskCount)" "INFO"
    }
    
    # 分析任務類型和優先級
    $HighPriorityTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Critical" -or $_.TaskPriority -eq "High" }
    $NormalTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Medium" -or $_.TaskPriority -eq "Low" }
    
    Write-VersionControlLog "任務分析:" "DEBUG"
    Write-VersionControlLog "  高優先級: $($HighPriorityTasks.Count) 個" "DEBUG"
    Write-VersionControlLog "  普通優先級: $($NormalTasks.Count) 個" "DEBUG"
    
    return @{
        Mode = $VersionControlMode
        HighPriorityTasks = $HighPriorityTasks
        NormalTasks = $NormalTasks
        TotalTasks = $Tasks
    }
}

# ============================================
# 階段 4: 版控執行
# ============================================

function Start-VersionControlProcess {
    [CmdletBinding()]
    param(
        [string]$Mode,
        [array]$HighPriorityTasks,
        [array]$NormalTasks
    )
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "階段 4: 執行版控過程" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $AllTasks = $HighPriorityTasks + $NormalTasks
    $VersionControlResults = @()
    
    foreach ($Task in $AllTasks) {
        Write-VersionControlLog "`n開始版控任務: $($Task.TaskID) - $($Task.TaskTitle)" "VERSIONCONTROL"
        
        # 標記為提交中
        Update-TaskStatus -Task $Task -Status "Committing"
        
        # 執行版控
        $Result = Invoke-TaskVersionControl -Task $Task -Mode $Mode
        
        # 更新最終狀態
        if ($Result.Success) {
            Update-TaskStatus -Task $Task -Status "已完成" -Result $Result
            $VersionControlTracker.TotalCommitted++
            Write-VersionControlLog "  [✓] 版控成功: $($Task.TaskID)" "SUCCESS"
        }
        else {
            Update-TaskStatus -Task $Task -Status "Debug" -Result $Result
            # 回到未提交狀態
            Update-TaskStatus -Task $Task -Status "未提交"
            $VersionControlTracker.TotalFailed++
            Write-VersionControlLog "  [✗] 版控失敗: $($Task.TaskID) - $($Result.ErrorMessage)" "ERROR"
        }
        
        $VersionControlResults += $Result
        $VersionControlTracker.CurrentTasks = $VersionControlTracker.CurrentTasks | Where-Object { $_.TaskID -ne $Task.TaskID }
    }
    
    Write-VersionControlLog "版控完成: 成功 $($VersionControlTracker.TotalCommitted) 個，失敗 $($VersionControlTracker.TotalFailed) 個" "SUCCESS"
    
    return $VersionControlResults
}

function Invoke-TaskVersionControl {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Mode
    )
    
    $Result = @{
        TaskID = $Task.TaskID
        Success = $false
        CommitTime = 0
        Output = ""
        ErrorMessage = ""
        Warnings = @()
        CommitHash = ""
        Branch = ""
    }
    
    $Stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    
    try {
        Write-VersionControlLog "  執行版控命令..." "DEBUG"
        
        # 檢查Git倉庫狀態
        $GitRepoPath = Join-Path $ProjectRoot ".git"
        if (Test-Path $GitRepoPath) {
            Write-VersionControlLog "    檢測到Git倉庫，開始智能提交..." "DEBUG"
            
            # 獲取當前分支
            $CurrentBranch = & git -C $ProjectRoot rev-parse --abbrev-ref HEAD 2>$null
            if ($LASTEXITCODE -eq 0) {
                $Result.Branch = $CurrentBranch
                Write-VersionControlLog "    當前分支: $CurrentBranch" "DEBUG"
            }
            
            # 檢查是否有未提交的更改
            $StatusOutput = & git -C $ProjectRoot status --porcelain 2>$null
            if ($LASTEXITCODE -eq 0 -and $StatusOutput) {
                Write-VersionControlLog "    檢測到未提交的更改，開始提交..." "DEBUG"
                
                # 添加所有更改
                & git -C $ProjectRoot add . 2>$null
                if ($LASTEXITCODE -ne 0) {
                    throw "Git add 失敗"
                }
                
                # 創建智能提交訊息
                $CommitMessage = "feat: $($Task.TaskTitle) [$(Task.TaskID)]"
                if ($Task.Changes) {
                    $CommitMessage += "`n`n$($Task.Changes -join "`n")"
                }
                
                # 提交更改
                $CommitOutput = & git -C $ProjectRoot commit -m $CommitMessage 2>&1
                if ($LASTEXITCODE -eq 0) {
                    # 獲取提交哈希
                    $CommitHash = & git -C $ProjectRoot rev-parse --short HEAD 2>$null
                    if ($LASTEXITCODE -eq 0) {
                        $Result.CommitHash = $CommitHash
                        $Result.Success = $true
                        $Result.Output = "成功提交: $CommitHash"
                        Write-VersionControlLog "    提交成功: $CommitHash" "DEBUG"
                    }
                }
                else {
                    $Result.ErrorMessage = "Git commit 失敗: $CommitOutput"
                    Write-VersionControlLog "    提交失敗: $CommitOutput" "DEBUG"
                }
            }
            else {
                Write-VersionControlLog "    沒有未提交的更改" "DEBUG"
                $Result.Success = $true
                $Result.Output = "沒有需要提交的更改"
            }
        }
        else {
            # 初始化Git倉庫
            Write-VersionControlLog "    初始化Git倉庫..." "DEBUG"
            & git -C $ProjectRoot init 2>$null
            if ($LASTEXITCODE -eq 0) {
                # 配置Git用戶信息
                & git -C $ProjectRoot config user.name "MingGoRTS VersionControl" 2>$null
                & git -C $ProjectRoot config user.email "versioncontrol@minggorts.com" 2>$null
                
                # 添加所有文件
                & git -C $ProjectRoot add . 2>$null
                if ($LASTEXITCODE -eq 0) {
                    # 初始提交
                    $CommitMessage = "feat: 初始化MingGoRTS項目 [$(Task.TaskID)]"
                    $CommitOutput = & git -C $ProjectRoot commit -m $CommitMessage 2>&1
                    if ($LASTEXITCODE -eq 0) {
                        $CommitHash = & git -C $ProjectRoot rev-parse --short HEAD 2>$null
                        if ($LASTEXITCODE -eq 0) {
                            $Result.CommitHash = $CommitHash
                            $Result.Success = $true
                            $Result.Output = "Git倉庫初始化成功: $CommitHash"
                            Write-VersionControlLog "    Git倉庫初始化成功: $CommitHash" "DEBUG"
                        }
                    }
                    else {
                        $Result.ErrorMessage = "初始提交失敗: $CommitOutput"
                    }
                }
                else {
                    $Result.ErrorMessage = "Git add 失敗"
                }
            }
            else {
                $Result.ErrorMessage = "Git倉庫初始化失敗"
            }
        }
    }
    catch {
        $Result.ErrorMessage = "版控過程發生異常: $($_.Exception.Message)"
        Write-VersionControlLog "    版控異常: $($_.Exception.Message)" "ERROR"
    }
    finally {
        $Stopwatch.Stop()
        $Result.CommitTime = $Stopwatch.Elapsed.TotalSeconds
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
            $TaskToUpdate | Add-Member -NotePropertyName "Status" -NotePropertyValue $Status -Force
            $TaskToUpdate | Add-Member -NotePropertyName "UpdatedAt" -NotePropertyValue (Get-Date -Format "yyyy-MM-dd HH:mm:ss") -Force
            
            if ($Result) {
                $TaskToUpdate | Add-Member -NotePropertyName "VersionControlResult" -NotePropertyValue @{
                    Success = $Result.Success
                    CommitTime = $Result.CommitTime
                    CommitHash = $Result.CommitHash
                    Branch = $Result.Branch
                    ErrorMessage = $Result.ErrorMessage
                    Warnings = $Result.Warnings
                } -Force
                
                # 如果成功，標記為已提交
                if ($Result.Success) {
                    $TaskToUpdate | Add-Member -NotePropertyName "Committed" -NotePropertyValue $true -Force
                }
            }
            
            # 保存更新後的需求單
            $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
            
            Write-VersionControlLog "    狀態更新: $($Task.TaskID) -> $Status" "DEBUG"
        }
    }
    catch {
        Write-VersionControlLog "    狀態更新失敗: $($_.Exception.Message)" "ERROR"
    }
}

# ============================================
# 階段 5: 版控報告生成
# ============================================

function New-VersionControlReport {
    [CmdletBinding()]
    param([array]$Results)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "階段 5: 生成版控報告" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $ReportFile = Join-Path $TasksPath "VersionControl\VersionControlReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    # 確保目錄存在
    $ReportDir = Split-Path -Parent $ReportFile
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $TotalTime = (Get-Date) - $VersionControlTracker.StartTime
    
    $Report = @"
# 版控工作流程報告

**執行時間**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**專案路徑**: $ProjectRoot  
**總執行時間**: $($TotalTime.ToString("hh\:mm\:ss"))

## 版控摘要

| 指標 | 數值 |
|------|------|
| 總任務數 | $($Results.Count) |
| 成功提交 | $($VersionControlTracker.TotalCommitted) |
| 提交失敗 | $($VersionControlTracker.TotalFailed) |
| 成功率 | $([math]::Round(($VersionControlTracker.TotalCommitted / [math]::Max($Results.Count, 1)) * 100, 2))% |

## 詳細結果

### ✅ 成功提交的任務

"@
    
    foreach ($Result in $Results | Where-Object { $_.Success }) {
        $Report += "- **$($Result.TaskID)**: 提交成功 (耗時: $([math]::Round($Result.CommitTime, 2))秒, 哈希: $($Result.CommitHash))`n"
    }
    
    $Report += @"

### ❌ 提交失敗的任務

"@
    
    foreach ($Result in $Results | Where-Object { -not $_.Success }) {
        $Report += "- **$($Result.TaskID)**: 提交失敗 - $($Result.ErrorMessage)`n"
    }
    
    $Report += @"

## 版控統計

- 平均提交時間: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Average).Average, 2))秒
- 最長提交時間: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Maximum).Maximum, 2))秒
- 最短提交時間: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Minimum).Minimum, 2))秒

## Git 狀態

"@
    
    # 獲取Git狀態
    try {
        $GitStatus = & git -C $ProjectRoot status --porcelain 2>$null
        if ($LASTEXITCODE -eq 0) {
            if ($GitStatus) {
                $Report += "當前有未提交的更改：`n````"
                $Report += $GitStatus
                $Report += "`````n"
            }
            else {
                $Report += "工作目錄乾淨，沒有未提交的更改。`n"
            }
        }
        
        $CurrentBranch = & git -C $ProjectRoot rev-parse --abbrev-ref HEAD 2>$null
        if ($LASTEXITCODE -eq 0) {
            $Report += "當前分支: $CurrentBranch`n"
        }
        
        $LastCommit = & git -C $ProjectRoot log --oneline -1 2>$null
        if ($LASTEXITCODE -eq 0) {
            $Report += "最新提交: $LastCommit`n"
        }
    }
    catch {
        $Report += "無法獲取Git狀態信息。`n"
    }
    
    $Report += @"

## 下一步操作

1. 檢查提交失敗的任務並修復問題
2. 對成功提交的任務進行代碼審查
3. 推送到遠程倉庫（如果需要）
4. 重新運行版控工作流程驗證修復

---
*由 MingGoRTS Version Control Workflow System 自動生成*
"@
    
    $Report | Set-Content -Path $ReportFile
    Write-VersionControlLog "版控報告已生成: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# 自動化模式監控
# ============================================

function Start-AutoMonitoring {
    Write-VersionControlLog "啟動自動化監控模式，掃描間隔: $ScanInterval 秒" "INFO"
    
    while ($true) {
        try {
            Write-VersionControlLog "`n--- 開始掃描 $(Get-Date -Format 'HH:mm:ss') ---" "DEBUG"
            
            # 尋找已完成任務
            $DoneTasks = Find-DoneTasks
            
            if ($DoneTasks.Count -gt 0) {
                Write-VersionControlLog "發現 $($DoneTasks.Count) 個待提交任務，開始處理..." "INFO"
                
                # 認領任務
                $ClaimedTasks = Request-TasksForCommit -Tasks $DoneTasks
                
                if ($ClaimedTasks.Count -gt 0) {
                    # 選擇版控模式
                    $VersionControlPlan = Select-VersionControlMode -Tasks $ClaimedTasks
                    
                    if ($VersionControlPlan) {
                        # 執行版控
                        $Results = Start-VersionControlProcess -Mode $VersionControlPlan.Mode -HighPriorityTasks $VersionControlPlan.HighPriorityTasks -NormalTasks $VersionControlPlan.NormalTasks
                        
                        # 生成報告
                        $ReportFile = New-VersionControlReport -Results $Results
                        Write-VersionControlLog "版控週期完成，報告: $ReportFile" "SUCCESS"
                    }
                }
            }
            else {
                Write-VersionControlLog "未發現待提交任務，繼續監控..." "INFO"
            }
            
            Write-VersionControlLog "等待 $ScanInterval 秒後進行下次掃描..." "DEBUG"
            Start-Sleep -Seconds $ScanInterval
        }
        catch {
            Write-VersionControlLog "自動監控發生錯誤: $($_.Exception.Message)" "ERROR"
            Start-Sleep -Seconds 60 # 錯誤時等待較長時間
        }
    }
}

# ============================================
# 主執行流程
# ============================================

function Main {
    Show-VersionControlBanner
    
    if ($AutoMode) {
        Write-VersionControlLog "啟動自動化版控模式" "INFO"
        Start-AutoMonitoring
    }
    else {
        Write-VersionControlLog "執行單次版控流程" "INFO"
        
        # 尋找已完成任務
        $DoneTasks = Find-DoneTasks
        
        if ($DoneTasks.Count -eq 0) {
            Write-VersionControlLog "未發現待提交任務" "INFO"
            return
        }
        
        # 認領任務
        $ClaimedTasks = Request-TasksForCommit -Tasks $DoneTasks
        
        if ($ClaimedTasks.Count -eq 0) {
            Write-VersionControlLog "沒有成功認領的任務" "WARNING"
            return
        }
        
        # 選擇版控模式
        $VersionControlPlan = Select-VersionControlMode -Tasks $ClaimedTasks
        
        if (-not $VersionControlPlan) {
            Write-VersionControlLog "無法確定版控模式" "ERROR"
            return
        }
        
        # 執行版控
        $Results = Start-VersionControlProcess -Mode $VersionControlPlan.Mode -HighPriorityTasks $VersionControlPlan.HighPriorityTasks -NormalTasks $VersionControlPlan.NormalTasks
        
        # 生成報告
        $ReportFile = New-VersionControlReport -Results $Results
        
        Write-VersionControlLog "`n==========================================" "SUCCESS"
        Write-VersionControlLog "版控工作流程執行完成" "SUCCESS"
        Write-VersionControlLog "==========================================" "SUCCESS"
        
        Write-VersionControlLog "`n📊 執行摘要:" "INFO"
        Write-VersionControlLog "  成功提交: $($VersionControlTracker.TotalCommitted) 個任務" "SUCCESS"
        Write-VersionControlLog "  提交失敗: $($VersionControlTracker.TotalFailed) 個任務" $(if ($VersionControlTracker.TotalFailed -gt 0) { "WARNING" } else { "INFO" })
        Write-VersionControlLog "  詳細報告: $ReportFile" "INFO"
    }
}

# 執行主程序
Main
