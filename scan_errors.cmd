@echo off
chcp 65001 >nul
echo ========================================
echo Scanning Current Build Errors
echo ========================================
echo.

echo Checking BuildLog.txt for errors...
if exist "BuildLog.txt" (
    findstr /i "error" "BuildLog.txt" | findstr /v "warning" | findstr /v "deprecation"
    echo.
    echo Total error count:
    findstr /i "error" "BuildLog.txt" | findstr /v "warning" | findstr /v "deprecation" | find /c /v ""
) else (
    echo BuildLog.txt not found!
)

echo.
echo Checking for common syntax issues...
if exist "BuildLog.txt" (
    echo.
    echo UENUM errors:
    findstr "UENUM" "BuildLog.txt" | findstr "Error"
    
    echo.
    echo Character constant errors:
    findstr "character constant" "BuildLog.txt"
    
    echo.
    echo EOF errors:
    findstr "EOF" "BuildLog.txt"
    
    echo.
    echo Include errors:
    findstr "include.*appear at top" "BuildLog.txt"
)

echo.
echo ========================================
echo Scan Complete
echo ========================================
pause
