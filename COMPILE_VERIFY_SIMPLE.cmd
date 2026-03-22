@echo off
chcp 65001 >nul
set UE_ENGINE_DIR=C:\Program Files\Epic Games\UE_5.7
"%UE_ENGINE_DIR%\Engine\Build\BatchFiles\Build.bat" MingGoRTSEditor Win64 Development -Project="C:\HW\MingGoRTS\MingGoRTS.uproject" -WaitMutex -NoUBTMakefiles 2>&1 | findstr /V "^Using.*cache" | findstr /V "@progress" | findstr /V "Parsing.*headers" | findstr /V "^Done building" | findstr /V "^BUILD SUCCESSFUL"
