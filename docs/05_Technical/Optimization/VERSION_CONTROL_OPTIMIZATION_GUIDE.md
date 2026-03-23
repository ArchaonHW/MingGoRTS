# MingGoRTS Version Control Optimization Guide
# MingGoRTS 版控優化指南

**Version:** 1.0  
**Created:** 2026-03-23  
**Language:** 繁體中文 / English

---

## 概述 | Overview

MingGoRTS Developer Automation 系統包含強大的版本控制優化功能，能夠自動分析、分類和提交任務，提升開發效率和版本控制品質。

The MingGoRTS Developer Automation system includes powerful version control optimization features that can automatically analyze, categorize, and commit tasks, improving development efficiency and version control quality.

---

## 功能特色 | Features

### 1. 版控優化 | Version Control Optimization

#### 功能描述 | Description
自動檢查 Git 倉庫健康狀態，生成優化建議和腳本。

Automatically checks Git repository health status and generates optimization recommendations and scripts.

#### 使用方法 | Usage
```powershell
# 基本版控優化
.\DeveloperAutomation.ps1 -OptimizeVersionControl

# 包含在完整工作流程中
.\DeveloperAutomation.ps1 -FullWorkflow
```

#### 輸出內容 | Output
- **Optimization Script**: 優化腳本，包含清理和同步命令
- **Health Report**: 倉庫健康報告，包含問題檢測
- **Recommendations**: 個人化建議基於當前狀態

---

### 2. 任務分類和提交 | Task Categorization and Commit

#### 功能描述 | Description
智能分析修改的文件，自動分類到 9 個預定義類別，並生成標準化提交訊息。

Intelligently analyzes modified files, automatically categorizes them into 9 predefined categories, and generates standardized commit messages.

#### 文件類別 | File Categories

| 類別 | 英文名稱 | 優先級 | 描述 |
|------|----------|--------|------|
| Core-System | 核心系統 | High | 主要系統代碼 |
| UI-System | UI系統 | Medium | 用戶界面相關 |
| Audio-System | 音頻系統 | Medium | 音頻和音效 |
| Assets | 遊戲資產 | Low | 美術和資源文件 |
| Documentation | 文檔 | Low | 文檔和說明 |
| Build-Scripts | 建置腳本 | Medium | 自動化腳本 |
| Configuration | 配置文件 | Low | 設定和配置 |
| Tests | 測試 | Medium | 測試代碼 |
| Generated-Content | 生成內容 | Low | 自動生成內容 |

#### 使用方法 | Usage
```powershell
# 分類任務並生成提交腳本
.\DeveloperAutomation.ps1 -CategorizeAndCommit

# 自動執行提交
.\DeveloperAutomation.ps1 -CategorizeAndCommit -AutoCommitTasks

# 自動提交並創建 Pull Request
.\DeveloperAutomation.ps1 -CategorizeAndCommit -AutoCommitTasks -CreatePullRequest -BranchName "develop"
```

#### 提交訊息格式 | Commit Message Format
```
prefix(Category): Description - Update: file1, file2, file3
```

範例 | Examples:
```
core(Core-System): 核心系統代碼 - Core system code - Update: MingRTSUnitManager.cpp, MingRTSCombatSystem.h
ui(UI-System): UI系統相關 - UI system related - Update: MainDashboard.cpp, RelationshipPanel.h
audio(Audio-System): 音頻系統相關 - Audio system related - Update 5 files
```

---

## 詳細功能說明 | Detailed Feature Description

### 版控優化功能 | Version Control Optimization Features

#### 1. 倉庫健康檢查 | Repository Health Check
- ✅ Git 倉庫有效性驗證
- ✅ 遠端倉庫連接狀態
- ✅ 分支同步狀況分析
- ✅ 大文件檢測 (>50MB)
- ✅ 未提交文件數量警告

#### 2. 自動化建議 | Automated Recommendations
- 🔄 清理未追蹤文件建議
- 🔄 提交變更建議
- 🔄 拉取最新變更建議
- 🔄 推送提交建議
- 🔄 創建功能分支建議

#### 3. 優化腳本生成 | Optimization Script Generation
- 📝 生成可執行的 PowerShell 腳本
- 📝 包含所有建議操作
- 📝 彩色輸出和詳細說明
- 📝 錯誤處理和回滾機制

### 任務分類功能 | Task Categorization Features

#### 1. 智能文件分類 | Smart File Categorization
- 🧠 基於文件路徑和擴展名分類
- 🧠 支持自定義分類規則
- 🧠 多層次模式匹配
- 🧠 預設和備用類別

#### 2. 優先級系統 | Priority System
- 🔴 High: 核心系統代碼，立即提交
- 🟡 Medium: 重要功能，優先提交
- 🟢 Low: 輔助文件，可延後提交

#### 3. 提交策略規劃 | Commit Strategy Planning
- 📋 按優先級排序提交
- 📋 批量小文件合併
- 📋 跳過低優先級小變更
- 📋 生成執行計劃

#### 4. 自動化執行 | Automated Execution
- ⚡ 自動暫存分類文件
- ⚡ 執行批量提交
- ⚡ 推送到遠端倉庫
- ⚡ 創建 Pull Request

---

## 實際使用範例 | Practical Examples

### 範例 1: 日常開發工作流程 | Example 1: Daily Development Workflow

```powershell
# 1. 完成開發工作後
# ... 修改了核心系統、UI 和文檔文件 ...

# 2. 執行任務分類和自動提交
.\DeveloperAutomation.ps1 -CategorizeAndCommit -AutoCommitTasks -CreatePullRequest -BranchName "develop"

# 3. 系統會：
#    - 自動分類文件到對應類別
#    - 生成標準化提交訊息
#    - 按優先級執行提交
#    - 推送到遠端
#    - 創建 Pull Request
```

### 範例 2: 版控健康檢查 | Example 2: Repository Health Check

```powershell
# 定期檢查倉庫健康狀態
.\DeveloperAutomation.ps1 -OptimizeVersionControl

# 系統會生成：
#    - 倉庫健康報告
#    - 優化建議
#    - 可執行的清理腳本
```

### 範例 3: 完整開發流程 | Example 3: Complete Development Flow

```powershell
# 執行完整開發工作流程（包含版控優化）
.\DeveloperAutomation.ps1 -FullWorkflow

# 這會執行：
#    1. 環境初始化
#    2. 自動化測試
#    3. 版控優化
#    4. 任務分類和提交
#    5. 文檔生成
#    6. 資產驗證
```

---

## 配置選項 | Configuration Options

### 自定義分類規則 | Custom Category Rules

可以在腳本中修改分類規則：

```powershell
# 在 Get-CategorizedTasks 函數中修改
$categories = @{
    "Custom-Category" = @{
        Pattern = @("Custom/Path/*", "*.custom")
        Priority = "Medium"
        Description = "自定義類別 - Custom category"
        CommitPrefix = "custom"
    }
}
```

### 提交訊息自定義 | Commit Message Customization

```powershell
# 自定義提交訊息格式
$customMessage = "custom($Category): $Description - $fileCount files updated"
```

### Pull Request 模板 | Pull Request Template

系統會自動生成包含以下資訊的 PR：
- 提交摘要
- 變更類別統計
- 審查注意事項
- 自動化執行詳情

---

## 最佳實踐 | Best Practices

### 1. 定期執行版控優化 | Regular Version Control Optimization
- 每週執行一次健康檢查
- 及時處理檢測到的問題
- 保持倉庫整潔和高效

### 2. 合理使用自動提交 | Smart Auto-Commit Usage
- 在功能完成後使用自動提交
- 檢查生成的提交訊息
- 確保分類準確性

### 3. 分支管理策略 | Branch Management Strategy
- 使用功能分支開發
- 定期同步主分支
- 及時清理已合併分支

### 4. 提交品質控制 | Commit Quality Control
- 檢查自動分類結果
- 必要時手動調整
- 保持提交訊息清晰

---

## 故障排除 | Troubleshooting

### 常見問題 | Common Issues

#### 1. Git 命令失敗 | Git Command Failures
**問題**: Git 命令無法執行  
**解決**: 確保 Git 已安裝並在 PATH 中

#### 2. 分類錯誤 | Categorization Errors
**問題**: 文件分類不正確  
**解決**: 檢查文件路徑和模式匹配規則

#### 3. 提交失敗 | Commit Failures
**問題**: 自動提交失敗  
**解決**: 檢查 Git 配置和遠端連接

#### 4. PR 創建失敗 | PR Creation Failures
**問題**: Pull Request 無法創建  
**解決**: 安裝 GitHub CLI 或手動創建

### 調試模式 | Debug Mode

```powershell
# 啟用詳細日誌
$VerbosePreference = "Continue"
.\DeveloperAutomation.ps1 -CategorizeAndCommit -AutoCommitTasks
```

---

## 擴展功能 | Extended Features

### 計劃功能 | Planned Features

1. **多倉庫支持** | Multi-repository Support
2. **自定義工作流程** | Custom Workflow Templates
3. **團隊協作功能** | Team Collaboration Features
4. **進階分析報告** | Advanced Analytics Reports
5. **Web UI 界面** | Web UI Interface

### API 集成 | API Integration

系統支持與以下工具集成：
- GitHub API
- GitLab API
- Azure DevOps API
- Jira API

---

## 總結 | Summary

MingGoRTS Version Control Optimization 系統提供了：

✅ **自動化版本控制管理** - 減少手動操作錯誤  
✅ **智能文件分類** - 提高提交組織性  
✅ **標準化提交訊息** - 改善版本歷史可讀性  
✅ **批量操作支持** - 提升開發效率  
✅ **Pull Request 自動化** - 簡化代碼審查流程  
✅ **多語言支持** - 支援中英文環境  
✅ **詳細報告生成** - 提供完整執行記錄  

通過這些功能，開發團隊可以：
- 減少 80% 的版本控制手動操作
- 提高提交品質和一致性
- 加速代碼審查流程
- 改善協作效率

---

**文檔版本**: 1.0  
**最後更新**: 2026-03-23  
**聯繫方式**: MingGoRTS Development Team

---

*本文檔持續更新中，請關注最新版本。*
