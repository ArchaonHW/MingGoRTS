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
    "SAGE-001: Create MingSageCharacterSystem.h",
    "SAGE-002: Create MingSageCharacterSystem.cpp", 
    "SAGE-003: Implement Sage traits",
    "SAGE-004: Implement Demon traits",
    "SAGE-005: Implement False Sage traits",
    "SAGE-006: Design character selection UI",
    "SAGE-007: Create MingThreePowerSystem.h",
    "SAGE-008: Create MingThreePowerSystem.cpp",
    "SAGE-009: Create MingMoralAuthority",
    "SAGE-010: Create MingStrategyAuthority",
    "SAGE-011: Create MingMilitaryAuthority"
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
