---
title: 'E-2 談判與嚇阻決算——無戰路徑 playable 整合 + 首波優勢兌現'
type: 'feature'
created: '2026-09-22'
status: 'in-progress'
baseline_commit: '25907c3'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/Campaign/NoBattleResolver.h'
  - '{project-root}/Gameplay/HistorianReport.h'
  - '{project-root}/Gameplay/BattleResources.h'
  - '{project-root}/Examples/DuanqiaoPlayable.cpp'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** E-1 落地的 `NoBattleResolver`（CheckOptions 灰顯報表 +
Resolve 判定入帳 + disposition 記錄）是**純函式庫無消費端**——
`DuanqiaoPlayable` 從不呼叫，`enemyAdvantage` 旗標沒有任何兌現者。
epics Story E.2 要求：談判以民心換和平、嚇阻以軍威換退兵、
失敗進戰鬥且敵獲首波優勢——代價結構要被玩家實際經歷。

**Approach:** 兩個缺口分層補——
(1) `Gameplay::ApplyFirstWaveAdvantage` 無頭可測的戰前修正：
敵方 +CP/+intel、我軍小隊士氣減益，把 `enemyAdvantage` 變成
機械事實；
(2) `DuanqiaoPlayable` 接線：章節 `noBattle` 定義非空 → 部署階段
開局彈出 CheckOptions 報表讓玩家選路徑或開戰；Resolve Success
→ 跳過戰鬥走 E-4 和平語域結算；Failed/Backfired → 套首波優勢
後照常開戰。

## Boundaries & Constraints

**Always:**
- `ApplyFirstWaveAdvantage` 放 Gameplay 層、只吃 `BattleController&` +
  `BattleResources&` + team 編號——Gameplay 不 include Campaign
  （NoBattleVerdict 留在 Campaign；呼叫端翻譯成 bool/參數）
- 首波優勢效果**確定性**：敵隊 `AddCP(+2)`/`AddIntel(+2)`、
  我軍每小隊士氣 −0.10（夾到 ≥0.10 防即潰）——無亂數
- 成功路徑**不產生假戰鬥**——battle 停在部署態，不走
  BeginExecution；戰報走 `HistorianInput.peacePathZh`（E-4 已備）
- 失敗/中計的優勢套用要在 `BeginExecution` 前——部署階段修正
  才影響開局
- CheckOptions 報表灰顯可見（available=false 列原因），玩家
  可強行嘗試——Resolve 判 Failed 是設計內代價
- 「開戰」永遠是合法選項——無戰路徑是選擇不是強制
- 判定結果已由 Resolve 入帳（Record-is-Truth）；戰鬥側另經
  `BattleRecorder`/`SetEventCallback` 留「敵搶先機」事件

**Ask First:**
- 首波優勢的具體配方（+2CP/+2intel/−0.10 士氣是提案值）——
  要調強度或換形式（如敵方開場知我軍部署=撤 fog）需裁決
- 無戰 UI 呈現位置——提案是部署階段 modal；若要在章節選擇
  畫面就出示需裁決

**Never:**
- 不改 `NoBattleResolver` 公開介面（CheckOptions/Resolve/Gather）
- 不改 `potato.campaign/1` schema、不改 `potato.battle_map/1`
- 不做 E-3 顛覆的倒戈小隊生成（本故事只管選項判定與首波優勢；
  倒戈小隊是 E-3 範疇——Subversion Success 目前同 Negotiation
  跳戰鬥結算即可）
- 不接 Incursion/MythLayer——談判不注壓滲透
- 不讓和平結算跳過 frontsTaken/AdvanceChapter/SaveToFile——
  章節推進必須與戰鬥勝利同權
- 不新增 Campaign→Gameplay 反向依賴以外的接線（依賴方向
  PotatoEngine ← Gameplay ← Campaign ← playable 維持）

## Scope

**In:**
- `Gameplay/NoBattleAdvantage.h/.cpp` — `ApplyFirstWaveAdvantage`
  一次性戰前修正（敵資源+、我軍士氣−，含夾取與零小隊邊界）
- `Examples/DuanqiaoPlayable.cpp` — 部署階段無戰選項 modal、
  Resolve 接線、Success→和平結算路徑、失敗→優勢套用
- `Examples/NoBattleAdvantageTest.cpp` — 無頭測試
- `CMakeLists.txt` — 註冊源檔+測試+POTATO_TESTS
- `auto-dev-queue.yaml` — 補 E-2 條目（佇列原缺）

**Out:**
- E-3 倒戈小隊實體生成、顛覆中計的開場劣勢細節
- 多章節 noBattle 定義內容（assets 側新章節檔）
- 無戰結算的專屬 UI 美化

</frozen-after-approval>

## Code Map

- `Gameplay/NoBattleAdvantage.h/.cpp` — 新：首波優勢一次性套用
  （AddCP/AddIntel/Squad::AdjustMorale 夾取）
- `Campaign/NoBattleResolver.{h,cpp}` — 消費端（不改，E-1 已落地
  CheckOptions/Resolve/Gather）
- `Gameplay/HistorianReport.h` — `HistorianInput.peacePathZh`
  和平語域入口（不改，E-4 已備）
- `Gameplay/ChapterConventions.h` — `ClosingContext.peacePathZh`
  收場鉤（不改）
- `Examples/DuanqiaoPlayable.cpp` — pendingChapter→noBattle modal
  →Resolve→分流；和平結算重用 chronicler/AdvanceChapter 尾段
- `Examples/NoBattleAdvantageTest.cpp` — 優勢套用 I/O 矩陣
- `Examples/NoBattleResolverTest.cpp` — 既有判定測試（不回歸）

## Tasks & Acceptance

**Execution:**
- [ ] `Gameplay/NoBattleAdvantage.h/.cpp` — ApplyFirstWaveAdvantage
- [ ] `Examples/DuanqiaoPlayable.cpp` — modal + Resolve + 分流
- [ ] `Examples/NoBattleAdvantageTest.cpp` — I/O 矩陣全列
- [ ] `CMakeLists.txt` — 註冊 lib 源檔 + 測試 target + POTATO_TESTS
- [ ] `auto-dev-queue.yaml` — E-2 條目補登

**Acceptance Criteria:**
- Given 章節 `noBattle` 有啟用路徑，when 進入部署階段，then
  彈出 CheckOptions 報表（未達標灰顯含原因）+ 開戰按鈕
- Given 玩家選達標談判，when Resolve，then 民心消費入帳 +
  disposition=Negotiated + 跳過戰鬥進和平結算（peacePathZh=談判）
  + 章節推進存檔
- Given 玩家強行選未達標路徑，when Resolve，then verdict=Failed +
  enemyAdvantage 兌現（敵 CP/intel 增加、我軍士氣減）+ 戰鬥照常
- Given 顛覆帳本有未駁斥 planted，when Resolve，then Backfired +
  同樣敵獲首波優勢
- Given ApplyFirstWaveAdvantage 零我軍小隊，when 呼叫，then
  不崩潰且敵資源照常增加
- Given 我軍士氣 0.05，when 套用 −0.10，then 夾到 0.10 不歸零

## Design Notes

和平結算路徑：`battle.GetOutcome()` 恆 Ongoing（未執行）——
用 `peaceResult` 狀態觸發同一個戰後結算窗口，HistorianInput
帶 `peacePathZh` + outcome=Victory；名冊全員生還、recorder
無戰況可摘（E-4 測試已定調）。PostBattle.Settle/Gov.Accumulate
跳過（無戰鬥事件）；frontsTaken/AdvanceChapter/SaveToFile 保留。

首波優勢配方（提案值，Ask First 可調）：敵隊 +2 CP +2 intel
（先機=更多介入籌碼）；我軍小隊士氣 −0.10 夾 ≥0.10（敗談後
軍心浮動但不即潰）。全部確定性、無亂數、可回放。

顛覆 Success 目前等同和平結算（disposition=Defected）——倒戈
小隊生成屬 E-3；本故事 Subversion 成功照樣跳戰鬥結算即可，
AC 不區分。

## Verification

**Commands:**
- `cmake --build build --config Release --target NoBattleAdvantageTest` -- MSVC 編譯
- `cmake --build build --config Release --target DuanqiaoPlayable` -- 接線可編
- `cd build && ctest -C Release -R "NoBattle"` -- 判定器回歸 + 新測試
- MinGW 編譯 `NoBattleAdvantageTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中
