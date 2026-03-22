@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS 編譯除錯工具
echo ========================================
echo.

set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"
set DEBUG_LOG="%cd%\DebugLog.txt"

echo 檢查常見編譯問題...
echo.

echo 1. 檢查專案檔案完整性...
if not exist "MingGoRTS.uproject" (
    echo ✗ 錯誤: 找不到MingGoRTS.uproject
) else (
    echo ✓ MingGoRTS.uproject 存在
)

if not exist "MingGoRTS.sln" (
    echo ✗ 錯誤: 找不到MingGoRTS.sln
) else (
    echo ✓ MingGoRTS.sln 存在
)

echo.
echo 2. 檢查插件目錄結構...
if not exist "Plugins" (
    echo ✗ 錯誤: 找不到Plugins目錄
) else (
    echo ✓ Plugins目錄存在
    
    if exist "Plugins\MingTactical" (
        echo ✓ MingTactical插件存在
        if exist "Plugins\MingTactical\MingTactical.uplugin" (
            echo ✓ MingTactical.uplugin存在
        ) else (
            echo ✗ 錯誤: 找不到MingTactical.uplugin
        )
    ) else (
        echo ✗ 錯誤: 找不到MingTactical插件
    )
    
    if exist "Plugins\MingAI" (
        echo ✓ MingAI插件存在
        if exist "Plugins\MingAI\MingAI.uplugin" (
            echo ✓ MingAI.uplugin存在
        ) else (
            echo ✗ 錯誤: 找不到MingAI.uplugin
        )
    ) else (
        echo ✗ 錯誤: 找不到MingAI插件
    )
    
    if exist "Plugins\MingCore" (
        echo ✓ MingCore插件存在
        if exist "Plugins\MingCore\MingCore.uplugin" (
            echo ✓ MingCore.uplugin存在
        ) else (
            echo ✗ 錯誤: 找不到MingCore.uplugin
        )
    ) else (
        echo ✗ 錯誤: 找不到MingCore插件
    )
)

echo.
echo 3. 檢查Source目錄結構...
if not exist "Source" (
    echo ✗ 錯誤: 找不到Source目錄
) else (
    echo ✓ Source目錄存在
    
    if exist "Source\MingGoRTS" (
        echo ✓ MingGoRTS主要源碼存在
        if exist "Source\MingGoRTS\MingGoRTS.Build.cs" (
            echo ✓ MingGoRTS.Build.cs存在
        ) else (
            echo ✗ 錯誤: 找不到MingGoRTS.Build.cs
        )
        if exist "Source\MingGoRTS\MingGoRTS.Target.cs" (
            echo ✓ MingGoRTS.Target.cs存在
        ) else (
            echo ✗ 錯誤: 找不到MingGoRTS.Target.cs
        )
    ) else (
        echo ✗ 錯誤: 找不到MingGoRTS主要源碼
    )
)

echo.
echo 4. 檢查插件源碼結構...
if exist "Plugins\MingTactical\Source\MingTactical" (
    echo ✓ MingTactical源碼存在
    if exist "Plugins\MingTactical\Source\MingTactical\MingTactical.Build.cs" (
        echo ✓ MingTactical.Build.cs存在
    ) else (
        echo ✗ 錯誤: 找不到MingTactical.Build.cs
    )
)

if exist "Plugins\MingAI\Source\MingAI" (
    echo ✓ MingAI源碼存在
    if exist "Plugins\MingAI\Source\MingAI\MingAI.Build.cs" (
        echo ✓ MingAI.Build.cs存在
    ) else (
        echo ✗ 錯誤: 找不到MingAI.Build.cs
    )
)

if exist "Plugins\MingCore\Source\MingCore" (
    echo ✓ MingCore源碼存在
    if exist "Plugins\MingCore\Source\MingCore\MingCore.Build.cs" (
        echo ✓ MingCore.Build.cs存在
    ) else (
        echo ✗ 錯誤: 找不到MingCore.Build.cs
    )
)

echo.
echo 5. 檢查最近的編譯日誌...
if exist "Logs" (
    echo 找到Logs目錄，檢查最近的編譯日誌...
    dir /b /o-d "Logs\*.txt" | head -5
) else (
    echo 沒有找到Logs目錄
)

echo.
echo 6. 檢查中間檔案和二進制檔案...
if exist "Intermediate" (
    echo ⚠️  警告: Intermediate目錄存在，可能需要清理
    dir "Intermediate" /s | find "個檔案" > nul
) else (
    echo ✓ Intermediate目錄已清理
)

if exist "Binaries" (
    echo ⚠️  警告: Binaries目錄存在，可能需要清理
    dir "Binaries" /s | find "個檔案" > nul
) else (
    echo ✓ Binaries目錄已清理
)

echo.
echo 7. 生成詳細診斷報告...
echo 診斷報告生成時間: %date% %time% > %DEBUG_LOG%
echo. >> %DEBUG_LOG%

echo ===== 專案結構檢查 ===== >> %DEBUG_LOG%
dir /b >> %DEBUG_LOG%
echo. >> %DEBUG_LOG%

echo ===== 插件檢查 ===== >> %DEBUG_LOG%
if exist "Plugins" (
    dir /b "Plugins" >> %DEBUG_LOG%
) else (
    echo 插件目錄不存在 >> %DEBUG_LOG%
)
echo. >> %DEBUG_LOG%

echo ===== 源碼檢查 ===== >> %DEBUG_LOG%
if exist "Source" (
    dir /b /s "Source\*.h" | find /c ".h" >> %DEBUG_LOG%
    dir /b /s "Source\*.cpp" | find /c ".cpp" >> %DEBUG_LOG%
) else (
    echo 源碼目錄不存在 >> %DEBUG_LOG%
)
echo. >> %DEBUG_LOG%

echo ===== 環境檢查 ===== >> %DEBUG_LOG%
echo UE5引擎路徑: %ENGINE_PATH% >> %DEBUG_LOG%
if exist %ENGINE_PATH% (
    echo ✓ UE5引擎存在 >> %DEBUG_LOG%
) else (
    echo ✗ UE5引擎不存在 >> %DEBUG_LOG%
)
echo. >> %DEBUG_LOG%

echo.
echo 8. 建議的修復步驟...
echo.
echo 如果發現問題，建議按以下順序修復:
echo.
echo 步驟1: 清理編譯緩存
echo   - 刪除Intermediate目錄
echo   - 刪除Binaries目錄
echo   - 刪除.sln檔案（重新生成）
echo.
echo 步驟2: 重新生成專案檔案
echo   - 運行UnrealBuildTool生成專案檔案
echo   - 檢查.uplugin檔案語法
echo   - 檢查.Build.cs檔案依賴
echo.
echo 步驟3: 編譯單個插件
echo   - 先編譯MingCore
echo   - 再編譯MingTactical
echo   - 最後編譯MingAI
echo.
echo 步驟4: 編譯主專案
echo   - 編譯MingGoRTSEditor
echo   - 檢查連結錯誤
echo   - 修復語法錯誤
echo.

echo ========================================
echo 診斷完成！
echo ========================================
echo 詳細診斷日誌: %DEBUG_LOG%
echo.

set /p choice="是否執行清理和重新編譯? (y/n): "
if /i "%choice%"=="y" (
    echo.
    echo 執行清理和重新編譯...
    call BUILD_OPTIMIZED.cmd
) else (
    echo.
    echo 請手動檢查問題並執行相應修復步驟。
)

pause
