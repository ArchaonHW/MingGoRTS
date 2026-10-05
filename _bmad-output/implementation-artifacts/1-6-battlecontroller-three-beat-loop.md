---
baseline_commit: e889739e
---

# Story 1.6: BattleController Three-Beat Loop

Status: done

## Story

As a player,
I want a Planning→Execution→Aftermath state machine on a fixed 20 Hz tick,
So that battles have defined phases.

## Acceptance Criteria

1. **Given** a BattleState,
   **When** driven headless,
   **Then** Planning accepts input without ticking, Execution ticks deterministically, Aftermath emits results
   **And** illegal transitions (Planning→Aftermath) are rejected by the transition table.

## Tasks / Subtasks

- [x] Task 1 — Beat state machine (AC: 1)
  - [x] `Gameplay/Sim/BattleController.h` — `enum class BattleBeat { Planning, Execution, Aftermath }` + `constexpr bool CanTransition(BattleBeat, BattleBeat)` (the table is the spec; P→E, E→A only)
  - [x] `RequestBeat(target)` → bool — table-gated; illegal transitions rejected without side effects
- [x] Task 2 — Execution tick orchestration (AC: 1)
  - [x] `BattleController` owns: `Sim` (seed → tick + checksum + sole `Prng`), `const BattleMap&`, `std::vector<Squad>` live state, `std::vector<SquadSheet>`, `const DoctrineLibrary&`, `int cpPool` (start 3 per FR3; regen/spend is Story 1.7), `std::vector<SimEvent>` battle log
  - [x] `Tick()` — Execution only; per tick: EvalTick(map, squads, sheets, cards, sim rng, cpPool, tick) → ApplyDeltas → `TickMove()` per squad → append events to log → `sim.Tick()` (stream draw + checksum)
  - [x] Deterministic battle `Checksum()` — fold numeric squad fields + tick into the sim checksum (hash field values, not object memory — Squad is non-POD, see deferred-work)
- [x] Task 3 — Planning input surface (AC: 1)
  - [x] `DeploySquad(template, regionIndex, side)` — Planning only; creates squad + empty-by-default sheet slot (sheets must be set via SetSheet before Execution? no — allow empty sheet vector entry = no cards)
  - [x] `SetSheet(squadIndex, SquadSheet)` — Planning only
  - [x] `Tick()` outside Execution is a no-op returning false
- [x] Task 4 — Aftermath results (AC: 1)
  - [x] Auto-transition Execution→Aftermath when one side has zero `IsEffective()` squads (checked post-deltas each tick)
  - [x] `BattleResult` {winnerSide (-1 draw), elapsedTicks, per-side effective/routed/destroyed counts} — emitted once on entry to Aftermath; `Result()` accessor
  - [x] Ticks in Aftermath no-op
- [x] Task 5 — Headless test `potato_test_battle` (AC: 1)
  - [x] Transition table: P→E ok, E→A ok, P→A rejected, A→anything rejected
  - [x] Planning: Tick() no-op, deploy/set-sheet accepted; Execution: deploy/set-sheet rejected, Tick() advances
  - [x] Determinism: two identical (seed, squads, sheets) runs → identical Checksum + event stream
  - [x] Auto-Aftermath: rout all enemies → beat flips, result correct; ticks no-op after
- [x] Task 6 — Verify: MinGW junction build; ctest 7/7; dep guard green; MSVC disclosed

## Dev Notes

### Prior story intelligence

- `Prng` lives in `Sim.h` — `Sim` owns the stream; add `Prng& Rng()` accessor (non-const) for the controller; do NOT construct a second Prng in the controller.
- `Sim::Tick()` currently draws + folds into checksum — call it once per controller tick; document draw order: doctrine eval (0 draws in vocab v0) → housekeeping draw.
- `Doctrine::EvalTick(map, snapshot const&, sheets&, cards, rng&, cpPool, tick)` — snapshot can be the live `squads` vector (EvalTick is const-pure on it).
- `Squad::TickMove()` advances edge traversal; `IsEffective()` excludes Routed/Destroyed; `ApplyEvent`/`ApplyHit` exist for future combat.
- `BattleMap::Neighbors`/`RegionCount` used by EvalTick — controller keeps a `const BattleMap&` (owner may be caller).
- Beat transitions are log-worthy boundaries (arch: "log at boundaries: beat transitions") — but Gameplay has no logger wired (engine leaf unlinked); emit a `BeatChanged` SimEvent into the battle log instead — deterministic and replayable.

### Design decisions (this story)

- **`BattleController` in `Gameplay/Sim/`** — arch assigns "BattleController / tick kernel" to `Gameplay/Sim`.
- **Deploy in Planning only** — AC says Planning "accepts input"; post-Execution deployment is reinforcement mechanics, out of scope.
- **Auto-Aftermath on side wipe** is the minimal loop-closer; full win evaluation (rout thresholds, objectives) is Story 1.12.
- **`cpPool` is a plain int field** (default 3) — economy (regen, intervention costs) is Story 1.7; it's needed now only as EvalTick input.
- **Checksum**: `sim.Checksum()` XOR-folded with per-squad numeric fields each tick — proves end-to-end determinism (events already proven identical streams; this covers state).

### LLM-trap warnings

- Do NOT let Tick() run in Planning — AC explicitly: Planning ticks nothing.
- Do NOT create a second Prng — one stream owned by Sim.
- Do NOT emit `BeatChanged` to an engine EventBus — boundary events are a Game-shell concern; the SimEvent log is the truth.
- `Squads`/`sheets` must stay index-aligned — `DeploySquad` pushes to both.
- Auto-Aftermath check runs AFTER ApplyDeltas + TickMove within the same tick (a rout arriving via tick must close the battle that tick).

### References

- [Source: epics.md — Story 1.6]
- [Source: game-architecture.md — BattleBeat transition table (verbatim), fixed-tick model, canonical eval, log-at-boundaries]
- [Source: gdd.md — FR2 three-beat (Planning untimed/paused, Execution 5–10 min, Aftermath), FR3 CP start 3]

## Review Findings (three-layer code review, 2026-09-29)

7 patch / 4 defer / 0 decision_needed.

- [x] `DeploySquad` zero validation (all 3 layers) → `regionIndex >= RegionCount` (covers NO_REGION phantoms) + `side ∈ {0,1}` rejects — phantoms previously counted as effective, invisible to enemies, self-buffing
- [x] `Checksum()` omitted mutable state → folds `maxHp`/`speedMilli`/squad count/`routTimers`/per-slot `{cardIndex, cooldownRemaining}`; `NO_REGION` sentinel normalized for 32/64-bit portability
- [x] `BeatChanged` tick-stamp inconsistency (auto-close stamped N+1 vs CardFired N) → wipe path stamps the producing tick via `EmitBeatChanged`; convention documented
- [x] Routing squads were unresolvable zombies — all-Routing side never closed the battle → controller drives `RetreatComplete` after `ROUT_TICKS` (2 s); a rout now closes the battle the same tick it completes
- [x] Manual E→A vs mutual-wipe both yielded `winnerSide=-1` → `BattleOutcome::forced` flag
- [x] Incomplete-`Squad` member hazard → `~BattleController()` + copy ctor declared in header, defaulted in cpp; copy-assign deleted (ref members)
- [x] Docs/includes: `map_`/`cards_` lifetime contract, `SimEvent::aux` wire-stable ordinals, `<utility>` include, dropped unused `Result.h`

Deferred (deferred-work.md): squad id uniqueness (template ids legitimately shared — battle-scoped renaming is a roster concern), cpPool cap (Story 1.7 economy), manual-E→A-as-concede semantics note.

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

### Completion Notes List

- `Gameplay/Sim/BattleController.{h,cpp}` — `BattleBeat` + constexpr `CanTransition` (arch-verbatim table); `RequestBeat` table-gated, emits `BeatChanged` SimEvent into the append-only battle log.
- Execution tick order: EvalTick -> ApplyDeltas -> TickMove -> append events -> sim.Tick (housekeeping draw) -> side-wipe check auto-closes to Aftermath with `BattleOutcome` (winner/counts/elapsed ticks).
- Planning: `DeploySquad`/`SetSheet`/`SetCpPool` accepted, `Tick` no-op; Execution/Aftermath reject deploy.
- `Sim::Rng()` accessor added — single stream stays Sim-owned; `SimEvent` gains `BeatChanged` kind + `aux` field.
- `Checksum()` folds sim checksum + tick + beat + per-squad numeric fields (strings excluded — Squad non-POD note honored).
- MinGW verify: 34/34 battle checks, ctest 7/7 green, dep guard green. MSVC unverified.
- Post-review verify: 43 battle checks, ctest 7/7 green (MinGW).

### File List

- Gameplay/Sim/BattleController.h (new)
- Gameplay/Sim/BattleController.cpp (new)
- Gameplay/Sim/Sim.h (modified — Rng() accessor)
- Gameplay/Doctrine/Doctrine.h/.cpp (modified — BeatChanged kind + aux field)
- Examples/potato_test_battle.cpp (new)
- CMakeLists.txt (modified — potato_test_battle target/add_test)
