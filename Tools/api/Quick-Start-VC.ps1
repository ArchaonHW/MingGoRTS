# MingGoRTS Version Control API - Quick Start
# 快速啟動版控API系統

param(
    [ValidateSet("init", "status", "commit", "tag", "branch", "report", "help")]
    [string]$Action = "help"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS Version Control API" -ForegroundColor Green
Write-Host "Quick Start Tool" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# 載入API
try {
    . (Join-Path $PSScriptRoot "MingVersionControlAPI-Core.ps1") | Out-Null
} catch {
    Write-Host "Error loading Version Control API: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

switch ($Action) {
    "init" {
        Write-Host "Initializing Version Control System..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            Write-Host "✓ System initialized successfully" -ForegroundColor Green
            Write-Host "Current branch: $(git rev-parse --abbrev-ref HEAD)" -ForegroundColor White
            Write-Host "Last commit: $(git log -1 --format='%h')" -ForegroundColor White
        } else {
            Write-Host "✗ Initialization failed" -ForegroundColor Red
        }
    }
    
    "status" {
        Write-Host "Getting Version Control Status..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            Get-MingVCStatus -Detailed
        } else {
            Write-Host "✗ Failed to get status" -ForegroundColor Red
        }
    }
    
    "commit" {
        Write-Host "Quick Commit..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            $Message = Read-Host "Enter commit message"
            $Type = Read-Host "Enter commit type (feat/fix/docs/etc.) [feat]"
            if (-not $Type) { $Type = "feat" }
            $Scope = Read-Host "Enter scope (optional)"
            $AutoStage = Read-Host "Auto stage all changes? (y/N)"
            
            $Params = @{
                Message = $Message
                Type = $Type
            }
            if ($Scope) { $Params.Scope = $Scope }
            if ($AutoStage -eq 'y' -or $AutoStage -eq 'Y') { $Params.AutoStage = $true }
            
            $CommitResult = New-MingVCCommit @Params
            if ($CommitResult) {
                Write-Host "✓ Committed successfully" -ForegroundColor Green
            } else {
                Write-Host "✗ Commit failed" -ForegroundColor Red
            }
        } else {
            Write-Host "✗ Failed to initialize" -ForegroundColor Red
        }
    }
    
    "tag" {
        Write-Host "Version Tag Management..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            $CurrentVersion = Get-MingVCCurrentVersion
            Write-Host "Current version: $CurrentVersion" -ForegroundColor White
            
            $Action = Read-Host "Create new tag or auto-increment? (new/auto) [auto]"
            if ($Action -eq 'new') {
                $Version = Read-Host "Enter new version (e.g., 1.1.0)"
                $Message = Read-Host "Enter tag message"
                $Push = Read-Host "Push to remote? (y/N)"
                
                $Params = @{
                    Version = $Version
                    Message = $Message
                }
                if ($Push -eq 'y' -or $Push -eq 'Y') { $Params.Push = $true }
                
                $TagResult = New-MingVCTag @Params
            } else {
                $Push = Read-Host "Push to remote? (y/N)"
                $Params = @{ AutoIncrement = $true }
                if ($Push -eq 'y' -or $Push -eq 'Y') { $Params.Push = $true }
                
                $TagResult = New-MingVCTag @Params
            }
            
            if ($TagResult) {
                Write-Host "✓ Tag operation successful" -ForegroundColor Green
            } else {
                Write-Host "✗ Tag operation failed" -ForegroundColor Red
            }
        } else {
            Write-Host "✗ Failed to initialize" -ForegroundColor Red
        }
    }
    
    "branch" {
        Write-Host "Branch Management..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            $CurrentBranch = git rev-parse --abbrev-ref HEAD
            Write-Host "Current branch: $CurrentBranch" -ForegroundColor White
            
            $BranchName = Read-Host "Enter branch name"
            $Create = Read-Host "Create new branch? (y/N) [switch to existing]"
            
            $Params = @{
                BranchName = $BranchName
            }
            if ($Create -eq 'y' -or $Create -eq 'Y') { $Params.Create = $true }
            
            $BranchResult = Switch-MingVCBranch @Params
            if ($BranchResult) {
                Write-Host "✓ Branch operation successful" -ForegroundColor Green
                Write-Host "Current branch: $(git rev-parse --abbrev-ref HEAD)" -ForegroundColor White
            } else {
                Write-Host "✗ Branch operation failed" -ForegroundColor Red
            }
        } else {
            Write-Host "✗ Failed to initialize" -ForegroundColor Red
        }
    }
    
    "report" {
        Write-Host "Generating Report..." -ForegroundColor Cyan
        $Result = Initialize-MingVersionControl
        if ($Result) {
            $Format = Read-Host "Report format (text/json) [text]"
            $Save = Read-Host "Save to file? (y/N)"
            
            $Params = @{}
            if ($Format -eq 'json') { $Params.Json = $true }
            if ($Save -eq 'y' -or $Save -eq 'Y') {
                $Timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
                $Ext = if ($Format -eq 'json') { 'json' } else { 'txt' }
                $Params.OutputPath = "Reports\VCReport_$Timestamp.$Ext"
            }
            
            $ReportResult = New-MingVCReport @Params
            if ($ReportResult) {
                Write-Host "✓ Report generated successfully" -ForegroundColor Green
                if ($Params.OutputPath) {
                    Write-Host "Saved to: $($Params.OutputPath)" -ForegroundColor White
                }
            } else {
                Write-Host "✗ Report generation failed" -ForegroundColor Red
            }
        } else {
            Write-Host "✗ Failed to initialize" -ForegroundColor Red
        }
    }
    
    "help" {
        Write-Host "Usage:" -ForegroundColor Yellow
        Write-Host "  .\Quick-Start-VC.ps1 -Action <action>" -ForegroundColor White
        Write-Host ""
        Write-Host "Available actions:" -ForegroundColor Yellow
        Write-Host "  init    - Initialize version control system" -ForegroundColor White
        Write-Host "  status  - Show detailed status" -ForegroundColor White
        Write-Host "  commit  - Interactive commit" -ForegroundColor White
        Write-Host "  tag     - Version tag management" -ForegroundColor White
        Write-Host "  branch  - Branch management" -ForegroundColor White
        Write-Host "  report  - Generate reports" -ForegroundColor White
        Write-Host "  help    - Show this help" -ForegroundColor White
        Write-Host ""
        Write-Host "Examples:" -ForegroundColor Yellow
        Write-Host "  .\Quick-Start-VC.ps1 -Action init" -ForegroundColor White
        Write-Host "  .\Quick-Start-VC.ps1 -Action status" -ForegroundColor White
        Write-Host "  .\Quick-Start-VC.ps1 -Action commit" -ForegroundColor White
        Write-Host "  .\Quick-Start-VC.ps1 -Action tag" -ForegroundColor White
        Write-Host "  .\Quick-Start-VC.ps1 -Action branch" -ForegroundColor White
        Write-Host "  .\Quick-Start-VC.ps1 -Action report" -ForegroundColor White
    }
}

Write-Host ""
Write-Host "MingGoRTS Version Control API - Operation completed" -ForegroundColor Cyan
