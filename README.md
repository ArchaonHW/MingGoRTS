# MingGoRTS

## 民國傭兵 - RTS 即時戰略遊戲

一款使用 Unreal Engine 5 開發的即時戰略遊戲，背景設定在民國時期，融合 RTS 與硬派動作元素。

## 項目亮點

- **歷史題材**：體驗民國時期的傭兵傳奇。
- **RTS 核心**：大規模單位控制、資源管理、基地建設。
- **AI 生成資產**：使用 Stable Diffusion 與 AIVA 輔助生成美術與音樂。
- **跨平台**：支援 Windows、Android 及 iOS。

## 技術堆疊

- **引擎**：Unreal Engine 5.4+
- **開發語言**：C++ + Blueprint
- **AI 工具**：Stable Diffusion + ControlNet (美術)，AIVA API (音樂)

## 快速上手

### 1. 開啟專案

點擊 `MingGoRTS.uproject` 檔案，使用 Unreal Engine 5.7 開啟。

### 2. 生成 C++ 專案檔

首次開啟時，UE5 會自動為您生成 Visual Studio 專案檔。如果需要手動生成，請執行：

```powershell
# 請替換成您的引擎路徑
& "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="C:\HW\MingGoRTS\MingGoRTS.uproject" -game -rocket -progress
```

### 3. 編譯

在 Visual Studio 或 Rider 中開啟解決方案 (`.sln`) 並編譯，或在專案目錄下執行：

```powershell
# 請替換成您的引擎路徑
& "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" MingGoRTSEditor Win64 Development -Project="C:\HW\MingGoRTS\MingGoRTS.uproject" -waitmutex
```

### 4. 運行

直接在 UE5 編輯器中點擊 "Play" 按鈕即可運行遊戲。

## 控制說明

| 操作 | 按鍵/滑鼠 |
|:---|:---|
| 框選單位 | 滑鼠左鍵拖拽 |
| 移動單位 | 選擇單位後，右鍵點擊地面 |
| 攻擊單位 | 選擇單位後，右鍵點擊敵人 |
| 全選單位 | Ctrl + A |
| 建造建築 | (待定) |
| 暫停/繼續 | 空格鍵 |

## 開發計畫與里程碑

### ✅ 已完成里程碑

| 里程碑 | 狀態 | 說明 |
|:---|:---|:---|
| **Alpha 版本** | ✅ 完成 | Sprint 1-3: RTS核心迴圈、資源建築、AI戰役 |
| **Beta 版本** | ✅ 完成 | Story 3.5-3.7: 音頻、UI、保存載入系統 |
| **Release 版本** | ✅ 完成 | 所有Epic完成，正式發布 v1.0.0 |

### 核心系統完成狀態

| 系統 | 狀態 | 關鍵檔案 |
|:---|:---|:---|
| **RTS 核心** | ✅ 完成 | `MingRTSUnitManager`, `MingRTSCombatSystem` |
| **資源建築** | ✅ 完成 | `MingRTSResourceManager`, `MingRTSBuildingSystem` |
| **AI 戰役** | ✅ 完成 | `MingRTSAIEnhancedSystem`, `MingRTSCampaignSystem` |
| **角色關係** | ✅ 完成 | `MingRelationshipManager`, `MingReputationManager` |
| **音頻系統** | ✅ 完成 | `MingAudioSystem`, `MingRepublicEraAudioThemes` |
| **UI 系統** | ✅ 完成 | `MingPersonalUIManager`, `MingDialoguePanel` |
| **保存載入** | ✅ 完成 | `MingRTSSaveLoadEnhancedSystem` |
| **四層整合** | ✅ 完成 | `MingFourLayerManager` |
| **網絡多人** | ✅ 完成 | `MingRTSNetworkEnhancedSystem` |
| **性能優化** | ✅ 完成 | `MingRTSPerformanceEnhancedSystem` (Epic 8.1) |
| **本地化** | ✅ 完成 | `MingRTSLocalizationEnhancedSystem` (Epic 7.1) |
| **Release 測試** | ✅ 完成 | `MingReleaseIntegrationTest` (38測試) |

### Beta 版本特性 (Story 3.5-3.7)

#### Story 3.5: 音頻和音效系統
- 10個民國時期音頻主題 (戰鬥、探索、外交等)
- 10個地區音效 (北平、上海、南京等)
- 音頻-關係系統互動
- MetaSounds 程序化音效

#### Story 3.6: UI 和界面系統
- 完整個人系統 UI (關係、聲望、對話、任務)
- 主儀表板 (MingMainDashboard)
- 打字機效果對話系統
- 輔助功能支持 (字體大小、高對比度等)

#### Story 3.7: 保存和載入系統
- 11種保存數據類型
- 10種保存格式 (二進制、JSON、壓縮、加密等)
- 自動保存和備份恢復
- 雲端保存支持

### Release 版本特性 (v1.0.0)

#### 10 個 Epic 系統全部完成
- **Epic 1.1**: RTS 戰鬥系統 - 200+ 單位, 6 AI 類型, 7 傷害類型
- **Epic 2.1**: 經濟系統 - 10 資源, 15 建築, 貿易系統
- **Epic 3.1**: AI 戰役系統 - 12 行為模式, 8 戰役類型
- **Epic 4.1**: UI 音頻系統 - 21 UI 類型, 16 音頻情感
- **Epic 5.1**: 網絡系統 - 10 網絡類型, 11 遊戲模式
- **Epic 6.1**: 保存系統 - 11 數據類型, 雲端支持
- **Epic 7.1**: 本地化 - 20 語言, 17 地區
- **Epic 8.1**: 性能調試 - 11 監控類別
- **Epic 9**: AI 內容生成 - 12 內容類型
- **Epic 10**: 四層整合 - 策略/戰術/個人/建築

### 📁 文檔指南

- **Release 發布準備**: `docs/RELEASE_INTEGRATION_GUIDE.md` (完整發布指南)
- **Beta 集成指南**: `docs/BETA_INTEGRATION_GUIDE.md`
- **音頻系統**: `docs/STORY_3_5_AUDIO_INTEGRATION.md`
- **UI 系統**: `docs/STORY_3_6_UI_INTEGRATION.md`
- **保存載入**: `docs/STORY_3_7_SAVE_LOAD_INTEGRATION.md`
- **Release 測試套件**: `Plugins/MingCore/Source/MingCore/Tests/MingReleaseIntegrationTest`

### 舊版開發計畫

- **Sprint 1**：RTS 核心迴圈 (單位選擇、移動、資源)。✅ 已完成
- **Sprint 2**：資源與建築系統。✅ 已完成
- **Sprint 3**：AI 與戰役基礎。✅ 已完成
- **Sprint 4+**：UI、音效、關卡設計、傭兵模式。✅ 已完成

## 貢獻人員
- 開發者：Archaon (V仔)
- 助理：馬鈴薯 (🥔)

## 許可

MIT License

---

*"民國亂世，傭兵為王。"*
