@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

set ENGINE_PATH=C:\Program Files\Epic Games\UE_5.7
set PROJECT_PATH=C:\HW\MingGoRTS
set PROJECT_FILE=%PROJECT_PATH%\MingGoRTS.uproject

echo Starting build verification...
echo.

cd /d "%PROJECT_PATH%"

echo Running UnrealBuildTool...
"%ENGINE_PATH%\Engine\Build\BatchFiles\Build.bat" MingGoRTSEditor Win64 Development -project="%PROJECT_FILE%" 2>&1

set BUILD_RESULT=%ERRORLEVEL%

echo.
if %BUILD_RESULT% EQU 0 (
    echo BUILD SUCCESSFUL
) else (
    echo BUILD FAILED with error code %BUILD_RESULT%
)

pause
exit /b %BUILD_RESULT%
