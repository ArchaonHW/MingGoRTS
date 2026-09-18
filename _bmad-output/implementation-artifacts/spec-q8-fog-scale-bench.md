---
title: 'Q-8 大規模疊加態效能'
type: 'feature'
created: '2026-09-18'
status: 'done'
baseline_commit: 'c3b026d'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml
  - Gameplay/QuantumFog.h
  - Quantum/Qudit.h
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

量測 50+ 實體 × 多候選的 qudit Update/Observe 成本。先量測建立基線；超標才做 LOD（遠距粗格化或共享池）或數量上限。**不預先最佳化**——數據說話。

## Boundaries

**In scope:**
- QuantumTest 加 benchmark 段（或獨立 target）：N=100 實體 × 6 候選
- 量 `Update(dt)` 單幀耗時（std::chrono,Release）與 Observe 單次耗時
- 門檻：Update 單幀 < 1ms；超標 → 最小修法（候選 LOD/上限）+ 記錄數據
- 結果印在測試輸出（BENCH 行），不設硬性 FAIL（機器相依,只告警）

**Out of scope:**
- 記憶體最佳化（state space 壓縮是另一題）
- GPU/平行化（量級未到）
- 改變機率語義的任何重構

## I/O Contract

| 輸入 | 輸出 |
|---|---|
| 100 entities × 6 candidates, Update(0.1f) × 60 幀 | 平均單幀 ms + 告警（>1ms 時） |
| 1000 次 Observe | 平均單次 µs |

## Implementation Checklist

- [ ] `Examples/QuantumTest.cpp`：`BenchQuantumFogScale`——建 100×6 雲、混合 revealed/疊加態、計時 Update 60 幀與 Observe 1000 次、印 BENCH 行；超標僅警告不 FAIL
- [ ] 若超標：先做最便宜的修法（Diffuse 只算非 revealed、候選數 cap），再量一次

## Acceptance Criteria

- [ ] benchmark 進 QuantumTest，無顯示環境可跑
- [ ] 100 實體 × 6 候選 Update 單幀耗時有數據輸出；>1ms 時有明確 WARNING
- [ ] MSVC + MinGW 建置通過；既有測試無回歸

## Verification

**Commands:**
- `cmake --build build --config Release --target QuantumTest` + 執行看 BENCH 行
- `cmake --build build-mingw --target QuantumTest`

</frozen-after-approval>
