# Sage Command Development Workflow Launcher
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

Write-Host "=== BMAD Sage Command Development Workflow ===" -ForegroundColor Cyan
Write-Host "Processing 44 Sage Command development tasks..." -ForegroundColor Yellow
Write-Host ""

# Create workflow coordinator
$coordinator = [MingBMADWorkflowCoordinator]::new()

# Initialize with Sage Command development tasks
$sageTasks = @(
    "SAGE-001: 創建 MingSageCharacterSystem.h",
    "SAGE-002: 創建 MingSageCharacterSystem.cpp", 
    "SAGE-003: 實現聖者特性",
    "SAGE-004: 實現魔王特性",
    "SAGE-005: 實現偽聖者特性",
    "SAGE-006: 設計角色選擇 UI",
    "SAGE-007: 創建 MingThreePowerSystem.h",
    "SAGE-008: 創建 MingThreePowerSystem.cpp",
    "SAGE-009: 創建 MingMoralAuthority.h/cpp",
    "SAGE-010: 創建 MingStrategyAuthority.h/cpp",
    "SAGE-011: 創建 MingMilitaryAuthority.h/cpp"
)

$coordinator.InitializeWithTasks($sageTasks)

if ($Monitor) {
    Write-Host "Starting Sage Command monitor mode..." -ForegroundColor Green
    while ($true) {
        Clear-Host
        Write-Host "=== Sage Command Development Monitor ===" -ForegroundColor Cyan
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
