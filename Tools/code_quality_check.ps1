#!/usr/bin/env pwsh
# Copyright (c) 2026 MingGoRTS. All rights reserved.
# Code Quality Check Script
# Performs automated code quality checks for MingGoRTS project

param(
    [switch]$Fix,
    [switch]$Verbose,
    [string]$Path = "Source/MingGoRTS"
)

$ErrorCount = 0
$WarningCount = 0
$FixedCount = 0

# Color output functions
function Write-ErrorLine($message) {
    Write-Host "  [ERROR] $message" -ForegroundColor Red
    $script:ErrorCount++
}

function Write-WarningLine($message) {
    Write-Host "  [WARN] $message" -ForegroundColor Yellow
    $script:WarningCount++
}

function Write-FixLine($message) {
    Write-Host "  [FIXED] $message" -ForegroundColor Green
    $script:FixedCount++
}

function Write-InfoLine($message) {
    if ($Verbose) {
        Write-Host "  [INFO] $message" -ForegroundColor Gray
    }
}

# Check 1: MINGGORTS_API macro errors
function Check-MacroErrors {
    Write-Host "`n[Check 1] Checking MINGGORTS_API macro errors..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    $found = $false
    
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        if ($content -match 'MINGGORTS_API') {
            $found = $true
            Write-ErrorLine "Found MINGGORTS_API in $($file.FullName)"
            if ($Fix) {
                $content = $content -replace 'MINGGORTS_API', 'MINGRTS_API'
                Set-Content $file.FullName $content -NoNewline
                Write-FixLine "Fixed MINGGORTS_API in $($file.Name)"
            }
        }
    }
    
    if (-not $found) {
        Write-InfoLine "No MINGGORTS_API errors found"
    }
}

# Check 2: Class closure errors (using ); instead of };)
function Check-ClassClosureErrors {
    Write-Host "`n[Check 2] Checking class closure errors..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    $found = $false
    
    foreach ($file in $files) {
        $lines = Get-Content $file.FullName
        for ($i = 0; $i -lt $lines.Count; $i++) {
            # Check for class/struct closure with );
            if ($lines[$i] -match '^\s*\);\s*$' -and $i -gt 0) {
                # Check if previous line contains function declaration
                if ($lines[$i-1] -match 'void.*\(\)|bool.*\(\)|int32.*\(\)' -or 
                    $lines[$i-1] -match 'UFUNCTION\s*\(\)') {
                    $found = $true
                    Write-ErrorLine "Found class closure error at line $($i+1) in $($file.Name)"
                    if ($Fix) {
                        $lines[$i] = $lines[$i] -replace '\);', '};'
                        Set-Content $file.FullName ($lines -join "`n") -NoNewline
                        Write-FixLine "Fixed class closure at line $($i+1)"
                    }
                }
            }
        }
    }
    
    if (-not $found) {
        Write-InfoLine "No class closure errors found"
    }
}

# Check 3: Enum definition format
function Check-EnumFormat {
    Write-Host "`n[Check 3] Checking enum definition format..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    $found = $false
    
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        # Check for enum values without initial value for first item
        if ($content -match 'enum\s+class\s+\w+\s*:\s*uint8\s*\{[^}]*?(\w+)\s+UMETA\([^}]*\}') {
            $matches = [regex]::Matches($content, 'enum\s+class\s+(\w+)\s*:\s*uint8\s*\{[^}]*?\n\s*(\w+)\s+UMETA')
            foreach ($match in $matches) {
                $enumName = $match.Groups[1].Value
                $firstValue = $match.Groups[2].Value
                if ($firstValue -ne 'None' -and $firstValue -ne 'Count') {
                    Write-WarningLine "Enum $enumName first value $firstValue may need = 0"
                }
            }
        }
    }
    
    if (-not $found) {
        Write-InfoLine "Enum format check completed"
    }
}

# Check 4: Missing UPROPERTY on TMap with TArray
function Check-UPROPERTYTMap {
    Write-Host "`n[Check 4] Checking UPROPERTY TMap<TArray> declarations..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        # Check for TMap with TArray without proper UPROPERTY handling
        if ($content -match 'UPROPERTY\s*\([^)]*\)\s*\r?\n\s*TMap<[^,]+,\s*TArray<') {
            Write-WarningLine "Found UPROPERTY TMap<TArray> in $($file.Name) - may need to remove UPROPERTY"
        }
    }
}

# Check 5: File encoding (UTF-8 BOM check)
function Check-FileEncoding {
    Write-Host "`n[Check 5] Checking file encoding..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    
    foreach ($file in $files) {
        $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
        # Check for UTF-8 BOM
        if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
            Write-InfoLine "$($file.Name) has UTF-8 BOM encoding"
        }
        # Check for non-UTF-8 encoding (simplified check)
        elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE) {
            Write-WarningLine "$($file.Name) has UTF-16 LE encoding - should be UTF-8"
        }
        elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF) {
            Write-WarningLine "$($file.Name) has UTF-16 BE encoding - should be UTF-8"
        }
    }
}

# Check 6: Missing include guards (should use #pragma once)
function Check-IncludeGuards {
    Write-Host "`n[Check 6] Checking include guards..." -ForegroundColor Cyan
    $files = Get-ChildItem -Path $Path -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        if (-not ($content -match '#pragma\s+once')) {
            Write-ErrorLine "$($file.Name) missing #pragma once"
        }
    }
}

# Main execution
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "  MingGoRTS Code Quality Checker" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Checking path: $Path"
if ($Fix) {
    Write-Host "Mode: Fix enabled (will auto-fix issues)" -ForegroundColor Green
} else {
    Write-Host "Mode: Check only (use -Fix to auto-fix)" -ForegroundColor Yellow
}

# Run all checks
Check-MacroErrors
Check-ClassClosureErrors
Check-EnumFormat
Check-UPROPERTYTMap
Check-FileEncoding
Check-IncludeGuards

# Summary
Write-Host "`n========================================" -ForegroundColor Magenta
Write-Host "  Summary" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Errors:   $ErrorCount" -ForegroundColor $(if ($ErrorCount -gt 0) { 'Red' } else { 'Green' })
Write-Host "Warnings: $WarningCount" -ForegroundColor $(if ($WarningCount -gt 0) { 'Yellow' } else { 'Green' })
if ($Fix) {
    Write-Host "Fixed:    $FixedCount" -ForegroundColor Green
}

if ($ErrorCount -eq 0 -and $WarningCount -eq 0) {
    Write-Host "`nAll checks passed!" -ForegroundColor Green
    exit 0
} elseif ($ErrorCount -eq 0) {
    Write-Host "`nChecks completed with warnings." -ForegroundColor Yellow
    exit 0
} else {
    Write-Host "`nChecks failed with errors." -ForegroundColor Red
    exit 1
}
