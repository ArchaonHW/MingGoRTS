---
title: 'Q-4 敵將人格 → 機率雲先驗'
type: 'feature'
created: '2026-09-17'
status: 'draft'
baseline_commit: 'db85b7a'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - _bmad-output/implementation-artifacts/spec-q2-entangled-squads.md
  - Gameplay/EnemyGeneral.h
  - Gameplay/QuantumFog.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

敵將卡的人格三軸不再只影響 doctrine——連「他大概率會把人藏哪」都人格化：侵略型敵將的機率雲先驗偏前線、狡詐型偏側翼、紀律型雲更集中。玩家讀敵將卡能真正「讀人」。

## Boundaries

**In scope:**
- `EnemyGeneral::FogBiasPoint(center, enemyDir, radius)` — 人格 → 先驗偏置點
- `EnemyGeneral::FogPriorScale()` — 紀律 → 先驗集中度
- `QuantumFog::AddEntityCloud` 加 `priorScale` 參數（預設 1.0，向後相容）
- demo：glock 人格注入 fog entity 註冊
- `QuantumTest` 新段落

**Out of scope:**
- BattleController 自動讀卡（卡注入仍在 demo/scenario 層）
- 地圖特徵伏擊格（狡詐用抽象垂直偏移，不查地形）
- 動態人格（戰中變化）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| aggression 0~100 | bias 點沿 enemyDir 偏移 `(a−0.5)·1.6·radius`（90→前推 0.64r，10→後縮 0.64r，50→中立） |
| cunning 0~100 | bias 點沿 enemyDir 垂直方向偏移 `(c−0.5)·0.8·radius` |
| discipline 0~100 | priorScale `= 1.4 − 0.8·d`（90→0.68 集中，10→1.32 鬆散，50→1.0） |
| enemyDir 零向量 | 回傳 center（退化安全） |
| 無卡/缺 personality 欄位 | 三軸預設 50 → bias=center、scale=1.0，等同現行行為 |

**語義保證**：只偏置先驗，不改候選格集合——偵查/觀測仍能推翻人格推測（狡詐敵將也可能站正中，只是先驗不信）。

</frozen-after-approval>

## Implementation Checklist

- [ ] `Gameplay/EnemyGeneral.h/.cpp`：`FogBiasPoint`（enemyDir 內部 normalize，perp = `(-dy, dx)`）+ `FogPriorScale`
- [ ] `Gameplay/QuantumFog.cpp`：`AddEntityCloud` 尾部加 `float priorScale = 1.0f`，`sigma = radius·0.5·priorScale`（scale≤0 clamp 1.0）
- [ ] `Examples/DuanqiaoPlayable.cpp`：entity 註冊的 biasPoint 改 `glock.FogBiasPoint(center, dir, 3.5f)`（dir 朝向北岸集結點）、傳 `glock.FogPriorScale()`；southCamp 場景偏置由人格偏置取代
- [ ] `Examples/QuantumTest.cpp`：新 `TestQuantumFogPersonality`——侵略 vs 謹慎卡先驗分佈可測差異、紀律集中度、空卡退回中立

## Design Notes

- enemyDir 由呼叫方給（demo：敵營→北岸集結點方向）——fog 不懂戰役幾何。
- priorScale 走既有 σ 參數化（高斯先驗 `exp(−d²/2σ²)`），不改藍噪候選生成。
- 侵略係數 1.6r 讓偏置點可越出候選半徑——先驗依然落在雲內最近格，只是分佈更斜；數值經 demo 目視調過（0.64r 前推在 6 格雲上肉眼可辨）。

## Acceptance Criteria

- [ ] 不同人格卡產生的 priors 分佈有測得出的差異（同幾何下前線候選機率不同）
- [ ] 無卡/缺欄位退回中立先驗不崩潰
- [ ] MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target QuantumTest DuanqiaoPlayable` + `cd build && ctest -C Release -R "QuantumTest|QuantumFogBattleTest|BattleSceneTest" --output-on-failure`
- `cmake --build build-mingw --target QuantumTest DuanqiaoPlayable`

**Manual checks:**
- demo：格洛克（侵略 90）的雲明顯偏向北岸——與南岸駐紮雲心分離
