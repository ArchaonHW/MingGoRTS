# MingGoRTS GDD — Decision Log

## 2026-09-29 — Session 1: Create intent confirmed

**Context**
- No prior planning artifacts on disk (`_bmad-output/` absent). User confirmed: building from zero; README vision is the seed input.
- Engine + IDE exist (PotatoEngine, MingGoRTS_IDE, docs/ completion reports). Game layer (Gameplay/, Campaign/, assets/) does not exist — README describes the target, not the current state.
- Environment note: `uv`, `python`, and `git` are not installed on this machine — customization resolved by reading `customize.toml` directly (no overrides present in `_bmad/custom/`).

**Inputs**
- `README.md` — full game vision (doctrine cards, dual-layer battles, governance axes, Epic A–G + L sketch).
- `docs/` — engine/IDE architecture and completion reports (engine capabilities only; kept out of GDD per no-implementation-leakage rule).

**Decisions**
- D1: Workflow = GDS GDD Create mode. Game type matched to `strategy` (RTS/tactics/planning signals), secondary signals RPG + turn-based-tactics + card-game. Pending user confirmation.
- D2: Four draft pillars extracted from README (authorship / subversion / governance / dual-layer). Pending user confirmation.

## 2026-09-29 — Session 1 (cont.): Discovery resolved, Express draft

**User decisions**
- D3: Game type = `strategy` (primary), with acknowledged hybrid signals (RPG persistence, tactics planning, card composition).
- D4: Four pillars confirmed as drafted (authorship / subversion / governance / dual-layer).
- D5: Working mode = Express.
- D6: Audience = hybrid three-way (strategy > tactics/deck > narrative-history), priority order noted.
- D7: Defeat handling = defeat converts to governance path; no single-battle game over.
- D8: Campaign scope = full 8–12 chapters including no-combat chapters and four-handed ending.
- D9: Art direction = pixel art.
- D10: Out of scope = multiplayer, mobile/console, commercial release — all excluded for v1.0.

**Assumptions carried into draft** (pending confirmation/playtest): solo developer; mouse+keyboard controls; doctrine sheet 3–5 slots; CP start 3 / +1 per 60s / cap 5; intervention costs 1/2/3; execution phase 5–10 min; card pool ~40–60; 60 FPS @1080p target; pixel sprites + ink frontispiece blend; audio style open; unit matrix draft.

**Epic structure rebuilt** for greenfield state: E0 Battle Core added as foundation; A–G + L retained from vision with resequenced order E0 → B → A → L → D → C → E → F → G.

## 2026-09-29 — Session 1 (cont.): Validation & reconciliation fixes applied

**Review inputs**
- Validator subagent: PASS WITH CONCERNS — 9 major, 7 minor findings.
- Input-reconciliation subagent: README fidelity good overall; flagged unstoried systems (QuantumFog, BattlePlan, RivalDeck, RefitCamp, MythLog, GodStance), voice/tone loss, and stale README doc links/status claims.

**Fixes applied to draft**
- D11: epics.md extended — E0-9 QuantumFog, E0-10 BattlePlan, E0-11 symmetric AI; B-5 RivalDeck, B-6 Roster/RefitCamp; D-6 MythLog, D-7 GodStance (restored from README vision). Traceability tables now map every named mechanic to a story.
- D12: gdd.md de-leaked — engine/build identifiers (FNV-1a, InjectForgery, MarkSuspect, rootHash, Potato::JsonValue, potato.* schema strings, POTATO_TESTS/CTest, tmp+rename) rewritten as player-facing requirements; API-level naming deferred to architecture.
- D13: measurability added — QuantumFog certainty scale/decay/probes, rout threshold 20%, governance thresholds 民心≥70/秩序≥60, economy-tempo table per chapter band, unit-matrix stat columns, 4 worked example doctrine cards, pacing model (chapter duration, plan:execute ratio, campaign hours).
- D14: ledger relationship clarified — 秩序/墮落 are derived campaign accumulators, not spendable accounts.
- D15: terminology canonicalized — "four-voice ending (四聲部結局)", "doctrine deck" (collection) vs "squad doctrine sheet" (per-squad slots), "Ledger".
- D16: chronicle voice restored — added motto + "Voice and Tone" section; BattleRecorder tied to pillar 3 (replay is audit).
- D17: epic table gained Seq column (execution order); quantifiers bounded (min-spec GPU, ≤3s interventions).
- D18: assumptions itemized 1:1 + Open Questions section added (OQ-1 audio, OQ-2 ending voices).

**Set aside / deferred**
- README's stale doc links and aspirational status line ("Epic B 完成 / demo 可玩") — not corrected here; flag to user as a follow-up doc fix.
- Engine/game repo split and C# Tools layer — noted in gdd.md as deferred/post-v1.0.
- doc_standards is empty (no editorial-review skills installed) — polish step is a no-op.
- Genre guide carries no narrative flag; gds-create-narrative still worth offering given chaptered-chronicle framing.
