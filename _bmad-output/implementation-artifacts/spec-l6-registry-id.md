---
title: 'L-6 帳簿登錄編號（rootHash 派生短碼）'
type: 'feature'
created: '2026-09-21'
status: 'done'
context:
  - '{project-root}/docs/research/GBAA_BAAF_RESEARCH.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-l7-ledger-provenance.md'
---

## Intent

**Problem:** 已結算帳簿有完整雜湊鏈可自證清白，但玩家沒有可引用、可貼出的短識別碼——對齊 BAAF L3「登錄編號供揭露引用」需要一個由帳簿內容派生的確定性編號。

**Approach:** `LedgerChain::RegistryId()` 由末端 `RootHash` 派生 8 位十六進制短碼（高低 32 位折叠）；`ToJson` 寫出 `"registryId"` 可選欄位、載入時重算比對；史官查帳段附登錄字號。

## Boundaries & Constraints

**Always:**
- 登錄編號**純函數**於帳簿內容——同一帳簿恆產同一編號，不記隨機/時間
- `potato.ledger_chain/1` schema 不升版；`registryId` 為可選附加欄位
- 舊檔（無 `registryId`）載入正常；`FromJson` 對 registryId 不強制驗（Verify() 才是篡改防線）
- 空鏈也有確定性編號（由創世雜湊派生）
- banned C 函式禁令；不引第三方庫

**Ask First:**
- 若想把編號顯示做進 demo UI（本項只做戰報文字層）
- 若編號格式要改為可攜式校验碼（base32+check digit）而非 hex

**Never:**
- 不建中心化登錄簿資料結構（本項只產生編號，L-8 不管；無 registry 伺服器概念）
- 不改 Verify()/Canon()/hash 語義——編號是衍生唯讀視圖
- 不影響 prov/suspect 既有欄位

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 確定性 | 同一帳簿多次呼叫 | `RegistryId()` 恆相同 | N/A |
| 敏感於內容 | 任一 entry/prov 改動 | 登錄編號改變 | N/A |
| roundtrip | ToJson→FromJson | registryId 欄位寫出、回讀後重算一致 | N/A |
| 舊檔降級 | 無 registryId 的 /1 存檔 | 載入成功、RegistryId() 照常產生 | N/A |
| 空鏈 | 無分錄 | 產生固定編號（創世根派生） | N/A |
| 查帳段 | 有帳簿的戰報 | 查帳段含登錄字號 | N/A |

## Code Map

- `Gameplay/Ledger.h` / `Ledger.cpp` -- `RegistryId()` 與 ToJson/FromJson 改動點（a5f1f12 已入帳 L-7 版）
- `Gameplay/HistorianReport.cpp` -- 查帳段（L-3 段內,L-7 來源統計之後）附登錄字號
- `Examples/LedgerProvenanceTest.cpp` -- 測試風格範本（Check 巨集、篡改字串操作 helper）
- `CMakeLists.txt` -- 測試 target 段與 `POTATO_TESTS` 清單

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/Ledger.h` -- `std::string RegistryId() const` 宣告 -- API 定義
- [x] `Gameplay/Ledger.cpp` -- `RegistryId()` 實作（RootHash 高低 32 位 XOR → `%08X`）+ `"registryId"` 寫出/讀入 -- 編號與持久化
- [x] `Gameplay/HistorianReport.cpp` -- 查帳段附登錄字號（平衡/存疑兩分支皆帶） -- 玩家可見引用點
- [x] `Examples/LedgerRegistryTest.cpp` -- 無頭測試覆蓋 I/O 矩陣 -- 驗證
- [x] `CMakeLists.txt` -- 註冊 `LedgerRegistryTest` 進 `POTATO_TESTS` -- 測試入列

**Acceptance Criteria:**
- Given 任一帳簿,when 兩次取 `RegistryId()`,then 完全相同
- Given 帳簿任一 entry 改動（含 prov),when 重取編號,then 編號不同
- Given ToJson 含 `"registryId"` 的存檔,when FromJson,then 載入成功且 `RegistryId()` 與寫出值一致
- Given 無 `registryId` 的舊檔,when FromJson,then 載入成功且編號照常派生
- Given 含帳簿的戰報,when 組裝,then 查帳段含登錄字號

## Design Notes

編號格式：`XXXX-XXXX`(8 位大寫 hex，`RootHash()>>32 ^ RootHash()` 折叠）——夠短可貼、碰撞機率對單機帳簿可忽略；要更強的唯一性把 full 16 hex 輸出即可，格式函數集中一處。

史官體示例：「本章記帳 5 筆，借貸相符——戰陣 3、營務 1、失考 1，武功增 120。登錄字號 A1B2-C3D4。」

## Verification

**Commands:**
- `cmake --build build --config Release --target LedgerRegistryTest` -- 編譯通過(MSVC)
- `cd build && ctest -C Release` -- 全綠
- MinGW 編譯 `LedgerRegistryTest` target -- 通過
- banned 函式掃描 -- 無新增命中
