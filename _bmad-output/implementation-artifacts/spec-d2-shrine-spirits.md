---
title: 'D-2 神社與精靈實體（BattleMap shrine 節點 + 佔領互動）'
type: 'feature'
created: '2026-09-21'
status: 'done'
baseline_commit: '409e046'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/Gameplay/GovernanceField.h'
  - '{project-root}/Gameplay/BattleMap.h'
  - '{project-root}/Campaign/MythLayer.h'
  - '{project-root}/Gameplay/BattleRecorder.h'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** D-1 給了滲透狀態機與 favor 表，但戰場上沒有任何東西寫它——
神話層還不是「可玩的第二張地圖」。epics.md Story D.2 要 BattleMap 可定義
神社節點，小隊進駐後玩家做安撫/挑釁/獻祭選擇，選擇入帳、神明 favor 改變、
視覺狀態供渲染層讀取（供品態追蹤民心——環境敘事）。

**Approach:** 照 `GovernanceField` 樣板新增 `Gameplay::ShrineField`——
有地圖知識的呼叫端每拍驅動的場域追蹤器。`MapInteractable` 擴充
`spirit`/`offering` 可選欄位（potato.battle_map/1 向後相容）；
小隊進駐 shrine 圈 → 節點轉 Occupied 待互動；`ApplyChoice` 收
`ShrineChoice{Appease,Provoke,Offer}` → 發 favor delta 回呼（呼叫端接
`MythLayer::AdjustFavor`，GodStance C.4 介面即 D-1 favor 表）+
選擇字串回呼（呼叫端接 `recorder.AddRecord(battle.GetElapsed(), …)`）；
每節點視覺態 `Idle/Occupied/Appeased/Provoked/Offered` + offering 字串
唯讀供渲染層。

## Boundaries & Constraints

**Always:**
- 只吃 `type=="shrine"` 的 `MapInteractable`；`spirit` 空 → 通用「境靈」
- 佔領條件同 GovernanceField：team 0、`!IsEliminated`、`!IsRouting`、
  `|sq.pos − node.pos| ≤ radius`；每節點首次進駐發一筆「發現神社」事件
- `ApplyChoice` 只在 Occupied 且未結算時有效；每節點結算一次
  （record-is-truth，不可反悔）
- favor delta 常數：Appease `+kAppeaseFavor`、Provoke `−kProvokeFavor`、
  Offer `+kOfferFavor`；一律經回呼發出——ShrineField 永不碰 Campaign 層
- 選擇事件字串經事件回呼發出（`"神社:<spirit> <選擇>"` 形式）——
  呼叫端負責入 Recorder，回放 hash 鏈自然覆蓋
- 純邏輯無引擎依賴、全確定性；banned C 函式禁令；JSON 用 `Potato::JsonValue`
- 依賴方向 `Gameplay ← Campaign` 不倒流——favor 寫入靠回呼翻轉依賴

**Ask First:**
- 選擇要不要順帶 `MythLayer::Feed` 滲透壓力（如挑釁=暴行類壓力）——
  本實作只發型別化回呼，要不要接由呼叫端決定
- `DuanqiaoPlayable` 的互動選項 UI（快捷鍵/選單）——本項只出
  headless 可消費的 pending 狀態與 `ApplyChoice` API

**Never:**
- 不改 `GovernanceEvent` 列舉、不改 `MythLog`/`MythEvent` schema、
  不改 `BattleController`（Emit 維持私有，事件走 ShrineField 自帶回呼）
- 不讓 `Gameplay/` 含入 `Campaign/` 標頭
- 不做神社 sprite/視效（F-1 素材、D-5 消費端）；渲染層只讀狀態
- 不引入隨機/時間戳

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 佔領觸發 | 我軍活隊進 shrine 圈 | 節點→Occupied、發現事件一筆 | N/A |
| 重複佔領 | 已 Occupied 再有隊進圈 | 不重發事件、狀態不變 | N/A |
| 敵隊不觸發 | team!=0 進圈 | 無事件、不 Occupied | N/A |
| 潰逃隊不觸發 | team 0 但 IsRouting 進圈 | 不 Occupied | N/A |
| 未佔領選擇 | Idle 節點 ApplyChoice | 回 false、無回呼 | 拒絕 |
| 重複選擇 | 已結算節點再 ApplyChoice | 回 false、無回呼 | 拒絕 |
| favor 回呼 | ApplyChoice(Appease/Provoke/Offer) | spirit+正負 delta 各一筆 | N/A |
| 選擇入帳 | 同上 | 事件字串一筆（含 spirit/選擇名） | N/A |
| 視覺態 | Occupied→ApplyChoice | Idle→Occupied→Appeased 等；offering 字串可讀 | N/A |
| 非 shrine | village/oil_slick 等 type | ShrineField 完全忽略 | N/A |
| 缺 spirit | shrine 節點無 spirit 欄 | 視為「境靈」，回呼照發 | N/A |
| schema 相容 | 舊 battle_map JSON（無 spirit/offering） | 載入成功、欄位為空 | 缺欄位容忍 |

</frozen-after-approval>

## Code Map

- `Gameplay/BattleMap.h/.cpp` — `MapInteractable` 加 `spirit`/`offering`
  字串欄（置末位保 aggregate 初始化相容）+ JSON 解析兩行
- `Gameplay/ShrineField.h/.cpp` — 新：`ShrineChoice` enum、`ShrineState`、
  `Bind`/`Update`/`ApplyChoice`/`IsPending`/`GetShrine` + 雙回呼
- `Examples/ShrineFieldTest.cpp` — 新無頭測試（風格同 GovernanceFieldTest）
- `CMakeLists.txt` — Gameplay lib 源檔 + 測試 target + `POTATO_TESTS`

## Tasks & Acceptance

**Execution:**
- [x] `BattleMap` — `MapInteractable` 擴 `spirit`/`offering` + 解析
- [x] `Gameplay/ShrineField.h` — `ShrineChoice`/`ShrineState` enum、
  `Bind`/`Update`/`ApplyChoice`/`IsPending`/`GetShrine`/
  `SetEventCallback`/`SetFavorCallback` 宣告
- [x] `Gameplay/ShrineField.cpp` — 佔領偵測 + 選擇結算 + 雙回呼
- [x] `Examples/ShrineFieldTest.cpp` — I/O 矩陣全列
- [x] `CMakeLists.txt` — 註冊

**Acceptance Criteria:**
- Given BattleMap 載入含 shrine 節點，when 我軍小隊進駐圈內，then
  節點轉 Occupied 且發現事件發一筆
- Given Occupied 節點，when `ApplyChoice(Provoke)`，then favor 回呼
  收到該 spirit 負 delta 且選擇事件字串發一筆
- Given 已結算節點，when 再 `ApplyChoice`，then 拒絕且無回呼
- Given 渲染層，when 讀 `GetShrine(i)`，then 狀態/offering/spirit 可見
  且無 mutator 外漏

## Design Notes

回呼翻轉依賴是本項的架構核心：ShrineField 住 Gameplay，但 favor 歸
Campaign 的 MythLayer——所以 ShrineField 只發「誰的 favor 變多少」，
接線由呼叫端（戰役殼）做 `myth.AdjustFavor(spirit, delta)`；
同理選擇入帳由呼叫端 `recorder.AddRecord(battle.GetElapsed(), msg)`，
回放 hash 鏈自動覆蓋，BattleController 一行都不用改。

每節點只結算一次——「拜過的神社不會再受理」，簡單且防止
蹲點刷 favor；獻祭是否需要消耗資源屬 D-3/平衡範疇，本項只記選擇。

## Verification

**Commands:**
- `cmake --build build --config Release --target ShrineFieldTest` -- MSVC 編譯
- `cd build && ctest -C Release` -- 全綠（含 GovernanceFieldTest 不回歸）
- MinGW 編譯 `ShrineFieldTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中

## Suggested Review Order

**節點定義與 schema**

- MapInteractable 末位擴 spirit/offering——聚合初始化相容的關鍵
  [`BattleMap.h:46`](../../Gameplay/BattleMap.h#L46)

- JSON 解析兩行——缺欄位容忍,舊圖免改
  [`BattleMap.cpp:112`](../../Gameplay/BattleMap.cpp#L112)

**場域核心**

- ShrineState/mapIndex——原圖索引定址是公開 API 的對帳錨點
  [`ShrineField.h:47`](../../Gameplay/ShrineField.h#L47)

- Update:佔領偵測掃描、事件收集後統一派出(防回呼再入懸空)
  [`ShrineField.cpp:57`](../../Gameplay/ShrineField.cpp#L57)

- ApplyChoice:先驗 enum(default 拒絕)→結算→再發回呼
  [`ShrineField.cpp:95`](../../Gameplay/ShrineField.cpp#L95)

**依賴翻轉**

- 雙回呼出口:favor delta 與事件字串——Gameplay 不碰 Campaign
  [`ShrineField.h:82`](../../Gameplay/ShrineField.h#L82)

**外圍**

- 無頭測試:I/O 矩陣全列含潰逃隊、OOB/非法 enum、MythLayer 接線
  [`ShrineFieldTest.cpp:34`](../../Examples/ShrineFieldTest.cpp#L34)

- Gameplay lib 源檔 + 測試 target + POTATO_TESTS 註冊
  [`CMakeLists.txt:410`](../../CMakeLists.txt#L410)
