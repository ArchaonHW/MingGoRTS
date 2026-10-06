---
title: "Addendum — Game Brief: 民國史詩 RTS"
status: reference
created: 2026-09-17
---

# Addendum: 民國史詩 RTS

Depth captured during ideation that belongs to downstream documents (GDD, PRD, architecture) — not lost, just parked here. Full source: `_bmad-output/brainstorming-session-2026-09-16.md` (70 ideas, numbered).

## Deferred design depth (→ GDD input)

- **Numeric curves** (session §數值曲線): doctrine slot growth 4→7 across chapters, CP economy (base 3, +1 per 5 commander levels, no in-battle regen, morale-for-CP dark option), per-squad morale bands (>100 = over-execution buff, <30 = 70% execution, <10 = 40%), intel pricing & degradation vs. repeat purchases, battle-length pressure (>4 min mutual morale decay).
- **Failure-state rules** (session §失敗設計): L0–L5 failure ladder with per-tier narrative unlocks; roster-as-balance-sheet; corpse-recovery decisions; morale-debt spiral with bankruptcy protection (rest camp events); enemy salvage of abandoned equipment; kinship-grief propagation; shrine/memorial with last-10-seconds replays.
- **Enemy economy** (session §敵方經濟): enemy resources = manpower / forage / command-will; on-map attackable assets (grain carts, relay stations, couriers); command-will depletion forces enemy back to pure-script predictability; inter-battle economic attrition (burning supply trains persists).
- **Tutorial beats** (session §教學關): teach "regret" not syntax — pre-filled deck wins half a battle then visibly fails at an uncovered flank; vocabulary taught by unit-type not grammar; recording-mode crutch produces intentionally inferior cards; intel taught via 15-second enemy-camp vignette; scripted unavoidable death of squad leader "老周" inaugurates the roster.
- **Audio identity** (session §音樂音效): doctrine-as-score (each card = a motif; battle music is assembled live from fired cards), enemy commander musical signatures, audible morale, historian's-pen sound design for the aftermath, muffled "soul-leaving" mix during CP intervention, audio intel layer (startled birds = ambushers moving).
- **Diplomacy layer** (session §外交層 — post-MVP): allied commanders as readable third-party doctrines; pre-battle lobbying spends favor points to edit ally scripts; hireable third parties (bandits, militia) as one-shot purchased doctrines; enemy command-chain fissures as attack surface; prisoner/disposition decisions; superiors impose mandatory constraint cards on the player.
- **UI forms** (session §Doctrine UI): card-slot row, timeline view, on-map behavior pins, hybrid map+card; recording-to-doctrine transcription; replay→editor zero-friction jump; paper-doll squad loadout (unit type determines available doctrine vocabulary); intel probability-cloud visualization.
- **World-skin candidates** (8 retained): banner-general, necromancer-scripter, golem-engineer, disgraced general, war-god's voice, puppeteer, gods' chessboard, echo-battlefield — doctrine is tech-agnostic so skins are reskinnable; Republican-era setting (selected) absorbs the staff-officer variant.

## Parked / rejected

- Full 4X diplomacy — rejected as scope; commander-level interpersona layer kept instead.
- Per-unit pathfinding — rejected; squad-level flow fields only (engine-fit decision).
- Deterministic replay via re-simulation — rejected; event-recording replay chosen (float physics is non-deterministic).

## Technical constraints & engine mapping (→ architecture input)

- PotatoEngine C++20, CMake, MSVC+MinGW; EventBus → doctrine triggers; ECS → squads; Physics → terrain interaction; TimeManager → phase machine; Serialization → roster/saves.
- Implementation task list T-1–T-12 in session file §斷橋原型→引擎任務清單; `Gameplay/` (Doctrine, BattleController, BattlePlanner) already exists with CAP-1–CAP-8 verified in `DoctrineBattleDemo`.
