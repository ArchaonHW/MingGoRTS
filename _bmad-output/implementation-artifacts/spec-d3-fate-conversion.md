---
title: 'D-3 天命貨幣與轉換（favor → 民心/秩序兌換 + 灰顯選項）'
type: 'feature'
created: '2026-09-21'
status: 'done'
baseline_commit: '8992ca3'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/epic-d-context.md'
  - '{project-root}/Campaign/MythLayer.h'
  - '{project-root}/Campaign/Governance.h'
  - '{project-root}/Campaign/NoBattleResolver.h'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** D-1/D-2 讓 favor 能累積（ShrineField→MythLayer::AdjustFavor），
但 favor 只進不出——epics.md Story D.3 要它變成可消費的天命貨幣：
兌換民心/秩序讓神話行為機械性放大治理軸，且 favor 不足時選項
「灰顯可見不隱藏」（可見的門檻是設計本體）。

**Approach:** 新增 `Campaign::FateExchange`——持有 `MythLayer&` +
`Governance&` 的純邏輯兌換器（同 `NoBattleResolver` 的 CheckOptions
灰顯慣例）：`Options(spirit)` 回傳兩個固定價目（民心/秩序各一筆，
`{target, cost, gain, affordable, battleUsesLeft, reason}`）；
`Convert(spirit, target, inBattle)` 驗證餘額與戰中次數後扣 favor、
加治理軸、發型別化事件回呼（呼叫端接 Recorder/LedgerChain 入帳）。
Governance 補兩個夾取 setter（既有 `Accumulate` 是事件固定 delta，
兌換需要任意值寫入）。

## Boundaries & Constraints

**Always:**
- 貨幣 = `MythLayer::favor`（逐神 0–100），**不**用 `LedgerAccount::Fate`
  （那是 NoBattleResolver 的記帳對方科目，不相連）
- 價目固定常數：每項 `kFateCost` favor 換 `kFateGain` 治理點；
  戰中每場限 `kMaxBattleUses` 次（章節邊界 `inBattle=false` 不限次）
- `Convert` 失敗（favor 不足/戰中額度用盡/未綁定）→ 回 false、
  零副作用、不發回呼；成功才扣 favor→加軸→發事件（先變異後派出，
  同 MythLayer 回呼再入安全慣例）
- `Options` 永遠列出全部價目——不可兌換的項 `affordable=false`
  + `reason` 中文原因字串（照 NoBattleOptionInfo 慣例）
- 事件回呼型別化：`FateEvent{spirit, target, cost, gain, inBattle}`；
  呼叫端決定去向（戰中 `recorder.AddRecord`、章節邊界
  `LedgerChain::Append` 用現成 `EntrySource::Myth`）
- Governance setter 夾 [0,100]；純邏輯無引擎依賴、全確定性
- banned C 函式禁令；依賴方向不倒流（住 Campaign 可用 MythLayer/
  Governance 同層型別）

**Ask First:**
- 兌換要不要同時寫滲透壓力回饋（如大額兌換引神明不悅）——
  本實作只發 FateEvent，要不要接 `MythLayer::Feed` 由呼叫端決定
- `DuanqiaoPlayable` 兌換選單 UI——本項只出 headless 可消費的
  Options/Convert API

**Never:**
- 不改 `MythLayer` 公開介面（favor 表語義不變）、不改
  `Governance::Accumulate` 事件表、不改 `potato.campaign/1` schema
  （favor 已持久化；戰中次數是執行期計數不入檔）
- 不接 `LedgerAccount::Fate` 複式帳戶、不改 NoBattleResolver
- 不做戰中 SageCommand 軸寫入——兌換一律記 Campaign::Governance
  （帳本真相層）；戰中 UI 同步屬切片整合範疇
- 不引入隨機/時間戳；不做 sprite/視效

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 兌換成功 | favor≥cost，Convert(spirit, Support, false) | favor−cost、民心+gain、事件一筆 | N/A |
| favor 不足 | favor<cost | Convert 回 false、軸/favor 不變、無回呼；Options 該項 affordable=false+reason | 拒絕 |
| 灰顯可見 | favor<cost 查 Options | 兩項都列出，不足項附原因字串 | N/A |
| 戰中限次 | inBattle=true 連續 Convert | 前 kMaxBattleUses 次成功，之後 false | 額度拒絕 |
| 戰中額度重置 | ResetBattleUses() 後 | inBattle Convert 恢復可用 | N/A |
| 章節邊界不限 | inBattle=false 多次 Convert | 不限次（只受 favor 餘額） | N/A |
| 未綁 spirit | 空 spirit 字串 Convert | 回 false、無副作用 | 拒絕 |
| 軸夾取 | 民心 95 +gain 10 | 民心=100（clamp） | 夾取 |
| favor 未登錄 | 新 spirit 查 Options | Favor()=kNeutralFavor=50 → affordable 依 50 判定 | N/A |
| 未 Bind | FateExchange 未綁層 | Convert 回 false、Options affordable=false | 拒絕 |
| 事件入帳接線 | Convert 成功 | FateEvent 回呼一筆含 spirit/target/cost/gain | N/A |

</frozen-after-approval>

## Code Map

- `Campaign/Governance.h/.cpp` — 補 `AdjustPopularSupport(float)` /
  `AdjustCivilOrder(float)` 夾取 setter（既有軸無直接寫入通道）
- `Campaign/FateExchange.h/.cpp` — 新：`FateTarget` enum、
  `FateOption`/`FateEvent` struct、`Bind`/`Options`/`Convert`/
  `ResetBattleUses`/`SetEventCallback`
- `Examples/FateExchangeTest.cpp` — 新無頭測試（I/O 矩陣全列；
  事件接線比照 ShrineFieldTest favor 接線寫法）
- `CMakeLists.txt` — Campaign lib 源檔 + 測試 target + `POTATO_TESTS`

## Tasks & Acceptance

**Execution:**
- [x] `Campaign/Governance.h/.cpp` — 兩個夾取 setter
- [x] `Campaign/FateExchange.h` — enum/struct/API 宣告
- [x] `Campaign/FateExchange.cpp` — Options 灰顯清單 + Convert 結算
- [x] `Examples/FateExchangeTest.cpp` — I/O 矩陣全列 + 回呼接線驗證
- [x] `CMakeLists.txt` — 註冊

**Acceptance Criteria:**
- Given favor≥kFateCost 的神祇，when Convert(spirit, PopularSupport,
  false)，then favor 扣 kFateCost、民心 +kFateGain、FateEvent 回呼一筆
- Given favor<kFateCost，when 查 Options，then 該項 affordable=false
  且附中文原因（灰顯可見）；Convert 回 false 且零副作用
- Given 戰中情境，when inBattle=true 連續兌換，then 第
  kMaxBattleUses+1 次起拒絕；ResetBattleUses 後恢復
- Given 呼叫端，when 接 FateEvent 回呼，then 可寫入 Recorder/
  LedgerChain（EntrySource::Myth）完成事件入帳

## Design Notes

「灰顯可見」用 NoBattleResolver::CheckOptions 同款形狀（
`{available, reason}` 全列），渲染層/選單層直接消費不必再造邏輯。
戰中限次用執行期計數器而非存檔欄位——次數是「這一場戰」的語義，
跨戰重置歸呼叫端 `ResetBattleUses()`（比照 ShrineField::Bind 的
場域生命週期慣例）。

favor 扣帳走 `MythLayer::AdjustFavor(spirit, -cost)`——既有夾取
與非有限值防禦自動生效；FateExchange 自己守「餘額不足拒絕」
（MythLayer 不管消費語義，職責分層）。

已知上游坑（defer 不堵）：ShrineField::Bind 重綁會清已結算狀態，
同節點可二次入帳 favor——已錄 deferred-work.md，D-3 不負責堵
刷錢洞，只保證兌換側守衛正確。

## Verification

**Commands:**
- `cmake --build build --config Release --target FateExchangeTest` -- MSVC 編譯
- `cd build && ctest -C Release` -- 全綠（含 GovernanceTest/MythLayerTest 不回歸）
- MinGW 編譯 `FateExchangeTest` target -- 通過
- banned 函式掃描 `gets|strcpy|strcat|sprintf|vsprintf|scanf` -- 無新增命中

## Spec Change Log

- **觸發**：三路審查 — 未登錄 spirit 走 `Favor()=kNeutralFavor` 起算，
  `Convert` 就地鑄 50-favor 錢包再扣費（章節邊界不限次 → 幻影錢包）。
  **裁決**（使用者）：Options 維持中立 50 顯示價目（矩陣原列不動），
  Convert 對未登錄 spirit 拒絕。**修訂**:`MythLayer` 補唯讀
  `HasSpirit()`（不改 favor 語義，越過「不改公開介面」邊界由使用者
  授權）;Convert 守衛加 `!HasSpirit → false`；另補滿軸拒絕
  （滿格兌換實得零）、`FateEvent.gain` 報實效、Options 額度盡附
  reason、Governance FromJson/setter NaN 消毒。**KEEP**：回呼翻轉
  依賴、Options 全列價目、戰中額度執行期計數、先變異後派出。

## Suggested Review Order

**貨幣守衛**

- Convert 守衛鏈——未綁/空名/未登錄/額度/非法 enum/餘額/滿軸，
  逐條拒絕保零副作用
  [`FateExchange.cpp:57`](../Campaign/FateExchange.cpp#L57)

- HasSpirit——幻影錢包防線的唯讀查詢（不改 favor 語義）
  [`MythLayer.h:60`](../Campaign/MythLayer.h#L60)

**價目與結算**

- Options 灰顯清單——全列價目 + 分層 reason（未綁定/未指定/
  不足/額度盡/滿軸）
  [`FateExchange.cpp:31`](../Campaign/FateExchange.cpp#L31)

- gain 報實效——after−before,record-is-truth 不虛報
  [`FateExchange.cpp:89`](../Campaign/FateExchange.cpp#L89)

**下游**

- Governance 夾取 setter + FromJson NaN 消毒
  [`Governance.cpp:33`](../Campaign/Governance.cpp#L33)

- 無頭測試：I/O 矩陣 + 幻影拒絕 + 邊界值 + 回呼再入
  [`FateExchangeTest.cpp`](../Examples/FateExchangeTest.cpp)
