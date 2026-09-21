# 未解決數學問題 → 引擎技術對照研究

**目的**：盤點著名未解決/半解決數學問題中「局部可落地」的技術，決定哪些
引入 Potato Engine、哪些誠實拒絕。方法論同量子批次：取數學結構，不取
硬體或純理論炫技。

**狀態**：done（首批採用項已實作）

## 對照表

| 未解決問題 | 引擎可取的局部 | 裁決 |
|---|---|---|
| **最優低差異序列**（star-discrepancy 下界/Korobov 猜想仍開放） | 準隨機序列在取樣上嚴格優於 PRNG：R2 黃金比例序列、Halton、van der Corput | **採用** → `MathUtils/QuasiRandom.h` |
| **球體堆積/覆蓋**（高維 Kepler 問題、Poisson 覆蓋數仍未解） | Bridson Poisson-disk 取樣=2D 藍噪近似最密排列 | **採用** → `QuasiRandom::PoissonDisk` |
| **Navier–Stokes 存在與光滑性**（千禧年難題） | 即時流體只能離散近似（格子法/stable fluids），無法也無需證明光滑性 | **拒絕**——現階段無流體需求；水流用 shader 動畫即可 |
| **P vs NP / TSP** | 尋路是圖上最短路（多項式可解）；TSP 只出現在巡邏路線 | **拒絕**——flow-field 已最優；TSP 需求不存在 |
| **N 體問題**（n≥3 無一般解析解） | 物理本就走數值積分；剛體模擬不吃解析解 | **拒絕**——無新技術可得 |
| **黎曼猜想/素數分佈** | 素數雜湊、goldilocks seed 擴散已有更好工程解（xxHash 類） | **拒絕**——無 gameplay 價值 |
| **Collatz 猜想** | 停滯時間可作確定性雜湊產生地形特徵 | **採用** → `MathUtils/CollatzHash.h` |
| **最優 Golomb 尺**（n≤27 才證得最優；一般 n 無閉式解） | 全異距刻度=採樣抽頭去相關/編隊槽位去對稱；內建已驗證最優表（n≤12），大 n 貪心保性質不保最優 | **採用** → `MathUtils/GolombRuler.h` |
| **量子混沌/譜統計**（RMT–黎曼零點連結） | 量子批次的退相干/機率雲已在走 | **已涵蓋**於量子批次 Q-層 |

## 首批採用項（本研究直接產出）

### 1. 準隨機序列（Quasi-Random）

未解決事實：d 維 star-discrepancy 的最優漸進常數仍未知；Roberts 2018
的 R2/R3（塑性常數泛化黃金比例）是實務上最佳無表查序列之一。

引擎用途：

- **疊加態候選格生成**：QuantumFog 候選點改為藍噪散佈，取代手排清單
  → `QuantumFog::AddEntityCloud`（本批實作）
- 未來：軟陰影/AA jitter、粒子初始分佈、合成資料取樣（SynthDataDemo）

### 2. Poisson-disk 藍噪取樣

未解決事實：圓盤最密堆積在受限區域的最優半徑–數量關係無閉式解；
Bridson 2007 給出 O(n) 近似，遊戲業界標準。

引擎用途：候選格散佈保證最小間距（機率雲不重疊糊在一起）、
未來可用於關卡道具/敵軍佈點。

## 誠實性聲明

- 「解決未解決問題」指的是**把問題局部已知的最佳近似技術引入引擎**；
  本研究不聲稱證明任何開放問題。
- 準隨機序列有確定性偏差（低維投影有結構）——需 jitter 時由呼叫方
  加 PRNG 擾動，文件已註明。

## 產出物

- `MathUtils/QuasiRandom.h` — VanDerCorput / Halton / R2 / R3 / PoissonDisk
- `Gameplay/QuantumFog` — `AddEntityCloud`（藍噪候選格生成）
- `Examples/QuasiRandomTest.cpp` — headless 驗證（差異度比較、最小間距、
  種子決定性）→ CTest

### 第二批（2026-09-21）：Collatz 備選落地

- `MathUtils/CollatzHash.h` — `CollatzStoppingTime`（熔斷+溢位摺疊，
  不假定猜想成立）/ `CollatzField01`（重尾場值 [0,1]）/
  `CollatzFeature`（特徵桶）；header-only
- `Gameplay/MapGenerator` — `Config.featureThr`：Collatz 場稀有高值格
  →「古林深處」特徵（type=forest，守方減傷）；<=0 關閉
- `Examples/QuasiModelsTest.cpp` — 已知停滯值/熔斷/重尾分佈/值域 → CTest

擱置理由消失：當時「無明確消費者」——G-10 MapGenerator 落地後
程序戰場需要「非週期、稀有、確定性」的特徵點來源，正是停滯時間
分佈的形狀。

### 第三批（2026-09-21）：Golomb 全異距尺

- `MathUtils/GolombRuler.h` — `OptimalGolombRuler`（n≤12 已驗證
  最優表，長度對 OEIS A079426）/ `IsGolomb`（全異距驗證）/
  `GolombMarks`（n>12 貪心：保全異距不保最優長度）/
  `FormationOffsets`（黃金角×尺半徑螺旋槽位）；header-only
- `Examples/UnsolvedBatch3Test.cpp` — 尺表逐階驗證（刻度數/全異距/
  最優長度）、貪心性質保持、槽位半徑全異、退化輸入 → CTest

用途：任意兩對刻度間距不重複 → 採樣抽頭無共振條紋、編隊槽位
去同心環對稱。誠實性：貪心尺長非最優（n=13 貪心=147>最優106），
緊緻性敏感者自查 `GolombMarks(n).back()`。
