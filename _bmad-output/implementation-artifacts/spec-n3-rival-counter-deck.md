---
title: 'N-3 Rival Strategist Counter-Deck (ledger leaks)'
type: 'feature'
created: '2026-09-18'
status: 'in-progress'
route: 'dispatch'
baseline_revision: '63edf81'
review_loop_iteration: 0
followup_review_recommended: false
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: N-3)
  - _bmad-output/narrative-design.md (Opposition + "ledger leaks")
warnings: []
deferred: []
---

<intent-contract>

## Intent

**Problem:** The rival strategist never learns — every battle the enemy
runs the same personality deck regardless of how the player fought last
time, so player habits carry no narrative consequence.

**Approach:** Mine the previous battle's recorder stream for the
player's most-fired doctrine trigger, persist it as a habit ledger JSON,
and have the rival splice counter-rules into its doctrine next battle;
the historian report writes the tell ("彼之陣法，似針對我軍慣用").

## Boundaries & Constraints

**Always:**
- Habit stats derive from `BattleRecorder` events of the form
  `"<squadName> doctrine: <TriggerName> -> <ActionName>"` emitted by
  `BattleController::EvaluateDoctrines`; only player-team squads count
  (team resolved via `Roster` squad names, same approach as
  `TeamOfEliminated` in `HistorianReport.cpp`).
- Cross-battle persistence is a new JSON schema `potato.habit_ledger/1`
  (counts per trigger, battle count, top trigger); loads tolerate a
  missing/partial file as empty.
- Counter rules are `DoctrineRule`s spliced into the enemy's
  `BuildDoctrineFor` output at priority 5–15 (below signature at 0,
  above personality patches/fallback) — the produced `DoctrineSet` must
  round-trip `potato.doctrine_set/1`.
- `HistorianInput` gains an optional `counteredHabit` string; when
  non-empty the report emits the tell line before the omission count.
- Everything headless-testable; two simulated battles suffice (per queue
  note — campaign identity belongs to E-7, do not build it here).

**Never:**
- No new trigger/action enum values, no BattleController event-format
  change (the existing emit already carries trigger+action names).
- No `DuanqiaoPlayable.cpp` edits (parallel session owns it; headless
  test proves the mechanism).
- No networking, no third-party JSON lib — use
  `Serialization/JsonParser.h` only.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| HABIT_MINE | recorder with mixed-team doctrine events | counts only player-team triggers; `TopTrigger()` = most-fired | missing roster → all events treated as player's (documented) |
| LEDGER_ROUNDTRIP | ToJson → FromJson | counts/battles/top_trigger identical | wrong schema or malformed JSON → false, ledger unchanged |
| LEDGER_MERGE | two battles' ledgers merged | counts accumulate; top re-derived | empty merge is a no-op |
| COUNTER_DECK | habit trigger X | `CounterRulesFor(X)` ≥1 rule; spliced set serializes | unknown/empty habit → no counter rules (signature-only deck) |
| REPORT_TELL | `counteredHabit="EnemyInRange"` | report text contains the tell line naming the habit | empty string → no tell line |

</intent-contract>

## Code Map

- `Gameplay/BattleController.cpp:699-704` -- emits
  `"<squad> doctrine: <Trigger> -> <Action>"` on doctrine-driven order
  change; the habit-mining source format
- `Gameplay/BattleRecorder.h` -- `records` (t+event); `AddRecord` is
  public so tests can craft events without running a battle
- `Gameplay/Doctrine.h/.cpp` -- `DoctrineTrigger`/`DoctrineAction`
  enums; `DoctrineSet::AddRule` sorts by priority asc;
  `ToJson/FromJson` = `potato.doctrine_set/1`;
  `TriggerName`/`TriggerFromName` for (de)serialization
- `Gameplay/EnemyGeneral.{h,cpp}` -- `BuildDoctrineFor(squad, rank,
  count)` returns a mutable `DoctrineSet`; `ApplyTo(battle, team)` is
  the model for counter application
- `Gameplay/HistorianReport.{h,cpp}` -- `HistorianInput` +
  `ComposeHistorianReport`; tell line goes before the omission-count
  trailer (pattern: `TeamOfEliminated` for roster-based team lookup)
- `Gameplay/Roster.h` -- `RosterEntry{squadName, team}` for player/enemy
  discrimination of doctrine events
- `Serialization/JsonParser.h` -- `Potato::JsonValue` for the ledger
  schema (no third-party JSON)
- `Examples/BattlePlanTest.cpp` + `CMakeLists.txt` -- test binary +
  `POTATO_TESTS` registration convention

## Tasks & Acceptance

**Execution:**
- `Gameplay/RivalStrategist.h` -- new: `HabitLedger` (counts per
  `DoctrineTrigger`, `battles`, `FromRecorder`, `Merge`, `TopTrigger`,
  `ToJson`/`FromJson`/`SaveToFile`/`LoadFromFile`,
  `potato.habit_ledger/1`) + `RivalStrategist` (`CounterRulesFor`,
  `ApplyTo(battle, team, general, ledger)`) + `CounterLine(habit)` --
  single home for the whole ledger-leak mechanism
- `Gameplay/RivalStrategist.cpp` -- implement: event parsing via
  `TriggerFromName`/`ActionFromName`, roster team filter, JSON
  roundtrip, counter-rule table (data-driven: habit → 1-2 rules),
  doctrine splicing mirroring `EnemyGeneral::ApplyTo` -- all logic lives
  here
- `Gameplay/HistorianReport.h` -- add `std::string counteredHabit` to
  `HistorianInput` -- opt-in tell line
- `Gameplay/HistorianReport.cpp` -- emit tell line when
  `counteredHabit` non-empty, before omission trailer -- report-side
  only
- `Examples/RivalStrategistTest.cpp` -- new headless test covering the
  I/O matrix rows + a real battle run proving the emitted event string
  parses back into the ledger -- end-to-end proof
- `CMakeLists.txt` -- add `Gameplay/RivalStrategist.cpp` to `Gameplay`
  lib, `RivalStrategistTest` target + `POTATO_TESTS` -- registration

**Acceptance Criteria:**
- Given a recorder stream with player and enemy doctrine events, when
  `HabitLedger::FromRecorder` runs, then only player-team triggers are
  counted and `TopTrigger()` returns the player's most-fired trigger.
- Given a saved ledger file, when the next battle builds the enemy deck
  via `RivalStrategist::ApplyTo`, then every enemy `GetDoctrine` result
  contains at least one counter rule and serializes to valid
  `potato.doctrine_set/1`.
- Given `HistorianInput.counteredHabit` set, when the report is
  composed, then the text contains the tell line; given it empty, the
  line is absent.

## Spec Change Log

## Review Triage Log

## Design Notes

Counter-rule table is data-driven; counter semantics follow the
"answer the habit" reading of the narrative design:

```
EnemyInRange (rush habit)    -> EnemyInRange/AttackWeakest p5 (focus lead rusher)
                                + AllyEngaged/DefendNearestAlly p15 (collapse)
ObjectiveReached/Always push -> EnemyInRange/AttackNearest p5
                                + Always/HoldPosition p90 (anchored, beats p100 advance)
HealthBelow (retreat habit)  -> EnemyInRange/AttackWeakest p5 (finish the wounded)
MoraleBelow (morale-fragile) -> EnemyInRange/AttackNearest p5 + AllyEngaged p15 (mass contact)
UnderAttack/Outnumbered      -> EnemyInRange/AttackNearest p5 (out-aggress early)
AllyEngaged (support habit)  -> EnemyInRange/AttackWeakest p5 (pick off isolated)
```

Report tell line: `彼之陣法，似針對我軍慣用「<habit>」。—` emitted
between the roster sentences and the omission count, so the audit
trailer stays last.

## Verification

**Commands:**
- `cmake --build build --config Debug --target RivalStrategistTest` --
  expected: build success (MSVC)
- `build\bin\Debug\RivalStrategistTest.exe` -- expected: all checks
  PASS, exit 0
- `cd build && ctest -C Debug` -- expected: no regressions
- `cmake --build build-mingw --target RivalStrategistTest` +
  `build-mingw/bin/RivalStrategistTest.exe` -- expected: PASS
