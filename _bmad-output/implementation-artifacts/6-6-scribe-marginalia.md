---
baseline_commit: NO_VCS
---

# Story 6.6 — Scribe Marginalia

> Epic 6 — Narrative Systems (C) · the ledger's second hand ·
> **Status: review**

## Story (from epics.md)

As a player,
I want the Scribe's small notes annotating ledger entries — some
authentic, some suspect,
So that the ledger has a second hand.

## Acceptance Criteria

- **Given** posted ledger entries, **when** marginalia render,
  **then** each note carries an authenticity flag state
- **And** forged-looking marginalia can be flagged suspect,
  feeding the Judgment beat.

## Design

`Campaign/Narrative/Marginalia.{h,cpp}` — the ledger-wide half
the 10.3 Buchao note deferred to. Notes bind to entries by `seq`
(the ledger's stable append-order key); one note per entry.

- **Wire**: `potato.marginalia/1` — `{"schema", "notes":[{seq,
  note, suspect?}]}`, append-ordered, sibling-doc convention
  (same seam as `potato.buchao/1`). Tolerant load: seqs beyond
  the ledger head persist (judgment metadata), duplicate seqs
  dedupe first-wins.
- **`suspect`** — same overlay convention as `LedgerEntry::
  suspect` and `BuchaoPage::suspect`: post-hoc flag, persisted,
  never hash-covered. `SetSuspect(seq)` flips it; 6.8's audit
  reads it.
- **`AnnotateScribe(ledger, store, maxNotes)`** — scans entries
  in ledger order, annotates `ScribeWorthy` ones not yet noted,
  cap-bounded (`0` = no-op), idempotent. Mirrors `DeliverBuchao`.
- **`ScribeWorthy`** — only where the chronicle turned:
  `atrocity` / `myth` / `march` / `raid` tags and
  `resolution:`/`chapter:` prefixes. Untagged bookkeeping stays
  silent.
- **`ComposeScribeNote`** — 史官體 clause pools per salient tag
  family (atrocity→myth→march→raid→resolution→chapter priority);
  pick = FNV-1a(memo ‖ seq LE) % pool — replay-stable, no PRNG.
- **`RenderMarginalia`** — `批〔seq〕<note>` per line; `（疑）`
  suffix when the note's own flag is set OR the annotated entry
  carries ledger `suspect` — the ink is tainted by the row
  beneath it either way (AC's authenticity flag state).

## Implementation Tasks

- [x] `Marginalia.{h,cpp}` — store + annotate + render
- [x] `potato_test_narrative` — 6.6 section, 19 pins
- [x] Build + ctest green; story + sprint-status sync

## Dev Agent Record

### Completion Notes List

- Reuses the Ledger::TAG_* constants — no literal tag strings
  except `march`/`raid`, which predate the constant convention
  (DeedBook emits them bare; same namespace, same bytes).
- Note text composes from the entry alone — no ledger position
  or caller state leaks into the pick, so re-annotation after
  any interleaving history stays stable.
- Store is NOT embedded in `potato.campaign` — like
  `BuchaoStore`, it's a sibling doc the caller persists where it
  chooses; save integration is a later seam (12.8 set the
  embed convention if it lands).

### File List

- `Campaign/Narrative/Marginalia.{h,cpp}` — NEW
- `Examples/potato_test_narrative.cpp` — 6.6 section
- `_bmad-output/implementation-artifacts/6-6-*.md`,
  `sprint-status.yaml`

## Validation

- `potato_test_narrative` — 19 new pins: worthiness gating,
  cap/idempotent annotate, deterministic composition, clean vs
  flagged render, entry-taint propagation, wire round-trip,
  schema reject, dedupe, beyond-head tolerance.
- ctest **25/25** green incl. `gameplay_dep_guard`.
