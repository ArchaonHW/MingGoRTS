---
title: 'Q-7 相位干涉觀測（實驗性）'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: '67dde3d'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Gameplay/QuantumFog.h
  - Quantum/Qudit.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

Qudit 振幅相位目前恆 0——量子主題的「干涉」還沒有玩法消費。實驗機制：**同一 tick 對同一朵雲做第二次觀測**時，新目標候選與當前主峰的相位差決定干涉方向——建設性（情報穿透：時效加成）或破壞性（觀測被拒）。預設關閉（experimental flag）。

## Boundaries

**In scope:**
- `Qudit::SetPhases`（振幅乘 e^{iφ}，機率分佈不變）
- `QuantumFog`：entity `phases`/`lastObserveAt`、`SetEntityPhases`、`SetInterferenceEnabled`（預設 false）
- `Observe` 已揭露路徑的同 tick 干涉判定
- `QuantumTest` 覆蓋雙向結果 + flag 關閉零影響
- spec 附 doctrine 卡範例文字（卡層整合 defer——先驗手感）

**Out of scope:**
- DoctrineTrigger 新枚舉（干涉是情報層不是小隊層，硬塞不當）
- Reveal/Probe 干涉（接觸是物理目視非情報觀測；probe 是弱測量）
- 相位自動生成（由 scenario 顯式注入；地形相位場留待後續）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| `SetEntityPhases(id, phases)` | 長度不足補 0、超出截斷；同時寫入 entity.phases 與 state.SetPhases |
| Observe 已揭露 + flag 開 + `lastObserveAt == fogTime` | Δφ = phases[nearest] − phases[modal]；cos<0 → 回 false（觀測被拒，不刷新）；cos≥0 → 刷新 + revealTimer += 0.5·intelDuration（穿透加成） |
| flag 關 / phases 空 / 非同 tick | 既有免費刷新行為，零回歸 |
| Update/過期/SetProbabilities | 相位隨退相干歸零（SetProbabilities 重建實振幅）——decoherence 殺相位，主題自洽 |

**「同 tick」定義**：`fogTime`（Update 累積的 fog 時鐘）未推進前算同 tick——Update 之間的多次觀測會互相干涉。

</frozen-after-approval>

## Implementation Checklist

- [ ] `Quantum/Qudit.h/.cpp`：`SetPhases(const vector<double>&)`（長度不符回 false；a_i *= e^{iφ}）
- [ ] `Gameplay/QuantumFog.h/.cpp`：`UncertainEntity::phases`/`lastObserveAt`；`SetEntityPhases`/`SetInterferenceEnabled`；`fogTime`；Observe 干涉分支；抽出 `NearestCandidate`/`ModalCandidate` helper
- [ ] `Examples/QuantumTest.cpp`：`TestQuantumFogInterference`——建設性（時效加長可驗）、破壞性（回 false 不刷新）、flag 關閉行為不變、相位 API 不改機率

## Doctrine 卡範例（defer 到正式牌組）

```json
{"name":"干涉雙測","trigger":"FogCloudDense","action":"DoubleObserve","note":"情報官同刻雙測:相位建設性則看穿更久"}
```

——觸發/動作枚舉待 Q-9 教學情境一併定案，先驗機制本身。

## Acceptance Criteria

- [ ] Qudit 相位初始化 API，既有機率語義不變（SetPhases 後 Probabilities() 相同）
- [ ] 干涉雙向結果可測：cos<0 觀測被拒；cos≥0 時效加成
- [ ] 功能開關可關閉（預設 off）
- [ ] MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target QuantumTest` + `ctest -C Release -R "QuantumTest|QuantumFogBattleTest" --output-on-failure`
- `cmake --build build-mingw --target QuantumTest`
