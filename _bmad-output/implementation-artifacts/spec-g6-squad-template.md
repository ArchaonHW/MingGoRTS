---
title: 'G-6 小隊編制模板（HOI4 division designer）'
type: 'feature'
created: '2026-09-18'
status: 'draft'
baseline_commit: '2977e75'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: G-6)
---

## Objective

Squad 不再只能手刻參數——JSON 模板定義編制（人數/兵種/火力/速度/
接戰距離/成本），編成階段用模板建隊，預算限制下配兵。HOI4
師級設計的 squad 粒度版本。

## Approach

**1. `Gameplay/SquadTemplate.h/.cpp`（新）**

schema `potato.squad_template/1`：
```json
{
  "schema": "potato.squad_template/1",
  "id": "rifle_company",
  "name": "步兵連",
  "members": 20,
  "unit_class": "infantry",
  "damage_per_member": 0.05,
  "speed": 2.0,
  "engage_range": 1.5,
  "morale": 0.8,
  "cost": 100
}
```

- `LoadFromString/LoadFromFile`（JsonParser，沿用 BattleMap 慣例；
  逐層 `../` 容錯）；欄位缺省 = Squad 預設值，未知 `unit_class` → Infantry
- `cost` 缺省 0（免費模板合法）
- `LoadAll(json)`：JSON 陣列 → `std::vector<SquadTemplate>`（一個檔可放
  整個師的編制表，對齊 HOI4 多營概念但保持 squad 粒度）
- `Instantiate(BattleController&, team, pos, nameOverride="")` → `Squad*`：
  CreateSquad 後逐欄位 stamp（members 走建構參數，其餘 Set*）

**2. 預算配兵（寫死規則：超支拒絕）**

- `SquadTemplate::Budgeted` 包裝：`Instantiate` 前檢查
  `remaining >= cost`，不足 → 回 nullptr 且 Emit 拒絕訊息
- API 形狀：`BattleController::CreateSquadFromTemplate(tpl, team, pos,
  budget)` 內部扣帳；budget < 0 視為無限（向後相容的零成本路徑）
- 不降規——拒絕理由寫進 Emit（缺多少），玩家自行換模板

**3. 範例資產** `assets/templates/infantry_company.json` +
`cavalry_platoon.json` 各一份（給測試與 demo 吃真檔）

**4. `Examples/SquadTemplateTest.cpp`** 入 POTATO_TESTS：
解析/缺省值/未知兵種/陣列載入/Instantiate 欄位逐項對驗/
預算足→成隊+扣帳、不足→nullptr+預算不動

## Out of scope

- 編成階段 UI（PlanningDeck 之後再串；本項只交付邏輯層 + 資產）
- 模板繼承/加成疊加、人員素質分級、裝備槽
- 與 G-9 整補循環的成本聯動（G-9 自行決定是否吃 cost 欄位）

## Acceptance

- [ ] 模板檔載入 → Instantiate 建出欄位全對的 squad
- [ ] 預算不足拒絕建隊且不扣帳；足額扣帳成隊
- [ ] 陣列檔可載入多模板；既有測試無回歸（MSVC + MinGW）

## Files

- new: `Gameplay/SquadTemplate.h`, `Gameplay/SquadTemplate.cpp`,
  `Examples/SquadTemplateTest.cpp`, `assets/templates/*.json` ×2
- edit: `Gameplay/BattleController.h/.cpp`（CreateSquadFromTemplate）、
  `CMakeLists.txt`（lib source + test target + POTATO_TESTS）
