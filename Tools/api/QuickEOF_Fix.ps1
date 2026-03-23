# Quick EOF Fix Script
param([switch]$DryRun = $false)

$files = @(
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

Write-Host "Starting EOF fix for $($files.Count) files..." -ForegroundColor Green

$fixed = 0
$total = $files.Count

foreach ($file in $files) {
    $fullPath = Join-Path $pwd $file
    
    if (Test-Path $fullPath) {
        $content = Get-Content $fullPath -Raw
        
        if ($content -and $content -notmatch '#endif') {
            $fileName = [System.IO.Path]::GetFileNameWithoutExtension($file)
            $guardName = 'MING_' + $fileName.ToUpper() + '_H'
            
            $fix = "`r`n`r`n#endif // $guardName"
            
            if ($DryRun) {
                Write-Host "DRY RUN: Would fix $file" -ForegroundColor Yellow
            } else {
                Add-Content $fullPath $fix -Encoding UTF8
                Write-Host "Fixed: $file" -ForegroundColor Green
                $fixed++
            }
        } else {
            Write-Host "OK: $file" -ForegroundColor Gray
        }
    } else {
        Write-Host "Missing: $file" -ForegroundColor Red
    }
}

$rate = if ($total -gt 0) { [math]::Round(($fixed / $total) * 100, 1) } else { 0 }
Write-Host "Done: $fixed/$total files fixed ($rate%)" -ForegroundColor Cyan
