---
title: 'Core systems test coverage: ECS / EventBus / ResourceCache'
type: 'feature'
created: '2026-09-17'
status: 'done'
baseline_revision: 'db85b7ade599ad55bd9eea97e16934360ce4eada'
review_loop_iteration: 0
followup_review_recommended: false
context: []
warnings: []
deferred:
  - summary: >-
      ResourceCache::Load<T> is declared but never defined — any instantiation is a guaranteed link error.
    evidence: |-
      Resources/ResourceManager.h:94-95 declares the template; no definition exists anywhere in the repo.
    location: >-
      Resources/ResourceManager.h:94
    severity: medium
  - summary: >-
      EventBus eventQueue / ProcessEventQueue / ClearEventQueue / GetQueuedEventCount are dead code — no QueueEvent API exists.
    evidence: |-
      Events/EventBus.cpp:39-101 and EventBus.h:106-113 expose queue processing but nothing ever enqueues.
    location: >-
      Events/EventBus.h:106
    severity: low
  - summary: >-
      ResourceCache keys resources by path only — loading the same path as a different ResourceType returns the wrong-typed pointer and corrupts metadata.
    evidence: |-
      ResourceManager.cpp:47-58 checks path before type; resources/metadata maps at ResourceManager.h:125-126 key on path alone.
    location: >-
      Resources/ResourceManager.cpp:47
    severity: medium
  - summary: >-
      ResourceCache stores a null resource when a loader returns true but leaves *resource == nullptr — IsLoaded becomes true on a null entry.
    evidence: |-
      ResourceManager.cpp:68-84 only checks the bool return and writes into resources unconditionally.
    location: >-
      Resources/ResourceManager.cpp:68
    severity: medium
  - summary: >-
      ResourceCache holds a non-recursive mutex while invoking loader Load/Unload callbacks — a loader calling back into the cache deadlocks.
    evidence: |-
      ResourceManager.cpp:47-115,178-205 call loader hooks under lock; EventBus-style snapshot-outside-lock pattern is not used.
    location: >-
      Resources/ResourceManager.cpp:47
    severity: medium
  - summary: >-
      ECSCoordinator::AddComponent never updates Entity::componentMask — Entity::HasComponent/GetComponentMask always report empty for coordinator-added components.
    evidence: |-
      ECS/ECSCoordinator.h:36-37 admits this in-comment; Entity is passed by value so the mask cannot be updated through it.
    location: >-
      ECS/ECSCoordinator.h:36
    severity: medium
  - summary: >-
      Entity handles carry no generation counter — a stale handle re-validates once its ID is recycled by a new entity.
    evidence: |-
      ECS/Entity.h stores only the ID; EntityManager tracks validity per ID (Entity.cpp:23-55), so old copies of a recycled Entity become "valid" again.
    location: >-
      ECS/Entity.cpp:23
    severity: medium
  - summary: >-
      ECSCoordinator::AddComponent does not check IsEntityValid — destroyed or forged entity IDs silently write into component maps.
    evidence: |-
      ECS/ECSCoordinator.h:31-38 forwards to ComponentManager without a validity guard.
    location: >-
      ECS/ECSCoordinator.h:31
    severity: medium
  - summary: >-
      EventBus function handlers cannot be unsubscribed, and SubscribeListener allows duplicate registration while UnsubscribeListener removes only the first match.
    evidence: |-
      Events/EventBus.h:63-73 — only IEventListener* has an unsubscribe path; handler vectors accumulate.
    location: >-
      Events/EventBus.h:63
    severity: medium
  - summary: >-
      ResourceManager has no public loader-registration seam — Load*/Get*/Unload* success paths are untestable without modifying the class.
    evidence: |-
      ResourceManager.h:140-180 — cache is private with no RegisterLoader pass-through; every LoadX returns nullptr in-repo.
    location: >-
      Resources/ResourceManager.h:140
    severity: medium
  - summary: >-
      ResourceManager::UnloadUnusedResources frees everything regardless of refcount, and the GetX family actually loads + increments refcount on every call.
    evidence: |-
      ResourceManager.cpp:382-385 (calls cache->UnloadAll, "簡化實現"), :288-380 (GetX → LoadRaw).
    location: >-
      Resources/ResourceManager.cpp:382
    severity: medium
  - summary: >-
      ResolvePath treats any path containing ':' as absolute — POSIX-relative names like 'foo:bar.png' bypass resolution.
    evidence: |-
      ResourceManager.cpp:425-449 uses path.find(':') instead of a platform-aware is_absolute check.
    location: >-
      Resources/ResourceManager.cpp:431
    severity: low
  - summary: >-
      ResourceCache hardcodes 1KB per resource — SetMaxMemory/EnableAutoUnload/SetAutoUnloadThreshold have no trustworthy behavior.
    evidence: |-
      ResourceManager.cpp:80-84 assigns a fixed size; refcount-0 resources are already freed eagerly so CheckMemoryUsage rarely has anything to collect.
    location: >-
      Resources/ResourceManager.cpp:80
    severity: low
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

### 2026-09-17 — Review pass
- verdicts: 28 findings — high 0, medium 0, low 4, false 7, maybe-false 0 (13 pre-existing defects deferred)
- findings:
  - `[low]` `[defer]` ResourceCache::Load<T> declared-never-defined link trap — pre-existing defect in ResourceManager.h:94, only surfaced by the test comment; recorded in deferred list.
  - `[low]` `[defer]` EventBus eventQueue dead API (no QueueEvent) — pre-existing; deferred.
  - `[low]` `[defer]` ResourceCache path-only key allows cross-type collision — pre-existing defect; deferred (severity noted medium in deferred list).
  - `[low]` `[defer]` loader true+nullptr stored as loaded — pre-existing missing guard; deferred.
  - `[low]` `[defer]` ResourceCache mutex held during loader callbacks — re-entrant deadlock, pre-existing; deferred.
  - `[low]` `[defer]` ECSCoordinator::AddComponent doesn't update Entity mask — pre-existing admitted gap; deferred.
  - `[low]` `[defer]` stale Entity handles re-validate after ID recycle (no generation) — pre-existing design gap; deferred.
  - `[low]` `[defer]` AddComponent lacks IsEntityValid check — pre-existing; deferred.
  - `[low]` `[defer]` EventBus function-handler unsubscribe missing + duplicate listener semantics — pre-existing; deferred.
  - `[low]` `[patch]` test lacked RemoveSystem + Initialize/Shutdown lifecycle coverage — patched: RecordingSystem counts init/shutdown/update; RemoveSystem covered; updateCount asserted. PASS 59/0.
  - `[low]` `[defer]` ResourceManager has no loader-registration seam — pre-existing API gap; deferred.
  - `[low]` `[defer]` UnloadUnusedResources frees referenced resources; GetX family loads + refcounts — pre-existing; deferred.
  - `[low]` `[defer]` ResolvePath ':' heuristic misjudges POSIX names — pre-existing; deferred.
  - `[low]` `[defer]` hardcoded 1KB resource size makes memory APIs decorative — pre-existing; deferred.
  - `[low]` `[patch]` LIFO recycle order hardcoded as contract — patched: now asserts membership in destroyed-ID set {1,2} + no collision + fresh ID 3.
  - `[low]` `[patch]` unguarded GetComponent calls could abort binary on regression instead of FAIL — patched: component ops wrapped in try/catch recording Check(false).
  - `[low]` `[defer]` GetX-loads-and-refcounts semantics (verification-gap duplicate of the UnloadUnusedResources finding) — deferred.
  - `[low]` `[defer]` UnloadUnusedResources-UnloadAll (verification-gap duplicate) — deferred.
  - `[low]` `[defer]` dead eventQueue (verification-gap duplicate) — deferred.
  - `[low]` `[defer]` Load<T> undefined (verification-gap duplicate) — deferred.
  - `[low]` `[defer]` Entity mask unmaintained (verification-gap duplicate) — deferred.
  - `[false]` `[reject]` ResourceCache-vs-ResourceManager surface divergence — spec's Design Notes explicitly re-designate ResourceCache as the testable seam because no IResourceLoader exists in-repo; divergence is spec-sanctioned, not a defect.
  - `[false]` `[reject]` unexercised public API subset (GetAllComponents, memory APIs, globals) — spec's matrix defines the required surface; broader coverage is beyond intent.
  - `[false]` `[reject]` filesystem::exists touch during LoadTexture — assertion holds regardless of probe results; no fixture needed, harmless.
  - `[false]` `[reject]` acceptance text "PASS: N / FAIL: 0" vs two-line output — mandated TimeTest convention takes precedence over literal wording; cosmetic.
  - `[false]` `[reject]` engine status prints interleave with PASS lines — exit code and ctest result unaffected.
  - `[low]` `[patch]` dead test fields (updateCount unasserted) — patched alongside lifecycle coverage. (GetType-override unexercised is inherent: RegisterLoader takes the type argument.)
  - `[false]` `[reject]` globals (gECSCoordinator etc.) untested — spec explicitly forecloses Reading D to avoid global init ordering.

## Design Notes

Testable-seam choices (from investigation): `ECSCoordinator` used directly, not `gECSCoordinator` globals — avoids global init ordering. `ResourceManager` is tested only for `Initialize`/path resolution because every `LoadTexture`-family call returns nullptr (no `IResourceLoader` exists in-repo); `ResourceCache` with a local fake loader is where load/unload semantics live. Fake loader returns a heap `int` sentinel cast through `void*` — no real asset files needed.

## Verification

**Commands:**
- `cmake --build build --config Release --target CoreSystemsTest` -- expected: builds clean
- `./build/bin/Release/CoreSystemsTest.exe` -- expected: `FAIL: 0`, exit 0
- `cd build && ctest -C Release -R CoreSystemsTest` -- expected: Passed
- `g++ -std=c++20 -fsyntax-only -I. Examples/CoreSystemsTest.cpp` (MinGW) -- expected: no errors

## Auto Run Result

Status: done

Summary: Added `CoreSystemsTest` — a self-contained headless smoke test covering ECS (entity lifecycle, component ops, system priority/lifecycle), EventBus (synchronous dispatch, nested-publish re-entrancy, listener unsubscribe), and ResourceCache/ResourceManager (fake-loader refcount/caching/unload, init + path resolution). Registered in `POTATO_TESTS` for ctest.

Files changed:
- `Examples/CoreSystemsTest.cpp` — new 300+ line headless test, 59 checks, TimeTest-style Check/PASS-FAIL skeleton.
- `CMakeLists.txt` — `CoreSystemsTest` target (links PotatoEngine) + POTATO_TESTS entry.

Review findings: 28 findings across 4 layers. 4 patched (all `low`: exception-safe component ops, relaxed ID-recycle contract, system lifecycle + RemoveSystem coverage, updateCount assertion). 13 pre-existing defects in the systems under test recorded in `deferred:` (ResourceCache Load<T> link trap, dead eventQueue, path-only cache key, mutex-during-callback deadlock surface, Entity mask/generation/validity gaps, EventBus unsubscribe gap, ResourceManager loader-seam/UnloadUnusedResources/GetX semantics, ':' path heuristic, 1KB hardcode). 7 rejected (spec-sanctioned seam choice, cosmetic divergences, foreclosed global-service reading).

Patches by verdict: high 0, medium 0, low 4. followup_review_recommended: false (no high patched, <2 medium patched).

Verification: `CoreSystemsTest.exe` → PASS 59 / FAIL 0, exit 0; `ctest -C Release -R CoreSystemsTest` → Passed; MinGW `-fsyntax-only` clean; MSVC Release build clean.

Residual risks: none for the diff itself. The deferred list now documents 13 real defects in ECS/EventBus/Resources that this coverage work surfaced — candidates for a future hardening pass.
