# Story 6.4 — IntelLedger

> Epic 6 — Narrative Systems (C) · claim→verdict intel memory ·
> **Status: done**

## Story (from epics.md)

As a system,
I want distorted/misleading intelligence recorded as a narrative
device,
so that my intel has a history of being wrong.

## Acceptance Criteria

- **Given** intel inputs to a chapter, **when** the IntelLedger
  stores them, **then** entries record both claim and later
  verdict (true/false/unresolved) **and** distorted intel feeds
  RivalDeck and QuantumFog priors.

## Design

`Campaign/Narrative/IntelLedger.{h,cpp}` — append-only claim
registry (`potato.intel/1`):

- `Record(subject, kind, claim, chapter)` → seq. Kinds:
  RivalTemperament / RivalHabit / EnemyDisposition / TerrainIntel.
- `Resolve(seq, verdict)` — Unresolved → True|False only; verdicts
  are terminal (the ledger doesn't un-judge).
- `DistortionFor(subject)` → 0..100 — resolved-false share of all
  resolved claims. The feed:
  - **RivalDeck** — `PrepareCounterDeck` takes an optional
    `const IntelLedger*`: effective depth scales down by the
    distortion (a rival whose intel on us is bad counters worse).
  - **QuantumFog** — callers apply the value to briefing-time
    certainty (`SetCloudBelief`/`initialIntel`); distorted intel
    enters the field as thinner certainty. Campaign feeds belief;
    Gameplay stays clean.
- `ProvenWrongFor(rivalId)` → `std::vector<ClaimKind>` — false
  verdicts on RivalTemperament/RivalHabit map to 6.3's
  `RenderDossier` provenWrong seam — the claim-verdict loop closes
  in narrative.

## Implementation Tasks

- [x] `IntelLedger.{h,cpp}` — store + verdicts + feeds + persist
- [x] `RivalBook::PrepareCounterDeck` optional intel param
- [x] Tests: record/resolve lifecycle, terminal verdict, distortion
      math, counter-depth erosion, dossier seam, persist round-trip
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_ledger` — 13 pins (dense seqs, shape gates,
  terminal verdicts, distortion share, unresolved≠discredited,
  dossier seam, pending list, depth erosion, persist round-trip,
  schema reject)
- `potato_test_campaign` — RivalDeck regression (default param)
- ctest 19/19 + dep_guard
