# Story 4.3: Governance Accumulators

## Status: done

## Story

**As a** system,
**I want** 民心/秩序/墮落 derived by folding ledger entries, with 墮落
monotonic,
**So that** governance is computed truth, not a stat bar.

## Acceptance Criteria

1. Given a ledger chain,
   When accumulators fold,
   Then 民心/秩序 reflect net postings and 墮落 equals the max over
   atrocity-tagged folds
   And no code path can write an accumulator directly.

## Dev Notes

- **民心** is a real account — `popularSupport =
  Balance(Account::PopularSupport)`. Net postings, exactly as the
  ledger already folds.
- **秩序** is NOT an account (Ledger.h: "秩序/墮落 are deliberately
  absent"). It folds from tags: `order:±N` contributions sum to the
  accumulator. Sign optional (`order:5` = +5); malformed governance
  tags don't fold (same fail-quiet posture as `record_root:`).
- **墮落** is the ratchet: fold `corruption:±N` tags in seq order and
  keep the **running maximum**. Penance entries may drag the running
  sum down; the accumulator remembers the peak — "a one-way debt
  that can never be repaid, only outrun" (GDD). Equals the max over
  atrocity-tagged folds because atrocity deeds are the producers.
- **No write API, by construction**: `FoldGovernance(const Ledger&)`
  is a pure function returning a POD by value. There is no
  accumulator object to mutate, no setter, no persisted field —
  saves re-derive at load (posting twice would double-book).
- **Forged entries fold** (2.3 contract — suspicion is data, not
  exclusion): a forged atrocity still ratchets 墮落. The enemy can
  literally write atrocities into your chronicle. That's the
  fiction working as intended.
- DeedBook gains governance tags: occupation `order:+5`, convoy
  arrival `order:+2`, raid `order:-3`, burn `order:-10` +
  `corruption:+15`, execution `order:-5` + `corruption:+20` —
  initial balance targets, asymmetric with the leg pricing.
- `Campaign/Governance/Accumulators.{h,cpp}` — new module per the
  architecture tree.

## Review findings (three-layer: acceptance / edge / independent)

AC audit: PASS. No critical or high findings. Applied hardening:

- **Governance tag magnitude cap** (`±1000`): an extreme
  `corruption:-9e18` could hold the running sum below zero
  indefinitely, and rail values could pin accumulators at integer
  limits. The fold now rejects magnitudes over the cap — poisoning
  is bounded, the ratchet can still be outrun.
- **Semantic per-entry dedup**: `order:5` / `order:+5` / `order:05`
  are the same posting; variants now dedupe on (axis, value) within
  a single entry, so tag-spelling can't multiply one contribution.
- **`static_assert` pinning `MAX_ENTRIES * MAX_AMOUNT <= INT64_MAX`**
  in `Ledger::Balance` — the no-overflow claim is structural (one
  leg per account per entry), now mechanically checked.
- `<string_view>` self-containment fix; empty-axis rejection;
  `INT64_MIN`-magnitude rejection (asymmetric, documented).
- **Ratchet semantics clarified**: 墮落 folds `corruption:` tags
  (the fold axis), not the literal `atrocity` tag — `atrocity` is
  the audit/audit-report marker, `corruption:` is the contribution.
  Docs updated in DeedBook.h and this file.

Verification: `potato_test_ledger` green (incl. dedup, outrun,
poison-floor, empty-ledger, INT64_MIN rejection); ctest 18/18;
gameplay_dep_guard pass.

## File List

- `Campaign/Governance/Accumulators.h` (new)
- `Campaign/Governance/Accumulators.cpp` (new)
- `Campaign/Ledger/DeedBook.cpp` (governance tags on deed postings)
- `Campaign/Ledger/DeedBook.h` (doc)
- `Campaign/Ledger/Ledger.cpp` (balance-bound static_assert)
- `Examples/potato_test_ledger.cpp` (fold/parser/adversarial tests)

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `FoldGovernance` + tag grammar + POD result
- [x] Task 2 — DeedBook governance tags
- [x] Task 3 — tests (fold rules, ratchet monotonicity, forged-fold,
    malformed tags, no-write-API structural check)
- [x] Task 4 — story/sprint docs; commit
- [x] Task 5 — three-layer review
