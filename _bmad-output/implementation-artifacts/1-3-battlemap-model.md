---
baseline_commit: NO_VCS
---

# Story 1.3: BattleMap Model

Status: done

## Story

As a developer,
I want a tile/region battlefield model with terrain flags and strategic points (ferries, depots, villages),
So that battles have ground to fight over.

## Acceptance Criteria

1. **Given** a `potato.map/1` file,
   **When** loaded into `BattleMap`,
   **Then** regions, adjacency, and strategic-point flags are queryable.
2. **And** the map carries dual-layer marks (historical + shrine/myth) from the start.
3. **And Given** a malformed or invalid `potato.map/1` document,
   **When** loaded,
   **Then** `Result` carries an error reason and no partial `BattleMap` is produced.

## Tasks / Subtasks

- [x] Task 1 — Region + flag model (AC: 1, 2)
  - [x] `Gameplay/Map/BattleMap.h` — `struct Region` (string `id`, string `name`, uint32 bitmask fields) + `class BattleMap` (region array + per-region neighbor index list)
  - [x] Bitmask enums: `Terrain` (water/river/road/forest/highland/chokepoint/open), `Strategic` (ferry/depot/village), `HistMark` (grain_route/telegraph/supply_line), `MythMark` (shrine/spirit_road/haunted) — uint32, integer-only per determinism rules
  - [x] `IsStrategicPoint(Region)` helper — ferry|depot|village|shrine (GDD counts shrines as strategic points)
- [x] Task 2 — Loader `potato.map/1` (AC: 1, 2, 3)
  - [x] `BattleMap::Load(path)` → `Json::Load(path, "potato.map/1")` then `FromJson`
  - [x] `BattleMap::FromJson(const JsonValue&)` → `Result<BattleMap>`; validates: `regions` non-empty array of objects, unique string `id` per region, `name` optional string, flag arrays decode via string→bit tables (unknown token rejects with the token in the reason), `edges` array of `[id,id]` pairs (endpoints must exist, no self-edges, no duplicate edges either direction)
  - [x] Edges undirected: each `[a,b]` registers `b` in `Neighbors(a)` AND `a` in `Neighbors(b)`; neighbor order = edge declaration order (deterministic)
- [x] Task 3 — Query API (AC: 1, 2)
  - [x] `RegionCount()`, `RegionAt(i)`, `FindRegion(id)` → `const Region*` (nullptr on miss), `Neighbors(regionIndex)` → `const std::vector<std::size_t>&`, `RegionIndexOf(id)` for certainty-field indexing later (QuantumFog keys on region index)
- [x] Task 4 — Headless test `potato_test_map` (AC: 1, 2, 3)
  - [x] Temp `potato.map/1` file (3 regions, 2 edges, full flag coverage incl. CJK name), plus `FromJson` checks
  - [x] Rejects: wrong schema, regions empty, dup region id, edge endpoint missing, self-edge, unknown terrain/strategic/myth token, flag field not an array, bad center shape, duplicate edge both directions
  - [x] CMake target + `add_test`; removed `Gameplay/Map/.gitkeep`
- [x] Task 5 — Verify (AC: 1–3)
  - [x] MinGW via junction `C:\MingGoRTS`: build green, ctest 4/4
  - [x] `gameplay_dep_guard` green
  - [ ] MSVC: unverifiable in this environment (no `cl`) — disclosed

## Dev Notes

### Prior story intelligence (1.1, 1.2 — landed)

- `Result<T>` call shape: `r.ok()` method, `r.error`, `r.reason`; `Ok<T>(v)` / `Fail<T>(e,r)` helpers; `T` must be default-constructible (BattleMap default = empty map, fine).
- `Json::Load(path, expectedSchema)` gates io → parse → object root → string `schema` → exact match. Post-review it is hardened: UTF-8 path handling (CJK filenames OK), 64 MiB cap, UTF-16 explicit reject. Use it — do NOT open files yourself.
- `JsonValue` accessors: `IsObject/IsArray/IsString`, `FindString(key)` → `const std::string*`, `operator[]`, `Items()`, `Members()`, `AsInt/AsDouble/AsString` fallbacks. Object keys ordered via `std::map` + `std::less<>` (transparent string_view find — pass `string_view`, not `std::string`).
- `PotatoGameplay` lib globs `Gameplay/**/*.cpp` `CONFIGURE_DEPENDS` — new .cpp auto-pickup; new test exe needs `add_executable` + `add_test` at `CMakeLists.txt` end + a **re-configure** before `--target` build.
- Build via junction: `cd C:\MingGoRTS\build-mingw && mingw32-make potato_test_map` then `ctest --test-dir C:\MingGoRTS\build-mingw`.

### Schema decision (new — `potato.map/1`)

```json
{
  "schema": "potato.map/1",
  "id": "map_hefe",
  "name": "合肥外圍",
  "regions": [
    {"id": "north_ford", "name": "北渡", "terrain": ["river","road"],
     "strategic": ["ferry"], "historical": ["grain_route"], "myth": ["shrine"] },
    {"id": "south_bank", "name": "南岸", "terrain": ["river","open"],
     "strategic": ["village"] }
  ],
  "edges": [["north_ford", "south_bank"]]
}
```

- Regions are the node graph (architecture: "region/node graph" — NOT a tile grid; D-ARCH-7 fog keys on map regions). Optional `center: [x,y]` int pair allowed for later Plan/presentation use — validate type if present, ignore semantically now.
- Flag fields optional (absent = 0); present-but-wrong-type or unknown token = reject.
- `edges` optional (default empty).

### Architecture constraints that apply

- Integer-only in sim-reachable code — bitmasks + indices, no floats anywhere (determinism rule).
- Region **index** (file order) is the canonical key for downstream systems (`CertaintyField` per-region, BattlePlan coverage) — expose it; string `id` is the content reference only.
- Boundary code: `Result<T>` everywhere, never throw, no exceptions.
- No file I/O outside `Load`; `FromJson` is pure — failed validation must not mutate the target (build into a local, return by move).
- This model is immutable after load (registries load at boot, immutable during battle).

### LLM-trap warnings

- Do NOT build a tile grid — architecture says region/node graph; tiles are a presentation concern (Epic F).
- Do NOT put movement costs/pathfinding in this story — model + load + query only (1.4/1.9 consume it).
- Do NOT make edges directed or auto-symmetrize silently — undirected by construction, both directions registered explicitly.
- Do NOT `catch(...)` — parser and loader are `Result`-only.
- Do NOT create `assets/` — test fixtures are temp files inside the test.
- Flag tables: keep the string→bit maps `constexpr`-style arrays in the .cpp anonymous namespace; unknown token rejection must name the token in `reason`.

### References

- [Source: _bmad-output/planning-artifacts/epics.md — Epic 1 / Story 1.3]
- [Source: _bmad-output/game-architecture.md — `Gameplay/Map` location, D-ARCH-7 region-keyed fog, determinism/no-float rules, registry immutability]
- [Source: _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md — Map and Terrain (dual-layer lists, strategic points), Two worlds one map pillar]
- [Source: Gameplay/Json/Json.h, JsonValue.h — post-review API]
- [Source: 1-2 story file — Result call shape, CTest wiring, junction build]

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

- `std::minmax` needs `<algorithm>` — first MinGW build failed on it; added include.

### Review Findings

- [x] [Review][Patch] `RegionAt`/`Neighbors` unchecked indexing — `NO_REGION` fed in = UB; precondition docs + `assert` added (all 3 layers flagged) [BattleMap.h]
- [x] [Review][Patch] `FromJson` bypassed version gate — now verifies exact `potato.map/1` when a `schema` key is present (absent = pre-gated/test DOM) [BattleMap.cpp]
- [x] [Review][Patch] non-string `name`/`id` silently ignored or misreported — `Has`+`IsString` distinguishes missing vs wrong-type, better reasons [BattleMap.cpp]
- [x] [Review][Patch] `["x","x"]` reported self-edge before unknown-endpoint — endpoint resolution now precedes self-edge check [BattleMap.cpp]
- [x] [Review][Patch] story-file schema example was malformed JSON (dup `edges`, stray `]`, dangling region ref) — fixed
- [x] [Review][Patch] test hardening — `reject` lambda early-out on parse failure (distinct failure, no spurious pass), `ofstream` open checked, +21 checks: non-object root, missing/non-string ids+names, schema-tag reject via FromJson, edges arity/non-array, same-direction dup, lenient-policy pins (edges:null/[], unknown fields, dup flag tokens), neighbor ORDER, default-map safety, failure purity `RegionCount==0`, missing-file io [potato_test_map.cpp]
- [x] [Review][Patch] `<cstddef>`/`<cassert>` includes made explicit [BattleMap.h]
- [x] [Review][Defer] unscoped bitmask enums allow cross-domain mixing (`r.terrain & MYTH_SHRINE` compiles) — scoped-enums refactor deferred; fields are raw uint32 by design
- [x] [Review][Defer] `RegionIndexOf` O(n) linear scan; `indexOf` map discarded post-load — fine at tens-of-regions scale; rebuild/retain only if measured hot
- [x] [Review][Defer] `IsStrategicPoint` hardcodes capturable-bit mask — intentional enumeration of which assets count; revisit if a 4th STRATEGIC bit lands
- [x] [Review][Defer] id hygiene (whitespace/NUL/invalid-UTF-8 accepted) — hand-authored content; JsonValue raw-byte leniency already a deferred item
- [x] [Review][Defer] duplicate flag tokens silently OR'd — idempotent, harmless; strictness intentionally on structure not redundancy

### Completion Notes List

- `BattleMap` = region/node graph per architecture (no tile grid); regions keyed by file-order index (canonical key for CertaintyField/BattlePlan), string `id` is content reference only.
- Flag bitmasks as uint32 enums: `Terrain`(7) `Strategic`(3) `HistMark`(3) `MythMark`(3); `IsStrategicPoint` counts MYTH_SHRINE per GDD "ferries, depots, shrines".
- `potato.map/1` loader: `Load(path)` gated via `Json::Load`; `FromJson` validates region uniqueness, flag tokens (names token in reason), optional `[int,int]` center, undirected edges (endpoints exist, no self/duplicate), schema tag re-checked when present. Builds into local then moves — no partial map on failure.
- Post-review: `RegionAt`/`Neighbors` carry assert + documented preconditions; id/name type strictness; endpoint-resolution before self-edge check.
- 51 checks green; ctest 4/4 (`potato_test_gameplay`, `potato_test_json`, `potato_test_map`, `gameplay_dep_guard`).
- MSVC still unverified (no `cl`); code is stdlib-only.

### File List

- `Gameplay/Map/BattleMap.h` (new)
- `Gameplay/Map/BattleMap.cpp` (new)
- `Examples/potato_test_map.cpp` (new)
- `CMakeLists.txt` (modified — potato_test_map target + add_test)
- `Gameplay/Map/.gitkeep` (deleted — real files landed)
