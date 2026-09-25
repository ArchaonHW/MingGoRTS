# Spec: D-6 神話層採用接線（playable 消費端）

- 狀態：done（6e132a1，2026-09-25）
- 來源：epic-d-retro-2026-09-23 action item 1——D-2/D-3/D-4 機械層單測全綠但無 playable 消費者（死碼三連）
- 前置：D-1~D-5 done；retro 強化包 9debcf5（第三方隊隔離/生命期/契約修復）

## 問題

`ShrineField`/`FateExchange`/`MythIncursion` 三個系統全部只有單測消費：

| 系統 | 缺什麼 |
|---|---|
| ShrineField | playable 從未 `Bind(map.GetInteractables())`；duanqiao.json 無 shrine 節點；三動詞無 UI 出口 |
| FateExchange | 無任何呼叫者；favor 無生產端（ShrineField 是唯一上游）→ 兌換門檻永遠中立 50 |
| MythIncursion | `ChapterDef.incursion` 已解析無人讀；duanqiao.json 章節無 incursion 塊；Arm/Update 無人呼叫 |
| MythLog | `Myths().SetEventCallback` 單槽被音景 lambda 佔走（L614-623），升階事件丟失不記錄 |

## 範圍

四條接線 + 內容補齊，全在 `DuanqiaoPlayable.cpp` + 兩個 assets JSON。**不改機械層 API、不改存檔 schema。**

### 1. ShrineField 接線（D-2 採用）

- `battle` 建立後 `shrineField.Bind(map.GetInteractables())`（緊鄰 govField.Bind L702）
- 每幀 `shrineField.Update(battle)`（Execution 期間）
- `onFavor` → `campaign.Myths().AdjustFavor(spirit, delta)`——favor 生產端打通
- `onEvent` → eventLog + recorder 同規（戰報可見）
- **UI**：神社 pending（`IsPending`）時在 HUD 側彈小三選一模態：安撫/挑釁/獻祭 → `ApplyChoice`；選後或離開半徑關閉。不擋戰鬥節奏（不暫停）
- `ShrineField` 是 per-battle 物件：戰後解構，下一場重 Bind

### 2. FateExchange 接線（D-3 採用）

- 殼層持有 `FateExchange fateExchange`；`Bind(campaign.Myths(), campaign.Gov())`（載入後一次，Myths/Gov 引用跨存檔穩定——`CampaignState` 聚合體不移動）
- **UI**：部署階段「天命兌換」面板——列出已 BindSpirit 的 spirit（`Levels()`/`SpiritOf` 取境靈名），每尊顯示 `Options()` 兩項，`known`/`affordable`/`battleUsesLeft`/`reason` 全消費（灰顯語義終於有出口）
- 戰中也可開（`inBattle=true` 吃 `kMaxBattleUses` 額度）；每場開始 `ResetBattleUses()`
- `FateEvent` → `campaignChain.Append(EntrySource::Myth, ...)` 帳鏈分錄 + eventLog——**record-is-truth：gain 是實得量**
- **favor 來源依賴**：ShrineField 是本場 favor 唯一生產端；章節跨場 favor 已持久化於 myth_layer 段，不需新存檔

### 3. MythIncursion 接線（D-4 採用）

- 殼層持有 `MythIncursion incursion`；章節套用後若 `def->incursion.enabled`：
  `ParseIncursionKind(def->incursion.kind)` → `Arm(def->incursion.seepage, kind, spawnPos)`（spawnPos 取 map 的 pin `南岸敵營` 或場中點——Ask First）
- Execution 期間每幀 `incursion.Update(dt*timeScale, battle, seepageLv, seepageRegion, myths.SpiritOf(seepageRegion))`
- `IncursionEvent` → `campaign.MythLog().Record(shrine, spirit, when, detail)` + `SetTimeScale(0)` 暫停彈窗（具名三欄+安撫/忽視按鈕）
- `OutcomeEvent` → pacified：`Gov().AdjustPopularSupport(+3)`；ignored：`Myths().Feed(region, Atrocity)` 等價注壓（依 spec 語義選既存 enum）
- `onEvent` 字串 → recorder（戰報「入侵:…」行）
- 戰鬥結束/章節切換 `incursion.Disarm()`（9debcf5 新增 API，堵跨場 UAF）

### 4. MythLog 接線（事件記錄出口）

- `CampaignState` **未聚合** MythLog（已查證，AGENTS.md 描述有誤）——殼層持有 `Gameplay::MythLog mythLog`，session 級不持久（同 campaignChain 慣例）；跨存檔丟失記為已知限制
- `Myths().SetEventCallback` 單槽問題：在現有音景 lambda 內**加一行** `mythLog.Record(ev.shrine, ev.spirit, ev.when, ev.detail)`
- incursion 的 IncursionEvent 同接 MythLog（上節）
- 消費端：結算畫面加「神蹟錄」段——`TestimonyLines()` 每筆一行（C-5 EndingPage 正在平行開發，若其需要持久 MythLog 則由該 story 決定是否收進 CampaignState——存檔 schema 變更不屬本 story）

### 5. 內容補齊（assets）

- `assets/maps/duanqiao.json`：interactables 加 1~2 個 `type:"shrine"` 節點（spirit:"橋姬"、offering 字串）——放在渡口側翼路徑上讓偵查自然觸發
- `assets/campaign/duanqiao.json`：加 `incursion: {seepage:2, kind:"fox_rumor"}`（狐仙假訊比重生鬼軍更貼章節調性；seepage=2 要玩家治理失當才觸發）

## 驗收

1. `DuanqiaoPlayable` 建置通過；duanqiao 章節開局可看到神社 prop、踩進半徑彈三選一
2. 安撫/獻祭後 `Myths().Favor("橋姬")` 實增，兌換面板可見（favor 不再是恆 50 死數）
3. 章節 incursion 達閾時彈暫停窗、安撫後鬼/幻影消散、忽視逾時留存——全部事件進 recorder 戰報
4. MythLog 收錄升階+入侵事件，結算頁可見神蹟錄
5. 全量 CTest 綠（本 story 不加新測試 target——整合層無無頭縫，屬已知限制，retro item 5 已記）
6. `Disarm` 在章節切換路徑確實呼叫（防 ghost/fog 跨場指標）

## 邊界

- 不改 ShrineField/FateExchange/MythIncursion/MythLog/MythLayer 任何 API
- 不改存檔 schema（favor/level 已持久；battleUses/ghost 是執行期態）
- 不做 EndingPage 本體（平行 session 負責 C-5）
- 不做多 region UI——seepageRegion 沿用 D-5 的 `"duanqiao"` 單域慣例

## Ask First（實作前需確認）

1. 神社 UI 形態：HUD 內嵌小三選一 vs 全屏模態（提案：內嵌不暫停，神社互動不該打斷指揮）
2. incursion spawnPos 來源：map pin vs 場中點（提案：找 pin「南岸敵營」fallback 場心）
3. incursion kind 用 fox_rumor（提案，假訊比重現鬼軍貼章節）

## 風險

- DuanqiaoPlayable 是平行 session 熱區——所有接線集中單檔，分批小 commit
- SetEventCallback 單槽：若平行 session 改動回呼結構，加 MythLog.Record 的行要重對齊
- 整合無無頭測試縫（retro 已知限制）——驗收靠 playable 人工跑+全量 CTest 不迴歸

## Review Log（2026-09-25 實作後補記）

- 實作 commit：`6e132a1`（DuanqiaoPlayable +243、兩 assets）
- Ask First 採用提案值：神社三選一內嵌不暫停；spawnPos=「南岸敵營」pin fallback 場心；kind=fox_rumor
- 帳目設計：FateEvent→帳鏈分錄 debit=Civil / credit=Fate（天命帳扣費）/source=Myth，amount=夾取後 gain（record-is-truth）
- 境靈綁定策略：距「斷橋」pin 最近的 shrine 守護靈 = 區域靈（橋姬）——內容驅動不寫死
- 暫停語義：IncursionEvent 回呼設 paused=true，Update 在暫停中不餵——抉擇窗期間窗口凍結，符合 MythIncursion 標頭契約
- 發現修正：LedgerAccount 無 Myth 帳戶（列舉是 Martial/Civil/Fate/Army/Supply）——credit 用 Fate
- 驗證：MSVC+MinGW 建置過、CTest 103/103、banned 乾淨；整合層無無頭縫（已知限制）

## Suggested Review Order

`DuanqiaoPlayable.cpp`（接線主體）→ `assets/maps/duanqiao.json`（兩祠）→ `assets/campaign/duanqiao.json`（incursion 塊）
