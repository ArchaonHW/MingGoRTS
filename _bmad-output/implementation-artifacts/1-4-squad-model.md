---
baseline_commit: NO_VCS
---

# Story 1.4: Squad Model

Status: done

### Review Findings

- [x] [Review][Patch] `ApplyHit` missing upper hp clamp — negative `hpLoss` overhealed past `maxHp`; negative args now clamp to 0, hp capped at maxHp [Squad.cpp]
- [x] [Review][Patch] `TicksForEdge(0)` integer divide-by-zero; `speedMilli<=0`→base, result floor 1 tick (no teleport) [Squad.h]
- [x] [Review][Patch] `RetreatComplete` unreachable — added public `Squad::ApplyEvent(SquadEvent)`; `TickMove`/`IssueMove`/`ApplyHit` all route through it (the table is the spec) [Squad.h/.cpp]
- [x] [Review][Patch] `Heal`/`RestoreCohesion` negative amounts bypassed the table (could kill/un-kill without events) — negative → early return; both no-op on Routed/Destroyed [Squad.cpp]
- [x] [Review][Patch] `ApplyHit` mutated terminal states (Routed+hp0 incoherence) — early return on Routed/Destroyed [Squad.cpp]
- [x] [Review][Patch] `IssueMove` accepted self-target/NO_REGION (sentinel committed into `regionIndex`) — rejected [Squad.cpp]
- [x] [Review][Patch] `Instantiate` trusted hand-built templates (hp≤0 zombie, cohesion<20 "Holding") — clamped fields; cohesion<ROUT_THRESHOLD spawns `Routing` [Squad.cpp]
- [x] [Review][Patch] `At()` unasserted; `~size_t{0}` sentinel duplicated; `<cassert>`/`<utility>`/`std::set` hygiene [Squad.h/.cpp]
- [x] [Review][Patch] test coverage +18 checks: sticky rout, terminal gating, ApplyEvent path, corrupt speedMilli, broken-spawn, missing unit/speed rejects
- [x] [Review][Defer] no rally path (`Routing`→`Holding` absent) — intentional: rout is morale collapse, sticky in-battle; recovery is RefitCamp's job. Documented in header.
- [x] [Review][Defer] `Squad` carries `std::string` members — not flat-POD; BattleState checksum (1.6+) must hash string contents, not memory. Noted for recorder story.
- [x] [Review][Defer] `artillery` "(ranged)" qualifier + counters column not expressible in `potato.squad/1` — defer to schema v2 when combat resolution needs it
- [x] [Review][Defer] invalid `SquadState` injection via public `state` field — accepted (serialization-owned writes land later)
- [x] [Review][Defer] missing `unit`/`speed` reports generic "must be a string token" — cosmetic
- [x] [Review][Defer] `IsEffective()` counts Routing — intended (still on-field)

## Story

As a developer,
I want named squads with stats, position/movement, and cohesion state on a rout threshold,
So that the battle has actors.

## Acceptance Criteria

1. **Given** squads instantiated from `potato.squad/1` templates,
   **When** the sim ticks,
   **Then** squads move per speed stat and cohesion updates from combat events.
2. **And** cohesion < 20% produces a rout transition in the state table.

## Tasks / Subtasks

- [x] Task 1 — Squad runtime model (AC: 1, 2)
  - [x] `Gameplay/Squad/Squad.h` — `enum class UnitType` (infantry/cavalry/artillery/engineers/scouts/militia), `enum class Speed` (slow/medium/fast/very_fast → milli multiplier), `enum class SquadState` (Holding/Moving/Routing/Routed/Destroyed), `enum class SquadEvent` (MoveOrder/Arrived/CohesionBreak/HpZero/RetreatComplete), `Squad` struct
  - [x] `constexpr bool CanTransition(SquadState, SquadEvent)` + `TargetOf` — explicit transition table (architecture pattern: table IS the spec, unit-testable)
  - [x] Squad fields: string `id`/`name`, `unit`, `maxHp`/`hp`, `attack`, `speedMilli`, `cohesion` (0–100), `cost`, `regionIndex`, `edgeTarget`, `edgeProgress` (ticks), `state`
- [x] Task 2 — Movement + damage tick (AC: 1, 2)
  - [x] `IssueMove(regionIndex)` — Holding only (table-gated); sets edgeTarget + zeroes progress. Adjacency validation is the CALLER's job (controller owns the BattleMap)
  - [x] `TickMove()` — Moving only: `edgeProgress++`; at `TicksForEdge(speedMilli)` ticks → arrive (regionIndex=target, state Holding via Arrived). Integer-only: `TICKS_PER_EDGE_BASE(40) * 1000 / speedMilli` → slow 66 / medium 40 / fast 26 / very_fast 20 ticks
  - [x] `ApplyHit(hpLoss, cohesionLoss)` — clamp hp ≥0, cohesion [0,100]; then: hp==0 → Destroyed (beats rout); else cohesion < `ROUT_THRESHOLD(20)` → Routing. Routing cancels in-flight move (edgeTarget cleared)
  - [x] `RestoreCohesion(n)` / `Heal(n)` helpers clamped; `IsEffective()` = not Routed/Destroyed
- [x] Task 3 — `potato.squad/1` template library (AC: 1)
  - [x] `SquadTemplate` (id, name, unit, hp, attack, speed, optional cohesion default 100, cost) + `SquadTemplateLibrary` (vector + `Find(id)`)
  - [x] `SquadTemplateLibrary::Load(path)` via `Json::Load(path, "potato.squad/1")`; `FromJson` validates: `squads` non-empty array, unique string id, `unit`/`speed` known tokens, hp ≥1 (required), attack/cost ≥0 ints, optional cohesion 0–100, optional name string, schema tag re-checked when present
  - [x] `Squad::Instantiate(const SquadTemplate&, regionIndex)` → Holding squad at full hp
- [x] Task 4 — Headless test `potato_test_squad` (AC: 1, 2)
  - [x] Template load + instantiate; movement timing per speed (medium 40 ticks: still Moving at 39, arrived at 40); CanTransition table spot-checks; ApplyHit rout at cohesion 19/20 boundary, Destroyed precedence, rout cancels move; validation rejects (unknown unit/speed token, dup id, bad ranges); file-path `Load`
  - [x] CMake target + `add_test`; removed `Gameplay/Squad/.gitkeep`
- [x] Task 5 — Verify (AC: 1, 2)
  - [x] MinGW via junction: build green, ctest 5/5
  - [x] `gameplay_dep_guard` green
  - [ ] MSVC: unverifiable (no `cl`) — disclosed

## Dev Notes

### Prior story intelligence (1.1–1.3 landed)

- `Result<T>`: `r.ok()`/`r.error`/`r.reason`; `Ok<T>(v)`/`Fail<T>(e,r)`; T default-constructible.
- `Json::Load(path, schema)` — hardened (CJK paths, 64 MiB cap); `JsonValue` — `Has`+`IsString` to distinguish missing vs wrong-type (1.3 review pattern); `Items()`, `IsInt()` then `AsInt()`.
- `BattleMap` (`Gameplay/Map/BattleMap.h`): `RegionIndexOf`, `Neighbors(i)` (assert-guarded, precondition `i < RegionCount`), `NO_REGION` sentinel. Squads store region INDICES, not ids.
- 20 Hz tick, integer/fixed-point only, no floats in sim code. `assert` + documented preconditions are the style now (1.3 review).
- New .cpp auto-picked by glob; new test exe needs target + `add_test` + **re-configure**; build via `C:\MingGoRTS` junction.

### Design decisions (this story)

- **SquadState table** (explicit, per architecture "enum class + transition table"):
  - Holding --MoveOrder--> Moving; Moving --Arrived--> Holding
  - Holding|Moving --CohesionBreak--> Routing; Routing --RetreatComplete--> Routed
  - any non-terminal --HpZero--> Destroyed; Routed, Destroyed = terminal
- **Speed mapping**: `slow 600 / medium 1000 / fast 1500 / very_fast 2000` milli multipliers; `TicksForEdge = 40*1000/milli` (2s base edge traversal at 20Hz). Edge distances uniform this story — `center` coords available for distance later.
- **Cohesion is a percent int 0–100**; `ROUT_THRESHOLD = 20` per GDD assumption. Rout auto-move/pathing NOT in scope — Routing is a state; retreat orders belong to BattleController (1.6).
- **Template vs instance**: `potato.squad/1` = template library file (array `squads`); `Squad` is the runtime instance (position/state/cohesion). Persistent roster identity lives in Campaign/Roster later — `id` here is the battle-scoped named squad.

### LLM-trap warnings

- Do NOT couple Squad to BattleMap — squad stores indices; the controller validates adjacency (keeps Squad unit-testable alone).
- Do NOT add combat resolution — `ApplyHit` is the interface combat will call; no damage formulas beyond clamps.
- Do NOT auto-path routing squads — state machine only.
- `enum class` (NOT the unscoped bitmask style) — these are exclusive states, not flags.
- Integer math only — no `float`/`double` anywhere in this file.

### References

- [Source: epics.md — Story 1.4]
- [Source: game-architecture.md — Squad/ location, transition-table pattern, BattleState flat arrays, canonical order, no-floats]
- [Source: gdd.md — unit matrix (HP/Attack/Speed/Cost), cohesion <20% rout, doctrine card stat references]
- [Source: 1-3 story — post-review API style (assert + preconditions, Has/IsString)]

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

- Clean first build — no issues.

### Completion Notes List

- `Squad` runtime instance holds region INDICES (controller owns BattleMap + adjacency validation); `SquadTemplate`/`SquadTemplateLibrary` handle `potato.squad/1` content.
- Transition table `CanTransition`+`TargetOf` is constexpr — the spec itself; terminal states reject all events. HpZero takes precedence over CohesionBreak in `ApplyHit`.
- Movement: `TicksForEdge = 40*1000/speedMilli` — slow 66/medium 40/fast 26/very_fast 20 ticks per edge (integer division, deterministic). Edge distances uniform this story.
- Routing is a state only (no auto-pathing) — retreat orders are a BattleController concern (1.6+).
- `FromJson` re-checks schema tag when present (1.3 review pattern); post-review hardening: negative-arg clamps, terminal-state gating, `ApplyEvent` public API, sentinel/self-move rejection, Instantiate spawn-state evaluation.
- 58 checks green; ctest 5/5.
- MSVC still unverified (no `cl`); stdlib-only code.

### File List

- `Gameplay/Squad/Squad.h` (new)
- `Gameplay/Squad/Squad.cpp` (new)
- `Examples/potato_test_squad.cpp` (new)
- `CMakeLists.txt` (modified — potato_test_squad target + add_test)
- `Gameplay/Squad/.gitkeep` (deleted — real files landed)
