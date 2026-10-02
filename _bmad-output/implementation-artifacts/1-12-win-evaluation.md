# Story 1.12: Win Evaluation

## Status: done

## Story

As a player,
I want battles to resolve military victory/defeat correctly,
So that chapters can end.

Final E0 story. Implements the Aftermath verdict: a `BattleResult`
(victor, casualties, ledger-event list) emitted at close, plus a
stalemate timer producing a draw evaluation per rules.

## Acceptance Criteria

1. **Given** Execution running, **when** one side's squads all
   rout/annihilate, **then** Aftermath emits a `BattleResult`
   (victor, casualties, ledger-event list).
2. A stalemate timer produces a draw evaluation per rules.

## Tasks / Subtasks

- [x] (AC: 1,2) Task 1 — `Gameplay/Eval/WinEval.{h,cpp}`
  - `enum class CloseReason { Wipe, Concede, Stalemate }` — wire-format
    stable ordinals (SimEvent::aux on `ResultDeclared`).
  - `struct LedgerEvent { Type{Victory,Draw,Casualty,Rout}, side,
    squadIndex, squadId }` — typed postings Epic 2's five-account
    ledger consumes; `squadIndex` is the unique battle-scoped key
    (template ids are not), verdict entry last.
  - `struct EvalConfig` — `"eval"` object inside `potato.balance/1`:
    `stalemate_ticks` (default 6 min at 20 Hz; 0 = disabled).
  - `struct BattleResult` — the Aftermath artifact: closeReason,
    winnerSide, forced, stalemate, elapsedTicks, per-side
    effective/routed/destroyed (destroyed = casualties; routed squads
    survive for RefitCamp), `ledger` event list. Replaces
    `BattleOutcome`.
- [x] (AC: 1,2) Task 2 — BattleController integration
  - ctor gains `EvalConfig` (defaulted); `Outcome()` returns
    `const BattleResult&`.
  - `CloseBattle(CloseReason)`: counts + winner + ledger list +
    emits `SimEvent::ResultDeclared` (param=winnerSide,
    aux=CloseReason, side=-1) — the journal is self-describing.
  - `Tick()`: after the wipe check, stalemate fires when
    `TickCount() >= stalemateTicks` with both sides alive — closes as
    a draw (`stalemate=true`, `forced=false`, `winnerSide=-1`).
    Defeat conversion is the campaign layer's job; the battle reports.
- [x] (AC: 1,2) Task 3 — Wire format
  - `SimEvent::Kind` gains `ResultDeclared` (=3); `EventFromJson`
    accepts it.
  - Record payload gains `"stalemate"` (bool) + `"closeReason"` (int);
    verifier strict-types and compares both.
- [x] Task 4 — `Examples/potato_test_eval.cpp`: wipe victory with
  rout+ledger contents, mutual wipe draw, wipe-vs-stalemate boundary,
  concede (draw and truthful-victor paths), mid-rout effective-count
  pin, stalemate auto-close at configured tick, stalemate=0 disables,
  EvalConfig FromJson, determinism, ResultDeclared in event stream.
  (Destroyed/Casualty postings need a damage action — deferred.)
- [x] Task 5 — CMake `potato_test_eval`; MinGW build + ctest +
  dep guard.
- [x] Task 6 — three-layer review; E0 retro candidate after.

## Dev Notes

- Stalemate ≠ concede ≠ mutual wipe: `closeReason` records which;
  `forced`/`stalemate` stay as booleans for wire stability.
- `ResultDeclared` is stamped by `CloseBattle` immediately after the
  closing `BeatChanged` — event order is BeatChanged → ResultDeclared
  in every close path.
- Ledger list order is canonical (squad index), so records/replays
  produce identical lists without extra sorting.

## Dev Agent Record

### Implementation Plan

`WinEval` module (CloseReason + LedgerEvent + EvalConfig + BattleResult)
→ BattleController integration (ctor param, CloseBattle(reason,
stampTick), stalemate check) → wire format (ResultDeclared kind,
record stalemate/closeReason fields) → verifier strict-typing +
EvalConfig replay wiring → potato_test_eval.

### Completion Notes

- `Gameplay/Eval/WinEval.h/.cpp`: `CloseReason{Wipe,Concede,Stalemate}`
  (SimEvent::aux ordinals), `LedgerEvent{Victory,Draw,Casualty,Rout}`
  (squad-order postings, verdict last), `EvalConfig::FromJson` reads
  `eval.stalemate_ticks` (0=disabled, cap=200min verifier bound),
  `BattleResult` replaces `BattleOutcome`.
- `CloseBattle(CloseReason, stampTick)`: ResultDeclared stamps the
  producing tick (same convention as BeatChanged); concede stamps
  TickCount between ticks.
- Stalemate: `Tick()` checks `TickCount() >= stalemateTicks` after the
  wipe check — a wipe on the boundary tick still wins (wipe checked
  first). `stalemateTicks=0` disables.
- Record format gains `closeReason` (int) + `stalemate` (bool);
  `TOOL_VERSION` bumped 1→2 — v1-era records fail strict field
  validation (records are pre-release; no durable corpus). Verifier
  strict-types, replays with embedded `eval` config, and compares all
  outcome fields bit-exact. `BindBalance` is part of the replay
  contract: non-default eval config without it produces an
  unverifiable record.
- `EventFromJson` kind range extended to 3 (ResultDeclared).
- `CloseBattle` emits the closing `BeatChanged` itself — adjacency to
  `ResultDeclared` is structural, not caller convention — and resets
  `result_` fully on entry.
- `LedgerEvent` gains `squadIndex` (unique key) alongside `squadId`
  (display id — template ids collide on repeat deploys).

### File List

- Gameplay/Eval/WinEval.h/.cpp (new)
- Gameplay/Doctrine/Doctrine.h (SimEvent::ResultDeclared)
- Gameplay/Sim/BattleController.h/.cpp (EvalConfig, CloseBattle,
  ResultDeclared emit, stalemate check)
- Gameplay/Record/BattleRecorder.h/.cpp (closeReason/stalemate fields)
- Gameplay/Record/ReplayVerifier.cpp (typing, evalCfg, compare)
- Examples/potato_test_eval.cpp (new)
- Examples/potato_test_record.cpp (stalemate round-trip + eval cfg)
- _bmad-output/implementation-artifacts/sprint-status.yaml
- _bmad-output/implementation-artifacts/deferred-work.md
- CMakeLists.txt (potato_test_eval)

### Debug Log References

- `.\build-mingw\bin\potato_test_eval.exe` — 28/28 checks.
- `ctest --test-dir build-mingw` — 15/15 (all green, incl.
  potato_verify_cli against the extended record format).

### Change Log

- 2026-09-30: Story drafted.
- 2026-09-30: Implemented per story + AC; 28 eval checks, 15/15 ctest.

## QA Results

Three-layer review (Blind Hunter / Edge Case Hunter / Acceptance
Auditor) — AC:PASS across all three. Patched: BeatChanged→ResultDeclared
adjacency made structural (emitted inside CloseBattle); result_
fully reset on close; LedgerEvent carries unique squadIndex; TOOL_VERSION
bumped to 2 for the required-field format revision; mid-rout
effective-count semantic pinned by test; concede truthfulness
documented + tested; stalemate record→verify round-trip (both
directions) added to potato_test_record. Deferred: Destroyed/Casualty
unexercisable without a damage action (deferred-work.md).
