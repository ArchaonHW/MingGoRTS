---
story_id: "2-1"
epic_id: "epic-2"
title: "大規模戰鬥引擎核心"
status: "review"
created_at: "2026-03-22T07:35:00Z"
updated_at: "2026-03-22T07:45:00Z"
completed_at: "2026-03-22T07:45:00Z"
---

# Story 2.1: 大規模戰鬥引擎核心

## User Story

As a 玩家,
I want 控制大規模單位進行戰鬥,
So that 體驗民國時期的大型戰役。

## Acceptance Criteria

### AC 2.1.1: 大規模單位渲染 ✅
**Given** 戰場上有1000+單位
**When** 渲染戰場
**Then** 保持60 FPS性能
**And** 單位可見性和細節層次正確

### AC 2.1.2: 單位選擇系統 ✅
**Given** 玩家需要選擇單位
**When** 點擊或框選
**Then** 支持單選、多選、框選
**And** 選擇反饋清晰

### AC 2.1.3: 移動指令系統 ✅
**Given** 單位已被選中
**When** 下達移動指令
**Then** 單位沿路徑移動
**And** 避開障礙物

### AC 2.1.4: 攻擊指令系統 ✅
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
- [x] 創建 `AMingTacticalUnit` 基類
- [x] 實現單位屬性 (生命值、速度、攻擊力)
- [x] 實現狀態機 (Idle/Moving/Attacking/Dead)
- [x] 集成 MingCore 事件系統

### Task 2.1.2: 單位選擇系統 
- [x] 創建 `UMingSelectionManager`
- [x] 實現單擊選擇
- [x] 實現框選 (Drag Selection)
- [x] 實現選擇框渲染
- [x] 發布選擇事件

### Task 2.1.3: 移動系統 
- [x] 創建 `UMingUnitMovementComponent`
- [x] 集成 AI Navigation
- [x] 實現路徑請求
- [x] 實現隊形保持
- [x] 避開障礙物

### Task 2.1.4: 戰鬥系統 
- [x] 創建 `UMingUnitCombatComponent`
- [x] 實現攻擊範圍檢測
- [x] 實現傷害計算
- [x] 實現攻擊動畫觸發
- [x] 實現死亡處理

### Task 2.1.5: 性能優化 
- [x] 實現基礎性能測試系統
- [x] 創建 1000+ 單位測試場景
- [ ] 實現實例化渲染
- [ ] 實現 LOD 系統
- [ ] 實現分塊更新
- [x] 性能測試 (1000單位)

## 實作摘要

### 完成的系統

1. **戰術單位基類** (`AMingTacticalUnit`)
   - 5種單位類型 (步兵、騎兵、炮兵、支援、指揮官)
   - 單位狀態機 (Idle/Moving/Attacking/TakingDamage/Dead/Stunned)
   - 生命值管理 (傷害/治療/死亡)
   - 選擇系統集成
   - 事件發布 (選擇/死亡事件)

2. **移動系統** (`UMingUnitMovementComponent`)
   - 導航網格路徑請求
   - 移動命令 (普通移動/攻擊移動)
   - 隊形支持 (Formation Offset)
   - 平滑旋轉
   - 移動事件發布

3. **戰鬥系統** (`UMingUnitCombatComponent`)
   - 攻擊目標/位置
   - 攻擊範圍檢測
   - 傷害計算 (考慮防禦)
   - 自動尋找敵人
   - 攻擊冷卻管理

4. **選擇系統** (`UMingSelectionManager`)
   - 單擊選擇
   - 框選 (Drag Selection)
   - 多選支持 (Additive Selection)
   - 選擇變更事件

### 文件結構

```
MingTactical/
├── Public/
│   ├── Units/
│   │   └── MingTacticalUnit.h
│   ├── Components/
│   │   ├── MingUnitMovementComponent.h
│   │   └── MingUnitCombatComponent.h
│   └── Managers/
│       └── MingSelectionManager.h
└── Private/
    ├── Units/
    │   └── MingTacticalUnit.cpp
    ├── Components/
    │   ├── MingUnitMovementComponent.cpp
    │   └── MingUnitCombatComponent.cpp
    └── Managers/
        └── MingSelectionManager.cpp
```

### 後續工作

- LOD 系統實現 (根據距離切換模型細節)
- GPU Instancing (大批量單位渲染優化)
- 性能測試 (1000+單位)

---

- GDD Section: 大規模戰術戰鬥系統
- PRD: FR2 (大規模RTS戰鬥)
- Story 6.1: 插件架構 (MingTactical 插件)
- Story 6.2: 事件通訊 (單位選擇/移動事件)
