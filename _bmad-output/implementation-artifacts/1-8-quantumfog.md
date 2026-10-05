# Story 1.8: QuantumFog

Status: done

## Story

As a player,
I want enemy positions represented as certainty gradients I can sharpen, collapse, and lose to decay,
So that intel is a resource, not a toggle.

## Acceptance Criteria

**Given** a CertaintyField (0–100 per region) and CloudEntity set,
**When** observation/probe/decay/entangle ops run per tick,
**Then** certainty updates per balance JSON (~10/min decay), collapse resolves clouds to truth
**And** no code outside `Gameplay/Sim` can read true enemy positions (truth boundary enforced).

## Tasks / Subtasks

- [x] Task 1 — Fog data model (`Gameplay/Fog/QuantumFog.h/.cpp`) (AC: 1)
  - [x] `struct CloudEntity { int believedRegion; int certainty /*0-100*/; int entangledWith /*cloud id*/; int id; }` — **belief only, no truth fields**
  - [x] `class QuantumFog` = per-observer view: `std::vector<int> certainty_` (per map region, 0–100) + `std::vector<CloudEntity> clouds_`
  - [x] `struct FogConfig` — balance knobs: `decayPerMinute=10`, `probeGain=25`, `detectThreshold=60`, `initialIntel=60`; `FromJson` reads `"fog"` object of `potato.balance/1`; `Defaults()`
- [x] Task 2 — Fog ops (AC: 1)
  - [x] `int AddCloud(believedRegion, certainty)` → cloud id; `RemoveCloud(id)`; `Clouds()` read-only
  - [x] `Observe(region)` — region certainty → 100 (cloud collapse is caller-driven — fog never sees truth)
  - [x] `Collapse(cloudId, believedRegion)` — snap + certainty 100
  - [x] `Probe(region)` — `certainty_[region] += probeGain` capped 100; **no cloud effect**
  - [x] `Entangle(a, b)` — mutual `entangledWith`
  - [x] `TickDecay()` — rational accumulator: `accum += decayPerMinute`; while `accum >= TICKS_PER_MIN` (1200) → −1 to all region + cloud certainty; deterministic, no draws
  - [x] `VisibleAt(region)` — count clouds with `believedRegion == region && certainty >= detectThreshold`
- [x] Task 3 — Controller integration (AC: 1)
  - [x] `fog_[2]` per observing side; `cloudOf_[fogSide][squadIndex]` truth mapping **stays in `Gameplay/Sim/`** — `std::array<std::vector<int>,2>` aligned with `squads_`
  - [x] `DeploySquad` → enemy fog `AddCloud(region, initialIntel)`
  - [x] Tick step 0.5 (after interventions, before EvalTick): auto-observe — enemy cloud collapses to truth when any effective friendly squad shares/adjoins its region; entangled partners collapse to their truth (reverse map cloudId→squadIndex)
  - [x] `TickDecay()` both fogs each Execution tick; Routed/Destroyed squads' clouds removed
  - [x] `IssueProbe(side, region)` — Execution, budget `PROBE_BUDGET=3`/side, no CP; queued as Intervention kind Probe
  - [x] `IssueEntangle(side, cloudA, cloudB)` — Execution, 2 CP; queued kind Entangle
  - [x] `const QuantumFog& Fog(int side)` read accessor (UI/tests); `SetCloudIntel(fogSide, squadIdx, region, certainty)` Planning-only hook
  - [x] Checksum folds both fogs (region certainty + cloud belief/certainty/entangle + pending kinds already covered)
- [x] Task 4 — Doctrine trigger rewire (AC: 1)
  - [x] `EvalTick` signature gains `const std::array<QuantumFog,2>& fog` — enemy triggers read `fog[sq.side]` only
  - [x] `enemy_in_region` → `fog[sq.side].VisibleAt(sq.regionIndex) > 0`; `enemy_adjacent` → any neighbor `VisibleAt > 0`
  - [x] Delete the truth-scan `AnyEnemy` path for these two kinds — the seam closes here (deferred-work item)
- [x] Task 5 — Test `potato_test_fog` + update existing tests (AC: 1)
  - [x] Unit: decay exactly 1pt/120 ticks at 10/min (rational), probe raises field not clouds, collapse snaps + propagates entangled, cap 100 / floor 0
  - [x] Boundary: `CloudEntity` has no truth field (struct review); doctrine fires on belief (cloud injected at wrong region → `enemy_in_region` fires = stale intel false positive) and misses when belief misplaced (truth present, cloud elsewhere → no fire)
  - [x] Integration: auto-observe co-located/adjacent each tick; probe budget 3 exhausted rejects; entangle via IssueEntangle propagates at collapse
  - [x] Update `potato_test_doctrine`/`potato_test_battle`/`potato_test_command` for new EvalTick signature + fog fixtures
- [x] Task 6 — Verify: MinGW ctest all green incl. dep guard; MSVC disclosed

## Dev Notes

### Architecture anchors

- `D-ARCH-7`: region certainty field + probability-cloud entities; collapse/probe/decay/entangle as sim ops (game-architecture.md:144).
- Truth boundary (NFR8, arch:349-360): sim keeps truth privately; UI/doctrine read certainty only. Consequence: **`CloudEntity` must not store squad indices or true positions.** Sim-side bridge (`cloudOf_` mapping, collapse calls) lives in `BattleController` — `Gameplay/Fog/` is a pure belief store and may not `#include` Squad/sim truth headers.
- Decay `~10/min` @20Hz → 1 point per 120 ticks. Use the rational accumulator (`accum += decayPerMinute; while accum >= 1200 {decay 1}`) — stays exact for non-divisor values like 7/min.
- `potato.balance/1` — first balance file; schema `"potato.balance/1"`, `"fog"` object optional (defaults fill).

### Prior-story contract points

- `EvalTick` currently: `(map, snapshot, sheets, cards, rng, cpPools, tick)` — add `fog` before `tick`; update **all three** existing test files' call sites.
- `AnyEnemy` helper in Doctrine.cpp currently truth-scans — replaced by `VisibleAt` for enemy kinds. `NO_REGION` guard stays relevant (fog `VisibleAt` bounds-check).
- `SimEvent::Kind::Intervention` already wire-documented; new kinds append ordinals (Probe=3, Entangle=4) — do not reorder.
- `forceNext` still bypasses trigger — an override on `enemy_in_region` fires regardless of fog (documented behavior).

### Library-first

- Reuse `Result<T>`, `Json::Load`, `JsonValue` — no new deps.
- Integer-only (NFR1). No floats anywhere in fog math.

### Testing

- Stale-intel false-positive is the load-bearing test — it proves doctrine reads belief, not truth.
- `SetCloudIntel` is the Planning-side equivalent of `SetCpPool` (test/briefing hook) — Execution must reject it.

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

### Completion Notes List

- ``Gameplay/Fog/QuantumFog.h/.cpp`` — belief-only store: ``CloudEntity{believedRegion, certainty, entangledWith}`` (NO truth fields), per-region ``CertaintyField``, ``FogConfig`` (``potato.balance/1`` ``"fog"`` section: decay_per_minute/probe_gain/detect_threshold/initial_intel).
- Ops: ``AddCloud/RemoveCloud/Observe/Collapse/SetCloudBelief/Probe/Entangle/TickDecay/VisibleAt``. Decay is exact rational accumulation (points per 1200 ticks) — deterministic, integer-only.
- ``BattleController``: ``fog_[2]`` per observer; ``cloudOf_[side][squadIdx]`` truth map stays in Sim/; DeploySquad seeds enemy fog at ``initialIntel=60``.
- ``SyncFog`` per tick (before EvalTick): prune clouds of off-field squads -> auto-observe (effective non-routing squads see own+neighbor regions; collapse + entangled-partner propagation to partner's OWN truth) -> TickDecay.
- New interventions: ``IssueProbe`` (0 CP, PROBE_BUDGET=3/side) + ``IssueEntangle`` (2 CP) — queued/applied like commands; InterventionKind ordinals appended (Probe=3, Entangle=4).
- ``SetCloudIntel`` Planning-only intel hook; ``Fog(side)`` read accessor for UI/tests.
- Doctrine seam CLOSED: ``EvalTick`` takes ``std::array<QuantumFog,2>``; ``enemy_in_region``/``enemy_adjacent`` read ``fog[side].VisibleAt`` — the truth-scan helpers are gone (deferred-work item resolved).
- Debug log: a "misplaced cloud hides adjacent truth" test was unwritable — auto-observe covers exactly the region set either trigger can check, so doctrine-level misses are unreachable by design; the miss direction is demonstrated at fog level instead (VisibleAt truth region == 0 while belief elsewhere) + a belief-adjacent false positive.
- Checksum folds both fogs (fields + clouds + cloudOf map + probe budgets).
- MinGW: ctest 9/9 green incl. dep guard. MSVC unverified.

### File List

- Gameplay/Fog/QuantumFog.h/.cpp (new)
- Gameplay/Doctrine/Doctrine.h/.cpp (modified — fog param, belief triggers)
- Gameplay/Command/Intervention.h (modified — Probe/Entangle kinds, PROBE_BUDGET)
- Gameplay/Sim/BattleController.h/.cpp (modified — fog_[2], cloudOf_, SyncFog, IssueProbe/Entangle, SetCloudIntel, checksum)
- Examples/potato_test_fog.cpp (new)
- Examples/potato_test_doctrine.cpp (modified — TruthfulFog helper, new signature)
- CMakeLists.txt (modified — potato_test_fog)
