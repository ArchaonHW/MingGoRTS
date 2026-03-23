# MingGoRTS 智能提交前Code Review腳本
# PowerShell Script

param(
    [string]$TargetPath = ".",
    [string]$FilePattern = "*.cpp;*.h",
    [switch]$AutoFix = $false,
    [switch]$Strict = $false,
    [switch]$Verbose = $false
)

# Configuration
$ProjectRoot = "C:\HW\MingGoRTS"
$LogPath = "$ProjectRoot\Logs\SmartCommitReview_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
$ReportPath = "$ProjectRoot\Reports\CodeReviewReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').html"

# Initialize logging
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# 獲取要檢查的文件
function Get-FilesToReview {
    Write-Log "獲取需要檢查的文件..." "INFO"
    
    $Files = @()
    
    # 獲取Git修改的文件
    try {
        $GitFiles = git diff --name-only --cached 2>$null
        if ($GitFiles) {
            foreach ($File in $GitFiles) {
                if ($File -match "\.(cpp|h|hpp|c)$") {
                    $Files += Join-Path $ProjectRoot $File
                }
            }
        }
    } catch {
        Write-Log "Git命令執行失敗，使用文件模式掃描" "WARNING"
    }
    
    # 如果沒有Git文件，掃描目標路徑
    if ($Files.Count -eq 0) {
        $TargetFullPath = Join-Path $ProjectRoot $TargetPath
        $Patterns = $FilePattern.Split(';')
        
        foreach ($Pattern in $Patterns) {
            $FoundFiles = Get-ChildItem -Path $TargetFullPath -Filter $Pattern -Recurse -File
            foreach ($File in $FoundFiles) {
                $Files += $File.FullName
            }
        }
    }
    
    Write-Log "找到 $($Files.Count) 個文件需要檢查" "INFO"
    return $Files
}

# 執行Code Review
function Start-CodeReview {
    param([array]$Files)
    
    Write-Log "開始執行Code Review..." "INFO"
    
    $TotalIssues = 0
    $CriticalIssues = 0
    $ErrorIssues = 0
    $WarningIssues = 0
    $InfoIssues = 0
    $AllIssues = @()
    
    foreach ($File in $Files) {
        Write-Log "檢查文件: $File" "INFO"
        
        $FileIssues = Review-SingleFile $File
        $AllIssues += $FileIssues
        
        # 統計問題
        foreach ($Issue in $FileIssues) {
            $TotalIssues++
            switch ($Issue.Severity) {
                "Critical" { $CriticalIssues++ }
                "Error" { $ErrorIssues++ }
                "Warning" { $WarningIssues++ }
                "Info" { $InfoIssues++ }
            }
        }
        
        Write-Log "  發現 $($FileIssues.Count) 個問題" "INFO"
    }
    
    # 生成報告
    $Report = @{
        TotalFiles = $Files.Count
        TotalIssues = $TotalIssues
        CriticalIssues = $CriticalIssues
        ErrorIssues = $ErrorIssues
        WarningIssues = $WarningIssues
        InfoIssues = $InfoIssues
        Issues = $AllIssues
        CanCommit = $false
    }
    
    # 評估提交資格
    $Report.CanCommit = Test-CommitReadiness $Report
    
    Write-Log "Code Review完成" "SUCCESS"
    Write-Log "  總文件數: $($Report.TotalFiles)" "INFO"
    Write-Log "  總問題數: $($Report.TotalIssues)" "INFO"
    Write-Log "  嚴重問題: $CriticalIssues" "INFO"
    Write-Log "  錯誤問題: $ErrorIssues" "INFO"
    Write-Log "  警告問題: $WarningIssues" "INFO"
    Write-Log "  信息問題: $InfoIssues" "INFO"
    Write-Log "  可以提交: $($Report.CanCommit)" $(if ($Report.CanCommit) { "SUCCESS" } else { "WARNING" })
    
    return $Report
}

# 檢查單個文件
function Review-SingleFile {
    param([string]$FilePath)
    
    $Issues = @()
    
    if (-not (Test-Path $FilePath)) {
        Write-Log "文件不存在: $FilePath" "ERROR"
        return $Issues
    }
    
    try {
        $Content = Get-Content -Path $FilePath -Raw -Encoding UTF8
        $Lines = $Content -split "`r`n"
        
        for ($i = 0; $i -lt $Lines.Count; $i++) {
            $Line = $Lines[$i]
            $LineNumber = $i + 1
            
            # 檢查語法問題
            $SyntaxIssues = Check-SyntaxIssues $FilePath $Line $LineNumber
            $Issues += $SyntaxIssues
            
            # 檢查代碼風格
            $StyleIssues = Check-StyleIssues $FilePath $Line $LineNumber
            $Issues += $StyleIssues
            
            # 檢查性能問題
            $PerfIssues = Check-PerformanceIssues $FilePath $Line $LineNumber
            $Issues += $PerfIssues
            
            # 檢查安全問題
            $SecurityIssues = Check-SecurityIssues $FilePath $Line $LineNumber
            $Issues += $SecurityIssues
            
            # 檢查文檔問題
            $DocIssues = Check-DocumentationIssues $FilePath $Line $LineNumber
            $Issues += $DocIssues
        }
        
    } catch {
        Write-Log "檢查文件時發生錯誤: $FilePath - $($_.Exception.Message)" "ERROR"
    }
    
    return $Issues
}

# 檢查語法問題
function Check-SyntaxIssues {
    param([string]$FilePath, [string]$Line, [int]$LineNumber)
    
    $Issues = @()
    
    # 檢查缺少分號
    if ($Line -match "^\s*[^/].*[^;]\s*$" -and $Line -notmatch "^\s*\{|^\s*\}|^\s*//|^\s*\*") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Syntax"
            Severity = "Error"
            Message = "行末缺少分號"
            Suggestion = "在行末添加分號"
            AutoFixable = $true
        }
    }
    
    # 檢查未閉合的括號
    if ($Line -match "\{" -and $Line -notmatch "\}") {
        # 這裡可以添加更複雜的括號匹配邏輯
    }
    
    return $Issues
}

# 檢查代碼風格問題
function Check-StyleIssues {
    param([string]$FilePath, [string]$Line, [int]$LineNumber)
    
    $Issues = @()
    
    # 檢查命名規範
    if ($Line -match "^\s*[a-zA-Z_][a-zA-Z0-9_]*\s+[a-z][a-zA-Z0-9]*\s*[;=]" -and $FilePath -match "\.cpp$") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Style"
            Severity = "Warning"
            Message = "變量命名不符合規範"
            Suggestion = "使用駝峰命名法或統一的命名規範"
            AutoFixable = $false
        }
    }
    
    # 檢查行長度
    if ($Line.Length -gt 120) {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Style"
            Severity = "Info"
            Message = "行長度超過120字符"
            Suggestion = "將長行分解為多行"
            AutoFixable = $false
        }
    }
    
    # 檢查Tab字符
    if ($Line -match "`t") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Style"
            Severity = "Warning"
            Message = "使用了Tab字符"
            Suggestion = "使用空格替代Tab字符"
            AutoFixable = $true
        }
    }
    
    return $Issues
}

# 檢查性能問題
function Check-PerformanceIssues {
    param([string]$FilePath, [string]$Line, [int]$LineNumber)
    
    $Issues = @()
    
    # 檢查低效的字符串操作
    if ($Line -match "\.Append\(" -or $Line -match "\+=.*\"") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Performance"
            Severity = "Warning"
            Message = "可能存在低效的字符串操作"
            Suggestion = "考慮使用FString::Printf或StringBuilder"
            AutoFixable = $false
        }
    }
    
    # 檢查可能的內存洩漏
    if ($Line -match "new\s+\w+" -and $Line -not-match "delete") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Performance"
            Severity = "Warning"
            Message = "可能的內存洩漏風險"
            Suggestion = "確保對應的delete操作"
            AutoFixable = $false
        }
    }
    
    return $Issues
}

# 檢查安全問題
function Check-SecurityIssues {
    param([string]$FilePath, [string]$Line, [int]$LineNumber)
    
    $Issues = @()
    
    # 檢查硬編碼密碼
    if ($Line -match "(?i)(password|pwd|pass)\s*=\s*[\"'][^\"']+[\"']") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Security"
            Severity = "Critical"
            Message = "檢測到硬編碼密碼"
            Suggestion = "使用安全的配置管理方式"
            AutoFixable = $false
        }
    }
    
    # 檢查SQL注入風險
    if ($Line -match "(?i)(execute|query).*\+.*\"") {
        $Issues += @{
            File = $FilePath
            Line = $LineNumber
            Type = "Security"
            Severity = "Error"
            Message = "可能的SQL注入風險"
            Suggestion = "使用參數化查詢"
            AutoFixable = $false
        }
    }
    
    return $Issues
}

# 檢查文檔問題
function Check-DocumentationIssues {
    param([string]$FilePath, [string]$Line, [int]$LineNumber)
    
    $Issues = @()
    
    # 檢查UFUNCTION文檔
    if ($Line -match "UFUNCTION.*" -and $LineNumber -gt 1) {
        $PrevLine = (Get-Content $FilePath)[$LineNumber - 2]
        if ($PrevLine -notmatch "^\s*///|^\s*\*|^\s*//") {
            $Issues += @{
                File = $FilePath
                Line = $LineNumber
                Type = "Documentation"
                Severity = "Warning"
                Message = "UFUNCTION缺少文檔註釋"
                Suggestion = "添加詳細的文檔註釋"
                AutoFixable = $false
            }
        }
    }
    
    return $Issues
}

# 測試提交資格
function Test-CommitReadiness {
    param($Report)
    
    Write-Log "評估提交資格..." "INFO"
    
    # 嚴重問題檢查
    if ($Report.CriticalIssues -gt 0) {
        Write-Log "發現 $($Report.CriticalIssues) 個嚴重問題，無法提交" "ERROR"
        return $false
    }
    
    # 錯誤問題檢查
    if ($Report.ErrorIssues -gt 5) {
        Write-Log "錯誤問題超過閾值 (5)，無法提交" "ERROR"
        return $false
    }
    
    # 嚴格模式檢查
    if ($Strict -and $Report.WarningIssues -gt 0) {
        Write-Log "嚴格模式下不允許警告問題，無法提交" "ERROR"
        return $false
    }
    
    # 問題總數檢查
    if ($Report.TotalIssues -gt 50) {
        Write-Log "問題總數過多，建議先修復部分問題" "WARNING"
        if ($Strict) {
            return $false
        }
    }
    
    Write-Log "代碼質量符合提交標準" "SUCCESS"
    return $true
}

# 自動修復問題
function Start-AutoFix {
    param($Report)
    
    if (-not $AutoFix) {
        Write-Log "自動修復已禁用" "INFO"
        return
    }
    
    Write-Log "開始自動修復..." "INFO"
    
    $FixedCount = 0
    
    foreach ($Issue in $Report.Issues) {
        if ($Issue.AutoFixable) {
            try {
                $Result = Fix-Issue $Issue
                if ($Result) {
                    $FixedCount++
                    Write-Log "  修復: $($Issue.File):$($Issue.Line) - $($Issue.Message)" "SUCCESS"
                }
            } catch {
                Write-Log "  修復失敗: $($Issue.File):$($Issue.Line) - $($_.Exception.Message)" "ERROR"
            }
        }
    }
    
    Write-Log "自動修復完成，修復了 $FixedCount 個問題" "SUCCESS"
}

# 修復單個問題
function Fix-Issue {
    param($Issue)
    
    $FilePath = $Issue.File
    $LineNumber = $Issue.Line
    
    if (-not (Test-Path $FilePath)) {
        return $false
    }
    
    $Content = Get-Content -Path $FilePath -Encoding UTF8
    $Line = $Content[$LineNumber - 1]
    
    # 根據問題類型進行修復
    switch ($Issue.Message) {
        "行末缺少分號" {
            $Content[$LineNumber - 1] = $Line.TrimEnd() + ";"
            Set-Content -Path $FilePath -Value $Content -Encoding UTF8
            return $true
        }
        "使用了Tab字符" {
            $Content[$LineNumber - 1] = $Line -replace "`t", "    "
            Set-Content -Path $FilePath -Value $Content -Encoding UTF8
            return $true
        }
        default {
            return $false
        }
    }
}

# 生成HTML報告
function New-HTMLReport {
    param($Report)
    
    $HTML = @"
<!DOCTYPE html>
<html>
<head>
    <title>MingGoRTS Code Review Report</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }
        .container { max-width: 1200px; margin: 0 auto; background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 20px; border-radius: 8px; margin-bottom: 20px; }
        .summary { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 20px; }
        .metric { background: #f8f9fa; padding: 15px; border-radius: 8px; text-align: center; border-left: 4px solid #007bff; }
        .metric.critical { border-left-color: #dc3545; }
        .metric.error { border-left-color: #fd7e14; }
        .metric.warning { border-left-color: #ffc107; }
        .metric.info { border-left-color: #17a2b8; }
        .metric.success { border-left-color: #28a745; }
        .issues { margin-top: 20px; }
        .issue { background: #f8f9fa; margin: 10px 0; padding: 15px; border-radius: 8px; border-left: 4px solid #007bff; }
        .issue.critical { border-left-color: #dc3545; }
        .issue.error { border-left-color: #fd7e14; }
        .issue.warning { border-left-color: #ffc107; }
        .issue.info { border-left-color: #17a2b8; }
        .severity { display: inline-block; padding: 2px 8px; border-radius: 4px; font-size: 12px; font-weight: bold; color: white; }
        .severity.critical { background-color: #dc3545; }
        .severity.error { background-color: #fd7e14; }
        .severity.warning { background-color: #ffc107; color: #212529; }
        .severity.info { background-color: #17a2b8; }
        .file-path { font-family: monospace; background: #e9ecef; padding: 2px 6px; border-radius: 4px; }
        .can-commit { font-size: 18px; font-weight: bold; padding: 10px; border-radius: 8px; text-align: center; margin: 20px 0; }
        .can-commit.yes { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .can-commit.no { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔍 MingGoRTS Code Review Report</h1>
            <p>生成時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')</p>
            <p>檢查模式: $(if ($Strict) { '嚴格模式' } else { '標準模式' })</p>
        </div>
        
        <div class="can-commit $(if ($Report.CanCommit) { 'yes' } else { 'no' })">
            $(if ($Report.CanCommit) { '✅ 代碼可以提交' } else { '❌ 代碼需要修復後才能提交' })
        </div>
        
        <div class="summary">
            <div class="metric">
                <h3>$($Report.TotalFiles)</h3>
                <p>檢查文件</p>
            </div>
            <div class="metric">
                <h3>$($Report.TotalIssues)</h3>
                <p>總問題數</p>
            </div>
            <div class="metric critical">
                <h3>$($Report.CriticalIssues)</h3>
                <p>嚴重問題</p>
            </div>
            <div class="metric error">
                <h3>$($Report.ErrorIssues)</h3>
                <p>錯誤問題</p>
            </div>
            <div class="metric warning">
                <h3>$($Report.WarningIssues)</h3>
                <p>警告問題</p>
            </div>
            <div class="metric info">
                <h3>$($Report.InfoIssues)</h3>
                <p>信息問題</p>
            </div>
        </div>
        
        <div class="issues">
            <h2>📋 問題詳情</h2>
"@
    
    # 添加問題詳情
    foreach ($Issue in $Report.Issues) {
        $HTML += @"
            <div class="issue $($Issue.Severity.ToLower())">
                <div class="severity $($Issue.Severity.ToLower())">$($Issue.Severity.ToUpper())</div>
                <h4>$($Issue.Message)</h4>
                <p><strong>文件:</strong> <span class="file-path">$($Issue.File)</span> : $($Issue.Line)</p>
                <p><strong>類型:</strong> $($Issue.Type)</p>
                <p><strong>建議:</strong> $($Issue.Suggestion)</p>
                $(if ($Issue.AutoFixable) { '<p><em>✨ 可自動修復</em></p>' })
            </div>
"@
    }
    
    $HTML += @"
        </div>
    </div>
</body>
</html>
"@
    
    # 確保報告目錄存在
    $ReportDir = Split-Path -Parent $ReportPath
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    Set-Content -Path $ReportPath -Value $HTML -Encoding UTF8
    Write-Log "HTML報告已生成: $ReportPath" "SUCCESS"
}

# 主執行函數
function Main {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS 智能Code Review開始" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "參數:" "INFO"
    Write-Log "  目標路徑: $TargetPath" "INFO"
    Write-Log "  文件模式: $FilePattern" "INFO"
    Write-Log "  自動修復: $AutoFix" "INFO"
    Write-Log "  嚴格模式: $Strict" "INFO"
    Write-Log "" "INFO"
    
    try {
        # 獲取文件
        $Files = Get-FilesToReview
        
        if ($Files.Count -eq 0) {
            Write-Log "沒有找到需要檢查的文件" "WARNING"
            exit 0
        }
        
        # 執行Code Review
        $Report = Start-CodeReview $Files
        
        # 自動修復
        if ($AutoFix) {
            Start-AutoFix $Report
            
            # 重新檢查修復後的結果
            Write-Log "重新檢查修復後的代碼..." "INFO"
            $Report = Start-CodeReview $Files
        }
        
        # 生成報告
        New-HTMLReport $Report
        
        Write-Log "" "INFO"
        Write-Log "Code Review完成!" "SUCCESS"
        Write-Log "日誌文件: $LogPath" "INFO"
        Write-Log "報告文件: $ReportPath" "INFO"
        
        # 根據結果設置退出代碼
        if ($Report.CanCommit) {
            Write-Log "✅ 代碼可以提交" "SUCCESS"
            exit 0
        } else {
            Write-Log "❌ 代碼需要修復後才能提交" "ERROR"
            exit 1
        }
        
    } catch {
        Write-Log "Code Review過程中發生錯誤: $($_.Exception.Message)" "ERROR"
        exit 2
    }
}

# 執行主函數
Main
