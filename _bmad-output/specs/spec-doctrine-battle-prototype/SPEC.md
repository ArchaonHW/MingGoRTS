---
id: SPEC-doctrine-battle-prototype
companions:
  - ../../brainstorming-session-2026-09-16.md
sources: []
---

> **Canonical contract.** This SPEC and the files in `companions:` are the complete, preservation-validated contract for what to build, test, and validate. Source documents listed in frontmatter are for traceability — consult them only if you need narrative rationale or prose color this contract intentionally omits.

# Doctrine Battle Prototype (斷橋)

## Why

**Opportunity + vision validation.** The 2026-09-16 brainstorming session produced a hybrid battle design — turn-layer planning × real-time execution — whose core hypothesis is unproven: the player's verb is *writing* doctrine (FF12-Gambit-style scripts), not micro-ing units. The "Broken Bridge" prototype exists to test whether the write → watch → fix loop is fun and whether doctrine rules are readable/exploitable, before any strategy-layer investment. Affected party: the game itself — this spec governs the battle-simulation core only.

## Capabilities

- **CAP-1**
  - **intent:** Deployment phase lets each squad be assigned a priority-ordered doctrine rule set (trigger + action + threshold); rule count is the command-slot cost.
  - **success:** `DoctrineSet::Evaluate` returns the first matching rule's action by priority and falls back to `HoldPosition` when nothing matches.
- **CAP-2**
  - **intent:** Execution phase auto-runs doctrine: every 0.25s game-time the controller rebuilds each squad's `SquadContext` (health, morale, nearest-enemy distance, under-attack, outnumbered, ally-engaged, objective-reached) and issues the evaluated `SquadOrder`.
  - **success:** After `BeginExecution()`, an unattended battle progresses and reaches `Resolution` inside the demo's tick budget.
- **CAP-3**
  - **intent:** Player can spend Command Points to override one squad's order for a limited window (firefighting micro).
  - **success:** `Intervene` consumes 1 CP, holds the override for `holdSeconds` of game time, then control returns to doctrine; returns `false` when CP is insufficient or phase is wrong.
- **CAP-4**
  - **intent:** Squads are the minimal unit: member count is strength, casualties drain morale proportionally, zero morale triggers uncontrolled routing, morale regenerates while disengaged.
  - **success:** `ApplyCasualties` reduces members and morale; `IsRouting()` is true at zero morale and the squad ignores orders; `RecoverMorale` only ticks while not engaged.
- **CAP-5**
  - **intent:** Squad-level flow-field pathfinding: reverse-Dijkstra integration field from the goal, obstacles and terrain costs, shared field with O(1) direction lookup.
  - **success:** In the demo, a wall with a single gap lets squads path through the gap to the objective (`AdvanceToObjective` produces movement, not stalls).
- **CAP-6**
  - **intent:** Three-phase loop Deployment → Execution → Resolution with outcome adjudication (enemy annihilated/routed = Victory, inverse = Defeat, time cap = Draw).
  - **success:** The demo reports a non-`Ongoing` `BattleOutcome` within 300s of game time.
- **CAP-7**
  - **intent:** Battle events (doctrine hits, routs, outcome) are emitted via a `std::function` callback — no EventBus dependency; upper layers may bridge it themselves.
  - **success:** `SetEventCallback` receives human-readable events during the demo run.
- **CAP-8**
  - **intent:** Each doctrine rule can carry a cooldown: once it fires it is skipped for N seconds of game time, preventing rule oscillation (decided 2026-09-17).
  - **success:** A rule in cooldown does not participate in evaluation; after cooldown elapses it can fire again. Default `0` = no cooldown (backward compatible). Implemented in `Gameplay/Doctrine.*` (`cooldown`/`coolingUntil` on `DoctrineRule`, `SquadContext::now`); verified by 3 assertions in `DoctrineBattleDemo`.
- **CAP-9** (T-4)
  - **intent:** Battlefield is data-driven: a `potato.battle_map/1` JSON defines grid, terrain cost/blocked patches, named zones, fords (holes in blocked water), per-team deploy zones, pins, and interactables.
  - **success:** `BattleMap::LoadFromFile` parses `assets/maps/duanqiao.json`; `ApplyToField` blocks the river and opens the ford; `IsInDeployZone` accepts north-bank positions for team 0 and rejects south-bank ones.
- **CAP-10** (T-5)
  - **intent:** The enemy is a named general with a readable 3-axis personality (aggression/discipline/cunning) and handwritten doctrine cards; the same schema as `assets/cards/` character cards (`signatureDoctrine` + `cards[]`).
  - **success:** `EnemyGeneral::MakeGlock()` (侵略90/紀律40/狡詐10, 4 cards) produces doctrine sets for all enemy squads; `LoadFromFile` accepts ROC character cards; personality measurably shifts thresholds (aggression lowers the retreat line).
- **CAP-11** (T-6)
  - **intent:** Battle resources: intel points are spent to reveal enemy information; command points cap interventions; per-squad morale below 30% drops doctrine execution to a 70% dice roll (orders can be disobeyed).
  - **success:** `BattleResources::SpendIntel`/`RevealEnemyPersonality` debit the pool; `SetMoraleExecution(0.30, 0.70)` produces visible "ignored orders (low morale)" events in `DuanqiaoDemo`.
- **CAP-12** (T-7)
  - **intent:** Battle events are recorded with game-time timestamps to `potato.battle_replay/1` JSON — replay is event playback, not re-simulation (avoids float nondeterminism).
  - **success:** `BattleRecorder::Attach` captures every emitted event; `SaveToFile`/`LoadFromFile` round-trip preserves count and order; `DuanqiaoDemo` writes a non-empty replay.
- **CAP-13** (T-8)
  - **intent:** Captain-rank-and-above units are enrolled in a named roster with relics; squad annihilation marks the captain KIA with a timestamp; the roster serializes to `potato.roster/1`.
  - **success:** `Roster::Enroll`/`Update`/`SaveToFile` produce a valid roster file. Note: rout-before-annihilation means KIA only fires on full elimination — pursuit-of-routing-squads is a follow-up need.

## Constraints

- The `Gameplay` layer is self-contained: no new third-party dependencies, no hard EventBus binding; squads are the smallest controllable unit (no per-agent pathfinding) — matches the engine's no-navmesh reality.
- Must compile under both MSVC and MinGW; no banned C functions; no edits under `external/` or `build/`.
- Headless-verifiable: `DoctrineBattleDemo` is a CLI target with assertions, registered in ctest (`POTATO_TESTS`).
- Doctrine evaluation is throttled to 0.25s game-time per squad — the balance point between perf and "script executes" feel, not per-frame.

## Non-goals

- Strategy layer: world map, diplomacy, progression, multi-general friction, chained timeline cards, enemy learning, sandbox simulator, numeric growth — all phase-2 ammunition from the brainstorm.
- ~~Intel-point economy, permadeath roster~~ — both implemented 2026-09-17 (CAP-11, CAP-13). Still out: rear-guard turns, pursuit mechanics for routing squads.
- Battle UI / ImGui panels — the prototype is CLI-only (T-9~T-11 track this).
- Unit-type action vocab (spear-wall / volley / charge bound to troop type) — prototype uses the generic 6-action set.
- Command-slot cap (4 vs 5) and "slot debt" overload delay — decided 2026-09-17: no slot enforcement in the prototype; rule count is only an implicit cost. Deferred to phase 2.
- Engine-layer changes riding in the same worktree (Physics/Rendering/Scene fixes, `MathUtils/Frustum.h`, `Physics/CollisionDetection.h`, PhysicsTest/MathTest additions) — concurrent hardening governed by repo policy and their own tests, not this spec.

## Success signal

`DoctrineBattleDemo` (a ctest case) resolves a battle within the time cap, the CP intervention is accepted and decrements the pool, and the three player squads show differentiated doctrine behavior (attacker / supporter / reserve) — the write → watch → evaluate loop is demonstrable headlessly.

## Assumptions

- Morale is per-squad (the brainstorm's "shared vs per-squad" open question is resolved per-squad in the prototype, preserving local-rout drama; pending playtest).
- Enemy uses the same `DoctrineSet` mechanism as the player — enemy personality / signature habits are expressed as rule content, not bespoke AI.
