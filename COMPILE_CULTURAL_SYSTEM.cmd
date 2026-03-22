@echo off
chcp 65001
cls

echo ========================================
echo MingGoRTS 文化适应系统 - 编译验证
echo ========================================
echo.
echo 时间: %date% %time%
echo.

set ENGINE_PATH=C:\Program Files\Epic Games\UE_5.4
set PROJECT_PATH=C:\HW\MingGoRTS\MingGoRTS.uproject
set LOG_FILE=C:\HW\MingGoRTS\Compile_CulturalSystem.log

echo [1/4] 检查引擎路径...
if not exist "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    echo 错误: 找不到 UE5 引擎路径
    echo 请确认 UE5.4 安装在: %ENGINE_PATH%
    pause
    exit /b 1
)
echo 引擎路径验证通过
echo.

echo [2/4] 编译文化适应系统模块...
echo 编译命令:
echo "%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTS Win64 Development -project="%PROJECT_PATH%" -SingleFile="MingRTSCulturalAdaptationSystem.cpp" -progress
echo.

"%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTS Win64 Development -project="%PROJECT_PATH%" -progress > "%LOG_FILE%" 2>&1

if %ERRORLEVEL% neq 0 (
    echo.
    echo ========================================
    echo 编译失败 - 查看错误日志
    echo ========================================
    echo 日志文件: %LOG_FILE%
    echo.
    echo 错误摘要:
    type "%LOG_FILE%" | findstr /i "error" | head -20
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo 编译成功!
echo ========================================
echo 日志文件: %LOG_FILE%
echo.

rem 检查编译输出
if exist "C:\HW\MingGoRTS\Binaries\Win64\UE4Editor-MingGoRTS.dll" (
    echo DLL 文件生成成功
    dir "C:\HW\MingGoRTS\Binaries\Win64\UE4Editor-MingGoRTS.dll" /b
) else (
    if exist "C:\HW\MingGoRTS\Binaries\Win64\UnrealEditor-MingGoRTS.dll" (
        echo DLL 文件生成成功
        dir "C:\HW\MingGoRTS\Binaries\Win64\UnrealEditor-MingGoRTS.dll" /b
    )
)

echo.
echo 下一步: 运行单元测试 MingRTSLocalizationSystemTest
echo.
pause
exit /b 0
