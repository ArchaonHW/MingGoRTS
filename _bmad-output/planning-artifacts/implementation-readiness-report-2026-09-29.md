---
stepsCompleted: [1, 2, 3, 4, 5, 6]
---

# Implementation Readiness Assessment Report

**Date:** 2026-09-29
**Project:** MingGoRTS

## Document Inventory

| Document | Path | Status |
|---|---|---|
| GDD | `planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md` | Complete (validated) |
| Architecture | `game-architecture.md` | Complete (9/9 steps) |
| Epics & Stories | `planning-artifacts/epics.md` | Complete (9 epics / 61 stories) |
| Narrative Design | `narrative-design.md` | Complete (11/11 steps) — supplementary input |
| GDD Decision Log | `planning-artifacts/gdds/.../decision-log.md` | Supplementary |
| Draft epics | `planning-artifacts/gdds/.../epics.md` | SUPERSEDED by planning-artifacts/epics.md |
| UX Design | — | Not present (F-epic carries UI requirements) |

## GDD Analysis

### Functional Requirements

FR1: Doctrine card system — trigger→condition→action→modifier; squad sheets 3–5 slots; deck pool ~40–60; cooldowns 5–60s.
FR2: Three-beat battle — Planning (untimed) → Execution (5–10 min RT) → Aftermath.
FR3: CP — start 3, +1/60s, cap 5; costs 1/2/3; resolve ≤3s.
FR4: QuantumFog — certainty 0–100, collapse/probe/decay ~10/min, entanglement, priors.
FR5: BattlePlan — certainty-scaled plan-arrow bonus.
FR6: Roster & RefitCamp — persistent casualties; deploy/heal/recruit/plunder on 物資.
FR7: Ledger — five accounts double-entry; derived 民心/秩序/墮落; append-only tamper-evident; forgery; suspect flags.
FR8: GovernanceField — village/convoy events → accumulators; governance victory 民心≥70 & 秩序≥60.
FR9: Myth dual-layer — infiltration 0–3, shrines, 天命, myth actions, invasions, GodStance, MythLog.
FR10: RivalDeck — cross-chapter counter-deck learning; hearsay dossiers.
FR11: Symmetric AI — same machinery, priors, no stat cheating.
FR12: BattleRecorder — record-is-truth replay; tamper rejection; downgrade warning.
FR13: Defeat conversion — loss routes to governance; no single-battle game over.
FR14: Victory paths — military rout (<20% cohesion), governance, subversion; zero-combat chapters; four-voice ending.
FR15: Campaign — 8–12 chapters, ChapterLibrary, doctrinal progression.
FR16: Narrative systems — HistorianReport omissions, ChapterConventions, IntelLedger, NarrativePack, dossiers.
FR17: Document voices — Scribe marginalia, rival defection arcs, variant rendering, audit spread, ending folios (from Narrative Design).
FR18: Presentation — sprite atlas, HUD, CJK, frontispieces, audio.
FR19: Content & tools — card pool, templates, plundering, editor, sandbox, tutorial.
FR20: Versioned JSON pipeline — `potato.<name>/<ver>` via game-layer DOM parser.

Total FRs: 20

### Non-Functional Requirements

NFR1: Determinism — fixed 20 Hz, integer/fixed-point, seeded PRNG canonical order.
NFR2: Headless gameplay layer — no GL/GUI.
NFR3: 60 FPS @1080p mid-range during 10-min execution.
NFR4: Atomic saves; schema rejection without mutation.
NFR5: MSVC + MinGW + Linux; headless tests green.
NFR6: Layered deps — Engine ← Gameplay ← Campaign ← Game.
NFR7: No third-party JSON; no unsafe C functions.
NFR8: Canonical eval order; truth boundary (fog-only reads outside Sim).
NFR9: Immutable registries in battle; no file I/O in tick path.
NFR10: SimEvent (recorded) vs EventBus (boundary) separation.

Total NFRs: 10

### Additional Requirements

- Greenfield game layer: no starter template; Gameplay skeleton built as Story 1.1.
- Balance in versioned JSON; constants via constexpr; player settings in save slots.
- Naming/structure per architecture doc; Result<T> boundaries; POTATO_DEBUG gates.
- ASSUMPTIONs pending playtest: chapter count 8–12, CP numbers, fog decay rate, thresholds, unit stats.

### GDD Completeness Assessment

Complete and internally consistent; open items OQ-1 (audio direction, due Epic 8) and OQ-2 (ending ledger conditions, due Epic 6) are explicitly deferred and non-blocking.
## Epic Coverage Validation

### Coverage Matrix

| FR | Epic Coverage | Status |
|---|---|---|
| FR1 | Epic 1 (S1.5), Epic 9 (S9.1) | Covered |
| FR2 | Epic 1 (S1.6) | Covered |
| FR3 | Epic 1 (S1.7) | Covered |
| FR4 | Epic 1 (S1.8) | Covered |
| FR5 | Epic 1 (S1.9) | Covered |
| FR6 | Epic 3 (S3.5, S3.6) | Covered |
| FR7 | Epic 2 (S2.1–2.5) | Covered |
| FR8 | Epic 4 (S4.1–4.4) | Covered |
| FR9 | Epic 5 (S5.1–5.7) | Covered |
| FR10 | Epic 3 (S3.7), Epic 6 (S6.3) | Covered |
| FR11 | Epic 1 (S1.10) | Covered |
| FR12 | Epic 1 (S1.11), Epic 2 (S2.5) | Covered |
| FR13 | Epic 4 (S4.5) | Covered |
| FR14 | Epic 1 (S1.12), Epic 4 (S4.4–4.5), Epic 7 (S7.1–7.5) | Covered |
| FR15 | Epic 3 (S3.3, S3.4) | Covered |
| FR16 | Epic 6 (S6.1–6.5) | Covered |
| FR17 | Epic 6 (S6.6–6.9), Epic 7 (S7.4) | Covered |
| FR18 | Epic 8 (S8.1–8.5) | Covered |
| FR19 | Epic 9 (S9.1–9.6) | Covered |
| FR20 | Epic 1 (S1.2), Epic 3 (S3.1, S3.3), Epic 6 (S6.5), Epic 9 (S9.1) | Covered |

### Missing Requirements

None — no FR uncovered; no epic claims an FR absent from the GDD.

### Coverage Statistics

- Total GDD FRs: 20
- FRs covered in epics: 20
- Coverage: 100%
## UX Alignment Assessment

### UX Document Status

Not found — no UX design document exists.

### Alignment Issues

- HUD/doctrine editor/page framing are heavily implied (FR3, FR18) and partially specified in Narrative Design (document-page metaphors, text-expansion tolerance), but no dedicated UX spec exists.
- Architecture defers HUD framework choice to Epic F — consistent, but the UX spec gap means Epic 8 stories lack interaction-detail requirements.

### Warnings

- WARNING: Player-facing UI is substantial (planning-phase editor, CP bar, document pages, ledger view) yet no UX Design doc exists. Recommend `gds-ux` before Epic 8 begins. Non-blocking for Epics 1–7 (headless/doc-data work).
## Epic Quality Review

### Best-Practices Validation

- Player value: all epics deliver playable/visible outcomes; Epic 2 (Ledger) is technical-sounding but the ledger is itself player-facing gameplay (readable book, suspicion, audits) — accepted.
- Epic independence: after the L↔B reorder, every epic builds only on earlier outputs. Verified chain: E0 → L → B → A → D → C → E → F → G.
- Story sizing: 61 stories, each single-session scope; largest is S1.8 QuantumFog (5 operations) — flagged for possible split at sprint level.
- Forward dependencies: none found within epics after reorder; B-epic stories (3.1, 3.6) legitimately consume Epic-2 ledger.
- Data creation timing: schemas/entities created by the first story needing them (parser 1.2, map 1.3, squad 1.4, ledger 2.1).
- AC quality: all stories carry testable Given/When/Then with error/rejection paths.

### Findings by Severity

**Critical:** none.

**Major:** none.

**Minor (yellow):**

1. S2.4 Audit-Aware Reports — AC says "the report renders" but the renderer is Epic 6 (S6.1). Remediation: scope S2.4 to audit segment data production; rendering consumed by S6.1. (Doc note, not a blocking defect.)
2. S5.6 MythLog — same pattern: this story produces the folk-register log data; page rendering lands in Epic 6/8.
3. Epic 9 is partly developer-facing (editor, sandbox) — acceptable for a tools epic; flagged for awareness.
4. S1.8 QuantumFog is the largest single story — recommend sprint planning split if it exceeds one session (observe/probe vs decay/entangle/priors).

### Compliance Checklist

| Check | Result |
|---|---|
| Epics deliver player/user value | Pass |
| Epic independence | Pass (post-reorder) |
| Story sizing | Pass |
| No forward dependencies | Pass |
| Data created when needed | Pass |
| Clear acceptance criteria | Pass |
| FR traceability | Pass (20/20) |
## Summary and Recommendations

### Overall Readiness Status

**READY** — with 4 minor advisories, none blocking.

### Critical Issues Requiring Immediate Action

None.

### Minor Advisories

1. UX document absent — substantial player-facing UI (doctrine editor, document pages, ledger view) lands in Epic 8 with no dedicated UX spec. Recommend `gds-ux` before Epic 8.
2. S2.4 / S5.6 produce data that renders in Epic 6 — scope is correct as data stories; noted for implementers.
3. S1.8 QuantumFog may exceed a single dev session — split at sprint level if needed.
4. GDD-embedded epic table and the superseded draft epics carry the old ordering — new epics.md is authoritative; README/docs still describe aspirational state.

### Recommended Next Steps

1. `gds-sprint-planning` — generate sprint status tracking from the 61 stories.
2. `gds-generate-project-context` / `bmad-project-context` — write AGENTS.md so implementation agents inherit the architecture rules (determinism, truth boundary, dependency direction).
3. Fix README stale claims/links (deferred housekeeping).
4. Begin Epic 1 (E0 Battle Core) via `gds-create-story` → `gds-dev-story`.

### Final Note

This assessment identified 0 critical, 0 major, and 4 minor issues across coverage, UX-alignment, and quality categories. The planning chain (GDD → Architecture → Narrative → Epics) is internally consistent and ready for sprint planning and implementation.