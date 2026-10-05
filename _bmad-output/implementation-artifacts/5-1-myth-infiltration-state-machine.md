# Story 5.1 — Myth Infiltration State Machine

## Status

done

## Story

As the myth layer, per-region infiltration state (0–3) is a first-class
deterministic value, driven by myth-layer events/actions through an
explicit transition table, and it persists across the battle into
campaign state.

## Acceptance Criteria

1. Per-region infiltration levels exist; values are exactly 0–3.
2. Transitions follow the explicit 0→1→2→3 table — the table is the
   spec (architecture §state transitions), unit-testable.
3. Myth-layer events/actions apply through the table; out-of-band
   mutation is impossible.
4. State persists across the battle and into campaign
   (`potato.myth/1`, versioned JSON via `JsonValue`).
5. Deterministic: integer-only state, folded into the battle checksum;
   journaled so replay reproduces it.

## Design

### Layers

- **`Gameplay/Myth/Infiltration.{h,cpp}`** — `InfiltrationLevel`
  (None/Whispered/Haunted/Invaded = 0..3), `MythEventKind`
  (Incursion=0, Pacification=1), the transition table
  `Transition(level, event)`, and `MythField` — the battle-scope
  per-region store (mirror of `GovernanceField`: borrowed map ref,
  `Init`, deterministic).
- **`BattleController`** — `myth_` member; Planning-phase verbs
  `SeedInfiltration(region, level)` (persisted carry-in) and
  `ApplyMythEvent(region, kind)` (table-driven apply, emits
  `InfiltrationChanged`). Mid-Execution myth drivers belong to
  Story 5.4 (myth actions), which own their own journal mechanism.
- **`SimEvent::Kind::InfiltrationChanged`** (ordinal 9, append-only):
  param = region, aux = new level 0–3, side = -1, squadIndex = -1 —
  the myth layer is side-less.
- **`BattleRecorder`** — `"mythseed"`/`"myth"` planning ops;
  `TOOL_VERSION` 3→4; event kind bound 8→9.
- **`Campaign/Myth/MythState.{h,cpp}`** — `potato.myth/1`, keyed
  `chapters -> <chapterId> -> <region> -> level`. Region indices are
  map-local, so the durable identity is (chapterId, region) — the
  chapter is the unit the campaign already keys progression on.
  `CommitMyth(state, chapterId, field)` is the battle→campaign merge
  seam (only nonzero levels stored — canonical form).

### The table (this IS the spec)

| level \ event | Incursion | Pacification |
|---|---|---|
| 0 None      | 1 | 0 |
| 1 Whispered | 2 | 0 |
| 2 Haunted   | 3 | 1 |
| 3 Invaded   | 3 | 2 |

Saturating at both ends. `Apply` emits `InfiltrationChanged` only when
the level actually changes — the event name is literal — and the
controller verb *rejects* a saturating no-op (journal contract: an
accepted, journaled call must move state, so a forged no-op op can
never ride a valid record).

## Tasks

- [x] `Gameplay/Myth/Infiltration.{h,cpp}`
- [x] `SimEvent::Kind::InfiltrationChanged` (Doctrine.h)
- [x] BattleController: `myth_`, verbs, checksum fold
- [x] Recorder/verifier: ops, bounds, TOOL_VERSION 4
- [x] `Campaign/Myth/MythState.{h,cpp}` + `CommitMyth`
- [x] `potato_test_myth` (linked against PotatoCampaign) — 83 checks
- [x] Full ctest + dep guard — 19/19
- [x] Three-layer review

## Review Outcome

Three layers (acceptance / edge / blind): AC 5/5 PASS. Fixes landed:

- **HIGH**: `potato_test_governance` wire-bound pin went stale when the
  kind bound rose 8→9 (asserted kind 9 rejects) — updated to kinds
  4–9 round-trip + kind 10 rejects.
- **MED**: `CommitMyth` returned void and discarded `Set` failures —
  a bad chapter id or a >1024-region field would silently lose the
  whole commit *after* clearing the old state. Now `Result<int>`
  (count written) with all fallible conditions preflighted before
  `ClearChapter`.
- **MED**: forged no-op `myth` ops (e.g. Pacification at level 0)
  replayed identically → could be injected into a root-consistent
  record. Journal contract tightened: `Apply`/`Seed` REJECT
  no-change calls — journaled ops must move state.
- **MED**: `BattleMap::FromJson` had no region cap (verifier could
  be fed a 100M-region DOM). `MAX_REGIONS = 1024` added;
  `MythState::MAX_REGIONS_PER_CHAPTER` aliases it.
- **LOW**: verifier bounds now use `kInfiltrationLevelCount` /
  `kMythEventKindCount` constants; `EventFromJson` kind bound uses
  the enum tail; `MythField::map_` dead member removed; non-canonical
  region keys (`"03"`) rejected; `Seed` validates enum domain;
  downgrade-flag doc corrected.

## Dev Notes

- Story 5.5 (invasion events) reads `LevelAt() == Invaded` as its
  trigger surface — this story deliberately does NOT implement
  invasions.
- `MythState` is a sibling doc (like `potato.rivals/1`), not a field
  inside `potato.campaign/1` — keeps schema versions independent.

## File List

- `Gameplay/Myth/Infiltration.h` / `Infiltration.cpp` — new
- `Gameplay/Doctrine/Doctrine.h` — `InfiltrationChanged` (kind 9)
- `Gameplay/Sim/BattleController.{h,cpp}` — `myth_`, verbs, checksum
- `Gameplay/Record/BattleRecorder.{h,cpp}` — ops, kind bound 9, v4
- `Gameplay/Record/ReplayVerifier.cpp` — `mythseed`/`myth` replay
- `Campaign/Myth/MythState.{h,cpp}` — new (potato.myth/1)
- `Gameplay/Map/BattleMap.{h,cpp}` — `MAX_REGIONS` cap (review)
- `Examples/potato_test_myth.cpp` — new
- `Examples/potato_test_governance.cpp` — wire-bound pin update
- `CMakeLists.txt` — test target
