# Story 5.4 — Myth Actions

## Status

done

## Story

As a player,
I want myth actions — pacify shrine, invoke possession, raise ghost
armies — costing 天命,
so that the second layer is a real option set.

## Acceptance Criteria

1. Given sufficient 天命 and a valid target, when a myth action
   triggers, its effect applies on the myth layer and amplifies the
   民心 axis (GDD: "pacifying a shrine converts directly into
   governance capital").
2. Each action enters the MythLog by name.
3. Actions are journaled — record→replay reproduces them bit-exact.

## Design

**Two layers, one seam.** 天命 lives in the campaign ledger;
effects land in the battle sim. Gameplay cannot touch the ledger,
so payment is the campaign layer's job (`SpendMandate`, 5.3) and
the sim carries the deed — mirroring how interventions work.

- **Campaign/Myth/MythActions.{h,cpp}**: `MythActionDef` catalog
  {kind, id, name, cost, sink account+amount, needsSquad} +
  `PerformMythAction(ledger, log, kind, side, region, squad)` —
  SpendMandate pays, MythLog records by name, caller then issues
  the battle verb. Sink choices amplify per GDD:
  - `pacify_shrine` 安撫: 15 天命 → +10 民心 (governance capital)
  - `invoke_possession` 降神: 20 → +6 武功 (the host line is honored)
  - `ghost_army` 陰兵: 25 → +8 軍威 (dread walks)
- **Campaign/Myth/MythLog.{h,cpp}**: `potato.mythlog/1` — minimal
  append container {seq, action id, chronicle name, side, region,
  squad}; 5.6 grows rendering + the folk/rumor register.
- **Gameplay**: myth actions ride the intervention journal —
  `InterventionKind` gains `MythPacify`/`MythPossession`/
  `MythGhostArmy` (aux 7-9, all 0 CP: the ledger is the price, same
  rule as Execute). Issued mid-Execution, applied next tick start.
  `SimEvent::Kind::MythActionInvoked` (11) emits at apply:
  aux = action kind, param = region, squadIndex = target, side =
  caster. Verifier re-issues via ReIssue like every command.
- **Effects** (`MythField` / `Squad`):
  - Pacify: `PacifyRegion` — Pacification through the table +
    releases the shrine's allegiance latch (the god withdraws;
    stance memory persists per 5.2). Rejects when nothing would
    change (level 0 AND unconsecrated shrine — journal no-op rule).
  - Possession: `Squad::possessed` + attack+2 / cohesion+25
    one-shot blessing; own effective squad only, once.
  - Ghost army: `ghosts_[region]` garrison — myth-layer presence.
    Ghosts hold the MYTH layer (they claim/dedicate shrines) but
    never the historical one (flesh holds villages). Requires
    infiltration >= 1 — the veil must be thin for spirits to rise.

## Tasks

- [x] MythActionKind enum + wire gates (Gameplay/Myth)
- [x] Squad::possessed + MythField ghosts_/PacifyRegion/RaiseGhost
- [x] InterventionKind 7-9 + Issue verbs + apply cases + checksum
- [x] Wire: kind bound 11, aux bound 9, ReIssue cases, TOOL_VERSION 5
- [x] Campaign: MythLog + MythActions catalog + PerformMythAction
- [x] Tests + ctest + dep guard + three-layer review — 19/19

## Review Notes (three-layer)

- AC: all three criteria pinned (myth-layer effect + 民心 sink;
  log-by-name via `MythLogEntry.name`; journaled through the
  Intervention command channel — sealed record replays bit-exact).
- Edge — kept as design, documented: pacify releases the latch but
  doesn't evict presence, so an occupier re-dwells and re-dedicates.
  Net −5 天命 / +10 民心 per cycle — a slow conversion channel, not
  a free exploit.
- Edge — ghosts hold the myth layer only: they dedicate shrines
  (own the claimant slot) but never villages; ghost-vs-flesh
  contests. Pinned.
- Blind — `MythActionInvoked` is deliberately NOT a DeedBook deed:
  the spend posting is written at action time; double-booking would
  count the cost twice. Comment added to prevent future misreads.
- Unpaid-miracle audit: a record can carry an act whose 天命 debit
  was never posted — pairing `MythActionInvoked` events against
  `action:`-tagged spend entries is a CrossCheck/audit seam.
  Deferred work (Epic 2's suspicion channel already exists to
  flag it).

## File List

- `Gameplay/Myth/Infiltration.{h,cpp}` — `MythActionKind`, ghost
  garrisons (`ghosts_`, `RaiseGhost`, `GhostAt`), `PacifyRegion`,
  `ShrineAtMut`, ghost presence in `Tick`
- `Gameplay/Command/Intervention.h` — `MythPacify`/`MythPossession`/
  `MythGhostArmy` (aux 7-9, 0 CP)
- `Gameplay/Doctrine/Doctrine.h` — `MythActionInvoked` (kind 11)
- `Gameplay/Squad/Squad.h` — `possessed` flag
- `Gameplay/Sim/BattleController.{h,cpp}` — `IssueMythPacify`/
  `IssueMythPossession`/`IssueGhostArmy` + `CheckMythVerb` dedup,
  apply cases, checksum folds (possessed, ghosts)
- `Gameplay/Record/BattleRecorder.{h,cpp}` — kind bound 11, aux
  bound 9, `TOOL_VERSION` 5
- `Gameplay/Record/ReplayVerifier.cpp` — `ReIssue` cases
- `Campaign/Myth/MythLog.{h,cpp}` — `potato.mythlog/1` log
- `Campaign/Myth/MythActions.{h,cpp}` — catalog + `PerformMythAction`
- `Examples/potato_test_myth.cpp` — verb gates, apply semantics,
  ghost dedication/contest, record→replay e2e
- `Examples/potato_test_ledger.cpp` — pricing, spend gate,
  MythLog round-trip
- `Examples/potato_test_governance.cpp` — wire pins (kind 12, aux 10)

## Deferred Work

- CrossCheck seam: pair `MythActionInvoked` events vs `action:`-tagged
  mandate debits; unpaired acts are audit findings (unpaid miracles).
- Possession is battle-scope by design (flag not roster-persisted);
  if narrative wants lasting scars it lands via LedgerEvent/Roster
  (Epic 6 territory).
- MythLog rendering + folk/rumor register: Story 5.6.
- `CommitMyth` persists infiltration only — ghost garrisons and
  shrine allegiance don't cross battles yet (map-revision question).
