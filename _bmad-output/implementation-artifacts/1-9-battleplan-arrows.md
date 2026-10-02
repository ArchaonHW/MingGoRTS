# Story 1.9: BattlePlan Arrows

Status: done

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

- [x] Task 1 — Plan model (`Gameplay/Plan/BattlePlan.h`) (AC: 1)
  - [x] `struct PlanArrow { std::vector<std::size_t> path; std::size_t cursor; int bonusApplied; bool active; }` — path[0] = squad's start region, consecutive entries must be adjacent
  - [x] `struct PlanConfig` — `"plan"` object of `potato.balance/1`: `bonus_percent_cap=100` (bonus = mean certainty, capped), `replan_cost=2`
  - [x] `int MeanCertainty(const QuantumFog& fog, const std::vector<std::size_t>& path)` — integer mean over covered regions
- [x] Task 2 — Arrow management in BattleController (AC: 1)
  - [x] `arrows_` vector index-aligned with `squads_` (one arrow per squad; `active=false` = none)
  - [x] `DrawArrow(side, squadIndex, path)` — Planning only; validates side, squad ownership, path.size()>=2, path[0]==squad region, pairwise adjacency; replaces existing arrow
  - [x] On Planning->Execution (`RequestBeat`): `SyncFog()` first (deployment footprint = initial intel), then for each active arrow `bonus = min(mean certainty, cap)`; `sq.attack += sq.attack * bonus / 100` recorded in `arrow.bonusApplied` (reversible)
  - [x] Tick arrow march (after ApplyDeltas): `active` arrow + squad `Holding` + `regionIndex == path[cursor]` && `cursor+1 < path.size()` → `IssueMove(path[cursor+1])`, `++cursor` on success
- [x] Task 3 — Replan mid-execution (AC: 1)
  - [x] `InterventionKind::Replan = 5` (append ordinal), `CostOf=2`; `Intervention` gains `std::vector<std::size_t> path`
  - [x] `IssueReplan(side, squadIndex, path)` — Execution, CheckCommand parity (Routing/dup guards apply — it DOES address a squad), CP deduct at issue, queued; issue-time ValidPath gate so broken paths can't burn CP
  - [x] Apply at tick start: path validated against squad's CURRENT region (path[0]==regionIndex, adjacent chain); on success reverse old `bonusApplied`, recompute `bonus = min(mean certainty NOW, cap)`, apply new, swap path/cursor=0 — real-time certainty scaling
  - [x] `SimEvent::Intervention` logged at issue (aux=5, param=path.size())
- [x] Task 4 — Test `potato_test_plan` (AC: 1)
  - [x] DrawArrow validation: non-adjacent path, wrong start region, enemy squad, degenerate path, Execution-phase reject
  - [x] Bonus: fully-observed path (mean 100) grants +100% attack vs 66%-mean path granting +6 on attack 10; `PlanConfig` cap bounds-checked from `potato.balance/1`
  - [x] Arrow march: squad auto-moves along path during Execution (no doctrine needed); completes path and stops at cursor end
  - [x] Replan: costs 2 CP, applies next tick, recomputes bonus from CURRENT certainty (grant 10 reversed, new grant 6 on mean-66 path); rejected replans keep CP and don't disturb the active arrow; dup-pending guard rejects a second queued replan
  - [x] Checksum sensitivity: same seed+plan → same checksum; different arrow → different checksum
- [x] Task 5 — Verify: MinGW ctest 10/10 green incl. dep guard; MSVC still unverified

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

SWE-2 (Devin CLI)

### Debug Log References

- Design catch during test authoring: CertaintyField only fills during Execution ticks, so a Planning-drawn arrow would always score mean 0. Fixed by running one `SyncFog()` observation pass at the Planning→Execution transition before `ApplyPlanBonuses()` — the deployment footprint IS the initial intel.
- Replan on a Moving squad: `regionIndex` still holds the leg's origin until arrival, so issue-time `ValidPath` accepts a path starting there; apply re-validates against the (possibly advanced) position — a squad that completed a leg between issue and apply gets its replan silently skipped (CP already spent, mirrors Redirect semantics).
- `potato_test_plan` lambdas needed `[&]` capture (MinGW noted `'tmpl' is not captured`).

### Completion Notes List

- `Gameplay/Plan/BattlePlan.{h,cpp}`: `PlanArrow` (path/cursor/bonusApplied/active), `PlanConfig::FromJson` reading the `"plan"` object of `potato.balance/1` (schema-gated when present, `bonus_percent_cap` 0..100, `replan_cost` 0..100), `MeanCertainty` integer mean helper.
- `BattleController`: `arrows_` index-aligned with squads; `DrawArrow` (Planning-only, side/ownership/bounds/adjacency/start checks); Execution-start `SyncFog()`+`ApplyPlanBonuses()`; `MarchArrows()` step after `ApplyDeltas` auto-advances on-path Holding squads one leg per tick.
- `InterventionKind::Replan=5` appended; `Intervention` gains `path` payload; `IssueReplan` runs the full `CheckCommand` gate (CP deduct at issue, dup-guard, Routing reject) plus issue-time path validation; apply-time re-validation, bonus reversal + recompute from CURRENT certainty.
- Checksum folds `pendingCommands_.path` contents and every `PlanArrow` field (active/cursor/bonusApplied/path nodes).
- `SimEvent` wire-format comment documents aux=5/Replan; `param` carries path length (full path lives in the queued command, which the checksum folds — recorder payload completeness deferred to 1.11).
- Verification: `potato_test_plan` 47/47 checks; `ctest` 10/10 green incl. `gameplay_dep_guard` (MinGW; MSVC unverified — no `cl` on PATH).

### File List

- `Gameplay/Plan/BattlePlan.h` (new)
- `Gameplay/Plan/BattlePlan.cpp` (new)
- `Gameplay/Command/Intervention.h` (Replan kind, path payload, cost)
- `Gameplay/Sim/BattleController.h` (Plan API, arrows_, planConfig_)
- `Gameplay/Sim/BattleController.cpp` (DrawArrow/IssueReplan/ApplyPlanBonuses/MarchArrows/ValidPath, checksum, ctor wiring)
- `Gameplay/Doctrine/Doctrine.h` (SimEvent wire-format comment)
- `Examples/potato_test_plan.cpp` (new, 31 checks)
- `CMakeLists.txt` (potato_test_plan target + add_test)

## Review Findings

Three-layer review (Blind Hunter / Edge Case Hunter / Acceptance Auditor) — 3 Med convergent defects + hygiene; all patched, tests grown 31 → 47 checks.

### Patched

1. **Med ×3層 — Mid-leg replan produced a permanently dead arrow.** `ValidPath` required `path[0]==regionIndex`; a Moving squad's regionIndex is still the departure region, so on arrival `edgeTarget != path[0]` and cursor=0 stalled forever (CP already spent). **Fix:** anchor semantics — `AnchorRegion = Moving ? edgeTarget : regionIndex`; replan paths must CONTAIN the anchor (not start at it); `SeekPath` sets `cursor` to the anchor index so the arrow resumes marching on arrival. Same seek also fixes land-ahead stalls (squad redirected onto `path[k]` resyncs instead of stalling). Verified: `mid-leg arrival` → `resumes marching`; `cursor resyncs` via Redirect.
2. **Med ×2層 — `sq.attack * bonus` signed-overflow UB.** attack reaches INT_MAX via stacked AttackBoost deltas; `*66` overflows int32 in the tick path (a determinism-break candidate Checksum exists to catch). **Fix:** int64 intermediate + INT_MAX saturation, mirroring `ApplyDeltas`, in both `ApplyPlanBonuses` and the Replan apply.
3. **Med ×2層 — `attack -= bonusApplied` could go negative.** Doctrine deltas clamp attack to [0,INT_MAX]; the reversal didn't — attack −10 → grant −6 → `bonusApplied` −6 compounding. **Fix:** clamped reversal `max(0, attack − bonusApplied)`, grant recomputed on the clamped base, actual applied delta recorded.
4. Low — Checksum arrow loop had no path-length prefix (segmentation ambiguity, e.g. `{x}`+`{y,z}` vs `{x,y}`+`{z}`). **Fix:** fold `a.path.size()` per arrow.
5. Low ×2 — `ValidPath` accepted revisits (`{0,1,0,1}` inflates `MeanCertainty` toward observed-region saturation and makes cursor-seek ambiguous). **Fix:** uniqueness check; length implicitly bounded by `RegionCount` (closes the unbounded-path hygiene item too).
6. Low ×2 — `replan_cost` range was 0..100 vs `CP_CAP=5` (>5 = permanently unaffordable, no warning). **Fix:** validated range now `[0, CP_CAP]`.
7. Low — `ApplyPlanBonuses` granted attack to spawn-Routing squads (template cohesion <20). **Fix:** skip Routing at grant time.
8. Low — `Intervention.h` cost list omitted Replan; `target` doc claimed unused for Replan (it carries path length). **Fix:** comments corrected.
9. Test-fidelity gaps (Acceptance F1/F3/F4 + Edge gaps) — added: `bonus_percent_cap=50` cap pin; probe-then-replan (mean 75 ≠ 66 proves apply-time certainty read); Planning-phase checksum isolation (arrow fold pinned without squad-state masking); cycle rejection on both DrawArrow and IssueReplan; DrawArrow replace; anchor-reject replans; post-arrival march continuation; redirect land-ahead resync.

### Deferred

- **Replan `SimEvent` records only path length** — the full path payload dies when `pendingCommands_` clears at apply. Every other command's event is self-describing; a future events-driven replay can't reissue a Replan. Recorder (1.11) needs a path side-channel or command-input recording regardless — track there.
- Replan on an **arrowless** squad creates+grants an arrow mid-Execution (kept as feature: "replan = new plan", costs CP); noted for AI opponent (1.10) contract review.
- No `potato.balance/1` file on disk yet — `PlanConfig::FromJson`/`FogConfig::FromJson` tested via inline JSON (same precedent as 1.8).

### Review-fix verification

`potato_test_plan` 47/47; full `ctest` 10/10 green (MinGW).
