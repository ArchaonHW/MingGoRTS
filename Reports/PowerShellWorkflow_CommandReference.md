# MingGoRTS PowerShell 工作流指令大全

**生成時間**: 2026-03-24  
**版本**: v1.0  
**適用範圍**: MingGoRTS 專案開發團隊

---

## 📋 目錄

1. [核心工作流程](#核心工作流程)
2. [編譯相關指令](#編譯相關指令)
3. [除錯相關指令](#除錯相關指令)
4. [版本控制指令](#版本控制指令)
5. [代碼審查指令](#代碼審查指令)
6. [BMAD 系統指令](#bmad-系統指令)
7. [API 工具指令](#api-工具指令)
8. [參數說明](#參數說明)
9. [使用範例](#使用範例)

---

## 🔄 核心工作流程

### 1. Debug Workflow - 除錯工作流
**檔案**: `Debug-Workflow.ps1`  
**用途**: 自動化 Bug 檢測與 BMAD 任務分配

```powershell
# 基本用法
.\Debug-Workflow.ps1

# 深度掃描並回報 BMAD 系統
.\Debug-Workflow.ps1 -DeepScan -ReportToBMAD

# 嘗試自動修復簡單 Bug
.\Debug-Workflow.ps1 -AutoFix

# 完整除錯流程
.\Debug-Workflow.ps1 -AutoFix -DeepScan -ReportToBMAD -ProjectRoot "C:\MingGoRTS"
```

### 2. Compilation Workflow - 編譯工作流
**檔案**: `Compilation-Workflow.ps1`  
**用途**: 自動化編譯工作流程系統

```powershell
# 自動化編譯模式
.\Compilation-Workflow.ps1 -AutoMode

# 手動編譯模式
.\Compilation-Workflow.ps1 -ManualMode

# 自動模式 + 自定義參數
.\Compilation-Workflow.ps1 -AutoMode -ScanInterval 60 -MaxConcurrentTasks 5
```

### 3. Version Control Workflow - 版控工作流
**檔案**: `VersionControl-Workflow.ps1`  
**用途**: 版控工作流程系統

```powershell
# 自動化版控模式
.\VersionControl-Workflow.ps1 -AutoMode

# 手動版控模式
.\VersionControl-Workflow.ps1 -ManualMode

# 自定義掃描間隔和並發數
.\VersionControl-Workflow.ps1 -AutoMode -ScanInterval 120 -MaxConcurrentCommits 3
```

### 4. Code Review Workflow - 代碼審查工作流
**檔案**: `CodeReview-Workflow.ps1`  
**用途**: 自動化代碼審查和安全掃描

```powershell
# 基本代碼審查
.\CodeReview-Workflow.ps1

# 深度安全掃描
.\CodeReview-Workflow.ps1 -DeepSecurityScan

# 自動修復模式
.\CodeReview-Workflow.ps1 -AutoFix

# 完整審查流程
.\CodeReview-Workflow.ps1 -DeepSecurityScan -AutoFix -GenerateReport
```

---

## 🔧 編譯相關指令

### Compilation Workflow 參數詳解

| 參數 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `AutoMode` | Switch | False | 自動化操作模式 |
| `ScanInterval` | Int | 30 | 掃描間隔（秒） |
| `MaxConcurrentTasks` | Int | 3 | 最大並發任務數 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `TasksPath` | String | $PWD\Tasks | 任務路徑 |
| `OutputPath` | String | $PWD\Logs | 輸出路徑 |

### 英文版本
```powershell
# 英文版編譯工作流
.\Compilation-Workflow-EN.ps1 -AutoMode
```

---

## 🐛 除錯相關指令

### Debug Workflow 參數詳解

| 參數 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `AutoFix` | Switch | False | 嘗試自動修復簡單的 Bug |
| `DeepScan` | Switch | False | 執行深度掃描（包括靜態分析） |
| `ReportToBMAD` | Switch | False | 將結果回報給 BMAD 系統 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `TaskOutputPath` | String | $PWD\Tasks\Debug | 任務輸出路徑 |

### PowerShell 5.1 相容版本
```powershell
# PowerShell 5.1 相容版本
.\Debug-Workflow-PS5.ps1 -AutoFix -DeepScan

# 英文版本
.\Debug-Workflow-EN.ps1 -DeepScan -ReportToBMAD

# PowerShell 5.1 英文版本
.\Debug-Workflow-PS5.ps1 -AutoFix
```

---

## 📝 版本控制指令

### Version Control Workflow 參數詳解

| 參數 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `AutoMode` | Switch | False | 自動化操作模式 |
| `ScanInterval` | Int | 60 | 掃描間隔（秒） |
| `MaxConcurrentCommits` | Int | 2 | 最大並發提交數 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `BranchName` | String | "main" | 目標分支名稱 |

### 英文版本
```powershell
# 英文版版控工作流
.\VersionControl-Workflow-EN.ps1 -AutoMode -ScanInterval 120
```

---

## 🔍 代碼審查指令

### Code Review Workflow 參數詳解

| 參數 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `DeepSecurityScan` | Switch | False | 深度安全掃描 |
| `AutoFix` | Switch | False | 自動修復發現的問題 |
| `GenerateReport` | Switch | False | 生成詳細審查報告 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `OutputPath` | String | $PWD\Reports | 報告輸出路徑 |
| `SeverityLevel` | String | "Medium" | 嚴重程度閾值 |

---

## 🎯 BMAD 系統指令

### BMAD Workflow System
**檔案**: `BMAD_WorkflowSystem.ps1`  
**用途**: BMAD 需求系統管理器

```powershell
# 基本用法
.\BMAD_WorkflowSystem.ps1

# 清理版本
.\BMAD_WorkflowSystem_Clean.ps1

# 英文版本
.\BMAD_WorkflowSystem_En.ps1

# 最終版本
.\BMAD_WorkflowSystem_Final.ps1

# 測試版本
.\BMAD_WorkflowSystem_Test.ps1

# 庫版本
.\BMAD_WorkflowSystem_Lib.ps1
```

### BMAD Coding Workflow
```powershell
# 基本編碼工作流
.\BMAD_CodingWorkflow.ps1

# 修復版本
.\BMAD_CodingWorkflow_Fixed.ps1

# 簡化版本
.\BMAD_CodingWorkflow_Simple.ps1
```

### 啟動 BMAD 工作流
```powershell
# 基本啟動
.\Start-BMADWorkflow.ps1

# 英文版本
.\Start-BMADWorkflow_En.ps1

# 最終版本
.\Start-BMADWorkflow_Final.ps1
```

---

## 🔌 API 工具指令

### SageBrain Console
```powershell
# 啟動 SageBrain 控制台
.\StartSageBrainConsole.ps1

# 修復版本
.\StartSageBrainConsole_Fixed.ps1
```

---

## 📊 參數說明

### 通用參數

| 參數名稱 | 類型 | 常用值 | 說明 |
|----------|------|--------|------|
| `AutoMode` | Switch | - | 自動化操作模式 |
| `ManualMode` | Switch | - | 手動操作模式 |
| `ProjectRoot` | String | $PWD | 專案根目錄路徑 |
| `OutputPath` | String | $PWD\Output | 輸出檔案路徑 |
| `ScanInterval` | Int | 30-120 | 掃描間隔（秒） |
| `MaxConcurrentTasks` | Int | 3-5 | 最大並發任務數 |
| `DeepScan` | Switch | - | 深度掃描模式 |
| `AutoFix` | Switch | - | 自動修復模式 |
| `ReportToBMAD` | Switch | - | 回報 BMAD 系統 |

### 路徑參數

| 參數 | 預設路徑 | 說明 |
|------|----------|------|
| `TasksPath` | $PWD\Tasks | 任務檔案目錄 |
| `LogsPath` | $PWD\Logs | 日誌檔案目錄 |
| `ReportsPath` | $PWD\Reports | 報告檔案目錄 |
| `DebugPath` | $PWD\Tasks\Debug | 除錯任務目錄 |
| `CodeReviewPath` | $PWD\Tasks\CodeReview | 代碼審查目錄 |

---

## 💡 使用範例

### 完整開發工作流程
```powershell
# 1. 開始除錯檢測
.\Debug-Workflow.ps1 -AutoFix -DeepScan -ReportToBMAD

# 2. 編譯專案
.\Compilation-Workflow.ps1 -AutoMode -ScanInterval 60

# 3. 代碼審查
.\CodeReview-Workflow.ps1 -DeepSecurityScan -AutoFix -GenerateReport

# 4. 版本控制
.\VersionControl-Workflow.ps1 -AutoMode -MaxConcurrentCommits 3
```

### 快速修復流程
```powershell
# 快速除錯 + 編譯
.\Debug-Workflow.ps1 -AutoFix | .\Compilation-Workflow.ps1 -AutoMode
```

### 深度檢查流程
```powershell
# 深度除錯 + 安全掃描
.\Debug-Workflow.ps1 -DeepScan -ReportToBMAD
.\CodeReview-Workflow.ps1 -DeepSecurityScan -GenerateReport
```

### BMAD 系統整合
```powershell
# 啟動 BMAD 工作流系統
.\Start-BMADWorkflow.ps1

# 使用 BMAD 編碼工作流
.\BMAD_CodingWorkflow.ps1
```

---

## 🔧 環境設定

### PowerShell 版本要求
- **推薦**: PowerShell 7.0+
- **最低**: PowerShell 5.1
- **相容性**: 提供 PS5.1 相容版本

### 執行權限設定
```powershell
# 設置執行權限
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# 或臨時繞過執行政策
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process
```

### 模組依賴
```powershell
# 安裝必要模組（如果需要）
Install-Module -Name PowerShellGet -Force
Install-Module -Name PackageManagement -Force
```

---

## 📝 日誌和輸出

### 日誌檔案位置
- **Debug 日誌**: `Logs\Debug_YYYYMMDD.log`
- **編譯日誌**: `Logs\Compile_YYYYMMDD.log`
- **版控日誌**: `Logs\VersionControl_YYYYMMDD.log`
- **代碼審查日誌**: `Logs\CodeReview_YYYYMMDD.log`

### 報告檔案位置
- **Debug 報告**: `Tasks\Debug\DebugReport_YYYYMMDD_HHMMSS.md`
- **編譯報告**: `Reports\CompilationReport_YYYYMMDD.md`
- **代碼審查報告**: `Reports\CodeReviewReport_YYYYMMDD.md`
- **BMAD 報告**: `Reports\BMADReport_YYYYMMDD.md`

---

## ⚠️ 注意事項

### 安全提醒
1. 在執行自動修復前，請務必備份程式碼
2. 版本控制操作前，確認當前分支狀態
3. 深度掃描可能耗時較長，建議在非工作時間執行

### 效能建議
1. 調整 `ScanInterval` 參數以平衡即時性和效能
2. 根據系統資源調整 `MaxConcurrentTasks`
3. 大型專案建議使用 `DeepScan` 模式

### 故障排除
1. 如果腳本執行失敗，檢查 PowerShell 版本
2. 確認專案路徑和權限設定
3. 查看日誌檔案獲取詳細錯誤信息

---

## 📞 技術支援

### 常見問題
1. **執行權限錯誤**: 請設置正確的 ExecutionPolicy
2. **路徑錯誤**: 確認 ProjectRoot 參數設定
3. **並發任務過多**: 調整 MaxConcurrentTasks 參數

### 聯絡方式
- **專案文檔**: 查看 `docs\` 目錄下的相關文檔
- **日誌分析**: 檢查 `Logs\` 目錄下的日誌檔案
- **問題回報**: 使用 Debug Workflow 生成問題報告

---

*此指令大全由 MingGoRTS 開發團隊維護*  
*最後更新: 2026-03-24*  
*版本: v1.0*
