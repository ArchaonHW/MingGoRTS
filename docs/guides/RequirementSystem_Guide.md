# MingGoRTS 需求單系統使用指南

## 系統概述

需求單系統自動根據需求內容進行智能分類與優先級判定，幫助團隊有效管理開發任務。

## 核心功能

### 1. 智能分類
系統基於關鍵詞分析自動將需求分類為以下類別：

| 分類代碼 | 名稱 | 權重 | 關鍵詞示例 |
|---------|------|------|-----------|
| CoreGameplay | 核心玩法 | 10 | gameplay, combat, unit, 戰鬥, 單位 |
| UIUX | UI/UX | 8 | ui, interface, menu, 介面, 選單 |
| Audio | 音頻 | 7 | audio, sound, music, 音效, 音樂 |
| Graphics | 圖形 | 7 | graphics, render, texture, 圖形, 渲染 |
| Performance | 性能 | 9 | performance, optimize, memory, 性能, 優化 |
| Network | 網絡 | 8 | network, multiplayer, sync, 網絡, 多人 |
| SaveLoad | 存檔 | 8 | save, load, persist, 存檔, 保存 |
| Localization | 本地化 | 6 | localize, translate, language, 本地化 |
| BugFix | Bug修復 | 10 | bug, fix, crash, error, 修復, 崩潰 |
| Refactor | 重構 | 5 | refactor, clean, 重構, 技術債 |
| Documentation | 文檔 | 4 | doc, document, readme, 文檔 |
| Tooling | 工具 | 5 | tool, script, automation, 工具, 腳本 |
| Testing | 測試 | 6 | test, verify, coverage, 測試 |
| Security | 安全 | 9 | security, secure, auth, 安全, 加密 |
| Analytics | 分析 | 5 | analytics, metrics, data, 分析, 數據 |

### 2. 智能優先級判定

系統根據以下因素自動計算優先級分數：

**關鍵詞加權：**
- 緊急關鍵詞 (crash, 崩潰, security): +50 分
- 高優先關鍵詞 (performance, 性能): +30 分
- 中優先關鍵詞 (feature, 功能): +15 分

**類別權重加成：**
- Bug修復類別: +20 分
- 安全相關類別: +25 分
- 性能相關類別: +15 分

**優先級閾值：**
- **Critical (緊急)**: 90+ 分 | 4小時內響應
- **High (高)**: 70+ 分 | 24小時內響應
- **Medium (中)**: 40+ 分 | 72小時內響應
- **Low (低)**: < 40 分 | 168小時內響應

## 使用方法

### 創建需求單

```powershell
# 基本創建 (自動分類與判定優先級)
.\RequirementTicketSystem.ps1 -Action create `
    -Title "修復記憶體洩漏問題" `
    -Description "在戰鬥系統中發現記憶體洩漏，導致長時間遊戲後性能下降"

# 指定分類和優先級
.\RequirementTicketSystem.ps1 -Action create `
    -Title "添加音效系統" `
    -Description "為戰鬥系統添加音效支持" `
    -Category "Audio" `
    -Priority "High" `
    -Assignee "Developer1" `
    -DueDate "2026-04-15"
```

### 列出需求單

```powershell
# 列出所有需求單
.\RequirementTicketSystem.ps1 -Action list

# 按條件過濾
.\RequirementTicketSystem.ps1 -Action list -Priority Critical
.\RequirementTicketSystem.ps1 -Action list -Category BugFix
.\RequirementTicketSystem.ps1 -Action list -Status Open
.\RequirementTicketSystem.ps1 -Action list -Assignee "Developer1"
```

### 查看詳情

```powershell
.\RequirementTicketSystem.ps1 -Action show -TicketID 1
```

### 更新需求單

```powershell
# 更新狀態和指派
.\RequirementTicketSystem.ps1 -Action update `
    -TicketID 1 `
    -Status "InProgress" `
    -Assignee "Developer2"

# 修改優先級
.\RequirementTicketSystem.ps1 -Action update `
    -TicketID 1 `
    -Priority "Critical"
```

### 添加評論

```powershell
.\RequirementTicketSystem.ps1 -Action comment `
    -TicketID 1 `
    -Description "已定位問題，正在修復"
```

### 刪除需求單

```powershell
.\RequirementTicketSystem.ps1 -Action delete -TicketID 1
```

### 查看統計報表

```powershell
# 分類統計
.\RequirementTicketSystem.ps1 -Action report

# 導出 HTML 報表
.\RequirementTicketSystem.ps1 -Action export
```

## 數據結構

需求單以 JSON 格式存儲，位置: `Data/RequirementTickets/REQ_XXXX.json`

```json
{
  "ID": 1,
  "Title": "修復記憶體洩漏問題",
  "Description": "在戰鬥系統中發現記憶體洩漏...",
  "Category": "BugFix",
  "CategoryName": "Bug修復",
  "Priority": "Critical",
  "PriorityName": "緊急",
  "Status": "InProgress",
  "Assignee": "Developer1",
  "CreatedAt": "2026-03-23 16:30:00",
  "UpdatedAt": "2026-03-23 16:35:00",
  "DueDate": "2026-03-24",
  "PriorityScore": 95,
  "Comments": [
    {
      "Timestamp": "2026-03-23 16:35:00",
      "Content": "已定位問題，正在修復"
    }
  ],
  "Tags": []
}
```

## 工作流程建議

### 1. 創建階段
- 使用描述性標題
- 提供詳細的問題描述或需求說明
- 讓系統自動分類和判定優先級
- 如需人工調整，可在創建後更新

### 2. 處理階段
- 及時更新狀態: Open → InProgress → Review → Testing → Completed
- 指派給合適的開發人員
- 添加進度評論

### 3. 監控階段
- 定期查看報表了解任務分布
- 關注 Critical 和 High 優先級任務
- 確保沒有長期阻塞的任務

## 最佳實踐

1. **標準化描述**: 使用統一的關鍵詞有助於準確分類
2. **及時更新**: 保持狀態和指派的實時性
3. **合理設定期限**: 根據優先級設置合理的截止日期
4. **定期審查**: 每週檢查報表，調整資源分配
5. **優先處理**: Critical 級別任務應在4小時內響應

## 擴展開發

如需添加新的分類或修改判定邏輯，編輯腳本中的 `$Categories` 和 `Get-RequirementPriority` 函數。

## 故障排除

| 問題 | 解決方案 |
|------|---------|
| 分類不正確 | 調整描述中的關鍵詞 |
| 優先級不符合預期 | 手動指定 -Priority 參數 |
| 報表導出失敗 | 檢查 Data/RequirementTickets 目錄權限 |
| 數據丟失 | 定期備份 Data/RequirementTickets 目錄 |
