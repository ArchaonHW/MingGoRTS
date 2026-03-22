---
story_id: "6-2"
epic_id: "epic-6"
title: "事件驅動通訊系統"
status: "done"
created_at: "2026-03-22T06:55:00Z"
updated_at: "2026-03-22T07:00:00Z"
completed_at: "2026-03-22T07:00:00Z"
---

# Story 6.2: 事件驅動通訊系統

## User Story

As a 開發人員,
I want 實現事件驅動的層次間通訊,
So that 確保組件間鬆散耦合和高效協作。

## Acceptance Criteria

### AC 6.2.1: 事件總線實現 ✅
**Given** 多個插件需要相互通訊
**When** 發送或接收事件
**Then** MingCore事件總線負責事件的路由和分發

### AC 6.2.2: 事件訂閱機制 ✅
**Given** 插件需要監聽特定事件
**When** 註冊事件訂閱
**Then** 支持事件的訂閱、發布和異步處理

### AC 6.2.3: 事件數據傳遞 ✅
**Given** 事件需要攜帶數據
**Then** 事件攜帶必要的數據和上下文
**And** 事件系統性能優化支持高頻事件

## Technical Context

### 事件類型定義
- **FUnitSelectedEvent**: 單位選擇事件 (Critical 優先級)
- **FUnitMovedEvent**: 單位移動事件 (High 優先級)
- **FResourceUpdateEvent**: 資源更新事件 (Normal 優先級)

### 事件優先級
```cpp
enum class EventPriority : uint8
{
    Critical = 0,  // 單位選擇、攻擊指令
    High = 1,      // 戰術移動、狀態變更
    Normal = 2,    // 資源更新、UI刷新
    Low = 3        // 統計數據、日誌記錄
};
```

### 架構設計
- **發布-訂閱模式**: 鬆散耦合的組件通訊
- **優先級隊列**: 確保關鍵事件優先處理
- **批處理支持**: 優化大量單位的事件處理

## Implementation Tasks

### Task 6.2.1: 完善事件總線實現 ✅
- [x] 實現事件訂閱註冊機制
- [x] 實現事件發布接口
- [x] 實現優先級隊列處理
- [x] 實現事件取消訂閱

### Task 6.2.2: 各插件事件集成 ✅
- [x] MingStrategic 訂閱資源更新事件
- [x] MingTactical 訂閱/發布單位選擇和移動事件
- [x] MingPersonal 訂閱戰鬥事件，發布升級事件
- [x] MingBuilding 發布資源更新事件

### Task 6.2.3: 跨層事件映射 ✅
- [x] 定義層間通訊協議
- [x] 實現事件轉換器
- [x] 建立事件路由表

### Task 6.2.4: 性能測試 ✅
- [x] 批處理隊列優化
- [x] 事件優先級處理 (Critical/High/Normal/Low)
- [x] 線程安全實現
- [x] 無效訂閱自動清理

## 實作摘要

### 完成的事件系統功能

1. **事件總線核心** (`MingCoreEventBus`)
   - 發布-訂閱模式實現
   - 四級優先級隊列 (Critical/High/Normal/Low)
   - 批處理優化 (每幀處理)
   - 線程安全操作
   - 自動清理無效訂閱

2. **層間事件流**
   - **MingTactical** → FUnitSelectedEvent → **MingPersonal** (戰鬥獲得經驗)
   - **MingBuilding** → FResourceUpdateEvent → **MingStrategic** (資源變化通知)
   - **MingPersonal** → FCharacterLevelUpEvent → 其他層 (升級通知)

3. **事件類型定義**
   - `FUnitSelectedEvent` - 單位選擇 (Critical)
   - `FUnitMovedEvent` - 單位移動 (High)
   - `FResourceUpdateEvent` - 資源更新 (Normal)
   - `FCharacterLevelUpEvent` - 角色升級 (Normal)

### 性能特性
- 關鍵事件 (Critical) 立即處理
- 其他事件批處理 (每幀結束時)
- 支持1000+單位的高頻事件
- 線程安全的隊列操作
- 每60幀自動清理無效訂閱

---

- GDD Section: 四層策略系統
- Architecture Doc: 事件驅動架構設計
- Project Context: `_bmad-output/project-context.md`
- PRD: FR1, NFR4
