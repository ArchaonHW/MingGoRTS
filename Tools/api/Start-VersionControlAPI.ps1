# MingGoRTS 版控API啟動腳本
# Version Control API Startup Script

param(
    [string]$Action = "start",
    [string]$ConfigPath = "Config\VersionControl.json",
    [switch]$Verbose,
    [switch]$Debug,
    [switch]$Test
)

# 載入版控API模組
try {
    . (Join-Path $PSScriptRoot "MingVersionControlAPI.ps1")
    Write-Host "版控API模組載入成功" -ForegroundColor Green
} catch {
    Write-Host "版控API模組載入失敗: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

# 主執行函數
function Main {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "MingGoRTS 版控API系統" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "動作: $Action" -ForegroundColor Yellow
    Write-Host "配置: $ConfigPath" -ForegroundColor Yellow
    Write-Host "詳細: $Verbose" -ForegroundColor Yellow
    Write-Host "調試: $Debug" -ForegroundColor Yellow
    Write-Host ""
    
    switch ($Action.ToLower()) {
        "start" {
            Start-MingVCSystem
        }
        "status" {
            Get-MingVCSystemStatus
        }
        "test" {
            Test-MingVCSystem
        }
        "backup" {
            Backup-MingVCSystem
        }
        "report" {
            New-MingVCSystemReport
        }
        "cleanup" {
            Clear-MingVCSystem
        }
        default {
            Write-Host "未知動作: $Action" -ForegroundColor Red
            Show-Help
        }
    }
}

# 啟動版控系統
function Start-MingVCSystem {
    Write-Host "啟動 MingGoRTS 版控系統..." -ForegroundColor Green
    
    # 初始化版控系統
    $InitResult = Initialize-MingVersionControl -ConfigPath $ConfigPath
    if (-not $InitResult) {
        Write-Host "版控系統初始化失敗" -ForegroundColor Red
        return $false
    }
    
    # 檢查系統狀態
    if ($Verbose) {
        Get-MingVCStatus -Detailed
    } else {
        Get-MingVCStatus
    }
    
    # 啟動自動化服務
    if ($Global:MingVCConfig.Automation.AutoBackup) {
        Write-Host "自動備份已啟用" -ForegroundColor Green
    }
    
    if ($Global:MingVCConfig.Integration.CIEnabled) {
        Write-Host "CI集成已啟用" -ForegroundColor Green
    }
    
    Write-Host "版控系統啟動完成!" -ForegroundColor Green
    return $true
}

# 獲取系統狀態
function Get-MingVCSystemStatus {
    Write-Host "獲取 MingGoRTS 版控系統狀態..." -ForegroundColor Green
    
    if (-not $Global:MingVCConfig) {
        Write-Host "版控系統未初始化，正在初始化..." -ForegroundColor Yellow
        Initialize-MingVersionControl -ConfigPath $ConfigPath | Out-Null
    }
    
    if ($Verbose) {
        Get-MingVCStatus -Detailed
    } else {
        Get-MingVCStatus
    }
    
    # 顯示配置資訊
    if ($Debug) {
        Write-Host ""
        Write-Host "配置資訊:" -ForegroundColor Yellow
        $Global:MingVCConfig | ConvertTo-Json -Depth 5 | Write-Host
    }
}

# 測試版控系統
function Test-MingVCSystem {
    Write-Host "測試 MingGoRTS 版控系統..." -ForegroundColor Green
    
    $TestResults = @()
    
    # 測試1: 初始化
    Write-Host "測試1: 系統初始化..." -ForegroundColor Cyan
    try {
        $InitResult = Initialize-MingVersionControl -ConfigPath $ConfigPath
        $TestResults += @{ Test = "初始化"; Result = if ($InitResult) { "PASS" } else { "FAIL" } }
        Write-Host "初始化測試: $(if ($InitResult) { '通過' } else { '失敗' })" -ForegroundColor $(if ($InitResult) { 'Green' } else { 'Red' })
    } catch {
        $TestResults += @{ Test = "初始化"; Result = "FAIL"; Error = $_.Exception.Message }
        Write-Host "初始化測試: 失敗 - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    # 測試2: 狀態檢查
    Write-Host "測試2: 狀態檢查..." -ForegroundColor Cyan
    try {
        $Status = Get-MingVCStatus
        Write-Host "當前狀態: $Status" -ForegroundColor Yellow
        $TestResults += @{ Test = "狀態檢查"; Result = "PASS" }
        Write-Host "狀態檢查: 通過" -ForegroundColor Green
    } catch {
        $TestResults += @{ Test = "狀態檢查"; Result = "FAIL"; Error = $_.Exception.Message }
        Write-Host "狀態檢查: 失敗 - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    # 測試3: 版本檢查
    Write-Host "測試3: 版本檢查..." -ForegroundColor Cyan
    try {
        $Version = Get-MingVCCurrentVersion
        $TestResults += @{ Test = "版本檢查"; Result = "PASS"; Version = $Version }
        Write-Host "版本檢查: 通過 - 當前版本 $Version" -ForegroundColor Green
    } catch {
        $TestResults += @{ Test = "版本檢查"; Result = "FAIL"; Error = $_.Exception.Message }
        Write-Host "版本檢查: 失敗 - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    # 測試4: 提交訊息格式化
    Write-Host "測試4: 提交訊息格式化..." -ForegroundColor Cyan
    try {
        $FormattedMessage = Format-MingVCCommitMessage -Message "test feature" -Type "feat" -Scope "api"
        $TestResults += @{ Test = "提交訊息格式化"; Result = "PASS"; Message = $FormattedMessage }
        Write-Host "提交訊息格式化: 通過 - $FormattedMessage" -ForegroundColor Green
    } catch {
        $TestResults += @{ Test = "提交訊息格式化"; Result = "FAIL"; Error = $_.Exception.Message }
        Write-Host "提交訊息格式化: 失敗 - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    # 測試5: 版本驗證
    Write-Host "測試5: 版本驗證..." -ForegroundColor Cyan
    try {
        $ValidVersion = Test-MingVCVersion -Version "1.0.0"
        $InvalidVersion = Test-MingVCVersion -Version "invalid"
        $TestResults += @{ Test = "版本驗證"; Result = "PASS"; Valid = $ValidVersion; Invalid = $InvalidVersion }
        Write-Host "版本驗證: 通過 - 有效: $ValidVersion, 無效: $InvalidVersion" -ForegroundColor Green
    } catch {
        $TestResults += @{ Test = "版本驗證"; Result = "FAIL"; Error = $_.Exception.Message }
        Write-Host "版本驗證: 失敗 - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    # 顯示測試結果
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "測試結果總結" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    $PassCount = ($TestResults | Where-Object { $_.Result -eq "PASS" }).Count
    $FailCount = ($TestResults | Where-Object { $_.Result -eq "FAIL" }).Count
    
    Write-Host "通過: $PassCount" -ForegroundColor Green
    Write-Host "失敗: $FailCount" -ForegroundColor $(if ($FailCount -gt 0) { 'Red' } else { 'Green' })
    
    if ($Verbose) {
        Write-Host ""
        Write-Host "詳細結果:" -ForegroundColor Yellow
        $TestResults | ForEach-Object {
            $Color = if ($_.Result -eq "PASS") { "Green" } else { "Red" }
            Write-Host "  $($_.Test): $($_.Result)" -ForegroundColor $Color
            if ($_.Error) {
                Write-Host "    錯誤: $($_.Error)" -ForegroundColor Red
            }
            if ($_.Version) {
                Write-Host "    版本: $($_.Version)" -ForegroundColor Cyan
            }
            if ($_.Message) {
                Write-Host "    訊息: $($_.Message)" -ForegroundColor Cyan
            }
        }
    }
    
    Write-Host ""
    if ($FailCount -eq 0) {
        Write-Host "所有測試通過! 版控系統運行正常。" -ForegroundColor Green
        return $true
    } else {
        Write-Host "有 $FailCount 個測試失敗，請檢查系統配置。" -ForegroundColor Red
        return $false
    }
}

# 備份系統
function Backup-MingVCSystem {
    Write-Host "執行 MingGoRTS 版控系統備份..." -ForegroundColor Green
    
    if (-not $Global:MingVCConfig) {
        Write-Host "版控系統未初始化，正在初始化..." -ForegroundColor Yellow
        Initialize-MingVersionControl -ConfigPath $ConfigPath | Out-Null
    }
    
    $BackupResult = Backup-MingVCRepository -Compress -IncludeHistory
    if ($BackupResult) {
        Write-Host "備份完成!" -ForegroundColor Green
    } else {
        Write-Host "備份失敗!" -ForegroundColor Red
    }
    
    return $BackupResult
}

# 生成系統報告
function New-MingVCSystemReport {
    Write-Host "生成 MingGoRTS 版控系統報告..." -ForegroundColor Green
    
    if (-not $Global:MingVCConfig) {
        Write-Host "版控系統未初始化，正在初始化..." -ForegroundColor Yellow
        Initialize-MingVersionControl -ConfigPath $ConfigPath | Out-Null
    }
    
    $Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
    $ReportPath = "Reports\VersionControl\VCReport_$Timestamp.html"
    
    # 確保報告目錄存在
    $ReportDir = Split-Path $ReportPath -Parent
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $ReportResult = New-MingVCReport -ReportType "full" -OutputPath $ReportPath -Html
    if ($ReportResult) {
        Write-Host "報告已生成: $ReportPath" -ForegroundColor Green
        
        # 詢問是否打開報告
        $OpenReport = Read-Host "是否打開報告? (y/N)"
        if ($OpenReport -eq 'y' -or $OpenReport -eq 'Y') {
            Invoke-Item $ReportPath
        }
    } else {
        Write-Host "報告生成失敗!" -ForegroundColor Red
    }
    
    return $ReportResult
}

# 清理系統
function Clear-MingVCSystem {
    Write-Host "清理 MingGoRTS 版控系統..." -ForegroundColor Green
    
    # 清理全域變數
    $Global:MingVCConfig = $null
    $Global:MingVCState = $null
    $Global:MingVCHistory = @()
    
    # 清理臨時檔案
    $TempFiles = @(
        "Temp\VersionControl\*",
        "Logs\VersionControl\*",
        "Cache\VersionControl\*"
    )
    
    foreach ($TempFile in $TempFiles) {
        if (Test-Path $TempFile) {
            Remove-Item -Path $TempFile -Recurse -Force
            Write-Host "已清理: $TempFile" -ForegroundColor Yellow
        }
    }
    
    Write-Host "系統清理完成!" -ForegroundColor Green
}

# 顯示幫助
function Show-Help {
    Write-Host "MingGoRTS 版控API系統使用說明:" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "用法:" -ForegroundColor Yellow
    Write-Host "  .\Start-VersionControlAPI.ps1 -Action <動作> [選項]" -ForegroundColor White
    Write-Host ""
    Write-Host "動作:" -ForegroundColor Yellow
    Write-Host "  start   - 啟動版控系統 (默認)" -ForegroundColor White
    Write-Host "  status  - 獲取系統狀態" -ForegroundColor White
    Write-Host "  test    - 測試系統功能" -ForegroundColor White
    Write-Host "  backup  - 執行系統備份" -ForegroundColor White
    Write-Host "  report  - 生成系統報告" -ForegroundColor White
    Write-Host "  cleanup - 清理系統緩存" -ForegroundColor White
    Write-Host ""
    Write-Host "選項:" -ForegroundColor Yellow
    Write-Host "  -ConfigPath <路徑>  - 指定配置檔案路徑" -ForegroundColor White
    Write-Host "  -Verbose             - 顯示詳細資訊" -ForegroundColor White
    Write-Host "  -Debug               - 顯示調試資訊" -ForegroundColor White
    Write-Host "  -Test                - 執行測試模式" -ForegroundColor White
    Write-Host ""
    Write-Host "範例:" -ForegroundColor Yellow
    Write-Host "  .\Start-VersionControlAPI.ps1" -ForegroundColor White
    Write-Host "  .\Start-VersionControlAPI.ps1 -Action status -Verbose" -ForegroundColor White
    Write-Host "  .\Start-VersionControlAPI.ps1 -Action test" -ForegroundColor White
    Write-Host "  .\Start-VersionControlAPI.ps1 -Action backup" -ForegroundColor White
    Write-Host "  .\Start-VersionControlAPI.ps1 -Action report" -ForegroundColor White
}

# 執行主函數
Main
