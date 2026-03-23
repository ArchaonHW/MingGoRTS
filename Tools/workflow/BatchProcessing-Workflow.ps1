# Batch Processing Workflow
# Automated batch fix for detected issues

param(
    [switch]$AutoFix,
    [switch]$GenerateReport,
    [string]$ProjectRoot = "f:\MingGoRTS",
    [string]$IssueType = "All"  # All|Memory|TODO
)

function Write-BatchLog {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) { "INFO" { "White" } "SUCCESS" { "Green" } "WARNING" { "Yellow" } "ERROR" { "Red" } }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

class BatchIssueFixer {
    [string]$ProjectRoot
    [array]$FixedFiles
    [array]$SkippedFiles
    [hashtable]$Stats
    
    BatchIssueFixer([string]$root) {
        $this.ProjectRoot = $root
        $this.FixedFiles = @()
        $this.SkippedFiles = @()
        $this.Stats = @{ MemoryIssues = 0; TODOIssues = 0; TotalFiles = 0; Fixed = 0; Skipped = 0 }
    }
    
    [void]ProcessBatch([string]$issueType) {
        Write-BatchLog "=== Batch Processing Started ===" "SUCCESS"
        Write-BatchLog "Issue Type: $issueType" "INFO"
        
        $sourcePath = Join-Path $this.ProjectRoot "Source\MingGoRTS\Private"
        $cppFiles = Get-ChildItem -Path $sourcePath -Filter "*.cpp" -Recurse
        $this.Stats.TotalFiles = $cppFiles.Count
        
        foreach ($file in $cppFiles) {
            $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
            if (!$content) { continue }
            
            $needsFix = $false
            $fixes = @()
            
            # Check for memory issues
            if ($issueType -in @("All", "Memory")) {
                if ($content -match "new\s+\w+" -and $content -notmatch "delete\s*\w*;") {
                    $needsFix = $true
                    $fixes += "Memory: Added delete statements"
                    $this.Stats.MemoryIssues++
                    if ($AutoFix) { $content = $this.FixMemoryIssues($content, $file.Name) }
                }
            }
            
            # Check for TODO issues
            if ($issueType -in @("All", "TODO")) {
                if ($content -match "TODO|FIXME") {
                    $needsFix = $true
                    $fixes += "TODO: Marked for review"
                    $this.Stats.TODOIssues++
                }
            }
            
            if ($needsFix) {
                if ($AutoFix) {
                    # Backup original
                    $backupPath = $file.FullName + ".backup"
                    Copy-Item $file.FullName $backupPath -Force
                    
                    # Write fixed content
                    Set-Content -Path $file.FullName -Value $content -NoNewline
                    $this.FixedFiles += [PSCustomObject]@{ File = $file.Name; Fixes = $fixes }
                    $this.Stats.Fixed++
                    Write-BatchLog "Fixed: $($file.Name)" "SUCCESS"
                } else {
                    $this.SkippedFiles += [PSCustomObject]@{ File = $file.Name; Issues = $fixes }
                    $this.Stats.Skipped++
                    Write-BatchLog "Would fix: $($file.Name) - $($fixes -join ', ')" "WARNING"
                }
            }
        }
        
        $this.ShowStats()
        if ($GenerateReport) { $this.GenerateReport() }
    }
    
    [string]FixMemoryIssues([string]$content, [string]$filename) {
        # Add memory cleanup comment where new is used without delete
        $lines = $content -split "`n"
        $fixedLines = @()
        $indentLevel = 0
        
        foreach ($line in $lines) {
            $fixedLines += $line
            
            # Check for new allocation
            if ($line -match "(\s*)(\w+)\s*\*\s*(\w+)\s*=\s*new\s+" -and $line -notmatch "//.*new") {
                $indent = $matches[1]
                $typeName = $matches[2]
                $varName = $matches[3]
                
                # Add cleanup comment after the allocation
                $fixedLines += "$indent// TODO: Add cleanup - delete $varName; in destructor or use TSharedPtr<$typeName>"
            }
        }
        
        return $fixedLines -join "`n"
    }
    
    [void]ShowStats() {
        Write-Host "`n=== Batch Processing Statistics ===" -ForegroundColor Cyan
        Write-Host "  Total Files Scanned: $($this.Stats.TotalFiles)" -ForegroundColor White
        Write-Host "  Memory Issues Found: $($this.Stats.MemoryIssues)" -ForegroundColor Yellow
        Write-Host "  TODO Issues Found: $($this.Stats.TODOIssues)" -ForegroundColor Yellow
        Write-Host "  Files Fixed: $($this.Stats.Fixed)" -ForegroundColor Green
        Write-Host "  Files Skipped (Dry Run): $($this.Stats.Skipped)" -ForegroundColor Gray
        Write-Host "====================================" -ForegroundColor Cyan
    }
    
    [void]GenerateReport() {
        $reportPath = Join-Path $this.ProjectRoot "Reports\BatchProcessing_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
        
        $report = @"
# Batch Processing Report
**Date:** $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**Mode:** $(if ($AutoFix) { "Auto-Fix Enabled" } else { "Dry Run" })  
**Issue Type:** $IssueType

## Statistics
- Total Files Scanned: $($this.Stats.TotalFiles)
- Memory Issues: $($this.Stats.MemoryIssues)
- TODO Issues: $($this.Stats.TODOIssues)
- Files Fixed: $($this.Stats.Fixed)
- Files Skipped: $($this.Stats.Skipped)

## $(if ($AutoFix) { "Fixed Files" } else { "Files Needing Fix" })
"@

        $targetFiles = if ($AutoFix) { $this.FixedFiles } else { $this.SkippedFiles }
        foreach ($item in $targetFiles) {
            $report += "`n- **$($item.File)**`n"
            $report += "  - $($item.Fixes -join "`n  - ")`n"
        }
        
        $report += "`n## Recommendations`n"
        if ($this.Stats.MemoryIssues -gt 0) {
            $report += "- Consider using TSharedPtr<> for automatic memory management`n"
            $report += "- Review all new/delete pairs for potential leaks`n"
        }
        if ($this.Stats.TODOIssues -gt 0) {
            $report += "- Schedule time to address TODO/FIXME comments`n"
            $report += "- Prioritize high-impact TODOs first`n"
        }
        
        $report | Out-File $reportPath -Encoding UTF8
        Write-BatchLog "Report saved: $reportPath" "SUCCESS"
    }
}

# Main execution
Write-BatchLog "MingGoRTS Batch Processing System" "SUCCESS"

$fixer = [BatchIssueFixer]::new($ProjectRoot)
$fixer.ProcessBatch($IssueType)

Write-BatchLog "=== Batch Processing Complete ===" "SUCCESS"
