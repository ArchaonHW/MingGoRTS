# MingGoRTS 自動化編譯除錯清單

> 生成時間: 2026-03-23  
> 適用版本: UE 5.4+  
> 專案: MingGoRTS

---

## 快速診斷流程圖

```
編譯失敗?
├── 是 → 查看錯誤類型
│   ├── 語法錯誤 → 執行步驟 1-5
│   ├── 連結錯誤 → 執行步驟 6-8
│   ├── 型別未定義 → 執行步驟 9-12
│   └── 其他錯誤 → 執行步驟 13-15
└── 否 → 編譯成功 ✓
```

---

## 第一階段: 自動化快速修復

### 步驟 1: 執行自動語法修復
```powershell
# 執行 PowerShell 修復腳本
.\FIX_SYNTAX_ERRORS.ps1
```

**修復內容包含:**
- 未閉合的字元常數 (single quotes)
- UENUM 語法錯誤
- 遺失的分號
- 錯誤的 API 宏位置

### 步驟 2: 執行批次修復
```batch
:: 批次修復常見問題
BATCH_FIX.cmd
```

### 步驟 3: 清理並重建專案檔案
```batch
:: 清理中間檔案
rmdir /s /q .\Intermediate
rmdir /s /q .\Binaries
rmdir /s /q .\Build

:: 重新產生專案檔案
RUN_UBT.cmd
```

### 步驟 4: 驗證基本編譯
```batch
:: 快速編譯驗證
COMPILE_VERIFY.cmd
```

---

## 第二階段: 常見錯誤類型處理

### 🔴 錯誤類型 A: Delegate 宏宣告錯誤

**錯誤特徵:**
```
DECLARE_DYNAMIC_MULTICAST_DELEGATE 參數數量不匹配
```

**自動修復步驟:**
1. 檢查 Delegate 宣告參數數量
2. 修正為正確格式:
   - `DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam` (1參數)
   - `DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams` (2參數)
   - `DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams` (3參數)

**手動修復檔案:**
- `MingRTSCulturalAdaptationSystem.h`
- `MingSaveGameManager.h`

---

### 🔴 錯誤類型 B: UPROPERTY TMap<TArray> 不支持

**錯誤特徵:**
```
TMap<TArray<...>> 不被 UPROPERTY 支持
```

**自動修復步驟:**
1. 移除 `TMap<FString, TArray<...>>` 的 UPROPERTY 標記
2. 改用普通成員變數或自定義結構包裝

**修復範例:**
```cpp
// 錯誤:
UPROPERTY()
TMap<FString, TArray<FCulturalVariant>> ContentVariants;

// 正確:
TMap<FString, TArray<FCulturalVariant>> ContentVariants;
// 或自定義結構包裝
```

---

### 🔴 錯誤類型 C: 未定義型別錯誤

**錯誤特徵:**
```
EMingResourceType/ELanguageCode 未定義
AMingGoRTSUnit 未定義
```

**自動修復步驟:**
1. 添加正確的 include 路徑
2. 檢查型別命名 (EMingLayer → EMingGameLayer)
3. 重命名重複定義的結構

**需要檢查的檔案:**
- `MingResourceSystem.h` - 添加 `EMingResourceType` include
- `MingRTSLocalizationTypes.h` - 重命名 `EMingLanguage` → `EMingRTSLanguage`
- `MingStrategicFourLayerTypes.h` - 修復 `EMingLayer` → `EMingGameLayer`

---

### 🔴 錯誤類型 D: EOF 和語法錯誤

**錯誤特徵:**
```
錯誤: 預期外的檔案結尾
錯誤: 遺失的類別閉合符號
```

**自動修復步驟:**
執行 EOF 修復腳本:
```powershell
# 檢查並修復類別閉合
Get-ChildItem -Path "Source" -Recurse -Filter "*.h" | ForEach-Object {
    $content = Get-Content $_.FullName -Raw
    $openBraces = ($content -match "{").Count
    $closeBraces = ($content -match "}").Count
    
    if ($openBraces -gt $closeBraces) {
        Add-Content $_.FullName "`n};"
        Write-Host "Fixed EOF: $($_.Name)"
    }
}
```

**需要修復的檔案 (根據歷史記錄):**
- `MingFormationManager.h`
- `MingFogOfWarManager.h`
- `MingTacticalLayer.h`
- `MingPerformanceManager.h`
- `MingTriggerManager.h`

---

### 🔴 錯誤類型 E: UENUM/USTRUCT 範圍錯誤

**錯誤特徵:**
```
UENUM 必須在全局範圍
USTRUCT 必須在全局範圍
```

**自動修復步驟:**
1. 將 UENUM 和 USTRUCT 移動到類別外部 (全局範圍)
2. 添加 `#include "Templates/Function.h"`

**修復範例:**
```cpp
// 錯誤:
class UMyClass {
    UENUM()
    enum class EMyEnum { ... };
};

// 正確:
UENUM()
enum class EMyEnum { ... };

class UMyClass {
    // 使用枚舉
};
```

---

## 第三階段: 進階除錯

### 步驟 5: 檢查編譯日誌

**查看詳細日誌:**
```powershell
# 查看最新編譯日誌
Get-Content "C:\Users\$env:USERNAME\AppData\Local\UnrealBuildTool\Log_GPF.txt" -Tail 100

# 查看專案編譯日誌
Get-Content ".\BuildLog.txt" -Tail 50
```

**關鍵錯誤字串搜尋:**
```powershell
# 搜尋常見錯誤類型
Select-String -Path "*.cpp","*.h" -Pattern "error C\d+" | Select-Object -First 20
```

### 步驟 6: 驗證 Include 路徑

**檢查缺失的 includes:**
```powershell
# 掃描未定義的型別引用
$files = Get-ChildItem -Path "Source" -Recurse -Filter "*.h"
foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw
    if ($content -match "EMingResourceType|ELanguageCode|AMingGoRTSUnit" -and 
        -not ($content -match "#include.*MingResourceSystem|#include.*MingUnit")) {
        Write-Host "可能需要添加 include: $($file.Name)"
    }
}
```

### 步驟 7: 檢查重複定義

**掃描重複的型別定義:**
```powershell
# 掃描重複的結構定義
$enums = @{}
Get-ChildItem -Path "Source" -Recurse -Filter "*.h" | ForEach-Object {
    $content = Get-Content $_.FullName
    $matches = [regex]::Matches($content, "(UENUM\(\)|enum\s+class)\s+(\w+)")
    foreach ($match in $matches) {
        $enumName = $match.Groups[2].Value
        if ($enums.ContainsKey($enumName)) {
            Write-Host "重複定義: $enumName in $($_.Name) 和 $($enums[$enumName])"
        } else {
            $enums[$enumName] = $_.Name
        }
    }
}
```

**常見重複定義:**
- `FCulturalPreferences` → 重命名為 `FRTSCulturalPreferences`
- `ELanguageCode` → 重命名為 `ERTSLanguageCode`
- `EDisabilityType` → 重命名為 `EPersonalDisabilityType`

---

## 第四階段: 完整編譯測試

### 步驟 8: 執行完整編譯

```batch
:: 最佳化編譯
BUILD_OPTIMIZED.cmd

:: 或偵錯編譯
DEBUG_BUILD.cmd
```

### 步驟 9: 執行整合測試

```powershell
# 執行整合測試
.\Tools\build\IntegrationTestRunner.ps1
```

---

## 第五階段: 特定系統編譯

### 文化系統編譯
```batch
COMPILE_CULTURAL_SYSTEM.cmd
```

### 個人系統編譯
```batch
:: 編譯 MingPersonal 插件
RUN_UBT.cmd MingPersonal Win64 Development
```

---

## 除錯工具腳本參考

### 快速修復指令碼
| 腳本 | 用途 |
|------|------|
| `FIX_SYNTAX_ERRORS.ps1` | 修復語法錯誤 |
| `BATCH_FIX.cmd` | 批次修復常見問題 |
| `QUICK_FIX.cmd` | 快速修復 |
| `QUICK_FIX_2.cmd` | 快速修復 (替代) |
| `fix_enum.py` | 修復枚舉錯誤 |
| `fix_building.py` | 修復建築系統錯誤 |

### 編譯指令碼
| 腳本 | 用途 |
|------|------|
| `BUILD_OPTIMIZED.cmd` | 最佳化編譯 |
| `DEBUG_BUILD.cmd` | 偵錯編譯 |
| `COMPILE_VERIFY.cmd` | 編譯驗證 |
| `VERIFY_COMPILE.cmd` | 驗證編譯 |
| `RUN_UBT.cmd` | 執行 UBT |

---

## 常見錯誤速查表

| 錯誤訊息 | 原因 | 解決方案 |
|---------|------|----------|
| `DECLARE_DYNAMIC_MULTICAST_DELEGATE` | 參數數量不匹配 | 使用正確的 Delegate 宏 |
| `TMap<TArray<...>>` | UPROPERTY 不支持 | 移除 UPROPERTY 或包裝 |
| `error C2065: 未定義識別項` | 缺少 include | 添加正確的 include 路徑 |
| `error C2011: 型別重複定義` | 重複定義 | 重命名或移除重複 |
| `error C1004: 預期外的檔案結尾` | 遺失閉合符號 | 添加 `};` |
| `UENUM must be global` | 範圍錯誤 | 移動到全局範圍 |
| `error LNK2019` | 連結錯誤 | 檢查函式實作 |
| `error C2664` | 參數類型不匹配 | 檢查函式簽名 |

---

## 預防措施

### 編譯前檢查清單
- [ ] 所有新檔案已加入 .Build.cs
- [ ] 所有 includes 路徑正確
- [ ] 沒有重複的型別定義
- [ ] 所有枚舉都在全局範圍
- [ ] 所有類別都有正確的閉合
- [ ] UPROPERTY 不使用不支持的型別

### 編譯後驗證
- [ ] 無編譯錯誤
- [ ] 無連結錯誤
- [ ] 無警告 (或警告已審查)
- [ ] 測試通過

---

## 相關文件

- `docs/COMPILATION_FIXES_SUMMARY.md` - 編譯修復總結
- `FIX_SYNTAX_ERRORS.ps1` - 語法修復腳本
- `BuildLog.txt` - 編譯日誌

---

*最後更新: 2026-03-23*  
*版本: 1.0*
