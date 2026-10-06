---
baseline_commit: NO_VCS
---

# Story 12.4 — Character Model & Creation

> Epic 12 — 行營輿圖 Open Campaign World (W) · new schema
> `potato.character/1` · new dir `Campaign/Characters/` (D-ARCH-10) ·
> **Status: done**

## Story (from epics.md)

As a player,
I want to begin a campaign as a named general or a commander I
create — name, origin, personality priors, starting doctrine-deck
seed in `potato.character/1`,
so that the protagonist of the chronicle is mine.

## Acceptance Criteria

- **Given** the predefined general roster and the creation
  schema, **when** a campaign starts, **then** the
  chosen/created character binds priors into QuantumFog and
  RivalDeck surfaces
- **And** a bad-schema character doc rejects cleanly; the
  character serializes into the save
- **And** the hearsay record refers to the commander by their
  own name and priors.

## Context

Design authority: sprint-change-proposal-2026-10-06 (approved),
FR24. 12.1–12.3 landed geography, world state, and movement;
this story lands the **protagonist**: who the warband belongs
to. The Commander（統帥）is a persisted identity doc, not a sim
entity — Chronicler remains the narrative voice recording the
chosen commander's deeds (narrative-design.md).

**"Predefined roster + created commander" — resolved
interpretation:** both are the same wire doc. Predefined
generals are authored `potato.character/1` files shipped in a
content dir; a player-created commander is a `potato.character/1`
doc authored by the creation UI (12.9). The registry doesn't
distinguish provenance mechanically — an optional `created`
flag records it for later systems (mint claims, display).
Creation-time validation IS `FromJson`: the schema is the
boundary, so the UI can serialize-then-load its own draft for a
free round-trip check.

**"Starting doctrine-deck seed":** a u64 `deck_seed` plus an
optional explicit `deck` array of doctrine card ids. Seed-driven
deck construction is Epic 9/12.9 territory — this story persists
the field and (when `deck` is present) validates shape/bounds
only. Cross-validation against `DoctrineLibrary` is a bind-time
concern of the encounter pipeline (12.5), NOT a load-time
rejection — deck content may legitimately reference cards the
test library lacks.

### Wire format — `potato.character/1`

```json
{
  "schema": "potato.character/1",
  "id": "lv_bu",
  "name": "呂布",
  "origin": "九原",
  "prior": "aggressive",
  "deck_seed": 42,
  "deck": ["press_the_advance"],
  "created": false
}
```

- `id` — non-empty, ≤64 bytes; the hearsay subject key (the
  IntelLedger `subject` — rivals' dossiers key off this id).
- `name` — non-empty, ≤64 bytes; display name, CJK welcome.
- `origin` — non-empty, ≤64 bytes; 出身 as bounded free string
  (content pass may tighten to a vocabulary later).
- `prior` — REQUIRED; reuse the `RivalPrior` vocabulary from
  `Campaign/Rivals/RivalDeck.h` (`aggressive|defensive|cunning`)
  via `RivalPriorFromName`. The dossier comment already calls
  this "personality prior — biases doctrine authorship and fog
  shape"; the player character shares the vocabulary.
- `deck_seed` — u64, required; may be 0 (a valid seed).
- `deck` — optional array of card-id strings (each ≤64 bytes,
  ≤32 entries); absent = deck derived from seed later.
- `created` — optional bool, default false.
- Unknown top-level fields reject (schema-gate convention);
  wrong types, bounds violations, bad prior names reject.
- `ToJson`/`FromJson` round-trip is byte-identical for
  equivalent docs (JsonValue canonical ordering does the work).

### Registry — `CharacterLibrary`

Mirror `WorldLibrary`/`ChapterLibrary` exactly: `LoadDir` scans a
directory, sorted filenames (deterministic order), schema-gated
`Json::Load` per file, per-file `rejected` list, bad files don't
mutate committed state, duplicate `id` rejects, empty dir
succeeds, unreadable dir returns `io`. `Count()`/`At()`/`Find()`.

### Priors binding — the two surfaces

`Campaign/Characters/CommanderBind.{h,cpp}` (or fold into
`Character.cpp` — dev's call, keep functions free):

```cpp
// Fog-shape bias per prior — applied to a FogConfig BEFORE the
// QuantumFog is constructed at battle assembly (12.5 consumes;
// this story defines + tests the bias table).
struct PriorFogBias {
    int probeGainDelta = 0;
    int decayPerMinuteDelta = 0;
    int detectThresholdDelta = 0;
    int initialIntelDelta = 0;
};
PriorFogBias PriorBias(RivalPrior p);
// Apply deltas with the same clamps FogConfig semantics imply:
// decay >= 1, threshold/initialIntel in [0,100], probeGain >= 0.
void ApplyPrior(Gameplay::FogConfig& cfg, RivalPrior p);

// Hearsay binding: records the commander as a RivalTemperament
// intel claim — subject = character.id, claim carries name +
// prior (e.g. "統帥呂布，性向 aggressive" — exact text is dev's
// call; tests assert the claim contains BOTH the name and the
// prior wire name). chapter = 0 (prologue intel).
Gameplay::Result<std::uint64_t> RecordCommander(
    const Character& c, IntelLedger& intel);
```

Bias table (ship these numbers; tuning is content-work later):

| prior       | probeGain | decay/min | detectThr | initialIntel |
|-------------|-----------|-----------|-----------|--------------|
| aggressive  | +10       | 0         | 0         | −10          |
| defensive   | 0         | −5        | 0         | 0            |
| cunning     | 0         | 0         | −10       | 0            |

Rationale (one line for the record): aggressive scouts press
hard but read thin; defensive holds what it learns; cunning sees
through feints.

**RivalDeck surface:** rivals' dossiers (`RivalBook`,
`potato.rivals/1`) key by subject id — the commander's `id` is
that key. `RecordCommander` planting a `RivalTemperament` claim
on `character.id` IS the RivalDeck-adjacent hearsay surface:
`IntelLedger::ProvenWrongFor`/`DistortionFor` already thread it
into `PrepareCounterDeck`. No `RivalBook` API change needed —
encounter assembly (12.5) will pass the ledger through.

### Save serialization

`Character::ToJson`/`FromJson` byte-identical round-trip is the
"serializes into the save" contract — the doc rides the
`potato.campaign` envelope verbatim in 12.8 (world save
integration owns the envelope seam; do NOT touch
`CampaignState`/`WorldState` wire formats here).

### Explicit non-goals

- No character-to-WorldState binding field (12.8's envelope).
- No deck construction from `deck_seed` (Epic 9/12.9).
- No creation UI (12.9) — the schema is the creation contract.
- No sim/battle presence — the commander never appears inside
  `Gameplay/`; priors shape config + hearsay only.
- No predefined-roster content pass beyond a couple of sample
  files for tests — authored generals are content, not code.

## Implementation Tasks

- [x] `Campaign/Characters/Character.{h,cpp}` — `Character`
      struct, `potato.character/1` `ToJson`/`FromJson` (validate
      into locals, commit on success), `CharacterLibrary`
      (`Load`, sorted scan, `rejected[]`,
      unique-id, empty/unreadable-dir rules)
- [x] `Campaign/Characters/CommanderBind.{h,cpp}` — `PriorBias`,
      `ApplyPrior` (FogConfig deltas + clamps), `RecordCommander`
      (IntelLedger RivalTemperament claim)
- [x] `Examples/potato_test_character.cpp` — happy-path decode
      (CJK name/origin), round-trip byte-identical, 16 rejection
      cases (schema, missing/bad fields, bounds, bad prior, bad
      deck shape), library dir-scan/duplicate/empty/unreadable,
      fog bias table applies + clamps, hearsay claim carries
      name+prior and feeds `PendingFor`/`ProvenWrongFor` surface
- [x] `CMakeLists.txt` — `potato_test_character` target + ctest
      registration (mirror `potato_test_world` block)
- [x] Build + ctest; story file + sprint-status sync

## Dev Agent Record

**Implemented 2026-10-06.**

- `Character.{h,cpp}` — `potato.character/1` doc: `id`/`name`/
  `origin` bounded strings (64B), `prior` via `RivalPriorFromName`,
  `deck_seed` u64 bitcast to int64 wire (WorldState seq precedent),
  optional `deck` (≤32 card ids ≤64B), optional `created` bool.
  Closed field set — unknown top-level keys reject. `ToJson` emits
  `deck`/`created` only when non-default so round-trips are
  byte-identical; ungated DOM accepted (schema tag verified when
  present).
- `CharacterLibrary` — WorldLibrary mirror: sorted `.json` dir
  scan, per-file `rejected[]` isolation, duplicate `id` rejects
  (first wins), empty dir ok, unreadable dir → `io`, canonical
  order by character id.
- `CommanderBind.{h,cpp}` — `PriorBias` table (aggressive
  `+probe/−intel`, defensive `−decay`, cunning `−detect`),
  `ApplyPrior` with clamps (decay ≥1, threshold/intel [0,100]),
  `RecordCommander` posting a `RivalTemperament` claim keyed by
  `character.id` carrying name + prior wire name — the hearsay
  surface RivalDeck already reads.
- Test fixup: initial "minimal doc emits identical bytes" pin was
  wrong — ungated input lacks `schema` while `ToJson` always
  stamps it; corrected to emit→parse→emit fixed-point + stamp
  assertion.

### Completion Notes List

- **Verified MinGW/Ninja (`POTATO_BUILD_GUI=OFF`)**:
  `potato_test_character` all-green (decode, byte-identical
  round-trip, 16 rejections + oversized id, library dir-scan/
  duplicate/empty/unreadable, bias table + clamps, hearsay claim
  surface); `ctest` 23/23 incl. `gameplay_dep_guard`.
- MSVC not verified this session.

### File List

- `Campaign/Characters/Character.h` (new)
- `Campaign/Characters/Character.cpp` (new)
- `Campaign/Characters/CommanderBind.h` (new)
- `Campaign/Characters/CommanderBind.cpp` (new)
- `Examples/potato_test_character.cpp` (new)
- `CMakeLists.txt` (`potato_test_character` target + ctest)

## Dev Notes — guardrails

- **Reuse `RivalPrior`** — do not fork a parallel prior enum;
  `RivalPriorName`/`RivalPriorFromName` are already the wire
  codec. If a distinct C++ type reads better, `using
  CharacterPrior = RivalPrior;` is acceptable — wire names must
  not change.
- Library pattern: copy `WorldLibrary`'s shape (sorted
  `ReadDir` scan, `rejected` vector of `{file, error}` entries —
  match the existing member names).
- `FromJson` validates ALL fields into locals before touching
  the member/commit — file-is-untrusted policy, transactional
  parse (same as `WorldState::FromJson`).
- `IntelLedger::Record` claim bound is 256 BYTES — a CJK claim
  string fits easily, but construct it from the doc's already-
  bounded fields (name ≤64 + fixed template ≪ 256).
- `ApplyPrior` mutates a `FogConfig&` ONLY — it does not
  construct a `QuantumFog` (that needs a `BattleMap&`; battle
  assembly is 12.5).
- Determinism: no PRNG, no I/O in bind functions; `Record` is
  the only mutation and it's deterministic insertion.
- Campaign stays headless; `Gameplay/Fog/QuantumFog.h` is the
  only Gameplay include needed for `FogConfig`.

## Validation

- `potato_test_character` green; `ctest` green incl.
  `gameplay_dep_guard`.
- Determinism pin: same doc → same bytes (canonical Emit);
  same prior → same FogConfig deltas.
