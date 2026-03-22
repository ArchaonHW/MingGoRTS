@echo off
chcp 65001 >nul
echo ========================================
echo MingGoRTS 文化适应系统 - 编译验证
echo ========================================
echo.
echo 引擎版本: UE 5.4
echo 目标模块: MingGoRTS (包含 CulturalAdaptationSystem)
echo 编译模式: Development
echo.

set UBT=C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set PROJECT=C:\HW\MingGoRTS\MingGoRTS.uproject
set LOG=C:\HW\MingGoRTS\Logs\Compile_Verification_2026_03_23.log

if not exist "C:\HW\MingGoRTS\Logs" mkdir "C:\HW\MingGoRTS\Logs"

echo [1/3] 验证引擎路径...
if not exist "%UBT%" (
    echo 错误: 找不到 UnrealBuildTool
    echo 路径: %UBT%
    pause
    exit /b 1
)
echo 引擎路径验证通过
echo.

echo [2/3] 执行编译...
echo 命令: UnrealBuildTool.exe MingGoRTS Win64 Development
echo 日志: %LOG%
echo.
echo 编译进行中，请稍候... (预计 2-5 分钟)
echo.

"%UBT%" MingGoRTS Win64 Development -Project="%PROJECT%" -Progress > "%LOG%" 2>&1

set EXITCODE=%ERRORLEVEL%

echo [3/3] 编译结果检查...
echo.

if %EXITCODE% equ 0 (
    echo ========================================
    echo 编译成功!
    echo ========================================
    echo.
    echo 验证通过项:
    echo - 头文件包含正确
echo - 语法检查通过
echo - 链接成功
echo.
    echo 输出文件:
    dir "C:\HW\MingGoRTS\Binaries\Win64\*.dll" /b 2>nul | findstr /i "ming" >nul
    if %ERRORLEVEL% equ 0 (
        echo - DLL 文件已生成
    )
    echo.
    echo 下一步: 运行单元测试 MingRTSLocalizationSystemTest
echo.
    pause
    exit /b 0
) else (
    echo ========================================
    echo 编译失败! (Exit Code: %EXITCODE%)
    echo ========================================
    echo.
    echo 错误摘要:
    findstr /i /n "error:" "%LOG%" 2>nul | head -10
    echo.
    echo 完整日志: %LOG%
    echo.
    echo 建议操作:
    echo 1. 检查日志中的具体错误
echo 2. 确认头文件包含路径正确
echo 3. 验证所有修改的语法
echo.
    pause
    exit /b %EXITCODE%
)
