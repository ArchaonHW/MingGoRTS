---
baseline_commit: NO_VCS
---

# Story 12.3 — Warband & Movement

> Epic 12 — 行營輿圖 Open Campaign World (W) · extends `potato.worldstate/1` ·
> **Status: done**

## Story (from epics.md)

As a player,
I want my warband — the commander plus attached roster squads —
to march along map routes on orders I issue,
so that I choose where the chronicle goes next.

## Acceptance Criteria

- **Given** a warband on a region node, **when** I issue a move
  order along an adjacent route, **then** the warband arrives
  after the route's day cost and 物資 supply drains per balance
  JSON
- **And** rival warbands appear only as hearsay/certainty
  markers — the truth boundary holds at strategic scale
- **And** arrival/proximity events queue into the beat resolution
  deterministically.

## Context

Design authority: sprint-change-proposal-2026-10-06 (approved).
12.1 landed the geography (`WorldMap` + routes with `days`);
12.2 landed the runtime spine (`WorldState` + ordered event
queue). This story lands the **player-facing verb**: issue a
march order → supply posts to the ledger → arrival resolves
through the event queue, deterministically.

**"per balance JSON" — resolved interpretation:** no
`potato.balance` doc exists today; RefitCamp (3.6) prices actions
from code-resident band tables, and that is the established
precedent. 12.3 ships `MarchRules` as code-resident constants
(`kSupplyPerDay`), exposed as a parameter so a future balance doc
can supply the number without touching the mechanic. Logged in
deferred-work as "balance JSON doesn't exist; constant pending
content pass."

**Hearsay, not truth:** rival warbands exist only as sighting
markers — "word reached the camp that banners moved through
X." A `Sight` event records node + day + count; no rival entity
holds a live position the player can read. Same boundary as
RivalDeck/Dossier: uncertainty is the data.

### Schema addition (same doc, still v1 — never shipped)

`potato.worldstate/1` gains, additive-only:

- `"seq": <u64>` — monotonic emission counter for canonical
  event ordering (12.2 let callers pass seq; movement needs a
  state-owned counter so emits never collide).
- `"sightings": {"<node>": {"day": <int>, "count": <int>}}` —
  hearsay marks. Multiple sightings of the same node bump count;
  day keeps the freshest.
- `queue[].kind` vocabulary grows: `"march"` (node = destination),
  `"sight"` (node = sighted location). `control`/`resolve`
  unchanged.

## Design

### `Campaign/World/March.{h,cpp}` — the order verb

```cpp
struct MarchPlan {
    std::string from;
    std::string to;
    std::int64_t days;    // route cost
    std::int64_t supply;  // days * rules.supplyPerDay
};

struct MarchRules {
    std::int64_t supplyPerDay = 5; // code-resident until a
                                   // balance doc exists
};

// Issue a march order. All checks pass before ANY mutation:
// dest must be adjacent to WarbandAt(), warband must not already
// be marching, Balance(Materiel) must cover supply. On success:
// posts {debit Materiel, credit ArmyPrestige, memo "march
// <from>→<to>", tags {"march","region:<to>"}} and enqueues a
// March event at day+days with the state's own seq counter.
// Atomic: any failure leaves ledger and state untouched.
Gameplay::Result<MarchPlan> IssueMarch(
    WorldState& ws, const WorldMap& map, Ledger& ledger,
    std::string_view dest, const MarchRules& rules = {});

// Record hearsay: rival banners sighted at `node`. Enqueues a
// Sight event for the current day (sights apply immediately on
// the next beat — or ResolveBeats(0) drains same-day events).
Gameplay::Result<bool> IssueSighting(
    WorldState& ws, const WorldMap& map, std::string_view node);
```

### WorldState amendments (additive)

- `WorldEventKind` += `March`, `Sight` (wire `"march"`/`"sight"`).
- `NextSeq()` — returns `nextSeq_`++ — the state-owned emission
  counter, persisted as `"seq"`.
- `Marching()`/`MarchEta()` — a warband mid-march shows
  destination + ETA for UI/encounter checks. March event carries
  destination in `node`; in-flight state = "a March event is
  pending" — tracked via a `marchingTo_`/`marchEta_` pair set at
  IssueMarch, cleared on apply.
- `Sightings()` — read-only view of the hearsay map.
- Apply rules: `March` → `warband_ = node` (clears in-flight);
  `Sight` → upsert `sightings_[node] = {day_, count+1}`.
- Node-vanished tolerance: March/Sight on an edited-out node
  skips (marching warband stays put — it never reached a place
  that no longer exists).

### Determinism & boundaries

- Arrival goes through the SAME event queue — beats decide when
  things land, nothing teleports.
- Seq counter is per-state and persisted: replays of the same
  order sequence produce identical queues.
- Ledger posting happens at order-issue (the cost is committed
  when the order is given — the army eats whether or not it
  survives the road). Region tag `region:<dest>` threads the
  12.7 regional-binding seam.
- No floats, no I/O, no exceptions. `Ledger::Post` failure
  (capacity) rejects the order before enqueue.

### Explicit non-goals

- No pathfinding/multi-leg routes — orders are single-hop along
  one edge; chaining is a UI convenience for 12.9.
- No encounter triggers on arrival — 12.5 reads WarbandAt
  changes.
- No rival warband AI/positions — sightings are inert data until
  a later story gives them emitters.
- No character/commander binding (12.4), no save envelope (12.8).

## Implementation Tasks

- [x] `Campaign/World/WorldState.{h,cpp}` — add `March`/`Sight`
      kinds, `nextSeq_`/`marchingTo_`/`marchEta_`/`sightings_`,
      wire fields `"seq"`/`"sightings"`/kind spellings, apply
      rules, `NextSeq()`/`Marching()`/`Sightings()` accessors
- [x] `Campaign/World/March.{h,cpp}` — `MarchRules`, `MarchPlan`,
      `IssueMarch`, `IssueSighting`
- [x] `Examples/potato_test_world.cpp` — 12.3 block: order →
      ledger debit + queue → beats → arrival; insufficient 物資
      rejects atomically; non-adjacent rejects; double-march
      rejects; sightings accumulate; seq monotonic; round-trip
      preserves in-flight march + sightings + seq
- [x] Build + ctest; story file + sprint-status sync

## Dev Agent Record

**Implemented 2026-10-06.**

- `Campaign/World/March.{h,cpp}` — `IssueMarch` (adjacency →
  already-marching → affordability → `Ledger::Post` → `Enqueue`
  → `BeginMarch`, reject-before-mutate throughout) and
  `IssueSighting` (same-day `Sight` event; `ResolveBeats(0)`
  drains it). `MarchRules::supplyPerDay = 5` is code-resident per
  the RefitCamp band-table precedent (no `potato.balance` doc
  exists yet — deferred-work note in Context).
- `WorldState` additive: `March`/`Sight` event kinds (wire
  `"march"`/`"sight"`), `NextSeq()` emission counter persisted as
  `"seq"` (explicit field wins; queue-derived floor tolerates
  older saves), in-flight `marchingTo_`/`marchEta_` pair
  (`"marching"` object), `sightings_` hearsay map (`"sightings"`
  object, `{day,count}` — freshest day wins, count accumulates).
- `Enqueue` floors `nextSeq_` past any queued `ev.seq` — hand-
  authored events can't make the counter replay an id, which
  keeps ToJson/FromJson a fixed point (caught by the
  byte-identical-serialization pin).
- March apply sets `warband_` and clears the in-flight marker;
  sight upserts `{day_, count+1}`. Vanished-node events skip
  per the 12.2 tolerance rule.
- Supply posts `debit Materiel / credit ArmyPrestige` at order-
  issue (the army eats whether or not it survives the road);
  tags `{"march","region:<dest>"}` thread the 12.7 regional fold.

### Completion Notes List

- **Verified MinGW/Ninja (`POTATO_BUILD_GUI=OFF`)**:
  `potato_test_world` all-green incl. 12.3 pins (plan fields,
  supply posting, tags, in-flight marker, arrival via beats,
  atomic rejects, hearsay counts, mid-march round-trip, seq
  continuity); `ctest` 22/22 incl. `gameplay_dep_guard`.
- MSVC not verified this session.

### File List

- `Campaign/World/March.h` (new)
- `Campaign/World/March.cpp` (new)
- `Campaign/World/WorldState.h` (March/Sight kinds, seq counter,
  in-flight march pair, sightings)
- `Campaign/World/WorldState.cpp` (apply rules + wire fields +
  Enqueue seq floor)
- `Examples/potato_test_world.cpp` (12.3 block)

## Dev Notes — guardrails

- `Ledger::Post` — check `Balance(Account::Materiel)` BEFORE
  posting (RefitCamp affordability pattern); Posting legs:
  debit Materiel / credit ArmyPrestige, tags `{"march",
  "region:<id>"}`.
- WorldState.h already includes Ledger? No — March.h takes the
  `Ledger&` param so WorldState stays ledger-free.
- Reject-before-mutate order: adjacency → marching check →
  affordability → Post → Enqueue → set in-flight fields.
- `Enqueue` still validates node/day/capacity; March passes the
  auto seq via `NextSeq()`.
- Wire `seq` field: absent on load = derive `max(existing seqs)+1`
  so older saves stay compatible (they're all from this same
  dev window anyway — still, tolerate).

## Validation

- `potato_test_world` green incl. 12.3 pins; `ctest` green.
- Determinism pin: identical order sequence → identical emitted
  bytes (Emit canonical).
