---
title: 'D-1 MythLayer 滲透狀態機（quiet→anomalies→seep→manifest）'
type: 'feature'
created: '2026-09-22'
status: 'done'
baseline_commit: 'b60c921'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/Gameplay/MythLog.h'
  - '{project-root}/Gameplay/GovernanceEvent.h'
  - '{project-root}/Campaign/CampaignState.h'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** 神話層目前只有 `MythLog` 事後記錄——沒有「滲透等級」這個機械狀態。
Epic D 全部故事（D-2 神社、D-3 天命貨幣、D-4 入侵、D-5 視效）都掛在
「區域滲透等級 0~3 隨事件推進」這根骨幹上；epics.md 架構約束指定
`Campaign/MythLayer.*` 持有它（純邏輯、headless，如 `QuantumFog`）。

**Approach:** 新增 `Campaign::MythLayer`——每區域 `{level, pressure}` +
神明好感表（god-stance）。治理事件（屠殺/焚村）累壓力，跨閾值升階並錄
轉換記錄（record-is-truth）；章內等級只升不降（ratchet），章節邊界由
`CampaignState::AdvanceChapter` 觸發 `DeriveFrom` 從治理帳重推導。

## Boundaries & Constraints

**Always:**
- 四階 `enum class Seepage { Quiet=0, Anomalies=1, Seep=2, Manifest=3 }`；
  區域以 string id 鍵控（地圖空間綁定屬 D-2 範疇）
- 章內單調：level 只升不降——壓力可波動，等級一旦跨閾不回落
- 每次跨階產生 transition 記錄（region/from/to/章節序），並經回呼
  發一筆 `MythEvent` 給 `MythLog`（record-is-truth）
- 神明態度：favor 表 `AdjustFavor/Favor`（夾 [0,100]）；怒神
  （favor < 閾值）區域的暴行類事件加成壓力
- 純邏輯無引擎依賴；banned C 函式禁令；JSON 用 `Potato::JsonValue`
- 依賴方向：`Gameplay ← Campaign`——MythLayer 放 `Campaign/`，
  可含入 `Gameplay/GovernanceEvent.h` / `MythLog.h`

**Ask First:**
- 若要把滲透等級接進 `BattleController`/`GovernanceField` 做戰中
  自動驅動（那是 D-2 地圖節點的範疇，本項只提供 `Feed` API）
- 若要改動 `MythLog`/`MythEvent` 既有 schema 或 `GovernanceEvent` 列舉

**Never:**
- 不做視覺/音效實作（D-5）；不做神社/精靈實體（D-2）；不做天命
  貨幣兌換（D-3）；不做入侵事件觸發（D-4）
- 不讓 `Gameplay/` 任何檔案含入 `Campaign/` 標頭
- 渲染層只能讀——不提供任何會改變狀態的 API 給消費端以外路徑
- 不引入隨機/時間戳進推進邏輯（全確定性）

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 閾值升階 | 區域連續 Feed 暴行事件 | 壓力跨 `kAnomalies`→level 0→1，錄轉換 | N/A |
| 單調ratchet | 已 Seep 後 Feed 仁政事件 | level 不降（壓力可降、等級鎖） | N/A |
| 多區域獨立 | 只 Feed region "渡口" | region "神社" 仍 Quiet | 未知區域視為 Quiet 且自動建檔 |
| 跨章重推導 | `DeriveFrom` 高墮落/低秩序治理快照 | 各區域補壓力、等級推進 | N/A |
| 重推導不降 | `DeriveFrom` 清廉治理快照 | 既有等級不回落 | N/A |
| 怒神加成 | spirit favor < `kAngryFavor` 的區域 Feed 暴行 | 壓力增量 > 正常值 | favor 邊界夾取 |
| 跳階 | 單次大壓力 Feed | 連跨多階、每階各錄一筆轉換 | N/A |
| 持久化 | ToJson→FromJson roundtrip | levels+favor+轉換記錄逐位一致 | 壞檔回 false 不更動 |
| 缺段降級 | 無 `myth_layer` 段的舊存檔 | 載入成功、全區域 Quiet | 不報錯 |
| 渲染唯讀 | `Level()`/`Levels()` | const 存取、無 mutator 外漏 | N/A |

</frozen-after-approval>

## Code Map

- `Campaign/MythLayer.h/.cpp` -- 新：`Seepage` enum + 區域態 + god-stance 表 + `Feed`/`DeriveFrom`/JSON（`potato.myth_layer/1`）
- `Campaign/CampaignState.h/.cpp` -- 掛 `Myths()` 成員 + 存檔 `myth_layer` 段 + `AdvanceChapter` 呼叫 `DeriveFrom`
- `Gameplay/GovernanceEvent.h` -- 既有事件表（Feed 的輸入型別，不改）
- `Gameplay/MythLog.h` -- `MythEvent`/`Record`（轉換事件的敘事出口，不改）
- `Examples/MythLayerTest.cpp` -- 新無頭測試（風格同 LedgerProvenanceTest）
- `CMakeLists.txt` -- 測試 target + `POTATO_TESTS` 註冊

## Tasks & Acceptance

**Execution:**
- [x] `Campaign/MythLayer.h` -- `Seepage` enum、`MythLayer` 類（`Feed`/`DeriveFrom`/`Level`/`Levels`/`AdjustFavor`/`Favor`/`Transitions`/`ToJson`/`FromJson`/`SetEventCallback`）宣告 -- API 定義
- [x] `Campaign/MythLayer.cpp` -- 壓力累積 + 閾值 ratchet + 怒神加成 + 轉換記錄 + JSON -- 狀態機落地
- [x] `Campaign/CampaignState.h/.cpp` -- `myth` 成員 + `Myths()` 存取器 + `myth_layer` 存檔段 + `AdvanceChapter` 內 `DeriveFrom` -- 戰役整合
- [x] `Examples/MythLayerTest.cpp` -- 無頭測試覆蓋 I/O 矩陣全列 -- 驗證
- [x] `CMakeLists.txt` -- 註冊 `MythLayerTest` 進 `POTATO_TESTS` -- 測試入列

**Acceptance Criteria:**
- Given 區域治理事件流，when 壓力跨閾值，then 滲透等級推進且轉換錄事件
- Given 任一已升階區域，when 後續任何事件或重推導，then 等級不下降
- Given 章節邊界 `AdvanceChapter`，when 觸發，then `DeriveFrom` 以治理快照重推導
- Given `potato.campaign/1` 存檔，when roundtrip，then `myth_layer` 段完整保留；缺段舊檔照載

## Design Notes

壓力閾值為類別常數（如 `kAnomalies`/`kSeep`/`kManifest`），暴行類事件
權重高於一般治理負面事件——`Feed` 簽章收 `GovernanceEvent` 直接複用
既有 delta 表的語義分類，不另建平行事件列舉。

`DeriveFrom` 語義：章節邊界以治理快照（depravity、civilOrder、unrest）
換算壓力注入各已知區域再 ratchet——「土地不會遺忘」，跨章同樣只升不降。
新區域在 Feed 時自動建檔為 Quiet。

怒神耦合：區域可綁定守護靈（`BindSpirit(region, spirit)`），其 favor
低於 `kAngryFavor` 時該區域暴行壓力 ×加成——神明態度機械性放大滲透，
供 D-3 天命兌換的 favor 欄位預留。

## Verification

**Commands:**
- `cmake --build build --config Release --target MythLayerTest` -- 編譯通過(MSVC)
- `cd build && ctest -C Release` -- 全綠(含既有測試不回歸)
- MinGW 編譯 `MythLayerTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中

## Suggested Review Order

**狀態機核心**

- 滲透四階 enum 與閾值常數——設計入口,先讀語義再看機制
  [`MythLayer.h:32`](../../Campaign/MythLayer.h#L32)

- Feed:治理事件→壓力 delta,怒神加成只限暴行類(Atrocity/VillageBurned)
  [`MythLayer.cpp:37`](../../Campaign/MythLayer.cpp#L37)

- Ratchet:跨階錄轉換,事件收集而非即發——防回呼再入 rehash 懸空
  [`MythLayer.cpp:54`](../../Campaign/MythLayer.cpp#L54)

- Dispatch:變異完成後統一派出 MythEvent,回呼再入層安全
  [`MythLayer.cpp:81`](../../Campaign/MythLayer.cpp#L81)

**神明態度**

- AdjustFavor:未註冊 spirit 以 kNeutralFavor 起算(try_emplace),讀寫對稱
  [`MythLayer.cpp:97`](../../Campaign/MythLayer.cpp#L97)

**跨章重推導**

- DeriveFrom:治理快照換算注入,非有限/非正值早退;ratchet 只升不降
  [`MythLayer.cpp:129`](../../Campaign/MythLayer.cpp#L129)

- AdvanceChapter 邊界掛點:章節推進觸發重推導
  [`CampaignState.cpp:20`](../../Campaign/CampaignState.cpp#L20)

**持久化**

- FromJson:缺段降級清態、型別檢查拒絕壞段、isfinite 防 nan/inf 磚檔
  [`MythLayer.cpp:191`](../../Campaign/MythLayer.cpp#L191)

- myth_layer 段寫入存檔 root
  [`CampaignState.cpp:39`](../../Campaign/CampaignState.cpp#L39)

- 載入:tmp 驗證後交換 + 保留已註冊事件回呼
  [`CampaignState.cpp:112`](../../Campaign/CampaignState.cpp#L112)

**外圍**

- 無頭測試:I/O 矩陣全列 + 審查 regression 段([9b]favor 對稱、[9c]回呼再入)
  [`MythLayerTest.cpp:34`](../../Examples/MythLayerTest.cpp#L34)

- Campaign lib 源檔 + 測試 target + POTATO_TESTS 註冊
  [`CMakeLists.txt:422`](../../CMakeLists.txt#L422)
