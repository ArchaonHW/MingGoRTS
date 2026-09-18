---
title: 'Quantum-aware battle plan bonus (QuantumFog x BattlePlan)'
type: 'feature'
created: '2026-09-19'
status: 'draft'
route: 'dispatch'
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
- No morale/doctrine side effects on wrong gambles (see Open Questions).

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

## Open Questions

- GAMBLE_PENALTY — options: SCALED_ONLY (wrong intel merely yields bonus ≈ 1; no downside beyond losing the bonus — simplest, zero new coupling) / RESOLVED_EMPTY_DEBUFF (if the nearest cloud resolves with no enemy within R of the tip, the squad takes a small morale hit — adds `AdjustMorale` coupling, harsher gambling feel)

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
- [ ] `Gameplay/BattlePlan.h` -- add `SetUncertaintyRadius`/`UpdateUncertainty`, `squadArrowIdx` map, `appliedTeam`, `uncertaintyRadius=2.0f` -- new public surface for live certainty re-evaluation
- [ ] `Gameplay/BattlePlan.cpp` -- implement certainty query over `battle.GetFog()->GetCloud`, fold scaling into Apply and UpdateUncertainty via planAttackMul normalization -- core feature
- [ ] `Gameplay/BattleController.h/.cpp` -- `BindPlan(BattlePlan*)` + per-tick `UpdateUncertainty` call in execution-phase fog block -- automatic live tracking without touching combat rules
- [ ] `Examples/QuantumPlanTest.cpp` -- new headless test covering every I/O matrix row -- verification target
- [ ] `Examples/DuanqiaoPlayable.cpp` -- `battle.BindPlan(&plan)` + show live effective multiplier in the plan panel -- user-facing surface
- [ ] `CMakeLists.txt` -- build `QuantumPlanTest` and register in `POTATO_TESTS` -- CTest integration

**Acceptance Criteria:**
- Given a plan arrow whose tip sits inside an unresolved cloud with tip-mass 0.8 and `attackMul` 1.3, when the plan is applied, then the squad's effective multiplier is ≈1.24, and rises toward 1.3 after the cloud is revealed at the tip.
- Given a cloud that resolves away from all arrow tips, when the next execution tick runs, then the affected squad's multiplier drops toward 1.0 without any re-Apply.
- Given repeated `UpdateUncertainty` calls with unchanged fog, then `damagePerMember` is bit-identical across calls (no compounding).
- Given no bound fog, when `Apply`/`UpdateUncertainty` run, then behavior is identical to pre-feature code.

## Implementation Notes

## Spec Change Log

## Review Triage Log

## Design Notes

Certainty is `max over entities` of mass-within-R, not a sum — two overlapping clouds still cap the bonus at one attack target's worth of confidence. Re-evaluation rides the same normalize-divide-multiply slot as `Apply`, so mid-battle re-Apply stays safe.

## Verification

**Commands:**
- `cmake --build build-mingw --target QuantumPlanTest` -- expected: builds clean
- `./build-mingw/bin/QuantumPlanTest.exe` -- expected: all PASS
- `cmake --build build --config Release --target QuantumPlanTest DuanqiaoPlayable` -- expected: MSVC builds clean
- `cd build && ctest -C Release --output-on-failure` -- expected: no regressions
