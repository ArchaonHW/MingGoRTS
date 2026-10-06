---
title: 'Q-9 量子偵查教學情境'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '7469776'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Examples/DuanqiaoPlayable.cpp
  - Gameplay/QuantumFog.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

腳本化教學關卡：引導玩家走完 **observe → probe → 情報時效** 的量子偵查循環。吃 DuanqiaoPlayable 既有殼（場景/fog/點選全現成），新檔 `DuanqiaoTutorial.cpp`，全離線資源、不依賴外部檔案。

## Boundaries

**In scope:**
- 新範例 `DuanqiaoTutorial`：小型戰場 + 3 朵敵軍機率雲 + 玩家偵查隊
- 三段引導（步驟 UI 面板 + 高亮目標 + 完成判定）：
  1. **觀測**：LMB 點雲 → Observe → 真身顯示（扣情報）
  2. **探測**：RMB/Probe 鍵對第二朵雲 → Probe → 雲收縮不塌縮
  3. **時效**：等情報過期 → 看真身回雲（加速提示 or 縮短 duration）
- 每段完成條件由 fog 事件/狀態輪詢判定；可重看（R 鍵重置段落）
- HUD 步驟指示（ImGui 文字 + 進度）；完成後顯示總結
- headless 驗證：教學邏輯抽成可測狀態機（步進/完成判定），進 ctest

**Out of scope:**
- 語音/動畫引導、任務系統泛化（這是單一腳本關）
- 干涉/糾纏教學（Q-2/Q-7 是進階機制，教學只教基礎循環）
- 存檔/成就

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| LMB 點雲（階段 1） | Observe 成功 → 進入階段 2 |
| Probe 鍵 + 點雲（階段 2） | Probe 成功 → 進入階段 3 |
| 情報過期（階段 3） | 完成 → 總結面板；R 重置 |
| 非當前階段操作 | 提示「先看提示」但不禁用（自由探索） |

**狀態機**（可 headless 測）：`TutorialStep{Observe, Probe, Expire, Done}` + `Advance(event)` 純函式。

## Implementation Checklist

- [x] `Gameplay/TutorialScript.h/.cpp`（或 Examples 內 header-only）：步驟狀態機、事件→步進映射、每步提示文字；純邏輯可測
- [x] `Examples/DuanqiaoTutorial.cpp`：複用 DuanqiaoPlayable 場景骨架（最小戰場 12×8、3 敵雲、1 偵查隊）、步驟面板、高亮當前目標雲、R 重置
- [x] `Examples/TutorialScriptTest.cpp` 或併入既有測試：步進序列、亂序操作不推進、重置回第一步
- [x] `CMakeLists.txt`：target + POTATO_TESTS

## Acceptance Criteria

- [x] 三段引導可完成、R 可重看
- [x] 全離線資源（只用既有 mesh/UI）
- [x] headless 狀態機測試 PASS；MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target DuanqiaoTutorial <test-target>`
- `cd build && ctest -C Release --output-on-failure`
- `cmake --build build-mingw --target DuanqiaoTutorial`

**Manual checks:**
- `DuanqiaoTutorial.exe`：跟提示走完三段，R 重置可重跑

</frozen-after-approval>

## Review Triage Log

### 2026-09-18 — Review pass
- verdicts: 19 findings — high 0, medium 5, low 8, false 3, maybe-false 1
- findings:
  - `[medium]` `[patch]` Intel-exhaustion soft-lock (all steps) — HUD now computes neededIntel per step and shows "情報不足——按 R 重置" when locked.
  - `[medium]` `[patch]` No feedback on failed ops / missed clicks — nudge now surfaces on fog-op failure and PickFogCloud miss; failure log no longer hardcodes 情報不足.
  - `[medium]` `[patch]` Highlight ring boundingRadius 1.0 < mesh extent 1.05*CELL — frustum-culled while visible; bound now 1.1*CELL.
  - `[medium]` `[patch]` setupFog reset relied on identical entity ids/counts — sync.SetFog(&fog) now re-invoked inside setupFog to tear down stale marker nodes.
  - `[medium]` `[patch]` fog.AddEntity return -1 unchecked — logged + bind skipped.
  - `[low]` `[patch]` Magic number 3 duplicated — kFogEntities/kObserveCost/kProbeCost constants.
  - `[low]` `[patch]` TutorialScript ctor accepts -1 targets — clamped to 0 with sentinel comment.
  - `[low]` `[patch]` Probe-step recovery UX gap (target already revealed → unpickable, highlight hidden) — per-step hint when target is revealed.
  - `[low]` `[patch]` HUD-surface getters untested at steps 2/3 — test now asserts Hint/Nudge non-empty every step, StepIndex per step, StepCount==3, TargetEntity==-1 at Done, empty Nudge at Done, Advance(None) no-op (43 checks).
  - `[false]` `[reject]` Vertex tangent/bitangent uninitialized — Vertex fields are Vector3/Vector2 with zero-init ctors; verified nothing uninitialized uploaded.
  - `[false]` `[reject]` Advance(event) 純函式 literal reading — semantic reading (pure logic, headless-testable) is the contract's own goal clause; entityId param required by target-gating.
  - `[false]` `[reject]` Steps 1-2 not event/poll-based — call-site bool return is fog's own verdict and more correct (contact Reveal must not count as Observe).
  - `[low]` `[reject]` Key bindings duplicated in Hint/Nudge/HUD strings — cosmetic, single consumer.
  - `[low]` `[reject]` Expiry double-logged (fog event + tutorial line) — cosmetic.
  - `[low]` `[reject]` Font path Windows-only — same pattern as DuanqiaoPlayable; fallback exists; exe is Windows-targeted demo.
  - `[maybe-false]` `[defer]` Expired event carries no entity provenance — any expiry completes step 3; intent's "等情報過期" is generic; pinning per-entity provenance would need spec change.
  - `[low]` `[defer]` Click→Advance wiring has no automated coverage — same headless-[SKIP] convention as DuanqiaoPlayable; FSM extraction was the deliberate testability mitigation.
  - `[low]` `[defer]` PickFogCloud duplicated from DuanqiaoPlayable — "既有殼" scaffold reading permits copy-reuse; extraction refactor is a separate item.
  - `[low]` `[defer]` No ESC quit / WantCaptureKeyboard check — demo-scope UX gap, matches playable conventions.

## Auto Run Result

Status: done

Summary: Q-9 scripted scout tutorial — three-stage guided mission (Observe → Probe → intel expiry) on a procedural 12x8 battlefield with 3 enemy clouds and a scout squad. TutorialScript is a header-only, dependency-free FSM (TutorialStep{Observe,Probe,Expire,Done} + Advance(event, entityId)) with target-gated advancement, per-step hints, nudges, and reset; DuanqiaoTutorial wires it to the playable's shell patterns (fog markers, PickFogCloud, intel economy) with HUD progress, highlight ring, and R-reset; TutorialScriptTest covers the FSM headless.

Files changed: Gameplay/TutorialScript.h (new FSM), Examples/DuanqiaoTutorial.cpp (new demo), Examples/TutorialScriptTest.cpp (new test), CMakeLists.txt (targets; committed earlier with parallel work).

Review findings breakdown: 9 findings patched (intel soft-lock all steps, failed-op/missed-click feedback, highlight culling bound, reset robustness, AddEntity -1 check, magic constants, -1 target clamp, probe-step recovery hint, HUD-getter test coverage); 4 deferred (expiry provenance, wiring coverage, PickFogCloud duplication, demo UX gaps); 6 rejected (3 false + 3 low/cosmetic).

Verification: MSVC Release build PASS (DuanqiaoTutorial + TutorialScriptTest); MinGW build PASS (DuanqiaoTutorial); TutorialScriptTest 43 PASS/0 FAIL, registered in POTATO_TESTS; banned-function scan clean; exe smoke-run opens window and renders (interactive click-through remains manual).

Residual risks: interactive tutorial flow (click→advance) verified only by smoke-run + FSM tests — recommend one human playthrough; deferred items listed above.
