<!-- bmad:context -->
<!-- Verified 2026-09-16 against 91989f7. Managed by bmad-project-context; edits inside this block are replaced on refresh. Keep anything you want preserved outside the markers. -->

## MingGoRTS (Potato Engine)

C++20 遊戲引擎 + MingGoRTS IDE，CMake 建置，無 UE5 依賴。BMAD v6 已安裝：skills 在 `.agents/skills/`，本體在 `_bmad/`，規劃產出到 `_bmad-output/`。長篇文件與報告在 `docs/`。

## Policy

- 不直接推 `main`/`develop`——所有變更走 PR。
- Commit message 與 PR title 必須是 conventional commits（CI commitlint 強制）。
- 不修改 `external/`（第三方依賴）與 `build/`（建置輸出）。
- 禁止硬編碼 secrets / 憑證檔——CI security-scan 會擋。

## Where things are

- 引擎與所有 target 定義：根 `CMakeLists.txt`（唯一 CMake 清單，無子目錄 CMakeLists）
- Windows 建置入口：`BuildEngine.bat`（需 VS Developer Command Prompt）
- Agent 語言規則：`.windsurf/AGENT_LANGUAGE_CONFIG`——回覆與註解用繁體中文，識別符與檔名保持英文
- `C:\HWC\PotatoEngine`（repo 外）是活的上游參考副本——別與 repo 內引擎混淆，相關改動需留意同步

## Running and verifying

- 本地建置必須 MSVC 與 MinGW 都過——MinGW 專用連結用 `if(WIN32 AND NOT MSVC)` 守衛（參考 psapi 寫法）
- 沒有測試框架/ctest——驗證 = `cmake --build build` 後跑 `Examples/` 執行檔（如 `AITestSuite`）
- CI 在 Linux g++ 建置；另掃 banned C 函式 `gets|strcpy|strcat|sprintf|vsprintf|scanf`——用 `strncpy`/`snprintf` 等安全替代
- 需求：CMake ≥3.15、C++20 編譯器、OpenGL；GLFW 由 FetchContent 拉取

## Known pitfalls

- IDE GUI state 用固定長度 char buffer（如 `state.developmentResponse`）——必須 `strncpy` + 結尾 `\0` 或 `memset`，不可直接 `=` 指派 `std::string`

<!-- /bmad:context -->
