---
title: 'E-4 無戰章節敘事與結算——和平語域戰報 + 章尾鉤 + 結局軌跡'
type: 'feature'
created: '2026-09-21'
status: 'done'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/spec-e1-nobattle-chapters.md'
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
---

## Intent

**Problem:** E-1 落地了無戰門檻與入帳，但「不戰的章節」在敘事層
留白——HistorianReport 只有戰鬥語域、ClosingHook 只有勝負兩態、
結局階梯不認無戰軌跡。epics E.4：無戰章節同樣被書寫。

**Approach:** HistorianInput 加和平語域欄位（呼叫端傳中文路徑名與
敵將名——Gameplay 層不回依賴 Campaign 的 NoBattlePath enum）；
ChapterConventions 收場鉤與結局輸入各加無戰維度。Gameplay 層
純文字組裝，headless 可全驗。

## Boundaries & Constraints

**Always:**
- 無戰戰報走同一個 `ComposeHistorianReport`——`peacePathZh` 非空
  即切和平語域；查帳段與省略計數行不改（無戰也有帳要查）
- 和平語域必帶敵將去向句（「不入陣亡錄」——收服者不列陣亡）
- 結局判定：`EndingInput.peaceChapters` 計入收服側（不戰而勝是
  收服的最高形式）；`ComposeEnding` 從帳本處置記錄數無戰章節
  （Negotiated/Intimidated/Defected），Mandate 聲部附「凡 N 章
  不戰而屈」句
- 失敗/中計不產和平語域戰報（那些章節仍進戰鬥）——本項只處理
  成功路徑的書寫
- banned C 函式禁令；不引第三方庫

**Ask First:**
- 若要給無戰章節配專屬結局聲部（第五聲部）——本項計入既有
  Mandate/無字階梯
- 若要把和平語域接進 DuanqiaoPlayable 章節流程（本項交付
  組裝 API；播放層接線另議）

**Never:**
- 不改戰鬥語域任何既有句式（和平語域為附加分支）
- 不在 Gameplay 層引入 Campaign 型別（NoBattlePath 留在
  Campaign——傳中文名/布林進來）

## Design

**和平語域戰報**（`peacePathZh` 非空觸發）：
```
史官曰：<章名>之局，兵不血刃。
<路徑句>：
  談判 → 遣使入帳，以民心曉之，「敵將」罷兵言和——議和而去，不入陣亡錄。
  嚇阻 → 陳師疆埸，軍威所懾，「敵將」未交一矢引兵而退——懾服而去，不入陣亡錄。
  顛覆 → 內應發於敵帳，「敵將」眾叛親離——變節投誠，不入陣亡錄。
查帳段（與戰鬥語域同）
本報告省略 N 項。
```
戰歷秒數行改寫「不戰而定」；戰況摘錄與名冊句不現（無戰可摘）。

**章尾鉤**：`ClosingContext.peacePathZh` 非空 → 主語改無戰體
（談判「談笑罷兵」/嚇阻「不戰而退敵」/顛覆「敵營自潰」），
尾綴「——欲知…且聽下回分解」不變。

**結局軌跡**：`EndingInput.peaceChapters` 加進 subdued 等效值
（武功顯判定與分數皆用）；`ComposeEnding` 數帳本無戰處置
（Negotiated/Intimidated/Defected）→ Mandate 聲部在主文後、
引用前附「凡 N 章不戰而屈，仁者之師也。」

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 和平語域 | peacePathZh=談判 + ledger | 文含「兵不血刃」「不入陣亡錄」、查帳段在場 | N/A |
| 三路徑 | 各 pathZh | 對應路徑句各異 | N/A |
| 戰鬥語域 | peacePathZh 空 | 既有句式逐字不變 | 迴歸 |
| 無敵將名 | peaceGeneral 空 | 去向句用「敵將」泛稱 | 降級 |
| 章尾鉤 | peacePathZh 各值 | 無戰體主語 + 下回分解尾 | N/A |
| 結局軌跡 | peaceChapters=2, 治績不足 | subdued 等效+2 → 可翻 Mandate | N/A |
| 結局文句 | 帳本含無戰處置 + Mandate | 附「凡 N 章不戰而屈」 | N/A |
| 無戰處置零 | Mandate 但無無戰記錄 | 不附該句 | N/A |

## Code Map

- `Gameplay/HistorianReport.h/.cpp` — `HistorianInput` 加
  `peacePathZh`/`peaceGeneral`；和平語域組裝分支
- `Gameplay/ChapterConventions.h/.cpp` — `ClosingContext.peacePathZh`、
  `EndingInput.peaceChapters`、ResolveEnding 收服等效、
  ComposeEnding Mandate 附句
- `Examples/NoBattleNarrativeTest.cpp` — 無頭測試
- `CMakeLists.txt` — 註冊

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/HistorianReport.h/.cpp` — 和平語域欄位與組裝分支
- [x] `Gameplay/ChapterConventions.h/.cpp` — 收場鉤無戰體 + 結局軌跡
- [x] `Examples/NoBattleNarrativeTest.cpp` + CMakeLists 註冊

**Acceptance Criteria:**
- Given 無戰成功的章節（三語域各一），when 組裝戰報，then
  和平語域文含路徑句與「不入陣亡錄」，查帳段與省略計數照常在場
- Given peacePathZh 非空的 ClosingContext，when ClosingHook，then
  產無戰體鉤句
- Given peaceChapters>0 且治績/武功臨界，when ResolveEnding，then
  無戰章節計入收服側影響聲部
- Given 帳本含無戰處置且聲部 Mandate，when ComposeEnding，then
  附「凡 N 章不戰而屈」句

## Verification

- `cmake --build build --config Release --target NoBattleNarrativeTest`（MSVC）— 通過
- `ctest -C Release -R "NoBattle|ChapterConventions|Historian|Ledger|Campaign"` — MSVC **12/12 全綠**
- MinGW 編譯 + `ctest -R NoBattle` — **NoBattleNarrativeTest 25/25 PASS**，NoBattleResolverTest 不迴歸
- banned 函式掃描 — 無新增命中
