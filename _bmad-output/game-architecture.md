---
title: 'Game Architecture'
project: 'MingGoRTS'
date: '2026-09-29'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8, 9]
status: 'complete'
engine: 'PotatoEngine (in-repo, C++20)'
platform: 'PC (Windows, Linux)'

# Source Documents
gdd: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md'
epics: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/epics.md'
brief: null
---

# Game Architecture

MingGoRTS runs a deterministic, fixed-tick battle simulation where both sides execute player-authored doctrine scripts on identical machinery — replays are the audit trail. A pure `Gameplay` sim library (headless, no GL) sits under a `Campaign` persistence layer and a thin `Game` shell; all content is versioned JSON, all campaign state lives in an append-only double-entry ledger, and presentation concerns are deferred to a later epic.

## Document Status

This architecture document is being created through the GDS Architecture Workflow.

**Steps Completed:** 2 of 9 (Initialize, Project Context)

---

## Project Context

### Game Overview

**MingGoRTS（民國史詩 RTS）** — doctrine-card-programming tactics hybrid: players write unit behavior (trigger→condition→action) instead of micromanaging; battles execute symmetric scripts in real time; defeat converts to governance play across an 8–12 chapter campaign.

### Technical Scope

**Platform:** PC (Windows MSVC/MinGW, Linux) — custom PotatoEngine (C++20, OpenGL 3.3+)
**Genre:** Strategy (medium complexity; hybrid RPG persistence + card composition)
**Project Level:** Solo dev, high system complexity / low asset complexity
**Networking:** None — strictly single-player

### Core Systems

| System | Complexity | Notes |
|---|---|---|
| Doctrine interpreter (cards→runtime) | High | core differentiator; shared by player & AI |
| BattleController 3-beat loop + CP | Medium | fixed-tick, deterministic |
| QuantumFog probabilistic intel | High | novelty: certainty-gradient fog, priors, entanglement |
| BattleRecorder (record-is-truth) | High | determinism + tamper-evidence; replay = audit |
| Roster/Refit + Campaign persistence | Medium | builds on SerializationManager save slots |
| Ledger (5 accounts, hash chain) | Medium | data-integrity system doubling as gameplay |
| Governance accumulators | Low-Med | derived stats from booked events |
| Myth dual-layer | Medium | second data layer over same map |
| Symmetric AI + RivalDeck | High | reuses doctrine machinery + cross-chapter stats |
| Presentation (pixel atlas/HUD/CJK) | Medium | SpriteAtlas greenfield; ImGui GUI exists |

### Technical Requirements

- Gameplay layer must run headless (no GL) — determinism enables replay
- 60 FPS @1080p target; fixed-tick simulation
- All game data: namespaced versioned JSON via engine parser
- Atomic saves; schema-version rejection without state mutation

### Complexity Drivers / Risks

- Determinism is load-bearing (replay=audit) — forbids float nondeterminism in sim
- Engine facade is rough (subsystem accessors disabled; ECS internals flagged) — architecture may bypass facade and bind subsystems directly
- No general JSON value parser exists yet — decision needed (extend Serialization vs game-layer parser)
- QuantumFog + dual-layer + ledger interplay is the cross-cutting knot

---

## Engine & Framework

### Selected Engine

**PotatoEngine** — in-repo custom engine, C++20, CMake, OpenGL 3.3+

**Rationale:** The project IS the engine — PotatoEngine + MingGoRTS IDE already exist with completion reports in `docs/`. Switching engines would discard the entire repo. The GDD's technical specifications already mandate it. No alternative was evaluated; this is a constraint, not a choice.

### Project Initialization

Existing CMake build:

```bash
cmake -B build -S .
cmake --build build --config Release
cd build && ctest -C Release    # all headless tests must pass
```

MinGW path also supported (`-G "MinGW Makefiles"`); local verification requires both MSVC and MinGW.

### Engine-Provided Architecture

| Component | Solution | Notes |
|---|---|---|
| Rendering | OpenGLRenderer + Camera/Shader/Lighting (GL 3.3) | 2D sprite path must be built (no SpriteAtlas) |
| Physics | PhysicsSystem | depth TBD — gameplay likely needs only grid/graph logic |
| Input | InputManager | |
| Scene | SceneHandle / SceneNode | facade-level |
| ECS | ECSCoordinator (Entity/Component/System) | internals flagged "needs adjustment" in code comments |
| Events | EventBus (typed events) | |
| Serialization | SerializationManager + Json/BinarySerializer + save slots + autosave | object-level only; no general JSON value parser |
| GUI | ImGui layer (AgentGUI) | IDE-grade; game HUD needs its own pass |
| Build/Test | CMake + BuildEngine.bat + CTest | headless test convention established |
| AI platform | LLM/RAG/RL/NLP/AgentChain/ToolFramework | primarily IDE-facing; not required for game AI |
| Engine facade | PotatoEngine (lifecycle, events, resource/scene handles, time scale, callbacks) | subsystem accessors currently disabled — may bypass facade |

### Remaining Architectural Decisions

To be resolved in Step 4:

1. Game-layer module structure (`Gameplay/` / `Campaign/` layering, headless boundary)
2. Fixed-tick deterministic simulation model (record-is-truth enabler)
3. Doctrine runtime model (data-driven interpreter vs. ECS systems)
4. QuantumFog data model and dual-layer map representation
5. Ledger + hash-chain design as gameplay system
6. General JSON value parser: build vs. extend Serialization
7. Sprite pipeline (SpriteAtlas) + game HUD + CJK font path
8. Versioned schema conventions for all game data
9. How the game binds engine subsystems (facade vs. direct)

### MCPs & Starter Templates

N/A — custom in-repo engine; no MCP ecosystem or starter templates apply.

---

## Architectural Decisions

### Decision Summary

_Version column omitted — every choice below is in-repo/in-house; no external dependencies are introduced by this architecture. Toolchain: C++20, CMake ≥ 3.15, OpenGL 3.3._

| # | Category | Decision | Rationale |
|---|---|---|---|
| D-ARCH-1 | Simulation determinism | Fixed 20 Hz tick + integer/fixed-point math + seeded PRNG | replay = audit: bit-exact reproduction across MSVC/MinGW; floats banned from sim |
| D-ARCH-2 | Battle entity model | Bespoke flat POD sim state (BattleState arrays) | deterministic, fully serializable, hash-comparable, headless — engine ECS reserved for presentation later |
| D-ARCH-3 | Doctrine runtime | Data-driven interpreter over versioned JSON cards | add cards without code; same interpreter serves player & AI (symmetric AI for free); auditable execution order |
| D-ARCH-4 | Module layering | `Gameplay` lib (pure sim, headless) ← `Campaign` lib (cross-chapter) ← Shell/apps | single-direction dependencies; gameplay compiles without GL |
| D-ARCH-5 | JSON parsing | Game-layer `JsonValue` DOM parser (self-built) | Serialization only does object-level ISerializable; content files need DOM access; repo rule bans third-party JSON |
| D-ARCH-6 | Replay model | Event sourcing: seed + deployment + doctrine + CP event stream; replay re-runs sim | small files, hash-rooted integrity, tamper-evident — the audit IS the replay |
| D-ARCH-7 | QuantumFog model | Region certainty field + probability-cloud entities | per-node certainty gradient for map intel; superposed cloud objects for enemy positions; collapse/probe/decay/entangle as sim ops |
| D-ARCH-8 | Campaign save | Versioned JSON on SerializationManager slots + `potato.<name>/<ver>` header + tmp→rename atomic write | readable, auditable, rejects bad schema without mutating state |

### Simulation Kernel

**Fixed 20 Hz deterministic tick.** All sim math uses integer/fixed-point (`int32` milliprecision where fractions needed); every random draw consumes the seeded PRNG stream in canonical order. Physics/ECS/rendering never feed back into sim state — sim is a pure function of (state, inputs, seed).

### Battle State

`BattleState` = flat struct arrays: squads, doctrine sheets, card cooldowns, plan arrows, fog field, cloud entities, pending ledger events, CP pool. Whole state serializes to versioned JSON and hashes — used for save-anywhere, replay verification, and test assertions.

### Doctrine Interpreter

Per tick, the interpreter walks each squad's sheet in slot order: evaluate trigger → check condition → execute action → apply modifier → write cooldown. Cards are `potato.doctrine_card/<ver>` JSON. The enemy runs the identical interpreter — symmetric AI with zero special-casing.

### Persistence

- **Content files** (cards, maps, squads, chapters, narrative packs): `potato.<name>/<ver>` JSON via the game-layer DOM parser.
- **Saves**: SerializationManager slots carrying versioned JSON + atomic write; rejects bad version, preserves existing save.
- **Ledger**: append-only event chain; each entry hashes the previous — verification walks the chain; forgery injection is an explicit write path, suspect marks persist in-file.
- **Replays**: event-sourced stream + integrity root hash.

### Engine Binding

Bypass the `PotatoEngine` facade (subsystem accessors are disabled): `Gameplay` links engine leaf modules directly (Events, FileSystem, Logging, MathUtils); the Shell binds Rendering/Input/GUI. Facade may be rehabilitated later — not load-bearing.

### Deferred to Epic F

SpriteAtlas batching, HUD framework choice, audio implementation, CJK/localization pipeline — recorded as open, not decided.

---

## Cross-cutting Concerns

Mandatory rules for every implementation in this repo's game layer.

### Error Handling

**Strategy:** no exceptions in the sim tick path; Result objects at all I/O and content boundaries.

- Sim code (`Gameplay` internals): returns `bool`/status enums; never throws — exceptions break determinism review and hot-path performance.
- Boundary code (file loads, JSON parse, save/load, content validation): `Result<T>{ value, error, reason }` — reject bad input, never mutate existing state (matches the repo's atomic-save convention).
- Critical vs recoverable: a bad content file = reject with reason logged; a corrupted save = refuse load, preserve file; sim invariant violation = `POTATO_ASSERT` in debug, logged+abort in release.

```cpp
// boundary example
Result<DoctrineCard> loadDoctrineCard(const std::string& path) {
    auto json = JsonValue::parseFile(path);
    if (!json.ok()) return fail("parse", json.error());
    if (json["schema"] != "potato.doctrine_card/1") return fail("schema", "bad version");
    return ok(DoctrineCard::fromJson(json));
}
```

### Logging

**Format:** engine `ILogger` (`Logging/Logger.h`), plain-text tagged lines `[LEVEL][subsystem] message`.

**Levels:** ERROR / WARN / INFO / DEBUG / TRACE (TRACE compiled out in release).

**Rules:**
- Zero logging inside the tick path — determinism and 60 FPS first; sim observability comes from the tick's emitted event list, not from logs.
- Log at boundaries only: file I/O, beat transitions (Planning→Execution→Aftermath), content validation, save/load.
- Headless test exes log to stdout; the Shell may also mirror to an in-game console.

### Configuration

**Three tiers, never mixed:**

| Tier | Storage | Examples |
|---|---|---|
| Constants | `constexpr` headers | tick rate, CP cap, array bounds |
| Balance values | versioned JSON (`potato.balance/<ver>`) | unit stats, cooldowns, fog decay, economy tempo |
| Player settings | save-slot JSON | volume, HUD density, keybinds |

Balance values must never be hardcoded in sim constants — tuning without rebuild is a requirement.

### Event System

**Two regimes, kept strictly separate:**

- **Inside sim:** each tick produces a typed `SimEvent` list appended to `BattleState.events` — the recorder consumes exactly this list (record-is-truth). Sim code never calls an external bus.
- **At layer boundaries:** engine `EventBus` (typed) carries Shell↔Gameplay notifications (phase changes, UI refresh, aftermath results). Subscribers must never write back into `BattleState`.

```cpp
// sim event (deterministic, recorded)
struct SquadRouted : SimEvent { SquadId id; Tick t; };

// boundary bus event (not recorded)
bus.emit<AftermathReady>(report);
```

### Debug Tools

- Headless sim driver test exe (same code path as production sim).
- Replay-verifier CLI: re-run a recorded event stream + seed, diff outcome hash.
- `POTATO_DEBUG` compile gate for all debug features; absent in release.
- ImGui debug overlay in Shell (battle-state inspector, fog certainty view) built on existing GUI infrastructure.

---

## Project Structure

### Organization Pattern

**Layered domain directories** — the repo's existing convention (one top-level dir per engine subsystem) extended by the D-ARCH-4 layering: `Gameplay` (pure sim) ← `Campaign` (cross-chapter) ← `Game` (shell) ← `Examples` (tests/demos).

### Directory Structure

```
MingGoRTS/
├── (existing engine dirs: Core/ ECS/ Events/ Rendering/ … — unchanged)
├── Gameplay/                  # pure sim lib — headless, no GL/Rendering/GUI
│   ├── Sim/                   # BattleState (POD arrays + serialize + hash), tick loop, seeded PRNG, SimEvent
│   ├── Map/                   # BattleMap: region/node graph, terrain, dual-layer markers
│   ├── Squad/                 # squad model, cohesion, movement, rout threshold
│   ├── Doctrine/              # card defs, interpreter, cooldowns, squad sheets
│   ├── Fog/                   # QuantumFog: certainty field, cloud entities, priors
│   ├── Plan/                  # BattlePlan arrows + certainty scaling
│   ├── Command/               # CP interventions
│   ├── Record/                # BattleRecorder: event stream + integrity root
│   ├── Governance/            # GovernanceField (battle-scope tracking)
│   ├── Json/                  # JsonValue DOM parser + schema guards
│   └── Result.h               # Result<T> boundary types
├── Campaign/                  # cross-chapter persistence lib (depends only on Gameplay public headers)
│   ├── State/                 # CampaignState facade, ChapterState
│   ├── Chapters/              # ChapterLibrary
│   ├── Roster/                # named squads, casualties, RefitCamp
│   ├── Ledger/                # five accounts + hash chain + forgery/suspect
│   ├── Governance/            # 民心/秩序/墮落 accumulators
│   ├── Rivals/                # GeneralDossier, RivalDeck, personality priors
│   ├── Narrative/             # IntelLedger, NarrativePack, ChapterConventions, EndingPage
│   ├── Myth/                  # infiltration 0–3, shrines, 天命, MythLog, GodStance
│   └── Save/                  # save slots, atomic write, schema gate
├── Game/                      # shell app (windowed) — binds Rendering/Input/GUI
│   ├── Render/                # SpriteAtlas, map view (Epic F)
│   └── Ui/                    # HUD / panels (Epic F)
├── assets/                    # read-only versioned content: potato.<name>/<ver>
│   ├── cards/ maps/ squads/ chapters/ narrative/ balance/ fonts/ avatars/
├── Examples/                  # existing + new POTATO_TESTS headless exes
└── docs/ _bmad-output/ _bmad/ .agents/ (existing)
```

### System Location Mapping

| System | Location | Notes |
|---|---|---|
| Doctrine cards + interpreter | `Gameplay/Doctrine` | data-driven; shared by both sides |
| BattleController / tick kernel | `Gameplay/Sim` | fixed 20 Hz deterministic |
| QuantumFog | `Gameplay/Fog` | certainty field + clouds |
| BattlePlan | `Gameplay/Plan` | arrows + certainty-scaled bonus |
| CP interventions | `Gameplay/Command` | ≤3s resolution |
| BattleRecorder | `Gameplay/Record` | event-sourced replay |
| GovernanceField (battle) | `Gameplay/Governance` | events feed campaign |
| JsonValue DOM | `Gameplay/Json` | all content files go through it |
| Roster / RefitCamp | `Campaign/Roster` | persistent casualties |
| Ledger + hash chain | `Campaign/Ledger` | 5 accounts, forgery/suspect |
| Governance accumulators | `Campaign/Governance` | 民心/秩序/墮落 |
| Rivals / RivalDeck / priors | `Campaign/Rivals` | hearsay model |
| Narrative systems | `Campaign/Narrative` | chaptered conventions, endings |
| Myth layer | `Campaign/Myth` | infiltration, shrines, MythLog, GodStance |
| Save system | `Campaign/Save` | atomic, schema-gated |
| Sprites / HUD / game UI | `Game/` | Epic F — deferred |

### Naming Conventions

- **Files:** `PascalCase.h` / `PascalCase.cpp` (matches engine style)
- **Namespaces:** `Potato::Gameplay`, `Potato::Campaign` (engine code stays `Potato::…`)
- **Classes / methods:** PascalCase (`BattleController`, `RunTick`)
- **Members / locals:** camelCase (`certaintyField`, `squadIndex`)
- **Constants:** `constexpr` UPPER_SNAKE (`TICK_RATE_HZ`, `CP_CAP`)
- **Schema strings:** `potato.<name>/<ver>` (`potato.doctrine_card/1`)
- **Test exes:** `potato_test_<name>`, registered in root `POTATO_TESTS`

### Architectural Boundaries

1. `Gameplay` must not include Rendering / GUI / platform-GL headers — checked by the IDE's existing dependency-direction guardrail plus review.
2. `Campaign` uses only `Gameplay` public headers; never reaches into `Gameplay` internals.
3. `Game/` may bind everything; nothing below it may include `Game/`.
4. All content-file access goes through `Gameplay/Json` — no ad-hoc parsing.
5. `assets/` is read-only at runtime; all writes go through `Campaign/Save` atomic path.

---

## Implementation Patterns

### Novel Patterns

#### Dual-Script Execution（雙腳本同拍執行）

**Purpose:** both sides run doctrine programs on identical machinery — execution order is part of the spec, not an implementation detail.

**Components:** `DoctrineInterpreter` (per-side), `SquadSheet[]`, `CardCooldown[]`, `SimEvent` sink.

**Data flow:** each tick → for each squad in canonical order (squad index, then slot index) → eval trigger → check condition → apply action → write cooldown → emit `SimEvent`. Player-side and AI-side share the same interpreter instance and tick order. No side may observe the other's *pending* tick decisions — cards see state at tick start.

```cpp
// canonical order — changing this changes the game
for (auto& sq : state.squads)
  for (auto& slot : sq.sheet.slots)
    interpreter.evalSlot(state, sq, slot, rng);
```

**Rule:** triggers/conditions read `BattleState` snapshot at tick start; actions write into `pendingDelta` applied after all slots eval — prevents intra-tick ordering exploits.

#### Probabilistic Intel（QuantumFog）

**Purpose:** UI and doctrines never read truth — only certainty.

**Components:** `CertaintyField` (per-map-region int 0–100), `CloudEntity[]` (superposed enemy positions), `PriorTable` (per-general bias), ops: `observe/probe/decay/entangle`.

**Data flow:** sim maintains ground truth privately; `CertaintyField` decays ~10/min; observation collapses clouds in radius to truth; probes add certainty without collapse; entangled clouds share fate. BattlePlan bonus = f(certainty over covered regions).

```cpp
// doctrine condition sees fog, not truth
bool cond_enemySpotted(ctx) {
    return ctx.fog.certaintyAt(ctx.region) >= 60; // balance JSON
}
```

**Rule:** nothing outside `Gameplay/Sim` may read true enemy positions — presentation renders clouds; a "true view" exists only in the debug overlay (`POTATO_DEBUG`).

#### Ledger-as-Gameplay

**Purpose:** the five-account double-entry ledger IS the campaign's state — derived stats are computed, never stored.

**Components:** `Ledger` (5 accounts, entries), `LedgerChain` (append-only, each entry hashes previous), `GovernanceAccumulators` (民心/秩序/墮落 derived by folding entries), `ForgeryChannel` (explicit inject path), `SuspectFlag` (persisted on entries).

**Data flow:** every battle/campaign action produces a **balanced entry pair** (credit in one account, debit in another). 秩序/墮落 are fold-functions over the entry chain — write paths only ever append. `verify()` replays the chain; broken chain = load rejection; forged entry = detectable but *kept* (suspicion is data, not an error).

```cpp
// burning a village — one action, two accounts
ledger.post({ .credit = {"物資", +40}, .debit = {"民心", -15},
              .tags = {"atrocity", villageId} });
// 墮落 ratchet = max over folded atrocity tags — computed, never written
```

### Standard Patterns

- **Communication:** sim emits typed `SimEvent` list per tick; `EventBus` (typed) only at layer boundaries; bus subscribers never write `BattleState`.
- **Entity creation:** template factory + BudgetedBuild — `SquadTemplate` JSON instantiates squads within a 物資 budget; per-item `skipReason` (`unknown_id`, `over_budget`) recorded, never silent.
- **State transitions:** `enum class` + explicit transition table for every stateful system (BattleBeat, InfiltrationLevel, GodStance); the table is the spec and is unit-testable.

```cpp
enum class BattleBeat { Planning, Execution, Aftermath };
constexpr bool canTransition(BattleBeat a, BattleBeat b) {
    return (a==Planning && b==Execution) || (a==Execution && b==Aftermath);
}
```

- **Data access:** boot-time registry load — all `potato.<name>/<ver>` JSON parsed once into typed registries (`CardRegistry`, `ChapterLibrary`, `BalanceTable`); read-only during battle; zero file I/O in tick path.

### Consistency Rules

| Rule | Convention | Enforcement |
|---|---|---|
| No floats in sim | int/fixed-point only | code review + `static_assert` on sim structs |
| Canonical eval order | squad idx → slot idx | single interpreter entry point |
| RNG discipline | one seeded stream, draws in order | PRNG owned by `Sim`, never global `rand()` |
| Truth stays in sim | fog-only reads outside `Sim/` | boundary review; debug overlay gated |
| Ledger append-only | accumulators are folds | no public write API on accumulators |
| Registry immutability | load at boot, read-only at runtime | `const` registry refs only |
| Schema gate | `potato.<name>/<ver>` checked on load | `Json` module rejects bad version |

---

## Architecture Validation

### Validation Summary

| Check | Result | Notes |
|---|---|---|
| Decision Compatibility | PASS | 16 decisions coherent; ECS/Physics/facade deliberately unused with recorded rationale |
| GDD Coverage | PASS | every named mechanic maps to an architecture location |
| Pattern Completeness | PASS | communication/creation/state/error/data/event all covered with examples |
| Epic Mapping | PASS | E0→Gameplay, B→Campaign State/Save/Roster/Rivals, A→Governance, L→Ledger, C→Narrative, D→Myth, E→doctrine paths, F→Game/, G→tools boundary |
| Document Completeness | PASS | executive summary + version note added during validation; no placeholders |

### Coverage Report

**Systems Covered:** 10/10 (all GDD core systems)
**Patterns Defined:** 7 (3 novel + 4 standard)
**Decisions Made:** 16 (8 architecture + 4 cross-cutting + 4 pattern)

### Issues Resolved

1. Missing executive summary → added.
2. Decision table lacked Version column → annotated (all in-house stack; toolchain C++20/CMake≥3.15/GL3.3).

### Deferred / Known Gaps

- Epic E negotiation/deterrence/subversion internals — detailed design belongs to that epic's spec stage (sequenced late).
- README-promised `JsonParser.h` and `potato.<name>/<ver>` schemas are defined by this architecture but do not exist yet — must be created during E0.
- Engine ECS/Physics/facade unused for sim by design; revisit ECS for the render view layer at Epic F.

### Validation Date

2026-09-29

---

## Development Environment

### Prerequisites

- CMake ≥ 3.15, C++20 compiler (MSVC via VS Developer Prompt, or MinGW; Linux g++ in CI)
- OpenGL 3.3+; GLFW fetched automatically by CMake FetchContent
- No vcpkg / Bullet / OpenAL — third-party is vendored `external/` only

### AI Tooling (MCP Servers)

None — custom in-repo engine; no MCP ecosystem exists. The MingGoRTS IDE's own AI-agent platform is the in-house equivalent.

### Setup Commands

```bash
cmake -B build -S .
cmake --build build --config Release
cd build && ctest -C Release
# MinGW: cmake -B build-mingw -S . -G "MinGW Makefiles" && cmake --build build-mingw && ctest --test-dir build-mingw
```

### First Steps

1. Scaffold `Gameplay/` module skeleton + CMake target + first headless test (E0-1).
2. Build `Gameplay/Json` — the `JsonValue` DOM parser all content depends on.
3. Implement `Sim/` kernel: `BattleState` POD arrays + fixed-tick loop + seeded PRNG (E0-2/3/5).
