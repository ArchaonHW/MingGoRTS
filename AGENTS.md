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
- `C:\HWC\PotatoEngine`（repo 外）是**獨立引擎 repo**（filter-repo 保留歷史，扁平模組佈局；舊 include/+src/ 唯讀鏡像已廢除）——遊戲側正式切出（`add_subdirectory` 消費）前 repo 內引擎仍是真相來源，上游模組目錄不直接編輯；引擎模組變更後跑 `tools/sync-potatoengine-upstream.sh`（Git Bash）單向同步到上游，並在上游 repo 提交結果

## Running and verifying

- 本地建置必須 MSVC 與 MinGW 都過——MinGW 專用連結用 `if(WIN32 AND NOT MSVC)` 守衛（參考 psapi 寫法）
- CTest 已啟用（`POTATO_TESTS` 清單）：`cd build && ctest -C Release`；驗證 = 建置 + ctest 全綠
- CI 在 Linux g++ 建置；另掃 banned C 函式 `gets|strcpy|strcat|sprintf|vsprintf|scanf`——用 `strncpy`/`snprintf` 等安全替代
- 需求：CMake ≥3.15、C++20 編譯器、OpenGL；GLFW 由 FetchContent 拉取

## Known pitfalls

- IDE GUI state 用固定長度 char buffer（如 `state.developmentResponse`）——必須 `strncpy` + 結尾 `\0` 或 `memset`，不可直接 `=` 指派 `std::string`

<!-- /bmad:context -->

## Gameplay 層（doctrine 戰鬥原型）

- `Gameplay/`：斷橋原型的玩法層——`FlowField`（群體尋路）/`Squad`/`Doctrine`（含規則冷卻）/`BattleController`（三拍狀態機）/`BattlePlanner`（AI 參謀）/`BattleMap`（地圖 JSON+互動點）/`EnemyGeneral`（敵將人格腳本）/`BattleResources`（情報/CP/士氣執行率）/`BattleRecorder`（事件錄製回放，存檔帶 rootHash 完整性驗證）/`Roster`（名冊）
- 情報戰：`QuantumFog`（機率雲疊加/觀測/探測/糾纏/人格先驗/相位）+ `BattlePlan`（計畫箭頭，加成依情報確定度即時縮放）
- 敘事/帳本：`HistorianReport`（史官戰報+查帳段）/`CampaignLedger`/`GeneralDossier`/`RivalDeck`/`MythLog`/`ChapterConventions`/`Ledger`+`LedgerChain`（複式記帳五帳戶+雜湊鏈帳簿）
- 後勤：`RefitCamp`（跨場整補）/`SquadTemplate`（模板配兵）/`PostBattle`/`MapGenerator`
- `Gameplay` lib 連結 `PotatoEngine`（BattleSceneSync 需要場景類型）；`PotatoEngine` 不反向依賴 Gameplay
- 資產 schema：`assets/cards/` 角色卡（`potato.character_card/1`）、`assets/maps/` 戰場圖（`potato.battle_map/1`）、回放 `potato.battle_replay/1`（含 rootHash，舊檔降級載入）、名冊 `potato.roster/1`、帳簿 `potato.ledger_chain/1`
- JSON 解析用 `Serialization/JsonParser.h`（`Potato::JsonValue`），不要引第三方 JSON 庫

## Campaign 層（戰役持久，Epic B 已完成）

- `Campaign/`：`CampaignState`（potato.campaign/1 facade，聚合 Camp/NamedRoster/CampaignLedger/Governance/MythLayer，章節邊界 tmp+rename 原子存檔；**不含** GeneralDossier/MythLog——兩者目前 session 級）/`ChapterLibrary`（章節定義庫，arc→chapter→id 排序）/`Governance`（治理累加器：佔村/護輜/暴行→民心秩序軸）
- `Gameplay` 不依賴 `Campaign`——依賴方向 PotatoEngine ← Gameplay ← Campaign
- 垂直切片：`Examples/DuanqiaoPlayable`（標題殼→章節選擇→戰鬥→史官戰報+整補→再戰）；`tools/package_demo.bat` 產出 `dist/` 自包含包

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
- 目前 vendored：`external/tinygltf/`（tinygltf v2.9.7 + nlohmann/json + stb_image/stb_image_write，glTF/VRM 載入用）、`external/miniaudio/`（miniaudio 0.11.22 單標頭，public domain/MIT-0，F-4 音訊後端——實作巨集在 `Audio/MiniaudioImpl.cpp`）

## 平行開發衛生（多 session 協作）

- 本 repo 常有多 session 並行——提交前 `git status` 認清自己的檔，只 stage 自己改的（`git add <path>` 不用 `-A`）
- 提交引用**未追蹤檔案**的 CMakeLists 會讓乾淨 checkout 斷 build——註冊新 target 時源檔必須同 commit 入帳
- `CMakeLists.txt`/`DuanqiaoPlayable.cpp`/佇列檔是高碰撞熱區；要拆 hunk 可「checkout HEAD → 重放自己的編輯 → stage → 還原工作檔」
- **hash-object staged 法**（Epic D retro 實證，三次 index 競態後採用）：對混有平行 session 變更的熱區檔，`git show HEAD:<file> > base` → 套用自己的 hunk → `git hash-object -w` → `git update-index --cacheinfo 100644,<blob>,<file>`——stage 的是乾淨 blob 而非整檔工作區內容。提交一律 `git commit -- <path>...`（path-limited）防 commit 間隙被對方 stage 污染
- 測試同時寫同名 JSON 會互撞——ctest 批量失敗先單獨重跑確認是不是平行測試競爭，再當真 bug 追

## Story 完成定義（採用閘門，Epic D retro 規則）

- 機械層 story 不算 done 除非至少有一個**測試外消費者**（playable/工具/資產檔）實際呼叫——否則標「mechanism-only」並在 queue/spec 明記採用層另立 story。Epic D 的 D-2/D-3/D-4 曾因缺此閘門讓單測全綠的死碼活到 retro 才浮現
- 新機制入帳時同步檢查：內容端（assets JSON schema 欄位）是否有至少一個定義塊觸發它

## 已知環境坑：MinGW libstdc++ DLL 錯配

- 在 git-bash 手工 `g++` 編出的 binary 會依 PATH 載入 `libstdc++-6.dll`——`C:\Program Files\Git\mingw64\bin` 若排在 scoop MinGW 前面，會載到**版本不符的 DLL**，`-O1/-O2` 下 segfault（`-O0` 僥倖通過）。
- 解法：執行前 `export PATH="/c/Users/potat/scoop/apps/mingw/16.2.0-rt_v14-rev1/bin:$PATH"`，或連結時加 `-static-libstdc++ -static-libgcc`。
- `build-mingw/bin/` 的 CMake 產物不受影響（ctest 全部正常）。
