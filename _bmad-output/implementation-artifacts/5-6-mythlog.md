# Story 5.6 — MythLog

> Epic 5 — Myth Dual-Layer (D) · Folk register · **Status: done**

## Story (from epics.md)

As a player,
I want a folk-register log of myth events parallel to the official
record,
so that the people remember differently than the clerks.

## Acceptance Criteria

- **Given** myth events in a chapter, **when** MythLog renders,
  **then** entries use the folk/rumor register (傳聞/俚語) distinct
  from HistorianReport **and** MythLog entries may contradict the
  official account without either being marked wrong.

## Context

The container landed in 5.4 (`potato.mythlog/1`, seq-gated append)
and the settlement bridge in 5.5 (`LogMythEvents`, wired into
`ResolveAftermath`). What remains is the voice: narrative-design's
register table says MythLog is "the people talking — rumors, not
records", and the contradiction is a feature ("the reward is
discrepancy, not lore").

## Design

`RenderMythLog(const MythLog&)` → folk text, one line per entry:

- Every line opens with a hearsay marker (據說/聽說/有人發誓) — the
  folk register asserts nothing falsifiable.
- Per-action formula keyed by `action` id; variant = `seq % 2`
  (deterministic — no PRNG in the text layer).
- **Licensed exaggeration**: the contradiction mechanism. Ghost
  armies are always 數以千計 in the telling though the sim/ledger
  know one garrison; invasions are 天罰 regardless of which banner
  the host carried (folk memory misattributes). Neither side is
  marked wrong — the ledger doesn't correct rumor, rumor doesn't
  cite the ledger.
- Unknown action ids render a generic folk line — the register
  outlives the catalog.

## Implementation Tasks

- [x] `RenderMythLog` — folk formulas, hearsay markers, seq-variant picks
- [x] Tests: register distinctness, exaggeration license, determinism, unknown action
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_ledger` — render pins; ctest + dep_guard
