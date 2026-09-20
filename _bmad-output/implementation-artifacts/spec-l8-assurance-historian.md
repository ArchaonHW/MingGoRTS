---
title: 'L-8 查證式史官——確信報告（seeded 抽驗 + 三態結論）'
type: 'feature'
created: '2026-09-21'
status: 'done'
baseline_commit: 'f43c351'
context:
  - '{project-root}/docs/research/GBAA_BAAF_RESEARCH.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-l7-ledger-provenance.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-l6-registry-id.md'
---

## Intent

**Problem:** 史官查帳段目前只回答「鏈有沒有斷、帳平不平衡」——被動陳述驗證結果。
對齊 ISSA 5000 確信流程（GBAA 研究可採項目 #3），史官應**主動執行查證方法**：
固定種子抽驗 N 筆分錄重算雜湊與鏈環連續性，再給出可重現的三態確信結論——
史官從記錄者升格為查證者。

**Approach:** `Gameplay/Ledger.h` 新增 `AssureLedger()` 查證函數（與報告文句解耦、
可獨立測試）：全鏈 `Verify()` + 健全性掃描 + seeded 抽驗複算，輸出
`LedgerAssurance`（verdict/抽驗數/斷點/疑帳數）。`HistorianReport` 查帳段在
登錄字號後附確信結論句。

## Boundaries & Constraints

**Always:**
- 抽驗可重現：同帳簿 + 同 seed → 同樣本、同結論（跨編譯器可重現——
  mt19937_64 為標準化引擎，取樣用 modulo 不用 `uniform_int_distribution`）
- `seed == 0` → 以 `RootHash()` 自引種：同帳簿恆抽同批分錄，帳變則樣本變
- 三態結論：
  - `Assured`（相符）：鏈完整 + 抽驗全過 + 無 malformed + 無標記疑帳
  - `Qualified`（存疑）：鏈完整但 `SoundnessViolation()` 命中或有 `MarkSuspect` 疑帳
  - `Adverse`（拒絕）：`Verify()` 斷鏈或抽驗樣本重算不符
- 抽驗檢查 = 重算 `HashEntry(storedPrev, entry)` 比對 stored hash
  **且** prevHash 鏈環連續（`c.prevHash == 前一筆 hash`）
- `potato.ledger_chain/1` schema 不變；本項不新增持久化欄位
- banned C 函式禁令；不引第三方庫

**Ask First:**
- 若要把確信結論接進 CampaignState 存檔或回放驗證（本項只做戰報文字層）
- 若結論文句要改動既有「借貸相符／墨跡未乾／登錄字號」句式（本項只追加）

**Never:**
- 不改 `Verify()`/`Canon()`/`HashEntry`/`RegistryId` 語義
- 不引入非確定性來源（random_device、時間戳）進抽驗路徑
- 不實作中心化登錄簿或外部查證者角色——方法論內建於史官

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 確定性 | 同帳簿同 seed 兩次 `AssureLedger` | 樣本集與結論完全相同 | N/A |
| 自引種 | `seed==0` | 以 RootHash 為種；同帳簿恆同樣本 | N/A |
| 相符 | 乾淨帳簿 | verdict=Assured，抽驗全過 | N/A |
| 存疑（偽帳） | `InjectForgery` malformed 分錄 | verdict=Qualified（鏈合法但借貸不成立） | 揭露不入 Adverse |
| 存疑（疑帳） | `MarkSuspect` 標記 | verdict=Qualified | L-4 整合 |
| 拒絕 | 剝 prov/改 hash 斷鏈 | verdict=Adverse | 篡改被揭露 |
| 抽驗全覆蓋 | `sampleN >= size` | 每筆都被抽中（全覆蓋 = 全驗） | N/A |
| 小帳簿 | `size < sampleN` | 抽驗數 = size | 不越界 |
| 戰報文句 | 三態各一 | 查帳段附對應結論句 | N/A |

## Code Map

- `Gameplay/Ledger.h` / `Ledger.cpp` -- `AssuranceVerdict` / `LedgerAssurance` / `AssureLedger()`
- `Gameplay/HistorianReport.h` -- `HistorianInput` 加 `assuranceSeed`/`assuranceSample`
- `Gameplay/HistorianReport.cpp` -- 查帳段登錄字號後附確信結論句
- `Examples/LedgerAssuranceTest.cpp` -- 新無頭測試（風格同 LedgerProvenanceTest）
- `CMakeLists.txt` -- 測試 target 段與 `POTATO_TESTS` 清單

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/Ledger.h` -- `AssuranceVerdict` enum + `LedgerAssurance` struct + `AssureLedger()` 宣告 -- 查證 API
- [x] `Gameplay/Ledger.cpp` -- `AssureLedger()` 實作：驗鏈 + 健全掃描 + seeded Fisher-Yates 部分洗牌取樣 + 樣本重算/鏈環檢查 + 三態判定 -- 方法論落地
- [x] `Gameplay/HistorianReport.h/.cpp` -- `HistorianInput` 加欄位；查帳段附三態結論句 -- 玩家可見
- [x] `Examples/LedgerAssuranceTest.cpp` -- 無頭測試覆蓋 I/O 矩陣 -- 驗證
- [x] `CMakeLists.txt` -- 註冊 `LedgerAssuranceTest` 進 `POTATO_TESTS` -- 測試入列

**Acceptance Criteria:**
- Given 任一帳簿與固定 seed,when 兩次 `AssureLedger`,then 樣本與結論完全相同
- Given 乾淨帳簿,when 查證,then verdict=Assured 且抽驗全過
- Given 含 malformed 分錄或標記疑帳的帳簿,when 查證,then verdict=Qualified
- Given 斷鏈帳簿,when 查證,then verdict=Adverse
- Given 含帳簿的戰報（三態各一）,when 組裝,then 查帳段含對應結論句

## Design Notes

抽驗取樣用 Fisher-Yates 部分洗牌：索引陣列 `0..n-1`，前 k 位各與
`i + rng() % (n - i)` 交換——保證不重複且跨編譯器可重現
（不依賴 `std::uniform_int_distribution`，其結果隨標準庫實作而異）。

三態對應 ISSA 5000 確信結論的光譜：Assured=無保留、Qualified=保留、
Adverse=否定/拒絕。偽帳（鏈合法但 malformed）歸 Qualified 而非 Adverse——
鏈條完整性是更高的防線，會計不成立是帳面瑕疵。

史官體示例：
- 相符：「抽驗 3 筆皆符，鏈環相續——史官具結：帳目相符。」
- 存疑：「抽驗 3 筆皆符，然第 3 筆借貸不成立——史官存疑，不予具結。」
- 拒絕：「鏈斷帳毀——史官拒絕具結，本章帳目不予採信。」

## Verification

**Commands:**
- `cmake --build build --config Release --target LedgerAssuranceTest` -- 編譯通過(MSVC)
- `cd build && ctest -C Release` -- 全綠(含既有 Ledger*Test 不迴歸)
- MinGW 編譯 `LedgerAssuranceTest` target -- 通過
- banned 函式掃描 -- 無新增命中
