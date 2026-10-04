# Story 3.4: Chapter Shell & Progression

## Status: done

## Story

As a player,
I want chapter progression with unlock rules over a campaign map,
So that victories move me forward.

## Acceptance Criteria

1. **Given** a resolved chapter (win or converted defeat),
   **When** the campaign advances,
   **Then** the next chapter unlocks and carries forward starting
   conditions (roster, ledger, intel).

## Design

`Campaign/Chapters/Progression` — binds `ChapterLibrary` (3.3) to
`ChapterProgress` (3.1). Linear campaign map for now (Epic 4/6
may add branches).

Chapter space contract (review-established at 3.3): vectors are
sized `MaxIndex()+1` and indexed BY `ChapterDef.index` — sparse
indexes produce dead flag slots, never confusion.

- `InitializeProgress(state, lib)`: sizes `unlocked`/`resolved`
  to the chapter space, `current` = lowest index, that chapter
  unlocked. Fails on empty library.
- `ResolveAndAdvance(state, lib)`: resolution is a caller
  decision (win OR converted defeat — Epic 4 owns the
  conversion); this just records it. Marks `resolved[current]`,
  finds the next index in library order, sets `current` to it and
  unlocks it. Past the last chapter → `current = MaxIndex()+1`
  (campaign-complete sentinel). Fails if the campaign is already
  complete or `current` isn't a registered chapter.
- **Carry-forward is structural**: roster and ledger live in
  `CampaignState` and are never reset by progression — the same
  objects flow into the next chapter. Intel: no intel store
  exists yet (vacuously satisfied); when Epic 6's IntelLedger
  lands it carries forward by the same mechanism. `record_root`
  anchors attest record integrity — they do NOT transport intel
  priors.

Also fixes the 3.1 residual: `current == MAX_CHAPTERS` was
rejected before the complete-sentinel check — a full 64-chapter
space's completed campaign couldn't round-trip.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Progression.{h,cpp}` + the 3.1 sentinel
    fix
- [x] Task 2 — tests: init, advance, unlock next, completion
    sentinel (incl. the 64-deep boundary), sparse indexes,
    roster/ledger carry-forward, already-complete rejection,
    unregistered current, dead-slot/locked-chapter rejection,
    re-init guard, empty library
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- "Win or converted defeat" — both are `resolved`; the win/loss
  distinction lives in the ledger entries the resolution posts
  (Epic 4), not in progression state.
- Review-established guards: `InitializeProgress` refuses to wipe
  live progress (boot-only); `ResolveAndAdvance` requires the
  current chapter to be unlocked and unresolved (untrusted-state
  posture — a crafted save can't resolve a never-unlocked
  chapter); empty progress vectors fail as "not initialized", not
  "complete".
- Known residual: nothing prevents back-to-back resolves
  ("resolve-spam" skips playing a chapter) — no phase state
  exists yet; caller discipline until a shell phase lands.
- Deferred (recorded in deferred-work): `ChapterState` —
  mid-chapter runtime state (presented record sets for
  OrphanAnchors, fog priors) has no home yet; see
  `_bmad-output/implementation-artifacts/deferred-work.md`.

## File List

- `Campaign/Chapters/Progression.h` (new)
- `Campaign/Chapters/Progression.cpp` (new)
- `Campaign/State/CampaignState.cpp` (sentinel boundary fix)
- `Examples/potato_test_campaign.cpp` (+3.4 test block)
- `_bmad-output/implementation-artifacts/3-4-chapter-shell-progression.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/deferred-work.md`

