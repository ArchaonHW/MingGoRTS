# Design Log

## Current

- Spec: `_bmad-output/implementation-artifacts/spec-ide-dev-assistant.md` (status: approved 2026-09-17, baseline d5eff75)
- Branch: `feat/gameplay-doctrine-battle`（dev-assistant 工作來自 `feat/engine-security-hardening` 系列 commit）
- Activity: [T] Acceptance Testing — 2026-09-17 重跑驗證 PASS（smoke 18/18、ctest 12/12、banned 0），已簽核 SO-001
- 立繪產線（2026-09-17）：`PortraitRenderer`（VRM→PNG 蒙皮/A-pose/打光/批量/labels.csv）+ `PortraitBaker`（引擎內 2D 分層卡面，取代 PIL）；`EnemyGeneral` 補 art/faction/rarity/id 解析、`RosterEntry.art` 欄位；ctest 28/28 PASS

## Progress

### 2026-09-17 — Evolution: Card Gallery（武將名冊）立繪上屏
- IDE 新增 Card Gallery 面板（View menu）：掃描 assets/cards/**/*.json，
  左列清單（缺圖紅標）+ 右側立繪/metadata；`art` 空時按
  `cards/art/<id>.png` 慣例 fallback
- ImageCodec→GL texture lazy cache + Shutdown 釋放；「重新產生立繪」
  按鈕背景呼叫 PortraitBaker 完成後 invalidate texture
- 附帶修正：NeuralGraphicsTest SR 門檻 flake（1200 epoch 不足，
  27.85 < bilinear 28.07dB）→ 4000 epoch 後 41.34dB 穩定通過
- 驗證：MSVC Release 建置乾淨、ctest 31/31 PASS、banned 掃描 0
- 範圍/計畫：`_bmad-output/_progress/evolution/card-gallery/scope.md`

## Design Loop Status

| Scenario slug | NN.X | Page name | Status | Date |
|---|---|---|---|---|
| ide-dev-assistant | - | IDE Intelligent Development Assistant | approved | 2026-09-17 |
| ide-dev-assistant | - | IDE Intelligent Development Assistant | approved | 2026-09-16 |

## Verification Evidence (2026-09-16)

- `DevAssistantSmoke` MSVC Release build clean；14 assertions ALL PASS
- `MingGoRTS_IDE_GUI` MSVC Release build clean（僅既有 warnings）
- Banned-function grep（`gets|strcpy|strcat|sprintf|vsprintf|scanf`）MingGoRTS_IDE/ AI/：0 new matches
- 修正：`CopyToBuffer` 的 `strncpy`/`strncat` 在 `/sdl` 下為 C4996 error → 改 `memcpy` + 精確長度
- MinGW 未安裝於本機，僅驗證 MSVC 路徑（spec acceptance 為「MSVC or MinGW」）
- 迴歸：`AITestSuite` 7/7 PASS、`SecurityRedTeamTest` 39 PASS / 0 BYPASS
- 注意：build cache 由 scoop cmake 4.4 生成，`cmake --build` 需用同一把（VS bundled 4.2 會因模組路徑混用在 reconfigure 時失敗）

## Spec Deviations

- `CopyToBuffer` 使用 `memcpy` + 精確長度而非 `strncpy`/`strncat`：`/sdl` 將 C4996 視為 error，且 CI banned-function 掃描本就禁 `strncpy`。行為等價（NUL 結尾 + `...[truncated]` 標記）。

## PR Description（草稿）

**feat(ide): wire Development Assistant to in-process codegen pipeline**

- Summary：IDE Development Assistant 改由引擎自家 pipeline 驅動——NLP 意圖解析 → KnowledgeGraph 專案上下文 → 模板化 C++ 合成，離線可用不需外部 LLM；外部 LLM client 保留為可選 fallback。
- Changes：`AI/IntelligentDevelopmentSystem.*`（`GenerateLocal`/`ParseIntent`/模板表）、`IDEGUI.cpp/h`（`std::async` 非同步生成 + `PollDevelopmentResult` 每帧寫回、`developmentProcessing` 防重入、`POTATO_LLM_*` 環境變數預設 + Settings UI）、`Examples/DevAssistantSmoke.cpp`（無 LLM 的 CLI 驗證）、`CMakeLists.txt`（新 target）
- Testing：`cmake --build build --target DevAssistantSmoke` → 14 assertions；GUI：Ctrl+D → prompt → response view 出現程式碼且 UI 不卡
- Deviations：`memcpy` 取代 `strncpy`（/sdl 相容）

## Acceptance Criteria

- [x] 無外部 LLM 時，可識別 prompt → response view 出現合成 C++（smoke 驗證本地管線；UI async poll 寫回）
- [x] 不可識別 prompt → 明確錯誤訊息，不 hang/crash/改 editor（smoke 驗證）
- [x] MSVC `cmake --build` 通過；無新增 banned function

---

# E4-3 斷橋可玩 demo + E4-1 overlay 收尾(2026-09-17,commit d4eb1e4)

## Review 結果(三路:blind/edge-case/acceptance)

**已修 patch:**
- `MakeBox` 面表抄錯三頂點(+X/-X/-Y 面非平面)→ 對照 BattleRenderDemo 正確版修正(critical)
- `Intervene` 兩重載不拒 routing 小隊 → CP 被扣但指令同幀被強制撤退覆寫 → 加 `IsRouting()` 拒絕
- squad 目標 Intervene:加目標驗證(自己/同隊/潰逃敵軍拒絕;非 Engage 退化為位置指令)
- `SetOverlayMeshes` 契約補完:nullptr 真拆節點、重設 mesh/barY/barWidth 更新既有節點、Detach 清 selected
- 選取環 y 0.05→0.22(原位置在橋板體積內被遮擋);血條平躺→直立 XY(俯角下原只剩 ~2px)
- squad 包圍球放大至涵蓋 overlay(防 hierarchical culling 提早剔除血條)
- `BattlePicker::PickSquad` 略過 routing 小隊
- demo:`SetCommandPoints(0,5)` 移到 ApplyPlan 後(否則被建議值 2 覆寫);eventLog 先於 battle 宣告、SetEventCallback 移到 CreateSquad 前(早期事件不再丟);HiDPI 游標×fb/win 比例 + 視窗外不點選;滾輪 gating `!io.WantCaptureMouse`;L/R 鍵獨立處理;字型 fallback;早退走 shutdownAll;`<algorithm>`/`GLFW_INCLUDE_NONE`

**defer/reject:** interventionUntil 語義兩重載一致(皆剩餘秒數,非 deadline)——非 bug;暫停下可下單為 RTS 慣例保留;interventionUntil 不清死鍵(append-only,無害)

## Spec deviation
- overlay 掛 squad 節點子層(非 spec 原註的 parentNode):子層自動跟隨、squad 不旋轉無繼承問題;Design Notes 已改述

## 驗證
- MSVC:DuanqiaoPlayable/BattleSceneTest/BattlePickerTest 建置過;ctest 4/4(SceneTest 16 PASS 含新 nullptr 拆除斷言)
- MinGW:DuanqiaoPlayable/BattleSceneTest 建置過;SceneTest 16 PASS

## Commit 拆分
- 本工作單獨 commit d4eb1e4(pathspec 提交);CMakeLists 其餘 hunks(NeuralNetwork/AIAgentSmoke/QuasiRandomTest 等)屬平行 session 工作,未捲入
- 落地分支為 feat/game-backend-services(平行 session 已切換原 gameplay 分支)

---

# Q-1 QuantumFog 接入斷橋(2026-09-17,commit aa51f42)

## 機制

敵軍開戰即疊加態:6 候選格機率雲(藍噪散佈,先驗偏向南岸敵營,雲心朝進攻方向偏 1.5 格不洩真值)。揭露雙路徑:情報點觀測(intel=4/cost=1/時效 25s)或小隊接觸(3 格內,免費且刷新時效)。負面觀測:目視覆蓋候選格但無人→消去該候選,雲縮小。

## Review 修復(2 路:blind + acceptance)

- Scout 卡死(走到 modal 格但真身不在範圍→永遠 Hold)→ 負面觀測讓雲隨偵查縮小,Scout 自然推進
- 情報洩漏:doctrine 感知(EnemyInRange/outnumbered)與 Engage 用真實位置 → IsHiddenByFog 全面閘控
- 雲心=真實出生點洩漏真值 → suspectedCenter 偏移
- Detach/SetFog 不清 fog 指標與 fogNodes → 補清
- marker 子節點超過篩選後雲大小 → 逐個 SetActive
- 接觸揭露 Emit 改為迭代外批次發送(callback reentrancy 防禦)且僅新揭露才發
- BindFogSquad 驗證 entityId;EnemyGeneral 補 Scout 字串映射;HUD 未揭露數跳過全滅

## defer

- 已 Engage 中的敵軍回雲後 order 不清空(接觸再現會重新揭露,自癒)
- 全滅但未揭露的 entity 殘留(不影響顯示)

## 驗證

MSVC+MinGW 建置過;QuantumFogBattleTest 17 PASS(觀測扣點/重複不扣/時效回雲/接觸免費/Scout 目標/負面觀測/無 fog 退回);BattleSceneTest 22 PASS;既有測試無回歸。

---

# 2026-09-17 — Q-3 弱觀測 Probe(18f48b3)

## 做法

`QuantumFog::Probe(id, truePos, strength)`:扣 probeCost(建構子第三參數,demo=1 < observe=2),候選機率軟重加權 `p_i' ∝ p_i·((1-s)+s·w_i/wMax)`、`w_i=1/(1+d_i²)`——雲向真值收縮但不塌縮,revealed 不變,不寫 priors(軟情報隨時效/退相干衰減,與 EliminateCandidate 的硬證據分層)。demo:LMB 點雲=探測(1 情報)、Shift+LMB=觀測(2 情報),情報池 4→8。

## Review 修復(1 路 edge+acceptance)

- NaN strength 穿透 `<=0` 防護 → `!(strength>0)`;建構子 clamp 非負費用(負費用會反產情報)
- 測試:comma-expression 改明確 CHECK、失敗探測逐候選比較、補 strength>1 clamp 與連續探測收斂斷言
- demo 戰報帶最高機率百分比;header 操作說明更新
- 順帶修 `AgentGUI` unique_ptr copy(7cefc54,MinGW 建置阻斷)

## 驗證

MSVC+MinGW 建置過;QuantumTest +20 checks(單調收斂/逼近塌縮仍不揭露/歸一化/費用<觀測/失敗不變/已揭露不扣點);6 target ctest 全過。

---

# 2026-09-17 — Q-2 糾纏小隊(db85b7a)

## 做法

`QuantumFog::Entangle(a,b)`:1:1 雙向糾纏,以雲質心相對偏移最近鄰建 corr 表(同向機動假設——佯攻偏其雲心北→主力也偏北)。任一方經 Observe/Reveal/ObserveRandom 塌縮時,未揭露對象的機率 blend 0.7 向相關候選集中——「推測」非「親見」,revealed 維持 false。Probe 不傳遞(弱測量不構成 joint 測量);已消去候選不復活(硬證據優先);情報只扣被觀測方。

## Review 修復

- **major**:Reveal 被接觸迴圈每 tick 呼叫 → blend 複利,0.1s 內對方雲免費收斂 ~1.0 → Reveal 比照 Observe 加已揭露早退(仍刷新時效),糾纏只在塌縮轉換傳遞一次;ObserveRandom 同
- 測試補:接觸傳遞、重複接觸不複利、Probe 不觸發、消去候選不復活、對稱觀測
- demo Entangle 失敗加 log

## 驗證

MSVC+MinGW 建置過;QuantumTest Entangle 段落 ~20 checks;ctest 6/6。平行 session 的 DuanqiaoPlayable hunks(A-1/A-2)被其 commit 捲入先行,本 commit 僅 fog 核心+測試+spec。

---

# 2026-09-17 — Q-4 敵將人格→先驗(67dde3d)

## 做法

`EnemyGeneral::FogBiasPoint(center, enemyDir, radius)`:侵略沿敵向偏移 (a−0.5)·1.6r,狡詐沿垂直側翼 (c−0.5)·0.8r,50=中立;`FogPriorScale()` = 1.4−0.8·d 縮放先驗高斯 σ(紀律高→雲集中)。`AddEntityCloud` 加 `priorScale` 尾參(向後相容)。demo 格洛克(侵略90)四朵敵雲偏北岸前線,southCamp 場景偏置由人格偏置取代。

## Review 修復(1 路,全 Low)

- `AddEntityCloud`/`Qudit::SetProbabilities` 的 `total<=0` 擋不住 NaN → `!(total>0)`
- 人格軸 LoadFromString 不 clamp → 越界卡產生極端/靜默錯誤偏置 → clamp 0~100
- 測試補:同 seed 佈局前提顯式斷言、狡詐軸斷言、frontMass 綁 center.y

## 已知非我事項

- 平行 session 未提交的 QuantumFogBattleTest「[2b] 揭露位置更新為新真值」deterministic FAIL(敵軍 4s 內走不到 (15,9),停在 y=8.6);語義上與我的 Reveal 早退無關(revealedPos 刷新行為相同),屬其測試時序假設錯誤
- 平行 session 的 Q-6 fog 存檔/Emit 程式碼混入本 commit 的 QuantumFog.h/.cpp(同檔 hunks,pathspec 無法分離),建置+測試已驗過

## 驗證

MSVC+MinGW 建置過;QuantumTest Personality 段落 ~12 checks;其餘 ctest 無回歸。

---

# 2026-09-18 — Q-7 相位干涉觀測(c3b026d + ae83a4e 修復)

## 做法

實驗開關 `SetInterferenceEnabled`(預設關):同 tick(fogTime 未推進)對已揭露實體二次觀測 → Δφ = phases[nearest]−phases[modal];cos<0 觀測被拒不刷新,cos≥0 刷新+時效 1.5x。`SetEntityPhases` 注入候選相位(同步寫進 Qudit 振幅);`Qudit::SetPhases` 為乘法旋轉,機率語義不變。Reveal/Probe 不干涉(接觸是物理目視、弱測量不成 joint 觀測)。

## Review 修復(1 路)

- 過期退相干沒清 entity.phases → 過期後干涉仍生效,與 spec「decoherence 殺相位」矛盾 → expiry 清 phases+lastObserveAt
- SetPhases 重複注入相位累積 → 注入前先 SetProbabilities 歸零相位
- NaN 相位 → cos(NaN)<0 為 false → 白拿建設性加成 → isfinite 拒絕 + `!(c>=0)` 擋 NaN
- 空相位注入變全零相位 → 恆建設性漏洞 → 空輸入=清除
- ObserveRandom 沒蓋干涉時鐘;Update 負/NaN dt 會倒轉時鐘

## 驗證

MSVC+MinGW 全過;干涉段落 22 checks(雙向/跨 tick/flag 關閉零回歸/相位清零/機率語義不變)。平行 session 將實作掃入 c3b026d,本 session 的 review 修復另立 ae83a4e。

## 佇列態

平行 session 極活躍:Q-5/Q-6/Q-8、A-1~A-3、C-1~C-3、T-9~T-11 皆已落地,Q-9 教學進行中(TutorialScript 檔案在途)。剩 G-1~G-10(TW/HOI4 戰鬥機制)與 P-1。
