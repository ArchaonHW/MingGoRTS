@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS Enhanced Build Performance Tool
echo ========================================
echo.

REM Set environment variables
set ENGINE_PATH="C:\Program Files\Epic Games\UE_5.7"
set PROJECT_PATH="%cd%\MingGoRTS.uproject"
set BUILD_LOG="%cd%\BuildLog.txt"
set PERFORMANCE_LOG="%cd%\PerformanceLog.txt"
set START_TIME=%time%

REM Parse arguments
set BUILD_MODE=Development
set CLEAN_BUILD=false
set PERFORMANCE_MODE=false
set PARALLEL_BUILD=true
set SKIP_PLUGINS=false

:parse_args
if "%1"=="" goto :args_done
if /i "%1"=="debug" set BUILD_MODE=Debug
if /i "%1"=="shipping" set BUILD_MODE=Shipping
if /i "%1"=="clean" set CLEAN_BUILD=true
if /i "%1"=="perf" set PERFORMANCE_MODE=true
if /i "%1"=="noplugins" set SKIP_PLUGINS=true
if /i "%1"=="noparallel" set PARALLEL_BUILD=false
shift
goto :parse_args

:args_done

echo Build Configuration:
echo - Mode: %BUILD_MODE%
echo - Clean Build: %CLEAN_BUILD%
echo - Performance Monitor: %PERFORMANCE_MODE%
echo - Parallel Build: %PARALLEL_BUILD%
echo - Skip Plugins: %SKIP_PLUGINS%
echo.

REM Check engine path
if not exist %ENGINE_PATH% (
    echo [ERROR] UE5 engine path not found: %ENGINE_PATH%
    pause
    exit /b 1
)

REM Check project file
if not exist %PROJECT_PATH% (
    echo [ERROR] Project file not found: %PROJECT_PATH%
    pause
    exit /b 1
)

echo [SUCCESS] Environment check passed
echo.

REM Clean build cache (if needed)
if "%CLEAN_BUILD%"=="true" (
    echo Step 1: Cleaning build cache...
    if exist "Intermediate" (
        echo Deleting Intermediate directory...
        rmdir /s /q "Intermediate" > nul 2>&1
    )
    if exist "Binaries" (
        echo Deleting Binaries directory...
        rmdir /s /q "Binaries" > nul 2>&1
    )
    if exist "MingGoRTS.sln" (
        echo Deleting solution file...
        del "MingGoRTS.sln" > nul 2>&1
    )
    echo [SUCCESS] Clean completed
    echo.
)

REM Regenerate project files
echo Step 2: Regenerating project files...
%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project=%PROJECT_PATH% -game -rocket -progress > %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Project file generation failed
    echo Check log: %BUILD_LOG%
    findstr /i "error" %BUILD_LOG%
    pause
    exit /b 1
)

echo [SUCCESS] Project files generated successfully!
echo.

REM Build main project
echo Step 3: Building MingGoRTSEditor (%BUILD_MODE% mode)...

set BUILD_COMMAND=%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH%

REM Add parallel build parameters
if "%PARALLEL_BUILD%"=="true" (
    set BUILD_COMMAND=%BUILD_COMMAND% -parallel
)

REM Add performance monitoring
if "%PERFORMANCE_MODE%"=="true" (
    echo Starting performance monitoring...
    echo Build start time: %START_TIME% > %PERFORMANCE_LOG%
    set BUILD_COMMAND=%BUILD_COMMAND% -detailedlogging
)

echo Executing: %BUILD_COMMAND%
%BUILD_COMMAND% -progress >> %BUILD_LOG% 2>&1

if %ERRORLEVEL% neq 0 (
    echo [ERROR] %BUILD_MODE% build failed
    echo Check log: %BUILD_LOG%
    
    REM Try fallback build
    if not "%BUILD_MODE%"=="Debug" (
        echo [RETRY] Trying Debug mode build...
        %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingGoRTSEditor Win64 Debug -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
        if %ERRORLEVEL% equ 0 (
            echo [SUCCESS] Debug build successful!
        ) else (
            echo [ERROR] Debug build also failed
            findstr /i "error" %BUILD_LOG% | head -10
            pause
            exit /b 1
        )
    ) else (
        findstr /i "error" %BUILD_LOG% | head -10
        pause
        exit /b 1
    )
) else (
    echo [SUCCESS] %BUILD_MODE% build successful!
)

REM Build plugins (if needed)
if not "%SKIP_PLUGINS%"=="true" (
    echo.
    echo Step 4: Building plugins...
    
    echo Building MingCore plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingCoreEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo Building MingTactical plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingTacticalEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo Building MingAI plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingAIEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo Building MingBuilding plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingBuildingEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo Building MingAudio plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingAudioEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo Building MingPersonal plugin...
    %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe MingPersonalEditor Win64 %BUILD_MODE% -project=%PROJECT_PATH% -progress >> %BUILD_LOG% 2>&1
    
    echo [SUCCESS] Plugin build completed!
)

REM Check build results
echo.
echo Step 5: Checking build results...

set SUCCESS_COUNT=0
set TOTAL_COUNT=0

REM Check main project
set /a TOTAL_COUNT+=1
if exist "Binaries\Win64\MingGoRTSEditor.exe" (
    echo [SUCCESS] MingGoRTSEditor.exe build successful
    set /a SUCCESS_COUNT+=1
) else (
    echo [FAILED] MingGoRTSEditor.exe build failed
)

REM Check plugins
if not "%SKIP_PLUGINS%"=="true" (
    for %%P in (MingCore MingTactical MingAI MingBuilding MingAudio MingPersonal) do (
        set /a TOTAL_COUNT+=1
        if exist "Plugins\%%P\Binaries\Win64\%%PEditor.dll" (
            echo [SUCCESS] %%PEditor.dll build successful
            set /a SUCCESS_COUNT+=1
        ) else (
            echo [FAILED] %%PEditor.dll build failed
        )
    )
)

REM Performance analysis
if "%PERFORMANCE_MODE%"=="true" (
    echo.
    echo Step 6: Performance analysis...
    set END_TIME=%time%
    echo Build end time: %END_TIME% >> %PERFORMANCE_LOG%
    
    REM Calculate build time (simplified)
    echo Build Statistics: >> %PERFORMANCE_LOG%
    echo Successful Modules: %SUCCESS_COUNT%/%TOTAL_COUNT% >> %PERFORMANCE_LOG%
    echo Start Time: %START_TIME% >> %PERFORMANCE_LOG%
    echo End Time: %END_TIME% >> %PERFORMANCE_LOG%
    
    echo [INFO] Performance Report: %PERFORMANCE_LOG%
)

REM Optimization suggestions
echo.
echo Step 7: Optimization suggestions...

REM Check common issues
findstr /i "circular dependency" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Circular dependency issues detected - check module dependencies
)

findstr /i "cannot find" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Missing include files detected - check #include paths
)

findstr /i "syntax error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Syntax errors detected - check code syntax
)

findstr /i "link error" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Link errors detected - check function implementations
)

findstr /i "out of memory" %BUILD_LOG% > nul
if %ERRORLEVEL% equ 0 (
    echo [WARNING] Out of memory issues detected - try -noparallel parameter
)

REM Check build size
if exist "Binaries\Win64" (
    for /f "delims=" %%S in ('dir "Binaries\Win64" /s ^| find "bytes"') do echo [INFO] Build Size: %%S
)

REM Summary
echo.
echo ========================================
echo Build Complete!
echo ========================================
echo [SUCCESS] Successful Modules: %SUCCESS_COUNT%/%TOTAL_COUNT%
echo [INFO] Build Log: %BUILD_LOG%

if "%PERFORMANCE_MODE%"=="true" (
    echo [INFO] Performance Report: %PERFORMANCE_LOG%
)

echo.
echo Usage Instructions:
echo   Basic Usage: RUN_UBT.cmd
echo   Debug Build: RUN_UBT.cmd debug
echo   Clean Build: RUN_UBT.cmd clean
echo   Performance Monitor: RUN_UBT.cmd perf
echo   Skip Plugins: RUN_UBT.cmd noplugins
echo   Combined Usage: RUN_UBT.cmd clean debug perf
echo.

if %SUCCESS_COUNT%==%TOTAL_COUNT% (
    echo [SUCCESS] Build completely successful! You can start UE5 Editor.
) else (
    echo [WARNING] Build partially failed, please check log files.
)

echo.
pause
