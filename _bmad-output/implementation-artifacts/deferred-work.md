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
- [x] GL-context-dependent changes — `GLSmokeTest` covers `VertexArray` lazy create (construct + empty `Draw` before context), `Mesh::Draw` non-indexed/indexed/instanced paths, `ShouldClose` null guard; self-SKIPs on headless. (`AdvancedShader` reload no longer exists — stale item.)
- [x] `TimeManager` paused-FPS — `TimeTest` pins `UpdateFPS(rawDeltaTime)`: paused → deltaTime/totalTime frozen but real FPS still reported. (`DashboardPanel` no longer exists — stale item.)
- [x] IDE dev-assistant write-back seam — `WriteGenerationResult` extracted to `GuiTextUtils.h` (UI-free); `PollDevelopmentResult` uses it; `DevAssistantSmoke` covers success/failure write-back paths.

## Cleanup / leftovers

- [x] `CMakeLists.txt` — commented-out `# add_executable(PlatformTest …)` removed (real target kept above)
- [x] `Resources/ResourceManager.cpp` — stale comment updated to `ResourceCacheHandle 實現`
- [x] `Mesh::Draw`/`DrawInstanced` — early-return when both buffers empty; no longer binds/creates VAO pointlessly
- [ ] `_bmad-output/brainstorming-session-2026-09-16.md` — skipped: file has unstaged user edits (still in use?)
- [x] Untracked root `nul` artifact — already absent (verified via `\\?\` path)

- source_spec: `_bmad-output/implementation-artifacts/spec-intelligent-suggestions.md`
  summary: Run intelligent-suggestion analysis off the render thread / cache compiled regexes — GenerateSuggestions constructs std::regex objects per call and scans the whole 8KB buffer synchronously each quiet-period.
  evidence: Blind Hunter review — std::regex built per call (e.g. magicNumberRegex in GetBestPracticeRecommendations); bounded but can hitch a frame on each analysis pass.
  resolution: 已全修（2026-09-17）：regex 改 function-local static const 快取；分析搬離 render thread——debounce 滿 500ms 後 `std::async` 背景跑 GenerateSuggestions，render thread 只輪詢 future；過期結果以 inflight-hash 丟棄、LearnFromFeedback 與 worker 以 `g_SuggestionMutex` 互斥、Shutdown 有界等待 2s 後才 reset。
- source_spec: `_bmad-output/implementation-artifacts/spec-intelligent-suggestions.md`
  summary: Replace file-scope raw-pointer AI globals (g_SuggestionSystem et al.) with owned instances guarded against double-Init; Initialize() return value currently ignored.
  evidence: Blind Hunter review — second IDEGUI instance overwrites/leaks the global; Shutdown timeout early-return leaves the pointer dangling.
  resolution: 已修（2026-09-17）：六個 AI 全域改為 `std::unique_ptr`；ctor 加 `if (!g_DevSystem)` 防重複初始化；Shutdown 逾時分支保留「不釋放以避免 UAF」語義（unique_ptr 存活即洩漏）。
- source_spec: `_bmad-output/implementation-artifacts/spec-render-training-data.md`
  summary: `NeuralLayer::Backward` calls `activationDerivative(lastOutput[i])` on the post-activation value — sigmoid derivative computes σ(y)·(1−σ(y)) instead of y·(1−y) (ReLU coincidentally correct). Gradients are distorted though sign is preserved; any fix changes training dynamics globally, so evaluate against AITestSuite before changing.
  evidence: Edge Case Hunter review of SynthDataDemo — AI/NeuralNetwork.cpp ~line 141.
  resolution: 已修（2026-09-17）：Forward 改存 `lastPreActivation`（z），Backward 導數吃 z——sigmoid/tanh 梯度正確。驗證：AITestSuite 7/7 + headless AND 訓練 loss 0.25→0.0096 收斂。
- source_spec: `_bmad-output/implementation-artifacts/spec-render-training-data.md`
  summary: `NeuralLayer` seeds its mt19937 from `std::random_device` with no seed API — in-tree NN init is non-reproducible. Consider a `NeuralNetwork`/`NeuralLayer` seed parameter (or Build(seed)) so callers don't need the SetWeights/SetBiases override workaround used in SynthDataDemo.
  evidence: Blind Hunter + Edge Case Hunter reviews.
  resolution: 已修（2026-09-17）：`NeuralLayer::SetSeed`（重 seed + 重建權重）、`NeuralNetwork::Build(unsigned seed)` 重載、Train shuffle 改用成員 rng——同 seed 建構權重完全一致（headless 驗證）。
- source_spec: `_bmad-output/implementation-artifacts/spec-game-backend-services.md`
  summary: Add DELETE endpoints (or a documented retention policy) for /api/replays and /api/rosters — currently the only way to remove a bad upload is deleting the H2 files.
  evidence: Blind-hunter review — services accept unauthenticated writes forever with no removal path; deferred because DELETE adds public API surface the spec's intent did not request.
- source_spec: `_bmad-output/implementation-artifacts/spec-game-backend-services.md`
  summary: Extract a services-common Maven module for the duplicated parse/json/badRequest/notFound helpers in RosterController and ReplayController.
  evidence: Blind-hunter review — two controllers already diverge in style (StringBuilder concat vs record serialization); a shared module is a structural refactor beyond this change's scope.
- source_spec: `_bmad-output/implementation-artifacts/spec-game-backend-services.md`
  summary: Manage H2 schema with Flyway instead of ddl-auto=update, and wire Maven into CI when Java services are expected to build in CI.
  evidence: Blind-hunter + verification-gap reviews — schema drift is unmanaged and Java tests never run in CI; both were explicitly out of this spec's boundaries.
