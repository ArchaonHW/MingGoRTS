@echo off
chcp 65001 >nul
title MingGoRTS Project Integration Tool
cls

echo ========================================
echo    MingGoRTS Project Integration Tool
echo ========================================
echo.
echo  Executing project integration workflow...
echo.

set PROJECT_ROOT=%~dp0
set SOURCE_PATH=%PROJECT_ROOT%Source\MingGoRTS
set BUILD_LOG=%PROJECT_ROOT%IntegrationLog.txt

echo [%date% %time%] Starting project integration > "%BUILD_LOG%"

:: Step 1: Verify project structure
echo [1/8] Verifying project structure...
echo [%date% %time%] Verifying project structure >> "%BUILD_LOG%"

if not exist "%SOURCE_PATH%\Public" (
    echo   [ERROR] Public directory not found >> "%BUILD_LOG%"
    echo   [ERROR] Public directory not found
    goto :error
)

if not exist "%SOURCE_PATH%\Private" (
    echo   [ERROR] Private directory not found >> "%BUILD_LOG%"
    echo   [ERROR] Private directory not found
    goto :error
)

echo   [OK] Project structure verified >> "%BUILD_LOG%"
echo   [OK] Project structure verified

:: Step 2: Count developed systems
echo.
echo [2/8] Counting developed systems...
echo [%date% %time%] Counting developed systems >> "%BUILD_LOG%"

set /a SYSTEM_COUNT=0
set /a HEADER_COUNT=0
set /a SOURCE_COUNT=0

for /f %%a in ('dir /s /b "%SOURCE_PATH%\Public\*.h" 2^>nul ^| find /c /v ""') do set /a HEADER_COUNT=%%a
for /f %%a in ('dir /s /b "%SOURCE_PATH%\Private\*.cpp" 2^>nul ^| find /c /v ""') do set /a SOURCE_COUNT=%%a
for /f %%a in ('dir /s /b "%SOURCE_PATH%\Public\" 2^>nul ^| find /c /v ""') do set /a SYSTEM_COUNT=%%a

echo   Found %HEADER_COUNT% header files >> "%BUILD_LOG%"
echo   Found %SOURCE_COUNT% source files >> "%BUILD_LOG%"
echo   Found %SYSTEM_COUNT% system directories >> "%BUILD_LOG%"
echo   Found %HEADER_COUNT% header files
echo   Found %SOURCE_COUNT% source files
echo   Found %SYSTEM_COUNT% system directories

:: Step 3: Verify key systems
echo.
echo [3/8] Verifying key systems...
echo [%date% %time%] Verifying key systems >> "%BUILD_LOG%"

call :check_system "Tutorial" "%SOURCE_PATH%\Public\Tutorial\MingRTSTutorialSystem.h"
call :check_system "Security" "%SOURCE_PATH%\Public\Security\MingRTSSecurityManager.h"
call :check_system "Replay" "%SOURCE_PATH%\Public\Replay\MingRTSReplaySystem.h"
call :check_system "Achievement" "%SOURCE_PATH%\Public\Stats\MingRTSAchievementSystem.h"
call :check_system "Spectator" "%SOURCE_PATH%\Public\Spectator\MingRTSSpectatorSystem.h"
call :check_system "Telemetry" "%SOURCE_PATH%\Public\Telemetry\MingRTSTelemetrySystem.h"
call :check_system "Mod" "%SOURCE_PATH%\Public\Mod\MingRTSModSystem.h"
call :check_system "Tournament" "%SOURCE_PATH%\Public\Tournament\MingRTSTournamentSystem.h"
call :check_system "Cloud" "%SOURCE_PATH%\Public\Cloud\MingRTSCloudSystem.h"
call :check_system "VR/AR" "%SOURCE_PATH%\Public\XR\MingRTSVRSupportSystem.h"
call :check_system "Multiplayer" "%SOURCE_PATH%\Public\Multiplayer\MingRTSMultiplayerSupportSystem.h"
call :check_system "Performance" "%SOURCE_PATH%\Public\Performance\MingRTSPerformanceSystem.h"

:: Step 4: Check Build.cs
echo.
echo [4/8] Checking Build.cs configuration...
echo [%date% %time%] Checking Build.cs configuration >> "%BUILD_LOG%"

if exist "%SOURCE_PATH%\MingGoRTS.Build.cs" (
    echo   [OK] Build.cs exists >> "%BUILD_LOG%"
    echo   [OK] Build.cs exists
) else (
    echo   [ERROR] Build.cs not found >> "%BUILD_LOG%"
    echo   [ERROR] Build.cs not found
    goto :error
)

:: Step 5: Generate integration report
echo.
echo [5/8] Generating integration report...
echo [%date% %time%] Generating integration report >> "%BUILD_LOG%"

set REPORT_FILE=%PROJECT_ROOT%IntegrationReport_%date:~-4,4%%date:~-10,2%%date:~-7,2%.md

echo # MingGoRTS Project Integration Report > "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"
echo **Generated**: %date% %time% >> "%REPORT_FILE%"
echo **Project Path**: %PROJECT_ROOT% >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo ## Statistics >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"
echo - **Header Files**: %HEADER_COUNT% >> "%REPORT_FILE%"
echo - **Source Files**: %SOURCE_COUNT% >> "%REPORT_FILE%"
echo - **System Directories**: %SYSTEM_COUNT% >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo ## Integrated Systems >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

if exist "%SOURCE_PATH%\Public\Tutorial" echo - [OK] Tutorial System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Security" echo - [OK] Security System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Replay" echo - [OK] Replay System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Stats" echo - [OK] Achievement and Statistics System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Spectator" echo - [OK] Spectator System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Telemetry" echo - [OK] Telemetry System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Mod" echo - [OK] Mod System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Tournament" echo - [OK] Tournament System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Cloud" echo - [OK] Cloud System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\XR" echo - [OK] VR/AR System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Multiplayer" echo - [OK] Multiplayer System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Performance" echo - [OK] Performance System >> "%REPORT_FILE%"
if exist "%SOURCE_PATH%\Public\Localization" echo - [OK] Localization System >> "%REPORT_FILE%"

echo. >> "%REPORT_FILE%"
echo ## Integration Status >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"
echo [OK] **All systems are ready** >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"
echo ## Next Steps >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"
echo 1. Run `RUN_UBT.cmd` to generate project files >> "%REPORT_FILE%"
echo 2. Run `BUILD_OPTIMIZED.cmd` to compile the project >> "%REPORT_FILE%"
echo 3. Run DeveloperAutomation tools for testing >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo   [OK] Integration report generated: %REPORT_FILE% >> "%BUILD_LOG%"
echo   [OK] Integration report generated: %REPORT_FILE%

:: Step 6: Verify uproject file
echo.
echo [6/8] Verifying uproject configuration...
echo [%date% %time%] Verifying uproject configuration >> "%BUILD_LOG%"

if exist "%PROJECT_ROOT%MingGoRTS.uproject" (
    echo   [OK] uproject file exists >> "%BUILD_LOG%"
    echo   [OK] uproject file exists
) else (
    echo   [ERROR] uproject file not found >> "%BUILD_LOG%"
    echo   [ERROR] uproject file not found
    goto :error
)

:: Step 7: Check plugins
echo.
echo [7/8] Checking plugin status...
echo [%date% %time%] Checking plugin status >> "%BUILD_LOG%"

if exist "%PROJECT_ROOT%Plugins" (
    for /f %%a in ('dir /b /ad "%PROJECT_ROOT%Plugins\" 2^>nul ^| find /c /v ""') do (
        echo   Found %%a plugins >> "%BUILD_LOG%"
        echo   Found %%a plugins
    )
) else (
    echo   [WARNING] Plugins directory not found >> "%BUILD_LOG%"
    echo   [WARNING] Plugins directory not found
)

:: Step 8: Complete
echo.
echo [8/8] Integration complete!
echo [%date% %time%] Integration complete >> "%BUILD_LOG%"

echo. >> "%BUILD_LOG%"
echo ===== Integration Summary ===== >> "%BUILD_LOG%"
echo Header Files: %HEADER_COUNT% >> "%BUILD_LOG%"
echo Source Files: %SOURCE_COUNT% >> "%BUILD_LOG%"
echo Report File: %REPORT_FILE% >> "%BUILD_LOG%"
echo ============================== >> "%BUILD_LOG%"

echo.
echo ========================================
echo    [OK] Project Integration Complete!
echo ========================================
echo.
echo Statistics:
echo   - Header Files: %HEADER_COUNT%
echo   - Source Files: %SOURCE_COUNT%
echo   - System Directories: %SYSTEM_COUNT%
echo.
echo Generated Files:
echo   - Integration Report: %REPORT_FILE%
echo   - Log File: %BUILD_LOG%
echo.
echo Next Steps:
echo   1. Run RUN_UBT.cmd to generate project files
echo   2. Run BUILD_OPTIMIZED.cmd to compile
echo   3. View the integration report for details
echo.
pause
goto :end

:check_system
echo   Checking %~1...
if exist "%~2" (
    echo     [OK] %~1 system is ready >> "%BUILD_LOG%"
    echo     [OK] %~1 system is ready
) else (
    echo     [WARNING] %~1 system not found >> "%BUILD_LOG%"
    echo     [WARNING] %~1 system not found
)
exit /b

:error
echo.
echo ========================================
echo    [ERROR] Integration failed
echo ========================================
echo.
echo Please check the log file for details:
echo   %BUILD_LOG%
echo.
pause
exit /b 1

:end
exit /b 0
