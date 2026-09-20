# MingGoRTS（民國史詩 RTS）

**回合制 RPG × 即時戰略的 hybrid 戰術遊戲，執行於自研 PotatoEngine（C++20）**

玩家在戰鬥中不點選單位——而是**書寫**單位。每場戰鬥前編排 doctrine 卡
（trigger → condition → action）、部署具名小隊，然後看雙方腳本即時執行，
僅以稀少的指揮點（CP）在計畫崩潰時介入數秒。

戰役圍繞兩個哲學軸：

- **至聖者無戰**——不戰而屈人之兵：倒戈優於殲滅、嚇阻優於接戰，
  指定章節可完全不戰而勝。
- **治平者無勝**——軍事勝利不是目的，治理才是：民心／秩序是
  跨戰鬥的持久軸，戰場敗北仍可轉化為治理勝利。

每場戰鬥跑在同一張地圖的兩個層面上：**歷史層**（縱隊、渡口、糧道、
電報線）與**神話層**（土地神、狐仙、戰神附體、怨靈鬼軍）。神話行為
是民心軸的主要放大器——安撫神社直接轉化為下一場戰鬥的治理資本。

---

## 架構分層

```
PotatoEngine  ←  Gameplay   ←  Campaign   ←  Examples / apps / tests
（引擎）          （單場戰鬥）   （跨章節持久）  （外殼與無頭測試）
```

依賴方向單向：引擎不依賴玩法層；玩法層不依賴戰役層；
`Gameplay` 全部 headless 可測（無 OpenGL context 需求）。

### 目錄

```
MingGoRTS/
├── Core/ Rendering/ Physics/ Audio/ Input/   # PotatoEngine 子系統
├── Resources/ ECS/ GameObject/ Events/
├── FileSystem/ Logging/ MathUtils/ Memory/
├── Platform/ Scene/ Serialization/ Time/ Security/ Quantum/
├── AI/                  # AI Agent / 強化學習（DQN）/ 神經網路
├── GUI/                 # ImGui 介面系統
├── MingGoRTS_IDE/       # 整合開發環境（智能建議含專案規範護欄：
│                        #   禁第三方 JSON／依賴方向檢查／註解與
│                        #   字串遮罩防誤報／學習權重持久化）
├── Gameplay/            # 戰鬥層：FlowField / Squad / Doctrine /
│                        #   BattleController / BattlePlanner /
│                        #   QuantumFog / BattleRecorder / Roster /
│                        #   RefitCamp / CampaignLedger /
│                        #   HistorianReport / GeneralDossier /
│                        #   RivalDeck / MythLog / Ledger /
│                        #   ChapterConventions / SquadTemplate /
│                        #   GovernanceField / GovernanceEvent /
│                        #   MapGenerator / PostBattle / PlanningDeck …
├── Campaign/            # 戰役層：CampaignState（potato.campaign/1
│                        #   facade，聚合各子存儲）/ ChapterState /
│                        #   ChapterLibrary / Governance（民心·秩序·
│                        #   墮落 ratchet 戰役累加器）
├── Examples/            # 85+ 可執行檔：demo + POTATO_TESTS 無頭測試
├── assets/              # 唯讀內容：cards/ maps/ squads/ fonts/
│                        #   avatars/ campaign/（C-3 起 templates/
│                        #   已併入 squads/ 單一目錄）
├── services/            # Java 後端服務（replay/roster，Maven）
├── external/            # vendored 第三方（tinygltf 等；勿改既有內容）
├── _bmad-output/        # BMAD 規劃產物（GDD/架構/epics/UX）
└── docs/                # 指南、架構、報告、研究文件
```

---

## 建置與測試

### 需求

- CMake ≥ 3.15、C++20 編譯器
- Windows：MSVC（VS Developer Prompt）或 MinGW
- Linux：g++（CI 覆蓋）
- OpenGL 3.3+；GLFW 由 FetchContent 自動拉取
- **不需要** vcpkg／Bullet／OpenAL——第三方僅 `external/` vendored

### Windows

```bat
BuildEngine.bat          :: 一鍵建置入口（需 VS Developer Prompt）
```

或手動：

```bash
cmake -B build -S .
cmake --build build --config Release
cd build && ctest -C Release    # 全部 POTATO_TESTS 必須通過
```

MinGW：

```bash
cmake -B build-mingw -S . -G "MinGW Makefiles"
cmake --build build-mingw
cd build-mingw && ctest
```

### 測試約定

- CTest 已啟用：新測試 exe 加進 `POTATO_TESTS` 清單（根 `CMakeLists.txt`）
- 玩法測試一律 headless——不得依賴 GL context
- 測試寫出的 replay/roster JSON 屬預期行為，勿當副作用刪除

---

## 主要系統（戰鬥層，已落地）

| 系統 | 說明 |
|---|---|
| `Doctrine` | trigger→condition→action→modifier 卡解譯器＋冷卻 |
| `BattleController` | 三拍狀態機（Planning → Execution → Aftermath） |
| `QuantumFog` | 機率雲霧：疊加/觀測/探測/衰減/糾纏/人格先驗 |
| `BattleRecorder` | 事件錄製回放（record-is-truth：顯示只是視圖）；`rootHash` 完整性根——篡改檔拒載、舊版降級警告（回放即審計） |
| `Roster` / `RefitCamp` | 具名名冊、傷亡持久、整補（部署/醫治/招募/掠奪） |
| `HistorianReport` | 史官戰報組裝器；省略計數恆在場（「本報告省略 N 項」）；查帳段消費 LedgerChain |
| `GeneralDossier` | 敵將判詞（聽聞態）戰鬥視圖 |
| `RivalDeck` | 對手讀卡：統計我方慣用 trigger → 預寫反制牌組 |
| `CampaignLedger` | 敵將處置＋稱號持久帳（potato.campaign_ledger/1） |
| `Ledger` / `LedgerChain` | 複式記帳五帳戶（武功/民心/天命/軍威/物資借貸必相等）＋ FNV-1a 雜湊鏈 append-only 帳簿：Verify() 斷鏈偵測、SoundnessViolation() 偽帳偵測、`InjectForgery` 對手偽帳注入通道、`MarkSuspect` 疑帳標記隨 `"suspect"` 欄位持久（potato.ledger_chain/1） |
| `GovernanceField` | 戰場治理追蹤（Epic A 地圖知識層）：村莊佔領/焚村標記/護輜抵達/劫輜，由有地圖知識的呼叫端每拍驅動，事件經 `RecordGovernanceEvent` 入帳 |
| `MythLog` | 神話事件具名記錄＋滲透掛鉤（potato.myth_log/1） |
| `ChapterConventions` | 章回慣例：題詞/敵將判詞/欲知後事/結局四聲部 |
| `BattlePlan` | 計畫箭頭＋量子感知加成（依情報確定度即時縮放） |
| `SquadTemplate` | 巢狀 JSON 模板→小隊實例化＋BudgetedBuild（skipReasons 逐項對齊跳過原因：unknown_id/over_budget） |

## 戰役層進度（Epic A–G，已拆 story）

- **A 治理之軸**（接線中）：`GovernanceField` 追蹤佔領/焚村/護輜/
  劫輜/運輸隊互動（地圖知識層）＋ `BattleController` 潰逃受降/
  暴行自動偵測；`Campaign::Governance` 累計民心/秩序/
  **墮落 ratchet**（只增不減），動亂事件入史官筆；
  垂直切片 `DuanqiaoPlayable` 已 Bind 地圖互動物與運輸隊
- **B 戰役持久骨架 ✅ 完成**：Potato::Log、章節定義包
  （`ChapterLibrary`）、存檔完整性（tmp+rename 原子寫）、章節地圖殼
- **C 敘事系統**（進行中）：IntelLedger 失真帳本、NarrativePack
  內容包、章回體例、GodStance、EndingPage 四手結局
- **D 神話雙層**：滲透狀態機 0–3、神社實體、天命貨幣、入侵事件
- **E 無戰章節**：談判/嚇阻/顛覆路徑
- **F 呈現層**：sprite atlas（`Rendering/SpriteAtlas` 已起）、
  UI scale、HUD 密度、audio、CJK 字體
- **G 內容工具**：卡池擴充、牌庫掠奪、敵將編輯器、沙盤、教學章

另有 **L 層帳本機械化**（複式記帳＋雜湊鏈＋查帳戰報＋回放
Merkle 根，L-1/2/3/5 已落地；L-4 帳面機制落地——`InjectForgery`
偽帳通道＋`MarkSuspect` 疑帳標記持久化＋戰報借貸不符揭露，
對手注入觸發與反制解除仍待 D-1 滲透/N-3）與**拆倉計畫**
（引擎/遊戲分 repo，見
`_bmad-output/planning-artifacts/epics-engine-split.md`）。

詳見 `_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md`
與 `_bmad-output/implementation-artifacts/sprint-status.yaml`。

### C# 工具層（規劃）

`Tools/` 將放 .NET 工具（敵將編輯器、沙盤視覺化），只經 `potato.*`
JSON schema 與 C++ CLI 驗證器互動——不連結 C++、不進 CMake/CI。

---

## 開發規範（詳見 AGENTS.md）

- 不直接推 `main`/`develop`——一律走 PR，Conventional Commits（CI 強制）
- 不修改 `external/` 既有內容；新第三方僅以「新增子目錄＋README」引入
- JSON 一律用 `Serialization/JsonParser.h`（`Potato::JsonValue`），
  不引第三方 JSON 庫；檔案 schema 版本化 `potato.<name>/<ver>`
- 禁用 C 函式 `gets|strcpy|strcat|sprintf|vsprintf|scanf`（CI 掃描）；
  用 `strncpy`/`snprintf` 等安全替代
- 本地驗證需 MSVC **與** MinGW 雙過；MinGW 專用連結用
  `if(WIN32 AND NOT MSVC)` 守衛
- 存檔寫入一律 tmp＋rename 原子路徑；壞 schema/版本 → 拒絕不動現況

## 文件索引

- [GDD](_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md)——完整遊戲設計
- [產品簡報](_bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md)——專案定位與決策
- [技術架構](_bmad-output/game-architecture.md)——分層、橫切面、8 實作模式
- [架構脊柱](_bmad-output/planning-artifacts/architecture/architecture-MingGoRTS-2026-09-18/ARCHITECTURE-SPINE.md)——架構決策紀錄
- [敘事設計](_bmad-output/narrative-design.md)——章回結構、四手結局、雙層世界
- [Epics & Stories](_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md)——34 個實作 story
- [Sprint 狀態](_bmad-output/implementation-artifacts/sprint-status.yaml)——Epic A–G 進度追蹤
- [UX 規範](_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/DESIGN.md)——12 條 UX-DR（行為面見 [EXPERIENCE](_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/EXPERIENCE.md)）
- [拆倉計畫](_bmad-output/planning-artifacts/epics-engine-split.md)——引擎/遊戲分 repo 規劃
- [引擎路線圖](_bmad-output/planning-artifacts/potato-engine-roadmap.md)——Potato Engine roadmap
- [docs/](docs/index.md)——引擎指南、架構、研究報告（完整索引見 [docs/index.md](docs/index.md)）

---

**MingGoRTS — 以筆代兵，以治為勝**

*狀態：活躍開發中 · 戰鬥原型與垂直切片 demo 可玩（dist/MingGoRTS-Demo）· Epic B 戰役骨架完成 · Epic A 治理追蹤接線中 · L 層帳本機械化落地（L-4 帳面機制完成、對手觸發待 D-1/N-3）· 引擎/遊戲拆倉規劃中*
