# MingGoRTS Version Control API - Core Functions
# Simple version control system for MingGoRTS

# Global variables
$Global:MingVCConfig = $null
$Global:MingVCState = $null

# Initialize version control system
function Initialize-MingVersionControl {
    param(
        [string]$ProjectPath = "C:\HW\MingGoRTS"
    )
    
    Write-Host "Initializing MingGoRTS Version Control..." -ForegroundColor Green
    
    try {
        # Check if it's a Git repository
        if (-not (Test-Path (Join-Path $ProjectPath ".git"))) {
            Write-Host "Error: Not a Git repository: $ProjectPath" -ForegroundColor Red
            return $false
        }
        
        # Initialize configuration
        $Global:MingVCConfig = @{
            ProjectPath = $ProjectPath
            BranchStrategy = @{
                Main = "main"
                Develop = "develop"
                Feature = "feature/"
                Hotfix = "hotfix/"
                Release = "release/"
            }
            TagStrategy = @{
                Pattern = "v{major}.{minor}.{patch}"
                AutoTag = $true
            }
            CommitStrategy = @{
                AutoFormat = $true
                MaxMessageLength = 100
            }
        }
        
        # Initialize state
        $Global:MingVCState = @{
            CurrentBranch = git rev-parse --abbrev-ref HEAD
            LastCommit = git log -1 --format="%H"
            PendingChanges = git status --porcelain
            LastTag = git describe --tags --abbrev=0 2>$null
        }
        
        Write-Host "Version Control initialized successfully" -ForegroundColor Green
        Write-Host "Current branch: $($Global:MingVCState.CurrentBranch)" -ForegroundColor Cyan
        Write-Host "Last commit: $($Global:MingVCState.LastCommit.Substring(0,8))" -ForegroundColor Cyan
        
        return $true
    } catch {
        Write-Host "Initialization failed: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# Get version control status
function Get-MingVCStatus {
    param(
        [switch]$Detailed
    )
    
    if (-not $Global:MingVCState) {
        Write-Host "Version Control not initialized" -ForegroundColor Red
        return $false
    }
    
    # Update state
    $Global:MingVCState.CurrentBranch = git rev-parse --abbrev-ref HEAD
    $Global:MingVCState.PendingChanges = git status --porcelain
    
    if ($Detailed) {
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "MingGoRTS Version Control Status" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Branch Information:" -ForegroundColor Yellow
        Write-Host "  Current branch: $($Global:MingVCState.CurrentBranch)" -ForegroundColor White
        Write-Host "  Last commit: $($Global:MingVCState.LastCommit.Substring(0,8))" -ForegroundColor White
        Write-Host "  Last tag: $($Global:MingVCState.LastTag)" -ForegroundColor White
        Write-Host ""
        Write-Host "Change Status:" -ForegroundColor Yellow
        Write-Host "  Pending changes: $($Global:MingVCState.PendingChanges.Count)" -ForegroundColor White
        
        if ($Global:MingVCState.PendingChanges) {
            Write-Host ""
            Write-Host "Changes:" -ForegroundColor Yellow
            $Global:MingVCState.PendingChanges | ForEach-Object {
                $Status = $_.Substring(0,2)
                $File = $_.Substring(2)
                $Color = switch ($Status) {
                    "M " { "Yellow" }
                    " M" { "Yellow" }
                    "A " { "Green" }
                    "D " { "Red" }
                    "?? " { "Cyan" }
                    default { "White" }
                }
                Write-Host "  $Status $File" -ForegroundColor $Color
            }
        }
    } else {
        $StatusColor = if ($Global:MingVCState.PendingChanges) { "Yellow" } else { "Green" }
        $StatusText = if ($Global:MingVCState.PendingChanges) { "Has changes" } else { "Clean" }
        
        Write-Host "Branch: $($Global:MingVCState.CurrentBranch) | Status: $StatusText" -ForegroundColor $StatusColor
        if ($Global:MingVCState.PendingChanges.Count -gt 0) {
            Write-Host "Changes: $($Global:MingVCState.PendingChanges.Count) files" -ForegroundColor Cyan
        }
    }
    
    return $Global:MingVCState
}

# Smart commit
function New-MingVCCommit {
    param(
        [string]$Message,
        [string]$Type = "feat",
        [string]$Scope = "",
        [switch]$AutoStage,
        [switch]$DryRun
    )
    
    if (-not $Global:MingVCConfig) {
        Write-Host "Version Control not initialized" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS Smart Commit" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        # Check for changes
        $Changes = git status --porcelain
        if (-not $Changes) {
            Write-Host "No changes to commit" -ForegroundColor Yellow
            return $false
        }
        
        # Auto stage
        if ($AutoStage) {
            Write-Host "Auto-staging all changes..." -ForegroundColor Cyan
            git add .
            Write-Host "Staging completed" -ForegroundColor Green
        }
        
        # Format commit message
        if ($Global:MingVCConfig.CommitStrategy.AutoFormat) {
            $FormattedMessage = Format-MingVCCommitMessage -Message $Message -Type $Type -Scope $Scope
        } else {
            $FormattedMessage = $Message
        }
        
        Write-Host "Commit message: $FormattedMessage" -ForegroundColor Cyan
        
        if (-not $DryRun) {
            # Execute commit
            git commit -m $FormattedMessage
            
            Write-Host "Commit successful!" -ForegroundColor Green
            
            # Update state
            $Global:MingVCState.LastCommit = git log -1 --format="%H"
            $Global:MingVCState.PendingChanges = git status --porcelain
            
            return $true
        } else {
            Write-Host "Preview mode - no actual commit performed" -ForegroundColor Yellow
            return $true
        }
    } catch {
        Write-Host "Commit failed: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# Format commit message
function Format-MingVCCommitMessage {
    param(
        [string]$Message,
        [string]$Type = "feat",
        [string]$Scope = ""
    )
    
    $ValidTypes = @("feat", "fix", "docs", "style", "refactor", "test", "chore", "perf", "ci", "build", "revert")
    
    if ($Type -notin $ValidTypes) {
        Write-Host "Warning: Unsupported commit type '$Type', using default 'feat'" -ForegroundColor Yellow
        $Type = "feat"
    }
    
    $FormattedMessage = "$Type"
    
    if ($Scope) {
        $FormattedMessage += "($Scope)"
    }
    
    $FormattedMessage += ": $Message"
    
    # Length check
    if ($FormattedMessage.Length -gt $Global:MingVCConfig.CommitStrategy.MaxMessageLength) {
        Write-Host "Warning: Commit message too long, will truncate" -ForegroundColor Yellow
        $FormattedMessage = $FormattedMessage.Substring(0, $Global:MingVCConfig.CommitStrategy.MaxMessageLength - 3) + "..."
    }
    
    return $FormattedMessage
}

# Create version tag
function New-MingVCTag {
    param(
        [string]$Version,
        [string]$Message = "",
        [switch]$AutoIncrement,
        [switch]$Push,
        [switch]$DryRun
    )
    
    if (-not $Global:MingVCConfig) {
        Write-Host "Version Control not initialized" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS Version Tag Creation" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        # Auto increment version
        if ($AutoIncrement) {
            $CurrentVersion = Get-MingVCCurrentVersion
            $Version = Increment-MingVCVersion -Version $CurrentVersion
            Write-Host "Auto increment version: $CurrentVersion -> $Version" -ForegroundColor Cyan
        }
        
        # Validate version format
        if (-not (Test-MingVCVersion -Version $Version)) {
            Write-Host "Error: Invalid version format" -ForegroundColor Red
            return $false
        }
        
        # Check if tag exists
        $ExistingTag = git tag -l $Version
        if ($ExistingTag) {
            Write-Host "Error: Tag '$Version' already exists" -ForegroundColor Red
            return $false
        }
        
        # Default message
        if (-not $Message) {
            $Message = "MingGoRTS Version $Version"
        }
        
        Write-Host "Creating tag: $Version" -ForegroundColor Cyan
        Write-Host "Tag message: $Message" -ForegroundColor Cyan
        
        if (-not $DryRun) {
            # Create tag
            git tag -a $Version -m $Message
            
            Write-Host "Tag created successfully!" -ForegroundColor Green
            
            # Push tag
            if ($Push) {
                Write-Host "Pushing tag to remote..." -ForegroundColor Cyan
                git push origin $Version
                Write-Host "Tag push completed" -ForegroundColor Green
            }
            
            # Update state
            $Global:MingVCState.LastTag = $Version
            
            return $true
        } else {
            Write-Host "Preview mode - no actual operation performed" -ForegroundColor Yellow
            return $true
        }
    } catch {
        Write-Host "Tag creation failed: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# Get current version
function Get-MingVCCurrentVersion {
    try {
        $LatestTag = git describe --tags --abbrev=0 2>$null
        if ($LatestTag) {
            return $LatestTag.TrimStart('v')
        } else {
            return "0.0.0"
        }
    } catch {
        return "0.0.0"
    }
}

# Increment version
function Update-MingVCVersion {
    param(
        [string]$Version = "0.0.0",
        [ValidateSet("major", "minor", "patch")]
        [string]$Increment = "patch"
    )
    
    if ($Version -notmatch '^(\d+)\.(\d+)\.(\d+)(?:-.+)?$') {
        Write-Host "Error: Invalid version format" -ForegroundColor Red
        return $Version
    }
    
    $Major = [int]$Matches[1]
    $Minor = [int]$Matches[2]
    $Patch = [int]$Matches[3]
    
    switch ($Increment) {
        "major" {
            $Major++
            $Minor = 0
            $Patch = 0
        }
        "minor" {
            $Minor++
            $Patch = 0
        }
        "patch" {
            $Patch++
        }
    }
    
    return "$Major.$Minor.$Patch"
}

# Validate version format
function Test-MingVCVersion {
    param([string]$Version)
    
    return $Version -match '^\d+\.\d+\.\d+(?:-.+)?$'
}

# Branch management
function Switch-MingVCBranch {
    param(
        [string]$BranchName,
        [switch]$Create,
        [switch]$Force
    )
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS Branch Management" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        if ($Create) {
            Write-Host "Creating and switching to branch: $BranchName" -ForegroundColor Cyan
            git checkout -b $BranchName
        } else {
            Write-Host "Switching to branch: $BranchName" -ForegroundColor Cyan
            git checkout $BranchName
        }
        
        # Update state
        $Global:MingVCState.CurrentBranch = $BranchName
        
        Write-Host "Branch switch successful!" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "Branch operation failed: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# Generate report
function New-MingVCReport {
    param(
        [string]$OutputPath = "",
        [switch]$Json
    )
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS Version Control Report" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        $Report = @{
            GeneratedAt = Get-Date
            ProjectPath = $Global:MingVCConfig.ProjectPath
            Status = Get-MingVCStatus -Detailed
            History = git log --oneline -20
            Branches = git branch -a
            Tags = git tag --sort=-version:refname
            Statistics = @{
                TotalCommits = git rev-list --count HEAD
                TotalAuthors = (git shortlog -s | Measure-Object).Count
                TotalBranches = (git branch -a | Measure-Object).Count
                TotalTags = (git tag | Measure-Object).Count
            }
        }
        
        if ($OutputPath) {
            if ($Json) {
                $Report | ConvertTo-Json -Depth 10 | Set-Content $OutputPath
            } else {
                ConvertTo-MingVCTextReport -Report $Report | Set-Content $OutputPath
            }
            Write-Host "Report saved: $OutputPath" -ForegroundColor Green
        } else {
            if ($Json) {
                return $Report | ConvertTo-Json -Depth 10
            } else {
                ConvertTo-MingVCTextReport -Report $Report
            }
        }
        
        return $true
    } catch {
        Write-Host "Report generation failed: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# Convert to text report
function ConvertTo-MingVCTextReport {
    param([hashtable]$Report)
    
    $Output = @"
========================================
MingGoRTS Version Control Report
========================================
Generated: $($Report.GeneratedAt)
Project: $($Report.ProjectPath)

Status Information:
Current Branch: $($Report.Status.CurrentBranch)
Last Commit: $($Report.Status.LastCommit)
Last Tag: $($Report.Status.LastTag)
Pending Changes: $($Report.Status.PendingChanges.Count)

Statistics:
Total Commits: $($Report.Statistics.TotalCommits)
Contributors: $($Report.Statistics.TotalAuthors)
Branches: $($Report.Statistics.TotalBranches)
Tags: $($Report.Statistics.TotalTags)

Recent History:
$($Report.History -join "`n")

========================================
"@
    
    return $Output
}

# Functions are available for direct use
Write-Host "MingGoRTS Version Control API loaded" -ForegroundColor Green
