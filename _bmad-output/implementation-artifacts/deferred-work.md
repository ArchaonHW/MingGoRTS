# Deferred Work

Findings deferred from `spec-ide-dev-assistant` review (iteration 1). All items below are in files outside that spec's scope — they belong to concurrent work in the worktree (rendering, physics, input, platform, audio, security, serialization changes) or need harnesses that do not exist yet. Triaged from blind-hunter, edge-case-hunter, and verification-gap review layers.

## Bugs / correctness (other modules)

- `Rendering/ModelLoader.cpp:87-103` — face `resolve` lambda dropped the upper bounds check: `f 9999/1/1` reads `positions[vIdx]` out of bounds. Also `-INT_MIN` token causes signed overflow before the negative-index path.
- `Rendering/ModelLoader.cpp` — no face triangulation: `f v1 v2 v3 v4` pushes 4 sequential indices rendered as `GL_TRIANGLES` (degenerate third triangle); `tangent`/`bitangent` never computed.
- `Platform/PlatformSystem.cpp:318-338` — `StandardConditionVariable::Wait`/`WaitFor` use `std::adopt_lock`; the `unique_lock` destructor unlocks the mutex on return while the caller still expects to hold it and calls `IMutex::Unlock()` → double-unlock UB. Needs `lock.release()`.
- `Platform/GLFWSharedContext.cpp:58-61` — context map keyed by raw `GLFWwindow*`; a window destroyed without `GLFWWindow::Shutdown` (e.g. raw `glfwDestroyWindow` in `MingGoRTS_IDE/GUI/Main.cpp:136`) leaves a stale entry; a recycled address inherits dangling `inputOwner`/`windowOwner` (UAF).
- `Input/InputManager.cpp` — keymap half-finished both directions: `GLFWKeyToKeyCode` still maps punctuation/numpad/F13–F25/`GLFW_KEY_WORLD_*` to `Unknown`; `KeyCodeToGLFWKey` omits `World1`/`World2`. `keyStates[512]`/`mouseButtonStates[8]` written but never read (`IsKeyPressed` polls GLFW directly).
- `Physics/PhysicsSystem.cpp` — two parallel kinematic mechanisms (`SetBodyType(Kinematic)` vs `kinematic` bool via `SetKinematic`) with inconsistent semantics across `UpdateBodies`/`IntegrateVelocity`/`IntegratePosition`/`ApplyForce`.
- `Physics/PhysicsSystem.cpp:409-414` — `DetectCollisions` fires `bodyA->collisionCallback` but never `bodyB->collisionCallback`.
- `Audio/AudioSystem.cpp:254-297` — `LoadWAV` never validates `RIFF`/`WAVE` magic, doesn't check the payload `file.read` result (short reads return true with zero/garbage tail), and `resize(dataSize)` is unbounded (`bad_alloc` on huge files).
- `Serialization/Serialization.cpp` — `GetSaveSlots` returns raw stems but `GetSaveSlotPath` sanitizes to `[A-Za-z0-9_-]`: listed slot `"a.b"` maps to `"ab.json"`, so `SaveSlotExists`/`DeleteSaveSlot` can't act on enumerated slots; `entry.is_regular_file()` can throw mid-iteration (use `error_code` overload).
- `Rendering/Camera.cpp:137-141` — `SetViewport` guards `height > 0` but not `width > 0` (aspect <= 0 → inf/NaN projection).
- `Examples/SecurityRedTeamTest.cpp:668-671` — restore-path `VirtualProtect` failure writes to an RX page (AV crash); `:729` `spawnTool(pi2)` failure silently skips whitelist verification; `:755` fixed `Sleep(100)` races the trampoline thread (flaky FAIL).

## Missing verification (other modules)

- Input dispatch/key-mapping changes (`OnKeyEvent`, `GLFWKeyToKeyCode`, `KeyCodeToGLFWKey`, `GLFWSharedContext` dispatch) have no test. Headless-testable half: construct `GLFWInputManager`, register callback, call `OnKeyEvent(GLFW_KEY_ESCAPE, ...)`, assert `KeyCode::Escape`.
- OBJ face-parser rewrite (`v//vn`, negative indices, skip-invalid) has no test — add `LoadFromMemory` checks for the four face formats.
- `PhysicsTest` doesn't exercise restitution/impulse changes: no post-collision velocity assertion, no destroy-inside-callback case, no coincident-overlap case, no zero-mass pair, no non-sphere `BoundingRadius`.
- `SceneTest` parents are all translation-only — the `TransformPoint` world-position fix is unobserved; add a rotated-parent case.
- `OpenALAudioManager::LoadWAV` hardening unchecked — write a 10-byte temp `.wav`, assert `LoadAudio` returns `nullptr`.
- `CheckExternalHandles` system-dir auto-trust direction unpinned (needs real system-dir process holding a handle — not hermetic).
- GL-context-dependent changes (`VertexArray` lazy create, `Mesh::Draw` non-indexed fallback, `AdvancedShader` reload cleanup, `ShouldClose` null guard) have no verification path — needs a GL-context-capable smoke run.
- `TimeManager` paused-FPS (`UpdateFPS(rawDeltaTime)`), `Camera::SetViewport` height guard, `DashboardPanel` success-rate formula — unpinned; `Update()` needs real sleeping, others are trivial guards.
- IDE dev-assistant async write-back seam (`std::async` → `PollDevelopmentResult` → `developmentResponse`) has no executable check — needs an ImGui harness or a UI-free result-application helper.

## Cleanup / leftovers

- [x] `CMakeLists.txt` — commented-out `# add_executable(PlatformTest …)` removed (real target kept above)
- [x] `Resources/ResourceManager.cpp` — stale comment updated to `ResourceCacheHandle 實現`
- [x] `Mesh::Draw`/`DrawInstanced` — early-return when both buffers empty; no longer binds/creates VAO pointlessly
- [ ] `_bmad-output/brainstorming-session-2026-09-16.md` — skipped: file has unstaged user edits (still in use?)
- [x] Untracked root `nul` artifact — already absent (verified via `\\?\` path)
