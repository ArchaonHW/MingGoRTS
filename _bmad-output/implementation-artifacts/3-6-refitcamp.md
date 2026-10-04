# Story 3.6: RefitCamp

## Status: done

## Story

As a player,
I want between-chapter actions — deploy/heal/recruit/plunder —
spending 物資,
So that I can rebuild between battles.

## Acceptance Criteria

**Given** a RefitCamp phase with a 物資 balance,
**When** I heal (~20–40) or recruit (~40–80) per economy band,
**Then** costs deduct via ledger posting and the roster updates
**And** overspending is rejected with a skip reason.

## Design

`Campaign/Roster/RefitCamp.{h,cpp}` — the between-chapter action
set. Costs come from the GDD economy table by band:

| Band (chapter index) | heal | recruit | plunder (+物資/−民心) |
|---|---|---|---|
| early (0–2) | 20 | 40 | +30 / −10 |
| mid (3–7) | 30 | 60 | +45 / −15 |
| late (8+) | 40 | 80 | +60 / −20 |

`RefitCosts BandCosts(int64_t chapterIndex)` exposes the table.

Actions (all `CampaignState&`-mutating free functions, atomic —
check first, then post + mutate):

- `Deploy(state, name)` — free; validates the squad exists and is
  alive (deployment selection is phase bookkeeping; the caller
  tracks who goes).
- `Heal(state, name, chapterIndex)` — clears the squad's current
  `casualties` to 0 (replacements absorb losses) for the band
  heal cost. Dead squads can't be healed — `dead` is permanent
  (3.5's memorial semantics); the ledger, not the roster counter,
  is the permanent scar.
- `Recruit(state, name, chapterIndex)` — `Enlist` a new squad for
  the band recruit cost.
- `Plunder(state, chapterIndex)` — no roster effect; posts the
  asymmetric plunder pair (credit Materiel / debit
  PopularSupport).

Spending posts debit Materiel / credit ArmyPrestige (investment
converts materiel into army standing — the same asymmetric
posting model as Burn). Overspend = `Balance(Materiel) < cost`
rejects BEFORE any posting, with the skip reason in
`Result::reason` ("insufficient materiel") — the AC's skip
reason.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `RefitCamp.{h,cpp}`: BandCosts + four
    actions
- [x] Task 2 — tests: band table pins, heal clears casualties,
    dead squad can't heal, recruit enlists + posts, plunder
    posts asymmetric pair, deploy validates alive, overspend
    rejects with no ledger/roster mutation
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- Three-layer review (AC:PASS) fixes:
  - **Recruit atomicity was duplicated-check fragile** (post →
    Enlist mirrored every invariant by hand). Now `CanEnlist`
    (exported from Roster.h, shared by Enlist) + a ledger-full
    gate run BEFORE the post — after the gates both Post and
    Enlist are infallible. Structural, not mirrored.
  - **Plunder posted tagless** — invisible to the Epic 4 墮落
    fold. Now tags `{"plunder"}`.
  - **Negative chapterIndex clamped to the cheapest band**
    (fail-open pricing) — now clamps to late (fail-closed).
  - Skip reasons now carry numbers: "insufficient materiel
    (need X, have Y)".
  - `casualties < 0` (mutable-accessor corruption) is a
    `"roster"` error, not a "full strength" skip — same
    untrusted-operand posture as ApplyAftermath.
  - Heal re-fetches the entry by index after PostSpend — no
    roster reference held across a ledger mutation.
- Documented in header: `Result<int>` value semantics differ
  per call (roster index vs ledger seq); `Balance(Materiel)` is
  spendable-truth (forged entries fold identically — 2.3
  contract); plunder is intentionally ungated — the −民心 leg IS
  the cost.
- Deferred (deferred-work.md): plunder has no per-chapter rate
  limit — a per-visit plunder allowance belongs to the chapter
  shell's phase state (ChapterState, 3.4 deferred).

## File List

- `Campaign/Roster/RefitCamp.h` (new)
- `Campaign/Roster/RefitCamp.cpp` (new)
- `Campaign/Roster/Roster.h` (`CanEnlist` export)
- `Campaign/Roster/Roster.cpp` (Enlist refactored onto
  CanEnlist)
- `Examples/potato_test_campaign.cpp` (+3.6 test block)
- `_bmad-output/implementation-artifacts/3-6-refitcamp.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/deferred-work.md`
