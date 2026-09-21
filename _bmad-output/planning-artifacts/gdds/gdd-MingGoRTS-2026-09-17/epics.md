# MingGoRTS — Development Epics

Companion to `gdd.md`. Each epic delivers mechanics serving the pillars; status reflects the implemented codebase as of 2026-09-17.

## E-1 Battle Core — Implemented

Doctrine interpreter (trigger/condition/action/cooldown), squad entities, flow-field group pathing, three-beat phase machine (Planning → Execution → Aftermath), named squad captains.

*Serves pillars 1, 4.*

## E-2 Intelligence Layer — Implemented

Probability-cloud fog (`QuantumFog`): superposed candidate clouds, Observe (intel 2), Probe (intel 1, partial), contact reveal (~3 cells, free), intel decay back to superposition (~25 s), entangled squads (Q-2), personality priors (Q-4), injectable random sources (Q-5), save/replay of fog state (Q-6), phase interference prototype (Q-7), perf bench (Q-8).

*Serves pillars 1, 3.*

## E-3 Planning Layer UI — Implemented

Card-slot deck editor, planner template one-click load, objective/rally map pins (A-1, `PlanningDeck` + `DuanqiaoPlayable`).

*Serves pillar 1.*

## E-4 Real-time + Post-battle — Implemented

Squad status bars, morale display, CP intervention buttons, doctrine-trigger message stream (A-2); historian's-pen report, named roster, replay timeline (A-3).

*Serves pillars 3, 4.*

## E-5 Governance Axis — Partially

**Done:** five-rung outcome ladder (`SubdueWithoutWar`, `Victory`, `GovernedPeace`, `Defeat`, `Fallen`), defection-vs-annihilation accounting, 民心/秩序 battle-level fields.

**Remaining:** in-battle 民心/秩序 *sources* (village occupation, surrender acceptance, convoy protection, atrocity penalties), campaign accumulators, unrest events at low campaign order.

*Serves pillar 2.*

## E-6 Myth Layer — Not started

Shrine/spirit entities on battle maps, pacify/provoke verbs, myth favor (天命) currency, myth→governance conversion, ghost-legion incursion events.

*Serves pillars 2, 4; enables narrative dual-layer.*

## E-7 Campaign Frame — Not started

Chapter map, ~15 commander-biography structure, persistent 民心/秩序/墮落, roster carry-over, inter-chapter causality hooks.

*Serves all pillars; delivery vehicle for the Heavy narrative.*

## E-8 無戰 Chapters — Not started

Negotiation/deterrence/subversion resolution path on designated chapters: pre-battle council phase where intelligence + reputation + myth favor can retire the enemy commander without combat. Highest-tier evaluation; gated by campaign 民心 and chapter flags.

*Serves pillar 2; the 至聖者 fantasy made literal.*

## E-9 Content Pipeline — Partially

**Done:** `potato.character_card/1` schema, ROC commander card set, VRM portrait pipeline (`PortraitRenderer`/`PortraitBaker`), synth-data tools.

**Remaining:** doctrine card pool expansion, enemy commander deck authoring tool, myth-layer art set.

## E-10 Polish & Onboarding — In progress

Tutorial scenario (Q-9) — pedagogical design per the brief: teach *regret*, not syntax. The player starts with a pre-filled deck that fails visibly; the lesson is finding and patching the hole, not learning card grammar. Also: difficulty curve pass, balance telemetry, perf verification, JP/KO i18n completion, and the **doctrine sandbox simulator** (deck-vs-deck instant testbed — required balancing tool per the brief, excluded from MVP but in scope for v1.0).

## E-11 Narrative Systems — Not started

Source: `_bmad-output/narrative-design.md` (steps 1–3: premise, structure, beats). The ledger/pen axiom turns into load-bearing systems:

- **Modular historian report** — replay event stream → fragment + judgment-template assembly (never fully authored prose); every report states its omission count (「本報告省略 N 項」); replay doubles as the audit tool. *Extends E-4.*
- **敵將判詞 (rumor-state)** — commander entrance carries a hearsay judgment, systematically *underrated* (fixed learnable bias); scouting verifies/falsifies; high-cunning commanders plant false判詞. *Extends E-2 dossier + E-9 cards.*
- **Rival counselor counter-deck** — the recurring foil fields a deck countering the player's most-used trigger type from the previous battle ("彼之陣法，似針對我軍慣用"). *Extends E-1/E-9 + BattleRecorder stats.*
- **Dispositions + reputation persistence** — enemy commander fates (subdued/slain/defected/retired) and the title track ("斷橋軍師") recorded per chapter; the ending cites them. *Depends E-7/G-9.*
- **Named myth-layer event log** — which shrine, which spirit, when; feeds the gods' testimony at the ending and the myth-seepage UX hook. *Depends E-6.*
- **Chapter conventions** — 題詞 opening verse, 判詞 entrance judgment, 欲知後事 closing hook; ending voices: 霸業 / 仁政 / 無字 / rival's hand (Fallen). *Depends E-7.*
- **Scope guard:** MVP (Broken Bridge) ships without the campaign frame; N-1/N-2/N-3 are battle-layer and can land earlier.

*Serves all pillars; the four-beat emotional register (command→suspense→mourning→the brush that stops).*

## Sequence

E-5 → E-6 → E-7 → E-8 is the critical path for the philosophy axis; E-9/E-10 run in parallel. E-1..E-4 provide the playable substrate already. E-11 rides the same critical path (its battle-layer items may start early; campaign items follow E-7).
