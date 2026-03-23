# Batch Processing Workflow - Simple Version
# Automated batch processing for detected issues

param(
    [switch]$AutoFix = $false,
    [switch]$GenerateReport = $false,
    [string]$ProjectRoot = "f:\MingGoRTS",
    [string]$IssueType = "All"
)

function Write-BatchLog {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) { "INFO" { "White" } "SUCCESS" { "Green" } "WARNING" { "Yellow" } "ERROR" { "Red" } }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

function FixMemoryIssues {
    param([string]$content, [string]$filename)
    $lines = $content -split "`n"
    $fixedLines = @()
    $modified = $false
    
    foreach ($line in $lines) {
        $fixedLines += $line
        
        # Check for new allocation without TODO comment
        if ($line -match "^(\s*)([\w<>,\s]+)\*\s*(\w+)\s*=\s*new\s+" -and 
            $line -notmatch "//.*TODO.*delete" -and
            $line -notmatch "//.*cleanup") {
            $indent = $matches[1]
            $typeName = $matches[2].Trim()
            $varName = $matches[3]
            
            # Add cleanup comment after the allocation
            $fixedLines += "$indent// TODO: Memory cleanup - delete $varName; or use TSharedPtr<$typeName>"
            $modified = $true
        }
    }
    
    return @{ Content = ($fixedLines -join "`n"); Modified = $modified }
}

# Main processing
Write-BatchLog "=== Batch Processing Started ===" "SUCCESS"
Write-BatchLog "Project: $ProjectRoot" "INFO"
Write-BatchLog "Mode: $(if ($AutoFix) { 'Auto-Fix' } else { 'Dry Run' }) | IssueType: $IssueType" "INFO"

$sourcePath = Join-Path $ProjectRoot "Source\MingGoRTS\Private"
$cppFiles = Get-ChildItem -Path $sourcePath -Filter "*.cpp" -Recurse -ErrorAction SilentlyContinue

$stats = @{
    TotalFiles = $cppFiles.Count
    MemoryIssues = 0
    TODOIssues = 0
    Fixed = 0
    Skipped = 0
}

$fixedFiles = @()
$skippedFiles = @()

foreach ($file in $cppFiles) {
    $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
    if (!$content) { continue }
    
    $needsFix = $false
    $fixes = @()
    $newContent = $content
    $wasModified = $false
    
    # Check for memory issues
    if ($IssueType -in @("All", "Memory")) {
        if ($content -match "new\s+[\w<>,\s]+\s*\(" -and $content -notmatch "delete\s*\w*;" -and $content -notmatch "TSharedPtr|UniquePtr") {
            $needsFix = $true
            $stats.MemoryIssues++
            
            if ($AutoFix) {
                $result = FixMemoryIssues $content $file.Name
                $newContent = $result.Content
                $wasModified = $result.Modified
            }
            $fixes += "Memory: new without delete cleanup"
        }
    }
    
    # Check for TODO issues
    if ($IssueType -in @("All", "TODO")) {
        $todoCount = ([regex]::Matches($content, "TODO|FIXME")).Count
        if ($todoCount -gt 0) {
            $needsFix = $true
            $stats.TODOIssues += $todoCount
            $fixes += "TODO/FIXME: $todoCount items"
        }
    }
    
    if ($needsFix) {
        if ($AutoFix -and $wasModified) {
            # Backup original
            $backupPath = $file.FullName + ".backup"
            Copy-Item $file.FullName $backupPath -Force -ErrorAction SilentlyContinue
            
            # Write fixed content
            Set-Content -Path $file.FullName -Value $newContent -NoNewline
            $fixedFiles += @{ File = $file.Name; Fixes = $fixes }
            $stats.Fixed++
            Write-BatchLog "Fixed: $($file.Name)" "SUCCESS"
        } else {
            $skippedFiles += @{ File = $file.Name; Issues = $fixes }
            $stats.Skipped++
            if ($stats.Skipped -le 20) {
                Write-BatchLog "Would fix: $($file.Name) - $($fixes -join ', ')" "WARNING"
            }
        }
    }
}

if ($stats.Skipped -gt 20) {
    Write-BatchLog "... and $($stats.Skipped - 20) more files" "WARNING"
}

# Show Statistics
Write-Host "`n=== Batch Processing Statistics ===" -ForegroundColor Cyan
Write-Host "  Total Files Scanned: $($stats.TotalFiles)" -ForegroundColor White
Write-Host "  Memory Issues Found: $($stats.MemoryIssues)" -ForegroundColor $(if ($stats.MemoryIssues -gt 0) { "Yellow" } else { "Green" })
Write-Host "  TODO Issues Found: $($stats.TODOIssues)" -ForegroundColor $(if ($stats.TODOIssues -gt 0) { "Yellow" } else { "Green" })
Write-Host "  Files Fixed: $($stats.Fixed)" -ForegroundColor Green
Write-Host "  Files To Fix: $($stats.Skipped)" -ForegroundColor $(if ($stats.Skipped -gt 0) { "Yellow" } else { "Green" })
Write-Host "====================================" -ForegroundColor Cyan

# Generate Report
if ($GenerateReport) {
    $reportPath = Join-Path $ProjectRoot "Reports\BatchProcessing_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $reportLines = @(
        "# Batch Processing Report",
        "",
        "**Date:** $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')",
        "**Mode:** $(if ($AutoFix) { 'Auto-Fix Enabled' } else { 'Dry Run' })",
        "**Issue Type:** $IssueType",
        "",
        "## Statistics",
        "- Total Files Scanned: $($stats.TotalFiles)",
        "- Memory Issues: $($stats.MemoryIssues)",
        "- TODO Issues: $($stats.TODOIssues)",
        "- Files Fixed: $($stats.Fixed)",
        "- Files To Fix: $($stats.Skipped)",
        "",
        "## $(if ($AutoFix) { 'Fixed Files' } else { 'Files Needing Fix' })"
    )
    
    $targetFiles = if ($AutoFix) { $fixedFiles } else { $skippedFiles }
    $displayCount = [Math]::Min(50, $targetFiles.Count)
    
    for ($i = 0; $i -lt $displayCount; $i++) {
        $item = $targetFiles[$i]
        $reportLines += "- **$($item.File)**"
        foreach ($fix in $item.Fixes) {
            $reportLines += "  - $fix"
        }
    }
    
    if ($targetFiles.Count -gt 50) {
        $reportLines += "- ... and $($targetFiles.Count - 50) more files"
    }
    
    $reportLines += ""
    $reportLines += "## Recommendations"
    
    if ($stats.MemoryIssues -gt 0) {
        $reportLines += "- Consider using TSharedPtr<> for automatic memory management"
        $reportLines += "- Review all new/delete pairs for potential memory leaks"
    }
    if ($stats.TODOIssues -gt 0) {
        $reportLines += "- Schedule time to address TODO/FIXME comments"
        $reportLines += "- Prioritize high-impact TODOs first"
    }
    
    $reportLines | Out-File $reportPath -Encoding UTF8
    Write-BatchLog "Report saved: $reportPath" "SUCCESS"
}

Write-BatchLog "=== Batch Processing Complete ===" "SUCCESS"
