---
title: 'Q-9 量子偵查教學情境'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '7469776'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Examples/DuanqiaoPlayable.cpp
  - Gameplay/QuantumFog.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

腳本化教學關卡：引導玩家走完 **observe → probe → 情報時效** 的量子偵查循環。吃 DuanqiaoPlayable 既有殼（場景/fog/點選全現成），新檔 `DuanqiaoTutorial.cpp`，全離線資源、不依賴外部檔案。

## Boundaries

**In scope:**
- 新範例 `DuanqiaoTutorial`：小型戰場 + 3 朵敵軍機率雲 + 玩家偵查隊
- 三段引導（步驟 UI 面板 + 高亮目標 + 完成判定）：
  1. **觀測**：LMB 點雲 → Observe → 真身顯示（扣情報）
  2. **探測**：RMB/Probe 鍵對第二朵雲 → Probe → 雲收縮不塌縮
  3. **時效**：等情報過期 → 看真身回雲（加速提示 or 縮短 duration）
- 每段完成條件由 fog 事件/狀態輪詢判定；可重看（R 鍵重置段落）
- HUD 步驟指示（ImGui 文字 + 進度）；完成後顯示總結
- headless 驗證：教學邏輯抽成可測狀態機（步進/完成判定），進 ctest

**Out of scope:**
- 語音/動畫引導、任務系統泛化（這是單一腳本關）
- 干涉/糾纏教學（Q-2/Q-7 是進階機制，教學只教基礎循環）
- 存檔/成就

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| LMB 點雲（階段 1） | Observe 成功 → 進入階段 2 |
| Probe 鍵 + 點雲（階段 2） | Probe 成功 → 進入階段 3 |
| 情報過期（階段 3） | 完成 → 總結面板；R 重置 |
| 非當前階段操作 | 提示「先看提示」但不禁用（自由探索） |

**狀態機**（可 headless 測）：`TutorialStep{Observe, Probe, Expire, Done}` + `Advance(event)` 純函式。

## Implementation Checklist

- [ ] `Gameplay/TutorialScript.h/.cpp`（或 Examples 內 header-only）：步驟狀態機、事件→步進映射、每步提示文字；純邏輯可測
- [ ] `Examples/DuanqiaoTutorial.cpp`：複用 DuanqiaoPlayable 場景骨架（最小戰場 12×8、3 敵雲、1 偵查隊）、步驟面板、高亮當前目標雲、R 重置
- [ ] `Examples/TutorialScriptTest.cpp` 或併入既有測試：步進序列、亂序操作不推進、重置回第一步
- [ ] `CMakeLists.txt`：target + POTATO_TESTS

## Acceptance Criteria

- [ ] 三段引導可完成、R 可重看
- [ ] 全離線資源（只用既有 mesh/UI）
- [ ] headless 狀態機測試 PASS；MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target DuanqiaoTutorial <test-target>`
- `cd build && ctest -C Release --output-on-failure`
- `cmake --build build-mingw --target DuanqiaoTutorial`

**Manual checks:**
- `DuanqiaoTutorial.exe`：跟提示走完三段，R 重置可重跑

</frozen-after-approval>
