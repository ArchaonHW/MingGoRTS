# MingGoRTS 聖者指導EOF錯誤修復腳本 (簡化版)
# 智慧化編譯錯誤修復系統

param(
    [switch]$DryRun = $false
)

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

Write-Host "🧠 聖者指導EOF錯誤修復開始..." -ForegroundColor Green
Write-Host "📁 總計 $($EOFFiles.Count) 個文件需要處理" -ForegroundColor Cyan

$FixedCount = 0
$TotalCount = $EOFFiles.Count

foreach ($file in $EOFFiles) {
    $fullPath = Join-Path $pwd $file
    
    if (Test-Path $fullPath) {
        try {
            $content = Get-Content $fullPath -Raw -Encoding UTF8
            
            if ($content -and $content -notmatch '#endif') {
                $fileName = [System.IO.Path]::GetFileNameWithoutExtension($file)
                $guardName = 'MING_' + $fileName.ToUpper() + '_H'
                
                $fixContent = "`r`n`r`n#endif // $guardName"
                
                if ($DryRun) {
                    Write-Host "DRY RUN: 將修復 $file (添加 #endif // $guardName)" -ForegroundColor Yellow
                } else {
                    Add-Content $fullPath $fixContent -Encoding UTF8
                    Write-Host "✅ 修復 $file - 聖者智慧應用" -ForegroundColor Green
                    $FixedCount++
                }
            } else {
                Write-Host "ℹ️  $file - 無需修復" -ForegroundColor Gray
            }
        } catch {
            Write-Host "❌ 處理 $file 時出錯: $($_.Exception.Message)" -ForegroundColor Red
        }
    } else {
        Write-Host "❌ 文件不存在: $file" -ForegroundColor Red
    }
}

$SuccessRate = if ($TotalCount -gt 0) { [math]::Round(($FixedCount / $TotalCount) * 100, 1) } else { 0 }
Write-Host "`n🎯 聖者EOF修復完成: $FixedCount/$TotalCount 個文件修復 ($SuccessRate%)" -ForegroundColor Cyan

if (-not $DryRun -and $FixedCount -gt 0) {
    Write-Host "🧠 聖者智慧已應用，代碼完整性得到提升" -ForegroundColor Green
}
