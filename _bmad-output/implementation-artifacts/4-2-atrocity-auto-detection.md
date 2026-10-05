# Story 4.2: Atrocity Auto-Detection

## Status: done

## Story

**As a** system,
**I want** atrocities (village burning, refusing rout-surrender)
auto-flagged into reports,
**So that** cruelty can't hide in the numbers.

## Acceptance Criteria

1. Given a burned village or refused surrender event,
   When aftermath resolves,
   Then the event posts with an atrocity tag
   And the report must either acknowledge or visibly omit it.

## Dev Notes

- **Refusing surrender needs a verb.** No combat resolution exists —
  nothing damages squads — so "refused rout-surrender" is modeled as
  the explicit refusal: `IssueExecute(side, victimSquadIndex)`.
  Routing is the surrender plea; the default is acceptance (the
  squad ages off to Routed). Execute revokes that mercy.
- `IssueExecute` rules (InterventionKind::Execute, ordinal 6):
  - Execution beat only, issuer side 0/1.
  - Target must be a **Routing enemy** squad — you can only refuse
    a plea that exists.
  - **0 CP** — cruelty is free; the ledger is the price (Probe is
    the existing 0-cost precedent).
  - **Proximity**: the refusing side needs a **Holding** squad in
    the victim's region, checked at issue AND apply — a marching
    killer has already left (mid-leg squads' regionIndex is the
    departure node), a routing one is fleeing not refusing.
    Execution is allowed in contested regions — presence, not
    exclusivity, is the rule (a routing squad's comrades don't
    shield it; contrast the village dwell's ExclusiveSide).
  - Queued like all interventions; applies at next tick start.
    Apply-time re-gate: nothing can alter the victim between issue
    and apply (rout aging runs later in the same tick) — the
    reachable apply-failure is the refuser's own earlier-queued
    Retreat flipping presence off. Queue order decides:
    Retreat-then-Execute no-ops, Execute-first kills — same
    authored-order rule as doctrine slots.
  - On apply: `hp = 0` (the kill shot — a Destroyed corpse must not
    keep a routing victim's hp) then `ApplyEvent(HpZero)` —
    Routing -> Destroyed — plus a `SquadExecuted` SimEvent
    (kind 8): param = victim's region, squadIndex = victim,
    side = refuser. Emitted at intervention-apply step, i.e.
    before the tick's doctrine/burn events — command resolution
    precedes doctrine in the tick pipeline.
- Wire: `SimEvent::Kind` bound 7→8 (SquadExecuted=8); `aux` bound
  0..5 → 0..6 (Execute rides aux=6; max legitimate aux across all
  kinds is now the InterventionKind domain). Same TOOL_VERSION=3 —
  ordinals are append-only within the /1 schema generation.
- Deferred from the AC's "when aftermath resolves": `BookDeeds`
  itself is the detection/tagging seam and is fully tested at the
  unit level, but the aftermath call-site (who invokes BookDeeds at
  battle close) belongs to the battle→campaign resolution pass —
  tracked in deferred-work.md (owner: Story 4.5 defeat conversion /
  Game shell).
- **DeedBook** (`Campaign/Ledger/DeedBook`): the battle→ledger
  translation seam. Walks the recorded event stream and posts the
  player's deeds (event.side == playerSide; enemy deeds are the
  enemy's ledger problem — out of scope). Initial legs
  (balance targets, asymmetric per ledger convention):
  - `VillageOccupied`: +民心 10 / −物資 5 (garrison upkeep)
  - `VillageBurned`: +物資 40 / −民心 15 + tags {atrocity,
    region:<idx>} — the architecture's example pricing
  - `ConvoyArrived`: +物資 30 / −民心 5 (escort levy)
  - `ConvoyRaided` (player raids): +物資 25 / −民心 5 + tags {raid,
    region:<idx>} — commerce raiding is war, not atrocity
  - `SquadExecuted`: +軍威 5 / −民心 10 + tags {atrocity,
    squad:<idx>} — terror reads as deterrence to the army and
    revulsion to the populace
  - Posts in event order via `Ledger::Post`; the first failure
    aborts and reports (partial posting = honest history).
- **Report acknowledgment**: `HistorianReport` gains
  `audit.atrocities` — entries carrying the `atrocity` tag counted
  unconditionally (audit is the truth layer, independent of
  OmissionPolicy) and rendered as `atrocities: N`. AC's
  "acknowledge or visibly omit": an atrocity entry narrated under
  an including policy is acknowledged by the audit count; an
  omitted one shows in the omission confession AND still counts —
  cruelty can't hide in the numbers either way.
- Not in scope: the 墮落 ratchet fold (4.3), casualty/verdict
  postings from `BattleResult::ledger` (defeat conversion 4.5),
  a "no quarter" doctrine stance (Execute is the intervention
  verb; an authored stance is Epic 7 territory), pursuit damage
  mechanics (combat layer).

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `IssueExecute` + `SquadExecuted` event +
    InterventionKind::Execute + apply gates + wire bounds
- [x] Task 2 — `DeedBook` event→posting translation w/ atrocity
    tags
- [x] Task 3 — `HistorianReport` atrocity count + render line
- [x] Task 4 — tests (governance: execute paths; campaign: deed
    postings + report)
- [x] Task 5 — story/sprint docs; commit
- [x] Task 6 — three-layer review

## Review Findings (three-layer, AC:PASS)

| Finding | Fix |
|---|---|
| `ApplyEvent(HpZero)` leaves hp untouched — only path to Destroyed with hp>0 | `vic.hp = 0` before the transition; pinned |
| `RefuserPresent` counted Moving squads at their departure region | tightened to `state == Holding` — a marching killer is gone |
| `aux` bound 0..5→0..127 over-permissive; justification misdescribed the wire layout | bound is 0..6 (Execute is the max legitimate value); Replan's path length rides `param`, not `aux` — comment + test corrected |
| `BookDeeds` partial-post comment claimed the caller sees progress; Result can't carry it | failure reason now names the posted count; no-retry contract documented (append-only, resume don't re-fold) |
| `squad:<idx>` tag conflated victim vs perpetrator across kinds | renamed `victim:<idx>` |
| `"atrocity"` magic string in 3 places | `Ledger::TAG_ATROCITY` shared constant |
| No replay coverage for Execute | `RunAndRecordExecute` end-to-end record→verify |
| Doc drift: GovernanceField.h aux bound, BattleRecorder.h v3 note, Intervention.h target comment | all corrected |

## File List

- `Gameplay/Command/Intervention.h` — Execute kind + 0 CP cost
- `Gameplay/Doctrine/Doctrine.h` — `SimEvent::Kind::SquadExecuted`
- `Gameplay/Sim/BattleController.{h,cpp}` — `IssueExecute`, `RefuserPresent`, apply branch
- `Gameplay/Record/BattleRecorder.{h,cpp}` — kind bound 8, aux bound 6
- `Gameplay/Record/ReplayVerifier.cpp` — ReIssue Execute arm
- `Campaign/Ledger/Ledger.h` — `TAG_ATROCITY` shared tag
- `Campaign/Ledger/DeedBook.{h,cpp}` — NEW: deed→posting translation
- `Campaign/Ledger/HistorianReport.{h,cpp}` — `audit.atrocities` + render line
- `Examples/potato_test_governance.cpp` — execute paths + wire bounds
- `Examples/potato_test_ledger.cpp` — deed posting + atrocity audit
- `Examples/potato_test_record.cpp` — execute record e2e verify
- `_bmad-output/implementation-artifacts/deferred-work.md` — BookDeeds wiring owner, no-quarter stance, ApplyHit status
