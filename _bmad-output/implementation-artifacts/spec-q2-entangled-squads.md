---
title: 'Q-2 糾纏小隊：測 A 即知 B'
type: 'feature'
created: '2026-09-17'
status: 'draft'
baseline_commit: '18f48b3'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - _bmad-output/implementation-artifacts/spec-q1-quantum-fog-battle.md
  - _bmad-output/implementation-artifacts/spec-q3-weak-probe.md
  - Gameplay/QuantumFog.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

兩支敵軍疊加態可以「糾纏」：佯攻隊與主力同向機動時，觀測其中一支即獲得另一支的強烈位置線索——不付第二次情報，但拿到的是「推測」而非「親見」（對方雲收縮但不揭露真身）。對應「觀察/道權」主題的資訊經濟深化。

## Boundaries

**In scope:**
- `QuantumFog::Entangle(a, b)` + `EntangledPartner(id)` 查詢
- 觀測/接觸/隨機觀測塌縮時，未揭露的糾纏對象機率向「相關候選」集中（blend 0.7，不設 revealed）
- 相關性 = 雲內相對位置匹配（同向機動的設計假設）
- `DuanqiaoPlayable`：糾纏兩支敵軍 + 觀測時事件訊息
- `QuantumTest` 新段落覆蓋

**Out of scope:**
- joint qudit（dim = candA×candB 的完整實作——blend 近似已滿足驗收）
- 多對多糾纏（1:1；重複 Entangle 拒絕）
- Probe 觸發糾纏（弱測量不傳遞——弱到不構成 joint 測量）
- 糾纏鏈級聯（單跳即止）

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| `Entangle(a,b)` 雙方合法、未糾纏、a≠b | true；雙向 corr 表建立 |
| 無效 id / a==b / 任一方已糾纏 | false |
| Observe/Reveal/ObserveRandom(A) 塌縮到候選 i（僅未揭露→揭露轉換） | 未揭露 B 的機率 blend 向 corrA2B[i]（0.7 權重）；B 已揭露則跳過；重複 Reveal/Observe 僅刷新時效不再傳遞 |
| corr 目標候選已被 EliminateCandidate 歸零 | 跳過（硬證據優先於糾纏推測） |
| Probe | 不觸發糾纏 |

**對稱性寫死**：糾纏雙向——觀測 B 同樣使 A 集中（corrB2A）。量子力學上測量任一半都塌縮 joint 態，玩法上對稱更誠實也更平衡（敵方被糾纏的兩隊都是「測一知二」的誘餌與風險）。

**集中不揭露**：對方 entity 的 `revealed` 維持 false——糾纏給的是高置信雲（單峰 ~0.7+），仍需接觸/觀測/探測確認真值。

</frozen-after-approval>

## Implementation Checklist

- [ ] `QuantumFog.h/.cpp`：`struct EntangleLink {int a,b; vector<int> a2b,b2a;}`；`Entangle` 以雲質心相對偏移最近鄰建 corr 表；`CollapseNear` 回傳塌縮 index；`PropagateEntanglement(id, candIdx)` 在 Observe/Reveal/ObserveRandom 成功路徑呼叫
- [ ] `Examples/DuanqiaoPlayable.cpp`：糾纏兩支敵軍 entity；觀測後若 partner 存在且未揭露 → 事件「糾纏:X 雲收縮」
- [ ] `Examples/QuantumTest.cpp`：新 `TestQuantumFogEntangle`——相關候選集中、對稱、已揭露跳過、消去候選不復活、情報只扣一次、非法/重複 Entangle

## Design Notes

- corr 表：`corrA2B[i] = argmin_j |(B_j − centroidB) − (A_i − centroidA)|`——「佯攻偏北則主力也偏北」。糾纏建立時計算一次，候選向量不變（EliminateCandidate 只歸零機率不移位），表不需維護。
- blend 0.7 而非 onehot：保留糾纏推測的殘餘不確定性，也避免「集中雲」在視覺上與真揭露混淆（單一 1.0 標記）。
- 集中不寫 priors：與 Probe 同屬軟情報，時效/退相干會沖淡。

## Acceptance Criteria

- [ ] Entangle API + 測試：observe A 後 B 機率集中於相關候選格
- [ ] 單獨觀測 B 對稱觸發 A 集中（spec 寫死）
- [ ] demo 情境使用 Entangle
- [ ] 情報只在被觀測方扣一次
- [ ] MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target QuantumTest QuantumFogBattleTest DuanqiaoPlayable` + `cd build && ctest -C Release -R "QuantumTest|QuantumFogBattleTest|BattleSceneTest" --output-on-failure`
- `cmake --build build-mingw --target QuantumTest DuanqiaoPlayable`

**Manual checks:**
- demo：糾纏對其一點雲觀測 → 另一朵雲立刻縮成單峰但仍不顯真身
