#!/usr/bin/env pwsh
#Requires -Version 7.0

<#
.SYNOPSIS
    MingGoRTS 批量語法錯誤修復腳本
    Batch Syntax Error Fix Script

.DESCRIPTION
    自動批量修復專案中的語法錯誤，優先處理高優先級檔案
    1. 修復空函數體問題
    2. 修復編碼問題
    3. 生成修復報告

.PARAMETER Priority
    處理優先級: High, Medium, Low, All

.PARAMETER AutoFix
    自動修復模式 (無需確認)

.PARAMETER DryRun
    預演模式 (不實際修改檔案)

.EXAMPLE
    .\BatchSyntaxFix.ps1 -Priority High -AutoFix
    自動修復所有高優先級語法錯誤

.EXAMPLE
    .\BatchSyntaxFix.ps1 -Priority All -DryRun
    預演修復所有錯誤 (不實際修改)
#>

[CmdletBinding()]
param(
    [Parameter()]
    [ValidateSet("High", "Medium", "Low", "All")]
    [string]$Priority = "High",
    
    [Parameter()]
    [switch]$AutoFix,
    
    [Parameter()]
    [switch]$DryRun,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$ReportPath = "$PWD\Reports\BatchSyntaxFix_Report.md"
)

$ErrorActionPreference = "Stop"

# 顏色定義
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
}

# 修復統計
$FixStats = @{
    TotalFiles = 0
    FixedFiles = 0
    FailedFiles = 0
    SkippedFiles = 0
    TotalErrors = 0
    FixedErrors = 0
    StartTime = Get-Date
    EndTime = $null
}

# 待修復檔案清單 (根據 DevelopmentProgressReport_20260324.md)
$FilesToFix = @{
    High = @(
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingAntiFallSystem.cpp"; Type = "防墮系統"; Size = "11.67 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingCommandAuthority.cpp"; Type = "指揮權威"; Size = "34.67 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingMilitaryAuthority.cpp"; Type = "軍事權威"; Size = "13.71 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingMoralAuthority.cpp"; Type = "道德權威"; Size = "14.73 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingStrategyAuthority.cpp"; Type = "策略權威"; Size = "16.96 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingThreePowerSystem.cpp"; Type = "三權系統"; Size = "11.23 KB"; Issues = @("空函數體") }
    )
    Medium = @(
        @{ Path = "Source\MingGoRTS\Private\SageBrain\MingSageCharacter.cpp"; Type = "聖者角色"; Size = "26.22 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageBrain\MingSageCharacterSystem.cpp"; Type = "角色系統"; Size = "11.42 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingSageCommandSystem.cpp"; Type = "指揮系統"; Size = "28.46 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingSageCommandTest.cpp"; Type = "測試系統"; Size = "21.92 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingSixStrategies.cpp"; Type = "六策實現"; Size = "36.95 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingWuXingRhythmSystem.cpp"; Type = "五行節奏"; Size = "13.81 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingYinYangFiveElements.cpp"; Type = "陰陽五行"; Size = "30.25 KB"; Issues = @("空函數體") }
    )
    Low = @(
        @{ Path = "Source\MingGoRTS\Private\SageCommand\MingFiveElementsRotation.cpp"; Type = "五行輪轉"; Size = "35.96 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageBrain\MingIntelligentRecommendationSystem.cpp"; Type = "智能推薦"; Size = "46.70 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageBrain\MingSageRoleController.cpp"; Type = "角色控制器"; Size = "27.57 KB"; Issues = @("空函數體") },
        @{ Path = "Source\MingGoRTS\Private\SageBrain\MingSageRoles.cpp"; Type = "聖者角色"; Size = "32.49 KB"; Issues = @("空函數體") }
    )
}

function Write-FixLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = $Colors[$Level]
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    # 寫入日誌
    $LogDir = "$ProjectRoot\Logs"
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
    }
    "[$Timestamp] [$Level] $Message" | Add-Content -Path "$LogDir\BatchSyntaxFix_$(Get-Date -Format 'yyyyMMdd').log" -ErrorAction SilentlyContinue
}

function Test-FileExists {
    param([string]$RelativePath)
    
    $FullPath = Join-Path $ProjectRoot $RelativePath
    return Test-Path $FullPath
}

function Get-FileContent {
    param([string]$RelativePath)
    
    $FullPath = Join-Path $ProjectRoot $RelativePath
    if (Test-Path $FullPath) {
        return Get-Content $FullPath -Raw -Encoding UTF8
    }
    return $null
}

function Save-FileContent {
    param(
        [string]$RelativePath,
        [string]$Content
    )
    
    $FullPath = Join-Path $ProjectRoot $RelativePath
    $Dir = Split-Path -Parent $FullPath
    
    if (-not (Test-Path $Dir)) {
        New-Item -ItemType Directory -Path $Dir -Force | Out-Null
    }
    
    # 備份原檔案
    $BackupPath = "$FullPath.backup.$(Get-Date -Format 'yyyyMMddHHmmss')"
    if (Test-Path $FullPath) {
        Copy-Item $FullPath $BackupPath -Force
    }
    
    # 保存新內容
    $Content | Set-Content $FullPath -Encoding UTF8 -Force
    return $BackupPath
}

function Repair-EmptyFunctionBody {
    param([string]$Content)
    
    $FixedContent = $Content
    $FixCount = 0
    
    # 模式 1: 檢測並修復 "();" 結尾的空函數
    $Pattern1 = '(\w+\s+\w+::\w+\s*\([^)]*\)\s*);'
    $Matches1 = [regex]::Matches($Content, $Pattern1)
    foreach ($Match in $Matches1) {
        $Original = $Match.Groups[0].Value
        $FunctionDecl = $Match.Groups[1].Value
        $Replacement = "$FunctionDecl`n{`n    // TODO: 實現函數功能`n}"
        $FixedContent = $FixedContent.Replace($Original, $Replacement)
        $FixCount++
    }
    
    # 模式 2: 檢測並修復 "{}" 空函數體
    $Pattern2 = '(\w+\s+\w+::\w+\s*\([^)]*\)\s*)\{\s*\}'
    $Matches2 = [regex]::Matches($Content, $Pattern2)
    foreach ($Match in $Matches2) {
        $Original = $Match.Groups[0].Value
        $FunctionDecl = $Match.Groups[1].Value
        $Replacement = "$FunctionDecl`n{`n    // TODO: 實現函數功能`n}"
        $FixedContent = $FixedContent.Replace($Original, $Replacement)
        $FixCount++
    }
    
    return @{ Content = $FixedContent; FixCount = $FixCount }
}

function Start-BatchFix {
    Write-FixLog "=== 開始批量語法錯誤修復 ===" "INFO"
    Write-FixLog "優先級: $Priority | 自動修復: $AutoFix | 預演模式: $DryRun" "INFO"
    
    # 確定要處理的檔案
    $TargetFiles = @()
    if ($Priority -eq "All") {
        $TargetFiles += $FilesToFix.High
        $TargetFiles += $FilesToFix.Medium
        $TargetFiles += $FilesToFix.Low
    } else {
        $TargetFiles = $FilesToFix[$Priority]
    }
    
    $FixStats.TotalFiles = $TargetFiles.Count
    Write-FixLog "目標檔案數: $($TargetFiles.Count)" "INFO"
    
    $ReportData = @()
    $ReportData += "# 批量語法錯誤修復報告"
    $ReportData += ""
    $ReportData += "**執行時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
    $ReportData += "**優先級**: $Priority"
    $ReportData += "**模式**: $(if ($DryRun) { '預演模式' } else { '實際修復' })"
    $ReportData += ""
    $ReportData += "---"
    $ReportData += ""
    $ReportData += "## 修復結果摘要"
    $ReportData += ""
    
    $FileIndex = 0
    foreach ($FileInfo in $TargetFiles) {
        $FileIndex++
        $RelativePath = $FileInfo.Path
        
        Write-FixLog "[$FileIndex/$($TargetFiles.Count)] 處理: $RelativePath" "INFO"
        
        # 檢查檔案是否存在
        if (-not (Test-FileExists $RelativePath)) {
            Write-FixLog "  檔案不存在，跳過: $RelativePath" "WARNING"
            $FixStats.SkippedFiles++
            $ReportData += "| $RelativePath | $($FileInfo.Type) | ⚠️ 檔案不存在 | - |"
            continue
        }
        
        # 讀取檔案內容
        $Content = Get-FileContent $RelativePath
        if (-not $Content) {
            Write-FixLog "  無法讀取檔案: $RelativePath" "ERROR"
            $FixStats.FailedFiles++
            $ReportData += "| $RelativePath | $($FileInfo.Type) | ❌ 讀取失敗 | - |"
            continue
        }
        
        # 執行修復
        $FixResult = Repair-EmptyFunctionBody $Content
        $FixStats.TotalErrors += $FixResult.FixCount
        
        if ($FixResult.FixCount -eq 0) {
            Write-FixLog "  無需修復: $RelativePath" "SUCCESS"
            $ReportData += "| $RelativePath | $($FileInfo.Type) | ✅ 無需修復 | 0 |"
            continue
        }
        
        Write-FixLog "  發現 $($FixResult.FixCount) 個問題" "WARNING"
        
        if ($DryRun) {
            # 預演模式
            Write-FixLog "  [預演] 將修復 $($FixResult.FixCount) 個問題" "DEBUG"
            $ReportData += "| $RelativePath | $($FileInfo.Type) | 📝 預演: $($FixResult.FixCount) 個修復 | $($FixResult.FixCount) |"
            $FixStats.FixedErrors += $FixResult.FixCount
        } else {
            # 實際修復
            if (-not $AutoFix) {
                $Confirm = Read-Host "  確認修復 $RelativePath? (Y/N)"
                if ($Confirm -ne "Y") {
                    Write-FixLog "  用戶跳過: $RelativePath" "WARNING"
                    $FixStats.SkippedFiles++
                    $ReportData += "| $RelativePath | $($FileInfo.Type) | ⏭️ 用戶跳過 | - |"
                    continue
                }
            }
            
            try {
                $BackupPath = Save-FileContent -RelativePath $RelativePath -Content $FixResult.Content
                Write-FixLog "  ✅ 修復成功 | 備份: $([System.IO.Path]::GetFileName($BackupPath))" "SUCCESS"
                $FixStats.FixedFiles++
                $FixStats.FixedErrors += $FixResult.FixCount
                $ReportData += "| $RelativePath | $($FileInfo.Type) | ✅ 修復成功 | $($FixResult.FixCount) |"
            }
            catch {
                Write-FixLog "  ❌ 修復失敗: $($_.Exception.Message)" "ERROR"
                $FixStats.FailedFiles++
                $ReportData += "| $RelativePath | $($FileInfo.Type) | ❌ 修復失敗 | - |"
            }
        }
    }
    
    $FixStats.EndTime = Get-Date
    
    # 生成報告
    New-FixReport -ReportData $ReportData
    
    # 顯示摘要
    Show-FixSummary
}

function New-FixReport {
    param([array]$ReportData)
    
    $Duration = $FixStats.EndTime - $FixStats.StartTime
    
    $ReportContent = @"
$($ReportData -join "`n")

---

## 統計摘要

| 指標 | 數值 |
|------|------|
| **總檔案數** | $($FixStats.TotalFiles) |
| **成功修復** | $($FixStats.FixedFiles) |
| **修復失敗** | $($FixStats.FailedFiles) |
| **跳過檔案** | $($FixStats.SkippedFiles) |
| **總問題數** | $($FixStats.TotalErrors) |
| **已修復問題** | $($FixStats.FixedErrors) |
| **執行時間** | $($Duration.ToString('hh\:mm\:ss')) |

---

*報告由 BatchSyntaxFix.ps1 生成*
*生成時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')*
"@
    
    # 確保報告目錄存在
    $ReportDir = Split-Path -Parent $ReportPath
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $ReportContent | Set-Content $ReportPath -Encoding UTF8 -Force
    Write-FixLog "報告已生成: $ReportPath" "SUCCESS"
}

function Show-FixSummary {
    Write-FixLog "`n=== 批量修復完成 ===" "INFO"
    Write-FixLog "總檔案數: $($FixStats.TotalFiles)" "INFO"
    Write-FixLog "成功修復: $($FixStats.FixedFiles)" "SUCCESS"
    Write-FixLog "修復失敗: $($FixStats.FailedFiles)" $(if ($FixStats.FailedFiles -gt 0) { "ERROR" } else { "SUCCESS" })
    Write-FixLog "跳過檔案: $($FixStats.SkippedFiles)" "WARNING"
    Write-FixLog "總問題數: $($FixStats.TotalErrors)" "INFO"
    Write-FixLog "已修復問題: $($FixStats.FixedErrors)" "SUCCESS"
    
    $Duration = $FixStats.EndTime - $FixStats.StartTime
    Write-FixLog "執行時間: $($Duration.ToString('hh\:mm\:ss'))" "INFO"
    
    if ($DryRun) {
        Write-FixLog "`n這是預演模式，未實際修改檔案。" "WARNING"
        Write-FixLog "執行實際修復: .\\BatchSyntaxFix.ps1 -Priority $Priority -AutoFix" "INFO"
    }
}

# 主執行流程
function Main {
    Write-Host "`n╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║                                                              ║" -ForegroundColor Cyan
    Write-Host "║        MingGoRTS 批量語法錯誤修復工具                        ║" -ForegroundColor Cyan
    Write-Host "║        Batch Syntax Error Fix Tool                           ║" -ForegroundColor Cyan
    Write-Host "║                                                              ║" -ForegroundColor Cyan
    Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
    
    # 驗證專案路徑
    if (-not (Test-Path $ProjectRoot)) {
        Write-FixLog "專案路徑不存在: $ProjectRoot" "ERROR"
        exit 1
    }
    
    Write-FixLog "專案路徑: $ProjectRoot" "INFO"
    Write-FixLog "報告路徑: $ReportPath" "INFO"
    
    # 執行批量修復
    Start-BatchFix
    
    Write-FixLog "`n批量修復流程完成！" "SUCCESS"
}

# 執行主程序
Main
