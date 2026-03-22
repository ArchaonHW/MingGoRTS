@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS 快速修復工具
echo ========================================
echo.

set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"

echo 1. 清理編譯緩存...
if exist "Intermediate" (
    rmdir /s /q "Intermediate"
    echo Intermediate目錄已清理
)

if exist "Binaries" (
    rmdir /s /q "Binaries"
    echo Binaries目錄已清理
)

echo.
echo 2. 重新生成專案檔案...
"%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project=%PROJECT_PATH% -game -rocket -progress

if %ERRORLEVEL% neq 0 (
    echo 專案檔案生成失敗
    pause
    exit /b 1
)

echo.
echo 3. 編譯主專案...
"%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTSEditor Win64 Development -project=%PROJECT_PATH% -progress

if %ERRORLEVEL% neq 0 (
    echo 主專案編譯失敗，嘗試編譯插件...
    
    echo 編譯MingCore...
    "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingCoreEditor Win64 Development -project=%PROJECT_PATH% -progress
    
    echo 編譯MingTactical...
    "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingTacticalEditor Win64 Development -project=%PROJECT_PATH% -progress
    
    echo 編譯MingAI...
    "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingAIEditor Win64 Development -project=%PROJECT_PATH% -progress
    
    echo 重新編譯主專案...
    "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTSEditor Win64 Development -project=%PROJECT_PATH% -progress
) else (
    echo 主專案編譯成功！
)

echo.
echo 4. 檢查編譯結果...
if exist "Binaries\Win64\MingGoRTSEditor.exe" (
    echo MingGoRTSEditor.exe 編譯成功
) else (
    echo MingGoRTSEditor.exe 編譯失敗
)

echo.
echo 修復完成！
pause
