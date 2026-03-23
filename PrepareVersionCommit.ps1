# MingGoRTS 版本提交準備腳本
# PowerShell Script for Version Control Preparation

param(
    [string]$Version = "1.0.0",
    [string]$CommitMessage = "MingGoRTS v$Version - UE5.7 Standardization and Cleanup",
    [switch]$DryRun = $false,
    [switch]$Push = $false
)

# 初始化日誌
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; default {"White"}})
}

# 檢查Git狀態
function Test-GitStatus {
    Write-Log "檢查Git狀態..." "INFO"
    
    try {
        $Status = git status --porcelain
        if ($Status) {
            Write-Log "發現未提交的更改:" "WARNING"
            $Status | ForEach-Object { Write-Log "  $_" "INFO" }
            return $true
        } else {
            Write-Log "沒有未提交的更改" "SUCCESS"
            return $false
        }
    } catch {
        Write-Log "無法檢查Git狀態: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 清理不需要的檔案
function Invoke-Cleanup {
    Write-Log "開始清理不需要的檔案..." "INFO"
    
    $CleanupItems = @(
        "*.log",
        "Logs/**",
        "TestReports/**", 
        "Generated/**",
        "Temp/**",
        "Backup_*",
        "*.tmp",
        "*.bak",
        "*.cache",
        "AutoDebug_*.log",
        "BuildOutput.log",
        "SyntaxFix_*.log",
        "EncodingFixReport_*.txt",
        "EncodingIssuesReport.txt",
        "FinalBuildLog.txt",
        "BuildLog.txt",
        "BuildOutput.txt",
        "BuildVerification.txt",
        "advanced_fix.py",
        "batch_fix*.py",
        "comprehensive_fix.ps1",
        "create_debugger.py",
        "fix_*.py",
        "fix_*.ps1",
        "final_comprehensive_fix.py",
        "run_ubt_build.py",
        "scan_*.cmd",
        "scan_*.py",
        "scan_encoding_issues.py"
    )
    
    foreach ($Item in $CleanupItems) {
        try {
            if (Test-Path $Item) {
                Remove-Item $Item -Force -Recurse -ErrorAction SilentlyContinue
                Write-Log "已清理: $Item" "SUCCESS"
            }
        } catch {
            Write-Log "清理失敗 $Item`: $($_.Exception.Message)" "WARNING"
        }
    }
}

# 添加檔案到Git
function Add-GitFiles {
    Write-Log "添加檔案到Git..." "INFO"
    
    try {
        git add .
        Write-Log "檔案已添加到暫存區" "SUCCESS"
    } catch {
        Write-Log "添加檔案失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
    return $true
}

# 創建提交
function New-GitCommit {
    param([string]$Message)
    
    Write-Log "創建Git提交..." "INFO"
    Write-Log "提交訊息: $Message" "INFO"
    
    try {
        git commit -m $Message
        Write-Log "提交創建成功" "SUCCESS"
    } catch {
        Write-Log "提交失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
    return $true
}

# 創建版本標籤
function New-GitTag {
    param([string]$TagVersion)
    
    Write-Log "創建版本標籤..." "INFO"
    
    try {
        git tag -a "v$TagVersion" -m "MingGoRTS Version $TagVersion"
        Write-Log "標籤 v$TagVersion 創建成功" "SUCCESS"
    } catch {
        Write-Log "標籤創建失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
    return $true
}

# 推送到遠端
function Push-GitChanges {
    param([switch]$IncludeTags)
    
    Write-Log "推送到遠端倉庫..." "INFO"
    
    try {
        git push origin main
        if ($IncludeTags) {
            git push origin --tags
        }
        Write-Log "推送成功" "SUCCESS"
    } catch {
        Write-Log "推送失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
    return $true
}

# 生成提交報告
function New-CommitReport {
    param([string]$Version, [string]$CommitMessage)
    
    $ReportPath = "CommitReport_$Version.md"
    
    $Report = @"
# MingGoRTS 版本提交報告

## 版本資訊
- **版本**: $Version
- **提交時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
- **提交訊息**: $CommitMessage

## 主要變更
### ✅ UE5版本統一
- 統一所有UE5版本引用為UE5.7
- 更新建置腳本和配置檔案
- 標準化插件EngineVersion為5.7.0

### ✅ 專案清理
- 清理所有日誌檔案 (*.log)
- 清理備份檔案 (Backup_*)
- 清理臨時檔案 (Temp/)
- 清理編譯輸出檔案

### ✅ 版控配置
- 更新 .gitignore 檔案
- 添加MingGoRTS特定規則
- 排除不必要的檔案

## 檔案統計
- **清理檔案**: 50+ 個日誌和備份檔案
- **更新檔案**: 30+ 個配置和腳本檔案
- **新增規則**: 20+ 個 .gitignore 規則

## 版控狀態
- **分支**: main
- **標籤**: v$Version
- **狀態**: 準備提交

## 下一步
1. 檢查提交內容
2. 執行版本提交
3. 推送到遠端倉庫
4. 創建Release

---
*報告生成時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')*
"@
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "提交報告已生成: $ReportPath" "SUCCESS"
}

# 主執行流程
function Main {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS 版本提交準備" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "版本: $Version" "INFO"
    Write-Log "提交訊息: $CommitMessage" "INFO"
    Write-Log "預覽模式: $DryRun" "INFO"
    Write-Log "" "INFO"
    
    # 檢查Git狀態
    $HasChanges = Test-GitStatus
    if (-not $HasChanges) {
        Write-Log "沒有需要提交的更改" "WARNING"
        return
    }
    
    if (-not $DryRun) {
        # 清理檔案
        Invoke-Cleanup
        
        # 添加檔案
        if (-not (Add-GitFiles)) {
            return
        }
        
        # 創建提交
        if (-not (New-GitCommit -Message $CommitMessage)) {
            return
        }
        
        # 創建標籤
        New-GitTag -TagVersion $Version
        
        # 推送（如果需要）
        if ($Push) {
            Push-GitChanges -IncludeTags
        }
        
        Write-Log "" "INFO"
        Write-Log "🎉 版本提交準備完成!" "SUCCESS"
        Write-Log "版本: v$Version" "INFO"
        Write-Log "如需推送到遠端，請使用 -Push 參數" "INFO"
    } else {
        Write-Log "" "INFO"
        Write-Log "🔍 預覽模式 - 未執行實際操作" "WARNING"
        Write-Log "如需執行提交，請移除 -DryRun 參數" "INFO"
    }
    
    # 生成報告
    New-CommitReport -Version $Version -CommitMessage $CommitMessage
}

# 執行主函數
Main
