@echo off
chcp 65001 >nul
echo ==========================================
echo MingGoRTS 编译与优化工具
echo ==========================================
echo.

REM 设置变量
set "ProjectRoot=C:\HW\MingGoRTS"
set "EnginePath=C:\Program Files\Epic Games\UE_5.7"
set "UBTPath=%EnginePath%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
set "ProjectFile=%ProjectRoot%\MingGoRTS.uproject"

echo [1/5] 检查环境...
if not exist "%UBTPath%" (
    echo [错误] 未找到 UnrealBuildTool: %UBTPath%
    echo 请确认 UE5.7 已正确安装
    pause
    exit /b 1
)
echo [✓] UnrealBuildTool 已找到

if not exist "%ProjectFile%" (
    echo [错误] 未找到项目文件: %ProjectFile%
    pause
    exit /b 1
)
echo [✓] 项目文件已找到

echo.
echo [2/5] 清理旧文件...
if exist "%ProjectRoot%\Intermediate" (
    echo 清理 Intermediate...
    rmdir /s /q "%ProjectRoot%\Intermediate" 2>nul
)
if exist "%ProjectRoot%\Binaries" (
    echo 清理 Binaries...
    rmdir /s /q "%ProjectRoot%\Binaries" 2>nul
)
if exist "%ProjectRoot%\.vs" (
    echo 清理 .vs...
    rmdir /s /q "%ProjectRoot%\.vs" 2>nul
)
echo [✓] 清理完成

echo.
echo [3/5] 生成项目文件...
"%UBTPath%" -projectfiles -project="%ProjectFile%" -game -progress
if errorlevel 1 (
    echo [错误] 项目文件生成失败
    pause
    exit /b 1
)
echo [✓] 项目文件生成成功

echo.
echo [4/5] 编译项目 (Development 配置)...
"%UBTPath%" MingGoRTSEditor Win64 Development -project="%ProjectFile%" -progress
if errorlevel 1 (
    echo [错误] 编译失败
    pause
    exit /b 1
)
echo [✓] 编译成功

echo.
echo [5/5] 编译项目 (Shipping 配置)...
"%UBTPath%" MingGoRTS Win64 Shipping -project="%ProjectFile%" -progress
if errorlevel 1 (
    echo [警告] Shipping 编译失败，但 Development 编译成功
) else (
    echo [✓] Shipping 编译成功
)

echo.
echo ==========================================
echo 编译与优化完成！
echo ==========================================
echo.
echo 后续建议:
echo   1. 在 UE5 编辑器中打开项目测试
echo   2. 检查 Saved\Logs 目录下的日志文件
echo   3. 运行游戏测试各项功能
echo   4. 验证所有 Epic 系统是否正常工作
echo.
pause
