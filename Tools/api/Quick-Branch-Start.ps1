# MingGoRTS Branch Management Quick Start
# 快速啟動分支管理系統

param(
    [ValidateSet("status", "feature", "hotfix", "merge", "checks", "help", "init")]
    [string]$Action = "help"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS Branch Management Quick Start" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Check if Git is available
try {
    git --version | Out-Null
} catch {
    Write-Host "ERROR: Git is not available. Please install Git first." -ForegroundColor Red
    exit 1
}

# Check if we're in a Git repository
if (-not (Test-Path ".git")) {
    Write-Host "ERROR: Not in a Git repository. Please navigate to the project root." -ForegroundColor Red
    exit 1
}

# Initialize branch management system
function Initialize-BranchSystem {
    Write-Host "Initializing branch management system..." -ForegroundColor Cyan
    
    # Check if main branch exists
    $mainExists = git branch --list "main"
    if (-not $mainExists) {
        Write-Host "Creating main branch..." -ForegroundColor Yellow
        git checkout -b main | Out-Null
        git push -u origin main | Out-Null
    }
    
    # Check if develop branch exists
    $developExists = git branch --list "develop"
    if (-not $developExists) {
        Write-Host "Creating develop branch..." -ForegroundColor Yellow
        git checkout -b develop | Out-Null
        git push -u origin develop | Out-Null
    }
    
    # Switch to develop branch
    git checkout develop | Out-Null
    
    Write-Host "Branch management system initialized successfully!" -ForegroundColor Green
}

# Quick status check
function Get-QuickStatus {
    $currentBranch = git rev-parse --abbrev-ref HEAD
    $isClean = (git status --porcelain) -eq ""
    $unpushed = git log --count origin/$currentBranch..HEAD 2>$null
    
    Write-Host "Quick Status:" -ForegroundColor Cyan
    Write-Host "  Branch: $currentBranch" -ForegroundColor White
    Write-Host "  Status: $(if ($isClean) { 'Clean' } else { 'Has changes' })" -ForegroundColor White
    Write-Host "  Unpushed: $unpushed commits" -ForegroundColor White
    
    # Show recent commits
    Write-Host "`nRecent commits:" -ForegroundColor Cyan
    git log --oneline -5
}

# Create feature branch
function New-QuickFeature {
    $featureName = Read-Host "Enter feature name (e.g., ui-improvements)"
    if ([string]::IsNullOrEmpty($featureName)) {
        Write-Host "Feature name cannot be empty" -ForegroundColor Red
        return
    }
    
    $branchName = "feature/$featureName"
    
    Write-Host "Creating feature branch: $branchName" -ForegroundColor Cyan
    
    try {
        git checkout develop | Out-Null
        git pull origin develop | Out-Null
        git checkout -b $branchName | Out-Null
        git push -u origin $branchName | Out-Null
        
        Write-Host "Feature branch created successfully!" -ForegroundColor Green
        Write-Host "You are now on branch: $branchName" -ForegroundColor White
        Write-Host "Start developing your feature..." -ForegroundColor Yellow
    } catch {
        Write-Host "Failed to create feature branch" -ForegroundColor Red
    }
}

# Quick merge
function Invoke-QuickMerge {
    $currentBranch = git rev-parse --abbrev-ref HEAD
    
    if ($currentBranch -eq "main") {
        Write-Host "WARNING: You are on main branch. Cannot merge to main directly." -ForegroundColor Red
        Write-Host "Please merge to develop branch first." -ForegroundColor Yellow
        return
    }
    
    $targetBranch = Read-Host "Enter target branch (default: develop)"
    if ([string]::IsNullOrEmpty($targetBranch)) {
        $targetBranch = "develop"
    }
    
    Write-Host "Merging '$currentBranch' to '$targetBranch'" -ForegroundColor Cyan
    
    # Quick checks
    $isClean = (git status --porcelain) -eq ""
    if (-not $isClean) {
        Write-Host "ERROR: Working directory not clean. Please commit changes first." -ForegroundColor Red
        return
    }
    
    $confirm = Read-Host "Confirm merge? (y/N)"
    if ($confirm -ne 'y' -and $confirm -ne 'Y') {
        Write-Host "Merge cancelled" -ForegroundColor Yellow
        return
    }
    
    try {
        git checkout $targetBranch | Out-Null
        git pull origin $targetBranch | Out-Null
        
        # Simple merge
        git merge $currentBranch --no-ff | Out-Null
        git push origin $targetBranch | Out-Null
        
        Write-Host "Merge successful!" -ForegroundColor Green
        
        # Ask about deleting feature branch
        if ($currentBranch -like "feature/*") {
            $deleteConfirm = Read-Host "Delete feature branch '$currentBranch'? (y/N)"
            if ($deleteConfirm -eq 'y' -or $deleteConfirm -eq 'Y') {
                git checkout develop | Out-Null
                git branch -D $currentBranch | Out-Null
                git push origin --delete $currentBranch | Out-Null
                Write-Host "Feature branch deleted" -ForegroundColor Green
            }
        }
    } catch {
        Write-Host "Merge failed. Please resolve conflicts manually." -ForegroundColor Red
        try {
            git merge --abort | Out-Null
            Write-Host "Merge aborted" -ForegroundColor Info
        } catch {
            # Cannot abort, already resolved
        }
    }
}

# Quick checks
function Test-QuickChecks {
    Write-Host "Running quick checks..." -ForegroundColor Cyan
    
    # Check 1: Working directory
    $isClean = (git status --porcelain) -eq ""
    if ($isClean) {
        Write-Host "✓ Working directory clean" -ForegroundColor Green
    } else {
        Write-Host "✗ Working directory has changes" -ForegroundColor Red
        return
    }
    
    # Check 2: Branch up to date
    try {
        git fetch | Out-Null
        $behind = git rev-list --count HEAD..origin/$(git rev-parse --abbrev-ref HEAD) 2>$null
        if ($behind -eq 0) {
            Write-Host "✓ Branch is up to date" -ForegroundColor Green
        } else {
            Write-Host "! Branch is $behind commits behind" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "! Cannot check branch status" -ForegroundColor Yellow
    }
    
    # Check 3: Build (if available)
    $buildScript = "BUILD_OPTIMIZED.cmd"
    if (Test-Path $buildScript) {
        Write-Host "Running build check..." -ForegroundColor Cyan
        try {
            $process = Start-Process -FilePath $buildScript -Wait -PassThru -NoNewWindow
            if ($process.ExitCode -eq 0) {
                Write-Host "✓ Build successful" -ForegroundColor Green
            } else {
                Write-Host "✗ Build failed" -ForegroundColor Red
                return
            }
        } catch {
            Write-Host "✗ Build check failed" -ForegroundColor Red
            return
        }
    } else {
        Write-Host "! No build script found" -ForegroundColor Yellow
    }
    
    Write-Host "All checks passed!" -ForegroundColor Green
}

# Show help
function Show-Help {
    Write-Host "MingGoRTS Branch Management Quick Start" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Usage:" -ForegroundColor White
    Write-Host "  .\Quick-Branch-Start.ps1 -Action <action>" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Available actions:" -ForegroundColor White
    Write-Host "  init     - Initialize branch management system" -ForegroundColor Gray
    Write-Host "  status   - Show quick status" -ForegroundColor Gray
    Write-Host "  feature  - Create feature branch" -ForegroundColor Gray
    Write-Host "  merge    - Quick merge to target" -ForegroundColor Gray
    Write-Host "  checks   - Run quick checks" -ForegroundColor Gray
    Write-Host "  help     - Show this help" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Quick workflow:" -ForegroundColor Yellow
    Write-Host "  1. .\Quick-Branch-Start.ps1 -Action init" -ForegroundColor Gray
    Write-Host "  2. .\Quick-Branch-Start.ps1 -Action feature" -ForegroundColor Gray
    Write-Host "  3. (develop your feature)" -ForegroundColor Gray
    Write-Host "  4. .\Quick-Branch-Start.ps1 -Action checks" -ForegroundColor Gray
    Write-Host "  5. .\Quick-Branch-Start.ps1 -Action merge" -ForegroundColor Gray
    Write-Host ""
    Write-Host "For advanced features, use Branch-Management.ps1" -ForegroundColor Cyan
}

# Main execution
switch ($Action.ToLower()) {
    "init" {
        Initialize-BranchSystem
    }
    "status" {
        Get-QuickStatus
    }
    "feature" {
        New-QuickFeature
    }
    "merge" {
        Invoke-QuickMerge
    }
    "checks" {
        Test-QuickChecks
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "Unknown action: $Action" -ForegroundColor Red
        Show-Help
    }
}

Write-Host ""
Write-Host "Quick Start operation completed" -ForegroundColor Cyan
