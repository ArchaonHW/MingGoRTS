---
title: "Sprint Change Proposal — Epic 12: W 行營輿圖 (Open Campaign World)"
date: 2026-10-06
project: MingGoRTS
trigger: "New requirement: open-world campaign map — predefined generals + player-created commander roam freely; battles become encounters"
status: approved (2026-10-06)
---

# Sprint Change Proposal — 2026-10-06

## 1. Issue Summary

**Trigger:** New stakeholder requirement (not a defect). The user stated: *"MingGoRTS為開放世界遊戲，玩家除了可以使用既定角色，也可以自創角色在開放世界中任意探索"* — MingGoRTS is an open-world game where players can field predefined characters or a self-created character and explore freely.

**Clarified intent (activation questions, user answers):**

- **Scope:** open *campaign* map — the battle core and chronicle framing are preserved; linear chapter progression is replaced by a freely traversable strategic map (Total War / Mount & Blade campaign-layer precedent), not a continuous-world pivot and not a bolt-on side mode.
- **Player identity:** both predefined named generals and a player-created commander exist as movable pieces on the world map.
- **Battle integration:** encounters — world movement/triggers marshal into the existing three-beat battle (Planning → Execution → Aftermath), then write results back to the world.

**Type:** New requirement emerged from stakeholders → **structural addition**. All completed work (Epics 1–5, 6.1–6.4, 10.1–10.2) is preserved; the change adds a new epic and re-scopes *backlog* stories in Epics 6–9 and one wording item in Epic 11.

## 2. Impact Analysis

### 2.1 Epic Impact

| Epic | Status | Impact | Detail |
|---|---|---|---|
| 1 (E0) | done | None | Battles unchanged; encounters marshal into the existing BattleController. |
| 2 (L) | done | Additive | World events book ledger entries exactly like battle events; add a region tag dimension (entries already carry tags). |
| 3 (B) | done | Re-scope | ChapterLibrary (3.3) gains world-bindings — additive schema fields. Chapter-shell progression (3.4) is reinterpreted: linear shell → prerequisite-graph over place-bound scenarios; predicate logic reusable, sequence semantics change. CampaignState (3.1) extends to hold WorldState — additive. **Regression-verification needed.** |
| 4 (A) | done | Additive | Accumulators stay campaign-scope folds; new per-region folds enabled by region tags on booked events. Governance victory semantics unchanged (aggregate thresholds). |
| 5 (D) | done | Additive | Infiltration is already per-region (0–3) — maps onto world regions; shrines become world POIs. Region-id scheme must unify across BattleMap/Governance/Myth. |
| 6 (C) | in-progress | Partial re-scope | 6.1–6.4 done and unaffected. **6.5–6.9 (backlog) re-scope:** trigger keys re-anchor from chapter index to world-state predicates (region held, POI resolved, ledger threshold); EndingPage gating becomes "mandatory anchored beats + ledger state" rather than chapter N. |
| 7 (E) | backlog | Moderate re-scope | No-combat chapters become location-bound world events (negotiation/deterrence encounters); mechanics survive, delivery vehicle changes. |
| 8 (F) | backlog | Grows | Adds overworld map rendering, warband movement UI, encounter banners, character-creation screen. |
| 9 (G) | backlog | Minor | Tutorial becomes world intro; sandbox gains world parameters. |
| 10 (BC) | in-progress | Additive | Unlock engine gains world-state trigger keys (region held, shrine pacified); no rework. |
| 11 (MT) | in-progress | Minor | 11.1 (in review): claim kind `chapter-settlement` → generalize to `settlement` (battle, encounter, or governance milestone). Merkle/replay gating untouched. |
| **New Epic 12 (W)** | — | Add | World map model, world state/time, warband movement, character model + creation, encounter pipeline, chapter anchoring refactor, regional bindings, world save, overworld presentation. |

### 2.2 Story Impact

- **Modified stories:** none among completed work. Backlog stories re-scoped: 6.5–6.9 (trigger-key re-anchoring), all of Epic 7 (location-bound delivery), 8.x additions, 9.5/9.6 wording.
- **Done-code rework risk:** one item — Story 3.4's linear chapter-shell progression becomes a prerequisite graph. The schema/library work is additive; the progression semantics change.
- **New stories (proposed Epic 12):**
  - 12.1 WorldMap schema & registry — `potato.world/1` region graph (nodes = regions/POIs, edges = routes), faction control flags, terrain/dual-layer tags; headless load/query via existing JsonValue registry conventions.
  - 12.2 WorldState & time — campaign-layer world model; event-driven resolution at day-scale beats (no tick sim); deterministic ordered event queue, integer math, separate seeded PRNG stream; serializes into CampaignState.
  - 12.3 Warband & movement — player warband entity (commander + attached roster squads) moving along routes; per-march 物資 supply drain; rival warbands exist as hearsay-tracked entities only (dossier convention preserved).
  - 12.4 Character model & creation — `potato.character/1` (name, origin, personality priors, starting doctrine-deck seed); predefined general roster + player-created commander; chosen character binds priors into QuantumFog/RivalDeck surfaces.
  - 12.5 Encounter pipeline — world triggers (region entry, proximity, POI) → battle assembly (map template + belligerents + stakes) → three-beat battle → aftermath writeback (control flags, ledger entries, roster, infiltration).
  - 12.6 Chapter anchoring refactor — ChapterLibrary entries gain world-bindings (region/POI/prerequisite predicates); progression = prerequisite graph over place-bound scenarios; mandatory beats become anchored setpieces.
  - 12.7 Regional governance & myth binding — ledger events carry region ids; per-region 民心/秩序 folds; world POIs bind to shrine entities.
  - 12.8 World save integration — world state joins the save schema (version bump); old-format saves reject cleanly per schema gate (no migration in v1.0).
  - 12.9 Overworld presentation & creation UI — map render, movement orders, encounter banners, character-creation screen; co-scheduled with Epic F; placeholder tiles acceptable.

### 2.3 Artifact Conflicts

| Artifact | Conflict | Required Update |
|---|---|---|
| GDD | Core gameplay loop is chapter-linear; "Level Progression: 8–12 chapters" assumes sequence; protagonist framing implies a fixed campaign shape | Edit core loop (insert world-map beat), level-design framework (place-bound scenarios + dynamic encounters), assumptions (world scale, time model), progression section |
| game-architecture.md | No world-layer module exists; Campaign dirs have no World/Characters | New decision **D-ARCH-10: campaign world layer**; directory structure adds `Campaign/World/`, `Campaign/Characters/`; system-location map rows |
| narrative-design.md | Structure type is chapter-linear 章回體 with sequential beat placement; story gating is "chapter N+1 unlocks on N"; protagonist is a faceless Chronicler with no selectable character | Structure → place-bound 回目 (chapters keyed to regions/deeds); beats re-anchor to world-state triggers; gating → prerequisite predicates; protagonist roster gains the selectable Commander （統帥) — Chronicler remains the narrative voice recording the chosen commander's deeds |
| UX designs | DESIGN.md/EXPERIENCE.md are stubs | New required surfaces: overworld map, warband orders, encounter flow, character creation — fold into UX work before Epic F |
| epics.md | FR15 defines campaign as 8–12 sequential chapters; no world/character FRs | Amend FR15 wording; add FR23 (open campaign world) + FR24 (player characters); Epic 12 section; FR coverage map; execution sequence |
| sprint-status.yaml | — | Add epic-12 + story entries as `backlog` on approval |
| Implementation stories 3.4, 6.5–6.9, 7.x | Backlog story files reference linear chapter progression | Reword trigger/gating language at story creation time (files not yet written for backlog items; 3.4 done — annotate, don't rewrite) |
| bencao-worldview.md, deferred-work.md | None | — |

### 2.4 Technical Impact

- **Determinism preserved:** the world layer is *not* a second tick sim. WorldState resolves at discrete day-scale beats via an ordered event queue — integer math, single canonical ordering, a dedicated seeded PRNG stream separate from the battle stream. NFR1 applies to battles; the world layer follows the same discipline (integer-only, seeded, canonical order) but at event granularity.
- **Layering preserved:** world model lives in `Campaign/World/` — depends on `Gameplay` public headers only for battle marshalling; encounters instantiate `BattleState` and consume `Aftermath` results. No reverse dependency.
- **Save schema:** world state joins the save document; version bump; bad/old schema rejects without mutation (existing gate).
- **Region-id unification:** one region-id scheme must serve BattleMap regions, GovernanceField tags, Myth infiltration regions, and world nodes — small design task inside 12.1.
- **Truth boundary:** world map shows rival warbands as hearsay/certainty markers (QuantumFog priors at strategic scale) — strategic intel stays uncertain; no new truth leaks.
- **In-flight protection:** Epic 6 (6.5–6.9 backlog) and Epic 7 stories should be drafted *after* 12.6's anchoring model lands, to avoid writing them twice.

## 3. Recommended Approach

**Option 1 — Direct Adjustment (selected).** Add Epic 12 at the world layer; keep all completed epics untouched; re-scope only backlog stories.

- Option 2 (Rollback): not applicable — nothing to revert; the only rework candidate (3.4 progression semantics) is cheaper to extend than to revert.
- Option 3 (MVP review): considered — the world layer could be deferred to v2.0, but the user's requirement defines the game's identity ("開放世界遊戲"); deferring it changes what v1.0 *is*. Rejected; noted as a descope escape hatch if schedule pressure demands.

**Rationale:** the design converges on a campaign-layer addition, not an architectural change — the battle kernel, ledger, narrative voice, and chain layer all survive intact. The world map is exactly the kind of "campaign-scope persistence" `Campaign/` already exists for.

**Effort:** High (a full new subsystem — the largest single epic since E0).
**Risk:** Medium (world-model design is new ground; mitigated by event-driven determinism, the file/schema boundary discipline, and all sim work being already proven).
**Timeline:** Epic 12 inserts before Epic 7's content (which must ride it) and before Epic 8's overworld surfaces; it can start in parallel with Epic 6's remaining items *if* 6.5–6.9 are held until the anchoring model exists.

## 4. Detailed Change Proposals

### 4.1 epics.md

```
MODIFIED — Functional Requirements:
- FR15: Campaign structure — 8–12 chapters via versioned ChapterLibrary;
- doctrinal progression (cards/slots unlock, veterancy); chapter shell
- progression.
+ FR15: Campaign structure — the campaign is an open strategic map of
+ regions and POIs; 8–12 authored scenarios anchor to places/predicates
+ via the versioned ChapterLibrary (place-bound 回目, non-linear order);
+ doctrinal progression (cards/slots unlock, veterancy) unchanged;
+ progression follows a prerequisite graph, not a fixed sequence.

NEW — Functional Requirements:
+ FR23: Open campaign world — persistent strategic map (potato.world/1);
+ player warband moves freely along routes; world resolves at day-scale
+ beats via ordered deterministic events; encounters marshal into the
+ three-beat battle and write back to world state; rival warbands exist
+ as hearsay-tracked entities; world events book into the ledger with
+ region tags.
+ FR24: Player characters — the campaign protagonist is chosen at start:
+ a predefined named general or a player-created commander
+ (potato.character/1: name, origin, personality priors, starting deck
+ seed); character priors bind into QuantumFog priors and the hearsay
+ record.

NEW — Epic List:
+ ### Epic 12: W — 行營輿圖 (Open Campaign World)
+ The chronicle gains a geography: the last field army marches a living
+ map of regions, shrines, and rival warbands; battles happen where the
+ player chooses to fight them; the protagonist is a named general —
+ historical or self-created — whose deeds the Chronicler records.
+ **FRs covered:** FR15 (revised), FR23, FR24

FR Coverage Map: amend FR15 row → `| FR15 | B + W | Anchored scenario
library & prerequisite-graph progression |`; append:
+ | FR23 | W | World map, warband movement, encounters, regional bindings |
+ | FR24 | W | Character model, creation, priors binding |

Execution sequence:
- … → C → E → F → G → MT
+ … → C → W → E → F → G → MT   (W may parallel C's remaining items;
+ E's content and F's overworld surfaces land after W's anchoring model)
```

### 4.2 GDD

```
Section "Core Gameplay Loop" — insert between steps 3 and 4:
+ 3.5. **World map** (day-scale) — the warband marches between regions,
+ chooses engagements, visits shrines and refit points; encounters
+ (field battles, no-combat confrontations, anchored scenarios) open the
+ three-beat battle where they occur.

Section "Level Design Framework" — rewrite Level Progression:
- 8–12 chapters … via a chapter library. Early: doctrine tutorials …
+ The campaign world is a persistent map of regions and POIs. 8–12
+ authored scenarios (回目) anchor to places and prerequisite predicates
+ — order is the player's march, not a fixed list. Mandatory beats
+ (commission, midpoint pivot, final confrontation) anchor to fixed
+ locations or ledger thresholds. Dynamic encounters fill the space
+ between anchored scenarios; defeat on the map converts to governance
+ play as before.

Section "Core Concept" — append:
+ The campaign is crossed on foot: an open strategic map where the
+ player's commander — a chosen historical general or a self-created
+ one — marches a warband, and every battle begins where the army
+ stands.

Section "Player Progression" — append:
+ The protagonist's identity is itself a choice: predefined generals
+ carry authored priors; a created commander is defined by name, origin,
+ personality priors, and a starting doctrine-deck seed.

Assumptions — append:
+ 15. World map scale ~20–40 regions/POIs; time resolves at day-scale
+    beats; world layer is event-driven (no second tick sim).
+ 16. Character creation fields committed at Epic 12 spec time; priors
+    reuse the existing personality-prior machinery.

Open Questions — append:
+ OQ-3: Whether the player may field multiple named generals as
+   subordinate warbands (multi-stack) or commands a single warband —
+   default single-warband for v1.0 unless design review says otherwise.
```

### 4.3 game-architecture.md

```
Decision Summary — append:
+ | D-ARCH-10 | Campaign world layer | `Campaign/World/` — event-driven
+   world model (WorldMap, WorldState, Warband, EncounterPipeline);
+   day-scale beats resolve via ordered event queue, integer math,
+   dedicated seeded PRNG | No second tick sim; encounters marshal into
+   Gameplay battles and write back at aftermath; keeps NFR1 intact and
+   world logic inside the persistence layer it mutates |

Directory Structure — under Campaign/ append:
+ │   ├── World/                 # WorldMap (potato.world/1), WorldState,
+ │   │                          # Warband movement, EncounterPipeline
+ │   ├── Characters/            # potato.character/1 model + creation data

System Location Mapping — append rows:
+ | World map / warband / encounters | `Campaign/World` | day-scale
+   beats; no tick sim |
+ | Character model & creation | `Campaign/Characters` | priors bind to
+   Fog priors + Rivals |

Consistency Rules — append:
+ | World determinism | ordered event queue, integer math, dedicated
+   seeded stream | same review discipline as sim |
```

### 4.4 narrative-design.md

```
Section "Structure Type" — revise:
- Episodic (chapter-form 章回體) with branching endings
+ Episodic (chapter-form 章回體) with branching endings — the 回目 are
+ place-bound: each chapter is a region/deed on the campaign map, its
+ order the player's march rather than a fixed index.

Section "Characters / Protagonists" — add:
+ #### The Commander（統帥）
+ The diegetic protagonist — a predefined named general or the player's
+ self-created commander (name, origin, priors, starting deck). The
+ Chronicler remains the voice; the Commander is whose war the chronicle
+ records. Authorship stays with the player: the Commander is the hand
+ on the map, the Chronicler the hand on the page.

Section "Story Gating" — revise first paragraph:
- Chapter sequence is a hard gate (戰役 chronological); inside chapters…
+ Scenarios are gated by world-state predicates (region control, POI
+ resolution, ledger thresholds) — the prerequisite graph replaces the
+ hard chapter sequence; mandatory beats anchor to fixed places or
+ ledger states.

Section "Story Beats / Beat Placement" — annotate:
+ Beats 1–4 anchor to the starting region (commission). Beat 8 anchors
+ to a ledger-threshold trigger anywhere on the map. Beats 11–14 anchor
+ to late-campaign regions/conditions. All other beats bind to
+ world-state triggers at Epic 12 spec time.
```

### 4.5 sprint-status.yaml (on approval)

```yaml
+ epic-12: backlog
+ 12-1-worldmap-schema-registry: backlog
+ 12-2-worldstate-time: backlog
+ 12-3-warband-movement: backlog
+ 12-4-character-model-creation: backlog
+ 12-5-encounter-pipeline: backlog
+ 12-6-chapter-anchoring-refactor: backlog
+ 12-7-regional-governance-myth-binding: backlog
+ 12-8-world-save-integration: backlog
+ 12-9-overworld-presentation-creation-ui: backlog
+ epic-12-retrospective: optional
```

## 5. Implementation Handoff

**Scope classification: Major** — a full new epic plus backlog reorganization plus a narrative-structure revision; all completed code preserved.

**Handoff:**
1. This proposal approved → apply §4 edits (epics.md, gdd.md, game-architecture.md, narrative-design.md, sprint-status.yaml).
2. **Hold drafting of stories 6.5–6.9 and all Epic 7 stories** until Story 12.6's anchoring model lands — they re-anchor onto it.
3. Epic 11.1 (done 2026-10-06): the `potato.mintclaim/1` claim-kind vocabulary shipped with `chapter-settlement`; generalize it to `settlement` kinds (battle/encounter/governance) when Epic 12.5's encounter pipeline emits claims — the shipped value stays valid, so this is an additive enum extension, not a schema break.
4. `gds-create-story` per Epic 12 story when it enters the queue; UX surfaces (overworld map, character creation) must land in the UX docs before Epic F.
5. OQ-3 (multi-stack warbands) defaults to single-warband for v1.0.

**Success criteria:**
- World map loads headless via `potato.world/1`; warband movement resolves deterministically (same seed → same world state) on MSVC and MinGW.
- An encounter marshals into the existing three-beat battle and its aftermath writes back: region control, ledger entries, roster, infiltration — all visible in a reloaded save.
- A custom-created commander (`potato.character/1`) binds priors into QuantumFog and appears in the hearsay record; a predefined general is selectable equivalently.
- All previously green `potato_test_*` suites remain green; the only changed done-story is 3.4's progression semantics (extended, not broken).
