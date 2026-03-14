@echo off
cd /d C:\HW\MingGoRTS
echo Generating project files...
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="MingWarRTS.uproject" -game -progress
pause