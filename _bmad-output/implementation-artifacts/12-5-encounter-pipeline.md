---
baseline_commit: NO_VCS
---

# Story 12.5 — Encounter Pipeline

> Epic 12 — 行營輿圖 Open Campaign World (W) · new schema
> `potato.encounter/1` · `potato.mintclaim/1` additive kind ·
> **Status: review**

## Story (from epics.md)

As a system,
I want world triggers (region entry, proximity, POI) to marshal a
battle — map template, belligerents, stakes — into the three-beat
BattleController, and the aftermath to write back to the world,
so that battles happen where the army stands.

## Acceptance Criteria

- **Given** an armed encounter, **when** it triggers, **then**
  battle assembly produces a valid `BattleState`, and Aftermath
  writeback applies control flags, ledger entries, roster
  casualties, and infiltration deltas
- **And** a converted defeat returns the warband to the map via
  the existing defeat-conversion path — no dead ends
- **And** replay-gated claim kinds (`potato.mintclaim/1`)
  generalize to `settlement` so encounters mint like chapters.

## Context

Design authority: sprint-change-proposal-2026-10-06 (approved),
FR23. 12.1–12.4 landed geography, world state, movement, and the
commander. This story is the **bridge**: world position → armed
encounter → existing three-beat battle → world writeback. Nothing
new is simulated; everything already provable machinery
(`ApplyAftermath`, `BookDeeds`, `LogMythEvents`, ledger seal,
`CrossCheckRecord` gate) is reused.

**"a converted defeat returns the warband to the map" — resolved
interpretation:** the existing defeat-conversion path (4.5) is
"defeat settles as a chapter outcome; recovery is RefitCamp, not
a reload". For encounters the equivalent is: a lost field still
settles (roster buries, deeds post, `defeat` verdict seals), the
encounter marks resolved (no re-trigger death-loop — the warband
may still be standing on the node), the control stake does NOT
flip, and the warband stays put. No game-over, no dead end.

**Resolved-marker seam:** `Enqueue` requires `ev.node` to exist
in the map — encounter ids are their own id space, so they can't
ride the `Resolve` queue event. `WorldState` gains a direct
`MarkResolved(id)` accessor: the `resolved_` set already
serializes arbitrary strings (12.2 comment: "POI/encounter id"),
this just gives writers the pen. Queue events stay node-keyed.

## Design

### `potato.encounter/1` — `Campaign/World/Encounter.{h,cpp}`

```json
{
  "schema": "potato.encounter/1",
  "id": "longmen_ambush",
  "node": "longmen",
  "trigger": "arrival",
  "map": "ch01_plain",
  "defenders": [
    {"template": "militia", "region": 1,
     "deck": ["card_hold", "card_brace", "card_flee"]}
  ],
  "stakes": {"control": "player"},
  "seed": 0
}
```

- `id` ≤64 unique — also the `resolved_` marker key.
- `node` ≤64 required — region binding; existence is a MARSHAL
  check (the registry doesn't hold the world — same discipline as
  `WorldNode.map` refs).
- `trigger`: `"arrival"` (default) | `"proximity"` | `"poi"`.
- `map` optional ≤256 — battle-map ref; falls back to the node's
  own `map` field; neither → marshal fails.
- `defenders` required non-empty; each `{template ≤64, region int
  ≥0, deck 3..5 card-id strings ≤64}` — closed field set.
- `stakes` optional object; `control` ∈ the `WorldControl` wire
  vocabulary = control the node flips to when the PLAYER wins.
- `seed` optional int64 → u64 bitcast battle seed; absent →
  caller-supplied seed (e.g. a `WorldState::Draw()` taken at
  trigger resolution).
- Unknown top-level fields AND unknown defender fields reject
  (schema-gate convention from 12.4).

### `EncounterLibrary`

WorldLibrary/CharacterLibrary mirror: sorted dir scan, per-file
`rejected[]`, duplicate id rejects, canonical order by id, empty
dir ok, unreadable → `io`.

### Triggers — `PendingEncounters`

```cpp
std::vector<const EncounterDef*> PendingEncounters(
    const WorldState& ws, const WorldMap& map,
    const EncounterLibrary& lib);
```

Deterministic scan in library (id-sorted) order; skip when
`ws.IsResolved(enc.id)` or the node vanished. Fire rules:
- `arrival` — `ws.WarbandAt() == enc.node`
- `proximity` — node is AT the warband node or adjacent to it
  (hearsay pressure — banners on a neighboring road; a
  proximity encounter at the warband's own node fires too —
  you're in the ambush already)
- `poi` — warband at node AND node carries any `strategic` or
  `myth` flag bits (the POI quality)

Pure read: no mutation, no I/O.

### Marshal — `MarshalEncounter` + `DeployAssembly`

```cpp
struct PlayerDeploy {   // caller supplies the warband side
    const Gameplay::SquadTemplate* tmpl;
    std::size_t region;
    std::vector<std::string> deck; // card ids → SquadSheet
    std::string rosterName;        // casualty-mapping key
};
struct DeployRow {
    int side;                            // 0 player, 1 defender
    const Gameplay::SquadTemplate* tmpl; // resolved, lib-owned
    std::size_t region;
    Gameplay::SquadSheet sheet;          // built
    std::string rosterName;              // side 0 only
};
struct EncounterAssembly {
    const EncounterDef* enc;
    std::string mapId;
    Gameplay::FogConfig fog;   // commander prior applied (12.4)
    std::uint64_t seed;
    std::vector<DeployRow> rows; // side 0 (caller order) then
                                 // side 1 (doc order)
};
Gameplay::Result<EncounterAssembly> MarshalEncounter(
    const EncounterDef& enc, const WorldMap& world,
    const WorldState& ws, const Gameplay::BattleMap& battleMap,
    const Gameplay::SquadTemplateLibrary& squads,
    const Gameplay::DoctrineLibrary& cards,
    std::span<const PlayerDeploy> players,
    const Character* commander, std::uint64_t seed);
// Applies every DeploySquad + SetSheet during Planning;
// marshal prevalidated, so a false return is a caller bug.
bool DeployAssembly(Gameplay::BattleController& bc,
                    const EncounterAssembly& a);
```

Marshal rejects: unknown node, no map ref (enc or node), empty
players, bad template id, region ≥ battleMap.RegionCount(),
deck unbuildable. All checks before ANY output row.

### Settlement — `Campaign/World/EncounterSettle.{h,cpp}`

```cpp
struct EncounterSettlement {
    ChapterResolution resolution; // same verdict enum — the
                                  // ledger still decides first
    AftermathResult roster;
    std::size_t deedsPosted = 0;
    std::size_t mythLogged = 0;
    bool controlFlipped = false;
};
Gameplay::Result<EncounterSettlement> SettleEncounter(
    CampaignState& state, WorldState& ws, const WorldMap& world,
    const EncounterDef& enc, bool battleWon, int playerSide,
    std::span<const Gameplay::SimEvent> deeds,
    const std::vector<AftermathRow>& casualties,
    std::uint64_t recordRoot, MythLog* mythLog = nullptr);
```

Mirror `ResolveAftermath`'s ceremony minus the chapter:
1. Preflight (all before mutation): recordRoot ≠ 0; not already
   anchored (export `SettlementRecorded` from Aftermath.cpp —
   reuse, don't duplicate); playerSide ∈ {0,1}; `enc.node` must
   exist in `world`; ledger capacity for deeds + 1 seal; mythLog
   capacity for invasions; world queue has ≥1 free slot.
2. `ApplyAftermath` (roster) → `BookDeeds` (ledger) →
   `LogMythEvents` (infiltration deltas) — same order as 4.5.
3. Resolution: `FoldGovernance` thresholds override the field
   (same rule as ConcludeChapter — the ledger decides), then
   seal posting with the SAME leg table as `SealPosting` (copy
   the legs; memo `encounter <id> resolved: <res>`; tags
   `{"resolution:<res>", "encounter:<id>", "region:<node>",
   "record_root:<hex>"}` — no `chapter:` tag).
4. World writeback: `MarkResolved(enc.id)` ALWAYS (defeat
   included — prevents refire deadlock); on battleWon && stake
   → enqueue `SetControl` (day=Day, seq=NextSeq) then
   `ResolveBeats(world, 0)` drains it canonically.
5. Return the settlement; `resolution == Defeat` is a valid
   outcome — continuation, not game-over (the 4.5 conversion).

### `potato.mintclaim/1` — additive `settlement` kind

- `ClaimKind::Settlement`, wire `"settlement"`.
- Fields: `encounter` + `node` required (non-empty ≤64 each);
  `record_root` + `resolution` required (same rules as
  chapter_settlement); `chapter` FORBIDDEN; `achievement`
  forbidden.
- `chapter_settlement`/`achievement` unchanged; they now also
  FORBID `encounter`/`node` (closed-matrix).
- `ClaimId`: `"settlement-<hex>"` — same root-derived id as
  chapter_settlement (the root binds the battle; same record can
  never be claimed twice under either kind).
- ToJson emits `encounter`/`node` for Settlement, omits
  `chapter`.

### `ClaimGate` — `EmitGatedSettlement`

```cpp
Gameplay::Result<ClaimGateReport> EmitGatedSettlement(
    Ledger& ledger, const Gameplay::JsonValue& recordDoc,
    std::string_view resolution, std::string_view encounterId,
    std::string_view node, const MintOutbox& outbox);
```

Same gate: `CrossCheckRecord` must be Clean → emit one
`settlement` claim (no achievement claims — those are
chapter-keyed). Refusal is data, not an error.

### Explicit non-goals

- No auto-trigger inside `ResolveBeats` — the shell/UI calls
  `PendingEncounters` after beats; the world layer doesn't run
  battles itself.
- No player-side squad selection UI — `PlayerDeploy` is a
  caller parameter (12.9).
- No chapter binding of encounters (12.6 anchors 回目; encounter
  docs are the dynamic-encounter vehicle).
- No achievement claims from encounters (chapter-keyed system).
- No encounter-driven `MythState`/`Infiltration` seeding —
  `SeedInfiltration` at deploy time is a caller concern.

## Implementation Tasks

- [x] `Campaign/Chain/MintClaim.{h,cpp}` — `Settlement` kind,
      `encounter`/`node` fields, kind-field matrix, ClaimId
- [x] `Campaign/Chain/ClaimGate.{h,cpp}` — `EmitGatedSettlement`
- [x] `Campaign/World/WorldState.{h,cpp}` — `MarkResolved`
      accessor (+ MAX_RESOLVED bound)
- [x] `Campaign/Aftermath/Aftermath.{h,cpp}` — export
      `SettlementRecorded` for the encounter settle preflight
- [x] `Campaign/World/Encounter.{h,cpp}` — doc, library,
      `PendingEncounters`, `MarshalEncounter`, `DeployAssembly`
- [x] `Campaign/World/EncounterSettle.{h,cpp}` —
      `SettleEncounter` ceremony
- [x] `Examples/potato_test_encounter.cpp` + CMake registration
- [x] Build + ctest; story file + sprint-status sync

## Dev Agent Record

### Completion Notes List

- Mixed-authorship landing: MintClaim Settlement kind,
  `EmitGatedSettlement`, `MarkResolved`, `SettlementRecorded`
  export, and `Encounter.h` were landed by a parallel edit;
  this pass supplied `Encounter.cpp` (parse/library/pending/
  marshal/deploy), `EncounterSettle.{h,cpp}`, the test suite,
  and CMake registration.
- `MarshalEncounter` ships without the spec'd `WorldState&`
  param (the header the parallel edit committed omits it —
  resolved-encounter gating stays with `PendingEncounters`).
- `SettleEncounter` preflight adds two story-implied checks:
  seal-tag fit (`encounter:`/`region:` must land inside
  `Ledger::MAX_TAG_LEN`) and resolved-set capacity (new
  `WorldState::ResolvedCount()` accessor) — both required to
  keep the ceremony unfailable post-mutation.
- Defeat semantics verified: verdict seals `resolution:defeat`,
  no control flip, `MarkResolved` lands anyway (no refire
  deadlock), warband stays put — the 4.5 conversion.
- Encounter docs use closed field sets at all three levels
  (top / defender / stakes) — unknown keys reject.

### File List

- `Campaign/Chain/MintClaim.h` — settlement wire doc +
  `encounter`/`node` fields (parallel edit + dedupe)
- `Campaign/Chain/MintClaim.cpp` — Settlement kind matrix
  (parallel edit)
- `Campaign/Chain/ClaimGate.{h,cpp}` — `EmitGatedSettlement`
  (parallel edit)
- `Campaign/Aftermath/Aftermath.{h,cpp}` — `SettlementRecorded`
  exported (parallel edit)
- `Campaign/World/WorldState.{h,cpp}` — `MarkResolved` +
  `ResolvedCount` (parallel edit + accessor)
- `Campaign/World/Encounter.h` — schema/library/marshal decl
  (parallel edit)
- `Campaign/World/Encounter.cpp` — NEW: closed-field parse,
  `EncounterLibrary::Load`, `PendingEncounters`,
  `MarshalEncounter`, `DeployAssembly`
- `Campaign/World/EncounterSettle.{h,cpp}` — NEW:
  `SettleEncounter` ceremony (5-stage, preflighted)
- `Examples/potato_test_encounter.cpp` — NEW: 56 pins
- `Tools/MintRelayer/src/claim.mjs` — `settlement` kind ported
  (encounter/node required, chapter/achievement forbidden,
  shared root-derived id + anchor check) — the relayer is the
  minting path, so the new kind must validate there too
- `Tools/MintRelayer/test/relayer.test.mjs` — settlement-kind
  wire-parity pins
- `CMakeLists.txt` — `potato_test_encounter` registration

Verified MinGW/Ninja (`POTATO_BUILD_GUI=OFF`, junction
`C:\MingGoRTS`): `potato_test_encounter` 56/56 PASS;
`ctest` 24/24 incl. `gameplay_dep_guard`; relayer `node --test`
8/8 (incl. new settlement pins). MSVC not verified.

## Dev Notes — guardrails

- `SettlementRecorded` lives in Aftermath.cpp's anonymous
  namespace today — lift it to a named export in Aftermath.h so
  both ceremonies share the idempotency check.
- `SealPosting` legs (Victory.cpp anonymous namespace): copy the
  three-case leg table verbatim into EncounterSettle with a
  comment "must mirror Victory.cpp SealPosting legs" — duplicating
  15 lines beats exporting a Posting-shaped helper through
  Victory.h (same trade as the flag-table duplication precedent).
- `MarkResolved` bound: `resolved_.size() >= MAX_RESOLVED`
  rejects; already-resolved returns true (idempotent — second
  settle call is rejected earlier by the record anchor anyway).
- `playerSide` for encounters is conventionally 0 — still
  validate and pass through (BookDeeds gates on it).
- Marshal borrows: `tmpl`/`sheet`/`enc` pointers are library-
  owned — the assembly is valid only while the libraries live
  (same borrowed-ref contract as BattleController).
- `DeployAssembly` must run in Planning beat; it returns the
  deploy count expectation via rows.size().
- Wire spellings: `trigger` names `"arrival"/"proximity"/"poi"`;
  control names reuse DecodeControl vocabulary
  (`"neutral"/"player"/"rival"` — factor or copy the decoder;
  it's 12 lines, copying is precedent-consistent).
- No floats, no I/O in marshal/settle; File I/O only inside
  EncounterLibrary::Load.

## Validation

- `potato_test_encounter` green; `ctest` green incl.
  `gameplay_dep_guard`.
- Pins: marshal→deploy produces a Planning controller whose
  squads deploy on both sides; win flips control + marks
  resolved + seals `resolution:battle_victory`; defeat seals
  `resolution:defeat`, no flip, resolved marked, warband stays;
  re-settle with same recordRoot rejects (idempotent);
  `settlement` claim round-trips and emits through the gate on
  a Clean record only.
