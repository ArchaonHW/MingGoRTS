# Story 5.3 — 天命 Currency

## Status

done

## Story

As a player,
I want 天命 earned through myth-layer deeds and spent on my myth
actions,
so that the gods have an economy.

## Acceptance Criteria

1. Given a shrine pacified, when the ledger posts, then 天命 credits
   on the five-account ledger.
2. Myth actions debit 天命 with insufficient-funds rejection.
3. All postings are ordinary double-entry entries — hash-chained,
   foldable, forgeable-suspectable like everything else.

## Design

- **Earn leg** (`DeedBook`, Story 4.2's bridge): `ShrineCaptured`
  (SimEvent kind 10, player-side only — the ledger is the player's
  chronicle) posts `+天命 10 / −物資 5` — dedication consumes
  offerings. Tags: `myth` (new canonical fold key,
  `Ledger::TAG_MYTH`), `region:<n>`, `order:+2` (sacred ground
  legitimized calms the land). GDD tempo: ~10/chapter early.
- **Spend gate** (`Campaign/Myth/Mandate.{h,cpp}` —
  `SpendMandate(ledger, posting)`): the ledger deliberately permits
  negative balances (a deficit is meaningful ledger state), so the
  insufficient-funds rule lives HERE, at the spend boundary — not in
  the ledger. Contract: `debit.account == Mandate`,
  `Balance(Mandate) >= debit.amount`, then `Post`. Story 5.4's myth
  actions call this gate; each action picks its own credit sink
  (pacify → 民心 awe, ghost army → 軍威 dread).
- **Forged mandate spends**: forged entries fold into Balance like
  honest ones (2.3 contract — suspicion is data, not corruption), so
  a forged 天命 grant really does spend; the audit trail is the
  suspect flag. Tested, not accidental.

## Tasks

- [x] `Ledger::TAG_MYTH` canonical fold key
- [x] `DeedBook`: `ShrineCaptured` case
- [x] `Campaign/Myth/Mandate.{h,cpp}` — `SpendMandate` gate
- [x] Tests: credit posting, spend ok/exact-boundary/insufficient,
      forged-mandate spend, non-Mandate debit rejected
- [x] ctest + dep guard — 19/19
- [x] Three-layer review

## Review Notes

- AC / Edge / Blind passes: no HIGH/MED findings.
- Edge-self: `debit.amount <= 0` is gated BEFORE the balance fold —
  otherwise `Balance(0) >= cost(0)` would pass vacuously and Post
  would reject with the wrong reason.
- `credit == Mandate` needs no explicit check: `ValidateLegs`
  already requires the two legs name different accounts, and the
  debit leg is pinned to Mandate by the gate.
- LOW accepted: a `cost > MAX_AMOUNT` posting on a poor purse fails
  "insufficient 天命" rather than "amount too large" — Post still
  rejects either way; the gate's job is the funds rule.
- Blind: `DeedBook.h` comment now names the myth earn leg.

## File List

- `Campaign/Ledger/Ledger.h` — `TAG_MYTH` fold key
- `Campaign/Ledger/DeedBook.{h,cpp}` — `ShrineCaptured` →
  `+天命10 / −物資5`, tags `myth`/`region:<n>`/`order:+2`
- `Campaign/Myth/Mandate.{h,cpp}` — `SpendMandate` insufficient-
  funds gate
- `Examples/potato_test_ledger.cpp` — 19 new 5.3 checks

## Deferred Work

- Myth action verbs (pacify/possession/ghost army) calling
  `SpendMandate`: **Story 5.4** — each names its own credit sink.
