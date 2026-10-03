# Story 2.1: Five-Account Double-Entry Ledger

## Status: done

## Story

As a system,
I want every action to post a balanced entry pair across
武功/民心/天命/軍威/物資,
So that nothing is free.

First Epic L story. Lands `Campaign/Ledger` — the campaign's state
substrate. Story 2.2 adds the hash chain; 2.3 the forgery channel;
Epic 4 wires battlefield deeds into postings.

## Acceptance Criteria

1. **Given** a game event (e.g., village burned: +物資, −民心),
   **when** posted, **then** both halves of the pair record atomically
   **and** an unbalanced posting is rejected.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Campaign/Ledger/Ledger.{h,cpp}` (new layer)
  - `enum class Account { MartialMerit, PopularSupport, Mandate,
    ArmyPrestige, Materiel }` — the five spendable accounts.
    (秩序/墮落 are derived accumulators, Epic 4 — NOT here.)
  - `struct Leg { account, amount }` — one side of a pair;
    `amount > 0` always.
  - `struct LedgerEntry { seq, credit Leg, debit Leg, memo, tags }` —
    the append-only unit. `seq` is the stable append order key.
  - `struct Posting { credit, debit, memo, tags }` — the write input.
  - `Ledger::Post(Posting) -> Result<uint64_t seq>`: the ONLY write
    path. Rejects: either amount <= 0, or credit.account ==
    debit.account. Magnitudes need NOT match — asymmetric costs are
    the design (+40 物資 / −15 民心); the invariant is structural:
    every gain books a priced cost in a different account.
  - `Balance(account) -> int64_t`: pure fold over entries; balances
    are computed, never stored. Negative balances legal.
  - Append-only: `Entries()` returns const view; no mutation API.
- [x] (AC: 1) Task 2 — `potato.ledger/1` serialization
  - `ToJson`/`FromJson` round-trip; strict field typing; the same
    balance invariant is re-validated on load (a corrupted file
    can't smuggle in an unbalanced entry).
  - Account wire ids: `martial_merit`/`popular_support`/`mandate`/
    `army_prestige`/`materiel` (stable ASCII tokens; CJK labels are
    presentation-layer, Epic 8).
- [x] Task 3 — `PotatoCampaign` lib + dep guard extension
  - New static lib globbing `Campaign/*.cpp`, links PotatoGameplay
    (uses `Gameplay/Json`, `Gameplay/Result`); dependency direction
    Gameplay ← Campaign holds.
  - `CheckGameplayDeps.ps1`: scan `Campaign/` for the same forbidden
    tokens + `Game/`; add `Campaign/`/`Game/` to the Gameplay scan.
- [x] Task 4 — `Examples/potato_test_ledger.cpp`
- [x] Task 5 — CMake registration; MinGW build + ctest + dep guard.
- [x] Task 6 — three-layer review.

## Dev Notes

- The posting is the atomic unit: validate-then-append — a rejected
  posting leaves `Entries()` untouched.
- `LedgerEvent` (Story 1.12, Gameplay/Eval) is the battle-side
  *source* of postings; battle→ledger translation is Epic 4's job.
- Forgery/suspect (2.3) is a deliberately separate write path —
  `Post` stays the honest channel.
- `potato.ledger/1` tolerates unknown fields (forward-compat); the
  hash chain (2.2) should bump to `potato.ledger/2` since a /1 loader
  would silently skip chain verification — and unknown per-entry
  fields are dropped on resave (relevant when 2.3 adds `suspect`).

## Dev Agent Record

### Implementation Plan

`Campaign/Ledger` (first Campaign-layer file: Account/Leg/Posting/
LedgerEntry/Ledger + shared ValidateLegs/ValidateMeta) →
`potato.ledger/1` round-trip → `PotatoCampaign` lib + dep guard
extension → potato_test_ledger.

### Completion Notes

- `Account{MartialMerit,PopularSupport,Mandate,ArmyPrestige,Materiel}`
  with ASCII wire ids; 秩序/墮落 stay derived (Epic 4), not booked.
- `Post` validates legs (amounts > 0 and ≤ MAX_AMOUNT=1e12, accounts
  known and distinct) then meta (memo ≤256, tags ≤16×64 chars,
  non-empty, no duplicates) then appends atomically; `MAX_ENTRIES`
  (1M) caps both write and load paths so a posted ledger always
  reloads.
- `Balance` is a pure int64 fold — bounded to ~2e18 by the
  amount×entry caps; no stored accumulators.
- `potato.ledger/1`: strict typing, seq must be contiguous from 0,
  legs re-validated on load, meta re-validated via shared
  `ValidateMeta` — a crafted save can't smuggle state `Post` forbids.
- `PotatoCampaign` links `PotatoGameplay`; dep guard now scans both
  layers (`Campaign/` + `Game/` forbidden in Gameplay; `Game/` +
  presentation tokens forbidden in Campaign; *.hpp included).

### File List

- Campaign/Ledger/Ledger.h/.cpp (new)
- Examples/potato_test_ledger.cpp (new)
- CMakeLists.txt (PotatoCampaign, potato_test_ledger)
- scripts/CheckGameplayDeps.ps1 (two-layer scan)
- _bmad-output/implementation-artifacts/2-1-five-account-double-entry-ledger.md
- _bmad-output/implementation-artifacts/sprint-status.yaml
- _bmad-output/implementation-artifacts/deferred-work.md

### Debug Log References

- `.\build-mingw\bin\potato_test_ledger.exe` — 40/40 checks.
- `ctest --test-dir build-mingw` — 16/16 (all green).

### Change Log

- 2026-10-03: Story drafted.
- 2026-10-03: Implemented; three-layer review patched (account range
  check, MAX_AMOUNT/MAX_ENTRIES caps, shared ValidateMeta, memo cap,
  tag set-semantics, dep guard *.hpp); 40 checks, 16/16 ctest.

## QA Results

Three-layer review (Blind Hunter / Edge Case Hunter / Acceptance
Auditor) — AC:PASS on first pass; patched: phantom-account postings
now rejected at validate (were postable but unloadable); Balance
overflow bound enforced via MAX_AMOUNT+MAX_ENTRIES on both write and
load; shared `ValidateMeta` closes the load-weaker-than-write gap;
duplicate/empty tags rejected (fold keys are set semantics); memo cap;
error class `posting` for meta rejections; test gate checks now assert
`bad.ok()` before `!FromJson(...)`. Deferred: dep guard is a textual
tripwire (substring over-match on dir names ending `game/`/`gui/`,
`#if 0` blocks, macro-aliased includes, unscanned `.inl/.cc/.cxx`);
`potato.ledger/2` bump advised when the hash chain lands.
