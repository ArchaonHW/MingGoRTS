# BMAD 智能開發工作流啟動腳本

param(
    [string[]]$Tasks = @(
        "Story 3.6: UI和界面系統",
        "Story 3.8: 多人遊戲支持", 
        "Story 3.9: 本地化和國際化",
        "Story 3.10: 性能優化"
    ),
    [switch]$Interactive,
    [switch]$StatusOnly,
    [switch]$Monitor,
    [switch]$Help
)

# 顯示幫助信息
if ($Help) {
    Write-Host "=== BMAD 智能開發工作流啟動腳本 ===" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "用法:" -ForegroundColor Yellow
    Write-Host "  .\Start-BMADWorkflow.ps1 [參數]"
    Write-Host ""
    Write-Host "參數:" -ForegroundColor Yellow
    Write-Host "  -Tasks        : 指定要處理的任務列表"
    Write-Host "  -Interactive   : 啟用互動模式"
    Write-Host "  -StatusOnly   : 僅顯示當前狀態"
    Write-Host "  -Monitor      : 啟動監控面板"
    Write-Host "  -Help         : 顯示此幫助信息"
    Write-Host ""
    Write-Host "範例:" -ForegroundColor Yellow
    Write-Host "  .\Start-BMADWorkflow.ps1"
    Write-Host "  .\Start-BMADWorkflow.ps1 -Interactive"
    Write-Host "  .\Start-BMADWorkflow.ps1 -StatusOnly"
    Write-Host "  .\Start-BMADWorkflow.ps1 -Monitor"
    Write-Host "  .\Start-BMADWorkflow.ps1 -Tasks @('UI系統', '性能優化')"
    exit 0
}

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

# 載入工作流系統
try {
    . "$PSScriptRoot\BMAD_WorkflowSystem.ps1"
}
catch {
    Write-Log "無法載入 BMAD 工作流系統: $($_.Exception.Message)" "ERROR"
    exit 1
}

# 工作流監控面板
function Show-BMADMonitor {
    Clear-Host
    Write-Host "=== BMAD 工作流監控面板 ===" -ForegroundColor Cyan
    Write-Host ""
    
    try {
        while ($true) {
            # 載入需求系統狀態
            $requirementSystem = [MingRequirementSystem]::new()
            
            if ($requirementSystem.Requirements.Count -eq 0) {
                Write-Host "暫無任務數據" -ForegroundColor Yellow
                Write-Host "請先運行工作流: .\Start-BMADWorkflow.ps1" -ForegroundColor Gray
                break
            }
            
            $status = $requirementSystem.Requirements | Group-Object -Property Status
            
            # 顯示狀態
            Write-Host "更新時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
            Write-Host "總任務數: $($requirementSystem.Requirements.Count)" -ForegroundColor White
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
            
            # 計算完成率
            $completedTasks = $requirementSystem.Requirements | Where-Object { $_.Status -eq "完成" }
            $completionRate = if ($requirementSystem.Requirements.Count -gt 0) { 
                [math]::Round(($completedTasks.Count / $requirementSystem.Requirements.Count) * 100, 1) 
            } else { 0 }
            
            Write-Host ""
            Write-Host "完成率: $completionRate%" -ForegroundColor Green
            Write-Host ""
            Write-Host "按 Ctrl+C 退出監控" -ForegroundColor Gray
            Start-Sleep -Seconds 10
            Clear-Host
            Write-Host "=== BMAD 工作流監控面板 ===" -ForegroundColor Cyan
            Write-Host ""
        }
        
        Write-Host "Workflow Status:" -ForegroundColor Yellow
        foreach ($task in $Tasks) {
            $status = Get-Random -InputObject @("Pending", "Developed", "CodeReview", "Done", "Completed")
            $color = $statusColors[$status]
            Write-Host "  [$($task.PadRight(45))] $($status.PadRight(12))" -ForegroundColor $color
        }
        
        Write-Host ""
        Write-Host "Next update in 10 seconds..." -ForegroundColor Gray
        Start-Sleep -Seconds 10
        Clear-Host
        Write-Host "=== BMAD Workflow Monitor Panel ===" -ForegroundColor Cyan
        Write-Host "Press Ctrl+C to exit monitoring" -ForegroundColor Gray
        Write-Host ""
    }
}

# 啟動 BMAD 工作流
function Start-BMADWorkflow {
    Write-Host "=== BMAD Intelligent Development Workflow System ===" -ForegroundColor Cyan
    Write-Host "Version: 1.0" -ForegroundColor Gray
    Write-Host "Author: MingGoRTS Development Team" -ForegroundColor Gray
    Write-Host ""
    
    try {
        # 創建工作流協調器
        $coordinator = [MingBMADWorkflowCoordinator]::new()
        
        # 初始化任務
        $coordinator.InitializeWithTasks($Tasks)
        
        if ($StatusOnly) {
            $coordinator.ShowWorkflowStatus()
            return
        }
        
        if ($Monitor) {
            Show-BMADMonitor
            return
        }
        
        if ($Interactive) {
            Write-Host "Interactive mode enabled" -ForegroundColor Green
            Write-Host "Press Ctrl+C to stop workflow" -ForegroundColor Yellow
            Write-Host ""
        }
        
        # 啟動工作流
        $coordinator.StartWorkflow()
    }
    catch {
        Write-Host "Workflow execution error: $($_.Exception.Message)" -ForegroundColor Red
        Write-Host "Stack trace: $($_.ScriptStackTrace)" -ForegroundColor Yellow
        exit 1
    }
}

# 主程序
function Main {
    Write-Log "BMAD 工作流啟動腳本初始化..." "INFO"
    
    if ($Monitor) {
        Show-BMADMonitor
    }
    else {
        Start-BMADWorkflow
    }
    
    Write-Log "腳本執行完成" "INFO"
}

# 執行主程序
try {
    Main
}
catch {
    Write-Log "腳本執行失敗: $($_.Exception.Message)" "ERROR"
    exit 1
}
