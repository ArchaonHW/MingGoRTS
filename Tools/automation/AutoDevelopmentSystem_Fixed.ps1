# MingGoRTS Automated Development System
# Complete automated development workflow

param(
    [string]$Mode = "Quick",
    [switch]$SkipBuild = $false,
    [switch]$SkipTest = $false,
    [switch]$SkipDeploy = $false,
    [string]$Target = "Development"
)

Write-Host "=== MingGoRTS Automated Development System ===" -ForegroundColor Green
Write-Host "Mode: $Mode" -ForegroundColor Yellow
Write-Host "Target: $Target" -ForegroundColor Yellow

# Set working directory
$ProjectRoot = "c:\HW\MingGoRTS"
$BuildDir = "$ProjectRoot\Build"
$LogDir = "$ProjectRoot\Logs"
$TempDir = "$ProjectRoot\Temp"

# Create necessary directories
if (!(Test-Path $LogDir)) { New-Item -ItemType Directory -Path $LogDir -Force }
if (!(Test-Path $TempDir)) { New-Item -ItemType Directory -Path $TempDir -Force }
if (!(Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir -Force }

# Logging function
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogMessage = "[$Timestamp] [$Level] $Message"
    Write-Host $LogMessage -ForegroundColor $(switch($Level) {"ERROR"{"Red"};"WARN"{"Yellow"};"INFO"{"Green"};"DEBUG"{"Gray"}})
    Add-Content "$LogDir\AutoDevelopment_$(Get-Date -Format 'yyyyMMdd').log" $LogMessage
}

# Check system environment
function Test-DevelopmentEnvironment {
    Write-Log "Checking development environment..." "INFO"
    
    # Check UE5
    $UEPath = Get-Command ue5editor -ErrorAction SilentlyContinue
    if ($UEPath) {
        Write-Log "UE5 Editor installed" "INFO"
    } else {
        Write-Log "UE5 Editor not found" "ERROR"
        return $false
    }
    
    # Check Visual Studio
    $VSPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
    if (Test-Path $VSPath) {
        Write-Log "Visual Studio 2022 installed" "INFO"
    } else {
        Write-Log "Visual Studio 2022 not found, will use UE5 editor" "WARN"
    }
    
    # Check Git
    $GitPath = Get-Command git -ErrorAction SilentlyContinue
    if ($GitPath) {
        Write-Log "Git installed" "INFO"
    } else {
        Write-Log "Git not installed" "ERROR"
        return $false
    }
    
    return $true
}

# Code quality check
function Invoke-CodeQualityCheck {
    Write-Log "Executing code quality check..." "INFO"
    
    Set-Location $ProjectRoot
    
    # Check code formatting
    Write-Log "Checking code format..." "DEBUG"
    $CppFiles = Get-ChildItem -Recurse -Filter "*.cpp" -Exclude "bin","obj","Temp"
    $HeaderFiles = Get-ChildItem -Recurse -Filter "*.h" -Exclude "bin","obj","Temp"
    
    $TotalFiles = $CppFiles.Count + $HeaderFiles.Count
    Write-Log "Found $TotalFiles source files" "INFO"
    
    # Check common issues
    $Issues = 0
    foreach ($file in ($CppFiles + $HeaderFiles)) {
        $content = Get-Content $file.FullName -Raw
        
        # Check TODO comments
        if ($content -match "TODO|FIXME|HACK") {
            Write-Log "Found TODO/FIXME/HACK: $($file.Name)" "WARN"
            $Issues++
        }
        
        # Check hardcoded paths
        if ($content -match "C:\\|D:\\") {
            Write-Log "Found hardcoded path: $($file.Name)" "WARN"
            $Issues++
        }
    }
    
    Write-Log "Code quality check completed, found $Issues issues" "INFO"
    return $Issues -eq 0
}

# Auto build
function Invoke-AutoBuild {
    if ($SkipBuild) {
        Write-Log "Skipping build step" "INFO"
        return $true
    }
    
    Write-Log "Starting auto build..." "INFO"
    
    $BuildScript = "$ProjectRoot\Build\DeveloperAutomation.ps1"
    if (Test-Path $BuildScript) {
        try {
            & $BuildScript -BuildMode $Target
            if ($LASTEXITCODE -eq 0) {
                Write-Log "Build successful" "INFO"
                return $true
            } else {
                Write-Log "Build failed" "ERROR"
                return $false
            }
        } catch {
            Write-Log "Build exception: $($_.Exception.Message)" "ERROR"
            return $false
        }
    } else {
        Write-Log "Build script not found: $BuildScript" "ERROR"
        return $false
    }
}

# Auto test
function Invoke-AutoTest {
    if ($SkipTest) {
        Write-Log "Skipping test step" "INFO"
        return $true
    }
    
    Write-Log "Starting auto test..." "INFO"
    
    $TestResults = @()
    
    # Run unit tests
    $TestScripts = Get-ChildItem -Path "$ProjectRoot\Tools\test" -Filter "*.ps1"
    foreach ($script in $TestScripts) {
        Write-Log "Running test: $($script.Name)" "DEBUG"
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
    
    # Generate test report
    $PassedTests = ($TestResults | Where-Object { $_.Status -eq "PASS" }).Count
    $TotalTests = $TestResults.Count
    $PassRate = if ($TotalTests -gt 0) { [math]::Round($PassedTests / $TotalTests * 100, 2) } else { 0 }
    
    Write-Log "Tests completed: $PassedTests/$TotalTests passed ($PassRate%)" "INFO"
    
    # Save test report
    $ReportPath = "$LogDir\TestReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $TestResults | ConvertTo-Json -Depth 3 | Out-File $ReportPath
    Write-Log "Test report saved: $ReportPath" "INFO"
    
    return $PassRate -ge 80
}

# Auto deploy
function Invoke-AutoDeploy {
    if ($SkipDeploy) {
        Write-Log "Skipping deploy step" "INFO"
        return $true
    }
    
    Write-Log "Starting auto deploy..." "INFO"
    
    # Create deployment package
    $DeployDir = "$BuildDir\Deploy\$(Get-Date -Format 'yyyyMMdd_HHmmss')"
    New-Item -ItemType Directory -Path $DeployDir -Force
    
    # Copy necessary files
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
            Write-Log "Copied: $file" "DEBUG"
        }
    }
    
    # Generate deployment info
    $DeployInfo = @{
        Timestamp = Get-Date
        Version = "1.0.0"
        Target = $Target
        Files = $FilesToDeploy
        Size = (Get-ChildItem $DeployDir -Recurse | Measure-Object -Property Length).Sum
    }
    
    $DeployInfo | ConvertTo-Json -Depth 3 | Out-File "$DeployDir\deploy_info.json"
    Write-Log "Deployment completed: $DeployDir" "INFO"
    
    return $true
}

# Git automation
function Invoke-GitAutomation {
    Write-Log "Executing Git automation..." "INFO"
    
    Set-Location $ProjectRoot
    
    # Check Git status
    $GitStatus = git status --porcelain
    if ($GitStatus) {
        Write-Log "Found uncommitted changes" "WARN"
        
        # Auto add changes
        git add .
        Write-Log "Added all changes" "INFO"
        
        # Auto commit (if there are changes)
        $CommitMessage = "Auto-commit $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
        git commit -m $CommitMessage
        Write-Log "Committed changes: $CommitMessage" "INFO"
    } else {
        Write-Log "No uncommitted changes" "INFO"
    }
    
    # Push to remote (optional)
    if ($Mode -eq "Full") {
        try {
            git push origin main
            Write-Log "Pushed to remote repository" "INFO"
        } catch {
            Write-Log "Push failed: $($_.Exception.Message)" "WARN"
        }
    }
}

# Performance monitoring
function Invoke-PerformanceMonitoring {
    Write-Log "Starting performance monitoring..." "INFO"
    
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
    
    Write-Log "Performance metrics recorded" "INFO"
}

# Generate development report
function New-DevelopmentReport {
    Write-Log "Generating development report..." "INFO"
    
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
    Write-Log "Development report generated: $ReportPath" "INFO"
}

# Main execution flow
function Main {
    $StartTime = Get-Date
    
    try {
        # 1. Environment check
        if (!(Test-DevelopmentEnvironment)) {
            Write-Log "Environment check failed, terminating execution" "ERROR"
            exit 1
        }
        
        # 2. Code quality check
        $script:CodeQualityResult = Invoke-CodeQualityCheck
        
        # 3. Auto build
        $BuildStartTime = Get-Date
        $script:BuildResult = Invoke-AutoBuild
        $script:BuildTime = (Get-Date) - $BuildStartTime
        
        # 4. Auto test
        $TestStartTime = Get-Date
        $script:TestResult = Invoke-AutoTest
        $script:TestTime = (Get-Date) - $TestStartTime
        
        # 5. Auto deploy
        $script:DeployResult = Invoke-AutoDeploy
        
        # 6. Git automation
        if ($Mode -eq "Full") {
            Invoke-GitAutomation
        }
        
        # 7. Performance monitoring
        Invoke-PerformanceMonitoring
        
        # 8. Generate report
        New-DevelopmentReport
        
        $TotalTime = (Get-Date) - $StartTime
        Write-Log "Automated development completed, total time: $($TotalTime.TotalMinutes) minutes" "INFO"
        
        # Determine overall success
        $OverallSuccess = $BuildResult -and $TestResult -and $DeployResult
        if ($OverallSuccess) {
            Write-Log "Automated development all successful!" "INFO"
            exit 0
        } else {
            Write-Log "Automated development partially failed" "ERROR"
            exit 1
        }
        
    } catch {
        Write-Log "Automated development exception: $($_.Exception.Message)" "ERROR"
        exit 1
    }
}

# Execute main program
Main
