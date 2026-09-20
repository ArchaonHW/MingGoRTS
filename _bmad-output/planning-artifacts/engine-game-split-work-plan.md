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

## Phase 1 — 邊界固化（monorepo 內先做）

- [ ] W1.1 `GUI/AgentGUI.{h,cpp}` 移出引擎範圍（遊戲側安置），解除 `PotatoEngine`→AIAgentSystem 殘留耦合
- [ ] W1.2 引擎 CMake：`CMAKE_SOURCE_DIR` → `CMAKE_CURRENT_SOURCE_DIR`（全檔，add_subdirectory 消費前提）
- [ ] W1.3 standalone 守衛：`if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)` 包住 examples/tests/install——被消費時只產出 lib target
- [ ] W1.4 驗證：monorepo 建置 + ctest 無回歸（此步可先提交）

## Phase 2 — 歷史拆分（git filter-repo，已安裝）

- [ ] W2.1 導出未提交變更：`git diff HEAD` + untracked 清單 → 依歸屬表分側
- [ ] W2.2 clone → engine repo → `filter-repo --paths-from-file`（引擎路徑清單）
- [ ] W2.3 clone → game repo → `filter-repo --paths-from-file`（遊戲路徑清單）
- [ ] W2.4 跨界檔後處理：engine `git rm GUI/AgentGUI.*`；game `git rm AI/NeuralNetwork.*`（header 經引擎 include path 取得）
- [ ] W2.5 未提交變更回灌各側 working tree

## Phase 3 — 新 CMake 架構

- [ ] W3.1 引擎 CMakeLists 精簡：去遊戲 lib/executable/POTATO_TESTS 遊戲項
- [ ] W3.2 遊戲 CMakeLists：`project(MingGoRTS)` + `POTATO_ENGINE_ROOT` cache var + `add_subdirectory`
- [ ] W3.3 imgui/external 引用改 `${POTATO_ENGINE_ROOT}/external/...`（IDE_GUI、DuanqiaoPlayable 等直接編 imgui .cpp 的 target）
- [ ] W3.4 `MingGoRTS_IDE_GUI` 直接編譯的 `Rendering/ImageCodec.cpp`、`Logging/Logger.cpp` 改指引擎路徑（或改連結 PotatoEngine lib——建議後者）

## Phase 4 — 驗證

- [ ] W4.1 引擎 repo standalone：configure + build + ctest（MSVC）
- [ ] W4.2 遊戲 repo：`cmake -DPOTATO_ENGINE_ROOT=...` + build + ctest
- [ ] W4.3 MinGW 雙邊建置（AGENTS.md 要求 MSVC+MinGW 都過）
- [ ] W4.4 `.github/workflows` 拆分兩側（遊戲 CI 加 engine checkout 步驟）

## Phase 5 — 交接

- [ ] W5.1 兩側 README/AGENTS/docs 更新
- [ ] W5.2 `sync-potatoengine-upstream.sh` 退役或改寫為引擎 repo 的 CI 發布腳本
- [ ] W5.3 remote/branch/PR 由 user 決定——不直推 main/develop，commit 用 conventional commits

## 風險

- filter-repo 路徑清單遺漏 → 用 `git ls-files` 比對原 repo 覆蓋率驗證
- 遊戲側 AI lib 同目錄 include `NeuralNetwork.h` → 靠引擎 include path 解析，需實測
- untracked 檔案歸屬錯誤 → W2.1 分側表逐一核對
