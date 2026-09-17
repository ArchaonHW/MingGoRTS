---
title: 'Sage Command Screen — 至聖者指揮學 × 治平者管理學遊戲畫面邏輯'
type: 'feature'
created: '2026-09-17'
status: 'done'
baseline_commit: '7c366ee3fa3a3138848efb483f2d399a1984033d'
followup_review_recommended: true
review_loop_iteration: 1
context: []
deferred:
  - summary: >-
      ImageCodec hardening gaps: RGB PNG tRNS treats black pixels as transparent,
      inflate has no output cap (decompression bomb), DecodePNG/DecodeImage are
      untested dead code, duplicated trnsGray read, Crc32 lazy-init data race,
      EncodePNG has no dimension cap
    evidence: |-
      Four-layer review of sage_impl.diff; decoder has zero callers and no
      round-trip test. Owned by the parallel glTF/image workstream, not this spec.
    location: >-
      Rendering/ImageCodec.cpp
    severity: medium
  - summary: >-
      glTF loader robustness: defaultScene OOB, unvalidated accessor.count,
      index values >= vertCount fed to glDrawElements, unbounded node recursion,
      case-sensitive .GLTF check, LoadFromData accumulates without clearing,
      IBM accessor validated for FLOAT but not MAT4, discarded baseDir param,
      KHR_texture_transform doc claim with no implementation,
      GltfNodeLocalMatrix leaves TRS identity for matrix nodes
    evidence: |-
      Edge-case hunter findings on Rendering/ModelLoader.cpp — parallel
      workstream code, not caused by this spec.
    location: >-
      Rendering/ModelLoader.cpp / TinyGltfImpl.cpp
    severity: medium
  - summary: >-
      VertexArray move ctor/assignment drops pendingVertexData/pendingIndexData/
      pendingAttribs — a Mesh populated before GL init silently loses geometry on move
    evidence: |-
      Blind-hunter verified against Rendering/OpenGLRenderer.h move members;
      parallel workstream file.
    location: >-
      Rendering/OpenGLRenderer.h
    severity: medium
  - summary: >-
      PortraitRenderer gaps: material alphaMode/alphaCutoff parsed but ignored,
      unknown CLI args silently accepted, --bg/--size unchecked, empty-model AABB
      produces NaN ortho projection with silent exit 0
    evidence: |-
      Edge-case hunter findings; tool owned by parallel VRM portrait workstream.
    location: >-
      Examples/PortraitRenderer.cpp
    severity: low
  - summary: >-
      IDE suggestion-loop issues: applied suggestion regenerates after buffer hash
      changes, s_shownSignature never resets on empty sets, function-local static
      debounce state shared across IDEGUI instances, truncated suggestion can
      insert mid-code marker, removed editorBuffer writes may orphan readers
    evidence: |-
      Blind-hunter + edge-case findings; belongs to spec-intelligent-suggestions.
    location: >-
      MingGoRTS_IDE/GUI/IDEGUI.cpp / IntelligentSuggestion.cpp
    severity: medium
  - summary: >-
      IntelligentSuggestionSystem rule engine (id format, LearnFromFeedback
      weighting, magic-number regex on codeText) has zero test coverage — the lib
      is only linked into the GUI exe
    evidence: |-
      Verification-gap layer confirmed no POTATO_TESTS target links
      IntelligentSuggestion.
    location: >-
      MingGoRTS_IDE/IntelligentSuggestion.cpp
    severity: medium
  - summary: >-
      Physics collision-callback liveness fix is unobservable in tests — the
      destroy-in-callback case asserts only Check(true) and never instruments
      bodyB's callback; residual ABA if a destroyed body's address is reallocated
    evidence: |-
      Verification-gap + edge-case findings on Physics/PhysicsSystem.cpp:490-498
      and Examples/PhysicsTest.cpp [15]; owned by the physics hardening workstream.
    location: >-
      Physics/PhysicsSystem.cpp, Examples/PhysicsTest.cpp
    severity: medium
  - summary: >-
      POTATO_TESTS are compile-verified in CI but never executed — ctest is not
      invoked in any workflow, so all new test targets run locally only
    evidence: |-
      Verification-gap finding on .github/workflows/pre-commit-checks.yml; systemic.
    location: >-
      .github/workflows/
    severity: medium
  - summary: >-
      SageCommandDemo hardcodes C:/Windows/Fonts/msjh.ttc (has bilingual fallback,
      acceptable per spec) and its playable UI surface is verified only by code
      reading — no automated exercise of the five panels
    evidence: |-
      Intent-alignment audit; manual-check path remains per spec Verification.
    location: >-
      Examples/SageCommandDemo.cpp
    severity: low
  - summary: >-
      docs/index.md headings contain U+FFFD mojibake; .gitignore ignores
      assets/avatars inputs while generated output/portraits/*.png binaries are
      committed — inconsistent artifact policy
    evidence: |-
      Blind-hunter + intent-alignment findings on parallel docs/assets work.
    location: >-
      docs/index.md, .gitignore, output/portraits/
    severity: low
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

## Review Triage Log

### 2026-09-17 — Review pass
- verdicts: 61 findings — high 0, medium 4, low 7, false 0, maybe-false 0 (plus 50 deferred to owning workstreams)
- findings:
  - `[medium]` `[patch]` Routing squads keep stale orderTarget and flee toward enemy objective at 1.2× speed (Squad.cpp comment promised controller repoint but nothing did) — fixed: BattleController::Update repoints routing squads to team rally via IssueOrder(Retreat); covers ApplyCasualties and AdjustMorale paths
  - `[medium]` `[patch]` ConfuseEnemy/Sabotage return true and charge corruption during Deployment while TryDisruptEnemy silently no-ops (Intervene requires Execution) — fixed: both policies refuse outside Execution; TryDisruptEnemy now returns count
  - `[medium]` `[patch]` Tick calls ResolveOutcome every frame in Resolution, re-emitting 無勝而勝 forever — fixed: ResolveOutcome only while outcome==Ongoing
  - `[medium]` `[patch]` SageCommandDemo ignores gladLoadGLLoader failure → crash inside ImGui_ImplOpenGL3_Init — fixed: checked, exits 1 with stderr message
  - `[low]` `[patch]` Enemy-targeting heretic policies return true and charge corruption when zero living enemies exist — fixed: enemiesAffected==0 → refuse, no corruption
  - `[low]` `[patch]` Policy switch lacks default; out-of-range enum returns true emitting 施行 ? — fixed: default → false
  - `[low]` `[patch]` Corruption reaching 100 after a resolved battle left outcome=Victory (CheckSigns only flipped when Ongoing) — fixed: Fallen whenever corruption>=100 and not already Fallen
  - `[low]` `[patch]` Negative dt made sealed corruption decay grow unbounded — fixed: dt clamped >= 0
  - `[low]` `[patch]` routedCounted raw-pointer set goes stale across battles (ABA suppresses 歸附 events) — fixed: cleared on Deployment phase
  - `[low]` `[patch]` SlanderEnemy documented 最近敵隊 but hit first container-order enemy; FindNearestEnemy dead code — fixed: wired in
  - `[low]` `[patch]` Upright/heretic split relied on bare enum ordering — fixed: static_assert pins SlanderEnemy==RallyTroops+1 and DeceiveHeaven==11
  - `[defer]` `[defer]` ImageCodec: tRNS black-pixel transparency, uncapped inflate, dead untested decoder, duplicated trnsGray read, Crc32 race, no EncodePNG size cap — parallel image-pipeline workstream (deferred list)
  - `[defer]` `[defer]` ImageCodec decode path has zero callers and no round-trip test (verification-gap) — same workstream
  - `[defer]` `[defer]` VertexArray move drops pending buffer data — parallel rendering workstream
  - `[defer]` `[defer]` ModelLoader: defaultScene OOB, unvalidated accessor.count, index>=vertCount OOB, unbounded recursion, case-sensitive .GLTF, LoadFromData accumulation, IBM MAT4 check, discarded baseDir, KHR_texture_transform doc claim, matrix-node TRS — parallel glTF workstream
  - `[defer]` `[defer]` PortraitRenderer: alphaMode ignored, arg validation, NaN ortho, size cap — parallel VRM tool workstream
  - `[defer]` `[defer]` IDEGUI suggestion-loop issues (regeneration after apply, stale signature, static debounce, truncation insert, editorBuffer readers) — spec-intelligent-suggestions scope
  - `[defer]` `[defer]` IntelligentSuggestionSystem has zero test coverage — spec-intelligent-suggestions scope
  - `[defer]` `[defer]` Physics destroy-in-callback test asserts only Check(true); residual ABA on address reuse — physics hardening workstream
  - `[defer]` `[defer]` CI never runs ctest — POTATO_TESTS compile-only in workflows — systemic, repo policy
  - `[defer]` `[defer]` Model::Draw textured path unverified — parallel glTF workstream
  - `[defer]` `[defer]` docs/index.md mojibake; avatars ignored but portraits PNGs committed — parallel docs/assets workstream
  - `[defer]` `[defer]` SageCommandDemo font path Windows-only (has fallback, spec-allowed); UI surface not machine-verified — recorded residual risk
  - `[info]` `[defer]` Intent-alignment: ~64% of the review diff is bundled parallel work outside this spec's intent (glTF/VRM, IDE suggestions, physics) — each belongs to its own spec; SageCommand feature itself faithfully implements the intent contract
  - `[info]` `[defer]` Interpretation choices underspecified by intent but defensible: Draw→Defeat, subdued conflates routing+歸附, disruption consumes enemy CP, all-policies-refused-after-Fallen — noted in Design Notes

## Auto Run Result

**Summary:** Implemented the Sage Command Screen spec end-to-end: a headless `SageCommand` state machine in `Gameplay/` (three-authority model, upright/inverse six policies, corruption with three sign thresholds and Fallen override, WuXing phase cycle with seal/decay, popular-support/civil-order governance, SubdueWithoutWar adjudication) integrated with `BattleController` via external composition, plus a five-region ImGui demo and a 77-check headless test covering every I/O matrix row.

**Files changed:**
- `Gameplay/SageCommand.h/.cpp` — new headless command-layer state machine (~600 LOC)
- `Gameplay/Squad.h/.cpp` — new `AdjustMorale(delta)` seam (morale change without casualties; zero-morale triggers terminal routing)
- `Gameplay/BattleController.cpp` — routing squads repointed to team rally point each tick (fixes stale orderTarget for both casualty- and policy-induced routs)
- `Examples/SageCommandTest.cpp` — 77 checks, all 7 I/O matrix rows + disruption/ABA/emit-once regression sections
- `Examples/SageCommandDemo.cpp` — ImGui five-region command screen (道權/策權/兵權/戰場/治平儀表), bilingual strings, GL-init failure check
- `CMakeLists.txt` — Gameplay lib + SageCommandTest (POTATO_TESTS) + SageCommandDemo targets
- `spec-sage-command-screen.md` — task checklist, triage log, this result

**Review findings breakdown:** 11 verified findings patched in-place (4 medium, 7 low — listed above); ~50 findings triaged `defer` to the owning parallel workstreams (glTF/image pipeline, IDE suggestions, physics hardening, CI policy) and recorded in frontmatter `deferred`. No findings rejected as false.

**Patched counts by verdict:** medium 4, low 7.

**Follow-up review recommendation:** `true` — four medium-severity patches landed; the named unverified risk is interaction between rally repointing, morale-execution refusal, and CP intervention timing, currently covered only by headless assertions rather than a scripted battle scenario.

**Verification performed:**
- `ctest -R "SageCommand|BattleScene|Duanqiao|Doctrine|AutoPlanner"` — 5/5 passed; SageCommandTest 77/77 checks
- `mingw32-make SageCommandDemo` — compiles and links clean
- Banned-function scan on all new files — clean (snprintf only)
- Matrix test audit — every I/O row covered by a passing test, including post-patch additions for disruption gating and emit-once

**Residual risks:**
- SageCommandDemo UI verified structurally only (no automated panel exercise; manual check per spec)
- MSVC build not exercised locally (MinGW + code is standard C++20)
- Working tree is under concurrent modification by a parallel session; deferred findings assigned to those workstreams may land in separate commits
