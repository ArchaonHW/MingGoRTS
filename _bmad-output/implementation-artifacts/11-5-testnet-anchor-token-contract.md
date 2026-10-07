---
baseline_commit: NO_VCS
---

# Story 11.5 — Testnet Anchor & Token Contract

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · chain contracts ·
> **Status: review**

## Story (from epics.md)

As a developer,
I want an anchor store + ERC-20 token deployed on a test network
(Sepolia or local Anvil) with the relayer submitting mint calls,
So that verified play produces real on-chain artifacts.

## Acceptance Criteria

- **Given** a verified claim batch, **when** the relayer submits,
  **then** the Merkle root lands in the anchor store and tokens
  mint to the player wallet **and** re-submitting an identical
  root/claim is idempotent (on-chain dedupe or client-side skip)
  **and** the entire path is demonstrable on a local Anvil/
  Hardhat chain without Sepolia access.

## Context

11.4 ships the validator + the submitter seam: claims validated
against the ledger become `planned` entries. This story lands the
chain end — two contracts, a real local EVM, and the ethers
driver behind the seam.

**Chain choice (environment-driven):** no Anvil/forge/Hardhat on
this machine; npm IS reachable. **`ganache`** — the npm-native
local EVM (same class as Anvil/Hardhat node) — runs in-process
for tests and as a daemon for the demo. `solc` (solc-js) +
`ethers` v6 complete the toolchain; all three are devDeps of
`Tools/MintRelayer` only — the game binary never sees them.

The anchor root is the **LedgerMerkleRoot over verified state**:
the relayer re-computes it from the replayed ledger + accepted
claims (JS port of the 11.2 fold) rather than trusting a root in
the file — the same untrusted-input discipline as everything else
it relays.

## Design

`Tools/MintRelayer/contracts/`:

- `PotatoAnchor.sol` — `anchor(bytes32)` stores
  `root→timestamp`, emits `Anchored`; already-anchored roots
  return the original timestamp (no-op — on-chain idempotence).
  `anchoredAt(bytes32)` view.
- `PotatoToken.sol` — minimal ERC-20 (`POTATO`, `decimals 0` —
  the game's integer-only discipline on-chain).
  `mint(bytes32 claimId, address to, uint256 amount)` is
  `onlyOwner` (the relayer wallet) and dedupes via
  `claimed[claimId]` — a re-submitted claim mints zero, returns
  false, never reverts. Claim id → `keccak256(utf8 id)` on-chain
  key.

`src/merkle.mjs` — JS port of `LedgerMerkleRoot`: entry leaves in
seq order (`0x01‖hash LE`), claim leaves sorted by (id,emit)
(`0x02‖canonical emit`), `0x00‖L‖R` nodes, odd-leaf promote. Root
is the u64 → bytes32 (low 8 bytes, BE).

`src/chain.mjs` — solc compile + deploy helpers shared by the
test suite and `scripts/deploy.mjs` (writes
`deployed.local.json`, gitignored).

`src/submit.mjs` — `EthersSubmitter`: anchors the batch root once,
then `mint(keccak256(id), player, amount)` per accepted claim.
Amounts come from config (`amounts.settlement`/`amounts.
achievement`; defaults 100/10). Returns per-claim tx results;
contract-level dedupe makes a second `submit()` a no-op.

`relayer.mjs` gains `--submit`: with it, accepted claims go to
`EthersSubmitter` (needs `--endpoint` + key + `--anchor`/`--token`
/`--to` or the same fields in `--config`); without it, unchanged
dry-run. Report gains `submission` (root, anchoredAt, minted[],
skipped[]).

## Implementation Tasks

- [ ] `contracts/PotatoAnchor.sol`, `PotatoToken.sol`
- [ ] `src/merkle.mjs` — fold port + bytes32 pack
- [ ] `src/chain.mjs`, `scripts/deploy.mjs` — compile/deploy
- [ ] `src/submit.mjs` + `relayer.mjs --submit` wiring
- [ ] `test/chain.test.mjs` — ganache in-process E2E: deploy →
      submit → balances/anchor assertions → idempotent re-submit
- [ ] `potato_export_chain` — also emits the batch merkle root
      (`merkle.json`) so the JS root is pinned against C++
- [ ] Review (three passes), sprint-status sync

## Dev Notes — guardrails

- npm deps (solc/ethers/ganache) are devDeps only; the validator
  core stays zero-dep — lazy `import("ethers")` inside submit so
  `node --test` of the validator needs no node_modules.
- Root packing: u64 → `zeroPadValue(toBeHex(root,8),32)`.
- Contract dedupe is the idempotence anchor — client-side skip is
  an optimization, never the guarantee.
- `deployed.local.json` + `*.local.json` gitignored; no keys,
  no addresses committed.
- Test runs against ganache in-process (no network beyond
  localhost); Sepolia is the same `--endpoint` shape, later.

## Validation

- `node --test` — existing 7 validator tests untouched + chain
  suite: compile succeeds, deploy works, submit anchors the
  C++-matched merkle root and mints the expected amounts,
  second submit changes nothing (idempotent), non-owner mint
  reverts.
- `potato_export_chain` regenerated fixtures carry `merkle.json`
  — JS `ledgerMerkleRoot` must equal it byte-for-byte.
- ctest stays green (new example code only).

## Dev Agent Record

**Implemented 2026-10-06** (Node 24 + solc-js 0.8.28 + ethers
6.13.5 + ganache 7.9.2, all devDeps of `Tools/MintRelayer`):

- `contracts/PotatoAnchor.sol` — `root→timestamp` store;
  re-anchoring returns the original stamp (on-chain idempotence).
- `contracts/PotatoToken.sol` — minimal ERC-20 (`POT`, 0
  decimals); `mint(claimId,to,amount)` is `onlyOwner` +
  `claimed[]` dedupe (returns false, never reverts).
- `src/merkle.mjs` — JS port of the 11.2 fold; `rootToBytes32`
  packs the u64 into the low 8 bytes BE.
- `src/chain.mjs` + `scripts/deploy.mjs` — solc compile +
  ethers deploy; writes `deployed.local.json`.
- `src/submit.mjs` — `EthersSubmitter`: recomputes the root over
  verified state (never trusts a file), anchors once, mints per
  unique claim id with per-kind amounts (default 100/10,
  config-overridable); `signer` cfg key allows in-process tests.
- `relayer.mjs` — `--submit` flag; key required only when
  submitting or probing.
- `potato_export_chain` — now also emits `merkle.json`
  (C++-computed root) into the fixture dir.
- `run.mjs` — report gains `submission` (root, anchoredAt,
  minted[]); unique-claim docs feed the fold.

**Verification:**
`node --test` 11/11 pass — incl. ganache in-process E2E
(deploy→submit→balance 110 POT + anchored root→idempotent
resubmit→non-owner revert) and JS merkle == `merkle.json` C++
root byte-exact. `potato_export_chain` builds under MinGW;
ctest verified green on the earlier run (new code is exporter
output only). MSVC not verified.
