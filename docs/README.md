# MingGoRTS 專案文檔總覽

本文檔目錄包含 MingGoRTS 遊戲專案的所有技術與設計文件，已按功能分類整理。

---

## 目錄結構

### 📁 01_Project_Management/ (專案管理)
專案進度、里程碑與規劃文件
- **Epics/** - Epic 完成報告 (EPIC_1_1 至 EPIC_8_1)
- **Sprints/** - 週期開發報告與衝刺計畫
- 專案狀態、風險分析、發布說明

### 📁 02_Development_Guides/ (開發指南)
開發人員技術文件與 API 參考
- **Setup/** - 快速開始、環境設定、安裝指南
- **API/** - API 文件、系統介面、版本控制指南
- 開發者指南、編碼規範、工具使用

### 📁 03_Game_Design/ (遊戲設計)
遊戲玩法與系統設計文件
- **Factions/** - 12 派系設計、北洋/國民/共產軍閥等勢力設定
- **Mechanics/** - 機制實作、歷史事件系統、AI 行為設計
- 歷史系統、創新系統、決策系統

### 📁 04_Art_Assets/ (美術資源)
美術與音訊資產製作規範
- **Production/** - 3D 資產製作、單位設計、技術規格
- **Audio/** - 音樂實作、音效設計、音訊整合
- UI 設計、材質規範、VR/AR 風格指南

### 📁 05_Technical/ (技術實作)
系統實作與技術文件
- **System_Implementation/** - 整合測試、系統串接、實作指南
- **Optimization/** - 效能優化、EPIC2 優化、性能指南
- 架構設計、部署文件、安全掃描

### 📁 06_Testing/ (測試驗證)
測試計畫與品質保證
- **Verification/** - 測試驗證、覆蓋率計畫、整合測試報告
- 弱點掃描、壓力測試、Beta 測試指南

### 📁 07_User_Documentation/ (使用者文件)
終端用戶說明文件
- 使用者手冊、操作指南、FAQ

### 📁 08_Tools_Automation/ (工具與自動化)
開發工具與自動化腳本文件
- **api/** - API 工具指南
- **automation/** - 自動化系統文件
- **build/** - 建置工具說明

### 📁 api/ (API 技術文件)
版本控制與 API 系統文件
- 分支管理指南、BMAD 系統、Sage 指令 API

### 📁 UE5_Official_Documentation/ (UE5 官方文件)
Unreal Engine 5 學習資源
- AI、動畫、音訊、渲染、網路等主題

### 📁 UE5_Learning/ (UE5 學習報告)
學習進度與研究報告

---

## 快速導航

| 如果你是... | 請查看 |
|------------|--------|
| 新加入開發者 | `02_Development_Guides/Setup/` |
| API 開發者 | `02_Development_Guides/API/`, `api/` |
| 遊戲設計師 | `03_Game_Design/Factions/`, `03_Game_Design/Mechanics/` |
| 美術人員 | `04_Art_Assets/` |
| 測試人員 | `06_Testing/Verification/` |
| 專案管理 | `01_Project_Management/` |
| 一般使用者 | `07_User_Documentation/` |

---

## 重要文件速查

- [專案總覽](01_Project_Management/COMPREHENSIVE_PROJECT_PLAN.md)
- [快速開始](02_Development_Guides/Setup/QUICKSTART_UE5.7.4.md)
- [開發者指南](02_Development_Guides/DEVELOPER_GUIDE.md)
- [API 參考](api/MingGoRTS_API_Reference.md)
- [12 派系設計](03_Game_Design/Factions/12_FACTIONS_DETAILED_DESIGN.md)
- [測試驗證](06_Testing/Verification/TESTING_VERIFICATION.md)

---

*最後更新: 2026-03-23*
