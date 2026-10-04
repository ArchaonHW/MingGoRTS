# Story 3.3: ChapterLibrary

## Status: done

## Story

As a developer,
I want versioned chapter definition packs (potato.chapter/1)
loaded into a boot-time registry,
So that chapters are content, not code.

## Acceptance Criteria

1. **Given** chapter JSON files in `assets/chapters/`,
   **When** the campaign boots,
   **Then** all chapters register immutably
   **And** a bad version rejects that file without failing the
   library.

## Design

`Campaign/Chapters/ChapterLibrary` — boot-time registry built by
`Load(dir)`, immutable thereafter (const access only; registries
are immutable during play per NFR9).

`potato.chapter/1` (chapter spine — richer fields are Epic 4/6
schema bumps):

```json
{
  "schema": "potato.chapter/1",
  "id": "ch01_rebellion",
  "index": 0,
  "title": "第一章 · 起兵",
  "map": "assets/maps/ch01.json",
  "combat": true,
  "briefing": "..."
}
```

- `id` unique across the library (dup → file rejected).
- `index` is the persisted chapter coordinate (saves store
  `chapter.current` as an index) — explicit content, unique, and
  **may be sparse** ({0,5} is legal). The chapter space is
  `[0, MaxIndex()+1)`: ChapterProgress vectors are sized to
  `MaxIndex()+1` and indexed BY `ChapterDef.index`, not by
  position in `Chapters()` (3.4 contract, review-established).
- `combat=false` marks a designated zero-combat chapter (Epic E).

Per-file failure isolation (the AC's core): `Load` iterates
`*.json` (case-folded extension) in sorted filename order
(deterministic); each file is `Json::Load` + field-validated
independently; a bad file appends `{path, error, reason}` to
`rejected` and the library keeps going. `Load` fails wholesale
only when the directory is unreadable — an empty-but-readable
dir yields an empty library.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `ChapterLibrary.{h,cpp}`
- [x] Task 2 — tests: register+lookup, per-file rejection (bad
    schema, missing field, dup id, dup index), index ordering,
    empty dir, unreadable dir, sparse indexes
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- Review fixes: per-entry stat uses its own `error_code` (a shared
  one could be cleared by the next `increment()`, silently
  dropping a file); extension filter is case-folded; mistyped
  `briefing` rejects rather than silently dropping; dup checks
  precede either commit (a dup-index reject can't squat an id);
  `Json::Load`'s `file_size` ec hole patched alongside (a failed
  stat previously bypassed the 64 MiB cap).
- `MAX_CHAPTERS` is `static_assert`-tied to
  `CampaignState::MAX_CHAPTERS` — drift would break the 3.4
  chapter-space contract.
- On `!ok`, `out` is untouched (a failed reload keeps the prior
  registry); call once at boot per NFR9.
- Known residual (3.4 precondition): `CampaignState` currently
  rejects `current >= MAX_CHAPTERS` before the complete-sentinel
  check — a full 64-chapter space's `current==64` can't
  round-trip yet; 3.4 decides whether to widen it.

## File List

- `Campaign/Chapters/ChapterLibrary.h` (new)
- `Campaign/Chapters/ChapterLibrary.cpp` (new)
- `Gameplay/Json/Json.cpp` (file_size stat-failure guard)
- `Examples/potato_test_campaign.cpp` (+3.3 test block)
- `_bmad-output/implementation-artifacts/3-3-chapterlibrary.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
