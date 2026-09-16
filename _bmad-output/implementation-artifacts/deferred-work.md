# Deferred Work

Findings deferred from `spec-ide-dev-assistant` review (iteration 1). All items below are in files outside that spec's scope — they belong to concurrent work in the worktree (rendering, physics, input, platform, audio, security, serialization changes) or need harnesses that do not exist yet. Triaged from blind-hunter, edge-case-hunter, and verification-gap review layers.

## Bugs / correctness (other modules)

- [x] `Rendering/ModelLoader.cpp` — face index resolution now bounds-checks all three index fields (`f 9999/1/1` skipped), `-INT_MIN` handled via `long long` widening before negation; verified by `ModelLoaderTest`
- [x] `Rendering/ModelLoader.cpp` — quad/ngon faces fan-triangulated; `tangent`/`bitangent` computed per-vertex in `OptimizeMeshes` (degenerate UV determinant skipped)
- [x] `Platform/PlatformSystem.cpp` — `Wait`/`WaitFor` call `lock.release()` after wait so `unique_lock` dtor no longer steals the mutex; PlatformTest exercises Wait/Notify handoff + WaitFor timeout
- [x] `Platform/GLFWSharedContext.cpp` — `installed` flag + user-pointer validation in `GetOrCreateGLFWContext` detects recycled-address stale entries; new `DestroyGLFWWindow()` is the only sanctioned destroy path (used by `GLFWWindow::Shutdown` and `OpenGLRenderer::ShutdownGLFW`); `Main.cpp` documented as not using engine dispatch
- [x] `Input/InputManager.cpp` — `GLFWKeyToKeyCode` completed (punctuation, World1/2, F13–F25, full numpad); `KeyCodeToGLFWKey` maps World1/World2; `keyStates`/`mouseButtonStates` integrated as headless fallback when no window handle; `InputTest` covers OnKeyEvent + 26-pair bidirectional mapping
- [x] `Physics/PhysicsSystem.cpp` — kinematic semantics unified: `IsKinematic()` returns `kinematic || bodyType == Kinematic`; `ApplyForce`/`ApplyTorque`/`ApplyImpulse`/`IntegrateVelocity`/separation all respect it; `bodyB->collisionCallback` fired with flipped `otherBodyID`
- [x] `Audio/AudioSystem.cpp` — `LoadWAV` validates RIFF/WAVE/fmt magic, caps payload at 512MB, checks read result; `AudioTest` covers tiny/non-RIFF/non-WAVE/valid cases
- [x] `Serialization/Serialization.cpp` — `GetSaveSlots` only lists stems that pass sanitization unchanged (round-trip guaranteed); `is_regular_file(entryEc)` non-throwing; `SanitizeSlotName` shared with `GetSaveSlotPath`
- [x] `Rendering/Camera.cpp` — `SetViewport` guards `width > 0 && height > 0`; covered by SceneTest
- [x] `Examples/SecurityRedTeamTest.cpp` — restore-path `VirtualProtect` result checked before write (no AV); `spawnTool(pi2)` failure now `Fail`s instead of silently skipping; `Sleep(100)` race replaced with 2s polling loop on `CheckThreadContexts`

## Missing verification (other modules)

- [x] Input: `InputTest` — headless `GLFWInputManager`, `OnKeyEvent(ESCAPE)` → `KeyCode::Escape`, 26-pair bidirectional map, unknown-key safety
- [x] OBJ parser: `ModelLoaderTest` — all four face formats, negative indices, OOB + INT_MIN, quad triangulation, tangent/bitangent direction
- [x] `PhysicsTest` — added: both collision callbacks, destroy-inside-callback, kinematic-bool force immunity, zero-mass pair, coincident overlap, restitution velocity assertion, non-sphere `BoundingRadius` broadphase
- [x] `SceneTest` — rotated-parent case (90° Y) asserts child world position via `TransformPoint`; Camera viewport guard checks added
- [x] `LoadWAV` hardening — `AudioTest` malformed-file cases
- `CheckExternalHandles` system-dir auto-trust direction unpinned (needs real system-dir process holding a handle — not hermetic).
- GL-context-dependent changes (`VertexArray` lazy create, `Mesh::Draw` non-indexed fallback, `AdvancedShader` reload cleanup, `ShouldClose` null guard) have no verification path — needs a GL-context-capable smoke run.
- `TimeManager` paused-FPS (`UpdateFPS(rawDeltaTime)`), `DashboardPanel` success-rate formula — unpinned; `Update()` needs real sleeping, others are trivial guards.
- IDE dev-assistant async write-back seam (`std::async` → `PollDevelopmentResult` → `developmentResponse`) has no executable check — needs an ImGui harness or a UI-free result-application helper.

## Cleanup / leftovers

- [x] `CMakeLists.txt` — commented-out `# add_executable(PlatformTest …)` removed (real target kept above)
- [x] `Resources/ResourceManager.cpp` — stale comment updated to `ResourceCacheHandle 實現`
- [x] `Mesh::Draw`/`DrawInstanced` — early-return when both buffers empty; no longer binds/creates VAO pointlessly
- [ ] `_bmad-output/brainstorming-session-2026-09-16.md` — skipped: file has unstaged user edits (still in use?)
- [x] Untracked root `nul` artifact — already absent (verified via `\\?\` path)
