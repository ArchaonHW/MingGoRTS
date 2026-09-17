---
title: 'E4-3 斷橋可玩 demo（點選→移動→交戰）+ A-4 膠囊體單位'
type: 'feature'
created: '2026-09-17'
status: 'in-review'
baseline_commit: '6e5d8e4'
context:
  - AGENTS.md
  - _bmad-output/planning-artifacts/potato-engine-roadmap.md
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** 斷橋原型 T-1~T-8 玩法層完整、BattleRenderDemo 已能把戰鬥渲染成 MP4，但全程無人可介入——「寫 doctrine → 看戰場 → CP 救火」的核心循環缺最後一塊：玩家能即時點選、下令的圖形介面（roadmap E4-3）。

**Approach:** 新增 `DuanqiaoPlayable` 視窗程式：OpenGLRenderer 開窗 + SceneRenderer 畫場景 + ImGui HUD，載入 `assets/maps/duanqiao.json` 真實地圖；滑鼠經 BattlePicker 選取我軍小隊、右鍵地面/敵軍下達 CP 介入指令；單位視覺升級為膠囊體（A-4），`BattleSceneSync` 加選取環與血條 overlay 收尾 E4-1。

## Boundaries & Constraints

**Always:**
- 無顯示環境下程式印出 `[SKIP]` 並 `return 0`（比照 GLSmokeTest），不 crash
- 戰場地形視覺由 `BattleMap`（zones/fords/pins/interactables）+ `FlowField::IsBlocked` 推得，不另寫地圖資料
- 小隊操作只走 `BattleController::Intervene`（CP 機制不繞過）；doctrine/數值邏輯不動
- 沿用既有模式：imgui 源碼內嵌（比照 SageCommandDemo/QuantumFogDemo target）、glad+glfw、Gameplay lib
- MSVC + MinGW 皆可編譯；無 banned C 函式；不動 `external/`、`build/`

**Ask First:**
- 新增 `Intervene` 以外的小隊控制手段（如直接 IssueOrder 無視 CP）
- 改動 doctrine 評估、士氣、傷害等模擬邏輯

**Never:**
- 不做 T-9~T-11：無 doctrine 編輯器、無回合層卡槽 UI、無戰報/回放時間軸 UI
- 不做 per-member 單位、不做網路/存檔功能
- 不進 `POTATO_TESTS`（互動視窗程式，CI 無顯示環境）
- 不碰 `assets/cards/art_vrm/`（平行工作產出中）

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 選取 | Execution 中 LMB 點我軍小隊 | 該隊成為 selected，腳下顯示選取環 | 點到敵軍/空地 → 不選取（或取消選取） |
| 下令移動 | 已選取 + RMB 點地面 | `Intervene(AttackMove, cell)`；CP-1，小隊朝該點移動 | CP=0 → Intervene 回 false，HUD 訊息流提示「CP 不足」 |
| 下令追擊 | 已選取 + RMB 點敵軍小隊 | `Intervene(Engage, squad)`；CP-1，小隊追擊該敵 | 同上 |
| 潰逃/陣亡 | 小隊 routing 或 eliminated | 血條消失/單位消隱（既有 sync 行為）；被選中的潰逃單位下令無效（Intervene 內部拒絕 routing） | 顯示事件訊息即可 |
| 暫停 | Space | timeScale 0 ↔ 1 切換；1/2/3 鍵 = 0.5/1.0/2.0 倍速 | N/A |
| 結算 | outcome != Ongoing | HUD 顯示勝/敗/和 banner；指令輸入停用，畫面繼續渲染 | N/A |
| HUD 遮蔽 | 滑鼠在 ImGui 視窗上 | `io.WantCaptureMouse` 時不觸發世界點選 | N/A |
| Headless | 無 DISPLAY/GLFW init 失敗 | stderr 提示 + `[SKIP]` + exit 0 | N/A |

</frozen-after-approval>

## Code Map

- `Gameplay/BattleSceneSync.h/.cpp` — E4-1 收尾：加 `SetSelectedSquad`/`SetOverlayMeshes`，Sync 更新選取環位置與每隊血條（子節點，scale.x=healthPct，綠→紅漸變）
- `Gameplay/BattlePicker.h` — 既有：`ScreenToWorldRay`/`IntersectGround`/`PickSquad`/`WorldToCell`
- `Gameplay/BattleController.h` — `Intervene`/`SetTimeScale`/`GetCommandPoints`/`GetPhase`/`GetOutcome`
- `Gameplay/BattleMap.h` — 地圖載入與 zones/fords/pins/interactables
- `Gameplay/EnemyGeneral.h` — `MakeGlock()` + `ApplyTo` 敵方 doctrine
- `Gameplay/BattlePlanner.h` — 我軍預設 doctrine（不操作時仍自動應戰）
- `Examples/BattleRenderDemo.cpp` — 渲染管線範本（OpenGLRenderer→SceneRenderer→lambert shader→靜態地形盒）
- `Examples/SageCommandDemo.cpp` — raw GLFW + ImGui 視窗迴圈範本
- `Examples/BattleSceneTest.cpp` — headless 測試，擴充 overlay 斷言
- `Examples/DuanqiaoDemo.cpp` — 斷橋編成/部署設定範本

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/BattleSceneSync.h/.cpp` -- 加 `SetSelectedSquad(const Squad*)`、`SetOverlayMeshes(ring, barBg, barFill)`；Sync 每幀更新選取環世界位置（跟隨 selected，未選隱藏）與每隊血條子節點（y≈2.2 平躺細盒，scale.x=healthPct，顏色 lerp 綠→紅；潰逃/全滅隱藏） -- E4-1 收尾 + A-4 選取框/血條
- [x] `Examples/BattleSceneTest.cpp` -- 加斷言：Attach 後每 binding 有血條子節點、SetSelectedSquad 後環節點可見且位置跟隨、全滅小隊血條隱藏 -- headless 驗證 overlay 邏輯
- [x] `Examples/DuanqiaoPlayable.cpp` -- 新視窗程式：地圖載入→地形節點（地面/河面/渡口橋板/圖釘/互動物）→編成（4v4，敵 Glock，我軍 BattlePlanner 預設 doctrine）→ BeginExecution；迴圈內 battle.Update(dt)、sync.Sync、SceneRenderer.Render、ImGui HUD（CP/phase/selected 資訊/事件流/操作提示/outcome banner）；LMB 選取、RMB 下令、Space 暫停、1/2/3 倍速、Esc 取消選取、WASD 平移視角 -- E4-3 最小循環
- [x] `CMakeLists.txt` -- `DuanqiaoPlayable` target（imgui 源碼內嵌、link glfw glad ${OPENGL_LIBRARIES} Gameplay），不進 POTATO_TESTS -- 建置註冊

**Acceptance Criteria:**
- Given 斷橋地圖已載入且 `BeginExecution` 完成，when LMB 點選我軍小隊再 RMB 點地面，then 該隊朝目標格移動且 CP 減 1
- Given 已選取小隊，when RMB 點敵軍小隊，then 該隊獲 Engage 覆寫並追擊目標
- Given 小隊受傷（members<max），when 畫面渲染，then 其血條長度隨 healthPct 縮短、顏色偏紅
- Given 無顯示環境，when 執行 `DuanqiaoPlayable`，then 輸出 `[SKIP]` 並以 0 結束
- Given `battle.GetOutcome()!=Ongoing`，when 玩家再點擊下令，then 不再消耗 CP 且 HUD 顯示戰果
- `BattleSceneTest` 既有斷言不回歸，新 overlay 斷言通過；MSVC `cmake --build` 成功

## Design Notes

- 血條做直立 XY 面板（面朝 +z 相機方向），非 billboard shader：平躺 XZ 版在 63° 俯角下投影只剩 ~2px 不可讀（edge review 發現），直立版高度可讀且不需每幀轉向。
- 膠囊體用程式化 lathe 產生（圓柱身 + 半球蓋），normal 用頂點方向近似即可——lambert shader 下視覺差異小。
- 選取環/血條掛為 squad 節點子層自動跟隨（squad 節點永不旋轉，故無繼承問題）；選取環 y≈0.22 高過橋板頂（≈0.17）防遮擋。若日後 squad 引入旋轉需改掛 parentNode。

## Verification

**Commands:**
- `cmake --build build --config Release --target DuanqiaoPlayable BattleSceneTest` -- 編譯通過
- `cd build && ctest -C Release -R "BattleSceneTest|BattlePickerTest|DuanqiaoDemo" --output-on-failure` -- 全 PASS

**Manual checks (if no CLI):**
- 本機執行 `build/bin/Release/DuanqiaoPlayable.exe`：視窗開啟顯示斷橋戰場；LMB 選取小隊出現環；RMB 下令移動；HUD 顯示 CP/事件；Space 暫停生效；戰鬥結束出 banner
