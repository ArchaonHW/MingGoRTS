---
title: 'G-6 小隊編制模板（HOI4 division designer）'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '2977e75'
context:
  - AGENTS.md
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Gameplay/Squad.h
  - Gameplay/BattleController.h
  - Gameplay/BattleMap.cpp
  - Serialization/JsonParser.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

HOI4 式小隊編制模板：squad 的組成（人數/兵種/屬性/成本）定義成 JSON 模板檔，
編成階段用模板建隊而非逐隊手寫參數；預算限制下配兵，超支有寫死的降規規則。
模板 schema 沿用 `potato.* /1` 風格（`assets/maps`、`assets/cards` 慣例）。

## Boundaries

**In scope:**
- 新 schema `potato.squad_template/1`：`{schema, id, name, unit_class, members, cost, stats{speed, engage_range, damage_per_member, stamina{drain_move, drain_combat, regen, threshold, penalty_mul}}}`
- `Gameplay/SquadTemplate.h/.cpp`：`SquadTemplate` struct + `LoadFromFile`（JsonValue 解析，schema 驗證，缺欄位用預設值）+ `Instantiate(battle, name, team, pos)` 建 squad 並套用全部屬性
- `Gameplay/SquadTemplateLibrary`（可併入同一 header）：掃描目錄載入多模板、`Find(id)`、依 cost 排序查詢
- 預算規則（寫死）：`BudgetedBuild(library, budget, orderedWishlist)` — 依序檢查每個 wishlist 項目，cost ≤ 剩餘預算才建；單項超支**跳過**（非降人數），回傳 `{built[], skipped[], spent}`；budget=0 視為無上限（保回歸）
- `assets/squads/` 內建模板 ≥3 個（如 `infantry_line`、`archer_skirmish`、`cavalry_shock`），吃 G-2 的 unit_class 字串
- `Examples/SquadTemplateTest.cpp`：載入/建隊屬性套用/預算跳過/壞檔拒絕；入 POTATO_TESTS
- `CMakeLists.txt` target + 註冊

**Out of scope:**
- 編制 UI（division designer 介面是 A-1 範圍）
- 模板內混編多兵種（v1 一模板一兵種）
- 與 BattleResources.loot/cp 的經濟串接（預算值由呼叫端給）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| 模板 JSON（合法 schema） | `SquadTemplate` 物件，`Instantiate` 建出屬性全套的 Squad* |
| 模板 JSON（schema 錯/JSON 壞） | LoadFromFile false，不產生半成品 |
| wishlist + budget 充足 | 全部建成，spent=總成本 |
| 單項 cost > 剩餘預算 | 該項跳過、記入 skipped，後續項繼續評估 |
| budget=0 | 不檢查成本，全部建成（無上限模式） |

**錯誤處理**：模板缺 `stats` 子欄位 → 用 Squad 預設值（不 fail）；`unit_class` 無效字串 → infantry + 警告路徑（比照 EnemyGeneral 字串解析慣例）；Instantiate 對已存在同名 squad 不擋（與 CreateSquad 一致）。

## Implementation Checklist

- [ ] `Gameplay/SquadTemplate.h/.cpp` — struct + LoadFromFile + Instantiate + SquadTemplateLibrary（LoadDir/Find/SortedByCost）
- [ ] `Gameplay/BudgetedBuild`（放 SquadTemplate.cpp 或獨立小函式）— 依序配兵 + skipped 記錄
- [ ] `assets/squads/*.json` — ≥3 內建模板（infantry/archer/cavalry 各一，數值對齊 demo 既有小隊手感）
- [ ] `Examples/SquadTemplateTest.cpp` — 快樂路徑/屬性全套驗證/預算跳過/budget=0/壞 JSON/缺 stats 預設/unit_class 無效降級
- [ ] `CMakeLists.txt` — target + POTATO_TESTS

## Design Notes

- JSON 解析用 `Serialization/JsonParser.h`（`Potato::JsonValue`），比照 `BattleMap::LoadFromFile` 的逐欄位防禦式讀法——不引第三方 JSON 庫。
- `unit_class` 字串→enum 映射直接複用 `EnemyGeneral`/`Squad` 現有的字串解析（找現成 helper，沒有就在 SquadTemplate.cpp 寫一個小 map）。
- Instantiate 走 `battle.CreateSquad` 再逐個 setter 套 stats——不改 CreateSquad 簽名，引擎面零侵入。
- 「跳過而非降人數」是寫死規則：HOI4 模板是原子單位，人數是模板的一部分；降規會讓驗收語義模糊。
- stamina 子物件整段可選；有寫就五欄全要求（缺一欄 → 該欄用 Squad 預設）。

## Acceptance Criteria

- [x] 模板檔可載入並建出對應 squad（屬性全套驗證：members/unitClass/speed/engageRange/damagePerMember/stamina）
- [x] 預算超支時依規則跳過並記錄；budget=0 全部建成
- [x] headless 測試 PASS 入 ctest；MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target SquadTemplateTest` — PASS
- `cd build && ctest -C Release -R "SquadTemplateTest|RefitCampTest"` — 2/2 PASS（SquadTemplateTest 98 checks）
- `cmake --build build-mingw --target SquadTemplateTest RefitCampTest` — PASS；MinGW 執行 98/98
- banned-function 掃描（Gameplay/SquadTemplate.*、RefitCamp.*、兩測試）— 0 命中

**Manual checks:** 無（純邏輯，headless 全覆蓋）

</frozen-after-approval>

## Implementation Summary

四路 review（Blind/Edge/Verification-Gap/Intent）後的 patch 全數落地：

- `Gameplay/SquadTemplate.h/.cpp`：`potato.squad_template/1` 解析（schema 嚴驗、失敗先重置乾淨狀態）、`ReadNonNeg` 數值消毒（負值/非有限→夾值+警告、members/cost 防 int 溢位）、`Instantiate`（匿名模板回 nullptr）+ `ApplyStats`（供 RefitCamp 複用）、`SquadTemplateLibrary`（LoadDir 同 id keep-latest 除重、空目錄守衛、Find/SortedByCost 指標生命週期已註明）、`BudgetedBuild`（`budget<=0` 皆無上限）。
- `Gameplay/RefitCamp.h/.cpp`：`Deploy` 增 `library` 參數，招募單位回補模板 stats——唯一實際消費路徑不再丟屬性。
- `Gameplay/BattleController.h`：`CreateSquadFromTemplate` 哨兵差異已文件化（`budget<0`=無限 vs `BudgetedBuild` 的 `budget<=0`，逐隊活錢包 vs 願望清單篩選，刻意不同）。
- `assets/templates/*.json`：遷移至巢狀 `stats{}` 布局（原扁平欄位會靜默解析成全預設）；morale 欄位廢棄已註記。
- `Examples/DuanqiaoPlayable.cpp`：recruit library 載入 `squads` + `templates` 兩目錄，Deploy 傳 library。
- `Examples/SquadTemplateTest.cpp`：98 checks——含數值消毒、重置語義、匿名拒絕、LoadFromFile 直接/逐層容錯、Add/Clear、雙 API 哨兵差異、遷移資產驗證。

## Review Dispositions

- **patch（已修）**：budget 哨兵矛盾、stats 負值/溢位未消毒、members/cost double→int 溢位、空 id 匿名隊、靜默夾值無警告、LoadFromString 失敗殘留、LoadDir 空字串/重複載入、舊扁平模板靜默失值、RefitCamp::Deploy 丟 stats、CreateSquadFromTemplate 失測、LoadFromFile/Add/Clear/負預算測試缺口。
- **defer**：`BudgetedBuildResult::skipped` 不區分「查無模板/超支」原因——屬 API 增強，spec 合約已滿足，需要時加 skipReasons。
- **reject**：「警告非 user-visible」——warnings 向量即機器可讀警告面，合約已足；`Instantiate` nullptr 路徑不可達——屬防禦設計；LoadFromFile 近端壞檔遮蔽上層好檔——沿用 BattleMap 既有 ../ 容錯慣例。

## Residual Risks

- 遷移後 `assets/templates/` 與 `assets/squads/` 兩目錄並存於 DuanqiaoPlayable recruit library——同 id keep-latest 已防衝突，長期可考慮整併目錄。
- morale 欄位自 schema 廢除——舊卡面資料含 morale 者被忽略（v1 刻意不收錄）。
