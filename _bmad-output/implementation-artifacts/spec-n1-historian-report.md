---
title: 'N-1 模組化史官戰報 + 省略計數'
type: 'feature'
created: '2026-09-18'
status: 'in-progress'
route: 'dispatch'
baseline_commit: '546b77f'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: N-1)
  - _bmad-output/narrative-design.md (the ledger is a lossy map)
  - Gameplay/BattleRecorder.h
  - Gameplay/Roster.h
  - Examples/DuanqiaoPlayable.cpp (現行手寫戰報段)
---

# N-1 模組化史官戰報 + 省略計數

## Intent

帳本是失真地圖——史官戰報從「手寫整段散文」改為「事件流 → 片段組裝」，
且每份報告自帶審計欄位「本報告省略 N 項」。記錄（BattleRecorder）與
書寫（HistorianReport）是同一機制的兩端：回放是完整帳，戰報是節選帳，
省略計數讓玩家永遠知道帳目不全。

## Acceptance

- [ ] 戰報由片段/模板組裝產出，headless 可測（不再於 demo 內手寫字串）
- [ ] 每份報告輸出省略計數欄位（結構化 + 文末「本報告省略 N 項」）
- [ ] DuanqiaoPlayable 戰後結算改吃新組裝器；既有 potato.battle_replay/1
      回放格式不破壞
- [ ] MSVC + MinGW 建置過、新測試進 POTATO_TESTS、全 ctest 綠

## Design

### 新檔 `Gameplay/HistorianReport.h/.cpp`

```cpp
struct HistorianInput {
    std::string battleName;            // 「斷橋之役」
    BattleOutcome outcome;
    float elapsedSec;
    const BattleRecorder* recorder;    // nullable
    const Roster* roster;              // nullable
    int playerTeam = 0;
    int enemyTeam = 1;
};

struct HistorianReport {
    std::string text;          // 組裝全文（史官體）
    int narratedCount = 0;     // 被書寫入報的事件數
    int omittedCount = 0;      // 記錄了但未書寫的事件數
    int routCount = 0;         // 潰逃起數
    int playerLosses = 0;      // 我軍殲滅隊數
    int enemyLosses = 0;       // 敵軍殲滅隊數
    int revealCount = 0;       // 情報揭露次數
    int interventionCount = 0; // CP 強令次數
};

HistorianReport ComposeHistorianReport(const HistorianInput& in);
```

### 事件分類（LedgerClass）

對 `BattleRecorder::Record::event` 做子字串分類——

| 類別 | 判定子字串 | 入報方式 |
|---|---|---|
| Rout | `潰逃` / `is routing` | 計入「潰逃 N 起」 |
| Elimination | `ELIMINATED` / `slain` | 比對 roster.squadName 判隊別 → 斬敵/我軍覆 |
| Reveal | `目擊` / `揭露` / `探測` | 計入「斥候揭敵 N 處」 |
| Intervention | `CP intervention` | 計入「強令介入 N 次」 |
| Phase/Deploy/Other | `===` / `deployed` / `doctrine assigned` / 其餘 | **省略** |

省略計數 = records.size() − narratedCount（clamp ≥0）。
階段標記與部署噪音屬「記錄了但不入史」——正是省略語義。

### 報告結構（片段序）

1. `史官曰：<battleName>，` + 結局句（Victory/Defeat/Draw 各一模板）
2. `戰歷 %.0f 秒。`
3. 戰況摘錄：非零計數項以頓號串接（`強令介入 N 次`、`斥候揭敵 N 處`、
   `潰逃 N 起`、`斬敵 N 隊`、`我軍覆 N 隊`）
4. 名冊句：殉國（含遺物）→ 得全 → 斬敵（沿用現行用語）
5. `本報告省略 N 項。`

### 測試 `Examples/HistorianReportTest.cpp`

- 空記錄：報告仍成立、omitted=0、含「本報告省略 0 項」
- 混合事件流：分類計數正確、omitted = total − narrated
- ELIMINATED 隊別歸屬：roster 比對正確分桶
- Victory/Defeat/Draw 結局句各異
- 無 recorder（nullptr）：omitted=0 不崩潰

### Demo 接線

`DuanqiaoPlayable` 1220–1253 手寫段 → `ComposeHistorianReport`，
`chronicler = report.text`（逐字敲出機制不變）。

## Out of scope

- 判詞（N-2）、對手反制（N-3）— 依賴各自佇列項
- 省略數隨墮落路線暴漲 — E-7 戰役層之後
- 戰報多語系 — string table 是 shell 層里程碑
