---
title: "Game Brief: 民國史詩 RTS (Republican-Era Epic, working title)"
status: final
created: 2026-09-17
updated: 2026-09-17
---

# Game Brief: 民國史詩 RTS (working title)

## Executive Summary

A single-player tactics RPG-RTS hybrid set in Republican-era China (warlord era → Northern Expedition → War of Resistance → Civil War) where you never click units in battle — you *write* them. Before each engagement you compose a small hand of doctrine cards (trigger → condition → action), deploy your named squads, then watch your script execute in real time, spending scarce Command Points to seize a squad and intervene for a few seconds when the plan breaks. Afterwards: a historian's after-action report, a roster of the dead, and a replay that shows exactly which card failed and when — so the next draft of your battle plan is born from the last one's corpse.

Enemy commanders run on the same doctrine language you do. Every named enemy is a readable, decipherable deck of habits — scout them, counter-write them, defeat them, and loot their doctrine. The campaign is a book of commander biographies fought on two layers: history above (railways, fords, grain convoys, field telephones) and myth beneath (fox spirits, war gods, local deities, the ghosts of unavenged dead) — where what you change in myth rewrites what history hands you next.

The game is built on PotatoEngine (C++20, custom engine) — the doctrine interpreter, squad AI, and battle phase machine already have working prototypes (`Gameplay/` + `DoctrineBattleDemo`).

## Vision

**Core fantasy (one sentence):** You are the staff officer who wins the battle the night before — in the war council, over a map, not on the field.

Elevator pitch: *FF12's Gambit system grown into a full game — a tactics RPG where your loadout is a script, your losses have names, and your enemies are puzzles written in your own language.*

Players walk away with the three-beat emotional loop: **command** (the confidence of a plan well written), **suspense** (watching it collide with an enemy's plan, powerless except for a few seconds of intervention), and **mourning** (the names on the roster, the historian's prose, the decision of whether to risk more lives to recover the dead).

## Target Players & Market

Four overlapping audiences — tactics players and scripting players are co-primary, narrative and showcase are supporting:

1. **Tactics/strategy players who dislike APM pressure** — FFT, Total War, and wargame fans who want commander's decisions without twitch execution. [Co-primary]
2. **Systems/programming-battle players** — Opus Magnum and FF12 Gambit fans who find "write the script, watch it run" intrinsically delightful. [Co-primary]
3. **Narrative RPG players** — drawn by the named-casualty roster, the historian's-pen after-action reports, and the history×myth commander biographies. [Secondary]
4. **Engine showcase / personal audience** — the project doubles as a PotatoEngine capability demonstration. [Secondary]

**Market context:** The "pre-battle scripting" niche is validated but underserved — Gambit-style systems are beloved as *features* (FF12) or *puzzle games* (Opus Magnum), but no major title makes doctrine-writing the core of a full narrative campaign. Party-mode review flagged the real risk: audiences 1 and 2 overlap less than assumed, so onboarding must prove a non-programmer RPG player can field a battle in 10 minutes.

## Core Fundamentals

**Genre:** Single-player tactics RPG × auto-battle RTS. Turn-layer pre-battle planning → real-time execution → turn-layer aftermath.

**Core loop:**
```
INTEL → compose doctrine cards → deploy squads & pins
   → WATCH script execute (enemy commander's script runs too)
   → burn CP for 8-second hero intervention when it breaks
   → aftermath: roster of the dead, historian report, replay → revise cards
```

**Pillars:**

1. **Writing IS fighting** — doctrine is the only verb that matters; the UI makes scripting feel like arranging units, not coding. Party gate: a non-programmer must field a battle in 10 minutes.
2. **Loss is content** — named casualties, corpse-recovery decisions, looted enemy doctrine; failure branches (L0–L5 ladder) rather than binary game-over.
3. **Enemies are readable decks** — enemy commanders use the same doctrine language; their personalities are observable signatures you scout, counter-write, and loot.
4. **History × Myth, Republican era** — every battle exists on two layers sharing one map: history (warlord columns, grain convoys, telegraph lines) above, myth (local gods, fox spirits, vengeful ghosts) beneath; myth-layer actions rewrite the history layer's next battle.

**Key systems:** doctrine interpreter (trigger/condition/action/modifier + cooldown), squad-level orders (no per-unit pathing — flow fields), enemy economy (manpower/forage/command-will — all attackable on-map assets), intelligence-as-terrain (probability-cloud fog), per-squad morale driving doctrine-execution rate (weighted global average), event-recorded replay (not deterministic re-sim).

## References & Differentiation

| Title | Take | Leave |
|---|---|---|
| **FF12 Gambits** | Script-your-squad combat as the whole game; enemy scripts in the same language | Pure stat-check farming battles; gambits as an optional subsystem |
| **Total War** | Battle-scale spectacle, morale system, named commander identities | Real-time micro requirement; massive armies you cannot meaningfully command |
| **The Last Spell** | Prepare-then-defend rhythm; preparation IS the game | Roguelite randomness, wave defense |
| **Opus Magnum** | Mechanism-as-toy: watchable, shareable, exportable solutions | Pure puzzle framing with no living enemy |

**Differentiators:** (1) Doctrine-writing as the core verb of a *narrative campaign*, not a puzzle or subsystem. (2) Enemy commanders as hand-written, lootable doctrine decks — scouting reads their script, not their stats. (3) Named-casualty roster + historian's-pen reports make loss mechanically productive and emotionally load-bearing. (4) History×myth dual-layer battles with cross-layer causality.

## Scope & MVP

- **Platform:** PC first; console/handheld only if the game proves out.
- **Team:** Solo developer, full-time. Custom engine (PotatoEngine, C++20) — engine-side gameplay foundations already prototyped.
- **MVP = "Broken Bridge" (斷橋):** one medium map (broken bridge, two fords, ambush forest, high ground), 4 player squads + hero, one enemy commander (Grok the Reckless, aggression 90/discipline 40/cunning 10, two signature habits), 4 doctrine slots, 10 intel points, 3 CP, ~90 s–4 min battles. **Deliberately excluded:** campaign map, multi-commander battles, doctrine chains, enemy learning, sandbox simulator, equipment/progression.
- **MVP acceptance:** the write→watch→revise loop is fun in isolation; a non-programmer fields a battle in ≤10 min; enemy signature habits are observable and exploitable.
- **Vertical slice** (post-MVP): one full commander-biography chapter with history×myth dual layer.

## Content & Direction

- **World:** Republican-era China (民國史詩) — warlord era through Civil War as the history layer; Chinese folk mythology (local gods, fox spirits, war-god possessions, ghost legions) as the myth layer. Campaign structured as commander biographies (Architecture C skeleton + B dual-layer mechanics + A inter-chapter causality): ~15 chapters tracing the era's arc. Commanders will be fictional or thinly-veiled composites rather than direct portrayals of real figures.
- **Narrative:** Historian's-pen framing — after-action reports render as chronicle prose; the roster of the dead IS the story the player writes.
- **Content breadth:** MVP is one battle; campaign vision is ~15 chapters of named commander biographies with escalating doctrine vocabulary (4→7 slots).
- **Art:** Prototype uses capsule/color-block units; vertical slice needs readable battlefield + doctrine-card UI. Audio: doctrine-triggered musical motifs (battle score is assembled live from fired cards), morale audible as army noise, CP intervention as muffled "soul-leaving" soundscape.

## Risks & Open Questions

**Top risks (from Party Mode review):**
1. **Audience overlap** — scripting players ∩ tactics/RPG players may be small. *Mitigation: tutorial teaches "regret" not syntax (pre-filled deck fails visibly, player patches the hole); 10-minute non-programmer acceptance gate.*
2. **Enemy doctrine authoring cost** — enemy-AI parity means hand-writing every commander's deck. *Mitigation: accepted — commanders are designed puzzles; personality axes only modulate weighting.*
3. **Feedback latency** — write→watch→revise must cycle fast. *Mitigation: sandbox simulator is a requirement, not a luxury; real-time layer gets 4× speed.*

**Resolved in session:** emergency-stop button (exists, costs 1 CP), per-squad morale (independent + weighted global), doctrine cooldowns (implemented, CAP-8).

**Open questions for GDD:** 4 vs. 5 starting doctrine slots (needs playtest); hero squad's auto-behavior during manual control; which Republican-era theaters and myth cycles anchor the ~15 chapters; degree of historical fidelity vs. fictionalization of commanders.
