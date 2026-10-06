---
baseline_commit: NO_VCS
---

# Story 12.1 — WorldMap Schema & Registry

> Epic 12 — 行營輿圖 Open Campaign World (W) · `potato.world/1` ·
> **Status: done**

## Story (from epics.md)

As a developer,
I want `potato.world/1` world documents — a region graph (nodes,
routes, POIs), faction-control flags, dual-layer tags — loaded into a
boot-time registry,
so that the campaign has a geography to march across.

## Acceptance Criteria

- **Given** a `potato.world/1` file, **when** loaded via the content
  pipeline, **then** regions, routes, adjacency, and POI flags are
  queryable, and bad schema/version rejects without mutating state
- **And** region ids are the single scheme shared by governance tags,
  myth infiltration, and encounter map references — no parallel id
  space
- **And** dual-layer marks (historical layer + shrine POI bindings)
  exist from the start.

## Context

Design authority:
`_bmad-output/planning-artifacts/sprint-change-proposal-2026-10-06.md`
(approved) — Epic 12 adds the open campaign world: battles stay
encounter-based inside the existing three-beat loop; the world layer
is campaign-scope state, NOT a second tick sim.

This story lands the **static geography only**: the `potato.world/1`
schema, the `WorldMap` model, and the `WorldLibrary` boot-time
registry. World runtime state (warband position, control changes,
beat resolution) is 12.2+; encounter marshalling is 12.5; chapter
anchoring is 12.6; regional governance/myth binding is 12.7 — do not
build any of those here.

`WorldLibrary` mirrors `ChapterLibrary` (3.3) / `BencaoLibrary`
(10.1): boot-time, immutable, per-file-isolated registry. The map
model itself mirrors `BattleMap` (1.3): a region/node graph with
typed flag sets — NOT a tile grid (presentation concerns live in
Epic F / story 12.9).

## Design

`Campaign/World/WorldMap.{h,cpp}` + `WorldLibrary` — the world map is
campaign-layer content (D-ARCH-10): it is read at campaign
boundaries, never inside the battle tick, and lives outside
`Gameplay/` by construction. `Campaign` may include only `Gameplay`
public headers.

### Wire shape — `potato.world/1`, one file per world

```json
{
  "schema": "potato.world/1",
  "id": "republic_fall",
  "name": "河山殘卷",
  "nodes": [
    {"id": "kaifeng", "name": "開封",
     "terrain": ["road", "open"], "strategic": ["depot"],
     "historical": ["grain_route"], "myth": [],
     "control": "player", "map": "ch01_plain",
     "center": [12, 8]},
    {"id": "longmen_shrine", "name": "龍門祠",
     "terrain": ["highland"], "myth": ["shrine"],
     "control": "neutral", "center": [15, 4]}
  ],
  "routes": [{"a": "kaifeng", "b": "longmen_shrine", "days": 2}],
  "start": "kaifeng"
}
```

- `nodes` — non-empty array; each node is a world region or POI.
  `id` unique in-file (the canonical campaign-scope region id);
  `name` optional UTF-8 display name.
- Flag fields `terrain`/`strategic`/`historical`/`myth` reuse the
  **BattleMap token vocabularies** verbatim (water/river/road/…,
  ferry/depot/village, grain_route/telegraph/supply_line,
  shrine/spirit_road/haunted) — one flag language for both layers.
- `control` — initial faction control: closed enum
  `player`/`rival`/`neutral` (default `neutral` when absent).
  Runtime control drift is WorldState's job (12.2), not this file.
- `map` — optional content ref (a `potato.map/1` id): the battle map
  an encounter on this node marshals into (12.5). Validated for
  shape only — cross-registry existence is 12.5/12.6's concern.
- `center` — optional `[x, y]` int pair, presentation hint, no world
  semantics (BattleMap precedent).
- `routes` — array of `{"a","b","days"}`; endpoints must be known
  node ids; undirected; `days` is the march cost (integer ≥ 1);
  duplicate/self/unknown-endpoint routes reject.
- `start` — required: id of the warband's starting node (12.3's
  spawn anchor); must name a known node.

### Model

```cpp
enum class WorldControl : std::uint8_t { Neutral, Player, Rival };

struct WorldNode {
    std::string id;                 // canonical campaign region id
    std::string name;
    std::uint32_t terrain = 0;      // Gameplay TERRAIN_* bits
    std::uint32_t strategic = 0;    // STRATEGIC_* bits
    std::uint32_t historical = 0;   // HIST_* bits
    std::uint32_t myth = 0;         // MYTH_* bits
    WorldControl control = WorldControl::Neutral;
    std::string map;                // potato.map/1 ref (may be empty)
    std::int64_t centerX = 0, centerY = 0;
    bool hasCenter = false;
};

class WorldMap {                    // one potato.world/1 doc
public:
    static Gameplay::Result<WorldMap> FromJson(const Gameplay::JsonValue&);
    std::size_t NodeCount() const;
    const WorldNode& NodeAt(std::size_t i) const;   // precond-checked
    const WorldNode* FindNode(std::string_view id) const; // null on miss
    std::size_t NodeIndexOf(std::string_view id) const;   // NO_NODE
    // Neighbors(i) -> vector<pair<nodeIndex, days>> — route cost is
    // part of the adjacency, not a side table.
    const std::vector<std::pair<std::size_t, std::int64_t>>&
        Neighbors(std::size_t i) const;
    const std::string& Id() const;  const std::string& Name() const;
    std::size_t StartIndex() const;                    // `start` node
};

class WorldLibrary {                // ChapterLibrary precedent
public:
    static constexpr std::string_view SCHEMA = "potato.world/1";
    static constexpr std::size_t MAX_WORLDS = 64;
    static constexpr std::size_t MAX_NODES = 256;
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::size_t MAX_NAME_LEN = 128;
    static constexpr std::size_t MAX_REF_LEN = 256;
    static constexpr std::int64_t  MAX_ROUTE_DAYS = 30;

    static WorldLoadResult Load(const std::filesystem::path& dir,
                                WorldLibrary& out);
    const WorldMap* Find(std::string_view id) const;
    std::size_t Size() const;
};
```

`WorldLoadResult{ok,error,reason,rejected[]}` + `RejectedWorld`
mirror the `ChapterLoadResult`/`BencaoLoadResult` shape exactly.

### Validation rules (file-is-untrusted)

- `schema` must equal `potato.world/1` exactly → `schema`.
- Required: `id` (non-empty, ≤ MAX_ID_LEN), `nodes` (non-empty array,
  ≤ MAX_NODES), `start` (names a node). `routes` optional.
- Node: `id` required/unique; unknown flag tokens reject (BattleMap
  strictness, not silent ignore); `control` must be one of the three
  spellings; `center` must be `[int,int]` when present.
- Route: both endpoints resolve to known nodes; `days` in
  [1, MAX_ROUTE_DAYS]; duplicate (unordered pair) and self-routes
  reject.
- Duplicate world `id` across files → second file rejected.
- Per-file isolation: bad file → `rejected[]`, library continues;
  unreadable dir → `ok=false, error="io"`; empty dir → empty ok.

### Region-id scheme (the AC's core)

World node ids ARE the canonical campaign-scope region id space:
ledger region tags (12.7), world-myth keys (12.7), and encounter map
refs (12.5) all key on `WorldNode.id` strings. This does NOT collide
with battle-local ids: `BattleMap::Region.id` is file-local and
`MythState` persists `(chapterId, map-local int index)` — a distinct
scope by construction. Do NOT "unify" by rewriting MythState or
BattleMap here; the 12.7 story adds world-node keying as an additive
dimension.

### Explicit non-goals

- No WorldState, movement, or beat resolution (12.2/12.3).
- No encounter marshalling or `map` ref resolution (12.5).
- No chapter bindings (12.6), no governance/myth writes (12.7).
- No rendering, no save integration (12.8/12.9).
- No `assets/world/` content authoring — test fixtures only.

## Implementation Tasks

- [x] `Campaign/World/WorldMap.{h,cpp}` — `WorldNode`, `WorldMap`
      (FromJson + queries), flag decode, `WorldControl` enum
- [x] `Campaign/World/WorldLibrary.{h,cpp}` — dir-scan registry,
      per-file `rejected[]`, `io` on unreadable dir
      (or fold into WorldMap.cpp if small — ChapterLibrary keeps
      them in one file pair per type)
- [x] `Examples/potato_test_world.cpp` — temp-dir fixtures
      (`fs::temp_directory_path()` + `ofstream` + `remove_all`,
      the `potato_test_campaign`/`potato_test_bencao` pattern)
- [x] `CMakeLists.txt` — `add_executable(potato_test_world …)` +
      `add_test` linking `PotatoCampaign` (copy the
      `potato_test_bencao` block); glob `CONFIGURE_DEPENDS` picks up
      `Campaign/World/` automatically
- [ ] Review pass, sprint-status sync

## Dev Notes — guardrails

- **Layering**: `Campaign/World/` may include only `Gameplay/`
  public headers (`Json/Json.h`, `Json/JsonValue.h`, `Result.h`,
  `Map/BattleMap.h` for the flag enums); never Rendering/GUI —
  `scripts/CheckGameplayDeps.ps1` / `gameplay_dep_guard` enforces.
  **Do not link `PotatoEngine`** (MinGW breakage — AGENTS.md).
- **Flag tables**: BattleMap's decode tables live in an anonymous
  namespace inside `BattleMap.cpp` — not reusable as-is. Either lift
  them into a small shared header (`Gameplay/Map/MapFlags.h`) or
  duplicate the tables in `WorldMap.cpp`; the vocabularies MUST stay
  byte-identical either way (Bencao.cpp `IsTerrainFlag` is an
  existing duplication precedent). Prefer the shared header only if
  it stays a pure-include refactor of BattleMap.
- **Errors**: `Result<T>{value,error,reason}`; never throw; failed
  validation mutates nothing (per-file rejection leaves the library
  untouched — ChapterLibrary contract).
- **Load**: file load goes through `Json::Load(path, SCHEMA)` (BOM
  strip + schema gate built in); `FromJson` on a DOM also accepts
  pre-gated test docs — absent `schema` tolerated, present must
  match (BattleMap convention).
- **No third-party JSON** — `JsonValue` only
  (`FindString`/`AsInt`/`AsString`/`Items`/`Members`; defensive
  accessors return fallbacks, so check `Has`/`Is*` for required
  fields).
- **Bounds**: every string/array/int field needs a `MAX_*` wire
  bound — a load must not admit content no write path could
  produce.
- **Determinism**: trivially satisfied — no PRNG, no clock; sorted
  filename order + in-file declaration order are canonical.
- **UTF-8/CJK**: `JsonValue` accepts raw ≥0x80 bytes (deferred
  leniency) — CJK node names inherit it; no validation pass here.
- **Naming**: PascalCase files/types/methods, camelCase members,
  `UPPER_SNAKE` constants, `Potato::Campaign` namespace.
- **Tests**: name `potato_test_world`; pin valid load (node
  queries, route adjacency with days, start index), each rejection
  (bad schema, dup node id, unknown flag, bad control, self/dup/
  unknown-endpoint route, bad days, missing start, dup world id,
  empty dir ok, unreadable dir `io`).
- **Build env**: exec shell is non-functional here (`ls`/`uv`
  unavailable) — inspect via file tools; MinGW builds go through the
  `C:\MingGoRTS` ASCII junction (CJK repo path breaks MinGW make);
  headless via `-DPOTATO_BUILD_GUI=OFF`.

## Validation

- `potato_test_world` green; `ctest` green incl.
  `gameplay_dep_guard` (no new Gameplay→Campaign edge).
- Verify on MSVC; MinGW via the ASCII junction if buildable.

## Dev Agent Record

**Implemented 2026-10-06.**

- `Campaign/World/WorldMap.{h,cpp}` — single file pair holds
  `WorldNode`/`WorldControl`/`WorldMap` **and** `WorldLibrary`
  (folded; ChapterLibrary keeps one file pair per type).
- Flag decode duplicates BattleMap's anonymous-namespace tables
  (documented in-file; bit values come from `BattleMap.h` so the
  vocabularies share one constant source). A wholesale
  `kMaxRoutes` bound was added on top of the sketched bounds.
- `start` resolves to `StartIndex()`; routes store `{neighbor,
  days}` pairs in declaration order, undirected.
- `WorldLibrary::Load` copies the ChapterLibrary dir-scan contract:
  sorted filenames, case-folded `.json`, per-file `rejected[]`,
  unreadable dir → `io`, world count > `MAX_WORLDS` → wholesale
  `overflow` (Bencao precedent), canonical order = world id.
- Test: `potato_test_world` — 19-pass fixture decode block, ~30
  reject pins, lenient-acceptance pins, `Json::Load` gate checks,
  temp-dir registry cases (bad-file isolation, dup id, empty dir,
  unreadable dir, canonical order).

### Completion Notes List

- **Verified 2026-10-06 (MinGW/Ninja, `POTATO_BUILD_GUI=OFF`)**:
  `potato_test_world` 73/73 checks pass; `ctest` 22/22 pass incl.
  `gameplay_dep_guard` — no new Gameplay→Campaign edge.
- Full `cmake --build` (all targets) still fails on the pre-existing
  `AI/LLMIntegration.cpp` MinGW issue (`memset` missing
  `<cstring>`) — engine leaf, unrelated to this story; game-layer
  targets are unaffected.
- MSVC not verified this session (environment is MinGW only).

### File List

- `Campaign/World/WorldMap.h` (new)
- `Campaign/World/WorldMap.cpp` (new)
- `Examples/potato_test_world.cpp` (new)
- `CMakeLists.txt` (test target registration)
