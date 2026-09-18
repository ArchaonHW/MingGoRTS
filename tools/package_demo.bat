@echo off
rem U-1 vertical-slice packaging: MinGW exe + assets -> dist\MingGoRTS-Demo\
rem MinGW links -static; only system DLLs remain, zip-ready.
rem NOTE: keep this file ASCII-only (cmd.exe misreads UTF-8 comments).
setlocal
set "ROOT=%~dp0.."
set "DIST=%ROOT%\dist\MingGoRTS-Demo"

if not exist "%ROOT%\build-mingw\bin\DuanqiaoPlayable.exe" (
    echo [ERR] build-mingw\bin\DuanqiaoPlayable.exe not found - build MinGW target first
    exit /b 1
)

if exist "%DIST%" rmdir /s /q "%DIST%"
mkdir "%DIST%\assets"

copy /y "%ROOT%\build-mingw\bin\DuanqiaoPlayable.exe" "%DIST%\" >nul
rem demo only loads maps + fonts; cards/avatars stay out (~75MB saved)
xcopy /e /i /y /q "%ROOT%\assets\maps" "%DIST%\assets\maps" >nul
xcopy /e /i /y /q "%ROOT%\assets\fonts" "%DIST%\assets\fonts" >nul
copy /y "%ROOT%\tools\demo-readme.txt" "%DIST%\README.txt" >nul

echo [OK] packaged to %DIST%
echo      run %DIST%\DuanqiaoPlayable.exe to verify
endlocal
