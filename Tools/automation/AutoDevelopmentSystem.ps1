# MingGoRTS 自動化開發系統
# 提供完整的自動化開發工作流程

param(
    [string]$Mode = "Full",
    [switch]$SkipBuild = $false,
    [switch]$SkipTest = $false,
    [switch]$SkipDeploy = $false,
    [string]$Target = "Development"
)

Write-Host "=== MingGoRTS 自動化開發系統 ===" -ForegroundColor Green
Write-Host "模式: $Mode" -ForegroundColor Yellow
Write-Host "目標: $Target" -ForegroundColor Yellow

# 設置工作目錄
$ProjectRoot = "c:\HW\MingGoRTS"
$BuildDir = "$ProjectRoot\Build"
$LogDir = "$ProjectRoot\Logs"
$TempDir = "$ProjectRoot\Temp"

# 創建必要目錄
if (!(Test-Path $LogDir)) { New-Item -ItemType Directory -Path $LogDir -Force }
if (!(Test-Path $TempDir)) { New-Item -ItemType Directory -Path $TempDir -Force }
if (!(Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir -Force }

# 日誌函數
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogMessage = "[$Timestamp] [$Level] $Message"
    Write-Host $LogMessage -ForegroundColor $(switch($Level) {"ERROR"{"Red"};"WARN"{"Yellow"};"INFO"{"Green"};"DEBUG"{"Gray"}})
    Add-Content "$LogDir\AutoDevelopment_$(Get-Date -Format 'yyyyMMdd').log" $LogMessage
}

# 檢查系統環境
function Test-DevelopmentEnvironment {
    Write-Log "檢查開發環境..." "INFO"
    
    # 檢查 UE5
    $UEPath = Get-Command ue5editor -ErrorAction SilentlyContinue
    if ($UEPath) {
        Write-Log "✓ UE5 編輯器已安裝" "INFO"
    } else {
        Write-Log "✗ UE5 編輯器未找到" "ERROR"
        return $false
    }
    
    # 檢查 Visual Studio
    $VSPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
    if (Test-Path $VSPath) {
        Write-Log "✓ Visual Studio 2022 已安裝" "INFO"
    } else {
        Write-Log "⚠ Visual Studio 2022 未找到，將使用 UE5 編輯器" "WARN"
    }
    
    # 檢查 Git
    $GitPath = Get-Command git -ErrorAction SilentlyContinue
    if ($GitPath) {
        Write-Log "✓ Git 已安裝" "INFO"
    } else {
        Write-Log "✗ Git 未安裝" "ERROR"
        return $false
    }
    
    return $true
}

# 代碼質量檢查
function Invoke-CodeQualityCheck {
    Write-Log "執行代碼質量檢查..." "INFO"
    
    Set-Location $ProjectRoot
    
    # 檢查代碼格式
    Write-Log "檢查代碼格式..." "DEBUG"
    $CppFiles = Get-ChildItem -Recurse -Filter "*.cpp" -Exclude "bin","obj","Temp"
    $HeaderFiles = Get-ChildItem -Recurse -Filter "*.h" -Exclude "bin","obj","Temp"
    
    $TotalFiles = $CppFiles.Count + $HeaderFiles.Count
    Write-Log "找到 $TotalFiles 個源文件" "INFO"
    
    # 檢查常見問題
    $Issues = 0
    foreach ($file in ($CppFiles + $HeaderFiles)) {
        $content = Get-Content $file.FullName -Raw
        
        # 檢查 TODO 註釋
        if ($content -match "TODO|FIXME|HACK") {
            Write-Log "發現 TODO/FIXME/HACK: $($file.Name)" "WARN"
            $Issues++
        }
        
        # 檢查硬編碼路徑
        if ($content -match "C:\\|D:\\") {
            Write-Log "發現硬編碼路徑: $($file.Name)" "WARN"
            $Issues++
        }
    }
    
    Write-Log "代碼質量檢查完成，發現 $Issues 個問題" "INFO"
    return $Issues -eq 0
}

# 自動編譯
function Invoke-AutoBuild {
    if ($SkipBuild) {
        Write-Log "跳過編譯步驟" "INFO"
        return $true
    }
    
    Write-Log "開始自動編譯..." "INFO"
    
    $BuildScript = "$ProjectRoot\Build\DeveloperAutomation.ps1"
    if (Test-Path $BuildScript) {
        try {
            & $BuildScript -BuildMode $Target
            if ($LASTEXITCODE -eq 0) {
                Write-Log "✓ 編譯成功" "INFO"
                return $true
            } else {
                Write-Log "✗ 編譯失敗" "ERROR"
                return $false
            }
        } catch {
            Write-Log "編譯過程發生異常: $($_.Exception.Message)" "ERROR"
            return $false
        }
    } else {
        Write-Log "編譯腳本未找到: $BuildScript" "ERROR"
        return $false
    }
}

# 自動測試
function Invoke-AutoTest {
    if ($SkipTest) {
        Write-Log "跳過測試步驟" "INFO"
        return $true
    }
    
    Write-Log "開始自動測試..." "INFO"
    
    $TestResults = @()
    
    # 運行單元測試
    $TestScripts = Get-ChildItem -Path "$ProjectRoot\Tools\test" -Filter "*.ps1"
    foreach ($script in $TestScripts) {
        Write-Log "運行測試: $($script.Name)" "DEBUG"
        try {
            $result = & $script.FullName
            $TestResults += @{
                Name = $script.BaseName
                Status = if ($result -eq $true) { "PASS" } else { "FAIL" }
                Output = $result
            }
        } catch {
            $TestResults += @{
                Name = $script.BaseName
                Status = "ERROR"
                Output = $_.Exception.Message
            }
        }
    }
    
    # 生成測試報告
    $PassedTests = ($TestResults | Where-Object { $_.Status -eq "PASS" }).Count
    $TotalTests = $TestResults.Count
    $PassRate = if ($TotalTests -gt 0) { [math]::Round($PassedTests / $TotalTests * 100, 2) } else { 0 }
    
    Write-Log "測試完成: $PassedTests/$TotalTests 通過 ($PassRate%)" "INFO"
    
    # 保存測試報告
    $ReportPath = "$LogDir\TestReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $TestResults | ConvertTo-Json -Depth 3 | Out-File $ReportPath
    Write-Log "測試報告已保存: $ReportPath" "INFO"
    
    return $PassRate -ge 80
}

# 自動部署
function Invoke-AutoDeploy {
    if ($SkipDeploy) {
        Write-Log "跳過部署步驟" "INFO"
        return $true
    }
    
    Write-Log "開始自動部署..." "INFO"
    
    # 創建部署包
    $DeployDir = "$BuildDir\Deploy\$(Get-Date -Format 'yyyyMMdd_HHmmss')"
    New-Item -ItemType Directory -Path $DeployDir -Force
    
    # 複製必要文件
    $FilesToDeploy = @(
        "MingGoRTS.uproject",
        "Config",
        "Content",
        "Plugins",
        "Source"
    )
    
    foreach ($file in $FilesToDeploy) {
        $Source = "$ProjectRoot\$file"
        $Destination = "$DeployDir\$file"
        
        if (Test-Path $Source) {
            Copy-Item -Path $Source -Destination $Destination -Recurse -Force
            Write-Log "複製: $file" "DEBUG"
        }
    }
    
    # 生成部署信息
    $DeployInfo = @{
        Timestamp = Get-Date
        Version = "1.0.0"
        Target = $Target
        Files = $FilesToDeploy
        Size = (Get-ChildItem $DeployDir -Recurse | Measure-Object -Property Length).Sum
    }
    
    $DeployInfo | ConvertTo-Json -Depth 3 | Out-File "$DeployDir\deploy_info.json"
    Write-Log "部署完成: $DeployDir" "INFO"
    
    return $true
}

# Git 自動化
function Invoke-GitAutomation {
    Write-Log "執行 Git 自動化..." "INFO"
    
    Set-Location $ProjectRoot
    
    # 檢查 Git 狀態
    $GitStatus = git status --porcelain
    if ($GitStatus) {
        Write-Log "發現未提交的更改" "WARN"
        
        # 自動添加更改
        git add .
        Write-Log "已添加所有更改" "INFO"
        
        # 自動提交（如果有更改）
        $CommitMessage = "Auto-commit $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
        git commit -m $CommitMessage
        Write-Log "已提交更改: $CommitMessage" "INFO"
    } else {
        Write-Log "沒有未提交的更改" "INFO"
    }
    
    # 推送到遠程（可選）
    if ($Mode -eq "Full") {
        try {
            git push origin main
            Write-Log "已推送到遠程倉庫" "INFO"
        } catch {
            Write-Log "推送失敗: $($_.Exception.Message)" "WARN"
        }
    }
}

# 性能監控
function Invoke-PerformanceMonitoring {
    Write-Log "開始性能監控..." "INFO"
    
    $Metrics = @{
        Timestamp = Get-Date
        CPUUsage = (Get-WmiObject Win32_Processor | Measure-Object -Property LoadPercentage -Average).Average
        MemoryUsage = (Get-WmiObject Win32_OperatingSystem | ForEach-Object { [math]::Round(($_.TotalVisibleMemorySize - $_.FreePhysicalMemory) / 1MB, 2) })
        DiskUsage = (Get-WmiObject Win32_LogicalDisk | Where-Object { $_.DeviceID -eq "C:" } | ForEach-Object { [math]::Round(($_.Size - $_.FreeSpace) / 1GB, 2) })
    }
    
    $MetricsPath = "$LogDir\Performance_$(Get-Date -Format 'yyyyMMdd').json"
    if (Test-Path $MetricsPath) {
        $ExistingMetrics = Get-Content $MetricsPath | ConvertFrom-Json
        $ExistingMetrics += $Metrics
        $ExistingMetrics | ConvertTo-Json -Depth 3 | Out-File $MetricsPath
    } else {
        $Metrics | ConvertTo-Json -Depth 3 | Out-File $MetricsPath
    }
    
    Write-Log "性能指標已記錄" "INFO"
}

# 生成開發報告
function New-DevelopmentReport {
    Write-Log "生成開發報告..." "INFO"
    
    $Report = @{
        Timestamp = Get-Date
        Mode = $Mode
        Target = $Target
        Environment = @{
            UE5Installed = (Get-Command ue5editor -ErrorAction SilentlyContinue) -ne $null
            VS2022Installed = Test-Path "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
            GitInstalled = (Get-Command git -ErrorAction SilentlyContinue) -ne $null
        }
        Results = @{
            CodeQuality = $CodeQualityResult
            BuildSuccess = $BuildResult
            TestSuccess = $TestResult
            DeploySuccess = $DeployResult
        }
        Statistics = @{
            SourceFiles = (Get-ChildItem -Path $ProjectRoot -Recurse -Filter "*.cpp","*.h" | Measure-Object).Count
            TestFiles = (Get-ChildItem -Path "$ProjectRoot\Tools\test" -Filter "*.ps1" | Measure-Object).Count
            BuildTime = $BuildTime
            TestTime = $TestTime
        }
    }
    
    $ReportPath = "$LogDir\DevelopmentReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $Report | ConvertTo-Json -Depth 3 | Out-File $ReportPath
    Write-Log "開發報告已生成: $ReportPath" "INFO"
}

# 主執行流程
function Main {
    $StartTime = Get-Date
    
    try {
        # 1. 環境檢查
        if (!(Test-DevelopmentEnvironment)) {
            Write-Log "環境檢查失敗，終止執行" "ERROR"
            exit 1
        }
        
        # 2. 代碼質量檢查
        $script:CodeQualityResult = Invoke-CodeQualityCheck
        
        # 3. 自動編譯
        $BuildStartTime = Get-Date
        $script:BuildResult = Invoke-AutoBuild
        $script:BuildTime = (Get-Date) - $BuildStartTime
        
        # 4. 自動測試
        $TestStartTime = Get-Date
        $script:TestResult = Invoke-AutoTest
        $script:TestTime = (Get-Date) - $TestStartTime
        
        # 5. 自動部署
        $script:DeployResult = Invoke-AutoDeploy
        
        # 6. Git 自動化
        if ($Mode -eq "Full") {
            Invoke-GitAutomation
        }
        
        # 7. 性能監控
        Invoke-PerformanceMonitoring
        
        # 8. 生成報告
        New-DevelopmentReport
        
        $TotalTime = (Get-Date) - $StartTime
        Write-Log "自動化開發完成，總耗時: $($TotalTime.TotalMinutes) 分鐘" "INFO"
        
        # 判斷整體成功
        $OverallSuccess = $BuildResult -and $TestResult -and $DeployResult
        if ($OverallSuccess) {
            Write-Log "🎉 自動化開發全部成功！" "INFO"
            exit 0
        } else {
            Write-Log "❌ 自動化開發部分失敗" "ERROR"
            exit 1
        }
        
    } catch {
        Write-Log "自動化開發發生異常: $($_.Exception.Message)" "ERROR"
        exit 1
    }
}

# 執行主程序
Main
