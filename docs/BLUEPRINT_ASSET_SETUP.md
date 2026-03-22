# MingGoRTS 選擇系統 Blueprint 資產創建指南

## 概述

本文檔說明如何在UE5編輯器中創建選擇系統所需的Blueprint UI資產。

---

## 1. 創建選擇框 Widget Blueprint

### 步驟 1: 創建父類別 C++ Widget

1. 在UE5編輯器中，前往 **內容瀏覽器**
2. 右鍵點擊 -> **Blueprint類別**
3. 在 **所有類別** 中搜尋 `MingSelectionWidget`
4. 選擇 `MingSelectionWidget` 作為父類別
5. 命名為 `WBP_SelectionBox`
6. 保存位置：`/Game/Blueprints/UI/`

### 步驟 2: 設置Widget佈局

在 **WBP_SelectionBox** 的設計器中：

```
Canvas Panel (根節點)
└── Border (命名: SelectionBoxBorder)
    ├── 位置: 0, 0
    ├── 大小: 100, 100 (可變)
    ├── 可見性: Hidden
    ├── 背景顏色: RGBA(0, 255, 0, 0.3) (半透明綠色)
    └── 邊框: 2px 實線綠色

TextBlock (命名: SelectionCountText)
├── 位置: 10, 10
├── 文字: "0 / 0"
├── 字體大小: 16
├── 顏色: 白色
└── 可見性: Hidden
```

### 步驟 3: 綁定Widget組件

在 **Graph** 標籤中：

1. 點擊 **SelectionBoxBorder** 組件
2. 在 **Details** 面板中確認 **Is Variable** 已勾選
3. 點擊 **SelectionCountText** 組件
4. 確認 **Is Variable** 已勾選

---

## 2. 創建 Input Mapping Context

### 步驟 1: 創建 Input Actions

創建以下 **Input Action** Blueprints (`/Game/Input/`):

#### IA_Select
```
Value Type: Vector2D (軸2D)
Modifiers: None
Triggers: 
  - Press (開始)
  - Hold (持續)
  - Release (結束)
```

#### IA_SelectComplete
```
Value Type: Digital (bool)
Modifiers: None
Triggers:
  - Release
```

#### IA_MoveCommand
```
Value Type: Digital (bool)
Modifiers: None
Triggers:
  - Press
```

#### IA_AttackCommand
```
Value Type: Digital (bool)
Modifiers: None
Triggers:
  - Press
```

#### IA_SelectAll
```
Value Type: Digital (bool)
Modifiers: 
  - Left Control (或 Right Control)
Triggers:
  - Chorded Action (組合按鍵)
  - Press
```

### 步驟 2: 創建 Input Mapping Context

創建 **IMC_RTS_Default** (`/Game/Input/`):

```
Mappings:

1. Select (IA_Select)
   - 滑鼠左鍵 (Mouse Left Button)
   - 觸發器: Press, Hold, Release

2. SelectComplete (IA_SelectComplete)
   - 滑鼠左鍵 (Mouse Left Button)
   - 觸發器: Release

3. MoveCommand (IA_MoveCommand)
   - 滑鼠右鍵 (Mouse Right Button)
   - 觸發器: Press

4. AttackCommand (IA_AttackCommand)
   - 滑鼠右鍵 (Mouse Right Button)
   - 修飾器: Left Alt
   - 觸發器: Press

5. SelectAll (IA_SelectAll)
   - 鍵盤 A
   - 修飾器: Left Control
   - 觸發器: Press

6. CameraPan_Up
   - 鍵盤 W / 上箭頭
   - 觸發器: Press, Hold

7. CameraPan_Down
   - 鍵盤 S / 下箭頭
   - 觸發器: Press, Hold

8. CameraPan_Left
   - 鍵盤 A / 左箭頭
   - 觸發器: Press, Hold

9. CameraPan_Right
   - 鍵盤 D / 右箭頭
   - 觸發器: Press, Hold
```

---

## 3. 創建遊戲模式 Blueprint

### WBP_RTSGameMode

**父類別**: GameModeBase

**設定**:
```
Player Controller Class: MingRTSPlayerController (C++)
Default Pawn Class: DefaultPawn (或自定義相機Pawn)
HUD Class: WBP_RTSHUD (見下文)
```

---

## 4. 創建 HUD Widget

### WBP_RTSHUD

**父類別**: UserWidget

**結構**:
```
Canvas Panel (根節點)
├── Selection Widget (WBP_SelectionBox)
│   ├── 位置: 0, 0
│   ├── 對齊: 填滿
│   └── ZOrder: 100
├── Bottom Panel
│   ├── 位置: 0, 底部
│   ├── 大小: 填滿, 100
│   └── 背景: 半透明黑色
└── Top Panel
    ├── 位置: 0, 0
    ├── 大小: 填滿, 50
    └── 背景: 半透明黑色
```

---

## 5. 創建測試地圖

### L_Test_RTS_Core

**設定**:
- **Game Mode**: WBP_RTSGameMode
- **玩家起始位置**: (0, 0, 1000)
- **地面**: 大型平面 (5000x5000)
- **測試單位**: 放置10-20個 MingTacticalUnit

**單位佈置**:
```
Unit_1: (1000, 1000, 0)
Unit_2: (1200, 1000, 0)
Unit_3: (1000, 1200, 0)
Unit_4: (1500, 1500, 0)
...
```

---

## 6. Blueprint 整合步驟

### 在 MingRTSPlayerController Blueprint 中：

```
Event BeginPlay
└── Create Widget (WBP_SelectionBox)
    └── Add to Viewport
    └── 保存引用到變數 SelectionWidget

Event Tick
└── 更新 SelectionWidget 位置 (如果需要)

// 輸入處理
Input Action Select (Started)
└── SelectionManager.StartSelectionBox(GetMousePosition())

Input Action Select (Triggered)
└── SelectionManager.UpdateSelectionBox(GetMousePosition())

Input Action SelectComplete (Completed)
└── SelectionManager.EndSelectionBox()

Input Action MoveCommand
└── IssueMoveCommand(GetMouseWorldLocation())
```

---

## 7. 調試和測試

### 控制台命令：
```
show collision           // 顯示碰撞體
stat game                // 顯示遊戲統計
stat unit                // 顯示單位統計
debug.Selection 1        // 啟用選擇系統除錯
```

### 預期行為：
1. **左鍵拖動** → 顯示綠色選擇框
2. **框內單位** → 被選中並高亮
3. **Ctrl+左鍵** → 添加/移除單個單位選擇
4. **右鍵** → 選中單位移動到該位置
5. **Ctrl+A** → 選擇所有單位

---

## 8. 常見問題解決

### Q: 選擇框不顯示
**A**: 檢查 WBP_SelectionBox 的 ZOrder 是否設置為較高值 (100+)

### Q: 單位無法被選中
**A**: 確保 MingTacticalUnit 有碰撞體組件且 Collision Profile 設為 "Unit"

### Q: 輸入無響應
**A**: 檢查 IMC_RTS_Default 是否已添加到玩家控制器的 Enhanced Input Subsystem

---

## 9. 下一步

Blueprint資產創建完成後：
1. 測試選擇系統功能
2. 驗證性能目標
3. 進入AI控制器開發

**參考文檔**: `/docs/SELECTION_SYSTEM_SETUP.md`
