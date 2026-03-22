# MingGoRTS 自動化弱點掃描系統 / Automated Vulnerability Scanning System
# 版本 / Version: 1.0
# 作者 / Author: MingGoRTS Security Team
# 最後更新 / Last Updated: 2026-03-23

param(
    [Parameter(Mandatory=$false)]
    [string]$ScanType = "Full",  # Full, Code, Dependencies, Quick
    
    [Parameter(Mandatory=$false)]
    [string]$OutputPath = "Reports\Security",
    
    [Parameter(Mandatory=$false)]
    [switch]$GenerateReport = $true,
    
    [Parameter(Mandatory=$false)]
    [switch]$FailOnCritical = $false,
    
    [Parameter(Mandatory=$false)]
    [string[]]$ExcludePaths = @(".git", ".venv", "Temp", "Logs", "Saved"),
    
    [Parameter(Mandatory=$false)]
    [int]$SeverityThreshold = 7  # 1-10, 7=High
)

# 設置嚴格模式 / Set strict mode
$ErrorActionPreference = "Stop"
$ProgressPreference = "Continue"

# 顏色定義 / Color definitions
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Critical = "Magenta"
}

# 弱點嚴重性級別 / Vulnerability severity levels
$SeverityLevels = @{
    1 = @{ Name = "Info"; Color = "Gray" }
    2 = @{ Name = "Low"; Color = "Green" }
    3 = @{ Name = "Low"; Color = "Green" }
    4 = @{ Name = "Medium"; Color = "Yellow" }
    5 = @{ Name = "Medium"; Color = "Yellow" }
    6 = @{ Name = "Medium"; Color = "Yellow" }
    7 = @{ Name = "High"; Color = "Orange" }
    8 = @{ Name = "High"; Color = "Red" }
    9 = @{ Name = "Critical"; Color = "Magenta" }
    10 = @{ Name = "Critical"; Color = "Magenta" }
}

# 掃描結果存儲 / Scan results storage
$Script:ScanResults = @{
    Timestamp = Get-Date
    ProjectName = "MingGoRTS"
    ScanType = $ScanType
    TotalFiles = 0
    IssuesFound = 0
    CriticalCount = 0
    HighCount = 0
    MediumCount = 0
    LowCount = 0
    InfoCount = 0
    ScannedFiles = @()
    Vulnerabilities = @()
    PerformanceMetrics = @{}
}

function Write-Log {
    param(
        [string]$Message,
        [string]$Level = "Info",
        [string]$Category = "General"
    )
    
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = $Colors[$Level]
    if (-not $color) { $color = "White" }
    
    Write-Host "[$timestamp] [$Level] [$Category] $Message" -ForegroundColor $color
    
    # 記錄到文件 / Log to file
    $logPath = Join-Path $OutputPath "scan.log"
    if (-not (Test-Path $OutputPath)) {
        New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    }
    "[$timestamp] [$Level] [$Category] $Message" | Out-File -FilePath $logPath -Append
}

function Initialize-ScanEnvironment {
    Write-Log "初始化弱點掃描環境 / Initializing vulnerability scan environment" -Level "Info" -Category "Setup"
    
    # 創建輸出目錄 / Create output directory
    if (-not (Test-Path $OutputPath)) {
        New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
        Write-Log "創建輸出目錄 / Created output directory: $OutputPath" -Level "Success" -Category "Setup"
    }
    
    # 檢查必要工具 / Check required tools
    $tools = @("cl.exe", "clang-tidy", "powershell")
    foreach ($tool in $tools) {
        $toolPath = Get-Command $tool -ErrorAction SilentlyContinue
        if ($toolPath) {
            Write-Log "找到工具 / Found tool: $tool at $($toolPath.Source)" -Level "Success" -Category "Setup"
        } else {
            Write-Log "警告: 未找到工具 / Warning: Tool not found: $tool" -Level "Warning" -Category "Setup"
        }
    }
    
    # 獲取項目路徑 / Get project path
    $Script:ProjectRoot = $PSScriptRoot | Split-Path -Parent | Split-Path -Parent
    Write-Log "項目根目錄 / Project root: $Script:ProjectRoot" -Level "Info" -Category "Setup"
}

function Invoke-CodeSecurityScan {
    Write-Log "開始代碼安全掃描 / Starting code security scan" -Level "Info" -Category "CodeScan"
    
    $codePatterns = @{
        # 危險函數 / Dangerous functions
        "DangerousFunctions" = @{
            Pattern = '(strcpy|strcat|sprintf|scanf|gets|system|exec|eval)\s*\('
            Severity = 8
            Category = "DangerousFunction"
            Description = "使用危險函數 / Using dangerous function"
        }
        
        # 硬編碼密碼 / Hardcoded passwords
        "HardcodedPassword" = @{
            Pattern = '(password|passwd|pwd|secret|key|token)\s*=\s*["\'][^"\']{4,}["\']'
            Severity = 9
            Category = "CredentialExposure"
            Description = "硬編碼憑證 / Hardcoded credential detected"
        }
        
        # SQL注入風險 / SQL injection risk
        "SQLInjection" = @{
            Pattern = '(SELECT|INSERT|UPDATE|DELETE|DROP).*\+.*\$|sprintf.*SELECT|Format.*SELECT'
            Severity = 9
            Category = "SQLInjection"
            Description = "潛在SQL注入風險 / Potential SQL injection risk"
        }
        
        # 緩衝區溢出 / Buffer overflow
        "BufferOverflow" = @{
            Pattern = 'char\s+\w+\[\d+\].*;\s*.*strcpy|memcpy\s*\(|memmove\s*\('
            Severity = 7
            Category = "BufferOverflow"
            Description = "潛在緩衝區溢出 / Potential buffer overflow"
        }
        
        # 不安全的隨機數 / Insecure random
        "InsecureRandom" = @{
            Pattern = 'rand\s*\(|srand\s*\('
            Severity = 5
            Category = "WeakCryptography"
            Description = "使用不安全的隨機數生成器 / Using insecure random generator"
        }
        
        # 路徑遍歷 / Path traversal
        "PathTraversal" = @{
            Pattern = '\.\./|\.\.\\\\|%2e%2e|%252e%252e'
            Severity = 7
            Category = "PathTraversal"
            Description = "潛在路徑遍歷風險 / Potential path traversal risk"
        }
        
        # 不安全的反序列化 / Insecure deserialization
        "InsecureDeserialization" = @{
            Pattern = 'Deserialize|Unserialize|LoadObject.*Binary'
            Severity = 8
            Category = "Deserialization"
            Description = "潛在不安全反序列化 / Potential insecure deserialization"
        }
        
        # 硬編碼IP / Hardcoded IP
        "HardcodedIP" = @{
            Pattern = '\b(?:[0-9]{1,3}\.){3}[0-9]{1,3}\b'
            Severity = 4
            Category = "Configuration"
            Description = "硬編碼IP地址 / Hardcoded IP address"
        }
        
        # 調試代碼 / Debug code
        "DebugCode" = @{
            Pattern = '(UE_LOG.*Warning|UE_LOG.*Error|printf|console\.log|DEBUG|TODO|FIXME|HACK)'
            Severity = 3
            Category = "CodeQuality"
            Description = "調試代碼或註釋 / Debug code or comments"
        }
        
        # 資源洩漏 / Resource leak
        "ResourceLeak" = @{
            Pattern = 'new\s+\w+.*;.*[^delete]|malloc\s*\(.*\).*[^free]'
            Severity = 6
            Category = "ResourceLeak"
            Description = "潛在資源洩漏 / Potential resource leak"
        }
        
        # 整數溢出 / Integer overflow
        "IntegerOverflow" = @{
            Pattern = 'int.*\*.*int|short.*\*.*short|malloc\s*\(\s*\w+\s*\*'
            Severity = 6
            Category = "IntegerOverflow"
            Description = "潛在整數溢出 / Potential integer overflow"
        }
        
        # 格式字符串漏洞 / Format string vulnerability
        "FormatString" = @{
            Pattern = 'printf\s*\(\s*[^"\']*\)|fprintf\s*\(\s*[^,]+,\s*[^"\']+\)'
            Severity = 7
            Category = "FormatString"
            Description = "格式字符串漏洞風險 / Format string vulnerability risk"
        }
        
        # 競態條件 / Race condition
        "RaceCondition" = @{
            Pattern = 'if\s*\(.*Exists.*\).*\{.*[^}]*\w+.*=.*Read|CheckThenAct'
            Severity = 6
            Category = "RaceCondition"
            Description = "潛在競態條件 / Potential race condition"
        }
        
        # 敏感數據記錄 / Sensitive data logging
        "SensitiveLogging" = @{
            Pattern = 'UE_LOG.*password|UE_LOG.*secret|UE_LOG.*token|UE_LOG.*key'
            Severity = 8
            Category = "SensitiveDataExposure"
            Description = "敏感數據可能被記錄 / Sensitive data may be logged"
        }
    }
    
    $sourceFiles = Get-ChildItem -Path "$Script:ProjectRoot\Source" -Recurse -Include "*.cpp", "*.h", "*.hpp" | 
        Where-Object { 
            $exclude = $false
            foreach ($path in $ExcludePaths) {
                if ($_.FullName -like "*$path*") { $exclude = $true; break }
            }
            -not $exclude
        }
    
    Write-Log "找到 $($sourceFiles.Count) 個源文件 / Found $($sourceFiles.Count) source files" -Level "Info" -Category "CodeScan"
    $Script:ScanResults.TotalFiles += $sourceFiles.Count
    
    foreach ($file in $sourceFiles) {
        $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $content) { continue }
        
        $relativePath = $file.FullName.Replace($Script:ProjectRoot, "").TrimStart("\")
        
        foreach ($patternName in $codePatterns.Keys) {
            $pattern = $codePatterns[$patternName]
            $matches = [regex]::Matches($content, $pattern.Pattern, [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
            
            foreach ($match in $matches) {
                $lines = $content.Substring(0, $match.Index).Split("`n").Count
                
                $vulnerability = @{
                    Type = $pattern.Category
                    Severity = $pattern.Severity
                    File = $relativePath
                    Line = $lines
                    Pattern = $patternName
                    Description = $pattern.Description
                    Code = $match.Value.Trim()
                    Timestamp = Get-Date
                }
                
                $Script:ScanResults.Vulnerabilities += $vulnerability
                
                # 更新計數 / Update counts
                switch ($pattern.Severity) {
                    { $_ -ge 9 } { $Script:ScanResults.CriticalCount++ }
                    { $_ -ge 7 -and $_ -lt 9 } { $Script:ScanResults.HighCount++ }
                    { $_ -ge 4 -and $_ -lt 7 } { $Script:ScanResults.MediumCount++ }
                    { $_ -ge 2 -and $_ -lt 4 } { $Script:ScanResults.LowCount++ }
                    default { $Script:ScanResults.InfoCount++ }
                }
                
                $severityName = $SeverityLevels[$pattern.Severity].Name
                $severityColor = $SeverityLevels[$pattern.Severity].Color
                
                Write-Log "[$severityName] $($pattern.Description) in $relativePath`:$lines" -Level $severityName -Category "CodeScan"
            }
        }
        
        $Script:ScanResults.ScannedFiles += $relativePath
    }
}

function Invoke-DependencyScan {
    Write-Log "開始依賴項安全掃描 / Starting dependency security scan" -Level "Info" -Category "DependencyScan"
    
    # 掃描第三方庫 / Scan third-party libraries
    $dependencyFiles = @(
        "$Script:ProjectRoot\*.uproject"
        "$Script:ProjectRoot\*.uplugin"
        "$Script:ProjectRoot\Plugins\*\*.uplugin"
    )
    
    foreach ($file in $dependencyFiles) {
        $files = Get-ChildItem -Path $file -ErrorAction SilentlyContinue
        foreach ($depFile in $files) {
            Write-Log "分析依賴文件 / Analyzing dependency file: $($depFile.Name)" -Level "Info" -Category "DependencyScan"
            
            # 檢查過時的引擎版本 / Check for outdated engine versions
            $content = Get-Content $depFile.FullName -Raw
            if ($content -match '"EngineVersion"\s*:\s*"([^"]+)"') {
                $engineVersion = $matches[1]
                Write-Log "檢測到引擎版本 / Detected engine version: $engineVersion" -Level "Info" -Category "DependencyScan"
                
                # 檢查是否使用最新版本 / Check if using latest version
                if ($engineVersion -notmatch "5\.[4-9]") {
                    $Script:ScanResults.Vulnerabilities += @{
                        Type = "OutdatedDependency"
                        Severity = 5
                        File = $depFile.FullName.Replace($Script:ProjectRoot, "").TrimStart("\")
                        Line = 0
                        Pattern = "OldEngineVersion"
                        Description = "使用較舊的Unreal Engine版本 / Using older Unreal Engine version: $engineVersion"
                        Code = "EngineVersion: $engineVersion"
                        Recommendation = "建議升級到UE 5.4或更高版本 / Consider upgrading to UE 5.4 or higher"
                    }
                    $Script:ScanResults.MediumCount++
                }
            }
        }
    }
}

function Invoke-ConfigurationScan {
    Write-Log "開始配置文件安全掃描 / Starting configuration security scan" -Level "Info" -Category "ConfigScan"
    
    $configFiles = Get-ChildItem -Path "$Script:ProjectRoot\Config" -Recurse -Include "*.ini", "*.json", "*.xml" -ErrorAction SilentlyContinue
    
    foreach ($file in $configFiles) {
        $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $content) { continue }
        
        $relativePath = $file.FullName.Replace($Script:ProjectRoot, "").TrimStart("\")
        
        # 檢查敏感配置 / Check for sensitive configurations
        $sensitivePatterns = @{
            "DebugMode" = @{
                Pattern = '(?i)(DebugMode\s*=\s*True|DevelopmentMode\s*=\s*True|bDebug\s*=\s*True)'
                Severity = 4
                Description = "調試模式可能啟用 / Debug mode may be enabled"
            }
            "VerboseLogging" = @{
                Pattern = '(?i)(Verbose|LogLevel\s*=\s*Debug|bLogVerbose\s*=\s*True)'
                Severity = 3
                Description = "詳細日誌記錄可能啟用 / Verbose logging may be enabled"
            }
            "WeakCrypto" = @{
                Pattern = '(?i)(EncryptionKey\s*=\s*["\'][^"\']{1,8}["\']|WeakEncryption|NoEncryption)'
                Severity = 8
                Description = "弱加密或硬編碼密鑰 / Weak encryption or hardcoded key"
            }
        }
        
        foreach ($patternName in $sensitivePatterns.Keys) {
            $pattern = $sensitivePatterns[$patternName]
            if ($content -match $pattern.Pattern) {
                $Script:ScanResults.Vulnerabilities += @{
                    Type = "Configuration"
                    Severity = $pattern.Severity
                    File = $relativePath
                    Line = 0
                    Pattern = $patternName
                    Description = $pattern.Description
                    Code = $matches[0]
                }
                
                $severityName = $SeverityLevels[$pattern.Severity].Name
                Write-Log "[$severityName] $($pattern.Description) in $relativePath" -Level $severityName -Category "ConfigScan"
                
                if ($pattern.Severity -ge 7) { $Script:ScanResults.HighCount++ }
                elseif ($pattern.Severity -ge 4) { $Script:ScanResults.MediumCount++ }
                else { $Script:ScanResults.LowCount++ }
            }
        }
    }
}

function Invoke-BlueprintScan {
    Write-Log "開始Blueprint安全掃描 / Starting Blueprint security scan" -Level "Info" -Category "BlueprintScan"
    
    $blueprintFiles = Get-ChildItem -Path "$Script:ProjectRoot\Content" -Recurse -Include "*.uasset", "*.umap" -ErrorAction SilentlyContinue | 
        Select-Object -First 100  # 限制數量以提高性能
    
    Write-Log "找到 $($blueprintFiles.Count) 個Blueprint資源 / Found $($blueprintFiles.Count) Blueprint assets" -Level "Info" -Category "BlueprintScan"
    
    foreach ($file in $blueprintFiles) {
        # 檢查文件大小（異常大的文件可能包含問題）/ Check file size
        if ($file.Length -gt 100MB) {
            $relativePath = $file.FullName.Replace($Script:ProjectRoot, "").TrimStart("\")
            $Script:ScanResults.Vulnerabilities += @{
                Type = "LargeAsset"
                Severity = 3
                File = $relativePath
                Line = 0
                Pattern = "OversizedBlueprint"
                Description = "異常大的Blueprint資源 / Unusually large Blueprint asset: $($file.Length / 1MB) MB"
                Code = "FileSize: $($file.Length)"
            }
            $Script:ScanResults.InfoCount++
        }
    }
}

function Export-SecurityReport {
    param(
        [string]$Format = "HTML"  # HTML, JSON, CSV, XML
    )
    
    Write-Log "生成安全報告 / Generating security report: $Format" -Level "Info" -Category "Reporting"
    
    $timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
    $reportFile = Join-Path $OutputPath "SecurityScan_Report_$timestamp.$($Format.ToLower())"
    
    switch ($Format) {
        "HTML" {
            $html = @"
<!DOCTYPE html>
<html>
<head>
    <title>MingGoRTS Security Scan Report</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; background: #f5f5f5; }
        .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; }
        .summary { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 30px; }
        .card { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .critical { border-left: 4px solid #e74c3c; }
        .high { border-left: 4px solid #e67e22; }
        .medium { border-left: 4px solid #f39c12; }
        .low { border-left: 4px solid #27ae60; }
        .info { border-left: 4px solid #95a5a6; }
        .vulnerability { background: white; margin: 10px 0; padding: 15px; border-radius: 5px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
        .severity-critical { color: #e74c3c; font-weight: bold; }
        .severity-high { color: #e67e22; font-weight: bold; }
        .severity-medium { color: #f39c12; font-weight: bold; }
        .severity-low { color: #27ae60; }
        .severity-info { color: #95a5a6; }
        table { width: 100%; border-collapse: collapse; margin: 20px 0; }
        th { background: #34495e; color: white; padding: 12px; text-align: left; }
        td { padding: 10px; border-bottom: 1px solid #ecf0f1; }
        tr:hover { background: #f8f9fa; }
        .code { background: #2c3e50; color: #ecf0f1; padding: 10px; border-radius: 4px; font-family: 'Courier New', monospace; font-size: 12px; overflow-x: auto; }
    </style>
</head>
<body>
    <div class="header">
        <h1>🔒 MingGoRTS Security Scan Report</h1>
        <p>Generated: $($Script:ScanResults.Timestamp)</p>
        <p>Scan Type: $($Script:ScanResults.ScanType)</p>
    </div>
    
    <div class="summary">
        <div class="card critical">
            <h3>Critical</h3>
            <h2>$($Script:ScanResults.CriticalCount)</h2>
        </div>
        <div class="card high">
            <h3>High</h3>
            <h2>$($Script:ScanResults.HighCount)</h2>
        </div>
        <div class="card medium">
            <h3>Medium</h3>
            <h2>$($Script:ScanResults.MediumCount)</h2>
        </div>
        <div class="card low">
            <h3>Low</h3>
            <h2>$($Script:ScanResults.LowCount)</h2>
        </div>
        <div class="card info">
            <h3>Info</h3>
            <h2>$($Script:ScanResults.InfoCount)</h2>
        </div>
        <div class="card">
            <h3>Total Files</h3>
            <h2>$($Script:ScanResults.TotalFiles)</h2>
        </div>
    </div>
    
    <h2>Vulnerability Details</h2>
    <table>
        <thead>
            <tr>
                <th>Severity</th>
                <th>Type</th>
                <th>File</th>
                <th>Line</th>
                <th>Description</th>
            </tr>
        </thead>
        <tbody>
"@
            
            # 按嚴重性排序 / Sort by severity
            $sortedVulns = $Script:ScanResults.Vulnerabilities | Sort-Object -Property Severity -Descending
            
            foreach ($vuln in $sortedVulns) {
                $severityClass = "severity-$($SeverityLevels[$vuln.Severity].Name.ToLower())"
                $html += @"
            <tr>
                <td class="$severityClass">$($SeverityLevels[$vuln.Severity].Name) ($($vuln.Severity))</td>
                <td>$($vuln.Type)</td>
                <td>$($vuln.File)</td>
                <td>$($vuln.Line)</td>
                <td>$($vuln.Description)</td>
            </tr>
"@
            }
            
            $html += @"
        </tbody>
    </table>
    
    <h2>Detailed Findings</h2>
"@
            
            foreach ($vuln in $sortedVulns | Select-Object -First 50) {
                $severityClass = "severity-$($SeverityLevels[$vuln.Severity].Name.ToLower())"
                $html += @"
    <div class="vulnerability">
        <h4 class="$severityClass">[$($SeverityLevels[$vuln.Severity].Name)] $($vuln.Type) - $($vuln.File):$($vuln.Line)</h4>
        <p><strong>Description:</strong> $($vuln.Description)</p>
        <div class="code">$($vuln.Code)</div>
    </div>
"@
            }
            
            $html += @"
</body>
</html>
"@
            
            $html | Out-File -FilePath $reportFile -Encoding UTF8
        }
        
        "JSON" {
            $Script:ScanResults | ConvertTo-Json -Depth 10 | Out-File -FilePath $reportFile -Encoding UTF8
        }
        
        "CSV" {
            $csvData = $Script:ScanResults.Vulnerabilities | ForEach-Object {
                [PSCustomObject]@{
                    Timestamp = $_.Timestamp
                    Severity = $_.Severity
                    Type = $_.Type
                    File = $_.File
                    Line = $_.Line
                    Description = $_.Description
                    Code = $_.Code -replace '"', '""'
                }
            }
            $csvData | Export-Csv -Path $reportFile -NoTypeInformation -Encoding UTF8
        }
    }
    
    Write-Log "報告已生成 / Report generated: $reportFile" -Level "Success" -Category "Reporting"
    return $reportFile
}

function Send-Notification {
    param(
        [string]$ReportPath
    )
    
    # 檢查是否有嚴重問題 / Check for critical issues
    if ($Script:ScanResults.CriticalCount -gt 0 -or $Script:ScanResults.HighCount -gt 0) {
        Write-Log "⚠️ 發現嚴重安全問題! / Critical security issues found!" -Level "Critical" -Category "Notification"
        Write-Log "關鍵: $($Script:ScanResults.CriticalCount), 高危: $($Script:ScanResults.HighCount) / Critical: $($Script:ScanResults.CriticalCount), High: $($Script:ScanResults.HighCount)" -Level "Critical" -Category "Notification"
        
        # 在這裡可以添加郵件通知、Slack通知等 / Add email, Slack notifications here
        # Send-EmailAlert -ReportPath $ReportPath
        # Send-SlackAlert -ReportPath $ReportPath
    } else {
        Write-Log "✅ 未發現嚴重安全問題 / No critical security issues found" -Level "Success" -Category "Notification"
    }
}

# 主執行流程 / Main execution flow
try {
    Write-Log "===============================================" -Level "Info" -Category "Main"
    Write-Log "MingGoRTS 自動化弱點掃描開始 / MingGoRTS Automated Vulnerability Scan Started" -Level "Info" -Category "Main"
    Write-Log "掃描類型 / Scan Type: $ScanType" -Level "Info" -Category "Main"
    Write-Log "嚴重性閾值 / Severity Threshold: $SeverityThreshold" -Level "Info" -Category "Main"
    Write-Log "===============================================" -Level "Info" -Category "Main"
    
    $scanStartTime = Get-Date
    
    Initialize-ScanEnvironment
    
    # 執行掃描 / Execute scans
    switch ($ScanType) {
        "Full" {
            Invoke-CodeSecurityScan
            Invoke-DependencyScan
            Invoke-ConfigurationScan
            Invoke-BlueprintScan
        }
        "Code" {
            Invoke-CodeSecurityScan
        }
        "Dependencies" {
            Invoke-DependencyScan
        }
        "Quick" {
            Invoke-CodeSecurityScan
        }
    }
    
    $scanEndTime = Get-Date
    $duration = $scanEndTime - $scanStartTime
    
    $Script:ScanResults.PerformanceMetrics = @{
        StartTime = $scanStartTime
        EndTime = $scanEndTime
        Duration = $duration.ToString()
        DurationSeconds = [math]::Round($duration.TotalSeconds, 2)
    }
    
    # 生成報告 / Generate reports
    if ($GenerateReport) {
        $htmlReport = Export-SecurityReport -Format "HTML"
        $jsonReport = Export-SecurityReport -Format "JSON"
        $csvReport = Export-SecurityReport -Format "CSV"
    }
    
    # 發送通知 / Send notifications
    Send-Notification -ReportPath $htmlReport
    
    # 輸出摘要 / Output summary
    Write-Log "===============================================" -Level "Info" -Category "Summary"
    Write-Log "掃描完成 / Scan completed" -Level "Success" -Category "Summary"
    Write-Log "耗時 / Duration: $($duration.ToString())" -Level "Info" -Category "Summary"
    Write-Log "總文件數 / Total files: $($Script:ScanResults.TotalFiles)" -Level "Info" -Category "Summary"
    Write-Log "發現問題 / Issues found: $($Script:ScanResults.Vulnerabilities.Count)" -Level "Info" -Category "Summary"
    Write-Log "  嚴重 / Critical: $($Script:ScanResults.CriticalCount)" -Level $(if ($Script:ScanResults.CriticalCount -gt 0) { "Critical" } else { "Info" }) -Category "Summary"
    Write-Log "  高危 / High: $($Script:ScanResults.HighCount)" -Level $(if ($Script:ScanResults.HighCount -gt 0) { "Error" } else { "Info" }) -Category "Summary"
    Write-Log "  中危 / Medium: $($Script:ScanResults.MediumCount)" -Level $(if ($Script:ScanResults.MediumCount -gt 0) { "Warning" } else { "Info" }) -Category "Summary"
    Write-Log "  低危 / Low: $($Script:ScanResults.LowCount)" -Level "Info" -Category "Summary"
    Write-Log "  信息 / Info: $($Script:ScanResults.InfoCount)" -Level "Info" -Category "Summary"
    Write-Log "===============================================" -Level "Info" -Category "Summary"
    
    # 如果發現關鍵問題且設置了失敗標誌 / Fail if critical issues found and flag is set
    if ($FailOnCritical -and ($Script:ScanResults.CriticalCount -gt 0 -or $Script:ScanResults.HighCount -gt 0)) {
        Write-Log "因發現嚴重問題而退出 / Exiting due to critical issues found" -Level "Error" -Category "Exit"
        exit 1
    }
    
    exit 0
} catch {
    Write-Log "掃描過程中發生錯誤 / Error during scan: $($_.Exception.Message)" -Level "Error" -Category "Error"
    Write-Log "錯誤詳情 / Error details: $($_.ScriptStackTrace)" -Level "Error" -Category "Error"
    exit 1
}
