---
title: 'L-7 LedgerChain provenance 欄位（產生軌跡）'
type: 'feature'
created: '2026-09-21'
status: 'done'
baseline_commit: '319c0c5daeb7ff7ba8f7057f3392f03ce624c3a7'
context:
  - '{project-root}/docs/research/GBAA_BAAF_RESEARCH.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-l-layer-ledger-accounting.md'
---

## Intent

**Problem:** 帳簿分錄只記「借貸多少、哪一章」,無法回答「這筆帳由哪個系統、因哪個事件產生」——對齊 BAAF L1「產生軌跡」前，審計只能看金額不能看來源。

**Approach:** `LedgerEntry` 增加 `Provenance`（來源系統 enum + tick + eventId）,canonical 字串有條件納入 prov 使 hash 保護產生軌跡；JSON 附加可選 `"prov"` 欄位維持 `potato.ledger_chain/1` 降級；史官查帳段附來源統計。

## Boundaries & Constraints

**Always:**
- `potato.ledger_chain/1` schema 不升版——`prov` 為每筆分錄的**可選**附加欄位
- provenance 為非預設值時必須納入 entry hash（事後竄改/補登 prov → 斷鏈）
- 舊檔（無 `prov` 欄位）載入後 `Verify()` 必須仍通過——hash 向後相容
- 三分錄模板各自填預設來源：BattleVictory→Battle、Recruitment→Refit、Negotiation→Negotiation
- `InjectForgery` 通道照常收帶 prov 的分錄（偽帳可偽造來源，偵測責任不變）
- banned C 函式禁令；JSON 用 `Serialization/JsonParser.h`；不引第三方庫

**Ask First:**
- 若要順便把 `LedgerChain` 接線進 `CampaignState` 存檔（本項不做，留給後續整合項）
- 若查帳段史官體措辭需大幅更動既有句式

**Never:**
- 不改 `external/`、`build/`；不改 `MarkSuspect`/`suspect` 持久化行為
- 不對 prov 做「必填」強制——Unknown 來源是合法值（舊檔/手動分錄）
- 不在本項實作登錄編號（L-6）或確信報告（L-8)

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 模板帶源 | `LedgerEntry::BattleVictory(120,1)` | `prov.source==Battle`,tick/eventId 預設 | N/A |
| prov 入 hash | 同 entry 不同 prov | 兩者 `HashEntry` 結果不同 | N/A |
| 剝除 prov | JSON 中刪除某筆 `prov` 欄位 | `FromJson` 載入、`Verify()` 回該筆索引（斷鏈） | 篡改被揭露 |
| 舊檔降級 | 無 `prov` 的 /1 存檔 | 載入成功、prov=Unknown/0/""、`Verify()` 通過 | N/A |
| roundtrip | 帶 prov 分錄 ToJson→FromJson | 三欄位全保留、Verify 通過 | N/A |
| 偽造 prov | 對已上鏈舊分錄補 prov | `Verify()` 斷於該筆 | 篡改被揭露 |
| 查帳統計 | 帳簿含多來源分錄 | 查帳段附各來源筆數（Unknown 也計） | N/A |

## Code Map

- `Gameplay/Ledger.h` -- `LedgerEntry`/`LedgerChain` 定義；新增 `EntrySource` enum + `Provenance` struct
- `Gameplay/Ledger.cpp` -- `Canon()`、`HashEntry`、三模板、`ToJson`/`FromJson` 改動點
- `Gameplay/HistorianReport.cpp` -- L-3 查帳段（約 193–227 行）平衡分支附來源統計
- `Examples/LedgerChainTest.cpp` / `LedgerAuditTest.cpp` / `LedgerForgeTest.cpp` -- 既有測試模式（Check 巨集、無頭）
- `CMakeLists.txt` -- 測試 target 段落（約 736–753 行）與 `POTATO_TESTS` 清單（約 974–977 行）

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/Ledger.h` -- 加 `EntrySource` enum（Unknown/Battle/Refit/Negotiation/Governance/Myth/System/Count）+ `SourceNameZh` + `Provenance` struct；`LedgerEntry` 加 `Provenance prov` 成員 -- 產生軌跡型別定義
- [x] `Gameplay/Ledger.cpp` -- `Canon()` 有條件附加 prov 段；三模板填來源；`ToJson`/`FromJson` 讀寫 `"prov"` 可選欄位 -- hash/持久化落地
- [x] `Gameplay/HistorianReport.cpp` -- 平衡分支在「借貸相符」句附各來源筆數 -- 查帳段輸出來源統計
- [x] `Examples/LedgerProvenanceTest.cpp` -- 新無頭測試覆蓋 I/O 矩陣全列 -- 驗證
- [x] `CMakeLists.txt` -- 註冊 `LedgerProvenanceTest` 進 `POTATO_TESTS` -- 測試入列

**Acceptance Criteria:**
- Given 三分錄模板各建一筆,when 檢查 `prov.source`,then 分別為 Battle/Refit/Negotiation
- Given 內容相同但 prov 不同的兩分錄,when 各自 `HashEntry`,then 雜湊不同
- Given 無 `prov` 欄位的 /1 舊存檔,when `FromJson`+`Verify()`,then 載入成功且鏈完整
- Given 帶 prov 分錄上鏈後從 JSON 剝除 `prov`,when `Verify()`,then 回報該筆斷鏈索引
- Given 含多來源分錄的帳簿,when 組裝史官戰報,then 查帳段含各來源筆數統計

## Design Notes

`Canon()` 向後相容是關鍵：**prov 全為預設值（Unknown/0/""）時維持舊格式**——

```
prov 為預設:  "Martial|Supply|120|1|斷橋之捷"          (舊格式)
prov 非預設:  "Martial|Supply|120|1|斷橋之捷|Battle|42|ev-xyz"
```

效果:① 舊存檔 hash 不變、`Verify()` 照過;② 新分錄的 prov 受 hash 保護;③ 對舊分錄事後補 prov → canon 變 → 斷鏈(正是「不可事後竄改」)。

`SourceNameZh` 供史官體:Battle→戰陣、Refit→營務、Negotiation→談判、Governance→治理、Myth→神異、System→紀要、Unknown→失考。統計句示例:「本章記帳 5 筆,借貸相符——戰陣 3、營務 1、失考 1,武功增 120。」

## Verification

**Commands:**
- `cmake --build build --config Release --target LedgerProvenanceTest` -- 編譯通過(MSVC)
- `cd build && ctest -C Release` -- 全綠(含既有 Ledger*Test 不迴歸)
- MinGW 編譯 `LedgerProvenanceTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中
