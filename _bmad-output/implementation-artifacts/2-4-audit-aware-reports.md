# Story 2.4: Audit-Aware Reports

## Status: done

## Story

As a player,
I want chapter reports to include omission counters and ledger audit
segments,
So that the chronicle confesses what it hides.

## Acceptance Criteria

1. **Given** a resolved chapter,
   **When** the HistorianReport renders,
   **Then** it always includes the omission counter (本報告省略 N 項)
   **And** audit segments reflect ledger integrity state.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Campaign/Ledger/HistorianReport.{h,cpp}`
  - `struct AuditSegment { entries, forged, suspect, chainOk, tip,
    seal }` — the integrity surface folded from the ledger.
  - `struct OmissionPolicy { omitForged, omitSuspect }` (both default
    true): entries under question stay OUT of the chronicle body —
    but the counter must confess them.
  - `struct HistorianReport { includedSeqs, omissions, audit,
    RenderText() }` — included = entries minus omitted.
  - `RenderHistorianReport(const Ledger&, OmissionPolicy={})`.
- [x] (AC: 1) Task 2 — `RenderText()`
  - Deterministic integer-only text; always ends with the confession
    line `本報告省略 N 項` (the AC's literal marker — layout/styling is
    Epic 8 presentation).
- [x] Task 3 — tests: clean ledger omits 0; forged/suspect omitted +
    counted under each policy; audit reflects counts/chain/tip;
    RenderText always carries the omission line.
- [x] Task 4 — story/sprint docs; commit.
- [x] Task 5 — three-layer review.

## Dev Notes

- Read-only consumer: the report queries `Entries()`/`provenance`/
  `suspect`/`Verify()`/`Tip()`/`Seal()` — no new ledger write paths.
- Omission is POLICY, not deletion: the entries stay in the ledger
  (append-only); the chronicle chooses what to narrate and confesses
  the count. The Judgment spread (6.8) later lays them open.
## Completion Notes

- Confession literal follows authoritative epics.md （本報告省略 N 項） — an earlier draft used a variant; caught by review.
- `AuditSegment.breakReason` preserves Verify()'s reason; `RenderText` carries tip/seal (AC: audit segments reflect integrity state).
- Review additions: forged∧suspect overlap omitted once but counted in both audits; narrate-all keeps audit counts; partial-policy case; tip/seal render pins.
- Residual (noted): the confession assertion pins UTF-8 bytes, not decoded text — Epic 8 should re-assert against real strings once a text pipeline exists.
- Tests: 89 checks; ctest 16/16 green (MinGW).
