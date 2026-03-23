# MingGoRTS Version Control API - Simple Startup
param(
    [string]$Action = "start"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS Version Control API" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

try {
    # Load the version control API
    . (Join-Path $PSScriptRoot "MingVersionControlAPI.ps1")
    
    switch ($Action.ToLower()) {
        "start" {
            Write-Host "Starting Version Control System..." -ForegroundColor Cyan
            $Result = Initialize-MingVersionControl
            if ($Result) {
                Write-Host "Version Control System started successfully!" -ForegroundColor Green
                Get-MingVCStatus -Detailed
            } else {
                Write-Host "Failed to start Version Control System" -ForegroundColor Red
            }
        }
        "status" {
            Write-Host "Getting Version Control Status..." -ForegroundColor Cyan
            Get-MingVCStatus -Detailed
        }
        "test" {
            Write-Host "Testing Version Control System..." -ForegroundColor Cyan
            $Result = Initialize-MingVersionControl
            if ($Result) {
                Write-Host "System initialized successfully" -ForegroundColor Green
                $Version = Get-MingVCCurrentVersion
                Write-Host "Current version: $Version" -ForegroundColor Cyan
                $Status = Get-MingVCStatus
                Write-Host "Status check completed" -ForegroundColor Green
            } else {
                Write-Host "System test failed" -ForegroundColor Red
            }
        }
        default {
            Write-Host "Unknown action: $Action" -ForegroundColor Red
            Write-Host "Available actions: start, status, test" -ForegroundColor Yellow
        }
    }
} catch {
    Write-Host "Error: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host "Version Control API operation completed" -ForegroundColor Cyan
