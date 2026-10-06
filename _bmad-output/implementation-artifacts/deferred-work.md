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

## Deferred from: code review of 1-1-gameplay-module-skeleton (2026-09-29)

- `static_assert` no-float enforcement on sim structs — nothing to assert until Story 1.3 (BattleState POD arrays); carry requirement into that story.
- `PotatoGameplay` does not link/exercise any engine leaf module — blocked on `Security/SecuritySystem.cpp` MinGW breakage (pre-existing). Revisit when engine leaf targets exist.
- `BuildEngine.bat` has no `-DPOTATO_BUILD_GUI=OFF` path — headless builds need manual cmake or the ASCII junction workaround (documented in AGENTS.md).
- `ENGINE_SOURCES`/`ENGINE_HEADERS` globs (pre-existing) lack `CONFIGURE_DEPENDS`; new `GAMEPLAY_*` globs get it, engine globs unchanged.

## Deferred from: code review of 1-2-jsonvalue-dom-parser-schema-guard (2026-09-29)

- Raw bytes ≥0x80 in strings not UTF-8-validated — accepted leniency (RFC 8259 permits); escaped `\u` paths are strictly validated. Add a validation pass if malformed UTF-8 ever breaks text rendering.
- `\u0000` embeds a raw NUL into `std::string` values — spec-legal; downstream `.c_str()` consumers would silently truncate. Reject only if a real consumer trips.
- `JsonValue` node is ~96 B (string+vector+map members per node) and `FindString`/`Items`/`Members` return refs that dangle after move/destroy — documented semantics; revisit only under measured memory pressure or a real bug.
- `potato_test_json` fixture files land in the implicit ctest working dir (build tree) — cleaned unconditionally on all paths; add explicit `WORKING_DIRECTORY` only if a second test ever races on the filenames.

## Deferred from: code review of 1-3-battlemap-model (2026-09-29)

- Unscoped bitmask enums (`Terrain`/`Strategic`/`HistMark`/`MythMark`) allow silent cross-domain mixing (`r.terrain & MYTH_SHRINE` compiles) — scoped-enums + flag-operator refactor deferred; fields are raw uint32 by design and `IsStrategicPoint`'s cross-domain bit test is intentional.
- `RegionIndexOf`/`FindRegion` are O(n) linear scans; the `indexOf` map built during validation is discarded — fine at tens-of-regions scale; retain the map only if measured hot.
- `IsStrategicPoint` hardcodes the capturable-bit mask — deliberate enumeration; revisit if a 4th `STRATEGIC_*` bit lands.
- Region/map `id` hygiene: whitespace-only, embedded NUL, invalid UTF-8 accepted — hand-authored content; strictness deliberately on structure, not content hygiene.

## Deferred from: code review of 1-4-squad-model (2026-09-29)

- `Squad` is not flat POD (`std::string id/name`) — when `BattleState::Checksum` lands (Story 1.6+/recorder), hash string *contents*, not object memory. Also gate: per-squad POD arrays vs struct-of-objects decision is made there.
- No rally transition (`Routing`→`Holding`) — deliberate design: rout is morale collapse, sticky in-battle; RefitCamp is the recovery path. Revisit only if a "rally" doctrine card is ever designed.
- `potato.squad/1` cannot express artillery's "(ranged)" qualifier or the counters/countered-by column — add `potato.squad/2` when combat resolution needs ranged/role data.
- Invalid `SquadState` values via public `state` field freeze the squad — accepted (serialization writes land in later stories; validate on deserialize then).
- `Heal`/`RestoreCohesion` no-op on terminal states; `ApplyHit` no-op on Routed/Destroyed — Routed squads are off-field (cannot be finished off); revisit if pursuit mechanics are designed.

## Deferred from: code review of 1-5-doctrine-interpreter (2026-09-29)

- `enemy_in_region`/`enemy_adjacent` triggers read ground-truth `regionIndex` — a truth-boundary seam. QuantumFog/CertaintyField (Story 1.8) must rewire these two kinds through certainty data; vocabulary stays stable, only the detector changes.
- `cpPool` is a single shared pool read by both sides — per-side CP pools land with symmetric AI (Story 1.10).
- `EvalTick` takes the snapshot as `const&` — "tick-start" purity is caller discipline, not enforced; if a future caller interleaves `ApplyDeltas` between squad evals this silently breaks. Snapshot-copy enforcement deferred until a real caller needs it.
- Determinism test is vacuous w.r.t. `rng` (vocab v0 draws nothing) — AC2 holds structurally; when a card draws PRNG, add a draw-order sensitivity test.
- `potato.doctrine_cards/1` is a library file vs architecture's singular `potato.doctrine_card/<ver>` naming — deliberate (consistent with potato.map/1, potato.squad/1); rename only if architecture text is updated.
- `ReadClause` treats `param` on a no-param kind (`always`, `hold`, `retreat`, `none`) strictly (only `0` allowed) — loosen to "ignore" if content authors find it hostile.

## Deferred from: code review of 1-6-battlecontroller-three-beat-loop (2026-09-29)

- Deployed squads share their template's `id` — two infantry from one template collide on battle-scoped identity. Events key on `squadIndex` so nothing corrupts; battle-scoped instance naming belongs to the roster/naming work (Epic B/C).
- `cpPool` has no upper cap (`SetCpPool` clamps negatives only) — CP economy (regen +1/60s, cap 5, intervention costs) is Story 1.7's job.
- Manual `RequestBeat(Aftermath)` mid-Execution is the concede path; `BattleOutcome::forced` distinguishes it from a wipe draw. Sub-story: whether "concede" should still produce a winner by objective score is Story 1.12 win-evaluation territory.
- `ROUT_TICKS` (2 s) is a v0 constant — rout-off-field pacing may want per-terrain or card-driven variance later.

## Deferred from: code review of 1-8-quantumfog (2026-09-30)

- No `potato.balance/1` file exists on disk — `FogConfig::FromJson` verified via inline JSON only; content loading wires in when the asset pipeline story lands.
- Truth boundary is discipline-enforced: `CheckGameplayDeps.ps1` bans Rendering/GUI includes but not `Gameplay/Squad` in `Gameplay/Fog`, nor doctrine truth-scans. Extend the guard when Campaign/Game layers appear.
- `BattleController::Squads()` exposes full enemy truth — fine pre-UI; presentation must consume `Fog(side)` for enemy rendering.
- `FogConfig` is a bare aggregate: out-of-range hand-built configs bypass `FromJson` validation (negative decayPerMinute drifts the accumulator).

## Deferred from: code review of 1-9-battleplan-arrows (2026-09-30)

- Replan `SimEvent` records only `param=path.size()` — the full path payload is lost when `pendingCommands_` clears at apply. Every other command is self-describing in the log; events-driven replay cannot reissue a Replan. Story 1.11 (BattleRecorder) needs command-input recording or a path side-channel anyway — resolve there.
- Replan on an arrowless squad creates and grants an arrow mid-Execution (deliberately kept — "replan = new plan" for CP). Re-check the contract when the symmetric AI opponent (1.10) starts issuing replans.
- No `potato.balance/1` file exists on disk — `PlanConfig::FromJson` verified via inline JSON only (same precedent as `FogConfig`); wires in with the asset pipeline.
- `PlanConfig` is a bare aggregate like `FogConfig` — hand-built configs bypass `FromJson` bounds (e.g. replanCost > CP_CAP).

## Deferred from: code review of 1-10-symmetric-ai-opponent (2026-09-30)

- Cloud-shaping is cunning-only: aggressive/defensive priors never touch fog. The AC mechanism is demonstrated on the prior that cares; a per-prior fog signature would be a design follow-up.
- `BattleAI::Act` reads `.side` on enemy squad rows to locate its own squads (short-circuit; no other enemy field touched). Mechanical own-index iteration lands when roster indices are tracked — same deferral bucket as `Squads()` truth exposure.
- `Plan` header claims "no mutation on failure" — held today only because all failure modes pre-validate; a future mid-loop `DeploySquad` gate would partially deploy. Latent.
- Cunning probes scan in region-index order, not weakest-certainty-first (comment says weakest); v0 acceptable.
- `BattleAI` ctor validates neither `side` nor `prior`; `Plan` gates bad sides and `Act` is benign (Issue* side-checks reject) — harden if AI objects are ever host-driven without Plan.
