# BMAD Workflow Launcher Script
param(
    [string[]]$Tasks = @(
        "Story 3.6: UI and Interface System",
        "Story 3.8: Multiplayer Support", 
        "Story 3.9: Localization and Internationalization",
        "Story 3.10: Performance Optimization"
    ),
    [switch]$Interactive,
    [switch]$StatusOnly,
    [switch]$Monitor
)

# Import BMAD Workflow System Library
try {
    . "$PSScriptRoot\BMAD_WorkflowSystem_Lib.ps1"
}
catch {
    Write-Host "Error loading BMAD Workflow System: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

function Start-BMADWorkflow {
    Write-Host "=== BMAD Intelligent Development Workflow System ===" -ForegroundColor Cyan
    Write-Host "Version: 1.0" -ForegroundColor Gray
    Write-Host "Author: MingGoRTS Development Team" -ForegroundColor Gray
    Write-Host ""
    
    # Create workflow coordinator
    $coordinator = [MingBMADWorkflowCoordinator]::new()
    
    # Initialize tasks
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
    
    # Start workflow
    $coordinator.StartWorkflow()
}

# Enhanced Monitor Panel
function Show-BMADMonitor {
    Clear-Host
    Write-Host "=== BMAD Workflow Monitor Panel ===" -ForegroundColor Cyan
    Write-Host "Press Ctrl+C to exit monitoring" -ForegroundColor Gray
    Write-Host ""
    
    while ($true) {
        # Create a temporary coordinator to check status
        $coordinator = [MingBMADWorkflowCoordinator]::new()
        $coordinator.InitializeWithTasks($Tasks)
        
        # Display status
        Write-Host "Update time: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
        Write-Host "Monitoring tasks: $($Tasks.Count)" -ForegroundColor White
        Write-Host ""
        
        # Simulate status display (in real implementation, this would read from persistent storage)
        $statusColors = @{
            "Pending" = "White"
            "Developed" = "Cyan"
            "CodeReview" = "Yellow"
            "Done" = "Blue"
            "Completed" = "Green"
            "Debug" = "Red"
            "ToOptimize" = "Magenta"
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

# Main execution
try {
    if ($Monitor) {
        Show-BMADMonitor
    } else {
        Start-BMADWorkflow
    }
}
catch {
    Write-Host "Workflow execution error: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Stack trace: $($_.ScriptStackTrace)" -ForegroundColor Yellow
    exit 1
}
