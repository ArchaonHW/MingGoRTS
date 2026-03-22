---
story_id: "1-1"
epic_id: "epic-1"
title: "歷史決策系統"
status: "done"
created_at: "2026-03-22T08:18:00Z"
updated_at: "2026-03-22T08:25:00Z"
completed_at: "2026-03-22T08:25:00Z"
---

# Story 1.1: 歷史決策系統

## User Story

As a 玩家,
I want 在遊戲中做出歷史性決策,
So that 影響遊戲進程並體驗不同的歷史走向。

## Acceptance Criteria

### AC 1.1.1: 決策觸發 ✅
**Given** 遊戲進行中
**When** 達到特定條件
**Then** 觸發歷史決策事件
**And** 顯示決策選項

### AC 1.1.2: 決策選擇 ✅
**Given** 決策事件已觸發
**When** 玩家選擇選項
**Then** 記錄決策結果
**And** 觸發對應後果

### AC 1.1.3: 決策UI ✅
**Given** 決策事件已觸發
**Then** 顯示決策對話框
**And** 包含背景描述、選項、後果預測

### AC 1.1.4: 決策歷史 ✅
**Given** 玩家已做出決策
**Then** 記錄到決策歷史
**And** 支持查看過往決策

## Technical Context

### 功能範圍
1. **決策數據結構**: 定義決策ID、描述、選項、後果
2. **決策觸發系統**: 條件檢查、時間觸發、事件觸發
3. **決策UI界面**: 對話框、選項按鈕、動畫效果
4. **決策後果系統**: 立即效果、長期影響

### 依賴
- Story 6.1: UE5模組化插件架構 (MingStrategic 插件)
- Story 6.2: 事件驅動通訊 (決策事件發布)
- Story 6.3: 跨平台適配 (UI適配)

### 文件結構
```
MingStrategic/
├── Public/
│   ├── Decisions/
│   │   ├── MingDecision.h
│   │   ├── MingDecisionOption.h
│   │   └── MingDecisionEffect.h
│   ├── Managers/
│   │   └── MingDecisionManager.h
│   └── UI/
│       └── MingDecisionWidget.h
└── Private/
    ├── Decisions/
    │   ├── MingDecision.cpp
    │   ├── MingDecisionOption.cpp
    │   └── MingDecisionEffect.cpp
    ├── Managers/
    │   └── MingDecisionManager.cpp
    └── UI/
        └── MingDecisionWidget.cpp
```

## Implementation Tasks

### Task 1.1.1: 決策數據結構 ✅
- [x] 創建 `FMingDecision` 結構
- [x] 定義決策ID、標題、描述
- [x] 定義決策選項列表
- [x] 定義觸發條件
- [x] 創建 `FMingDecisionOption` 結構
- [x] 定義選項文本、後果描述
- [x] 創建 `FMingDecisionEffect` 結構
- [x] 定義效果類型和數值

### Task 1.1.2: 決策管理器 ✅
- [x] 創建 `UMingDecisionManager`
- [x] 實現決策註冊系統
- [x] 實現決策觸發檢查
- [x] 實現決策執行邏輯
- [x] 集成 MingCore 事件系統

### Task 1.1.3: 決策UI系統 ✅
- [x] 創建 `UMingDecisionWidget`
- [x] 實現決策對話框布局
- [x] 實現選項按鈕生成
- [x] 實現動畫效果 (淡入/彈出)
- [x] 支持平台適配 (PC/Mobile)

### Task 1.1.4: 決策歷史記錄 ✅
- [x] 創建決策歷史數據結構
- [x] 實現決策記錄保存
- [x] 實現決策歷史查看界面
- [x] 支持序列化/反序列化

### Task 1.1.5: 示例決策內容 ⏳
- [ ] 創建民國時期歷史決策示例
- [ ] 北伐路線選擇
- [ ] 政治聯盟決策
- [ ] 軍事策略決策

## 實作摘要

### 完成的系統

1. **決策數據結構** (`MingStrategic/Decisions/`)
   - `FMingDecision` - 決策主結構 (ID/標題/描述/選項/觸發條件)
   - `FMingDecisionOption` - 決策選項 (文本/效果/後續決策)
   - `FMingDecisionEffect` - 決策效果 (類型/目標/數值)
   - 支持多種觸發類型 (手動/時間/事件/條件/位置)

2. **決策管理器** (`MingStrategic/Managers/`)
   - `UMingDecisionManager` - 決策核心管理
   - 決策註冊與觸發
   - 條件檢查與時間觸發
   - 決策執行與效果應用
   - 決策歷史記錄
   - 事件委託 (OnDecisionTriggered/OnDecisionMade)

3. **決策UI系統** (`MingStrategic/UI/`)
   - `UMingDecisionWidget` - 決策對話框Widget
   - 標題/描述/背景圖片顯示
   - 動態選項按鈪態生成
   - 平台適配 (移動端觸控優化)
   - 動畫支持 (Show/Hide)

### 文件結構

```
MingStrategic/
├── Public/
│   ├── Decisions/
│   │   ├── MingDecision.h
│   │   ├── MingDecisionOption.h
│   │   └── MingDecisionEffect.h
│   ├── Managers/
│   │   └── MingDecisionManager.h
│   └── UI/
│       └── MingDecisionWidget.h
└── Private/
    ├── Decisions/
    │   ├── MingDecision.cpp
    │   ├── MingDecisionOption.cpp
    │   └── MingDecisionEffect.cpp
    ├── Managers/
    │   └── MingDecisionManager.cpp
    └── UI/
        └── MingDecisionWidget.cpp
```

### 後續工作

- 創建民國時期歷史決策示例內容
- 北伐路線選擇、政治聯盟、軍事策略等具體決策
- 決策效果與資源/派系/角色系統的集成

---

- GDD Section: 歷史決策體驗基礎
- PRD: FR1 (歷史決策系統)
- Story 6.1: UE5模組化插件架構
- Story 6.2: 事件驅動通訊系統
- Story 6.3: 跨平台適配系統
