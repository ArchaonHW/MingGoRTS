# MingGoRTS — Development Epics & Story Breakdown

Companion to `gdd.md` (same folder). Rebuilt 2026-09-29 for actual repo state: the game layer is greenfield — engine subsystems (Core/ECS/Rendering/etc.) and the IDE exist; no gameplay or campaign code yet.

Execution sequence: **E0 → B → A → L → D → C → E → F → G** (F and G may partially parallel once E0 lands).

---

## E0 — Battle Core (foundation)

The headless vertical slice: a single battle runs planning → execution → aftermath with doctrines executing on both sides. Nothing else in the project compiles against gameplay until this lands.

- E0-1 Gameplay module skeleton — layered dependency direction (engine ← gameplay), CMake target, headless test harness entry.
- E0-2 BattleMap — tile/graph battlefield model, terrain flags, strategic points (ferries, depots, villages).
- E0-3 Squad model — named squads, stats, position/movement, cohesion/morale state, rout threshold.
- E0-4 Doctrine interpreter — trigger→condition→action→modifier cards, per-card cooldowns, squad doctrine sheets (3–5 slots).
- E0-5 BattleController — Planning→Execution→Aftermath state machine, tick loop, CP pool (start 3 / +1 per 60s / cap 5).
- E0-6 CP interventions — redirect (1 CP), doctrine override (2 CP), retreat (3 CP); ≤3s resolution.
- E0-7 BattleRecorder — event recording + replay, record-is-truth, tamper-evident integrity seal, downgrade warning.
- E0-8 Win evaluation — military victory (rout/annihilation) and defeat detection feeding the three-path model.
- E0-9 QuantumFog — certainty-gradient fog (0–100): observation collapse, probes, ~10/min decay, entanglement, personality-prior bias.
- E0-10 BattlePlan — plan arrows with execution bonus scaled by QuantumFog certainty.
- E0-11 Symmetric AI opponent — enemy runs the same doctrine machinery and CP rules; personality priors (aggressive/defensive/cunning).

## B — Campaign Persistence

- B-1 CampaignState facade — aggregates chapter, roster, ledger stores under a versioned schema.
- B-2 ChapterLibrary — versioned chapter definition packs.
- B-3 Save system — atomic writes, bad schema/version rejects cleanly.
- B-4 Chapter shell — campaign map chapter progression, unlock rules.
- B-5 RivalDeck — cross-chapter opponent learning: track player's habitual triggers, pre-write counter-decks; difficulty via counter-deck depth, not stats.
- B-6 Roster & RefitCamp — persistent named casualties; between-chapter refit actions (deploy/heal/recruit/plunder) spending 物資.

## A — Governance Axis

- A-1 GovernanceField — per-battle village occupation/burning, convoy escort/raid events.
- A-2 Atrocity auto-detection — rout-surrender acceptance, village burning flagged into reports.
- A-3 Campaign governance accumulators — 民心/秩序/墮落 derived from booked ledger events; 墮落 ratchet monotonic.
- A-4 Governance victory path — chapter win via 民心/秩序 thresholds.
- A-5 Defeat conversion — battle loss routes into governance/recovery continuation instead of game over.

## L — Ledger Mechanization

- L-1 Ledger — five accounts (武功/民心/天命/軍威/物資), double-entry invariant enforced.
- L-2 Tamper-evident chain — append-only hash chain with break detection.
- L-3 Forgery & suspicion — forged-book detection, enemy forgery-injection channel, suspect flagging persisted into reports.
- L-4 HistorianReport audit segments — ledger-aware chapter reports; omission counters always present ("本報告省略 N 項").
- L-5 Replay audit cross-check — battle archive integrity vs ledger chain consistency.

## C — Narrative Systems

- C-1 ChapterConventions — chaptered framing: frontispiece (題詞), enemy judgment (判詞), cliffhanger (欲知後事), four-voice ending (四聲部).
- C-2 GeneralDossier — enemy general views in hearsay register (聽聞態).
- C-3 IntelLedger — distorted/misleading intelligence as narrative device.
- C-4 NarrativePack — versioned narrative content bundles.
- C-5 EndingPage — four-voice ending resolution from final ledger state.

## D — Myth Dual-Layer

- D-1 Myth infiltration — per-region state machine 0–3.
- D-2 Shrine entities — capturable myth assets on the map's second layer.
- D-3 天命 currency — earn/spend rules, ledger account wiring.
- D-4 Myth actions — pacify shrine, invoke possession, raise ghost armies; 民心 amplification.
- D-5 Invasion events — myth-layer counterattack when infiltration peaks.
- D-6 MythLog — named myth event register feeding the historian's chronicle voice.
- D-7 GodStance — per-region deity disposition tracking, modulating myth action outcomes.

## E — No-Combat Chapters

- E-1 Negotiation system — capitulation/treaty doctrine path.
- E-2 Deterrence — threat-display victory without engagement.
- E-3 Subversion — defection triggers, infiltrated units.
- E-4 Designated zero-combat chapters — at least 2 chapters winnable without combat.

## F — Presentation

- F-1 SpriteAtlas completion — pixel-art sprites for unit matrix, terrain, myth entities.
- F-2 HUD — planning-phase doctrine UI, execution CP/intervention bar, density scaling.
- F-3 CJK fonts — Traditional Chinese text pipeline, chaptered typography.
- F-4 Audio — battle ambience, UI feedback, chapter music (style decision due at this epic; see GDD OQ-1).
- F-5 Chapter frontispieces — ink-illustration 冊頁 per chapter.

## G — Content & Tools

- G-1 Card pool — expand to ~40–60 doctrine cards with balance pass.
- G-2 SquadTemplate — nested JSON templates, budgeted builds with per-item skip reasons.
- G-3 Deck plundering — capture enemy cards as spoils.
- G-4 Enemy general editor (developer-facing tool; optional C#/.NET over the versioned-JSON boundary — boundary decision deferred to architecture).
- G-5 Sandbox mode — doctrine playground.
- G-6 Tutorial chapter — teaches authorship loop inside the fiction.

---

## Traceability

| Pillar | Served by |
|---|---|
| Authorship over control | E0-4, E0-5, E0-6, E0-10; G-1 |
| Subversion over annihilation | E-1..4; B-5; C-2; A-5 |
| Governance over conquest | A-1..5; L-1..5; B-6; C-5 |
| Two worlds, one map | D-1..7; E0-9 (myth priors); F-1, F-5 |

| Mechanic (GDD) | Delivered by |
|---|---|
| Doctrine cards | E0-4, G-1, G-2 |
| Three-beat battle | E0-5 |
| CP interventions | E0-6 |
| QuantumFog | E0-9 |
| BattlePlan | E0-10 |
| Symmetric AI / priors | E0-11, B-5 |
| Roster & RefitCamp | B-6 |
| Ledger & forgery | L-1..5 |
| GovernanceField | A-1..5 |
| Myth infiltration & log | D-1..7 |
| RivalDeck | B-5 |
| BattleRecorder | E0-7 |
| Chaptered voice / endings | C-1..5, L-4 |
