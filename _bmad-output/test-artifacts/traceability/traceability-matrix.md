---
stepsCompleted: ['step-01-load-context', 'step-02-discover-tests', 'map-criteria', 'gate-decision']
lastStep: 'gate-decision'
lastSaved: '2026-09-26'
coverageBasis: 'acceptance_criteria'
oracleConfidence: 'high'
oracleResolutionMode: 'formal_requirements'
oracleSources:
  - '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '_bmad-output/implementation-artifacts/auto-dev-queue.yaml'
  - '_bmad-output/implementation-artifacts/sprint-status.yaml'
externalPointerStatus: 'not_used'
collectionStatus: 'COLLECTED'
sourceSha: 'f8600a5'
---

# 需求追溯矩陣 — MingGoRTS @ f8600a5

Oracle：epics.md 的 FR1–FR15 + NFR1–7 + UX-DR1–12（正式需求，
信心 high——GDD/架構/敘事文件萃取的需求表，逐條有 epic 映射）。
測試清單：CMakeLists.txt `POTATO_TESTS` 107 個無頭測試 +
services/ Maven 測試（CI services-test job）。

## FR → 實作 → 測試證據

| FR | 需求 | 實作 | 測試證據 | 覆蓋 |
|---|---|---|---|---|
| FR1 | Doctrine 卡 trigger→condition→action、cooldown、手牌 4→7 | `Gameplay/Doctrine`、`PlanningDeck`、`DoctrineLibrary` | `DoctrineBattleDemo`、`PlanningDeckTest`、`DoctrineLibraryTest` | ✅ |
| FR2 | 即時執行 + CP 介入（~8s 癲癇窗口、計略） | `BattleController::Intervene`、`SageCommand` | `DoctrineBattleDemo`、`DuanqiaoDemo`、`SageCommandTest` | ✅ |
| FR3 | 機率雲霧（疊加/觀測/探測/衰減/糾纏/先驗/相位） | `QuantumFog` | `QuantumTest`、`QuasiRandomTest`、`QuasiModelsTest`、`QuantumFogBattleTest`、`QuantumPlanTest`、`QuantumBench` | ✅ |
| FR4 | 士氣→執行率；潰隊違令/倒戈 | `Squad`（morale/execution/stamina）、`SubversionDefect` | `SquadFatigueTest`、`DoctrineBattleDemo`、`SubversionDefectTest` | ✅ |
| FR5 | 戰場治理源（佔村/受降/護輜/神社→民心秩序；暴行→墮落） | `GovernanceField`、`Governance`、`ShrineField`、`BattleController::RecordGovernanceEvent` | `GovernanceFieldTest`（[7][8] 階段閘+radius）、`GovernanceTest`、`ShrineFieldTest` | ✅ |
| FR6 | 五階結局梯子（SubdueWithoutWar…Fallen） | `SageCommand` 判級、`ChapterConventions`、`EndingPage` | `SageCommandTest`、`ChapterConventionsTest`、`EndingPageTest` | ✅ |
| FR7 | 名冊 + 傷亡持久 + 回放 + 史官戰報 | `Roster`、`BattleRecorder`（rootHash）、`HistorianReport` | `HistorianReportTest`、`ReplayIntegrityTest`、`LedgerForgeTest`、`LedgerChainTest` | ✅ |
| FR8 | 可讀敵將人格牌組（三軸+簽名） | `EnemyGeneral`、`GeneralDossier` | `GeneralDossierTest`、`CardValidatorTest`、`CardEditorTest` | ✅ |
| FR9 | 無戰章節（談判/嚇阻/顛覆） | `NoBattleResolver`、`NoBattleAdvantage`、`SubversionDefect` + playable 接線 | `NoBattleResolverTest`、`NoBattleNarrativeTest`、`NoBattleAdvantageTest`、`SubversionDefectTest` | ✅ |
| FR10 | 神話層（神社/天命/兌換/入侵） | `MythLayer`、`ShrineField`、`FateExchange`、`MythIncursion`、`SeepageStage`、`MythLog` | `MythLayerTest`、`ShrineFieldTest`、`FateExchangeTest`、`MythIncursionTest`、`SeepageStageTest`、`MythLogTest` | ✅ |
| FR11 | 戰役骨架（~15 章、持久累加器、carry-over） | `CampaignState`、`ChapterLibrary`、`Governance`、`PeaceHazard` | `CampaignStateTest`、`ChapterLibraryTest`、`PeaceHazardTest` | ⚠️ 機制齊、內容 2/15 章 |
| FR12 | doctrine 掠奪（牌庫=科技樹） | `PlayerDeck`、`EnemyDeck`、`DoctrineLibrary` | `PlayerDeckTest`、`EnemyDeckTest`、`DoctrineLibraryTest` | ✅ |
| FR13 | 敘事系統（戰報/判詞/反制牌/處置/神蹟錄/慣例/四結局） | `HistorianReport`、`GeneralDossier`、`MythLog`、`ChapterConventions`、`NarrativePack`、`EndingPage` | `NarrativePackTest`、`EndingPageTest`+上列各測試 | ✅ |
| FR14 | doctrine 沙盤模擬器 | `tools/` BattleSandbox（deck-vs-deck seeded） | `BattleSandboxTest` | ✅ |
| FR15 | 持久整補營（部署/醫治/募兵/掠奪） | `RefitCamp` | `RefitCampTest` | ✅ |

## NFR 覆蓋

| NFR | 需求 | 證據 | 覆蓋 |
|---|---|---|---|
| NFR1 | 60 FPS（≤8 隊 + 霧雲 + 回放記錄） | `QuantumBench` 只量霧雲；**無整幀 frame-time 預算測試** | ⚠️ 缺口 |
| NFR2 | 無頭確定性——所有玩法規則無 GL 可測 | 107 個 POTATO_TESTS 全無頭 | ✅ |
| NFR3 | 跨編譯器 MSVC+MinGW+Linux g++ | 本機雙工具鏈 + CI | ✅ |
| NFR4 | banned C API | CI security-scan + 本機掃描慣例 | ✅ |
| NFR5 | 無第三方 JSON；版本化 `potato.*` schema | 10+ schema 落地，缺 tag 容忍/錯 tag 拒絕有測試 | ✅ |
| NFR6 | zh-TW/en 雙語 ~5 萬字 | `potato.narrative_pack/1` 雙 locale 機制齊（NarrativePackTest）；**字數量級無稽核** | ⚠️ 缺口 |
| NFR7 | 新手 ≤10min 成局 | `TutorialFlow` + `tutorial_01.json` + `TutorialFlowTest` 無頭步驟序列驗證；**真機計時屬人工驗證（V-2）** | ⚠️ 半自動 |

## UX-DR 覆蓋（對 epics.md 表的現況修正）

| UX-DR | 需求 | 現況 | 覆蓋 |
|---|---|---|---|
| DR1 | token 四主題 | UITheme.h 落地 | ✅ `UIThemeTest` |
| DR2 | 形狀+圖示雙編碼 | 小地圖 ■◆● | ✅ |
| DR3 | 對比 ≥4.5:1 | token 層 | ✅ `UIThemeTest` |
| DR4 | 記錄體排印 | chronicler register 部分 | ⚠️ |
| DR5 | 滲透全域換主題（有界） | `SeepageStage`+`UISettings` 落地（Epic D） | ✅ `SeepageStageTest` |
| DR6 | 小地圖形狀碼 + WorldToScreen | 落地 | ✅ |
| DR7 | 直排 CJK 限標題/印 | `VTextAt` | ✅ |
| DR8 | UI 縮放 75–150% | slider + `ScaleAllSizes` | ✅ `UIScaleTest` |
| DR9 | 圖集管線 | `SpriteAtlas` | ✅ `SpriteAtlasTest` |
| DR10 | 音訊里程碑 | miniaudio 後端 + `AudioCues` | ✅ `AudioTest`、`MiniaudioBackendTest` |
| DR11 | HUD 密度三檔 | `HUDDensityUI` | ✅ `HUDDensityTest` |
| DR12 | 自由字型打包 | Noto 字型包裝存在 | ⚠️ 部分 |

## 覆蓋缺口（gate 關注項）

1. **NFR1 幀預算無自動證據**——`QuantumBench` 只基準化霧雲；整幀
   16.6ms 預算（8 隊+霧+回放）無 headless 計時測。建議：
   `BattleBenchTest` 跑固定 tick 數計 wall time（粗上限即可）。
2. **FR11 內容量缺口**——戰役骨架機制全綠，但 `assets/campaign/`
   僅 duanqiao + tutorial_01 兩章，距 ~15 章的內容目標尚遠。
   屬內容工作非機制缺口。
3. **NFR6 字數無稽核**——NarrativePack 雙語機制齊，5 萬字量級
   無自動計數。可加 `NarrativePackTest` 字數下界斷言或 CI 步驟。
4. **NFR7 ≤10min 需真人**——引導步驟序列有無頭驗證，但「十分
   鐘」是體感量測，屬 V-2 人工驗證範疇。
5. **名實對應斷鏈已補但靠註記**——c-1→`GeneralDossier`、
   c-4→`MythLayer` favor 的 epic↔程式映射目前只在
   sprint-status 註解；epics.md 的 FR→story 對應（E-8/E-6/E-7/
   E-9/E-10/E-11 舊編號）與現行字母 epic 有世代差，建議在
   epics.md 補新版 FR→story 對照段。

## Gate 決策：**PASS with CONCERNS**

- 15/15 FR 皆有實作 + 無頭測試證據；7 個 epic 全 done
- 三個 NFR 缺口皆屬「量測/內容」類而非「機制缺失」——無 P0/P1
  阻斷項
- epics.md 狀態欄嚴重過期（E-8❌/E-6❌/E-7❌ 皆已落地）——
  本矩陣即為對齊後的真相來源；建議回填 epics.md 或改指向
  sprint-status.yaml

## Live Verification Results

```json
{
  "present": false,
  "results_file": "",
  "note": "未配置 live_results_input——collection_mode 非 runtime_manifest",
  "current_source_sha": "f8600a5"
}
```
