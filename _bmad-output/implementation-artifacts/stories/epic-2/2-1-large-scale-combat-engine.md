---
story_id: "2-1"
epic_id: "epic-2"
title: "大規模戰鬥引擎核心"
status: "ready-for-dev"
created_at: "2026-03-22T07:35:00Z"
updated_at: "2026-03-22T07:35:00Z"
---

# Story 2.1: 大規模戰鬥引擎核心

## User Story

As a 玩家,
I want 控制大規模單位進行戰鬥,
So that 體驗民國時期的大型戰役。

## Acceptance Criteria

### AC 2.1.1: 大規模單位渲染
**Given** 戰場上有1000+單位
**When** 渲染戰場
**Then** 保持60 FPS性能
**And** 單位可見性和細節層次正確

### AC 2.1.2: 單位選擇系統
**Given** 玩家需要選擇單位
**When** 點擊或框選
**Then** 支持單選、多選、框選
**And** 選擇反饋清晰

### AC 2.1.3: 移動指令系統
**Given** 單位已被選中
**When** 下達移動指令
**Then** 單位沿路徑移動
**And** 避開障礙物

### AC 2.1.4: 攻擊指令系統
**Given** 單位已選中且敵人在範圍內
**When** 下達攻擊指令
**Then** 單位攻擊目標
**And** 造成傷害並顯示效果

## Technical Context

### 性能目標
- 支持 1000+ 單位同時作戰
- 保持 60 FPS (PC) / 30 FPS (移動端)
- 內存使用 < 2GB

### 技術方案
1. **實例化渲染**: 減少Draw Call
2. **LOD系統**: 遠距離簡化模型
3. **分塊更新**: 分批處理單位邏輯
4. **空間分割**: 加速碰撞檢測

### 核心組件
- `AMingTacticalUnit` - 戰術單位基類
- `UMingUnitMovementComponent` - 移動組件
- `UMingUnitCombatComponent` - 戰鬥組件
- `UMingSelectionManager` - 選擇管理器

## Implementation Tasks

### Task 2.1.1: 戰術單位基類
- [ ] 創建 `AMingTacticalUnit` 基類
- [ ] 實現單位屬性 (生命值、速度、攻擊力)
- [ ] 實現狀態機 (Idle/Moving/Attacking/Dead)
- [ ] 集成 MingCore 事件系統

### Task 2.1.2: 單位選擇系統
- [ ] 創建 `UMingSelectionManager`
- [ ] 實現單擊選擇
- [ ] 實現框選 (Drag Selection)
- [ ] 實現選擇框渲染
- [ ] 發布選擇事件

### Task 2.1.3: 移動系統
- [ ] 創建 `UMingUnitMovementComponent`
- [ ] 集成 AI Navigation
- [ ] 實現路徑請求
- [ ] 實現隊形保持
- [ ] 避開障礙物

### Task 2.1.4: 戰鬥系統
- [ ] 創建 `UMingUnitCombatComponent`
- [ ] 實現攻擊範圍檢測
- [ ] 實現傷害計算
- [ ] 實現攻擊動畫觸發
- [ ] 實現死亡處理

### Task 2.1.5: 性能優化
- [ ] 實現實例化渲染
- [ ] 實現 LOD 系統
- [ ] 實現分塊更新
- [ ] 性能測試 (1000單位)

## References

- GDD Section: 大規模戰術戰鬥系統
- PRD: FR2 (大規模RTS戰鬥)
- Story 6.1: 插件架構 (MingTactical 插件)
- Story 6.2: 事件通訊 (單位選擇/移動事件)
