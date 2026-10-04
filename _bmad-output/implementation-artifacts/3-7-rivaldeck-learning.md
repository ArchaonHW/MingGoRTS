# Story 3.7: RivalDeck Learning

## Status: done

## Story

As a player,
I want enemy generals to learn my habitual triggers across
chapters and field counter-decks,
So that predictability is my hardest enemy.

## Acceptance Criteria

**Given** three chapters of recorded player doctrine usage,
**When** a rival prepares chapter N+1,
**Then** its deck includes counter-cards biased against my
most-used triggers
**And** learning data lives in Campaign/Rivals, difficulty
scaling via counter-deck depth only.

## Design

`Campaign/Rivals/RivalDeck.{h,cpp}` — `potato.rivals/1`.

- `GeneralDossier` — the hearsay model: `id`, personality
  `prior`, observed `chaptersObserved`, and a per-TriggerKind
  histogram. Aggregates only — no per-battle detail, matching
  "enemy generals exist only as hearsay" (never omniscient).
- `RivalBook::RecordChapter(id, prior, usage)` — folds one
  chapter's observed CardFired-by-trigger histogram into the
  dossier (creates it on first sight).
- `RivalBook::PrepareCounterDeck(id, counters, depth)` — below
  MIN_CHAPTERS_TO_LEARN (3) the hearsay hasn't formed: empty
  deck. Past it: triggers sorted by usage desc (ties by
  ordinal — deterministic), each maps to `counter.<trigger>`
  and only ids resolving in the caller's counter
  DoctrineLibrary are emitted, capped at `depth`.
- **Difficulty = depth only** — no stat scaling; the same
  counter-cards, just more of the player's habits answered.
- `potato.rivals/1` ToJson/FromJson with full validation
  (unique ids, prior enum, nonnegative bounded counts).
  The histogram is a NAME-KEYED object (`"always":5`), not a
  positional array — a TriggerKind insert/reorder can't
  misattribute saved counts; missing keys read as 0
  (forward-compat), mistyped values reject.
  `kTriggerKindCount` is `static_assert`ed against the enum.

Counter-card id convention: `counter.always`,
`counter.cohesion_below`, `counter.enemy_in_region`,
`counter.enemy_adjacent` — the counter pool itself is content
(Epic 9 card pool). Wiring the rivals doc into a save slot
(second file or campaign/2 embed) is deferred — the doc
persists standalone today.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `RivalDeck.{h,cpp}`: dossier, book,
    record, prepare, serialization
- [x] Task 2 — tests: sub-3-chapter empty deck, 3-chapter
    biased ordering, depth cap, unknown counters skipped,
    deterministic ties, round-trip, validation rejects
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- Three-layer review (AC:PASS) fixes:
  - **Positional-array wire format scrapped** (Med): a
    TriggerKind insert/reorder would silently misattribute
    saved counts. Triggers now serialize name-keyed;
    `kTriggerKindCount` is static_asserted.
  - **`counter.unknown` fallback killed**: a new TriggerKind
    without a wire name is skipped, never emitted.
  - **`RecordChapter` returned a `GeneralDossier*` into the
    vector** — dangling on the next `push_back`. Now returns
    the index; `Find` documents pointer lifetime.
  - **Create-before-overflow-check ordering** — a rejected
    call could never strand a dossier today (fresh dossiers
    have zeroed triggers, making the check unreachable) but
    the ordering was fragile. All fallible checks now precede
    creation.
  - `chaptersObserved` bounded (round-trip symmetry with
    FromJson's bound).
  - `prior` is documented sticky (first-sighting wins —
    hearsay identity, not a live feed).
  - All-zero histogram at observed>=3 → empty deck (third
    empty-deck condition, now documented in the header).
- **Wiring deferred** (deferred-work.md): nothing calls
  RecordChapter from the battle loop yet — the CardFired →
  histogram fold and counter-deck → rival SquadSheet
  injection are the chapter-shell integration seam
  (ChapterState / Epic E); RivalBook is a standalone
  potato.rivals/1 doc — save-slot wiring deferred.

## File List

- `Campaign/Rivals/RivalDeck.h` (new)
- `Campaign/Rivals/RivalDeck.cpp` (new)
- `Examples/potato_test_campaign.cpp` (+3.7 test block)
- `_bmad-output/implementation-artifacts/3-7-rivaldeck-learning.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/deferred-work.md`
