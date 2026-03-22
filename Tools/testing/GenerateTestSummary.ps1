# Generate System Test Summary
$IntegrationReports = Get-ChildItem "c:\HW\MingGoRTS\TestReports\Reports\IntegrationTestReport_*.json" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
$PerformanceReports = Get-ChildItem "c:\HW\MingGoRTS\TestReports\Reports\PerformanceTestReport_*.json" | Sort-Object LastWriteTime -Descending | Select-Object -First 1

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
    
    $SummaryPath = "c:\HW\MingGoRTS\TestReports\Reports\SystemTestSummary_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $Summary | ConvertTo-Json -Depth 10 | Out-File -FilePath $SummaryPath -Encoding UTF8
    
    Write-Host "✅ Test summary report generated: $SummaryPath" -ForegroundColor Green
    Write-Host "📊 System Integration Test Success Rate: $($IntegrationData.SuccessRate)%" -ForegroundColor Cyan
    Write-Host "📊 Performance Test Success Rate: $($PerformanceData.SuccessRate)%" -ForegroundColor Cyan
    Write-Host "📊 Overall Success Rate: $($Summary.OverallSuccessRate)%" -ForegroundColor Cyan
    Write-Host "🎯 Test Status: $($Summary.Status)" -ForegroundColor $(if ($Summary.Status -eq "PASSED") { "Green" } else { "Yellow" })
    
    if ($Summary.Status -eq "PASSED") {
        Write-Host "`n🎉 All tests passed! System integration and performance verification completed successfully." -ForegroundColor Green
        exit 0
    } else {
        Write-Host "`n⚠️ Some tests failed. Please review detailed reports for specific issues." -ForegroundColor Yellow
        exit 1
    }
} else {
    Write-Host "⚠️ Unable to generate summary - missing test reports" -ForegroundColor Yellow
    exit 1
}
