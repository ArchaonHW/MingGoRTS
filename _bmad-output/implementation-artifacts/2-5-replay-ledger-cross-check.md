# Story 2.5: Replay-Ledger Cross-Check

## Status: done

## Story

As a system,
I want battle archive integrity cross-checked against ledger chain
consistency,
So that the two histories can't disagree silently.

## Acceptance Criteria

1. **Given** a replay stream and the ledger chain covering it,
   **When** cross-checked,
   **Then** mismatch flags the chapter state as suspect
   **And** a consistent pair verifies clean.

## Design

The ledger chain's seal is unkeyed (2.2 boundary) — the cross-check
is the EXTERNAL ANCHOR the threat model defers to: an entry's tag
binds the battle record's integrity root INTO the hashed chain, so
the record and the ledger attest each other.

- Convention: an entry covers a record iff it carries tag
  `record_root:<16 lowercase hex>` (fits MAX_TAG_LEN). Epic 4's
  battle→ledger translation posts it; this story lands the
  convention + the check.
- `CrossCheckRecord(Ledger&, recordDoc)`:
  1. record doc must carry `integrity.root` (else `BadRecord`)
  2. `l.Verify()` first — `ChainBroken` (defensive; unreachable on
     live ledgers)
  3. recompute the record root (payload sans `integrity`) and check
     the record's own seal: declared != recomputed →
     `RecordInconsistent`, independent of coverage
  4. collect RELEVANT anchors: stored root == declared OR
     recomputed (other records' anchors are skipped as foreign);
     `NoAnchor` = uncovered record — absence, not disagreement
  5. diverging anchors flag their entry `suspect` — "chapter state
     flagged suspect" lands concretely on the covering entry,
     surfacing in reports (2.4) as omission+audit.
  6. `Clean` = record self-consistent AND every relevant anchor
     agrees. With dual-relevance a relevant anchor always agrees
     with an honest record, so there is no "clean record / lying
     ledger" verdict — ledger-side fabrication surfaces via
     `OrphanAnchors()` (claims no presented record satisfies) and
     `forgedAnchors` (coverage by forged entries) in the result.
- Recompute via `BattleRecorder::ComputeRoot(payload sans
  integrity)` — same canonical-emit FNV-1a as the record itself.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Campaign/Ledger/CrossCheck.{h,cpp}`
- [x] Task 2 — tests: clean pair, mismatched seal → suspect flag,
    no-anchor, bad record, malformed tag ignored, multi-anchor
    disagreement flags all mismatching, forged-anchor coverage,
    orphan sweep, idempotent re-check.
- [x] Task 3 — story/sprint/deferred docs; commit.
- [x] Task 4 — three-layer review.

## Dev Notes

- Residual honesty: the tag binds root→chain, but the whole chain
  is still recompute-class forgeable (all hashes public). The real
  deterrent is the CROSS direction — an attacker rewriting the
  ledger must also keep every anchored record's root consistent,
  and records live outside the ledger file.
- Symmetric boundary (three-layer review finding): a record
  payload+root BOTH rewritten consistently is indistinguishable
  from an uncovered record → `NoAnchor`. `OrphanAnchors()` is the
  complementary ledger-side sweep; callers must present the
  chapter's COMPLETE record set for it to mean anything (Epic 3
  `ChapterState` / Epic 4 wiring owe that call site).
- `Clean` ignores anchor provenance by design (a hash-bound claim
  is accurate regardless of who posted it), but `forgedAnchors`
  exposes the count.
- `flagged` counts entries newly flagged this call; `anchors`
  counts relevant anchor claims (a multi-anchor entry counts once
  as flagged, multiple as claims).

## File List

- `Campaign/Ledger/CrossCheck.h` (new)
- `Campaign/Ledger/CrossCheck.cpp` (new)
- `Examples/potato_test_ledger.cpp` (cross-check test block)
- `_bmad-output/implementation-artifacts/2-5-replay-ledger-cross-check.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
