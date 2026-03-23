# MingGoRTS Version Control API - Simple Launcher
param(
    [string]$Action = "start"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS Version Control API" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

try {
    # Load the core version control API
    . (Join-Path $PSScriptRoot "MingVersionControlAPI-Core.ps1")
    
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
                $CurrentStatus = Get-MingVCStatus
                Write-Host "Status: $CurrentStatus" -ForegroundColor Cyan
            } else {
                Write-Host "System test failed" -ForegroundColor Red
            }
        }
        "commit" {
            Write-Host "Testing commit functionality..." -ForegroundColor Cyan
            $Result = Initialize-MingVersionControl
            if ($Result) {
                $TestMessage = Format-MingVCCommitMessage -Message "test version control API" -Type "feat" -Scope "api"
                Write-Host "Test commit message: $TestMessage" -ForegroundColor Cyan
                Write-Host "Commit functionality test completed" -ForegroundColor Green
            }
        }
        "tag" {
            Write-Host "Testing tag functionality..." -ForegroundColor Cyan
            $Result = Initialize-MingVersionControl
            if ($Result) {
                $CurrentVersion = Get-MingVCCurrentVersion
                Write-Host "Current version: $CurrentVersion" -ForegroundColor Cyan
                $NextVersion = Increment-MingVCVersion -Version $CurrentVersion
                Write-Host "Next version: $NextVersion" -ForegroundColor Cyan
                Write-Host "Tag functionality test completed" -ForegroundColor Green
            }
        }
        "report" {
            Write-Host "Generating Version Control Report..." -ForegroundColor Cyan
            $Result = Initialize-MingVersionControl
            if ($Result) {
                $ReportPath = "Reports\VCReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"
                $ReportDir = Split-Path $ReportPath -Parent
                if (-not (Test-Path $ReportDir)) {
                    New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
                }
                New-MingVCReport -OutputPath $ReportPath
                Write-Host "Report generated: $ReportPath" -ForegroundColor Green
            }
        }
        default {
            Write-Host "Unknown action: $Action" -ForegroundColor Red
            Write-Host "Available actions: start, status, test, commit, tag, report" -ForegroundColor Yellow
        }
    }
} catch {
    Write-Host "Error: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host "Version Control API operation completed" -ForegroundColor Cyan
