# Story 2.2: Append-Only Tamper-Evident Chain

## Status: done

## Story

As a system,
I want entries chained by hashing the previous entry, with break
detection,
So that history can't be quietly rewritten.

## Acceptance Criteria

1. **Given** a ledger chain, **when** `verify()` replays it, **then** a
   mutated historical entry fails verification and load is rejected
   **and** accumulators compute by folding entries — no public write
   API exists.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — chain fields on `LedgerEntry`
  - `prevHash`/`hash` (uint64). `Post` sets
    `prevHash = seal()` (genesis = FNV offset basis for entry 0) then
    `hash = FNV-1a(prevHash_le || canonical_emit(content_fields))`
    over `{seq, credit, debit, memo, tags}` — the same canonical JSON
    emit convention as the battle record.
  - `Tip()` accessor — the seal hash commits to the whole history.
- [x] (AC: 1) Task 2 — `Verify()`
  - Public `const char* Verify() const` (nullptr = clean): seq
    contiguity, prevHash linkage, stored-hash recomputation — first
    break reported.
  - `FromJson` runs it post-load: a broken chain is a load rejection,
    not a warning.
- [x] (AC: 1) Task 3 — `potato.ledger/2`
  - Schema bump (deferred-work note from 2.1): entries emit
    `prevHash`/`hash`; doc emits `seal` — catches truncation, since a
    shortened chain can't reproduce the stored seal.
  - `/1` docs reject with upgrade reason (pre-release; no durable
    corpus to migrate).
- [x] Task 4 — tests: chain formation, verify clean, mutated memo /
    relinked prevHash / truncated tail all rejected, round-trip.
- [x] Task 5 — story/sprint/deferred docs; commit.
- [x] Task 6 — three-layer review.

## Dev Notes

- Honest threat model (same as the battle record): the chain detects
  mutation that doesn't recompute; a wholesale rewrite produces a
  self-consistent chain with a *different seal* — anchoring the seal to
  an external truth is Story 2.5's cross-check, not this story.
- Forgery channel (2.3) writes through the chain (valid links, marked
  provenance) — suspicion is data, not a hash break.

## Completion Notes

- Three-layer review caught the headline defect: a plain stored `tip` is defeatable by copy-paste (truncate + copy `entries[k-1].hash` into tip — zero recompute). Fixed by `seal = SealHash(count || tip)` — truncation is now the same recompute-class attack as a wholesale rewrite. Threat-model pins make the honest boundary explicit: resealed truncation and full-rehash rewrites LOAD (unkeyed seal; external anchoring is Story 2.5).
- `Verify()` replays leg/meta invariants too — a future write path that bypasses `Post` still can't produce a verifying chain.
- Wire note: hash-family uint64s serialize bit-cast to int64 (bit-exact; ~half emit negative) — documented on `SCHEMA`.
- 2.3 flag design decision recorded in deferred-work: plain extra fields are hash-transparent and resave-dropped; a tamper-evident `suspect` flag requires entering `EntryContentJson` (→ /3).
- Tests: 61 checks; ctest 16/16 green (MinGW).
