#Requires -Version 5.1

<#
.SYNOPSIS
    MingGoRTS PowerShell IDE Clean Fix

.DESCRIPTION
    Removes all characters that cause IDE warnings while preserving functionality

.PARAMETER ProjectRoot
    Project root directory

.EXAMPLE
    .\Clean-FixAll.ps1
    .\Clean-FixAll.ps1 -ProjectRoot "F:\MingGoRTS"
#>

[CmdletBinding()]
param(
    [string]$ProjectRoot = ""
)

# Determine project root
if ([string]::IsNullOrWhiteSpace($ProjectRoot)) {
    if ($PWD -match "MingGoRTS") {
        $ProjectRoot = $PWD.Path -replace "\\MingGoRTS.*$", "\MingGoRTS"
    } else {
        $ProjectRoot = "f:\MingGoRTS"
    }
}

$ToolsPath = Join-Path $ProjectRoot "Tools"
$ReportsPath = Join-Path $ProjectRoot "Reports"
$LogsPath = Join-Path $ProjectRoot "Logs"

# Create directories if needed
if (-not (Test-Path $LogsPath)) {
    New-Item -ItemType Directory -Path $LogsPath -Force | Out-Null
}

function Write-FixLog {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Message,

        [Parameter(Mandatory = $false)]
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR")]
        [string]$Level = "INFO"
    )

    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"

    $LogFile = Join-Path $LogsPath ("CleanFix_" + (Get-Date -Format 'yyyyMMdd') + ".log")
    Add-Content -Path $LogFile -Value $LogEntry

    $Colors = @{
        "INFO" = "White"
        "SUCCESS" = "Green"
        "WARNING" = "Yellow"
        "ERROR" = "Red"
    }
    $Color = $Colors[$Level]
    Write-Host $LogEntry -ForegroundColor $Color
}

function Show-Banner {
    Write-Host ""
    Write-Host "==========================================" -ForegroundColor Cyan
    Write-Host "MingGoRTS Clean Fix Tool" -ForegroundColor Cyan
    Write-Host "Removes IDE warning triggers" -ForegroundColor Cyan
    Write-Host "==========================================" -ForegroundColor Cyan
    Write-Host ""
}

function Start-CleanFix {
    param([string]$FilePath)

    $Content = Get-Content -Path $FilePath -Raw -Encoding UTF8
    $Original = $Content
    $Changes = 0

    # Fix 1: Replace em dash with double dash
    if ($Content -match [char]0x2014) {
        $Content = $Content -replace [char]0x2014, "--"
        $Changes++
    }

    # Fix 2: Replace en dash with single dash
    if ($Content -match [char]0x2013) {
        $Content = $Content -replace [char]0x2013, "-"
        $Changes++
    }

    # Fix 3: Replace smart quotes with straight quotes
    $SmartQuotes = @(
        @{ Pattern = [char]0x201C; Replace = '"' },  # Left double
        @{ Pattern = [char]0x201D; Replace = '"' },  # Right double
        @{ Pattern = [char]0x2018; Replace = "'" },   # Left single
        @{ Pattern = [char]0x2019; Replace = "'" }    # Right single
    )

    foreach ($Quote in $SmartQuotes) {
        if ($Content -match $Quote.Pattern) {
            $Content = $Content -replace $Quote.Pattern, $Quote.Replace
            $Changes++
        }
    }

    # Fix 4: Remove zero-width characters
    $ZeroWidth = @(
        0x200B,  # Zero width space
        0x200C,  # Zero width non-joiner
        0x200D,  # Zero width joiner
        0xFEFF   # BOM
    )

    foreach ($Code in $ZeroWidth) {
        $Char = [char]$Code
        if ($Content -match $Char) {
            $Content = $Content -replace $Char, ""
            $Changes++
        }
    }

    # Fix 5: Remove Unicode box drawing characters
    $BoxRange1 = 0x2500..0x257F
    $BoxRange2 = 0x2550..0x256C
    foreach ($Code in $BoxRange1) {
        $Char = [char]$Code
        if ($Content -match $Char) {
            $Content = $Content -replace $Char, "="
            $Changes++
        }
    }
    foreach ($Code in $BoxRange2) {
        $Char = [char]$Code
        if ($Content -match $Char) {
            $Content = $Content -replace $Char, "="
            $Changes++
        }
    }

    # Fix 6: Remove emoji characters
    $EmojiRange = 0x1F600..0x1F64F
    foreach ($Code in $EmojiRange) {
        $Char = [char]::ConvertFromUtf32($Code)
        if ($Content -match $Char) {
            $Content = $Content -replace $Char, ""
            $Changes++
        }
    }

    # Fix 7: Replace ellipsis with three dots
    if ($Content -match [char]0x2026) {
        $Content = $Content -replace [char]0x2026, "..."
        $Changes++
    }

    # Fix 8: Replace bullet points with hyphens
    $Bullets = @([char]0x2022, [char]0x2023, [char]0x25E6)
    foreach ($Bullet in $Bullets) {
        if ($Content -match $Bullet) {
            $Content = $Content -replace $Bullet, "-"
            $Changes++
        }
    }

    # Fix 9: Clean up line endings
    $Content = $Content -replace "`r?`n", "`n" -replace "`n", "`r`n"

    # Fix 10: Remove trailing whitespace
    $Lines = $Content -split "`r`n"
    $CleanLines = foreach ($Line in $Lines) { $Line -replace "\s+$", "" }
    $Content = $CleanLines -join "`r`n"

    # Write if changed
    if ($Content -ne $Original) {
        $BackupPath = $FilePath + ".cleanbak." + (Get-Date -Format 'yyyyMMddHHmmss')
        Copy-Item -Path $FilePath -Destination $BackupPath -Force

        [System.IO.File]::WriteAllText($FilePath, $Content, [System.Text.Encoding]::UTF8)

        return @{ Fixed = $true; Changes = $Changes; Backup = $BackupPath }
    }

    return @{ Fixed = $false; Changes = 0; Backup = $null }
}

function Main {
    Show-Banner

    Write-FixLog -Message "Starting Clean Fix..." -Level "INFO"
    Write-FixLog -Message "Project Root: $ProjectRoot" -Level "INFO"

    # Find all PowerShell scripts
    $Scripts = Get-ChildItem -Path $ToolsPath -Recurse -Include "*.ps1" -File -ErrorAction SilentlyContinue

    if (-not $Scripts) {
        Write-FixLog -Message "No PowerShell scripts found" -Level "WARNING"
        return
    }

    Write-FixLog -Message "Found $($Scripts.Count) PowerShell scripts" -Level "INFO"

    $FixedCount = 0
    $TotalChanges = 0

    foreach ($Script in $Scripts) {
        $RelativePath = $Script.FullName.Substring($ProjectRoot.Length + 1)
        Write-FixLog -Message "Processing: $RelativePath" -Level "INFO"

        $Result = Start-CleanFix -FilePath $Script.FullName

        if ($Result.Fixed) {
            Write-FixLog -Message "Fixed $($Result.Changes) issues" -Level "SUCCESS"
            $FixedCount++
            $TotalChanges += $Result.Changes
        }
    }

    # Generate report
    $ReportFile = Join-Path $ReportsPath ("CleanFix_Report_" + (Get-Date -Format 'yyyyMMdd_HHmmss') + ".md")

    $Report = @"
# Clean Fix Report

**Generated**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Project Root**: $ProjectRoot

## Summary

| Metric | Value |
|--------|-------|
| Scripts Scanned | $($Scripts.Count) |
| Scripts Fixed | $FixedCount |
| Total Changes | $TotalChanges |

## Details

Fixed issues:
- Smart quotes (curly quotes)
- Em dash and en dash
- Unicode box drawing characters
- Emoji characters
- Zero-width characters
- Ellipsis
- Bullet points
- Trailing whitespace
- Line ending normalization

## Result

All scripts have been cleaned to eliminate IDE warnings.
Backups created with .cleanbak. timestamp extension.

---
*Generated by Clean-FixAll.ps1*
"@

    $Report | Set-Content -Path $ReportFile -Encoding UTF8

    Write-FixLog -Message "" -Level "INFO"
    Write-FixLog -Message "========================================" -Level "INFO"
    Write-FixLog -Message "Clean Fix Complete!" -Level "SUCCESS"
    Write-FixLog -Message "========================================" -Level "INFO"
    Write-FixLog -Message "Scripts scanned: $($Scripts.Count)" -Level "INFO"
    Write-FixLog -Message "Scripts fixed: $FixedCount" -Level "SUCCESS"
    Write-FixLog -Message "Total changes: $TotalChanges" -Level "SUCCESS"
    Write-FixLog -Message "Report: $ReportFile" -Level "INFO"
}

Main
