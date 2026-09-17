---
title: 'Core systems test coverage: ECS / EventBus / ResourceCache'
type: 'feature'
created: '2026-09-17'
status: 'in-progress'
baseline_revision: 'db85b7ade599ad55bd9eea97e16934360ce4eada'
review_loop_iteration: 0
followup_review_recommended: false
context: []
warnings: []
deferred: []
---

<intent-contract>

## Intent

**Problem:** Weakness-scan item #8 — `ECS/`, `Events/EventBus`, and `Resources/ResourceManager` are gameplay-layer foundations with zero test coverage; no Example test touches any of them.

**Approach:** Add one headless smoke-test executable `CoreSystemsTest` (repo convention: `Examples/` + `Check()` macro + POTATO_TESTS registration) exercising each system's real public API surface.

## Boundaries & Constraints

**Always:** Follow the `Examples/TimeTest.cpp` Check/PASS-FAIL pattern; pure headless (no GL context, no window, no file I/O fixtures); register in `POTATO_TESTS` in root `CMakeLists.txt`; comment style Traditional Chinese where the file adds comments.

**Never:** Do not modify the systems under test (bugs found are reported, not fixed, unless a one-line obvious fix unblocks the test); do not touch `external/` or `build/`; do not instantiate `ResourceCache::Load<T>` (declared, never defined — link error); do not call `ProcessEventQueue` expecting delivery (queue is dead code — no `QueueEvent` API).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| ECS lifecycle | CreateEntity ×2, AddComponent, destroy one | Valid IDs from 1, recycled LIFO; `HasComponent`/`GetComponent` reflect state; `GetComponent` on missing throws `std::out_of_range` | Wrap in try/catch, assert throw |
| ECS systems | Two ISystem impls with different priorities | `Update(dt)` runs all registered systems; priority sort ascending | No error expected |
| EventBus dispatch | Subscribe<T> handler, Publish(event) | Handler invoked synchronously with correct payload; `GetHandlerCount` reflects subs | No error expected |
| EventBus re-entrancy | Handler that Publishes a second event | No deadlock; nested event delivered | No error expected |
| ResourceCache | Fake `IResourceLoader` returning sentinel ptr | `LoadRaw` refcount increments, `IsLoaded` true, `Unload` frees at refcount 0 (loader `Unload` called), `IsLoaded` false | Double-load returns cached ptr |

</intent-contract>

## Code Map

- `ECS/ECSCoordinator.h:16-96` — facade; stack-allocatable, no globals needed. `CreateEntity/DestroyEntity/IsEntityValid`, templated `AddComponent/GetComponent/HasComponent/RemoveComponent`, `AddSystem/RemoveSystem/GetSystem`, `Update(dt)` (no-op until `Initialize()`), `GetEntityCount`. NOTE: `AddComponent` does NOT update the Entity component mask — query via coordinator only.
- `ECS/Entity.h:12-18` — `MAX_COMPONENTS=64`, `INVALID_ENTITY=0`, IDs start at 1, recycled LIFO (`Entity.cpp:23-56`).
- `ECS/Component.h:48,102,114` — `struct Foo : Component<Foo>` CRTP; only multi-entity query is `ComponentArray<T>::GetAllComponents()` via `GetComponentArray<T>()`.
- `ECS/System.h:16-34` — `ISystem` (`Update` pure virtual, `GetPriority`, `GetName`); `SystemManager` sorts ascending by priority, snapshot iteration.
- `Events/EventBus.h:57-113` — `EventBus` directly constructible; `Subscribe<T>(EventHandler<T>)`, `Publish` synchronous + snapshot (re-entrant safe); only `IEventListener` can be unsubscribed. `Event<T>` CRTP base at :27. `GameEvents::` predefined types at :188-253.
- `Resources/ResourceManager.h:88-113` — `ResourceCache` is the testable unit (public class): `LoadRaw`, `RegisterLoader`, `IsLoaded`, `Unload`, `UnloadAll`, `GetLoadedResourceCount`. `IResourceLoader` at :38 (`Load(path, void**)`, `Unload(void*)`, `GetType()`). `ResourceManager` itself only good for init/path-resolution — `LoadTexture` etc. return nullptr (no loaders exist in repo).
- `Examples/TimeTest.cpp` — test-skeleton template (`g_pass/g_fail`, `Check(bool, name)`).
- `CMakeLists.txt` `POTATO_TESTS` list (~line 692) — add `CoreSystemsTest`; add `add_executable` + `target_link_libraries(... PotatoEngine)` near other test targets.

## Tasks & Acceptance

**Execution:**
- `Examples/CoreSystemsTest.cpp` -- new headless test covering all I/O-matrix scenarios: ECS entity/component lifecycle + missing-component throw + system priority ordering; EventBus subscribe/publish/counts + nested-publish re-entrancy + listener unsubscribe; ResourceCache fake-loader load/refcount/unload-free + double-load caching -- single new file keeps the test self-contained.
- `CMakeLists.txt` -- add `CoreSystemsTest` executable linking `PotatoEngine` and append to `POTATO_TESTS` -- registers it for ctest.

**Acceptance Criteria:**
- Given the test binary built, when run headless, then it prints `PASS: N / FAIL: 0` and exits 0.
- Given `cd build && ctest -C Release -R CoreSystemsTest`, when executed, then the test reports Passed.
- Given MinGW `g++ -fsyntax-only` on the new file with repo include paths, when compiled, then no errors.

## Spec Change Log

## Review Triage Log

## Design Notes

Testable-seam choices (from investigation): `ECSCoordinator` used directly, not `gECSCoordinator` globals — avoids global init ordering. `ResourceManager` is tested only for `Initialize`/path resolution because every `LoadTexture`-family call returns nullptr (no `IResourceLoader` exists in-repo); `ResourceCache` with a local fake loader is where load/unload semantics live. Fake loader returns a heap `int` sentinel cast through `void*` — no real asset files needed.

## Verification

**Commands:**
- `cmake --build build --config Release --target CoreSystemsTest` -- expected: builds clean
- `./build/bin/Release/CoreSystemsTest.exe` -- expected: `FAIL: 0`, exit 0
- `cd build && ctest -C Release -R CoreSystemsTest` -- expected: Passed
- `g++ -std=c++20 -fsyntax-only -I. Examples/CoreSystemsTest.cpp` (MinGW) -- expected: no errors
