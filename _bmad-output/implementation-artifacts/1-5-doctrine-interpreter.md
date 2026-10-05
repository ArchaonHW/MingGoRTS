---
baseline_commit: NO_VCS
---

# Story 1.5: Doctrine Interpreter

Status: done

## Story

As a player,
I want my doctrine cards to execute trigger→condition→action per tick in canonical order,
So that what I write is what happens.

## Acceptance Criteria

1. **Given** a squad sheet of 3–5 slotted cards,
   **When** a tick evaluates,
   **Then** slots evaluate in (squad idx → slot idx) order against a tick-start snapshot, actions write pending deltas applied post-evaluation, and cooldowns tick down.
2. **And** two identical seeds+sheets produce identical event streams.

## Tasks / Subtasks

- [x] Task 1 — Card + sheet model (AC: 1)
  - [x] `Gameplay/Doctrine/Doctrine.h` — `DoctrineCard` (id, name, Trigger{kind,param}, Condition{kind,param}, Action{kind,param}, Modifier{kind,param}, `cooldownTicks`), `CardSlot` (cardIndex + cooldownRemaining), `SquadSheet` (3–5 slots)
  - [x] `potato.doctrine_cards/1` library file: `DoctrineLibrary::Load/FromJson` — `cards` non-empty array, unique id, known type tokens, int params within table-declared ranges, cooldown 5–60 s → ×20 ticks
  - [x] `SquadSheet::Build(library, cardIds)` → `Result` — 3–5 ids, all resolvable
- [x] Task 2 — Vocabulary v0 (AC: 1)
  - [x] Triggers: `always`, `cohesion_below{pct}`, `enemy_in_region`, `enemy_adjacent` (against tick-start snapshot + map)
  - [x] Conditions: `always`, `cohesion_above{pct}`, `cohesion_below{pct}`, `cp_at_least{n}`
  - [x] Actions: `hold`, `brace{amount}` (cohesion delta), `move{regionIndex}` (pending move), `retreat` (pending move to lowest-index neighbor — map-edge proxy)
  - [x] Modifiers: `none`, `attack_boost{amount}` (pending attack delta)
- [x] Task 3 — Interpreter tick (AC: 1, 2)
  - [x] `DoctrineInterpreter::EvalTick(map, snapshot, sheets, cards, rng, tick)` → `{events, deltas}` — canonical order `squadIdx → slotIdx`; every slot ticks cooldown first; skipped squad states (Routed/Destroyed) evaluate nothing but still tick cooldowns
  - [x] Trigger reads snapshot; condition gate; on fire: action+modifier write `PendingDelta`, emit `SimEvent{CardFired, tick, squadIdx, slotIdx, cardId}`, set `cooldown = card.cooldownTicks`
  - [x] `ApplyDeltas(squads, deltas)` — post-eval application (CohesionDelta / MoveDelta via IssueMove / AttackDelta); never during eval
  - [x] `Prng` extracted to shared type (`Gameplay/Sim/Sim.h`) — Sim and interpreter share SplitMix64; interpreter takes `Prng&` (draws reserved for future cards; canonical order contract)
- [x] Task 4 — Headless test `potato_test_doctrine` (AC: 1, 2)
  - [x] Sheet build 3–5 validation; canonical-order event indices; snapshot isolation (action on squad A must not be visible to squad B's trigger in same tick); cooldown counts down and gates re-fire; identical seed+sheets → identical event stream; loader rejects
  - [x] CMake target + `add_test`; remove `Gameplay/Doctrine/.gitkeep`
- [x] Task 5 — Verify
  - [x] MinGW junction build green; ctest 6/6; dep guard green; MSVC disclosed

## Dev Notes

### Prior story intelligence

- `Prng` does not exist yet — extract Sim's SplitMix64 into `struct Prng` in `Sim.h` (`Next()`, seed ctor), Sim delegates to it. Same bit stream semantics — Sim tests must stay green.
- `BattleMap`: `Neighbors(i)` (assert-guarded), `RegionAt`, `RegionIndexOf`. `Squad`: `IssueMove` (Holding-gated, rejects self/NO_REGION), `ApplyHit`, `cohesion`/`state` public fields, `IsEffective()`.
- `Json::Load` + `FromJson` schema-tag-when-present pattern (1.3/1.4); `Has`+`IsString` type distinction.
- Snapshot semantics are load-bearing: copy `std::vector<Squad>` per EvalTick — cheap at battle scale; never let same-tick writers be visible to other evaluators.

### Design decisions (this story)

- **Library file** `potato.doctrine_cards/1` (array `cards`), not per-card files — consistent with map/squad loaders; single-card files possible later over the same DOM.
- **`SimEvent`** is a tagged struct this story (`kind + tick + squadIndex + slotIndex + cardId`); typed-variant refinement belongs to BattleRecorder (1.11).
- **`enemy_in_region`/`enemy_adjacent`** need a notion of "enemy": squads carry no side field — EvalTick signature splits `allies`/`enemies` lists? No — sheets align with a `squads` array and a `side` flag per squad is simplest: add `int side` to Squad (0=player,1=enemy). Minimal field, symmetric AI needs it anyway.
- **Cooldown counts down every tick per occupied slot**, regardless of trigger outcome (cooldown = card cooldownTicks on fire; decrement happens at slot visit).
- **`retreat`** = IssueMove toward neighbor with lowest index (canonical, deterministic; "map edge" convention documented).

### LLM-trap warnings

- Do NOT evaluate against live squads — snapshot only; deltas apply after ALL slots eval.
- Do NOT let cooldown tick only on fire — it ticks per visit (otherwise cooldown is meaningless).
- Do NOT add combat damage resolution — doctrine writes deltas; no fighting model this story.
- Integer-only. No exceptions. `Result` only at file boundary.
- Routed/Destroyed squads: cooldowns still tick (sheet is a resource), slots don't evaluate.

### References

- [Source: epics.md — Story 1.5]
- [Source: game-architecture.md — Doctrine Interpreter section, canonical eval order, pendingDelta rule, D-ARCH-3]
- [Source: gdd.md — doctrine card structure, cooldown 5–60s, example cards]

## Review Findings (three-layer code review, 2026-09-29)

9 patch / 6 defer / 0 dismissed.

- [x] `move` action param never validated → teleport/OOB `regionIndex` corruption — eval-time bounds + adjacency gate added (loader can't see the map)
- [x] No param ranges (spec Task 1 promised table-declared ranges) → `TokenDef` gains `needsParam`/`pmin`/`pmax`; loader rejects missing/out-of-range params
- [x] `brace`/`Heal`/`attack` signed-overflow UB on huge params → int64 saturating adds in `RestoreCohesion`/`Heal`/`ApplyDeltas`
- [x] `AnyEnemy` lacked `NO_REGION` guard — off-field opposing squads co-triggered `enemy_in_region`
- [x] `ApplyDeltas` could mutate terminal squads (`attack` on Destroyed) → `IsEffective()` gate
- [x] `Instantiate` didn't set `side` (silent `enemy_*` blindness) → `Instantiate(t, region, side)` param
- [x] `sheets`/`snapshot` size mismatch silent → `assert` + header contract
- [x] `Prng` header overclaimed seed-independence → reworded (one period-2⁶⁴ stream, seed = offset)
- [x] Test gaps: cooldown suppression never asserted (t1..t99 now assert empty), `>=3`→`==4` exact counts, `enemy_in_region`/`cohesion_below` coverage added, `.gitkeep` deleted, `At()` assert, `ReadClause` missing-vs-wrong-type message, unused `<fstream>`, Prng golden checksum pinned (8436903512816397795)

Deferred (see deferred-work.md): enemy_* truth-boundary seam → Story 1.8; shared cpPool → Story 1.10; snapshot-as-const& caller discipline; rng-vacuous determinism test; library-vs-singular schema naming; strict param-on-no-param-kind.

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

### Completion Notes List

- Extracted SplitMix64 into shared `Prng` (Sim.h/Sim.cpp) — Sim delegates, identical stream semantics preserved (counter += golden ratio, avalanche); zero-seed-safe and seed-injective.
- Added `int side` to `Squad` (0=player,1=enemy) — prerequisite for enemy_* triggers and symmetric AI.
- `Gameplay/Doctrine/`: DoctrineCard/SquadSheet/CardSlot/SimEvent/PendingDelta + `DoctrineLibrary` (potato.doctrine_cards/1) + `Doctrine::EvalTick`/`ApplyDeltas`.
- EvalTick reads a tick-start snapshot (caller's `const&` — pure w.r.t. squads), writes events + PendingDeltas; ApplyDeltas commits after eval. Cooldown decrements per occupied slot visit, sets on fire (cooldown 5–60 s x20 ticks).
- `retreat` = lowest-index neighbor (map-edge proxy); `move` delta -> IssueMove; `brace` -> RestoreCohesion; `attack_boost` -> attack += delta (clamped >=0).
- `Prng&` accepted in EvalTick signature per contract (canonical draw order); unused in vocab v0 — `(void)rng`.
- MinGW verify: 27/27 doctrine checks, ctest 6/6 green, dep guard green. MSVC unverified (no cl locally).
- Post-review verify: 42 doctrine checks, ctest 6/6 green (MinGW).

### File List

- Gameplay/Doctrine/Doctrine.h (new)
- Gameplay/Doctrine/Doctrine.cpp (new)
- Gameplay/Sim/Sim.h (modified — Prng extraction)
- Gameplay/Sim/Sim.cpp (modified — Prng delegation)
- Gameplay/Squad/Squad.h (modified — side field)
- Examples/potato_test_doctrine.cpp (new)
- CMakeLists.txt (modified — potato_test_doctrine target/add_test)
