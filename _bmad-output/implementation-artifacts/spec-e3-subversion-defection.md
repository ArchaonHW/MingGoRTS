---
title: 'E-3 顛覆與內應——倒戈小隊生成 + 情報消費'
type: 'feature'
created: '2026-09-23'
status: 'done'
baseline_commit: '8a0a076'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-e2-nobattle-resolution.md'
  - '{project-root}/Campaign/NoBattleResolver.h'
  - '{project-root}/Gameplay/GeneralDossier.h'
  - '{project-root}/Gameplay/Squad.h'
  - '{project-root}/Examples/DuanqiaoPlayable.cpp'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** epics Story E.3 要求「情報消費、敵軍部分小隊開場即
倒戈」——但 E-2 落地的 Subversion Success 目前**同談判跳戰鬥
結算**（spec-e2 明定「目前」二字即為 E-3 預留）。顛覆路徑
因此名實不符：玩家看到的是和平結算，內應倒戈的機械事實
不存在；且 verified 判詞不消耗——同一批情報可無限重用，
「情報消費」僅是帳面一筆。

**Approach:** 三件分層補——
(1) `Squad::Defect` + `Gameplay::ApplyDefection`：確定性把
    指定數量敵軍小隊翻面成我軍（士氣最弱者先倒——被策反的
    總是先動搖的），部署階段重編 doctrine 防保留敵方腳本；
(2) `HearsayEntry.consumed` + `GeneralDossier::ConsumeVerified`：
    顛覆成功消耗 verified 判詞，`Gather` 不再計入已消耗者——
    情報資產是真消耗品；
(3) `DuanqiaoPlayable` 改接線：Subversion Success → 倒戈 +
    照常開戰（全倒戈 → 首 tick 自動 Victory，不戰而勝是
    湧現結果而非捷徑）；談判/嚇阻維持 E-2 和平路徑。

## Boundaries & Constraints

**Always:**
- 倒戈人數 = `min(verifiedIntel, 敵軍存活小隊數)`——情報條數
  即策反深度；全數倒戈合法（首 tick CheckOutcome → Victory，
  不戰而勝自然湧現）
- 倒戈選取**確定性**：士氣升冪 → 兵員升冪 → 名稱升冪，無亂數
- 倒戈小隊 `Defect(playerTeam)`：team 翻面 + engageTarget 清空
  + order=Hold；部署階段內 `AssignDoctrine` 補一條
  `{Always→AttackNearest}`——不保留敵方腳本、也不無令待機
- 情報消費**只在顛覆成功時**：Failed/Backfired 不消耗判詞
  （失敗代價是敵首波優勢，情報網還在）
- `Gather` 的 `verifiedIntel` 計數排除 `consumed` 條目——
  消耗是真實的，下一章對同敵將不能重複顛覆同一批內應
- `plantedUnrefuted` 判定不變（`planted && !verified`）——
  假情報陷阱與已消耗的真情報無關
- `ApplyDefection` 放 Gameplay 層、只吃 `BattleController&` +
  team 編號 + 數量上限——Gameplay 不 include Campaign
- 全倒戈時設 `peacePathZh="顛覆"`（戰報語域正確：敵陣自潰）；
  部分倒戈+實戰 → 正常戰報語域（發生過戰鬥不說「不戰」）

**Ask First:**
- 倒戈數 = verifiedIntel 條數（提案：1 條情報 = 1 支內應隊）——
  若要更激進（如 ×2 或比例制）需裁決
- 全倒戈 → 走正常戰鬥結算（首 tick Victory）而非
  peaceSettlement 捷徑——PostBattle/loot/治理折帳照跑但皆近零；
  若要直接和平結算需裁決
- 倒戈隊 doctrine 提案 `{Always→AttackNearest}`——
  若要「按兵不動待收編」（HoldPosition）需裁決

**Never:**
- 不改 `NoBattleResolver` 公開介面（CheckOptions/Resolve/Gather
  簽名不動；Gather 的 consumed 排除是語義修正不是介面變更）
- 不改 `potato.campaign/1`、`potato.battle_map/1` schema
- `consumed` 旗標不進 GeneralDossier 序列化（dossier 本就
  session 級不持久——不產生存檔相容問題）
- 不讓倒戈小隊變第三方（不引入 team 2）——CheckOutcome 把
  非 0 隊都算敵方，倒戈必須真的翻進 team 0
- 不改談判/嚇阻的既有路徑（E-2 行為凍結）
- 不注壓 MythLayer

## Scope

**In:**
- `Gameplay/Squad.{h,cpp}` — `Defect(int newTeam)` 翻面方法
- `Gameplay/SubversionDefect.h/.cpp` — `ApplyDefection` 確定性
  批次倒戈 + fallback doctrine
- `Gameplay/GeneralDossier.{h,cpp}` — `HearsayEntry.consumed` +
  `ConsumeVerified(generalName)`
- `Campaign/NoBattleResolver.cpp` — `Gather` 排除 consumed 條目
- `Examples/DuanqiaoPlayable.cpp` — Subversion Success 改接
  倒戈+開戰；全倒戈設 peacePathZh
- `Examples/SubversionDefectTest.cpp` — 無頭測試
- `CMakeLists.txt` — 註冊源檔+測試+POTATO_TESTS
- `auto-dev-queue.yaml` — E-3 條目補登

**Out:**
- 倒戈小隊的專屬視效/立場標記（UI 已以 team 0 色呈現）
- 敵方反制顛覆（E-3 後續 epic 範疇）
- 多章節顛覆內容

</frozen-after-approval>

## Code Map

- `Gameplay/Squad.{h,cpp}` — team 為 private 無 setter，需新增
  `Defect(int)`：team 翻面 + engageTarget=nullptr + order=Hold
- `Gameplay/SubversionDefect.{h,cpp}` — 新：`ApplyDefection`
  排序挑選 + Defect + AssignDoctrine（Deployment 階段內呼叫）
- `Gameplay/GeneralDossier.{h,cpp}` — HearsayEntry +consumed；
  ConsumeVerified 把該將 verified&&!planted 條目標消耗
- `Campaign/NoBattleResolver.cpp:44-53` — Gather 迴圈加
  `!e.consumed` 於 verifiedIntel 計數
- `Examples/DuanqiaoPlayable.cpp` ~L1450 — modal Resolve 段：
  Subversion Success 從 peaceSettlement 分支改接倒戈分支
- `Gameplay/EnemyGeneral.cpp:152` — ApplyTo 給敵隊各小隊
  AssignDoctrine（敵方腳本）——倒戈隊必須重編，否則帶敵令
- `Gameplay/BattleController.cpp:879` — CheckOutcome：
  `GetTeam()==0?0:1` 二分；全倒戈 → alive[1]=false → Victory

## Tasks & Acceptance

**Execution:**
- [ ] `Squad::Defect` + `Gameplay/SubversionDefect.h/.cpp`
- [ ] `GeneralDossier` consumed + ConsumeVerified
- [ ] `NoBattleResolver::Gather` 排除 consumed
- [ ] `DuanqiaoPlayable` Subversion Success 改接倒戈+開戰
- [ ] `Examples/SubversionDefectTest.cpp` — I/O 矩陣全列
- [ ] `CMakeLists.txt` 註冊
- [ ] `auto-dev-queue.yaml` E-3 補登

**Acceptance Criteria:**
- Given 顛覆成功且 verifiedIntel=N，when ApplyDefection，
  then min(N,敵存活隊數) 支敵隊翻為 team 0、order=Hold、
  doctrine=AttackNearest 兜底
- Given 敵隊全數倒戈，when BeginExecution 後首 tick，
  then outcome=Victory 且 peacePathZh=顛覆
- Given 顛覆成功，when ConsumeVerified，then 該將 verified
  判詞標消耗、Gather 重算 verifiedIntel=0（同章不可再顛覆）
- Given 顛覆 Failed/Backfired，when 結算，then 判詞不消耗
- Given 士氣 [0.3,0.5,0.9] 敵隊與 count=2，when 套用，
  then 士氣 0.3/0.5 兩隊倒戈（最弱者先）
- Given 零敵隊或 count=0，when ApplyDefection，then 回 0 不崩潰

## Design Notes

倒戈選取排序：士氣升冪（最動搖者先被策反）→ 兵員升冪
→ 名稱升冪（穩定 tiebreak，全確定性）。Defect 順序與
建隊序無關——不依賴指標或迭代序。

情報消費語義：`verified` 判詞是「已證實的敵將弱點檔案」——
顛覆把它們兌換成內應。消耗後檔案仍在（歷史記錄），但不再
計入可用情報——`consumed` 是「已兌現」標記非刪除。

全倒戈收斂：敵全隊翻 team 0 → CheckOutcome alive[1]=false
→ Victory。不繞 peaceSettlement 捷徑——讓戰鬥狀態機自己
走到勝利，結算/存檔/名冊全走正常路徑（近零傷亡是結果
而非特判）。peacePathZh=顛覆 只為敘事語域。

## Verification

**Commands:**
- `cmake --build build --config Release --target SubversionDefectTest`
- `cmake --build build --config Release --target DuanqiaoPlayable`
- `cd build && ctest -C Release -R "NoBattle|Subversion"`
- MinGW 編譯 SubversionDefectTest target -- 通過
- banned 函式掃描 -- 無新增命中

## Implementation Review Log（95114b6）

- `Squad::Defect` 實作語義：僅敵方存活隊可翻（team!=0 或 0 兵員回 false）；翻面清 `chasing_` 殘留追擊目標（否則新入隊的內應繼續咬舊目標）、`order=Hold`——倒戈軍按兵待命是心理寫實也是安全預設。
- 排序「stable name」落地為 `std::string` 字典序比較——同名同士氣同兵員的排序鍵，補排序決定性最後一格。
- `ConsumeVerified` 沿用 dossier 既有 `MarkVerified`/`MarkRefuted` 動詞-狀態命名慣例；`Gather` 的 consumed 過濾放在 verified 檢查同層——帳面憑證保留、僅排除於可用計數，符合「帳面已登記即不可抹除」。
- 全倒戈 `peacePathZh="顛覆"` 掛在 `nbWon` 且 `CheckOutcome()==Victory` 且敵隊為空——三條件缺一不可，避免誤把「顛覆未選卻全敵潰散」當內應語域。
- 驗證：MSVC `SubversionDefectTest` 全綠、`DuanqiaoPlayable` 建置通過、MinGW 全綠、相鄰 NoBattle*/Squad*/GeneralDossier 回歸 7/7、banned 掃描乾淨。測試 `Near()` 未用警告（C4505）已移除。
- 殘留風險：①倒戈隊 doctrine 固定 AttackNearest——若要「內應按兵不動」可切 Hold 兜底，規格未凍死；②倒戈後敵隊數變化使 MythLayer 語域與實況脫鉤屬可接受——語域是戰前情報態。

## Suggested Review Order

`Gameplay/Squad.h/.cpp`（Defect）→ `Gameplay/SubversionDefect.h/.cpp`（ApplyDefection 排序）→ `Gameplay/GeneralDossier.h/.cpp`（consumed + ConsumeVerified）→ `Campaign/NoBattleResolver.cpp`（Gather 過濾）→ `Examples/DuanqiaoPlayable.cpp`（Subversion 分支）→ `Examples/SubversionDefectTest.cpp` → `CMakeLists.txt`。
