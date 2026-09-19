---
stepsCompleted: ["step-01-requirements"]
inputDocuments:
  - _bmad-output/planning-artifacts/engine-game-split-work-plan.md
  - _bmad-output/game-architecture.md
  - _bmad-output/planning-artifacts/potato-engine-roadmap.md
  - AGENTS.md
---

# Engine/Game 拆倉 — Epic Breakdown

## Overview

需求來源：使用者決策「遊戲 repo + 上游引擎 repo、保留 git history、Examples 留引擎、AI/* 演算法庫歸遊戲」+ `engine-game-split-work-plan.md` 的邊界分析。

## Requirements Inventory

### Functional Requirements

- FR1: PotatoEngine 成為獨立 repo，編譯出不依賴任何遊戲層（Gameplay/Campaign/MingGoRTS_IDE）的靜態庫
- FR2: MingGoRTS 成為獨立 repo，以外部依賴方式消費 PotatoEngine（add_subdirectory / POTATO_ENGINE_ROOT）
- FR3: 拆倉保留完整 git history（git filter-repo 路徑過濾）
- FR4: 遊戲層所有權：Gameplay/、Campaign/、MingGoRTS_IDE/、assets/、services/、AI/*（除 NeuralNetwork）、GUI/AgentGUI
- FR5: 引擎層所有權：Core/Security/Events/FileSystem/Logging/Memory/Platform/Scene/Serialization/Time/Rendering/Physics/Audio/Input/Resources/ECS/GameObject/MathUtils/Quantum/Media/、AI/NeuralNetwork.*、external/
- FR6: Examples 依連結依賴分側：連結 Gameplay/Campaign/遊戲 AI lib 的歸遊戲；只連結 PotatoEngine/NeuralNetwork 的留引擎
- FR7: 未提交變更（CMakeLists MythLog、AGENTS.md、ChapterConventions、SuggestionRulesTest 等）在拆倉後完整保留於正確側
- FR8: 遊戲側 GUI target（IDE_GUI、DuanqiaoPlayable 等）直接編譯 imgui .cpp 的引用改經 ${POTATO_ENGINE_ROOT}/external
- FR9: MingGoRTS_IDE_GUI 對 Rendering/ImageCodec.cpp、Logging/Logger.cpp 的直編改為連結 PotatoEngine lib
- FR10: POTATO_TESTS 依 target 歸屬分流到兩側 ctest

### NonFunctional Requirements

- NFR1: 依賴方向單向——引擎不可反向依賴遊戲（編譯期驗證）
- NFR2: MSVC + MinGW 雙 toolchain 雙邊都過（AGENTS.md 既有要求）
- NFR3: CI Linux g++ 建置；遊戲 CI 需加 engine checkout 步驟
- NFR4: 消費引擎時（add_subdirectory）不註冊引擎 examples/tests——standalone 守衛
- NFR5: 不破壞 conventional commits / 不直推 main·develop / 不碰 external/ 既有內容

### Additional Requirements (Architecture)

- `CMAKE_SOURCE_DIR` → `CMAKE_CURRENT_SOURCE_DIR` 全量替換（引擎被 add_subdirectory 消費的前提）
- `ReinforcementLearning.cpp` 的 `#include "NeuralNetwork.h"` 依賴引擎 include path 解析（同目錄 include fallback → -I 路徑）
- 現有 `C:\HWC\PotatoEngine` 為 sync 鏡像非 git repo → 備份後以 filter-repo 產物取代
- `tools/sync-potatoengine-upstream.sh` 退役或改寫

### FR Coverage Map

| FR | Epic | 說明 |
|---|---|---|
| FR1 | ES-0, ES-2 | 引擎獨立編譯（先解耦、後收斂 CMake） |
| FR2 | ES-2 | 遊戲消費引擎 |
| FR3 | ES-1 | filter-repo 保留 history |
| FR4, FR5 | ES-1 | 歸屬表 → 路徑清單 |
| FR6 | ES-0, ES-1 | Examples 分側（決策在 0，執行在 1） |
| FR7 | ES-0.3, ES-1.2 | 未提交變更快照與回灌 |
| FR8, FR9 | ES-2.1 | imgui / IDE_GUI 直編改寫 |
| FR10 | ES-2, ES-3 | POTATO_TESTS 分流 + 驗證 |
| NFR1~NFR5 | ES-3 | 全部在驗證 epic 收口 |

## Epic List

| Epic | 標題 | 目標 | FRs |
|---|---|---|---|
| ES-0 | 前置決策與邊界固化 | monorepo 內先解耦、CMake 可消費化 | FR1, FR6, FR7 |
| ES-1 | 歷史拆分 | filter-repo 產出兩個帶完整 history 的 repo | FR3, FR4, FR5, FR6, FR7 |
| ES-2 | 消費架構落地 | 遊戲 repo 以 add_subdirectory 消費引擎、雙邊 CMake 收斂 | FR1, FR2, FR8, FR9, FR10 |
| ES-3 | 驗證與交接 | 雙 toolchain 建置測試、CI 拆分、文件更新、PR 流程 | FR10, NFR1~5 |

## Epic ES-0: 前置決策與邊界固化

在動 git history 之前，先在 monorepo 內把引擎/遊戲邊界做實，讓拆出去的引擎第一天就能獨立編譯。

### Story ES-0.1: 解除引擎 → 遊戲 AI 的殘留耦合

As a engine maintainer,
I want PotatoEngine 不引用 AIAgentSystem/遊戲 AI，
So that 引擎 repo 可獨立編譯。

**Acceptance Criteria:**

**Given** monorepo 現狀（GUI/AgentGUI → AI/AIAgentSystem.h）
**When** AgentGUI.{h,cpp} 移至遊戲側範圍並從引擎 target 移除
**Then** `PotatoEngine` lib 編譯通過且 source 掃描無 `AI/`（NeuralNetwork 除外）引用
**And** `NeuralGraphics`/`NeuralArtTool` 等依賴 NeuralNetwork 的功能仍可用
**And** 跨邊界 unqualified include 先行改寫：`AI/ReinforcementLearning.{h,cpp}` 的 `#include "NeuralNetwork.h"` 改為 `"AI/NeuralNetwork.h"`（限定路徑，經引擎 root -I 解析；拆倉後同目錄解析會失效）

### Story ES-0.2: CMake 可消費化改造

As a game developer,
I want 引擎 CMakeLists 可在 add_subdirectory 情境下正確運作，
So that 遊戲 repo 能直接消費引擎原始碼。

**Acceptance Criteria:**

**Given** 引擎 CMakeLists.txt
**When** 全部 `CMAKE_SOURCE_DIR` 改為 `CMAKE_CURRENT_SOURCE_DIR`，且 examples/tests/install 以 `CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR` 守衛包住（不用 `PROJECT_IS_TOP_LEVEL`——專案下限 CMake 3.15，該變數需 ≥3.21）
**Then** monorepo standalone 建置行為不變
**And** 以 add_subdirectory 引用時只產出 lib target（glad/glfw/PotatoEngine/NeuralNetwork/Quantum/Media）

### Story ES-0.3: 未提交變更快照與歸屬

As a release engineer,
I want 工作樹未提交變更的完整快照與路徑歸屬表，
So that filter-repo clone 後能精準回灌到正確的 repo。

**Acceptance Criteria:**

**Given** `git status` 中 10 個 modified + 全部 untracked 檔
**When** 產出 `git diff HEAD` patch + untracked 檔案依歸屬表分側的清單
**Then** 每個檔案明確標記 engine/game/skip，無遺漏
**And** 快照範圍含：LogTest.cpp/Log.cpp（引擎側）、ChapterConventions.*/ChapterConventionsTest（遊戲側）、CMakeLists.txt/README.md（兩側）、DuanqiaoPlayable B.5 章節地圖殼（遊戲側）

## Epic ES-1: 歷史拆分

用 git filter-repo 從 monorepo 產出兩個保留完整 history 的 repo。

### Story ES-1.1: Engine repo 過濾

As a engine maintainer,
I want 一個只含引擎路徑、保留完整 history 的 PotatoEngine repo，
So that 引擎可獨立演進與發布。

**Acceptance Criteria:**

**Given** MingGoRTS clone + 引擎路徑清單（FR5 + 引擎型 Examples + 根檔案）
**When** `git filter-repo --paths-from-file` 執行
**Then** repo 含全部引擎 commit history
**And** `git rm` 跨界檔（GUI/AgentGUI.*）後 `git ls-files` 無遊戲路徑殘留
**And** `.gitmodules`（external/imgui submodule gitlink）歸引擎 repo；UE5 legacy 路徑（Plugins/Source/Math/Temp/Content）刻意不列入任何一側——死代碼隨過濾丟棄、倉庫瘦身

### Story ES-1.2: Game repo 過濾

As a game developer,
I want 一個只含遊戲路徑、保留完整 history 的 MingGoRTS repo，
So that 遊戲開發歷史不中斷。

**Acceptance Criteria:**

**Given** MingGoRTS clone + 遊戲路徑清單（FR4 + 遊戲型 Examples + _bmad/docs/services 等）
**When** filter-repo 執行 + `git rm AI/NeuralNetwork.*`
**Then** 遊戲 commit history 完整
**And** 未提交變更（ES-0.3 快照）回灌正確側

## Epic ES-2: 消費架構落地

遊戲 repo 透過 `POTATO_ENGINE_ROOT` + `add_subdirectory` 消費引擎。

### Story ES-2.1: 遊戲 CMakeLists 改寫

As a game developer,
I want `project(MingGoRTS)` + `add_subdirectory(${POTATO_ENGINE_ROOT})`，
So that `cmake -DPOTATO_ENGINE_ROOT=../PotatoEngine` 可完整建置遊戲。

**Acceptance Criteria:**

**Given** 遊戲 repo
**When** CMakeLists 改寫完成
**Then** Gameplay/Campaign/遊戲 AI libs/IDE target 全部可建
**And** imgui 直編 target 引用 `${POTATO_ENGINE_ROOT}/external/imgui`（FR8）
**And** IDE_GUI 的 ImageCodec/Logger 直編改為連結 PotatoEngine（FR9）
**And** `POTATO_ENGINE_ROOT` 無效/缺目錄時 configure 階段 `FATAL_ERROR` 明確提示設定方式

### Story ES-2.2: 引擎 CMakeLists 精簡

As a engine maintainer,
I want 引擎 CMakeLists 移除全部遊戲 target，
So that engine repo standalone 建置只剩引擎內容。

**Acceptance Criteria:**

**When** 引擎 CMakeLists 移除遊戲 lib/executable/測試
**Then** `cmake --build` 只產出引擎 target + 引擎型 Examples
**And** POTATO_TESTS 只含引擎測試

## Epic ES-3: 驗證與交接

### Story ES-3.1: 雙邊雙 toolchain 驗證

As a release engineer,
I want 兩個 repo 各自 MSVC + MinGW 建置 + ctest 通過，
So that 拆倉沒有破壞任何功能。

**Acceptance Criteria:**

**When** 引擎 standalone、遊戲（消費引擎）分別建置
**Then** 雙邊 ctest 全過，測試數 = 原 POTATO_TESTS 分流總和
**And** MinGW 建置同樣通過

### Story ES-3.2: CI 與文件交接

As a maintainer,
I want CI workflow 拆分 + 兩側 README/AGENTS 更新，
So that 貢獻者知道新的建置流程。

**Acceptance Criteria:**

**When** .github/workflows 分側配置（遊戲 CI 加 engine checkout）
**And** README 描述新的消費方式、sync script 處置完成
**Then** CI 綠燈
**And** 所有變更走 PR（conventional commits），不直推 main/develop
