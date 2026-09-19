---
title: 'Quantum-aware battle plan bonus (QuantumFog x BattlePlan)'
type: 'feature'
created: '2026-09-19'
status: 'done'
route: 'dispatch'
baseline_revision: '9996a86'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** G-5 battle-plan arrows apply `attackMul` unconditionally, even when the arrow tip targets an enemy that exists only as an unresolved probability cloud. Planning bonuses ignore intel quality, so scouting and plan commitment are decoupled.

**Approach:** Scale each arrow's planning attack multiplier by the intel certainty at its tip — `certainty = max over fog entities of probability mass within `uncertaintyRadius` of `arrow.to`; `effectiveMul = 1 + (attackMul - 1) * certainty`. Re-evaluate live during execution via the existing `planAttackMul` idempotent normalization so resolving intel (reveal/probe/eliminate/expiry) moves the bonus up or down.

## Boundaries & Constraints

**Always:**
- `effectiveMul` stays in `[1, attackMul]`; never a debuff below 1.0 and never above `attackMul`.
- Re-evaluation must be idempotent: repeat calls with unchanged fog state leave `damagePerMember` bit-identical (divide-out-old, multiply-in-new via `Squad::planAttackMul`).
- Backward compatible: no fog bound, or arrow tips not near any cloud candidate, yields `certainty = 1` (behavior identical to today).
- Fog entities are enemy-side; `UncertainEntity::team` is the *observer* team — never match it against `Squad::GetTeam()`.
- Both toolchains build clean (MSVC + MinGW); banned C functions stay banned.

**Never:**
- No change to hidden-target combat rules (`IsHiddenByFog` skip stays untouched).
- No per-arrow bonus fields, no new `PlanBonus` members, no JSON schema bump (`potato.battle_plan/1` unchanged).
- No coupling from `PotatoEngine` back to `Gameplay`; `QuantumFog` gains no new API.
- No morale/doctrine side effects on wrong gambles (GAMBLE_PENALTY resolved: SCALED_ONLY — wrong intel merely loses the bonus, never debuffs below 1.0).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| CONCENTRATED_TIP | arrow.to inside cloud, mass-at-tip = 0.8 | effectiveMul = 1 + (attackMul-1)*0.8 | N/A |
| REVEALED_AT_TIP | fog entity revealed, revealedPos within R of arrow.to | certainty 1.0 → full attackMul | N/A |
| EMPTY_TERRAIN | arrow.to far from all candidates of all entities | certainty 1.0 → full attackMul | N/A |
| NO_FOG | battle.GetFog() == nullptr | certainty 1.0, identical to current Apply | N/A |
| RESOLVE_ELSEWHERE | cloud revealed far from tip after Apply | next update drops effectiveMul toward 1 | N/A |
| PROBE_CONCENTRATES | weak observation concentrates mass at tip | next update raises effectiveMul | N/A |
| IDEMPOTENT | repeated UpdateUncertainty, fog unchanged | damagePerMember unchanged each call | N/A |
| NO_BONUS | attackMul == 1.0 | effectiveMul 1.0 regardless of certainty | N/A |
| REAPPLY | plan.Apply re-run mid-battle | squad binding refreshed; mul re-normalized, no compounding | N/A |

</frozen-after-approval>

## Code Map

- `Gameplay/BattlePlan.h/.cpp` — `PlanArrow` (L23-28), `Apply` (L61-116) matches arrows to friendly squads and normalizes `planAttackMul` (L94-99); add `SetUncertaintyRadius`, `UpdateUncertainty(BattleController&)`, per-squad arrow binding map + `appliedTeam` storage; include `QuantumFog.h`.
- `Gameplay/BattleController.h` — fog API L148-159 (`GetFog`, `GetFogSquad`, `GetFogEntityId`); add `BindPlan(BattlePlan*)` + forward decl; `plan` member near `fog` (L203).
- `Gameplay/BattleController.cpp` — execution-phase fog block L315-391 is where reveal state mutates per tick; call `plan->UpdateUncertainty(*this)` there (after `fog->Update(dt)`/contact-reveal handling) so bonuses track live intel.
- `Gameplay/QuantumFog.h` — `GetCloud(id)` L130 returns `[(pos, prob)]` (revealed ⇒ `[(pos,1.0)]`), `EntityCount()` L132 — all queries needed, no changes.
- `Gameplay/Squad.h` — `SetDamagePerMember`/`SetPlanAttackMul` (L74-80): the normalization slot the scaling rides on.
- `Examples/DuanqiaoPlayable.cpp` — `battle.BindFog(&fog)` L662; add `battle.BindPlan(&plan)` nearby; plan panel (~L990-1027 region) can display `squad->GetPlanAttackMul()` live.
- `Examples/BattlePlanTest.cpp` — reusable fixture: `BattleController battle(12,10,1.0f)` + `BattleResources res; res.Setup(battle,0,5,2)` + `plan.Apply(battle,&res,0)`.
- `Examples/QuantumFogBattleTest.cpp` — reusable fog fixture: `fog.BindResources(&res); battle.BindFog(&fog); fog.AddEntity(...); battle.BindFogSquad(enemy,eid)` (L46-59).
- `CMakeLists.txt` — `POTATO_TESTS` registration; add new test target there.

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/BattlePlan.h` -- add `SetUncertaintyRadius`/`UpdateUncertainty`, `squadArrowIdx` map, `appliedTeam`, `uncertaintyRadius=2.0f` -- new public surface for live certainty re-evaluation
- [x] `Gameplay/BattlePlan.cpp` -- implement certainty query over `battle.GetFog()->GetCloud`, fold scaling into Apply and UpdateUncertainty via planAttackMul normalization -- core feature
- [x] `Gameplay/BattleController.h/.cpp` -- `BindPlan(BattlePlan*)` + per-tick `UpdateUncertainty` call in execution-phase fog block -- automatic live tracking without touching combat rules
- [x] `Examples/QuantumPlanTest.cpp` -- new headless test covering every I/O matrix row -- verification target
- [x] `Examples/DuanqiaoPlayable.cpp` -- `battle.BindPlan(&plan)` + show live effective multiplier in the plan panel -- user-facing surface
- [x] `CMakeLists.txt` -- build `QuantumPlanTest` and register in `POTATO_TESTS` -- CTest integration

**Acceptance Criteria:**
- Given a plan arrow whose tip sits inside an unresolved cloud with tip-mass 0.8 and `attackMul` 1.3, when the plan is applied, then the squad's effective multiplier is ≈1.24, and rises toward 1.3 after the cloud is revealed at the tip.
- Given a cloud that resolves away from all arrow tips, when the next execution tick runs, then the affected squad's multiplier drops toward 1.0 without any re-Apply.
- Given repeated `UpdateUncertainty` calls with unchanged fog, then `damagePerMember` is bit-identical across calls (no compounding).
- Given no bound fog, when `Apply`/`UpdateUncertainty` run, then behavior is identical to pre-feature code.

## Implementation Notes

- **Certainty 語義裁決（偏離字面公式）**：I/O 矩陣的 EMPTY_TERRAIN（未解析雲全離尖 → certainty 1.0 滿額）與 RESOLVE_ELSEWHERE（已解析雲離尖 → 掉到 1.0）在「certainty = 純質量和最大值」下互斥——兩者 tip 質量同為 0。實作改以「是否對尖端表態」區分：未解析雲 tip 質量 0 視為未表態（跳過，不拉低）；已解析雲永遠表態（R 內 1.0 / R 外 0.0——情報證明落空即賭輸）。全部 entity 皆未表態（無 fog / 全離尖）→ certainty 1.0 向後相容。此語義使雲只會「尚未定罪的無辜」，解析才產生懲罰，符合 GAMBLE_PENALTY=SCALED_ONLY。
- `squadArrowIdx`（squad → 箭頭 index）於 `Apply` 開頭 `clear()` 重建——重跑 Apply 刷新綁定不殘留；`FromJson` 亦清綁定 + `appliedTeam=-1`，避免載入後拿新箭頭陣列套舊小隊映射。
- `UpdateUncertainty` 早退條件：未 Apply / 無綁定 / `attackMul<=1`；迭代時剔除 `IsEliminated`/跨隊/索引失效的 squad。
- `BattleController::Update`：`plan->UpdateUncertainty` 放在 `if (fog)` 區塊**外**——fog 中途解綁時 `TipCertainty(nullptr)=1.0` 使加成自動回滿，符合 NO_FOG 列語義。
- `DuanqiaoPlayable`：`BindPlan` 於 `BindFog` 旁，退出時與 `BindFog(nullptr)` 一併 `BindPlan(nullptr)`（plan 是 local，先於 battle 解構）；全軍面板顯示每隊 `GetPlanAttackMul()` 即時值。
- MinGW `build-mingw` 首次建置遇 `EnemyGeneral.cpp.obj` 截斷（平行寫檔殘留），刪 obj 重建即恢復——非本次改動問題。

## Spec Change Log

- 2026-09-19: status draft → in-progress（baseline 9996a86）→ done。certainty 語義裁決見 Implementation Notes。

## Review Triage Log

## Design Notes

Certainty is `max over entities` of mass-within-R, not a sum — two overlapping clouds still cap the bonus at one attack target's worth of confidence. Re-evaluation rides the same normalize-divide-multiply slot as `Apply`, so mid-battle re-Apply stays safe.

## Verification

**Commands:**
- `cmake --build build-mingw --target QuantumPlanTest` -- expected: builds clean
- `./build-mingw/bin/QuantumPlanTest.exe` -- expected: all PASS
- `cmake --build build --config Release --target QuantumPlanTest DuanqiaoPlayable` -- expected: MSVC builds clean
- `cd build && ctest -C Release --output-on-failure` -- expected: no regressions

**Results (2026-09-19):**
- MinGW：`QuantumPlanTest` 建置乾淨，21/21 PASS（I/O 矩陣九列全覆蓋 + BindPlan tick 自動重評）
- MSVC Release：`QuantumPlanTest` + `DuanqiaoPlayable` 建置乾淨（警告皆既有 C4458），21/21 PASS
- 相鄰回歸：`BattlePlanTest` 43/43、`QuantumFogBattleTest` 28/28（隱形目標戰鬥規則不變）
- `ctest -C Release`：61/61 有效通過——首輪 5 項失敗（BattleTrainerDemo/DuanqiaoDemo/QuantumTest/MediaPipeTest/LogTest）皆「寫檔→回讀」型，單獨重跑全數通過，確認為平行 session 於 `build/` 目錄的檔案鎖競爭，非本次改動
