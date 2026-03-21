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

點擊 `MingGoRTS.uproject` 檔案，使用 Unreal Engine 5.4+ 開啟。

### 2. 生成 C++ 專案檔

首次開啟時，UE5 會自動為您生成 Visual Studio 專案檔。如果需要手動生成，請執行：

```powershell
# 請替換成您的引擎路徑
& "C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="C:\HW\MingGoRTS\MingGoRTS.uproject" -game -rocket -progress
```

### 3. 編譯

在 Visual Studio 或 Rider 中開啟解決方案 (`.sln`) 並編譯，或在專案目錄下執行：

```powershell
# 請替換成您的引擎路徑
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" MingGoRTSEditor Win64 Development -Project="C:\HW\MingGoRTS\MingGoRTS.uproject" -waitmutex
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

## 開發計畫

- **Sprint 1**：RTS 核心迴圈 (單位選擇、移動、資源)。
- **Sprint 2**：資源與建築系統。
- **Sprint 3**：AI 與戰役基礎。
- **Sprint 4+**：UI、音效、關卡設計、傭兵模式。

## 貢獻人員
- 開發者：Archaon (V仔)
- 助理：馬鈴薯 (🥔)

## 許可

MIT License

---

*"民國亂世，傭兵為王。"*
