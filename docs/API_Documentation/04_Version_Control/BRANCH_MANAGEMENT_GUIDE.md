# MingGoRTS 分支管理和合併工作流程系統

## 系統概述

MingGoRTS 分支管理系統是一個完整的 Git 工作流程自動化工具，確保代碼質量和合併安全。系統強制執行預合併檢查，保護主分支的穩定性。

## 核心功能

### 🔧 分支管理
- **分支創建**: 自動從 develop 分支創建功能分支
- **分支狀態**: 顯示當前分支信息和工作目錄狀態
- **分支類型識別**: 自動識別主分支、開發分支、功能分支

### 🛡️ 預合併檢查
- **工作目錄檢查**: 確保工作目錄乾淨
- **測試檢查**: 自動運行所有測試腳本
- **構建檢查**: 驗證項目構建成功
- **安全掃描**: 檢查代碼安全性

### 🔄 合併工作流程
- **安全合併**: 只有通過所有檢查才能合併
- **自動回滾**: 合併失敗時自動取消
- **分支保護**: 主分支受到嚴格保護

## 文件結構

```
Tools/api/
├── Branch-Management.ps1      # 主要分支管理腳本
├── BranchWorkflow.ps1         # 完整工作流程腳本
├── Quick-Branch.ps1           # 快速啟動腳本
├── MingVersionControlAPI-Core.ps1  # 版本控制 API 核心
├── RequirementManagement.ps1   # 需求管理系統
└── IntegrateAllScripts.ps1     # 腳本整合工具
```

## 使用方法

### 基本命令

```powershell
# 顯示幫助
.\Branch-Management.ps1 -Action "help"

# 查看分支狀態
.\Branch-Management.ps1 -Action "status"

# 創建功能分支
.\Branch-Management.ps1 -Action "feature"

# 執行預合併檢查
.\Branch-Management.ps1 -Action "checks"

# 合併到目標分支
.\Branch-Management.ps1 -Action "merge"
```

### 標準工作流程

1. **創建功能分支**
   ```powershell
   .\Branch-Management.ps1 -Action "feature"
   # 輸入功能名稱，例如: ui-improvements
   ```

2. **開發和測試**
   - 在功能分支上進行開發
   - 定期提交變更
   - 運行測試確保功能正常

3. **預合併檢查**
   ```powershell
   .\Branch-Management.ps1 -Action "checks"
   ```

4. **合併到開發分支**
   ```powershell
   .\Branch-Management.ps1 -Action "merge"
   # 輸入目標分支: develop
   ```

5. **創建發布分支** (可選)
   ```powershell
   git checkout -b release/v1.0.0
   ```

6. **合併到主分支**
   ```powershell
   .\Branch-Management.ps1 -Action "merge"
   # 輸入目標分支: main
   ```

## 主分支保護規則

### 🚫 嚴格限制
- **禁止直接提交**: 不允許直接提交到 main 分支
- **強制檢查**: 合併到 main 前必須通過所有檢查
- **測試要求**: 所有測試必須通過
- **構建要求**: 項目必須成功構建
- **乾淨狀態**: 工作目錄必須乾淨

### ✅ 允許操作
- **功能分支**: 從 develop 創建功能分支
- **開發合併**: 功能分支可以合併到 develop
- **發布合併**: 發布分支可以合併到 main
- **熱修復**: 從 main 創建熱修復分支

## 預合併檢查詳情

### 1. 工作目錄檢查
```powershell
# 檢查是否有未提交的變更
git status --porcelain
```

### 2. 測試檢查
```powershell
# 運行所有測試腳本
Get-ChildItem -Path "C:\HW\MingGoRTS" -Filter "*Test*.ps1" -Recurse
```

### 3. 構建檢查
```powershell
# 運行構建腳本
C:\HW\MingGoRTS\BUILD_OPTIMIZED.cmd
```

### 4. 安全掃描
```powershell
# 檢查代碼安全性
# (未來實現)
```

## 分支命名規範

### 🌟 功能分支
```
feature/<feature-name>
例如: feature/ui-improvements
     feature/audio-system
     feature/ai-enhancement
```

### 🔧 熱修復分支
```
hotfix/<hotfix-name>
例如: hotfix/critical-bug-fix
     hotfix/security-patch
```

### 📦 發布分支
```
release/<version>
例如: release/v1.0.0
     release/v1.1.0
```

## 配置文件

### BranchWorkflow.json
```json
{
  "BranchNaming": {
    "Main": "main",
    "Develop": "develop",
    "FeaturePrefix": "feature/",
    "HotfixPrefix": "hotfix/",
    "ReleasePrefix": "release/"
  },
  "RequiredChecks": {
    "PreMerge": [
      "CleanWorkingDirectory",
      "UpToDate",
      "CodeQuality",
      "Tests",
      "Build",
      "SecurityScan",
      "Documentation"
    ]
  },
  "Protection": {
    "MainBranch": {
      "RequirePreMergeChecks": true,
      "RequireTests": true,
      "RequireBuild": true,
      "RequireSecurityScan": true
    }
  }
}
```

## 錯誤處理

### 常見錯誤及解決方案

1. **工作目錄不乾淨**
   ```
   ERROR: Working directory not clean, please commit changes first
   ```
   **解決方案**: 提交或暫存所有變更

2. **測試失敗**
   ```
   FAIL: Test failed - TestScript.ps1
   ```
   **解決方案**: 修復測試中的錯誤

3. **構建失敗**
   ```
   FAIL: Build failed
   ```
   **解決方案**: 修復構建錯誤

4. **合併衝突**
   ```
   ERROR: Merge failed
   ```
   **解決方案**: 解決衝突後重新嘗試

## 最佳實踐

### 📋 開發流程
1. **小步驟開發**: 經常提交小變更
2. **測試驅動**: 先寫測試，再寫代碼
3. **代碼審查**: 合併前進行代碼審查
4. **文檔更新**: 更新相關文檔

### 🔄 分支策略
1. **主分支**: 保持穩定，只接受發布版本
2. **開發分支**: 集成所有功能分支
3. **功能分支**: 獨立開發新功能
4. **熱修復分支**: 緊急修復生產問題

### 🛡️ 安全措施
1. **定期備份**: 定期備份重要分支
2. **訪問控制**: 限制主分支寫入權限
3. **審計日誌**: 記錄所有分支操作
4. **自動化檢查**: 自動執行質量檢查

## 集成系統

### 與版本控制 API 集成
- **MingVersionControlAPI-Core.ps1**: 提供底層 Git 操作
- **自動化操作**: 自動執行 Git 命令
- **錯誤處理**: 統一的錯誤處理機制

### 與需求管理系統集成
- **RequirementManagement.ps1**: 管理開發需求
- **分支關聯**: 將分支與需求關聯
- **進度追蹤**: 追蹤開發進度

### 與腳本整合系統集成
- **IntegrateAllScripts.ps1**: 統一腳本管理
- **自動化工作流**: 完整的開發工作流
- **監控和報告**: 詳細的操作報告

## 未來擴展

### 計劃功能
- **Web 界面**: 基於 Web 的分支管理界面
- **CI/CD 集成**: 與持續集成系統集成
- **多項目支持**: 支持多個項目管理
- **高級報告**: 詳細的分支分析報告

### 技術改進
- **性能優化**: 提高大型項目的性能
- **並行處理**: 支持並行檢查執行
- **智能建議**: 基於歷史數據的智能建議
- **機器學習**: 使用 ML 預測合併風險

## 總結

MingGoRTS 分支管理系統提供了一個完整、安全、自動化的 Git 工作流程解決方案。通過強制執行預合併檢查和主分支保護規則，確保代碼質量和項目穩定性。

系統的模組化設計使其易於擴展和維護，為開發團隊提供了強大的工具來管理複雜的開發工作流程。

---

**版本**: 1.0.0  
**最後更新**: 2026-03-23  
**維護者**: MingGoRTS 開發團隊
