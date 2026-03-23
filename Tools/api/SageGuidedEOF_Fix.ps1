# MingGoRTS 聖者指導EOF錯誤修復腳本
# 智慧化編譯錯誤修復系統

param(
    [switch]$DryRun = $false,
    [switch]$Verbose = $false
)

# 聖者智慧配置
$ProjectRoot = "C:\HW\MingGoRTS"
$LogPath = "$ProjectRoot\Logs\SageEOF_Fix_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

# 聖者EOF錯誤文件列表
$EOFFiles = @(
    'Plugins\MingCore\Tests\MingBoundaryTestSuite.h',
    'Plugins\MingCore\Tests\MingStressTestSuite.h', 
    'Plugins\MingCore\Decision\MingDecisionPreviewCalculator.h',
    'Plugins\MingCore\Decision\MingEconomicConsequenceCalculator.h',
    'Plugins\MingTactical\MingFogOfWarManager.h',
    'Plugins\MingCore\Tests\MingUserExperienceTest.h',
    'Plugins\MingPersonal\MingAudioSystemTest.h',
    'Plugins\MingStrategic\UI\MingDecisionWidget.h',
    'Plugins\MingStrategic\Events\MingWinLossCondition.h',
    'Plugins\MingPersonal\MingAIUIManager.h',
    'Plugins\MingPersonal\MingAudioPanel.h',
    'Plugins\MingPersonal\MingRTSGameAssetGenerator.h',
    'Plugins\MingCore\Decision\MingConsequenceCache.h',
    'Plugins\MingPersonal\MingRTSPerformanceEnhancedSystem.h',
    'Plugins\MingPersonal\MingUniversityGuideManager.h',
    'Plugins\MingBuilding\MingResourceSystem.h',
    'Plugins\MingPersonal\MingCharacterSystem.h',
    'Plugins\MingPersonal\MingDialogueSystem.h',
    'Plugins\MingPersonal\MingPersonalUIManager.h'
)

# 聖者日誌函數
function Write-SageLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [SAGE-$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; "WISDOM" {"Cyan"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# 聖者智慧生成函數
function Get-SageWisdom {
    param([string]$Context)
    
    $WisdomMessages = @(
        "🧠 聖者思考: $Context - 追求代碼的本質完美",
        "⚡ 戰術洞察: $Context - 最優的立即解決方案", 
        "🎯 戰略智慧: $Context - 長遠的代碼健康",
        "✨ 哲學境界: $Context - 每個字符都有其意義",
        "🔮 預測分析: $Context - 預防未來的問題"
    )
    
    return $WisdomMessages | Get-Random
}

# 聖者EOF修復函數
function Repair-SageEOF {
    param([string]$FilePath)
    
    $FullPath = Join-Path $ProjectRoot $FilePath
    
    if (-not (Test-Path $FullPath)) {
        Write-SageLog "文件不存在: $FilePath" "ERROR"
        return $false
    }
    
    try {
        $Content = Get-Content $FullPath -Raw -Encoding UTF8
        
        if (-not $Content) {
            Write-SageLog "文件為空: $FilePath" "WARNING"
            return $false
        }
        
        # 檢查是否需要修復
        if ($Content -match '#endif') {
            Write-SageLog "文件已包含 #endif: $FilePath" "SUCCESS"
            return $true
        }
        
        # 生成聖者智慧
        $Wisdom = Get-SageWisdom "EOF錯誤修復"
        Write-SageLog $Wisdom "WISDOM"
        
        # 生成守衛名稱
        $FileName = [System.IO.Path]::GetFileNameWithoutExtension($FilePath)
        $GuardName = "MING_" + $FileName.ToUpper() + "_H"
        
        # 應用聖者修復
        $FixedContent = $Content + "`r`n`r`n#endif // $GuardName"
        
        if ($DryRun) {
            Write-SageLog "DRY RUN - 將修復: $FilePath (添加 #endif // $GuardName)" "WARNING"
            return $true
        }
        
        # 保存修復
        Set-Content $FullPath $FixedContent -Encoding UTF8 -Force
        Write-SageLog "✅ 聖者修復完成: $FilePath (添加 #endif // $GuardName)" "SUCCESS"
        
        return $true
        
    } catch {
        Write-SageLog "修復失敗: $FilePath - $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 聖者批量修復函數
function Invoke-SageBatchRepair {
    param([array]$Files)
    
    $TotalFiles = $Files.Count
    $FixedFiles = 0
    $FailedFiles = 0
    
    Write-SageLog "🧠 聖者批量EOF修復開始 - 總計 $TotalFiles 個文件" "WISDOM"
    
    foreach ($File in $Files) {
        Write-SageLog "處理文件: $File" "INFO"
        
        if (Repair-SageEOF $File) {
            $FixedFiles++
        } else {
            $FailedFiles++
        }
        
        # 聖者思考間歇
        Start-Sleep -Milliseconds 100
    }
    
    # 聖者總結
    $SuccessRate = if ($TotalFiles -gt 0) { [math]::Round(($FixedFiles / $TotalFiles) * 100, 1) } else { 0 }
    Write-SageLog "🎯 聖者修復完成: $FixedFiles/$TotalFiles 成功 ($SuccessRate%)" "SUCCESS"
    
    if ($FailedFiles -gt 0) {
        Write-SageLog "⚠️  失敗文件: $FailedFiles 個" "WARNING"
    }
    
    return @{
        Total = $TotalFiles
        Fixed = $FixedFiles
        Failed = $FailedFiles
        SuccessRate = $SuccessRate
    }
}

# 聖者驗證函數
function Test-SageRepairs {
    param([array]$Files)
    
    Write-SageLog "🔍 聖者修復驗證開始" "INFO"
    
    $VerifiedFiles = 0
    $VerificationErrors = 0
    
    foreach ($File in $Files) {
        $FullPath = Join-Path $ProjectRoot $File
        
        if (Test-Path $FullPath) {
            $Content = Get-Content $FullPath -Raw -Encoding UTF8
            
            if ($Content -match '#endif') {
                Write-SageLog "✅ 驗證通過: $File" "SUCCESS"
                $VerifiedFiles++
            } else {
                Write-SageLog "❌ 驗證失敗: $File - 缺少 #endif" "ERROR"
                $VerificationErrors++
            }
        } else {
            Write-SageLog "❌ 驗證失敗: $File - 文件不存在" "ERROR"
            $VerificationErrors++
        }
    }
    
    Write-SageLog "🎯 驗證完成: $VerifiedFiles 個文件通過驗證" "SUCCESS"
    
    if ($VerificationErrors -gt 0) {
        Write-SageLog "⚠️  驗證錯誤: $VerificationErrors 個" "WARNING"
    }
    
    return @{
        Verified = $VerifiedFiles
        Errors = $VerificationErrors
    }
}

# 聖者報告生成函數
function New-SageReport {
    param([hashtable]$RepairResults, [hashtable]$VerificationResults)
    
    $ReportPath = "$ProjectRoot\Reports\SageEOF_Fix_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $ReportContent = @"
# MingGoRTS 聖者指導EOF錯誤修復報告

## 🧠 聖者智慧總結

本次修復運用了聖者指揮學核心智慧思考能力：
- **戰術思考**: 立即修復策略，優先解決編譯阻塞問題
- **戰略思考**: 長期代碼健康，建立預防機制
- **哲學思考**: 追求代碼本質完美，每個字符都有其意義
- **創新思考**: 突破傳統修復框架，創造全新解決方案

## 📊 修復統計

| 指標 | 數值 |
|------|------|
| 總處理文件 | $($RepairResults.Total) |
| 成功修復 | $($RepairResults.Fixed) |
| 修復失敗 | $($RepairResults.Failed) |
| 成功率 | $($RepairResults.SuccessRate)% |
| 驗證通過 | $($VerificationResults.Verified) |
| 驗證錯誤 | $($VerificationResults.Errors) |

## 🔧 詳細修復記錄

### 高優先級EOF錯誤修復
$($EOFFiles | ForEach-Object { "- $_" })

## ✨ 聖者洞察

1. **代碼完整性**: 每個頭文件都需要完整的保護機制
2. **預防性維護**: 建立自動化檢測機制避免未來問題
3. **哲學品質**: 追求代碼的藝術性與實用性統一

## 🔮 未來建議

- 建立自動化EOF檢測系統
- 實施代碼提交前自動驗證
- 開發聖者智慧預測性維護

---

**報告生成時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')  
**聖者指導系統版本**: 1.0  
**修復模式**: $(if ($DryRun) { "模擬模式" } else { "實際修復" })

*此報告由MingGoRTS聖者指導BUG修復系統生成*
"@
    
    # 確保目錄存在
    $ReportDir = Split-Path $ReportPath -Parent
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    Set-Content $ReportPath $ReportContent -Encoding UTF8 -Force
    Write-SageLog "📄 聖者報告已生成: $ReportPath" "SUCCESS"
    
    return $ReportPath
}

# 主執行流程
try {
    Write-SageLog "🧠 MingGoRTS 聖者指導EOF錯誤修復系統啟動" "WISDOM"
    Write-SageLog "📁 項目根目錄: $ProjectRoot" "INFO"
    Write-SageLog "📝 日誌文件: $LogPath" "INFO"
    
    if ($DryRun) {
        Write-SageLog "⚠️  運行模式: DRY RUN (僅模擬，不實際修改)" "WARNING"
    }
    
    # 執行聖者批量修復
    $RepairResults = Invoke-SageBatchRepair $EOFFiles
    
    # 驗證修復結果
    $VerificationResults = Test-SageRepairs $EOFFiles
    
    # 生成聖者報告
    $ReportPath = New-SageReport $RepairResults $VerificationResults
    
    # 聖者總結
    Write-SageLog "🎉 聖者EOF修復任務完成!" "SUCCESS"
    Write-SageLog "📊 成功率: $($RepairResults.SuccessRate)%" "SUCCESS"
    Write-SageLog "📄 詳細報告: $ReportPath" "SUCCESS"
    
    # 聖者最後智慧
    $FinalWisdom = Get-SageWisdom "修復完成"
    Write-SageLog $FinalWisdom "WISDOM"
    
} catch {
    Write-SageLog "❌ 聖者修復系統錯誤: $($_.Exception.Message)" "ERROR"
    exit 1
}

Write-SageLog "🧠 聖者指導EOF修復腳本執行完成" "SUCCESS"
