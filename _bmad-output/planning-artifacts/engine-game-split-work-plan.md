# PotatoEngine / MingGoRTS 拆倉工作分解（WBS）

日期：2026-09-19
目標：MingGoRTS 從 PotatoEngine 獨立 —— 遊戲 repo + 上游引擎 repo，保留 git history，遊戲以外部依賴消費引擎。

## 邊界結論（recon 已完成）

- 引擎原始碼層**無**直接引用遊戲層（Gameplay/Campaign/IDE）——邊界本身乾淨
- 唯二引擎→AI 耦合：
  - `Rendering/NeuralGraphics.h` → `AI/NeuralNetwork.h`（`PotatoEngine` 連結 `NeuralNetwork`）
  - `GUI/AgentGUI.h/.cpp` → `AI/AIAgentSystem.h`（GUI/ 目錄只有這兩個檔）
- 既有機制：`tools/sync-potatoengine-upstream.sh` 已把引擎模組鏡像到 `C:\HWC\PotatoEngine`（include/+src/，非 git repo，是過時快照）
- 未提交變更：CMakeLists.txt、AGENTS.md、README.md、VideoDataDemo.cpp 已修改；ChapterConventions、SuggestionRulesTest 等新檔未追蹤——clone 前先導出

## 歸屬表

| 歸引擎（PotatoEngine repo） | 歸遊戲（MingGoRTS repo） |
|---|---|
| Core/ Security/ Events/ FileSystem/ Logging/ Memory/ Platform/ Scene/ Serialization/ Time/ Rendering/ Physics/ Audio/ Input/ Resources/ ECS/ GameObject/ MathUtils/ Quantum/ Media/ | Gameplay/ Campaign/ MingGoRTS_IDE/ assets/ services/ Saves/ tools/ |
| AI/NeuralNetwork.{h,cpp}（NeuralGraphics 依賴） | AI/ 其餘全部（ReinforcementLearning、AIAgentSystem、LLM、RAG…） |
| external/（vendored：glad/imgui/tinygltf/GLFW FetchContent） | GUI/AgentGUI.{h,cpp}（依賴 AIAgentSystem） |
| 引擎型 Examples（33 個：MinimalTest、PhysicsTest、RenderTargetTest、NeuralGraphicsTest、PortraitBaker、AngelLegionVideo…） | 遊戲型 Examples（40 個：所有連結 Gameplay/Campaign/AI libs 者）+ DemoAssets.h/UITheme.h/compat_log.h |
| CMakeLists.txt、LICENSE、SECURITY.md、.gitignore、BuildEngine.bat | CMakeLists.txt、AGENTS.md、README.md、_bmad/、_bmad-output/、docs/、.agents/、.windsurf/、design-artifacts/ |
| .github/（引擎 CI 子集） | .github/（遊戲 CI 子集） |

## Phase 0 — 前置決策（gate，已裁決 2026-09-21）

- [x] W0.1 `NeuralNetwork` 歸屬：**留引擎**（Rendering/NeuralGraphics、NeuralArtTool、SynthDataDemo 都依賴；遊戲側 ReinforcementLearning 連結引擎 target）
- [x] W0.2 `GUI/AgentGUI` 歸屬：**移遊戲**——已於 Phase 1 遷至 `MingGoRTS_IDE/`（依賴 AIAgentSystem；GUI/ 目錄僅此二檔）
- [x] W0.3 消費方式：**sibling checkout + `add_subdirectory(${POTATO_ENGINE_ROOT})`**（預設 `../PotatoEngine`，可用 cache var 覆寫）
- [x] W0.4 上游 repo 落地：**filter-repo 產物直接取代 PotatoEngine repo**——前提已更新：`C:\HWC\PotatoEngine` 現為真 git repo（include/src 佈局 + github.com/ArchaonHW/PotatoEngine remote + 測試已從 Examples 移植）；取代前先備份現 repo。其既有測試已反向同步回 MingGoRTS Examples（commit 5b28ab0），不隨 filter-repo 丟失
- [x] W0.5 遊戲 Examples 跟遊戲走（連結 Gameplay 的 test 留在引擎會破壞引擎獨立性）

## Phase 1 — 邊界固化（monorepo 內先做）✅ 2026-09-21 完成（82f5a1d）

- [x] W1.1 `GUI/AgentGUI.{h,cpp}` 遷至 `MingGoRTS_IDE/`，孤兒 `install(GUI/)` 與 check_all.sh 殘留清除
- [x] W1.2 `CMAKE_SOURCE_DIR` → `CMAKE_CURRENT_SOURCE_DIR` 全檔 137 處替換
- [x] W1.3 standalone 守衛包住 examples/tests/install/CPack；NeuralNetwork/Quantum/Media 提升守衛外
- [x] W1.4 驗證：MSVC(VS18)+MinGW 建置皆過，ctest 68/68 全綠

## Phase 2 — 歷史拆分（git filter-repo，已完成）

- [x] W2.1 導出未提交變更：`git diff HEAD` + untracked 清單 → 依歸屬表分側（快照存 `C:\HWC\_split_stage\`）
- [x] W2.2 clone → engine repo → `filter-repo --paths-from-file`（引擎路徑清單，165 檔）
- [x] W2.3 clone → game repo → `filter-repo --paths-from-file`（遊戲路徑清單，2351 檔）
- [x] W2.4 跨界檔後處理：AgentGUI 已在 `MingGoRTS_IDE/`（Phase 1 完成）、NeuralNetwork 留引擎；路徑清單不含 GUI/ 於引擎側
- [x] W2.5 未提交變更回灌各側 working tree（L-6/L-8/Jamming/QuasiModels 依歸屬分側；平行 session 持續提交的 L-6/L-8 commit 以 format-patch 補植）

注意：`--paths-from-file` 的路徑行**不加前綴**（`path:` 會被當字面路徑，產出空 repo）；路徑清單需與切點時刻的 `git ls-files` 對帳——新增檔案（如 LedgerRegistryTest/LedgerAssuranceTest）要補進清單再濾。

## Phase 3 — 新 CMake 架構（已完成）

- [x] W3.1 引擎 CMakeLists 精簡：去遊戲 lib/executable/POTATO_TESTS 遊戲項；`glfw` imported target 提升 `GLOBAL`（imported target 預設目錄範圍，消費端看不到）
- [x] W3.2 遊戲 CMakeLists：`project(MingGoRTS)` + `POTATO_ENGINE_ROOT` cache var + `add_subdirectory` + 雙根目錄 include
- [x] W3.3 imgui/external 引用改 `${POTATO_ENGINE_ROOT}/external/...`
- [x] W3.4 `MingGoRTS_IDE_GUI` 的 `ImageCodec.cpp`/`Logger.cpp` 改指 `${POTATO_ENGINE_ROOT}/`——**不**改連結 PotatoEngine：`GLFW_INCLUDE_NONE` 會傳染，IDEGUI 依賴 `glfw3.h` 帶入的系統 `gl.h`

## Phase 4 — 驗證（已完成，staging 於 `C:\HWC\_split_stage\`）

- [x] W4.1 引擎 standalone：MSVC 建置 + ctest **28/28**、MinGW 建置 + ctest **28/28**
- [x] W4.2 遊戲 repo：`-DPOTATO_ENGINE_ROOT=../engine-src` MSVC 建置 + ctest **44/44**
- [x] W4.3 MinGW 雙邊建置：遊戲 **44/44**（合計 72 = 68 基線 + L-6/L-8/Jamming/QuasiModels 新增）
- [ ] W4.4 `.github/workflows` 拆分兩側（遊戲 CI 加 engine checkout 步驟）——待遊戲側正式切出時做

## Phase 5 — 交接

- [x] W5.0 置換決策：遊戲側**維持 `C:\HWC\MingGoRTS` 不動**（平行 session 在途，引擎仍是 repo 內真相來源）；`C:\HWC\PotatoEngine` 由 filter-repo 產物取代（舊鏡像 repo 備份 `PotatoEngine-mirror-old/`）。遊戲側正式切出（`game-src` 已驗證，存 `_split_stage/`）留待平行工作收斂後執行
- [x] W5.1 兩側 README/AGENTS/docs 更新（引擎新 AGENTS.md 已入帳 55c28fb；MingGoRTS AGENTS.md 鏡像描述改為獨立 repo ca0980f）
- [x] W5.2 `sync-potatoengine-upstream.sh` 改寫為扁平佈局目錄級同步（ca0980f）：AI/ 混合目錄改檔案級白名單、舊 include/src 佈局安全閘、imgui submodule 不觸碰；upstream-mirror CI 路徑對齊
- [ ] W5.3 remote/branch/PR 由 user 決定——不直推 main/develop，commit 用 conventional commits（新引擎 repo 歷史重寫過，推送需 force）

## 風險

- filter-repo 路徑清單遺漏 → 用 `git ls-files` 比對原 repo 覆蓋率驗證
- 遊戲側 AI lib 同目錄 include `NeuralNetwork.h` → 靠引擎 include path 解析，需實測
- untracked 檔案歸屬錯誤 → W2.1 分側表逐一核對
