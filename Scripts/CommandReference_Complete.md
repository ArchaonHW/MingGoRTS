# MingGoRTS 腳本指令大全一覽

**生成時間**: 2026-03-24  
**版本**: v2.0  
**適用範圍**: MingGoRTS 專案開發團隊  
**總計腳本數**: 28個 PowerShell 腳本

---

## 📋 目錄

1. [核心工作流程](#核心工作流程)
2. [編譯相關指令](#編譯相關指令)
3. [除錯相關指令](#除錯相關指令)
4. [版本控制指令](#版本控制指令)
5. [代碼審查指令](#代碼審查指令)
6. [BMAD 系統指令](#bmad-系統指令)
7. [部署工具指令](#部署工具指令)
8. [API 工具指令](#api-工具指令)
9. [編碼工作流指令](#編碼工作流指令)
10. [系統維護指令](#系統維護指令)
11. [參數說明](#參數說明)
12. [使用範例](#使用範例)
13. [故障排除](#故障排除)

---

## 🔄 核心工作流程

### 1. Debug Workflow - 除錯工作流
**檔案**: `Debug-Workflow.ps1`  
**用途**: 自動化 Bug 檢測與 BMAD 任務分配  
**版本**: PS7.0, PS5.1相容版, 英文版

```powershell
# 基本用法
.\Debug-Workflow.ps1

# 深度掃描並回報 BMAD 系統
.\Debug-Workflow.ps1 -DeepScan -ReportToBMAD

# 嘗試自動修復簡單 Bug
.\Debug-Workflow.ps1 -AutoFix

# 完整除錯流程
.\Debug-Workflow.ps1 -AutoFix -DeepScan -ReportToBMAD -ProjectRoot "C:\MingGoRTS"

# PowerShell 5.1 相容版本
.\Debug-Workflow-PS5.ps1 -AutoFix -DeepScan

# 英文版本
.\Debug-Workflow-EN.ps1 -DeepScan -ReportToBMAD
```

### 2. Compilation Workflow - 編譯工作流
**檔案**: `Compilation-Workflow.ps1`  
**用途**: 自動化編譯工作流程系統  
**版本**: 中文版, 英文版

```powershell
# 自動化編譯模式
.\Compilation-Workflow.ps1 -AutoMode

# 手動編譯模式
.\Compilation-Workflow.ps1 -ManualMode

# 自動模式 + 自定義參數
.\Compilation-Workflow.ps1 -AutoMode -ScanInterval 60 -MaxConcurrentTasks 5

# 英文版編譯工作流
.\Compilation-Workflow-EN.ps1 -AutoMode
```

### 3. Version Control Workflow - 版控工作流
**檔案**: `VersionControl-Workflow.ps1`  
**用途**: 版控工作流程系統  
**版本**: 中文版, 英文版

```powershell
# 自動化版控模式
.\VersionControl-Workflow.ps1 -AutoMode

# 手動版控模式
.\VersionControl-Workflow.ps1 -ManualMode

# 自定義掃描間隔和並發數
.\VersionControl-Workflow.ps1 -AutoMode -ScanInterval 120 -MaxConcurrentCommits 3

# 英文版版控工作流
.\VersionControl-Workflow-EN.ps1 -AutoMode -ScanInterval 120
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
| `ManualMode` | Switch | False | 手動操作模式 |
| `ScanInterval` | Int | 30 | 掃描間隔（秒） |
| `MaxConcurrentTasks` | Int | 3 | 最大並發任務數 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `TasksPath` | String | $PWD\Tasks | 任務路徑 |
| `OutputPath` | String | $PWD\Logs | 輸出路徑 |

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

### 可用版本
- `Debug-Workflow.ps1` - 標準版本 (PS7.0+)
- `Debug-Workflow-PS5.ps1` - PowerShell 5.1 相容版本
- `Debug-Workflow-EN.ps1` - 英文版本

---

## 📝 版本控制指令

### Version Control Workflow 參數詳解

| 參數 | 類型 | 預設值 | 說明 |
|------|------|--------|------|
| `AutoMode` | Switch | False | 自動化操作模式 |
| `ManualMode` | Switch | False | 手動操作模式 |
| `ScanInterval` | Int | 60 | 掃描間隔（秒） |
| `MaxConcurrentCommits` | Int | 2 | 最大並發提交數 |
| `ProjectRoot` | String | $PWD | 專案根目錄 |
| `BranchName` | String | "main" | 目標分支名稱 |

### 可用版本
- `VersionControl-Workflow.ps1` - 中文版
- `VersionControl-Workflow-EN.ps1` - 英文版

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

# 修復版本
.\BMAD_WorkflowSystem_Fixed.ps1
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

## 🚀 部署工具指令

### MingGoRTS 部署腳本
**檔案**: `Deploy-MingGoRTS.ps1`  
**用途**: UE5 專案自動化部署

```powershell
# 基本部署
.\Deploy-MingGoRTS.ps1 -Configuration Shipping

# 指定平台部署
.\Deploy-MingGoRTS.ps1 -Configuration Development -TargetPlatform Win64

# 跳過測試的快速部署
.\Deploy-MingGoRTS.ps1 -Configuration Shipping -SkipTests

# 自定義輸出目錄
.\Deploy-MingGoRTS.ps1 -Configuration Shipping -OutputDir "D:\Deployments"

# 完整部署參數
.\Deploy-MingGoRTS.ps1 -Configuration Shipping -TargetPlatform Win64 -SkipTests -OutputDir "D:\Deployments"
```

#### 部署參數

| 參數 | 類型 | 選項 | 說明 |
|------|------|------|------|
| `Configuration` | String | Development, Shipping, Debug, Test | 編譯配置 |
| `TargetPlatform` | String | Win64, Android, IOS, Mac, Linux | 目標平台 |
| `SkipTests` | Switch | - | 跳過測試執行 |
| `OutputDir` | String | - | 輸出目錄 |

---

## 🔌 API 工具指令

### SageBrain Console
**檔案**: `StartSageBrainConsole.ps1`  
**用途**: Sage Brain 控制台互動腳本

```powershell
# 完整演示
.\StartSageBrainConsole.ps1 -FullDemo

# 快速啟動
.\StartSageBrainConsole.ps1 -QuickStart

# 僅執行測試
.\StartSageBrainConsole.ps1 -TestOnly

# 互動模式
.\StartSageBrainConsole.ps1 -Interactive

# 執行自定義命令
.\StartSageBrainConsole.ps1 -CustomCommand "your-command"

# 詳細輸出模式
.\StartSageBrainConsole.ps1 -Verbose

# 顯示幫助
.\StartSageBrainConsole.ps1 -Help

# 修復版本
.\StartSageBrainConsole_Fixed.ps1 -FullDemo
```

#### SageBrain 參數

| 參數 | 類型 | 說明 |
|------|------|------|
| `FullDemo` | Switch | 執行完整控制台互動演示 |
| `QuickStart` | Switch | 快速啟動控制台互動 |
| `TestOnly` | Switch | 僅執行測試功能 |
| `Interactive` | Switch | 進入互動模式 |
| `CustomCommand` | String | 執行自定義命令 |
| `Verbose` | Switch | 顯示詳細輸出 |
| `Help` | Switch | 顯示幫助信息 |

---

## 💻 編碼工作流指令

### BMAD 編碼工作流系統

```powershell
# 標準編碼工作流
.\BMAD_CodingWorkflow.ps1

# 修復版本（修復已知問題）
.\BMAD_CodingWorkflow_Fixed.ps1

# 簡化版本（精簡功能）
.\BMAD_CodingWorkflow_Simple.ps1
```

---

## 🛠️ 系統維護指令

### 編碼問題修復
**檔案**: `Fix-EncodingIssues.ps1`  
**用途**: 修復編碼問題

```powershell
# 基本修復
.\Fix-EncodingIssues.ps1

# 修復版本
.\Fix-EncodingIssues-Fixed.ps1

# 簡化版本
.\Fix-EncodingIssues-Simple.ps1
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

### 完整部署流程
```powershell
# 編譯 + 測試 + 部署
.\Compilation-Workflow.ps1 -AutoMode
.\Deploy-MingGoRTS.ps1 -Configuration Shipping -TargetPlatform Win64
```

### API 開發流程
```powershell
# 啟動 SageBrain 控制台進行開發
.\StartSageBrainConsole.ps1 -Interactive

# 或執行完整演示
.\StartSageBrainConsole.ps1 -FullDemo
```

---

## 🔧 環境設定

### PowerShell 版本要求
- **推薦**: PowerShell 7.0+
- **最低**: PowerShell 5.1
- **相容性**: 提供PS5.1相容版本

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
- **BMAD 日誌**: `Logs\BMAD_YYYYMMDD.log`

### 報告檔案位置
- **Debug 報告**: `Tasks\Debug\DebugReport_YYYYMMDD_HHMMSS.md`
- **編譯報告**: `Reports\CompilationReport_YYYYMMDD.md`
- **代碼審查報告**: `Reports\CodeReviewReport_YYYYMMDD.md`
- **BMAD 報告**: `Reports\BMADReport_YYYYMMDD.md`
- **部署報告**: `Reports\DeploymentReport_YYYYMMDD.md`

---

## ⚠️ 故障排除

### 常見問題及解決方案

#### 1. 執行權限錯誤
**症狀**: `無法載入檔案，因為在此系統上禁止執行腳本`
**解決**:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### 2. 路徑錯誤
**症狀**: 找不到檔案或目錄
**解決**: 確認 `ProjectRoot` 參數設定正確
```powershell
.\Debug-Workflow.ps1 -ProjectRoot "C:\YourProjectPath"
```

#### 3. 並發任務過多
**症狀**: 系統資源不足
**解決**: 調整 `MaxConcurrentTasks` 參數
```powershell
.\Compilation-Workflow.ps1 -AutoMode -MaxConcurrentTasks 2
```

#### 4. 編碼問題
**症狀**: 中文字符顯示異常
**解決**: 使用英文版本或編碼修復腳本
```powershell
.\Debug-Workflow-EN.ps1
.\Fix-EncodingIssues.ps1
```

#### 5. PowerShell 版本不相容
**症狀**: 語法錯誤或不支援的功能
**解決**: 使用 PowerShell 5.1 相容版本
```powershell
.\Debug-Workflow-PS5.ps1
```

### 效能優化建議

1. **掃描間隔調整**: 根據專案大小調整 `ScanInterval` 參數
2. **並發任務控制**: 根據系統資源調整 `MaxConcurrentTasks`
3. **深度掃描時機**: 大型專案建議在非工作時間執行 `DeepScan`
4. **日誌管理**: 定期清理舊日誌檔案以釋放空間

### 進階故障排除

#### 檢查系統狀態
```powershell
# 檢查 PowerShell 版本
$PSVersionTable

# 檢查執行權限
Get-ExecutionPolicy -List

# 檢查模組狀態
Get-Module -ListAvailable
```

#### 詳細錯誤信息
```powershell
# 啟用詳細輸出
.\Debug-Workflow.ps1 -Verbose

# 查看錯誤日誌
Get-Content "Logs\Debug_$(Get-Date -Format 'yyyyMMdd').log" | Select-String "ERROR"
```

---

## 📞 技術支援

### 聯絡方式
- **專案文檔**: 查看 `docs\` 目錄下的相關文檔
- **日誌分析**: 檢查 `Logs\` 目錄下的日誌檔案
- **問題回報**: 使用 Debug Workflow 生成問題報告

### 相關文檔
- `docs\BMAD_BRAIN_WORKFLOW_GUIDE.md` - BMAD 系統指南
- `Tools\workflow\docs\BMAD_WORKFLOW_GUIDE.md` - 工作流詳細指南
- `Reports\PowerShellWorkflow_CommandReference.md` - 舊版指令參考

---

## 📈 腳本統計

### 分類統計
- **核心工作流**: 4個腳本 (Debug, Compile, VersionControl, CodeReview)
- **BMAD系統**: 9個腳本 (多種版本和變體)
- **API工具**: 2個腳本 (SageBrain Console)
- **部署工具**: 1個腳本 (Deploy-MingGoRTS)
- **編碼工作流**: 3個腳本
- **系統維護**: 3個腳本 (編碼修復)
- **啟動腳本**: 3個腳本 (Start-BMADWorkflow)

### 版本支援
- **PowerShell 7.0+**: 25個腳本
- **PowerShell 5.1**: 3個相容腳本
- **英文版本**: 6個腳本
- **中文版本**: 22個腳本

---

*此指令大全由 MingGoRTS 開發團隊維護*  
*最後更新: 2026-03-24*  
*版本: v2.0*  
*總計腳本數: 28個*
