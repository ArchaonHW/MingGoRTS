# Story 4.5: Defeat Conversion

## Status: done

## Story

**As a** player,
**I want** a lost battle to route into governance/recovery play
rather than game over,
**So that** defeat is a fork, not an ending.

## Acceptance Criteria

1. Given a battle resolved as defeat,
   When aftermath posts to campaign,
   Then casualties persist, ledger books the loss (墮落 may
   ratchet), and the campaign offers a recovery/governance
   continuation
   And no game-over screen appears for a single lost battle.

## Dev Notes

- This story is the **aftermath orchestrator** — the production
  wiring deferred from 4.2 (`BookDeeds`) and the defeat branch of
  4.4 (`ChapterResolution::Defeat`) converge here.
- **`ResolveAftermath(state, lib, battleWon, playerSide, deeds,
  casualties)`** — one call = one chapter's settlement:
  1. `ApplyAftermath` — casualties persist to the roster
     (validate-then-mutate; a bad report rejects everything).
  2. `BookDeeds` — player-side deed events post (atrocities carry
     `corruption:` tags, so a brutal defeat can ratchet 墮落).
  3. `ConcludeChapter` — the seal is the ledger's booking of the
     loss: `resolution:defeat` + `chapter:N`, salvage legs
     (軍威→物資). Progression advances — the chapter is spent.
- **Ordering is semantics**: deeds post BEFORE the governance fold
  inside `ConcludeChapter`, so the chapter's own deeds feed the
  verdict — and the verdict is decided before the seal posts, so
  the seal can't contaminate it.
- **Atomicity**: `CanResolveChapter` + ledger-capacity preflight up
  front, then three mutations each provably unfailable — apply →
  book → seal+advance. No partial-settlement state exists.
- **"Recovery/governance continuation" is structural**: a Defeat
  resolution still advances the campaign (converted defeat is a
  chapter outcome per GDD; next chapter + RefitCamp is the recovery
  loop). Nothing in the campaign layer can terminate a run —
  there is no game-over path to render.
- **Caller contract**: the Game shell maps `BattleResult` +
  deployment table → `AftermathRow`s (names) and supplies the
  recorded deed events. This story wires campaign-side; shell
  extraction is Epic 8's Game layer.
- New module: `Campaign/Aftermath/Aftermath.{h,cpp}`.
- Deferred: mid-chapter multi-battle chapters (a defeat currently
  spends the chapter — single-battle chapter model).

## Review findings (three-layer: acceptance / edge / independent)

AC audit: PASS. One high, two mediums — all resolved:

- **HIGH (all three layers converged)**: `playerSide` was validated
  inside `BookDeeds`, AFTER `ApplyAftermath` had already buried
  squads — `side=7` returned Fail with the roster mutated and no
  seal: a partial settlement that couldn't even be retried (wiped
  rows now fail "dead squad"). Hoisted the domain check into the
  preflight before any mutation.
- **MED**: no idempotency — a retry after an ambiguous failure
  would settle the NEXT chapter with the old battle's data.
  `recordRoot` is now a required nonzero parameter: it anchors the
  seal via the `record_root:` tag family (CrossCheck, Story 2.5 —
  the verdict cites its evidence) and a preflight scan rejects an
  already-anchored root with "battle already settled".
- **MED**: every settle spends the chapter (single-battle model
  baked into the signature) — documented in the header + deferred
  to whichever story adds multi-battle chapters.
- **LOW**: `deeds.size()` overflow guard (`>= MAX_ENTRIES` early
  reject); `DeedBook.h` now pins the invariant that generated
  postings must satisfy ValidateLegs+ValidateMeta (the "unfailable"
  claim rests on it); `ChapterSettlement.nextChapter` stays `int`
  (bounded by MAX_CHAPTERS=64, precedent is Result<int>).

Verification: `potato_test_campaign` green (new pins: invalid-side
pre-mutation rejection, uninitialized progress, zero root, same-root
re-settle rejection, final-chapter sentinel); ctest 18/18; dep
guard pass.

## File List

- `Campaign/Aftermath/Aftermath.h` (new)
- `Campaign/Aftermath/Aftermath.cpp` (new)
- `Campaign/Governance/Victory.h/.cpp` (recordRoot seal anchor)
- `Campaign/Ledger/DeedBook.h` (posting-validity invariant)
- `Examples/potato_test_campaign.cpp`

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `ResolveAftermath` orchestrator +
    `ChapterSettlement` result
- [x] Task 2 — tests (defeat persists casualties + seal + advances;
    atrocity deeds ratchet 墮落 through a loss; win path shares
    same flow; atomicity refusals)
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review
