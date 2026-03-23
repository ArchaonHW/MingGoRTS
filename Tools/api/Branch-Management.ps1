# MingGoRTS Branch Management System
# Branch management and merge workflow

param(
    [string]$Action = "help"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS Branch Management System" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Get current branch
function Get-CurrentBranch {
    try {
        $branch = git rev-parse --abbrev-ref HEAD
        return $branch
    } catch {
        return "unknown"
    }
}

# Check working directory status
function Test-WorkingDirectory {
    try {
        $result = & git status --porcelain
        if ($LASTEXITCODE -ne 0) {
            return $false
        }
        # Check if result is empty or null (git returns array)
        if ($null -eq $result) {
            return $true
        }
        # If result is an array, check if it's empty
        if ($result -is [Array]) {
            return $result.Count -eq 0
        }
        # If result is a string, check if it's empty
        return $result.Trim() -eq ""
    } catch {
        return $false
    }
}

# Run pre-merge checks
function Invoke-PreMergeChecks {
    Write-Host "Running pre-merge checks..." -ForegroundColor Cyan
    
    # Check working directory
    if (-not (Test-WorkingDirectory)) {
        Write-Host "ERROR: Working directory not clean, please commit changes first" -ForegroundColor Red
        return $false
    }
    Write-Host "PASS: Working directory clean" -ForegroundColor Green
    
    # Run tests
    Write-Host "Running tests..." -ForegroundColor Cyan
    $testFiles = Get-ChildItem -Path "C:\HW\MingGoRTS" -Filter "*Test*.ps1" -Recurse -ErrorAction SilentlyContinue
    foreach ($testFile in $testFiles) {
        try {
            & $testFile.FullName
            Write-Host "PASS: Test passed - $($testFile.Name)" -ForegroundColor Green
        } catch {
            Write-Host "FAIL: Test failed - $($testFile.Name)" -ForegroundColor Red
            return $false
        }
    }
    
    # Check build
    Write-Host "Running build check..." -ForegroundColor Cyan
    $buildScript = "C:\HW\MingGoRTS\BUILD_OPTIMIZED.cmd"
    if (Test-Path $buildScript) {
        try {
            $process = Start-Process -FilePath $buildScript -Wait -PassThru -NoNewWindow
            if ($process.ExitCode -eq 0) {
                Write-Host "PASS: Build successful" -ForegroundColor Green
            } else {
                Write-Host "FAIL: Build failed" -ForegroundColor Red
                return $false
            }
        } catch {
            Write-Host "FAIL: Build check failed" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "SKIP: No build script found" -ForegroundColor Yellow
    }
    
    Write-Host "All checks passed" -ForegroundColor Green
    return $true
}

# Create feature branch
function New-FeatureBranch {
    $featureName = Read-Host "Enter feature name"
    $branchName = "feature/$featureName"
    
    Write-Host "Creating feature branch: $branchName" -ForegroundColor Cyan
    
    try {
        git checkout develop | Out-Null
        git pull origin develop | Out-Null
        git checkout -b $branchName | Out-Null
        git push -u origin $branchName | Out-Null
        
        Write-Host "Feature branch created successfully: $branchName" -ForegroundColor Green
    } catch {
        Write-Host "Failed to create feature branch" -ForegroundColor Red
    }
}

# Merge to target branch
function Merge-ToTarget {
    $currentBranch = Get-CurrentBranch
    $targetBranch = Read-Host "Enter target branch (default: develop)"
    if ([string]::IsNullOrEmpty($targetBranch)) {
        $targetBranch = "develop"
    }
    
    Write-Host "Merging '$currentBranch' to '$targetBranch'" -ForegroundColor Cyan
    
    # Confirm operation
    $confirm = Read-Host "Confirm merge? (y/N)"
    if ($confirm -ne 'y' -and $confirm -ne 'Y') {
        Write-Host "Merge cancelled" -ForegroundColor Yellow
        return
    }
    
    try {
        git checkout $targetBranch | Out-Null
        git pull origin $targetBranch | Out-Null
        
        # Run pre-merge checks
        if (-not (Invoke-PreMergeChecks)) {
            Write-Host "Pre-merge checks failed, cancelling merge" -ForegroundColor Red
            return
        }
        
        git merge $currentBranch --no-ff | Out-Null
        git push origin $targetBranch | Out-Null
        
        Write-Host "Merge successful" -ForegroundColor Green
    } catch {
        Write-Host "Merge failed" -ForegroundColor Red
        try {
            git merge --abort | Out-Null
            Write-Host "Merge aborted" -ForegroundColor Info
        } catch {
            Write-Host "Cannot abort merge" -ForegroundColor Warning
        }
    }
}

# Show branch status
function Show-BranchStatus {
    $currentBranch = Get-CurrentBranch
    $isClean = Test-WorkingDirectory
    
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Branch Status" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Current branch: $currentBranch" -ForegroundColor White
    Write-Host "Working directory: $(if ($isClean) { 'Clean' } else { 'Has changes' })" -ForegroundColor White
    
    if ($currentBranch -eq "main") {
        Write-Host "Branch type: Main branch" -ForegroundColor Yellow
    } elseif ($currentBranch -eq "develop") {
        Write-Host "Branch type: Develop branch" -ForegroundColor Yellow
    } elseif ($currentBranch -like "feature/*") {
        Write-Host "Branch type: Feature branch" -ForegroundColor Cyan
    } else {
        Write-Host "Branch type: Other" -ForegroundColor White
    }
}

# Show help
function Show-Help {
    Write-Host "Usage:" -ForegroundColor Yellow
    Write-Host "  .\Branch-Management.ps1 -Action <action>" -ForegroundColor White
    Write-Host ""
    Write-Host "Available actions:" -ForegroundColor Yellow
    Write-Host "  status   - Show branch status" -ForegroundColor White
    Write-Host "  feature  - Create feature branch" -ForegroundColor White
    Write-Host "  merge    - Merge to target branch" -ForegroundColor White
    Write-Host "  checks   - Run pre-merge checks" -ForegroundColor White
    Write-Host "  help     - Show help" -ForegroundColor White
    Write-Host ""
    Write-Host "Standard workflow:" -ForegroundColor Yellow
    Write-Host "  1. Create feature branch from develop" -ForegroundColor White
    Write-Host "  2. Develop on feature branch" -ForegroundColor White
    Write-Host "  3. Run pre-merge checks" -ForegroundColor White
    Write-Host "  4. Merge to develop" -ForegroundColor White
    Write-Host "  5. Create release branch from develop" -ForegroundColor White
    Write-Host "  6. Test release branch" -ForegroundColor White
    Write-Host "  7. Merge to main branch" -ForegroundColor White
    Write-Host ""
    Write-Host "Main branch protection rules:" -ForegroundColor Red
    Write-Host "  - All checks must pass before merging to main" -ForegroundColor White
    Write-Host "  - Tests must pass" -ForegroundColor White
    Write-Host "  - Build must succeed" -ForegroundColor White
    Write-Host "  - Working directory must be clean" -ForegroundColor White
}

# Main program
switch ($Action.ToLower()) {
    "status" {
        Show-BranchStatus
    }
    "feature" {
        New-FeatureBranch
    }
    "merge" {
        Merge-ToTarget
    }
    "checks" {
        Invoke-PreMergeChecks
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "Unknown action: $Action" -ForegroundColor Red
        Write-Host "Use 'help' to see available actions" -ForegroundColor Yellow
    }
}

Write-Host ""
Write-Host "Branch management operation completed" -ForegroundColor Cyan
