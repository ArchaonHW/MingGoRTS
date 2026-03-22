#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Create PowerShell script with proper UTF-8 BOM encoding"""

import os

script_content = '''#!/usr/bin/env pwsh
# 自動化編譯除錯工具
param(
    [switch]$QuickFix,
    [switch]$DeepScan,
    [switch]$VerifyCompile
)

$ProjectPath = "C:\\HW\\MingGoRTS"
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$ErrorLog = "$ProjectPath\\CompileErrors_$timestamp.log"

function Write-Log {
    param($Message, $Level = "INFO")
    $time = Get-Date -Format "HH:mm:ss"
    $line = "[$time] [$Level] $Message"
    Write-Host $line
    Add-Content $ErrorLog $line -ErrorAction SilentlyContinue
}

# 快速修復
if ($QuickFix) {
    Write-Host "=== 開始快速修復 ===" -ForegroundColor Cyan
    $fixed = 0
    
    # 修復 EOF
    Get-ChildItem -Path "$ProjectPath\\Source" -Recurse -Filter "*.h" | ForEach-Object {
        $content = Get-Content $_.FullName -Raw -ErrorAction SilentlyContinue
        if ($content) {
            $open = ([regex]::Matches($content, "{").Count)
            $close = ([regex]::Matches($content, "}").Count)
            if ($open -gt $close) {
                Add-Content $_.FullName "`n};" -NoNewline
                Write-Host "Fixed EOF: $($_.Name)" -ForegroundColor Green
                $fixed++
            }
        }
    }
    
    # 修復重複定義
    $dup = @{
        "FCulturalPreferences" = "FRTSCulturalPreferences"
        "ELanguageCode" = "ERTSLanguageCode"
    }
    
    Get-ChildItem -Path "$ProjectPath\\Source" -Recurse -Filter "*.h" | ForEach-Object {
        $c = Get-Content $_.FullName -Raw -ErrorAction SilentlyContinue
        $mod = $false
        foreach ($k in $dup.Keys) {
            if ($c -match $k) {
                $c = $c -replace $k, $dup[$k]
                $mod = $true
            }
        }
        if ($mod) {
            Set-Content $_.FullName $c -NoNewline
            Write-Host "Fixed duplicates: $($_.Name)" -ForegroundColor Yellow
            $fixed++
        }
    }
    
    Write-Host "修復完成! 共修復 $fixed 個檔案" -ForegroundColor Green
}

# 深度掃描
if ($DeepScan) {
    Write-Host "=== 開始深度掃描 ===" -ForegroundColor Cyan
    $issues = @()
    
    Get-ChildItem -Path "$ProjectPath\\Source" -Recurse -Filter "*.cpp" | ForEach-Object {
        $c = Get-Content $_.FullName -Raw -ErrorAction SilentlyContinue
        if ($c -match "EMingResourceType|ELanguageCode") {
            $issues += $_.Name
        }
    }
    
    if ($issues.Count -gt 0) {
        Write-Host "發現 $($issues.Count) 個潛在問題:" -ForegroundColor Yellow
        $issues | ForEach-Object { Write-Host "  - $_" }
    } else {
        Write-Host "未發現明顯問題" -ForegroundColor Green
    }
}

# 驗證編譯
if ($VerifyCompile) {
    Write-Host "=== 開始編譯驗證 ===" -ForegroundColor Cyan
    Set-Location $ProjectPath
    if (Test-Path ".\\COMPILE_VERIFY.cmd") {
        & ".\\COMPILE_VERIFY.cmd"
    } else {
        Write-Host "COMPILE_VERIFY.cmd 不存在" -ForegroundColor Red
    }
}

Write-Host "`n完整除錯清單: docs\\AUTOMATED_COMPILATION_DEBUG_CHECKLIST.md" -ForegroundColor Cyan
'''

# Write with UTF-8 BOM
output_path = r'C:\HW\MingGoRTS\Tools\build\AutoCompileDebugger.ps1'
with open(output_path, 'w', encoding='utf-8-sig') as f:
    f.write(script_content)

print(f"Created: {output_path}")
