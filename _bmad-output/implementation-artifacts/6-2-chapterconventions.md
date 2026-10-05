# Story 6.2 — ChapterConventions

> Epic 6 — Narrative Systems (C) · 章回 frames · **Status: done**

## Story (from epics.md)

As a player,
I want each chapter framed with 敕命 frontispiece, 評斷 enemy
judgment, and 懸念 cliffhanger,
so that the campaign reads as 章回.

## Acceptance Criteria

- **Given** a chapter open/close, **when** conventions render,
  **then** the correct per-chapter frames appear from
  potato.narrative/1 content **and** frames vary by ledger state.

## Design

`Campaign/Narrative/Conventions.{h,cpp}` — schema + render, not
the loader (6.5 owns file/registry):

- **`potato.narrative/1` doc** — `"schema"` + `"chapters"` map of
  `chapterId → {frontispiece, judgment, cliffhanger}`. Per-file
  rejection stays the loader's job; bad doc fails `FromJson` clean.
- **`ChapterConventions`** — parsed frames. `judgment` and
  `cliffhanger` are condition-keyed variant maps (bare string =
  `default` shorthand); lookup falls back requested → `default` →
  first entry (std::map order, deterministic).
- **Ledger levers** — `LedgerMood` reads the corruption ratchet
  (clean <10 / tainted <40 / corrupt ≥40) for the 評斷;
  `LedgerOmen` reads the 天命 balance (blessed ≥30 / fading ≥5 /
  barren) for the 懸念. The court cannot hide what the books show.
- **`RenderChapterOpen`** — `敕命·<id>` + sealed frontispiece (or a
  generic mandate). Frontispiece does NOT vary by ledger — the
  commission is sealed before facts arrive; that distance is the
  fiction.
- **`RenderChapterClose`** — `評曰：<judgment[mood]>` +
  `<cliffhanger[omen]>——且聽下回分解。`

## Implementation Tasks

- [x] `ChapterConventions::FromJson/ToJson` + caps
- [x] `LedgerMood`/`LedgerOmen` thresholds + key mapping
- [x] `RenderChapterOpen` / `RenderChapterClose`
- [x] Tests: schema gates, variant fallback chain, ledger levers,
      canonical round-trip, deterministic render
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_ledger` — 15 pins (parse, header, generic mandate,
  clean/corrupt verdicts, barren/blessed hooks, shorthand default,
  deterministic fallback, schema/shape reject, canonical emit,
  determinism)
- ctest 19/19 + dep_guard
