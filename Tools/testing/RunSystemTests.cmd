@echo off
REM MingGoRTS 系統測試執行器
REM 執行系統集成測試和性能優化驗證

setlocal enabledelayedexpansion

echo ========================================
echo MingGoRTS 系統測試執行器
echo ========================================
echo.

REM 設置測試參數
set TEST_MODE=%1
set OUTPUT_PATH=%2
set DURATION_MINUTES=%3
set VERBOSE=%4

REM 設置默認值
if "%TEST_MODE%"=="" set TEST_MODE=Full
if "%OUTPUT_PATH%"=="" set OUTPUT_PATH=c:\HW\MingGoRTS\TestReports
if "%DURATION_MINUTES%"=="" set DURATION_MINUTES=30
if "%VERBOSE%"=="" set VERBOSE=false

echo 測試模式: %TEST_MODE%
echo 輸出路徑: %OUTPUT_PATH%
echo 測試時長: %DURATION_MINUTES% 分鐘
echo 詳細輸出: %VERBOSE%
echo.

REM 創建輸出目錄
if not exist "%OUTPUT_PATH%" mkdir "%OUTPUT_PATH%"
if not exist "%OUTPUT_PATH%\Integration" mkdir "%OUTPUT_PATH%\Integration"
if not exist "%OUTPUT_PATH%\Performance" mkdir "%OUTPUT_PATH%\Performance"
if not exist "%OUTPUT_PATH%\Logs" mkdir "%OUTPUT_PATH%\Logs"
if not exist "%OUTPUT_PATH%\Reports" mkdir "%OUTPUT_PATH%\Reports"

echo ========================================
echo 執行系統集成測試
echo ========================================
echo.

REM 執行系統集成測試
powershell -ExecutionPolicy Bypass -File "c:\HW\MingGoRTS\Tools\testing\SystemIntegrationTestSuite.ps1" -TestMode "%TEST_MODE%" -OutputPath "%OUTPUT_PATH%" -Verbose:%VERBOSE%

if %ERRORLEVEL% neq 0 (
    echo ❌ 系統集成測試失敗
    goto :error
)

echo.
echo ========================================
echo 執行性能優化驗證
echo ========================================
echo.

REM 執行性能優化驗證
if "%TEST_MODE%"=="Full" (
    powershell -ExecutionPolicy Bypass -File "c:\HW\MingGoRTS\Tools\testing\PerformanceOptimizationVerification.ps1" -TestMode Standard -OutputPath "%OUTPUT_PATH%" -DurationMinutes %DURATION_MINUTES% -Verbose:%VERBOSE%
) else if "%TEST_MODE%"=="Stress" (
    powershell -ExecutionPolicy Bypass -File "c:\HW\MingGoRTS\Tools\testing\PerformanceOptimizationVerification.ps1" -TestMode Stress -OutputPath "%OUTPUT_PATH%" -DurationMinutes %DURATION_MINUTES% -Verbose:%VERBOSE% -StressTest
) else if "%TEST_MODE%"=="Continuous" (
    powershell -ExecutionPolicy Bypass -File "c:\HW\MingGoRTS\Tools\testing\PerformanceOptimizationVerification.ps1" -TestMode Continuous -OutputPath "%OUTPUT_PATH%" -DurationMinutes %DURATION_MINUTES% -Verbose:%VERBOSE% -Continuous
)

if %ERRORLEVEL% neq 0 (
    echo ❌ 性能優化驗證失敗
    goto :error
)

echo.
echo ========================================
echo 生成測試摘要報告
echo ========================================
echo.

REM 生成測試摘要報告
powershell -Command "& {
    $IntegrationReports = Get-ChildItem '%OUTPUT_PATH%\Reports\IntegrationTestReport_*.json' | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    $PerformanceReports = Get-ChildItem '%OUTPUT_PATH%\Reports\PerformanceTestReport_*.json' | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    
    if ($IntegrationReports -and $PerformanceReports) {
        $IntegrationData = Get-Content $IntegrationReports.FullName | ConvertFrom-Json
        $PerformanceData = Get-Content $PerformanceReports.FullName | ConvertFrom-Json
        
        $Summary = @{
            TestTime = Get-Date
            IntegrationTests = @{
                Total = $IntegrationData.TotalTests
                Passed = $IntegrationData.PassedTests
                Failed = $IntegrationData.FailedTests
                SuccessRate = $IntegrationData.SuccessRate
            }
            PerformanceTests = @{
                Total = $PerformanceData.TotalTests
                Passed = $PerformanceData.PassedTests
                Failed = $PerformanceData.FailedTests
                SuccessRate = $PerformanceData.SuccessRate
            }
            OverallSuccessRate = [math]::Round((($IntegrationData.SuccessRate + $PerformanceData.SuccessRate) / 2), 2)
            Status = if ((($IntegrationData.SuccessRate + $PerformanceData.SuccessRate) / 2) -ge 90) { 'PASSED' } else { 'FAILED' }
        }
        
        $SummaryPath = '%OUTPUT_PATH%\Reports\SystemTestSummary_$(Get-Date -Format ''yyyyMMdd_HHmmss'').json'
        $Summary | ConvertTo-Json -Depth 10 | Out-File -FilePath $SummaryPath -Encoding UTF8
        
        Write-Host '✅ 測試摘要報告已生成:' $SummaryPath
        Write-Host '📊 系統集成測試成功率:' $IntegrationData.SuccessRate '%'
        Write-Host '📊 性能測試成功率:' $PerformanceData.SuccessRate '%'
        Write-Host '📊 總體成功率:' $Summary.OverallSuccessRate '%'
        Write-Host '🎯 測試狀態:' $Summary.Status
    } else {
        Write-Host '⚠️  無法生成摘要報告 - 缺少測試報告文件'
    }
}"

echo.
echo ========================================
echo 測試完成
echo ========================================
echo.

REM 顯示測試結果摘要
echo 📁 測試報告位置: %OUTPUT_PATH%\Reports
echo 📊 查看詳細報告以了解測試結果
echo.

REM 檢查是否有失敗的測試
echo 檢查測試結果...
powershell -Command "& {
    $SummaryReports = Get-ChildItem '%OUTPUT_PATH%\Reports\SystemTestSummary_*.json' | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if ($SummaryReports) {
        $Summary = Get-Content $SummaryReports.FullName | ConvertFrom-Json
        if ($Summary.Status -eq 'PASSED') {
            Write-Host '🎉 所有測試通過！系統集成和性能驗證成功完成。' -ForegroundColor Green
            exit 0
        } else {
            Write-Host '⚠️  部分測試失敗，請查看詳細報告了解具體問題。' -ForegroundColor Yellow
            exit 1
        }
    } else {
        Write-Host '❌ 無法確定測試結果，請檢查日誌文件。' -ForegroundColor Red
        exit 1
    }
}"

goto :end

:error
echo.
echo ========================================
echo 測試執行失敗
echo ========================================
echo.
echo 請檢查錯誤日誌: %OUTPUT_PATH%\Logs
echo.
exit /b 1

:end
echo.
echo 測試執行器完成
echo.
exit /b 0
