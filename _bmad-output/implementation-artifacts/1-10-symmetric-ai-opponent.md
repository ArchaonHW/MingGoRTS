# Story 1.10: Symmetric AI Opponent

Status: review

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

- [x] Task 1 — AI module (`Gameplay/AI/BattleAI.h/.cpp`) (AC: 1)
  - [x] `enum class Prior : std::uint8_t { Aggressive, Defensive, Cunning }` — battle-scope personality (Campaign/Rivals maps GeneralDossier hearsay onto this later)
  - [x] `class BattleAI { BattleAI(seed, side, prior, map, cards); bool Plan(bc, troops, startRegions, cardPool, objective); void Act(bc); static int ScoreCard(card, prior); }` — a CLIENT of `BattleController`'s public API only; no friend access, no stat writes
  - [ ] Own `Prng` stream seeded from `(battleSeed, side)` — outside the sim stream; AI decisions enter the battle only through `DeploySquad`/`SetSheet`/`DrawArrow`/`Issue*`/`SetCloudIntel`, exactly like a player's inputs
  - [ ] Truth boundary: `Act` reads own-side `Squads()` fields + `Fog(side_)` only — never enemy `Squads()` truth (documented discipline; mechanical guard deferred)
- [x] Task 2 — Planning behavior (AC: 1)
  - [ ] Deploys `troops` onto `startRegions` in prior-ordered choice (Aggressive: closest-to-objective first; Defensive: furthest first; Cunning: closest, then feints)
  - [ ] Builds one sheet per squad via `SquadSheet::Build` over `cardPool` ranked by `ScoreCard` (ties → pool order, canonical); needs ≥3 cards else sheets stay empty
  - [ ] Draws arrows via BFS `ShortestPath` toward `objective`: Aggressive full path; Defensive first hop only; Cunning full path + `SetCloudIntel` feint (decoy neighbor off the arrow path, certainty 70)
- [x] Task 3 — Execution policy `Act(bc)` (AC: 1)
  - [ ] Called between `Tick()`s by the host — commands queue and apply next tick start, same as a player
  - [ ] Aggressive: Redirect toward believed-adjacent enemy clouds; Override the attack/move slot
  - [ ] Defensive: Retreat when own cohesion < 45; Override the brace slot when enemy believed in own region
  - [ ] Cunning: Probe queued intel targets (probe budget honored); Entangle the two lowest-certainty live clouds; Replan squads standing off their arrow path
  - [ ] One command per squad per Act (dup-guard means extras are wasted-attempts anyway — order priorities)
  - [ ] All calls go through `Issue*` → identical `CheckCommand` gates, own `cpPool_[side_]` — rejections just fail
- [x] Task 4 — Test `potato_test_ai` (AC: 1)
  - [ ] Symmetry: AI-deployed squads carry template stats exactly (no stat cheating); AI-issued commands appear as ordinary `Intervention` events
  - [ ] Prior bias visible: Aggressive picks attack/move cards and marches to objective; Defensive picks brace and retreats earlier; Cunning plants a feint (enemy cloud believedRegion != truth) and probes
  - [ ] Determinism: identical setup → identical checksums across two runs
  - [ ] AI obeys CP economy: cannot issue when pool insufficient (same gate as player)
- [x] Task 5 — Verify: MinGW ctest all green incl. dep guard; MSVC disclosed

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

### Debug Log References

### Completion Notes List

### File List
