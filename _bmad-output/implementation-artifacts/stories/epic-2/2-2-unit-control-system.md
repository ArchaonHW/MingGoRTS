---
story_id: "2-2"
epic_id: "epic-2"
title: "單位控制系統"
status: "ready-for-dev"
created_at: "2026-03-22T08:09:00Z"
updated_at: "2026-03-22T08:09:00Z"
---

# Story 2.2: 單位控制系統

## User Story

As a 玩家,
I want 通過直覺的界面控制單位,
So that 高效地指揮部隊作戰。

## Acceptance Criteria

### AC 2.2.1: 快捷鍵系統
**Given** 玩家需要快速執行命令
**When** 按下快捷鍵
**Then** 執行對應的單位控制命令
**And** 支持自定義快捷鍵綁定

### AC 2.2.2: 快捷欄
**Given** 玩家需要快速訪問技能和命令
**Then** 顯示快捷欄界面
**And** 支持點擊或快捷鍵觸發

### AC 2.2.3: 技能釋放
**Given** 單位擁有技能
**When** 玩家激活技能
**Then** 進入技能瞄準模式
**And** 點擊目標後釋放技能

### AC 2.2.4: 隊形控制
**Given** 多個單位被選中
**When** 選擇隊形類型
**Then** 單位以指定隊形移動

## Technical Context

### 功能範圍
1. **快捷鍵綁定系統**: 鍵盤/觸控映射到單位命令
2. **快捷欄界面**: 可配置的技能和命令欄
3. **技能系統**: 瞄準、冷卻、效果觸發
4. **隊形系統**: 多種預設隊形 (線形、楔形、方形等)

### 依賴
- Story 2.1: 大規模戰鬥引擎核心 (單位基類、選擇系統)
- Story 6.3: 跨平台適配 (輸入管理)

### 文件結構
```
MingTactical/
├── Public/
│   ├── Input/
│   │   ├── MingHotkeyManager.h
│   │   └── MingCommandBinding.h
│   ├── UI/
│   │   ├── MingQuickbarWidget.h
│   │   └── MingAbilitySlotWidget.h
│   ├── Ability/
│   │   ├── MingAbility.h
│   │   ├── MingAbilityManager.h
│   │   └── MingAbilityTarget.h
│   └── Formation/
│       ├── MingFormationManager.h
│       └── MingFormationType.h
└── Private/
    ├── Input/
    │   ├── MingHotkeyManager.cpp
    │   └── MingCommandBinding.cpp
    ├── UI/
    │   ├── MingQuickbarWidget.cpp
    │   └── MingAbilitySlotWidget.cpp
    ├── Ability/
    │   ├── MingAbility.cpp
    │   ├── MingAbilityManager.cpp
    │   └── MingAbilityTarget.cpp
    └── Formation/
        ├── MingFormationManager.cpp
        └── MingFormationType.cpp
```

## Implementation Tasks

### Task 2.2.1: 快捷鍵綁定系統
- [ ] 創建 `UMingHotkeyManager`
- [ ] 實現鍵盤輸入映射
- [ ] 實現快捷鍵綁定配置
- [ ] 支持命令優先級

### Task 2.2.2: 快捷欄界面
- [ ] 創建 `UMingQuickbarWidget`
- [ ] 實現快捷欄布局 (1x8 或 2x4)
- [ ] 實現技能圖標顯示
- [ ] 實現冷卻動畫
- [ ] 支持觸控和快捷鍵觸發

### Task 2.2.3: 技能系統
- [ ] 創建 `UMingAbility` 基類
- [ ] 創建 `UMingAbilityManager`
- [ ] 實現技能瞄準模式
- [ ] 實現技能冷卻管理
- [ ] 實現技能效果觸發

### Task 2.2.4: 隊形系統
- [ ] 創建 `UMingFormationManager`
- [ ] 定義隊形類型枚舉
- [ ] 實現線形隊形 (Line)
- [ ] 實現楔形隊形 (Wedge)
- [ ] 實現方形隊形 (Square)
- [ ] 實現散開隊形 (Scatter)

### Task 2.2.5: 移動端適配
- [ ] 觸控快捷欄支持
- [ ] 手勢技能釋放
- [ ] 簡化隊形選擇界面

## References

- GDD Section: 大規模戰術戰鬥系統
- PRD: FR2 (單位控制)
- Story 2.1: 大規模戰鬥引擎核心
- Story 6.3: 跨平台適配系統
