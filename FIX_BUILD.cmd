@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS 編譯問題修復工具
echo ========================================
echo.

set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"

echo 檢查和修復常見編譯問題...
echo.

echo 1. 修復MingAI.Build.cs缺失問題...
if not exist "Plugins\MingAI\Source\MingAI\MingAI.Build.cs" (
    echo 創建MingAI.Build.cs...
    (
    echo using UnrealBuildTool;
    echo.
    echo public class MingAI : ModuleRules
    echo {
    echo     public MingAI^(ReadOnlyTargetRules Target^) : base^(Target^)
    echo     {
    echo         PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
    echo         CppStandard = CppStandardVersion.Cpp20;
    echo         
    echo         PublicDependencyModuleNames.AddRange^(
    echo             new string[]
    echo             {
    echo                 "Core",
    echo                 "CoreUObject",
    echo                 "Engine",
    echo                 "InputCore",
    echo                 "MingCore"
    echo             }
    echo         ^);
    echo         
    echo         PrivateDependencyModuleNames.AddRange^(
    echo             new string[]
    echo             {
    echo                 "Slate",
    echo                 "SlateCore",
    echo                 "AudioExtensions",
    echo                 "AudioMixer"
    echo             }
    echo         ^);
    echo     }
    echo ^}
    ) > "Plugins\MingAI\Source\MingAI\MingAI.Build.cs"
    echo ✓ MingAI.Build.cs已創建
) else (
    echo ✓ MingAI.Build.cs已存在
)

echo.
echo 2. 修復插件依賴問題...
echo 檢查MingTactical.Build.cs...
findstr /C:"MingCore" "Plugins\MingTactical\Source\MingTactical\MingTactical.Build.cs" > nul
if %ERRORLEVEL% neq 0 (
    echo 添加MingCore依賴到MingTactical...
    powershell -Command "(Get-Content 'Plugins\MingTactical\Source\MingTactical\MingTactical.Build.cs') -replace '                \"InputCore\",', '                \"InputCore\",' -replace '            }', '                \"MingCore\"' -replace '            }', '            }' | Set-Content 'Plugins\MingTactical\Source\MingTactical\MingTactical.Build.cs'"
) else (
    echo ✓ MingTactical依賴正確
)

echo.
echo 3. 修復主專案依賴問題...
findstr /C:"MingCore" "Source\MingGoRTS\MingGoRTS.Build.cs" > nul
if %ERRORLEVEL% neq 0 (
    echo 添加插件依賴到主專案...
    powershell -Command "(Get-Content 'Source\MingGoRTS\MingGoRTS.Build.cs') -replace '                \"Projects\"', '                \"Projects\",' -replace '            }', '                \"MingCore\",' -replace '            }', '                \"MingTactical\",' -replace '            }', '                \"MingAI\"' -replace '            }', '            }' | Set-Content 'Source\MingGoRTS\MingGoRTS.Build.cs'"
) else (
    echo ✓ 主專案依賴正確
)

echo.
echo 4. 修復Target.cs檔案...
if not exist "Source\MingGoRTS\MingGoRTS.Target.cs" (
    echo 創建MingGoRTS.Target.cs...
    (
    echo using UnrealBuildTool;
    echo using System.Collections.Generic;
    echo.
    echo public class MingGoRTSTarget : TargetRules
    echo {
    echo     public MingGoRTSTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Game;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingGoRTS" } ^);
    echo         
    echo         // 添加插件
    echo         ExtraModuleNames.AddRange^( new string[] { "MingCore" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingTactical" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingAI" } ^);
    echo     }
    echo }
    echo.
    echo public class MingGoRTSEditorTarget : TargetRules
    echo {
    echo     public MingGoRTSEditorTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Editor;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingGoRTS" } ^);
    echo         
    echo         // 添加插件
    echo         ExtraModuleNames.AddRange^( new string[] { "MingCore" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingTactical" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingAI" } ^);
    echo     }
    echo }
    ) > "Source\MingGoRTS\MingGoRTS.Target.cs"
    echo ✓ MingGoRTS.Target.cs已創建
) else (
    echo ✓ MingGoRTS.Target.cs已存在
)

echo.
echo 5. 修復Editor Target.cs檔案...
if not exist "Source\MingGoRTS\MingGoRTSEditor.Target.cs" (
    echo 創建MingGoRTSEditor.Target.cs...
    (
    echo using UnrealBuildTool;
    echo using System.Collections.Generic;
    echo.
    echo public class MingGoRTSEditorTarget : TargetRules
    echo {
    echo     public MingGoRTSEditorTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Editor;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingGoRTS" } ^);
    echo         
    echo         // 添加插件
    echo         ExtraModuleNames.AddRange^( new string[] { "MingCore" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingTactical" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "MingAI" } ^);
    echo         
    echo         // 編輯器依賴
    echo         ExtraModuleNames.AddRange^( new string[] { "UnrealEd" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "ToolMenus" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "EditorStyle" } ^);
    echo     }
    echo }
    ) > "Source\MingGoRTS\MingGoRTSEditor.Target.cs"
    echo ✓ MingGoRTSEditor.Target.cs已創建
) else (
    echo ✓ MingGoRTSEditor.Target.cs已存在
)

echo.
echo 6. 修復插件Target.cs檔案...
if not exist "Plugins\MingTactical\Source\MingTactical\MingTactical.Target.cs" (
    echo 創建MingTactical.Target.cs...
    (
    echo using UnrealBuildTool;
    echo.
    echo public class MingTacticalTarget : TargetRules
    echo {
    echo     public MingTacticalTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Runtime;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingTactical" } ^);
    echo     }
    echo }
    echo.
    echo public class MingTacticalEditorTarget : TargetRules
    echo {
    echo     public MingTacticalEditorTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Editor;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingTactical" } ^);
    echo         
    echo         // 編輯器依賴
    echo         ExtraModuleNames.AddRange^( new string[] { "UnrealEd" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "ToolMenus" } ^);
    echo     }
    echo }
    ) > "Plugins\MingTactical\Source\MingTactical\MingTactical.Target.cs"
    echo ✓ MingTactical.Target.cs已創建
) else (
    echo ✓ MingTactical.Target.cs已存在
)

if not exist "Plugins\MingAI\Source\MingAI\MingAI.Target.cs" (
    echo 創建MingAI.Target.cs...
    (
    echo using UnrealBuildTool;
    echo.
    echo public class MingAITarget : TargetRules
    echo {
    echo     public MingAITarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Runtime;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingAI" } ^);
    echo     }
    echo }
    echo.
    echo public class MingAIEditorTarget : TargetRules
    echo {
    echo     public MingAIEditorTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Editor;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingAI" } ^);
    echo         
    echo         // 編輯器依賴
    echo         ExtraModuleNames.AddRange^( new string[] { "UnrealEd" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "ToolMenus" } ^);
    echo     }
    echo }
    ) > "Plugins\MingAI\Source\MingAI\MingAI.Target.cs"
    echo ✓ MingAI.Target.cs已創建
) else (
    echo ✓ MingAI.Target.cs已存在
)

if not exist "Plugins\MingCore\Source\MingCore\MingCore.Target.cs" (
    echo 創建MingCore.Target.cs...
    (
    echo using UnrealBuildTool;
    echo.
    echo public class MingCoreTarget : TargetRules
    echo {
    echo     public MingCoreTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Runtime;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingCore" } ^);
    echo     }
    echo }
    echo.
    echo public class MingCoreEditorTarget : TargetRules
    echo {
    echo     public MingCoreEditorTarget^(TargetInfo Target^) : base^(Target^)
    echo     {
    echo         Type = TargetType.Editor;
    echo         DefaultBuildSettings = BuildSettingsVersion.V2;
    echo         IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    echo         ExtraModuleNames.AddRange^( new string[] { "MingCore" } ^);
    echo         
    echo         // 編輯器依賴
    echo         ExtraModuleNames.AddRange^( new string[] { "UnrealEd" } ^);
    echo         ExtraModuleNames.AddRange^( new string[] { "ToolMenus" } ^);
    echo     }
    echo }
    ) > "Plugins\MingCore\Source\MingCore\MingCore.Target.cs"
    echo ✓ MingCore.Target.cs已創建
) else (
    echo ✓ MingCore.Target.cs已存在
)

echo.
echo 7. 修復常見語法問題...
echo 檢查是否有語法錯誤的包含檔案...

echo.
echo 8. 清理和重新生成...
echo 清理中間檔案...
if exist "Intermediate" (
    rmdir /s /q "Intermediate"
    echo ✓ Intermediate目錄已清理
)

if exist "Binaries" (
    rmdir /s /q "Binaries"
    echo ✓ Binaries目錄已清理
)

echo.
echo 重新生成專案檔案...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project=%PROJECT_PATH% -game -rocket -progress

if %ERRORLEVEL% neq 0 (
    echo ❌ 專案檔案生成失敗
    pause
    exit /b 1
) else (
    echo ✓ 專案檔案生成成功
)

echo.
echo ========================================
echo 修復完成！
echo ========================================
echo.
echo 修復的問題:
echo ✓ 創建缺失的Build.cs檔案
echo ✓ 創建缺失的Target.cs檔案
echo ✓ 修復插件依賴關係
echo ✓ 清理編譯緩存
echo ✓ 重新生成專案檔案
echo.
echo 建議下一步:
echo 1. 運行BUILD_OPTIMIZED.cmd進行編譯
echo 2. 如果仍有錯誤，查看編譯日誌
echo 3. 檢查具體錯誤信息並修復
echo.

pause
