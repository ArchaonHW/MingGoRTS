---
title: 'Sage Command Screen — 至聖者指揮學 × 治平者管理學遊戲畫面邏輯'
type: 'feature'
created: '2026-09-17'
status: 'in-review'
baseline_commit: '7c366ee3fa3a3138848efb483f2d399a1984033d'
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** 現有 doctrine 戰鬥層（BattleController/Doctrine/BattlePlanner）只有 console demo，沒有可視化的「指揮畫面」。使用者要把網路上的《至聖者指揮學》（三權模型、正邪切換、墮落防線、五行節奏）與《治平者管理學》（政戰合一、無勝而勝）變成可玩的遊戲畫面邏輯。

**Approach:** 新增 headless 邏輯核心 `SageCommand`（三權/正逆六策/墮落值/五行階段/民心秩序）整合現有 `BattleController`，再用 ImGui 寫一個可視 demo `SageCommandDemo`：左道權（墮落監察）、中戰場、右策權（正逆策切換）、下兵權（小隊/CP 介入）、上治平儀表（民心/秩序/勝利型態）。

## Boundaries & Constraints

**Always:**
- 邏輯規則全部在 `Gameplay/`（headless 可測）；ImGui/GLFW 只出現在 `Examples/SageCommandDemo.cpp`。
- 註解與 UI 字串用繁體中文；識別符/檔名用英文。
- CMake 加 target 比照既有範例模式（imgui 源碼比照 `MingGoRTS_IDE_GUI` 區塊）。
- 禁用 banned C 函式（`gets|strcpy|strcat|sprintf|vsprintf|scanf`）——用 `snprintf`/`strncpy`。
- 繁體中文 ImGui 顯示若缺 CJK 字型，UI 字串可中英並列（如 `道權 Tao`），不阻塞功能。

**Ask First:** 修改 `BattleController`/`Doctrine`/`Squad` 既有 public API 行為（新增唯讀查詢方法可直接做）；引入新第三方依賴。

**Never:** 改 `external/`、`build/`；寫死 secrets；把遊戲邏輯寫進 ImGui 繪製碼裡。

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 正策生效 | `ApplyPolicy` 正策（立人/立制/立國）任一階段 | 我方小隊士氣或民心/秩序上升；墮落值不變 | 非法階段呼叫回傳 false |
| 逆策生效+累墮 | 木火階段 `ApplyPolicy` 逆策 | 敵方受效果（士氣降/命令干擾），墮落值 +N（木火期折扣） | 墮落≥100 後拒絕並回 false |
| 土階段封邪 | Earth 階段 `SealHeresy()` | 逆策鎖定、墮落值隨時間衰減、秩序回升 | 非土階段呼叫回傳 false |
| 墮落徵象 | 墮落 60/80/100 門檻 | 分別觸發徵象 1/2/3 警告；≥100 → outcome=Fallen（失格，勝負無效） | 每徵象只觸發一次 |
| 至聖者無戰 | 戰場 Victory 且敵軍潰逃/歸附數 > 殲滅數，且民心≥60 | outcome=SubdueWithoutWar（「不戰而屈人之兵」） | 全殲取勝則為普通 Victory |
| 治平者無勝 | 戰場 Defeat/Draw 且民心≥70 且秩序≥70 | outcome=GovernedPeace（「戰敗而治成」——敗戰不算真敗） | 治理未達標則為普通 Defeat；Fallen 仍優先 |
| 階段輪轉 | `AdvancePhase()` | 木火→土→金水→木火；金水期所有策效果減半（裁斷與不作為） | — |
| 兵權執行 | Deployment 階段用 BattlePlanner 產 doctrine → `BeginExecution` | 進入即時層；策權仍可即時切換（指揮=切換權） | phase 非 Deployment 時回 false |

</frozen-after-approval>

## Code Map

- `Gameplay/SageCommand.h`（新）— 三權狀態機：`Authority`（道/策/兵）、`Policy`（正六策/逆六策）、`WuXingPhase`（木火/土/金水）、墮落值、民心/秩序、`SageOutcome`（Ongoing/Victory/Defeat/SubdueWithoutWar/GovernedPeace/Fallen）
- `Gameplay/SageCommand.cpp`（新）— `ApplyPolicy`/`SealHeresy`/`AdvancePhase`/`Tick`/`ResolveOutcome`，政策效果經 callback 或直接調 `Squad` 士氣/`BattleController`
- `Examples/SageCommandDemo.cpp`（新）— ImGui 畫面：五區佈局 + `ImDrawList` 2D 戰場；Deployment→Execution→Resolution 畫面流
- `Examples/SageCommandTest.cpp`（新）— headless 邏輯測試，覆 I/O matrix 全行
- `CMakeLists.txt` — 加 `SageCommandDemo`（imgui 源碼+glfw+OpenGL+Gameplay）與 `SageCommandTest`（進 POTATO_TESTS）
- `Gameplay/BattleController.h` — 既有：`TotalMembers`、objective/rally 查詢、`Intervene` CP 介入可直接用
- `Gameplay/BattlePlanner.{h,cpp}` — 既有：Deployment 階段自動產 doctrine
- `Gameplay/Squad.h` — 既有：morale/members/routing/position 供畫面與政策效果
- `MingGoRTS_IDE/GUI/Main.cpp` — 既有 ImGui+GLFW 主迴圈樣板
- `_bmad-output/specs/spec-doctrine-battle-prototype/SPEC.md` — doctrine 原型規格參考

## Tasks & Acceptance

**Execution:**
- [x] `Gameplay/SageCommand.h` / `SageCommand.cpp` — 實作三權/正逆六策/墮落/五行/治理狀態機 — 邏輯核心，全部 headless
- [x] `Examples/SageCommandTest.cpp` — 覆 I/O matrix 每行 Given/When/Then — 無 GUI 依賴的可驗證測試
- [x] `Examples/SageCommandDemo.cpp` — ImGui 五區指揮畫面接 BattleController+SageCommand — 可視畫面邏輯
- [x] `CMakeLists.txt` — 註冊兩 target，`SageCommandTest` 入 POTATO_TESTS — 建置整合

**Acceptance Criteria:**
- Given 新 build，when `ctest -R SageCommandTest`，then 全 I/O matrix 行通過
- Given Deployment 階段，when 玩家按策權面板的正/逆策，then 效果即時反映到戰場小隊與儀表，逆策累積墮落值
- Given 墮落值達 100，when 戰鬥仍在進行，then 道權面板顯示失格且 outcome=Fallen
- Given 敵軍潰逃比例高且民心≥60，when 戰鬥結束，then 結算畫面顯示「至聖者無戰」而非普通勝利
- Given 我軍戰敗/平手但民心≥70 且秩序≥70，when 戰鬥結束，then 結算顯示「治平者無勝」（戰敗而治成）；治理未達標則為普通敗北

## Design Notes

- 哲學映射：道權=唯讀監察（墮落儀表+徵象，不能下命令）；策權=唯一切換鈕（正/逆六策+五行節奏）；兵權=執行層（doctrine 指派+CP 介入）——呼應「指揮=切換權，非行動」。
- 墮落徵象對應原文三象：60=以非常為常、80=以手段為正義、100=以勝負代是非→失格。
- 政策效果落地：正策加我方 morale/民心/秩序；逆策扣敵 morale 或短暫覆寫敵 doctrine 評估（用 `Intervene` 同型機制）。金水期效果減半實作為效果係數 0.5。
- 核心價值軸（考證見下）：至聖者無戰=SubdueWithoutWar（勝利但歸附>殲滅 且民心≥60）；治平者無勝=GovernedPeace（敗/平 但民心≥70 且秩序≥70，治理成功使軍事敗北不算真敗）；Fallen 凌駕兩者（墮落滿值→勝負皆無效）。
- 無勝而勝判定：戰後統計敵隊 `IsRouting()` 加歸附數 > 被殲滅數且民心≥60。
- 治平無勝判定：戰場 Defeat/Draw 時檢查民心與秩序雙門檻——呼應治平者管理學「政戰合一」：治理是長期戰略，戰略是短期治理；軍事上的敗不等於治理上的敗。
- 考證（2026-09-17 網路確認）：「不戰而屈人之兵，善之善者也」出自《孫子·謀攻》；「善戰者之勝也，無智名，無勇功」出自《孫子·軍形》；「仁者無敵」《孟子》；「夫唯不爭，故天下莫能與之爭」《道德經》81 章；「治平」語出《大學》修齊治平八目。《至聖者指揮學》《治平者管理學》為網路（vocus）上的中西哲學整合框架，核心命題「治平者無勝」= 超越勝負的「無勝而勝」。以上門檻數值（60/70/70）為本作遊戲化詮釋，非原文數值。
- Demo 初始部署直接複用 `AutoPlannerDemo` 的場景（20x15 斷牆地形、3v2 小隊、BattlePlanner 產 doctrine），玩家專注「切換」而非編成。

## Verification

**Commands:**
- `cmake --build build --target SageCommandTest SageCommandDemo`（或對應 build 目錄）— expected: MSVC/MinGW 皆編譯過
- `ctest -R SageCommandTest`（在 build 目錄）— expected: ALL CHECKS PASSED

**Manual checks (if no CLI):**
- 跑 `SageCommandDemo`：五區面板可見、戰場小隊移動、逆策後墮落條上升、土階段封邪可按、戰後結算顯示勝利型態
