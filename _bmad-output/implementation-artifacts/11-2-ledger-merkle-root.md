---
baseline_commit: NO_VCS
---

# Story 11.2 — Ledger Merkle Root

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · pure fold, no schema ·
> **Status: done**

## Story (from epics.md)

As a system,
I want a deterministic Merkle root folded over the ledger hash
chain and the pending claim set,
So that one small constant anchors the whole history on-chain.

## Acceptance Criteria

- **Given** a ledger chain and a claim set, **when** the root
  computes, **then** leaves are ordered canonically (entry seq,
  then claim id) and the result is a fixed-width digest
  reproducible bit-exact across MSVC/MinGW **and** the root
  changes iff covered content changes — same hash discipline as
  the record integrity root.

## Context

Story 11.1 ships the claim container and outbox; the relayer
(11.4) and anchor contract (11.5) need ONE uint64 that commits to
everything being claimed: the ledger history plus the pending
claim set. That root is what lands on-chain — the game never
touches the chain itself (D-ARCH-9).

The ledger already gives every entry a content commitment
(`LedgerEntry::hash` — FNV-1a over prevHash ‖ canonical emit,
same convention as the battle record's integrity root), and every
claim a canonical JSON emit (`MintClaim::ToJson`). This story adds
no new hash machinery: it defines the *fold* that joins the two
existing commitments into a single tree digest.

## Design

`Campaign/Chain/LedgerMerkle.{h,cpp}` — chain layer, next to
`MintClaim`/`MintOutbox`. No schema, no persistence — the root is
computed on demand (the anchor store is 11.5's problem).

```cpp
// The anchor fold (Story 11.2): one uint64 committing to the whole
// ledger hash chain plus a claim set. Canonical leaf order — all
// ledger entries in seq order, then claims sorted by
// (ClaimId, canonical emit). The claim set is a SET: exact
// duplicates dedupe before the fold.
//
// FNV-1a under a per-level domain byte (same hash family as the
// ledger chain and record integrity root):
//   entry leaf = Fnv1a(0x01 ‖ entry.hash LE8)
//   claim leaf = Fnv1a(0x02 ‖ claim canonical emit bytes)
//   node       = Fnv1a(0x00 ‖ left LE8 ‖ right LE8)
// Pair-wise fold left to right; an odd leaf promotes unpaired
// (RFC-6962 style — deterministic, no duplication ambiguity).
// `suspect` flags are NOT covered — they live outside hashed
// content (Ledger.h), and flipping judgment must not move the
// anchor. Empty ledger + empty claim set → 0 (the "nothing
// anchored" sentinel, matching recordRoot==0 convention).
//
// Fails `field` if a claim's ToJson fails — an oversized in-memory
// claim can't reach the fold, same rejection class as emission.
Gameplay::Result<std::uint64_t>
LedgerMerkleRoot(const Ledger& l,
                 std::span<const MintClaim> claims);
```

### Rules

- **Canonical order**: ledger entries by `seq` (vector order —
  chains are contiguous); claims sorted by `ClaimId`, ties broken
  by canonical emit, exact-duplicate claims deduped (a claim can't
  be "more pending" than once).
- **Bit-exact**: FNV-1a over raw bytes only — no endianness-
  dependent reads, no floats, no locale. The same inputs yield the
  same root on MSVC and MinGW.
- **Iff coverage**: entry leaf binds `entry.hash` (which itself
  binds content + prevHash); claim leaf binds the full emitted
  doc — `ledgerCount`/`ledgerTip` changes move the root even when
  the claim id (content-derived per kind) doesn't.
- **Domain separation**: the leading byte means a leaf hash and a
  node hash can never collide by construction — a forged second
  level can't pose as a first.
- **No production call site** (same deferral class as
  `ResolveBencaoUnlocks`/`DeliverBuchao`): the relayer-facing
  caller lands with 11.3/11.4's emission path.

## Implementation Tasks

- [ ] `Campaign/Chain/LedgerMerkle.{h,cpp}` — domain-tagged
      FNV-1a helpers, `LedgerMerkleRoot` fold
- [ ] `potato_test_mintclaim` — extend with an 11.2 block (one
      test binary per chain area)
- [ ] Review (three passes), sprint-status sync

## Dev Notes — guardrails

- Everything per 11.1 Dev Notes: layering (`Campaign/` →
  `Gameplay/` public headers only; **no PotatoEngine link**),
  `Result<T>` at boundaries, PascalCase/camelCase/UPPER_SNAKE,
  MinGW junction build (`C:\MingGoRTS`), `uv`/`python`/`git` not
  on PATH.
- `std::span<const MintClaim>` — the caller decides which claims
  are "pending" (outbox Scan, an in-memory set, anything); the
  fold doesn't read directories.
- LE byte expansion: write the 8 bytes of a uint64
  least-significant-first — the same little-endian convention
  `EntryHash` uses for prevHash.
- `Ledger::Entries()` returns the append-ordered vector; seq is
  index-implicit (Verify enforces contiguity).

## Validation

- `potato_test_mintclaim` 11.2 block pins: empty fold → 0;
  determinism (same inputs → same root, twice); claim-order
  independence (shuffled span → same root); dedupe (repeated
  claim in the span → same root as once); sensitivity — mutating
  an entry posting, a claim's `ledgerTip`, or the claim set each
  moves the root; `SetSuspect` does NOT move it; odd/even leaf
  counts both fold.
- `ctest` green incl. `gameplay_dep_guard`.

## Dev Agent Record

**Implemented 2026-10-06** (MinGW via `C:\MingGoRTS` junction):

- `Campaign/Chain/LedgerMerkle.{h,cpp}` — `LedgerMerkleRoot`
  fold with per-level domain bytes (0x00 node / 0x01 entry /
  0x02 claim), FNV-1a same family as `EntryHash` and the record
  integrity root. Leaves: entry.hash (seq order) then claims
  sorted by (ClaimId, canonical emit) with exact-dup removal.
  Pair-wise fold, odd leaf promotes (RFC-6962 style); empty → 0.
- `potato_test_mintclaim` — +17 pins; all Validation items hold.

**Verification (MinGW, `build-mingw`):**
`potato_test_mintclaim` 46/46 PASS; focused `ctest` 5/5 green
incl. `gameplay_dep_guard`, `potato_test_bencao`,
`potato_test_ledger`, `potato_test_campaign`. MSVC not verified.
