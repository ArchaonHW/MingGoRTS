# MingGoRTS 系統測試指南

## 概述

本指南詳細介紹了 MingGoRTS 系統集成測試和性能優化驗證的完整流程，確保所有系統在高負載下穩定運行。

## 測試架構

### 測試類別

#### 1. 系統集成測試
- **核心系統集成**: AI內容質量控制器、多人系統、技術債務管理器、性能監控
- **個人系統集成**: 關係管理器、音頻系統、UI系統、保存載入
- **RTS核心集成**: 單位管理器、戰鬥系統、資源管理器、建築系統
- **AI系統集成**: 自我學習、動態任務生成、行為預測、進化AI
- **高級功能集成**: 本地化系統、自動場景生成、跨媒體敘事、情感AI

#### 2. 性能優化驗證
- **系統性能**: CPU使用率、內存使用、GPU性能、磁盤IO
- **遊戲性能**: 幀率、渲染性能、物理性能、音頻性能
- **網絡性能**: 延遲、帶寬、多人同步、連接壓力
- **AI性能**: AI處理、路徑規劃、決策制定、學習算法
- **資源性能**: 資源分配、內存池、資產載入、垃圾回收

## 測試工具

### 1. 系統集成測試套件
**文件**: `Tools/testing/SystemIntegrationTestSuite.ps1`

**功能**:
- 5個主要測試類別，20個具體測試項目
- 自動化測試執行和報告生成
- 詳細的錯誤診斷和日誌記錄
- 支持完整測試和連續測試模式

**使用方法**:
```powershell
# 執行完整測試套件
.\SystemIntegrationTestSuite.ps1 -TestMode Full -OutputPath "c:\HW\MingGoRTS\TestReports" -Verbose

# 執行連續測試
.\SystemIntegrationTestSuite.ps1 -TestMode Continuous -OutputPath "c:\HW\MingGoRTS\TestReports"
```

### 2. 性能優化驗證套件
**文件**: `Tools/testing/PerformanceOptimizationVerification.ps1`

**功能**:
- 5個性能測試類別，20個性能指標測試
- 實時性能監控和數據收集
- 性能基準測試和壓力測試
- 詳細的性能分析報告

**使用方法**:
```powershell
# 標準性能測試
.\PerformanceOptimizationVerification.ps1 -TestMode Standard -DurationMinutes 30

# 壓力測試
.\PerformanceOptimizationVerification.ps1 -TestMode Stress -DurationMinutes 60 -StressTest

# 連續性能監控
.\PerformanceOptimizationVerification.ps1 -TestMode Continuous -DurationMinutes 120 -Continuous
```

### 3. 系統測試執行器
**文件**: `Tools/testing/RunSystemTests.cmd`

**功能**:
- 一鍵執行所有系統測試
- 自動生成測試摘要報告
- 統一的測試結果評估

**使用方法**:
```cmd
# 執行完整系統測試
RunSystemTests.cmd Full c:\HW\MingGoRTS\TestReports 30 true

# 執行壓力測試
RunSystemTests.cmd Stress c:\HW\MingGoRTS\TestReports 60 true

# 執行連續測試
RunSystemTests.cmd Continuous c:\HW\MingGoRTS\TestReports 120 true
```

## 測試指標

### 系統集成測試指標

#### 成功率標準
- **優秀**: ≥95% 測試通過
- **良好**: 90-94% 測試通過
- **需要改進**: <90% 測試通過

#### 關鍵測試項目
1. **AI內容質量控制器集成**
   - 多供應商集成測試
   - 質量控制流程測試
   - 性能監控集成測試

2. **多人系統集成**
   - 網絡同步測試
   - 關係數據同步測試
   - 會話管理測試

3. **技術債務管理器集成**
   - 代碼質量分析集成測試
   - 性能監控集成測試
   - 優化計劃執行測試

### 性能優化驗證指標

#### 系統性能基準
- **CPU使用率**: ≤80%
- **內存使用率**: ≤85%
- **GPU使用率**: ≤90%
- **磁盤IO延遲**: ≤50ms

#### 遊戲性能基準
- **目標幀率**: ≥60 FPS
- **最低幀率**: ≥30 FPS
- **渲染時間**: ≤16.67ms
- **物理計算時間**: ≤5ms

#### 網絡性能基準
- **網絡延遲**: ≤100ms
- **多人同步延遲**: ≤50ms
- **數據包丟失率**: ≤2%
- **連接建立時間**: ≤200ms

#### AI性能基準
- **AI決策時間**: ≤30ms
- **路徑規劃時間**: ≤10ms
- **學習算法訓練時間**: ≤50ms
- **AI處理負載**: ≤70%

## 測試流程

### 1. 測試準備

#### 環境要求
- Windows 10/11 操作系統
- PowerShell 5.1 或更高版本
- 至少 8GB RAM
- 至少 10GB 可用磁盤空間
- 網絡連接（用於網絡性能測試）

#### 依賴檢查
```powershell
# 檢查 PowerShell 版本
$PSVersionTable.PSVersion

# 檢查磁盤空間
Get-PSDrive C

# 檢查網絡連接
Test-Connection google.com
```

### 2. 執行測試

#### 快速測試
```cmd
# 執行快速系統檢查
RunSystemTests.cmd Quick
```

#### 完整測試
```cmd
# 執行完整系統測試
RunSystemTests.cmd Full
```

#### 壓力測試
```cmd
# 執行壓力測試
RunSystemTests.cmd Stress
```

### 3. 結果分析

#### 測試報告位置
```
TestReports/
├── Integration/
│   ├── 測試日誌和詳細結果
├── Performance/
│   ├── 性能監控數據和基準
├── Logs/
│   ├── 完整的執行日誌
└── Reports/
    ├── JSON格式的測試報告
    └── 系統測試摘要
```

#### 報告解讀
- **IntegrationTestReport_*.json**: 系統集成測試詳細結果
- **PerformanceTestReport_*.json**: 性能測試詳細結果
- **SystemTestSummary_*.json**: 系統測試總體摘要

## 故障排除

### 常見問題

#### 1. 測試執行失敗
**症狀**: PowerShell 腳本執行錯誤
**解決方案**:
```powershell
# 檢查執行策略
Get-ExecutionPolicy

# 設置執行策略
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### 2. 性能計數器訪問失敗
**症狀**: 無法獲取系統性能數據
**解決方案**:
```powershell
# 以管理員身份運行 PowerShell
Start-Process PowerShell -Verb RunAs

# 檢查性能計數器
Get-Counter -ListSet *
```

#### 3. 網絡測試失敗
**症狀**: 網絡延遲測試無法執行
**解決方案**:
```powershell
# 檢查防火牆設置
Get-NetFirewallRule

# 測試基本網絡連接
Test-NetConnection google.com -Port 80
```

### 調試技巧

#### 1. 啟用詳細輸出
```powershell
# 執行測試時啟用詳細輸出
.\SystemIntegrationTestSuite.ps1 -Verbose
```

#### 2. 檢查日誌文件
```powershell
# 查看最新的測試日誌
Get-Content "TestReports\Logs\*.log" | Select-Object -Last 50
```

#### 3. 單獨執行測試項目
```powershell
# 單獨測試特定功能
.\SystemIntegrationTestSuite.ps1 -TestCategory "Core_System_Integration" -TestName "AI_Content_Quality_Controller_Integration"
```

## 最佳實踐

### 1. 測試策略
- **定期執行**: 每日執行完整測試套件
- **持續集成**: 在代碼提交後自動執行測試
- **發布前驗證**: 在版本發布前執行完整測試

### 2. 性能監控
- **建立基準**: 記錄正常狀態下的性能基準
- **趨勢分析**: 監控性能指標的長期趨勢
- **警報設置**: 為關鍵指標設置警報閾值

### 3. 測試環境
- **隔離環境**: 在專用的測試環境中執行測試
- **一致性**: 確保測試環境的一致性
- **資源充足**: 確保測試環境有足夠的系統資源

## 自動化集成

### CI/CD 集成

#### GitHub Actions 示例
```yaml
name: System Tests
on: [push, pull_request]

jobs:
  system-tests:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v2
    - name: Run System Tests
      run: |
        .\Tools\testing\RunSystemTests.cmd Full
      shell: cmd
```

#### Azure DevOps 示例
```yaml
trigger:
- main

pool:
  vmImage: 'windows-latest'

steps:
- task: PowerShell@2
  inputs:
    filePath: 'Tools/testing/RunSystemTests.cmd'
    arguments: 'Full $(Agent.TempDirectory)/TestReports'
```

### 報告自動化

#### 自動生成報告
```powershell
# 自動生成測試報告並發送通知
$Report = .\Tools\testing\RunSystemTests.cmd Full
if ($Report.Status -eq "FAILED") {
    Send-MailMessage -To "team@example.com" -Subject "System Tests Failed" -Body $Report.Summary
}
```

## 擴展和自定義

### 添加新測試項目

#### 1. 系統集成測試
```powershell
# 在 SystemIntegrationTestSuite.ps1 中添加新測試
function Test-NewFeatureIntegration {
    $TestResults = @()
    
    # 添加測試邏輯
    $TestResults += @{
        Test = "New_Feature_Integration"
        Status = "Passed"
        Details = "驗證新功能集成"
    }
    
    return $TestResults
}
```

#### 2. 性能測試
```powershell
# 在 PerformanceOptimizationVerification.ps1 中添加新測試
function Test-NewFeaturePerformance {
    param([int]$DurationSeconds)
    
    # 添加性能測試邏輯
    return @{
        AverageTime = 10.5
        MaxTime = 15.2
        WithinTarget = $true
    }
}
```

### 自定義性能基準
```powershell
# 修改性能指標
$Global:PerformanceMetrics = @{
    TargetFPS = 120  # 提高目標幀率
    MaxCPUUsage = 70  # 降低CPU使用率閾值
    MaxMemoryUsage = 80  # 降低內存使用率閾值
}
```

## 總結

MingGoRTS 系統測試框架提供了全面的系統集成和性能驗證能力，確保遊戲在各種負載條件下的穩定運行。通過定期執行這些測試，開發團隊可以：

- 及早發現系統集成問題
- 監控性能趨勢和瓶頸
- 確保代碼質量和系統穩定性
- 為發布提供質量保證

通過遵循本指南中的最佳實踐，團隊可以建立可靠的測試流程，為 MingGoRTS 的成功發布奠定堅實基礎。
