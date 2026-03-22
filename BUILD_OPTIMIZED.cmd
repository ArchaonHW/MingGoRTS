@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS Build Optimization Tool
echo ========================================
echo.

set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"
set BUILD_LOG="%cd%\BuildLog.txt"

echo Checking engine path...
if not exist %ENGINE_PATH% (
    echo ERROR: UE5 engine path not found %ENGINE_PATH%
    pause
    exit /b 1
)

echo Checking project file...
if not exist %PROJECT_PATH% (
    echo ERROR: Project file not found %PROJECT_PATH%
    pause
    exit /b 1
)

echo.
echo Step 1: Cleaning intermediate files...
if exist "Intermediate" (
    echo Deleting Intermediate directory...
    rmdir /s /q "Intermediate"
)

if exist "Binaries" (
    echo Deleting Binaries directory...
    rmdir /s /q "Binaries"
)

echo.
echo Step 2: Regenerating project files...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project=%PROJECT_PATH% -game -rocket -progress > %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo ERROR: Project files generation failed
    echo Check log: %BUILD_LOG%
    pause
    exit /b 1
)

echo Project files generated successfully!
echo.

echo Step 3: Compiling MingGoRTSEditor (Development mode)...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo ERROR: Development build failed
    echo Check log: %BUILD_LOG%
    echo.
    echo Trying Debug mode...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 Debug -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    if %ERRORLEVEL% neq 0 (
        echo ERROR: Debug build also failed
        echo Check log: %BUILD_LOG%
        pause
        exit /b 1
    ) else (
        echo Debug build successful!
    )
) else (
    echo Development build successful!
)

echo.
echo Step 4: Compiling plugins...
echo Compiling MingTactical plugin...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingTacticalEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo Compiling MingAI plugin...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingAIEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo Compiling MingCore plugin...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingCoreEditor Win64 Development -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1

echo.
echo Step 5: Checking build results...
if exist "Binaries\Win64\MingGoRTSEditor.exe" (
    echo [OK] MingGoRTSEditor.exe build successful
) else (
    echo [ERROR] MingGoRTSEditor.exe build failed
)

if exist "Plugins\MingTactical\Binaries\Win64\MingTacticalEditor.dll" (
    echo [OK] MingTacticalEditor.dll build successful
) else (
    echo [ERROR] MingTacticalEditor.dll build failed
)

if exist "Plugins\MingAI\Binaries\Win64\MingAIEditor.dll" (
    echo [OK] MingAIEditor.dll build successful
) else (
    echo [ERROR] MingAIEditor.dll build failed
)

if exist "Plugins\MingCore\Binaries\Win64\MingCoreEditor.dll" (
    echo [OK] MingCoreEditor.dll build successful
) else (
    echo [ERROR] MingCoreEditor.dll build failed
)

echo.
echo Step 6: Optimization check...
echo Checking common build issues...

REM Check for circular dependencies
findstr /i "circular dependency" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Found circular dependency issues
)

REM Check for missing include files
findstr /i "cannot find" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Found missing include files
)

REM Check for syntax errors
findstr /i "syntax error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Found syntax errors
)

REM Check for link errors
findstr /i "link error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Found link errors
)

echo.
echo ========================================
echo Build complete!
echo ========================================
echo Build log: %BUILD_LOG%
echo.
echo Recommended next steps:
echo 1. Check build log for errors and warnings
echo 2. If there are errors, check specific error messages
echo 3. Run UE5 editor to test loading
echo 4. Check if plugins load correctly
echo.

pause
