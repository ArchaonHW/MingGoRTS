# Accessibility & Battle-Legibility Review — ux-MingGoRTS-2026-09-18

審查範圍：DESIGN.md、EXPERIENCE.md、.decision-log.md、mockups/ 四方向 HTML。
對比度為依文件中 hex 值以 WCAG 2.x 相對亮度公式手算之近似值（±0.2）。
嚴重度：BLOCKER = 出貨前必改；MAJOR = 應改；MINOR = 建議；NOTE = 確認即可。

---

## 1. 雙編碼覆蓋（hue-only 殘留）

| # | 嚴重度 | 位置 | 問題 |
|---|---|---|---|
| D1 | MAJOR | DESIGN.md:62-63（`mini-f #7d9a72` / `mini-e #a84a3a`）、DESIGN.md:328（minimap「f/e/c dots」） | **小地圖友/敵點位僅以色相區分**。cloud dot 有指定「blurred circle = distinct shape」，但 f 與 e 點同形同大、只差綠/紅——對紅綠色弱（最常見類型）這對正是最易混淆的 hue pair。需指定形狀差（如友=圓點、敵=三角/方點）或敵點加描邊。 |
| D2 | MINOR | DESIGN.md:150-153、309；mockups/direction-war-map.html:139-141（`.morale-v` 直立士氣條僅 `fill #8a7434`） | **地圖上選取單位的士氣條**以填充色 ok/warn/break 三態 hue 漸變。roster 列有 status tag+icon 補救，但 on-map bar 本身在三態切換時無 icon/pattern；warn 與 break 只靠赭→紅 hue。建議 break 態加 broken-fill pattern（ink-chronicle 已有，DESIGN.md:309）或 ⚠ 尾標。 |
| D3 | MINOR | DESIGN.md:162、315（card-slot「accent strip keyed to trigger family」） | 卡片頂條以色相標 trigger family；卡面另有 trigger→action 文字行可補救，但「掃一眼牌列識別觸發族」的快讀路徑是 hue-only。建議頂條加 glyph/紋路（8 種 trigger 對應 8 個小 icon）。 |
| D4 | NOTE | DESIGN.md:223（intel 在 war-map =「ink on paper-hi (no hue)」） | 此為正面案例——theme 重新配色但保留數字+標籤編碼。確認四主題皆如此即可。 |
| D5 | OK | DESIGN.md:203-215、327、334 | faction=形狀+填充、morale=icon+斷筆、cloud=形狀+`?`/P、selection=環+角括、feed 急=戳記+glyph——表格層面雙編碼覆蓋完整。Outcome 五階梯以文字命名（EXPERIENCE.md:71），無 hue 依賴。 |

## 2. 對比度（實算 WCAG 近似值）

**通過項（參考）**：war-map `ink/paper` 7.3:1、`enemy-tag/paper-hi` 6.0:1、estop 字 `f0e6cc/wax` 6.0:1、`friendly` 描邊/paper 3.8:1；command-room `ink/slip` 10.2:1、`brass-hi/plate-dk` 9.7:1、estop `f4e6c8/a02020` 6.2:1、`seal/slip` 4.75:1（擦邊）；tactical-sim `text/bg` 15.0:1、`dim/bg` 6.0:1、`teal/bg` 10.3:1、`amber/bg` 8.7:1、red-on-panel ≈4.6:1（擦邊）；ink-chronicle `ink/paper` 11.3:1、`ink-soft/paper` 6.3:1、`vermilion/paper` 5.3:1、dab 字 `paper/ink-dab` ≈9.5:1。

| # | 嚴重度 | 位置 | 實算值 | 問題 |
|---|---|---|---|---|
| C1 | MAJOR | mockups/direction-war-map.html:70、129（`.bridge-note`、`.warn-tag` 13px `#b03a2a` on `#d3c096`）；DESIGN.md:31、66 | **grease/paper ≈ 3.37:1**（on paper-hi ≈ 4.3:1） | war-map 的警告註記（「← 士氣動搖!」是 DESIGN.md:66 指定語域）13px 紅蠟筆字落在沙盤紙上只有 3.4:1——未達 4.5:1 body 門檻，也未達「大型文字」資格（13px <18.66px）。這是主題的招牌 alert 顏色，需在 alert 情境改用 `wax #9c2f22` 加深（≈4.6:1 on paper-hi）、墊 paper-hi 底、或加粗至 700+描邊。 |
| C2 | MAJOR | mockups/direction-command-room.html:55、76、97（`.t-slip .h`、`.intel-slip .h`、`.folder .st`，10px `#8a6a3a` on `#e6d9b8`） | **brass/slip ≈ 3.56:1** | 電報標頭、情報標籤、卷宗狀態戳全部 10px 黃銅字在米色紙上 3.6:1——未達 4.5:1。`.folder .st` 是 squad 狀態戳（承載 gameplay 意義的 tag！），非 gloss。需改用 `brass-dk #5a4322`（≈5.8:1）或 ink。 |
| C3 | MAJOR | mockups/direction-tactical-sim.html:98、107、126（`.phead .en`、`.mbar .k`、`.frow .t` 9–10px `#4a5f6a` on panel） | **faint/panel ≈ 2.86:1** | `faint` 規則上僅作 gloss（DESIGN.md:262），但 mockup 實際拿它放 **事件時間戳** 與 **士氣/兵力條的 axis key**（`.mbar .k` 9px）——時間戳是 feed 條目的一部分，bar key 是「這條是兵力還是士氣」的唯一標籤，均非純 gloss。2.86:1 連 3:1 都不到。時間戳應升 `dim`（6.0:1），bar key 升 `dim` 或併入 tooltip。 |
| C4 | MINOR | mockups/direction-war-map.html:50（`.gridnum` 11px `rgba(74,66,52,.55)`） | ≈ **2.3:1** | 座標格號是 gloss 定位尚可，但 11px+55% alpha 已接近不可讀；若有「依格號報位」玩法需升 alpha。 |
| C5 | MINOR | mockups/direction-command-room.html:62（`.rail .lbl` 12px `#a8843c` on 木紋漸層 `#3f2f1c`–`#241a10`） | ≈ **3.7–4.9:1** | 在燈暈亮部處低於 4.5；屬 gloss label，可接受但建議統一加 `plate-dk` 底或改用 `brass-hi`。 |
| C6 | MINOR | DESIGN.md:41、mockups/direction-war-map.html:121（`.bar.mor>i #8a7a58`→`#8a7434` on track `rgba(55,48,31,.14)`/paper） | fill vs paper ≈ **2.5:1**、vs track ≈ 2.0:1 | 士氣條赭色填充對紙面/軌道低於非文字元件 3:1 建議值；9px 高度尚可辨，但與 break 紅的辨識在低視力玩家可能不足。可加邊框或提高 track alpha。 |
| C7 | MINOR | mockups/direction-ink-chronicle.html:103（河道標籤 `rgba(70,80,86,.75)` on wash） | ≈ **2.0–2.5:1** | 純裝飾地名浮水印，可接受；確認無 gameplay 文字用 `.75` 以下 alpha 即可（`ink-faint` token 本身 ≈2:1，DESIGN.md:90，只准當邊框/divider）。 |
| C8 | NOTE | DESIGN.md:242 | 對比地板條文本身 | 「primary text ≥4.5:1」寫了，但「never set `pencil`/`ink-soft` below 11px」只限字級、未限對比——C1/C2 的違規正是文件自己定的 token 在合法字級下產生的。建議加一條：alert/accent 色用於 <14px 文字時須達 4.5:1，否則墊底或換 token。 |

## 3. 字級與 CJK 可讀性

| # | 嚴重度 | 位置 | 問題 |
|---|---|---|---|
| T1 | MAJOR | DESIGN.md:255（ramp 含 9/10/11px）、EXPERIENCE.md:37（UI 縮放 75–150%+字級） | **75% 縮放無下限保護**。繁中最小可讀約 12px@1080p（筆畫密度如 鬱/驚/體）；tactical-sim body 13px×0.75=9.75px、9px tag×0.75=**6.75px**——低於任何 CJK 地板。規格未寫 effective-px 下限。建議：文字渲染強制 clamp ≥12px effective（縮放時文字獨立地板），9–10px 的 ramp 階直接廢除或標「僅 150% 密度以上顯示」。 |
| T2 | MINOR | mockups/direction-command-room.html:111（`.seize-stamp .t3` 8px）、tactical-sim `.srow .tag`/.`mbar .k` 9px | 接管中戳記第三行 8px、狀態 tag 9px——即使 100% 也在 CJK 邊緣；且這些是承載 seizure 狀態的資訊行。 |
| T3 | MINOR | DESIGN.md:99-119（PMingLiU 為第一優先 serif） | PMingLiU 在 <14px 的渲染品質差（bitmap-era hinting）；war-map/command-room/ink-chronicle body 皆 14–15px 貼邊。建議小字級 fallback 順序把 Noto Serif TC 提前，或 12px 以下強制 sans（Microsoft JhengHei）。 |
| T4 | NOTE | EXPERIENCE.md:37 | 「字級」調整與「UI 縮放」的互動未定義（相乘？取大？）——需在 Settings 規格寫明，且兩者皆受 T1 地板約束。 |

## 4. 光敏性與動態減弱

| # | 嚴重度 | 位置 | 評估 |
|---|---|---|---|
| P1 | OK | DESIGN.md:321、EXPERIENCE.md:157；mockups/direction-tactical-sim.html:114-116 | 1.1s collapse pulse ≈ **0.9Hz**，遠低於 3 flashes/s 門檻；且只動 2px 邊框 + rgba .04→.14 背景，亮度擺幅小。安全。 |
| P2 | MINOR | EXPERIENCE.md:138、147-148 | 神話滲透全主題 swap：tactical-sim(L≈0.005)→ink-chronicle(L≈0.80) 是**全螢幕亮度跳變**，單次轉場非 WCAG flash 危害，但夜間遊玩會眩目；且若事件連發，反覆全屏亮暗交換接近重複閃爍。建議：(a) swap-in 下限時長 + 事件冷卻期強制 crossfade；(b) 「lamp flicker」類過場明確寫入 <3Hz 上限。 |
| P3 | MAJOR | EXPERIENCE.md:137、157 | **動態減弱覆蓋不完整**。條文只列 shake、seepage motion、cloud turbulence；但規格中仍會動的有：collapse pulse（0.9Hz 持續閃爍——reduced-motion 下是否停？未寫）、cloud「gentle pulse」（:137 明寫保留 pulse——即 reduced-motion 仍有東西在動）、reveal ring sweep、seizure countdown ring、卡片觸發 flash、開戰 seal slam、戰報打字機、roster 逐行浮現、minimap blink（若有）。需補一張「reduced-motion 行為對照表」：每項動畫的 fallback（static/crossfade/instant）。 |
| P4 | NOTE | EXPERIENCE.md:139 | 打字機與逐行浮現對動暈玩家低風險，但建議 reduced-motion 下直接整段呈現（現規格只說 skippable）。 |

## 5. 鍵盤/手把可達性

| # | 嚴重度 | 位置 | 問題 |
|---|---|---|---|
| K1 | MAJOR | EXPERIENCE.md:87、106（Alt+drag 佈釘） | **拖曳型操作無具體替代路徑**。:125 有「every mouse action has a key path」宣言，但全文未寫機制。Alt+drag 需具體化：如選取 pin 後方向鍵微移 + Enter 放下，或座標輸入。宣言不等於規格。 |
| K2 | MAJOR | EXPERIENCE.md:80、107（卡拖入槽、拖曳排序） | 同上：card 的 slot 指派與 priority 排序只有 drag 路徑。需補「click-to-pick + click-to-place」與 priority 的 +/- stepper（card editor 已有 stepper 模式，:81，延伸即可）。 |
| K3 | MINOR | EXPERIENCE.md:85；DESIGN.md:166 | **雲朵點選目標大小 ∝ P**——低機率雲=小目標，滑鼠精度要求高。未寫最小點選半徑（.decision-log.md:9 僅提掌機重算）。建議 min hit-area ≥ 24px 且 Tab 可循環選取地圖互動物。 |
| K4 | MINOR | EXPERIENCE.md:103、110 | RMB context command 的鍵盤對等鍵未指定（如 Menu 鍵/Enter）；radial menu 是手把方案但鍵盤路徑空白。 |
| K5 | MINOR | EXPERIENCE.md:109 [ASSUMPTION] | 攝影機 edge-pan/WASD 未確認——edge-pan 對 motor-impaired 不友善，需確認 WASD/方向鍵 pan + 可關 edge-pan。 |
| K6 | OK | EXPERIENCE.md:88 | Timeline scrub 有 click-to-jump 替代 ✓；hover-only 全文多次禁止 ✓。 |

## 6. 極簡密度下的資訊存活

對照 EXPERIENCE.md:42-50 逐項：

| 需感知項 | 極簡下存活？ | 評估 |
|---|---|---|
| CP 可用量 | ✅ CP/急停 always（:45） | OK |
| **情報剩餘** | ❌ **intel count 只在標準以上**（:44 top strip 極簡=phase+clock+pause） | **MAJOR**：情報是消耗性資源（Observe 2/Probe 1），極簡玩家花完才發現沒情報。建議極簡至少留 pip/badge，或情報歸零/不足時觸發臨時徽章。 |
| 小隊潰逃 | ✅ roster=status pips+alert badges（:46）；feed 急 entry | OK——rout 是 alert 級。 |
| **情報過期（expiring）** | ⚠️ 不明 | **MINOR**：:93 intel lifecycle 有 `expiring` 態，但極簡 feed 只留急 entries——若過期不蓋急戳，此態在極簡完全消失。需指定過期=stale-badge on cloud 或 急 entry。 |
| 神話事件進行中 | ⚠️ 條件性 | **MINOR**：滲透開著時全主題 swap 不可能錯過 ✓；但 Settings 可關「神話滲透」（:37）——關閉後神話事件如何提示？規格無 badge/icon fallback。需補「seepage off = event icon + feed 急戳」。 |
| doctrine 卡觸發 | ⚠️ | **MINOR**：牌列極簡 hidden、觸發 flash 是標準以上（:50）；達級 feed 也隱去。玩家選極簡=失去「計畫正在執行」的核心回饋。建議極簡保留觸發瞬間的 badge 閃現（transient，不算 persistent chrome）。 |
| 疲勞條 | ❌ | **MINOR**：.decision-log.md:46 自承 mocks 無疲勞呈現；:46 疲勞條只存在完整級。若疲勞影響執行率，極簡/標準玩家無法察覺——至少 warn 態要升 badge。 |

## 7. 急停 hold-to-confirm

| # | 嚴重度 | 位置 | 問題 |
|---|---|---|---|
| H1 | MAJOR | EXPERIENCE.md:83、105 | **hold-to-confirm 無非按住式替代**。對震顫/肌力不足玩家，維持按壓是已知障礙模式（Xbox Accessibility Guidelines 明列）。急停本身已是 fail-safe（按錯只花 1CP 換暫停，SPACE 免費暫停仍在），防誤觸需求可用：double-tap、可調按住時長（0–2s 滑桿）、或設定關閉 confirm。建議三者至少實作一個，寫進 Settings。 |
| H2 | MINOR | 同上 | 按住期間的進度回饋（按住多久了）未指定——需視覺環/填充進度，否則玩家不知何時放開才算確認。 |

---

## 統計

- BLOCKER：0
- MAJOR：9（D1, C1, C2, C3, T1, P3, K1, K2, H1；另含 §6 情報項 1 件計入 MINOR→MAJOR 者見表）
  - 嚴格計：D1, C1, C2, C3, T1, P3, K1, K2, H1, +極簡 intel 1 件 = **10**
- MINOR：約 14
- NOTE/OK：其餘

**總評**：規格在「原則層」的無障礙論述完整且優於多數商業 RTS spec（雙編碼表、無 hover-only、動態減弱、雙語），但**執行層出現原則與 mockup 自相違背**的系統性落差：自己定的地板（4.5:1、gloss-only faint、無 hover-only）在自己產出的 mockup 裡被破壞（grease 3.4:1、brass 3.6:1、faint 2.9:1 放時間戳、drag 無替代機制）。修正方向多為 token 層調整（換色/加底/補 fallback 表），非結構重設。
