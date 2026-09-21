---
title: 'D-4 神話入侵事件（鬼軍夜行/狐仙假訊——mid-battle 插入）'
type: 'feature'
created: '2026-09-21'
status: 'done'
baseline_commit: 'd30c7f3'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/epic-d-context.md'
  - '{project-root}/Gameplay/MythLog.h'
  - '{project-root}/Gameplay/ShrineField.h'
  - '{project-root}/Campaign/MythLayer.h'
  - '{project-root}/Campaign/ChapterLibrary.h'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Epic D 的神話層目前只有神社靜態互動——epics.md Story D.4
要指定戰鬥被神話事件打斷（鬼軍夜行/狐仙假訊），讓「戰場不只屬於人」：
滲透達標的章節在中場插入入侵，安撫成功治理加分、忽視滲透推進，
事件入 MythLog 具名三欄（何祠何靈何時），且有稀有性守衛
（意義改變者才用）。

**Approach:** 新增 `Gameplay::MythIncursion` field-driver（ShrineField
姊妹類）：呼叫端每拍餵區域滲透等級快照（Gameplay 不讀 Campaign），
達閾→一次性觸發入侵——鬼軍走 `BattleController::CreateSquad` 中場
生成小隊、假訊走 `QuantumFog` 綁定幻影 entity（無主雲不渲染，必須
綁真 squad）；解決窗口內 `Resolve(true)` 發治理加分回呼、逾時或
`Resolve(false)` 發滲透推進回呼；入侵發生時發 `IncursionEvent`
回呼（呼叫端接 `MythLog::Record` 具名三欄 + `SetTimeScale(0)` 暫停
提示）。章節側 `ChapterDef` 仿 `no_battle` 加 `incursion` 子物件旗標。

## Boundaries & Constraints

**Always:**
- 觸發條件由呼叫端注入：`Arm(seepageThreshold, kind)` + 每拍
  `Update(dt, battle, seepageLevel, region, spirit)`——field 永不吃
  `Campaign/` 型別（依賴方向不倒流）
- 稀有性守衛：每場最多一次入侵（one-shot latch）；未 Arm 或
  seepageLevel < threshold 永不觸發；觸發後不再重複
- 鬼軍夜行：`CreateSquad` 建 team=2（第三方）幻影小隊 +
  `BindFogSquad` 掛雲（中場建隊拿不到 doctrine，預設 Hold——
  鬼軍不靠 AI 走動，威懾性存在）
- 狐仙假訊：CreateSquad 小額鬼隊 + `AddEntity` 候選格指向
  虛構方位——情報層造假而非實體造假；時效到 or 解決後由
  `ApplyCasualties` 收隊（無 RemoveSquad API，消散=殲滅語義）
- 解決窗口：觸發起 `kResolveWindow` 秒內呼叫端可 `Resolve`；
  逾時自動以 ignored 結算（deterministic——Update 的 dt 累積）
- 三路回呼出口（全型別化、先變異後派出）：`IncursionEvent`
  （觸發，呼叫端接 MythLog::Record+暫停）、`OutcomeEvent`
  （pacified→呼叫端 `gov.Adjust*` 加分；ignored→呼叫端
  `MythLayer` 推滲透）、事件字串（呼叫端 `recorder.AddRecord`）
- `ChapterDef` 加 `incursion` 子物件：`{"enabled"/"seepage"/"kind"}`，
  比照 no_battle parseOpt 慣例（缺欄關閉、壞欄 warnings 不 throw）
- `MythLayer` 補唯讀 `SpiritOf(region)` getter（具名三欄缺靈名）
  ——D-3 HasSpirit 同款唯讀追加，不改語義
- 純邏輯無引擎依賴、全確定性；banned C 函式禁令
- 渲染層唯讀：`GetIncursion()` 回目前狀態供 UX 讀取

**Ask First:**
- 鬼軍是否要有實際戰力（members/士氣數值）——本實作給象徵性
  小隊（存在感>戰鬥力），數值平衡屬調參範疇
- 「忽視→滲透推進」的壓力量——本項發型別化 OutcomeEvent，
  換算注壓由呼叫端決定（不加 MythLayer 注壓 API）

**Never:**
- 不改 `BattleController`/`AssignDoctrine` 的 phase 守衛、
  不改 `QuantumFog`/`BattleSceneSync`（無主雲不渲染是既有語義）
- 不改 `MythLog`/`MythEvent` schema、不改 `potato.campaign/1`
  （incursion 屬 `potato.campaign_chapter/1` 章節定義）
- 不做暫停 UI/prompt 實作（呼叫端 `SetTimeScale(0)`+UX 層）、
  不做 sprite/視效（D-5 消費端）
- 不引入隨機/時間戳；不做 D-3 兌換連動

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 達閾觸發 | Arm(2,GhostLegion)+Update(level=Seep) | 鬼隊生成、IncursionEvent+字串回呼各一筆 | N/A |
| 未達閾 | Update(level< threshold) | 不觸發、無回呼 | N/A |
| 稀有性 | 已觸發再 Update 達閾 | 不重複觸發 | one-shot |
| 未 Arm | Update 達閾 | 不觸發 | N/A |
| 假訊注入 | Arm(kind=FoxRumor) 達閾 | 幻影 squad+fog entity 建立、可觀測 | N/A |
| 安撫解決 | 觸發後窗口內 Resolve(true) | OutcomeEvent(pacified)+入帳字串 | N/A |
| 忽視逾時 | 觸發後 dt 累積超窗口 | 自動 ignored 結算+OutcomeEvent | 逾時結算 |
| 主動忽視 | Resolve(false) | OutcomeEvent(ignored) | N/A |
| 未觸發 Resolve | 無 pending 時 Resolve | 回 false、零副作用 | 拒絕 |
| 重複 Resolve | 已結算再 Resolve | 回 false、無回呼 | 拒絕 |
| 鬼隊存在 | 觸發後查 battle.GetSquads | 新 squad 在列、team=2 | N/A |
| 具名三欄 | IncursionEvent 欄位 | shrine/spirit/when/detail 齊全 | N/A |
| ChapterDef 旗標 | incursion JSON 缺/壞/好 | 缺=關閉、壞=warnings、好=解析 | 降級容忍 |
| SpiritOf | MythLayer 綁定後查 | 回 spirit 名；未綁回空 | N/A |

</frozen-after-approval>

## Code Map

- `Campaign/MythLayer.h/.cpp` — 補 `SpiritOf(region)` 唯讀 getter
- `Campaign/ChapterLibrary.h/.cpp` — `IncursionDef` 子結構 +
  `incursion` 解析（仿 no_battle parseOpt）
- `Gameplay/MythIncursion.h/.cpp` — 新：`IncursionKind` enum、
  `Arm`/`Update`/`Resolve`/`GetIncursion` + 三路回呼
- `Examples/MythIncursionTest.cpp` — 新無頭測試（I/O 矩陣全列；
  接線比照 ShrineFieldTest）
- `CMakeLists.txt` — Gameplay lib 源檔 + 測試 target + `POTATO_TESTS`

## Tasks & Acceptance

**Execution:**
- [x] `Campaign/MythLayer` — `SpiritOf` getter
- [x] `Campaign/ChapterLibrary` — `IncursionDef` + 解析
- [x] `Gameplay/MythIncursion.h` — enum/struct/API 宣告
- [x] `Gameplay/MythIncursion.cpp` — 觸發/生成/窗口結算/回呼
- [x] `Examples/MythIncursionTest.cpp` — I/O 矩陣全列
- [x] `CMakeLists.txt` — 註冊

**Acceptance Criteria:**
- Given Arm(seepage=2)+區域達 Seep，when Update 驅動，then 入侵
  觸發一筆、鬼隊/假訊依 kind 生成、IncursionEvent 具名三欄齊
- Given 已觸發，when 窗口內 Resolve(true)，then pacified 結算、
  治理加分回呼發出；when 逾時未 Resolve，then ignored 結算、
  滲透推進回呼發出
- Given 任何狀態，when 已觸發過，then 不再第二次觸發
  （稀有性守衛）
- Given 章節定義 `"incursion": {"seepage": 2, "kind": "ghost_legion"}`，
  when 載入，then ChapterDef.incursion 解析正確；缺欄降級關閉

## Design Notes

依賴翻轉同 D-2/D-3：MythIncursion 住 Gameplay，滲透等級/
區域名/靈名每拍由呼叫端注入（讀 MythLayer 的活是 Campaign
側呼叫端做），輸出全走回呼——MythLog 記錄、治理加分、
滲透推進都由呼叫端接線，field 保持無頭可測。

「戰鬥短暫暫停提示」刻意不落 field:BattleController 無 pause
概念，暫停是 `SetTimeScale(0)` + UI prompt 的 UX 層事——
IncursionEvent 回呼即觸發點，呼叫端自行接暫停。

鬼軍 team=2（第三方）：不是敵軍增援也不是我軍——「戰場不只
屬於人」的字面實現。AssignDoctrine 鎖 Deployment 是故意的：
鬼軍中場現身就該無 doctrine 的 Hold（幽影不聽軍令），要動
靠呼叫端 Intervene。

## Verification

**Commands:**
- `cmake --build build --config Release --target MythIncursionTest` -- MSVC 編譯
- `cd build && ctest -C Release` -- 全綠（含 MythLayerTest/ShrineFieldTest 不回歸）
- MinGW 編譯 `MythIncursionTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中

## Spec Change Log

- **觸發**：三路審查 — (1) FoxRumor 假雲以 `team=1` 註冊，
  `QuantumFog` team 參數是觀測方語義 → 觀測扣錯陣營情報；
  (2) 忽視/逾時後幻影隊留存且隱形 → 永遠卡死勝利判定；
  (3) `onIncursion` 先於入帳字串派出 → 回呼再入 Resolve 時
  ledger 順序變「結算→觸發」;(4) 空靈名以「境靈」入 state →
  OutcomeEvent 回傳會被下游當真靈鑄幻影錢包；(5) `seepage: 2.5`
  被截斷靜默接受；(6) 越界門檻可 Arm 成永不觸發的啞彈；
  (7) 假雲偏移可能出界。**修訂**：觀測方改 0、Settle 統一收隊
  （假訊任何結局都消散——情報層造假不留實體；鬼軍安撫才散）、
  先變異後派出且入帳字串先於 IncursionEvent、state.spirit 存
  原始名（「境靈」只做顯示）、seepage 整數性驗證、Arm 越界
  門檻視為關閉、假雲位置夾進場界、dt≤0/NaN 不累積窗口、
  重 Arm 先消散舊鬼隊、Settle 後清指防 UAF。**KEEP**：回呼
  翻轉依賴、team=2 第三方、無 doctrine 的 Hold、一次性 latch、
  `incursion` 子物件 warnings 降級。

## Suggested Review Order

**觸發與生成**

- Update→Trigger——達閾一次性 latch、鬼軍 CreateSquad team=2、
  假訊 AddEntityCloud 觀測方=0 + BindFogSquad、假雲夾界
  [`MythIncursion.cpp:60`](../Gameplay/MythIncursion.cpp#L60)

- Arm 生命期——越界門檻關閉、重 Arm 先消散舊鬼隊
  [`MythIncursion.cpp:33`](../Gameplay/MythIncursion.cpp#L33)

**結算**

- Settle——鬼軍安撫才散/假訊必散、Reveal→ApplyCasualties、
  清指防 UAF、先 state 後回呼
  [`MythIncursion.cpp:133`](../Gameplay/MythIncursion.cpp#L133)

**章節定義**

- IncursionDef 解析——seepage 1-3 整數驗證、缺欄/壞型別
  warnings 降級不 throw
  [`ChapterLibrary.cpp:180`](../Campaign/ChapterLibrary.cpp#L180)

**測試**

- I/O 矩陣 + 觀測計費 + 雙結局收隊 + 逾時 + 邊界組
  [`MythIncursionTest.cpp`](../Examples/MythIncursionTest.cpp)
