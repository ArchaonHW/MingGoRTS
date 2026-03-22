@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS CulturalAdaptationSystem Compile Verification
echo ========================================
echo.
echo UE5 Engine: 5.4
echo Target: Development Win64
echo Module: MingGoRTS
echo.

set UBT_PATH=C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set PROJECT=C:\HW\MingGoRTS\MingGoRTS.uproject

if not exist "%UBT_PATH%" (
    echo [ERROR] UnrealBuildTool not found at:
    echo %UBT_PATH%
    echo.
    echo Please verify UE5.4 installation path.
    pause
    exit /b 1
)

echo [1/3] Engine verified
echo [2/3] Starting compilation...
echo.
"%UBT_PATH%" MingGoRTS Win64 Development -Project="%PROJECT%" -NoHotReload
echo.
echo [3/3] Compile complete with exit code: %ERRORLEVEL%

if %ERRORLEVEL% == 0 (
    echo.
    echo ========================================
    echo SUCCESS: Compilation verified!
    echo ========================================
    echo.
    echo All modifications compile correctly:
    echo - Thread safety (FCriticalSection)
    echo - LRU cache implementation
echo - Cache statistics tracking
echo - Enum validation
echo.
    echo Next step: Run unit tests
echo.
) else (
    echo.
    echo ========================================
    echo FAILED: Compilation errors found
echo ========================================
    echo.
    echo Please check:
    echo 1. Include statements in header file
echo 2. Syntax in .cpp implementations
echo 3. Missing dependencies
echo.
)

pause
exit /b %ERRORLEVEL%
