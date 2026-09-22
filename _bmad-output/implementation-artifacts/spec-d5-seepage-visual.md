---
title: 'D-5 滲透視效消費端（UX-DR5——等級→呈現映射 + scrim 過渡）'
type: 'feature'
created: '2026-09-22'
status: 'done'
baseline_commit: '5a67158'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/epic-d-context.md'
  - '{project-root}/_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/DESIGN.md'
  - '{project-root}/Gameplay/BattleSceneSync.h'
  - '{project-root}/Examples/UITheme.h'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** MythLayer 滲透等級（D-1）目前無任何視覺出口——epics.md
Story D.5 / UX-DR5 要求等級改變時 HUD 做漸進式主題轉換：等級 1
局部異常（霧色偏移）、等級 2 神話音景先於視覺+物件微移、等級 3
全主題切換（token swap + scrim 過渡），且 motion-reduction 降為
crossfade、關鍵資訊雙編碼恆在、渲染層唯讀。

**Approach:** 新增 `Gameplay::SeepageStage` 純函式呈現層——吃整數
滲透序數（Gameplay 不讀 Campaign）輸出 `SeepageVisual` 指令包
（霧色偏移/微移幅度/音景等級/主題覆寫/scrim 曲線），加過渡狀態機
管 scrim alpha 時序；`BattleSceneSync` 補唯讀霧色 setter、
`UISettings` 補 motion-reduction 欄位；DuanqiaoPlayable 戰鬥迴圈
讀 `campaign.Myths().Level(region)` 接線消費。

## Boundaries & Constraints

**Always:**
- `SeepageStage` 只收 `int` 滲透序數——Gameplay 不 include
  Campaign（MythIncursion 同款型別隔離）
- 渲染層唯讀：sync/UI 只消費指令包，不回寫滲透狀態
- scrim 過渡用 ForegroundDrawList 全屏 rect（DuanqiaoPlayable
  themeFade 同款先例）——無 RTT/FBO
- motion-reduction 開啟時過渡降為短 crossfade（scrim 時長驟減、
  無漸變演出）
- 音景走既有 `AudioCues::SetSeepageLevel`（0-2 clamp）——等級 3
  不新增專屬音態，bed+layer 延續
- 等級變化才觸發過渡；同等級重餵不重播
- 關鍵資訊（士氣崩潰/倒數/CP）不經 scrim 遮擋語義——scrim 只改
  底色氛圍，不覆蓋 HUD 面板

**Ask First:**
- Manifest（等級 3）的目標主題——預設切 `InkChronicle`（史卷=神話
  層語域），要換別主題或做「同主題變體 token」需使用者裁決
- 若要在 ChapterDef 加正式 `region` 欄位（目前用章節地圖名當
  MythLayer 區域鍵的慣例）

**Never:**
- 不做 shader/sprite 特效（墨暈、湍流位移貼圖）——draw-list 上限
- 不改 `QuantumFog`/`MythLayer`/`UITheme` 內部狀態機
- 不改 `potato.campaign/1`、`potato.settings/1` 既有欄位語義
  （motion-reduction 走可選新欄，舊檔預設降級）
- 不接 `MythIncursion` 事件驅動視效（入侵事件的暫停提示屬 D-4 回呼
  消費端，本故事只管「等級→呈現」映射）
- 不阻塞戰鬥節拍——過渡不改 `SetTimeScale`

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| LEVEL0_QUIET | level=0 | SeepageVisual 全零（無偏移/無音景/無主題覆寫） | N/A |
| LEVEL1_ANOMALIES | level=1 | fogTint 偏移非零、shadowOffset 非零；audioLevel=1 | N/A |
| LEVEL2_SEEP | level=2 | jitterAmp>0、audioLevel=2（音景先於視覺：bed+layer） | N/A |
| LEVEL3_MANIFEST | level=3 | themeOverride 啟用、scrim 過渡排程、audioLevel=2 | N/A |
| TRANSITION_ONCE | level 0→2 連續餵 | 過渡只觸發一次，同等級重餵不重播 | N/A |
| MOTION_REDUCED | level→3 + reduced=true | scrim 時長縮為 crossfade 短檔、無長漸變 | N/A |
| LEVEL_SKIP | level 0→3 跳級 | 直接取最高檔呈現，不逐級播 | N/A |
| OOB_LEVEL | level<0 或 >3 | clamp 到 [0,3] 或視為 0，不 UB | clamp |
| NO_CALLBACK | 無 sync/audio 接線 | Update 仍可跑、指令包仍正確 | N/A |

</frozen-after-approval>

## Code Map

- `Gameplay/SeepageStage.h/.cpp` — 新：等級→SeepageVisual 映射 +
  過渡狀態機（scrim alpha 曲線、motion-reduction 分支）
- `Gameplay/BattleSceneSync.{h,cpp}` — 補唯讀 `SetSeepageTint`
  （比照 SetFogDrift 注入慣例；EnsureFogNode 色值加偏移）
- `Examples/UISettings.h` — `Data` 加 `motionReduction` 可選欄
  （F-3 hudDensity 同款相容做法）
- `Examples/DuanqiaoPlayable.cpp` — 接線：讀 `Myths().Level(地圖名)`
  → SeepageStage::Update → sync tint/audio level/theme swap+scrim
- `Examples/SeepageStageTest.cpp` — I/O 矩陣全列無頭測試
- `Audio/AudioCues.h` — 消費端（SetSeepageLevel，不改）
- `Campaign/MythLayer.h` — Level/Levels 唯讀出口（不改）

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/SeepageStage.h` — SeepageVisual 指令包 + API 宣告
- [x] `Gameplay/SeepageStage.cpp` — 等級映射 + 過渡 FSM + 曲線
- [x] `Gameplay/BattleSceneSync.{h,cpp}` — SetSeepageTint 唯讀注入
- [x] `Examples/UISettings.h` — motionReduction 欄（舊檔相容）
- [x] `Examples/DuanqiaoPlayable.cpp` — 戰鬥迴圈接線消費
- [x] `Examples/SeepageStageTest.cpp` — I/O 矩陣全列
- [x] `CMakeLists.txt` — 註冊 lib 源檔 + 測試 target + POTATO_TESTS

**Acceptance Criteria:**
- Given level 由 0 升至 1/2/3，when SeepageStage::Update 驅動，
  then 輸出對應 SeepageVisual 且過渡只觸發一次
- Given motion-reduction 開啟，when level→3，then scrim 時長為
  crossfade 短檔
- Given 渲染層接線，when sync.SetSeepageTint 注入，then fog 雲
  色偏移但 QuantumFog 狀態不變
- Given 舊 settings.json 無 motionReduction 欄，when Load，
  then 預設值降級不報錯

## Design Notes

「音景先於視覺」（等級 2）：SeepageVisual.audioLevel 在視覺指令
啟用前就位——呼叫端先送 SetSeepageLevel 再套視覺參數，語序
保證聽覺先抵達。

Manifest 目標主題預設 InkChronicle：四主題裡唯一以「史官/神話
語域」為 fiction 的皮——「世界層滑動」視覺上等於 HUD 換成另一
卷。若裁決改同主題變體，SeepageVisual.themeOverride 改為強度
參數即可，映射層不用動。

區域鍵慣例：DuanqiaoPlayable 用章節 `def.map` 去副檔名當
MythLayer 區域鍵（與 D-2 shrine region 同命名空間）；正式
`region` 欄位留給後續章節定義擴充。

## Verification

**Commands:**
- `cmake --build build --config Release --target SeepageStageTest` -- MSVC 編譯
- `cd build && ctest -C Release` -- 全綠（含 UIThemeTest 不回歸）
- MinGW 編譯 `SeepageStageTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中

## Review Log（三路審查後修訂）

- `SeepageVisual` 純量全補 NSDMI——L0 常態路徑 `lv==level` 不會
  走 `ForLevel`，未初始化時會把垃圾值送進 sync。
- **Producer 補線**：`"duanqiao"` 區域原無 Feed 來源——
  DuanqiaoPlayable 戰後結算段把 `GetGovernanceEvents()`
  （event→count 對映）逐次餵 `Myths().Feed`，區域由此建檔。
- `Update` 改餵 `dt * GetTimeScale()`——暫停中 scrim 與世界
  同凍（契約已寫進檔頭）。
- `svfx.audioLevel` 每幀消費取代事件回呼映射——單一映射源，
  讀檔後（FromJson 不發事件）音景自動補齊。
- 曲線快照：`scrimReduced` 在過渡觸發當下鎖定——途中切
  motion-reduction 不跳變；release 分支加零時長守衛。
- jitter 相位綁候選格子座標（非可見槽位索引）——候選被篩掉
  時倖存標記不跳相位；`Detach` 歸零全部 seepage 欄位。
- `PickFogCloud` 加 `shadowOffset` 參數——點選命中與渲染
  位移一致。
- **Manifest fallback**：玩家主題已是 InkChronicle 時 L3 改切
  WarMap——Manifest 須有可見信號。
- `motionReduction` 改用 `AsNumber` 解析——避極端值 `AsInt`
  轉換 UB；舊檔缺欄預設 0。
- **偏離追認**：scrim 用 `GetBackgroundDrawList`（非凍結區的
  ForegroundDrawList）——罩 3D 場景底色、不遮 HUD 面板，
  比字面規格更滿足「關鍵資訊雙編碼恆在」驗收條。

## Suggested Review Order

**映射與過渡**

- ForLevel——三階映射表（tint/shadow/jitter/audio/theme）
  [`SeepageStage.cpp:19`](../Gameplay/SeepageStage.cpp#L19)

- Update/ScrimAlpha——上升才觸發、跳級直取、曲線快照、
  dt 守衛、release 除零
  [`SeepageStage.cpp:46`](../Gameplay/SeepageStage.cpp#L46)

**消費端**

- Sync 修飾段——shadow 靜態偏移 + 相位綁格子的 jitter + tint
  [`BattleSceneSync.cpp:224`](../Gameplay/BattleSceneSync.cpp#L224)

- Detach 歸零 + SetSeepageFX
  [`BattleSceneSync.cpp:295`](../Gameplay/BattleSceneSync.cpp#L295)

**接線**

- 戰鬥迴圈——縮放 dt、指令包→sync/cues/theme、scrim 用
  BackgroundDrawList、Manifest fallback
  [`DuanqiaoPlayable.cpp:1180`](../Examples/DuanqiaoPlayable.cpp#L1180)

- Producer——戰後治理事件計數餵 MythLayer
  [`DuanqiaoPlayable.cpp:1952`](../Examples/DuanqiaoPlayable.cpp#L1952)

- Picker 偏移一致
  [`DuanqiaoPlayable.cpp:1099`](../Examples/DuanqiaoPlayable.cpp#L1099)

**測試**

- I/O 矩陣 + 過渡一次性 + reduced 曲線 + 跳級 + 越界 clamp +
  UISettings 相容
  [`SeepageStageTest.cpp`](../Examples/SeepageStageTest.cpp)
