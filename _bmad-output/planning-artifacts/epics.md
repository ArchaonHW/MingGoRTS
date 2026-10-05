---
stepsCompleted: [1, 2, 3, 4]
inputDocuments:
  - '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md'
  - '_bmad-output/game-architecture.md'
  - '_bmad-output/narrative-design.md'
---

# MingGoRTS - Epic Breakdown

## Overview

This document provides the complete epic and story breakdown for MingGoRTS, decomposing the requirements from the GDD, Architecture, and Narrative Design into implementable stories. It supersedes the draft at `gdds/gdd-MingGoRTS-2026-09-29/epics.md`.

## Requirements Inventory

### Functional Requirements

FR1: Doctrine card system — cards as trigger→condition→action→modifier; squad doctrine sheets of 3–5 slots; deck pool ~40–60 cards; per-card cooldowns 5–60s.
FR2: Three-beat battle state machine — Planning (untimed, paused) → Execution (real-time 5–10 min) → Aftermath.
FR3: Command Points — start 3, +1/60s, cap 5; interventions: redirect 1 CP, card override 2 CP, retreat 3 CP; resolve ≤3s.
FR4: QuantumFog — probabilistic intel: 0–100 certainty field, observation collapse, probes (~2–3/battle), decay ~10/min, entanglement, personality-prior bias.
FR5: BattlePlan — plan arrows with execution bonus scaled by fog certainty over covered ground.
FR6: Roster & RefitCamp — named squads, persistent casualties; between-chapter deploy/heal/recruit/plunder spending 物資.
FR7: Ledger — five accounts (武功/民心/天命/軍威/物資) double-entry; derived accumulators 民心/秩序/墮落 (墮落 ratchet monotonic); append-only tamper-evident chain; forgery injection channel; suspect flags persisted into reports.
FR8: GovernanceField — village occupation/burning, convoy escort/raid booking into campaign accumulators; governance victory via 民心≥70 & 秩序≥60 at chapter end.
FR9: Myth dual-layer — per-region infiltration state machine 0–3, capturable shrines, 天命 currency, myth actions (pacify/possess/ghost armies), invasion events at peak infiltration, GodStance, MythLog.
FR10: RivalDeck — cross-chapter learning of player's habitual triggers; pre-written counter-decks; enemy generals via hearsay dossiers only.
FR11: Symmetric AI — identical doctrine machinery and CP rules; personality priors; no stat cheating.
FR12: BattleRecorder — recorded replayable battles, record-is-truth, tamper-evident integrity seal, downgrade warning.
FR13: Defeat conversion — battle loss routes into governance/recovery play; no single-battle game over.
FR14: Victory paths — military rout (<20% cohesion), governance thresholds, subversion (defection/deterrence/negotiation); designated zero-combat chapters; four-voice ending from final ledger state.
FR15: Campaign structure — 8–12 chapters via versioned ChapterLibrary; doctrinal progression (cards/slots unlock, veterancy); chapter shell progression.
FR16: Narrative systems — HistorianReport with omission counters, ChapterConventions (題詞/判詞/欲知後事/四聲部), IntelLedger distortion, NarrativePack bundles, GeneralDossier hearsay register.
FR17: Document-voice systems — Scribe marginalia (authenticity-flagged), rival defection arcs (Cautious via governance record, Nemesis late-campaign), ledger/GodStance-driven document variant rendering, audit spread (Judgment of the Brush), four-voice folios.
FR18: Presentation — pixel sprite atlas (units/terrain/myth), HUD (doctrine UI, CP bar, density scaling), CJK text pipeline, chapter frontispieces (冊頁), audio (direction deferred OQ-1).
FR19: Content & tools — card pool expansion+balance, SquadTemplate budgeted builds with skip reasons, deck plundering, developer-facing enemy editor, sandbox mode, tutorial chapter.
FR20: Versioned content pipeline — all game data as `potato.<name>/<ver>` JSON via game-layer JsonValue DOM parser; boot-time registries.

### NonFunctional Requirements

NFR1: Determinism — fixed 20 Hz tick; integer/fixed-point only in sim (no floats); single seeded PRNG stream in canonical draw order; replay reproduces recorded outcome bit-exact across MSVC/MinGW.
NFR2: Headless gameplay layer — `Gameplay/` compiles and tests without GL/Rendering/GUI/Platform headers.
NFR3: Performance — 60 FPS @1080p on mid-range PC during 10-min execution with full QuantumFog.
NFR4: Atomic saves — tmp→rename; invalid schema/version rejected without mutating existing state.
NFR5: Build matrix — MSVC + MinGW + Linux; headless test suite green; `POTATO_TESTS` integration.
NFR6: Layered dependencies — Engine ← Gameplay ← Campaign ← Game/Examples; single direction only.
NFR7: No third-party JSON libraries; no unsafe C functions (gets/strcpy/strcat/sprintf/vsprintf/scanf).
NFR8: Canonical evaluation order (squad idx → slot idx); RNG owned by sim; truth boundary (nothing outside Sim reads true enemy state).
NFR9: Registries immutable during battle; zero file I/O in tick path.
NFR10: Event regime separation — typed SimEvent list inside sim (recorded); EventBus only at boundaries; subscribers never write BattleState.

### Additional Requirements

- No starter template — E0-1 scaffolds `Gameplay/` module, CMake target, and headless harness from scratch.
- `Result<T>` boundary type for all I/O/content loads; sim internals return status enums, never throw.
- `POTATO_DEBUG` compile gate for debug tooling (truth view, overlays); replay-verifier CLI.
- Test executables named `potato_test_<name>`; headless sim driver shares production code path.
- Balance values live in versioned JSON (`potato.balance/<ver>`), never hardcoded in sim constants.
- Naming: PascalCase files/classes/methods; camelCase members; namespaces `Potato::Gameplay`/`Potato::Campaign`.
- Directory layout per architecture: `Gameplay/{Sim,Map,Squad,Doctrine,Fog,Plan,Command,Record,Governance,Json}`, `Campaign/{State,Chapters,Roster,Ledger,Governance,Rivals,Narrative,Myth,Save}`, `Game/{Render,Ui}`, `assets/`.

### UX Design Requirements

N/A — no UX design document exists; F-epic will carry UI requirements.

### FR Coverage Map

| FR | Epic | Note |
|---|---|---|
| FR1 | E0 | Doctrine cards, sheets, cooldowns |
| FR2 | E0 | Three-beat battle |
| FR3 | E0 | CP interventions |
| FR4 | E0 | QuantumFog |
| FR5 | E0 | BattlePlan arrows |
| FR6 | B | Roster & RefitCamp |
| FR7 | L | Ledger, chain, forgery |
| FR8 | A | GovernanceField & victory |
| FR9 | D | Myth dual-layer |
| FR10 | B | RivalDeck learning |
| FR11 | E0 | Symmetric AI |
| FR12 | E0 | BattleRecorder |
| FR13 | A | Defeat conversion |
| FR14 | A + E | Military/governance paths → A; subversion paths → E |
| FR15 | B | Chapter library & progression |
| FR16 | C | Narrative systems |
| FR17 | C + E | Document voices → C; defection mechanics → E |
| FR18 | F | Presentation |
| FR19 | G | Content & tools |
| FR20 | E0 | Versioned JSON pipeline & JsonValue |

## Epic List

### Epic 1: E0 — Battle Core
The headless vertical slice: a single battle runs Planning → Execution → Aftermath with doctrine scripts on both sides, QuantumFog intel, CP intervention, and a replayable record. Nothing else compiles against gameplay until this lands.
**FRs covered:** FR1, FR2, FR3, FR4, FR5, FR11, FR12, FR20

### Epic 2: L — Ledger Mechanization
The five-account double-entry ledger becomes real gameplay: balanced postings, tamper-evident append-only chain, forgery injection and suspect flags, audit-aware reports.
**FRs covered:** FR7

### Epic 3: B — Campaign Persistence
The campaign becomes durable: versioned saves, a chapter library, persistent named roster with between-chapter refit, and rival-deck learning that carries across chapters.
**FRs covered:** FR6, FR10, FR15

### Epic 4: A — Governance Axis
Governance becomes a win condition: battlefield deeds book into 民心/秩序/墮落 accumulators, governance victories resolve chapters, and defeats convert into recovery play.
**FRs covered:** FR8, FR13, FR14 (military/governance paths)

### Epic 5: D — Myth Dual-Layer
The second layer of the map comes alive: region infiltration 0–3, capturable shrines, 天命 currency, myth actions, invasion events, GodStance, MythLog.
**FRs covered:** FR9

### Epic 6: C — Narrative Systems
The chronicle speaks: historian reports with omission counters, chapter conventions, hearsay dossiers, Scribe marginalia, document-variant rendering by ledger state, the audit spread, and four-voice endings.
**FRs covered:** FR16, FR17 (document voices)

### Epic 7: E — No-Combat Chapters
The subversion pillar becomes playable: negotiation, deterrence, defection mechanics (including rival defection arcs), and designated zero-combat chapters.
**FRs covered:** FR14 (subversion paths), FR17 (defection mechanics)

### Epic 8: F — Presentation
The game becomes visible and audible: sprite atlas, HUD, CJK pipeline, frontispieces, audio.
**FRs covered:** FR18

### Epic 9: G — Content & Tools
Content breadth and developer tooling: card pool expansion, squad templates, deck plundering, enemy editor, sandbox, tutorial chapter.
**FRs covered:** FR19

**Execution sequence:** E0 → L → B → A → D → C → E → F → G (F and G may partially parallel once E0 lands).

---

## Epic 1: E0 — Battle Core

The headless vertical slice: a single deterministic battle runs Planning → Execution → Aftermath with doctrine scripts on both sides. Foundation for everything.

### Story 1.1: Gameplay Module Skeleton

As a developer,
I want a Gameplay/ module with CMake target, headless test harness, and layered dependency guards,
So that all game code has a deterministic, GL-free home.

**Acceptance Criteria:**

**Given** a clean checkout,
**When** potato_test_gameplay builds and runs,
**Then** it links only engine leaf modules (Events, FileSystem, Logging, MathUtils) and passes a smoke tick
**And** Gameplay/ sources compile with no Rendering/GUI/Platform-GL includes (verified by dependency check).

### Story 1.2: JsonValue DOM Parser & Schema Guard

As a developer,
I want a game-layer JSON DOM parser with potato.<name>/<ver> schema gating and Result<T> returns,
So that all content files load through one validated path.

**Acceptance Criteria:**

**Given** a well-formed potato.test/1 file,
**When** loaded via Json::load,
**Then** a typed value returns.
**And Given** a bad schema tag or malformed JSON,
**When** loaded,
**Then** Result carries an error reason and existing state is untouched.

### Story 1.3: BattleMap Model

As a developer,
I want a tile/region battlefield model with terrain flags and strategic points (ferries, depots, villages),
So that battles have ground to fight over.

**Acceptance Criteria:**

**Given** a potato.map/1 file,
**When** loaded into BattleMap,
**Then** regions, adjacency, and strategic-point flags are queryable
**And** the map carries dual-layer marks (historical + shrine) from the start.

### Story 1.4: Squad Model

As a developer,
I want named squads with stats, position/movement, and cohesion state on a rout threshold,
So that the battle has actors.

**Acceptance Criteria:**

**Given** squads instantiated from potato.squad/1 templates,
**When** the sim ticks,
**Then** squads move per speed stat and cohesion updates from combat events
**And** cohesion < 20% produces a rout transition in the state table.

### Story 1.5: Doctrine Interpreter

As a player,
I want my doctrine cards to execute trigger→condition→action per tick in canonical order,
So that what I write is what happens.

**Acceptance Criteria:**

**Given** a squad sheet of 3–5 slotted cards,
**When** a tick evaluates,
**Then** slots evaluate in (squad idx → slot idx) order against a tick-start snapshot, actions write pending deltas applied post-evaluation, and cooldowns tick down
**And** two identical seeds+sheets produce identical event streams.

### Story 1.6: BattleController Three-Beat Loop

As a player,
I want a Planning→Execution→Aftermath state machine on a fixed 20 Hz tick,
So that battles have defined phases.

**Acceptance Criteria:**

**Given** a BattleState,
**When** driven headless,
**Then** Planning accepts input without ticking, Execution ticks deterministically, Aftermath emits results
**And** illegal transitions (Planning→Aftermath) are rejected by the transition table.

### Story 1.7: CP Interventions

As a player,
I want to spend Command Points mid-execution to redirect squads, override cards, or order retreats,
So that I can save a collapsing plan.

**Acceptance Criteria:**

**Given** Execution running with CP pool (start 3, +1/60s, cap 5),
**When** I issue redirect (1) / override (2) / retreat (3),
**Then** the intervention resolves within ≤3s of issue and CP deducts
**And** insufficient CP rejects the command with a reason.

### Story 1.8: QuantumFog

As a player,
I want enemy positions represented as certainty gradients I can sharpen, collapse, and lose to decay,
So that intel is a resource, not a toggle.

**Acceptance Criteria:**

**Given** a CertaintyField (0–100) and CloudEntity set,
**When** observation/probe/decay/entangle ops run per tick,
**Then** certainty updates per balance JSON (~10/min decay), collapse resolves clouds to truth
**And** no code outside Gameplay/Sim can read true enemy positions (truth boundary enforced).

### Story 1.9: BattlePlan Arrows

As a player,
I want drawn plan arrows whose execution bonus scales with fog certainty over covered ground,
So that better intel makes better plans.

**Acceptance Criteria:**

**Given** a planned arrow over map regions,
**When** execution begins,
**Then** the bonus equals f(mean certainty of covered regions) per balance JSON
**And** replans mid-execution cost CP per the command rules.

### Story 1.10: Symmetric AI Opponent

As a player,
I want the enemy to run the same doctrine interpreter and CP rules under personality priors,
So that victories are earned, not stat-inflated.

**Acceptance Criteria:**

**Given** an AI side with a squad sheet and a prior (aggressive/defensive/cunning),
**When** the battle ticks,
**Then** AI cards evaluate through the identical interpreter entry point and CP pool
**And** priors bias card selection and fog-cloud shapes, never stats.

### Story 1.11: BattleRecorder & Replay Verifier

As a player,
I want every battle recorded as an event stream that replays to the identical outcome,
So that the record is the truth.

**Acceptance Criteria:**

**Given** a completed battle's recorded stream + seed,
**When** the replay-verifier CLI re-runs it,
**Then** outcome hash matches bit-exact
**And** a tampered stream fails integrity verification and is rejected.

### Story 1.12: Win Evaluation

As a player,
I want battles to resolve military victory/defeat correctly,
So that chapters can end.

**Acceptance Criteria:**

**Given** Execution running,
**When** one side's squads all rout/annihilate,
**Then** Aftermath emits a BattleResult (victor, casualties, ledger-event list)
**And** a stalemate timer produces a draw evaluation per rules.

---

## Epic 2: L — Ledger Mechanization

The five-account double-entry ledger becomes real gameplay.

### Story 2.1: Five-Account Double-Entry Ledger

As a system,
I want every action to post a balanced entry pair across 武功/民心/天命/軍威/物資，
So that nothing is free.

**Acceptance Criteria:**

**Given** a game event (e.g., village burned: +物資， −民心）,
**When** posted,
**Then** both halves of the pair record atomically
**And** an unbalanced posting is rejected.

### Story 2.2: Append-Only Tamper-Evident Chain

As a system,
I want entries chained by hashing the previous entry, with break detection,
So that history can't be quietly rewritten.

**Acceptance Criteria:**

**Given** a ledger chain,
**When** erify() replays it,
**Then** a mutated historical entry fails verification and load is rejected
**And** accumulators compute by folding entries — no public write API exists.

### Story 2.3: Forgery & Suspicion Channels

As a designer,
I want explicit forgery injection and suspect-flag mechanics,
So that dishonest history is content, not corruption.

**Acceptance Criteria:**

**Given** the ForgeryChannel write path,
**When** an enemy injects a forged entry,
**Then** the entry persists with detectable provenance and a suspect flag can be set
**And** suspect flags surface in later reports.

### Story 2.4: Audit-Aware Reports

As a player,
I want chapter reports to include omission counters and ledger audit segments,
So that the chronicle confesses what it hides.

**Acceptance Criteria:**

**Given** a resolved chapter,
**When** the HistorianReport renders,
**Then** it always includes the omission counter (「本報告省略 N 項」)
**And** audit segments reflect ledger integrity state.

### Story 2.5: Replay-Ledger Cross-Check

As a system,
I want battle archive integrity cross-checked against ledger chain consistency,
So that the two histories can't disagree silently.

**Acceptance Criteria:**

**Given** a replay stream and the ledger chain covering it,
**When** cross-checked,
**Then** mismatch flags the chapter state as suspect
**And** a consistent pair verifies clean.

---

## Epic 3: B — Campaign Persistence

The campaign becomes durable across chapters.

### Story 3.1: CampaignState Facade

As a developer,
I want a CampaignState facade aggregating chapter, roster, and ledger stores under one versioned schema,
So that save/load is one operation.

**Acceptance Criteria:**

**Given** mid-campaign state,
**When** serialized to potato.campaign/1 JSON,
**Then** it round-trips losslessly
**And** it depends only on public Gameplay headers.

### Story 3.2: Atomic Save System

As a player,
I want saves written atomically and bad files rejected without touching my game,
So that a crash never eats my campaign.

**Acceptance Criteria:**

**Given** a save slot write,
**When** interrupted mid-write,
**Then** the previous save remains valid (tmp→rename)
**And Given** a file with bad schema/version,
**When** loaded,
**Then** it is rejected with reason and current state is unmodified.

### Story 3.3: ChapterLibrary

As a developer,
I want versioned chapter definition packs (potato.chapter/1) loaded into a boot-time registry,
So that chapters are content, not code.

**Acceptance Criteria:**

**Given** chapter JSON files in ssets/chapters/,
**When** the campaign boots,
**Then** all chapters register immutably
**And** a bad version rejects that file without failing the library.

### Story 3.4: Chapter Shell & Progression

As a player,
I want chapter progression with unlock rules over a campaign map,
So that victories move me forward.

**Acceptance Criteria:**

**Given** a resolved chapter (win or converted defeat),
**When** the campaign advances,
**Then** the next chapter unlocks and carries forward starting conditions (roster, ledger, intel).

### Story 3.5: Persistent Roster

As a player,
I want named squads whose casualties, veterancy, and scars persist,
So that survival means something.

**Acceptance Criteria:**

**Given** battle aftermath with casualties,
**When** applied to the roster,
**Then** dead members persist as dead, survivors gain veterancy
**And** the roster serializes inside CampaignState.

### Story 3.6: RefitCamp

As a player,
I want between-chapter actions — deploy/heal/recruit/plunder — spending 物資,
So that I can rebuild between battles.

**Acceptance Criteria:**

**Given** a RefitCamp phase with a 物資 balance,
**When** I heal (~20–40) or recruit (~40–80) per economy band,
**Then** costs deduct via ledger posting and the roster updates
**And** overspending is rejected with a skip reason.

### Story 3.7: RivalDeck Learning

As a player,
I want enemy generals to learn my habitual triggers across chapters and field counter-decks,
So that predictability is my hardest enemy.

**Acceptance Criteria:**

**Given** three chapters of recorded player doctrine usage,
**When** a rival prepares chapter N+1,
**Then** its deck includes counter-cards biased against my most-used triggers
**And** learning data lives in Campaign/Rivals, difficulty scaling via counter-deck depth only.

---

## Epic 4: A — Governance Axis

Governance becomes a win condition.

### Story 4.1: GovernanceField Battle Events

As a system,
I want per-battle tracking of village occupation/burning and convoy escort/raid,
So that field conduct has campaign weight.

**Acceptance Criteria:**

**Given** a battle with villages and convoys,
**When** occupation/burning/escort/raid events occur,
**Then** each emits a typed SimEvent destined for ledger posting
**And** the event carries enough context (region, perpetrator) for accounting.

### Story 4.2: Atrocity Auto-Detection

As a system,
I want atrocities (village burning, refusing rout-surrender) auto-flagged into reports,
So that cruelty can't hide in the numbers.

**Acceptance Criteria:**

**Given** a burned village or refused surrender event,
**When** aftermath resolves,
**Then** the event posts with an atrocity tag
**And** the report must either acknowledge or visibly omit it.

### Story 4.3: Governance Accumulators

As a system,
I want 民心/秩序/墮落 derived by folding ledger entries, with 墮落 monotonic,
So that governance is computed truth, not a stat bar.

**Acceptance Criteria:**

**Given** a ledger chain,
**When** accumulators fold,
**Then** 民心/秩序 reflect net postings and 墮落 equals the max over atrocity-tagged folds
**And** no code path can write an accumulator directly.

### Story 4.4: Governance Victory Path

As a player,
I want to win chapters by holding 民心≥70 and 秩序≥60 at chapter end,
So that ruling well beats fighting well.

**Acceptance Criteria:**

**Given** a chapter reaching its end condition,
**When** accumulators meet thresholds,
**Then** the chapter resolves as governance victory
**And** the report renders it in chronicle voice, not as a battle rout.

### Story 4.5: Defeat Conversion

As a player,
I want a lost battle to route into governance/recovery play rather than game over,
So that defeat is a fork, not an ending.

**Acceptance Criteria:**

**Given** a battle resolved as defeat,
**When** aftermath posts to campaign,
**Then** casualties persist, ledger books the loss (墮落 may ratchet), and the campaign offers a recovery/governance continuation
**And** no game-over screen appears for a single lost battle.

---

## Epic 5: D — Myth Dual-Layer

The second layer of the map comes alive.

### Story 5.1: Myth Infiltration State Machine

As a system,
I want per-region infiltration levels 0–3 driven by myth-layer actions,
So that the spirit world encroaches gradually.

**Acceptance Criteria:**

**Given** a region's infiltration state,
**When** myth events/actions apply,
**Then** transitions follow the explicit 0→1→2→3 table
**And** state persists across the battle and into campaign.

### Story 5.2: Shrine Entities

As a player,
I want capturable shrines on the map's myth layer,
So that sacred ground is an objective.

**Acceptance Criteria:**

**Given** a map's shrine layer,
**When** a squad occupies a shrine region,
**Then** the shrine's allegiance flips and its deity's stance updates
**And** shrine state co-renders on the same geometry as terrain.

### Story 5.3: 天命 Currency

As a player,
I want 天命 earned through myth-layer deeds and spent on myth actions,
So that the gods have an economy.

**Acceptance Criteria:**

**Given** a shrine pacified,
**When** the ledger posts,
**Then** 天命 credits on the five-account ledger
**And** myth actions debit 天命 with insufficient-funds rejection.

### Story 5.4: Myth Actions

As a player,
I want myth actions — pacify shrine, invoke possession, raise ghost armies — costing 天命，
So that the second layer is a real option set.

**Acceptance Criteria:**

**Given** sufficient 天命 and a valid target,
**When** a myth action triggers,
**Then** its effect applies on the myth layer and amplifies the 民心 axis
**And** each action enters the MythLog by name.

### Story 5.5: Invasion Events

As a system,
I want myth-layer counterattack events when infiltration peaks at 3,
So that the gods push back.

**Acceptance Criteria:**

**Given** a region at infiltration 3,
**When** the invasion check fires,
**Then** a ghost-army/curse event spawns per the region's deity
**And** the event books into ledger and MythLog.

### Story 5.6: MythLog

As a player,
I want a folk-register log of myth events parallel to the official record,
So that the people remember differently than the clerks.

**Acceptance Criteria:**

**Given** myth events in a chapter,
**When** MythLog renders,
**Then** entries use the folk/rumor register （耳聞體） distinct from HistorianReport
**And** MythLog entries may contradict the official account without either being marked wrong.

### Story 5.7: GodStance

As a system,
I want per-region deity disposition tracking that modulates myth action outcomes,
So that the gods have moods, not just effects.

**Acceptance Criteria:**

**Given** player myth conduct and governance posture,
**When** GodStance computes per region/deity,
**Then** action success/cost modulates by stance
**And** stance is readable in the narrative layer (dossier/shrine text variants).

---

## Epic 6: C — Narrative Systems

The chronicle speaks.

### Story 6.1: HistorianReport Renderer

As a player,
I want post-battle reports in 史官體 with elliptical counts and omission counters,
So that the account is literature, not a log dump.

**Acceptance Criteria:**

**Given** a resolved battle's event list,
**When** the report renders,
**Then** it uses clause pools (「斬獲甚多」, not numbers) and always prints the omission count
**And** rendering is deterministic from the same event list.

### Story 6.2: ChapterConventions

As a player,
I want each chapter framed with 題詞 frontispiece, 判詞 enemy judgment, and 欲知後事 cliffhanger,
So that the campaign reads as 章回體.

**Acceptance Criteria:**

**Given** a chapter open/close,
**When** conventions render,
**Then** the correct per-chapter frames appear from potato.narrative/1 content
**And** frames vary by ledger state.

### Story 6.3: GeneralDossier

As a player,
I want enemy generals presented in hearsay register,
So that I never truly know my enemy.

**Acceptance Criteria:**

**Given** a rival general,
**When** their dossier renders,
**Then** all statements carry hearsay markers （據聞/或云） with no omniscient stats
**And** dossier accuracy is itself a tracked uncertainty that proven-wrong events can revise.

### Story 6.4: IntelLedger

As a system,
I want distorted/misleading intelligence recorded as narrative device,
So that my intel has a history of being wrong.

**Acceptance Criteria:**

**Given** intel inputs to a chapter,
**When** the IntelLedger stores them,
**Then** entries record both claim and later verdict (true/false/unresolved)
**And** distorted intel feeds RivalDeck and QuantumFog priors.

### Story 6.5: NarrativePack

As a developer,
I want versioned narrative content bundles loaded as registries,
So that story content is data, not code.

**Acceptance Criteria:**

**Given** potato.narrative/1 packs in ssets/narrative/,
**When** the campaign boots,
**Then** packs register read-only
**And** bad-version packs reject without failing the library.

### Story 6.6: Scribe Marginalia

As a player,
I want the Scribe's small notes annotating ledger entries — some authentic, some suspect,
So that the ledger has a second hand.

**Acceptance Criteria:**

**Given** posted ledger entries,
**When** marginalia render,
**Then** each note carries an authenticity flag state
**And** forged-looking marginalia can be flagged suspect, feeding the Judgment beat.

### Story 6.7: Document Variant Rendering

As a system,
I want document text selected by ledger state and GodStance,
So that what the game writes about you tracks what you did.

**Acceptance Criteria:**

**Given** a document type and current ledger/GodStance,
**When** it renders,
**Then** clause variants select per state thresholds
**And** variant pools are modular clauses, not full rewrites.

### Story 6.8: Audit Spread (Judgment of the Brush)

As a player,
I want a late-campaign audit event laying open forged and suspect entries,
So that I answer for what was written.

**Acceptance Criteria:**

**Given** the audit trigger point in late campaign,
**When** the spread renders,
**Then** all suspect/forged entries present with provenance
**And** the Scribe testifies or stays silent per marginalia authenticity state.

### Story 6.9: EndingPage — Four Voices

As a player,
I want one of four ending folios determined by final ledger state,
So that the book closes on what I actually wrote.

**Acceptance Criteria:**

**Given** campaign completion,
**When** the final ledger folds,
**Then** one of four voices renders (believed/doubted/forged/abandoned chronicle)
**And** no player-facing ending selection exists — the ledger decides.

---

## Epic 7: E — No-Combat Chapters

The subversion pillar becomes playable.

### Story 7.1: Negotiation System

As a player,
I want capitulation/treaty paths resolvable without battle,
So that words can end wars.

**Acceptance Criteria:**

**Given** a chapter permitting negotiation,
**When** treaty conditions meet ledger/GodStance prerequisites,
**Then** the chapter resolves via signed terms
**And** terms post as ledger entries like any other action.

### Story 7.2: Deterrence

As a player,
I want threat-display victories achieved without engagement,
So that massed might can be a bluff that wins.

**Acceptance Criteria:**

**Given** a deterrence-eligible chapter,
**When** my displayed strength and ledger reputation exceed the rival's threshold,
**Then** the enemy withdraws and the chapter resolves
**And** deterrence success/failure depends on intel certainty, not just stats.

### Story 7.3: Subversion & Defection Triggers

As a player,
I want defection triggers — infiltrated units, induced surrender — as doctrine-level options,
So that armies can be unmade from inside.

**Acceptance Criteria:**

**Given** a squad with a subversion card and valid conditions,
**When** the trigger fires,
**Then** enemy units defect/stand down per card effect
**And** defection events book into both ledger and dossier records.

### Story 7.4: Rival Defection Arcs

As a player,
I want the Cautious rival persuadable via sustained governance record and the Nemesis inducible in the late campaign,
So that my greatest enemy can become my greatest conversion.

**Acceptance Criteria:**

**Given** the Cautious general and a qualifying governance record,
**When** the defection check runs at its chapter,
**Then** he can lay down arms — resolving the chapter without battle
**And** the Nemesis defection requires late-movement prerequisites and remains optional.

### Story 7.5: Designated Zero-Combat Chapters

As a player,
I want at least 2 chapters winnable with zero combat,
So that 至聖者無戰 is proven, not promised.

**Acceptance Criteria:**

**Given** the chapter library,
**When** designated chapters play,
**Then** negotiation/deterrence/subversion paths can fully resolve them
**And** a no-combat resolution posts to the ledger as a distinct chapter outcome type.

---

## Epic 8: F — Presentation

The game becomes visible and audible.

### Story 8.1: SpriteAtlas & Map Rendering

As a player,
I want pixel-art sprites for units, terrain, and myth entities on a rendered map,
So that I can watch my words execute.

**Acceptance Criteria:**

**Given** `Game/Render` bound to the sim's fog/certainty view (never truth),
**When** a battle renders,
**Then** units, terrain, and shrine-layer overlay draw at 60 FPS @1080p
**And** low-certainty regions visually grain/blur rather than black out.

### Story 8.2: HUD — Doctrine UI & CP Bar

As a player,
I want a planning-phase doctrine editor and an execution-phase CP/intervention bar with density scaling,
So that the interface serves authorship first.

**Acceptance Criteria:**

**Given** the three beats,
**When** each renders,
**Then** Planning shows sheet/deck/arrows fully interactive, Execution shows CP + interventions only
**And** HUD density is a player setting, not a rebuild.

### Story 8.3: CJK Text Pipeline

As a player,
I want Traditional Chinese text rendering with chaptered typography throughout,
So that 史官體 reads as intended.

**Acceptance Criteria:**

**Given** CJK fonts in `assets/fonts/`,
**When** any document page renders,
**Then** text renders correctly with the document/page framing
**And** text expansion tolerance (~1.5-2x) is built into layouts.

### Story 8.4: Audio

As a player,
I want battle ambience, UI feedback, and chapter music,
So that the world is heard, not just read.

**Acceptance Criteria:**

**Given** the audio direction decision (OQ-1, due this epic),
**When** battles and pages play,
**Then** page-sonics ground the ledger/document screens and field audio stays readable during script observation
**And** no voice acting exists anywhere.

### Story 8.5: Chapter Frontispieces

As a player,
I want ink-illustration 冊頁 at chapter opens and endings,
So that each 回 feels like a page in an album.

**Acceptance Criteria:**

**Given** a chapter open,
**When** the commission page renders,
**Then** its frontispiece accompanies the 題詞
**And** ending folios get their own 冊頁 treatment.

---

## Epic 9: G — Content & Tools

Content breadth and developer tooling.

### Story 9.1: Doctrine Card Pool Expansion

As a player,
I want the card pool grown to ~40-60 cards with a balance pass,
So that deck-building has real breadth.

**Acceptance Criteria:**

**Given** the card registry,
**When** the full pool loads,
**Then** all cards parse, trigger/condition/action semantics resolve, and cooldowns/stat targets match `potato.balance/1`
**And** balance values live in data, never sim constants.

### Story 9.2: SquadTemplate Budgeted Builds

As a developer,
I want nested squad templates instantiated within a 物資 budget with per-item skip reasons,
So that armies build cleanly from data.

**Acceptance Criteria:**

**Given** a `potato.squad/1` template exceeding budget,
**When** BudgetedBuild runs,
**Then** affordable items instantiate and each skip records a reason (over_budget, unknown_id)
**And** no silent drops occur.

### Story 9.3: Deck Plundering

As a player,
I want captured enemy doctrine cards as spoils,
So that beating a rival literally takes his tricks.

**Acceptance Criteria:**

**Given** a defeated rival,
**When** aftermath resolves,
**Then** eligible enemy cards enter my deck pool
**And** the plunder posts to the ledger.

### Story 9.4: Enemy General Editor

As a developer,
I want a developer-facing editor for rival generals over the versioned-JSON boundary,
So that rivals are content.

**Acceptance Criteria:**

**Given** the editor tool,
**When** authoring a general,
**Then** it emits `potato.rival/1` (dossier, priors, counter-deck rules) with no engine linking
**And** optional C#/.NET implementation stays outside the game build.

### Story 9.5: Sandbox Mode

As a developer,
I want a doctrine playground — arbitrary squads, cards, maps — for testing,
So that systems are exercisable without a campaign.

**Acceptance Criteria:**

**Given** sandbox launch,
**When** composing sides and cards freely,
**Then** a battle runs headless or in-shell
**And** sandbox battles still produce valid records.

### Story 9.6: Tutorial Chapter

As a player,
I want a first chapter that teaches the authorship loop inside the fiction,
So that I learn to write by writing.

**Acceptance Criteria:**

**Given** a new campaign,
**When** Ch.1 plays,
**Then** doctrine composition, execution watching, and CP triage are each exercised with in-fiction framing (the commission page)
**And** no out-of-fiction tutorial UI appears.