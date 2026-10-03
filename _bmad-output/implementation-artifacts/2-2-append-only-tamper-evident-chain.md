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
    `prevHash = tip()` (genesis = FNV offset basis for entry 0) then
    `hash = FNV-1a(prevHash_le || canonical_emit(content_fields))`
    over `{seq, credit, debit, memo, tags}` — the same canonical JSON
    emit convention as the battle record.
  - `Tip()` accessor — the tip hash commits to the whole history.
- [x] (AC: 1) Task 2 — `Verify()`
  - Public `const char* Verify() const` (nullptr = clean): seq
    contiguity, prevHash linkage, stored-hash recomputation — first
    break reported.
  - `FromJson` runs it post-load: a broken chain is a load rejection,
    not a warning.
- [x] (AC: 1) Task 3 — `potato.ledger/2`
  - Schema bump (deferred-work note from 2.1): entries emit
    `prevHash`/`hash`; doc emits `seal = SealHash(count || tip)` —
    the seal binds the entry count too, so truncation is the same
    recompute-class attack as a rewrite (a plain stored tip could
    be copied from the file itself — review catch).
  - `/1` docs reject with upgrade reason (pre-release; no durable
    corpus to migrate).
- [x] Task 4 — tests: chain formation, verify clean, mutated memo /
    relinked prevHash / truncated tail all rejected, round-trip,
    plus threat-model pins (recomputed-seal truncation and full
    rehash rewrites LOAD — the documented boundary).
- [x] Task 5 — story/sprint/deferred docs; commit.
- [x] Task 6 — three-layer review.

## Dev Notes

- Honest threat model (same as the battle record): the chain detects
  mutation that doesn't recompute; a wholesale rewrite produces a
  self-consistent chain with a *different seal* — and a truncator
  who recomputes the seal loads too (seal is unkeyed; tests pin both
  boundaries). Anchoring the tip to an external truth is Story 2.5's
  cross-check, not this story.
- `Verify()` also replays the leg/meta invariants, not just links —
  a future write path that bypasses `Post` still can't produce a
  verifying chain.
- Wire note: hash-family values serialize as uint64 bit-cast to
  int64 (bit-exact; ~half emit negative) — external producers must
  bit-cast, unsigned literals >=2^63 parse as Real and get rejected.
- Forgery channel (2.3) writes through the chain (valid links, marked
  provenance) — suspicion is data, not a hash break.
