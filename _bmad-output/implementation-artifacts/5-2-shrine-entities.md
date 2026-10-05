# Story 5.2 — Shrine Entities

## Status

done

## Story

As a player, I want capturable shrines on the map's myth layer,
so that sacred ground is an objective.

## Acceptance Criteria

1. Shrines exist on `MYTH_SHRINE`-marked regions (map data already
   carries the mark — `IsStrategicPoint` counts them).
2. A squad occupying a shrine region (exclusive presence, dedication
   dwell) flips the shrine's allegiance; losing exclusive control
   frees the latch so recapture re-dwells and re-emits.
3. Capture updates the shrine deity's stance toward both sides
   (`GodStance` — minimal model here; 5.7 grows the full per-region
   disposition that modulates myth actions).
4. Shrine state is keyed by region — same geometry as terrain — so
   the renderer/narrative layer can co-render it (`Shrines()`,
   `ShrineAt(region)`, `StanceAt(region, side)`); headless layers
   expose data, they don't draw.
5. Deterministic + journaled: shrine capture emits `ShrineCaptured`
   (SimEvent kind 10, append-only), shrine state folds into the
   checksum, replay reproduces it.

## Design

- `Gameplay/Myth/Infiltration.{h,cpp}` extends `MythField` (the
  myth-layer battle store already owns per-region state):
  - `GodStance` enum: `Wrathful(-1)/Neutral(0)/Favorable(+1)` toward
    each side. Capture → favorable toward capturer, wrathful toward
    the other side (taking sacred ground offends the loser's claim).
  - `ShrineTrack`: same dwell/latch discipline as villages —
    `SHRINE_DEDICATION_TICKS` (4 s at 20 Hz, same tempo — dedicating
    ground takes as long as holding it).
  - `Tick(squads, tick)` scans shrines in map order after governance
    events — canonical order: doctrine → burn → governance → myth.
- `SimEvent::Kind::ShrineCaptured` (ordinal 10): param = region,
  side = new owner, squadIndex = witness (lowest index at region),
  aux = 0.
- Out of scope (later stories): persistence of allegiance into
  `potato.myth/1` (needs the map-revision question answered first —
  region ids are map-local), 天命 credit on capture (5.3), myth
  action effects on shrines (5.4), stance-driven modulation (5.7).

## Tasks

- [x] `GodStance` + `ShrineTrack` + `MythField::Tick` + accessors
- [x] `SimEvent::Kind::ShrineCaptured` (kind 10)
- [x] BattleController: `myth_.Tick` in pipeline, checksum fold
- [x] Tests in `potato_test_myth` — incl. record→replay e2e
- [x] ctest + dep guard — 19/19
- [x] Three-layer review + convergence fixes

## Review Fixes (three-layer pass)

- **MED — claimant bounds**: `ExclusiveSide` returns the raw
  `squad.side`; a hand-built squad with `side ∉ {0,1}` would have
  indexed `stance[]` out of bounds. `Tick` now gates the claimant to
  the two-side domain before any array use; a `side=2` squad pinning
  test verifies zero captures/UB.
- **MED — retained stance (kept as design)**: releasing a latch
  clears `owner` but leaves `stance` — the deity remembers the prior
  dedication/violation. Documented and pinned by a release test
  (`Favorable`/`Wrathful` persist on unconsecrated ground).
- **MED — village burn vs shrine (kept as design)**: burning a
  co-located village does not touch shrine state — gods cannot be
  burned out by a historical-layer action. Pinned: `r1` carries both
  marks; after `scorch` burns the village, the shrine still
  dedicates to the occupying side while `VillageOccupied` never
  fires.
- **Tests**: checksum-divergence test made real (controller `b`
  deploys off-shrine; checksums must differ); sealed record with a
  true `ShrineCaptured` (kind 10) verified end-to-end via
  `Replay::Verify`.
- **Docs**: `Init` now documents shrine scanning; class comment
  notes shrine state folds into the checksum; `ShrineCaptured` is
  tick-scan-generated (not a `MythEventKind` planning verb);
  dedication emits a recorded *event* — ledger pricing is deferred
  to Story 5.3.

## File List

- `Gameplay/Myth/Infiltration.h` — `GodStance`, `ShrineTrack`,
  `MythField::Tick`/`Shrines`/`ShrineAt`/`StanceAt`,
  `SHRINE_DEDICATION_TICKS`
- `Gameplay/Myth/Infiltration.cpp` — shrine indexing, dwell/latch
  scan, stance update, claimant gate
- `Gameplay/Doctrine/Doctrine.h` — `SimEvent::Kind::ShrineCaptured`
  (ordinal 10)
- `Gameplay/Sim/BattleController.{h,cpp}` — `myth_.Tick` in the tick
  pipeline (governance → myth), shrine state in `Checksum()`
- `Gameplay/Record/BattleRecorder.{h,cpp}` — wire kind bound 10,
  kind 11 rejected
- `Examples/potato_test_myth.cpp` — shrine tests, burn asymmetry,
  side-2 safety, record→replay e2e
- `Examples/potato_test_governance.cpp` — wire bound pin updated
  (4–10 round-trip, 11 rejected)

## Deferred Work

- `ShrineCaptured` → 天命 credit/debit posting: **Story 5.3**
  (`DeedBook` drops it via default path for now — intentional).
- Shrine allegiance persistence into `potato.myth/1`: needs the
  map-revision question resolved (region ids are map-local).
- `GodStance` modulation of myth actions: Story 5.7.
