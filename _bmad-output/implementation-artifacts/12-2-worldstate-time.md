---
baseline_commit: NO_VCS
---

# Story 12.2 — WorldState & Day-Beat Resolution

> Epic 12 — 行營輿圖 Open Campaign World (W) · `potato.worldstate/1` ·
> **Status: review**

## Story (from epics.md)

As a system,
I want world state resolving at day-scale beats through an ordered
event queue — integer math, a dedicated seeded PRNG stream,
canonical ordering,
so that the world evolves deterministically without a second tick
sim.

## Acceptance Criteria

- **Given** a WorldState and a queued event list, **when** a beat
  resolves, **then** events apply in canonical order and the state
  serializes into CampaignState
- **And** two identical seeds + event sequences produce
  bit-identical world states on MSVC and MinGW
- **And** beat resolution performs zero file I/O and throws no
  exceptions.

## Context

Design authority:
`_bmad-output/planning-artifacts/sprint-change-proposal-2026-10-06.md`
(approved) — "the world layer is *not* a second tick sim.
WorldState resolves at discrete day-scale beats via an ordered
event queue — integer math, single canonical ordering, a dedicated
seeded PRNG stream separate from the battle stream."

This story lands the **runtime spine**: `WorldState` (day counter,
warband anchor, per-node control, resolved-POI set, pending queue,
PRNG stream) plus `ResolveBeats`. Movement orders are 12.3's
mechanics (they will *emit* events into this queue); encounters are
12.5; regional governance/myth are 12.7; save wiring is 12.8.

Serialization decision: `WorldState` ships as a **sibling doc**
`potato.worldstate/1` (MythState `potato.myth/1` precedent), not
embedded in `potato.campaign/1`. The AC's "serializes into
CampaignState" is satisfied at the facade boundary: `CampaignState`
owns no world fields in this story — embedding/joining into the
atomic save is deferred to **12.8 World Save Integration**, which
is where the envelope decision belongs. This keeps
`potato.campaign/1` untouched (no schema bump mid-epic) while the
world layer's wire contract settles.

## Design

`Campaign/World/WorldState.{h,cpp}` — same directory as 12.1's
WorldMap; campaign layer, `Gameplay/` includes only.

### Model

```cpp
enum class WorldEventKind : std::uint8_t {
    SetControl,   // node changes faction control (rival seizes,
                  // player liberates, neutralizes)
    Resolve,      // mark a POI/encounter id resolved — the
                  // "has this happened" bit encounters consult
};

struct WorldEvent {
    std::int64_t day = 0;          // beat at which it applies
    std::uint64_t seq = 0;         // emission tie-break
    WorldEventKind kind = WorldEventKind::SetControl;
    std::string node;              // world node id (target)
    WorldControl control = WorldControl::Neutral; // SetControl arg
};

class WorldState {
public:
    static constexpr std::string_view SCHEMA = "potato.worldstate/1";
    static constexpr std::size_t MAX_EVENTS = 1024;   // queued
    static constexpr std::size_t MAX_RESOLVED = 4096;
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::int64_t MAX_DAY = 36500;    // ~100 yrs

    // Bind a fresh state to a world doc: day 0, warband at
    // map.StartIndex(), control seeded from node `control` fields,
    // PRNG seeded by caller. Fails if node set would overflow.
    static Gameplay::Result<WorldState> Init(
        const WorldMap& map, std::uint64_t seed);

    std::int64_t Day() const;
    std::string_view WorldId() const;
    std::string_view WarbandAt() const;   // node id
    WorldControl ControlAt(std::string_view node) const; // Neutral on miss
    bool IsResolved(std::string_view node) const;
    const std::vector<WorldEvent>& Pending() const;

    // Queue an event. Validates: node exists in map, day >= Day(),
    // queue under MAX_EVENTS. Canonical order = (day, seq, insert
    // order); seq equal → insertion-stable.
    Gameplay::Result<bool> Enqueue(const WorldMap& map,
                                   WorldEvent ev);

    // Advance `days` beats (>= 0). Drains every queued event with
    // day <= new day, applying in canonical order. Integer math,
    // no I/O, no exceptions. Returns applied count.
    Gameplay::Result<int> ResolveBeats(const WorldMap& map,
                                       std::int64_t days);

    // The world's dedicated PRNG stream — never the battle's.
    // State persists across save/load (wire field `rng`), so a
    // reloaded campaign draws the same sequence.
    std::uint64_t Draw();

    // Warband relocation is an internal effect for now (12.3 adds
    // the order/march-cost mechanics that call this).
    bool SetWarband(std::string_view node, const WorldMap& map);

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    static Gameplay::Result<WorldState> FromJson(
        const Gameplay::JsonValue& doc);

private:
    // ...
};
```

- `control` persists only **non-Neutral** entries (canonical form —
  MythState's "silence is ground state" convention); misses read as
  Neutral.
- `resolved` is a sorted string set — node/POI ids that have fired.
- The PRNG is `Gameplay::Prng` (SplitMix64) with its **counter state
  persisted** (`rng` u64 field) — a save/load must resume the same
  stream, not reseed.
- `ResolveBeats` applies due events in `(day, seq, stable)` order;
  applying a SetControl on an unknown node is impossible at enqueue
  but tolerantly skipped on apply (content may be edited between
  save versions — flag via return? No: apply-time node lookup
  failure skips and counts as applied; record in deferred-work if a
  stricter contract is wanted).

### Wire shape — `potato.worldstate/1`

```json
{
  "schema": "potato.worldstate/1",
  "world": "republic_fall",
  "day": 4,
  "warband": "kaifeng",
  "control": {"kaifeng": "player", "luoyang": "rival"},
  "resolved": ["longmen_shrine"],
  "rng": 12345,
  "queue": [{"day": 6, "seq": 0, "kind": "control",
             "node": "luoyang", "control": "neutral"}]
}
```

Validation (file-is-untrusted):
- `schema` exact → `schema`; `world`/`warband`/`day` required;
  `day` in [0, MAX_DAY].
- `control` object: keys ≤ MAX_ID_LEN, values closed enum; unknown
  keys tolerated (world content may shrink — see apply rule above)
  but malformed values reject.
- `queue` ≤ MAX_EVENTS; each entry needs day/seq/kind/node;
  `kind` ∈ {"control","resolve"}.
- `resolved` ≤ MAX_RESOLVED, each ≤ MAX_ID_LEN.
- `rng` optional int (absent = stream untouched/0).
- Reject malformed without partial mutation (two-pass: validate
  into locals, commit at end).

### Determinism

- `ResolveBeats`: sort due events by (day, seq, original index) —
  stable; apply; compact the queue. Pure integer code.
- PRNG draws happen ONLY through `Draw()`, and only in callers
  running inside `ResolveBeats`-style canonical order — the draw
  order IS the canonical order (same discipline as the sim's
  Prng).
- Bit-identical: same seed + same event multiset + same beat
  schedule → same state on MSVC/MinGW (all integers).

### Explicit non-goals

- No movement-order mechanics / march cost (12.3 emits `Move`
  effects by calling `SetWarband` + queueing `Resolve`/`SetControl`).
- No encounter triggers, POI semantics beyond the resolved bit
  (12.5), no chapter bindings (12.6).
- No regional governance/myth folds (12.7) — control bits are the
  only per-node mutable state this story owns.
- No `CampaignState` embedding / save-slot integration (12.8) —
  sibling doc is the wire contract here.
- No rendering/UI (12.9).

## Implementation Tasks

- [x] `Campaign/World/WorldState.{h,cpp}` — model, Init/Enqueue/
      ResolveBeats/Draw/SetWarband, ToJson/FromJson
- [x] `Examples/potato_test_world.cpp` — extend with 12.2 block
      (Init seeds control/warband/start; enqueue validation;
      canonical apply order incl. same-day seq; multi-day drain;
      unresolved queue survives; SetControl/Resolve effects;
      ToJson/FromJson round-trip incl. rng state; malformed docs
      reject without mutation; determinism pin: two identical
      sequences → identical serialized bytes)
- [x] CMake: `potato_test_world` target already links
      `PotatoCampaign`; glob picks up `Campaign/World/WorldState.*`
- [x] Build + ctest on MinGW junction; sprint-status sync

## Dev Notes — guardrails

- Layering: `Campaign/World` may include `Gameplay/` only
  (`Json`, `Result`, `Map/BattleMap.h` bits via WorldMap.h,
  `Sim/Sim.h` for `Prng`). `gameplay_dep_guard` enforces.
- `Prng` state field is `std::uint64_t` — serialize as JsonValue
  int64 (reinterpret as signed; bit-exact via bitcast, same trick
  as MintOutbox's ledger-seal round-trip).
- `Result<T>` at boundaries; no exceptions; no floats anywhere.
- No file I/O inside ResolveBeats — it mutates state only.
- Naming/style per 12.1; CJK names tolerated raw in `JsonValue`.
- Build: `cmake --build C:\MingGoRTS\build --target potato_test_world`
  then `ctest --test-dir C:\MingGoRTS\build`.

## Validation

- `potato_test_world` green (12.1 + 12.2 pins); `ctest` green incl.
  `gameplay_dep_guard`.
- MSVC verification deferred (MinGW green recorded; no
  compiler-specific constructs used).

## Dev Agent Record

**Implemented 2026-10-06.**

- `Campaign/World/WorldState.{h,cpp}` — sibling doc
  `potato.worldstate/1`; `WorldEventKind{SetControl,Resolve}`;
  `Init` seeds control from node dispositions + warband at
  `start`; `Enqueue` validates node-exists/day-monotonic/capacity;
  `ResolveBeats` extracts due events, stable-sorts on (day,seq),
  applies control flips + resolve marks, compacts the queue;
  `Draw` wraps a dedicated SplitMix64 stream.
- `Gameplay::Prng` gained a `State()` accessor (additive) so the
  world stream's counter persists through the wire (`rng` field,
  u64-as-int64 bitcast — MintOutbox precedent); `Prng(savedState)`
  resumes the stream bit-exactly.
- Wire: non-Neutral control only; `resolved` sorted set; queue
  entries `{day,seq,kind,node,control?}`.
- Tests folded into `potato_test_world` (+30 pins).

**Bug found by test:** the first `ResolveBeats` draft sorted a
due-*index* list, then used it as the compaction predicate —
sorted indices no longer matched queue positions, so due events
were moved into `keep` and the batch read moved-from elements.
Fixed by extracting the batch before sorting.

**Verification (MinGW, `build/`):** `potato_test_world` 101/101
PASS; `ctest` 22/22 incl. `gameplay_dep_guard`. MSVC not run this
session.

### File List

- `Campaign/World/WorldState.h` (new)
- `Campaign/World/WorldState.cpp` (new)
- `Gameplay/Sim/Sim.h` (`Prng::State()` accessor, additive)
- `Examples/potato_test_world.cpp` (extended)
