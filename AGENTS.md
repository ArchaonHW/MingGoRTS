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
- `C:\HWC\PotatoEngine`（repo 外）是**產生的唯讀鏡像**——repo 內引擎是唯一真相來源，上游不直接編輯；引擎模組變更後跑 `tools/sync-potatoengine-upstream.sh`（Git Bash）重新同步，上游殘留檔自動移入其 `legacy/`

## Running and verifying

- 本地建置必須 MSVC 與 MinGW 都過——MinGW 專用連結用 `if(WIN32 AND NOT MSVC)` 守衛（參考 psapi 寫法）
- 沒有測試框架/ctest——驗證 = `cmake --build build` 後跑 `Examples/` 執行檔（如 `AITestSuite`）
- CI 在 Linux g++ 建置；另掃 banned C 函式 `gets|strcpy|strcat|sprintf|vsprintf|scanf`——用 `strncpy`/`snprintf` 等安全替代
- 需求：CMake ≥3.15、C++20 編譯器、OpenGL；GLFW 由 FetchContent 拉取

## Known pitfalls

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
- AI 訓練：`BattleTrainerDemo` 用 `BattleCommandEnv`（包 BattleController 成 RLEnvironment）訓練 DQN 指揮官；`DQNAgent::SeedWeights`+`SetSeed`+seeded env = 完全可重現。注意：PG/AC/DQN 輸出層是 `linear`（Q 值/ logits 需無界），ReLU 輸出層會截斷負值
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
