---
baseline_commit: NO_VCS
---

# Story 12.8 — World Save Integration

> Epic 12 — 行營輿圖 Open Campaign World (W) ·
> `potato.campaign/2` ·
> **Status: done**

## Story (from epics.md)

As a player,
I want the world state — warband position, control flags,
resolved encounters, chosen character — inside the atomic save,
So that a reloaded campaign stands exactly where it was left.

## Acceptance Criteria

- **Given** a save carrying world state, **when** loaded, **then**
  the schema gate validates and the world resumes bit-identically
- **And** old-format saves reject cleanly per the schema gate
  (no migration in v1.0).

## Context

Save granularity is one file per slot (`save_<slot>.json` =
one `potato.campaign` doc via tmp→rename) — "inside the atomic
save" means embedding, not a second file. The schema gate +
the AC's "old-format saves reject" together make the version
bump the honest choice: `potato.campaign/1` docs fail the
`/2` gate cleanly, no migration path.

Embed, not reference: `ledger` already rides as its own
`potato.ledger/3` sub-document; `world`/`character` follow —
the sub-doc schema inside the envelope, same convention.

Wire shape (`potato.campaign/2`):

```json
{"schema":"potato.campaign/2",
 "chapter":{...},"roster":[...],"ledger":{potato.ledger/3},
 "world":{potato.worldstate/1},        // optional
 "character":{potato.character/1}}    // optional
```

- `world` optional — absent = no world bound (linear/test
  states stay expressible); present = a full
  `potato.worldstate/1` sub-doc validated by
  `WorldState::FromJson` inside the envelope.
- `character` optional — the chosen commander's
  `potato.character/1` doc verbatim (player-created included:
  `created`/`deck` round-trip inside the character's own rules).
- Neither id is reconciled against content at load —
  `world.world` matching the loaded `WorldMap` and `character`
  existing in the `CharacterLibrary` are CALLER checks (the doc
  can't see content registries); documented seam.

CampaignState gains (hasX + value pattern, same as
EncounterDef::hasControlStake):

```cpp
bool hasWorld_ = false; WorldState world_;
bool hasCharacter_ = false; Character character_;
// HasWorld()/GetWorld()/TakeWorld? — keep minimal:
HasWorld(), GetWorld() (mutable+const), SetWorld(),
ClearWorld();
HasCharacter(), GetCharacter(), SetCharacter(),
ClearCharacter();
```

`CampaignState.h` now includes `Campaign/World/WorldState.h` +
`Campaign/Characters/Character.h` — both Campaign-internal,
no cycle (neither includes CampaignState).

## Dev Notes — guardrails

- ToJson emits `world`/`character` ONLY when present — absent
  == default keeps a worldless campaign byte-identical to its
  v1 shape minus the schema string.
- FromJson: `world`/`character` present-but-mistyped rejects;
  absent is fine. Their sub-schema strings are checked inside
  WorldState::FromJson / Character::FromJson — the envelope
  does not re-check `schema` fields itself, the sub-parsers do.
- Unknown top-level keys: CampaignState::FromJson today reads
  named fields only (tolerant envelope) — keep it.
- `SetWorld` takes the WorldState by value — validation is
  FromJson's; the setter is a plain move-in.
- potato_test_campaign: the `/2`-as-rejection pin at ~line 121
  inverts — update fixtures to `/2`, and add a `/1`-rejects pin
  (the AC's clean-reject requirement).
- World id ↔ map reconciliation: caller-side (shell compares
  `GetWorld().WorldId()` to the loaded map's id); document it.

## Implementation Tasks

- [x] CampaignState.{h,cpp} — SCHEMA→/2, world/character
      members + accessors, ToJson/FromJson sub-doc embed
- [x] potato_test_campaign — bump fixtures to /2, /1-rejects
      pin, world+character round-trip pins
- [x] Build + ctest; story + sprint-status sync

## Dev Agent Record

### Completion Notes List

- `potato.campaign/2` landed: `SCHEMA` bumped; two optional
  sub-docs — `world` (verbatim `potato.worldstate/1`) and
  `character` (verbatim `potato.character/1`) — emit only when
  bound, so a worldless campaign stays byte-minimal.
- Accessors follow the hasX + value pattern: `HasWorld()`/
  `GetWorld()` (mutable+const)/`SetWorld()`/`ClearWorld()`;
  `HasCharacter()`/`GetCharacter()`/`SetCharacter()`/
  `ClearCharacter()`. Setters are plain move-ins — validation
  lives in the sub-parsers.
- FromJson gates: present-but-mistyped sub-docs reject the
  envelope (`"world"`/`"character"` non-object fails); each
  sub-parser checks its own `schema` field — the envelope does
  not re-check. Absent fields load unbound.
- The `/2`-as-rejection pin inverted: fixture asserts a
  `potato.campaign/1` doc now fails the schema gate cleanly
  (no migration in v1.0, per AC).
- World-id ↔ loaded `WorldMap` and character-id ↔
  `CharacterLibrary` reconciliation documented as caller-side
  checks — the doc can't see content registries.
- Verified on the F: build (`F:\dev\MingGoRTS\build-f`):
  `potato_test_campaign` green incl. the 12.8 section
  (round-trip bit-identical via emit→parse→emit, SaveSystem
  slot round-trip, /1 rejection, mistyped sub-doc rejections);
  full ctest **24/24**.

### File List

- `Campaign/State/CampaignState.{h,cpp}` — /2 schema, world +
  character members/accessors, sub-doc embed/parse
- `Examples/potato_test_campaign.cpp` — /2 fixtures, /1-reject
  pin, world+character round-trip pins, slot-level save/load
- `_bmad-output/implementation-artifacts/12-8-*.md`,
  `sprint-status.yaml`

## Validation

- `potato_test_campaign` green: /2 round-trip with world +
  character bit-identical via emit→parse→emit; /1 doc rejects
  with `schema` error; mistyped sub-docs reject; absent fields
  preserve byte-minimal emit.
- `ctest` green incl. `gameplay_dep_guard`.
