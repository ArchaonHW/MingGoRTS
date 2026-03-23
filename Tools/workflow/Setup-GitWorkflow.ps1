#!/usr/bin/env pwsh
#Requires -Version 7.0

<#
.SYNOPSIS
    MingGoRTS Git 工作流設置腳本
    Git Workflow Setup Script

.DESCRIPTION
    自動化配置 Git 工作流和本地協作規範
    - 初始化 Git 倉庫
    - 配置 Git LFS 大文件管理
    - 設置分支策略
    - 配置預提交鉤子
    - 建立協作規範

.PARAMETER Init
    初始化新的 Git 倉庫

.PARAMETER ConfigureLFS
    配置 Git LFS 大文件管理

.PARAMETER SetupHooks
    設置預提交鉤子

.PARAMETER SetupBranches
    設置分支策略

.EXAMPLE
    .\Setup-GitWorkflow.ps1 -Init -ConfigureLFS -SetupHooks -SetupBranches
    完整配置 Git 工作流
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$Init,
    
    [Parameter()]
    [switch]$ConfigureLFS,
    
    [Parameter()]
    [switch]$SetupHooks,
    
    [Parameter()]
    [switch]$SetupBranches,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$RemoteURL = ""
)

$ErrorActionPreference = "Stop"

# Git 配置
$GitConfig = @{
    UserName = "MingGoRTS Developer"
    UserEmail = "dev@minggorts.local"
    DefaultBranch = "main"
    DevelopBranch = "development"
    FeaturePrefix = "feature/"
    BugfixPrefix = "bugfix/"
    HotfixPrefix = "hotfix/"
    ReleasePrefix = "release/"
}

# 大文件類型 (Git LFS)
$LargeFileTypes = @(
    "*.uasset"
    "*.umap"
    "*.fbx"
    "*.obj"
    "*.png"
    "*.jpg"
    "*.jpeg"
    "*.tga"
    "*.wav"
    "*.mp3"
    "*.mp4"
    "*.avi"
    "*.dll"
    "*.exe"
    "*.pdb"
    "*.zip"
    "*.rar"
    "*.7z"
)

function Write-GitLog {
    param([string]$Message, [string]$Level = "INFO")
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Colors = @{
        "INFO" = "White"
        "SUCCESS" = "Green"
        "WARNING" = "Yellow"
        "ERROR" = "Red"
    }
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Colors[$Level]
}

function Test-GitRepository {
    $GitPath = Join-Path $ProjectRoot ".git"
    return Test-Path $GitPath
}

function Initialize-GitRepository {
    Write-GitLog "初始化 Git 倉庫..." "INFO"
    
    if (Test-GitRepository) {
        Write-GitLog "Git 倉庫已存在，跳過初始化" "WARNING"
        return
    }
    
    Set-Location $ProjectRoot
    
    # 初始化倉庫
    git init
    
    # 配置用戶信息
    git config user.name $GitConfig.UserName
    git config user.email $GitConfig.UserEmail
    
    # 配置默認分支
    git checkout -b $GitConfig.DefaultBranch
    
    Write-GitLog "✅ Git 倉庫初始化完成" "SUCCESS"
}

function Install-GitLFS {
    Write-GitLog "配置 Git LFS 大文件管理..." "INFO"
    
    # 檢查 Git LFS 是否已安裝
    try {
        $LFSVersion = git lfs version
        Write-GitLog "Git LFS 已安裝: $LFSVersion" "SUCCESS"
    }
    catch {
        Write-GitLog "Git LFS 未安裝，正在安裝..." "WARNING"
        git lfs install
    }
    
    # 初始化 LFS
    Set-Location $ProjectRoot
    git lfs install
    
    # 追蹤大文件類型
    foreach ($FileType in $LargeFileTypes) {
        try {
            git lfs track $FileType
            Write-GitLog "追蹤大文件類型: $FileType" "INFO"
        }
        catch {
            Write-GitLog "追蹤失敗: $FileType - $($_.Exception.Message)" "WARNING"
        }
    }
    
    # 提交 .gitattributes
    git add .gitattributes
    git commit -m "🔧 配置 Git LFS 大文件管理

- 追蹤 UE5 資源文件 (*.uasset, *.umap)
- 追蹤媒體文件 (*.png, *.jpg, *.wav, *.mp3)
- 追蹤二進製文件 (*.dll, *.exe)"
    
    Write-GitLog "✅ Git LFS 配置完成" "SUCCESS"
}

function New-PreCommitHook {
    Write-GitLog "設置預提交鉤子..." "INFO"
    
    $HookPath = Join-Path $ProjectRoot ".git\hooks\pre-commit"
    
    $HookContent = @'#!/bin/sh
# MingGoRTS 預提交檢查鉤子
# Pre-commit Hook for MingGoRTS

echo "🔍 執行預提交檢查..."

# 1. 檢查是否有語法錯誤
if command -v pwsh >/dev/null 2>&1; then
    echo "  檢查語法錯誤..."
    pwsh -Command "& '\"$PWD\Tools\workflow\Debug-Workflow.ps1\"' -DeepScan" 2>/dev/null || true
fi

# 2. 檢查代碼風格
echo "  檢查代碼風格..."
# TODO: 集成代碼風格檢查工具

# 3. 檢查文件大小
echo "  檢查大文件..."
LARGE_FILES=$(git diff --cached --numstat | awk '$1 > 10000 || $2 > 10000 {print $3}')
if [ -n "$LARGE_FILES" ]; then
    echo "⚠️  檢測到大文件更改:"
    echo "$LARGE_FILES"
    echo "確保這些文件已配置 Git LFS"
fi

echo "✅ 預提交檢查完成"
exit 0
'@
    
    $HookContent | Set-Content $HookPath -Encoding UTF8 -Force
    
    # 設置執行權限 (Linux/Mac)
    if ($IsLinux -or $IsMacOS) {
        chmod +x $HookPath
    }
    
    Write-GitLog "✅ 預提交鉤子設置完成" "SUCCESS"
}

function Setup-BranchStrategy {
    Write-GitLog "設置分支策略..." "INFO"
    
    Set-Location $ProjectRoot
    
    # 確保在主分支
    git checkout $GitConfig.DefaultBranch 2>$null || git checkout -b $GitConfig.DefaultBranch
    
    # 創建開發分支
    git branch $GitConfig.DevelopBranch 2>$null
    Write-GitLog "創建分支: $($GitConfig.DevelopBranch)" "INFO"
    
    # 配置分支保護規則 (本地)
    $BranchConfig = @"
# 分支策略配置
# Branch Strategy Configuration

## 分支說明
- $($GitConfig.DefaultBranch): 主分支，穩定版本
- $($GitConfig.DevelopBranch): 開發分支，日常開發
- $($GitConfig.FeaturePrefix)*: 功能分支
- $($GitConfig.BugfixPrefix)*: 修復分支
- $($GitConfig.HotfixPrefix)*: 熱修復分支
- $($GitConfig.ReleasePrefix)*: 發布分支

## 工作流程
1. 從 $($GitConfig.DevelopBranch) 創建功能分支
2. 功能開發完成後合併回 $($GitConfig.DevelopBranch)
3. 發布時從 $($GitConfig.DevelopBranch) 創建發布分支
4. 發布分支測試通過後合併到 $($GitConfig.DefaultBranch) 和 $($GitConfig.DevelopBranch)

## 提交規範
- 🎉 新功能: `:tada:`
- 🔧 修復: `:wrench:`
- 📚 文檔: `:books:`
- ⚡ 優化: `:zap:`
- 🧪 測試: `:test_tube:`
- 🔀 合併: `:twisted_rightwards_arrows:`
"@
    
    $BranchConfig | Set-Content "$ProjectRoot\.git\BRANCH_STRATEGY.md" -Encoding UTF8 -Force
    
    # 提交分支策略文檔
    git add .git/BRANCH_STRATEGY.md
    git commit -m "📚 添加分支策略文檔

- 定義主分支/開發分支/功能分支規範
- 說明工作流程和提交規範" --allow-empty
    
    Write-GitLog "✅ 分支策略設置完成" "SUCCESS"
}

function New-GitIgnore {
    Write-GitLog "創建 .gitignore 文件..." "INFO"
    
    $GitIgnoreContent = @'
# MingGoRTS .gitignore

## Unreal Engine 自動生成文件
Binaries/
DerivedDataCache/
Intermediate/
Saved/
Build/
*.sln
*.vcxproj
*.vcxproj.filters

## Visual Studio
.vs/
*.suo
*.user
*.userosscache
*.sln.docstates

## JetBrains Rider
.idea/
*.iml

## 編譯輸出
*.obj
*.pdb
*.ilk
*.exe
*.dll
*.lib
*.exp

## 日誌和臨時文件
*.log
*.tmp
*.temp

## 操作系統文件
.DS_Store
Thumbs.db
desktop.ini

## 敏感信息
*.key
*.pem
*.password
config/secrets.json

## 本地開發配置
.vscode/settings.json
.idea/

## 暫存文件
*.swp
*.swo
*~

## 大文件 (已由 Git LFS 管理)
# *.uasset
# *.umap
# *.fbx
# *.png
# *.wav
'@
    
    $GitIgnorePath = Join-Path $ProjectRoot ".gitignore"
    
    if (Test-Path $GitIgnorePath) {
        Write-GitLog ".gitignore 已存在，更新內容..." "WARNING"
    }
    
    $GitIgnoreContent | Set-Content $GitIgnorePath -Encoding UTF8 -Force
    
    # 提交 .gitignore
    Set-Location $ProjectRoot
    git add .gitignore
    git commit -m "🔧 添加 .gitignore 配置

- 忽略 UE5 自動生成文件
- 忽略編譯輸出和臨時文件
- 忽略敏感信息" --allow-empty
    
    Write-GitLog "✅ .gitignore 創建完成" "SUCCESS"
}

function New-CollaborationGuidelines {
    Write-GitLog "創建協作規範文檔..." "INFO"
    
    $GuidelinesPath = "$ProjectRoot\docs\COLLABORATION_GUIDELINES.md"
    
    $GuidelinesContent = @'
# MingGoRTS 本地協作規範

## 🎯 協作原則

### 1. 分支管理
- **主分支 (main)**: 只包含穩定版本，禁止直接推送
- **開發分支 (development)**: 日常開發，功能分支由此創建
- **功能分支 (feature/*)**: 新功能開發，命名規範 `feature/功能名稱`
- **修復分支 (bugfix/*)**: Bug 修復，命名規範 `bugfix/問題描述`
- **熱修復 (hotfix/*)**: 緊急修復，命名規範 `hotfix/修復內容`

### 2. 提交規範

#### 提交信息格式
```
[類型] 簡短描述

詳細說明（可選）

- 相關任務: #任務編號
```

#### 類型標籤
- 🎉 `:tada:` 新功能
- 🔧 `:wrench:` Bug 修復
- 📚 `:books:` 文檔更新
- ⚡ `:zap:` 性能優化
- 🧪 `:test_tube:` 測試相關
- 🔀 `:twisted_rightwards_arrows:` 分支合併
- 🎨 `:art:` 代碼格式調整

#### 示例
```
[🎉] 實現三權指揮系統核心

- 添加道權/策權/兵權三權分立模型
- 實現權力制衡機制
- 添加權力切換功能

- 相關任務: SC-001
```

### 3. 代碼審查流程

#### 提交前必須
1. ✅ 本地編譯通過
2. ✅ 運行 Debug Workflow 無錯誤
3. ✅ 代碼風格符合規範
4. ✅ 提交信息符合格式

#### 審查檢查項
- [ ] 代碼邏輯正確
- [ ] 命名規範統一
- [ ] 注釋清晰完整
- [ ] 無安全隱患
- [ ] 性能影響可接受

### 4. 衝突解決

#### 預防衝突
1. 頻繁拉取更新 `git pull origin development`
2. 小步提交，頻繁同步
3. 大型改動提前溝通

#### 解決步驟
1. 本地創建備份分支
2. 執行 `git pull` 獲取最新代碼
3. 解決衝突文件
4. 測試驗證
5. 提交解決結果

### 5. 發布流程

#### 版本號規範 (語義化版本)
- 格式: `主版本.次版本.修訂號`
- 示例: `v1.2.3`

#### 發布檢查清單
- [ ] 所有測試通過
- [ ] 文檔已更新
- [ ] 版本號已標記
- [ ] 發布說明已準備

## 🛠️ 工具配置

### 必需工具
- Git 2.30+
- Git LFS
- PowerShell 7.6+
- Visual Studio 2022

### 推薦工具
- Git GUI 客戶端 (SourceTree/GitKraken)
- 代碼編輯器 (VS Code/Rider)
- 差異比較工具 (Beyond Compare)

## 📋 工作流程

### 日常開發流程
```bash
# 1. 切換到開發分支
git checkout development
git pull origin development

# 2. 創建功能分支
git checkout -b feature/新功能名稱

# 3. 開發並提交
git add .
git commit -m "[🎉] 實現新功能"

# 4. 同步更新
git checkout development
git pull origin development
git checkout feature/新功能名稱
git merge development

# 5. 提交審查 (如果是中央倉庫)
# git push origin feature/新功能名稱

# 6. 合併回開發分支
git checkout development
git merge feature/新功能名稱
git branch -d feature/新功能名稱
```

### 緊急修復流程
```bash
# 1. 從主分支創建熱修復分支
git checkout main
git checkout -b hotfix/緊急修復

# 2. 修復並提交
git add .
git commit -m "[🔧] 緊急修復問題"

# 3. 合併到主分支和開發分支
git checkout main
git merge hotfix/緊急修復
git checkout development
git merge hotfix/緊急修復
git branch -d hotfix/緊急修復
```

## 🚨 禁止事項

- ❌ 直接向 main 分支推送
- ❌ 提交二進製文件到普通 Git (必須使用 LFS)
- ❌ 提交敏感信息 (密碼、API Key)
- ❌ 提交個人配置文件
- ❌ 提交編譯輸出文件

## 📞 聯絡支持

### 問題反饋
- 使用 Debug Workflow 生成問題報告
- 在任務系統中創建問題單
- 緊急問題直接聯絡項目負責人

### 培訓資源
- 查看 `docs/DEVELOPER_GUIDE.md`
- 參考 `docs/GIT_WORKFLOW_GUIDE.md`
- 執行 `Tools\workflow\SmartWorkflowSelector.ps1` 獲取幫助

---

*最後更新: 2026-03-24*
*版本: v1.0*
'@
    
    # 確保目錄存在
    $DocDir = "$ProjectRoot\docs"
    if (-not (Test-Path $DocDir)) {
        New-Item -ItemType Directory -Path $DocDir -Force | Out-Null
    }
    
    $GuidelinesContent | Set-Content $GuidelinesPath -Encoding UTF8 -Force
    
    # 提交協作規範
    Set-Location $ProjectRoot
    git add docs/COLLABORATION_GUIDELINES.md
    git commit -m "📚 添加協作規範文檔

- 定義分支管理策略
- 說明提交規範和審查流程
- 提供日常開發和緊急修復流程" --allow-empty
    
    Write-GitLog "✅ 協作規範文檔創建完成" "SUCCESS"
}

# 主程序
Write-Host "`n╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║                                                              ║" -ForegroundColor Cyan
Write-Host "║        MingGoRTS Git 工作流設置工具                        ║" -ForegroundColor Cyan
Write-Host "║        Git Workflow Setup Tool                               ║" -ForegroundColor Cyan
Write-Host "║                                                              ║" -ForegroundColor Cyan
Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

# 驗證專案路徑
if (-not (Test-Path $ProjectRoot)) {
    Write-GitLog "專案路徑不存在: $ProjectRoot" "ERROR"
    exit 1
}

Write-GitLog "專案路徑: $ProjectRoot" "INFO"

# 執行選擇的操作
if ($Init) {
    Initialize-GitRepository
}

if (Test-GitRepository) {
    if ($ConfigureLFS) {
        Install-GitLFS
    }
    
    if ($SetupHooks) {
        New-PreCommitHook
    }
    
    if ($SetupBranches) {
        Setup-BranchStrategy
    }
    
    # 始終創建 .gitignore 和協作規範
    New-GitIgnore
    New-CollaborationGuidelines
    
    # 設置遠程倉庫 (如果提供)
    if ($RemoteURL) {
        Set-Location $ProjectRoot
        git remote add origin $RemoteURL
        Write-GitLog "設置遠程倉庫: $RemoteURL" "SUCCESS"
    }
    
    Write-GitLog "`n=== Git 工作流設置完成 ===" "SUCCESS"
    Write-GitLog "請查看以下文件了解更多信息:" "INFO"
    Write-GitLog "  - .git/BRANCH_STRATEGY.md" "INFO"
    Write-GitLog "  - docs/COLLABORATION_GUIDELINES.md" "INFO"
    Write-GitLog "  - .gitignore" "INFO"
} else {
    Write-GitLog "未找到 Git 倉庫，請使用 -Init 參數初始化" "ERROR"
    exit 1
}
