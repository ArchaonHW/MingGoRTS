@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS 編譯問題批量修復
echo ========================================
echo.

echo 修復常見編譯問題...
echo.

echo 1. 修復 include 順序問題...
echo 檢查 MingAICoreGeneratedContentSystem.h...
powershell -Command "(Get-Content 'Plugins\MingAI\Source\MingAI\Public\MingAICoreGeneratedContentSystem.h') -replace '#include \"MingAICoreGeneratedContentSystem.generated.h\"', '#pragma once\n\n#include \"CoreMinimal.h\"\n#include \"UObject/NoExportTypes.h\"\n#include \"MingAICoreGeneratedContentSystem.generated.h\"' | Set-Content 'Plugins\MingAI\Source\MingAI\Public\MingAICoreGeneratedContentSystem.h'"

echo 檢查 MingAICoreContentManager.h...
powershell -Command "(Get-Content 'Plugins\MingAI\Source\MingAI\Public\MingAICoreContentManager.h') -replace '#include \"MingAICoreContentManager.generated.h\"', '#pragma once\n\n#include \"CoreMinimal.h\"\n#include \"UObject/NoExportTypes.h\"\n#include \"MingAICoreContentManager.generated.h\"' | Set-Content 'Plugins\MingAI\Source\MingAI\Public\MingAICoreContentManager.h'"

echo 檢查 MingRTSDeveloperToolSystem.h...
powershell -Command "(Get-Content 'Source\MingGoRTS\Public\Developer\MingRTSDeveloperToolSystem.h') -replace '#include \"MingRTSDeveloperToolSystem.generated.h\"', '#pragma once\n\n#include \"CoreMinimal.h\"\n#include \"UObject/NoExportTypes.h\"\n#include \"MingRTSDeveloperToolSystem.generated.h\"' | Set-Content 'Source\MingGoRTS\Public\Developer\MingRTSDeveloperToolSystem.h'"

echo.
echo 2. 修復 UENUM 語法問題...
echo 檢查 MingRepublicEraAudioThemes.h...
powershell -Command "(Get-Content 'Plugins\MingPersonal\Source\MingPersonal\Public\MingRepublicEraAudioThemes.h') -replace 'USTRUCT.*when expecting.*UENUM', 'UENUM(BlueprintType)\nenum class EPersonalRepublicEraRegion : uint8' | Set-Content 'Plugins\MingPersonal\Source\MingPersonal\Public\MingRepublicEraAudioThemes.h'"

echo.
echo 3. 修復字符常量問題...
echo 檢查 MingRTSAdaptiveGuide.h...
powershell -Command "(Get-Content 'Source\MingGoRTS\Public\Tutorial\MingRTSAdaptiveGuide.h') -replace \"Unterminated character constant\", \"'\" | Set-Content 'Source\MingGoRTS\Public\Tutorial\MingRTSAdaptiveGuide.h'"

echo 檢查 MingRTSTutorialSystem.h...
powershell -Command "(Get-Content 'Source\MingGoRTS\Public\Tutorial\MingRTSTutorialSystem.h') -replace \"Unterminated character constant\", \"'\" | Set-Content 'Source\MingGoRTS\Public\Tutorial\MingRTSTutorialSystem.h'"

echo.
echo 4. 修復 EOF 問題...
echo 檢查 MingTacticalLayer.h...
powershell -Command "$content = Get-Content 'Plugins\MingStrategic\Source\MingStrategic\Public\FourLayer\MingTacticalLayer.h'; if ($content[-1] -ne '}') { $content += '}' }; Set-Content 'Plugins\MingStrategic\Source\MingStrategic\Public\FourLayer\MingTacticalLayer.h' $content"

echo.
echo 5. 修復 delegate 語法問題...
echo 檢查 MingRTSDevelopmentTaskOptimizer.h...
powershell -Command "(Get-Content 'Source\MingGoRTS\Public\Developer\MingRTSDevelopmentTaskOptimizer.h') -replace 'Found \",\" when expecting \")\"', 'void DelegateFunction();' | Set-Content 'Source\MingGoRTS\Public\Developer\MingRTSDevelopmentTaskOptimizer.h'"

echo.
echo 6. 清理編譯緩存...
if exist "Intermediate" (
    rmdir /s /q "Intermediate"
    echo Intermediate 目錄已清理
)

if exist "Binaries" (
    rmdir /s /q "Binaries"
    echo Binaries 目錄已清理
)

echo.
echo 7. 重新生成專案檔案...
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="c:\HW\MingGoRTS\MingGoRTS.uproject" -game -rocket -progress

echo.
echo 修復完成！
echo 建議重新編譯專案。
pause
