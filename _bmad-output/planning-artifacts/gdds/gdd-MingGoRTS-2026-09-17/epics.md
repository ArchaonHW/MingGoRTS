---
title: 'MingGoRTS — Development Epics'
project: 'MingGoRTS'
updated: '2026-09-18'
status: 'complete'
stepsCompleted: [1, 2, 3]
inputDocuments:
  - '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md'
  - '_bmad-output/game-architecture.md'
  - '_bmad-output/narrative-design.md'
  - '_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/DESIGN.md'
---

# MingGoRTS — Development Epics

Companion to `gdd.md`. Each epic delivers mechanics serving the pillars; status reflects the implemented codebase as of 2026-09-18. Architecture constraints cite `_bmad-output/game-architecture.md`.

## E-1 Battle Core — Implemented

Doctrine interpreter (trigger/condition/action/cooldown), squad entities, flow-field group pathing, three-beat phase machine (Planning → Execution → Aftermath), named squad captains.

*Serves pillars 1, 4.*

## E-2 Intelligence Layer — Implemented

Probability-cloud fog (`QuantumFog`): superposed candidate clouds, Observe (intel 2), Probe (intel 1, partial), contact reveal (~3 cells, free), intel decay back to superposition (~25 s), entangled squads (Q-2), personality priors (Q-4), injectable random sources (Q-5), save/replay of fog state (Q-6), phase interference prototype (Q-7), perf bench (Q-8).

*Serves pillars 1, 3.*

## E-3 Planning Layer UI — Implemented

Card-slot deck editor, planner template one-click load, objective/rally map pins (A-1, `PlanningDeck` + `DuanqiaoPlayable`).

*Serves pillar 1.*

## E-4 Real-time + Post-battle — Implemented

Squad status bars, morale display, CP intervention buttons, doctrine-trigger message stream (A-2); historian's-pen report, named roster, replay timeline (A-3).

*Serves pillars 3, 4.*

## E-5 Governance Axis — Partially

**Done:** five-rung outcome ladder (`SubdueWithoutWar`, `Victory`, `GovernedPeace`, `Defeat`, `Fallen`), defection-vs-annihilation accounting, 民心/秩序 battle-level fields.

**Remaining:** in-battle 民心/秩序 *sources* (village occupation, surrender acceptance, convoy protection, atrocity penalties), campaign accumulators, unrest events at low campaign order.

**Architecture constraints:** accumulators live in `Campaign/Governance.*` (new layer); battle emits governance events into the recorder stream — `BattleController` must not know `Campaign` exists; all values serialize through `potato.campaign/1` via the `CampaignState` facade.

*Serves pillar 2.*

## E-6 Myth Layer — Not started

Shrine/spirit entities on battle maps, pacify/provoke verbs, myth favor (天命) currency, myth→governance conversion, ghost-legion incursion events.

**Architecture constraints:** `Campaign/MythLayer.*` owns per-region seepage levels (0 quiet → 1 anomalies → 2 seep → 3 manifest) and god-stance table — pure logic, headless-testable like `QuantumFog`. Renderer is a read-only consumer (fog tint → soundscape → full theme-swap via `UITheme` tokens). Seepage never decreases within a chapter; transitions record events (record-is-truth). Sound precedes visuals.

*Serves pillars 2, 4; enables narrative dual-layer.*

## E-7 Campaign Frame — Not started

Chapter map, ~15 commander-biography structure, persistent 民心/秩序/墮落, roster carry-over, inter-chapter causality hooks.

**Architecture constraints:** new `Campaign/` layer — `CampaignState` facade aggregates subsystem stores (`RefitCamp`/`Roster` [existing] + `Governance`/`Dispositions`/`GodStance`/`IntelLedger`/`TitleTrack`/`ChapterState` [new]) into one `potato.campaign/1` file. Saves at chapter boundaries only (atomic tmp+rename, schema-validated, versioned migration). Dependency direction `Gameplay ← Campaign` — `Gameplay` never includes `Campaign` headers. Chapter/arc content as data: `assets/campaign/*.json`. Soft gates within hard arcs: arc order fixed, chapters inside an arc freely sequenced.

*Serves all pillars; delivery vehicle for the Heavy narrative.*

## E-8 無戰 Chapters — Not started

Negotiation/deterrence/subversion resolution path on designated chapters: pre-battle council phase where intelligence + reputation + myth favor can retire the enemy commander without combat. Highest-tier evaluation; gated by campaign 民心 and chapter flags.

**Architecture constraints:** a chapter flag in `assets/campaign/` chapter defs marks 無戰-eligible; resolution is a `ChapterState` transition, not a battle — no `BattleController` involvement; outcome still emits recorder-equivalent events so the ledger writes it. Gating reads `Governance` (民心/治績) + `GodStance` (天命) through the `CampaignState` facade — never directly.

*Serves pillar 2; the 至聖者 fantasy made literal.*

## E-9 Content Pipeline — Partially

**Done:** `potato.character_card/1` schema, ROC commander card set, VRM portrait pipeline (`PortraitRenderer`/`PortraitBaker`), synth-data tools.

**Remaining:** doctrine card pool expansion, enemy commander deck authoring tool, myth-layer art set, `assets/narrative/` + `assets/campaign/` content authoring (packs for E-11/E-7).

**Architecture constraints:** all content loads through per-type Library+LoadDir (SquadTemplateLibrary precedent) — schema-validated, bad files skipped with warnings; narrative strings are content IDs, never inline text in code.

## E-10 Polish & Onboarding — In progress

Tutorial scenario (Q-9) — pedagogical design per the brief: teach *regret*, not syntax. The player starts with a pre-filled deck that fails visibly; the lesson is finding and patching the hole, not learning card grammar. Also: difficulty curve pass, balance telemetry, perf verification, JP/KO i18n completion, and the **doctrine sandbox simulator** (deck-vs-deck instant testbed — required balancing tool per the brief, excluded from MVP but in scope for v1.0).

## E-11 Narrative Systems — Battle layer landed, campaign layer not started

Source: `_bmad-output/narrative-design.md` (complete, 11 steps). The ledger/pen axiom turns into load-bearing systems:

- **Modular historian report** ✅ — `Gameplay/HistorianReport` assembles the replay event stream into fragment + judgment-template reports; every report states its omission count (「本報告省略 N 項」); `HistorianReportTest` green. *Extends E-4.* Campaign follow-up: swap selection to `NarrativePack` content IDs.
- **敵將判詞 (rumor-state)** ✅ battle view — `Gameplay/GeneralDossier` shows the hearsay judgment per commander; `GeneralDossierTest` green. Remaining: `Campaign/IntelLedger` owns cross-chapter provenance (dated/sourced/plantable entries — Lossy-Ledger pattern); scouting writes verified entries; cunning plants fakes. *Extends E-2 dossier + E-9 cards.*
- **Rival counselor counter-deck** ✅ — `Gameplay/RivalDeck`: `BattleController` tracks per-team trigger-hit stats (`triggerUsage`), `RecordBattle` accumulates across battles, `BuildCounterDeck` picks the pre-authored counter-variant keyed to most-used trigger; `potato.rival_deck/1` persists; warning line 「彼之陣法，似針對我軍慣用『X』」 via `HistorianInput::counteredHabit`. `RivalDeckTest` green. *Extends E-1/E-9 + BattleRecorder stats.*
- **Dispositions + reputation persistence** — enemy commander fates (subdued/slain/defected/retired) and the title track ("斷橋軍師") recorded per chapter; the ending cites them. `Campaign/Dispositions` + `Campaign/TitleTrack`. *Depends E-7/G-9.*
- **Named myth-layer event log** — which shrine, which spirit, when; feeds the gods' testimony at the ending and the myth-seepage UX hook. `Campaign/GodStance`. *Depends E-6.*
- **Chapter conventions** — 題詞 opening verse, 判詞 entrance judgment, 欲知後事 closing hook; ending voices: 霸業 / 仁政 / 無字 / rival's hand (Fallen) via `Campaign/EndingPage` — generated from the accumulated record, never menu-picked; no true ending. *Depends E-7.*
- **Narrative content packs** — `potato.narrative_pack/1` in `assets/narrative/`, locale-keyed (zh-TW primary, en); fragment corpus + register templates (historian/fox/human/rival's-hand); `Campaign/NarrativePack` loads + validates via Library+LoadDir.
- **Scope guard:** MVP (Broken Bridge) ships without the campaign frame; N-1/N-2/N-3 battle-layer items have landed.

*Serves all pillars; the four-beat emotional register (command→suspense→mourning→the brush that stops).*

## Sequence

E-5 → E-7 → E-11 → E-6 is the dependency-ordered path per `game-architecture.md` First Steps (E-6's MythLayer needs `Campaign/` to exist first; E-8 mounts on E-7's ChapterState). E-9/E-10 run in parallel. E-1..E-4 provide the playable substrate already. E-11's battle-layer items (N-1 report, N-2 dossier, N-3 rival deck) are landed; its campaign items follow E-7.

---

## Epic List (Implementation Order)

Approved 2026-09-18. User-value epics for remaining work; original
E-1..E-11 system map retained above for reference.

### Epic A: 治理之軸實裝 (Governance Axis Made Real)

In-battle 民心/秩序 sources wire up: village occupation, surrender
acceptance, convoy protection, atrocity penalties — plus
`Campaign/Governance` accumulators so values persist across battles.
**FRs covered:** FR5, FR6 (remainder)

### Epic B: 戰役持久骨架 (Campaign Spine)

`Campaign/` layer exists: `CampaignState` facade aggregates
**existing** Gameplay sub-stores (RefitCamp, Roster, CampaignLedger,
Governance…) rather than rebuilding them; `ChapterState`,
`potato.campaign/1` chapter-boundary saves (atomic, versioned),
`assets/campaign/` chapter defs, `Potato::Log` infra. The campaign
can be saved and continued across ~15 chapters.
**FRs covered:** FR11 + architecture-derived requirements
**Landed (parallel):** `Campaign/CampaignState` +
`Campaign/ChapterState` — facade aggregating RefitCamp/Roster/
CampaignLedger; AdvanceChapter syncs ledger + chapter state.

### Epic C: 敘事戰役系統 (Narrative Campaign Systems)

The ledger remembers across chapters: `IntelLedger` (lossy-ledger
判詞 provenance), `Dispositions`, `TitleTrack`, `GodStance`,
`NarrativePack` + `assets/narrative/` packs, chapter conventions
(題詞/判詞/欲知後事), `EndingPage` four-hand ending generator.
**FRs covered:** FR13 remainder, FR6 (ending side) — depends B

### Epic D: 神話雙層 (Myth Layer)

`Campaign/MythLayer` seepage state machine (0–3), shrine/spirit
entities on battle maps, pacify/provoke verbs, 天命 currency,
myth→governance conversion, seepage rendering consuming UITheme
tokens (UX-DR5).
**FRs covered:** FR10, UX-DR5 — depends B

### Epic E: 無戰章節 (No-Battle Chapters)

Designated chapters resolve by council phase: negotiation,
deterrence, or subversion retires the enemy commander without
combat; gated by campaign 民心/治績 + 天命. Highest-tier evaluation.
**FRs covered:** FR9 — depends B + C

### Epic F: 呈現層補完 (Presentation Completion)

Sprite/texture atlas pipeline (UX-DR9), audio milestone via vendored
miniaudio (UX-DR10), HUD density tiers (UX-DR11), libre font
bundling (UX-DR12), UI scale 75–150% real plumbing (UX-DR8),
chronicler typography completion (UX-DR4).
**FRs covered:** UX-DR4/8/9/10/11/12 — parallel

### Epic G: 內容與工具 (Content & Tools)

Doctrine card pool expansion, enemy commander deck authoring tool,
doctrine sandbox simulator (deck-vs-deck testbed), tutorial
scenario completion.
**FRs covered:** FR12, FR14 + E-10 remainder — parallel

### FR Coverage Map

- FR1–FR4, FR7, FR8, FR15: implemented (E-1..E-4, G-9)
- FR5, FR6: Epic A ✅
- FR6 (ending): Epic C ✅
- FR9: Epic E ✅（NoBattleResolver/Advantage/SubversionDefect 全鏈）
- FR10: Epic D ✅（機械層+採用層，D-6 playable 接線）
- FR11: Epic B ✅（機制；內容 2/15 章屬內容缺口）
- FR12: Epic G ✅
- FR13: Epic C ✅（C-2 NarrativePack/C-5 EndingPage 已落地）
- FR14: Epic G ✅（BattleSandbox d030375）
- UX-DR1~3/6/7: landed；UX-DR5: Epic D ✅；UX-DR8~11: Epic F 已落地
- UX-DR4/12: partial
- 註：下方 FR 表「Epic」欄為萃取時的舊編號（E-6=E pic D 等），
  逐條追溯見 `_bmad-output/test-artifacts/traceability/traceability-matrix.md`

---

## Requirements Coverage

Extracted 2026-09-18 from gdd.md + game-architecture.md + narrative-design.md + DESIGN.md.

### Functional Requirements

| FR | Requirement | Epic |
|---|---|---|
| FR1 | Doctrine cards (trigger→condition→action, cooldown, hand 4→7) | E-1 ✅ |
| FR2 | Real-time execution + CP interventions (~8s seizures, stratagems) | E-4 ✅ |
| FR3 | Probability-cloud fog (superposition/observe/probe/decay/entanglement/priors) | E-2 ✅ |
| FR4 | Morale→execution rate; broken squads disobey/defect | E-1 ✅ |
| FR5 | In-battle governance sources (village/surrender/convoy/shrine → 民心秩序; atrocity → 墮落) | E-5 ✅ |
| FR6 | Five-rung outcome ladder (SubdueWithoutWar…Fallen) | E-5 ✅ |
| FR7 | Named roster + casualty persistence + replay + historian report | E-4/G-9 ✅ |
| FR8 | Readable enemy personality decks (3 axes + signatures) | E-1/E-9 ✅ |
| FR9 | 無戰 chapters (negotiation/deterrence/subversion) | E-8 ✅ |
| FR10 | Myth layer (shrines/天命/conversion/incursions) | E-6 ✅ |
| FR11 | Campaign frame (~15 chapters, persistent accumulators, carry-over) | E-7 ✅ 機制（內容 2/15 章） |
| FR12 | Doctrine library looting (deck = tech tree) | E-9 ✅ |
| FR13 | Narrative systems (report/判詞/counter-deck/dispositions/myth log/conventions/4 endings) | E-11 ✅ |
| FR14 | Doctrine sandbox simulator | E-10 ✅ |
| FR15 | Persistent refit camp (deploy/heal/recruit/loot) | G-9 ✅ |

### Non-Functional Requirements

| NFR | Requirement |
|---|---|
| NFR1 | 60 FPS mid-range PC, ≤8 squads + fog clouds + replay logging |
| NFR2 | Headless determinism — every gameplay rule without GL (POTATO_TESTS) |
| NFR3 | Cross-compiler: MSVC + MinGW local, g++ CI |
| NFR4 | Banned C APIs (gets/strcpy/strcat/sprintf/vsprintf/scanf) |
| NFR5 | No third-party JSON; versioned `potato.*` schemas |
| NFR6 | Bilingual zh-TW/en; ~50k words; EN +30% expansion budget |
| NFR7 | Onboarding gate: non-programmer fields a battle ≤ 10 min |

### Architecture-Derived Requirements

- `Campaign/` layer; dependency `Gameplay ← Campaign` only
- `potato.campaign/1` + chapter-boundary atomic saves + schema migration
- `potato.narrative_pack/1` + `assets/narrative/` + `assets/campaign/`
- ReportAssembler → BattleNarrative single-interpretation fan-out
- IntelLedger lossy-ledger; MythLayer seepage 0–3; counter-deck via stats+pre-authored variants
- `Potato::Log` (Error/Warn/Info/Debug); Library+LoadDir; direct references
- `Campaign/EndingPage` — ending generated from record, 4 hands, no true ending

### UX Design Requirements (DESIGN.md)

| UX-DR | Requirement | Status |
|---|---|---|
| UX-DR1 | Token-driven 4-theme system (UITheme.h) | ✅ landed |
| UX-DR2 | Dual-encoding: shape+icon, never hue alone (minimap ■◆● landed) | ✅ partial |
| UX-DR3 | Contrast floor ≥4.5:1; gloss tokens never carry gameplay info | ✅ UIThemeTest |
| UX-DR4 | Chronicler register typography; fixed-advance numerics | ⚠️ partial |
| UX-DR5 | One anchor layout; myth seepage = full theme swap, bounded | ✅ landed（Epic D SeepageStage+UISettings） |
| UX-DR6 | Minimap shape-coding + WorldToScreen | ✅ landed |
| UX-DR7 | Vertical CJK for titles/seals only (VTextAt) | ✅ landed |
| UX-DR8 | UI scale 75–150% via ScaleAllSizes + dynamic atlas | ✅ landed（UIScaleTest） |
| UX-DR9 | Sprite/texture atlas pipeline (portraits/stamps/ink strokes) | ✅ landed（SpriteAtlasTest） |
| UX-DR10 | Audio milestone — OpenAL stub; miniaudio recommended; visual-first signals | ✅ landed（miniaudio + AudioCues） |
| UX-DR11 | HUD density tiers 極簡/標準/完整 | ✅ landed（HUDDensityTest） |
| UX-DR12 | Ship libre fonts (Noto Serif/Sans TC + mono) | ⚠️ font packaging exists |

---

## Epic A Stories — 治理之軸實裝

### Story A.1: 村莊佔領與民心事件

As a 玩家,
I want 完整佔領村莊與戰火波及村莊產生民心/秩序/墮落變化,
So that 地圖上的治理行為有真實後果.

**Acceptance Criteria:**

**Given** 戰鬥地圖含村莊節點（BattleMap village flag）
**When** 我方小隊進駐且村莊格未被交戰火力穿過
**Then** 民心 +N、錄製 governance 事件到 BattleRecorder
**And** 村莊格被火力穿過 → 秩序 -N + 墮落 +N（暴行）
**And** 村莊有視覺狀態位（intact/burned）供渲染層讀取
**And** 全 headless 可測

### Story A.2: 受降與倒戈記帳

As a 玩家,
I want 敵軍投降被接受計為倒戈、拒降後殲滅計為暴行,
So that 無戰路線有可量的正回饋.

**Acceptance Criteria:**

**Given** 敵小隊士氣歸零進入投降態
**When** 玩家選擇受降
**Then** 記為 defection、民心 +N、不計 annihilation
**And** 拒降後殲滅 → annihilation + 墮落 +N
**And** 投降選擇入 BattleRecorder（replay 可見）

### Story A.3: 護輜實體與路徑

As a 玩家,
I want 地圖上存在會移動的運輸隊實體,
So that 補給線成為可攻可守的目標.

**Acceptance Criteria:**

**Given** BattleMap 定義 convoy（起點/終點/路徑格）
**When** 戰鬥執行中
**Then** convoy 沿 flow-field 路徑向終點移動（不新增尋路碼）
**And** convoy 可被交戰（有 HP/可被劫）且事件入 Recorder

### Story A.4: 護輜結算與劫掠

As a 玩家,
I want 保護運輸隊抵達獲得秩序、被劫失秩序、劫敵輜重削敵士氣,
So that 經濟戰是可玩選項.

**Acceptance Criteria:**

**Given** A.3 的 convoy 實體存在
**When** 我方 convoy 全數抵達
**Then** 秩序 +N、錄製事件
**And** 我方 convoy 被劫/毀 → 秩序 -N
**And** 敵方 convoy 被劫 → 敵全軍士氣 -N

### Story A.5: 治理累計器（Gameplay 子存儲）

As a 玩家,
I want 民心/秩序/墮落跨戰鬥累計,
So that 單場勝負之外有長期治理軌跡.

**Acceptance Criteria:**

**Given** 戰鬥結算產生 governance delta
**When** Governance::Apply(delta) 被呼叫
**Then** 累計值更新、墮落只增不減（ratchet 單向，GDD 寫死無衰減）
**And** 低秩序門檻觸發 unrest 事件入佇列
**And** 位於 `Gameplay/Governance.*`——比照 CampaignLedger 先例：
       子存儲自管 ToJson/FromJson，日後被 CampaignState facade
       聚合（Epic B）而非現在依賴它
**And** bool-return 載入、壞 schema 拒絕（Step 5 錯誤規則）

---

## Epic B Stories — 戰役持久骨架

**Landed (parallel):** `Campaign` static lib 已建（`PotatoEngine ←
Gameplay ← Campaign` 連結方向正確）；`CampaignState` 聚合
RefitCamp/Roster/CampaignLedger/ChapterState；`CampaignStateTest`
已入 POTATO_TESTS；`DuanqiaoPlayable` 已改由 campaign 持有整補營。

### Story B.1: Potato::Log 日誌基建

As a 開發者,
I want 統一的分級日誌（Error/Warn/Info/Debug → stderr + 可選檔案）,
So that 新系統的降級/拒絕有一致記錄格式.

**Acceptance Criteria:**

**Given** 任一系統呼叫 Potato::Log::Warn/Error
**When** 訊息含 printf 風格格式化參數
**Then** 輸出到 stderr（可選檔案 sink via SetFile）
**And** Debug 級在 NDEBUG 下編譯期剔除
**And** 測試可捕獲 stderr 驗證 warning 發出（headless）

### Story B.2: 章節定義資料包

As a 設計者,
I want 章節/弧線以 assets/campaign/*.json 資料驅動,
So that 章回結構不用改碼就能調整.

**Acceptance Criteria:**

**Given** assets/campaign/ 下 potato.chapter/1 檔（arc/chapterId/
       標題/題詞id/無戰旗標/敵將id/地圖id）
**When** ChapterLibrary::LoadDir 掃描
**Then** 合法章節載入、壞檔跳過+warn（Library+LoadDir 模式）
**And** 弧內章節可亂序索引、弧序固定
**And** 15 章骨架資料檔至少含 arc 邊界與旗標位

### Story B.3: 存檔載入完整性

As a 玩家,
I want 戰役存檔能完整回復所有子系統狀態,
So that 關掉遊戲再開進度不丟.

**Acceptance Criteria:**

**Given** CampaignState.SaveToFile 已寫出 potato.campaign/1
**When** LoadFromFile 讀回
**Then** RefitCamp/Roster/CampaignLedger/ChapterState 全還原
**And** 壞 JSON/錯 schema/未知 version → false + 不動現況
       （先驗證再置換）
**And** 缺子段（未實作系統）→ 預設值不報錯（前向相容）
**And** 寫入走 tmp+rename 原子路徑

### Story B.4: 章節邊界存檔掛鉤

As a 玩家,
I want 章節結束時自動存檔、戰鬥中不存,
So that 存檔粒度與章回體裁一致.

**Acceptance Criteria:**

**Given** 一場戰鬥 aftermath 結算完成
**When** AdvanceChapter 被呼叫
**Then** 帳本章節序 + ChapterState 同步推進、觸發存檔
**And** 戰鬥執行中呼叫 Save 被拒絕（無 mid-battle save）
**And** 存檔失敗 → loudly 記錄 + 回 false，不崩潰

### Story B.5: 章節地圖殼（消費端最小 UI）

As a 玩家,
I want 章節之間看到戰役地圖/章節選擇畫面,
So that 戰鬥之外有戰役推進感.

**Acceptance Criteria:**

**Given** ChapterLibrary + CampaignState 存在
**When** 章節邊界畫面開啟
**Then** 顯示當前弧、已收戰線、可選章節（弧內自由序）
**And** 選章節 → 讀 chapter 定義 → 進部署流程
**And** UI 唯讀消費 CampaignState，不寫入（架構邊界）
**And** 極簡實作即可——ImGui 清單式，地圖美術屬 Epic F

---

## Epic C Stories — 敘事戰役系統

**Note:** Dispositions/TitleTrack 已由 `Gameplay/CampaignLedger`
覆蓋（N-4 landed）；`Gameplay/MythLog`（N-5）進行中——Epic C
補齊的是判詞持久帳、內容包、章回體例、神明態度與結局生成。

### Story C.1: IntelLedger 判詞持久帳（失真帳本）

As a 玩家,
I want 敵將判詞跨章節累積出處、可被觀測驗證或推翻,
So that 情報是一張會失真的地圖而不是標籤.

**Acceptance Criteria:**

**Given** Gameplay/IntelLedger 存在（比照 CampaignLedger 子存儲模式）
**When** 寫入條目 {date, source∈scout/fox/prisoner/planted,
       claim, bias_direction, verified, planted}
**Then** 顯示判詞由最新未被駁斥條目推導
**And** 觀測寫 verified 條目可駁斥先前傳聞；狡詐敵將可寫
       planted 條目（偽裝成 scout）
**And** 全被駁 → 顯示「不詳」；ToJson/FromJson 持久化
**And** GeneralDossier 改讀 IntelLedger（戰鬥視圖消費持久帳）

### Story C.2: NarrativePack 內容包

As a 寫手,
I want 戰報碎片/判詞/題詞/籤詩以 assets/narrative/*.json 資料驅動,
So that 文本改動不需要重新編譯.

**Acceptance Criteria:**

**Given** assets/narrative/ 下 potato.narrative_pack/1 檔
**When** NarrativePack::LoadDir 掃描
**Then** 條目以 {id, zh-TW, en} locale-keyed 載入、壞檔跳過+warn
**And** 碎片帶 register 軸（historian/fox/human/rival_hand）
       與事件型別標籤
**And** HistorianReport 改從 pack 取碎片（組裝器消費內容 id，
       不內嵌字串）
**And** 查無 id → 回退預設碎片 + warn，不崩潰

### Story C.3: 章回體例（題詞/判詞/欲知後事）

As a 玩家,
I want 每章開頭有題詞、敵將出場有判詞、章末有懸念句,
So that 戰役讀起來像章回傳記.

**Acceptance Criteria:**

**Given** 章節定義含題詞 id + 敵將 id
**When** 章節開始
**Then** 題詞自 NarrativePack 渲染；敵將出場顯示
       IntelLedger 推導的判詞
**And** 章節結束產生「欲知後事」收尾句（依戰果選碎片）
**And** 全部經帳本事件記錄（record-is-truth：顯示過=有記錄）

### Story C.4: GodStance 神明態度表

As a 玩家,
I want 各神對我的態度隨治理與神蹟行為累積,
So that 神話層記得我做了什麼.

**Acceptance Criteria:**

**Given** Gameplay/GodStance 存在（per-god favor 表）
**When** 神蹟相關事件發生（安撫/褻瀆/試煉通過）
**Then** 對應神 favor 增減、事件入 MythLog
**And** favor 影響天命可用度與無戰門檻（供 Epic D/E 讀取）
**And** ToJson/FromJson 持久化；CampaignState 聚合段就位

### Story C.5: EndingPage 四手結局生成

As a 玩家,
I want 結局由累積帳目生成而非選單選擇,
So that 最後一頁是我全程行為的判決.

**Acceptance Criteria:**

**Given** 戰役結束時 CampaignState 全量帳目可用
**When** EndingPage::Generate 執行
**Then** 依征服/治理指標+墮落棘輪選四手之一
       （霸業/仁政/無字/對手之筆）
**And** 征服與民心雙高矛盾 → 無字（空白頁）
**And** 墮落滿 → 對手之筆（register 換 rival_hand 重寫）
**And** 結局引用 EndingCitations（敵將去向/稱號/神蹟證詞）
       ——全部從帳目取，無真結局標記

---

## Epic D Stories — 神話雙層

### Story D.1: MythLayer 滲透狀態機

As a 玩家,
I want 地圖區域有神話滲透等級隨事件推進,
So that 神異是漸進降臨而非開關.

**Acceptance Criteria:**

**Given** Campaign/MythLayer（或 Gameplay 層的戰鬥視圖）存在
**When** 治理/屠殺密度/神明態度跨閾值
**Then** 區域滲透等級推進 0 quiet → 1 anomalies → 2 seep →
       3 manifest，轉換錄事件
**And** 章內等級只升不降（土地不會在戰中遺忘）
**And** 跨章從 CampaignState 重新推導；全 headless

### Story D.2: 神社與精靈實體

As a 玩家,
I want 戰場上有可互動的神社/精靈節點,
So that 神話層是可玩的第二張地圖.

**Acceptance Criteria:**

**Given** BattleMap 定義 shrine 節點（位置/神祇 id/供品態）
**When** 小隊進駐神社格
**Then** 觸發互動選項（安撫/挑釁/獻祭），選擇入 Recorder
**And** 神社狀態寫入 GodStance favor（C.4 介面）
**And** 神社有視覺狀態供渲染層（供品桌追蹤民心——環境敘事）

### Story D.3: 天命貨幣與轉換

As a 玩家,
I want 安撫神明累積天命並可兌換民心/秩序,
So that 神話行為機械性放大治理軸.

**Acceptance Criteria:**

**Given** GodStance favor ≥ 門檻的神祇存在
**When** 玩家發動天命兌換（章節邊界或戰鬥中有限次）
**Then** 民心/秩序 +N、favor 扣除、事件入帳
**And** favor 不足 → 選項灰顯不可點（不是隱藏——可見的門檻）

### Story D.4: 神話入侵事件

As a 玩家,
I want 指定戰鬥被神話事件打斷（鬼軍夜行/狐仙假訊）,
So that 戰場不只屬於人.

**Acceptance Criteria:**

**Given** 章節定義或滲透等級標記入侵條件
**When** 條件達成（mid-battle insert——稀有，保留給意義改變者）
**Then** 入侵事件觸發：鬼軍單位出現或假情報注入，
       戰鬥短暫暫停提示
**And** 安撫成功 → 治理加分；忽視 → 滲透等級推進
**And** 事件入 MythLog（具名：何祠何靈何時）

### Story D.5: 滲透視效消費端（UX-DR5）

As a 玩家,
I want 滲透等級改變時 HUD 做漸進式主題轉換,
So that 我看見世界層在滑動.

**Acceptance Criteria:**

**Given** MythLayer 滲透等級改變
**When** 渲染層讀到新等級
**Then** 等級 1 → 局部異常（霧色偏移/陰影錯位）；
       等級 2 → 神話音景先於視覺+物件微移；
       等級 3 → 全主題切換（UITheme token swap + scrim 過渡）
**And** 轉換期間士氣崩潰/倒數等關鍵資訊保持雙編碼可讀
       （DESIGN 禁令）
**And** motion-reduction 設定 → 降為 crossfade
**And** 渲染層唯讀——不寫滲透狀態（架構邊界）

---

## Epic E Stories — 無戰章節

### Story E.1: 無戰章節定義與門檻

As a 玩家,
I want 指定章節可以不戰而勝,
So that 至聖者的「無戰」是機械事實而非彩蛋.

**Acceptance Criteria:**

**Given** chapter 定義含 noBattle flag + 無戰選項集
**When** 章節開始且無戰選項可用
**Then** 玩家可選談判/嚇阻/顛覆路徑取代部署
**And** 每個選項顯示其門檻條件（民心/favor/情報值）——
       不滿足時灰顯可見（不隱藏）
**And** 選擇無戰 → 跳過戰鬥狀態機直接進結算

### Story E.2: 談判與嚇阻決算

As a 玩家,
I want 談判成功以民心換和平、嚇阻以軍威換退兵,
So that 不同的無戰路徑有不同代價結構.

**Acceptance Criteria:**

**Given** 玩家選擇無戰路徑
**When** 門檻判定執行
**Then** 談判成功 → 民心消費、敵將記 disposition:negotiated；
       嚇阻成功 → 依累計軍威判定、記 intimidated
**And** 失敗 → 章節進入戰鬥且敵方獲首波優勢（代價明確）
**And** 判定結果入帳本（Record-is-Truth）

### Story E.3: 顛覆與內應

As a 玩家,
I want 長期情報經營可轉化為敵營內應,
So that 判詞帳本是我能動用的資產.

**Acceptance Criteria:**

**Given** IntelLedger 對該敵將累積 ≥N 條 verified 條目
**When** 玩家選擇顛覆路徑
**Then** 情報消費、敵軍部分小隊開場即倒戈
**And** 若帳本含未駁斥的 planted 條目 → 顛覆反被利用
       （敵設局，我方獲開場劣勢）
**And** 失敗路徑同樣入帳——假情報的代價是機械性的

### Story E.4: 無戰章節的敘事與結算

As a 玩家,
I want 無戰章節有完整的章回結算（判詞/戰報/史官記錄）,
So that 不戰的章節同樣被書寫而非留白.

**Acceptance Criteria:**

**Given** 無戰路徑完成（E.2/E.3 任一成功）
**When** 章節結算執行
**Then** HistorianReport 產生無戰戰報（register 選和平語域）、
       欲知後事收尾、AdvanceChapter 存檔正常
**And** 敵將去向/稱號入 CampaignLedger（收服者不列陣亡）
**And** 結局階梯把無戰章節計入 GovernedPeace 軌跡

---

## Epic F Stories — 呈現層補完

### Story F.1: Sprite Atlas 管線（最大共享依賴）

As a 開發者,
I want 單位圖示/村莊/神社經過 sprite atlas 載入而非逐圖上傳,
So that UX-DR8 的最大共享依賴先落地、視覺工作全解鎖.

**Acceptance Criteria:**

**Given** assets/ 下 sprite 圖集 + 索引檔（frame name → uv rect）
**When** 渲染層載入 atlas
**Then** 各 entity 依 frame name 取 uv、單一 texture bind
**And** 索引檔走 JsonValue（不引新格式）；缺 frame → 預設
       圖塊 + warn
**And** headless 可驗索引解析（uv 查表無 GL 依賴）

### Story F.2: UI Scale 真管線（75–150%）

As a 玩家,
I want UI scale 設定實際改變所有版面而不只是縮字,
So that 75–150% 範圍內排版不壞.

**Acceptance Criteria:**

**Given** potato.settings/1 存 uiScale ∈ [0.75, 1.5]
**When** 玩家調整 scale
**Then** UITheme spacing/字級/panel 尺寸全經 scale 因子、
       錨點位置不動（UX-DR4 禁令）
**And** 邊界值 0.75/1.5 排版不破框；設定即時生效免重啟
**And** motion-reduction 與 scale 無交互 bug

### Story F.3: HUD 密度三檔

As a 玩家,
I want HUD 資訊密度可切換（精簡/標準/詳盡）,
So that 新手不被淹沒、老手不缺資訊.

**Acceptance Criteria:**

**Given** 設定含 hudDensity ∈ {minimal, standard, verbose}
**When** 切換檔位
**Then** minimal 僅剩士氣/CP/目標；standard 加 doctrine 牌與
       小地圖；verbose 加霧機率/執行率/帳本計數
**And** 三檔的關鍵資訊（士氣崩潰/倒數）永遠在場——
       密度不裁撤必要編碼
**And** 設定存檔持久；切換不重啟

### Story F.4: Audio 里程碑（miniaudio）

As a 玩家,
I want 神話音景、UI 回饋、戰鬥提示有音訊層,
So that 滲透的「聲先於形」成立且操作有聽覺回饋.

**Acceptance Criteria:**

**Given** miniaudio 以新增子目錄 vendored（external/miniaudio/
       + README 記錄來源/版本/license）
**When** 事件觸發音效（doctrine 觸發/士氣崩潰/滲透升級/
       UI 點擊）
**Then** 對應音檔播放；滲透 2 級音景循環
**And** 音檔缺失 → 靜音降級 + warn，不崩潰
**And** 音量/靜音入 settings；audio 初始化失敗 → 遊戲照跑

### Story F.5: Libre 字體打包與 CJK 完整性

As a 玩家,
I want 遊戲內建字體不依賴系統安裝,
So that 任何機器上中文渲染一致.

**Acceptance Criteria:**

**Given** assets/fonts/ 打包 libre CJK 字體（license 檔同捆）
**When** 任一字串渲染（含直書標題）
**Then** 全部經內建字體、無系統字體 fallback 依賴
**And** chronicler 字級規範生效（UX-DR3）
**And** 字體載入失敗 → 邊界框降級 + Error 記錄

---

## Epic G Stories — 內容與工具

### Story G.1: Doctrine 卡池擴充

As a 玩家,
I want 卡池從原型數量擴到完整 set（含無戰/神話軸卡）,
So that doctrine 寫作是真正的創意動詞.

**Acceptance Criteria:**

**Given** assets/doctrine/ 下擴充卡檔（potato.doctrine/1 既有 schema）
**When** DoctrineLibrary::LoadDir 掃描
**Then** 全卡載入驗證——trigger/condition/action/modifier 欄位
       齊全、冷卻與 cost 合理
**And** 含無戰軸卡（受降加成/談判籌碼）與神話軸卡（安撫/
       天命連動），與 E/D epic 的掛鉤點有測試覆蓋
**And** 壞卡跳過+warn 不影響其他卡

### Story G.2: 牌庫掠奪（deck=科技樹）

As a 玩家,
I want 擊敗敵將可獲得其 signature 卡,
So that 牌庫成長來自征服而非商店.

**Acceptance Criteria:**

**Given** 敵將定義含 signatureDoctrine 卡 id
**When** 敵將被擊敗/收服（disposition 記錄存在）
**Then** 其 signature 卡解鎖進玩家卡池、事件入帳
**And** 已擁有 → 轉化為強化版或資源（不重複）
**And** 無戰收服的敵將同樣給卡——不戰不懲罰收集

### Story G.3: 敵將編輯器（內容工具）

As a 設計者,
I want 敵將人格牌組以資料+工具編輯,
So that 新敵將不需要程式介入.

**Acceptance Criteria:**

**Given** assets/cards/ 既有 potato.character_card/1 schema
**When** 編輯器工具（ImGui 面板或 CLI 驗證器）載入卡片
**Then** 三軸人格/signature/對話碎片可編輯並驗證存回
**And** 驗證器獨立 headless 可跑（CI 可掃全卡目錄）
**And** 非法組合（如 unknown doctrine id）→ 拒存 + 明確錯誤

### Story G.4: 沙盤模擬器（FR14 平衡工具）

As a 設計者,
I want deck-vs-deck 批次模擬工具,
So that 平衡調整有數據依據.

**Acceptance Criteria:**

**Given** 兩副 doctrine 牌組 + 地圖 + 敵將定義
**When** 模擬器跑 N 場無頭對局
**Then** 輸出勝率/均時長/卡觸發率統計（JSON 或 stdout）
**And** 種子固定 → 結果可重現（CI 可用）
**And** 全程無 GL 依賴、走既有 BattleController 管線

### Story G.5: 教學章節（上手門檻 ≤10min）

As a 新玩家,
I want 第一章節引導我寫出第一張 doctrine 卡並看見它執行,
So that 核心迴圈在十分鐘內成立.

**Acceptance Criteria:**

**Given** chapter_01 定義為教學章（tutor flag + 引導步驟）
**When** 新玩家進入
**Then** 逐步引導：選卡→部署→看 doctrine 觸發→CP 介入一次
**And** 引導文字自 NarrativePack 取（不入碼）
**And** 可跳過；完成後進正常章節流程
**And** headless 可驗引導步驟序列完整性
