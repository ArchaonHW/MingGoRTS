# Story 3.5: Persistent Roster

## Status: done

## Story

As a player,
I want named squads whose casualties, veterancy, and scars persist,
So that survival means something.

## Acceptance Criteria

**Given** battle aftermath with casualties,
**When** applied to the roster,
**Then** dead members persist as dead, survivors gain veterancy
**And** the roster serializes inside CampaignState.

## Design

`Campaign/Roster/Roster.{h,cpp}` — the roster's business logic
(3.1 shipped the storage spine; this story ships the write paths).

- `Enlist(state, name)` — the roster's legitimate write path.
  Enforces the same invariants `CampaignState::FromJson` holds at
  the wire (nonempty, ≤ MAX_NAME_LEN bytes, unique, roster ≤
  MAX_ROSTER). Returns the new entry's index.
- `ApplyAftermath(state, rows)` — applies a battle's casualty
  report. `AftermathRow{name, casualties, wiped}` is keyed by
  roster NAME (the campaign identity; the battle layer's
  squadIndex is mapped to names by the caller at deployment).
  **Validate-then-mutate**: every row is checked before any
  mutation lands — unknown names, already-dead squads (a dead
  squad can't fight), duplicate names within the report, and
  counter bounds all reject the whole aftermath.
- Apply: `casualties += n` always; `wiped` sets `dead=true`
  (permanent — dead stays dead); survivors gain `veterancy++`.
- Serialization is already carried by `potato.campaign/1` (3.1) —
  the wire makes `dead` persist verbatim, which the round-trip
  test pins.

"Scars" as a distinct field have no producer or consumer yet —
veterancy/casualties/dead are the persisting numbers the
narrative layer reads ("chapters survived, merits, scars").
Deferred; recorded in deferred-work.md.

## File List

- `Campaign/Roster/Roster.h` (new)
- `Campaign/Roster/Roster.cpp` (new)
- `Campaign/State/CampaignState.cpp` (ToJson boundary
  re-validation)
- `Examples/potato_test_campaign.cpp` (+3.5 test block)
- `_bmad-output/implementation-artifacts/3-5-persistent-roster.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/deferred-work.md`

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `Roster.{h,cpp}`: Enlist + ApplyAftermath
- [x] Task 2 — tests: aftermath applies, dead stays dead,
    veterancy gain, round-trip persistence, unknown name /
    dead-squad / dup-row / bound rejections, atomic no-mutation
    on failure, boundary pins (roster-full, MAX name/counter,
    cumulative overflow, veterancy-bound wipe), ToJson boundary
    re-validation
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- Three-layer review (AC:PASS) produced two real fixes:
  - **Mutable `GetRoster()` seam**: callers can bypass every
    write-path invariant and `ToJson` would happily emit a save
    `FromJson` rejects. Fix: `ToJson` re-validates the emitted
    roster via `ValidateRoster` — the trust boundary is the wire,
    not the accessor.
  - **Signed-overflow UB in bound checks**: stored counters were
    trusted operands. Now range-checked before any arithmetic,
    and comparisons use subtraction form.
- Error-class convention: roster-invariant violations report
  `"roster"`; report-shape errors (dup row, out-of-range
  casualties, oversized report) report `"aftermath"`.
- Design decision (recorded): dead entries are memorials —
  corpses hold slots and names forever; long campaigns can
  exhaust MAX_ROSTER. Intended weight of permanent loss.
- A veterancy-bound live squad is battle-locked for survivor
  rows (a wipe still lands) — bound enforcement, reachable only
  from a boundary file or bypassed write.

