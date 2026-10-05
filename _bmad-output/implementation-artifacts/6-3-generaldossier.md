# Story 6.3 — GeneralDossier

> Epic 6 — Narrative Systems (C) · hearsay dossier render ·
> **Status: done**

## Story (from epics.md)

As a player,
I want enemy generals presented in hearsay register,
so that I never truly know my enemy.

## Acceptance Criteria

- **Given** a rival general, **when** their dossier renders,
  **then** all statements carry hearsay markers (據聞/或曰) with
  no omniscient stats **and** dossier accuracy is itself a
  tracked uncertainty that proven-wrong events can revise.

## Context

`GeneralDossier` (3.7, `RivalBook`) already persists the rival's
hearsay identity: `prior` (sticky first-sighting personality),
`chaptersObserved` (rumor age), `triggers` (what their scouts
learned about *our* habits — "the player is written about").
Story 6.3 renders it: every claim hearsay-marked, every claim
carrying a confidence tier, and a revision seam for 6.4's
IntelLedger verdicts.

## Design

`Campaign/Narrative/Dossier.{h,cpp}`:

- `enum class ClaimKind { Temperament, Habit, Repute }`
- `DossierClaims(d)` — derives the claim set:
  - **Temperament** from `prior` — 性如烈火 / 持重善守 / 詭譎不測
  - **Habit** from the dominant trigger — what their scouts say
    about OUR doctrine ("其斥候謂我軍見敵入境必應")
  - **Repute** from `chaptersObserved` — rumor's age, not stats
- `tier` — the tracked uncertainty: `< MIN_CHAPTERS_TO_LEARN` is
  風聞未確, else 屢聞. The number itself never renders.
- `revised` — proven-wrong claims render with `——然近事駁之。`;
  revision is a parameter seam (`std::span<const ClaimKind>`) that
  IntelLedger verdicts (6.4) will feed.
- Hearsay markers rotate by claim index (據聞/或曰/傳言) —
  deterministic, no PRNG.

## Implementation Tasks

- [x] `Dossier.{h,cpp}` — claims + render
- [x] Tests: marker coverage, no digits, tiers, revision marks,
      dominant-trigger habit, empty dossier
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_ledger` — 11 pins (marker rotation, temperament by
  prior, dominant-trigger habit, tier by chaptersObserved, digit
  ban, thin/unseen bounds, inline revision, determinism)
- ctest 19/19 + dep_guard
