---
title: 'Game Architecture'
project: 'MingGoRTS'
date: '2026-09-18'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8, 9]
status: 'complete'
engine: 'PotatoEngine (custom, C++20/OpenGL)'
platform: 'PC (Windows primary; MSVC/MinGW/Linux g++)'

# Source Documents
gdd: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md'
epics: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
brief: '_bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md'
narrative: '_bmad-output/narrative-design.md'
---

# Game Architecture

## Document Status

This architecture document is being created through the GDS Architecture Workflow.

**Steps Completed:** 9 of 9 (Complete)

---

## Executive Summary

**MingGoRTS** architecture is designed for **PotatoEngine** (custom,
C++20/OpenGL) targeting **PC** (Windows primary; MSVC/MinGW/Linux g++).

The battle layer exists and is tested; this architecture's subject is
the **campaign layer + narrative systems** that did not yet exist.

**Key Architectural Decisions:**

- **Facade-aggregated campaign state** — subsystems own their data;
  `CampaignState` serializes one `potato.campaign/1` file at chapter
  boundaries only
- **Data-driven narrative packs** — `potato.narrative_pack/1`,
  locale-keyed, runtime fragment+template assembly
- **One assembled interpretation** — ReportAssembler turns the
  recorder event stream into BattleNarrative for four consumers
- **Four novel patterns** — Record-is-Truth, Lossy-Ledger,
  Fragment Assembly, Seepage State Machine
- **Layered structure** — `PotatoEngine ← Gameplay ← Campaign ←
  Examples`; dependency direction enforced by link order

**Project Structure:** Layered by architecture level — battle-scoped
systems in `Gameplay/`, cross-chapter persistence in new `Campaign/`,
narrative content in `assets/narrative/` + `assets/campaign/`.

**Implementation Patterns:** 8 patterns (4 standard + 4 novel)
defined for AI-agent consistency.

**Ready for:** Epic implementation phase (E-5 補完 → E-7 戰役層 →
E-11 敘事系統 → E-6 神話層).

---

## Project Context

### Game Overview

**MingGoRTS** — single-player tactics-RPG × auto-battle RTS in a
semi-fictional Republican-era China. The player's only verb is writing:
doctrine cards (trigger→condition→action) composed pre-battle, executed
in real time against enemy commanders who run the same doctrine
language. Two philosophy axes — 至聖者無戰 and 治平者無勝 — are
load-bearing systems; a History×Myth dual layer amplifies governance.

### Technical Scope

**Platform:** PC (Windows primary; cross-compiler: MSVC/MinGW/Linux g++)
**Genre:** strategy + turn-based-tactics + RPG elements
**Project Level:** High — engine is custom (PotatoEngine, C++20/OpenGL/
GLFW/ImGui); battle layer exists and is tested; the architecture
workflow's real subject is the **campaign layer + narrative systems**
that do not yet exist.

### Core Systems

| System | Complexity | Status | GDD Ref |
|---|---|---|---|
| Battle core (doctrine interpreter, squads, flow fields, 3-beat phase machine) | High | Implemented | E-1 |
| Quantum-fog intel layer (superposition clouds, observe/probe, entanglement, personality priors) | High | Implemented | E-2 |
| Planning UI (card editor, pins, PlanningDeck) | Medium | Implemented | E-3 |
| Real-time + post-battle (HUD, CP, historian report, roster, replay) | Medium | Implemented | E-4 |
| Governance axis (民心/秩序/墮落, five-rung outcomes) | Medium | Partial | E-5 |
| Myth layer (shrines, spirit events, seepage→manifestation) | High | Not started | E-6 |
| Campaign frame (chapter map, persistent stats, biography structure) | High | Not started | E-7 |
| 無戰 chapters (negotiation/deterrence/subversion) | Medium | Not started | E-8 |
| Content pipeline (commander cards, doctrine pool, VRM portraits) | Medium | Partial | E-9 |
| Sandbox simulator + onboarding | Medium | In progress | E-10 |
| Narrative systems (modular historian report, 判詞 rumor-state, rival counter-deck, roster bios, myth event log, dispositions) | High | Not started | E-11 |

### Technical Requirements

- 60 FPS mid-range PC at ≤8 squads + fog clouds + replay logging
- **Headless determinism is an engine-level hard constraint** — every
  gameplay rule exercisable without GL context (all POTATO_TESTS)
- Banned C APIs enforced by CI (strncpy/snprintf only)
- Asset schemas: potato.character_card/1, battle_map/1,
  battle_replay/1, roster/1, squad_template/1, doctrine_set/1 —
  campaign/narrative schemas TBD
- No third-party JSON (project JsonParser); no networking; solo dev

### Complexity Drivers

1. **Campaign-state layer** — cross-chapter persistence (roster,
   民心/秩序/墮落 accumulators, dispositions, god-stance, title
   track) needs a data model that doesn't exist yet; E-11 mounts on
   it
2. **One event stream, four consumers** — replay feed → replay UI,
   modular historian report, omission-count audit, ending generator
3. **Rival counter-deck** — recorder stats player trigger habits; the
   foil's deck is selected from pre-authored counter-variants
4. **Dual-layer rendering/state** — myth seepage is progressive
   (local anomalies → full theme swap), needs a layered state machine
   driving the 4-theme token system
5. **判詞 dual-model** — rumor-state vs truth-state commander
   personality; scouting verifies; cunning plants fakes

### Technical Risks

- New persistence schema(s) for campaign + narrative content
  (potato.campaign/1?, potato.narrative_pack/1?) must stay consistent
  with existing asset conventions
- Myth-layer systems must remain headless-testable (no GL dependency)
- Narrative content volume (~50k words bilingual) must be data-driven —
  a content-pack format separate from code
- E-11 depends on E-7 — architecture should fix the campaign data
  model first, mount narrative systems on it

---

## Engine & Framework

### Selected Engine

**PotatoEngine** — custom, in-repo (C++20, OpenGL)

**Rationale:** The engine *is* the project — MingGoRTS doubles as
PotatoEngine's capability showcase. The battle layer is already
implemented and tested on it; no migration is contemplated. The
architecture workflow therefore treats the engine as fixed and
concentrates on the layers the engine does **not** yet provide.

### Dependency Baseline (verified)

| Dependency | Pinned | Latest upstream | Notes |
|---|---|---|---|
| GLFW | 3.3.8 (FetchContent fallback; local install preferred) | 3.5.1 | Stable API surface used is unchanged; upgrade is optional hygiene, not a requirement |
| glad | vendored `external/glad_gen` (glad1) | — | glad v2.x header layout is incompatible — do not regenerate |
| ImGui | vendored `external/imgui` + glfw/opengl3 backends | — | per-target source list; not a lib target |
| tinygltf | vendored `external/tinygltf` v2.9.7 | — | VRM/glTF portraits; impl macro in single TU |
| JSON | `Serialization/JsonParser.h` (Potato::JsonValue) | — | no third-party JSON, per repo policy |

### Engine-Provided Architecture

| Component | Solution | Notes |
|---|---|---|
| Windowing/input | GLFW | window, context, callbacks |
| Rendering | OpenGL + glad loader | forward-rendered primitives; 4-theme UI token system (UITheme.h) |
| UI | ImGui (immediate mode) | planning editor, HUD, title shell |
| Physics/movement | Custom FlowField | squad-level, no per-unit pathing |
| Serialization | JsonParser + versioned schemas | potato.* schema family |
| Build | CMake (single root CMakeLists) | MSVC + MinGW local, g++ CI; CTest via POTATO_TESTS |
| Testing | Headless test binaries | hard constraint: no GL context in gameplay systems |
| Asset loading | tinygltf + zero-dep PNG codec | VRM portraits, synth-data pipeline |

### Remaining Architectural Decisions

The engine provides the battle layer; the following are **open** and
decided in Step 4:

1. **Campaign-state model** — cross-chapter persistence (roster,
   accumulators, dispositions, god-stance, title track) — new
   `potato.campaign/1` schema candidate
2. **Narrative-content pipeline** — modular historian report corpus
   (fragments + templates), 判詞/題詞/籤詩 libraries, ~50k words
   bilingual — data-driven content pack vs. compiled-in
3. **Event-stream fan-out** — replay feed → 4 consumers (replay UI,
   report assembler, omission audit, ending generator)
4. **Rival counter-deck system** — player habit stats → counter-variant
   selection
5. **Dual-layer myth system** — seepage state machine (local anomalies
   → full manifestation) driving the theme-token layer
6. **判詞 dual-model** — rumor-state vs truth-state personality;
   scouting as verification; cunning-seeded fakes
7. **Save/load architecture** — campaign save format, mid-battle save
   policy

---

## Architectural Decisions

### Decision Summary

| Category | Decision | Schema/Version | Rationale |
|---|---|---|---|
| Campaign state | Facade aggregation — subsystems own data, CampaignState serializes whole | `potato.campaign/1` | Single atomic save without a god-object; matches per-system ownership convention |
| Narrative content | Data-driven packs — locale-keyed JSON, runtime fragment+template assembly | `potato.narrative_pack/1` | 50k words bilingual must not be compiled-in; localization = parallel pack |
| Event fan-out | Assembled intermediate — one ReportAssembler pass → BattleNarrative struct | in-memory + `potato.battle_narrative/1` (optional) | Four consumers read one interpretation; omission audit can't desync from the report |
| 判詞 model | Intel-report ledger — dated, sourced, plantable rumor entries; display derived | embedded in `potato.campaign/1` | "The ledger is a lossy map" as literal mechanism; cunning plants fake *entries* |
| Rival counter-deck | Habit stats → pre-authored counter-variant selection | in `potato.character_card/1` + recorder stats | Deterministic, testable, auditable — ledger can write "彼之陣法似針對我軍慣用" |
| Myth seepage | MythLayer parallel class — per-region seepage levels, god-stance, manifestation events | embedded in `potato.campaign/1` | Headless-testable like QuantumFog; renderer consumes state, never owns it |
| Save/load | Chapter-boundary saves only | `potato.campaign/1` | Mid-battle save would require serializing quantum cloud state; chapter boundaries match the 章回 form |

### Campaign State

**Approach:** Facade aggregation

```
CampaignState            ← owns the potato.campaign/1 file
├── RefitCamp            (persistent squads, wounded, loot)   [exists]
├── Roster               (names, micro-biographies)           [exists, extend]
├── Governance           (民心 / 秩序 / 墮落 accumulators)    [new]
├── Dispositions         (enemy generals: subdued/spared/fallen/where-now) [new]
├── GodStance            (per-god favor, myth event log)      [new]
├── IntelLedger          (判詞 entries per enemy general)     [new]
├── TitleTrack           (earned titles)                      [new]
└── ChapterState         (current arc/chapter, fronts taken)  [new]
```

Each subsystem owns its struct and its ToJson/FromJson; CampaignState
aggregates and writes one versioned file. No subsystem reaches into
another — they coordinate through CampaignState at chapter boundaries.

### Narrative Content Pipeline

**Format:** `assets/narrative/<pack>.json` — `potato.narrative_pack/1`

- String tables locale-keyed: `{ "id": "...", "zh-TW": "...", "en": "..." }`
- Fragment corpus: report fragments, judgment templates, 題詞, 判詞
  epithets, 籤詩 verses, epilogues, roster bio templates
- Runtime: `NarrativePack` loads + validates; `ReportAssembler`
  selects fragments by event type + register, interpolates stats
- Register selection (historian/fox/human/rival's hand) is a
  *template-axis*, not a content fork — Fallen path re-inks existing
  beats by switching template register

### Event Stream Fan-out

**Approach:** One assembled interpretation

```
BattleRecorder (exists) ──event stream──> ReportAssembler ──> BattleNarrative
                                                              ├── ReplayUI
                                                              ├── After-action report
                                                              ├── Omission audit (省略 N 項)
                                                              └── Ending generator
```

ReportAssembler runs once at aftermath; BattleNarrative holds selected
fragments, omission count, casualty/mercy stats, doctrine usage stats.
Consumers never parse raw events — one interpretation, four renderings.

### 判詞 Dual-Model

**IntelLedger:** per enemy general, an ordered entry list:

```
{ date, source(scout/fox/prisoner/planted), personality_claim,
  bias_direction, verified(bool), planted(bool) }
```

Displayed 判詞 = derived from latest unrefuted entry. Scouting writes
a verified entry that may refute earlier ones; cunning enemies write
planted entries disguised as scout reports. The ledger is *literally*
a lossy map — rumor has provenance.

### Rival Counter-Deck

BattleRecorder already logs player's doctrine triggers; extend with
per-trigger usage counts aggregated into CampaignState. At each rival
appearance: pick the counter-deck variant keyed to the player's
most-used trigger type (3 pre-authored variants in the rival's
character_card). Deterministic; the recovered ledger pages can
literally name which habit he countered.

### Myth Layer

`MythLayer` — parallel to QuantumFog in shape:

- Per-region seepage level (0–3: quiet → anomalies → seep → manifest)
- God-stance table (per-god favor, driven by governance + trials)
- Manifestation event queue
- Renderer reads seepage level → local anomalies (fog tint, displaced
  shadows) or full theme-swap; owns nothing itself
- Fully headless: all transitions are pure logic on numbers

### Save/Load

- **Where:** chapter boundaries only — save on aftermath/transition
  screens; no mid-battle save
- **What:** `potato.campaign/1` single file, atomic write
  (tmp+rename), schema-validated on load
- **Versioning:** schema `version` field; loader migrates or refuses
  loudly — never silently drops data
- **Content vs. state:** narrative packs and asset files are static
  content; the campaign file only references them by id

---

## Cross-cutting Concerns

These patterns apply to ALL systems and must be followed by every
implementation — including future AI agents.

### Error Handling

**Strategy:** Result/bool returns — no exceptions

- Loaders and parsers return `bool` (`FromJson`, `Load`) or `nullptr`
  (factory functions); callers check
- **Fatal** (schema mismatch, corrupt file, version unknown): refuse
  loudly — return false + `Potato::Log::Error`, never silently
  continue or drop data
- **Recoverable** (unknown enum value, missing optional field):
  degrade to default + `Potato::Log::Warn` — precedent:
  SquadTemplate's unknown `unit_class` → Infantry + warning
- Errors never pause the game; battle-loop failures degrade to the
  safe behavior (e.g. unreadable save → back to title, not crash)

**Example:**

```cpp
bool IntelLedger::FromJson(const std::string& json) {
    JsonValue root;
    if (!root.Parse(json) || root["schema"].AsString() !=
        "potato.intel_ledger/1") {
        Potato::Log::Error("IntelLedger: bad schema or JSON");
        return false; // 既有資料原封不動
    }
    // ...
    return true;
}
```

### Logging

**Format:** `Potato::Log` — single translation unit, no third-party

- Levels: `Error` / `Warn` / `Info` / `Debug`
- Destination: `stderr` always; optional file sink
  (`Potato::Log::SetFile`) for demo/QA runs
- Tests capture stderr to assert warnings were emitted
- Hot paths (per-frame flow field, combat ticks): `Debug` level only,
  compiled out via `#ifndef NDEBUG` guard — zero release cost

**Example:**

```cpp
Potato::Log::Warn("SquadTemplate '%s': unknown unit_class '%s', "
                  "defaulting to Infantry", id.c_str(), cls.c_str());
```

### Configuration

**Approach:** Three-layer separation

| Layer | Storage | Examples |
|---|---|---|
| Constants | header `constexpr` | grid size, max squads, tick rates |
| Balancing | JSON assets | squad_template stats, doctrine costs, commander personalities |
| Player settings | `potato.settings/1` in user dir | theme, UI scale, locale |

All file-based config goes through `Potato::JsonValue` — never a
second parser. Balancing JSONs are content; settings file is the only
player-mutable config.

### Event System

**Pattern:** Direct calls + recorded event stream

- In-system communication: direct member-function calls — type-safe,
  debuggable, matches existing code
- Cross-phase/cross-consumer communication: the BattleRecorder event
  stream → ReportAssembler fan-out (Step 4) — replay, report, audit,
  and ending all read the same recorded truth
- No event bus: the coupling graph doesn't justify the indirection;
  if a second live event channel ever appears, revisit

### Debug Tools

**Headless:** `POTATO_TESTS` binaries cover every gameplay rule —
new systems add a test exe to the list (hard constraint).

**Visual:** ImGui debug overlay in the playable shell — reads (never
writes) live state: fog cloud probabilities, doctrine trigger status,
myth seepage levels, ledger/omission counts, campaign accumulators.

**Activation:** overlay compiled into all builds, toggled with `F1`
(solo-dev build; no shipping-cert constraints). No command console —
numeric tweaks go through JSON assets, not runtime commands.

---

## Project Structure

### Organization Pattern

**Pattern:** Layered by architecture level — engine / battle / campaign

Existing layout is already type-organized at root (`Gameplay/`,
`Examples/`, `Serialization/`…); the campaign layer gets its own
sibling directory rather than swelling `Gameplay/`. Dependency
direction is enforced by link order, never by include tricks:

```
PotatoEngine  ←  Gameplay  ←  Campaign  ←  Examples (apps)
(engine)         (battle)      (meta)       (shells/tests)
```

### Directory Structure (delta on existing)

```
MingGoRTS/
├── Gameplay/              # battle-scoped systems [existing]
│   ├── HistorianReport.*  # ReportAssembler (N-1) — battle→narrative
│   ├── RefitCamp.*        # persistent roster sub-store
│   └── ... (Squad, Doctrine, QuantumFog, BattleRecorder, …)
├── Campaign/              # NEW — cross-chapter persistence layer
│   ├── CampaignState.*    # facade, owns potato.campaign/1 file
│   ├── Governance.*       # 民心/秩序/墮落 accumulators
│   ├── Dispositions.*     # enemy-general fates + where-now
│   ├── GodStance.*        # per-god favor + myth event log
│   ├── IntelLedger.*      # 判詞 rumor/truth entries
│   ├── TitleTrack.*       # earned titles
│   ├── ChapterState.*     # arc/chapter position, fronts taken
│   ├── MythLayer.*        # per-region seepage state machine
│   ├── NarrativePack.*    # potato.narrative_pack/1 loader
│   └── ReportAssembler.*  # (if HistorianReport graduates to
│                          #  campaign scope — else stays Gameplay/)
├── Examples/              # apps + headless tests [existing]
├── assets/
│   ├── cards/  maps/      # [existing]
│   ├── narrative/         # NEW — fragment/template/判詞/籤詩 packs
│   └── campaign/          # NEW — chapter defs, arc map
└── (user dir)             # saves + potato.settings/1 — NOT assets/
```

### System Location Mapping

| System | Location | Responsibility |
|---|---|---|
| Battle sim (squads/doctrine/fog/recorder) | `Gameplay/` | one battle's rules |
| Report assembly / historian text | `Gameplay/HistorianReport` | events→BattleNarrative |
| Persistent roster | `Gameplay/RefitCamp` + `Roster` | cross-battle unit state |
| Campaign persistence | `Campaign/CampaignState` | potato.campaign/1 facade |
| Governance accumulators | `Campaign/Governance` | 民心/秩序/墮落 |
| 判詞 rumor model | `Campaign/IntelLedger` | sourced/plantable intel entries (cross-chapter owner) |
| 判詞 battle view | `Gameplay/GeneralDossier` | per-battle rumor display; IntelLedger is the persistent store it reads/writes |
| Myth seepage | `Campaign/MythLayer` | per-region state machine |
| Narrative content | `Campaign/NarrativePack` + `assets/narrative/` | pack load + lookup |
| Ending generator | `Campaign/EndingPage` | campaign record → final page |
| Theme/visual seepage | `Examples/` (UI layer) | renderer consumes MythLayer |

### Naming Conventions

Follows existing repo conventions (AGENTS.md verified):

| Element | Convention | Example |
|---|---|---|
| Files | PascalCase matching class | `IntelLedger.h/.cpp` |
| Classes | PascalCase | `CampaignState` |
| Methods | PascalCase verbs | `FromJson`, `AddIntel` |
| Members/vars | camelCase | `bonusCredited`, `seepageLevel` |
| Namespaces | `Potato::Gameplay`, `Potato::Campaign` | — |
| Schemas | `potato.<name>/<version>` | `potato.campaign/1` |
| Test exes | `<System>Test` in Examples/ | `CampaignStateTest` |

### Architectural Boundaries

- `Campaign` may include `Gameplay` headers; `Gameplay` must NEVER
  include `Campaign` (battle stays self-contained/testable)
- `PotatoEngine` depends on neither
- UI/`Examples` may read all layers; writes flow through the owning
  system's API only
- `assets/` is read-only content; saves live in user dir
- All `Campaign` systems: headless, `JsonValue` only, `bool`-return
  loaders per Step 5

---

## Implementation Patterns

These patterns ensure consistent implementation across all AI agents.

### Standard Patterns

**Communication:** Direct references — owner passes pointers/refs to
systems that need them (`plan.Apply(battle, &res, team)` precedent).
Cross-phase communication uses the recorded event stream, not live
references.

**Entity creation:** Template + Instantiate — JSON template validates,
then produces the entity (`SquadTemplate::Instantiate`,
`SquadTemplateLibrary::BudgetedBuild` precedents). Enemy-general
cards, myth events, and future entity types follow this exactly.

**State transitions:** Explicit state machine — enum states + named
transition functions; every transition is a recording point
(BattleController 3-beat precedent). MythLayer seepage levels and
chapter progression follow the same shape — transitions record events,
so replay comes free.

**Data access:** Library + LoadDir — a per-type library scans a
directory, validates schemas, skips bad files with warnings, and
serves typed lookups (`SquadTemplateLibrary` precedent). Systems hold
IDs and ask the library; nothing opens files ad hoc.

### Novel Pattern 1: Record-is-Truth（記錄即真相）

**Purpose:** The BattleRecorder event stream is the single source of
truth for everything narrative. Displayed history is a *view*;
the record is the fact.

**Axiom:** `recorded_events ⊇ displayed_text`. The ledger may omit —
it may never invent.

**Data flow:**

```
BattleController ──emit──> BattleRecorder.events (ordered, typed)
                                │
                    ReportAssembler (post-battle)
                                │
                    BattleNarrative {fragments, omission_count, stats}
                                │
          ┌──────────────┬──────┴────────┬────────────────┐
        ReplayUI    After-action    Omission audit    Ending gen.
                     report         (record − shown)
```

**Rules:**

- Anything not recorded did not happen (for narrative purposes)
- omission_count = |recorded| − |displayed|, always shown, never hidden
- The rival's counter-deck reads recorder statistics — the enemy
  literally reads the same book
- All narrative consumers get BattleNarrative, never raw events

**Edge cases:** skipped cutscene → still recorded; player dismissed a
mid-battle insert → recorded as "looked away"; replay is the
reconciliation tool, always available.

### Novel Pattern 2: Lossy-Ledger（失真帳本）

**Purpose:** 判詞 rumor/truth as literal mechanism — every claim about
an enemy commander has provenance.

**Entry model (per enemy general, ordered):**

```
{ date, source ∈ {scout, fox, prisoner, planted},
  claim (personality axis values), bias_direction,
  verified: bool, planted: bool }
```

**Semantics:**

- Displayed 判詞 = derived from the newest *unrefuted* entry
- Scouting writes a verified entry → may refute prior entries
- Cunning enemies write `planted` entries disguised as scout reports
- A planted entry is indistinguishable to the player until refuted
  — fox/prisoner sources can expose it (bias_direction mismatch)

**Edge cases:** all entries refuted → 判詞 shows "不詳"; planted
entry never refuted at campaign end → epilogue reveals it in the
final audit.

### Novel Pattern 3: Fragment Assembly（碎片組裝）

**Purpose:** 筆法即人格 — the historian's voice is generated, not
authored per-beat.

**Pipeline:**

```
events → fragment selection (by event type + stats)
       → register template (historian | fox | human | rival's hand)
       → interpolation (names, numbers, places)
       → BattleNarrative.fragments
```

**Register is a template axis, not a content fork:**

- Same event, four registers → 霸業/仁政/Fallen tones without
  re-authoring
- Player's accumulated conduct (mercy rate, omission count,
  verification habit) selects the default register
- Fallen path re-inks existing beats by switching to rival's-hand
  templates — its cost is register, not volume

**Data:** fragments live in `assets/narrative/*.json`
(`potato.narrative_pack/1`), locale-keyed; NarrativePack loads and
validates; ReportAssembler selects.

### Novel Pattern 4: Seepage State Machine（滲透狀態機）

**Purpose:** Progressive myth presentation — "something is off"
arrives before "a god arrives."

**States per region:** `0 quiet → 1 anomalies → 2 seep → 3 manifest`

**Rules:**

- MythLayer owns levels + transition conditions (governance,
  slaughter density, god-stance); pure logic, headless
- Renderer is a read-only consumer: level 1 → fog tint/shadow
  anomalies; level 2 → myth soundscape + displaced objects; level 3 →
  full theme-swap (UITheme tokens)
- Sound leads visuals — level transitions fire audio first
- Transitions record events (record-is-truth applies)

**Edge cases:** seepage level never decreases within a chapter (the
land doesn't forget mid-battle); manifest is chapter-scoped — next
chapter re-derives from CampaignState.

### Consistency Rules

| Pattern | Convention | Enforcement |
|---|---|---|
| Serialization | `ToJson`/`FromJson` bool-return, schema-checked | Step 5 error rules |
| Events | emit at state transitions only | recorder API |
| Content IDs | string keys into libraries, never inline text | NarrativePack |
| Registers | enum, template-axis selection | ReportAssembler |
| Renderer | reads state, owns nothing | code review |
| Tests | one headless exe per system in POTATO_TESTS | CMake list |

---

## Architecture Validation

### Validation Summary

| Check | Result | Notes |
|---|---|---|
| Decision Compatibility | PASS | Engine+patterns aligned; `Potato::Log` is the only new infra piece |
| GDD Coverage | PASS | 11/11 epics mapped (E-1–E-4 existing, E-5–E-11 to Campaign/Gameplay layers) |
| Pattern Completeness | PASS | 4 standard + 4 novel patterns, all with examples |
| Epic Mapping | PASS | Every system has a location and owner |
| Document Completeness | PASS | No placeholders remain |

### Issues Resolved

1. Stale `Steps Completed` line in Document Status → synced to 8/9
2. `Gameplay/GeneralDossier` (N-2 battle-scoped 判詞 view) added to
   mapping table — clarified split: GeneralDossier = per-battle view,
   IntelLedger = cross-chapter persistent owner
3. "TBD name" for ending generator → `Campaign/EndingPage`

### Coverage Report

**Systems Covered:** 11/11 epics
**Patterns Defined:** 8 (4 standard + 4 novel)
**Decisions Made:** 7 architectural + 5 cross-cutting

### Validation Date

2026-09-18

---

## Development Environment

### Prerequisites

- CMake ≥3.15, C++20 compiler (MSVC or MinGW locally; g++ in CI)
- OpenGL-capable driver; GLFW via FetchContent fallback (local
  install preferred)
- No package manager, no third-party JSON — vendored deps in
  `external/` only

### AI Tooling (MCP Servers)

No engine-specific MCP servers selected — PotatoEngine is custom;
no existing MCP covers it.

### Setup Commands

```bash
cmake -B build -S .              # configure
cmake --build build --config Release
cd build && ctest -C Release     # all POTATO_TESTS must pass
```

### First Steps

1. **E-5 補完** — `Campaign/Governance` accumulators + persistence
   (民心/秩序/墮落), smallest foundation piece
2. **E-7 戰役層** — `Campaign/` skeleton: CampaignState facade +
   ChapterState + save/load; everything else mounts on it
3. **E-11 敘事系統** — NarrativePack + IntelLedger + Dispositions +
   GodStance; HistorianReport/GeneralDossier already landed in
   Gameplay/ as the battle-scoped halves
4. **E-6 神話層** — MythLayer state machine; renderer consumes last
