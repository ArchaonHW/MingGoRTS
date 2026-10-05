---
title: "Sprint Change Proposal — Epic 11: MT 鑄鏈存證 (Mint & Anchor)"
date: 2026-10-05
project: MingGoRTS
trigger: "New requirement: metaverse / play-to-earn virtual currency (forged to testnet token + NFT collectible layer)"
status: draft
source: "_bmad-output/forge/metaverse-currency/forged-idea.md"
---

# Sprint Change Proposal — 2026-10-05

## 1. Issue Summary

**Trigger:** New stakeholder requirement (not a defect). The user requested bringing "metaverse" concepts and play-to-earn virtual currency into MingGoRTS. A forge-idea session hardened the raw idea into a buildable design: **a blockchain technology showcase** — a testnet token minted on replay-verified chapter settlements plus NFT collectibles, integrated via an external relayer tool. Engine stays network-free; no real-money value; single-player architecture unchanged.

**Evidence:** `_bmad-output/forge/metaverse-currency/forged-idea.md` — locked decisions, rejected options (mainnet P2E, in-engine RPC, player-to-player trading), and accepted risks are recorded there.

**Type:** New requirement emerged from stakeholders → **additive scope**, zero rework of existing stories.

## 2. Impact Analysis

### 2.1 Epic Impact

| Epic | Impact | Detail |
|---|---|---|
| 1–10 | None | Additive change; no existing story modified. In-flight Epics 6 & 10 unaffected. |
| **New Epic 11 (MT)** | Add | Mint-claim pipeline + local chain primitives + external relayer + testnet anchoring + NFT collectibles. |
| Epic ordering | End of sequence | Epic 11 lands after Epic 9/10; dependencies (1.11 BattleRecorder, 2.2 hash chain, 3.2 atomic save) are all `done`. Soft content deps on Epic 8 (frontispiece art) and Epic 10 (Bencao codex) for NFT payloads — collectibles can ship with placeholder art. |

### 2.2 Story Impact

- **Modified stories:** none.
- **New stories (proposed Epic 11):**
  - 11.1 `MintClaim` schema + outbox model — `potato.mintclaim/1` versioned JSON; claim types (chapter-settlement, achievement); written via atomic tmp→rename at aftermath boundary only.
  - 11.2 Ledger Merkle root — fold the existing append-only hash chain (Story 2.2, done) into a Merkle root export; deterministic, headless, integer-only.
  - 11.3 Replay-gated claim emission — claims emitted only when BattleRecorder verification passes (Story 1.11, done) and achievement trigger keys resolve (zero-combat chapter, forgery bust, four-voice ending).
  - 11.4 External relayer tool — standalone tool (`Tools/MintRelayer/`, C#/.NET per GDD tooling precedent) that watches the claim outbox, verifies chain integrity, calls the testnet contract.
  - 11.5 Testnet anchoring + token contract — ERC-20 on Sepolia/local Anvil; Merkle root anchored on-chain; dev-wallet key from env var, never in repo.
  - 11.6 NFT collectible minting — token burn → ERC-721 mint of campaign artifacts (frontispiece 冊頁, Bencao codex entry, general dossier); metadata as versioned JSON.

### 2.3 Artifact Conflicts

| Artifact | Conflict | Required Update |
|---|---|---|
| GDD | "Out of Scope: single-player only / non-commercial" — **no conflict** (testnet, zero value, still single-player), but needs an explicit note so the chain layer isn't read as a scope violation. Add "chain provenance layer" to Resource Systems + new assumption row. | Minor section edit |
| game-architecture.md | "Networking: None — strictly single-player" needs refinement: no networking **in-engine**; the chain boundary is file-based (outbox JSON → external process). Add decision **D-ARCH-9: external relayer boundary**. | Add decision + module note |
| narrative-design.md | Optional fiction binding: collectible layer as 史館印鑑 / 藏書票 (archive-seal bookplates) — fits chronicle voice. | Optional |
| UX designs | Optional claim-status marginalia in chronicle documents; rides Epic 6/8 surface. | Optional stretch |
| epics.md | Add FR22 + Epic 11 stories; update FR coverage map. | Add section |
| sprint-status.yaml | Add `epic-11` + story entries as `backlog`. | After approval |
| bencao-worldview.md / docs | None | — |
| CI/CD, deployment | None exist | — |

### 2.4 Technical Impact

- **Layering preserved:** claim emission lives in `Campaign/Chain/` (headless, file I/O only at aftermath boundary — never in tick path). Relayer is outside the C++ tree entirely.
- **NFR preserved:** determinism untouched (claims derive from already-committed ledger state, no new sim input); zero file I/O in tick path unchanged; versioned-JSON rule satisfied via `potato.mintclaim/1`.
- **New external dep:** testnet RPC + contract toolchain (Foundry/`cast` or ethers/web3 lib inside the relayer process only). Dev-machine-only; no engine linkage.
- **Security:** private key lives in env/local config on the dev machine; claims are integrity-bound to the ledger hash chain but client-side forgery is possible — accepted risk (testnet, zero value).

## 3. Recommended Approach

**Option 1 — Direct Adjustment (selected).** Add Epic 11 at the end of the execution sequence; modify no existing stories.

- Option 2 (Rollback): not viable — nothing to revert; change is additive.
- Option 3 (MVP review): not needed — Epic 11 is off the critical path; v1.0 ships fine without it.

**Rationale:** all hard dependencies are already `done`; the feature is a boundary-layer appendage, not an architectural change. Keeps in-flight epics untouched.

**Effort:** Medium (local-chain primitives are small — the existing hash chain does most of the work; the unknowns concentrate in the relayer + contract toolchain).
**Risk:** Low-Medium (external toolchain unfamiliar; mitigated by testnet + file boundary).
**Timeline:** no impact on existing sprint items; Epic 11 schedules after Epics 9/10 or in parallel once its deps hold.

## 4. Detailed Change Proposals

### 4.1 epics.md

```
NEW — Requirements Inventory, Functional Requirements:
+ FR22: Chain provenance & mint claims — Merkle root over ledger hash chain;
+ replay-gated `potato.mintclaim/1` outbox at aftermath boundary; external
+ relayer anchors root + mints testnet token; token burns mint NFT
+ collectibles of campaign artifacts. Testnet only; zero real-world value;
+ engine remains network-free (file boundary to external process).

NEW — Epic List:
+ ### Epic 11: MT — 鑄鏈存證 (Mint & Anchor)
+ The chronicle gains a public seal: replay-verified victories mint a testnet
+ token; the token mints NFT keepsakes of the campaign's documents. Pure
+ showcase of blockchain primitives — file boundary to an external relayer;
+ the engine never touches a network.
+ **FRs covered:** FR22

FR Coverage Map: append row `| FR22 | MT | Mint claims, Merkle anchor, relayer, NFT collectibles |`
Execution sequence: append `→ MT` (after G/BC; deps already done).
```

### 4.2 GDD

```
Section "Out of Scope" — append clarifying note:
+ v1.0 remains single-player and non-commercial. A testnet-only blockchain
+ provenance layer (Epic MT) is included as a technology showcase: mint
+ claims leave the game as versioned files; an external tool anchors them
+ to a test network. No real-money value, no multiplayer, no in-engine
+ networking.

Section "Resource Systems" — append:
+ Mint-claim outbox — chapter settlements and rare achievements emit
+ `potato.mintclaim/1` files (aftermath boundary only), gated by replay
+ verification; an external relayer turns verified claims into testnet
+ tokens and NFT collectibles (章回箋 / 本草圖鑑 / 名將檔案).

Assumptions — append:
+ 11. Testnet anchoring (Sepolia or local Anvil) via external relayer;
+    dev-wallet key from environment, never committed.
```

### 4.3 game-architecture.md

```
Decision Summary — append:
+ | D-ARCH-9 | Chain boundary | External relayer over versioned-JSON
+   outbox (`Campaign/Chain/` writes claims; `Tools/MintRelayer/` does web3)
+   | Engine has no networking; file boundary preserves headless sim,
+   | determinism, and zero-I/O tick path; matches C# tooling precedent |

Project Context → Technical Scope:
- "Networking: None — strictly single-player"
+ "Networking: None in-engine — strictly single-player; optional external
+  relayer reads versioned-JSON claim files (process boundary, never in-engine)"
```

### 4.4 narrative-design.md (optional)

```
+ The mint layer presents in-fiction as 史館印鑑 (the archive's seal):
+ verified battles receive a seal; seals may be struck into keepsake
+ plates (藏書票) of the chronicle's documents.
```

### 4.5 sprint-status.yaml (on approval)

```yaml
+ epic-11: backlog
+ 11-1-mintclaim-schema-outbox: backlog
+ 11-2-ledger-merkle-root: backlog
+ 11-3-replay-gated-claim-emission: backlog
+ 11-4-external-relayer-tool: backlog
+ 11-5-testnet-anchor-token-contract: backlog
+ 11-6-nft-collectible-minting: backlog
+ epic-11-retrospective: optional
```

## 5. Implementation Handoff

**Scope classification: Moderate** — new epic, backlog reorganization, no rework.

**Handoff:**
1. This proposal approved → apply edits in §4 (epics.md, gdd.md, game-architecture.md, sprint-status.yaml; narrative note optional).
2. `gds-create-story` per Epic 11 story when it enters the sprint queue.
3. External toolchain decision (Foundry vs ethers.js vs web3.NET) deferred to Story 11.4/11.5.

**Success criteria:**
- Engine/game build unchanged headless; `potato_test_*` for Merkle root + claim emission green on MSVC.
- A verified chapter settlement produces a `potato.mintclaim/1` file; relayer anchors a Merkle root on a testnet and mints a token; token burn mints one ERC-721 collectible.
