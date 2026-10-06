---
title: 'Q-1 QuantumFog 接入斷橋戰場（疊加態敵軍 + 偵查玩法）'
type: 'feature'
created: '2026-09-17'
status: 'done'
followup_review_recommended: false
baseline_commit: '67825f7'
baseline_revision: '821ed5c8a56796078fbf885c7fa0ab1a685a4b47'
context:
  - AGENTS.md
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Gameplay/QuantumFog.h
  - Gameplay/BattleController.h
  - Gameplay/BattleSceneSync.h
  - Examples/DuanqiaoPlayable.cpp
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

把 QuantumFog 從獨立 demo 變成斷橋的實際玩法層：敵軍小隊開戰時以**疊加態**存在——玩家只看到候選位置的機率雲，看不到真身。揭露途徑兩種：花情報點觀測（即時、遠距）或小隊物理接觸偵查（免費、要走過去）。情報有時效，過期回疊加態。

**觀測語義**：`UncertainEntity.team` 沿用 QuantumFogDemo 慣例 = **付情報點的觀測方**（玩家 0），不是敵軍陣營。

## Boundaries

**In scope:**
- `QuantumFog::Reveal`（免情報點的塌縮，物理偵查路徑）
- `BattleController`：fog 綁定、squad↔entityId 映射、接觸揭露、`DoctrineAction::Scout`
- `BattleSceneSync`：未揭露敵軍藏真身 + 機率雲標記節點
- `DuanqiaoPlayable`：敵軍註冊疊加態、情報點經濟、LMB 點雲觀測、後衛 Scout doctrine、HUD 情報顯示
- `QuantumFogBattleTest`（新 headless 測試）+ `BattleSceneTest` fog 斷言

**Out of scope:**
- Q-2~Q-9（糾纏/Probe/人格先驗/隨機源/存檔回放/干涉/效能/教學）
- 機率雲半透明渲染（現有 lambert shader 無 alpha——用機率縮放標記大小代替）
- 敵軍反過來觀測玩家（單向戰爭迷霧）
- intelDuration 過期後候選格重新生成（v1 候選集固定於註冊時）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| LMB 點機率雲標記 | `fog.Observe` → 扣情報點 → 該敵軍真身顯示 intelDuration 秒 |
| 我軍小隊進入敵軍 `fogRevealRange`（預設 3 格） | `fog.Reveal`（免費）→ 真身顯示 + 戰報事件 |
| doctrine `Scout` 命中 | 小隊 AttackMove 向最近未揭露雲的 modal 候選格 |
| 情報時效到期 | 真身隱藏、機率雲標記復活（先驗重建） |
| 情報點不足 | Observe false、戰報「情報不足」、雲不變 |

**錯誤處理**：fog 未綁定 → 一切行為退回現狀（敵軍恆可見，Scout 空轉為 Hold）；entity 對應 squad 全滅 → 標記隱藏、Observe 仍可叫但 demo 不讓點；重複 Observe 已揭露實體 → 不重複扣點（既有行為）。

</frozen-after-approval>

## Implementation Checklist

- [x] `Gameplay/QuantumFog.h/.cpp` — `Reveal(entityId, truePos)`：塌縮+揭露但不走 SpendIntel；與 Observe 共用塌縮段
- [x] `Gameplay/Doctrine.h/.cpp` — `DoctrineAction::Scout` + `ActionName`
- [x] `Gameplay/BattleController.h/.cpp` — `BindFog`/`BindFogSquad`/`GetFog`/`GetFogEntityId`/`GetFogSquad`/`SetFogRevealRange`；Update：fog->Update(dt)、接觸揭露（Emit 戰報）；EvaluateDoctrines 加 Scout case（最近未揭露 entity 的最高機率候選格 → AttackMove；無目標退回 Hold）
- [x] `Gameplay/BattleSceneSync.h/.cpp` — `SetFog(const QuantumFog*)` + `SetFogMarkerMesh`；Sync 時未揭露 bound squad 藏節點（含 overlay）；`__fog_e<id>` 標記節點掛 parentNode，每候選一子節點（位置×CELL、scale∝機率、紫灰色）
- [x] `Examples/DuanqiaoPlayable.cpp` — `BattleResources res` + `res.Setup(battle,0,intel=4,cp=5)`（取代 SetCommandPoints）；4 敵軍 AddEntityCloud（center=出生點,radius=3.5,count=6,spacing=1.2,bias=南岸敵營）+ BindFogSquad；LMB 點雲→Observe；HUD 加情報點/未揭露數；後衛 doctrine 改寫含 Scout
- [x] `Examples/QuantumFogBattleTest.cpp` — 新測試：Observe 扣情報/到期回雲/接觸免費揭露/Scout 指令朝向雲 modal
- [x] `Examples/BattleSceneTest.cpp` — fog 段落：未揭露藏節點+標記存在、Reveal 後反轉、全滅藏標記
- [x] `CMakeLists.txt` — QuantumFogBattleTest 入 POTATO_TESTS

## Design Notes

- 接觸揭露在 squad Update 之後檢查（位置已刷新）；fog->Update 用縮放後 dt（暫停時情報不過期）。
- 機率雲標記掛 `__battle_units` 下做 squad 節點的兄弟——不是子節點（未揭露時父節點 inactive 會連子節點一起藏）。
- Scout 的目標 = `GetCloud` 中機率最大候選；雲擴散後 modal 會漂移，小隊自然追著雲走。
- 敵軍真身未揭露時仍可被點選攻擊？不行——PickSquad 只回傳可見（未隱藏）的不在此列；AttackMove 接近後接觸揭露自然形成「偵查→交戰」循環。
- 情報經濟：intel=4、cost=1、duration=25s——可觀測 4 次但敵軍也是 4 隊，逼玩家選擇花點或走接觸偵查。

## Acceptance Criteria

- [x] demo 中未觀測敵軍顯示機率雲而非實體
- [x] 情報點消費 → Observe → 真實位置顯示；intelDuration 過期回雲
- [x] Scout doctrine 可觸發觀測（接觸揭露）
- [x] `QuantumFogBattleTest` + `BattleSceneTest` 新斷言 PASS；既有測試無回歸
- [x] MSVC + MinGW 建置通過

## Verification

**Commands:**
- `cmake --build build --config Release --target DuanqiaoPlayable QuantumFogBattleTest BattleSceneTest`
- `cd build && ctest -C Release -R "QuantumFogBattleTest|BattleSceneTest|BattlePickerTest|DuanqiaoDemo|DoctrineBattleDemo|QuantumTest" --output-on-failure`
- `cmake --build build-mingw --target DuanqiaoPlayable QuantumFogBattleTest`

**Manual checks:**
- `DuanqiaoPlayable.exe`：南岸只見紫色機率雲；LMB 點雲扣情報顯真身；25s 後回雲；後衛自動走向雲、接近後敵軍現蹤

## Review Triage Log

### 2026-09-17 — Review pass
- verdicts: 37 findings — high 0, medium 7, low 4, false 14, maybe-false 0
- findings:
  - `[false]` `[reject]` CMakeLists hunk missing from commit — reviewer read stale artifact q1-review-diff.txt; real commit aa51f42 includes CMakeLists.txt (+10, registers QuantumFogBattleTest).
  - `[false]` `[reject]` Emit during fogEntities iteration — commit defers via `spotted` vector, emits after loop (BattleController.cpp:247-249).
  - `[false]` `[reject]` Fog not integrated into targeting — commit adds IsHiddenByFog gating in Intervene/FindNearestEnemy/FindWeakestEnemy/UpdateContexts.
  - `[false]` `[reject]` Sustained contact causes flicker — commit calls Reveal unconditionally in contact and emits only on transition.
  - `[false]` `[reject]` Scout deadlock on stale modal — commit adds negative observation (EliminateCandidate) shrinking covered-empty cells.
  - `[false]` `[reject]` BindFogSquad no validation — commit validates entityId against EntityCount.
  - `[false]` `[reject]` Stale ghost markers after filtering — commit hides children beyond cloud.size().
  - `[false]` `[reject]` Detach leaves stale fog pointer — commit nulls fog/mesh and clears fogNodes in Detach.
  - `[false]` `[reject]` HUD counts dead enemies — commit skips owner->IsEliminated() in hiddenFoes (DuanqiaoPlayable.cpp:708).
  - `[false]` `[reject]` AddEntityCloud without bias — commit offsets cloud center 1.5 cells toward attack direction (same effect as biasPoint).
  - `[false]` `[reject]` Enemy Scout doctrine permanent Hold — intended: FindScoutTarget only targets bound opposing entities; consistent with one-way fog scope.
  - `[low]` `[reject]` PickFogCloud radius vs marker scale mismatch — misclick costs nothing (Observe only fires on hit); rescaling tolerance adds tuning complexity.
  - `[low]` `[reject]` Contact reveal ignores UncertainEntity.team — only enemy squads are ever bound (one-way fog, out of scope); observer-side semantics moot.
  - `[low]` `[reject]` Observe free without BindResources — demo always binds; unbound-resources is a developer-facing seam, not user-facing harm.
  - `[medium]` `[patch]` Negative observation could erase the truth cell — observer near candidate within fogRevealRange of true pos — fixed: skip elimination when candidate within fogRevealRange of target.
  - `[medium]` `[patch]` Stale Engage hunts re-hidden enemy — squads Engage-ing a re-hidden fog entity now revert to Hold in Update.
  - `[medium]` `[patch]` Routing squads act as observers — IsRouting() added to both contact and negative-observation skip conditions.
  - `[medium]` `[patch]` BindFog swap keeps stale fogEntities ids — BindFog now revalidates/drops out-of-range bindings; BindFogSquad rejects duplicate entityId.
  - `[low]` `[patch]` SetFogRevealRange accepts non-positive/NaN — guard added (cells > 0).
  - `[medium]` `[patch]` Hidden squad can fight when engageRange > fogRevealRange — ResolveCombat skips pairs where either side IsHiddenByFog.
  - `[medium]` `[patch]` SetFog leaves orphaned active marker nodes — nodes now deactivated + removed from parentNode before fogNodes cleared.
  - `[medium]` `[patch]` Ownerless entity produces unobservable ghost cloud — !owner treated as hide.
  - `[low]` `[patch]` Hidden-enemy Engage untested — QuantumFogBattleTest asserts Intervene→false pre-reveal / true post-reveal with CP accounting.
  - `[low]` `[patch]` Re-observe refresh unverified — test now advances time between observes and checks still-revealed + updated GetRevealedPos.
  - `[low]` `[patch]` Contact-reveal event unobserved — SetEventCallback counts exactly one 目擊 across sustained contact.
  - `[low]` `[patch]` Marker child position/scale unasserted — BattleSceneTest asserts candidate*CELL position, 0.4+2p scale, and hide-branch via EliminateCandidate.
  - `[low]` `[patch]` BindFogSquad invalid id untested — test asserts rejection for -1 and >= EntityCount.
  - `[low]` `[patch]` ActionFromString("Scout") unverified — ActionFromString moved public; mapping pinned by test.
  - `[low]` `[defer]` PickSquad not visibility-aware — compensated at DuanqiaoPlayable callsite + Intervene backstop; making the shared picker fog-aware changes semantics for all callers — deferred.
  - `[low]` `[defer]` EliminateCandidate last-candidate stall — true pos rarely a literal candidate; if all-but-one eliminated and survivor covered-empty, scout parks; latent (enemies advance into contact in shipped demo).
  - `[low]` `[defer]` "情報不足" pushed to demo eventLog, not Emit — BattleRecorder wraps SetEventCallback so it isn't recorded; recording integration belongs to Q-6 scope.
  - `[low]` `[defer]` GetFogSquad linear scan per entity per frame — fine at 4 entities; revisit if entity counts grow (Q-8 perf item).
  - `[low]` `[defer]` DuanqiaoPlayable GUI paths (PickFogCloud ray math, LMB dispatch, HUD) have no automated coverage — exe is display-only/[SKIP] headless; manual check remains the surface-level verification.
  - `[low]` `[defer]` intelDuration expiry rebuild re-zeros priors for eliminated candidates — consistent with fixed-candidate-set scope; note for Q-3+ tuning.
  - `[low]` `[defer]` sync.SetFog(fogA) vs battle.BindFog(fogB) divergence — caller error, single-call-site demo; document-only risk.
  - `[low]` `[defer]` spec intent enumerates Reveal only; EliminateCandidate + negative observation are uncontracted additions (coherent, kept — noted as spec deviation).

## Auto Run Result

Status: done

Summary: Q-1 quantum fog battle integration — enemy squads enter as superposition clouds; paid intel Observe or free contact-scouting collapses them; intel expiry returns to cloud. Implemented across QuantumFog (Reveal/EliminateCandidate/re-observe refresh), BattleController (fog binding, contact reveal, negative observation, Scout doctrine action, IsHiddenByFog gating), BattleSceneSync (hidden squads + probability-scaled cloud markers), DuanqiaoPlayable (intel economy, LMB cloud observation, Scout rear guard, HUD), plus headless tests.

Files changed (commit aa51f42 + review-patch commits by parallel session): Gameplay/QuantumFog.{h,cpp} (Reveal, EliminateCandidate, refresh), Gameplay/BattleController.{h,cpp} (fog API, contact/negative observation, Scout, hidden-fog gating, combat guard), Gameplay/BattleSceneSync.{h,cpp} (fog markers, hide logic), Gameplay/Doctrine.{h,cpp} (Scout action), Gameplay/EnemyGeneral.{h,cpp} (Scout mapping, public ActionFromString), Examples/DuanqiaoPlayable.cpp (integration), Examples/QuantumFogBattleTest.cpp + Examples/BattleSceneTest.cpp (coverage), CMakeLists.txt (test target).

Review findings breakdown: 8 medium/low code findings patched (truth-cell elimination guard, stale-Engage reversion, routing observers, BindFog/BindFogSquad validation, range clamp, combat fog guard, SetFog node cleanup, ownerless-entity hide); 6 verification gaps patched (Engage/Intervene fog test, re-observe refresh, event emission, marker position/scale, invalid-id rejection, Scout string mapping); 7 findings deferred (PickSquad visibility, last-candidate stall, eventLog-vs-Emit, GetFogSquad O(n), GUI-only coverage, priors-on-rebuild, dual-fog divergence); 14 rejected as false (stale-artifact reads or intended behavior), 4 rejected as low.

Verification: `cmake --build build --config Release --target DuanqiaoPlayable QuantumFogBattleTest BattleSceneTest` PASS; `ctest -C Release -R "QuantumFogBattleTest|BattleSceneTest|BattlePickerTest|DuanqiaoDemo|DoctrineBattleDemo|QuantumTest"` 6/6 PASS (QuantumFogBattleTest 28 checks); `cmake --build build-mingw --target DuanqiaoPlayable QuantumFogBattleTest` PASS; banned-function scan clean on touched sources.

Residual risks: DuanqiaoPlayable interactive paths (cloud picking, HUD) verified only manually — recommend one human run; deferred items listed above; Q-3 Probe changes are layered on top in the working tree under spec-q3 (out of this story's scope).
