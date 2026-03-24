# 設置日誌函數
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) {
        "INFO" { "White" }
        "SUCCESS" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        default { "White" }
    }
    
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

# 需求系統管理器
class MingRequirementSystem {
    [System.Collections.Generic.List[PSObject]]$Requirements
    
    MingRequirementSystem() {
        $this.Requirements = [System.Collections.Generic.List[PSObject]]::new()
    }
    
    [void]AddRequirement([PSObject]$Requirement) {
        $this.Requirements.Add($Requirement)
        Write-Log "新增需求: $($Requirement.Name)" "INFO"
    }
    
    [System.Collections.Generic.List[PSObject]]GetPendingRequirements() {
        return $this.Requirements | Where-Object { $_.Status -eq "待開發" } | Sort-Object -Property Priority -Descending
    }
    
    [void]UpdateRequirementStatus([int]$ID, [string]$NewStatus) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $oldStatus = $requirement.Status
            $requirement.Status = $NewStatus
            Write-Log "需求 [$ID] 狀態更新: $oldStatus -> $NewStatus" "INFO"
        }
    }
}

# Coding API 服務
class MingCodingAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodingAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartDevelopment() {
        Write-Log "Coding API 啟動開發流程..." "INFO"
        
        $pendingTasks = $this.RequirementSystem.GetPendingRequirements()
        
        foreach ($task in $pendingTasks) {
            Write-Log "開始開發任務: $($task.ID) - $($task.Name)" "INFO"
            
            # 模擬開發過程
            $developmentResult = $this.DevelopTask($task)
            
            if ($developmentResult.Success -and $developmentResult.NoBugs) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "已開發")
                Write-Log "任務開發完成: $($task.ID)" "SUCCESS"
            } else {
                Write-Log "任務開發失敗: $($task.ID) - $($developmentResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]DevelopTask([PSObject]$Task) {
        # 模擬開發過程
        Write-Log "  正在開發: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 10 -Maximum 30)
        
        # 模擬開發結果 (90% 成功率)
        $success = (Get-Random -Maximum 10) -ne 0
        $noBugs = $success -and (Get-Random -Maximum 10) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            NoBugs = $noBugs
            Error = if (-not $success) { "開發過程中遇到技術問題" } else { $null }
        }
    }
    
    [void]HandleDebugTasks() {
        Write-Log "Coding API 處理 Debug 狀態任務..." "INFO"
        
        $debugTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "debug" }
        
        foreach ($task in $debugTasks) {
            Write-Log "修復任務: $($task.ID) - $($task.Name)" "INFO"
            
            # 重置為待開發狀態
            $this.RequirementSystem.UpdateRequirementStatus($task.ID, "待開發")
            
            # 重新開發
            $developmentResult = $this.DevelopTask($task)
            
            if ($developmentResult.Success -and $developmentResult.NoBugs) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "已開發")
                Write-Log "任務修復完成: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [void]HandleOptimizationTasks() {
        Write-Log "Coding API 處理待優化任務..." "INFO"
        
        $optTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "待優化" }
        
        foreach ($task in $optTasks) {
            Write-Log "優化任務: $($task.ID) - $($task.Name)" "INFO"
            
            # 優化開發
            $optimizationResult = $this.OptimizeTask($task)
            
            if ($optimizationResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "已開發")
                Write-Log "任務優化完成: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [PSObject]OptimizeTask([PSObject]$Task) {
        Write-Log "  正在優化: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 5 -Maximum 15)
        
        # 模擬優化結果 (95% 成功率)
        $success = (Get-Random -Maximum 20) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "優化過程中遇到問題" } else { $null }
        }
    }
}

# 編譯 API 服務
class MingCompileAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCompileAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartCompilation() {
        Write-Log "編譯 API 啟動編譯流程..." "INFO"
        
        $developedTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "已開發" }
        
        foreach ($task in $developedTasks) {
            Write-Log "編譯任務: $($task.ID) - $($task.Name)" "INFO"
            
            $compileResult = $this.CompileTask($task)
            
            if ($compileResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "code review")
                Write-Log "編譯成功: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "debug")
                Write-Log "編譯失敗: $($task.ID) - $($compileResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]CompileTask([PSObject]$Task) {
        Write-Log "  正在編譯: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 5 -Maximum 20)
        
        # 模擬編譯結果 (85% 成功率)
        $success = (Get-Random -Maximum 20) -le 17
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "編譯錯誤：語法問題或依賴缺失" } else { $null }
        }
    }
}

# 程式碼檢測 API 服務
class MingCodeScanAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodeScanAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartCodeScanning() {
        Write-Log "程式碼檢測 API 啟動掃描流程..." "INFO"
        
        $reviewTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "code review" }
        
        foreach ($task in $reviewTasks) {
            Write-Log "掃描任務: $($task.ID) - $($task.Name)" "INFO"
            
            $scanResult = $this.ScanCode($task)
            
            if ($scanResult.HasIssues) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "待優化")
                Write-Log "發現程式碼問題: $($task.ID) - $($scanResult.Issues)" "WARNING"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "done")
                Write-Log "程式碼掃描通過: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [PSObject]ScanCode([PSObject]$Task) {
        Write-Log "  正在掃描程式碼: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 3 -Maximum 10)
        
        # 模擬掃描結果 (30% 發現問題)
        $hasIssues = (Get-Random -Maximum 10) -le 3
        $issues = if ($hasIssues) { 
            @("程式碼規範問題", "潛在安全漏洞", "效能問題") | Get-Random 
        } else { $null }
        
        return [PSCustomObject]@{
            HasIssues = $hasIssues
            Issues = $issues
        }
    }
}

# 程式碼規範 API 服務
class MingCodeStandardAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodeStandardAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartStandardCheck() {
        Write-Log "程式碼規範 API 啟動檢查流程..." "INFO"
        
        $reviewTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "code review" }
        
        foreach ($task in $reviewTasks) {
            Write-Log "檢查任務規範: $($task.ID) - $($task.Name)" "INFO"
            
            $standardResult = $this.CheckStandards($task)
            
            if ($standardResult.Compliant) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "done")
                Write-Log "規範檢查通過: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "待優化")
                Write-Log "規範檢查失敗: $($task.ID) - $($standardResult.Violations)" "WARNING"
            }
        }
    }
    
    [PSObject]CheckStandards([PSObject]$Task) {
        Write-Log "  正在檢查規範: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 2 -Maximum 8)
        
        # 模擬規範檢查結果 (25% 發現違規)
        $compliant = (Get-Random -Maximum 10) -gt 2
        $violations = if (-not $compliant) { 
            @("命名規範違規", "註解不足", "函數過長") | Get-Random 
        } else { $null }
        
        return [PSCustomObject]@{
            Compliant = $compliant
            Violations = $violations
        }
    }
}

# 版控 API 服務
class MingVersionControlAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingVersionControlAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartSmartCommit() {
        Write-Log "版控 API 啟動智能提交流程..." "INFO"
        
        $doneTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "done" }
        
        foreach ($task in $doneTasks) {
            Write-Log "提交任務: $($task.ID) - $($task.Name)" "INFO"
            
            $commitResult = $this.SmartCommit($task)
            
            if ($commitResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "完成")
                Write-Log "任務提交成功: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "debug")
                # 重置為未提交狀態
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "已開發")
                Write-Log "任務提交失敗: $($task.ID) - $($commitResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]SmartCommit([PSObject]$Task) {
        Write-Log "  正在智能提交: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 3 -Maximum 10)
        
        # 模擬提交結果 (90% 成功率)
        $success = (Get-Random -Maximum 10) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "提交衝突或網絡問題" } else { $null }
        }
    }
}

# BMAD 工作流協調器
class MingBMADWorkflowCoordinator {
    [MingRequirementSystem]$RequirementSystem
    [MingCodingAPI]$CodingAPI
    [MingCompileAPI]$CompileAPI
    [MingCodeScanAPI]$CodeScanAPI
    [MingCodeStandardAPI]$CodeStandardAPI
    [MingVersionControlAPI]$VersionControlAPI
    
    MingBMADWorkflowCoordinator() {
        $this.RequirementSystem = [MingRequirementSystem]::new()
        $this.CodingAPI = [MingCodingAPI]::new($this.RequirementSystem)
        $this.CompileAPI = [MingCompileAPI]::new($this.RequirementSystem)
        $this.CodeScanAPI = [MingCodeScanAPI]::new($this.RequirementSystem)
        $this.CodeStandardAPI = [MingCodeStandardAPI]::new($this.RequirementSystem)
        $this.VersionControlAPI = [MingVersionControlAPI]::new($this.RequirementSystem)
    }
    
    [void]StartWorkflow() {
        Write-Log "=== BMAD 智能開發工作流啟動 ===" "INFO"
        
        # 持續運行工作流
        while ($true) {
            Write-Log "--- 工作流循環開始 ---" "INFO"
            
            # 1. Coding API 處理待開發任務
            $this.CodingAPI.StartDevelopment()
            
            # 2. 編譯 API 處理已開發任務
            $this.CompileAPI.StartCompilation()
            
            # 3. Coding API 處理 debug 狀態任務
            $this.CodingAPI.HandleDebugTasks()
            
            # 4. 程式碼檢測 API 處理 code review 狀態
            $this.CodeScanAPI.StartCodeScanning()
            
            # 5. 程式碼規範 API 檢查 code review 狀態
            $this.CodeStandardAPI.StartStandardCheck()
            
            # 6. Coding API 處理待優化任務
            $this.CodingAPI.HandleOptimizationTasks()
            
            # 7. 版控 API 處理 done 狀態任務
            $this.VersionControlAPI.StartSmartCommit()
            
            # 顯示當前狀態
            $this.ShowWorkflowStatus()
            
            Write-Log "--- 工作流循環完成 ---" "INFO"
            Start-Sleep -Seconds 30  # 每30秒檢查一次
            
            # 檢查是否所有任務都完成
            if ($this.AllTasksCompleted()) {
                Write-Log "所有任務已完成，工作流結束" "SUCCESS"
                break
            }
        }
    }
    
    [void]ShowWorkflowStatus() {
        $status = $this.RequirementSystem.Requirements | Group-Object -Property Status | Sort-Object -Property Name
        
        Write-Host "當前任務狀態:" -ForegroundColor Yellow
        foreach ($group in $status) {
            Write-Host "  $($group.Name): $($group.Count) 個任務" -ForegroundColor White
        }
        Write-Host ""
    }
    
    [bool]AllTasksCompleted() {
        $pendingTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -ne "完成" }
        return $pendingTasks.Count -eq 0
    }
    
    [void]InitializeWithTasks([string[]]$TaskNames) {
        Write-Log "初始化工作流任務..." "INFO"
        
        foreach ($taskName in $TaskNames) {
            $task = [PSCustomObject]@{
                TaskName = $taskName
                Priority = Get-Random -Minimum 1 -Maximum 10
                Complexity = "Medium"
                Dependencies = @()
                AssignedTeam = "開發團隊"
            }
            
            $this.RequirementSystem.AddRequirement($task)
        }
        
        Write-Log "工作流初始化完成，共 $($TaskNames.Count) 個任務" "SUCCESS"
    }
}

# BMAD 工作流啟動腳本
param(
    [string[]]$Tasks = @(
        "Story 3.6: UI和界面系統",
        "Story 3.8: 多人遊戲支持", 
        "Story 3.9: 本地化和國際化",
        "Story 3.10: 性能優化"
    ),
    [switch]$Interactive,
    [switch]$StatusOnly
)

function Start-BMADWorkflow {
    Write-Host "=== BMAD 智能開發工作流系統 ===" -ForegroundColor Cyan
    Write-Host ""
    
    # 創建工作流協調器
    $coordinator = [MingBMADWorkflowCoordinator]::new()
    
    # 初始化任務
    $coordinator.InitializeWithTasks($Tasks)
    
    if ($StatusOnly) {
        $coordinator.ShowWorkflowStatus()
        return
    }
    
    if ($Interactive) {
        Write-Host "互動模式已啟用" -ForegroundColor Green
        Write-Host "按 Ctrl+C 停止工作流" -ForegroundColor Yellow
        Write-Host ""
    }
    
    # 啟動工作流
    $coordinator.StartWorkflow()
}

# 執行工作流
try {
    Start-BMADWorkflow
}
catch {
    Write-Log "工作流執行錯誤: $($_.Exception.Message)" "ERROR"
    exit 1
}

# 工作流監控面板
function Show-BMADMonitor() {
    Clear-Host
    Write-Host "=== BMAD 工作流監控面板 ===" -ForegroundColor Cyan
    Write-Host ""
    
    while ($true) {
        # 載入需求系統狀態
        $requirementSystem = [MingRequirementSystem]::new()
        $status = $requirementSystem.Requirements | Group-Object -Property Status
        
        # 顯示狀態
        Write-Host "更新時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
        Write-Host ""
        
        foreach ($group in $status) {
            $color = switch ($group.Name) {
                "完成" { "Green" }
                "done" { "Blue" }
                "code review" { "Yellow" }
                "已開發" { "Cyan" }
                "待開發" { "White" }
                "debug" { "Red" }
                "待優化" { "Magenta" }
                default { "Gray" }
            }
            
            Write-Host "[$($group.Name.PadRight(12))] $($group.Count) 個任務" -ForegroundColor $color
        }
        
        Write-Host ""
        Write-Host "按 Ctrl+C 退出監控" -ForegroundColor Gray
        Start-Sleep -Seconds 10
        Clear-Host
        Write-Host "=== BMAD 工作流監控面板 ===" -ForegroundColor Cyan
        Write-Host ""
    }
}
