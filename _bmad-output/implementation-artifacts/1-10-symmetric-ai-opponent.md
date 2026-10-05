# Story 1.10: Symmetric AI Opponent

Status: done

## Story

As a player,
I want the enemy to run the same doctrine interpreter and CP rules under personality priors,
So that victories are earned, not stat-inflated.

## Acceptance Criteria

**Given** an AI side with a squad sheet and a prior (aggressive/defensive/cunning),
**When** the battle ticks,
**Then** AI cards evaluate through the identical interpreter entry point and CP pool
**And** priors bias card selection and fog-cloud shapes, never stats.

## Tasks / Subtasks

- [ ] Task 1 — AI module (`Gameplay/AI/BattleAI.h/.cpp`) (AC: 1)
  - [ ] `enum class Prior : std::uint8_t { Aggressive, Defensive, Cunning }` — battle-scope personality (Campaign/Rivals maps GeneralDossier hearsay onto this later)
  - [ ] `class BattleAI { BattleAI(seed, side, prior, map, cards); bool Plan(bc, troops, startRegions, cardPool, objective); void Act(bc); static int ScoreCard(card, prior); }` — a CLIENT of `BattleController`'s public API only; no friend access, no stat writes
  - [ ] Own `Prng` stream seeded from `(battleSeed, side)` — outside the sim stream; AI decisions enter the battle only through `DeploySquad`/`SetSheet`/`DrawArrow`/`Issue*`/`SetCloudIntel`, exactly like a player's inputs
  - [ ] Truth boundary: `Act` reads own-side `Squads()` fields + `Fog(side_)` only — never enemy `Squads()` truth (documented discipline; mechanical guard deferred)
- [ ] Task 2 — Planning behavior (AC: 1)
  - [ ] Deploys `troops` onto `startRegions` in prior-ordered choice (Aggressive: closest-to-objective first; Defensive: furthest first; Cunning: closest, then feints)
  - [ ] Builds one sheet per squad via `SquadSheet::Build` over `cardPool` ranked by `ScoreCard` (ties → pool order, canonical); needs ≥3 cards else sheets stay empty
  - [ ] Draws arrows via BFS `ShortestPath` toward `objective`: Aggressive full path; Defensive first hop only; Cunning full path + `SetCloudIntel` feint (decoy neighbor off the arrow path, certainty 70)
- [ ] Task 3 — Execution policy `Act(bc)` (AC: 1)
  - [ ] Called between `Tick()`s by the host — commands queue and apply next tick start, same as a player
  - [ ] Aggressive: Redirect toward believed-adjacent enemy clouds; Override the attack/move slot
  - [ ] Defensive: Retreat when own cohesion < 45; Override the brace slot when enemy believed in own region
  - [ ] Cunning: Probe queued intel targets (probe budget honored); Entangle the two lowest-certainty live clouds; Replan squads standing off their arrow path
  - [ ] One command per squad per Act (dup-guard means extras are wasted-attempts anyway — order priorities)
  - [ ] All calls go through `Issue*` → identical `CheckCommand` gates, own `cpPool_[side_]` — rejections just fail
- [ ] Task 4 — Test `potato_test_ai` (AC: 1)
  - [ ] Symmetry: AI-deployed squads carry template stats exactly (no stat cheating); AI-issued commands appear as ordinary `Intervention` events
  - [ ] Prior bias visible: Aggressive picks attack/move cards and marches to objective; Defensive picks brace and retreats earlier; Cunning plants a feint (enemy cloud believedRegion != truth) and probes
  - [ ] Determinism: identical setup → identical checksums across two runs
  - [ ] AI obeys CP economy: cannot issue when pool insufficient (same gate as player)
- [ ] Task 5 — Verify: MinGW ctest all green incl. dep guard; MSVC disclosed

## Dev Notes

### Architecture anchors

- D-ARCH-3: "same interpreter serves player & AI — symmetric AI for free"; canonical order fixed (squad index → slot index).
- `PriorTable` lives in `Campaign/Rivals` per module map; the battle-scope `Prior` enum is the seam it will feed.
- Event sourcing (D-ARCH-6): AI inputs are `SimEvent::Intervention` records like the player's — replay needs nothing extra.
- Arch:338 — "cards see state at tick start"; AI `Act` between ticks issues commands that apply at next tick start — same latency as a player.

### Prior-story contract points

- `BattleController` public API is the complete command surface: `DeploySquad`/`SetSheet`/`DrawArrow` (Planning), `Issue*` (Execution), `SetCloudIntel` (intel hook). The AI needs NO controller changes — that's the symmetry proof.
- `CheckCommand` dup-guard: one pending command per squad — `Act` must prioritize (retreat > redirect > override > fog ops > replan).
- `IssueReplan` anchor semantics (1.9): replan path must contain `Moving ? edgeTarget : regionIndex`.
- `SetCloudIntel(fogSide, squadIndex, region, certainty)` — the feint primitive already exists (1.8 review added it).
- `Fog(side).VisibleAt(region)` = believed-enemy presence ≥ detectThreshold — the AI's enemy sensor; own squads via `Squads()[i].side == side_`.
- `ArrowOf(i)` + cursor reveal whether a squad stands on its path — stall detection for replan policy.

### Testing

- Pin `ScoreCard` per prior — the bias table IS the personality spec.
- Feint test: Cunning deploys, then assert `bc.Fog(enemySide)` cloud for the squad sits on the decoy region while `Squads()[i].regionIndex` is the true one — proves cloud-shape bias, not stat bias.
- Determinism: two identical runs (seed, prior, inputs) → equal `Checksum()` after N ticks.

## Dev Agent Record

### Agent Model Used

SWE-2 (Devin CLI)

### Debug Log References

- `VisibleAt` returns a believed-cloud COUNT, not certainty — first draft compared it against `detectThreshold`; corrected to `> 0`.
- Cunning probes/entangles are side-level ops, not per-squad — moved out of the per-squad loop into `Act` (else the probe budget drains per squad per tick).
- Mid-Execution decision needs no `regionIndex` mutation — `Act` reads `bc.Fog(side_)` belief + own-side squad fields only.

### Completion Notes List

- `Gameplay/AI/BattleAI.{h,cpp}`: `Prior{Aggressive,Defensive,Cunning}` + `BattleAI` — a pure CLIENT of `BattleController`'s public API (zero controller changes; that absence IS the symmetry proof).
- `ScoreCard` pins the personality table (aggressive: AttackBoost/Move/EnemyAdjacent; defensive: Brace/CohesionBelow/Hold; cunning: EnemyInRegion ambush/CpAtLeast). Sheets ranked by score, ties canonical by pool order.
- `Plan`: prior-ordered startRegion pick (closest vs furthest to objective via BFS `HopDistance`), prior-shaped arrows (`ShortestPath` BFS; defensive takes first hop only), Cunning plants a `SetCloudIntel` feint (decoy neighbor off the arrow, certainty 70) into the ENEMY's fog — cloud-shape bias, no stat bias.
- `Act`: Aggressive Redirects toward believed contact / Overrides the punch slot; Defensive retreats below cohesion 45 / Overrides brace on contact; Cunning probes low-certainty regions, entangles the two weakest live clouds, replans off-path squads via anchor-validated shortest paths. One command per squad per call; all through `CheckCommand`/own CP pool.
- AI PRNG is a private stream seeded `(battleSeed, side)` — outside the sim stream; decisions enter the record as ordinary `SimEvent::Intervention`s.
- Verification: `potato_test_ai` 41/41 checks; `ctest` 11/11 green incl. `gameplay_dep_guard` (MinGW; MSVC unverified — no `cl` on PATH).

### File List

- `Gameplay/AI/BattleAI.h` (new)
- `Gameplay/AI/BattleAI.cpp` (new)
- `Examples/potato_test_ai.cpp` (new, 29 checks)
- `CMakeLists.txt` (potato_test_ai target + add_test)

## Review Findings

Three-layer review — 1 High + 2 Med convergent defects, several lows; all patched, tests grown 29 → 41 checks.

### Patched

1. **High ×2層 — Entangle picker indexed `Clouds()` by cloud id instead of position.** `AddCloud` ids stay dense only until the first `RemoveCloud` (order-preserving erase leaves ids sparse) — `SyncFog` prunes on every rout/death, so `clouds[id]` post-prune compared wrong certainties AND could read OOB (UB). **Fix:** select by position, submit `.id`; regression test removes two early-deployed clouds then asserts entangle picks ids {3,4} (the true two weakest), which fails on the buggy code.
2. **Med ×2層 — Unreachable start regions sorted FIRST for Aggressive/Cunning.** `HopDistance` returns -1 for unreachable; `-1 < any finite` preferred dead-end deployments (disconnected maps are legal content — no connectivity check in `BattleMap::FromJson`). **Fix:** -1 normalized to a large sentinel so unreachable sorts last under "closest".
3. **Med ×2層 — One unresolvable card id poisoned ALL sheets.** Unknown ids sank to the bottom of `ranked` but still landed in `sheetIds` whenever the pool didn't truncate, failing `Build` → every squad sheetless. **Fix:** filter unresolvable ids before truncation; Build only on >= MIN_SLOTS.
4. Low — `probed_`/`entangleTick_` never reset on `Plan` reuse → reset both; AI reuse across battles now sane.
5. Low — double-`Act` in one tick gap re-issued the same entangle pair (pending apply keeps `entangledWith==-1`) → double CP spend. **Fix:** `entangleTick_` watermark vs `GetSim().TickCount()`.
6. Low — Cunning replan could buy a degenerate 1-node arrow (squad already at objective) → skip `path.size()<2`.
7. Low — hardcoded CP literals → `CostOf(Redirect/Override/Retreat)`; Aggressive `Neighbors(at)` missing the `NO_REGION` guard the Cunning branch had.
8. Low — `rng_` documented as reserved (v0 policies fully deterministic; the member fixes the contract that AI draws never come from the sim stream).
9. Test-fidelity gaps (all three layers): defensive retreat event + CP deduction, brace `IssueOverride` → `CardFired card_brace` (pins ranked-sheet composition end-to-end), cunning probes/entangle/replan events, `Act` no-op outside Execution. Also fixed a self-inflicted test bug: the cunning scenario marched the AI to r2 within 45 ticks, auto-observing the planted clouds — objective set to the start region so the squad holds.

### Deferred

- **Cloud-shaping is cunning-only**: aggressive/defensive have no fog signature. The AC's "priors bias fog-cloud shapes" is demonstrated on the prior that cares; if every prior should shape clouds that's a design follow-up (needs design intent, not more code).
- AI reads `.side` on enemy rows to find its own squads (short-circuit — no other enemy field touched). A mechanical own-index-range iteration is possible once roster indices are tracked; noted alongside the existing `Squads()` truth-exposure deferral.
- `Plan` header's "no mutation" claim holds today because all failure modes are pre-validated; a future `DeploySquad` gate mid-loop would partially deploy — flagged latent.
- Probe order is index-order, not weakest-first (comment corrected concept); strategic prioritization is v0-acceptable.
- AI ctor doesn't validate `side`/`prior` — `Plan` gates bad sides; `Act` is benign (all `Issue*` side-checks reject).

### Review-fix verification

`potato_test_ai` 41/41; full `ctest` 11/11 green (MinGW).
