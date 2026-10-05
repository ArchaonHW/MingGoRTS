<!-- bmad:context -->
<!-- Verified 2026-09-29 (unversioned: git unavailable in this environment). Managed by bmad-project-context; edits inside this block are replaced on refresh. Keep anything you want preserved outside the markers. -->

## MingGoRTS / PotatoEngine

C++20 game engine (PotatoEngine) + greenfield game layer (MingGoRTS). Planning artifacts live in `_bmad-output/` (GDD, `game-architecture.md`, `narrative-design.md`, `planning-artifacts/epics.md`, `implementation-artifacts/sprint-status.yaml`); these are authoritative over README status claims.

## Policy

- Never push to `main`/`develop`; PRs only, Conventional Commits.
- Never modify `external/` — vendored dependencies.
- No third-party JSON libraries; use the game-layer `JsonValue` (to be built in `Gameplay/Json/`).
- No unsafe C functions: `gets`, `strcpy`, `strcat`, `sprintf`, `vsprintf`, `scanf`.
- All game content files are versioned JSON: `potato.<name>/<ver>` header, reject bad schema without mutating state.
- Saves write via temp-file + rename (atomic).

## Where things are

- Engine subsystems: top-level dirs (`Core/`, `ECS/`, `Events/`, `Rendering/`, `Serialization/`, `GUI/`, ...).
- Game layer (planned, may not exist yet): `Gameplay/` sim → `Campaign/` → `Game/` shell → `assets/`.
- Executable examples/tests: `Examples/` — each is its own `add_executable` target.
- Story files and sprint tracking: `_bmad-output/implementation-artifacts/`.

## Running and verifying

- Build (Windows/MSVC): `BuildEngine.bat`, or `cmake -B build -G "Visual Studio 17 2022" -A x64` then `cmake --build build --config Release`.
- MinGW builds: guard toolchain specifics with `WIN32 AND NOT MSVC`; verify both MSVC and MinGW before calling work done.
- No unified test command — there is no CTest; tests are standalone executables under `Examples/`. Game-layer tests use the `potato_test_<name>` convention (POTATO_TESTS option is planned, not yet in CMake).
- Environment: `git`, `python`, `uv` are NOT on PATH here — don't script around them.

## Conventions that differ from defaults

- `Gameplay/` must compile headless: no Rendering/GUI/OpenGL includes; dependency direction is Engine ← Gameplay ← Campaign ← Game, single direction only.
- Simulation is deterministic: fixed 20 Hz tick, integer/fixed-point only (no floats in sim), one seeded PRNG stream, canonical eval order (squad index → slot index).
- Truth boundary: nothing outside `Gameplay/Sim/` reads true enemy state — presentation and doctrine conditions read certainty only.
- Two event regimes: typed `SimEvent` list inside the sim (recorded for replay); engine `EventBus` only at layer boundaries — subscribers never write `BattleState`.
- `PotatoEngine` facade subsystem accessors are commented out — bind engine leaf modules directly; do not "fix" the facade as a side task.
- Error handling: no exceptions in the tick path; `Result<T>` at I/O/content boundaries; registries immutable during battle; zero file I/O in the tick path.

## Known pitfalls

- README describes aspirational state (completed epics, existing `Gameplay/`, old planning paths) that does not match the filesystem — trust `_bmad-output/` documents and the actual tree, not README status lines.
- PowerShell here-strings treat backticks as escapes — prefer `[IO.File]::WriteAllText` with single-quoted here-strings when writing content containing backticks.
- The repo path contains CJK (`F:\民國史詩`) — MinGW make and FetchContent subbuilds fail on it ("Illegal byte sequence"). Build via an ASCII junction, e.g. `C:\MingGoRTS -> F:\民國史詩\HWC\MingGoRTS`.
- `PotatoEngine` static lib does not compile under MinGW (`Security/SecuritySystem.cpp` uses MSVC-isms) — `PotatoGameplay` deliberately does not link it yet. For headless builds use `-DPOTATO_BUILD_GUI=OFF` (skips glfw/glad FetchContent, which also needs git/network).
- IDE GUI state 用固定長度 char buffer（如 `state.developmentResponse`）——必須 `strncpy` + 結尾 `\0` 或 `memset`，不可直接 `=` 指派 `std::string`

<!-- /bmad:context -->

## Gameplay 層（doctrine 戰鬥原型，2026-09-17 新增）

- `Gameplay/`：斷橋原型的玩法層——`FlowField`（群體尋路）/`Squad`/`Doctrine`（含規則冷卻）/`BattleController`（三拍狀態機）/`BattlePlanner`（AI 參謀）/`BattleMap`（T-4 地圖 JSON）/`EnemyGeneral`（T-5 敵將人格腳本）/`BattleResources`（T-6 情報/CP/士氣執行率）/`BattleRecorder`（T-7 事件錄製回放）/`Roster`（T-8 名冊）
- `Gameplay` lib 連結 `PotatoEngine`（BattleSceneSync 需要場景類型）；`PotatoEngine` 不反向依賴 Gameplay
- 資產 schema：`assets/cards/` 角色卡（`potato.character_card/1`，人格三軸 + signatureDoctrine）、`assets/maps/` 戰場圖（`potato.battle_map/1`）、回放 `potato.battle_replay/1`、名冊 `potato.roster/1`
- JSON 解析用 `Serialization/JsonParser.h`（`Potato::JsonValue`），不要引第三方 JSON 庫

## 驗證方式更新

- CTest 已啟用：`enable_testing()` + `POTATO_TESTS` 清單，`cd build && ctest -C Release`；新測試執行檔加進 `POTATO_TESTS` 即自動註冊
- 玩法 demo：`DoctrineBattleDemo`（手寫腳本）、`AutoPlannerDemo`（AI 規劃）、`DuanqiaoDemo`（斷橋整合）皆為無頭測試，回傳非零即失敗
- BattleRecorder/Roster 會寫出 JSON 檔到工作目錄——屬預期行為，勿當副作用刪除
- MSBuild 增量建置只比對時間戳：若 .obj 比 .cpp 新但內容是舊版（平行工具寫檔時保留 mtime 所致），測試會跑「看不見的舊碼」。症狀：原始碼檢查正確但行為不符。修法：`touch <file>.cpp` 或 rebuild 該 target；診斷可用 `dumpbin -SYMBOLS <obj>` 確認是否引用預期符號

## Vendored 第三方依賴（external/ 例外規則）

- 原則：不改 `external/` 既有內容；新增第三方庫僅允許「新增子目錄」形式：`external/<lib>/`
- 必要條件：目錄內附 `README.md` 記錄 來源 URL / 版本 / license；實作巨集（如 `TINYGLTF_IMPLEMENTATION`、`STB_IMAGE_IMPLEMENTATION`）集中放在引擎目錄的單一 TU（例：`Rendering/TinyGltfImpl.cpp`），其他檔案不得重複定義
- 引入前優先評估是否已有自研方案可複用（如 `Rendering/ImageCodec` 的零依賴 PNG）
- 目前 vendored：`external/tinygltf/`（tinygltf v2.9.7 + nlohmann/json + stb_image/stb_image_write，glTF/VRM 載入用）

## 已知環境坑：MinGW libstdc++ DLL 錯配

- 在 git-bash 手工 `g++` 編出的 binary 會依 PATH 載入 `libstdc++-6.dll`——`C:\Program Files\Git\mingw64\bin` 若排在 scoop MinGW 前面，會載到**版本不符的 DLL**，`-O1/-O2` 下 segfault（`-O0` 僥倖通過）。
- 解法：執行前 `export PATH="/c/Users/potat/scoop/apps/mingw/16.2.0-rt_v14-rev1/bin:$PATH"`，或連結時加 `-static-libstdc++ -static-libgcc`。
- `build-mingw/bin/` 的 CMake 產物不受影響（ctest 全部正常）。
