# MingGoRTS 文檔索引

```
docs/
├── index.md          # 本索引
├── README.md         # 文檔導航
├── requirements.md   # 核心專案需求規格書 ⭐ 新增
├── design.md         # 核心專案技術設計文檔 ⭐ 新增
├── guides/           # 使用指南與建置說明
├── architecture/     # 架構設計與開發計劃
├── reports/          # 整合報告與稽核
├── research/         # 研究文檔
└── archive/          # 歷史完成報告(已整合)
```

## 📋 核心規格書

- **[requirements.md](./requirements.md)** ⭐ — MingGoRTS 核心專案需求規格書 (46 項需求、460 條驗收標準)
  - 遊戲核心機制 (Doctrine、戰鬥、量子霧霾、名冊、錄製、帳本、治理等)
  - PotatoEngine 引擎架構 (核心、事件、資源、場景、渲染、物理等)
  - AI 與機器學習 (Agent、神經網路、強化學習、NLP、LLM、RAG等)
  - MingGoRTS IDE (核心架構、GUI、國際化、智能建議、模板系統)
  - 非功能性需求 (效能、可測試性、安全性、可維護性、相容性、文檔)

- **[design.md](./design.md)** ⭐ — MingGoRTS 核心專案技術設計文檔
  - 系統架構設計 (分層架構、組件圖、執行緒模型)
  - 核心模組設計 (PotatoEngine、Event System、Resource Manager、Serialization等)
  - 戰鬥層設計 (BattleController、Doctrine、QuantumFog、BattleRecorder、Ledger等)
  - 資料模型與JSON Schema
  - API設計與資料流
  - 效能設計與安全設計

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

## 🔬 研究 (research/)

- **[QUANTUM_DEVELOPMENT_RESEARCH.md](./research/QUANTUM_DEVELOPMENT_RESEARCH.md)** — 量子計算開發研究

## 🗄️ 歸檔 (archive/)

12 份原始開發完成報告,內容已整合至 `reports/CONSOLIDATED_COMPLETION_REPORT.md`,詳見 [archive/README.md](./archive/README.md)。

## 🔗 相關鏈接

- [主 README](../README.md)
- [GitHub 倉庫](https://github.com/ArchaonHW/MingGoRTS.git)
- [問題報告](https://github.com/ArchaonHW/MingGoRTS/issues)

---

**🥔 MingGoRTS 文檔索引** — 最後更新:2026-10-02
