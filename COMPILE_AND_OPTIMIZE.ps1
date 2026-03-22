# MingGoRTS 編譯與優化腳本
# 專業級解決方案 - PowerShell 版本

param(
    [switch]$Clean,
    [switch]$Rebuild,
    [switch]$Debug,
    [switch]$Release,
    [switch]$Optimize,
    [switch]$Test,
    [switch]$Verbose
)

Write-Host "🚀 MingGoRTS 編譯與優化工具" -ForegroundColor Green
Write-Host "======================================" -ForegroundColor Green

# 設定變數
$ProjectRoot = "C:\HW\MingGoRTS"
$EnginePath = "C:\Program Files\Epic Games\UE_5.7"
$UBTPath = "$EnginePath\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$ProjectFile = "$ProjectRoot\MingGoRTS.uproject"

function Write-Status {
    param($Message, $Status = "INFO")
    $Color = switch ($Status) {
        "SUCCESS" { "Green" }
        "ERROR" { "Red" }
        "WARNING" { "Yellow" }
        "INFO" { "Cyan" }
        default { "White" }
    }
    Write-Host $Message -ForegroundColor $Color
}

function Test-Environment {
    Write-Status "🔍 檢查系統環境..." "INFO"
    
    # 檢查 UE5 安裝
    if (-not (Test-Path $UBTPath)) {
        Write-Status "❌ UnrealBuildTool 未找到" "ERROR"
        return $false
    }
    Write-Status "✅ UnrealBuildTool: $UBTPath" "SUCCESS"
    
    # 檢查專案檔案
    if (-not (Test-Path $ProjectFile)) {
        Write-Status "❌ 專案檔案未找到" "ERROR"
        return $false
    }
    Write-Status "✅ 專案檔案: $ProjectFile" "SUCCESS"
    
    # 檢查 Windows SDK
    try {
        $SDKInfo = Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows Kits\Installed Roots" -Name "KitsRoot10" -ErrorAction SilentlyContinue
        if ($SDKInfo) {
            Write-Status "✅ Windows SDK 已安裝" "SUCCESS"
        }
    } catch {
        Write-Status "⚠️ Windows SDK 資訊無法確認" "WARNING"
    }
    
    return $true
}

function Clean-Project {
    Write-Status "🧹 清理專案..." "INFO"
    
    $PathsToClean = @(
        "$ProjectRoot\Intermediate",
        "$ProjectRoot\Binaries",
        "$ProjectRoot\Saved",
        "$ProjectRoot\.vs",
        "$ProjectRoot\DerivedDataCache"
    )
    
    foreach ($Path in $PathsToClean) {
        if (Test-Path $Path) {
            Write-Status "  清理: $Path" "INFO"
            try {
                Remove-Item -Path $Path -Recurse -Force -ErrorAction SilentlyContinue
                Write-Status "    ✅ 清理完成" "SUCCESS"
            } catch {
                Write-Status "    ⚠️ 清理失敗: $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-Status "✅ 專案清理完成" "SUCCESS"
}

function Rebuild-ProjectFiles {
    Write-Status "🔧 重新生成專案檔案..." "INFO"
    
    $Args = @(
        "-projectfiles",
        "-project=`"$ProjectFile`"",
        "-game",
        "-progress"
    )
    
    if ($Verbose) {
        $Args += "-verbose"
    }
    
    Write-Status "  執行: $UBTPath $Args" "INFO"
    
    try {
        $Process = Start-Process -FilePath $UBTPath -ArgumentList $Args -Wait -PassThru -NoNewWindow
        
        if ($Process.ExitCode -eq 0) {
            Write-Status "✅ 專案檔案生成成功" "SUCCESS"
            return $true
        } else {
            Write-Status "❌ 專案檔案生成失敗 (退出代碼: $($Process.ExitCode))" "ERROR"
            return $false
        }
    } catch {
        Write-Status "❌ 專案檔案生成異常: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

function Build-Project {
    param(
        [string]$Configuration = "Development",
        [string]$Platform = "Win64",
        [string]$Target = "MingGoRTSEditor"
    )
    
    Write-Status "🔨 編譯專案 ($Configuration)..." "INFO"
    
    $Args = @(
        $Target,
        $Platform,
        $Configuration,
        "-project=`"$ProjectFile`"",
        "-progress"
    )
    
    if ($Verbose) {
        $Args += "-verbose"
    }
    
    if ($Debug) {
        $Args += "-debug"
    }
    
    Write-Status "  執行: $UBTPath $Args" "INFO"
    
    try {
        $Process = Start-Process -FilePath $UBTPath -ArgumentList $Args -Wait -PassThru -NoNewWindow
        
        if ($Process.ExitCode -eq 0) {
            Write-Status "✅ 專案編譯成功 ($Configuration)" "SUCCESS"
            return $true
        } else {
            Write-Status "❌ 專案編譯失敗 (退出代碼: $($Process.ExitCode))" "ERROR"
            return $false
        }
    } catch {
        Write-Status "❌ 專案編譯異常: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

function Optimize-Project {
    Write-Status "⚡ 優化專案..." "INFO"
    
    # 1. 清理不必要的檔案
    Write-Status "  清理優化..." "INFO"
    $OptimizationPaths = @(
        "$ProjectRoot\Intermediate\Build",
        "$ProjectRoot\Saved\Logs",
        "$ProjectRoot\Saved\Crashes"
    )
    
    foreach ($Path in $OptimizationPaths) {
        if (Test-Path $Path) {
            try {
                Remove-Item -Path $Path -Recurse -Force -ErrorAction SilentlyContinue
            } catch {
                Write-Status "    ⚠️ 清理失敗: $Path" "WARNING"
            }
        }
    }
    
    # 2. 優化編譯設定
    Write-Status "  應用編譯優化..." "INFO"
    
    # 檢查並優化 .Build.cs 檔案
    $BuildFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.Build.cs"
    
    foreach ($BuildFile in $BuildFiles) {
        try {
            $Content = Get-Content -Path $BuildFile.FullName -Raw
            
            # 添加優化設定
            if ($Content -notmatch "OptimizeCode") {
                $Content = $Content -replace "PublicDependencyModuleNames.AddRange", "OptimizeCode = OptimizeCode.Code;`n        PublicDependencyModuleNames.AddRange"
                Set-Content -Path $BuildFile.FullName -Value $Content -Encoding UTF8
                Write-Status "    ✅ 優化: $($BuildFile.Name)" "SUCCESS"
            }
        } catch {
            Write-Status "    ⚠️ 優化失敗: $($BuildFile.Name)" "WARNING"
        }
    }
    
    Write-Status "✅ 專案優化完成" "SUCCESS"
}

function Test-Build {
    Write-Status "🧪 執行編譯測試..." "INFO"
    
    # 測試 Debug 編譯
    if (-not (Build-Project -Configuration "Debug")) {
        Write-Status "❌ Debug 編譯測試失敗" "ERROR"
        return $false
    }
    
    # 測試 Development 編譯
    if (-not (Build-Project -Configuration "Development")) {
        Write-Status "❌ Development 編譯測試失敗" "ERROR"
        return $false
    }
    
    # 測試 Release 編譯 (如果指定)
    if ($Release) {
        if (-not (Build-Project -Configuration "Shipping")) {
            Write-Status "❌ Shipping 編譯測試失敗" "ERROR"
            return $false
        }
    }
    
    Write-Status "✅ 所有編譯測試通過" "SUCCESS"
    return $true
}

function Analyze-Performance {
    Write-Status "📊 分析編譯性能..." "INFO"
    
    # 檢查編譯時間
    $BuildLogPath = "$ProjectRoot\Saved\Logs"
    if (Test-Path $BuildLogPath) {
        $LatestLog = Get-ChildItem -Path $BuildLogPath -Filter "*.log" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
        
        if ($LatestLog) {
            try {
                $LogContent = Get-Content -Path $LatestLog.FullName -Raw
                
                # 提取編譯時間資訊
                if ($LogContent -match "Total build time: ([\d.]+) seconds") {
                    $BuildTime = $matches[1]
                    Write-Status "  編譯時間: $BuildTime 秒" "INFO"
                    
                    if ([double]$BuildTime -gt 300) {
                        Write-Status "  ⚠️ 編譯時間較長，建議優化" "WARNING"
                    } else {
                        Write-Status "  ✅ 編譯時間正常" "SUCCESS"
                    }
                }
                
                # 檢查警告數量
                $WarningCount = ($LogContent | Select-String -Pattern "warning" | Measure-Object).Count
                Write-Status "  警告數量: $WarningCount" "INFO"
                
                if ($WarningCount -gt 50) {
                    Write-Status "  ⚠️ 警告較多，建議檢查" "WARNING"
                }
                
                # 檢查錯誤數量
                $ErrorCount = ($LogContent | Select-String -Pattern "error" | Measure-Object).Count
                Write-Status "  錯誤數量: $ErrorCount" "INFO"
                
                if ($ErrorCount -gt 0) {
                    Write-Status "  ❌ 發現錯誤，需要修復" "ERROR"
                }
                
            } catch {
                Write-Status "  ⚠️ 無法分析編譯日誌" "WARNING"
            }
        }
    }
    
    # 檢查專案大小
    $BinariesPath = "$ProjectRoot\Binaries\Win64"
    if (Test-Path $BinariesPath) {
        try {
            $Size = (Get-ChildItem -Path $BinariesPath -Recurse -File | Measure-Object -Property Length -Sum).Sum / 1MB
            Write-Status "  專案大小: $([math]::Round($Size, 2)) MB" "INFO"
            
            if ($Size -gt 1000) {
                Write-Status "  ⚠️ 專案較大，考慮優化" "WARNING"
            }
        } catch {
            Write-Status "  ⚠️ 無法計算專案大小" "WARNING"
        }
    }
}

function Generate-Report {
    Write-Status "📋 生成編譯報告..." "INFO"
    
    $ReportPath = "$ProjectRoot\Saved\BuildReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"
    
    $Report = @"
MingGoRTS 編譯報告
==================

生成時間: $(Get-Date)
專案路徑: $ProjectRoot
引擎版本: UE 5.7

編譯配置:
- 清理: $(if ($Clean) { "是" } else { "否" })
- 重建: $(if ($Rebuild) { "是" } else { "否" })
- Debug: $(if ($Debug) { "是" } else { "否" })
- Release: $(if ($Release) { "是" } else { "否" })
- 優化: $(if ($Optimize) { "是" } else { "否" })
- 測試: $(if ($Test) { "是" } else { "否" })
- 詳細: $(if ($Verbose) { "是" } else { "否" })

系統資訊:
- 作業系統: $((Get-WmiObject -Class Win32_OperatingSystem).Caption)
- 處理器: $((Get-WmiObject -Class Win32_Processor).Name)
- 記憶體: $([math]::Round((Get-WmiObject -Class Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 2)) GB

建議:
1. 定期清理 Intermediate 目錄
2. 使用增量編譯以提高速度
3. 考慮使用預編譯標頭檔
4. 監控編譯時間和警告數量
5. 定期更新引擎版本

"@
    
    try {
        $Report | Out-File -FilePath $ReportPath -Encoding UTF8
        Write-Status "✅ 報告已生成: $ReportPath" "SUCCESS"
    } catch {
        Write-Status "❌ 報告生成失敗" "ERROR"
    }
}

# 主要執行流程
try {
    Write-Status "🚀 開始執行編譯與優化流程..." "INFO"
    
    # 環境檢查
    if (-not (Test-Environment)) {
        Write-Status "❌ 環境檢查失敗，終止執行" "ERROR"
        exit 1
    }
    
    # 清理階段
    if ($Clean) {
        Clean-Project
    }
    
    # 重新生成階段
    if ($Rebuild) {
        if (-not (Rebuild-ProjectFiles)) {
            Write-Status "❌ 專案重新生成失敗，終止執行" "ERROR"
            exit 1
        }
    }
    
    # 優化階段
    if ($Optimize) {
        Optimize-Project
    }
    
    # 編譯階段
    $BuildSuccess = $false
    
    if ($Debug) {
        $BuildSuccess = Build-Project -Configuration "Debug"
    } elseif ($Release) {
        $BuildSuccess = Build-Project -Configuration "Shipping"
    } else {
        $BuildSuccess = Build-Project -Configuration "Development"
    }
    
    if (-not $BuildSuccess) {
        Write-Status "❌ 編譯失敗，終止執行" "ERROR"
        exit 1
    }
    
    # 測試階段
    if ($Test) {
        if (-not (Test-Build)) {
            Write-Status "❌ 編譯測試失敗" "ERROR"
            exit 1
        }
    }
    
    # 性能分析
    Analyze-Performance
    
    # 生成報告
    Generate-Report
    
    Write-Status "🎉 編譯與優化流程完成！" "SUCCESS"
    
    # 顯示後續建議
    Write-Status "💡 建議後續操作:" "INFO"
    Write-Status "  1. 在 UE5 編輯器中測試專案" "INFO"
    Write-Status "  2. 檢查編譯日誌中的警告" "INFO"
    Write-Status "  3. 運行遊戲測試功能" "INFO"
    Write-Status "  4. 考慮設置持續整合 (CI)" "INFO"
    
} catch {
    Write-Status "❌ 執行過程中發生錯誤: $($_.Exception.Message)" "ERROR"
    Write-Status "🔍 建議檢查:" "INFO"
    Write-Status "  1. 確認 UE5 安裝正確" "INFO"
    Write-Status "  2. 檢查專案檔案完整性" "INFO"
    Write-Status "  3. 確認系統權限" "INFO"
    Write-Status "  4. 檢查磁碟空間" "INFO"
    exit 1
}
