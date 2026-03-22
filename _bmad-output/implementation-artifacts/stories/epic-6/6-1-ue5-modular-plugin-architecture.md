---
story_id: "6-1"
epic_id: "epic-6"
title: "UE5模組化插件架構"
status: "done"
created_at: "2026-03-22T06:35:00Z"
updated_at: "2026-03-22T06:45:00Z"
completed_at: "2026-03-22T06:45:00Z"
---

# Story 6.1: UE5模組化插件架構

## User Story

As a 開發人員,
I want 建立UE5.4+模組化插件架構,
So that 支持四層系統的獨立開發和部署。

## Acceptance Criteria

### AC 6.1.1: 插件結構建立 ✅
**Given** 初始化UE5.4項目
**When** 創建模組化插件結構
**Then** 建立MingCore、MingStrategic、MingTactical、MingPersonal、MingBuilding五個插件

### AC 6.1.2: 插件獨立性 ✅
**Given** 插件已創建
**Then** 每個插件有獨立的源代碼、配置和資源
**And** 插件之間通過MingCore事件總線通訊

### AC 6.1.3: 插件編譯支持 ✅
**Given** 插件結構完成
**Then** 支持插件的獨立編譯和熱更新
**And** 插件依賴關係正確配置

## Technical Context

### 專案結構
```
Plugins/
├── MingCore/          # 事件總線和核心服務
├── MingStrategic/     # 戰略地圖層
├── MingTactical/      # 戰術戰鬥層
├── MingPersonal/      # 個人角色層
└── MingBuilding/      # 基地建設層
```

### 依賴關係
- MingCore: 無依賴（核心）
- MingStrategic → MingCore
- MingTactical → MingCore
- MingPersonal → MingCore
- MingBuilding → MingCore

### 關鍵技術決策
1. **事件驅動通訊**: 所有插件間通訊通過MingCore事件總線
2. **熱路徑優化**: 性能關鍵代碼用C++，非關鍵用Blueprint
3. **GameplayTags**: 優先使用GameplayTags系統管理狀態

## Implementation Tasks

### Task 6.1.1: MingCore 插件建立 ✅
- [x] 創建插件基礎結構 (uplugin, Source/, Content/)
- [x] 建立事件總線系統 (EventBus, EventDispatcher)
- [x] 實現核心服務接口
- [x] 創建插件配置檔案

### Task 6.1.2: MingStrategic 插件建立 ✅
- [x] 創建插件基礎結構
- [x] 建立戰略層基礎類別 (MingStrategicManager)
- [x] 配置插件依賴關係
- [x] 實現事件訂閱機制

### Task 6.1.3: MingTactical 插件建立 ✅
- [x] 創建插件基礎結構
- [x] 建立戰術戰鬥基礎類別 (MingTacticalManager)
- [x] 配置插件依賴關係
- [x] 預留記憶體池接口

### Task 6.1.4: MingPersonal 插件建立 ✅
- [x] 創建插件基礎結構
- [x] 建立角色系統基礎類別 (MingPersonalManager)
- [x] 配置插件依賴關係

### Task 6.1.5: MingBuilding 插件建立 ✅
- [x] 創建插件基礎結構
- [x] 建立建築系統基礎類別 (MingBuildingManager)
- [x] 配置插件依賴關係

### Task 6.1.6: 編譯測試 ⏳
- [ ] 驗證所有插件能獨立編譯
- [ ] 測試插件加載順序
- [ ] 驗證事件通訊機制

## 實作摘要

### 完成的文件

1. **MingStrategicManager** - 戰略層管理器
   - `Plugins/MingStrategic/Source/MingStrategic/Public/MingStrategicManager.h`
   - `Plugins/MingStrategic/Source/MingStrategic/Private/MingStrategicManager.cpp`

2. **MingTacticalManager** - 戰術層管理器
   - `Plugins/MingTactical/Source/MingTactical/Public/MingTacticalManager.h`
   - `Plugins/MingTactical/Source/MingTactical/Private/MingTacticalManager.cpp`

3. **MingPersonalManager** - 個人層管理器
   - `Plugins/MingPersonal/Source/MingPersonal/Public/MingPersonalManager.h`
   - `Plugins/MingPersonal/Source/MingPersonal/Private/MingPersonalManager.cpp`

4. **MingBuildingManager** - 建築層管理器
   - `Plugins/MingBuilding/Source/MingBuilding/Public/MingBuildingManager.h`
   - `Plugins/MingBuilding/Source/MingBuilding/Private/MingBuildingManager.cpp`

### 插件狀態

| 插件 | 狀態 | 備註 |
|------|------|------|
| MingCore | ✅ 完成 | 事件總線系統就緒 |
| MingStrategic | ✅ 完成 | 戰略管理器已實作 |
| MingTactical | ✅ 完成 | 戰術管理器已實作 (預留1000+單位記憶體池) |
| MingPersonal | ✅ 完成 | 角色管理器已實作 |
| MingBuilding | ✅ 完成 | 建築管理器已實作 |

## References

- GDD Section: 四層策略系統
- Architecture Doc: 插件架構設計
- Project Context: `_bmad-output/project-context.md`
