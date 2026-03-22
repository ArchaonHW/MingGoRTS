---
story_id: "1-2"
epic_id: "epic-1"
title: "動態事件觸發系統"
status: "done"
created_at: "2026-03-22T08:18:00Z"
updated_at: "2026-03-22T08:30:00Z"
completed_at: "2026-03-22T08:30:00Z"
---

# Story 1.2: 動態事件觸發系統

## User Story

As a 遊戲設計師,
I want 根據遊戲狀態動態觸發歷史事件,
So that 創造沉浸式的歷史體驗。

## Acceptance Criteria

### AC 1.2.1: 事件觸發條件 ✅
**Given** 定義了事件觸發條件
**When** 遊戲狀態變化
**Then** 評估觸發條件
**And** 條件滿足時觸發事件

### AC 1.2.2: 時間觸發 ✅
**Given** 設定了時間觸發器
**When** 遊戲時間達到設定值
**Then** 觸發對應的歷史事件

### AC 1.2.3: 條件觸發 ✅
**Given** 設定了複合條件
**When** 多個條件同時滿足
**Then** 觸發複雜的歷史事件

### AC 1.2.4: 隨機觸發 ✅
**Given** 設定了隨機觸發器
**Then** 按概率隨機觸發事件
**And** 支持加權隨機

## Technical Context

### 功能範圍
1. **事件觸發器基類**: 統一的事件觸發接口
2. **條件評估系統**: 解析和評估觸發條件
3. **時間觸發器**: 基於遊戲時間的事件觸發
4. **狀態觸發器**: 基於遊戲狀態變化的事件觸發
5. **隨機觸發器**: 基於概率的事件觸發

### 依賴
- Story 1.1: 歷史決策系統 (決策觸發基礎)
- Story 6.1: UE5模組化插件架構 (MingStrategic 插件)
- Story 6.2: 事件驅動通訊 (事件訂閱/發布)

### 文件結構
```
MingStrategic/
├── Public/
│   ├── Events/
│   │   ├── MingEventTrigger.h
│   │   ├── MingEventTriggerTime.h
│   │   ├── MingEventTriggerCondition.h
│   │   └── MingEventTriggerRandom.h
│   └── Managers/
│       └── MingEventTriggerManager.h
└── Private/
    ├── Events/
    │   ├── MingEventTrigger.cpp
    │   ├── MingEventTriggerTime.cpp
    │   ├── MingEventTriggerCondition.cpp
    │   └── MingEventTriggerRandom.cpp
    └── Managers/
        └── MingEventTriggerManager.cpp
```

## Implementation Tasks

### Task 1.2.1: 事件觸發器基類 ✅
- [x] 創建 `UMingEventTrigger` 基類
- [x] 定義觸發器接口 (CanTrigger/OnTrigger/Reset)
- [x] 支持觸發器啟用/禁用
- [x] 支持一次性/重複觸發

### Task 1.2.2: 時間觸發器 ✅
- [x] 創建 `UMingEventTriggerTime`
- [x] 實現絕對時間觸發 (1926年3月12日)
- [x] 實現相對時間觸發 (3個月後)
- [x] 實現週期性觸發 (每月/每年)

### Task 1.2.3: 條件觸發器 ✅
- [x] 創建 `UMingEventTriggerCondition`
- [x] 實現資源條件 (金錢 > 1000)
- [x] 實現屬性條件 (聲望 > 50)
- [x] 實現複合條件 (A AND B OR C)

### Task 1.2.4: 隨機觸發器 ✅
- [x] 創建 `UMingEventTriggerRandom`
- [x] 實現基礎概率觸發 (30%)
- [x] 實現加權隨機 (A:50%, B:30%, C:20%)
- [x] 實現冷卻時間管理

### Task 1.2.5: 觸發器管理器 ✅
- [x] 創建 `UMingEventTriggerManager`
- [x] 實現觸發器註冊/註銷
- [x] 實現每幀條件檢查
- [x] 集成 MingCore 事件總線

## 實作摘要

### 完成的系統

1. **事件觸發器基類** (`MingStrategic/Events/MingEventTrigger`)
   - 統一觸發器接口
   - 狀態管理 (Active/Cooldown/Triggered)
   - 冷卻時間支持
   - 事件委託 (OnTriggered)

2. **時間觸發器** (`MingEventTriggerTime`)
   - 絕對時間觸發 (指定年月日)
   - 相對時間觸發 (N個月後)
   - 週期性觸發 (每30天)
   - 每日觸發 (每天12:00)
   - 遊戲內時間系統

3. **條件觸發器** (`MingEventTriggerCondition`)
   - 單個條件評估
   - 複合條件組 (AND/OR)
   - 數值比較 (>, <, ==, 等)
   - 字符串比較

4. **隨機觸發器** (`MingEventTriggerRandom`)
   - 基礎概率觸發
   - 加權隨機選項
   - 概率衰減/增長
   - 觸發窗口管理

5. **觸發器管理器** (`MingEventTriggerManager`)
   - 觸發器註冊/註銷
   - 每幀更新管理
   - 全局觸發事件
   - 工廠方法 (CreateTrigger)

### 文件結構

```
MingStrategic/
├── Public/
│   ├── Events/
│   │   ├── MingEventTrigger.h
│   │   ├── MingEventTriggerTime.h
│   │   ├── MingEventTriggerCondition.h
│   │   └── MingEventTriggerRandom.h
│   └── Managers/
│       └── MingEventTriggerManager.h
└── Private/
    ├── Events/
    │   ├── MingEventTrigger.cpp
    │   ├── MingEventTriggerTime.cpp
    │   ├── MingEventTriggerCondition.cpp
    │   └── MingEventTriggerRandom.cpp
    └── Managers/
        └── MingEventTriggerManager.cpp
```

### 使用示例

```cpp
// 創建時間觸發器
UMingEventTriggerTime* TimeTrigger = Cast<UMingEventTriggerTime>(
    TriggerManager->CreateTrigger(UMingEventTriggerTime::StaticClass(), 
    "trigger_northern_expedition", "event_northern_expedition"));
TimeTrigger->SetAbsoluteTime(1926, 7, 9, 0, 0); // 1926年7月9日

// 創建條件觸發器
UMingEventTriggerCondition* ConditionTrigger = Cast<UMingEventTriggerCondition>(
    TriggerManager->CreateTrigger(UMingEventTriggerCondition::StaticClass(),
    "trigger_resource_check", "event_resource_shortage"));
FSingleCondition Condition;
Condition.TargetKey = "Gold";
Condition.Operator = EConditionOperator::Less;
Condition.TargetValue = 1000;
ConditionTrigger->AddCondition(Condition);

// 創建隨機觸發器
UMingEventTriggerRandom* RandomTrigger = Cast<UMingEventTriggerRandom>(
    TriggerManager->CreateTrigger(UMingEventTriggerRandom::StaticClass(),
    "trigger_random_event", "event_random"));
RandomTrigger->SetBaseProbability(0.3f);
RandomTrigger->SetIntervalRange(10.0f, 30.0f);
```

---

- GDD Section: 歷史決策體驗基礎
- PRD: FR1 (動態事件觸發)
- Story 1.1: 歷史決策系統
- Story 6.1: UE5模組化插件架構
- Story 6.2: 事件驅動通訊系統
