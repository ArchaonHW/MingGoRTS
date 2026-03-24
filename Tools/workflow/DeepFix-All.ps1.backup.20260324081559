#Requires -Version 5.1

<#
.SYNOPSIS
    MingGoRTS 深度修复脚本
    Deep Fix Script for PowerShell Issues

.DESCRIPTION
    全面扫描和修复所有 PowerShell 脚本的编码、语法和质量问题
    Comprehensive scan and fix for all PowerShell scripts

.PARAMETER ScanOnly
    仅扫描不修复

.PARAMETER FixAll
    执行所有修复

.EXAMPLE
    .\DeepFix-All.ps1 -ScanOnly
    .\DeepFix-All.ps1 -FixAll
#>

[CmdletBinding()]
param(
    [switch]$ScanOnly,
    [switch]$FixAll
)

# Auto-detect project root
if ($PWD -match "MingGoRTS.*Tools") {
    $ProjectRoot = $PWD -replace "\\Tools.*$", ""
} elseif ($PWD -match "MingGoRTS") {
    $ProjectRoot = $PWD.Path -replace "\\MingGoRTS.*$", "\MingGoRTS"
} else {
    $ProjectRoot = "f:\MingGoRTS"
}

$ReportsPath = Join-Path $ProjectRoot "Reports"
$LogsPath = Join-Path $ProjectRoot "Logs"

# Ensure directories exist
if (-not (Test-Path $LogsPath)) { New-Item -ItemType Directory -Path $LogsPath -Force | Out-Null }
if (-not (Test-Path $ReportsPath)) { New-Item -ItemType Directory -Path $ReportsPath -Force | Out-Null }

function Write-FixLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    
    $LogFile = Join-Path $LogsPath ("DeepFix_" + (Get-Date -Format 'yyyyMMdd') + ".log")
    Add-Content -Path $LogFile -Value $LogEntry
    
    $Colors = @{ "INFO" = "White"; "SUCCESS" = "Green"; "WARNING" = "Yellow"; "ERROR" = "Red" }
    $Color = if ($Colors.ContainsKey($Level)) { $Colors[$Level] } else { "White" }
    Write-Host $LogEntry -ForegroundColor $Color
}

function Show-Banner {
    Write-Host ""
    Write-Host "    ==========================================" -ForegroundColor Cyan
    Write-Host "        MingGoRTS Deep Fix Tool" -ForegroundColor Cyan
    Write-Host "        Comprehensive Script Repair" -ForegroundColor Cyan
    Write-Host "    ==========================================" -ForegroundColor Cyan
    Write-Host ""
}

# ============================================
# Scan Functions
# ============================================

function Test-ScriptIssues {
    param([string]$ScriptPath)
    
    $Issues = @()
    
    try {
        $Content = Get-Content -Path $ScriptPath -Raw -ErrorAction SilentlyContinue
        if (-not $Content) { return $Issues }
        
        # Check 1: Unicode box-drawing characters
        if ($Content -match '[\u2550-\u256C\u2500-\u257F]') {
            $Issues += @{ Type = "ENCODING"; Severity = "WARNING"; Message = "Unicode box-drawing characters detected" }
        }
        
        # Check 2: Emoji characters
        if ($Content -match '[\u2700-\u27BF\u2600-\u26FF\u1F600-\u1F64F]') {
            $Issues += @{ Type = "ENCODING"; Severity = "WARNING"; Message = "Emoji characters detected" }
        }
        
        # Check 3: Replacement character (encoding corruption)
        if ($Content -match '\uFFFD') {
            $Issues += @{ Type = "ENCODING"; Severity = "ERROR"; Message = "Encoding corruption (replacement character)" }
        }
        
        # Check 4: BOM marker issues
        $Bytes = [System.IO.File]::ReadAllBytes($ScriptPath)
        if ($Bytes.Length -ge 3 -and $Bytes[0] -eq 0xEF -and $Bytes[1] -eq 0xBB -and $Bytes[2] -eq 0xBF) {
            # UTF-8 BOM is okay, but flag for consistency
        }
        
        # Check 5: Mixed line endings
        $HasCRLF = $Content -match "`r`n"
        $HasLF = ($Content -replace "`r`n", "") -match "`n"
        if ($HasCRLF -and $HasLF) {
            $Issues += @{ Type = "FORMAT"; Severity = "WARNING"; Message = "Mixed line endings (CRLF and LF)" }
        }
        
        # Check 6: Trailing whitespace
        if ($Content -match " \n|\t\n|\r\n[ \t]+\r?\n") {
            $Issues += @{ Type = "FORMAT"; Severity = "INFO"; Message = "Trailing whitespace detected" }
        }
        
        # Check 7: Unused variables (simple check)
        $VariableAssignments = [regex]::Matches($Content, '\$([a-zA-Z_][a-zA-Z0-9_]*)\s*=') | ForEach-Object { $_.Groups[1].Value }
        $VariableUsages = [regex]::Matches($Content, '\$([a-zA-Z_][a-zA-Z0-9_]*)') | ForEach-Object { $_.Groups[1].Value }
        
        foreach ($Var in $VariableAssignments | Select-Object -Unique) {
            $UsageCount = ($VariableUsages | Where-Object { $_ -eq $Var }).Count
            $AssignmentCount = ($VariableAssignments | Where-Object { $_ -eq $Var }).Count
            
            # Skip common PowerShell automatic variables
            $AutoVars = @("PWD", "PSVersionTable", "Host", "null", "true", "false", "HOME", "PROFILE", "Error", "Args", "Input", "Matches", "PSItem", "_")
            if ($Var -notin $AutoVars -and $UsageCount -le $AssignmentCount) {
                # This is a simplified check - may have false positives
            }
        }
        
        # Check 8: Empty catch blocks
        if ($Content -match 'catch\s*\{[^}]*\}') {
            $EmptyCatchMatches = [regex]::Matches($Content, 'catch\s*\{\s*\}')
            if ($EmptyCatchMatches.Count -gt 0) {
                $Issues += @{ Type = "CODE_QUALITY"; Severity = "WARNING"; Message = "Empty catch blocks detected" }
            }
        }
        
        # Check 9: Write-Host without color (accessibility)
        $WriteHostMatches = [regex]::Matches($Content, 'Write-Host\s+([^-])')
        if ($WriteHostMatches.Count -gt 0) {
            $Issues += @{ Type = "CODE_QUALITY"; Severity = "INFO"; Message = "Write-Host without -ForegroundColor (accessibility)" }
        }
    }
    catch {
        $Issues += @{ Type = "ERROR"; Severity = "ERROR"; Message = "Failed to analyze: $($_.Exception.Message)" }
    }
    
    return $Issues
}

function Start-DeepScan {
    param([string]$ToolsPath)
    
    Write-FixLog "Starting deep scan..." "INFO"
    
    $Results = @()
    $ScriptFiles = Get-ChildItem -Path $ToolsPath -Recurse -Include "*.ps1" -File -ErrorAction SilentlyContinue
    
    Write-FixLog "Found $($ScriptFiles.Count) PowerShell scripts" "INFO"
    
    foreach ($File in $ScriptFiles) {
        $RelativePath = $File.FullName.Substring($ProjectRoot.Length + 1)
        $Issues = Test-ScriptIssues -ScriptPath $File.FullName
        
        $Results += @{
            File = $RelativePath
            FullPath = $File.FullName
            IssueCount = $Issues.Count
            Issues = $Issues
            Size = $File.Length
            LastModified = $File.LastWriteTime
        }
        
        if ($Issues.Count -gt 0) {
            $ErrorCount = ($Issues | Where-Object { $_.Severity -eq "ERROR" }).Count
            $WarnCount = ($Issues | Where-Object { $_.Severity -eq "WARNING" }).Count
            $Level = if ($ErrorCount -gt 0) { "ERROR" } elseif ($WarnCount -gt 0) { "WARNING" } else { "INFO" }
            Write-FixLog "$RelativePath : $($Issues.Count) issues ($ErrorCount errors, $WarnCount warnings)" $Level
            
            foreach ($Issue in $Issues) {
                Write-FixLog "  [$($Issue.Type)] $($Issue.Message)" $Issue.Severity
            }
        }
    }
    
    return $Results
}

# ============================================
# Fix Functions
# ============================================

function Start-FixIssues {
    param([array]$ScanResults)
    
    Write-FixLog "Starting automated fixes..." "INFO"
    
    $FixedCount = 0
    $BackupCount = 0
    
    foreach ($Result in $ScanResults | Where-Object { $_.IssueCount -gt 0 }) {
        $FilePath = $Result.FullPath
        $EncodingIssues = $Result.Issues | Where-Object { $_.Type -eq "ENCODING" }
        $FormatIssues = $Result.Issues | Where-Object { $_.Type -eq "FORMAT" }
        
        if ($EncodingIssues.Count -gt 0 -or $FormatIssues.Count -gt 0) {
            Write-FixLog "Fixing: $($Result.File)" "INFO"
            
            try {
                # Backup original
                $BackupPath = $FilePath + ".backup." + (Get-Date -Format 'yyyyMMddHHmmss')
                Copy-Item -Path $FilePath -Destination $BackupPath -Force
                $BackupCount++
                
                # Read content
                $Content = Get-Content -Path $FilePath -Raw -Encoding UTF8
                
                # Fix 1: Replace Unicode box characters
                if ($EncodingIssues | Where-Object { $_.Message -like "*box-drawing*" }) {
                    $Content = $Content -replace '[\u2550-\u256C]', '=' -replace '[\u2500-\u257F]', '-'
                    Write-FixLog "  Fixed box-drawing characters" "SUCCESS"
                }
                
                # Fix 2: Remove emoji
                if ($EncodingIssues | Where-Object { $_.Message -like "*Emoji*" }) {
                    $Content = $Content -replace '[\u2700-\u27BF\u2600-\u26FF\u1F600-\u1F64F]', ''
                    Write-FixLog "  Removed emoji characters" "SUCCESS"
                }
                
                # Fix 3: Remove trailing whitespace
                if ($FormatIssues | Where-Object { $_.Message -like "*Trailing*" }) {
                    $Content = $Content -replace " \n", "`n" -replace "\t\n", "`n"
                    Write-FixLog "  Removed trailing whitespace" "SUCCESS"
                }
                
                # Fix 4: Normalize line endings to CRLF
                $Content = $Content -replace "`r?`n", "`r`n"
                
                # Write fixed content
                Set-Content -Path $FilePath -Value $Content -Encoding UTF8 -Force
                
                $FixedCount++
                Write-FixLog "  Fixed and backed up" "SUCCESS"
            }
            catch {
                Write-FixLog "  Failed to fix: $($_.Exception.Message)" "ERROR"
            }
        }
    }
    
    Write-FixLog "Fixed $FixedCount files, created $BackupCount backups" "SUCCESS"
    return @{ Fixed = $FixedCount; Backups = $BackupCount }
}

# ============================================
# Report Generation
# ============================================

function New-DeepFixReport {
    param([array]$ScanResults, [hashtable]$FixResults)
    
    $ReportFile = Join-Path $ReportsPath ("DeepFix_Report_" + (Get-Date -Format 'yyyyMMdd_HHmmss') + ".md")
    
    $TotalFiles = $ScanResults.Count
    $FilesWithIssues = ($ScanResults | Where-Object { $_.IssueCount -gt 0 }).Count
    $TotalIssues = ($ScanResults | ForEach-Object { $_.IssueCount } | Measure-Object -Sum).Sum
    $ErrorCount = ($ScanResults | ForEach-Object { $_.Issues | Where-Object { $_.Severity -eq "ERROR" } }).Count
    $WarningCount = ($ScanResults | ForEach-Object { $_.Issues | Where-Object { $_.Severity -eq "WARNING" } }).Count
    
    $Report = @"
# MingGoRTS Deep Fix Report

**Generated**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Project Root**: $ProjectRoot

## Summary

| Metric | Value |
|--------|-------|
| Total Scripts Scanned | $TotalFiles |
| Scripts with Issues | $FilesWithIssues |
| Total Issues Found | $TotalIssues |
| Errors | $ErrorCount |
| Warnings | $WarningCount |
| Files Fixed | $($FixResults.Fixed) |
| Backups Created | $($FixResults.Backups) |

## Issue Types Distribution

| Type | Count |
|------|-------|
| ENCODING | $(($ScanResults | ForEach-Object { $_.Issues | Where-Object { $_.Type -eq "ENCODING" } }).Count) |
| FORMAT | $(($ScanResults | ForEach-Object { $_.Issues | Where-Object { $_.Type -eq "FORMAT" } }).Count) |
| CODE_QUALITY | $(($ScanResults | ForEach-Object { $_.Issues | Where-Object { $_.Type -eq "CODE_QUALITY" } }).Count) |

## Files with Issues

| File | Issues | Severity |
|------|--------|----------|
"@

    foreach ($Result in $ScanResults | Where-Object { $_.IssueCount -gt 0 } | Sort-Object IssueCount -Descending) {
        $MaxSeverity = if ($Result.Issues | Where-Object { $_.Severity -eq "ERROR" }) { "ERROR" } 
                     elseif ($Result.Issues | Where-Object { $_.Severity -eq "WARNING" }) { "WARNING" } 
                     else { "INFO" }
        $Report += "| $($Result.File) | $($Result.IssueCount) | $MaxSeverity |`n"
    }

    $Report += @"

## Detailed Issues

"@

    foreach ($Result in $ScanResults | Where-Object { $_.IssueCount -gt 0 }) {
        $Report += "### $($Result.File)`n`n"
        foreach ($Issue in $Result.Issues) {
            $Report += "- **[$($Issue.Severity)]** $($Issue.Type): $($Issue.Message)`n"
        }
        $Report += "`n"
    }

    $Report += @"
## Recommendations

1. **Encoding Issues**: Use only ASCII characters for box drawing, avoid emoji
2. **Format Issues**: Use consistent CRLF line endings, remove trailing whitespace
3. **Code Quality**: Add error handling to catch blocks, use Write-Host with colors

## Fixed Scripts

The following scripts were automatically fixed:
"@

    if ($FixResults.Fixed -gt 0) {
        $Report += "`n- Total: $($FixResults.Fixed) scripts fixed`n"
        $Report += "- Backups: $($FixResults.Backups) backup files created`n"
    } else {
        $Report += "`nNo scripts were automatically fixed (scan-only mode or no issues found).`n"
    }

    $Report += "`n---`n*Generated by DeepFix-All.ps1*"

    $Report | Set-Content -Path $ReportFile -Encoding UTF8
    Write-FixLog "Report generated: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# Main Execution
# ============================================

function Main {
    Show-Banner
    
    $ToolsPath = Join-Path $ProjectRoot "Tools"
    
    if (-not (Test-Path $ToolsPath)) {
        Write-FixLog "Tools directory not found: $ToolsPath" "ERROR"
        exit 1
    }
    
    # Phase 1: Deep Scan
    $ScanResults = Start-DeepScan -ToolsPath $ToolsPath
    
    # Phase 2: Fix Issues (if not scan-only)
    $FixResults = @{ Fixed = 0; Backups = 0 }
    if (-not $ScanOnly -or $FixAll) {
        $FixResults = Start-FixIssues -ScanResults $ScanResults
    }
    
    # Phase 3: Generate Report
    $ReportFile = New-DeepFixReport -ScanResults $ScanResults -FixResults $FixResults
    
    # Summary
    Write-FixLog ""
    Write-FixLog "========================================" "INFO"
    Write-FixLog "Deep Fix Complete!" "SUCCESS"
    Write-FixLog "========================================" "INFO"
    Write-FixLog "Scripts scanned: $($ScanResults.Count)" "INFO"
    Write-FixLog "Files with issues: $(($ScanResults | Where-Object { $_.IssueCount -gt 0 }).Count)" "INFO"
    Write-FixLog "Total issues: $(($ScanResults | ForEach-Object { $_.IssueCount } | Measure-Object -Sum).Sum)" "INFO"
    if (-not $ScanOnly) {
        Write-FixLog "Files fixed: $($FixResults.Fixed)" "SUCCESS"
    }
    Write-FixLog "Report: $ReportFile" "INFO"
    
    # Interactive menu
    Write-Host ""
    Write-Host "Available actions:" -ForegroundColor Yellow
    Write-Host "  [1] View report" -ForegroundColor White
    Write-Host "  [2] Run QuickFix" -ForegroundColor White
    Write-Host "  [0] Exit" -ForegroundColor White
    Write-Host ""
    
    $Choice = Read-Host "Select"
    switch ($Choice) {
        "1" { Start-Process notepad.exe $ReportFile }
        "2" { 
            $QuickFix = Join-Path $ProjectRoot "Tools\workflow\QuickFix-All.ps1"
            if (Test-Path $QuickFix) { & $QuickFix -FixAll }
        }
        "0" { exit 0 }
    }
}

# Execute
Main
