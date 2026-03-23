# MingGoRTS 版本控制API系統
# PowerShell Module for Version Control API

# 版本控制API模組
$MingVersionAPI = @{
    Name = "MingGoRTS Version Control API"
    Version = "1.0.0"
    Description = "Complete version control system for MingGoRTS project"
    Author = "MingGoRTS Development Team"
    Created = "2026-03-23"
}

# 全域變數
$Global:MingVCConfig = $null
$Global:MingVCState = $null
$Global:MingVCHistory = @()

# 初始化版控系統
function Initialize-MingVersionControl {
    param(
        [string]$ProjectPath = "C:\HW\MingGoRTS",
        [string]$ConfigPath = "Config\VersionControl.json",
        [switch]$Force
    )
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "初始化 MingGoRTS 版本控制系統" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        # 檢查Git倉庫
        if (-not (Test-Path (Join-Path $ProjectPath ".git"))) {
            Write-Host "錯誤: 目錄不是Git倉庫: $ProjectPath" -ForegroundColor Red
            return $false
        }
        
        # 載入配置
        $ConfigFile = Join-Path $ProjectPath $ConfigPath
        if (Test-Path $ConfigFile) {
            $Global:MingVCConfig = Get-Content $ConfigFile | ConvertFrom-Json
            Write-Host "配置已載入: $ConfigFile" -ForegroundColor Green
        } else {
            # 創建默認配置
            $Global:MingVCConfig = @{
                ProjectPath = $ProjectPath
                BranchStrategy = @{
                    Main = "main"
                    Develop = "develop" 
                    Feature = "feature/"
                    Hotfix = "hotfix/"
                    Release = "release/"
                }
                TagStrategy = @{
                    Pattern = "v{major}.{minor}.{patch}"
                    AutoTag = $true
                    PreRelease = $false
                }
                CommitStrategy = @{
                    AutoFormat = $true
                    ValidateMessage = $true
                    RequireIssue = $false
                    MaxMessageLength = 100
                }
                BackupStrategy = @{
                    Enabled = $true
                    Location = "Backups\VersionControl"
                    MaxBackups = 10
                    Compress = $true
                }
                Integration = @{
                    CIEnabled = $true
                    CIVendor = "GitHub"
                    AutoDeploy = $false
                    NotificationEnabled = $true
                }
            }
            
            # 保存配置
            $ConfigDir = Split-Path $ConfigFile -Parent
            if (-not (Test-Path $ConfigDir)) {
                New-Item -ItemType Directory -Path $ConfigDir -Force | Out-Null
            }
            $Global:MingVCConfig | ConvertTo-Json -Depth 10 | Set-Content $ConfigFile
            Write-Host "默認配置已創建: $ConfigFile" -ForegroundColor Yellow
        }
        
        # 初始化狀態
        $Global:MingVCState = @{
            CurrentBranch = (git rev-parse --abbrev-ref HEAD)
            LastCommit = (git log -1 --format="%H")
            PendingChanges = (git status --porcelain)
            StagedFiles = (git diff --cached --name-only)
            ModifiedFiles = (git diff --name-only)
            UntrackedFiles = (git ls-files --others --exclude-standard)
            LastTag = (git describe --tags --abbrev=0)
            NextVersion = $null
            BuildNumber = 1
        }
        
        Write-Host "版控系統初始化完成" -ForegroundColor Green
        Write-Host "當前分支: $($Global:MingVCState.CurrentBranch)" -ForegroundColor Cyan
        Write-Host "最後提交: $($Global:MingVCState.LastCommit.Substring(0,8))" -ForegroundColor Cyan
        
        return $true
    } catch {
        Write-Host "初始化失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 獲取版控狀態
function Get-MingVCStatus {
    param(
        [switch]$Detailed,
        [switch]$Json
    )
    
    if (-not $Global:MingVCState) {
        Write-Host "版控系統未初始化" -ForegroundColor Red
        return $false
    }
    
    # 更新狀態
    $Global:MingVCState.CurrentBranch = git rev-parse --abbrev-ref HEAD
    $Global:MingVCState.PendingChanges = git status --porcelain
    $Global:MingVCState.StagedFiles = git diff --cached --name-only
    $Global:MingVCState.ModifiedFiles = git diff --name-only
    $Global:MingVCState.UntrackedFiles = git ls-files --others --exclude-standard
    
    if ($Json) {
        return $Global:MingVCState | ConvertTo-Json -Depth 10
    }
    
    if ($Detailed) {
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "MingGoRTS 版控狀態報告" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "分支資訊:" -ForegroundColor Yellow
        Write-Host "  當前分支: $($Global:MingVCState.CurrentBranch)" -ForegroundColor White
        Write-Host "  最後提交: $($Global:MingVCState.LastCommit.Substring(0,8))" -ForegroundColor White
        Write-Host "  最後標籤: $($Global:MingVCState.LastTag)" -ForegroundColor White
        Write-Host ""
        Write-Host "變更狀態:" -ForegroundColor Yellow
        Write-Host "  待提交變更: $($Global:MingVCState.PendingChanges.Count)" -ForegroundColor White
        Write-Host "  已暫存檔案: $($Global:MingVCState.StagedFiles.Count)" -ForegroundColor White
        Write-Host "  修改檔案: $($Global:MingVCState.ModifiedFiles.Count)" -ForegroundColor White
        Write-Host "  未追蹤檔案: $($Global:MingVCState.UntrackedFiles.Count)" -ForegroundColor White
        
        if ($Global:MingVCState.PendingChanges) {
            Write-Host ""
            Write-Host "變更詳情:" -ForegroundColor Yellow
            $Global:MingVCState.PendingChanges | ForEach-Object {
                $Status = $_.Substring(0,2)
                $File = $_.Substring(2)
                $Color = switch ($Status) {
                    "M " { "Yellow" }
                    " M" { "Yellow" }
                    "A " { "Green" }
                    "D " { "Red" }
                    "?? " { "Cyan" }
                    default { "White" }
                }
                Write-Host "  $Status $File" -ForegroundColor $Color
            }
        }
    } else {
        # 簡潔狀態
        $StatusColor = if ($Global:MingVCState.PendingChanges) { "Yellow" } else { "Green" }
        $StatusText = if ($Global:MingVCState.PendingChanges) { "有變更待提交" } else { "乾淨" }
        
        Write-Host "分支: $($Global:MingVCState.CurrentBranch) | 狀態: $StatusText" -ForegroundColor $StatusColor
        if ($Global:MingVCState.PendingChanges.Count -gt 0) {
            Write-Host "變更: $($Global:MingVCState.PendingChanges.Count) 個檔案" -ForegroundColor Cyan
        }
    }
    
    return $Global:MingVCState
}

# 智能提交
function New-MingVCCommit {
    param(
        [string]$Message,
        [string]$Type = "feat",
        [string]$Scope = "",
        [string]$Issue = "",
        [switch]$AutoStage,
        [switch]$SkipValidation,
        [switch]$DryRun
    )
    
    if (-not $Global:MingVCConfig) {
        Write-Host "版控系統未初始化" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS 智能提交" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        # 檢查是否有變更
        $Changes = git status --porcelain
        if (-not $Changes) {
            Write-Host "沒有變更需要提交" -ForegroundColor Yellow
            return $false
        }
        
        # 自動暫存
        if ($AutoStage) {
            Write-Host "自動暫存所有變更..." -ForegroundColor Cyan
            git add .
            Write-Host "暫存完成" -ForegroundColor Green
        }
        
        # 格式化提交訊息
        if ($Global:MingVCConfig.CommitStrategy.AutoFormat -and -not $SkipValidation) {
            $FormattedMessage = Format-MingVCCommitMessage -Message $Message -Type $Type -Scope $Scope -Issue $Issue
        } else {
            $FormattedMessage = $Message
        }
        
        # 驗證提交訊息
        if ($Global:MingVCConfig.CommitStrategy.ValidateMessage -and -not $SkipValidation) {
            if (-not (Test-MingVCCommitMessage -Message $FormattedMessage)) {
                Write-Host "提交訊息驗證失敗" -ForegroundColor Red
                return $false
            }
        }
        
        Write-Host "提交訊息: $FormattedMessage" -ForegroundColor Cyan
        
        if (-not $DryRun) {
            # 執行提交
            git commit -m $FormattedMessage
            
            # 記錄到歷史
            $CommitRecord = @{
                Timestamp = Get-Date
                Hash = (git log -1 --format="%H")
                Message = $FormattedMessage
                Author = (git log -1 --format="%an")
                Branch = $Global:MingVCState.CurrentBranch
                FilesChanged = $Changes.Count
            }
            $Global:MingVCHistory += $CommitRecord
            
            Write-Host "提交成功!" -ForegroundColor Green
            Write-Host "提交哈希: $($CommitRecord.Hash.Substring(0,8))" -ForegroundColor Cyan
            
            # 更新狀態
            $Global:MingVCState.LastCommit = $CommitRecord.Hash
            $Global:MingVCState.PendingChanges = @()
            
            return $true
        } else {
            Write-Host "預覽模式 - 未執行實際提交" -ForegroundColor Yellow
            return $true
        }
    } catch {
        Write-Host "提交失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 格式化提交訊息
function Format-MingVCCommitMessage {
    param(
        [string]$Message,
        [string]$Type = "feat",
        [string]$Scope = "",
        [string]$Issue = ""
    )
    
    # 支持的提交類型
    $ValidTypes = @("feat", "fix", "docs", "style", "refactor", "test", "chore", "perf", "ci", "build", "revert")
    
    if ($Type -notin $ValidTypes) {
        Write-Host "警告: 不支援的提交類型 '$Type'，使用默認 'feat'" -ForegroundColor Yellow
        $Type = "feat"
    }
    
    $FormattedMessage = "$Type"
    
    if ($Scope) {
        $FormattedMessage += "($Scope)"
    }
    
    if ($Issue) {
        $FormattedMessage += " #$Issue"
    }
    
    $FormattedMessage += ": $Message"
    
    # 長度檢查
    if ($FormattedMessage.Length -gt $Global:MingVCConfig.CommitStrategy.MaxMessageLength) {
        Write-Host "警告: 提交訊息過長，將截斷" -ForegroundColor Yellow
        $FormattedMessage = $FormattedMessage.Substring(0, $Global:MingVCConfig.CommitStrategy.MaxMessageLength - 3) + "..."
    }
    
    return $FormattedMessage
}

# 驗證提交訊息
function Test-MingVCCommitMessage {
    param([string]$Message)
    
    # 基本格式檢查
    if (-not $Message -or $Message.Trim().Length -eq 0) {
        Write-Host "錯誤: 提交訊息不能為空" -ForegroundColor Red
        return $false
    }
    
    # 長度檢查
    if ($Message.Length -gt $Global:MingVCConfig.CommitStrategy.MaxMessageLength) {
        Write-Host "錯誤: 提交訊息長度超過限制 ($($Global:MingVCConfig.CommitStrategy.MaxMessageLength) 字符)" -ForegroundColor Red
        return $false
    }
    
    # 格式檢查 (Conventional Commits)
    if ($Message -match '^(feat|fix|docs|style|refactor|test|chore|perf|ci|build|revert)(\(.+\))?: .+') {
        return $true
    } else {
        Write-Host "警告: 提交訊息不符合 Conventional Commits 格式" -ForegroundColor Yellow
        Write-Host "建議格式: <type>(<scope>): <description>" -ForegroundColor Cyan
        Write-Host "例如: feat(ui): add language selection widget" -ForegroundColor Cyan
        return $true  # 警告但不阻止
    }
}

# 創建版本標籤
function New-MingVCTag {
    param(
        [string]$Version,
        [string]$Message = "",
        [switch]$AutoIncrement,
        [switch]$Push,
        [switch]$DryRun
    )
    
    if (-not $Global:MingVCConfig) {
        Write-Host "版控系統未初始化" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS 版本標籤創建" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        # 自動增量版本
        if ($AutoIncrement) {
            $CurrentVersion = Get-MingVCCurrentVersion
            $Version = Increment-MingVCVersion -Version $CurrentVersion
            Write-Host "自動增量版本: $CurrentVersion → $Version" -ForegroundColor Cyan
        }
        
        # 驗證版本格式
        if (-not (Test-MingVCVersion -Version $Version)) {
            Write-Host "錯誤: 版本格式無效" -ForegroundColor Red
            return $false
        }
        
        # 檢查標籤是否已存在
        $ExistingTag = git tag -l $Version
        if ($ExistingTag) {
            Write-Host "錯誤: 標籤 '$Version' 已存在" -ForegroundColor Red
            return $false
        }
        
        # 默認訊息
        if (-not $Message) {
            $Message = "MingGoRTS Version $Version"
        }
        
        Write-Host "創建標籤: $Version" -ForegroundColor Cyan
        Write-Host "標籤訊息: $Message" -ForegroundColor Cyan
        
        if (-not $DryRun) {
            # 創建標籤
            git tag -a $Version -m $Message
            
            Write-Host "標籤創建成功!" -ForegroundColor Green
            
            # 推送標籤
            if ($Push) {
                Write-Host "推送標籤到遠端..." -ForegroundColor Cyan
                git push origin $Version
                Write-Host "標籤推送完成" -ForegroundColor Green
            }
            
            # 更新狀態
            $Global:MingVCState.LastTag = $Version
            
            return $true
        } else {
            Write-Host "預覽模式 - 未執行實際操作" -ForegroundColor Yellow
            return $true
        }
    } catch {
        Write-Host "標籤創建失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 獲取當前版本
function Get-MingVCCurrentVersion {
    try {
        $LatestTag = git describe --tags --abbrev=0 2>$null
        if ($LatestTag) {
            return $LatestTag.TrimStart('v')
        } else {
            return "0.0.0"
        }
    } catch {
        return "0.0.0"
    }
}

# 增量版本
function Increment-MingVCVersion {
    param(
        [string]$Version = "0.0.0",
        [ValidateSet("major", "minor", "patch")]
        [string]$Increment = "patch"
    )
    
    if ($Version -notmatch '^(\d+)\.(\d+)\.(\d+)(?:-.+)?$') {
        Write-Host "錯誤: 無效的版本格式" -ForegroundColor Red
        return $Version
    }
    
    $Major = [int]$Matches[1]
    $Minor = [int]$Matches[2]
    $Patch = [int]$Matches[3]
    
    switch ($Increment) {
        "major" {
            $Major++
            $Minor = 0
            $Patch = 0
        }
        "minor" {
            $Minor++
            $Patch = 0
        }
        "patch" {
            $Patch++
        }
    }
    
    return "$Major.$Minor.$Patch"
}

# 驗證版本格式
function Test-MingVCVersion {
    param([string]$Version)
    
    $Pattern = $Global:MingVCConfig.TagStrategy.Pattern.Replace('{major}', '\d+').Replace('{minor}', '\d+').Replace('{patch}', '\d+')
    return $Version -match $Pattern
}

# 分支管理
function Switch-MingVCBranch {
    param(
        [string]$BranchName,
        [switch]$Create,
        [switch]$Force
    )
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS 分支管理" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        if ($Create) {
            Write-Host "創建並切換到分支: $BranchName" -ForegroundColor Cyan
            git checkout -b $BranchName
        } else {
            Write-Host "切換到分支: $BranchName" -ForegroundColor Cyan
            git checkout $BranchName
        }
        
        # 更新狀態
        $Global:MingVCState.CurrentBranch = $BranchName
        
        Write-Host "分支切換成功!" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "分支操作失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 備份管理
function Backup-MingVCRepository {
    param(
        [string]$BackupPath = "",
        [switch]$Compress,
        [switch]$IncludeHistory
    )
    
    if (-not $Global:MingVCConfig.BackupStrategy.Enabled) {
        Write-Host "備份功能未啟用" -ForegroundColor Yellow
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS 版控備份" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        $ProjectPath = $Global:MingVCConfig.ProjectPath
        $Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
        
        if (-not $BackupPath) {
            $BackupPath = Join-Path $ProjectPath $Global:MingVCConfig.BackupStrategy.Location
        }
        
        if (-not (Test-Path $BackupPath)) {
            New-Item -ItemType Directory -Path $BackupPath -Force | Out-Null
        }
        
        $BackupName = "MingGoRTS_Backup_$Timestamp"
        $BackupDir = Join-Path $BackupPath $BackupName
        
        Write-Host "創建備份: $BackupName" -ForegroundColor Cyan
        
        # 複製專案檔案
        Write-Host "複製專案檔案..." -ForegroundColor Cyan
        Copy-Item -Path $ProjectPath -Destination $BackupDir -Recurse -Force
        
        # 包含Git歷史
        if ($IncludeHistory) {
            Write-Host "包含Git歷史..." -ForegroundColor Cyan
            # Git歷史已包含在複製中
        }
        
        # 壓縮
        if ($Compress -or $Global:MingVCConfig.BackupStrategy.Compress) {
            Write-Host "壓縮備份..." -ForegroundColor Cyan
            $ZipPath = "$BackupDir.zip"
            Compress-Archive -Path $BackupDir -DestinationPath $ZipPath -Force
            Remove-Item -Path $BackupDir -Recurse -Force
            $BackupDir = $ZipPath
        }
        
        # 清理舊備份
        $Backups = Get-ChildItem -Path $BackupPath -Name "MingGoRTS_Backup_*" | Sort-Object -Descending
        if ($Backups.Count -gt $Global:MingVCConfig.BackupStrategy.MaxBackups) {
            Write-Host "清理舊備份..." -ForegroundColor Cyan
            $Backups | Select-Object -Skip $Global:MingVCConfig.BackupStrategy.MaxBackups | ForEach-Object {
                Remove-Item -Path (Join-Path $BackupPath $_) -Recurse -Force
            }
        }
        
        Write-Host "備份完成: $BackupDir" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "備份失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 生成版控報告
function New-MingVCReport {
    param(
        [ValidateSet("status", "history", "branches", "tags", "full")]
        [string]$ReportType = "full",
        [string]$OutputPath = "",
        [switch]$Json,
        [switch]$Html
    )
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "MingGoRTS 版控報告生成" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    try {
        $Report = @{
            GeneratedAt = Get-Date
            ProjectPath = $Global:MingVCConfig.ProjectPath
            ReportType = $ReportType
        }
        
        switch ($ReportType) {
            "status" {
                $Report.Status = Get-MingVCStatus -Detailed
            }
            "history" {
                $Report.History = git log --oneline -50
                $Report.Statistics = @{
                    TotalCommits = (git rev-list --count HEAD)
                    TotalAuthors = (git shortlog -s | Measure-Object).Count
                    TotalBranches = (git branch -a | Measure-Object).Count
                    TotalTags = (git tag | Measure-Object).Count
                }
            }
            "branches" {
                $Report.Branches = git branch -a
                $Report.CurrentBranch = $Global:MingVCState.CurrentBranch
            }
            "tags" {
                $Report.Tags = git tag --sort=-version:refname
                $Report.LatestTag = $Global:MingVCState.LastTag
            }
            "full" {
                $Report.Status = Get-MingVCStatus -Detailed
                $Report.History = git log --oneline -20
                $Report.Branches = git branch -a
                $Report.Tags = git tag --sort=-version:refname
                $Report.Statistics = @{
                    TotalCommits = (git rev-list --count HEAD)
                    TotalAuthors = (git shortlog -s | Measure-Object).Count
                    TotalBranches = (git branch -a | Measure-Object).Count
                    TotalTags = (git tag | Measure-Object).Count
                }
                $Report.Configuration = $Global:MingVCConfig
            }
        }
        
        # 輸出報告
        if ($OutputPath) {
            if ($Json) {
                $Report | ConvertTo-Json -Depth 10 | Set-Content $OutputPath
            } elseif ($Html) {
                ConvertTo-MingVCHtmlReport -Report $Report | Set-Content $OutputPath
            } else {
                ConvertTo-MingVCTextReport -Report $Report | Set-Content $OutputPath
            }
            Write-Host "報告已保存: $OutputPath" -ForegroundColor Green
        } else {
            if ($Json) {
                return $Report | ConvertTo-Json -Depth 10
            } else {
                ConvertTo-MingVCTextReport -Report $Report
            }
        }
        
        return $true
    } catch {
        Write-Host "報告生成失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 轉換為文本報告
function ConvertTo-MingVCTextReport {
    param([hashtable]$Report)
    
    $Output = @"
========================================
MingGoRTS 版控報告
========================================
生成時間: $($Report.GeneratedAt)
專案路徑: $($Report.ProjectPath)
報告類型: $($Report.ReportType)

"@
    
    if ($Report.Status) {
        $Output += @"
狀態資訊:
當前分支: $($Report.Status.CurrentBranch)
最後提交: $($Report.Status.LastCommit)
最後標籤: $($Report.Status.LastTag)
變更數量: $($Report.Status.PendingChanges.Count)

"@
    }
    
    if ($Report.History) {
        $Output += @"
提交歷史:
$($Report.History -join "`n")

"@
    }
    
    if ($Report.Branches) {
        $Output += @"
分支列表:
$($Report.Branches -join "`n")

"@
    }
    
    if ($Report.Tags) {
        $Output += @"
版本標籤:
$($Report.Tags -join "`n")

"@
    }
    
    if ($Report.Statistics) {
        $Output += @"
統計資訊:
總提交數: $($Report.Statistics.TotalCommits)
貢獻者數: $($Report.Statistics.TotalAuthors)
分支數量: $($Report.Statistics.TotalBranches)
標籤數量: $($Report.Statistics.TotalTags)

"@
    }
    
    $Output += "========================================"
    
    return $Output
}

# 轉換為HTML報告
function ConvertTo-MingVCHtmlReport {
    param([hashtable]$Report)
    
    $Html = @"
<!DOCTYPE html>
<html>
<head>
    <title>MingGoRTS 版控報告</title>
    <style>
        body { font-family: 'Segoe UI', Arial, sans-serif; margin: 20px; background: #f5f5f5; }
        .container { max-width: 1200px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }
        h2 { color: #34495e; margin-top: 30px; }
        .status-info { background: #ecf0f1; padding: 15px; border-radius: 5px; margin: 10px 0; }
        .commit-history { font-family: 'Courier New', monospace; background: #2c3e50; color: #ecf0f1; padding: 15px; border-radius: 5px; }
        .statistics { display: flex; justify-content: space-around; margin: 20px 0; }
        .stat-item { text-align: center; background: #3498db; color: white; padding: 20px; border-radius: 5px; }
        .stat-number { font-size: 2em; font-weight: bold; }
        .timestamp { color: #7f8c8d; font-size: 0.9em; }
    </style>
</head>
<body>
    <div class="container">
        <h1>MingGoRTS 版控報告</h1>
        <p class="timestamp">生成時間: $($Report.GeneratedAt) | 專案路徑: $($Report.ProjectPath) | 報告類型: $($Report.ReportType)</p>
        
"@
    
    if ($Report.Status) {
        $Html += @"
        <h2>狀態資訊</h2>
        <div class="status-info">
            <p><strong>當前分支:</strong> $($Report.Status.CurrentBranch)</p>
            <p><strong>最後提交:</strong> $($Report.Status.LastCommit)</p>
            <p><strong>最後標籤:</strong> $($Report.Status.LastTag)</p>
            <p><strong>變更數量:</strong> $($Report.Status.PendingChanges.Count)</p>
        </div>
        
"@
    }
    
    if ($Report.Statistics) {
        $Html += @"
        <h2>統計資訊</h2>
        <div class="statistics">
            <div class="stat-item">
                <div class="stat-number">$($Report.Statistics.TotalCommits)</div>
                <div>總提交數</div>
            </div>
            <div class="stat-item">
                <div class="stat-number">$($Report.Statistics.TotalAuthors)</div>
                <div>貢獻者數</div>
            </div>
            <div class="stat-item">
                <div class="stat-number">$($Report.Statistics.TotalBranches)</div>
                <div>分支數量</div>
            </div>
            <div class="stat-item">
                <div class="stat-number">$($Report.Statistics.TotalTags)</div>
                <div>標籤數量</div>
            </div>
        </div>
        
"@
    }
    
    if ($Report.History) {
        $Html += @"
        <h2>提交歷史</h2>
        <div class="commit-history">
            $($Report.History -join "<br>")
        </div>
        
"@
    }
    
    $Html += @"
    </div>
</body>
</html>
"@
    
    return $Html
}

# 導出函數
Export-ModuleMember -Function @(
    'Initialize-MingVersionControl',
    'Get-MingVCStatus',
    'New-MingVCCommit',
    'Format-MingVCCommitMessage',
    'Test-MingVCCommitMessage',
    'New-MingVCTag',
    'Get-MingVCCurrentVersion',
    'Increment-MingVCVersion',
    'Test-MingVCVersion',
    'Switch-MingVCBranch',
    'Backup-MingVCRepository',
    'New-MingVCReport'
)

Write-Host "MingGoRTS 版本控制API模組已載入" -ForegroundColor Green
