# Version Control Workflow - Simple Version
# Automated git commit for uncommitted changes

param(
    [string]$ProjectRoot = "f:\MingGoRTS",
    [int]$MaxCommits = 10
)

function Write-VCSLog {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) { "INFO" { "White" } "SUCCESS" { "Green" } "WARNING" { "Yellow" } "ERROR" { "Red" } }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

Write-VCSLog "=== Version Control Workflow Started ===" "SUCCESS"
Write-VCSLog "Project: $ProjectRoot" "INFO"

# Check git status
try {
    $gitStatus = & git -C $ProjectRoot status --short 2>$null
    if (!$gitStatus) {
        Write-VCSLog "No uncommitted changes found" "SUCCESS"
        exit 0
    }
    
    $changeCount = ($gitStatus -split "`n").Count
    Write-VCSLog "Found $changeCount uncommitted changes" "WARNING"
    
    # Show changes
    Write-Host "`n=== Changes to be committed ===" -ForegroundColor Cyan
    $changes = & git -C $ProjectRoot status -s 2>$null | Select-Object -First 20
    foreach ($change in $changes) {
        Write-Host "  $change" -ForegroundColor Gray
    }
    if ($changeCount -gt 20) {
        Write-Host "  ... and $($changeCount - 20) more" -ForegroundColor Gray
    }
    
    # Stage all changes
    Write-VCSLog "Staging changes..." "INFO"
    & git -C $ProjectRoot add . 2>$null
    
    # Create commit
    $commitMessage = "Auto-commit: $(Get-Date -Format 'yyyy-MM-dd HH:mm') - $changeCount files changed"
    Write-VCSLog "Committing with message: $commitMessage" "INFO"
    & git -C $ProjectRoot commit -m "$commitMessage" 2>$null
    
    Write-VCSLog "Successfully committed $changeCount files" "SUCCESS"
    
    # Generate report
    $reportPath = Join-Path $ProjectRoot "Reports\VCS_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    $report = "# Version Control Report`n`n"
    $report += "**Date:** $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
    $report += "**Action:** Auto-commit`n"
    $report += "**Files Committed:** $changeCount`n"
    $report += "**Commit Message:** $commitMessage`n`n"
    $report += "## Files Changed`n````n"
    $report += ($changes -join "`n")
    $report += "`n```"
    
    $report | Out-File $reportPath -Encoding UTF8
    Write-VCSLog "Report saved: $reportPath" "INFO"
    
} catch {
    Write-VCSLog "Error: $($_.Exception.Message)" "ERROR"
    exit 1
}

Write-VCSLog "=== Version Control Workflow Complete ===" "SUCCESS"
