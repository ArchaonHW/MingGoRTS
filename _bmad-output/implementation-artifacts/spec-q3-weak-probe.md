---
title: 'Q-3 弱觀測 Probe：廉價部分塌縮'
type: 'feature'
created: '2026-09-17'
status: 'done'
baseline_commit: 'aa51f42'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - _bmad-output/implementation-artifacts/spec-q1-quantum-fog-battle.md
  - Gameplay/QuantumFog.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

情報點經濟的第三層：不全額買斷（Observe），而是廉價「探測」——機率雲向真值方向**收縮**但不塌縮。可連續投入逼近確定，對應設計的觀察/道權主題：玩家在低確定度下先做便宜試探，再決定要不要花大錢觀測或派兵接觸。

## Boundaries

**In scope:**
- `QuantumFog::Probe(entityId, truePos, strength)` + `probeCost` 構造參數
- `DuanqiaoPlayable`：LMB 點雲 = Probe（1 情報），Shift+LMB 點雲 = Observe（2 情報）；demo 情報 4→8、觀測價 1→2
- `QuantumTest` 新增 Probe 覆蓋：機率單調收斂、費用 < Observe、防護分支

**Out of scope:**
- doctrine「探測」卡（Probe 是玩家主動動作，Q-7 干涉/教學另計）
- 探測的 UI 專屬圖示（沿用雲標記縮放視覺回饋）
- Probe 結果存檔（Q-6 一起）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| `Probe(id, truePos, strength)` 情報足夠 | 扣 probeCost，候選機率向 truePos 近端重加權（`p_i' ∝ p_i · ((1−s) + s·w_i/w_max)`，`w_i = 1/(1+d_i²)`），revealed 不變 |
| 情報不足 | false，雲不變 |
| strength ≤0 或 ≥1 | clamp 到 [0,1]；0 = 免費無效果（仍扣點？否——s≤0 直接回 false 不扣點） |
| 已揭露實體 | 回 true 不扣點（等同 Observe 的重複呼叫語義） |
| 無效 entityId | false |

**語義保證**：重加權只乘係數不設零——不會把候選消成單點（與 EliminateCandidate 的硬證據不同）；探測結果不寫回 priors（時效到期回先驗，軟情報會過期）。

</frozen-after-approval>

## Implementation Checklist

- [ ] `Gameplay/QuantumFog.h/.cpp` — 構造加 `probeCost`（預設 1，不破壞既有呼叫）；`Probe`：驗證→`SpendIntel(e.team, probeCost)`→重加權→`SetProbabilities`；總和 ≤0 防護
- [ ] `Examples/DuanqiaoPlayable.cpp` — fog 建構改 `(25.0f, /*observe=*/2, /*probe=*/1)`、`res.Setup` intel 4→8；點雲分流：Shift+LMB=Observe、LMB=Probe(strength 0.4)；戰報訊息含機率上限；HUD 提示
- [ ] `Examples/QuantumTest.cpp` — Probe 段落：扣點數 < Observe、maxProb 單調遞增、連續探測逼近塌縮、情報不足失敗不變、已揭露不扣點

## Design Notes

- `w_i = 1/(1+d_i²)`：免參數的單調距離衰減，不存註冊 radius 也夠用。
- `s=0.4` 的 demo 手感：一次探測讓 modal 機率約 +15~25%，連探三次約可分辨真值。
- Probe 不動 `priors`：Q-1 已確立「priors=先驗信念、state=當前情報」分層——EliminateCandidate 是硬證據才改 priors。

## Acceptance Criteria

- [ ] Probe 後候選機率方差下降且非零候選 >1（單次不完全塌縮）
- [ ] probeCost < observeCost；重複 Probe 可逼近塌縮（maxProb → 1）
- [ ] QuantumTest 覆蓋機率單調收斂 + 不歸一化防護；既有測試無回歸
- [ ] MSVC + MinGW 建置通過

## Verification

**Commands:**
- `cmake --build build --config Release --target QuantumFogBattleTest DuanqiaoPlayable` + `cd build && ctest -C Release -R "QuantumTest|QuantumFogBattleTest|BattleSceneTest" --output-on-failure`
- `cmake --build build-mingw --target QuantumFogBattleTest DuanqiaoPlayable`

**Manual checks:**
- demo：LMB 點雲 → 紫標記大小差距拉大（機率集中）；Shift+LMB → 直接塌縮現真身
