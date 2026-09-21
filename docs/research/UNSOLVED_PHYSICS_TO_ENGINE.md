# 未解決物理問題 → 引擎技術對照研究

**目的**：同量子/數學批次的方法論——盤點未解決物理問題中
「局部最佳近似可直接入引擎」的技術，誠實標註採用/拒絕。

**狀態**：done（首批採用項已實作）

## 對照表

| 未解決問題 | 引擎可取的局部 | 裁決 |
|---|---|---|
| **N 體問題**（n≥3 無一般閉式解） | 既然只能數值積分，積分器品質就是全部——辛積分器（symplectic）長期保能量 | **採用** → `PhysicsWorld::SetIntegrator(VelocityVerlet)` |
| **湍流**（Navier–Stokes 千禧年難題，「古典物理最後未解之謎」） | 不可壓縮湍流的關鍵不變量是「無散度」——curl noise 用向量勢解析構造，散度嚴格為零 | **採用** → `MathUtils/CurlNoise.h`（`TurbulenceField`） |
| **決定性混沌 / Lyapunov 指數**（三體敏感度無解析刻畫） | 蝴蝶效應即「seeded 但不可預測」——本身就是引擎要的行為；seed 控制已內建 | **已涵蓋**——QuasiRandom seed 語義 + 本批測試斷言敏感度 |
| **玻璃轉變/阻塞（jamming）** | 顆粒/人群壓力無統一理論；可用行為級近似（擁擠→移速懲罰） | **採用** → `MathUtils/JammingModel.h` |
| **湍流間歇性**（耗散集中稀疏突發、異常標度指數未解） | 平滑場畫不出「一陣一陣」——對數正態式乘性調製給重尾突發強度 | **採用** → `MathUtils/GustField.h`（`GustField`） |
| **量子測量問題 / 量子重力** | 量子批次已用「測量=情報消費」玩法化 | **已涵蓋**於 Q-層 |
| **高溫超導機制** | 無對應玩法 | **拒絕** |
| **暗物質/暗能量** | 世界觀素材，非引擎技術 | **拒絕** |
| **時間之箭/熵** | 回放系統已有事件流 | **拒絕**——無新技術可得 |

## 首批採用項

### 1. Velocity Verlet 辛積分器

**未解決事實**：三體以上無閉式解，數值誤差隨時間累積。
Euler 族每步漏掉 ½a·dt² 項——自由落體、拋物線、軌道全部系統性偏差。

**採用理由**：常數加速度下 velocity Verlet **解析精確**；一般力場下
是二階辛積分器，能量誤差有界不發散（Euler 誤差線性累積）。
對引擎的實際意義：拋射物落點、長時間物理模擬（回放）更可信。

實作：`PhysicsWorld::SetIntegrator(SemiImplicitEuler | VelocityVerlet)`。
預設維持 Euler 不改行為；Verlet 走半踢-漂移-半踢。

### 2. 無散度湍流場（CurlNoise / TurbulenceField）

**未解決事實**：Navier–Stokes 光滑性未證；遊戲不需要也無法即時求解，
但「看起來像湍流」的核心是**無散度**（流不會無中生有）。

**採用理由**：取向量勢 ψ = Σ Aᵢ·sin(kᵢ·p + φᵢ·t)，速度場 v = ∇×ψ
解析求出——散度 ∇·v ≡ 0 **嚴格成立**（非數值近似）。波向量用
QuasiRandom 的塑性常數方向取樣，避免週期性條紋。

用途：戰場霧漂移、粒子環境運動、砲彈風偏、鏡頭微晃——
消費端由呼叫方注入（本批只交引擎能力）。

## 誠實性聲明

- 不聲稱解決 N 體或湍流——採用的是「該問題迫使數學界發展出的
  最佳近似技術」中適合即時的部分。
- Verlet 只在力場不依賴速度時辛；有阻尼/摩擦力時退為近似——
  引擎阻尼仍在 UpdateBodies 先施加，文件已註明。
- CurlNoise 是合成湍流外觀，不是流體求解器；不可壓力-速度耦合。

## 產出物

- `Physics/PhysicsSystem.{h,cpp}` — `IntegratorType` + `IntegrateVerlet`
- `MathUtils/CurlNoise.h` — 解析無散度湍流場（header-only）
- `Examples/PhysicsMathTest.cpp` — 落體精度、能量守恆漂移、
  散度為零、決定性驗證 → CTest

### 第二批（2026-09-21）：jamming 備選落地

- `MathUtils/JammingModel.h` — 密度→移速倍率（基本圖式近似，
  header-only）+ `CrowdDensity` 圓盤佔有率（含員額加權版）
- `Gameplay/Squad` — `crowdFactor` 乘進 `GetEffectiveSpeed`
- `Gameplay/BattleController::SetJamming(radius, params)` — 每 tick
  依鄰近小隊密度設定 crowdFactor；預設關閉保回歸
- `Examples/QuasiModelsTest.cpp`（引擎層，可同步上游）+
  `Examples/JammingBattleTest.cpp`（玩法層）→ CTest

擱置理由消失：當時「等 Squad 密度玩法需求出現」——斷橋原型
多隊擠渡口已是日常場景，故落地為 opt-in 機制。

### 第三批（2026-09-21）：湍流間歇性

- `MathUtils/GustField.h` — `GustField`（對數正態式級聯強度場
  `g = exp(σ·χ̂ − σ²/2)`，E[g]≈1、重尾突發、seeded 決定性；
  header-only）
- `Gameplay/BattleSceneSync::SetFogDriftGust` — 雲標記漂移量乘
  間歇強度（P-1 湍流漂移的乘性調製）；nullptr 預設=不調製
- `Examples/DuanqiaoPlayable` — 接 `fogGust`（sigma=0.7）
- `Examples/UnsolvedBatch3Test.cpp` — E[g]≈1/重尾/恆正/決定性/
  sigma=0 退化 → CTest

誠實性：g·v 非嚴格無散度（∇·(gv)=∇g·v）——用不變量換突發
外觀，嚴格無散度仍走 TurbulenceField 本體。

---

## 附錄：VelocityVerlet 採用決策（P-2，2026-09-17）

**消費點盤點**（`grep PhysicsWorld|PhysicsSystem|CreateWorld` 全倉掃描）：

| 消費點 | 性質 | 結論 |
|---|---|---|
| `Examples/PhysicsTest.cpp` | API 行為測試 | 維持 Euler——測的是預設路徑 |
| `Examples/PhysicsMathTest.cpp` | 精度對照 | 兩積分器皆覆蓋（本來就是它的用途） |
| 生產碼（Gameplay/IDE/Scene/Rendering） | **無任何 PhysicsWorld 消費者** | — |

**決策**：

- **預設維持 Euler**——沒有現存消費者可切換，`SetIntegrator` 保持 opt-in。
  改預設等於對未來呼叫端偷偷改語義，得不償失。
- **新消費者建議用 VelocityVerlet** 的情境：拋射物彈道（P-1 風偏）、
  回放敏感模擬（同 seed 下能量守恆讓漂移可預期）、長時間背景模擬。
- **語義注意**：Verlet 模式下 `GetLinearVelocity` 讀到半步相位
  （偏移 ≤½a·dt）；`ApplyImpulse` 作用於半步速度，效果等效。
  未來消費者若做「讀速度 → 顯示/判斷」應知道這是半步值。

**結論**：本批無程式碼切換——盤點完成、準則立好，待 P-1 或後續
拋射物功能落地時依此表採用 Verlet。
