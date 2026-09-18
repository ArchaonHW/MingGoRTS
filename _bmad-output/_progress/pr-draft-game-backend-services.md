# PR Draft: feat/game-backend-services → main

## Title
```
feat: gameplay battle layer, VRM portrait pipeline, engine hardening
```

## Body

### Summary

大型功能分支（58 commits）：玩法戰鬥層、立繪產線（2D/3D）、
IDE 智能開發助理、安全紅隊強化、引擎正確性修復。

### 主要功能

**Gameplay（斷橋原型戰鬥層）**
- Doctrine 規則冷卻 + BattlePlanner AI 參謀 + 可玩 DuanqiaoDemo
- SageCommand 三權指揮層（五行政策 + 異端封印）
- BattleMap/Roster/BattleRecorder/BattleSceneSync、敵將人格腳本、
  screen-to-world picking、敵軍破壞政策 + 潰退重整
- QuantumFog：statevector 量子模擬 + 疊加態戰爭迷霧

**立繪產線**
- PortraitRenderer：VRM/glTF → 蒙皮渲染 → PNG（A-pose 啟發式放鬆、
  Lambert+rim 打光、head/half/full 取景、批量+labels.csv 標註）
- PortraitBaker：引擎內 2D 分層卡面產生器（ImGui stb_truetype
  點陣化，取代 Python/PIL 依賴）
- IDE Card Gallery 面板：角色卡瀏覽 + 立繪上屏 + 一鍵重 bake
- EnemyGeneral/RosterEntry 補 art/faction/rarity 欄位

**渲染**
- 蒙皮 mesh + glTF 動畫 + 姿勢編輯 + 惰性 GPU 上傳
- VRM humanoid/morph/spring/MToon 支援 + 內建 MToon shader
- 無第三方依賴 PNG 編解碼（ImageCodec）
- SynthDataDemo：渲染 → 標註資料集 → NN 訓練端到端

**IDE / AI**
- Development Assistant 由本地 codegen 管線驅動（NLP 意圖 →
  KnowledgeGraph → 模板合成），外部 LLM 為可選 fallback
- 智能建議系統非同步化、DevSystem mutex 保護
- GuiTextUtils 抽出 + 截斷路徑無頭測試

**安全**
- user-mode anti-cheat 偵測 16 項檢查：隱藏模組 VAD 掃描、
  RWX 私有記憶體、IAT hook、syscall stub 稽核、外部 handle、
  執行緒 RIP、已知工具行程、監控心跳等；紅隊測試 39 PASS/0 BYPASS

**正確性修復（deferred backlog 全清）**
- ModelLoader OBJ 越界/quad 三角化/真實 tangent
- CV adopt_lock double-unlock UB、GLFWSharedContext UAF
- Physics kinematic 雙機制統一、bodyB callback
- AudioSystem LoadWAV 驗證、Serialization slot sanitize
- Camera viewport 守衛、input keymap 補齊

### Testing

- `ctest -C Release`：**31/31 PASS**（新增 ModelLoader/Input/Audio/
  Time/GLSmoke/VrmLoad/NeuralGraphics/SageCommand/Quantum/
  QuasiRandom/BattlePicker/AIAgentSmoke 等）
- SecurityRedTeamTest：39 PASS / 0 BYPASS
- MSVC Release 全量建置乾淨；banned-function 掃描 0 命中

### Notes

- 不含工作樹中未提交的 NeuralGraphics 源碼、spec-suggestion-async
  等進行中工作（45 個檔案留待後續 commit）
- `tools/generate_roc_cards.py` 保留為參考產線；引擎內 PortraitBaker
  為正式路徑
- 外部相依：僅 vendored `external/`（tinygltf/imgui/glfw），
  無新增第三方函式庫
