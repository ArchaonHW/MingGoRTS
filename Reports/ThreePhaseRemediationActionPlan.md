# MingGoRTS 專案三階段修復行動計劃

**制定日期:** 2026-03-24  
**當前風險:** 🟡 Medium (48.88分)  
**目標風險:** 🟢 Low (<20分)  

---

## 📋 執行摘要

| 階段 | 時間線 | 主要任務 | 目標成果 |
|------|--------|----------|----------|
| **立即** | 0-1天 | 審查 High 風險、消除誤報 | Critical≤5, High≤10 |
| **短期** | 1-2週 | 修復 Medium 記憶體安全 | 消除 90% Medium 弱點 |
| **長期** | 1-3月 | CI/CD整合、安全規範 | 持續風險<20分 |

---

## 🔴 第一階段：立即修復 (0-1天)

### 目標
- 審查所有 High 風險弱點
- 消除掃描器誤報
- 確認無敏感憑證洩露

### 任務清單

#### 1.1 審查 64 個 High 風險

**分類統計:**
- Unsafe C Functions: ~58個 (主要在 llama.cpp 第三方庫)
- Debug Log Exposure: ~4個 (MingGoRTS 源碼)
- Hardcoded Secret: 1個 (測試文件)

**審查結果:**

| 類別 | 數量 | 真實風險 | 建議 |
|------|------|----------|------|
| llama.cpp Unsafe C | 58 | ⚪ 低 (第三方庫) | 暫不處理，關注官方更新 |
| Debug Log Exposure | 4 | ⚪ 極低 (誤報) | 調整掃描規則 |
| Hardcoded Secret | 1 | 🟡 中 (測試文件) | 確認是否包含真實憑證 |

#### 1.2 調整掃描規則消除誤報

**已完成的規則優化:**

```powershell
# 1. Command Injection - 已優化
# 修復前: 匹配 InitializeSystem() 等函數名
# 修復後: 僅匹配 system("cmd") 真實調用
Pattern = '(?<!\w)(system|popen)\s*\(|ShellExecute[A-Z]?\s*\(|CreateProcess[A-Z]?\s*\('

# 2. Debug Log Exposure - 需要調整
# 修復前: 匹配所有 UE_LOG LogTemp
# 修復後: 僅匹配包含敏感資訊的日誌
Pattern = 'UE_LOG.*\b(password|secret|token|api_key|credential|auth)\s*=\s*["\'\']?\w+'

# 3. Unsafe C Functions - 已優化
# 排除 UE 安全函數如 FMemory::Memcpy
Pattern = '(?<!FMemory::)(?<![A-Za-z_])(strcpy|strcat|sprintf|gets|scanf)\s*\('
```

#### 1.3 確認敏感憑證安全

**檢查文件:**
- [ ] `Source/ThirdParty/llama/llama.cpp/tests/test-chat-template.cpp:286`
  - Hardcoded Secret 報告位置
  - 需確認是否為真實 API Key 或僅測試數據

**驗收標準:**
- 無真實密碼、API Key 硬編碼在源碼中
- 所有生產憑證使用環境變數或安全存儲

#### 1.4 重新掃描驗證

**執行命令:**
```powershell
.\Tools\security\ProjectVulnerabilityScanner.ps1 -SeverityLevel Low
```

**預期結果:**
- Critical: ≤5
- High: ≤10
- RiskScore: <25 (Low)

---

## 🟡 第二階段：短期修復 (1-2週)

### 目標
- 修復 2,938 個 Medium 記憶體安全弱點
- 建立自動修復流程
- 實施輸入驗證框架

### 2.1 Medium 弱點分析

**分類統計:**
| 類別 | 數量 | 主要問題 | 修復策略 |
|------|------|----------|----------|
| Memory Safety | 2,766 | 不安全函數模式 | 批量替換 |
| Concurrency | 173 | 並行訪問風險 | 添加同步機制 |
| Information Disclosure | 28 | 資訊洩露模式 | 日誌審查 |
| Cryptography | 24 | 弱加密算法 | 算法升級 |
| Injection | 13 | 注入風險 | 輸入驗證 |
| Error Handling | 7 | 錯誤處理 | 完善異常處理 |
| Secrets | 4 | 敏感資訊 | 安全存儲 |

### 2.2 創建自動修復腳本

**文件名:** `Tools/security/AutoFixMemorySafety.ps1`

```powershell
#!/usr/bin/env pwsh

param(
    [string]$TargetDir = "$PSScriptRoot/../../Source/MingGoRTS",
    [switch]$DryRun
)

$FixStats = @{
    FilesProcessed = 0
    Replacements = @{
        strcpy = 0
        strcat = 0
        sprintf = 0
        uncheckedCast = 0
    }
}

function Repair-File {
    param([string]$FilePath)
    
    $content = Get-Content $FilePath -Raw
    $originalContent = $content
    
    # 1. 修復 strcpy -> FCString::Strcpy
    $content = $content -replace '(?<!FCString::)\bstrcpy\s*\(([^,]+),\s*([^)]+)\)', 
        'FCString::Strcpy($1, ARRAY_COUNT($1), $2)'
    
    # 2. 修復 strcat -> FCString::Strcat  
    $content = $content -replace '(?<!FCString::)\bstrcat\s*\(([^,]+),\s*([^)]+)\)',
        'FCString::Strcat($1, ARRAY_COUNT($1), $2)'
    
    # 3. 修復 sprintf -> FString::Printf
    $content = $content -replace '\bsprintf\s*\(([^,]+),\s*([^)]+)\)',
        '$1 = *FString::Printf($2)'
    
    # 4. 檢測未檢查的 Cast
    $castMatches = [regex]::Matches($content, 'Cast<([^>]+)>\s*\([^)]+\)\s*;')
    foreach ($match in $castMatches) {
        Write-Warning "發現未檢查的 Cast: $($match.Value) in $FilePath"
    }
    
    if (-not $DryRun -and $content -ne $originalContent) {
        Set-Content $FilePath $content -NoNewline
        Write-Host "已修復: $FilePath" -ForegroundColor Green
    }
    
    $FixStats.FilesProcessed++
}

# 處理所有源碼文件
Get-ChildItem $TargetDir -Recurse -Filter "*.cpp" | ForEach-Object {
    Repair-File $_.FullName
}

# 輸出統計
$FixStats | ConvertTo-Json
```

### 2.3 輸入驗證框架實施

**創建文件:** `Source/MingGoRTS/Public/Security/InputValidator.h`

```cpp
#pragma once

#include "CoreMinimal.h"

class FInputValidator
{
public:
    // 文件路徑驗證
    static bool ValidateFilePath(const FString& Path, FString& OutError);
    
    // 用戶輸入清理
    static FString SanitizeUserInput(const FString& Input);
    
    // 網路數據包驗證
    static bool ValidateNetworkPacket(const TArray<uint8>& Data, FString& OutError);
    
    // 檢查路徑遍歷攻擊
    static bool ContainsPathTraversal(const FString& Path);
    
    // SQL注入檢測
    static bool ContainsSQLInjection(const FString& Query);
};
```

### 2.4 修復優先級矩陣

**第一週 (高影響文件):**
| 優先級 | 文件 | 弱點數 | 修復工時 |
|--------|------|--------|----------|
| P0 | `MingSixStrategies.cpp` | 26 | 4h |
| P0 | `MingRTSIntegrationTestExecutor.cpp` | 26 | 4h |
| P0 | `MingNegativeStrategies.cpp` | 21 | 3h |
| P1 | `MingRTSArtEditor.cpp` | 15 | 2h |
| P1 | `MingRTSIntelligentScriptIntegration.cpp` | 20 | 3h |

**第二週 (批量修復):**
- 使用 AutoFix 腳本處理剩餘 2,600+ 個弱點
- 人工審查自動修復結果
- 修復異常處理邊界情況

### 2.5 驗收標準

- [ ] Medium 弱點數 ≤ 300 (消除 90%)
- [ ] 所有新代碼通過自動掃描
- [ ] 輸入驗證框架部署到核心模塊

---

## 🟢 第三階段：長期改進 (1-3月)

### 目標
- CI/CD 安全掃描整合
- 建立安全編碼規範
- 實施持續監控

### 3.1 CI/CD 安全整合

**GitHub Actions 工作流:**

```yaml
# .github/workflows/security-scan.yml
name: Security Scan

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]
  schedule:
    - cron: '0 2 * * 1'  # 每週一凌晨 2 點

jobs:
  vulnerability-scan:
    runs-on: windows-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v3
        
      - name: Run Vulnerability Scanner
        shell: pwsh
        run: |
          $result = & .\Tools\security\ProjectVulnerabilityScanner.ps1
          $result | ConvertTo-Json | Out-File scan-result.json
          
      - name: Check Risk Score
        shell: pwsh
        run: |
          $result = Get-Content scan-result.json | ConvertFrom-Json
          if ($result.RiskScore -gt 20) {
            Write-Error "風險評分過高: $($result.RiskScore)。要求 < 20"
            exit 1
          }
          if ($result.Stats.CriticalCount -gt 0) {
            Write-Error "發現 $($result.Stats.CriticalCount) 個 Critical 弱點"
            exit 1
          }
          
      - name: Upload Report
        uses: actions/upload-artifact@v3
        with:
          name: security-report
          path: Reports/VulnerabilityScan_Report.md
```

### 3.2 安全編碼規範

**創建文件:** `docs/02_Development_Guides/SecurityCodingGuidelines.md`

```markdown
# MingGoRTS 安全編碼規範

## 1. 禁止使用的危險函數

| 危險函數 | 風險 | 替代方案 |
|----------|------|----------|
| strcpy | 緩衝區溢出 | FCString::Strcpy |
| strcat | 緩衝區溢出 | FCString::Strcat |
| sprintf | 格式化字符串溢出 | FString::Printf |
| gets | 無限制輸入 | 使用 FString 流 |
| system() | 命令注入 | FPlatformProcess |
| malloc/free | 記憶體洩漏 | TArray/TSharedPtr |

## 2. 強制執行的模式

### 2.1 所有 Cast 必須檢查
```cpp
// 正確
if (auto* Casted = Cast<UMyClass>(Obj)) {
    Casted->Method();
}

// 錯誤
auto* Casted = Cast<UMyClass>(Obj);
Casted->Method(); // 可能崩潰
```

### 2.2 用戶輸入必須驗證
```cpp
void ProcessInput(const FString& Input)
{
    if (!FInputValidator::SanitizeUserInput(Input)) {
        UE_LOG(LogTemp, Warning, TEXT("無效輸入"));
        return;
    }
    // 繼續處理
}
```

### 2.3 敏感資訊禁止日誌
```cpp
// 錯誤
UE_LOG(LogTemp, Log, TEXT("Token: %s"), *Token);

// 正確
UE_LOG(LogTemp, Log, TEXT("Token: [REDACTED]"));
```

## 3. 安全審查清單

- [ ] 所有新代碼通過弱點掃描
- [ ] 無 Critical/High 級別弱點
- [ ] 所有外部輸入經過驗證
- [ ] 無敏感資訊洩露
- [ ] 無不安全函數使用
```

### 3.3 持續監控儀表板

**創建文件:** `Tools/security/SecurityDashboard.ps1`

```powershell
#!/usr/bin/env pwsh

param(
    [string]$HistoryDir = "$PSScriptRoot/../../Reports/SecurityHistory"
)

# 確保歷史目錄存在
if (-not (Test-Path $HistoryDir)) {
    New-Item -ItemType Directory -Path $HistoryDir -Force | Out-Null
}

# 執行掃描
$scanResult = & "$PSScriptRoot/ProjectVulnerabilityScanner.ps1" -SeverityLevel Low

# 保存歷史記錄
$historyFile = Join-Path $HistoryDir "$(Get-Date -Format 'yyyyMMdd').json"
$scanResult | ConvertTo-Json -Depth 3 | Out-File $historyFile

# 生成趨勢報告
$history = Get-ChildItem $HistoryDir -Filter "*.json" | 
    Sort-Object Name -Descending | 
    Select-Object -First 10 |
    ForEach-Object { 
        Get-Content $_.FullName | ConvertFrom-Json 
    }

# 輸出儀表板
Write-Host "`n=== 安全監控儀表板 ===" -ForegroundColor Cyan
Write-Host "當前風險評分: $($scanResult.RiskScore) ($($scanResult.RiskLevel))"
Write-Host "Critical 弱點: $($scanResult.Stats.CriticalCount)"
Write-Host "High 弱點: $($scanResult.Stats.HighCount)"
Write-Host "Medium 弱點: $($scanResult.Stats.MediumCount)"

if ($history.Count -ge 2) {
    $prev = $history[1]
    $trend = $scanResult.RiskScore - $prev.RiskScore
    $trendText = if ($trend -lt 0) { "↓ 改善" } elseif ($trend -gt 0) { "↑ 惡化" } else { "→ 持平" }
    Write-Host "趨勢: $trendText ($([math]::Abs($trend)))" -ForegroundColor $(if ($trend -le 0) { "Green" } else { "Red" })
}
```

### 3.4 安全培訓計劃

**第一月:**
- [ ] 全體開發者安全編碼規範培訓
- [ ] 弱點掃描工具使用教學
- [ ] 典型案例分析 (OWASP Top 10)

**第二月:**
- [ ] 安全代碼審查實踐
- [ ] 漏洞賞金計劃啟動
- [ ] 安全測試用例編寫

**第三月:**
- [ ] 第三方安全審查
- [ ] 滲透測試
- [ ] 持續改進機制建立

### 3.5 長期驗收標準

| 指標 | 目標值 | 當前值 | 差距 |
|------|--------|--------|------|
| 風險評分 | < 20 | 48.88 | -28.88 |
| Critical 弱點 | 0 | 13 | -13 |
| High 弱點 | < 5 | 64 | -59 |
| 掃描覆蓋率 | 100% | 100% | 0 |
| 新代碼通過率 | 100% | - | - |

---

## 📅 執行時間表

```
3月24日 (Day 0)
├── 立即階段開始
│   ├── 審查 64 個 High 風險 [4h]
│   ├── 調整掃描規則 [2h]
│   └── 重新掃描驗證 [1h]
│
3月25日-31日 (Week 1)
├── 短期階段 - 第一週
│   ├── 創建自動修復腳本 [8h]
│   ├── 修復 Top 10 高影響文件 [20h]
│   └── 輸入驗證框架實施 [12h]
│
4月1日-7日 (Week 2)
├── 短期階段 - 第二週
│   ├── 批量自動修復 [16h]
│   ├── 人工審查結果 [12h]
│   └── 驗收測試 [8h]
│
4月-6月 (Month 1-3)
├── 長期階段
│   ├── CI/CD 整合 [Week 1-2]
│   ├── 安全規範制定 [Week 3-4]
│   ├── 持續監控部署 [Week 5-8]
│   └── 安全培訓執行 [Week 9-12]
```

---

## ✅ 檢查點

### 立即階段檢查點 (3/24)
- [ ] High 風險審查完成
- [ ] 掃描規則優化完成
- [ ] Critical ≤ 5
- [ ] High ≤ 10

### 短期階段檢查點 (4/7)
- [ ] 自動修復腳本完成
- [ ] Top 10 文件修復完成
- [ ] Medium 減少 90%
- [ ] 輸入驗證框架部署

### 長期階段檢查點 (6/30)
- [ ] CI/CD 整合完成
- [ ] 安全規範發布
- [ ] 持續監控運行
- [ ] 風險評分 < 20

---

## 📊 資源需求

| 資源 | 立即 | 短期 | 長期 |
|------|------|------|------|
| 人力 (小時) | 8h | 60h | 120h |
| 工具授權 | - | - | PVS-Studio |
| 外部審查 | - | - | 1 次滲透測試 |
| 培訓成本 | - | - | 2 天工作坊 |

---

**計劃制定:** DevOps Team  
**審批:** Tech Lead  
**下次審查:** 2026-03-25

---

*計劃版本: v1.0 | 更新日期: 2026-03-24*
