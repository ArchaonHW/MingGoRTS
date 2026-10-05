# Story 5.5 — Invasion Events

> Epic 5 — Myth Dual-Layer (D) · Sim: Spirit Assaults · **Status: done**

## Story (from epics.md)

As a system,
I want myth-layer counterattack events when infiltration peaks at 3,
so that the gods push back.

## Acceptance Criteria

- **Given** a region at infiltration 3, **when** the invasion check fires,
  **then** a ghost-army/curse event spawns per the region's deity
  **AND** the event books into ledger and MythLog.

## Context

5.1 gave the infiltration ladder; 5.2 gave shrines a live presence-scan in
`MythField::Tick`; 5.4 put ghost garrisons into `ghosts_`. At level 3 the
myth layer stops being passive — it counterattacks. Invasions are tick-scan
events: no journal op, no CP, no mandate gate (the gods' economy is not ours —
their "cost" is booked as our terror/debt).

## Design

### Semantics — the deity pushes back on the occupier

- Invasions are **shrine-anchored** (the AC's "region's deity" — only shrine
  regions carry a deity; non-shrine level-3 regions are silent overrun).
- `INVASION_PERIOD_TICKS` (8s/160t) cadence per shrine while level==3;
  first fire is immediate on reaching 3.
- On fire (`MythInvasion` event, aux = form):
  - `aux=0 rise`: exclusive flesh claimant X → ghosts rise for the OTHER
    side (the god refuses the occupier) — `side` = ghost side.
  - `aux=1 wild`: empty/contested ground → `ghosts_=2` wild haunting —
    contests ALL claims. A seeded-3 shrine cannot be dedicated while wild.
  - `aux=2 sustain`: a garrison already stands → audit beat, no state change.
- **Wild dissipation**: `ghosts_==2` clears when the level drops below 3
  (pacify is the counter-play: 3→2, haunting subsides, dedication opens).
- Side garrisons (0/1) persist to battle end — pacify does not dismiss a
  side's spirit host.

### Ledger (visitation, not deed)

`MythInvasion` books **around** the `playerSide` gate — visitations are
universal:

| beneficiary | legs | note |
|---|---|---|
| `e.side == playerSide` | `+軍威5 / −天命5` | divine aid bills your mandate — 神助要還 |
| enemy/wild | `+軍威2 / −民心4` | terror stiffens ranks, empties hearts |

### MythLog

`LogMythEvents(log, events)` — campaign-side bridge folding the battle
stream: `MythInvasion` → `{action:"invasion", name:"神罰"}`.
`MythActionInvoked` is skipped (already logged at purchase — no double entry).

### Wire

- `SimEvent::MythInvasion` = 12; aux bound stays 9.
- `TOOL_VERSION` 6 — checksum fold gains `ShrineTrack::invasionTick`.
- Deterministic: per-shrine `lastInvasion` tick, map-order scan, no RNG.

## Implementation Tasks

- [x] `ShrineTrack::invasionTick` + `INVASION_PERIOD_TICKS` + Tick invasion loop + wild dissipation
- [x] `SimEvent::MythInvasion`(12); `BattleRecorder` bound + TOOL_VERSION 6
- [x] checksum fold invasionTick; governance wire pin 4-12/13-reject
- [x] `DeedBook`: visitation branch (pre-side-gate), blessing/terror legs
- [x] `MythLog::LogMythEvents` bridge
- [x] Tests: immediate fire, pushback side, wild deny-all, cadence sustain, wild dissipation via pacify, deed legs, log entries, e2e replay
- [x] Review (three passes)
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_myth` — invasion suite; `potato_test_ledger` — visitation
  legs + log; ctest + dep_guard
