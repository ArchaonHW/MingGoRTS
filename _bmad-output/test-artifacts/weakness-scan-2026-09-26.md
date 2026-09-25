# 專案弱點掃描 — @2feabd2 + 工作區（2026-09-26）

方法：四路並行靜態審計（Gameplay/Campaign 正確性、序列化/資產健壯性、
引擎核心、架構/建置/CI/services）+ 確定性掃描（banned 函式、TODO、
rand/time）。P0 級發現已抽查核實。此為掃描報告，未修改程式碼。

banned 函式掃描：全源碼乾淨（MingGoRTS_IDE 7 命中為
IntelligentSuggestion 的偵測規則字串表，誤報）。
Gameplay/Campaign/Examples 無 TODO/FIXME 殘留。

## P0 — 崩潰 / 存檔磚 / 注入

| # | 位置 | 問題 |
|---|---|---|
| 1 | `Serialization/JsonParser.h:135-183` | 遞迴 `ParseValue` **無深度上限**——所有 JSON 入口（存檔/資產/稽核/隔離清單）共用，深巢檔 = stack overflow。`WriteJsonValue` 同 |
| 2 | `Serialization/JsonWriter.h:54-63` | `%.*g` 對非有限數寫出 `nan`/`inf`（非法 JSON）→ 存檔變磚。注入源：`AsNumber` 對字串走 `strtod` 收 `"nan"`/`"inf"`。**writer 層單點修可關整類**；下游 Roster/QuantumFog/BattlePlan/DoctrineSet 的 FromJson 仍缺 isfinite |
| 3 | `Serialization/Serialization.cpp:23-37` | `JsonSerializer::Serialize`/`BinarySerializer::Serialize` 直寫無 tmp+rename，且**不查 stream state**——磁碟滿回 true。引擎正規 SaveGame 路徑不如 CampaignState/CardEditor 的原子寫 |
| 4 | `Gameplay/Roster.cpp:29-35` | `LoadFromFile` 後 `watched` 全為 `nullptr`，`Update` 解參考即崩；另留跨場 `Squad*` 懸指標 |
| 5 | `Gameplay/FlowField.cpp:10-20` | ctor 不驗 `w/h`/`cellSize`——負維度巨額配置，`cellSize<=0`/NaN → `WorldToCell` float→int UB。經 BattleMap 內容可觸達 |
| 6 | `Media/PipelineRunner.cpp:58-68`、`Media/VideoEncoder.cpp:52-66` | **shell 命令注入**：`std::system`/`popen` 拼 JSON plan 的路徑，`"`/`$(…)` 不跳脫。已有無 shell 的 `GuiTextUtils::RunProcess` 可複用 |
| 7 | 路徑注入（多處） | `FileSystem::ResolvePath`(390-409)、`ResourceManager::ResolvePath`(442)、`SpriteAtlas` image 欄、`IDEGUI::ResolveCardArtPath`+PortraitBaker `id` 全信資產字串 → `../../` 任意讀寫。需要共用 `IsSafeRelPath` 邊界檢查 |

## P1 — 邏輯錯 / 確定性破裂 / 資源型攻擊

**確定性（核心不變量）:**
- `BattleController.h:254` `execRng{std::random_device{}()}` — 士氣執行擲骰非 seed 驅動，**同 seed 不同局、回放 hash 漂移**
- `BattleController.cpp:395` 逐 tick 迭代 `fogEntities`(unordered_map) 做 Reveal/Emit——事件序依雜湊配置
- `Governance.cpp:11-30` Accumulate 逐事件 clamp，加總序依雜湊
- `RivalDeck.cpp:37` MostUsedTrigger 平手取 hash 序 → 反制牌組不穩；FromJson 收負 count → `out` 未寫讀未初始化 enum（UB）
- `DoctrineLibrary`/`EnemyDeck`/`NarrativePack` `LoadDir` 未排序 + 同 id 後載覆蓋 → 跨機器勝出者漂移（ChapterLibrary 已排序為對照）
- `MapGenerator`/`PeaceHazard`(in-flight)/`BattleCommandEnv` 用 `uniform_real_distribution`——跨 stdlib 不同（Ledger.cpp:380 刻意 modulo 已是專案慣例）

**邏輯錯：**
- `BattleController.cpp:352` `dt` 不攔 NaN → 全場毒化 + `static_cast<int>(NaN)` UB
- `Squad.h:55` `members/maxMembers`：`maxMembers=0` 產 NaN（非崩潰）→ 士氣/處決邏輯毒化；`CreateSquad` 不擋 0
- `CampaignState.h:61` + `PeaceHazard.cpp:74`(**in-flight**)：`a<0?0:(a>1?1:a)` 對 NaN 穿透 → 強制 Collapse 結局 + 存檔寫 nan
- `BattleMap::LoadFromString` 不清舊狀態（二次載入累積）；不驗 `potato.battle_map/1` schema；rect w/h int 加法溢位 → 無窮迴圈 DoS
- `EnemyGeneral::LoadFromString` 無 schema/NaN 人格 → `FogBiasPoint`/`BuildDoctrineFor` 產 NaN
- `BattlePlan::FromJson` `attack_mul` 不攔 inf → `static_cast<int>(inf)` UB；`squadName` snprintf 不跳脫+截斷
- `BattleResources::SpendIntel/SpendLoot` 不擋負數 → 負花費變加值
- `RefitCamp::FromJson` 不驗負值/一致性；`wounded=2^31-1` → CPU DoS
- `Ledger/CampaignLedger/MythLog::ToJson` snprintf 不跳脫 `"` → 非法 JSON → `CampaignState::Save` **靜默省略帳本段**（無聲資料遺失）
- `LedgerChain::FromJson` 不驗 hash 鏈（`CampaignState::Load` 也未補 `Verify()`）→ 竄改帳簿靜默載入
- `AI/NeuralNetwork.cpp:390` Deserialize 層尺寸無上界 → 小檔案記憶體炸彈
- `Time/TimeManager.cpp:293` `UnregisterUpdateCallback` 是空實作——callback 永遠移除不了（死 API + UAF 誘因）

**引擎層（跨生命期/執行緒）:**
- `Events/EventBus.cpp:47` listener 快照 UAF——handler 銷毀已訂閱 listener，同輪仍被呼叫（單執行緒可達）
- `Core/JobSystem.cpp:52,73` job 例外穿出 worker → `std::terminate` 全進程；inFlight 失真 → WaitIdle 永久 spin
- `Resources/ResourceManager.cpp` 回傳 noop-deleter `SharedPtr` → `UnloadX` 後所有持有 handle 懸空（設計級 UAF，且記憶體管理機制形同失效：`meta.size=1024` 寫死）
- `Networking/LockstepSync.cpp:142` 未認證記憶體 DoS——每封包永久新增 map 節點
- `ECS/Entity` ID 回收無 generation counter → ABA 陳舊 handle
- `SecuritySystem.cpp:817` EnumProcessModules >1024 越界讀；`GenerateRandomBytes` 降級為時間+PID xorshift **無告警** → 簽章金鑰可預測
- `Input/InputManager` callback 迭代中 push_back → 迭代器失效
- `FileSystem.cpp:417` `readlink` 截斷 off-by-one OOB 寫；:117 讀檔無大小上限
- `~Shader`/`~AdvancedShader`/`RenderTarget::Destroy` 無 glad null 檢查（VertexArray 有——同 binary 防護不一致）

## P2/P3 — 流程與衛生

**建置/CI：**
- `Networking/` 編進 PotatoEngine 卻不在 sync `MODULES`，但 `NetworkingTest` 在 EXAMPLES 白名單 → **上游 PotatoEngine 缺模組、CI 鏡像檢查盲區**（CI 綠但上游斷 build）
- `sync-potatoengine-upstream.sh` 的 `rm -rf` 閘門只驗目標有 CMakeLists.txt + 預設路徑寫死
- `upstream-mirror.yml` paths 漏 `Networking/**`/`external/miniaudio/**`
- `pre-commit-checks.yml`/`security-scan.yml` 無 `permissions:` 區塊；banned 掃描漏 `.c/.hpp/.cc/.cxx`；兩 workflow 排除規則不一致
- `file(GLOB_RECURSE)` 無 `CONFIGURE_DEPENDS`（與「看不見的舊碼」坑同源）
- `tools/split_cmake.py` 已 stale（GAME_TESTS 缺 ~40 測試、指向不存在 target）
- `csharp/` 無 CI 建置——NativeBridge ABI 失配無人驗
- 死檔：`EnhancedAIAgentExample.cpp`/`SimpleAIDemo.cpp`/註解掉的 AIAgentGUI 系列 + 過期文件引用

**services（Java）：** 全無認證授權（DELETE replay/POST roster 公開），Eureka console 無保護；H2 sa/空密碼（file mode 風險低但勿開 TCP）。SQLi 無（全 JPA）、Flyway 與 entity 對齊 ✓

**其他：** `BattleRecorder::Attach` 裸捕 this 無 Detach；`ChapterLibrary::LoadDir` 壞檔不記檔名原因（內容 bug 無診斷）；`ChapterDef.map` 越界路徑只 warn 仍存；手寫 JSON 逃逸器普遍缺控制字元跳脫（Roster/RefitCamp/MythLog）

## 系統性主題（跨模組，最值得立「系統修復」項的）

1. **JSON 邊界信任鏈斷裂**——parser 無深度限、writer 不擋非有限、手寫產生器不跳脫、FromJson 缺 isfinite/範圍驗。需：深度上限 + writer 非有限→0 + `IsSafeRelPath` + 手寫器遷移到 WriteJson
2. **確定性殘缺**——random_device 播種、unordered_map 迭代產出、跨 stdlib 分布、未排序 LoadDir。需 seed 注入 API + 排序迭代慣例
3. **快照迭代 UAF 模式**——EventBus/GameObject/Input/AudioSource 同一型：快照防容器變動不防元素刪除。需 handle/generation 或 weak 驗活
4. **原子寫不一致**——SaveGame 正規路徑反而不是 tmp+rename
5. **全域 singleton 無同步 init/shutdown**（8+ 處）——契約不明

## Top-5 修補優先序

1. `execRng` 改 seed 注入（`BattleController` ctor 參數）——核心不變量
2. JsonParser 深度上限 + WriteJson 非有限過濾——單點關整類磚檔
3. `Serialization::Serialize` 原子化 + stream 檢查
4. `Roster::Update` nullptr 跳過 + `watched` 生命期契約
5. `IsSafeRelPath` 共用邊界檢查套到所有 JSON→path 欄位
