---
baseline_commit: NO_VCS
---

# Story 12.7 — Regional Governance & Myth Binding

> Epic 12 — 行營輿圖 Open Campaign World (W) · region-id
> unification · derived folds ·
> **Status: done**

## Story (from epics.md)

As a system,
I want ledger events carrying region ids with per-region
民心/秩序 folds, and world POIs bound to shrine entities,
So that governance and myth live on the geography.

## Acceptance Criteria

- **Given** booked events with region tags, **when** accumulators
  fold, **then** per-region 民心/秩序 derive alongside the
  campaign totals — additive, derived state never stored
- **And** region tags thread through GovernanceField events and
  myth infiltration keys.

## Context

**The collision this story resolves** (change-proposal's
"region-id unification" design task): TWO namespaces currently
share the `region:` tag prefix.

- Battle-map-local region indices (`region:<int>`): DeedBook.cpp
  `RegionTag`, MythActions.cpp — map-local region 0..N of the
  battle's own map. Meaningless at campaign scope.
- World node ids (`region:<node-id>`): March.cpp,
  EncounterSettle.cpp — the `potato.world/1` node namespace.

A fold keyed on `region:` alone would misattribute `region:2`
(map-local) to a world node literally named `2`. Resolution:

- `region:<id>` is RESERVED for world node ids — the single
  canonical key a per-region governance fold reads.
- Battle-map-local regions rename to `field:<n>` — the tag now
  says which space it belongs to. (Ledger content is
  producer-side vocabulary; no wire schema moves — in-flight
  saves carry old tags harmlessly since nothing folded them.)
- `Ledger::TAG_REGION`/`TAG_FIELD` constants pin the vocabulary.

**World attribution of battle deeds**: `BookDeeds` gains an
optional `worldNode` — a world-anchored battle's deeds stamp
`region:<node>` alongside their `field:<n>` (both facts are
true: it happened at 龍門, on field-region 2). Linear-chapter
battles pass nothing (no world). `EncounterSettle` supplies the
node — that's the "threads through GovernanceField events" AC.

**Myth binding**: `MythState`'s `byChapter_` key is already a
bare string. The unified place-key convention: the key names the
PLACE — an unbound chapter keys by chapter id; world-bound myth
(shrine POIs, node-anchored battles) keys by node id. One id
space, no `node:` prefix (parallel spaces are what this story
exists to kill). `MythPlaceKey(def)` resolves a chapter's key:
`bind.node` when bound+node, else `id`. Shrine POIs
(`node.myth & MYTH_SHRINE`) key by node id directly; their
world-scale infiltration lives in slot 0 of the node's set.

## Design

### `Ledger.h` — canonical prefixes

```cpp
static constexpr std::string_view TAG_REGION = "region:"; // world
static constexpr std::string_view TAG_FIELD  = "field:";  // map-local
```

### `Accumulators` — `FoldGovernanceByRegion`

```cpp
// Per-world-node folds derived from `region:<id>`-tagged
// entries: a tagged entry's PopularSupport legs net into the
// region's 民心, its order:/corruption: tags into its 秩序/墮落
// (same saturating + per-(axis,value)-dedupe rules as the
// campaign fold). Map order = tag order of first appearance —
// std::map id-sorted for determinism. Untagged entries feed only
// the campaign totals. Derived state — never stored.
std::map<std::string, GovernanceAccumulators>
FoldGovernanceByRegion(const Ledger& l);
```

### `DeedBook` — world attribution

`BookDeeds(ledger, playerSide, events, std::string_view
worldNode = {})` — non-empty `worldNode` appends
`"region:"+node` to each deed posting. Callers preflight
MAX_TAGS room is not needed: deed postings carry ≤4 tags, cap is
16. Map-local `RegionTag` → `FieldTag` emitting `field:<n>`.

### `MythActions` — tag rename

`region:<int>` → `field:<int>` (map-local), matching DeedBook.

### `Campaign/World/RegionBind.{h,cpp}` — the bridge

```cpp
// Canonical world-region ledger tag for a node id.
std::string RegionTagFor(std::string_view node);
// The place-key a chapter's myth writes key under: bound+node →
// node id; unbound → chapter id.
std::string_view MythPlaceKey(const ChapterDef& def);
// A node that carries myth-shrine flags is a shrine POI; its
// world-scale infiltration keys under the node id at region
// slot 0 (MythState keys are place ids — one space).
inline bool IsShrinePoi(const WorldNode& n);
// World-scale shrine level read/write — slot-0 convention.
int ShrineLevel(const MythState& s, std::string_view node);
bool SetShrineLevel(MythState& s, std::string_view node,
                    int level);
```

### Callers

- `EncounterSettle`: `BookDeeds(..., enc.node)` — encounter
  deeds attribute to the node (its seal already carries
  `region:<node>`).
- `Aftermath.cpp` (linear path): unchanged — `worldNode`
  defaults empty.
- `March.cpp`/`EncounterSettle.cpp`: literal `"region:"` →
  `Ledger::TAG_REGION` constant (no behavior change).

## Implementation Tasks

- [x] Ledger.h TAG_REGION/TAG_FIELD constants
- [x] Accumulators FoldGovernanceByRegion
- [x] DeedBook worldNode param + field: rename
- [x] MythActions field: rename
- [x] RegionBind.{h,cpp} bridge helpers
- [x] EncounterSettle passes node to BookDeeds
- [x] potato_test_ledger `region:N`→`field:N` pins + fold pins
- [x] potato_test_world 12.7 section
- [x] Build + ctest; story + sprint-status sync

## Dev Agent Record

### Completion Notes List

- **Namespace unification landed**: `region:<id>` is reserved for
  world node ids; battle-map-local regions renamed to `field:<n>`
  (DeedBook RegionTag→FieldTag, MythActions, plus the 4 existing
  ledger-test pins updated to `field:`).
- `Ledger::TAG_REGION`/`TAG_FIELD` constants pin the vocabulary;
  March/EncounterSettle switched to the constant.
- `BookDeeds` gains optional trailing `worldNode` — stamps
  `region:<node>` on every deed posting. `EncounterSettle`
  supplies `enc.node`; the linear Aftermath path defaults empty.
- `FoldGovernanceByRegion` (parallel session implementation,
  kept): per-entry fold once, distribute to every distinct
  `region:` tag; PS legs net per region; order/corruption tags
  per region with the same dedupe/saturation; per-region
  corruption ratchet peaks independently. Untagged entries feed
  no region.
- `Campaign/World/RegionBind.{h,cpp}` (parallel session):
  `RegionTagFor`, `MythPlaceKey` (bound→node, unbound→chapter),
  `IsShrinePoi`, `ShrineLevel`/`SetShrineLevel` slot-0.
- Myth place-key convention: one id space — bound chapters and
  shrine POIs key myth under the NODE id; the shrine remembers
  the ground, not the book that burned it.
- Fixed a duplicate `FoldGovernanceByRegion` declaration in
  Accumulators.h from a merge collision.

### File List

- `Campaign/Ledger/Ledger.h` — TAG_REGION/TAG_FIELD constants
- `Campaign/Ledger/DeedBook.{h,cpp}` — worldNode param, field:
  rename
- `Campaign/Myth/MythActions.cpp` — field: rename
- `Campaign/Governance/Accumulators.{h,cpp}` —
  FoldGovernanceByRegion
- `Campaign/World/RegionBind.{h,cpp}` — bridge helpers (NEW)
- `Campaign/World/EncounterSettle.cpp` — node → BookDeeds +
  TAG_REGION
- `Campaign/World/March.cpp` — TAG_REGION
- `Examples/potato_test_ledger.cpp` — field: pins (parallel)
- `Examples/potato_test_world.cpp` — 12.7 section (fold, stamp,
  place key, shrine level)
- `_bmad-output/implementation-artifacts/12-7-*.md`,
  `sprint-status.yaml`

## Dev Notes — guardrails

- `region:` is now a RESERVED world-node namespace: producers of
  map-local ids must use `field:`; content may not name a world
  node a bare integer (it would collide with nothing — `field:`
  holds the ints — but keep the convention documented).
- FoldGovernanceByRegion must NOT include untagged entries
  (they're campaign-scope) and must NOT dedupe regions across
  entries — same rules as the campaign fold otherwise.
- `MythPlaceKey` returns the node id for bound chapters — myth
  belongs to the PLACE; two chapters anchored at one node share
  its myth state (intended: the shrine remembers, not the book).
- Shrine world-scale level uses region slot 0 — a documented
  convention, not a new schema.
- BookDeeds' worldNode is additive-with-default: existing
  signatures at Aftermath.cpp and all test sites stay valid.

## Validation

- `potato_test_ledger` + `potato_test_world` + full ctest green
  incl. `gameplay_dep_guard`.
- Pins: `field:<n>` on deed/myth postings; `region:<node>`
  stamped when worldNode supplied; per-region fold derives
  民心/秩序/墮落 for tagged entries only; MythPlaceKey bound/
  unbound; ShrineLevel slot-0 round-trip.
