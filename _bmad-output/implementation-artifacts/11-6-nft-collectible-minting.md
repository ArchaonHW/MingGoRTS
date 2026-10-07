---
baseline_commit: NO_VCS
---

# Story 11.6 — NFT Collectible Minting

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · collectible layer ·
> **Status: review**

## Story (from epics.md)

As a player,
I want to spend minted tokens striking keepsake plates (藏書票)
— chapter frontispieces, bencao entries, general dossiers —
as NFTs,
So that the campaign's own documents become things I hold
on-chain.

## Acceptance Criteria

- **Given** token balance and a chosen campaign artifact,
  **when** the burn+mint resolves, **then** an ERC-721 mints
  carrying a `potato.collectible/1` versioned metadata
  descriptor (artifact kind + source id + campaign seal)
  **and** collectible kinds map to existing artifacts —
  frontispiece (Epic 8), bencao entry (Epic 10), dossier
  (Epic 6) — with placeholder art valid until those epics land
  **and** the mint is gated on the artifact existing in the
  player's own campaign record — you can only seal what your
  chronicle wrote.

## Context

11.5 lands the token + anchor; this is the spend side. The gate
("artifact exists in the player's own record") is inherently
relayer-side — a contract cannot read the campaign save — so
the relayer re-uses its untrusted-file discipline: the player
presents evidence docs (bencao_state, campaign artifacts), the
relayer validates the artifact id is actually present, builds a
`potato.collectible/1` descriptor, burns the price, and mints.

Burn mechanics on a dev chain: `PotatoToken` gains
`burnFrom(from, amount)` — `onlyOwner` (the relayer wallet).
This is deliberately centralized for the demo: the relayer is
the trusted minter/burner of this dev-currency; production
would use ERC-20 allowance (`approve`/`burnFrom` standard) —
recorded as a deferred-work note.

Descriptor `potato.collectible/1`:
`{schema, kind, source_id, campaign:{count,tip,seal}, title?}`
— the campaign seal binds the collectible to the ledger state
that produced it. Stored on-chain per tokenId (dev chain: gas
is free; mainnet would tokenURI to IPFS — noted, not built).

Artifact-kind gates:
- `bencao` — `source_id` must appear in the presented
  `potato.bencao_state/1` `unlocked` list (real check —
  Epic 10 ships the state file).
- `frontispiece`, `dossier` — placeholder gates (Epics 8/6 not
  landed): the source id must appear verbatim in the provided
  evidence doc's canonical emit. Documented as placeholder.

## Design

`contracts/PotatoCollectible.sol` — minimal ERC-721-shaped
keepsake: `tokenId` auto-increment, `ownerOf`/`balanceOf`,
`descriptor(tokenId)` returning the stored JSON string;
`strike(bytes32 key, address to, string descriptor)` is
`onlyOwner` + `struck[key]` dedupe (false return, no revert).
`key = keccak256(kind ‖ ":" ‖ source_id)` — a player striking
the same artifact twice is a no-op.

`contracts/PotatoToken.sol` — add `burnFrom(address from,
uint256 amount)` `onlyOwner`.

`src/collectible.mjs` — `buildDescriptor(kind, sourceId,
ledgerState, evidenceDocs)`: gate check → `{ok, descriptor?,
reason?}`; `COLLECTIBLE_SCHEMA = "potato.collectible/1"`;
kinds `bencao|frontispiece|dossier`.

`src/submit.mjs` — `EthersSubmitter.strike(descriptor, to)`:
`token.burnFrom(to, price)` then `collectible.strike(key, to,
emitCanon(descriptor))`; price from config
(`collectiblePrice`, default 50).

`scripts/strike.mjs` — CLI: `--kind K --id ID --codex FILE
--ledger FILE [--evidence FILE …] --config deployed.local.json`
+ `--key-env`. Validates the ledger (full replay — the seal in
the descriptor must be real) before striking.

`chain.mjs deploy()` also deploys PotatoCollectible.

## Implementation Tasks

- [x] `contracts/PotatoCollectible.sol`; `PotatoToken.burnFrom`
- [x] `src/collectible.mjs` — descriptor + gates
- [x] `src/submit.mjs` — `strike()`; `chain.mjs` deploys 3rd
      contract
- [x] `scripts/strike.mjs` CLI
- [x] `test/collectible.test.mjs` — ganache E2E: fund player →
      strike → NFT minted + balance burned + descriptor on-chain;
      gate rejection (unlocked-miss); dedupe; non-owner revert
- [x] deferred-work note: production burn via allowance
- [x] Review, sprint-status sync

## Dev Notes — guardrails

- Everything per 11.4/11.5: zero-dep validator core, lazy
  ethers import, BigInt for wire ints, `*.local.json` secrets.
- The descriptor's campaign seal is recomputed from the
  presented ledger file — the player can't assert a seal their
  chain doesn't carry.
- `burnFrom` owner-gate is a dev-chain simplification —
  deferred-work entry records the allowance-based production
  path.
- Kind ids are the wire spellings `bencao`, `frontispiece`,
  `dossier`; unknown kinds reject.

## Validation

- `node --test` — existing 11 untouched + collectible suite:
  strike mints NFT + burns price + descriptor stored verbatim
  on-chain; bencao gate rejects absent ids; placeholder kinds
  gate on evidence text; duplicate strike no-ops; non-owner
  strike/burn reverts.
- ctest unaffected (tooling only).

## Dev Agent Record

**Implementation.** `PotatoCollectible.sol` ships a minimal
ERC-721-shaped keepsake: `strike(bytes32 key, address to,
string descriptor)` is `onlyOwner`, `struck[key]` dedupe
returns `false` (no revert — idempotent), `tokenId`
auto-increments from 1, `descriptor(tokenId)` returns the
stored `potato.collectible/1` JSON verbatim. `PotatoToken`
gained `burnFrom(from, amount)` `onlyOwner` for the spend side.
`EthersSubmitter.strike()` burns price (default 50, config
`collectiblePrice`) then mints; `tokenId` captured via
`staticCall`. `buildDescriptor()` gates: `bencao` requires
`source_id ∈ potato.bencao_state/1 .unlocked`; placeholder
kinds (`frontispiece`, `dossier`) require the id verbatim in
an evidence doc's canonical emit. The descriptor's campaign
seal is recomputed by replaying the presented ledger — the
strike CLI (`scripts/strike.mjs`) validates the full ledger
before striking.

**EVM-version fix.** solc 0.8.28 defaults to the `cancun`
target, which emits `MCOPY` for the `descriptor()` string
getter; ganache 7.9.2 only supports up to Shanghai →
`eth_call` failed with `missing revert data` / invalid opcode
(mint itself worked — the tx path didn't hit MCOPY). Pinned
`evmVersion: "shanghai"` in `compileContracts()`; descriptor
reads now succeed.

**Verification.** `node --test` — **16/16**: descriptor gate
accept/reject, burn+mint E2E (`strike` → tokenId 1, balance
−50, `ownerOf` = player, on-chain descriptor round-trips),
dedupe second strike no-ops, non-owner `strike`/`burnFrom`
revert, unknown kind rejects. ctest untouched (tooling only).
