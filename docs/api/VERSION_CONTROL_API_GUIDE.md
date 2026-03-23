# MingGoRTS 版本控制API文檔

## 概述

MingGoRTS 版本控制API是一個完整的版本控制管理系統，為MingGoRTS專案提供智能化的版本控制功能。

## 系統架構

### 核心組件

1. **MingVersionControlAPI-Core.ps1** - 核心API模組
2. **Start-VC-Test.ps1** - 測試和啟動腳本
3. **VersionControl.json** - 配置檔案

### 主要功能

- ✅ 系統初始化和配置
- ✅ 智能提交管理
- ✅ 版本標籤管理
- ✅ 分支管理
- ✅ 狀態監控
- ✅ 報告生成
- ✅ 備份管理

## 使用方法

### 1. 初始化系統

```powershell
# 載入API模組
. "Tools\api\MingVersionControlAPI-Core.ps1"

# 初始化版控系統
Initialize-MingVersionControl
```

### 2. 查看狀態

```powershell
# 簡潔狀態
Get-MingVCStatus

# 詳細狀態
Get-MingVCStatus -Detailed
```

### 3. 智能提交

```powershell
# 基本提交
New-MingVCCommit -Message "添加新功能" -Type "feat"

# 帶範圍的提交
New-MingVCCommit -Message "修復API錯誤" -Type "fix" -Scope "api"

# 自動暫存並提交
New-MingVCCommit -Message "更新文檔" -Type "docs" -AutoStage
```

### 4. 版本管理

```powershell
# 獲取當前版本
Get-MingVCCurrentVersion

# 創建版本標籤
New-MingVCTag -Version "1.1.0" -Message "版本1.1.0發布"

# 自動增量版本並創建標籤
New-MingVCTag -AutoIncrement -Push
```

### 5. 分支管理

```powershell
# 創建並切換分支
Switch-MingVCBranch -BranchName "feature/new-ui" -Create

# 切換現有分支
Switch-MingVCBranch -BranchName "develop"
```

### 6. 報告生成

```powershell
# 生成控制台報告
New-MingVCReport

# 生成檔案報告
New-MingVCReport -OutputPath "Reports\VCReport.txt"

# 生成JSON報告
New-MingVCReport -OutputPath "Reports\VCReport.json" -Json
```

## 提交類型

系統支援Conventional Commits標準：

| 類型 | 說明 | 範例 |
|------|------|------|
| feat | 新功能 | feat(ui): 添加語言選擇器 |
| fix | 修復bug | fix(api): 修復提交錯誤 |
| docs | 文檔更新 | docs(readme): 更新安裝說明 |
| style | 格式調整 | style(code): 統一代碼格式 |
| refactor | 重構 | refactor(core): 重構API架構 |
| test | 測試 | test(api): 添加單元測試 |
| chore | 維護 | chore(deps): 更新依賴 |
| perf | 性能 | perf(render): 優化渲染性能 |
| ci | CI/CD | ci(build): 添加建置腳本 |
| build | 建置 | build(config): 更新建置配置 |
| revert | 回復 | revert(prev): 回復上一提交 |

## 配置選項

### VersionControl.json

```json
{
  "ProjectPath": "C:\\HW\\MingGoRTS",
  "BranchStrategy": {
    "Main": "main",
    "Develop": "develop",
    "Feature": "feature/",
    "Hotfix": "hotfix/",
    "Release": "release/"
  },
  "TagStrategy": {
    "Pattern": "v{major}.{minor}.{patch}",
    "AutoTag": true,
    "PreRelease": false
  },
  "CommitStrategy": {
    "AutoFormat": true,
    "ValidateMessage": true,
    "MaxMessageLength": 100
  },
  "BackupStrategy": {
    "Enabled": true,
    "Location": "Backups\\VersionControl",
    "MaxBackups": 10,
    "Compress": true
  }
}
```

## 快速啟動

### 使用測試腳本

```powershell
# 測試系統功能
.\Tools\api\Start-VC-Test.ps1 -Action "test"

# 查看系統狀態
.\Tools\api\Start-VC-Test.ps1 -Action "status"

# 測試提交功能
.\Tools\api\Start-VC-Test.ps1 -Action "commit"

# 測試標籤功能
.\Tools\api\Start-VC-Test.ps1 -Action "tag"

# 生成報告
.\Tools\api\Start-VC-Test.ps1 -Action "report"
```

## API參考

### Initialize-MingVersionControl

初始化版本控制系統。

**參數:**
- `ProjectPath` (string): 專案路徑，默認 "C:\HW\MingGoRTS"

**返回值:**
- `$true` 成功
- `$false` 失敗

### Get-MingVCStatus

獲取版本控制狀態。

**參數:**
- `Detailed` (switch): 顯示詳細狀態

**返回值:**
- 系統狀態物件

### New-MingVCCommit

執行智能提交。

**參數:**
- `Message` (string): 提交訊息
- `Type` (string): 提交類型，默認 "feat"
- `Scope` (string): 提交範圍
- `AutoStage` (switch): 自動暫存
- `DryRun` (switch): 預覽模式

**返回值:**
- `$true` 成功
- `$false` 失敗

### New-MingVCTag

創建版本標籤。

**參數:**
- `Version` (string): 版本號
- `Message` (string): 標籤訊息
- `AutoIncrement` (switch): 自動增量
- `Push` (switch): 推送到遠端
- `DryRun` (switch): 預覽模式

**返回值:**
- `$true` 成功
- `$false` 失敗

### Switch-MingVCBranch

分支管理。

**參數:**
- `BranchName` (string): 分支名稱
- `Create` (switch): 創建分支
- `Force` (switch): 強制操作

**返回值:**
- `$true` 成功
- `$false` 失敗

### New-MingVCReport

生成版本控制報告。

**參數:**
- `OutputPath` (string): 輸出路徑
- `Json` (switch): JSON格式

**返回值:**
- `$true` 成功
- `$false` 失敗

## 最佳實踐

### 1. 提交訊息規範

```powershell
# 好的範例
New-MingVCCommit -Message "添加用戶認證功能" -Type "feat" -Scope "auth"
New-MingVCCommit -Message "修復登入頁面崩潰問題" -Type "fix" -Scope "ui"
New-MingVCCommit -Message "更新API文檔" -Type "docs"

# 避免的範例
New-MingVCCommit -Message "fix bug"
New-MingVCCommit -Message "update"
New-MingVCCommit -Message "temp"
```

### 2. 版本管理

```powershell
# 開發新功能時
Switch-MingVCBranch -BranchName "feature/new-feature" -Create

# 完成功能後
New-MingVCCommit -Message "完成新功能開發" -Type "feat" -AutoStage
Switch-MingVCBranch -BranchName "develop"

# 發布版本時
New-MingVCTag -AutoIncrement -Push
```

### 3. 工作流程

```powershell
# 1. 初始化系統
Initialize-MingVersionControl

# 2. 檢查狀態
Get-MingVCStatus -Detailed

# 3. 創建功能分支
Switch-MingVCBranch -BranchName "feature/feature-name" -Create

# 4. 開發並提交
# ... 開發工作 ...
New-MingVCCommit -Message "實現核心功能" -Type "feat" -AutoStage

# 5. 合併分支
# ... 合併到主分支 ...

# 6. 創建版本標籤
New-MingVCTag -Version "1.1.0" -Message "發布版本1.1.0"

# 7. 生成報告
New-MingVCReport -OutputPath "Reports\Release_1.1.0.txt"
```

## 故障排除

### 常見問題

1. **初始化失敗**
   - 檢查是否為Git倉庫
   - 確認專案路徑正確

2. **提交失敗**
   - 檢查是否有變更需要提交
   - 確認提交訊息格式

3. **標籤創建失敗**
   - 檢查版本格式
   - 確認標籤不存在

4. **分支操作失敗**
   - 檢查分支名稱
   - 確認工作目錄乾淨

### 調試模式

```powershell
# 使用預覽模式
New-MingVCCommit -Message "測試" -DryRun
New-MingVCTag -Version "1.0.0" -DryRun
```

## 擴展功能

### 自動化腳本

```powershell
# 自動化提交腳本
function Auto-Commit {
    param([string]$Message)
    
    Initialize-MingVersionControl | Out-Null
    New-MingVCCommit -Message $Message -AutoStage
    New-MingVCReport -OutputPath "Reports\AutoCommit_$(Get-Date -Format 'yyyyMMdd').txt"
}
```

### 集成到CI/CD

```yaml
# GitHub Actions 範例
- name: Setup Version Control
  run: |
    .\Tools\api\Start-VC-Test.ps1 -Action "test"
    
- name: Generate Report
  run: |
    .\Tools\api\Start-VC-Test.ps1 -Action "report"
```

## 版本歷史

- **v1.0.0** - 初始版本
  - 基本版本控制功能
  - 智能提交系統
  - 版本標籤管理
  - 分支管理
  - 狀態監控
  - 報告生成

## 支援

如有問題或建議，請聯繫MingGoRTS開發團隊。

---

*文檔最後更新: 2026-03-23*
