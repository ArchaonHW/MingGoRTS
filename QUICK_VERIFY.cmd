@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

REM Simple compile check - parse headers only
echo Starting compilation verification...
echo.

"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" MingGoRTSEditor Win64 Development -Project="C:\HW\MingGoRTS\MingGoRTS.uproject" -NoUBTMakefiles 2>&1 | findstr /R /C:"^[^ ].*Error:" /C:"Result:"

echo.
echo Verification complete.
pause
