---
title: 'G-5 作戰計畫箭頭（HOI4 battle plan → doctrine）'
type: 'feature'
created: '2026-09-18'
status: 'in-progress'
route: 'dispatch'
review_loop_iteration: 0
baseline_commit: '65a4227'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** The planning layer can only pin one team-wide objective; there is no way to give each squad its own attack axis like HOI4 battle-plan arrows, and no planning bonus rewarding a prepared plan.

**Approach:** A `BattlePlan` module holds per-squad `PlanArrow`s (from→to axis + priority), a rally point, and a plan bonus. Each arrow translates into a `DoctrineSet` (retreat-if-hurt → attack-on-arrival → advance-along-axis); `Apply` assigns doctrines, per-squad objectives, and credits intel/CP via `BattleResources`. A minimal planning-phase interaction in `DuanqiaoPlayable` lets the player draw an arrow for a squad; pressing 開戰 applies the plan.

## Boundaries & Constraints

**Always:**
- `potato.battle_plan/1` and `potato.doctrine_set/1` JSON schemas via the in-repo `JsonValue` parser — no third-party JSON.
- Squad-named arrows take precedence; a nameless arrow is the generic fallback; squads with no applicable arrow keep their existing doctrine.
- Plan bonus only applies when at least one squad was assigned (no free intel/CP for an unused plan).
- All logic headless-testable; demo additions follow the existing DuanqiaoPlayable conventions.

**Never:**
- No front-line/frontage solver, no automatic arrow generation, no enemy-side plans.
- No mid-execution plan editing (arrows bake into doctrine at 開戰).
- No new dependencies, no changes under `external/`.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Named arrow | `AddArrow("前鋒營", from, to)` + Apply | 前鋒營 gets 3-rule DoctrineSet + `SetSquadObjective(to)` | N/A |
| Generic arrow | nameless arrow only | All same-team squads get the fallback doctrine | N/A |
| No matching arrow | squad absent from plan, no generic | `Apply` returns 0 for it; existing doctrine untouched | N/A |
| Plan bonus | `SetPlanBonus(1.15, 2, 1)` + Apply | damagePerMember ×1.15; intel +2; CP +1 in resources AND controller | Negative/zero values clamp to 1.0/0 |
| Serialization | `ToJson` → `FromJson` | Round-trip preserves arrows, rally, bonus | Bad schema/malformed JSON → `false`, state unchanged |

</frozen-after-approval>

## Code Map

- `Gameplay/BattlePlan.h` / `Gameplay/BattlePlan.cpp` — NEW (already drafted in working tree): `PlanArrow`, `AddArrow/SetRallyPoint/SetPlanBonus`, `ArrowToDoctrine` (pure), `Apply(battle, res, team)`, `ToJson/FromJson`. Keep this design.
- `Examples/BattlePlanTest.cpp` — NEW (already drafted): 7 sections covering translation, both serializations, Apply semantics, execution-layer movement, no-arrow passthrough, bonus clamping. Register it.
- `Gameplay/BattleController.h/.cpp` — `SetSquadObjective/ClearSquadObjective` + `squadObjectives` map already exist; per-squad objective already wins over team objective in `UpdateContexts`/`AdvanceToObjective`. Do not change.
- `Gameplay/Doctrine.h/.cpp` — `ToJson/FromJson/TriggerFromName/ActionFromName` already exist (potato.doctrine_set/1). Reuse.
- `Gameplay/BattleResources.h` — `AddIntel(team,n)` / `AddCP(battle,team,n)` exist; `Apply` credits through them.
- `CMakeLists.txt` — add `Gameplay/BattlePlan.cpp` to the Gameplay lib; add `BattlePlanTest` exe + `POTATO_TESTS` entry.
- `Examples/DuanqiaoPlayable.cpp` — planning phase (`planningPhase`, Alt+drag pins at ~line 700, 開戰 button ~line 1020). Add minimal arrow drawing: in planning phase, hold a modifier (e.g. Ctrl) and LMB-drag from a friendly squad to a target cell → `plan.AddArrow(squad->GetName(), squad->GetPosition(), target)`; draw arrows as line/scene markers; 開戰 calls `plan.Apply(battle, &res, 0)` before `BeginExecution`. Keep the block small — the file is a parallel-work hotspot.

## Tasks & Acceptance

**Execution:**
- [ ] `CMakeLists.txt` — register `Gameplay/BattlePlan.cpp` in the Gameplay library; add `BattlePlanTest` target and `POTATO_TESTS` entry.
- [ ] `Examples/BattlePlanTest.cpp` — verify existing draft compiles and passes; extend only if a matrix row lacks coverage.
- [ ] `Examples/DuanqiaoPlayable.cpp` — planning-phase arrow drawing (modifier+drag), arrow visualization (scene-node lines or ImGui overlay), `plan.Apply` on 開戰, arrow list shown in the planning panel.
- [ ] `Gameplay/BattlePlan.cpp` — fix any defects found while wiring/verifying (e.g. `Apply` re-run double-multiplying attackMul, FromJson leaving stale `hasRally` when JSON lacks the field).

**Acceptance Criteria:**
- Given a plan with a named arrow, when `Apply` runs, then that squad receives the translated DoctrineSet (serializable via `potato.doctrine_set/1`) and advances toward the arrow target during execution.
- Given plan bonus (attackMul/intel/CP), when `Apply` assigns ≥1 squad, then damage and resources reflect the bonus; when 0 squads assigned, nothing is credited.
- Given the planning phase of DuanqiaoPlayable, when the player draws an arrow for a squad and presses 開戰, then the plan applies and the squad follows its arrow.
- BattlePlanTest passes; MSVC + MinGW build clean; no regressions in existing ctest entries.

## Implementation Notes

## Spec Change Log

## Review Triage Log

## Design Notes

ArrowToDoctrine emits three rules per arrow (survival first, engage-on-arrival, axis advance). `Apply` resolves arrows per squad: exact name match wins; first nameless arrow is the fallback. `SetSquadObjective` (G-5 groundwork already in BattleController) makes `ObjectiveReached`/`AdvanceToObjective` evaluate per-squad instead of the team pin.

## Verification

**Commands:**
- `cmake --build build --config Release --target BattlePlanTest DuanqiaoTutorial` — expected: 0 errors
- `cd build && ctest -C Release -R BattlePlanTest --output-on-failure` — expected: PASS
- `cmake --build build-mingw --target BattlePlanTest && build-mingw/bin/BattlePlanTest.exe` — expected: all checks PASS

**Manual checks:**
- `DuanqiaoPlayable.exe`: in planning phase draw an arrow for a squad, press 開戰, confirm the squad advances along the arrow and the plan panel lists it.
