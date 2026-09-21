---
title: 'G-4 地形加成（高地/伏擊/減速）'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '5c6aafb'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Gameplay/FlowField.h
  - Gameplay/BattleMap.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

地圖地形從「通行成本」升級為「戰鬥修正」：高地給攻方傷害加成、森林給守方減傷、泥濘用既有 cost 減速。Total War 式地形讀取——玩家部署時要看地勢。

## Boundaries

**In scope:**
- `FlowField` 加 cell 地形層：`SetTerrain(x,y,type)` / `TerrainAt(worldPos)`，型別 `enum class TerrainType { Plain, Highland, Forest, Mud }`
- `BattleMap`：TerrainPatch 加可選 `type` 字串欄位（highland/forest/mud/plain），缺省 plain（向後相容）；ApplyToField 寫進 FlowField
- `BattleController::ResolveCombat`：攻方在高地 → 傷害 ×1.25；守方在森林 → 承受傷害 ×0.7（與兵種克制疊乘）
- 新 headless 測試 `TerrainBonusTest` 入 POTATO_TESTS

**Out of scope:**
- 高地射程加成（無射程系統——攻擊是 engage 接觸制）
- 視線遮蔽（接 Q-1 fog 是另一層）
- 地形視覺化（渲染層留給 demo 自行配色）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| terrain patch `type:"highland"` | 該格上攻方傷害 ×1.25 |
| `type:"forest"` | 該格上守方受傷 ×0.7 |
| `type:"mud"` + `cost:2.5` | 既有 SetCost 減速（不需新機制） |
| 無 type 欄位 | plain——零回歸 |

## Implementation Checklist

- [ ] `Gameplay/FlowField.h/.cpp`：terrain 層 + `TerrainAt`（越界回 Plain）
- [ ] `Gameplay/BattleMap.h/.cpp`：patch `type` 解析 + ApplyToField 寫入
- [ ] `Gameplay/BattleController.cpp`：ResolveCombat 套地形倍率（localized patch，避開平行 session 的 G-2 區塊）
- [ ] `Examples/TerrainBonusTest.cpp`：高地/平地傷害差、森林減傷、無 terrain 退回平地、JSON 載入

## Acceptance Criteria

- [ ] 地圖可標 terrain 且 loader 讀取
- [ ] 高地 vs 平地傷害差可測（headless）
- [ ] 無 terrain 欄位時全平地零回歸
- [ ] MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target TerrainBonusTest` + ctest
- `cmake --build build-mingw --target TerrainBonusTest`

</frozen-after-approval>
