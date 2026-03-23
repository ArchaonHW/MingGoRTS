# MingGoRTS Branch Management Quick Start
# 分支管理快速啟動工具

param(
    [ValidateSet("status", "feature", "hotfix", "merge", "checks", "help")]
    [string]$Action = "help",
    [string]$BranchName = "",
    [string]$TargetBranch = "main",
    [switch]$DryRun = $false
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS 分支管理系統" -ForegroundColor Green
Write-Host "Quick Start Tool" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# 載入分支管理工作流程
try {
    . (Join-Path $PSScriptRoot "BranchWorkflow.ps1")
} catch {
    Write-Host "Error loading Branch Workflow: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

switch ($Action) {
    "status" {
        Write-Host "Checking branch status..." -ForegroundColor Cyan
        Show-BranchStatus
    }
    
    "feature" {
        Write-Host "Creating feature branch..." -ForegroundColor Cyan
        if (-not $BranchName) {
            $BranchName = Read-Host "Enter feature name"
        }
        New-FeatureBranch -FeatureName $BranchName
    }
    
    "hotfix" {
        Write-Host "Creating hotfix branch..." -ForegroundColor Cyan
        if (-not $BranchName) {
            $BranchName = Read-Host "Enter hotfix name"
        }
        New-HotfixBranch -HotfixName $BranchName
    }
    
    "merge" {
        Write-Host "Merging to target branch..." -ForegroundColor Cyan
        Write-Host "Target branch: $TargetBranch" -ForegroundColor White
        
        $Confirm = Read-Host "Confirm merge to $TargetBranch? (y/N)"
        if ($Confirm -eq 'y' -or $Confirm -eq 'Y') {
            $BranchInfo = Get-BranchInfo
            if ($BranchInfo) {
                Merge-ToTarget -SourceBranch $BranchInfo.CurrentBranch -TargetBranch $TargetBranch
            } else {
                Write-Host "Cannot get branch information" -ForegroundColor Red
            }
        } else {
            Write-Host "Merge cancelled" -ForegroundColor Yellow
        }
    }
    
    "checks" {
        Write-Host "Running pre-merge checks..." -ForegroundColor Cyan
        Invoke-PreMergeChecks
    }
    
    "help" {
        Write-Host "Usage:" -ForegroundColor Yellow
        Write-Host "  .\Quick-Branch.ps1 -Action <action> [parameters]" -ForegroundColor White
        Write-Host ""
        Write-Host "Available actions:" -ForegroundColor Yellow
        Write-Host "  status   - Show current branch status" -ForegroundColor White
        Write-Host "  feature  - Create feature branch" -ForegroundColor White
        Write-Host "  hotfix   - Create hotfix branch" -ForegroundColor White
        Write-Host "  merge    - Merge to target branch" -ForegroundColor White
        Write-Host "  checks   - Run pre-merge checks" -ForegroundColor White
        Write-Host "  help     - Show this help" -ForegroundColor White
        Write-Host ""
        Write-Host "Examples:" -ForegroundColor Yellow
        Write-Host "  .\Quick-Branch.ps1 -Action status" -ForegroundColor White
        Write-Host "  .\Quick-Branch.ps1 -Action feature -BranchName 'ui-improvements'" -ForegroundColor White
        Write-Host "  .\Quick-Branch.ps1 -Action merge -TargetBranch develop" -ForegroundColor White
        Write-Host "  .\Quick-Branch.ps1 -Action checks" -ForegroundColor White
        Write-Host ""
        Write-Host "Workflow:" -ForegroundColor Yellow
        Write-Host "  1. Create feature branch from develop" -ForegroundColor White
        Write-Host "  2. Develop and test on feature branch" -ForegroundColor White
        Write-Host "  3. Run pre-merge checks" -ForegroundColor White
        Write-Host "  4. Merge to develop" -ForegroundColor White
        Write-Host "  5. Create release branch from develop" -ForegroundColor White
        Write-Host "  6. Test and finalize release" -ForegroundColor White
        Write-Host "  7. Merge to main branch" -ForegroundColor White
        Write-Host ""
        Write-Host "Main Branch Protection:" -ForegroundColor Red
        Write-Host "  - All merges to main require pre-merge checks" -ForegroundColor White
        Write-Host "  - Tests must pass" -ForegroundColor White
        Write-Host "  - Code quality checks required" -ForegroundColor White
        Write-Host "  - Security scan required" -ForegroundColor White
        Write-Host "  - Build must succeed" -ForegroundColor White
    }
}

Write-Host ""
Write-Host "MingGoRTS Branch Management - Operation completed" -ForegroundColor Cyan
