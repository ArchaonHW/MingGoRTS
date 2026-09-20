# MingGoRTS 文檔索引

```
docs/
├── index.md          # 本索引
├── README.md         # 文檔導航
├── guides/           # 使用指南與建置說明
├── architecture/     # 架構設計與開發計劃
├── reports/          # 整合報告與稽核
├── business/         # 商業估算與成本收益分析
├── research/         # 研究文檔
└── archive/          # 歷史完成報告(已整合)
```

## 📚 指南 (guides/)

- **[BUILD_INSTRUCTIONS.md](./guides/BUILD_INSTRUCTIONS.md)** — 建置說明和步驟指南
- **[AI_AGENT_ENHANCEMENT_GUIDE.md](./guides/AI_AGENT_ENHANCEMENT_GUIDE.md)** — AI Agent 系統使用指南(代理類型、感知、記憶、工具、計畫系統)
- **[AI_AGENT_GUI_GUIDE.md](./guides/AI_AGENT_GUI_GUIDE.md)** — AI Agent GUI 整合指南
- **[MINGGORTS_IDE_USER_GUIDE.md](./guides/MINGGORTS_IDE_USER_GUIDE.md)** — IDE 使用指南(啟動說明、功能介紹)
- **[POTATO_ENGINE_OVERVIEW.md](./guides/POTATO_ENGINE_OVERVIEW.md)** — Potato Engine 專案總覽(原 PotatoEngine/README)
- **[POTATO_ENGINE_MIGRATION_GUIDE.md](./guides/POTATO_ENGINE_MIGRATION_GUIDE.md)** — UE5 移除遷移指南

## 🏗️ 架構 (architecture/)

- **[MINGGORTS_IDE_ARCHITECTURE.md](./architecture/MINGGORTS_IDE_ARCHITECTURE.md)** — IDE 架構設計(核心架構、AI 整合層、專門化代理)
- **[MINGGORTS_IDE_FULL_DEVELOPMENT_PLAN.md](./architecture/MINGGORTS_IDE_FULL_DEVELOPMENT_PLAN.md)** — IDE 完整開發計劃
- **[POTATO_ENGINE_ARCHITECTURE.md](./architecture/POTATO_ENGINE_ARCHITECTURE.md)** — Potato Engine 獨立架構設計(原 PotatoEngine/ARCHITECTURE)

## 📊 報告 (reports/)

- **[CONSOLIDATED_COMPLETION_REPORT.md](./reports/CONSOLIDATED_COMPLETION_REPORT.md)** — 整合完成報告:涵蓋引擎核心、AI 平台、智能開發系統、IDE 全開發歷程(取代 12 份原始報告)
- **[SECURITY_AUDIT.md](./reports/SECURITY_AUDIT.md)** — 安全稽核報告(2026-09-16)

## 💼 商業估算 (business/)

- **[PROJECT_COST_REVENUE_ESTIMATE.md](./business/PROJECT_COST_REVENUE_ESTIMATE.md)** — 專案總成本與總收益預估:三階段人力/外包/行銷成本、Steam 定價模型、銷量情境與回本門檻(約 1.5 萬套)

## 🔬 研究 (research/)

- **[QUANTUM_DEVELOPMENT_RESEARCH.md](./research/QUANTUM_DEVELOPMENT_RESEARCH.md)** — 量子計算開發研究
- **[REFERENCE_GAMES_MECHANICS.md](./research/REFERENCE_GAMES_MECHANICS.md)** — 參考作機制萃取(Total War/M&B/HOI4/ELIN)→ 引擎能力對照與佇列項
- **[UNSOLVED_MATH_TO_ENGINE.md](./research/UNSOLVED_MATH_TO_ENGINE.md)** — 未解決數學問題 → 引擎技術對照(採用/拒絕裁決)
- **[UNSOLVED_PHYSICS_TO_ENGINE.md](./research/UNSOLVED_PHYSICS_TO_ENGINE.md)** — 未解決物理問題 → 引擎技術對照(如辛積分器 VelocityVerlet)
- **[GBAA_BAAF_RESEARCH.md](./research/GBAA_BAAF_RESEARCH.md)** — GBAA 台灣區塊鏈會計查證白皮書研究:BAAF 三層架構、查證流程,及對 LedgerChain/HistorianReport 的設計啟發

## 🗄️ 歸檔 (archive/)

12 份原始開發完成報告,內容已整合至 `reports/CONSOLIDATED_COMPLETION_REPORT.md`,詳見 [archive/README.md](./archive/README.md)。

## 📐 BMAD 規劃產物(`_bmad-output/`)

遊戲層的規劃/設計/追蹤文件在 repo 根的 `_bmad-output/`:

- **[GDD](../_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md)** — 完整遊戲設計文件
- **[Epics & Stories](../_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md)** — Epic A–G + L/N/Q 層 story 清單
- **[產品簡報](../_bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md)** — 專案定位與決策(附 [addendum](../_bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/addendum.md))
- **[技術架構](../_bmad-output/game-architecture.md)** — 分層、橫切面、8 實作模式
- **[架構脊柱](../_bmad-output/planning-artifacts/architecture/architecture-MingGoRTS-2026-09-18/ARCHITECTURE-SPINE.md)** — 架構決策紀錄
- **[敘事設計](../_bmad-output/narrative-design.md)** — 章回結構、四手結局、雙層世界
- **[UX 規範](../_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/DESIGN.md)** — 12 條 UX-DR;行為面見 [EXPERIENCE](../_bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/EXPERIENCE.md)
- **[Sprint 狀態](../_bmad-output/implementation-artifacts/sprint-status.yaml)** — Epic A–G 進度追蹤
- **[引擎/遊戲拆倉計畫](../_bmad-output/planning-artifacts/epics-engine-split.md)** — 分 repo 規劃([工作計畫](../_bmad-output/planning-artifacts/engine-game-split-work-plan.md))
- **[Potato Engine Roadmap](../_bmad-output/planning-artifacts/potato-engine-roadmap.md)** — 引擎路線圖
- **研究報告** — [量子玩法建模](../_bmad-output/research/quantum-gameplay-modeling-2026-09-17/research.md) · [引擎技術調研](../_bmad-output/research/technical-latest-engine-techniques-2026-09-17/research.md)

## 🔗 相關鏈接

- [主 README](../README.md)
- [GitHub 倉庫](https://github.com/ArchaonHW/MingGoRTS.git)
- [問題報告](https://github.com/ArchaonHW/MingGoRTS/issues)

---

**🥔 MingGoRTS 文檔索引** — 最後更新:2026-09-21
