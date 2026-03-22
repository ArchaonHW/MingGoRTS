---
stepsCompleted: ["step-01-init", "step-02-discovery", "step-03-summary", "step-04-audience", "step-05-features", "step-06-nfr", "step-07-review", "step-08-complete"]
inputDocuments: [
  "c:\\HW\\MingGoRTS\\docs\\PROJECT_PLAN.md",
  "c:\\HW\\MingGoRTS\\docs\\domain_research.md",
  "c:\\HW\\MingGoRTS\\_bmad-output\\gdd.md",
  "c:\\HW\\MingGoRTS\\_bmad-output\\narrative-design.md",
  "c:\\HW\\MingGoRTS\\_bmad-output\\planning-artifacts\\epics.md",
  "c:\\HW\\MingGoRTS\\_bmad-output\\planning-artifacts\\ux-design-specification.md",
  "c:\\HW\\MingGoRTS\\_bmad-output\\project-context.md"
]
workflowType: 'prd'
documentCounts:
  productBriefs: 0
  research: 2
  brainstorming: 0
  projectDocs: 7
classification:
  projectType: 'Game (RTS - Real-Time Strategy)'
  domain: 'Gaming / Historical Strategy'
  complexity: 'High'
  projectContext: 'brownfield'
status: 'complete'
completedAt: '2026-03-22'
---

# Product Requirements Document - MingGoRTS / 民國史詩

**Author:** Potato
**Date:** 2026-03-22
**Version:** 1.0
**Status:** Complete

---

## 1. Executive Summary / 執行摘要

### 1.1 Product Name / 產品名稱
**民國史詩 (MingGoRTS)**

### 1.2 Core Concept / 核心概念
民國史詩是一款**歷史決策後果體驗遊戲**，核心是讓玩家在真實的民國歷史情境中做出策略決策，並親身體驗這些決策帶來的歷史後果。遊戲巧妙融合了四大經典策略遊戲（全軍破敵、騎馬與砍殺2、鋼鐵雄心、Elin）的精髓，創造出獨一無二的**民國時期大戰略體驗**。

### 1.3 Target Platforms / 目標平台
- **Windows PC** (主要平台)
- **Android / iOS** (移動平台)
- 跨平台同步支持

### 1.4 Unique Selling Points / 獨特賣點

**🎯 第一原理核心：**
- **決策真實性**：每個遊戲決策都基於歷史真實選擇
- **後果體驗**：玩家直接感受決策的歷史重量
- **人性理解**：通過遊戲理解歷史中的人性掙扎
- **時代還原**：還原民國時代的真實複雜性

**🏗️ 設計哲學：**
- **簡化但深刻**：去除不必要的複雜，保留核心的歷史體驗
- **技術為體驗**：所有技術選擇都服務於歷史還原
- **規模有意義**：大規模是為了還原歷史的真實感
- **教育娛樂並重**：在娛樂中自然傳達歷史理解

---

## 2. Functional Requirements / 功能需求

### 2.1 Core Game Systems / 核心遊戲系統

| ID | 需求 | 描述 | 驗收標準 | Epic |
|----|------|------|----------|------|
| FR1 | 四層策略系統 | 實現四個層次的策略系統（戰略、戰術、個人、建造） | 四層均可正常運作 | Epic 5 |
| FR2 | 大規模RTS戰鬥 | 支持1000+單位同時戰鬥 | 保持60 FPS，支持複雜地形 | Epic 2 |
| FR3 | 歷史決策後果 | 基於真實民國歷史的決策系統 | 每個決策都有歷史依據和後果體驗 | Epic 1 |
| FR4 | 角色成長系統 | 角色技能和發展系統 | 技能提升影響戰鬥和策略表現 | Epic 3 |
| FR5 | 基地建設系統 | 基地建造和資源管理 | 豐富的建造、升級、資源管理 | Epic 4 |
| FR6 | 動態歷史事件 | 歷史事件觸發和決策 | 根據玩家決策動態觸發事件 | Epic 1 |
| FR7 | 多兵種協同 | 多兵種協同作戰系統 | 兵種克制、協同、戰術配合 | Epic 2 |
| FR8 | 外交聯盟系統 | 外交和政治聯盟 | 動態外交關係和聯盟機制 | Epic 5 |
| FR9 | 科技樹研發 | 科技樹研發系統 | 深度科技樹，有先決條件 | Epic 4 |

### 2.2 Supporting Systems / 支持系統

| ID | 需求 | 描述 | 驗收標準 | Epic |
|----|------|------|----------|------|
| FR10 | 跨平台適配 | PC、Android、iOS支持 | 核心邏輯統一，UI適配不同平台 | Epic 6 |
| FR11 | AI資產生成 | AI輔助美術和音樂生成 | 集成Stable Diffusion和AIVA | Epic 5 |
| FR12 | 敘事對話系統 | 敘事整合和對話 | 多選項對話，影響後續事件 | Epic 3 |
| FR13 | 勝利失敗條件 | 勝利/失敗判定系統 | 評估歷史決策路徑，判定結局 | Epic 1 |
| FR14 | 難度漸進 | 初級、中級、高級 | AI難度調整，教程提示 | Epic 2 |
| FR15 | 歷史人物扮演 | 歷史人物扮演和互動 | 扮演真實歷史人物，體驗其決策 | Epic 3 |

---

## 3. Non-Functional Requirements / 非功能需求

| ID | 類別 | 需求 | 目標 |
|----|------|------|------|
| NFR1 | 性能 | 60 FPS性能目標（支持1000+單位） | 最低60 FPS |
| NFR2 | 兼容性 | 跨平台兼容性（Windows、Android、iOS） | 所有平台功能一致 |
| NFR3 | 架構 | 模組化插件架構支持獨立開發 | 五個插件獨立編譯 |
| NFR4 | 通訊 | 事件驅動通訊確保鬆散耦合 | MingCore事件總線 |
| NFR5 | AI | AI資產管道集成（SD、AIVA） | 自動生成民國風格資產 |
| NFR6 | 網絡 | 服務器驗證和客戶端預測架構 | 防止作弊 |
| NFR7 | 教育 | 歷史真實性和教育價值 | 歷史內容經過驗證 |
| NFR8 | 安全 | 安全性和反作弊系統 | 多層次安全防護 |

---

## 4. User Experience Requirements / 用戶體驗需求

| ID | 需求 | 描述 | 對應FR |
|----|------|------|--------|
| UX-DR1 | 歷史風格UI | 歷史風格的UI設計系統 | FR3, FR12 |
| UX-DR2 | 多層次界面 | 戰略、戰役、角色、建造界面 | FR1 |
| UX-DR3 | 雙重輸入 | 觸控和鼠標雙重輸入支持 | FR10 |
| UX-DR4 | 歷史信息展示 | 歷史信息展示和教育內容整合 | FR3, FR6 |
| UX-DR5 | 大規模戰鬥可視化 | 大規模戰鬥的可視化清晰度 | FR2, FR7 |
| UX-DR6 | 決策後果反饋 | 決策後果的視覺反饋系統 | FR3 |

---

## 5. Requirements Traceability Matrix / 需求追蹤矩陣

### 5.1 FR to Epic Mapping

| FR | Epic 1 | Epic 2 | Epic 3 | Epic 4 | Epic 5 | Epic 6 |
|----|--------|--------|--------|--------|--------|--------|
| FR1 | | | | | ✅ | |
| FR2 | | ✅ | | | | |
| FR3 | ✅ | | | | | |
| FR4 | | | ✅ | | | |
| FR5 | | | | ✅ | | |
| FR6 | ✅ | | | | | |
| FR7 | | ✅ | | | | |
| FR8 | | | | | ✅ | |
| FR9 | | | | ✅ | | |
| FR10 | | | | | | ✅ |
| FR11 | | | | | ✅ | |
| FR12 | | | ✅ | | | |
| FR13 | ✅ | | | | | |
| FR14 | | ✅ | | | | |
| FR15 | | | ✅ | | | |

---

## 6. Sprint Planning / Sprint規劃

### Sprint 1: Core Loop / 核心迴圈
- 單位控制（點選、框選、指揮）
- 資源採集系統
- HUD資源和單位信息顯示

### Sprint 2: Interaction & Conflict / 互動與衝突
- 基礎戰鬥（攻擊、生命值、動畫）
- 建築系統（建造、解鎖）
- EventManager和TriggerManager

---

## 7. Document Status / 文檔狀態

- **PRD Generated:** 2026-03-22
- **Status:** Complete
- **Total FRs:** 15
- **Total NFRs:** 8
- **Total UX-DRs:** 6
- **All requirements traced to Epics:** ✅

**Next Steps:** Proceed to implementation with complete PRD
