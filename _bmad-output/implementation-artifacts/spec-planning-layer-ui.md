# A-1 Spec：T-9 回合層 UI（卡槽欄 + 卡編輯器 + 地圖圖釘 + 起手牌組）

> status: done — PlanningDeck + PlanningDeckTest(30 PASS) + DuanqiaoPlayable
> Planning phase UI。MSVC/MinGW 皆編譯，ctest 全綠。

## 目標

DuanqiaoPlayable 目前 `ApplyPlan` 後直接 `BeginExecution`——玩家沒有
「戰前寫 doctrine」的回合層。本項加入 **Planning phase**：
開戰前玩家檢視/編輯每隊的 doctrine 卡組、在地圖上調整
objective/rally 圖釘、一鍵載入 AI 參謀起手牌組，按「開戰」才
進入即時層。

## 設計

### 邏輯層（headless 可測）

新增 `Gameplay/PlanningDeck.h/.cpp`：

- 持有 `std::vector<SquadSlots>`：`{ Squad* squad; DoctrineSet doctrine; }`
- `Init(battle, team)`：從 BattleController 取隊伍，讀回已指派 doctrine
- 操作：`AddRule / RemoveRule / SetRule / SwapRules`（priority 拖放排序）、
  `SlotCap()`（預設 7，4→7 成長掛點）
- `Validate()`：回傳警告清單（如：無 Always 墊底規則、空卡組）
- `Commit(battle)`：逐隊 `AssignDoctrine`
- 模板：`LoadPlannerTemplate(planner, battle)` → 填 `GeneratePlan` 結果
  （保留 rationale 供 UI 顯示）

`DoctrineSet` 補唯讀存取：`const std::vector<DoctrineRule>& Rules() const`、
`RemoveRule(index)`（編輯需要，現只進不出）。

### 表現層（DuanqiaoPlayable，ImGui）

- 新增 `Phase::Planning / Phase::Execution`；開場進 Planning
  （場景照常渲染 + 相機可操作，battle 不 tick、不顯示 CP HUD）
- Planning 視窗：
  - 左欄：我軍小隊清單（點選切換編輯對象，顯示槽位用量 n/7）
  - 中欄：卡槽清單——每列 `priority | trigger(條件+threshold) → action`，
    支援新增（若未滿）、刪除、上移/下移
  - 右欄：選中卡編輯器——trigger 下拉（`TriggerName` 全表）、
    action 下拉（`ActionName` 全表）、threshold DragFloat、
    priority InputInt、cooldown DragFloat
  - 底部：「AI 參謀規劃」（載入模板 + 顯示 posture/summary/rationale）、
    Validate 警告、「開戰」鈕 → `Commit` + `BeginExecution` → 轉 Execution
- 地圖圖釘：Planning 中 Alt+左鍵地面 = 移動 objective 圖釘、
  Alt+右鍵 = 移動 rally 圖釘（各一顆，拖曳即 `SetObjective/SetRallyPoint`），
  場景以小柱 mesh 標示

### 範圍外

- 卡槽 4→7 的成長解鎖（僅留 `SlotCap` 掛點）
- 卡面美術/卡背（先用文字列）
- 多人/敵方規劃檢視
- 存檔牌組到 JSON（後續項目）

## 驗收

- `PlanningDeckTest`（新 headless 測試，入 POTATO_TESTS）：
  add/remove/swap/validate/commit 語義 + 槽位上限 + 模板載入
- MSVC + MinGW 編譯 0 錯誤；既有 ctest 無回歸
- DuanqiaoPlayable 開場進 Planning；開戰後行為與現況一致
  （手動驗證；headless 環境仍 [SKIP]）
- 無 banned C 函式；ImGui 字串處理走既有 bounded-copy 慣例

## 規模

L~XL：新檔 PlanningDeck + 測試 + DuanqiaoPlayable 約 +350 行 UI。
