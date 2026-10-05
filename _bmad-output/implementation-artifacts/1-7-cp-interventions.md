---
baseline_commit: b71f7bbb
---

# Story 1.7: CP Interventions

Status: done

## Story

As a player,
I want to spend Command Points mid-execution to redirect squads, override cards, or order retreats,
So that I can save a collapsing plan.

## Acceptance Criteria

1. **Given** Execution running with CP pool (start 3, +1/60s, cap 5),
   **When** I issue redirect (1) / override (2) / retreat (3),
   **Then** the intervention resolves within ≤3s of issue and CP deducts
   **And** insufficient CP rejects the command with a reason.

## Tasks / Subtasks

- [x] Task 1 — Command vocabulary (AC: 1)
  - [x] `Gameplay/Command/Intervention.h` — `enum class InterventionKind { Redirect, Override, Retreat }`, `constexpr int CostOf(kind)` → 1/2/3, `struct Intervention {side, kind, squadIndex, target, issueTick}`
  - [x] Constants: `CP_START=3`, `CP_CAP=5`, `CP_REGEN_TICKS=1200` (60s×20Hz)
- [x] Task 2 — Per-side pools + regen (AC: 1)
  - [x] `BattleController::cpPool_[2]` (replaces scalar — symmetric AI needs its own pool); `CpPool(side)` accessor
  - [x] Regen in Tick: `++cpRegen_` each Execution tick; at `CP_REGEN_TICKS` → `+1` to both sides, capped at `CP_CAP`
  - [x] `Doctrine::EvalTick` signature: `int cpPool` → `const std::array<int,2>&` — `cp_at_least` reads the acting squad's side pool
- [x] Task 3 — Issue + resolve (AC: 1)
  - [x] `IssueRedirect(side, squadIdx, region)` — Execution only; CP ≥1; squad alive, target adjacent & in-bounds; deducts at issue, queues
  - [x] `IssueOverride(side, squadIdx, slotIdx)` — 2 CP; squad/slot valid; sets `CardSlot::forceNext` — the slot fires next eval bypassing trigger+condition (action/modifier/cooldown still apply)
  - [x] `IssueRetreat(side, squadIdx)` — 3 CP; squad Holding/Moving → `ApplyEvent(CohesionBreak)` → Routing (completes via existing rout timer)
  - [x] All reject with a `Result`-style reason and no deduction on failure
  - [x] Queue applies at the START of next Tick (before EvalTick — interventions are part of tick-start state); `SimEvent{Kind::Intervention}` emitted at issue with `param` = target/slot
- [x] Task 4 — Test `potato_test_command` (AC: 1)
  - [x] Economy: start 3, +1 at tick 1200, cap 5, per-side pools
  - [x] Each intervention: CP deducts, effect lands at issue+1 tick (≤3s), event logged
  - [x] Rejects: insufficient CP (reason non-empty, pool untouched), Planning phase, bad targets (non-adjacent redirect, OOB squad/slot, terminal-squad retreat)
  - [x] Override fires a card whose trigger is unmet AND whose cooldown is active
- [x] Task 5 — Verify: MinGW ctest 8/8; dep guard; MSVC disclosed

## Dev Notes

### Prior story intelligence

- `CardSlot` needs `bool forceNext` — EvalTick checks it BEFORE trigger/condition gates; cleared on visit; still writes cooldown + event (it's a fire, not a free pass).
- `cpPool` was a shared int — per-side now; `EvalTick` param changes to `std::array<int,2>`; clamp `sq.side` to [0,1] for safety (deploy validates anyway).
- `Squad::ApplyEvent(CohesionBreak)` works from Holding/Moving → Routing; the controller's rout timer (Story 1.6, `ROUT_TICKS`) completes it to Routed — retreat "resolves" by leaving the field.
- `IssueMove` is Holding-gated — redirect on a Moving squad fails at apply; validate adjacency at ISSUE so rejection is immediate (controller owns map per Squad.h contract).
- SimEvent gets `Kind::Intervention` + `int param` field (target region for redirect, slot index for override, unused for retreat).

### Design decisions (this story)

- **Deduct at issue, apply next tick** — prevents double-spend within a tick; resolve latency = 1 tick ≪ 3s budget. Commands issued the same tick apply in canonical (issue) order at next tick start.
- **Override semantics** = force-fire: bypasses trigger+condition gates for one eval, cooldown still written (an override is costly, not free). Retargeting a card's action param is NOT in scope (card params are immutable content).
- **Retreat** = force-rout (CohesionBreak event) — matches the rout-is-sticky design; the squad leaves the field and can never return this battle.
- **Queue is per-controller**, not per-side — both sides' commands interleave by issue order (canonical).

### LLM-trap warnings

- Do NOT deduct CP inside the apply step — deduction at issue is the atomicity point.
- Do NOT let interventions apply mid-tick between EvalTick and ApplyDeltas — they're queued; tick-start application keeps snapshot semantics clean.
- `forceNext` must be cleared even if the squad can't act (else a stale flag fires later) — clear on slot visit.
- Pending command targets may die between issue and apply — re-validate liveness at apply (squad still effective), else skip.

### References

- [Source: epics.md — Story 1.7]
- [Source: gdd.md — FR3 CP economy, intervention costs, ~3s resolve]
- [Source: game-architecture.md — Gameplay/Command placement, pendingDelta rule, canonical order]

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

### Completion Notes List

- `Gameplay/Command/Intervention.h` — `InterventionKind` + `CostOf` (1/2/3) + `Intervention` struct; economy constants `CP_START=3`/`CP_CAP=5`/`CP_REGEN_TICKS=1200`.
- `BattleController`: `cpPool_` now `std::array<int,2>` per-side; regen +1/1200 ticks both sides capped 5; `pendingCommands_` queue applied at tick start (step 0, before EvalTick — interventions are tick-start state).
- `IssueRedirect/Override/Retreat` — Execution-only, `CheckCommand` validates beat/side/CP/squad liveness; deduct-at-issue; `SimEvent::Intervention` (aux=kind, param=target/slot) logged at issue.
- `EvalTick` signature: `int cpPool` -> `std::array<int,2>` per-side; `cp_at_least` reads acting side's pool.
- `CardSlot::forceNext` — override force-fire; cleared on slot visit even when squad can't act.
- Debug log: forced-flag refactor initially set `pass=forced` — killed all condition-less cards (Always -> break leaves pass false); fixed to `pass=true` + gate-skip on forced.
- MinGW verify: 31 command checks, ctest 8/8 green. MSVC unverified.

### File List

- Gameplay/Command/Intervention.h (new)
- Gameplay/Sim/BattleController.h/.cpp (modified — per-side pools, regen, command queue, Issue* APIs)
- Gameplay/Doctrine/Doctrine.h/.cpp (modified — per-side cpPools, forceNext, Intervention event kind)
- Examples/potato_test_command.cpp (new)
- Examples/potato_test_battle.cpp (modified — new CpPool signatures)
- Examples/potato_test_doctrine.cpp (modified — array cpPools)
- CMakeLists.txt (modified — potato_test_command target)
