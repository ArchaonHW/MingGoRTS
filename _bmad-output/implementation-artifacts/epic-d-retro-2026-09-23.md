---
epic: d
date: 2026-09-23
verdict: accepted-with-open-items
criteria: declared
headless: false
---

# Epic D Retrospective — 神話雙層 (Myth Layer)

## Epic summary

- Epic: **D 神話雙層** — `Campaign/MythLayer` 滲透狀態機（0–3 ratchet）、神社/精靈實體、安撫/挑釁動詞、天命貨幣與治理兌換、神話入侵事件、滲透視效消費端（UX-DR5）。
- Diff range: `0224add^..cdd434b`（37 commits 全範圍；Epic D 歸屬 9 個，其餘為平行 session 的 F-3/F-4/G-4/Bridge/Security 等，不計入本 retro）。
- Story commits:
  - D-1: `0224add`（MythLayer 四階 ratchet + 怒神加成 + 跨章重推導）
  - D-2: `70a8564`（ShrineField 佔領觸發/三路選擇/favor 回呼）
  - D-3: `86bedfa` + `d30c7f3`（FateExchange favor→民心/秩序 + 灰顯價目）
  - D-4: `146937e` + `07bcb70`（MythIncursion 鬼軍夜行/狐仙假訊 + CMake 補回）
  - D-5: `7224e0d` + `4d42ff2` + `cdd434b`（SeepageStage 指令包 + scrim + 審查修正）
- pending_stories: 無（d-1~d-5 全 `done`，sprint-status.yaml）。
- Epic-scoped diff: 15 檔 +2470（MythLayer/FateExchange/MythIncursion/ShrineField/SeepageStage + 五個測試檔），另加 `DuanqiaoPlayable.cpp` 接線 hunk（+701 行 diff，與平行變更共檔）。

### Evidence inventory

| 證據 | 狀態 |
|---|---|
| Epic 驗收條件 | **declared** — epics.md L142-577（FR10、UX-DR5、五個 story 各自 GWT） |
| Story specs | spec-d1~d5 全在 `_bmad-output/implementation-artifacts/`，status=done，含 Review Log |
| 三路審查 diff | d1~d5-review.diff 全存檔 |
| Sprint status | d-1~d-5 = done（2026-09-23 對齊後） |
| 前次 retro | **無**——本檔為首份 epic retro |
| Session logs | 有——CLI summary 檔記錄三次 index 競態與幻影錢包類漏洞修復 |
| 平行 session 干擾 | 範圍內 28/37 commit 非 Epic D；檔案級 churn 已按 story 歸屬分離 |

## Findings

### Aggregate views（inline 推導，subagent 三路交叉佐證）

**AV-1 採用斷層（missing-adoption-gap）— 三鏡頭一致命中，本 epic 最大發現**

D-2 `ShrineField`、D-3 `FateExchange`、D-4 `MythIncursion` 三個機械層**單測全綠但 playable 從未接線**：
- `ShrineField` 全 repo 僅出現在自身檔案 + `ShrineFieldTest` + CMake + docs；`DuanqiaoPlayable.cpp:702` 接的是同型 `GovernanceField` 卻無 shrine 對應物；`assets/maps/duanqiao.json` 零 `type:"shrine"` 節點，`MapInteractable.spirit/offering`（BattleMap.h:53-54）是死 schema。
- `FateExchange` 的 `Bind/Options/Convert/ResetBattleUses` 無任何測試外呼叫者；且 favor 無生產端——`MythLayer::AdjustFavor/BindSpirit` 僅被 `FateExchange.cpp` 與測試呼叫（ShrineField 的 favor 回呼本是設計上的 producer，未接）。
- `MythIncursion` 的 `Arm/Update/Resolve` 無呼叫者；`assets/campaign/duanqiao.json` 無 `incursion` 塊；`ChapterDef.incursion`（ChapterLibrary.h:80）parsed but unconsumed。
- `MythLayer::SetEventCallback` 單槽被 `DuanqiaoPlayable.cpp:614` 的音訊 lambda 佔走（只讀 `Levels()` 丟棄 event payload）——`MythLog` 無實例存在，deferred-work.md:99-100 早已記此 hookup 屬 D-2+ 工作。因槽位「看似已用」，缺口不可見。

後果：epic 驗收條件的「可玩性」層面——「神社可互動」「天命可兌換」「戰鬥被神話事件打斷」——在出貨切片中不可驗證；只有 D-1（Feed→Level）與 D-5（SeepageStage 視效）真正端到端。

**AV-2 重複映射（duplication）**

- 滲透→音景映射有兩份且**數值不同**：Myths 事件 callback 映 `{≥3→2, ≥2→1, else 0}`；`SeepageStage::ForLevel` 映 `{L1→1, L2→2, L3→2}`。等級 2 時 callback 寫 1、逐幀路徑寫 2——逐幀每秒覆寫，callback 的 `SetSeepageLevel` 是 dead-and-misleading（僅 `seepage_up` one-shot 有效）。來源：DuanqiaoPlayable.cpp ~L619 vs ~L1219；F-4（平行）與 D-5 各自映射。
- `PickFogCloud` 複寫了 `BattleSceneSync` 的霧標偏移公式但只抄 `seepageShadow`，漏 `seepageJitter`（±0.22·cell y 向）與 gust `*g`——L2+ 看得見點不到。來源：DuanqiaoPlayable.cpp ~L1131 vs BattleSceneSync.cpp:224-236。

**AV-3 收斂性（god-class）**

`DuanqiaoPlayable.cpp` 本 epic +536/−288（10 commits），現已 ~3100 行且每個 epic 都繼續堆 integration——所有系統的生命期/回呼/讀檔全擠在 main()。非 Epic D 獨有問題但本 epic 加重了它（SeepageStage/Myths callback/settings/ledger producer 全塞同一 TU）。

**AV-4 規格↔實作對帳**

- D-4 spec 稱「忽視/逾時收隊」——實作上僅 FoxRumor 收隊；GhostLegion 忽視後**留存**（它是真的鬼軍）。claim 與行為分歧：讀者以為忽視必散，實則留下擋勝利的第三方活體。
- D-2 spec 把「選擇 UI」列 Ask First 但 Bind/Update/favor 回呼接線屬採用契約——未採用（見 AV-1）。
- `Campaign/MythLayer::ToJson` 無 `"schema"` 標記且 `regions` 以 `unordered_map` 迭代序輸出——違反專案 `WriteJson` 排序鍵慣例與兄弟 blob 的 schema 慣例（`potato.campaign/1` 等皆有）。FromJson 亦無 schema 檢查、接受空/重複 id。
- `FateExchange::Options` 契約矛盾：`battleUses ≥ max` 時 `affordable=true` 卻帶「本場兌換次數用盡」reason（章節邊界呼叫會看到戰中理由）；未註冊 spirit 顯示 affordable=true 但 `Convert` 恆被 `HasSpirit` 擋——「顯示≠可成交」。

### Adversarial lens（16 項，去重後主項）

| # | 位置 | 問題 |
|---|---|---|
| A1 | MythIncursion Trigger + BattleController:883 | team-2 鬼軍計入 `alive[1]`——忽視鬼軍→勝利永遠 Ongoing，軟鎖；`spawnPos` 未驗證/夾取，可生成到場外 |
| A2 | MythIncursion Trigger + DetectGovernanceEvents:311 | 鬼軍被擊潰記 `SurrenderAccepted`(+民心）、補刀記 `Atrocity`(+墮落+滲透）——治理帳本被神話單位污染 |
| A3 | MythIncursion Arm/Settle | `ghost`/`fog` 裸指標跨戰鬥生命期，無 `Disarm()`——re-Arm UAF |
| A4 | MythIncursion Trigger | `state.spirit` 存原始（可能空）字串，`OutcomeEvent.spirit` 未回填「境靈」→ `AdjustFavor("")` 造幻影錢包 |
| A5 | MythLayer ToJson/FromJson | 無 schema、unordered_map 非決定序、FromJson 收空/重複 id、Feed 對錯字 region 自動建檔 |
| A6 | FateExchange Options | affordable/reason 契約矛盾（見 AV-4） |
| A7 | BattleSceneSync Detach | `fogGust`/`fogDrift` 未歸零——Detach→Attach 留懸垂指標 UAF |
| A8 | ShrineField Bind | `radius<=0` 神社永遠不觸發且無警告 |
| A9 | UISettings Load | `theme` AsInt 未範圍檢查（兄弟欄 hudDensity 有夾）；註解誤稱 AsInt 有 UB |
| A10 | DuanqiaoPlayable | cwd 相對路徑：`trained_battle_agent.json`（AudioCues 已由平行 session 修）；`settingsDirty` 在 item active 時退出不 flush |

### Edge-case lens（14 項，與 adversarial 重疊者標註）

新獨立項：
- `kRumorOffset=8.0f` 註解「格數」但加在世界座標未乘 `cellSize`——cellSize≠1 的地圖假訊雲偏移失真（A 鏡頭 #13 同發現）
- re-Arm 丟棄 pending FoxRumor 前未 `fog->Reveal`——假雲永遠綁死隊不可觀測
- `std::clamp(fake.x, 0, maxX-1)` 在 maxX<1 時 lo>hi 是 UB
- `battle.CreateSquad` 先 Emit 再 latch `state.fired/pending`——callback 可重入 Arm/Resolve
- `Arm(kind=(IncursionKind)99)` 不拒絕非法 enum，靜默退化為鬼軍路徑
- `MythLayer::Feed` pressure+delta 可溢位成 inf→`%g` 寫 "inf"→存檔永久無法載入（註解宣稱防的 brick 正是它自己）

### Verification-gap lens（7 項）

- **vg-1/2/3** = AV-1 的三個細分（ShrineField/Incursion/FateExchange 皆 missing-adoption-gap，含證據行號）
- **vg-4** = MythLog 出口未採用（callback 槽被音訊佔走；EndingPage::Generate 的 MythLog* 參數設計上收這個資料）
- **vg-5** `BattleSceneTest` 驅動同一 Sync 路徑斷言位置到 1e-4，但從未呼叫 `SetSeepageFX/SetFogDriftGust`——新可觀測輸出零斷言；`seepageShadow` 被回歸拔掉會 render↔pick 失同步
- **vg-6** Feed→SeepageStage→SetSeepageFX 整鏈只在不可測 GUI main()；`"duanqiao"` region 鍵是裸慣例重複於寫(:2163)/讀(:1214）兩處
- **vg-7** 雙音景映射（與 AV-2 同，裁 cosmetic/trivial）

### 已驗證非問題（checked-and-clean）

banned 函式乾淨；Gameplay↛Campaign 依賴方向守住（新類只收 int）；squad 指標不因消滅懸垂（vector<UniquePtr> 不 erase）；`CampaignState::LoadFromFile` 有保留 event callback；jitter 相位綁格子座標+GustField seeded——確定性成立；五測試已註冊 POTATO_TESTS。

## Behavior verification

- **單元層**：MSVC 全量 CTest 96/96 PASS（含 MythLayer/ShrineField/FateExchange/MythIncursion/SeepageStage 五測試）；MinGW 同綠。D-1→D-5 各 spec 的雙工具鏈驗證記錄在案。
- **端到端層**：D-5 視效鏈（治理事件→Feed→Level→SeepageStage→SetSeepageFX/scrim/theme/audio）已在 playable 實接——但整鏈只在 GUI main() 內，無 headless 驗證路徑（vg-6）。
- **未執行**：D-2 神社互動、D-3 天命兌換、D-4 入侵事件在實機 playable 的端到端行為——因從未接線（AV-1）無法驗證，記為「檢查了，不存在」而非「檢查過，乾淨」。

## Previous-retro follow-through

無前次 retro——本節記錄為不適用。

## Action items

*（全部為提案——retro 不自動套用修復；人決定哪些進 dev loop）*

**落地追蹤（2026-09-23 更新）**：item 2/3/4 已由 `30d2475` 實作——CheckOutcome/DetectGovernanceEvents 第三方隊隔離、MythIncursion 全組（kind/座標驗證、latch 先行、re-Arm Reveal、Disarm、spirit 回填、cellSize 單位）、Feed 有限性重夾+空鍵早退+FromJson 空 id 跳過、FateOption.known、Detach 歸零、radius 保底、theme 夾取。MythIncursionTest +[7] 強化組 16 格，66/66 雙工具鏈全綠。item 1（採用 story）、5-7（流程規則/補測試）仍 open。

### 優先：補採用層（AV-1 直接對應）

1. **[新 story 提案] D-6 神話層 playable 採用** — owner: dev session
   - `DuanqiaoPlayable`：`ShrineField.Bind/Update` 接到 `map.GetInteractables()`；`FateExchange.Bind(Myths, Gov)` + 章節邊界兌換 UI（灰顯價目已現成）；`MythIncursion.Arm/Update/Resolve` 消費 `ChapterDef.incursion`
   - 內容：`duanqiao.json`（或新章節）加 `type:"shrine"` 節點與 `incursion` 定義塊——否則接了線仍無觸發內容
   - `MythLog` 實例化進 `CampaignState` 或 playable，與音訊 callback 合流（多播或鏈式）——deferred-work.md:99-100 的既有 defer 項併入此 story 一併消

2. **[修復] MythIncursion 鬼軍生命期與勝利判定** — owner: dev session（高風險項，採用前必修）
   - `CheckOutcome` 排除 `kGhostTeam`（或第三方隊標記）——忽視鬼軍不該軟鎖勝利（A1）
   - `Arm/Trigger` 驗證 `spawnPos`：`isfinite` + 夾進場界（同 fake clamp）；非法 `IncursionKind` 拒絕
   - 加 `Disarm()` 或每次用時重取 `battle.GetFog()`——消 `ghost`/`fog` 裸指標 UAF（A3）
   - re-Arm 丟棄 pending FoxRumor 前先 `Reveal`；`clamp` 防 lo>hi；`CreateSquad` 前先 latch `state.fired/pending` 防 Emit 重入

3. **[修復] 治理帳本污染 + 幻影錢包** — owner: dev session
   - `DetectGovernanceEvents` 跳過神話隊（`Squad` 標記或 team!=0/1 過濾）——鬼軍受降/暴行不應入民心墮落帳（A2）
   - `MythIncursion` `state.spirit` 存回填後的「境靈」而非原始空串；`MythLayer::AdjustFavor`/`Feed` 對空 id 早退——`""` 錢包與錯字 region 雙堵（A4/A5）
   - `Feed` 的 `pressure+delta` 重新 `isfinite` 夾取——防 inf 寫進存檔變無法載入的 brick

4. **[修復] 契約與確定性細項** — owner: dev session
   - `MythLayer::ToJson`：加 `"schema":"potato.myth_layer/1"`、regions 排序輸出；`FromJson` schema 軟檢查 + 跳過空 id
   - `FateExchange::Options`：`HasSpirit` 併入 `affordable`；quota reason 只在 `affordable=false` 時填（或拆 `inBattle` 參數）
   - `BattleSceneSync::Detach`：`fogGust`/`fogDrift` 歸零
   - `PickFogCloud` 補 jitter/gust 項或把偏移公式抽成共享 helper（消 AV-2 重複映射）
   - `ShrineField::Bind`：`radius<=0` 夾 1.0f + 警告
   - `UISettings::Load`：`theme` 範圍檢查；註解更正（AsInt 本就夾取，AsNumber 是為 bool/string 語義）
   - 音景單一映射：砍 Myths callback 的 `SetSeepageLevel`（留 `seepage_up` one-shot）或兩路共用 `SeepageStage::ForLevel` 表
   - `settingsDirty` 在 quit/視窗關閉時強制 flush；`trained_battle_agent.json` 走 `DemoAssets::ExeDir()`

### 流程教訓（防下一個）

5. **[規則] 採用閘門**：story spec 的「完成定義」加一條——機械層必須有至少一個測試外消費者（playable/工具/資產）才算 done，否則標「mechanism-only」。D-2/D-3/D-4 若當時有這條，AV-1 不會活到 retro 才浮現。
6. **[規則] 高碰撞檔 staged 慣例**：CMakeLists/DuanqiaoPlayable/queue 檔一律 hash-object 建 staged blob 再 update-index（本 epic 三次 index 競態的實證修法）——寫進 AGENTS.md 平行協作節。
7. **[補測試]** `BattleSceneTest` 加 `SetSeepageFX`/`SetFogDriftGust` 斷言段（vg-5）；region 鍵抽常數或加 schema 欄（vg-6 + deferred-work 既有項）。

### Defer（有上下文可後續接手）

- `DuanqiaoPlayable` god-file 拆解（AV-3）——獨立 refactor story，非本 epic 範圍
- `Hold` 階鬼軍是否應有 DPS（威懾性存在 vs 實戰單位）——設計裁決，adv 鏡頭未關閉的疑問
- 假訊雲偏移 `kRumorOffset` 單位歧義（格 vs 世界單位）——先統一註解/命名再決定要不要乘 cellSize

## Acceptance verdict

**accepted-with-open-items**（criteria: declared）

理由：五個 story 全 `done`（無 pending_stories 強制 rejected）；declared AC 的機械層面——滲透 ratchet 只升不降、神社三路選擇入 Recorder/favor、favor 兌換灰顯、入侵兩類事件+安撫/忽視後果、滲透視效三階+motion-reduction+唯讀渲染——**全部在單測中逐條驗證通過**（96/96 CTest）。但 AC 的「可玩/出貨」解讀面：D-2/D-3/D-4 在唯一 playable 切片中不可觸達（AV-1），嚴格說「玩家可互動的神社」「戰鬥被神話事件打斷」在產品中不成立。

判 accepted-with-open-items 而非 rejected：AC 文字以機械行為為主語且全部有測試證據；採用斷層是有名有據的 open item（action 1），不是未知缺陷。若人認為「playable 可觸達」屬於 AC 本義，應改判 **rejected** 並把 action 1 升級為 epic 未完項——人類裁決優先。

## Open questions

1. D-2/D-3/D-4 的 playable 採用是補開 D-6 還是回開原 story？（影響 verdict 解讀與 queue 記帳方式）
2. 鬼軍 `kGhostTeam` 應否從 `CheckOutcome` 敵方集合排除——設計上「忽視鬼軍」是要「鬼軍留存但不算敵」還是「必須清場」？
3. `Hold` 階鬼軍保留 Squad 預設 DPS 是有意（威懾）還是漏設？
4. 音訊 callback 槽位單一 vs 多播——MythLog 接入時是改 `MythLayer` 支援多 listener 還是 playable 端手動鏈式？
