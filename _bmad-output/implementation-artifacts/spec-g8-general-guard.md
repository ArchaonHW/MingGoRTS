---
title: 'G-8 玩家將軍親臨（M&B commander）'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '2977e75'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: G-8)
---

## Objective

我軍加一支可控「將軍衛隊」：可被選取/下令、手動技能走 CP、
將軍陣亡直接戰敗。俯視 RTS 版親臨，不做第一人稱。

## Approach

**1. `Squad` 加將軍旗標**

- `SetGeneralGuard(bool)` / `IsGeneralGuard()`——標記欄位，不開子類
- 將軍隊仍是一般 squad（移動/交戰/士氣全走既有系統），
  只是死亡有額外後果

**2. 陣亡即敗（`BattleController::CheckOutcome` 最小 patch）**

- 在既有 alive[] 掃描中加：隊伍含 `IsGeneralGuard() && IsEliminated()`
  → 該隊直接落敗（team0 衛隊全滅 → Defeat；不看其餘存活隊）
- 全滅才觸發——潰逃（routing）不算陣亡，將軍可以被打跑再回來
  （與「親臨被擊退」的 M&B 手感一致，寫死此規則）
- Emit "general slain" 事件進回放流

**3. 主動技能走 CP（`BattleController`）**

`bool GeneralRally(Squad* general)`：
- 前置：general 非 null、`IsGeneralGuard()`、未消滅未潰逃、CP ≥ 1
- 效果：扣 1 CP，同隊半徑 6 格內友軍士氣 +0.30（clamp 1.0），
  含將軍隊自身；Emit 喊話事件
- 對齊既有 `CPIntervene*` 的「回傳 false = CP 不足/條件不符」語義

`bool GeneralCharge(Squad* general)`（第二技能，規格寫死）：
- 扣 1 CP；將軍隊 6 秒內 speed ×1.5、damagePerMember ×1.5
- 實作：Squad 加 `chargeTimer`，Update 內自然衰減歸零；
  `GetEffectiveSpeed`/`GetAttackDPS` 讀 charge 狀態疊乘
  ——沿用 G-3 stamina 的「生效倍率走 getter」模式

**4. `Examples/GeneralGuardTest.cpp`** 入 POTATO_TESTS：
衛隊旗標、陣亡即敗（其餘隊伍仍存活也判 Defeat）、潰逃不觸發、
rally 扣 CP + 士氣上升 + 半徑外不吃 + CP=0 拒絕、
charge 倍率生效 + 到期歸零 + 非衛隊呼叫拒絕。

**5. DuanqiaoPlayable 接線（最小）**：開局建一支衛隊標
GeneralGuard；選中衛隊時 CP 鈕列多「將軍激勵」「帶隊突擊」兩顆。
（若與平行 session 編輯衝突，UI 層可降為註解 TODO，邏輯層照交。）

## Out of scope

- 第一人稱/第三人稱視角、將軍個人戰鬥動畫
- 敵方將軍（EnemyGeneral 是牌組人格，非場上單位——兩者不同層）
- 技能樹/多技能解鎖；將軍復活機制

## Acceptance

- [ ] 衛隊可被選取/下令（即一般 squad 行為，無額外限制）
- [ ] 衛隊全滅 → 立即 Defeat（即使其餘我軍存活）；潰逃不觸發
- [ ] 兩個技能各扣 1 CP、CP 不足/條件不符回 false
- [ ] GeneralGuardTest 全綠；MSVC + MinGW 建置；無回歸

## Files

- edit: `Gameplay/Squad.h/.cpp`（旗標 + charge 計時/倍率）、
  `Gameplay/BattleController.h/.cpp`（CheckOutcome + 兩技能）、
  `Examples/DuanqiaoPlayable.cpp`（衛隊 + CP 鈕）、
  `CMakeLists.txt`（test target + POTATO_TESTS）
- new: `Examples/GeneralGuardTest.cpp`
