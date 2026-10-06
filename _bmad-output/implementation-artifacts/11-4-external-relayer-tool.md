---
baseline_commit: NO_VCS
---

# Story 11.4 — External Relayer Tool

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · relay boundary ·
> **Status: done**

## Story (from epics.md)

As an operator,
I want a standalone relayer that consumes the outbox and submits
claims to chain,
So that no blockchain SDK ever touches the game binary.

## Acceptance Criteria

- **Given** an outbox dir + campaign ledger file, **when** the
  relayer runs, **then** it re-validates every claim before
  submitting: schema+id recompute+round-trip (the file is
  untrusted), seal consistency (claim's ledger block vs the
  file's head), and — for settlements — a `record_root:<hex>`
  anchor present in the ledger chain; invalid claims are skipped
  with a written reason, never aborting the batch. Dev-wallet key
  comes from env var or local config — never committed.

## Context

D-ARCH-9 draws the boundary: the game writes versioned JSON, an
external process relays it. 11.1–11.3 ship the game side; this
story is the first half of the other side — the untrusted-file
validator and batch discipline that stands between the outbox and
any transaction.

**Toolchain decision (deviation from epics.md's "C#/.NET"
parenthetical):** the story text assumed a C#/.NET precedent, but
none exists — there is no `Tools/` dir, and no `dotnet`/`csc` on
this machine. Installed runtimes: Node 24 + npm 12, rustc, MinGW
g++. **Node.js ESM, zero npm dependencies** — the web3 ecosystem
(ethers.js et al.) serves 11.5/11.6, and zero-dep keeps the tool
auditable and runnable without network access.

The relayer re-implements, in JS, exactly the verification the
game performs in C++:

- **Lossless JSON parse**: `seal`/`hash`/`tip` are int64 bit-casts
  whose magnitude exceeds 2^53 — `JSON.parse` would silently round
  them. A strict recursive-descent parser yields `BigInt` for all
  integers (the schemas carry no reals).
- **Canonical emit**: byte-for-byte replication of
  `JsonValue::Emit` (sorted keys, minimal escapes) so the ledger
  `EntryHash`/`SealHash` can be re-derived — a ledger file whose
  chain doesn't replay is rejected wholesale, same as
  `Ledger::FromJson`.
- **Claim validation**: mirror of `MintClaim::FromJson` including
  `ClaimId` recompute, plus a canonical emit→parse→validate
  round-trip before acceptance.
- **Seal consistency**: `claim.ledger.count == entries.length`,
  `claim.ledger.tip == Tip()` (genesis hash on empty chain).
- **Anchor**: settlement claims require some ledger entry tagged
  `record_root:<16hex>` matching `record_root` — byte-exact.

Actual transaction submission needs the contract ABI (Story
11.5). This story ships the submitter *seam*: `--endpoint` runs
an `eth_chainId` connectivity probe, and the report records every
accepted claim as `planned`; no real tx is sent until 11.5 lands
the contract driver behind the same interface.

## Design

`Tools/MintRelayer/` (outside CMake — nothing links this):

```
relayer.mjs     CLI: --outbox DIR --ledger FILE
                [--report FILE]  (default <outbox>.report.json)
                [--endpoint URL] [--config relayer.local.json]
src/json.mjs    strict lossless parse (ints → BigInt) +
                canonical Emit replication
src/fnv.mjs     FNV-1a-64 over bytes, BigInt
src/ledger.mjs  potato.ledger/3 verify (full Verify() replay +
                seal — mirrors Ledger::FromJson)
src/claim.mjs   potato.mintclaim/1 validate (mirrors FromJson +
                ClaimId recompute + round-trip)
src/run.mjs     scan → per-claim isolate → report
test/           node:test suite + C++-generated fixtures
```

Report schema `potato.relayer_report/1`:
`{schema, ledger:{count,tip,seal}, claims:[{file,id?,status,
reason?}], planned:[id...]}` — status `accepted`/`skipped`;
written atomically (tmp+rename) as a SIBLING of the outbox
(`<outbox>.report.json`) so the report never re-enters the scan.

Key handling: `POTATO_RELAYER_KEY` env var, or `keyFile`/`key`
fields in `--config` local JSON (`*.local.json` gitignored). The
key is read lazily — only when `--endpoint` is given — and never
appears in the report, stdout, or error text.

### Producer for fixtures + demo

`Examples/potato_export_chain.cpp` — deterministic exporter:
posts a small ledger (honest entries + one forged anchor), runs
`EmitGatedClaims`, writes `ledger.json` + claim files to a dir.
Doubles as the demo path:
`node Tools/MintRelayer/relayer.mjs --outbox out/claims
--ledger out/ledger.json`. Generated fixtures are checked into
`test/fixtures/` so the JS suite needs no built C++ exe.

## Implementation Tasks

- [ ] `Examples/potato_export_chain.cpp` + CMake target — fixture
      producer / demo
- [ ] `Tools/MintRelayer/` — package.json, src modules, CLI
- [ ] `test/relayer.test.mjs` — accept-reasons battery, per-file
      isolation, batch never aborts, report atomicity
- [ ] `.gitignore` — `*.local.json`, `node_modules/`
- [ ] Review (three passes), sprint-status sync

## Dev Notes — guardrails

- Zero npm deps for the validator; 11.5 may add ethers behind the
  submitter seam only.
- BigInt everywhere for hash-family values; `BigInt.asUintN(64,·)`
  after every multiply; compare wire int64s via
  `BigInt.asUintN(64, v)` — never `Number()`.
- Canonical emit key order = unsigned-byte order (C++ `std::map`
  on `std::string`), not UTF-16 order — fields are ASCII today
  but the comparator sorts bytes.
- The relayer re-checks `Ledger::Verify`'s full replay — chain
  link, stored-hash recompute, leg/meta invariants — not just the
  seal; a file is untrusted.
- Report path must not be inside `--outbox` (self-ingestion);
  refuse if it is.
- No network in default mode; `--endpoint` probe is the only
  socket use and is optional.

## Validation

- `node --test` suite: C++-exported fixtures accepted; tamper
  battery (bad schema, id mismatch, wrong seal, missing anchor,
  malformed JSON, non-object, oversized) each skipped with a
  reason while siblings still process; ledger chain-tamper file
  → wholesale rejection; report written to sibling path;
  env-var vs config key resolution; `--endpoint` absent → dry-run.
- `potato_export_chain` builds + runs under MinGW; ctest stays
  green.

## Dev Agent Record

**Implemented 2026-10-06** (MinGW via `C:\MingGoRTS` junction +
Node 24):

- `Examples/potato_export_chain.cpp` + `potato_export_chain`
  target — deterministic fixture/demo producer: three-post
  ledger (incl. anchored chapter seal), gate-emitted outbox,
  `ledger.json` via tmp→rename.
- `Tools/MintRelayer/` — zero-dep Node ESM relayer:
  - `src/json.mjs` — lossless parser (ints → BigInt; parity with
    `JsonValue::Parse`: depth 64, last-wins keys, surrogate
    pairing, int64-range degrade-to-Real) + byte-exact
    `Emit` replication (byte-order key sort, minimal escapes).
  - `src/fnv.mjs`, `src/ledger.mjs` — full `Verify()` replay:
    link + recompute + leg/meta invariants + seal, all u64
    arithmetic BigInt-clamped.
  - `src/claim.mjs` — `FromJson` mirror + `ClaimId` recompute +
    emit→parse→validate round-trip + seal-consistency +
    `record_root:` anchor checks.
  - `src/run.mjs` + `relayer.mjs` — sorted `.json` scan
    (case-folded, MAX_CLAIMS 4096), per-file isolation, atomic
    `potato.relayer_report/1` at a sibling path; `--endpoint`
    probes `eth_chainId`; key via `POTATO_RELAYER_KEY` env or
    config `keyFile`/`key`, never echoed.
- `test/relayer.test.mjs` + `test/fixtures/` — C++-produced
  fixtures checked in; 7 tests: accept-verbatim, 9-case tamper
  battery w/ sibling survival, ledger tamper → wholesale reject,
  seal truncation, report-path refusal, empty outbox, validator
  pins.
- `.gitignore` — `*.local.json`, `*.report.json`, node_modules.

**Toolchain note:** Node.js chosen over the epics.md "C#/.NET"
parenthetical — no dotnet SDK/csc on the machine and no Tools/
precedent existed; Node's web3 ecosystem serves 11.5/11.6.

**Deferred to 11.5:** actual tx submission (needs contract ABI);
the `--endpoint` path is probe-only by design.

**Verification (MinGW `build-mingw` + Node 24):**
`node --test` 7/7 pass; CLI end-to-end run on fixtures → 2/2
accepted, canonical report written; `ctest` 22/22 green incl.
`gameplay_dep_guard`. MSVC not verified (exporter is portable
C++17 filesystem code).
