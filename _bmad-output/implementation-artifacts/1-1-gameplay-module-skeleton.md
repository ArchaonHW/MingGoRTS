---
baseline_commit: NO_VCS
---

# Story 1.1: Gameplay Module Skeleton

Status: done

## Story

As a developer,
I want a Gameplay/ module with CMake target, headless test harness, and layered dependency guards,
So that all game code has a deterministic, GL-free home.

## Acceptance Criteria

1. **Given** a clean checkout,
   **When** `potato_test_gameplay` builds and runs,
   **Then** it links only engine leaf modules (Events, FileSystem, Logging, MathUtils) and passes a smoke tick.
2. **And** `Gameplay/` sources compile with no Rendering/GUI/Platform-GL includes (verified by dependency check).

## Tasks / Subtasks

- [x] Task 1 — Create `Gameplay/` skeleton directories (AC: 1, 2)
  - [x] `Gameplay/Sim/`, `Gameplay/Map/`, `Gameplay/Squad/`, `Gameplay/Doctrine/`, `Gameplay/Fog/`, `Gameplay/Plan/`, `Gameplay/Command/`, `Gameplay/Record/`, `Gameplay/Governance/`, `Gameplay/Json/`
  - [x] `Gameplay/Result.h` — `Result<T>{ value, error, reason }` boundary type (per architecture Error Handling; needed by Story 1.2 immediately)
  - [x] Minimal smoke content only: `Gameplay/Sim/Sim.h/.cpp` exposing seeded `Tick()` + `Checksum()` — enough to prove compile+link+run; no BattleState (Story 1.3/1.4 scope)
- [x] Task 2 — Add `PotatoGameplay` static library target to root `CMakeLists.txt` (AC: 1)
  - [x] `add_library(PotatoGameplay STATIC ...)` globbing `Gameplay/**/*.cpp` + headers (follows ENGINE_SOURCES/ENGINE_HEADERS pattern)
  - [x] `target_include_directories(PotatoGameplay PUBLIC ${CMAKE_SOURCE_DIR})` — root-relative includes
  - [x] Engine link: NOT linked — see Dev Notes + Completion Notes (PotatoEngine lib uncompilable under MinGW via SecuritySystem.cpp)
- [x] Task 3 — Add `potato_test_gameplay` headless test executable (AC: 1)
  - [x] `Examples/potato_test_gameplay.cpp` — smoke tick: same seed → same checksum, different seed → different checksum; returns non-zero on failure
  - [x] `add_executable(potato_test_gameplay ...)` following MathTest/SecurityRedTeamTest pattern
  - [x] `potato_test_<name>` convention used; POTATO_TESTS/CTest left as TODO comment in CMakeLists (does not exist yet)
- [x] Task 4 — Dependency-direction check (AC: 2)
  - [x] `scripts/CheckGameplayDeps.ps1` — greps `Gameplay/**/*.{h,cpp}` for forbidden includes (Rendering/GUI/Platform/OpenGL/glad/glfw/ImGui), exit 1 on hit
  - [x] Ran green: `PASS: 3 files clean`
- [x] Task 5 — Verify compilers (AC: 1, 2)
  - [x] MinGW: `cmake -B build-mingw -S . -G "MinGW Makefiles" -DPOTATO_BUILD_GUI=OFF` + build + run — PASS (`SMOKE TICK PASS`, exit 0)
  - [ ] MSVC: not verifiable in this environment (no `cl` on PATH; BuildEngine.bat requires VS Developer Prompt) — flagged in Completion Notes

## Dev Notes

### Leaf-module reality (READ FIRST — differs from AC's literal wording)

The engine static lib `PotatoEngine` currently compiles **only** `Core/*.cpp` + `Security/*.cpp`. Every other subsystem's `.cpp` glob is commented out in `CMakeLists.txt` (lines 110–131) with reasons: "structural issues", "depends on Math", encoding problems. Concretely:

| Module the AC names | Reality | What to do |
|---|---|---|
| `MathUtils` | Header-only (6 headers, 0 cpps) — `Vector2/3/4`, `Matrix4`, `Quaternion`, `MathUtils.h` | Include directly; no link needed |
| `Events` | `EventBus.cpp` disabled ("structural issues") | Header may be included; do NOT enable its cpp. Boundary EventBus use is deferred anyway — sim uses typed `SimEvent` lists |
| `FileSystem` | `FileSystem.cpp` disabled | No file I/O in tick path; do NOT enable its cpp |
| `Logging` | `Logger.cpp` disabled; `Logger.h` includes `Core/Interfaces/ILogger.h` | Use the `ILogger` interface from `Core/Interfaces/` if a log boundary is needed; do NOT enable `Logger.cpp` |

So "links only engine leaf modules" is satisfied by: link `PotatoEngine` (provides `Core`/`Security` objects) + use `MathUtils` headers. **Do not uncomment the disabled globs** — they are marked broken; rehabilitating engine modules is out of scope.

### Encoding pitfall

`Logging/Logger.h` (and likely other headers) contain comments in a legacy Chinese encoding that renders as mojibake under UTF-8. Do not bulk-reformat or "fix" comments in engine headers — mojibake there is pre-existing; touching it risks breaking MinGW builds (the CMake comments cite encoding issues as a disable reason).

### Architecture constraints that apply from day one

- `Gameplay/` must compile headless: no Rendering/GUI/Platform-GL/ImGui includes — enforced by Task 4's check.
- No floating point in sim code (`static_assert` on sim structs is the documented enforcement); integer/fixed-point only.
- One seeded PRNG owned by `Sim`; never `rand()`. The smoke tick should take a seed parameter to prove this convention now.
- Determinism target: same seed + same input → identical result across MSVC/MinGW (smoke tick asserts this).
- Naming: `PascalCase` files/classes/methods; `camelCase` members; namespace `Potato::Gameplay`; `constexpr` UPPER_SNAK constants.
- No exceptions in tick path; `Result<T>` at boundaries (create `Result.h` now — Story 1.2 will consume it).
- No third-party JSON libs; `Gameplay/Json/JsonValue` comes in Story 1.2 — do not stub a different JSON approach.
- Facade (`PotatoEngine.h` subsystem accessors) is disabled by design — bind leaf modules; do not "fix" the facade.

### Project Structure Notes

- Root-relative includes (`#include "Gameplay/Sim/Sim.h"`) — `target_include_directories(... ${CMAKE_SOURCE_DIR})` matches every existing target.
- `Gameplay/` does not exist yet — this story creates it. `Campaign/`, `Game/`, `assets/` stay absent; later epics create them.
- The dependency check script location `scripts/` is new — the repo has no scripts dir; a plain `.ps1` is fine (env note: python/git are NOT guaranteed on this machine).

### Project Context Rules

From `AGENTS.md` (load-bearing for this story):

- PRs only, Conventional Commits; never push `main`/`develop` directly.
- Never modify `external/`.
- No unsafe C functions: `gets`, `strcpy`, `strcat`, `sprintf`, `vsprintf`, `scanf`.
- MinGW toolchain specifics guarded by `if(WIN32 AND NOT MSVC)`.
- Zero file I/O in the tick path; registries immutable during battle.
- Truth boundary: nothing outside `Gameplay/Sim/` reads true enemy state (no-op here, but keep the shape).

### References

- [Source: _bmad-output/planning-artifacts/epics.md — Epic 1 / Story 1.1]
- [Source: _bmad-output/game-architecture.md — Project Structure, Cross-cutting Concerns, Engine Binding, Consistency Rules]
- [Source: CMakeLists.txt:110-178 — ENGINE_SOURCES/HEADERS glob, disabled subsystems, PotatoEngine target]
- [Source: CMakeLists.txt:185-192,307-314 — example/test target pattern]
- [Source: AGENTS.md — Policy, Running and verifying, Conventions]

### Review Findings

- [x] [Review][Patch] Seed aliasing — Sim(0) ≡ Sim(0x9E3779B9) collapse; switch PRNG to splitmix64 (zero-safe, injective, no remap branch) [Gameplay/Sim/Sim.cpp]
- [x] [Review][Patch] Dep-check token gaps — misses `GL/`, `GLES`, `KHR/` headers and backslash separators (`Platform\foo.h`) [scripts/CheckGameplayDeps.ps1]
- [x] [Review][Patch] Single-line file evasion — scalar `Get-Content` indexes chars, not lines [scripts/CheckGameplayDeps.ps1]
- [x] [Review][Patch] Guard & test unwired — add `enable_testing()` + `add_test` for potato_test_gameplay and dep-check script (WIN32 powershell) [CMakeLists.txt]
- [x] [Review][Patch] `Result::ok` field vs architecture's `ok()` call shape — make it a method [Gameplay/Result.h]
- [x] [Review][Patch] `kTicks` violates UPPER_SNAKE constant convention → `TICKS` [Examples/potato_test_gameplay.cpp]
- [x] [Review][Patch] GLOB without CONFIGURE_DEPENDS + 9 empty dirs won't survive clone → add CONFIGURE_DEPENDS + `.gitkeep` [CMakeLists.txt, Gameplay/]
- [x] [Review][Defer] static_assert no-float enforcement absent — deferred, nothing to assert until Story 1.3 BattleState
- [x] [Review][Defer] PotatoEngine link / leaf-module exercise vacuous — deferred, SecuritySystem.cpp MinGW breakage is pre-existing
- [x] [Review][Defer] BuildEngine.bat can't pass `-DPOTATO_BUILD_GUI=OFF` — deferred, junction workaround documented in AGENTS.md

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

- MinGW `mingw32-make` fails outright under the repo path (`F:\民國史詩\` — "Illegal byte sequence" on source stat). Workaround used: directory junction `C:\MingGoRTS -> F:\民國史詩\HWC\MingGoRTS`; configure/build there.
- `Security/SecuritySystem.cpp` (one of only two engine .cpp globs enabled) does not compile under MinGW: `CALLBACK` field declared void, `std::ifstream(std::wstring)`, member-function-pointer↔callback casts. Pre-existing; NOT fixed by this story.

### Completion Notes List

- Created `Gameplay/` skeleton (10 dirs), `Result.h`, and `Sim/` kernel shell: seeded xorshift32 PRNG owned by `Sim`, integer-only `Tick()` with SplitMix64 checksum, `TICK_RATE_HZ = 20`.
- Added `PotatoGameplay` static lib + `potato_test_gameplay` exe to root `CMakeLists.txt`.
- **Deviations from the literal plan (all documented, none silent):**
  1. `PotatoGameplay` does NOT link `PotatoEngine`: its only enabled implementations (`Core/*.cpp`, `Security/*.cpp`) fail under MinGW (SecuritySystem.cpp MSVC-isms). The leaf modules named in the AC (Events/FileSystem/Logging) ship headers only — their .cpps are repo-disabled. Link restored when engine leaf targets exist or Security builds clean.
  2. Added `option(POTATO_BUILD_GUI ...)` (default ON — existing behavior unchanged) wrapping `find_package(OpenGL)` + glfw/glad FetchContent + `MingGoRTS_IDE_GUI`. Rationale: headless Gameplay must configure without git/network/GL (FetchContent uses GIT_REPOSITORY; git absent here; CJK path also breaks FetchContent subbuilds). `-DPOTATO_BUILD_GUI=OFF` enables headless builds.
- Verified: MinGW build green; `potato_test_gameplay` → `SMOKE TICK PASS` (exit 0); `scripts/CheckGameplayDeps.ps1` → `PASS: 3 files clean`.
- **MSVC verification outstanding**: no `cl` in this environment. The code is pure C++20 stdlib (no platform calls), so MSVC risk is low, but the dual-compiler convention is only half-verified.
- **Post-review (2026-09-29):** all 7 patch findings applied — PRNG → splitmix64 (seed 0 covered by new test), dep-check tokens + normalization + single-line fix, CTest wired (`potato_test_gameplay` + `gameplay_dep_guard`, 2/2 pass), `Result::ok()` method per arch call shape, `TICKS` naming, `CONFIGURE_DEPENDS` + `.gitkeep`. Re-verified: MinGW build green, ctest 2/2.
- Ultimate context engine analysis completed - comprehensive developer guide created

### File List

- `Gameplay/Result.h` (new)
- `Gameplay/Sim/Sim.h` (new)
- `Gameplay/Sim/Sim.cpp` (new)
- `Gameplay/{Map,Squad,Doctrine,Fog,Plan,Command,Record,Governance,Json}/` (new empty dirs)
- `Examples/potato_test_gameplay.cpp` (new)
- `scripts/CheckGameplayDeps.ps1` (new)
- `CMakeLists.txt` (modified — PotatoGameplay + potato_test_gameplay targets; POTATO_BUILD_GUI option)
