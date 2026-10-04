# Story 3.1: CampaignState Facade

## Status: done

## Story

As a developer,
I want a CampaignState facade aggregating chapter, roster, and
ledger stores under one versioned schema,
So that save/load is one operation.

## Acceptance Criteria

1. **Given** mid-campaign state,
   **When** serialized to `potato.campaign/1` JSON,
   **Then** it round-trips losslessly
   **And** it depends only on public Gameplay headers.

## Design

`Campaign/State/CampaignState` — the single save/load unit. Three
stores under one versioned document:

- `ledger` — the real Epic 2 store, embedded as its full
  `potato.ledger/3` sub-document (chain verify rides inside
  `Ledger::FromJson`, so a tampered embedded chain rejects the
  whole campaign doc).
- `chapter` — `ChapterProgress`: `current` indexes into the
  parallel `unlocked`/`resolved` vectors (equal sizes required;
  empty ⇒ current==0; current==size is the campaign-complete
  sentinel). Stories 3.3/3.4 land the library and progression.
- `roster` — `RosterEntry` list: name (unique, 1–64 bytes),
  veterancy/casualties (0..1e6), dead. Story 3.5 enriches it.

Wire format (`potato.campaign/1`):

```json
{
  "schema": "potato.campaign/1",
  "chapter": {"current": 0, "unlocked": [true], "resolved": [false]},
  "roster": [{"name": "...", "veterancy": 0, "casualties": 0,
              "dead": false}],
  "ledger": { ... potato.ledger/3 doc ... }
}
```

Load semantics mirror Ledger: full validation into a temp value;
failure returns `Fail` and the caller's state is never touched.
Unknown member keys are ignored (forward-compat).

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `CampaignState.{h,cpp}` + stores
- [x] Task 2 — tests: round-trip (incl. CJK name + embedded
    chain/suspect), bad schema/field rejection, roster/chapter
    bounds, full-state Emit→Parse→FromJson pipeline
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- The facade is file-agnostic by design — `ToJson`/`FromJson` only;
  Story 3.2 owns tmp→rename durability.
- Roster/chapter fields are minimal spines — 3.3–3.5 grow them
  (schema bump is the designed absorption path).
- `MAX_NAME_LEN` counts bytes (~21 CJK chars), matching Ledger's
  byte-based tag/memo bounds.
- Review-established invariants: roster names unique; veterancy/
  casualties non-negative bounded; chapter vectors parallel-sized;
  `current` indexes the chapter space (complete = size()).

## File List

- `Campaign/State/CampaignState.h` (new)
- `Campaign/State/CampaignState.cpp` (new)
- `Examples/potato_test_campaign.cpp` (new)
- `CMakeLists.txt` (potato_test_campaign target + add_test)
- `_bmad-output/implementation-artifacts/3-1-campaignstate-facade.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
