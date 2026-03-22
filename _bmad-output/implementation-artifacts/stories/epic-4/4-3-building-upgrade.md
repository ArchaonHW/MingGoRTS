# Story 4-3: 建築升級系統 | Building Upgrade System

**狀態 | Status:** ✅ completed  
**負責人 | Owner:** AI Assistant  
**開始日期 | Start Date:** 2026-03-22  
**完成日期 | End Date:** 2026-03-22

---

## 用戶故事 | User Story

**作為** 玩家，
**我想要** 升級我的建築物，
**從而** 提升它們的生命值、生產效率和防禦能力。

**As a** player,  
**I want** to upgrade my buildings,  
**So that** I can improve their health, production efficiency, and defense capabilities.

---

## 驗收標準 | Acceptance Criteria

### ✅ 已完成 | Completed

1. **✅** 建築可以升級多次（多等級）
   - Buildings can be upgraded multiple times (multi-level)
   
2. **✅** 升級需要消耗資源和時間
   - Upgrades require resources and time
   
3. **✅** 升級影響建築屬性（生命值、生產、防禦）
   - Upgrades affect building stats (health, production, defense)
   
4. **✅** 提供藍圖函數庫接口
   - Blueprint function library provided
   
5. **✅** 實現默認升級配置（指揮中心、兵營、農場、礦場、城牆）
   - Default upgrade configs implemented (Command Center, Barracks, Farm, Mine, Wall)

---

## 技術實現 | Technical Implementation

### 新增文件 | New Files

1. `MingBuildingUpgradeBlueprintLibrary.h/cpp`
   - 藍圖函數庫，提供建築升級相關功能
   - Blueprint library for building upgrade functions

### 修改文件 | Modified Files

1. `MingTechTree.h/cpp`
   - 完善 `FMingBuildingUpgrade` 結構體（雙語注釋）
   - 添加 `SetupDefaultBuildingUpgrades()` 方法
   - 設置默認建築升級配置

2. `MingBuildingActor.h/cpp`
   - 添加升級相關方法和事件
   - 添加升級進度追踪
   - 實現升級效果應用

### 核心功能 | Core Features

1. **升級結構體 | Upgrade Structure**
   - `FMingBuildingUpgrade`: 定義升級屬性和成本
   - 支持多等級、資源成本、時間、屬性倍率

2. **科技樹管理 | Tech Tree Management**
   - `UMingTechTree`: 管理建築升級註冊和查詢
   - `RegisterBuildingUpgrade()`: 註冊升級
   - `CanUpgradeBuilding()`: 檢查是否可以升級
   - `GetAvailableUpgradesForBuilding()`: 獲取可用升級

3. **建築升級支持 | Building Upgrade Support**
   - `AMingBuildingActor`: 支持建築升級
   - `StartUpgrade()`: 開始升級
   - `CancelUpgrade()`: 取消升級
   - `ApplyUpgrade()`: 應用升級效果
   - 事件廣播：開始、進度、完成、取消

4. **藍圖接口 | Blueprint Interface**
   - `UMingBuildingUpgradeBlueprintLibrary`: 藍圖函數庫
   - 查詢升級等級、進度、可用升級
   - 檢查升級條件、獲取升級成本

---

## 升級配置示例 | Upgrade Configuration Examples

### 指揮中心 | Command Center
- **強化結構**: 生命值 +25%/級，防禦 +10%/級
- **成本**: 500 材料，30秒

### 兵營 | Barracks
- **高效訓練**: 生產效率 +30%/級
- **防禦工事**: 防禦 +25%/級，生命值 +15%/級

### 資源建築 | Resource Buildings
- **現代農業**: 糧食產量 +40%/級
- **深層開採**: 礦產產量 +35%/級

---

## 測試驗證 | Testing & Verification

### 單元測試 | Unit Tests
- ✅ 升級結構體創建和屬性設置
- ✅ 科技樹註冊和查詢功能
- ✅ 升級條件檢查

### 集成測試 | Integration Tests
- ✅ 建築升級流程測試
- ✅ 事件廣播驗證
- ✅ 藍圖函數調用

### 手動測試 | Manual Testing
- ✅ 在編輯器中測試升級配置
- ✅ 驗證升級效果應用
- ✅ 檢查資源消耗邏輯

---

## 相關文檔 | Related Documentation

- [建築系統設計文檔](../architecture.md)
- [資源收集系統](./4-2-resource-collection.md)
- [科技樹研發系統](./4-4-tech-tree-research.md)

---

## 備註 | Notes

- 所有代碼使用雙語注釋（繁體中文 + 英文）
- 支持 Blueprint 可視化腳本
- 擴展性設計，便於添加新的建築類型和升級
