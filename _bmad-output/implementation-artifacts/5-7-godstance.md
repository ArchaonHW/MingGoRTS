# Story 5.7 — GodStance

> Epic 5 — Myth Dual-Layer (D) · Stance modulates myth actions ·
> **Status: done**

## Story (from epics.md)

As a player,
I want the deity's disposition toward me to matter,
so that shrine diplomacy has teeth.

## Acceptance Criteria

- **Given** a deity stance (Favorable / Neutral / Wrathful, kept in
  `ShrineTrack::stance[2]` since 5.2), **when** a myth action is
  attempted, **then** its 天命 cost and its success gate reflect the
  stance **and** the stance renders as readable shrine text in the
  narrative layer (dossier/shrine variants).

## Context

Stance memory landed in 5.2 (dedication sets ±1, persists through
latch release — the god remembers). 5.7 wires it to consequence at
two layer-appropriate seams and gives it a voice.

## Design

Three seams, one mood:

- **Cost (Campaign)** — `EffectiveCost(kind, stance)`: Favorable
  −5 (floor 5), Wrathful +10, Neutral = catalog base. The deity
  prices miracles by mood; `PerformMythAction` takes the caller's
  stance (`MythField::StanceAt(region, side)`) — the sim knows the
  mood, the ledger prices it.
- **Success (Gameplay)** — wrathful gates at issue time, before
  the journal write: `IssueMythPossession` refuses a host standing
  on wrathful ground (`sq.regionIndex`, NO_REGION-safe — marching
  squads carry their departure index; undeployed carries NO_REGION),
  `IssueGhostArmy` refuses to raise dead for a banner the god hates.
  Pacify stays open — it's the counter-play that repairs the
  relationship. Rejections never reach `pendingCommands_` or the
  event stream (journal contract: rejected ops leave no trace).
- **Narrative (Campaign)** — `Campaign/Myth/GodStance.h`:
  `ShrineMoodText(stance)` → observed-mood text (神悅/不聞/神怒),
  folk register like MythLog — the shrine doesn't display
  "Wrathful(-1)", it says the crows won't leave. Epic 8 owns
  typography; this is the variant selector.

Bounds: `StanceAt` gates side ∉ {0,1} and non-shrine regions to
Neutral — a wrathful god can't ambush you on ground he doesn't own
(the veil check still applies to non-shrine ghost-raising).

## Implementation Tasks

- [x] `EffectiveCost` + `PerformMythAction(…, GodStance)` — debit leg uses effective cost
- [x] `IssueMythPossession` / `IssueGhostArmy` — wrathful gates pre-journal
- [x] `Campaign/Myth/GodStance.h` — `ShrineMoodText` variants
- [x] Tests: cost table, surcharge affordability, discount leg, wrathful/favored asymmetry, non-shrine neutrality, mood text
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_myth` — wrathful rejection for side 0 while the
  favored side 1 raises dead on the same ground; possession refused
  on wrathful shrine even after latch release; non-shrine ghost
  army still fails on the veil (not the mood)
- `potato_test_ledger` — 15→10/25 cost tiers, base-purse
  insufficient at wrathful surcharge, favorable debit leg = 10
- ctest 19/19 + dep_guard
