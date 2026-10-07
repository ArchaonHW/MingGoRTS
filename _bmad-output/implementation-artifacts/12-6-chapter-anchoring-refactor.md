---
baseline_commit: NO_VCS
---

# Story 12.6 — Chapter Anchoring Refactor

> Epic 12 — 行營輿圖 Open Campaign World (W) · refactor ·
> `potato.chapter/1` additive `bind` · prerequisite graph ·
> **Status: review**

## Story (from epics.md)

As a developer,
I want ChapterLibrary entries bound to world predicates (region,
POI, ledger thresholds) instead of a linear sequence,
So that the 回目 are place-bound and mandatory beats stay anchored.

## Acceptance Criteria

- **Given** the existing ChapterLibrary, **when** entries carry
  world-bindings, **then** scenario availability resolves from
  world-state predicates — a prerequisite graph replaces the
  shell sequence
- **And** the `potato.chapter` schema gains bindings via an
  additive optional field or a clean version bump; unbound
  entries remain loadable
- **And** mandatory beats (commission, midpoint pivot, final
  confrontation) anchor to fixed regions or ledger thresholds
  per narrative-design.

## Context

Design authority: sprint-change-proposal-2026-10-06 (approved),
FR15 revised. "The schema/library work is additive; the
progression semantics change" — the proposal flags
**regression-verification needed**: `ResolveAndAdvance`,
`CanResolveChapter`, and the sentinel convention are pinned by
`ConcludeChapter` (Victory.cpp) and ~65 test sites in
potato_test_campaign.

**Schema decision — stay `/1`, no bump.** `ValidateChapter`
already tolerates unknown top-level members ("forward-compat",
ChapterLibrary.cpp:24). An optional `bind` object is additive
under that contract; unbound files parse byte-identically to
today. Inside `bind` the vocabulary is closed — it's new schema
surface, so strict keys (12.4/12.5 convention) while the
envelope stays tolerant.

**Unlock semantics — latch, not live.** `unlocked[]` is persisted
progress (potato.campaign/1). A chapter's predicates latch the
flag the first time they all hold — discovery is progress, the
marker doesn't fade when the warband marches on. This keeps
`unlocked` meaningful as "was ever made available" and makes
refresh monotone. Entry-side gating ("you must stand at the node
to actually fight") is the shell/encounter pipeline's business —
PendingEncounters already owns presence checks; this story owns
availability.

**`current` semantics — reinterpreted, not removed.** It becomes
"the chapter in play, or the next gate the campaign is waiting
on": lowest-index `unlocked && !resolved`; when none are
playable it points at the lowest-index unresolved chapter (the
next gate) so `CanResolveChapter`'s `!unlocked` rejection stays
the honest "not yet" signal; when the spine is done it lands on
the complete sentinel. For a pure-unbound library every case
collapses to exactly today's behavior — the old tests are the
regression net.

**Completion — mandatory spine.** A chapter with
`bind.mandatory`/`bind.beat` is on the spine: the campaign is
complete when every mandatory chapter is resolved (side content
stays playable). A library with NO mandatory markers completes
exactly as before — all resolved.

**Two resolved-namespaces stay separate.** `bind.requires` names
CHAPTER ids resolved via progress flags; `bind.resolved` names
WORLD `resolved_`-set ids (POIs, encounters — 12.2/12.5's set).
A shell settling a world-anchored chapter may also
`ws.MarkResolved(chapter.id)` so world predicates can key off
chapters too — caller's choice, documented, not automated.

## Design

### `potato.chapter/1` — additive `bind`

```json
{"schema":"potato.chapter/1","id":"ch02_longmen","index":2,
 "title":"龍門渡","map":"ch02_ford","combat":true,
 "bind":{"node":"longmen",
         "requires":["ch01_kaifeng"],
         "resolved":["dragon_gate_shrine"],
         "control":"player",
         "ledger":{"axis":"corruption","at_least":30},
         "mandatory":true,"beat":"pivot"}}
```

- `bind` optional object; absent → unbound (legacy linear
  chapter).
- `node` ≤64 — world node id; availability needs
  `ws.WarbandAt() == node` (arrival = discovery). Existence is a
  world-check, not a library-check (encounter `map` ref
  precedent).
- `control` ∈ `neutral|player|rival` — region-held predicate:
  `ws.ControlAt(node...)`. Evaluated against `bind.node` when
  present; when `control` appears without `node` it needs its
  own `control_node` — NO: keep it simple, `control` REQUIRES
  `node` (reject otherwise).
- `resolved` — array ≤32 of ids ≤64, each checked via
  `ws.IsResolved`.
- `requires` — array ≤32 of CHAPTER ids ≤64; prerequisite-graph
  edges. Validated against the library at load (fixpoint prune:
  dangling references reject the file, which can cascade).
- `ledger` optional object, closed keys `axis` + `at_least`;
  `axis` ∈ `popular_support|order|corruption` (the
  FoldGovernance vocabulary); `at_least` int ≥1.
- `mandatory` optional bool → spine membership.
- `beat` optional ∈ `commission|pivot|final` — mandatory implied;
  names the narrative-design beat for 6.9's ending gating.
- Unknown keys INSIDE `bind`/`bind.ledger` reject; unknown
  top-level keys still tolerated (file convention).

```cpp
enum class ChapterBeat : std::uint8_t { None, Commission,
                                        Pivot, Final };
struct ChapterBind {
    std::string node;
    WorldControl control = WorldControl::Neutral;
    bool hasControl = false;          // requires node
    std::vector<std::string> resolved;
    std::vector<std::string> requires;
    bool hasLedger = false;
    LedgerAxis axis = LedgerAxis::PopularSupport;
    std::int64_t atLeast = 0;
    bool mandatory = false;           // IsMandatory() |= beat
    ChapterBeat beat = ChapterBeat::None;
    bool IsMandatory() const {
        return mandatory || beat != ChapterBeat::None;
    }
};
enum class LedgerAxis : std::uint8_t { PopularSupport, Order,
                                       Corruption };
// ChapterDef gains:  bool bound = false; ChapterBind bind;
```

### `Progression.{h,cpp}` — the graph, beside the spine

```cpp
// Pure availability predicate. Unbound: index-predecessor
// resolved (sparse-safe — previous library entry, not index-1),
// or the lowest-index chapter. Bound: every `requires` chapter
// resolved AND every world predicate holds — node/control/
// resolved evaluate against `ws`+`world`; a null `ws` makes
// world predicates UNSATISFIABLE (no world, no place). `ledger`
// folds state.GetLedger() — works without a world.
bool ChapterAvailable(const ChapterDef& def,
                      const ChapterLibrary& lib,
                      const CampaignState& state,
                      const WorldState* ws,
                      const WorldMap* world);
// Latch pass: set unlocked[i] for every available unresolved
// chapter. Monotone — never clears a flag.
void RefreshAvailability(CampaignState& state,
                         const ChapterLibrary& lib,
                         const WorldState* ws,
                         const WorldMap* world);
// Any mandatory marker present → all-mandatory-resolved;
// else → all resolved (legacy).
bool CampaignComplete(const CampaignState& state,
                      const ChapterLibrary& lib);
// Shell's pick: `index` must name an unlocked, unresolved
// chapter. false leaves current untouched.
bool SetCurrentChapter(CampaignState& state,
                       const ChapterLibrary& lib,
                       std::int64_t index);
```

### `ResolveAndAdvance` — reinterpreted advance

Signature UNCHANGED (`(state, lib)` — ConcludeChapter untouched).

1. `CanResolveChapter` gate (unchanged).
2. `resolved[current] = true`.
3. `RefreshAvailability(state, lib, nullptr, nullptr)` — null
   world → only `requires`-only and `ledger` binds latch here;
   node/control/resolved binds wait for the world-beat refresh.
4. `current` := lowest-index `unlocked && !resolved`; none →
   `CampaignComplete` ? sentinel : lowest-index unresolved.

For a pure-unbound library each step reproduces today's bytes:
predecessor-rule availability unlocks exactly the next index;
lowest-available IS that index; all-resolved lands the sentinel.

`InitializeProgress(state, lib)` — same treatment: size vectors,
`RefreshAvailability(nullptr)`, `current` = lowest unlocked
(or lowest unresolved if nothing latched — degenerate content
still gets a sane coordinate). Pure-unbound → identical bytes.

### Where the world refresh runs

`RefreshAvailability(state, lib, &ws, &world)` is called by the
shell after world beats and after settlements — same seam class
as `PendingEncounters`/`ResolveBencaoUnlocks` (world-shell glue,
deferred-work registered). This story lands the machinery and
pins the null-world path inside `ResolveAndAdvance`.

## Implementation Tasks

- [x] `ChapterLibrary.{h,cpp}` — `ChapterBind`/`LedgerAxis`/
      `ChapterBeat`, `bound` flag on ChapterDef, `bind` parse
      (strict keys inside), fixpoint requires-prune in Load
- [x] `Progression.{h,cpp}` — `ChapterAvailable`,
      `RefreshAvailability`, `CampaignComplete`,
      `SetCurrentChapter`; rework `ResolveAndAdvance` +
      `InitializeProgress` internals (signatures unchanged)
- [x] `Examples/potato_test_campaign.cpp` — 12.6 section:
      bind decode + rejections, requires fixpoint prune,
      availability per predicate kind, latch monotonicity,
      graph out-of-order resolution, mandatory completion,
      ledger-axis predicate, null-world unsatisfiable;
      regression = existing linear pins must pass unchanged
- [x] Build + ctest; story file + sprint-status sync

## Dev Agent Record

### Completion Notes List

- `potato.chapter` stays `/1` — `bind` is additive under the
  file's forward-tolerant envelope; strict keys inside `bind`
  and `bind.ledger` (new vocabulary).
- `requires` wire key maps to `ChapterBind::prereqs` in C++ —
  `requires` is a language keyword.
- Fixpoint prune implemented: candidates with dangling
  `requires` move to `rejected` (naming the missing id), cascade
  until stable; real file paths preserved via a parallel vector.
- **node/control semantics resolved mid-build** (parallel pins):
  `node` alone = arrival gate (`WarbandAt() == node`);
  `node` + `control` = held-region gate where `node` is control's
  argument — arrival is NOT required ("holding the ground opens
  the book"). Region held can therefore latch while the warband
  is elsewhere.
- `unlocked` latch is monotone; `RefreshAvailability` only sets.
  `current` repoint = lowest playable, else lowest unresolved
  (next gate), else sentinel. All-linear pins pass byte-for-byte.
- `CampaignComplete`: any mandatory marker → all-mandatory-
  resolved; else all-resolved (legacy). `beat` implies mandatory.
- InitializeProgress runs a null-world refresh so requires/
  ledger binds and unbound openers latch at boot; node-bound
  waits for the shell's world refresh.
- Regression check: all pre-existing 3.4/4.x pins pass
  unchanged; mixed-attribution 12.6 section co-authored with a
  parallel session (both pin sets green).

### File List

- `Campaign/Chapters/ChapterLibrary.h` — bind types + fields
- `Campaign/Chapters/ChapterLibrary.cpp` — bind parse, strict
  keys, fixpoint requires-prune
- `Campaign/Chapters/Progression.h` — graph API + semantics doc
- `Campaign/Chapters/Progression.cpp` — `ChapterAvailable`,
  `RefreshAvailability`, `CampaignComplete`, `SetCurrentChapter`,
  reworked `ResolveAndAdvance`/`InitializeProgress`
- `Examples/potato_test_campaign.cpp` — 12.6 section
- `_bmad-output/implementation-artifacts/12-6-chapter-anchoring-
  refactor.md`, `sprint-status.yaml`

## Dev Notes — guardrails

- The regression risk IS the AC: `ResolveAndAdvance` semantics
  change only where binds exist. Run potato_test_campaign FIRST
  before touching Progression — it must stay green untouched
  (pure-unbound pins), then grow new pins.
- `bind.control` REQUIRES `bind.node` — a held-region predicate
  with no region is meaningless; reject at parse.
- `bind` inside-keys are STRICT (new vocabulary); the envelope
  stays forward-tolerant per the file's own comment.
- `requires` fixpoint: candidate set = all parsed chapters;
  iteratively drop candidates whose requires miss the set
  (dangling → rejected, cascade until stable). ≤64 chapters —
  trivial bounded loop. Reject reason names the missing id.
- `ws == nullptr` → world predicates UNSATISFIABLE, not skipped:
  a place-bound chapter can't be entered without the place.
- `LedgerAxis` folds via `FoldGovernance` — campaign-scope totals
  (per-region folds are 12.7's).
- `SetCurrentChapter` rejects resolved/locked/unknown index —
  the shell's pick is a choice among unlocked, never a bypass.
- `RefreshAvailability` is monotone: OR-into-unlocked only.
  Re-locking on departure would fight the persisted-flag model.
- Keep `ChapterProgress` untouched — save schema does not move.

## Validation

- `potato_test_campaign` green — ALL pre-existing linear pins
  unchanged; new 12.6 pins green; `ctest` green incl.
  `gameplay_dep_guard`.
- Pins: bound chapter unlocks on warband arrival (latched —
  stays unlocked after departure); `requires` graph resolves
  out-of-order (resolve ch02 before ch01's unlock — wait, no:
  `requires` gates entry, so verify requires-missing stays
  locked then unlocks when prereq resolves); control + ledger +
  resolved predicates each gate independently; mandatory spine
  completes without resolving optional chapters; unbound
  fallback completion unchanged; `SetCurrentChapter` picks
  among unlocked and rejects locked.
