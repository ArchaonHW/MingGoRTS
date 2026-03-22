@echo off
echo ========================================
echo MingGoRTS 編譯除錯優化工具
echo ========================================
echo.

set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"
set BUILD_LOG="%cd%\BuildLog.txt"

echo 檢查引擎路徑...
if not exist %ENGINE_PATH% (
    echo 錯誤: 找不到UE5引擎路徑 %ENGINE_PATH%
    pause
    exit /b 1
)

echo 檢查專案檔案...
if not exist %PROJECT_PATH% (
    echo 錯誤: 找不到專案檔案 %PROJECT_PATH%
    pause
    exit /b 1
)

echo.
echo 步驟1: 清理中間檔案...
if exist "Intermediate" (
    echo 刪除Intermediate目錄...
    rmdir /s /q "Intermediate"
)

if exist "Binaries" (
    echo 刪除Binaries目錄...
    rmdir /s /q "Binaries"
)

echo.
echo 步驟2: 重新生成專案檔案...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project=%PROJECT_PATH% -game -rocket -progress > %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo 錯誤: 專案檔案生成失敗
    echo 查看日誌: %BUILD_LOG%
    pause
    exit /b 1
)

echo 專案檔案生成成功！
echo.

echo 步驟3: 編譯MingGoRTSEditor (Development模式)...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo 錯誤: Development編譯失敗
    echo 查看日誌: %BUILD_LOG%
    echo.
    echo 嘗試編譯Debug模式...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 Debug -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    if %ERRORLEVEL% neq 0 (
        echo 錯誤: Debug編譯也失敗
        echo 查看日誌: %BUILD_LOG%
        pause
        exit /b 1
    ) else (
        echo Debug編譯成功！
    )
) else (
    echo Development編譯成功！
)

echo.
echo 步驟4: 編譯插件...
echo 編譯MingTactical插件...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingTacticalEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo 編譯MingAI插件...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingAIEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo 編譯MingCore插件...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingCoreEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo.
echo 步驟5: 檢查編譯結果...
if exist "Binaries\Win64\MingGoRTSEditor.exe" (
    echo ✓ MingGoRTSEditor.exe 編譯成功
) else (
    echo ✗ MingGoRTSEditor.exe 編譯失敗
)

if exist "Plugins\MingTactical\Binaries\Win64\MingTacticalEditor.dll" (
    echo ✓ MingTacticalEditor.dll 編譯成功
) else (
    echo ✗ MingTacticalEditor.dll 編譯失敗
)

if exist "Plugins\MingAI\Binaries\Win64\MingAIEditor.dll" (
    echo ✓ MingAIEditor.dll 編譯成功
) else (
    echo ✗ MingAIEditor.dll 編譯失敗
)

if exist "Plugins\MingCore\Binaries\Win64\MingCoreEditor.dll" (
    echo ✓ MingCoreEditor.dll 編譯成功
) else (
    echo ✗ MingCoreEditor.dll 編譯失敗
)

echo.
echo 步驟6: 優化檢查...
echo 檢查常見編譯問題...

REM 檢查是否有循環依賴
findstr /i "circular dependency" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo ⚠️  警告: 發現循環依賴問題
)

REM 檢查是否有缺少的包含檔案
findstr /i "cannot find" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo ⚠️  警告: 發現缺少的包含檔案
)

REM 檢查是否有語法錯誤
findstr /i "syntax error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo ⚠️  警告: 發現語法錯誤
)

REM 檢查是否有連結錯誤
findstr /i "link error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo ⚠️  警告: 發現連結錯誤
)

echo.
echo ========================================
echo 編譯完成！
echo ========================================
echo 編譯日誌: %BUILD_LOG%
echo.
echo 建議的後續步驟:
echo 1. 檢查編譯日誌中的錯誤和警告
echo 2. 如果有錯誤，請查看具體錯誤信息
echo 3. 運行UE5編輯器測試載入
echo 4. 檢查插件是否正確載入
echo.

pause
