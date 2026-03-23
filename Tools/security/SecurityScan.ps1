#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Security Scan Script for MingGoRTS

.DESCRIPTION
    Performs basic security checks on the codebase
#>
[CmdletBinding()]
param(
    [string]$ScanType = "Full",
    [string]$OutputPath = "Reports\Security",
    [switch]$GenerateReport,
    [switch]$FailOnCritical
)

# Ensure output directory exists
if (-not (Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
}

$ScanResults = @{
    ScanType = $ScanType
    Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Issues = @()
    Summary = @{
        Critical = 0
        High = 0
        Medium = 0
        Low = 0
        Info = 0
    }
}

Write-Host "Starting Security Scan: $ScanType" -ForegroundColor Cyan
Write-Host "Output: $OutputPath" -ForegroundColor Gray

# Check 1: Look for potential hardcoded secrets patterns
$SecretPatterns = @(
    @{ Pattern = 'password\s*=\s*["\'][^"\']+["\']'; Severity = "Critical"; Description = "Potential hardcoded password" },
    @{ Pattern = 'api[_-]?key\s*=\s*["\'][^"\']+["\']'; Severity = "Critical"; Description = "Potential hardcoded API key" },
    @{ Pattern = 'secret\s*=\s*["\'][^"\']+["\']'; Severity = "Critical"; Description = "Potential hardcoded secret" },
    @{ Pattern = 'token\s*=\s*["\'][^"\']+["\']'; Severity = "High"; Description = "Potential hardcoded token" }
)

$SourceFiles = Get-ChildItem -Path "Source","Plugins" -Recurse -Include "*.cpp","*.h","*.cs" -ErrorAction SilentlyContinue
$CheckedFiles = 0

foreach ($File in $SourceFiles) {
    $CheckedFiles++
    $Content = Get-Content $File.FullName -Raw -ErrorAction SilentlyContinue
    
    foreach ($Pattern in $SecretPatterns) {
        $Matches = [regex]::Matches($Content, $Pattern.Pattern, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
        foreach ($Match in $Matches) {
            $Issue = @{
                File = $File.FullName.Replace($PWD, "").TrimStart("\")
                Line = ($Content.Substring(0, $Match.Index) -split "`n").Count
                Severity = $Pattern.Severity
                Description = $Pattern.Description
                Pattern = $Match.Value
            }
            $ScanResults.Issues += $Issue
            $ScanResults.Summary[$Pattern.Severity]++
        }
    }
}

Write-Host "Checked $CheckedFiles source files" -ForegroundColor Green

# Check 2: File permissions (basic check)
$ExecutableScripts = Get-ChildItem -Path "." -Recurse -Include "*.ps1","*.bat","*.sh" -ErrorAction SilentlyContinue | Where-Object { $_.FullName -notlike "*\.git*" }
$ScanResults.Summary.Info = $ExecutableScripts.Count

# Generate report
if ($GenerateReport) {
    $ReportFile = Join-Path $OutputPath "scan-report.json"
    $ScanResults | ConvertTo-Json -Depth 10 | Set-Content $ReportFile
    Write-Host "Report saved: $ReportFile" -ForegroundColor Green
    
    # Also generate text log
    $LogFile = Join-Path $OutputPath "scan.log"
    $LogContent = @"
Security Scan Report
====================
Timestamp: $($ScanResults.Timestamp)
Scan Type: $($ScanResults.ScanType)
Files Checked: $CheckedFiles

Summary:
- Critical: $($ScanResults.Summary.Critical)
- High: $($ScanResults.Summary.High)
- Medium: $($ScanResults.Summary.Medium)
- Low: $($ScanResults.Summary.Low)
- Info: $($ScanResults.Summary.Info)

Issues Found: $($ScanResults.Issues.Count)
"@
    $LogContent | Set-Content $LogFile
    Write-Host "Log saved: $LogFile" -ForegroundColor Green
}

# Output summary
Write-Host "" -ForegroundColor Cyan
Write-Host "Security Scan Complete" -ForegroundColor Cyan
Write-Host "=====================" -ForegroundColor Cyan
Write-Host "Critical: $($ScanResults.Summary.Critical)" -ForegroundColor $(if ($ScanResults.Summary.Critical -gt 0) { "Red" } else { "Green" })
Write-Host "High: $($ScanResults.Summary.High)" -ForegroundColor $(if ($ScanResults.Summary.High -gt 0) { "Red" } else { "Green" })
Write-Host "Medium: $($ScanResults.Summary.Medium)" -ForegroundColor Yellow
Write-Host "Low: $($ScanResults.Summary.Low)" -ForegroundColor Gray
Write-Host "Info: $($ScanResults.Summary.Info)" -ForegroundColor Gray

# Exit with error if critical issues found and FailOnCritical is set
if ($FailOnCritical -and $ScanResults.Summary.Critical -gt 0) {
    Write-Error "Critical security issues found!"
    exit 1
}

exit 0
