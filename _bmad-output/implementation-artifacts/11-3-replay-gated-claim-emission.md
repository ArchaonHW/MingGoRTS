---
baseline_commit: NO_VCS
---

# Story 11.3 — Replay-Gated Claim Emission

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · emission gate ·
> **Status: done**

## Story (from epics.md)

As a system,
I want claims emitted only when the battle record verifies clean
and the ledger anchors it,
So that the relayer never sees a claim the chronicle can't prove.

## Acceptance Criteria

- **Given** ResolveAftermath completing with a recordRoot,
  **when** the record cross-checks clean against the ledger
  (CrossCheckRecord Clean), **then** a chapter-settlement claim
  emits into the outbox **and** an inconsistent or unanchored
  record emits nothing (the failure confessed, not minted)
  **and** achievement trigger keys (zero-combat resolution,
  forgery bust, four-voice ending) emit achievement claims
  through the same outbox.

## Context

11.1 ships the claim container + atomic outbox; 11.2 the anchor
fold. This story is the *gate* between the aftermath ceremony and
the outbox — the only path by which a claim earns the right to a
file. The proof machinery already exists:

- `CrossCheckRecord(ledger, recordDoc)` recomputes the record's
  integrity root and checks every `record_root:` anchor agrees —
  verdict `Clean` means "the chronicle can prove this battle".
  Non-Clean verdicts each carry their own confession
  (RecordInconsistent already flags diverging anchors suspect;
  NoAnchor means the ledger never claimed this record).
- `ResolveAftermath` already anchors `record_root:` into the
  resolution seal, so a settled chapter's record is covered by
  construction — the gate verifies it anyway: the file is
  untrusted, the verdict is the gate.

The gate writes ONLY into the outbox. CrossCheck may flag suspect
entries inside its own contract; the gate itself posts nothing —
a refused claim leaves no artifact except the refusal the caller
confesses in the report stream.

## Design

`Campaign/Chain/ClaimGate.{h,cpp}`:

```cpp
// The replay gate (Story 11.3): cross-checks the battle record
// against the ledger, and only on verdict Clean emits the
// chapter-settlement claim plus any achievement claims through
// the outbox. A non-Clean verdict emits NOTHING — the verdict
// rides back in the report for the caller to confess.
struct ClaimGateReport {
    CrossVerdict verdict = CrossVerdict::BadRecord;
    std::uint64_t recordRoot = 0;         // recomputed root used
    std::vector<std::filesystem::path> emitted; // committed files
};

// Achievement trigger keys (the AC's three; charset is the
// MintClaim achievement-key alphabet [a-z0-9_]).
inline constexpr std::string_view ACH_ZERO_COMBAT = "zero_combat";
inline constexpr std::string_view ACH_FORGERY_BUST = "forgery_bust";
inline constexpr std::string_view ACH_FOUR_VOICE = "four_voice_ending";

// `resolution` is the ResolutionName spelling (or "subversion");
// `chapter` the chapter just settled; `achievementKeys` the
// caller-asserted triggers — the gate ADDS "forgery_bust" itself
// when the cross-check found forged anchors claiming the record
// (only the gate sees that evidence). Emits into `outbox`; every
// claim self-validates ToJson→FromJson before commit.
Gameplay::Result<ClaimGateReport> EmitGatedClaims(
    Ledger& ledger, const Gameplay::JsonValue& recordDoc,
    std::string_view resolution, std::int64_t chapter,
    std::span<const std::string_view> achievementKeys,
    const MintOutbox& outbox);
```

### Rules

- **Clean-or-nothing**: `verdict != Clean` → `report.verdict` set,
  `emitted` empty, Ok — the refusal is data, not an error. All
  non-Clean verdicts refuse alike (NoAnchor, RecordInconsistent,
  ChainBroken, BadRecord).
- **Root used is `recomputedRoot`** — for Clean, declared ==
  recomputed; the gate binds what it proved, not what was said.
- **Seal at emission time**: `ledgerCount = Size()`,
  `ledgerTip = Tip()` — the claim binds the chain state the gate
  saw, not the settlement-time value (identical in practice; the
  relayer re-verifies).
- **Self-validation**: each claim round-trips `ToJson →
  FromJson` before `Emit` — a claim the gate can't read back is a
  `field` failure, never a file.
- **Dedupe by ClaimId**: forgery_bust auto-added + caller-passed
  keys may collide — emit each id once, `emitted` order =
  settlement claim first, then achievements in id order.
- **Emit failure propagates**: an outbox io/field failure is a
  `Fail` — partial commits surface as errors (files already
  written remain; idempotent re-emission heals the set).

## Implementation Tasks

- [ ] `Campaign/Chain/ClaimGate.{h,cpp}` — gate + report + key
      constants
- [ ] `potato_test_mintclaim` — extend with an 11.3 block
- [ ] Review (three passes), sprint-status sync

## Dev Notes — guardrails

- Everything per 11.1/11.2 Dev Notes: layering, `Result<T>`,
  naming, MinGW junction build, no `uv`/`python`/`git`.
- `CrossCheckRecord` takes `Ledger&` non-const — flagging
  diverging anchors suspect is its own contract; the gate passes
  the same reference through.
- The record doc is the raw `potato.battle_record` JsonValue —
  the gate never parses it beyond what CrossCheck does.
- No production call site (same deferral class): the aftermath
  caller lands with the Game shell's settle path.

## Validation

- `potato_test_mintclaim` 11.3 block pins: Clean verdict emits
  settlement + achievements (files exist, ids correct); each
  non-Clean verdict emits zero files; `emitted` order; dedupe of
  an auto-added forgery_bust against a caller-passed one;
  settlement claim carries recomputedRoot + live seal; an
  achievement claim round-trips through FromJson; outbox-write
  failure propagates as `Fail`.
- `ctest` green incl. `gameplay_dep_guard`.

## Dev Agent Record

**Implemented 2026-10-06** (MinGW via `C:\MingGoRTS` junction):

- `Campaign/Chain/ClaimGate.{h,cpp}` — `EmitGatedClaims`,
  `ClaimGateReport`, ACH_* key constants. Clean-or-nothing on
  `CrossCheckRecord`; settlement binds `recomputedRoot` + live
  seal; `forgery_bust` auto-adds on forged anchors; claims
  self-validate ToJson→FromJson before `Emit`.
- `potato_test_mintclaim` — +13 pins (Clean emit path, forged-
  anchor dedupe, all three refusal verdicts leave no artifact).
- Incidental: repaired two private-access errors and a duplicated
  `WorldMap::Load` in the in-flight `Campaign/World/` (12.1)
  files so PotatoCampaign builds.

**Verification (MinGW, `build-mingw`):**
`potato_test_mintclaim` 59/59 PASS; focused `ctest` 6/6 green
incl. `gameplay_dep_guard`, `potato_test_bencao`,
`potato_test_world`, `potato_test_ledger`, `potato_test_campaign`.
MSVC not verified.
