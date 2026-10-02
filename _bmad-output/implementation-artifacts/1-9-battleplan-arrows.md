# Story 1.9: BattlePlan Arrows

Status: in-progress

## Story

As a player,
I want drawn plan arrows whose execution bonus scales with fog certainty over covered ground,
So that better intel makes better plans.

## Acceptance Criteria

**Given** a planned arrow over map regions,
**When** execution begins,
**Then** the bonus equals f(mean certainty of covered regions) per balance JSON
**And** replans mid-execution cost CP per the command rules.

## Tasks / Subtasks

- [ ] Task 1 — Plan model (`Gameplay/Plan/BattlePlan.h`) (AC: 1)
  - [ ] `struct PlanArrow { std::vector<std::size_t> path; std::size_t cursor; int bonusApplied; bool active; }` — path[0] = squad's start region, consecutive entries must be adjacent
  - [ ] `struct PlanConfig` — `"plan"` object of `potato.balance/1`: `bonus_percent_cap=100` (bonus = mean certainty, capped), `replan_cost=2`
  - [ ] `int MeanCertainty(const QuantumFog& fog, const std::vector<std::size_t>& path)` — integer mean over covered regions
- [ ] Task 2 — Arrow management in BattleController (AC: 1)
  - [ ] `arrows_` vector index-aligned with `squads_` (one arrow per squad; `active=false` = none)
  - [ ] `DrawArrow(side, squadIndex, path)` — Planning only; validates side, squad ownership, path.size()>=2, path[0]==squad region, pairwise adjacency; replaces existing arrow
  - [ ] On Planning->Execution (`RequestBeat`): for each active arrow, `bonus = min(mean certainty, cap)`; `sq.attack += sq.attack * bonus / 100` recorded in `arrow.bonusApplied` (reversible)
  - [ ] Tick arrow march (after ApplyDeltas): `active` arrow + squad `Holding` + `regionIndex == path[cursor]` && `cursor+1 < path.size()` → `IssueMove(path[cursor+1])`, `++cursor` on success
- [ ] Task 3 — Replan mid-execution (AC: 1)
  - [ ] `InterventionKind::Replan = 5` (append ordinal), `CostOf=2`; `Intervention` gains `std::vector<std::size_t> path`
  - [ ] `IssueReplan(side, squadIndex, path)` — Execution, CheckCommand parity (Routing/dup guards apply — it DOES address a squad), CP deduct at issue, queued
  - [ ] Apply at tick start: path validated against squad's CURRENT region (path[0]==regionIndex, adjacent chain); on success reverse old `bonusApplied`, recompute `bonus = min(mean certainty NOW, cap)`, apply new, swap path/cursor=0 — real-time certainty scaling
  - [ ] `SimEvent::Intervention` logged at issue (aux=5, param=path.size())
- [ ] Task 4 — Test `potato_test_plan` (AC: 1)
  - [ ] DrawArrow validation: non-adjacent path, wrong start region, enemy squad, Execution-phase reject
  - [ ] Bonus: arrow over well-observed ground (certainty 100) grants bigger attack boost than over unknown ground (0); cap respected
  - [ ] Arrow march: squad auto-moves along path during Execution (no doctrine needed); completes path and stops
  - [ ] Replan: costs 2 CP, applies next tick, recomputes bonus from CURRENT certainty (probe the new path first → bigger bonus); rejected replans don't disturb the active arrow
  - [ ] Checksum sensitivity to arrows; stale arrow stalls when squad redirected off-path
- [ ] Task 5 — Verify: MinGW ctest all green incl. dep guard; MSVC disclosed

## Dev Notes

### Architecture anchors

- `D-ARCH-7`/`arch:355`: "BattlePlan bonus = f(certainty over covered regions)" — `f` = integer mean of `CertaintyField` values, capped by balance JSON.
- Directory per arch: `Gameplay/Plan/` (header-only or .h/.cpp — keep it small).
- `BattleState` flat arrays (arch:153): plan arrows are part of battle state → fold into Checksum.

### Prior-story contract points

- `CertaintyField` is now live: `SyncFog` `Observe()`s seen regions to 100 each tick — plan over physically observed ground ⇒ mean 100; over unobserved ⇒ decayed/0. Probes raise the field without collapsing clouds.
- `Intervention` + `pendingCommands_` already exist — `Replan` reuses the queue; `path` vector extends the struct (checksum folds contents).
- `CheckCommand` dup-guard applies to Replan (one pending op per squad) — do NOT add it to the fog-op skip list.
- `Squad::attack` is currently unused by sim combat — the arrow bonus is the first consumer (percent-of-base boost, `bonusApplied` tracked so replan can reverse it).
- A squad redirected/retreated off its arrow path stalls (cursor check fails) — documented behavior, not a bug.

### Testing

- The interesting test: probe a region, THEN replan an arrow over it — the new bonus must reflect the raised certainty (real-time scaling).
- Arrow march emits Move deltas equivalent to IssueMove — squads move at their speed.

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
