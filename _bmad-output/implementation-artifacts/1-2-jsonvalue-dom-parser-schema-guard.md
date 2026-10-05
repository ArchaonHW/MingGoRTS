---
baseline_commit: NO_VCS
---

# Story 1.2: JsonValue DOM Parser & Schema Guard

Status: done

## Story

As a developer,
I want a game-layer JSON DOM parser with `potato.<name>/<ver>` schema gating and `Result<T>` returns,
So that all content files load through one validated path.

## Acceptance Criteria

1. **Given** a well-formed `potato.test/1` file,
   **When** loaded via `Json::Load`,
   **Then** a typed value returns.
2. **And Given** a bad schema tag or malformed JSON,
   **When** loaded,
   **Then** `Result` carries an error reason and existing state is untouched.

## Tasks / Subtasks

- [x] Task 1 — `JsonValue` DOM type (AC: 1)
  - [x] `Gameplay/Json/JsonValue.h` — DOM node: null / bool / int64 / double / string / array / object. Object = `std::map` (ordered keys, stable; file order not semantically relevant for registries)
  - [x] Typed accessors implemented; mismatches return fallback, never throw
  - [x] int64 vs double split at parse time per literal shape
- [x] Task 2 — Recursive-descent parser (AC: 1, 2)
  - [x] `JsonValue::Parse(string_view)` — full RFC 8259, `\uXXXX` + surrogate pairs → UTF-8, leading-zero rejection, dup keys last-wins, depth cap 64, trailing garbage rejected, never throws
- [x] Task 3 — Schema gate + file load (AC: 1, 2)
  - [x] `Json::Load(path, expectedSchema)` — `std::ifstream`, BOM strip, io/parse/schema `Fail()` classes, pure function (nothing mutated on rejection)
- [x] Task 4 — Headless test `potato_test_json` (AC: 1, 2)
  - [x] `Examples/potato_test_json.cpp` — 27 checks covering both ACs + hardening
  - [x] CMake target + `add_test` wired (ctest now 3 tests)
  - [x] `Gameplay/Json/.gitkeep` removed
- [x] Task 5 — Verify (AC: 1, 2)
  - [x] MinGW via junction: build green; `ctest` 3/3 pass
  - [x] `gameplay_dep_guard` green
  - [ ] MSVC: unverifiable in this environment (no `cl`)

## Dev Notes

### Prior story intelligence (1.1 — just landed)

- `Gameplay/` skeleton exists: `Sim/` (SplitMix64 PRNG — use it as the integer-only, no-throw style reference), `Result.h` (**updated in review**: `ok()` is a *method*, `ok_` the member — call shape `json.ok()`, `json.error`, `json.reason`; `Ok<T>(v)`/`Fail<T>(e,r)` helpers).
- `Result<T>` requires `T` default-constructible — `JsonValue` must be default-constructible. There is no `Result<void>` (deferred item).
- Build wiring exists: `PotatoGameplay` lib globs `Gameplay/**/*.cpp` with `CONFIGURE_DEPENDS` — new files auto-pickup. Test exes + `add_test` pattern established at `CMakeLists.txt` end.
- Environment: repo path has CJK — build through junction `C:\MingGoRTS`. `PotatoEngine` lib deliberately NOT linked (Security MinGW breakage) — Gameplay stays self-contained; `std::ifstream` is the file I/O path.
- `POTATO_BUILD_GUI=OFF` for headless builds.

### Architecture constraints that apply

- Boundary code (this story IS boundary code): `Result<T>` everywhere; never throw. `POTATO_ASSERT` not yet defined — don't invent it; pure `Result` flow is enough here.
- No third-party JSON. No unsafe C functions (`scanf` family banned anyway — parser is `string_view` walking, trivially safe).
- Schema gate is load-bearing: `potato.<name>/<ver>` is THE content contract; every future registry (cards, maps, squads, chapters, balance) goes through `Json::Load`. Keep the API surface minimal — registries and typed loaders are later stories.
- UTF-8: file bytes pass through as UTF-8 (`\uXXXX` decodes to UTF-8 bytes in `std::string`); CJK content is coming — do not "normalize" or transcode.
- `assets/` does not exist yet — do NOT create it. Test fixtures live inline in the test exe + temp files.
- Naming: `PascalCase` files/classes/methods, `camelCase` members, `Potato::Gameplay` namespace, `UPPER_SNAKE` constexpr.

### LLM-trap warnings

- Do NOT reach for nlohmann/rapidjson/any third-party parser — banned by policy.
- Do NOT enable `FileSystem.cpp`/`Serialization` — they're disabled repo-wide for being broken; `std::ifstream` at the boundary is correct.
- Do NOT parse numbers lazily as strings — DOM must distinguish int64 vs double now (balance JSON needs both).
- `Get-Content`-style encoding traps don't apply (C++); but parser must accept UTF-8 BOM at file start — strip `\xEF\xBB\xBF` in `Load` if present (content files may be authored on Windows).
- Depth limit prevents stack overflow on hostile/deep input — enforce in recursion, not via `catch(...)`.

### Project Context Rules

- No Rendering/GUI/Platform/GL includes — dep guard runs in CTest now.
- Root-relative includes: `#include "Gameplay/Json/JsonValue.h"`.
- `potato_test_<name>` + `add_test` — now part of the CTest suite (3 tests after this story).

### References

- [Source: _bmad-output/planning-artifacts/epics.md — Epic 1 / Story 1.2]
- [Source: _bmad-output/game-architecture.md — D-ARCH-5 JSON parser decision, Error Handling (`Result<T>` call shape), Consistency Rules (schema gate)]
- [Source: _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md — Technical Specs: versioned JSON, atomic rejection, no third-party]
- [Source: Gameplay/Result.h — boundary result type (updated call shape)]
- [Source: 1-1 story file — environment pitfalls, CTest pattern]

### Review Findings

- [x] [Review][Patch] `sscanf("%lf")` violates banned-C-functions policy + locale-dependent + 63-char token truncation + accepts `1e999`→inf — replace with `std::from_chars` (int64 & double), reject non-finite [Gameplay/Json/JsonValue.cpp]
- [x] [Review][Patch] `AsInt` UB casting non-finite/out-of-range Real → int64; also drop undocumented Bool→0/1 coercion [Gameplay/Json/JsonValue.cpp]
- [x] [Review][Patch] `INT64_MIN` rejected by magnitude-then-negate accumulation — from_chars fixes range handling [Gameplay/Json/JsonValue.cpp]
- [x] [Review][Patch] `Load` narrow ifstream fails non-ASCII paths on Windows (repo path itself is CJK); use `std::filesystem::path` from u8string [Gameplay/Json/Json.cpp]
- [x] [Review][Patch] `Load` hardening — input-size cap, post-read `in.bad()` check, UTF-16 BOM → explicit unsupported-encoding error [Gameplay/Json/Json.cpp]
- [x] [Review][Patch] `Object` map lacks transparent comparator — `std::less<>` enables true string_view find, kills per-lookup alloc [Gameplay/Json/JsonValue.h/.cpp]
- [x] [Review][Patch] Test gaps — assert `error`/`reason` non-empty on rejects; add INT64_MIN, `1e999` reject, lone-surrogate rejects, AsInt-on-huge-Real fallback [Examples/potato_test_json.cpp]
- [x] [Review][Dismissed] signed-char control-char check / `Literal()` overread / copy-heavy parse — code already casts `unsigned char`, uses bounds-clamping `substr`, and `std::move`s into containers
- [x] [Review][Defer] Raw bytes ≥0x80 not UTF-8-validated — accepted leniency per RFC 8259; escaped paths are strict
- [x] [Review][Defer] `\u0000` embeds NUL in strings — spec-legal; c_str consumers beware (documented)
- [x] [Review][Defer] JsonValue node size / API footguns (null singleton, dangling FindString refs) — documented behavior, revisit if hurt
- [x] [Review][Defer] ctest fixture WORKING_DIRECTORY implicit — lands in build dir, cleaned unconditionally

## Dev Agent Record

### Agent Model Used

SWE-2 High (Devin)

### Debug Log References

- `using Potato::Gameplay::Json;` rejected by GCC ("using-declaration may not name namespace") — fixed with `namespace Json = Potato::Gameplay::Json;` alias in the test.
- New CMake target required explicit re-configure before `--target` build (new add_executable isn't picked up by CONFIGURE_DEPENDS, which only covers source globs).

### Completion Notes List

- `JsonValue` DOM: tagged-union style class (Type enum + POD fields + string/array/object storage); `Object = std::map` for ordered iteration; defensive accessors (Null singleton + fallbacks).
- Parser: single-pass recursive descent, position-tagged errors, max depth 64, full escape set + surrogate-pair decoding to UTF-8, strict number grammar (leading zeros rejected, int64 overflow checked), duplicate keys last-wins.
- `Json::Load` gate chain: io → parse → object root → string "schema" → exact match; pure function, no mutation on reject. BOM stripped pre-parse.
- Post-review: number path fully on `std::from_chars` (locale-free, INT64_MIN-safe, non-finite rejected, huge int literals degrade to Real); `AsInt` UB-guarded; `Load` hardened (u8 filesystem path, 64 MiB cap, `in.bad()` check, UTF-16 explicit reject); `Object` got `std::less<>` transparent lookup.
- 38 checks green; ctest suite 3/3 (`potato_test_gameplay`, `potato_test_json`, `gameplay_dep_guard`).
- MSVC still unverified (no cl); code is stdlib-only.
- Ultimate context engine analysis completed - comprehensive developer guide created

### File List

- `Gameplay/Json/JsonValue.h` (new)
- `Gameplay/Json/JsonValue.cpp` (new)
- `Gameplay/Json/Json.h` (new)
- `Gameplay/Json/Json.cpp` (new)
- `Examples/potato_test_json.cpp` (new)
- `CMakeLists.txt` (modified — potato_test_json target + add_test)
- `Gameplay/Json/.gitkeep` (deleted — real files landed)
