---
title: 'MingGoRTS - Game Design Document'
game_type: 'strategy (primary) + turn-based-tactics (secondary) + rpg elements'
platforms: ['PC']
created: '2026-09-17'
updated: '2026-09-18'
status: 'draft'
narrative_complexity: 'Heavy'
source_brief: '_bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md'
source_narrative: '_bmad-output/narrative-design.md'
---

# MingGoRTS (民國史詩 RTS) — Game Design Document

## Executive Summary

### Core Concept

A single-player tactics-RPG × auto-battle RTS set in Republican-era China (warlord era → Northern Expedition → War of Resistance → Civil War). The player never clicks units in battle — they *write* them. Before each engagement they compose a hand of doctrine cards (trigger → condition → action), deploy named squads, then watch both scripts execute in real time, spending scarce Command Points for seconds-long interventions when the plan breaks.

The campaign is structured around two philosophies drawn from 至聖者指揮學 and 治平者管理學:

- **至聖者無戰** — the highest form of victory is subduing the enemy without fighting: defections over kills, deterrence over engagement, and — on designated chapters — completing the objective without battle at all.
- **治平者無勝** — military victory is not the goal; governance is. Hearts (民心) and order (秩序) form a persistent campaign axis that can convert a battlefield defeat into a governing success.

Every battle runs on two layers sharing one map: **history** above (columns, fords, grain convoys, telegraph lines) and **myth** beneath (local gods, fox spirits, war-god possessions, vengeful ghost legions). Myth-layer actions are the primary amplifier of hearts-and-order — pacifying a shrine or laying a ghost legion to rest translates directly into governance capital on the next battle.

### Target Audience

1. Tactics/strategy players who dislike APM pressure (FFT, Total War, wargame fans) — co-primary.
2. Systems/scripting players (FF12 Gambits, Opus Magnum) — co-primary.
3. Narrative RPG players drawn by named casualties, historian's-pen reports, commander biographies — secondary.
4. Engine showcase audience (PotatoEngine capability demonstration) — secondary.

### Unique Selling Points (USPs)

- **Doctrine-writing as the core verb of a narrative campaign** — not a puzzle mode or an optional subsystem.
- **Enemy commanders are readable, lootable doctrine decks** — scout their habits, counter-write them, take their cards.
- **A victory axis that can transcend battlefield outcomes** — "defeated in the field, victorious in governance" is a designed, achievable ending state.
- **No-battle chapters** — deterrence, negotiation, and subversion can complete designated chapters without engaging (the legendary 至聖者 path).
- **History × myth dual layer** — folk-religion interventions mechanically feed the governance axis.

## Goals and Context

### Project Goals

- Prove the write → watch → revise loop is fun in isolation (MVP "Broken Bridge" acceptance: a non-programmer fields a battle in ≤ 10 minutes; battles run ~90 s–4 min).
- Demonstrate PotatoEngine's gameplay stack end-to-end (doctrine interpreter, flow-field movement, probability-cloud fog, event-recorded replay).
- Build a campaign frame where the 無戰/無勝 philosophies are load-bearing systems, not flavor text.

### Background and Rationale

The "pre-battle scripting" niche is validated but underserved: Gambit-style systems exist as features (FF12) or puzzle frames (Opus Magnum), and prepare-then-defend rhythm exists in The Last Spell; no major title makes doctrine-writing the core of a full narrative campaign. The Republican era provides a setting where military victories routinely failed to produce order — a historical argument for the 治平者 axis.

Development is a solo-developer effort; scope decisions (chapter count, content volume) should be read against that constraint.

## Core Gameplay

### Game Pillars

1. **Writing IS fighting** — doctrine composition is the only verb that matters; the UI must make scripting feel like arranging pieces, not coding. Gate: a non-programmer fields a battle in 10 minutes.
2. **無戰而勝 (Subdue Without War)** — kills are the least-valued outcome; defection, deterrence, and intact occupation are the high-value outcomes. Every battle and several whole chapters offer a non-annihilation resolution path.
3. **Enemies are readable decks** — enemy commanders run the same doctrine language; personality is an observable signature to scout, counter-write, and loot.
4. **Loss is content** — named casualties, corpse recovery, looted enemy doctrine; failure branches by severity rather than binary game-over.

*Content structure (not a fifth pillar, but load-bearing):* the **History × Myth dual layer** carries pillars 2 and 4 — myth pacification feeds the governance axis, and what is provoked or pacified rewrites the next battle's history layer.

### Core Gameplay Loop

```
INTEL (scout the enemy commander's deck & the myth layer)
  → COMPOSE doctrine cards (planning layer, turn-based)
  → DEPLOY squads & objective/rally pins
  → WATCH both scripts execute in real time
      (probability-cloud fog: enemy positions are superposition
       until observed — intel points or contact scouting)
  → INTERVENE: spend CP to seize a squad for ~8 s when the plan breaks
  → GOVERN: occupy villages, spare defectors, pacify shrines
  → AFTERMATH: historian's report, roster of the dead, replay —
      which card failed, when, and what it cost
  → REVISE the hand; campaign 民心/秩序 accumulate
```

### Win/Loss Conditions

Battle-level outcomes resolve on a five-rung ladder (already implemented as `SageOutcome`):

| Outcome | Trigger | Meaning |
|---|---|---|
| **SubdueWithoutWar** | Victory AND defections > annihilations AND 民心 ≥ 60 | 至聖者無戰 — the enemy joined, not died |
| **Victory** | Objective taken, standard resolution | Conventional win |
| **GovernedPeace** | Objective lost (Defeat or Draw) AND 民心 ≥ 70 AND 秩序 ≥ 70 | 治平者無勝 — lost the field, won the mandate |
| **Defeat** | Objective lost without governance cover | Conventional loss |
| **Fallen** | 墮落 reaches 100 | Overrides everything — atrocity/chaos path |

A timer expiry with the objective contested resolves as a **Draw** at the battle layer, which then maps into the ladder as a non-victory (eligible for GovernedPeace, otherwise Defeat).

A squad counts as a **defection** when it surrenders or switches sides before destruction; it counts as an **annihilation** when destroyed in combat. Within Defeat, a severity ladder (L0 orderly retreat → L5 army destroyed) shapes the aftermath: named casualties, corpse-recovery opportunities, and looted-doctrine loss all scale with rung.

墮落 is intentionally a one-way ratchet — no reduction path. The atrocity path is a designed trap; restraint, accepting surrenders, and corpse recovery are the only counterplay.

Chapter-level completion additionally permits **無戰通關**: designated 治平 chapters can be resolved by negotiation/deterrence/subversion without entering real-time battle, yielding the highest campaign evaluation tier.

## Game Mechanics

### Primary Mechanics

- **Doctrine cards** — trigger → condition → action → modifier, with per-rule cooldown. Hand size 4 (MVP). The planning layer is turn-based; execution is real-time.
- **Command Points (CP)** — scarce resource spent on ~8-second squad seizures during execution (1 CP per seizure; direct orders override script) and on one-shot stratagems — e.g., the **emergency stop** (1 CP: all squads halt and hold). MVP: 3 CP. [ASSUMPTION: stratagem menu and per-stratagem costs pending tuning.]
- **Intelligence economy** — `intel` points fund remote Observe (cost 2) and Probe (cost 1, partial collapse) against the probability-cloud fog; observations decay on a timer (~25 s) back to superposition. Contact scouting (physical proximity, ~3 cells) reveals for free.
- **Probability-cloud fog** — unrevealed enemy squads exist as superposed candidate clouds (quasi-random disc distribution); personality priors bias the cloud (aggressive generals push toward the front, cunning toward flanks, disciplined produce tighter priors); entangled squads correlate — collapsing one concentrates its partner.
- **Morale → execution rate** — per-squad morale scales doctrine firing reliability (weighted global average); broken squads disobey and can defect.
- **Governance actions** — occupying villages intact, accepting surrender, protecting convoys, and myth-layer pacification raise in-battle 民心/秩序; pillaging, massacres, and shrine desecration raise 墮落.
- **Named roster & replay** — every squad captain is named; casualties persist on the campaign roster; battles are event-recorded for replay timeline and the historian's report.

### Controls and Input

- Planning layer: mouse-driven card editor (three-column rule lists, drag/drop, planner template one-click load), Alt+click to move objective/rally pins.
- Execution layer: LMB select/observe cloud, RMB doctrine-intervention orders, CP button bar, pause/time-scale keys.
- No per-unit pathing — squads navigate by flow fields.

## Strategy-Specific Design

### Resource Systems

| Resource | Layer | Source | Sink |
|---|---|---|---|
| Intel (情報) | Battle | Fixed per-battle budget (MVP 10) + recon events | Observe (2), Probe (1) |
| CP (指揮點) | Battle | Fixed per-battle (MVP 3) | 8-s squad seizures, stratagems |
| 民心 (hearts) | Battle + Campaign | Villages held intact, surrenders accepted, shrines pacified, low collateral | Enables GovernedPeace, SubdueWithoutWar; gates 無戰 options |
| 秩序 (order) | Battle + Campaign | Supply lines protected, occupation discipline, low 墮落 | Gates GovernedPeace; low campaign value spawns unrest events |
| 墮落 (corruption) | Campaign | Atrocities, desecration, abandoned dead | One-way ratchet — at 100 → Fallen (see Win/Loss) |
| Myth favor (天命) | Myth layer | Pacified gods/spirits, honored dead, laid ghost legions | Myth interventions that convert to 民心/秩序 |
| 治績 (governance record) | Campaign | Composite of accumulated 民心 + 秩序 across chapters | Gates 無戰 chapter options; evaluated at era ending |

Enemy-side economy: manpower, forage, command-will — all attackable on-map assets (raid the convoy, not the column). [ASSUMPTION: enemy-economy per-node values pending map data.]

### Unit Types and Stats

Squads are the atomic unit (no individual soldiers are commanded). Era-appropriate archetypes: line infantry, cavalry, artillery train, engineers, militia. The MVP fields 4 player squads plus 1 **hero** — a free-moving named officer unit under direct control (distinct from ordinary squad captains; [NOTE FOR DESIGNER: hero auto-behavior during CP seizures on other squads is still open]). Stats: strength, morale, cohesion (execution-rate multiplier; squad breaks and disobeys at morale 0), movement class. Counter logic is positional (fords, high ground, enfilade) rather than stat triangle — the doctrine script is the counter system.

### Technology and Progression

- **Doctrine library** — new cards are earned by looting defeated commanders' decks and by myth-layer boons; the deck IS the tech tree. Hand size grows **4 → 7 slots** across the campaign as the doctrine vocabulary expands.
- **Roster veterancy** — surviving squads gain cohesion; dead captains are permanently dead.
- **Reputation track** — campaign 民心/治績 unlock 無戰 resolution options on later chapters.

### Map and Terrain

Battle maps are asymmetric, narrative-driven (MVP "Broken Bridge": broken bridge, two fords, ambush forest, high ground, village, shrine). Terrain effects: fords slow and channel; elevation grants observation range on the fog layer; villages are 民心 nodes; shrines are myth nodes. Fog of war is the probability-cloud system — the player's information problem is literal quantum uncertainty, not hidden sprites.

### AI Opponent

Enemy commanders are personality-parameterized doctrine decks: three axes (aggression / discipline / cunning) + signature doctrines (e.g., "堅壁不戰", "擊弱潰敵"). Difficulty scales by deck quality and scouting clarity, not resource cheats. Personalities are authored composites — no direct real-figure portrayal.

MVP enemy commander: **格洛克 (Grok the Reckless)** — aggression 90 / discipline 40 / cunning 10, two signature habits (charges the nearest ford on contact; ignores flanking until casualties mount). His deck is the tutorial read: simple, loud, exploitable.

### Victory Conditions

Battle: objective-based (seize/hold/escort) resolved through the five-rung outcome ladder. Campaign: cumulative 民心/治績 evaluation determines the era's ending — unification by conquest, unification by mandate (無戰/無勝 optimal path), or collapse (Fallen). Early concession is always available and feeds back into governance rather than binary loss.

## Progression and Balance

### Player Progression

Doctrine library size, roster veterancy, and campaign governance stats carry forward. The player's skill progression is literal: better-written scripts.

### Difficulty Curve

MVP teaches one habit-reading loop; each chapter adds one commander with a stronger signature and one new pressure (time limit, convoy escort, myth-layer deadline). Onboarding acceptance: non-programmer fields a battle in ≤ 10 min.

### Economy and Resources

Battle resources are fixed budgets (no in-battle income beyond recon events) — scarcity is the design. Campaign 民心/秩序/墮落 are persistent accumulators with decay-free carry.

## Level Design Framework

### Level Types

- **Field battles** — standard doctrine-vs-doctrine engagements.
- **治平 chapters** — objectives resolvable without battle (negotiation deadlines, deterrence demonstrations, subversion chains).
- **Myth-incursion battles** — history layer battle disturbed by a myth-layer event (ghost legion, fox-spirit deception) that must be pacified mid-fight for governance credit.

### Level Progression

~15 commander-biography chapters tracing warlord era → Northern Expedition → War of Resistance → Civil War. Myth-layer causality links chapters: what is pacified or provoked changes the next battle's terrain, weather of morale, or available 無戰 options.

## Art and Audio Direction

### Art Style

History layer: muted Republican-era palette — mud, uniform drab, telegraph poles. Myth layer: saturated spectral palette — shrine golds, fox-fire, ghost-legion desaturation. Probability-cloud fog renders as visible shimmering candidate clusters. VRM-based character portraits for named commanders and captains.

### Audio and Music

Era instrumentation (dizi, erhu, martial percussion) for history; processed/treated versions for myth. The battle score is **assembled live from fired doctrine cards** — each card carries a motif; the music you hear is the script playing. CP interventions drop the mix into a muffled "soul-leaving" soundscape for the seizure's duration. Historian's report is accompanied by brush-stroke and paper sounds. Morale breaks are audible (routed-squad din) — audio is a gameplay signal, not decoration.

## Technical Specifications

### Performance Requirements

- 60 FPS sustained on mid-range PC during a full battle (target: ≤ 8 squads + fog clouds + replay logging).
- Headless determinism: every gameplay system runs without GL context (all POTATO_TESTS are headless).

### Platform-Specific Details

- PC (Windows primary; the engine targets cross-compiler builds).
- Custom engine: PotatoEngine (C++20, OpenGL).

### Asset Requirements

- Character cards: `potato.character_card/1` JSON + VRM portraits.
- Battle maps: `potato.battle_map/1` JSON; replays `potato.battle_replay/1`; roster `potato.roster/1`.

## Development Epics

See `epics.md` for the detailed breakdown. Summary:

| # | Epic | Status |
|---|---|---|
| E-1 | Battle core: doctrine interpreter, squads, flow fields, phase machine | Implemented |
| E-2 | Intelligence layer: probability-cloud fog, observe/probe, entanglement, personality priors, interference | Implemented |
| E-3 | Planning layer UI: card slots, deck editor, map pins | Implemented |
| E-4 | Real-time + Post-battle: HUD, CP buttons, historian report, roster, replay timeline | Implemented |
| E-5 | Governance axis: 民心/秩序 sources & sinks, five-rung outcomes, campaign accumulators | Partially (outcomes implemented) |
| E-6 | Myth layer: shrines, spirit events, myth→governance conversion | Not started |
| E-7 | Campaign frame: chapter map, biography structure, persistent stats | Not started |
| E-8 | 無戰 chapters: negotiation/deterrence/subversion resolution path | Not started |
| E-9 | Content Pipeline: commander roster, doctrine card pool, card art pipeline | Partially (card pipeline exists) |
| E-10 | Polish & Onboarding: tutorial (Q-9 in flight), difficulty curve, balance, sandbox simulator | In progress |

## Success Metrics

- Non-programmer fields a battle in ≤ 10 minutes (onboarding gate).
- A playtester can describe an enemy commander's signature habit after watching one battle — decks are legible.
- ≥ 1 in 5 players resolves a chapter via the 無戰 path on a second playthrough.
- Battle outcome spread: SubdueWithoutWar and GovernedPeace are each achieved in ≥ 10% of completed battles in tuning telemetry.

## Out of Scope (v1.0)

- Multiplayer / networked play.
- Campaign map as a 4X layer — the campaign is chaptered narrative, not freeform strategy.
- Direct portrayal of real historical figures (composites only).
- Modding support.
- Console/handheld ports.

*Not out of scope:* the **doctrine sandbox simulator** (instant-replay testing ground for decks vs. decks, no narrative) is a required balancing tool per the brief's risk mitigation — it is excluded from the MVP but in scope for v1.0 (tracked under E-10).

## Assumptions and Dependencies

- [ASSUMPTION: ~15 chapters is the target campaign length; chapter count may compress as scope is discovered.]
- [ASSUMPTION: 民心/秩序/墮落 exact tuning values (60/70/100 thresholds) are v1 starting points subject to playtest telemetry.]
- [NOTE FOR DESIGNER: which theaters and myth cycles anchor the ~15 chapters is still open — the narrative workflow should resolve this.]
- [NOTE FOR DESIGNER: hero squad's auto-behavior while the player seizes a different squad is undefined.]
- Dependency: every gameplay rule must be exercisable in headless test binaries (no GL context) — this is an engine-level constraint, not a rendering feature.
- Dependency: card/VRM art pipeline stays on the vendored glTF loader + zero-dependency PNG codec.
