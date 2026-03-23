#!/usr/bin/env pwsh
#Requires -Version 7.0

<#
.SYNOPSIS
    MingGoRTS UE5 專案部署腳本

.DESCRIPTION
    自動化部署 MingGoRTS UE5 專案，包含以下步驟：
    1. 清理舊構建文件
    2. 編譯專案（Development 或 Shipping）
    3. 執行測試驗證
    4. 打包專案
    5. 生成部署報告

.PARAMETER Configuration
    編譯配置：Development, Shipping, Debug, Test

.PARAMETER TargetPlatform
    目標平台：Win64, Android, IOS, Mac, Linux

.PARAMETER SkipTests
    跳過測試執行

.PARAMETER OutputDir
    輸出目錄

.EXAMPLE
    .\Deploy-MingGoRTS.ps1 -Configuration Shipping
    部署 Shipping 版本的 Win64 專案

.EXAMPLE
    .\Deploy-MingGoRTS.ps1 -Configuration Development -TargetPlatform Win64 -SkipTests
    快速部署開發版本（跳過測試）
#>

[CmdletBinding()]
param(
    [Parameter()]
    [ValidateSet("Development", "Shipping", "Debug", "Test")]
    [string]$Configuration = "Development",
    
    [Parameter()]
    [ValidateSet("Win64", "Android", "IOS", "Mac", "Linux")]
    [string]$TargetPlatform = "Win64",
    
    [Parameter()]
    [switch]$SkipTests,
    
    [Parameter()]
    [string]$OutputDir = "$PWD\Build\$Configuration",
    
    [Parameter()]
    [switch]$CleanBuild
)

$ErrorActionPreference = "Stop"

# 專案配置
$ProjectName = "MingGoRTS"
$UProjectPath = "$PWD\MingGoRTS.uproject"
$UE5Path = "C:\Program Files\Epic Games\UE_5.7"
$UATPath = "$UE5Path\Engine\Build\BatchFiles\RunUAT.bat"

# 顏色配置
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
}

# 部署結果追蹤
$DeployResults = @{
    StartTime = Get-Date
    StepsCompleted = 0
    TotalSteps = 5
    Errors = @()
    Warnings = @()
}

function Write-DeployLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        default { $Colors.Info }
    }
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    if ($Level -eq "ERROR") {
        $DeployResults.Errors += $Message
    } elseif ($Level -eq "WARNING") {
        $DeployResults.Warnings += $Message
    }
}

function Show-DeployBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║              MingGoRTS 部署系統                           ║
    ║                                                           ║
    ║     配置: $Configuration                                   
    ║     平台: $TargetPlatform                                  
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.Info
}

function Test-Prerequisites {
    Write-DeployLog "檢查部署前置條件..." "INFO"
    
    # 檢查 UE5 安裝
    if (-not (Test-Path $UE5Path)) {
        Write-DeployLog "UE5.7 未找到於: $UE5Path" "ERROR"
        return $false
    }
    Write-DeployLog "[✓] UE5.7 已安裝" "SUCCESS"
    
    # 檢查專案文件
    if (-not (Test-Path $UProjectPath)) {
        Write-DeployLog "專案文件未找到: $UProjectPath" "ERROR"
        return $false
    }
    Write-DeployLog "[✓] 專案文件存在" "SUCCESS"
    
    # 檢查 UAT
    if (-not (Test-Path $UATPath)) {
        Write-DeployLog "UAT 未找到: $UATPath" "ERROR"
        return $false
    }
    Write-DeployLog "[✓] 構建工具可用" "SUCCESS"
    
    # 確保輸出目錄存在
    if (-not (Test-Path $OutputDir)) {
        New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
        Write-DeployLog "[✓] 創建輸出目錄: $OutputDir" "SUCCESS"
    }
    
    return $true
}

function Start-BuildProcess {
    param(
        [string]$BuildConfig,
        [string]$Platform
    )
    
    Write-DeployLog "==========================================" "INFO"
    Write-DeployLog "步驟 1: 清理舊構建文件" "INFO"
    Write-DeployLog "==========================================" "INFO"
    
    if ($CleanBuild) {
        $DirsToClean = @("Intermediate", "Binaries", "Saved\StagedBuilds")
        foreach ($Dir in $DirsToClean) {
            $FullPath = Join-Path $PWD $Dir
            if (Test-Path $FullPath) {
                Remove-Item -Path $FullPath -Recurse -Force -ErrorAction SilentlyContinue
                Write-DeployLog "  已清理: $Dir" "INFO"
            }
        }
        Write-DeployLog "[✓] 清理完成" "SUCCESS"
    } else {
        Write-DeployLog "[!] 跳過清理（使用 -CleanBuild 強制清理）" "WARNING"
    }
    
    Write-DeployLog "`n==========================================" "INFO"
    Write-DeployLog "步驟 2: 編譯專案 ($BuildConfig $Platform)" "INFO"
    Write-DeployLog "==========================================" "INFO"
    
    $BuildBat = "$UE5Path\Engine\Build\BatchFiles\Build.bat"
    $BuildArgs = @(
        "-project=`"$UProjectPath`""
        "-target=MingGoRTS"
        "-platform=$Platform"
        "-configuration=$BuildConfig"
        "-progress"
    )
    
    try {
        & $BuildBat @BuildArgs
        
        if ($LASTEXITCODE -eq 0) {
            Write-DeployLog "[✓] 編譯成功" "SUCCESS"
            $DeployResults.StepsCompleted++
            return $true
        } else {
            Write-DeployLog "編譯失敗，退出代碼: $LASTEXITCODE" "ERROR"
            return $false
        }
    } catch {
        Write-DeployLog "編譯過程出錯: $_" "ERROR"
        return $false
    }
}

function Start-TestExecution {
    if ($SkipTests) {
        Write-DeployLog "`n==========================================" "INFO"
        Write-DeployLog "步驟 3: 跳過測試（-SkipTests 指定）" "WARNING"
        Write-DeployLog "==========================================" "INFO"
        return $true
    }
    
    Write-DeployLog "`n==========================================" "INFO"
    Write-DeployLog "步驟 3: 執行測試驗證" "INFO"
    Write-DeployLog "==========================================" "INFO"
    
    # 檢查測試腳本
    $TestScript = "$PWD\Tools\test\RunAllTests.ps1"
    if (Test-Path $TestScript) {
        try {
            & $TestScript -QuickMode
            
            if ($LASTEXITCODE -eq 0) {
                Write-DeployLog "[✓] 所有測試通過" "SUCCESS"
                $DeployResults.StepsCompleted++
                return $true
            } else {
                Write-DeployLog "部分測試失敗，請檢查測試報告" "WARNING"
                return $true  # 繼續部署，但記錄警告
            }
        } catch {
            Write-DeployLog "測試執行出錯: $_" "WARNING"
            return $true  # 繼續部署
        }
    } else {
        Write-DeployLog "測試腳本未找到，跳過測試" "WARNING"
        return $true
    }
}

function Start-Packaging {
    param(
        [string]$BuildConfig,
        [string]$Platform,
        [string]$Output
    )
    
    Write-DeployLog "`n==========================================" "INFO"
    Write-DeployLog "步驟 4: 打包專案" "INFO"
    Write-DeployLog "==========================================" "INFO"
    
    $UATArgs = @(
        "BuildCookRun"
        "-project=`"$UProjectPath`""
        "-targetplatform=$Platform"
        "-clientconfig=$BuildConfig"
        "-cook"
        "-stage"
        "-pak"
        "-archive"
        "-archivedirectory=`"$Output`""
    )
    
    # 根據配置添加額外選項
    if ($BuildConfig -eq "Shipping") {
        $UATArgs += "-distribution"
    }
    
    try {
        & $UATPath @UATArgs
        
        if ($LASTEXITCODE -eq 0) {
            Write-DeployLog "[✓] 打包成功" "SUCCESS"
            Write-DeployLog "  輸出位置: $Output" "INFO"
            $DeployResults.StepsCompleted++
            return $true
        } else {
            Write-DeployLog "打包失敗，退出代碼: $LASTEXITCODE" "ERROR"
            return $false
        }
    } catch {
        Write-DeployLog "打包過程出錯: $_" "ERROR"
        return $false
    }
}

function NewoyReport {
    Write-DeployLog "`n==========================================" "INFO"
    Write-DeployLog "步驟 5: 生成部署報告" "INFO"
    Write-DeployLog "==========================================" "INFO"
    
    $EndTime = Get-Date
    $Duration = $EndTime - $DeployResults.StartTime
    
    $ReportFile = "$OutputDir\DeploymentReport_$($Configuration)_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# MingGoRTS 部署報告

**部署時間**: $($DeployResults.StartTime.ToString("yyyy-MM-dd HH:mm:ss"))  
**完成時間**: $($EndTime.ToString("yyyy-MM-dd HH:mm:ss"))  
**總耗時**: $($Duration.ToString("hh\:mm\:ss"))  
**編譯配置**: $Configuration  
**目標平台**: $TargetPlatform

## 執行摘要

| 項目 | 狀態 |
|------|------|
| 前置檢查 | $(if ($DeployResults.StepsCompleted -ge 1) { "✅ 通過" } else { "❌ 失敗" }) |
| 編譯 | $(if ($DeployResults.StepsCompleted -ge 2) { "✅ 成功" } else { "❌ 失敗" }) |
| 測試 | $(if ($DeployResults.StepsCompleted -ge 3) { "✅ 通過" } elseif ($SkipTests) { "⏭️ 跳過" } else { "⚠️ 警告" }) |
| 打包 | $(if ($DeployResults.StepsCompleted -ge 4) { "✅ 成功" } else { "❌ 失敗" }) |

## 輸出文件

**主執行文件**: $OutputDir\$ProjectName.exe  
**構建目錄**: $OutputDir

## 錯誤和警告

### 錯誤 ($($DeployResults.Errors.Count))
"@

    if ($DeployResults.Errors.Count -eq 0) {
        $Report += "無錯誤`n"
    } else {
        foreach ($ErrorItemMessage in $DeployResults.Errors) {
            $Report += "- $ErrorItemMessage`n"
        }
    }
    
    $Report += @"

### 警告 ($($DeployResults.Warnings.Count))
"@

    if ($DeployResults.Warnings.Count -eq 0) {
        $Report += "無警告`n"
    } else {
        foreach ($Warning in $DeployResults.Warnings) {
            $Report += "- $Warning`n"
        }
    }
    
    $Report += @"

## 下一步

1. 驗證構建文件完整性
2. 測試打包後的執行文件
3. 分發給測試人員或發布

---
*由 MingGoRTS 部署系統自動生成*
"@

    $Report | Set-Content -Path $ReportFile -Encoding UTF8
    
    Write-DeployLog "[✓] 報告已生成: $ReportFile" "SUCCESS"
    $DeployResults.StepsCompleted++
    
    return $ReportFile
}

function Show-DeploySummary {
    param([string]$ReportFile)
    
    $EndTime = Get-Date
    $Duration = $EndTime - $DeployResults.StartTime
    
    Write-Host "`n==========================================" -ForegroundColor $Colors.Success
    Write-Host "部署完成摘要" -ForegroundColor $Colors.Success
    Write-Host "==========================================" -ForegroundColor $Colors.Success
    
    Write-Host "`n📊 執行統計:" -ForegroundColor $Colors.Info
    Write-Host "  完成步驟: $($DeployResults.StepsCompleted) / $($DeployResults.TotalSteps)" -ForegroundColor $Colors.Info
    Write-Host "  總耗時: $($Duration.ToString("hh\:mm\:ss"))" -ForegroundColor $Colors.Info
    Write-Host "  錯誤數: $($DeployResults.Errors.Count)" -ForegroundColor $(if ($DeployResults.Errors.Count -gt 0) { $Colors.Error } else { $Colors.Success })
    Write-Host "  警告數: $($DeployResults.Warnings.Count)" -ForegroundColor $(if ($DeployResults.Warnings.Count -gt 0) { $Colors.Warning } else { $Colors.Success })
    
    Write-Host "`n📁 輸出位置:" -ForegroundColor $Colors.Info
    Write-Host "  $OutputDir" -ForegroundColor $Colors.Info
    
    Write-Host "`n📄 部署報告:" -ForegroundColor $Colors.Info
    Write-Host "  $ReportFile" -ForegroundColor $Colors.Info
    
    if ($DeployResults.Errors.Count -eq 0) {
        Write-Host "`n✅ 部署成功！" -ForegroundColor $Colors.Success
    } else {
        Write-Host "`n⚠️ 部署完成，但有錯誤發生" -ForegroundColor $Colors.Warning
    }
    
    Write-Host "`n==========================================" -ForegroundColor $Colors.Success
    Write-Host "感謝使用 MingGoRTS 部署系統" -ForegroundColor $Colors.Success
    Write-Host "==========================================" -ForegroundColor $Colors.Success
}

# ============================================
# 主執行流程
# ============================================

Show-DeployBanner

# 檢查前置條件
if (-not (Test-Prerequisites)) {
    Write-DeployLog "前置檢查失敗，終止部署" "ERROR"
    exit 1
}

# 執行構建
if (-not (Start-BuildProcess -BuildConfig $Configuration -Platform $TargetPlatform)) {
    Write-DeployLog "構建失敗，終止部署" "ERROR"
    exit 1
}

# 執行測試
if (-not (Start-TestExecution)) {
    Write-DeployLog "測試驗證失敗" "WARNING"
}

# 執行打包
if (-not (Start-Packaging -BuildConfig $Configuration -Platform $TargetPlatform -Output $OutputDir)) {
    Write-DeployLog "打包失敗，終止部署" "ERROR"
    exit 1
}

# 生成報告
$ReportFile = New-DeployReport

# 顯示摘要
Show-DeploySummary -ReportFile $ReportFile
