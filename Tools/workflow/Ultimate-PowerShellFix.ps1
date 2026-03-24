#Requires -Version 5.1

<#
.SYNOPSIS
    MingGoRTS Ultimate PowerShell Fix
    終極 PowerShell 修復腳本

.DESCRIPTION
    徹底修復所有 PowerShell 腳本的編碼、語法和解析問題
    Thoroughly fix all encoding, syntax and parsing issues

.PARAMETER FixMode
    修復模式: Safe(安全), Aggressive(激進), Extreme(終極)

.EXAMPLE
    .\Ultimate-PowerShellFix.ps1 -FixMode Safe
    .\Ultimate-PowerShellFix.ps1 -FixMode Extreme
#>

[CmdletBinding()]
param(
    [ValidateSet("Safe", "Aggressive", "Extreme")]
    [string]$FixMode = "Safe"
)

# Auto-detect project root
if ($PWD -match "MingGoRTS.*Tools") {
    $ProjectRoot = $PWD -replace "\\Tools.*$", ""
} elseif ($PWD -match "MingGoRTS") {
    $ProjectRoot = $PWD.Path -replace "\\MingGoRTS.*$", "\MingGoRTS"
} else {
    $ProjectRoot = "f:\MingGoRTS"
}

$ToolsPath = Join-Path $ProjectRoot "Tools"
$ReportsPath = Join-Path $ProjectRoot "Reports"
$LogsPath = Join-Path $ProjectRoot "Logs"

# Ensure directories exist
if (-not (Test-Path $LogsPath)) { New-Item -ItemType Directory -Path $LogsPath -Force | Out-Null }
if (-not (Test-Path $ReportsPath)) { New-Item -ItemType Directory -Path $ReportsPath -Force | Out-Null }

function Write-FixLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"

    $LogFile = Join-Path $LogsPath ("UltimateFix_" + (Get-Date -Format 'yyyyMMdd') + ".log")
    Add-Content -Path $LogFile -Value $LogEntry

    $Colors = @{ "INFO" = "White"; "SUCCESS" = "Green"; "WARNING" = "Yellow"; "ERROR" = "Red"; "CRITICAL" = "Magenta" }
    $Color = if ($Colors.ContainsKey($Level)) { $Colors[$Level] } else { "White" }
    Write-Host $LogEntry -ForegroundColor $Color
}

function Show-Banner {
    Write-Host ""
    Write-Host "    ==========================================" -ForegroundColor Magenta
    Write-Host "        MingGoRTS Ultimate PowerShell Fix" -ForegroundColor Magenta
    Write-Host "        Thorough Script Repair System" -ForegroundColor Magenta
    Write-Host "        Mode: $FixMode" -ForegroundColor Magenta
    Write-Host "    ==========================================" -ForegroundColor Magenta
    Write-Host ""
}

# ============================================
# Phase 1: Extreme Encoding Cleanup
# ============================================

function Start-ExtremeEncodingFix {
    param([string]$ScriptPath)

    $Content = Get-Content -Path $ScriptPath -Raw -Encoding UTF8
    $OriginalContent = $Content
    $Changes = @()

    # Fix 1: Remove all Unicode box-drawing characters (aggressive)
    $BoxChars = @(
        0x2500..0x257F,  # Box drawing
        0x2550..0x256C,  # Double box drawing
        0x2580..0x259F   # Block elements
    ) | ForEach-Object { [char]$_ }

    foreach ($Char in $BoxChars) {
        if ($Content -contains $Char) {
            $Content = $Content -replace [regex]::Escape($Char), "="
            $Changes += "Replaced box char U+$([int]$Char)"
        }
    }

    # Fix 2: Remove all emoji
    $EmojiRanges = @(
        @{ Start = 0x1F600; End = 0x1F64F },  # Emoticons
        @{ Start = 0x2600; End = 0x26FF },    # Misc symbols
        @{ Start = 0x2700; End = 0x27BF }     # Dingbats
    )

    foreach ($Range in $EmojiRanges) {
        for ($i = $Range.Start; $i -le $Range.End; $i++) {
            $Char = [char]::ConvertFromUtf32($i)
            if ($Content -match [regex]::Escape($Char)) {
                $Content = $Content -replace [regex]::Escape($Char), ""
                $Changes += "Removed emoji U+$i"
            }
        }
    }

    # Fix 3: Fix problematic quote characters
    $SmartQuotes = @(
        @{ Char = '"'; Replace = '"'; Name = "Left double quote" },
        @{ Char = '"'; Replace = '"'; Name = "Right double quote" },
        @{ Char = "'"; Replace = "'"; Name = "Left single quote" },
        @{ Char = "'"; Replace = "'"; Name = "Right single quote" }
    )

    foreach ($Quote in $SmartQuotes) {
        if ($Content -match [regex]::Escape($Quote.Char)) {
            $Content = $Content -replace [regex]::Escape($Quote.Char), $Quote.Replace
            $Changes += "Fixed $Quote.Name"
        }
    }

    # Fix 4: Remove zero-width characters
    $ZeroWidth = @(
        0x200B,  # Zero width space
        0x200C,  # Zero width non-joiner
        0x200D,  # Zero width joiner
        0xFEFF   # Zero width no-break space (BOM)
    ) | ForEach-Object { [char]$_ }

    foreach ($Char in $ZeroWidth) {
        if ($Content -match [regex]::Escape($Char)) {
            $Content = $Content -replace [regex]::Escape($Char), ""
            $Changes += "Removed zero-width char U+$([int]$Char)"
        }
    }

    # Fix 5: Replace non-ASCII dashes/hyphens
    $Dashes = @(
        @{ Char = "-"; Replace = "-"; Name = "Em dash" },
        @{ Char = "-"; Replace = "-"; Name = "En dash" },
        @{ Char = "-"; Replace = "-"; Name = "Minus sign" }
    )

    foreach ($Dash in $Dashes) {
        if ($Content -match [regex]::Escape($Dash.Char)) {
            $Content = $Content -replace [regex]::Escape($Dash.Char), $Dash.Replace
            $Changes += "Fixed $Dash.Name"
        }
    }

    # Fix 6: Clean up line endings (normalize to CRLF)
    $Content = $Content -replace "`r`n", "`n" -replace "`n", "`r`n"

    # Fix 7: Remove trailing whitespace from each line
    $Lines = $Content -split "`r`n"
    $CleanLines = $Lines | ForEach-Object { $_ -replace "\s+$", "" }
    $Content = $CleanLines -join "`r`n"

    # Fix 8: Ensure file ends with single newline
    $Content = $Content -replace "(\r\n)+$", "`r`n"

    # Check if content changed
    if ($Content -ne $OriginalContent) {
        # Backup original
        $BackupPath = $ScriptPath + ".ultimate." + (Get-Date -Format 'yyyyMMddHHmmss') + ".bak"
        Copy-Item -Path $ScriptPath -Destination $BackupPath -Force

        # Write fixed content
        [System.IO.File]::WriteAllText($ScriptPath, $Content, [System.Text.Encoding]::UTF8)

        return @{ Fixed = $true; Changes = $Changes; Backup = $BackupPath }
    }

    return @{ Fixed = $false; Changes = @(); Backup = $null }
}

# ============================================
# Phase 2: Syntax Validation
# ============================================

function Test-ScriptSyntax {
    param([string]$ScriptPath)

    $Errors = @()

    try {
        # Parse the script to check for syntax errors
        $null = [System.Management.Automation.PSParser]::Tokenize(
            (Get-Content -Path $ScriptPath -Raw),
            [ref]$Errors
        )

        # Filter out non-critical errors
        $CriticalErrors = $Errors | Where-Object {
            $_.Type -eq "Error" -and
            $_.Content -notmatch "requires" -and
            $_.Content -notmatch "deprecated"
        }

        return $CriticalErrors
    }
    catch {
        return @(@{ Message = $_.Exception.Message; Line = 0 })
    }
}

# ============================================
# Phase 3: Main Execution
# ============================================

function Main {
    Show-Banner

    Write-FixLog "Starting Ultimate PowerShell Fix..." "INFO"
    Write-FixLog "Mode: $FixMode" "INFO"
    Write-FixLog "Project Root: $ProjectRoot" "INFO"

    # Find all PowerShell scripts
    $Scripts = Get-ChildItem -Path $ToolsPath -Recurse -Include "*.ps1" -File -ErrorAction SilentlyContinue
    Write-FixLog "Found $($Scripts.Count) PowerShell scripts" "INFO"

    $FixedCount = 0
    $ErrorCount = 0
    $Results = @()

    foreach ($Script in $Scripts) {
        $RelativePath = $Script.FullName.Substring($ProjectRoot.Length + 1)
        Write-FixLog "Processing: $RelativePath" "INFO"

        # Phase 1: Extreme encoding fix
        $FixResult = Start-ExtremeEncodingFix -ScriptPath $Script.FullName

        if ($FixResult.Fixed) {
            Write-FixLog "  Fixed encoding issues" "SUCCESS"
            foreach ($Change in $FixResult.Changes) {
                Write-FixLog "    - $Change" "INFO"
            }
            $FixedCount++
        }

        # Phase 2: Syntax validation
        $SyntaxErrors = Test-ScriptSyntax -ScriptPath $Script.FullName

        if ($SyntaxErrors.Count -gt 0) {
            Write-FixLog "  Found $($SyntaxErrors.Count) syntax issues" "WARNING"
            foreach ($ErrItem in $SyntaxErrors | Select-Object -First 3) {
                Write-FixLog "    Line $($ErrItem.Line): $($ErrItem.Message)" "WARNING"
            }
            $ErrorCount += $SyntaxErrors.Count
        }

        $Results += @{
            File = $RelativePath
            Fixed = $FixResult.Fixed
            Changes = $FixResult.Changes
            SyntaxErrors = $SyntaxErrors.Count
            Backup = $FixResult.Backup
        }
    }

    # Generate report
    $ReportFile = Join-Path $ReportsPath ("UltimatePowerShellFix_Report_" + (Get-Date -Format 'yyyyMMdd_HHmmss') + ".md")

    $Report = @"
# Ultimate PowerShell Fix Report

**Generated**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Mode**: $FixMode
**Total Scripts**: $($Scripts.Count)
**Scripts Fixed**: $FixedCount
**Syntax Issues Found**: $ErrorCount

## Summary

| Metric | Value |
|--------|-------|
| Scripts Scanned | $($Scripts.Count) |
| Scripts Fixed | $FixedCount |
| Syntax Issues | $ErrorCount |
| Mode | $FixMode |

## Fixed Scripts

| Script | Changes | Backup |
|--------|---------|--------|
"@

    foreach ($Result in $Results | Where-Object { $_.Fixed }) {
        $Report += "| $($Result.File) | $($Result.Changes.Count) changes | $(Split-Path $Result.Backup -Leaf) |`n"
    }

    $Report += @"

## Scripts with Syntax Issues

| Script | Issues |
|--------|--------|
"@

    foreach ($Result in $Results | Where-Object { $_.SyntaxErrors -gt 0 }) {
        $Report += "| $($Result.File) | $($Result.SyntaxErrors) |`n"
    }

    $Report += @"

## Recommendations

1. Review all fixed scripts for functionality
2. Test scripts in PowerShell 5.1 and 7.x
3. Check backup files if issues arise
4. Run scripts with `-WhatIf` first when possible

---
*Generated by Ultimate-PowerShellFix.ps1*
"@

    $Report | Set-Content -Path $ReportFile -Encoding UTF8

    # Summary
    Write-FixLog "" "INFO"
    Write-FixLog "========================================" "INFO"
    Write-FixLog "Ultimate Fix Complete!" $(if ($ErrorCount -eq 0) { "SUCCESS" } else { "WARNING" })
    Write-FixLog "========================================" "INFO"
    Write-FixLog "Scripts scanned: $($Scripts.Count)" "INFO"
    Write-FixLog "Scripts fixed: $FixedCount" "SUCCESS"
    Write-FixLog "Syntax issues: $ErrorCount" $(if ($ErrorCount -eq 0) { "SUCCESS" } else { "WARNING" })
    Write-FixLog "Report: $ReportFile" "INFO"

    return @{ Scripts = $Scripts.Count; Fixed = $FixedCount; Errors = $ErrorCount }
}

# Execute
$Result = Main

# Return result for automation
return $Result
