---
title: "MingGoRTS（民國史詩 RTS）"
game_type: strategy
platforms: "PC (Windows, Linux)"
created: 2026-09-29
updated: 2026-09-29
---

# MingGoRTS - Game Design Document

**Author:** potat
**Game Type:** Strategy (RTS × turn-based RPG hybrid; doctrine-card programming)
**Target Platform(s):** PC (Windows, Linux)

> 以筆代兵，以治為勝 — *Win with the brush, rule to victory.*

---

## Executive Summary

### Core Concept

A hybrid tactical game — turn-based RPG sensibilities × real-time strategy execution, running on the custom PotatoEngine (C++20). The player does not click units in battle; they **write** them. Before each battle the player composes doctrine cards (trigger → condition → action), deploys named squads, then watches both sides' scripts execute in real time — intervening only for seconds via scarce Command Points (CP) when a plan collapses.

The campaign is built on two philosophical axes:

- **至聖者無戰 (Win without fighting)** — defection over annihilation, deterrence over engagement; designated chapters are winnable with zero combat.
- **治平者無勝 (Governance over victory)** — military victory is not the goal; 民心 (popular support) and 秩序 (order) persist across battles, and battlefield defeat converts into a governance path rather than ending the run.

Every battle runs on two layers of the same map: the **historical layer** (columns, ferries, grain routes, telegraph lines) and the **myth layer** (earth gods, fox spirits, war-god possession, ghost armies). Mythic actions amplify the 民心 axis — pacifying a shrine converts directly into governance capital for the next battle.

The campaign presents itself as a **章回 chronicle the player is writing**: battles produce historian reports, omissions are confessed in print, enemy generals exist only as hearsay, and the replay archive is a tamper-evident historical record. The player is an author whose medium is war.

### Target Audience

Hybrid audience, in priority order:

1. **Strategy/planning players** — fans of deep pre-battle planning, auto-battler execution, and programmable systems (Unity of Command, Screeps-style authorship).
2. **Tactics & deck-building players** — drawn by doctrine composition and counter-deck dynamics.
3. **Narrative & history players** — drawn by the Republican-era China setting, chaptered storytelling, and the myth/history dual world.

Single-player only; depth and replayability over breadth.

### Unique Selling Points (USPs)

- **Authorship over micromanagement** — doctrine-card scripting replaces unit micro; a battle is a program the player writes and watches run.
- **Defeat is a fork, not an ending** — a lost battle writes into the chronicle's ledger and routes into governance play; the campaign has no single-battle game over.
- **Symmetric AI** — enemies run the same doctrine-script machinery; difficulty comes from better intel and adaptive counter-decks, never stat inflation.
- **Double-entry war** — every gain books a cost across five persistent accounts (武功/民心/天命/軍威/物資); forgery and audits are gameplay, not decoration.
- **Two worlds, one map** — historical logistics and mythic forces interleave on every battlefield.

---

## Goals and Context

### Project Goals

- Ship a complete single-player campaign of 8–12 chapters that proves the doctrine-authorship loop end to end. [ASSUMPTION: chapter count to be committed at first playtest of E0 + B.]
- Validate that no-combat victories and defeat-conversion produce genuinely playable campaigns, not gimmick paths.
- Demonstrate PotatoEngine as a viable game platform; this GDD is the engine's first real product. [ASSUMPTION: solo developer, non-commercial personal/portfolio project.]

### Background and Rationale

PotatoEngine (C++20, OpenGL 3.3+) and the MingGoRTS IDE are already implemented — engine core, AI platform, and tooling exist with completion reports in `docs/`. The game layer is greenfield: no gameplay or campaign code exists yet. This GDD is the first planning artifact and drives game architecture → epics → production. A post-v1.0 plan to split engine and game into separate repositories is acknowledged but deferred.

---

## Core Gameplay

### Game Pillars

1. **Authorship over control** — the player's agency lives in pre-battle writing, not mid-battle clicking. Any mechanic that rewards faster clicking over better planning violates this pillar.
2. **Subversion over annihilation** — the best victory is the one never fought. Defection, deterrence, and sabotage must always be first-class paths, not easter eggs.
3. **Governance over conquest** — wars are won in the ledger. 民心/秩序/墮落 persist across battles and decide the campaign as much as any field result. The chronicle must be trustworthy: an unauditable record makes governance meaningless.
4. **Two worlds, one map** — every battlefield exists in history and myth simultaneously; ignoring either layer is a viable but costly strategy.

### Core Gameplay Loop

1. **Planning** (untimed) — read intel through QuantumFog, compose doctrine cards onto squad sheets, deploy named squads, draw plan arrows.
2. **Execution** (real-time, target 5–10 min) — both sides' doctrine scripts run; the player intervenes only via scarce CP, and each intervention resolves within roughly 3 seconds of issue.
3. **Aftermath** — casualties persist to the roster, ledger entries settle (double-entry), governance deltas apply, and the historian issues the chapter's written account — including what it chose to omit.
4. **Campaign** — chapter outcome (including converted defeats) shapes intel, rival counter-decks, myth infiltration, and the next chapter's starting conditions.

### Win/Loss Conditions

- **Chapter victory** via one of three paths: military (enemy morale collapse — units rout below 20% cohesion [ASSUMPTION: balance target]), governance (民心 ≥ 70 and 秩序 ≥ 60 held at chapter end [ASSUMPTION: balance target]), or subversion (defection, deterrence, negotiated capitulation). Designated chapters are winnable with zero combat.
- **Defeat conversion** — a lost battle never ends the campaign. Losses book into the ledger (墮落 ratchet may rise), casualties persist, and the campaign routes into governance/recovery play.
- **Campaign resolution** — final ledger state produces one of four endings — the four-voice ending (四聲部結局); catastrophic governance collapse is itself one of the four, not a separate game-over screen.

---

## Game Mechanics

### Primary Mechanics

Numeric values are initial balance targets, to be tuned in playtesting.

- **Doctrine cards** — each card is trigger → condition → action → optional modifier. A squad carries a **squad doctrine sheet** of 3–5 slotted cards drawn from the player's **doctrine deck** (collection). Per-card cooldowns 5–60s. Deck pool grows to ~40–60 cards across the campaign [ASSUMPTION].
- **Three-beat battle state machine** — Planning → Execution → Aftermath.
- **Command Points (CP)** — start 3, +1 per 60s, cap 5 [ASSUMPTION]. Intervention costs: redirect squad 1 CP, override a doctrine card 2 CP, emergency retreat 3 CP [ASSUMPTION]. Interventions are deliberately seconds-scale — the battle belongs to the scripts.
- **QuantumFog** — probabilistic fog of war. Enemy positions exist as probability clouds on a 0–100 certainty scale; direct observation collapses a cloud to ground truth; probes sharpen certainty; certainty decays ~10 points/minute without observation [ASSUMPTION]; entanglement links fates of related clouds; rival generals' personality priors bias cloud shape. Intel certainty scales plan-arrow bonuses in real time.
- **BattlePlan** — the planning phase's centerpiece: drawn movement arrows whose execution bonus scales with QuantumFog certainty over the covered ground [ASSUMPTION].
- **Roster & RefitCamp** — named squads with persistent casualties; between chapters the player refits — deploy / heal / recruit / plunder — spending 物資.
- **Ledger (the five accounts)** — double-entry bookkeeping across 武功 (martial merit) / 民心 (popular support) / 天命 (mandate) / 軍威 (army prestige) / 物資 (materiel); every credit books a debit. 秩序 and 墮落 are **campaign accumulators derived from booked events**, not spendable accounts; 墮落 is a ratchet — it only increases. The ledger chain is append-only and tamper-evident: broken chains and forged books are detectable in-fiction, enemies can inject forged entries, and doubtful entries can be flagged — suspicion itself persists into later reports.
- **GovernanceField** — per-battle tracking of village occupation/burning, convoy escort/raid; events book into campaign 民心/秩序/墮落.
- **Myth infiltration** — per-region infiltration state machine 0–3; shrines as capturable myth assets; 天命 as a spendable currency; invasion events when infiltration peaks. Myth deeds enter the chronicle by name.
- **RivalDeck** — opponents statistically read the player's habitual triggers and pre-write counter-decks; enemy generals are presented through hearsay (聽聞態), never omniscient stats.
- **BattleRecorder** — every battle is a recorded, replayable account where the record is the truth; tampered archives are rejected, downgraded archives warned (pillar 3: replay is audit — a forgeable history is no history).

**Example doctrine cards** (worked examples, balance targets):

| Card | Trigger | Condition | Action | Modifier |
|---|---|---|---|---|
| 堅守陣地 Hold the Line | Enemy enters 2-tile range | Cohesion > 50% | Brace formation | −20% damage taken for 10s |
| 趁亂劫輜 Raid the Convoy | Enemy convoy spotted | Own squad unengaged | Move to intercept | +15% speed while closing |
| 見勢撤退 Prudent Withdrawal | Cohesion < 30% | No allied squad adjacent | Retreat toward map edge | Preserve squad for refit |
| 鳴鼓助攻 Sound the Advance | Ally routs enemy nearby | CP reserve ≥ 1 | Advance + volley | +10 軍威 on success |

### Controls and Input

[ASSUMPTION: mouse-driven planning UI — card slots, squad deployment, plan arrows — with hotkeys for CP interventions during Execution; Planning phase fully paused. Finalize at UX design.]

---

## Strategy Specific Design

### Resource Systems

- **Five ledger accounts** — 武功/民心/天命/軍威/物資. Double-entry: every credit books a debit — burning a village gains 物資 but debits 民心 and ratchets 墮落.
- **Battle-scope resources** — CP (intervention budget); intel/probes (QuantumFog sharpening, ~2–3 probes per battle [ASSUMPTION]).
- **Campaign-scope spending** — 物資 funds refit/recruit; 天命 funds myth actions; 民心/秩序 gate governance victories.
- **Scarcity as design** — CP cap 5 forces triage; the 墮落 ratchet makes atrocity a one-way debt that can never be repaid, only outrun.
- **Mint-claim outbox** — chapter settlements and rare achievements emit `potato.mintclaim/1` files at the aftermath boundary, gated by replay verification; an external relayer turns verified claims into testnet tokens and NFT collectibles (章回箋 / 本草圖鑑 / 名將檔案). Testnet only — technology showcase, zero real-world value.

**Economy tempo** (initial targets per chapter band [ASSUMPTION]):

| Band | 物資 income/chapter | Refit cost (typical) | 天命 earn/spend | Chapter duration |
|---|---|---|---|---|
| Ch. 1–3 (early) | ~100 | heal ~20, recruit ~40 | earn ~10, myth action ~15 | 30–45 min |
| Ch. 4–8 (mid) | ~150 | heal ~30, recruit ~60 | earn ~15, myth action ~25 | 45–60 min |
| Ch. 9–12 (late) | ~200 | heal ~40, recruit ~80 | earn ~20, myth action ~40 | 60–75 min |

Planning should run roughly 20–30% of chapter time; target total campaign 15–20 hours [ASSUMPTION].

### Unit Types and Stats

Unit matrix (stats are initial balance targets [ASSUMPTION]):

| Unit | Role | HP | Attack | Speed | Cost (物資) | Counters / countered by |
|---|---|---|---|---|---|---|
| 步兵 Infantry | Line holding, village occupation | 100 | 10 | medium | 40 | − artillery, cavalry flank |
| 騎兵 Cavalry | Fast flank, convoy raid | 80 | 14 | fast | 60 | − prepared infantry, bad terrain |
| 炮兵 Artillery | Ranged suppression | 60 | 25 (ranged) | slow | 80 | − cavalry, scouts |
| 工兵/輜重 Engineers & convoys | Ferries, supply, fortification | 70 | 2 | slow | 50 | non-combat; raiding them is a doctrine choice |
| 斥候 Scouts | QuantumFog observation/probing | 40 | 4 | very fast | 30 | fragile in combat |
| 民兵 Militia | Cheap garrison; ties to 民心 | 60 | 6 | medium | 15 | weak vs regulars; burning their villages costs 民心 |

Named generals (both sides) carry personality priors and rival-deck adaptation rather than raw stat superiority.

### Technology and Progression

No classic tech tree — progression is **doctrinal**: new cards and sheet slots unlock through chapters and ledger feats; squads gain veterancy through survival; 天命 unlocks myth actions. Power growth is horizontal (more options, better intel) rather than vertical (bigger numbers).

### Map and Terrain

- **Dual-layer maps** — historical layer: roads, ferries, grain routes, telegraph lines, villages, chokepoints, elevation, water. Myth layer: shrines, spirit roads, haunted zones overlaid on the same geometry.
- **QuantumFog replaces binary fog** — a certainty gradient, not a black/white reveal.
- **Strategic points** — ferries, depots, shrines; asymmetric maps favor doctrine planning over reflexes.

### AI Opponent

- **Symmetric doctrine scripts** — enemies run the same card machinery under the same CP rules; no cheating stats.
- **Personality priors** — aggressive / defensive / cunning generals bias both their doctrines and the shape of QuantumFog's predictions.
- **RivalDeck adaptation** — the enemy learns the player's habitual triggers across chapters and pre-writes counter-cards; difficulty scales through counter-deck depth and intel quality, not numbers.

### Victory Conditions

- **Military** — rout (cohesion collapse) or annihilation.
- **Governance** — 民心/秩序 thresholds held at chapter end.
- **Subversion** — defection, deterrence, or negotiated capitulation; designated chapters permit zero-combat wins.
- **Defeat** — converts into the governance path; the campaign continues with the loss booked into the ledger.
- **Campaign** — four-voice ending (四聲部結局) from final ledger state.

---

## Progression and Balance

### Player Progression

Card pool and sheet slots grow by chapter; squads accumulate veterancy and scars; generals earn titles in the campaign's persistent record; myth infiltration opens 天命-spending actions. Progression is account-shaped — what the player has *written into the ledger* defines who they are at the ending.

### Difficulty Curve

Early chapters teach doctrine primitives against thin counter-decks; mid-campaign adds governance stakes and myth infiltration; late chapters face rivals whose counter-decks have fully adapted to the player's habits — the final difficulty is the player's own predictability.

### Economy and Resources

Ledger double-entry enforces that nothing is free; refit consumes 物資, myth consumes 天命, atrocity banks 物資 against 民心 and permanent 墮落. Cross-resource tension (spend 民心 to win a battle vs. save it for the ending) is the core balance question.

---

## Level Design Framework

### Level Types

- **Field battles** — dual-layer maps with full doctrine execution.
- **No-combat chapters** — negotiation, deterrence, subversion; designated chapters winnable with zero fighting.
- **Setpieces** — convoy escort/raid, siege, myth-intrusion events.

### Level Progression

8–12 chapters [ASSUMPTION: exact count committed after E0 playtest] via a chapter library. Early: doctrine tutorials disguised as battles. Mid: governance and myth layers entangle — victories start costing things. Late: full rival adaptation, infiltration events, and the four-ending ledger reckoning.

---

## Art and Audio Direction

### Art Style

Pixel art (sprite atlas work already begun in the engine) with chaptered ink-illustration frontispieces (冊頁) for chapter opens and endings [ASSUMPTION: pixel sprites + ink accents blend — confirm at presentation epic]. CJK typography is a first-class UI concern; HUD density adjustable.

### Voice and Tone

The campaign reads as a 章回 historical chronicle the player is co-authoring: reports confess their omissions ("本報告省略 N 項"), enemy generals exist only in hearsay register, and myth deeds enter the record by name. Tone: literate, wry, and mournful — a war remembered, not a war won. This voice binds C-epic narrative systems, ledger audits, and myth logging into one fiction.

### Audio and Music

[OPEN — period instrumentation (古琴/鑼鼓) vs. modern minimalist score; decide at presentation epic. Battle audio must remain readable when the player is watching scripts rather than controlling units.]

---

## Technical Specifications

### Performance Requirements

[ASSUMPTION] 60 FPS at 1080p on a mid-range PC (GTX 1050-class GPU or better) during a 10-minute execution phase with full QuantumFog simulation; gameplay layer fully headless-testable (no GL context) — determinism required for the record-is-truth replay guarantee.

### Platform-Specific Details

- PotatoEngine (C++20), OpenGL 3.3+; Windows (MSVC + MinGW dual-verified) and Linux.
- Saves write atomically (no partial files); bad schema/version rejects without touching existing state.

### Asset Requirements

All game data as namespaced, versioned JSON via the engine's own JSON parser — no third-party JSON libraries. Content dirs: cards, maps, squads, fonts, avatars, campaign.

---

## Development Epics

### Epic Structure

Rebuilt for actual repo state (game layer is greenfield). Detail in `epics.md`.

| Seq | Epic | Name | Delivers |
|---|---|---|---|
| 1 | E0 | Battle Core | Gameplay layer skeleton: map, squads, doctrine interpreter, three-beat battle, QuantumFog, BattlePlan, symmetric AI, recorder — headless vertical slice |
| 2 | B | Campaign Persistence | Save/load (atomic), chapter library, roster & refit, rival-deck adaptation, campaign state facade |
| 3 | A | Governance Axis | GovernanceField, atrocity detection, 民心/秩序/墮落 accumulators, governance victory, defeat conversion |
| 4 | L | Ledger Mechanization | Five-account double-entry, tamper-evident chain, forgery/suspect mechanics, audit reports |
| 5 | D | Myth Dual-Layer | Infiltration state machine, shrines, 天命 currency, myth log, god stances, invasion events |
| 6 | C | Narrative Systems | Distorted intel ledger, narrative packs, chapter conventions, four-voice ending |
| 7 | E | No-Combat Chapters | Negotiation, deterrence, subversion paths |
| 8 | F | Presentation | Pixel sprite atlas, HUD density, audio, CJK fonts, frontispieces |
| 9 | G | Content & Tools | Card pool expansion, squad templates, deck plundering, enemy editor, sandbox, tutorial chapter |

Developer-facing C#/.NET tooling may supplement Epic G later over the versioned-JSON boundary only (no engine linking) — deferred to architecture. Engine/game repository split is post-v1.0.

---

## Success Metrics

### Technical Metrics

- Full headless test suite green on MSVC and MinGW builds.
- Headless battle simulation is deterministic — a replayed battle reproduces the recorded outcome.
- 60 FPS @1080p sustained over a 10-minute execution phase [ASSUMPTION target].

### Gameplay Metrics

- A designated no-combat chapter is winnable without firing a shot.
- A campaign remains viable after a mid-campaign battlefield defeat (defeat-conversion works).
- A player-authored doctrine deck beats a mirror AI running equivalent cards.

---

## Out of Scope

v1.0 explicitly excludes:

- Multiplayer of any kind — single-player only.
- Mobile and console platforms.
- Commercial/Steam release — personal project.
- Player-facing modding tools beyond the internal content pipeline (Epic G tools are developer-facing).

v1.0 remains single-player and non-commercial. A testnet-only blockchain provenance layer (Epic MT) is included as a technology showcase: mint claims leave the game as versioned files and an external tool anchors them to a test network. No real-money value, no multiplayer, no in-engine networking.

---

## Assumptions and Dependencies

Itemized assumptions pending confirmation or playtest:

1. Solo developer; non-commercial project.
2. Chapter count lands within 8–12; committed after E0 playtest.
3. Mouse + hotkey controls; Planning fully paused; interventions resolve ≤3s.
4. Squad doctrine sheet 3–5 slots; deck pool ~40–60 cards.
5. CP start 3 / +1 per 60s / cap 5; intervention costs 1/2/3.
6. Execution phase 5–10 min; planning ≈20–30% of chapter time; campaign ~15–20h.
7. QuantumFog certainty 0–100, decay ~10/min, 2–3 probes/battle.
8. Rout threshold: unit cohesion < 20%.
9. Governance victory: 民心 ≥ 70 and 秩序 ≥ 60 at chapter end.
10. Unit matrix stats and economy-tempo numbers are balance targets.
11. Testnet anchoring (Sepolia or local Anvil) via external relayer; dev-wallet key from environment, never committed.
11. 60 FPS @1080p on GTX 1050-class hardware.
12. Pixel sprites + ink frontispieces blend.
13. Engine (PotatoEngine) and MingGoRTS IDE exist and build; the game layer is greenfield.
14. All JSON via the engine's own parser; namespaced versioned schemas; atomic save writes.

### Open Questions

- OQ-1: Audio direction — period instrumentation vs. modern minimalist (decide at Epic F).
- OQ-2: Exact four ending voices and their ledger conditions (decide at Epic C).
