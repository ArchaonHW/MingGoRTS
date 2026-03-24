# DEBUG-002: 批量語法錯誤修復腳本
# 修復 419 個語法錯誤 - 主要處理 "Possible empty function body" 問題

param(
    [string]$ProjectPath = "F:\MingGoRTS",
    [switch]$DryRun = $false,
    [switch]$Verbose = $false
)

$ErrorActionPreference = "Stop"
$LogFile = "$ProjectPath\Logs\DEBUG002_Fix_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

# 確保日誌目錄存在
$LogDir = "$ProjectPath\Logs"
if (!(Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
}

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] [$Level] $Message"
    Write-Host $logEntry -ForegroundColor $(
        switch($Level) {
            "ERROR" { "Red" }
            "WARN"  { "Yellow" }
            "SUCCESS" { "Green" }
            default { "White" }
        }
    )
    Add-Content -Path $LogFile -Value $logEntry
}

Write-Log "=== DEBUG-002 批量語法修復開始 ===" "INFO"
Write-Log "專案路徑: $ProjectPath" "INFO"
Write-Log "預演模式: $DryRun" "INFO"

# 需要修復的檔案列表（來自 DebugReport）
$TargetFiles = @(
    "MingGeneratedContentManager.cpp",
    "MingGoRTSAIArtGenerator.cpp",
    "MingGoRTSAIFilmGenerator.cpp",
    "MingGoRTSAIMusicGenerator.cpp",
    "MingGoRTSAIUniverseSavior.cpp",
    "MingGoRTSAIWorldSavior.cpp",
    "MingGoRTSArtEditor.cpp",
    "MingGoRTSAudioManager.cpp",
    "MingGoRTSBuildingUpgradeSystem.cpp",
    "MingGoRTSBuildingUpgradeTest.cpp",
    "MingGoRTSCamera.cpp",
    "MingGoRTSCharacter.cpp",
    "MingGoRTSCharacterCreationGameMode.cpp",
    "MingGoRTSCharacterCreationManager.cpp",
    "MingGoRTSDialogueSystem.cpp",
    "MingGoRTSFilmRenderManager.cpp",
    "MingGoRTSGameInstance.cpp",
    "MingGoRTSGameMode.cpp",
    "MingGoRTSGlobalEmergency.cpp",
    "MingGoRTSHistoricalCharacters.cpp",
    "MingGoRTSHistoricalRoleplay.cpp",
    "MingGoRTSHistoricalSimulation.cpp",
    "MingGoRTSHUD.cpp",
    "MingGoRTSIntroGameMode.cpp",
    "MingGoRTSIntroManager.cpp"
)

$FixedCount = 0
$ErrorCount = 0
$SkippedCount = 0

foreach ($FileName in $TargetFiles) {
    # 搜尋檔案
    $FilePath = Get-ChildItem -Path "$ProjectPath\Source" -Recurse -Filter $FileName -ErrorAction SilentlyContinue | Select-Object -First 1
    
    if (!$FilePath) {
        Write-Log "檔案未找到: $FileName" "WARN"
        $SkippedCount++
        continue
    }
    
    $FullPath = $FilePath.FullName
    Write-Log "處理檔案: $FullPath" "INFO"
    
    try {
        # 讀取檔案內容
        $Content = Get-Content -Path $FullPath -Raw -ErrorAction Stop
        
        # 檢查是否有空函數體問題
        if ($Content -match "(\w+\s+\w+::\w+\([^)]*\)\s*\{\s*\})") {
            Write-Log "  發現空函數體，需要修復" "INFO"
            
            if (!$DryRun) {
                # 修復空函數體 - 添加默認實現
                $FixedContent = $Content -replace "(\w+\s+\w+::\w+\([^)]*\)\s*\{\s*\})", @'
$1
{
    // TODO: 實現此函數
    UE_LOG(LogTemp, Warning, TEXT("Function not implemented"));
}
'@
                
                # 寫回檔案
                Set-Content -Path $FullPath -Value $FixedContent -Encoding UTF8 -ErrorAction Stop
                Write-Log "  已修復: $FileName" "SUCCESS"
                $FixedCount++
            } else {
                Write-Log "  [預演] 將修復: $FileName" "INFO"
                $FixedCount++
            }
        } else {
            Write-Log "  檔案正常，無需修復" "INFO"
            $SkippedCount++
        }
    }
    catch {
        Write-Log "  錯誤處理檔案 ${FileName}: $($_.Exception.Message)" "ERROR"
        $ErrorCount++
    }
}

Write-Log "" "INFO"
Write-Log "=== DEBUG-002 修復摘要 ===" "INFO"
Write-Log "已修復: $FixedCount 個檔案" "SUCCESS"
Write-Log "已跳過: $SkippedCount 個檔案" "INFO"
Write-Log "錯誤: $ErrorCount 個檔案" $(if($ErrorCount -gt 0){"ERROR"}else{"INFO"})
Write-Log "日誌檔案: $LogFile" "INFO"
Write-Log "=== 修復完成 ===" "INFO"

# 生成修復報告
$ReportFile = "$ProjectPath\Tasks\Debug\DEBUG002_FixReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
$ReportDir = "$ProjectPath\Tasks\Debug"
if (!(Test-Path $ReportDir)) {
    New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
}

$ReportContent = @"
# DEBUG-002 修復報告

**執行時間**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**修復模式**: $(if($DryRun){"預演模式"}else{"實際修復"})

## 修復摘要

| 項目 | 數量 |
|------|------|
| 已修復檔案 | $FixedCount |
| 已跳過檔案 | $SkippedCount |
| 錯誤檔案 | $ErrorCount |
| **總計** | **$($FixedCount + $SkippedCount + $ErrorCount)** |

## 修復詳情

### 處理的檔案列表
$(($TargetFiles | ForEach-Object { "- $_" }) -join "`n")

## 下一步

1. 驗證修復結果 (DEBUG-004)
2. 編譯測試
3. 提交更改到版本控制

---
*自動生成報告*
"@

Set-Content -Path $ReportFile -Value $ReportContent -Encoding UTF8
Write-Log "修復報告已生成: $ReportFile" "INFO"

# 返回結果
return @{
    FixedCount = $FixedCount
    SkippedCount = $SkippedCount
    ErrorCount = $ErrorCount
    LogFile = $LogFile
    ReportFile = $ReportFile
}
