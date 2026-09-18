---
title: 'G-9 戰鬥間整補循環（ELIN 基地層）'
type: 'feature'
created: '2026-09-18'
status: 'ready-for-dev'
baseline_commit: '65a4227'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: G-9)
  - spec: deps G-7(戰後流轉, done) / G-6(編制模板, done)
---

## Objective

戰鬥間的 meta 層：把 G-7 結算出的傷兵/戰利品/遺物變成跨場
持續的「營地狀態」——傷兵恢復、兵員招募（吃 G-6 模板）、
遺物配備、資源跨場累積。這是專案最大的缺口層。

## Approach

**1. `Gameplay/RefitCamp.h/.cpp`（新，純邏輯可測）**

跨場常備軍模型：
```cpp
struct VeteranUnit {
    std::string squadName;      // 沿用原隊名（敘事連續性）
    std::string templateId;     // 招募來源（"" = 初始單位）
    UnitClass unitClass;
    int members;                // 現有兵力（傷兵不算在內）
    int wounded;                // 傷兵池（可恢復）
    int maxMembers;             // 編制上限（招募時記錄）
    std::string captainName;    // 具名隊長（接 Roster）
    std::vector<std::string> relics; // 配備的遺物
};
```

`RefitCamp` API：
- `Absorb(report, roster)` — 戰後收口：陣亡隊移除（隊長已入冊），
  存活隊 members 記回、wounded 入池、拾獲遺物進庫存
- `HealWounded(loot)` — 每點戰利品回 1 名傷兵（寫死 1:1），
  wounded → members，不超 maxMembers；回傳實際恢復數
- `Recruit(library, templateId)` — 花 `SquadTemplate::cost` 戰利品
  建新 VeteranUnit（滿編入伍）；不足回 false
- `AssignRelic(unit, relic)` / `Inventory()` — 遺物配備/庫存
- `Deploy(battle, team, positions)` — 下一場：逐 unit CreateSquad
  （members 保留），回傳 Squad* 陣列供呼叫端 Enroll/Roster
- `SaveToFile/LoadFromFile` — `potato.refit_camp/1` JSON，
  loot 餘額也存這裡（camp 自持戰利品帳，不再借 BattleResources）

**2. `BattleResources::AddLoot` 已存在——流向改為：**
`PostBattle::ApplyLoot` 入帳 res → 戰後由呼叫端
`camp.DepositLoot(res.GetLoot(team))` 轉存營地。
（camp 是 meta 層的帳本；res 是單場的帳本，分層清楚。）

**3. `Examples/RefitCampTest.cpp`** 入 POTATO_TESTS：
吸收報告→傷兵池、HealWounded 扣帳恢復+上限、Recruit 扣帳建軍/
不足拒絕、Deploy 重建戰力保存、save/load roundtrip、遺物配備、
被殲滅單位消失且隊長入冊。

**4. DuanqiaoPlayable 最小接線**：Title 殼加「整補營」區塊
（loot 餘額 + 各隊兵力/傷兵 + 醫治全部/招募步兵連鈕），
戰後自動 DepositLoot + Absorb。檔案熱區，僅加一個區塊，
不動既有結構；衝突大則降為邏輯層交付 + TODO。

## Out of scope

- 多章節戰役地圖（E-7 範圍；本項只有單場間循環）
- 裝備屬性加成（relic 是標記物，不配數值）
- doctrine 卡解鎖樹（卡池解鎖留給 PlanningDeck 層串）
- 敵軍 meta 層（只整我軍）

## Acceptance

- [ ] 兩場戰鬥之間狀態跨場持續（RefitCamp 跨場存活 + 存檔）
- [ ] 招募/恢復消費戰利品（不足拒絕）
- [ ] 傷兵入池可恢復、陣亡單位移除
- [ ] RefitCampTest 全綠；MSVC + MinGW 建置；無回歸

## Files

- new: `Gameplay/RefitCamp.h`, `Gameplay/RefitCamp.cpp`,
  `Examples/RefitCampTest.cpp`
- edit: `Examples/DuanqiaoPlayable.cpp`（Title 整補區塊 +
  戰後 DepositLoot/Absorb）、`CMakeLists.txt`
