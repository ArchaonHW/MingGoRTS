---
title: 'E-1 無戰章節定義與門檻（談判/嚇阻/顛覆）'
type: 'feature'
created: '2026-09-21'
status: 'in-progress'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-l7-ledger-provenance.md'
---

## Intent

**Problem:** 「至聖者無戰」是 pillar 2，但目前所有章節都只能打——
無戰是結局評語（SubdueWithoutWar），不是戰前可選的路徑。
epics E.1/E.2/E.3 要求指定章節提供談判/嚇阻/顛覆三條無戰路徑，
各有門檻與代價結構，判定結果一律入帳（Record-is-Truth）。

**Approach:** `ChapterDef` 加 `no_battle` 定義塊（schema /1 可選欄位）；
新增 `Campaign/NoBattleResolver`——`CheckOptions()` 產生灰顯可見的
選項報表（門檻 vs 現值皆列出），`Resolve()` 執行判定：成功寫帳＋
記敵將處置，失敗/中計標 `enemyAdvantage` 供戰鬥層兌現首波優勢。
門檻輸入讀既有系統：帳本 Civil/Army 試算淨額（民心/軍威）、
GeneralDossier 判詞檔案（verified/planted 情報）。

## Boundaries & Constraints

**Always:**
- 三條路徑各有獨立門檻；`CheckOptions` 對已定義但未達標的路徑回
  `available=false` + 門檻/現值（灰顯可見，不隱藏——UI 可鎖可不鎖，
  強行嘗試由 `Resolve` 判定為失敗）
- 判定結果一律入 `LedgerChain`（成功、失敗、中計皆記）——Record-is-Truth
- `Resolve` 對章節未定義的路徑回 `NotOffered`，不寫帳、不進戰鬥；
  已定義但未達門檻的強行嘗試回 `Failed`，寫失敗分錄
- 失敗與中計皆設 `enemyAdvantage=true`（敵獲首波優勢的機械旗標，
  由戰鬥層消費）
- `potato.campaign_chapter/1` schema 不變——`no_battle` 為可選欄位，
  舊章節檔照常載入
- banned C 函式禁令；不引第三方庫

**Ask First:**
- 若要把無戰路徑接進 DuanqiaoPlayable 章節流程（本項只交付機械
  契約 + 旗標，播放層接線歸 E-4/後續）
- 若要讓 HistorianReport 產生無戰語域戰報（歸 E-4）

**Never:**
- 不改 `LedgerChain`/`WellFormed`/`Verify` 語義；無戰分錄走同一套
  複式記帳守衛（amount>0、借貸異戶）
- 不新增持久化層——判定輸入由呼叫端從既有系統匯入，resolver 無狀態
- `GeneralDisposition`/`EntrySource` 只新增不更名（JSON 名稱相容）

## Design

### 門檻與代價（調性常數，寫死於 spec）

| 路徑 | 門檻（available 條件） | 成功代價/效果 | 強行嘗試（未達標）/中計 |
|------|----------------|--------------|---------|
| 談判 Negotiation | `no_battle.negotiation.cost` >0 且帳本 Civil 淨額 ≥ cost | 民心消費 cost（分錄：借 Fate 貸 Civil）；敵將記 `Negotiated` | 民心小損 max(1,cost/4)：借 Supply 貸 Civil；敵首波優勢 |
| 嚇阻 Deterrence | `no_battle.deterrence.threshold` >0 且帳本 Army 淨額 ≥ threshold | 軍威不損（分錄：借 Army 貸 Martial，象徵 amount=1）；敵將記 `Intimidated` | 虛張被識破：借 Supply 貸 Army amount=1；敵首波優勢 |
| 顛覆 Subversion | `no_battle.subversion.intel` >0 且該敵將 verified 判詞 ≥ intel | 內應倒戈（分錄：借 Army 貸 Fate amount=1）；敵將記 `Defected` | 未達標強行→Failed（借 Supply 貸 Civil amount=1）；達標但帳本含未駁斥 planted → **中計** Backfired（同分錄，敵首波優勢） |

顛覆的特殊性：`CheckOptions` 只看 verified 計數（planted 未駁斥
不可知——這正是假情報的陷阱）；`Resolve` 時若 `plantedUnrefuted`
為真 → `Backfired`。已駁斥的 planted（verified && planted）不算
陷阱也不算有效情報。

### 分錄設計（WellFormed 約束：amount>0、借貸異戶）

所有無戰判定分錄走 `LedgerEntry` 複式記帳；`EntrySource` 新增
`Deterrence`、`Subversion`（談判沿用既有 `Negotiation`）。provenance
填 `tick=0`/`eventId=章節id:路徑`——可追溯判定出處。

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 舊章節檔 | 無 `no_battle` 欄位 | 載入成功，三選項 offered=false | 相容降級 |
| 定義齊全 | 三路徑皆 enabled | CheckOptions 回三筆，門檻/現值正確 | N/A |
| 灰顯可見 | 民心不足談判門檻 | negotiation: offered=true, available=false, current<threshold | 不隱藏 |
| 談判成功 | Civil≥cost | verdict=Success、spent=cost、帳增一筆 Fate←Civil、敵將 Negotiated | N/A |
| 談判失敗 | Civil<cost 強行 Resolve | verdict=Failed、失敗分錄、enemyAdvantage | 強行嘗試有代價 |
| 嚇阻成功 | Army≥threshold | Success、敵將 Intimidated、象徵分錄 | N/A |
| 嚇阻不足 | Army<threshold 強行 | verdict=Failed、enemyAdvantage | N/A |
| 顛覆成功 | verified≥intel、無未駁斥 planted | Success、敵將 Defected | N/A |
| 顛覆中計 | verified 達標但 plantedUnrefuted | verdict=Backfired、enemyAdvantage=true、中計分錄 | 假情報機械性代價 |
| 顛覆不足 | verified<intel 強行 | verdict=Failed、enemyAdvantage | N/A |
| 失敗旗標 | 未定義路徑 | verdict=NotOffered，不寫帳、無 enemyAdvantage | NotOffered 不罰 |
| 未定義章節 | no_battle 全 disabled | CheckOptions 空表、Resolve 任意路徑 → NotOffered | N/A |
| JSON 降級 | `no_battle` 型別錯誤 | 載入成功 + warnings 記錄、該路徑 off | 比照既有欄位慣例 |

## Code Map

- `Campaign/ChapterLibrary.h/.cpp` — `NoBattleOptionDef`/`NoBattleDef` +
  `ChapterDef::noBattle` + `"no_battle"` JSON 解析（warnings 慣例）
- `Gameplay/Ledger.h/.cpp` — `EntrySource::Deterrence`/`Subversion` +
  中英名稱表
- `Gameplay/CampaignLedger.h/.cpp` — `GeneralDisposition::Negotiated`/
  `Intimidated` + 名稱表
- `Campaign/NoBattleResolver.h/.cpp` — `NoBattlePath`/`NoBattleVerdict`/
  `NoBattleContext`/`NoBattleOptionInfo`/`NoBattleResult` +
  `CheckOptions()`/`Resolve()`
- `Examples/NoBattleResolverTest.cpp` — 無頭測試覆蓋 I/O 矩陣
- `CMakeLists.txt` — 註冊 `NoBattleResolverTest` 進 `POTATO_TESTS`

## Tasks & Acceptance

**Execution:**
- [ ] `Gameplay/Ledger.h/.cpp` — EntrySource 增兩值 + 名稱表 — 來源粒度
- [ ] `Gameplay/CampaignLedger.h/.cpp` — Disposition 增兩值 + 名稱表 — 處置粒度
- [ ] `Campaign/ChapterLibrary.h/.cpp` — no_battle 定義塊解析 — 內容側
- [ ] `Campaign/NoBattleResolver.h/.cpp` — CheckOptions + Resolve — 判定核心
- [ ] `Examples/NoBattleResolverTest.cpp` + CMakeLists 註冊 — 驗證

**Acceptance Criteria:**
- Given 章節定義含 no_battle 塊，when CheckOptions，then 三選項列出
  門檻與現值，未達標者 available=false 仍可見
- Given 民心達標選談判，when Resolve，then 民心分錄入帳、敵將記
  Negotiated、結果 success
- Given 軍威達標選嚇阻，when Resolve，then 敵將記 Intimidated、
  軍威淨額不減
- Given verified 判詞達標但帳本有未駁斥 planted，when Resolve
  顛覆，then Backfired + enemyAdvantage + 中計分錄入帳
- Given 已定義但未達標路徑被強行嘗試，when Resolve，then Failed、
  失敗分錄入帳、enemyAdvantage=true
- Given 章節未定義的路徑，when Resolve，then NotOffered，不寫帳、
  無 enemyAdvantage

## Design Notes

門檻輸入全走帳本試算淨額與判詞檔案——Record-is-Truth：玩家的
民心/軍威/情報不是 UI 狀態而是帳面資產。嚇阻「存在即判定」意指
軍威達標即開放、成功不消耗（兵不血刃以威服人）；談判相反，
成功即消費民心（以人和換罷兵）。顛覆的 planted 陷阱對齊 N-2
狡詐敵將假判詞機制——未驗證就動用情報，謊在文字不在數的
代價由機械結構兌現。

## Verification

**Commands:**
- `cmake --build build --config Release --target NoBattleResolverTest`（MSVC）
- `cd build && ctest -C Release` — 全綠（ChapterLibraryTest 等不迴歸）
- MinGW 編譯 `NoBattleResolverTest` — 通過
- banned 函式掃描 — 無新增命中
