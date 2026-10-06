<!-- Powered by BMAD™ Core -->
# Story 10.4 — Bencao Codex Renderer

> Epic 10 — Bencao Codex (BC) · headless text render ·
> **Status: done**

## Story (from epics.md)

As a player,
I want the codex to render as a book — frontispiece disclaimer,
category browsing, six-field entry pages in 本草體 —
So that it reads as the desk's second book, not a menu.

## Acceptance Criteria

**Given** the registry plus the unlocked set,
**When** the codex screen renders,
**Then** the 序頁 disclaimer (worldview §4) always precedes browsing
and locked entries show as sealed 補鈔-pending slots under their
諱 title
**And** the text-layout path is headless-testable on the
HistorianReport precedent, with page chrome landing on the
F-epic UI.

## Design Notes

### RenderCodex

```cpp
// Campaign/Narrative/Codex.h
// Renders the whole codex as a text book. Three entry states:
//   unlocked (in codex)  — full 本草體 page, all authored fields
//                          + the delivered 批註 when `buchao` binds
//                          one (suspect pages carry a 書吏疑之 mark)
//   pending (queued)     — 【諱】補鈔在途: a page the Scribe knows
//                          is coming; name withheld, no fields
//   locked (untriggered) — 【諱】未錄: sealed slot, no fields
// The 諱 title never leaks factual fields — collection slots show
// that a page exists, never what it says.
std::string RenderCodex(const BencaoLibrary& lib,
                        const BencaoCodex& codex,
                        const BuchaoStore* buchao = nullptr);
```

### Page layout (headless text)

```
【本草拾遺】

序：是冊所載，皆前人之驗、草木之性，錄以備考，非為用藥之據。
    病家慎勿執紙上之言以試人身。

——山草類——
【三七】
釋名：山漆、金不換。
集解：生廣西、雲南山峒深處…
性味歸經：甘、微苦，溫。歸肝、胃經。
主治：止血散血，定痛…
批註：第三回，斥候隊…血止。（批校：書吏疑其不實。）
出處：《本草綱目》卷十二

【諱】補鈔在途
【諱】未錄

——隰草類——
…

凡 N 種，已錄 X 種。
Historical text reproduced for the fiction; not medical advice.
```

- Frontispiece text is the worldview §4 quote verbatim — fixed
  string, pinned in tests.
- Category headers always emit for all 8 部類 (the book's TOC is
  stable) even when a section holds only sealed slots.
- `aliases`/`origin` omit their field line when absent — a page
  skips empty fields rather than printing bare labels.
- `批註` renders only when `buchao` binds a page for the entry;
  suspect pages append `（批校：書吏疑其不實。）` — the same
  judgment-overlay semantics as ledger suspect flags.
- Footer: `凡 N 種，已錄 X 種。` + the English-facing disclaimer
  line from worldview §4.
- Deterministic: pure fold over `lib.Entries()` order; no PRNG,
  no clock. Text layer, not tick — string building is fine.
- `BencaoCategoryName` gains its missing definition (returns the
  wire id, round-tripping `BencaoCategoryFromName`); a sibling
  `BencaoCategoryTitle` returns the CJK 部類名 for display.

### Boundaries

- Renderer reads lib/codex/store; mutates nothing; no file I/O.
- Page chrome (冊頁 art, palette) is Epic F shell work — this
  story ships the headless text only (HistorianReport precedent).
- OQ-B1 (codex home — desk vs RefitCamp) decision deferred to the
  shell story (10.5); the renderer is home-agnostic.

## Implementation Tasks

- [x] `Bencao.{h,cpp}` — define `BencaoCategoryName` (wire id) +
      add `BencaoCategoryTitle` (CJK 部類名)
- [x] `Campaign/Narrative/Codex.{h,cpp}` — `RenderCodex`
- [x] `potato_test_bencao` — 10.4 block: frontispiece verbatim,
      category order/headers, unlocked full page fields,
      pending/locked sealed slots (name never leaks), 批註 render
      + suspect mark, footer counts, English disclaimer, empty
      library, determinism
- [x] Build PotatoCampaign + focused test + full ctest
- [x] Review (three passes), sprint-status sync, commit + push

## Dev Agent Record

**Implemented 2026-10-06** (MinGW via `C:\MingGoRTS` junction):

- `Campaign/Narrative/Codex.{h,cpp}` — `RenderCodex`: 序頁
  verbatim → 8 部類 headers (always emit, TOC stable) →
  three-state slots (unlocked full page / pending 補鈔在途 /
  locked 未錄, both under 【諱】 with zero field leak) →
  colophon counts + English disclaimer line.
- `BencaoCategoryName` gained its missing definition (wire id,
  round-trips `BencaoCategoryFromName` — declared in 10.1, first
  caller landed here); `BencaoCategoryTitle` added for the CJK
  部類 section titles.
- 批註 renders only when `BuchaoStore` binds a page; suspect
  pages append （批校：書吏疑其不實。） — judgment-overlay
  semantics shared with ledger suspect flags.
- Deferred (matches story scope): page chrome → Epic F shell;
  OQ-B1 codex home (desk vs RefitCamp) → 10.5 shell story.
- Verification: `potato_test_bencao` 10.4 block 10 pins green
  (75 total); `ctest` 23/23 incl. `gameplay_dep_guard`. MSVC
  not verified this session.