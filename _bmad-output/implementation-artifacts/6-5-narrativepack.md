---
baseline_commit: NO_VCS
---

# Story 6.5 — NarrativePack

> Epic 6 — Narrative Systems (C) · versioned content bundles ·
> **Status: review**

## Story (from epics.md)

As a developer,
I want versioned narrative content bundles loaded as registries,
So that story content is data, not code.

## Acceptance Criteria

- **Given** potato.narrative/1 packs in `assets/narrative/`,
  **when** the campaign boots, **then** packs register read-only
- **And** bad-version packs reject without failing the library.

## Design

`Campaign/Narrative/NarrativePack.{h,cpp}` — the loader/registry
6.2 deferred to this story. One file = one pack = one
`ChapterConventions` document; the library merges all accepted
packs into a single conventions view that the existing renderers
(`RenderChapterOpen`/`RenderChapterClose`) consume unchanged.

- **`NarrativeLibrary::Load(dir, out)`** — sorted-filename
  iteration, `.json` only, per-file rejection into
  `result.rejected` (`schema`/`parse`/`duplicate`/`field`/`io`).
  Directory unreadable or file count over `MAX_PACKS` (64) fails
  wholesale — the `CharacterLibrary` file-count bound precedent,
  capping the rejected-vector DoS surface.
- **Pack identity** — optional `"pack"` field in the doc, else
  the filename stem (UTF-8). Duplicate pack id rejects the later
  file.
- **Chapter ids are one global namespace** across packs — a
  second pack claiming an existing chapter id rejects as
  `duplicate`; all-or-nothing per file, no partial merge.
- **Merged cap** — cumulative frames bounded by
  `ChapterConventions::MAX_CHAPTERS` (64); overflow rejects the
  offending file.
- **Merge path** — accepted members accumulate into a JSON
  chapters object, then the merged doc goes through
  `ChapterConventions::FromJson` once: one parser, one truth.
- Registry is read-only after `Load` (NFR9).

## Implementation Tasks

- [x] `NarrativePack.{h,cpp}` — library + load result types
- [x] `potato_test_narrative` — 13 pins
- [x] `assets/narrative/core.json` seed pack
- [x] CMake registration + build + ctest green
- [x] Story + sprint-status sync

## Dev Agent Record

### Completion Notes List

- Registry follows the ChapterLibrary/BencaoLibrary contract:
  sorted iteration, per-entry stat on its own error channel,
  per-file isolation, immutable post-load.
- Packs: `"pack"` field wins over filename stem — content that
  wants a stable id carries it in-doc.
- `Packs()` exposes `[{id, chapters}]` in registration order for
  boot diagnostics; chapter lookup is merged-view only
  (`Find`/`Conventions`).

### File List

- `Campaign/Narrative/NarrativePack.{h,cpp}` — NEW
- `Examples/potato_test_narrative.cpp` — NEW
- `CMakeLists.txt` — test registration
- `assets/narrative/core.json` — NEW seed pack
- `_bmad-output/implementation-artifacts/6-5-*.md`,
  `sprint-status.yaml`

## Validation

- `potato_test_narrative` — 13 pins green: clean load, pack
  registration order, merged find + render through the 6.2 API,
  bad-version/bad-shape/garbage per-file rejection, chapter-id
  and pack-id dedupe, unreadable-dir wholesale fail, empty-dir ok.
- ctest **25/25** incl. `gameplay_dep_guard` (MinGW, build-f).
