# Debug Workflow Launcher
param(
    [switch]$Interactive,
    [switch]$Monitor
)

# Import BMAD Workflow System
try {
    . "$PSScriptRoot\BMAD_WorkflowSystem_En.ps1"
}
catch {
    Write-Host "Error loading BMAD Workflow System: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

Write-Host "=== BMAD Debug Workflow System ===" -ForegroundColor Cyan
Write-Host "Processing 699 detected issues..." -ForegroundColor Yellow
Write-Host ""

# Create workflow coordinator
$coordinator = [MingBMADWorkflowCoordinator]::new()

# Initialize with debug tasks
$debugTasks = @(
    "DEBUG-002: 修復語法錯誤 (388 個)",
    "DEBUG-003: 處理靜態分析警告 (311 個)", 
    "DEBUG-004: 驗證修復結果"
)

$coordinator.InitializeWithTasks($debugTasks)

if ($Monitor) {
    Write-Host "Starting monitor mode..." -ForegroundColor Green
    while ($true) {
        Clear-Host
        Write-Host "=== Debug Workflow Monitor ===" -ForegroundColor Cyan
        Write-Host "Time: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
        Write-Host ""
        
        $coordinator.ShowWorkflowStatus()
        Write-Host "Press Ctrl+C to stop monitoring" -ForegroundColor Yellow
        Start-Sleep -Seconds 30
    }
} else {
    if ($Interactive) {
        Write-Host "Interactive mode enabled" -ForegroundColor Green
        Write-Host "Press Ctrl+C to stop workflow" -ForegroundColor Yellow
        Write-Host ""
    }
    
    # Start workflow
    $coordinator.StartWorkflow()
}
