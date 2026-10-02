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
