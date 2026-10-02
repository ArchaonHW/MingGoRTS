# Story 1.11: BattleRecorder & Replay Verifier

## Status: done

## Story

As a player,
I want every battle recorded as an event stream that replays to the identical outcome,
So that the record is the truth.

Implements FR12 (recorded replayable battles, record-is-truth, tamper-evident
seal, downgrade warning) and D-ARCH-6 (event sourcing: seed + deployment +
doctrine + CP event stream; replay re-runs the sim; the audit IS the replay).
Closes the Story 1.9 deferral: the Replan event gains its full path payload.

## Acceptance Criteria

1. **Given** a completed battle's recorded stream + seed, **when** the
   replay-verifier CLI re-runs it, **then** outcome hash matches bit-exact.
2. A tampered stream fails integrity verification and is rejected.
3. A record whose schema/version the verifier doesn't recognize is rejected;
   a record stamped by an older tool version verifies with a downgrade
   warning (FR12).

## Tasks / Subtasks

- [x] (AC: 1,2) Task 1 — JSON emit end
  - `JsonValue::Emit()` / `EmitTo(std::string&)`: canonical serializer —
    object keys in `std::less<>` order (already the map order), no
    whitespace, ints exact, strings JSON-escaped
    (`\"` `\\` `\b` `\f` `\n` `\r` `\t`, other controls as `\u00XX`).
    Reals via `std::to_chars` (shortest round-trip, locale-independent);
    an integral-looking emission gets `.0` appended so `Real(1.0)` never
    re-parses as `Int` (`1.0`, `-0.0` are fixpoints); non-finite reals
    emit `null` (JSON can't represent them — documented lossy).
    Emission is byte-stable → records hash deterministically.
- [x] (AC: 1) Task 2 — Self-describing command events
  - `SimEvent` gains `int side = -1;` and `std::vector<size_t> path;`
    (wire-format additions; events stay append-only and cheap).
  - BattleController fills `side` on every `Intervention` event and `path`
    on `Replan` — the recorded stream now carries complete replay inputs.
- [x] (AC: 1,2) Task 3 — `Gameplay/Record/BattleRecorder.{h,cpp}`
  - `Bind(seed, mapDoc, cardsDoc)` — optional `BindBalance(doc)`,
    `SetToolVersion(int)`.
  - Planning inputs recorded as an ordered op stream:
    `RecordDeploy(template, region, side)` — the full `SquadTemplate`
    is embedded (squad stats are sim state),
    `RecordSheet(squadIndex, cardIds)`,
    `RecordArrow(side, squadIndex, path)` — argument order mirrors
    `BattleController::DrawArrow`,
    `RecordIntel(fogSide, targetSquad, region, certainty)`,
    `RecordCpPool(side, cp)` — the host calls each alongside the matching
    controller call (recorder is deliberately dumb; no hidden state reads).
    CONTRACT: mirror only calls the controller ACCEPTED (returned true) —
    a rejected op in the record can never verify.
  - Lifecycle: single-use per battle. `Bind` resets all per-battle state
    (inputs, events, sealed flag); all `Record*`/`BindBalance` calls are
    no-ops after `Seal`; `Seal` itself no-ops unless the battle is in
    `Aftermath` — records are completed battles only.
  - `Seal(const BattleController&)`: copies `bc.Events()` (Issue-stamped
    Intervention events = the command stream), records final `Checksum()`,
    outcome, endTick, endBeat; computes `integrityRoot` = FNV-1a over
    canonical emit of the whole payload (tamper-evident per FR12).
  - `ToJson()` emits `potato.battle_record/1` (fails if not bound+sealed);
    reading records back is `Replay::VerifyFile(path)` — it parses, then
    verifies.
- [x] (AC: 1,2,3) Task 4 — `Gameplay/Record/ReplayVerifier.{h,cpp}`
  - `Verify(JsonValue) -> VerifyResult{ok, tampered, downgrade, reason}`:
    1. schema gate — only `potato.battle_record/1` exists; version
       negotiation is on `toolVersion` (>current → reject, <current →
       `downgrade` warning);
    2. strict field typing — every required field must carry its exact
       JSON type (no `AsInt`/`AsBool` fallback coercion); all int fields
       range-checked into int32 with semantic bounds (side ∈ {-1,0,1},
       certainty ≤ 100, aux ≤ 5, region/squad ≥ 0, …);
    3. size caps — inputs ≤ 1024, events ≤ 1M, endTick ≤ 200 min of sim;
    4. `endBeat` must be Aftermath(2) — an unclosed record can't claim a
       valid seal;
    5. recompute integrity root over the payload → mismatch = `tampered`;
    6. rebuild sim: embedded map/cards docs + seed; replay planning ops in
       order; `RequestBeat(Execution)`;
    7. tick loop: at `TickCount()==t`, re-issue every recorded Intervention
       stamped `t` (side/kind/payload from the event) through the real
       `Issue*` API — same validation as live play; commands stamped at
       endTick replay once before the forced-close RequestBeat;
    8. replay must reach Aftermath; compare event stream + final Checksum
       + outcome (winner, forced, elapsedTicks) bit-exact.
  - Any divergence → `ok=false` with the first mismatched event index.
- [x] (AC: 1) Task 5 — `Examples/potato_verify.cpp` CLI
  - `potato_verify <record.json>` → prints `VERIFY OK` / `VERIFY FAIL: <reason>`
    (+ `WARNING: downgrade` line), exit 0/1. Real file I/O — CLI is not the sim.
- [x] Task 6 — `Examples/potato_test_record.cpp`
  - Scripted battle (deploy, sheets, arrows, intel, commands incl. replan)
    → record → seal → emit → re-load → `Verify` → OK; identical checksum.
  - Tamper cases: flipped command target, edited seed, altered checksum,
    truncated event, wrong schema → each rejected (integrity or replay
    mismatch), original untouched.
  - Missing file → error result. Downgrade-stamped record → ok + warning.
- [x] Task 7 — CMake `potato_test_record` + `potato_verify`; build MinGW
  Debug, focused test, ctest, dependency guard.
- [x] Task 8 — `gds-code-review`; record 1.9 deferred item closed.

## Dev Notes

- **Record file layout (`potato.battle_record/1`)**:
  ```json
  {"schema":"potato.battle_record/1",
   "toolVersion":1, "seed":12345,
   "map":{...}, "cards":{...}, "balance":{...},
   "inputs":[{"op":"deploy","region":0,"side":0,
              "template":{"id":"veteran","name":"...","hp":200,"attack":15,
                          "speed":4,"cohesion":100,"unit":0,"cost":0}},
             {"op":"sheet","squad":0,"cards":["hold","push"]},
             {"op":"arrow","side":0,"squad":0,"path":[0,1,2]},
             {"op":"intel","fogSide":1,"squad":2,"region":3,"certainty":70},
             {"op":"cp","side":0,"cp":3}],
   "events":[{"kind":3,"tick":0,"squad":1,"slot":-1,"param":2,"aux":0,
              "side":0,"path":[],"card":""}],
   "endTick":42,"endBeat":2,"winner":0,"forced":false,"checksum":...,
   "integrity":{"root":<u64>}}
  ```
  `integrity.root` = FNV-1a over canonical `Emit()` of every field above
  except `integrity` itself. Tampering any byte changes the root; replay
  re-verification catches a self-consistent but semantically wrong record.
- Replan path fix: `SimEvent::path` replaces the 1.9 deferred `param=size`
  hack; `param` now also stores `path.size()` redundantly for the index —
  verifier replays from `path`, asserts `param == path.size()`.
- Commands are recorded **at issue time** via the event stream — an issued-
  but-never-applied command (beat closed early) is part of the record and
  replays identically.
- Verifier runs each command through `Issue*` — validation is exercised on
  replay exactly as live; a rejected command simply isn't in the applied
  stream and the checksum diverges (tamper or corruption → reject).
- Deployment `name` is preserved (part of sim state → checksum).

## Dev Agent Record

### Implementation Plan

`JsonValue::Emit` (canonical writer) → `SimEvent` payload fields →
`BattleRecorder` (ordered input ops + sealed event stream + hash root) →
`ReplayVerifier` (root check, rebuild, tick-replay, bit-exact diff) →
`potato_verify` CLI → `potato_test_record`.

### Completion Notes

- `JsonValue::Emit()` added: canonical, whitespace-free, keys in map order;
  strings fully escaped; ints exact (int64-min safe); reals `std::to_chars`
  shortest round-trip + `.0` type marker for integral reals, non-finite →
  `null`. `parse∘emit` is byte-stable (fixpoint-tested incl. `-0.0`).
- `SimEvent{side, path}` added; BattleController stamps `side` on all
  Intervention events and `path` on Replan — recorded stream is now a
  complete input journal. Story 1.9 deferral CLOSED.
- `BattleRecorder`: ordered `inputs[]` op stream (deploy/sheet/arrow/intel/
  cp — host mirrors each ACCEPTED controller call); single-use lifecycle —
  `Bind` resets state, `Seal` no-ops outside Aftermath, `Record*` no-op
  after seal. `ToJson` folds `integrity.root` (FNV-1a over canonical emit).
- `ReplayVerifier::Verify(JsonValue)`: schema gate → strict field typing +
  int32/semantic range checks → size caps (inputs ≤1024, events ≤1M,
  endTick ≤200min) → `endBeat==Aftermath` gate → root recompute (tamper →
  reject before any sim work) → rebuild controller from embedded
  map/cards/balance docs → replay input ops in order → tick loop
  re-issuing recorded Interventions at their issue ticks through the real
  Issue* APIs (incl. commands stamped at endTick before the forced close)
  → Aftermath check → bit-exact event-stream + checksum + outcome
  comparison with first-divergence field in `reason`.
- `toolVersion` carries the FR12 semantics: >current → reject; <current →
  verify + `downgrade` flag (until schema v2 exists).
- `potato_verify` CLI: `potato_verify <file>` → `VERIFY OK`/`VERIFY FAIL:
  <reason>`/`WARNING: downgrade`, exit 0/1.
- `potato_test_record` (32 checks): scripted battle incl. asymmetric
  side/squad arrow + redirect/probe/replan commands; natural side-wipe
  close; canonical-emit fixpoint tests; tamper matrix (seed edit, resealed
  redirect-target flip, forged checksum, truncated stream, wrong schema,
  toolVersion bounds, missing integrity/endTick); lifecycle guards
  (unsealed ToJson, mid-battle Seal reject, rebind reset); temp-file
  VerifyFile round-trip with unique name + checked write.

### File List

- Gameplay/Json/JsonValue.h/.cpp (Emit + canonical real emission)
- Gameplay/Doctrine/Doctrine.h (SimEvent side/path)
- Gameplay/Sim/BattleController.cpp (event stamping)
- Gameplay/Record/BattleRecorder.h/.cpp (new)
- Gameplay/Record/ReplayVerifier.h/.cpp (new)
- Examples/potato_verify.cpp (new CLI)
- Examples/potato_test_record.cpp (new)
- CMakeLists.txt (potato_test_record, potato_verify, potato_record_emit,
  potato_verify_cli)

### Debug Log References

- `.\build-mingw\bin\potato_test_record.exe` — 32/32 checks.
- `ctest --test-dir build-mingw` — 14/14 (incl. potato_verify_cli E2E).

### Change Log

- 2026-09-30: Implemented per story + AC; all green.
- 2026-09-30: Three-layer review patches applied — non-terminal Seal gate,
  RecordArrow arg order, strict field typing + int range checks, canonical
  real emission (to_chars/.0/null), single-use lifecycle, size caps,
  endBeat checks; story doc synced to the actual wire format.

## QA Results

Three-layer review (Blind Hunter + Edge Case Hunter + Acceptance
Auditor): 2 High + 4 Med + Lows patched. Headline fixes:

| Finding | Fix |
|---|---|
| Mid-Execution `Seal` produced a valid-looking record; `endBeat` serialized but never checked (High) | `Seal` gates on Aftermath; verifier requires `endBeat==2` AND replay reaching Aftermath |
| `RecordArrow` arg order (squad, side) clashed with `DrawArrow(side, squad, path)` — a footgun only hidden by symmetric test args (High) | Signature aligned; test now pins with an asymmetric side-0/squad-2 call |
| int64→int silent narrowing on forged fields | `FitInt`/`EventFromJson` range-check every field with semantic bounds |
| `AsInt`/`AsBool`/`Items` fallbacks coerced malformed records | Strict `Is*` typing on every required field |
| `%.17g` real emit — `1.0`→`1` re-parses as Int, `-0` fixpoint break, NaN/Inf invalid JSON, locale-sensitive | `std::to_chars` + `.0` marker; non-finite → `null` |
| `Bind` left stale state; post-seal mutation possible | Single-use lifecycle: Bind resets; Record*/BindBalance no-op after Seal |
| Unbounded inputs/events on hostile records | Caps: inputs ≤1024, events ≤1M, endTick ≤200min |

Deferred: uint64 seed/checksum serialized through signed JSON ints
(rejected >INT64_MAX is acceptable for v1); root computation emits the
full record before size checks (bounded by parser depth+caps); `Bind`
defers embedded-doc validation to Verify.
