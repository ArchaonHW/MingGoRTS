---
baseline_commit: NO_VCS
---

# Story 10.2 — Bencao Unlock Engine

> Epic 10 — Bencao Codex (BC) · `potato.bencao_state/1` ·
> **Status: done**

## Story (from epics.md)

As a player,
I want codex pages to unlock from what my campaign actually did —
terrain fought over, deeds booked, shrines pacified — so the book
grows out of my history,
so that the collection is a record of play, not a checklist.

## Acceptance Criteria

- **Given** a per-entry trigger manifest (worldview §4), **when**
  campaign-layer state settles at chapter end, **then** the engine
  resolves matching entries deterministically in a fixed evaluation
  order — reading ledger entry tags (`atrocity`, `resolution:*`,
  `chapter:*`), DeedBook deeds, MythState infiltration, RosterEntry
  veterancy/casualties (the `scars` field does not exist yet;
  續斷/骨碎補 key on veterancy thresholds until it lands), and
  chapter-map terrain via the `ChapterDef.map` ref's `potato.map/1`
  flags **and** the unlocked set plus the pending 補鈔 queue persist
  in campaign state — embedded in `potato.campaign` via schema bump
  or as sibling `potato.bencao_state/1` (decided at implementation;
  same seam as RivalBook), round-tripping losslessly **and** all
  reads are read-only against ledger/roster/myth — no tick-path
  calls, no writes outside the codex store.

## Context

10.1 landed `BencaoLibrary` — the registry with validated unlock
manifests. This story adds the *engine* that turns manifests into
unlocks, plus the persistent codex state (unlocked set + pending
補鈔 queue). 補鈔 delivery/marginalia is 10.3; this story provides
the queue and a drain API for it.

Persistence decision taken here: **sibling doc `potato.bencao_state/1`**,
matching the `potato.myth/1` / `potato.rivals/1` precedent —
independent schema, `potato.campaign` untouched (embed stays
available if the save format later consolidates).

## Design

`Campaign/Narrative/BencaoCodex.{h,cpp}`:

```cpp
// Read-only signal bundle the caller assembles at chapter settle.
// Everything is a view — the engine writes nothing outside `codex`.
struct CodexSignals {
    const Ledger* ledger = nullptr;     // ledger_tag + corruption
    const MythLog* mythLog = nullptr;   // myth_state: action membership
    const MythState* myth = nullptr;    // myth_state: "infiltrated"
    const std::vector<RosterEntry>* roster = nullptr; // governance "veteran"
    std::string_view chapterId;         // chapter just settled
    std::int64_t chapterIndex = -1;     // for chapter_close (-1 = none)
    std::vector<std::string> terrains;  // terrain flags on the map
};
```

Trigger evaluation (per `BencaoEntry::unlockKind`):

| Kind | Evaluates against |
|---|---|
| `terrain` | `signals.terrains` contains `unlockParam` |
| `ledger_tag` | any `LedgerEntry.tags` carries `unlockParam` (byte-exact) |
| `governance` | any entry carries tag `resolution:<event>` or bare `<event>` |
| `myth_state` | `"infiltrated"` → `myth->HasChapter(chapterId)`; otherwise any MythLog entry `action == param` |
| `corruption` | `FoldGovernance(*ledger).corruption >= unlockInt` |
| `chapter_close` | `unlockInt == -1` (every settle) or `== chapterIndex` |

`veterancy`-keyed unlocks (the 續斷/骨碎補 deferral): not a new
kind — content expresses them as `governance` events whose producer
side lands with RefitCamp; roster reads are exposed via signals for
that future producer, but this story evaluates the six kinds only.

`BencaoCodex` — the persistent side, sibling doc:

```
{"schema":"potato.bencao_state/1",
 "unlocked":["sanqi",...],        // insertion order = unlock order
 "pending":["fuzi",...]}          // 補鈔 queue, FIFO for 10.3
```

API:

```cpp
class BencaoCodex {
    static constexpr std::string_view SCHEMA = "potato.bencao_state/1";
    bool IsUnlocked(std::string_view id) const;
    const std::vector<std::string>& Unlocked() const;
    const std::vector<std::string>& Pending() const;
    // Drain up to `max` pending ids (the 10.3 delivery seam);
    // returns the drained ids in queue order.
    std::vector<std::string> TakePending(std::size_t max);
    Result<JsonValue> ToJson() const;
    static Result<BencaoCodex> FromJson(const JsonValue& doc);
};

// The engine. Iterates the library in canonical order, unlocks
// every entry whose trigger matches the signals, appends to
// `codex` (unlocked + pending). Idempotent — already-unlocked ids
// skip. Returns the ids newly unlocked (canonical order).
Result<std::vector<std::string>>
ResolveBencaoUnlocks(const BencaoLibrary& lib,
                     const CodexSignals& signals,
                     BencaoCodex& codex);
```

Helper for the terrain seam:

```cpp
// Distinct TERRAIN_* flag names present on the map — lowercase ids
// ("water","river","road","forest","highland","chokepoint","open").
// Caller assembles CodexSignals::terrains from the chapter map.
std::vector<std::string>
TerrainFlagsOf(const Gameplay::BattleMap& map);
```

### Rules

- **Null signals mean "not triggered", never "crash"** — a null
  `ledger` makes `ledger_tag`/`corruption`/`governance` simply
  false; a null `mythLog`/`myth`/`roster` likewise. `chapter_close`
  works without any store at all.
- **Idempotent**: re-running the same signals unlocks nothing twice;
  `pending` never duplicates.
- **Determinism**: iteration order is `lib.Entries()` order
  (category-then-id) — the evaluation order is the canonical order.
- **Read-only**: the function takes everything const; the only
  mutation is `codex`. Zero sim/tick involvement by construction.
- **Wiring seam (deferred)**: who calls this at chapter settle is the
  chapter-shell's business (same class of deferral as RivalBook's
  "no production call site" note). `ResolveAftermath` is untouched;
  the Game shell assembles `CodexSignals` — loading the chapter's
  map for `TerrainFlagsOf` — when a chapter-resolution UI pass
  exists. Recorded in deferred-work.

## Implementation Tasks

- [x] Story scaffold: `Campaign/Narrative/BencaoCodex.{h,cpp}`
- [x] `BencaoCodex` — unlocked/pending stores, TakePending drain,
      `potato.bencao_state/1` To/FromJson round-trip
- [x] `ResolveBencaoUnlocks` + `TerrainFlagsOf` — six-kind
      evaluation, null-signal tolerance, idempotency
- [x] `potato_test_bencao` — extend the same exe with a 10.2 block
      (one test binary per content area is the established shape)
- [x] Review (three passes), sprint-status sync

## Dev Agent Record

Implementation landed as designed, with one semantic correction
found by the tests: **pending and unlocked are disjoint**. A
trigger fires a page into the `pending` 補鈔 queue only —
`TakePending` is the transcription move that writes it into the
book (`unlocked`). Rationale: `unlocked` is the citable set
(Story 10.5) — a page the book doesn't hold can't be quoted, and
hearsay precedes codex confirmation.

- `myth_state` semantics pinned: `"infiltrated"` →
  `MythState::HasChapter(chapterId)`; any other param → MythLog
  `action` membership (campaign-cumulative — MythLog isn't
  chapter-keyed, and cumulative memory is correct for a codex).
- `governance` events resolve against `resolution:<event>` seal
  tags first, then bare `<event>` deed tags.
- `TerrainFlagsOf` emits the seven `TERRAIN_*` flag ids in fixed
  flag order (deterministic regardless of region order).
- Verified: `potato_test_bencao` 36/36 pins green; `ctest` 20/20
  incl. `gameplay_dep_guard`. Full `--build` still hits the
  pre-existing `AI/NaturalLanguageProcessing.cpp` MinGW error —
  unrelated, unchanged.

## Dev Notes — guardrails

- Reuse: `FoldGovernance` (Accumulators.h), `Ledger::Entries()`,
  `LedgerEntry::tags`, `MythLog::Entries()` (`MythLogEntry.action`),
  `MythState::HasChapter`, `Region::terrain` + `TERRAIN_*` flags.
  **Do not** re-fold balances or re-scan files — callers own I/O.
- `ChapterDef.map` is a content ref string; this story never loads
  map files itself — `TerrainFlagsOf` takes an already-loaded map.
- Persist via `ToJson`/`FromJson` only; no filesystem in this unit.
- Bounds: unlocked/pending vectors bounded by `BencaoLibrary::
  MAX_ENTRIES` on FromJson; dedupe on load (first occurrence wins).
- Everything else per 10.1 Dev Notes (layering, Result, no engine
  link, naming, MinGW junction build, no working bash shell).

## Validation

- `potato_test_bencao` — new block covering: each trigger kind
  fires/silences correctly, null-signal tolerance, idempotent
  re-run, pending FIFO drain, state round-trip, bad schema reject.
- `ctest` green incl. `gameplay_dep_guard`.

### Review Findings

Reviewed 10.1+10.2 (4 layers, 2026-10-05):

- [x] [Review][Decision] Pending carries no trigger provenance — **decided: extend wire now.** `pending` carries `{id, kind, detail}` (matched tag / action / flag / corruption level / chapter index); `ComposeMarginalia` (10.3) consumes it via `%P` + pool keying on `page.kind`.
- [x] [Review][Decision] Dead-param validation breadth — **decided: full strict.** `terrain` → 7 flag ids; `myth_state` → `infiltrated`/`invasion` ∪ `MythActionDefs` ids; `governance` event ≤ `MAX_TAG_LEN − len("resolution:")` (53); `ledger_tag` ≤ `MAX_TAG_LEN`; `chapter_close` ∈ [-1, `MAX_CHAPTERS`-1]; `corruption.at_least` ≥ 1.
- [x] [Review][Patch] FromJson: union bound `unlocked+pending ≤ MAX_ENTRIES` + per-id non-empty/≤`MAX_ID_LEN` enforced; pending `kind` validated via `UnlockKindFromName` [BencaoCodex.cpp]
- [x] [Review][Patch] Test fixtures added: required-field matrix, `lang` reject, `aliases`/`origin` both directions, `unlock` int bounds, `governance` bare-tag path (`g_deed`/`epidemic`), uppercase `.JSON`, malformed `bencao_state` docs, union overflow, provenance pins
- [x] [Review][Patch] `lang` docs corrected — "must not assert zh-tw: false" [Bencao.h, Bencao.cpp]
- [x] [Review][Patch] Annotation guard now rejects `"批註"`/`"批注"` CJK spellings [Bencao.cpp]
- [x] [Review][Patch] Terrain vocab + `Ledger::TAG_RESOLUTION` constant in the seal path [Bencao.cpp, BencaoCodex.cpp]
- [x] [Review][Patch] `EvalCtx` precomputes ledger-tag set + `FoldGovernance` + myth-action set once per resolve [BencaoCodex.cpp]
- [x] [Review][Patch] Wording: "newly triggered" (queued, not yet in the book), `unlocked` = delivery order, `chapterId` documented as call-scoped view [BencaoCodex.h]
- [x] [Review][Patch] Direct includes added — `<utility>` BencaoCodex.cpp, `<utility>`/`<system_error>` Bencao.cpp, `<string>` test
- [x] [Review][Defer] `MAX_ENTRIES` overflow fixture — 1025 fixture files too heavy for this Check style; failure mode is loud wholesale rejection
- [x] [Review][Defer] Unbounded `files`/`rejected` vectors in Load — systemic pattern shared with ChapterLibrary; hostile-dir resource bound is a loader-class issue, not introduced here
- [x] [Review][Defer] `CodexSignals::roster` dead pointer / no veterancy kind / DeedBook unread / settled-chapter-id seam — extends the recorded scars + wiring deferrals; veterancy producer lands with RefitCamp
- [x] [Review][Defer] Persisted ids not reconciled against library — unknown ids silently persist; a `Reconcile(lib)` prune is a design choice for 10.3+

Rejected: `Json::Load` schema error code (false — verified returns `"schema"`); `TakePending` negative→size_t underflow (low — size_t contract); unchecked Find/indexing in tests (low — suite convention); "null signals" vs `chapter_close` firing (false — documented + pinned exception); `unlockInt` default −1 vs spec's 0 (rejected — fix edits spec; −1 is the semantic sentinel); `< 0` vs `== -1` (low — equivalent post-validation); FromJson non-array `Items()` silently empty (false — `IsArray` guards precede; fixture gap covered above).
