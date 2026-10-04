# Story 2.3: Forgery & Suspicion Channels

## Status: done

## Story

As a designer,
I want explicit forgery injection and suspect-flag mechanics,
So that dishonest history is content, not corruption.

## Acceptance Criteria

1. **Given** the ForgeryChannel write path,
   **When** an enemy injects a forged entry,
   **Then** the entry persists with detectable provenance and a
   suspect flag can be set
   **And** suspect flags surface in later reports.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Provenance` on `LedgerEntry` (hash-covered)
  - `enum class Provenance { Honest, Forged }`; `Post` always books
    `Honest`. `Ledger::Forge(Posting)` is the ForgeryChannel write
    path: same validation, same chaining — but marks `Forged`.
  - `provenance` enters `EntryContentJson` (the hashed content): the
    mark can't be stripped without a recompute-class rewrite —
    consistent with the existing unkeyed boundary.
- [x] (AC: 1) Task 2 — `suspect` flag (outside the hash)
  - `bool suspect` on `LedgerEntry`, NOT in `EntryContentJson`:
    flagging is post-hoc judgment; mutating hashed content would
    break the chain. Accepted surface: flags are flip-able by a
    file tamperer — they are judgment metadata, not claims.
  - `Ledger::SetSuspect(seq, bool)`; `SuspectEntries()` returns the
    seq list for report layers (2.4 consumes).
- [x] (AC: 1) Task 3 — `potato.ledger/3`
  - Entry shape gains required `provenance` + `suspect`; `/2`
    rejected (a /2 loader has no provenance semantics). Forged
    entries still fold into balances — they are kept data.
- [x] Task 4 — tests: forged entry persists chain-valid, provenance
    round-trip + flip-rejection, suspect set/persist/query,
    hash-transparency of the flag, Forge validates like Post.
- [x] Task 5 — story/sprint/deferred docs; commit.
- [x] Task 6 — three-layer review.

## Dev Notes

- The chain stays append-only: `Forge` is a second *write* path,
  not a mutation path — both produce valid links. `Verify` checks
  integrity, not provenance — forged entries verify clean BY DESIGN.
- Fiction: the forged entry's content claims honest bookkeeping;
  the provenance field is the system's record of which hand wrote
  it. Audit (2.4) and cross-check (2.5) are the detection stories;
  2.3 lands the data model.
## Completion Notes

- Design decision (from 2.2 deferred-work): `provenance` enters the hashed content — the forged mark is tamper-evident; `suspect` stays OUTSIDE the hash as freely-mutable judgment metadata but is a required field on resave (not silently dropped). Schema -> `potato.ledger/3` (/2 rejected: no provenance semantics).
- `Forge` = the ForgeryChannel: identical validation + chaining as `Post`; forged entries verify clean and fold into balances — suspicion is data, not corruption.
- Review triage: all three layers flagged stale /2 test literals (schema gate shadowing the named gates) — fixed; added edge probes (forge-on-empty, all-forged chain, honest-entry flag+clear+round-trip, case-variant provenance, missing-suspect). One test-authoring bug (provenance expectation on an all-forged ledger) fixed during patch.
- Tests: 75 checks; ctest 16/16 green (MinGW).
