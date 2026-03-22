# MingGoRTS 自動化弱點掃描系統文檔
# MingGoRTS Automated Vulnerability Scanning System Documentation

## 概述 / Overview

MingGoRTS 自動化弱點掃描系統是一個全面的安全掃描解決方案，專為 Unreal Engine 5 C++ 項目設計。系統提供代碼安全分析、依賴項檢查、配置文件審查和自動化報告生成。

The MingGoRTS Automated Vulnerability Scanning System is a comprehensive security scanning solution designed for Unreal Engine 5 C++ projects. It provides code security analysis, dependency checking, configuration file auditing, and automated report generation.

## 系統組件 / System Components

### 1. SecurityScan.ps1
主要掃描腳本，提供以下功能：
- 代碼安全掃描 (C++/C#)
- 危險函數檢測
- 硬編碼憑證識別
- SQL注入風險檢測
- 緩衝區溢出檢測
- 依賴項漏洞掃描
- 配置文件安全審查
- Blueprint資產分析

Main scanning script providing:
- Code security scanning (C++/C#)
- Dangerous function detection
- Hardcoded credential identification
- SQL injection risk detection
- Buffer overflow detection
- Dependency vulnerability scanning
- Configuration file security audit
- Blueprint asset analysis

### 2. SecurityConfig.json
配置文件，定義：
- 掃描規則和模式
- 嚴重性級別
- 排除路徑
- 通知設置
- CI/CD集成選項

Configuration file defining:
- Scan rules and patterns
- Severity levels
- Exclusion paths
- Notification settings
- CI/CD integration options

### 3. GitHub Actions Workflow
自動化CI/CD集成：
- 代碼提交時自動掃描
- PR安全檢查
- 定時掃描 (每日)
- CodeQL高級分析
- 依賴項審查
- 機密檢測

Automated CI/CD integration:
- Automatic scanning on code commit
- PR security checks
- Scheduled scans (daily)
- CodeQL advanced analysis
- Dependency review
- Secret detection

## 掃描類型 / Scan Types

| 類型 / Type | 描述 / Description | 耗時 / Duration | 使用場景 / Use Case |
|------------|-------------------|----------------|-------------------|
| **Full** | 完整掃描所有項目 / Complete scan of all items | 10-30分鐘 | 發布前檢查 / Pre-release check |
| **Code** | 僅代碼安全掃描 / Code security only | 5-15分鐘 | 開發過程中 / During development |
| **Dependencies** | 依賴項漏洞掃描 / Dependency vulnerabilities | 2-5分鐘 | 更新依賴後 / After dependency update |
| **Quick** | 快速掃描關鍵問題 / Quick scan for critical issues | 1-3分鐘 | 快速驗證 / Quick validation |
| **CI** | CI模式，嚴重問題失敗 / CI mode, fail on critical | 5-10分鐘 | 自動化流程 / Automated pipeline |

## 安裝與設置 / Installation and Setup

### 1. 先決條件 / Prerequisites
```powershell
# PowerShell 5.1 或更高版本 / PowerShell 5.1 or higher
$PSVersionTable.PSVersion

# Git (用於GitHub Actions) / Git for GitHub Actions
git --version
```

### 2. 文件位置 / File Locations
```
MingGoRTS/
├── Tools/
│   └── security/
│       ├── SecurityScan.ps1        # 主掃描腳本
│       └── SecurityConfig.json     # 配置文件
├── .github/
│   └── workflows/
│       └── security-scan.yml       # GitHub Actions
├── RUN_SECURITY_SCAN.bat           # 啟動腳本
└── Reports/
    └── Security/                   # 掃描報告輸出
```

### 3. 配置調整 / Configuration
編輯 `Tools\security\SecurityConfig.json`：

```json
{
  "SecurityScan": {
    "ScanProfiles": {
      "Full": {
        "Scans": ["Code", "Dependencies", "Configuration"],
        "Timeout": 3600
      }
    },
    "SeverityRules": {
      "Critical": {
        "Severity": 9,
        "FailBuild": true
      }
    }
  }
}
```

## 使用方法 / Usage

### 方法一：批次啟動器 / Method 1: Batch Launcher
```batch
# 雙擊運行 / Double-click to run
RUN_SECURITY_SCAN.bat

# 選擇掃描類型 / Select scan type:
# [1] Full Scan - 完整掃描
# [2] Code Scan - 代碼掃描
# [3] Dependency Scan - 依賴掃描
# [4] Quick Scan - 快速掃描
```

### 方法二：PowerShell直接運行 / Method 2: PowerShell Direct
```powershell
# 完整掃描 / Full scan
.\Tools\security\SecurityScan.ps1 -ScanType Full -GenerateReport

# 代碼掃描 / Code scan
.\Tools\security\SecurityScan.ps1 -ScanType Code -GenerateReport

# 快速掃描 (僅關鍵問題) / Quick scan (critical only)
.\Tools\security\SecurityScan.ps1 -ScanType Quick -SeverityThreshold 7

# CI模式 (嚴重問題導致失敗) / CI mode (fail on critical)
.\Tools\security\SecurityScan.ps1 -ScanType CI -FailOnCritical
```

### 方法三：GitHub Actions自動化 / Method 3: GitHub Actions Automation
系統已配置為以下情況自動運行：
- 推送代碼到 main/develop 分支
- 創建 Pull Request
- 每日凌晨2點 (定時掃描)
- 手動觸發 (workflow_dispatch)

Automatically runs on:
- Code push to main/develop branches
- Pull Request creation
- Daily at 2 AM (scheduled)
- Manual trigger (workflow_dispatch)

## 掃描規則 / Scan Rules

### 代碼安全規則 / Code Security Rules

| 規則 / Rule | 嚴重性 / Severity | 描述 / Description |
|-----------|------------------|-------------------|
| DangerousFunctions | High (8) | 使用 strcpy, strcat, sprintf 等危險函數 |
| HardcodedPassword | Critical (9) | 硬編碼密碼或密鑰 |
| SQLInjection | Critical (9) | 潛在SQL注入風險 |
| BufferOverflow | High (7) | 潛在緩衝區溢出 |
| InsecureRandom | Medium (5) | 使用不安全的隨機數生成器 |
| PathTraversal | High (7) | 路徑遍歷攻擊風險 |
| ResourceLeak | Medium (6) | 資源洩漏 (未釋放內存) |
| IntegerOverflow | Medium (6) | 整數溢出風險 |
| FormatString | High (7) | 格式字符串漏洞 |
| RaceCondition | Medium (6) | 競態條件 |
| SensitiveLogging | High (8) | 敏感數據記錄到日誌 |

### 配置文件規則 / Configuration Rules

| 規則 / Rule | 嚴重性 / Severity | 描述 / Description |
|-----------|------------------|-------------------|
| DebugMode | Low (4) | 調試模式可能啟用 |
| VerboseLogging | Low (3) | 詳細日誌記錄啟用 |
| WeakCrypto | High (8) | 弱加密或硬編碼密鑰 |

## 報告格式 / Report Formats

系統生成三種格式的報告：

### 1. HTML報告 / HTML Report
- 視覺化儀表板
- 嚴重性分級顯示
- 代碼片段高亮
- 交互式表格
- 趨勢分析圖表

Location: `Reports\Security\SecurityScan_Report_YYYYMMDD_HHMMSS.html`

### 2. JSON報告 / JSON Report
- 完整機器可讀數據
- 供其他工具集成使用
- 包含所有漏洞詳情

Location: `Reports\Security\SecurityScan_Report_YYYYMMDD_HHMMSS.json`

### 3. CSV報告 / CSV Report
- 表格格式
- 適合Excel分析
- 便於追蹤和排序

Location: `Reports\Security\SecurityScan_Report_YYYYMMDD_HHMMSS.csv`

## CI/CD集成 / CI/CD Integration

### GitHub Actions
```yaml
# 已在 .github/workflows/security-scan.yml 中配置
# 自動運行於：
# - 每次推送到主要分支
# - 每個Pull Request
# - 每日定時掃描
```

### 手動觸發 / Manual Trigger
1. 進入GitHub Actions頁面
2. 選擇 "Security Scan" workflow
3. 點擊 "Run workflow"
4. 選擇掃描類型 (Full/Code/Dependencies/Quick)

### Jenkins集成 / Jenkins Integration
```groovy
// Jenkinsfile 示例
pipeline {
    agent any
    stages {
        stage('Security Scan') {
            steps {
                powersScript '.\Tools\security\SecurityScan.ps1 -ScanType CI -FailOnCritical'
            }
        }
    }
}
```

## 常見問題 / FAQ

### Q: 掃描時間太長怎麼辦？
**A**: 使用 Quick 或 Code 掃描類型，或調整 `ExcludePaths` 排除不需要掃描的目錄。

### Q: 如何減少誤報？
**A**: 編輯 `SecurityConfig.json` 調整規則嚴重性，或使用註釋標記誤報代碼：
```cpp
// nosec: 這是安全的，因為...
strcpy(dest, src);
```

### Q: 掃描報告在哪裡查看？
**A**: 
- 本地：運行 `RUN_SECURITY_SCAN.bat` 選擇選項 6
- GitHub Actions：在Actions頁面下載Artifacts

### Q: 如何添加自定義掃描規則？
**A**: 編輯 `SecurityScan.ps1` 中的 `$codePatterns` 哈希表，添加新的正則表達式模式。

## 安全最佳實踐 / Security Best Practices

### 1. 定期掃描
- 每次重大代碼提交前運行 Full 掃描
- 每日運行 Quick 掃描
- 每月審查所有 Medium 以上嚴重性問題

### 2. 問題處理流程
```
發現問題 → 評估嚴重性 → 分配責任人 → 修復 → 重新掃描驗證
```

### 3. 嚴重性級別處理標準
| 級別 / Level | 響應時間 / Response Time | 修復期限 / Fix Deadline |
|-----------|------------------------|----------------------|
| Critical | 立即 / Immediate | 24小時內 / Within 24h |
| High | 24小時內 / Within 24h | 1週內 / Within 1 week |
| Medium | 1週內 / Within 1 week | 1個月內 / Within 1 month |
| Low | 下次迭代 / Next sprint | 酌情 / As appropriate |

### 4. 代碼審查整合
- 所有 PR 必須通過安全掃描
- Critical/High 問題必須修復後才能合併
- 定期進行安全培訓

## 故障排除 / Troubleshooting

### 問題：PowerShell 執行策略錯誤
```powershell
# 解決方案 / Solution
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### 問題：找不到報告文件
```powershell
# 檢查輸出目錄 / Check output directory
Test-Path "Reports\Security"
New-Item -ItemType Directory -Path "Reports\Security" -Force
```

### 問題：掃描過程中內存不足
```powershell
# 減少並行掃描文件數量 / Reduce concurrent scan files
# 編輯 SecurityScan.ps1 中的批次大小
```

## 更新日誌 / Changelog

### v1.0 (2026-03-23)
- 初始版本發布
- 支持5種掃描類型
- 13種代碼安全規則
- 3種報告格式 (HTML/JSON/CSV)
- GitHub Actions 集成
- 繁體中文/English 雙語支持

## 聯繫支持 / Support

如有問題或建議，請通過以下方式聯繫：
- 創建 GitHub Issue
- 發送郵件至 security@minggorts.com

---

**注意**: 此掃描系統提供自動化檢測，但不能替代人工安全審查。對於關鍵系統，建議結合專業安全團隊進行深度審計。

**Note**: This scanning system provides automated detection but cannot replace manual security review. For critical systems, it is recommended to combine with professional security team for in-depth auditing.
