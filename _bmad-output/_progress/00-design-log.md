# Design Log

## Current

- Spec: `_bmad-output/implementation-artifacts/spec-ide-dev-assistant.md` (status: in-progress, baseline d5eff75)
- Branch: `feat/engine-security-hardening`
- Activity: [D] Development — 實作大多已在工作樹中（先前 session 成果），本輪以驗證為主

## Design Loop Status

| Scenario slug | NN.X | Page name | Status | Date |
|---|---|---|---|---|
| ide-dev-assistant | - | IDE Intelligent Development Assistant | building | 2026-09-16 |

## Verification Evidence (2026-09-16)

- `DevAssistantSmoke` MSVC Release build clean；14 assertions ALL PASS
- `MingGoRTS_IDE_GUI` MSVC Release build clean（僅既有 warnings）
- Banned-function grep（`gets|strcpy|strcat|sprintf|vsprintf|scanf`）MingGoRTS_IDE/ AI/：0 new matches
- 修正：`CopyToBuffer` 的 `strncpy`/`strncat` 在 `/sdl` 下為 C4996 error → 改 `memcpy` + 精確長度
- MinGW 未安裝於本機，僅驗證 MSVC 路徑（spec acceptance 為「MSVC or MinGW」）
- 迴歸：`AITestSuite` 7/7 PASS、`SecurityRedTeamTest` 39 PASS / 0 BYPASS
- 注意：build cache 由 scoop cmake 4.4 生成，`cmake --build` 需用同一把（VS bundled 4.2 會因模組路徑混用在 reconfigure 時失敗）

## Spec Deviations

- `CopyToBuffer` 使用 `memcpy` + 精確長度而非 `strncpy`/`strncat`：`/sdl` 將 C4996 視為 error，且 CI banned-function 掃描本就禁 `strncpy`。行為等價（NUL 結尾 + `...[truncated]` 標記）。

## PR Description（草稿）

**feat(ide): wire Development Assistant to in-process codegen pipeline**

- Summary：IDE Development Assistant 改由引擎自家 pipeline 驅動——NLP 意圖解析 → KnowledgeGraph 專案上下文 → 模板化 C++ 合成，離線可用不需外部 LLM；外部 LLM client 保留為可選 fallback。
- Changes：`AI/IntelligentDevelopmentSystem.*`（`GenerateLocal`/`ParseIntent`/模板表）、`IDEGUI.cpp/h`（`std::async` 非同步生成 + `PollDevelopmentResult` 每帧寫回、`developmentProcessing` 防重入、`POTATO_LLM_*` 環境變數預設 + Settings UI）、`Examples/DevAssistantSmoke.cpp`（無 LLM 的 CLI 驗證）、`CMakeLists.txt`（新 target）
- Testing：`cmake --build build --target DevAssistantSmoke` → 14 assertions；GUI：Ctrl+D → prompt → response view 出現程式碼且 UI 不卡
- Deviations：`memcpy` 取代 `strncpy`（/sdl 相容）

## Acceptance Criteria

- [x] 無外部 LLM 時，可識別 prompt → response view 出現合成 C++（smoke 驗證本地管線；UI async poll 寫回）
- [x] 不可識別 prompt → 明確錯誤訊息，不 hang/crash/改 editor（smoke 驗證）
- [x] MSVC `cmake --build` 通過；無新增 banned function
