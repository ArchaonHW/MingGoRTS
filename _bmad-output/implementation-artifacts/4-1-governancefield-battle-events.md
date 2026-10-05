# Story 4.1: GovernanceField Battle Events

## Status: done

## Story

**As a** system,
**I want** per-battle tracking of village occupation/burning and convoy escort/raid,
**So that** field conduct has campaign weight.

## Acceptance Criteria

1. Given a battle with villages and convoys,
   When occupation/burning/escort/raid events occur,
   Then each emits a typed SimEvent destined for ledger posting
   And the event carries enough context (region, perpetrator) for
   accounting.

## Dev Notes

- Architecture home: `Gameplay/Governance/` — "GovernanceField
  (battle-scope tracking), events feed campaign". Campaign-side
  accumulators are Story 4.3; this story emits the *events*, the
  ledger fold consumes them later.
- New `SimEvent::Kind` ordinals (append-only, wire-stable):
  `VillageOccupied`(4) `VillageBurned`(5) `ConvoyArrived`(6)
  `ConvoyRaided`(7). `EventFromJson`'s kind bound bumps 3→7;
  `BattleRecorder::TOOL_VERSION` bumps 2→3 (new vocab + `convoy` op).
- Payload convention (param/side/squadIndex/aux carry the accounting
  context the AC asks for):
  - `VillageOccupied`: param=region, side=occupier,
    squadIndex=occupier witness (first effective squad at region).
  - `VillageBurned`: param=region, side=burner,
    squadIndex=burning squad.
  - `ConvoyArrived`: param=destination region, side=owner,
    aux=convoy index, squadIndex=escort of record — the friendly
    squad standing at the destination on the arrival tick (-1 if
    none). Destination-presence proxy: mid-route shepherding isn't
    tracked, and a bystander counts. Honest as "witness", weak as
    "escort" — revisit if accounting needs per-leg escort credit.
  - `ConvoyRaided`: param=region, side=raider, aux=convoy index,
    squadIndex=raiding squad.
- aux multiplexes again: the wire gate allows 0..5 — MAX_CONVOYS=4
  keeps convoy indices inside it without weakening the gate.
- **Occupation**: a village region held by exactly one side's
  effective non-Routing squads for `OCCUPY_TICKS` (4 s) consecutive
  ticks → one `VillageOccupied`. Losing exclusive control clears the
  latch; recapture re-dwells and re-emits (each capture is a deed —
  repeated income is the fold layer's call, not the tracker's).
  Burned villages are ash: no occupation, no re-burn. Squads
  mid-edge count at their departure region (same convention as fog).
- **Burning is authored**: `ActionKind::Burn` + `PendingDelta::Kind::Burn`
  — doctrine cards order the atrocity (symmetric AI gets it free; the
  GDD calls raiding "a doctrine choice"). The delta applies through
  `GovernanceField::TryBurn` — gated at apply: squad still effective
  AND `Holding` AND standing on an unburned village region. A squad
  that started moving (a Move delta earlier in the same delta list)
  can't burn — delta order is preserved by per-delta dispatch.
  Burning a *contested* village is allowed — perpetrator is recorded;
  the atrocity fold (4.2/4.3) prices it.
- **Convoys are entities, not squads**: there's no combat resolution
  yet, so a convoy is a GovernanceField-owned noncombatant:
  `SpawnConvoy(side, path)` (Planning only; path = ValidPathShape
  rules — in-bounds, pairwise-adjacent, no revisits, len 2..N).
  Marches one leg per `CONVOY_LEG_TICKS` (66 = Slow edge cost);
  stands at `path[cursor]` while in transit (departure-region
  semantics, same as squads). Per tick, in convoy-index order:
  march → raid check → arrival check.
  - Raid: an effective non-Routing enemy squad stands at the convoy's
    region AND no effective non-Routing owner-side squad does →
    `ConvoyRaided`, convoy removed. Presence is the deterrence check
    (no combat exists yet — strength comparisons deferred to the
    combat layer).
  - Arrival: reaches `path.back()` → `ConvoyArrived`. A raider
    standing at the destination raids at the gate — raid is checked
    before arrival each tick.
- `RecordConvoy` journals the op (`"convoy"`); the verifier replays
  it through `SpawnConvoy` like every other planning input.
- GovernanceField state (village tracks + convoys) folds into
  `BattleController::Checksum` — replay bit-exactness covers it.
- New tick step 3.5 in the canonical order: doctrine events still
  precede governance events within a tick (CardFired → VillageBurned
  ordering is structural).
- `ApplyDeltas` signature becomes `std::span<const PendingDelta>` so
  the controller can preserve interleaved Burn/Move ordering with
  single-delta calls. `ApplyDeltas` itself ignores `Kind::Burn`
  (controller-owned).
- Not in scope (deferred): atrocity *flagging* into reports (4.2),
  ledger posting of these events (4.3), convoy contents/loot values,
  strength-based raid resolution (combat layer), CP burn order
  (presentation-era host verb), convoys in wipe detection.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `GovernanceField.{h,cpp}`: village tracks,
    convoys, TryBurn, Tick scan, checksum surface
- [x] Task 2 — doctrine vocab: `ActionKind::Burn`, delta kind,
    span signature, ordered dispatch in `BattleController::Tick`
- [x] Task 3 — wire: SimEvent kinds 4-7, `RecordConvoy` op,
    verifier replay, kind bound, TOOL_VERSION 3, checksum fold
- [x] Task 4 — `potato_test_governance` + record-test convoy pin
- [x] Task 5 — story/sprint docs; commit
- [x] Task 6 — three-layer review

## Review findings (three-layer, AC:PASS — applied)

- **Dwell counter unbounded** (Edge, Low): clamped at
  `OCCUPY_TICKS` — nothing reads past the threshold; a stalemate-
  disabled battle can't grow it into signed overflow.
- **Arrow+burn asymmetry** (Blind, Med): a squad on an active plan
  arrow burns while still Holding, then marches the same tick —
  kept as designed. Arson is a deed, not a hold: priced by the
  ledger fold (民心 debit + 軍威 ratchet). Documented in
  GovernanceField.h; pinned by tests (arrow squad burns Holding;
  `[move,burn]` vs `[burn,move]` slot order diverges — authored).
- **Contested/instant burn** (Blind, Med): deliberate — documented
  in the header's design note.
- **Escort = destination-presence proxy** (Auditor): Dev Notes
  wording corrected; no mid-route escort tracking exists.
- **Coverage pins added**: CardFired precedes VillageBurned;
  slot-order preemption via real Move deltas; two arsonists → one
  event at lowest squad index; side-1 convoy raided by side 0;
  raider camped at spawn node raids tick 0; mid-route convoy at
  close emits nothing; governance kinds 4-7 wire round-trip;
  kind 8 rejected.
- **Deferred**: a convoy still in flight when the battle closes
  emits no terminal event — silence is "unsettled"; resolving
  in-flight convoys belongs to the chapter-shell integration seam
  (deferred-work.md).

## File List

- `Gameplay/Governance/GovernanceField.h` (new)
- `Gameplay/Governance/GovernanceField.cpp` (new)
- `Gameplay/Doctrine/Doctrine.h` (Burn action/delta, SimEvent kinds
  4-7, span signature)
- `Gameplay/Doctrine/Doctrine.cpp` (burn token, delta emit)
- `Gameplay/Sim/BattleController.h/.cpp` (field member, SpawnConvoy,
  ordered delta dispatch, tick step 3.5, checksum fold)
- `Gameplay/Record/BattleRecorder.h/.cpp` (RecordConvoy, kind bound
  7, TOOL_VERSION 3)
- `Gameplay/Record/ReplayVerifier.cpp` (convoy op replay)
- `Examples/potato_test_governance.cpp` (new — 64 checks)
- `Examples/potato_test_record.cpp` (convoy record round-trip)
- `CMakeLists.txt` (potato_test_governance target + ctest)
- `_bmad-output/implementation-artifacts/4-1-governancefield-battle-events.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/deferred-work.md`
