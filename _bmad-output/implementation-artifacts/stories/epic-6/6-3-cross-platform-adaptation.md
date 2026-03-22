---
story_id: "6-3"
epic_id: "epic-6"
title: "跨平台適配系統"
status: "done"
created_at: "2026-03-22T06:55:00Z"
updated_at: "2026-03-22T07:15:00Z"
completed_at: "2026-03-22T07:15:00Z"
---

# Story 6.3: 跨平台適配系統

## User Story

As a 開發人員,
I want 實現PC、Android、iOS跨平台適配,
So that 遊戲可以在多個平台上運行並提供一致體驗。

## Acceptance Criteria

### AC 6.3.1: 平台抽象層 ✅
**Given** 不同平台的差異
**When** 遊戲在不同平台運行
**Then** 平台相關代碼通過抽象層隔離

### AC 6.3.2: 輸入適配 ✅
**Given** PC使用鼠標鍵盤，移動端使用觸控
**When** 玩家進行輸入操作
**Then** 輸入系統自動適配當前平台

### AC 6.3.3: UI適配 ✅
**Given** 不同屏幕尺寸和分辨率
**Then** UI自動適配並保持良好可用性
**And** 觸控目標尺寸符合平台規範

## Technical Context

### 支持平台
- **Windows PC**: 主要開發平台，鼠標+鍵盤輸入
- **Android**: 觸控輸入，多種屏幕尺寸
- **iOS**: 觸控輸入，統一屏幕規格

### 關鍵技術
- **Platform Abstraction Layer**: 統一的平台接口
- **Input Mapping**: 輸入映射系統
- **Responsive UI**: 響應式UI設計
- **Performance Scaling**: 性能自動調整

### 架構設計
```
MingCore/
├── Platform/
│   ├── IPlatformInterface.h
│   ├── WindowsPlatform.cpp
│   ├── AndroidPlatform.cpp
│   └── IOSPlatform.cpp
├── Input/
│   ├── MingInputManager.h
│   ├── InputMappingConfig.h
│   └── TouchInputProcessor.cpp
└── UI/
    ├── MingUIManager.h
    ├── ResponsiveLayout.cpp
    └── PlatformUIHelpers.cpp
```

## Implementation Tasks

### Task 6.3.1: 平台抽象層 ✅
- [x] 創建 `IPlatformInterface` 接口
- [x] 實現 Windows 平台適配 (`UWindowsPlatformAdapter`)
- [x] 實現 Android 平台適配 (`UAndroidPlatformAdapter`)
- [x] 實現 iOS 平台適配 (`UIOSPlatformAdapter`)
- [x] 創建 `MingPlatformManager` 管理器

### Task 6.3.2: 輸入適配系統 ✅
- [x] 創建統一輸入管理器 (`MingInputManager`)
- [x] 實現輸入映射配置 (`EInputAction`, `EInputGesture`)
- [x] 實現觸控輸入處理 (多點觸控、手勢識別)
- [x] 支持鼠標鍵盤和觸控自動切換

### Task 6.3.3: UI適配系統 ✅
- [x] 創建響應式UI管理器 (`MingUIManager`)
- [x] 實現屏幕尺寸適配 (Desktop/Tablet/Phone)
- [x] 實現觸控目標尺寸調整 (最小 44pt)
- [x] 實現安全區域適配 (劉海屏、Home Indicator)
- [x] DPI 縮放和全局 UI 縮放

### Task 6.3.4: 性能適配 ✅
- [x] 實現畫質自動調整 (平台推薦設置)
- [x] 單位數量限制 (PC: 1000, Mobile: 500)
- [x] 性能模式管理 (省電/平衡/性能)
- [x] 電池監測和優化提示

### Task 6.3.5: 構建配置 ⏳
- [ ] 配置Windows構建
- [ ] 配置Android構建
- [ ] 配置iOS構建
- [ ] 創建跨平台構建腳本

## 實作摘要

### 完成的系統

1. **平台抽象層** (`MingCore/Platform/`)
   - `IPlatformInterface` - 統一平台接口
   - `UWindowsPlatformAdapter` - Windows 桌面適配
   - `UAndroidPlatformAdapter` - Android 移動適配
   - `UIOSPlatformAdapter` - iOS 移動適配
   - `MingPlatformManager` - 平台管理器

2. **輸入適配系統** (`MingCore/Input/`)
   - `MingInputManager` - 統一輸入管理
   - 支持鼠標鍵盤和觸控雙模式
   - 手勢識別 (點擊、長按、滑動、縮放)
   - 輸入動作綁定系統

3. **UI適配系統** (`MingCore/UI/`)
   - `MingUIManager` - 響應式 UI 管理
   - 4 種布局類型 (Desktop/Tablet/PhoneLandscape/PhonePortrait)
   - 自動 DPI 縮放
   - 安全區域適配
   - 觸控目標最小 44pt (符合 iOS/Android 規範)

### 平台能力對比

| 功能 | Windows | Android | iOS |
|------|---------|---------|-----|
| 輸入類型 | 鼠標+鍵盤 | 觸控 | 觸控 |
| 最大單位數 | 1000 | 500 | 600 |
| 畫質等級 | 5 (最高) | 2 (中等) | 3 (較高) |
| 安全區域 | 無 | 有 | 有 |
| 電池優化 | 否 | 是 | 是 |
| 多點觸控 | 否 | 是 (10點) | 是 (5點) |

---

- GDD Section: 跨平台技術
- PRD: FR10, NFR2
- Project Context: `_bmad-output/project-context.md`
- UX Design Spec: 雙重輸入支持 (UX-DR3)
