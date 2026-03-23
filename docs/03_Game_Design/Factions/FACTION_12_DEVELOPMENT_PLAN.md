# 民國史詩基礎遊戲 - 12可玩勢力詳細開發計劃

## 專案概述

**專案名稱**: 民國史詩基礎遊戲 - 12可玩勢力系統  
**開始日期**: 2026-03-23  
**預估工期**: 2週 (10個工作日)  
**目標**: 完成12個可玩勢力的完整設計與實現

---

## 12勢力總覽

| 編號 | 勢力名稱 | 難度 | 起始地 | 起始年份 | 特色機制 | 預估工時 |
|------|----------|------|--------|----------|----------|----------|
| 1 | 北洋政府 | ★★☆☆☆ | 北京 | 1912 | 中央正統、外交優勢 | 2天 |
| 2 | 國民政府 | ★★★☆☆ | 南京/廣州 | 1925 | 民族主義、現代化 | 2天 |
| 3 | 中國共產黨 | ★★★★☆ | 延安/江西 | 1927 | 人民戰爭、游擊戰 | 2天 |
| 4 | 奉系軍閥 | ★★★☆☆ | 瀋陽 | 1916 | 東北工業、騎兵優勢 | 1.5天 |
| 5 | 直系軍閥 | ★★★☆☆ | 保定/洛陽 | 1920 | 中原霸主、兵力龐大 | 1.5天 |
| 6 | 皖系軍閥 | ★★★☆☆ | 天津/合肥 | 1916 | 政治操作、日本援助 | 1.5天 |
| 7 | 晉系軍閥 | ★★★☆☆ | 太原 | 1911 | 山西模範、防禦專精 | 1.5天 |
| 8 | 桂系軍閥 | ★★★☆☆ | 桂林/南寧 | 1924 | 廣西民兵、山地戰 | 1.5天 |
| 9 | 滇系軍閥 | ★★★★☆ | 昆明 | 1915 | 護國傳統、邊疆擴張 | 1.5天 |
| 10 | 川系軍閥 | ★★★★☆ | 成都/重慶 | 1918 | 防區制、內部統一 | 1.5天 |
| 11 | 馬家軍 | ★★★★☆ | 蘭州/西寧 | 1912 | 回族騎兵、宗教團結 | 1.5天 |
| 12 | 新疆勢力 | ★★★★★ | 迪化 | 1912 | 邊疆要塞、民族複雜 | 1.5天 |

**總計工時**: 約2週 (19.5工作天)

---

## 開發時程 (2週衝刺)

### 第一週 (Day 1-5): 核心勢力實現

#### Day 1-2: 北洋政府 + 國民政府
**北洋政府 (Beiyang Government)**
- [ ] 基本屬性設計 (難度★★, 起始地北京, 1912年)
- [ ] 特色機制: 中央正統、外交優勢
- [ ] 專屬單位 (3-4個):
  - 北洋精銳步兵
  - 德式訓練砲兵
  - 北洋騎兵師
  - 外國顧問團
- [ ] 優勢劣勢分析
- [ ] 玩法指南

**國民政府 (Nationalist Government)**
- [ ] 基本屬性設計 (難度★★★, 起始地南京/廣州, 1925年)
- [ ] 特色機制: 民族主義、現代化
- [ ] 專屬單位 (3-4個):
  - 黃埔教導團
  - 德械師
  - 空軍支援隊
  - 政治工作隊
- [ ] 優勢劣勢分析
- [ ] 玩法指南

#### Day 3-4: 中國共產黨 + 奉系軍閥
**中國共產黨 (Chinese Communist Party)**
- [ ] 基本屬性設計 (難度★★★★, 起始地延安/江西, 1927年)
- [ ] 特色機制: 人民戰爭、游擊戰
- [ ] 專屬單位 (3-4個):
  - 紅軍游擊隊
  - 工農赤衛隊
  - 政治委員隊
  - 後勤支援隊
- [ ] 優勢劣勢分析
- [ ] 玩法指南

**奉系軍閥 (Fengtian Clique)**
- [ ] 基本屬性設計 (難度★★★, 起始地瀋陽, 1916年)
- [ ] 特色機制: 東北工業、騎兵優勢
- [ ] 專屬單位 (3-4個):
  - 東北軍步兵
  - 白俄僱傭兵
  - 奉天鐵甲車
  - 騎兵旅
- [ ] 優勢劣勢分析
- [ ] 玩法指南

#### Day 5: 直系軍閥 + 皖系軍閥
**直系軍閥 (Zhili Clique)**
- [ ] 基本屬性設計 (難度★★★, 起始地保定/洛陽, 1920年)
- [ ] 特色機制: 中原霸主、兵力龐大
- [ ] 專屬單位 (3-4個):
  - 直軍主力師
  - 吳佩孚衛隊
  - 中原騎兵
  - 地方民團

**皖系軍閥 (Anhui Clique)**
- [ ] 基本屬性設計 (難度★★★, 起始地天津/合肥, 1916年)
- [ ] 特色機制: 政治操作、日本援助
- [ ] 專屬單位 (3-4個):
  - 皖軍步兵
  - 日本顧問團
  - 皖系騎兵
  - 地方守備隊

---

### 第二週 (Day 6-10): 地方勢力實現 + 測試平衡

#### Day 6-7: 晉系 + 桂系 + 滇系
**晉系軍閥 (Shanxi Clique)**
- [ ] 基本屬性設計 (難度★★★, 起始地太原, 1911年)
- [ ] 特色機制: 山西模範、防禦專精
- [ ] 專屬單位 (3-4個):
  - 晉軍防守師
  - 山西工兵隊
  - 晉商資助隊
  - 地方保安團

**桂系軍閥 (Guangxi Clique)**
- [ ] 基本屬性設計 (難度★★★, 起始地桂林/南寧, 1924年)
- [ ] 特色機制: 廣西民兵、山地戰
- [ ] 專屬單位 (3-4個):
  - 桂軍山地師
  - 廣西民團
  - 山地游擊隊
  - 地方勢力聯盟

**滇系軍閥 (Yunnan Clique)**
- [ ] 基本屬性設計 (難度★★★★, 起始地昆明, 1915年)
- [ ] 特色機制: 護國傳統、邊疆擴張
- [ ] 專屬單位 (3-4個):
  - 滇軍邊防師
  - 少數民族聯軍
  - 護國軍精銳
  - 邊疆巡邏隊

#### Day 8-9: 川系 + 馬家軍 + 新疆
**川系軍閥 (Sichuan Clique)**
- [ ] 基本屬性設計 (難度★★★★, 起始地成都/重慶, 1918年)
- [ ] 特色機制: 防區制、內部統一
- [ ] 專屬單位 (3-4個):
  - 川軍防區師
  - 四川民團
  - 地方軍閥聯軍
  - 川東水師

**馬家軍 (Ma Family Forces)**
- [ ] 基本屬性設計 (難度★★★★, 起始地蘭州/西寧, 1912年)
- [ ] 特色機制: 回族騎兵、宗教團結
- [ ] 專屬單位 (3-4個):
  - 回族騎兵師
  - 穆斯林義勇軍
  - 馬家精銳衛隊
  - 宗教動員隊

**新疆勢力 (Xinjiang Forces)**
- [ ] 基本屬性設計 (難度★★★★★, 起始地迪化, 1912年)
- [ ] 特色機制: 邊疆要塞、民族複雜
- [ ] 專屬單位 (3-4個):
  - 新疆邊防軍
  - 多民族聯軍
  - 邊疆騎兵師
  - 地方勢力聯盟

#### Day 10: 測試與平衡調整
- [ ] 勢力平衡性測試
- [ ] 難度驗證
- [ ] 單位數值調整
- [ ] 最終文檔整理

---

## 技術架構

### 文件結構

```
Source/MingGoRTS/
├── Public/
│   └── RepublicEraFactions/
│       ├── MingRTSFactionManager.h
│       ├── MingRTSFactionData.h
│       ├── MingRTSFactionTypes.h
│       └── Factions/
│           ├── BeiyangGovernment.h
│           ├── NationalistGovernment.h
│           ├── CommunistParty.h
│           ├── FengtianClique.h
│           ├── ZhiliClique.h
│           ├── AnhuiClique.h
│           ├── ShanxiClique.h
│           ├── GuangxiClique.h
│           ├── YunnanClique.h
│           ├── SichuanClique.h
│           ├── MaFamilyForces.h
│           └── XinjiangForces.h
├── Private/
│   └── RepublicEraFactions/
│       ├── MingRTSFactionManager.cpp
│       ├── MingRTSFactionData.cpp
│       └── Factions/
│           ├── BeiyangGovernment.cpp
│           ├── NationalistGovernment.cpp
│           ├── CommunistParty.cpp
│           ├── FengtianClique.cpp
│           ├── ZhiliClique.cpp
│           ├── AnhuiClique.cpp
│           ├── ShanxiClique.cpp
│           ├── GuangxiClique.cpp
│           ├── YunnanClique.cpp
│           ├── SichuanClique.cpp
│           ├── MaFamilyForces.cpp
│           └── XinjiangForces.cpp
└── Tests/
    └── MingRTSFactionTests.cpp
```

### 核心類設計

```cpp
// 勢力基類
UCLASS(Abstract, Blueprintable)
class MINGGORTS_API UMingRTSFactionBase : public UObject
{
    GENERATED_BODY()
    
public:
    // 基本屬性
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
    FFactionAttributes Attributes;
    
    // 特色機制
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
    TArray<UFactionMechanic*> UniqueMechanics;
    
    // 專屬單位
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
    TArray<TSubclassOf<AMingRTSUnit>> UniqueUnits;
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Faction")
    virtual void InitializeFaction();
    
    // 獲取勢力描述
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual FFactionDescription GetFactionDescription() const;
    
    // 獲取玩法指南
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual FGameplayGuide GetGameplayGuide() const;
};
```

---

## 勢力設計標準

### 每個勢力必須包含

1. **基本屬性**
   - 勢力名稱 (中文/英文)
   - 難度等級 (1-5星)
   - 起始地點 (主要城市)
   - 起始年份 (1911-1949)
   - 勢力標誌/旗幟
   - 代表顏色

2. **特色機制** (2-3個)
   - 機制名稱
   - 機制描述
   - 遊戲效果
   - 觸發條件

3. **專屬單位** (3-4個)
   - 單位名稱
   - 單位類型 (步兵/騎兵/砲兵/特殊)
   - 基礎數值 (攻擊/防禦/速度/成本)
   - 特殊能力
   - 生產條件

4. **優劣勢分析**
   - 優勢 (3-4項)
   - 劣勢 (3-4項)
   - 適合玩家類型

5. **推薦玩法指南**
   - 早期策略
   - 中期發展
   - 後期目標
   - 外交建議
   - 軍事戰術

---

## 平衡性設計原則

### 數值平衡
- **難度★★**: 起始資源 +20%, AI侵略性 -10%
- **難度★★★**: 標準數值
- **難度★★★★**: 起始資源 -10%, 特殊單位成本 +15%
- **難度★★★★★**: 起始資源 -20%, 外部壓力 +30%

### 單位平衡公式
```
單位戰力 = (攻擊 × 1.2 + 防禦 × 1.0 + 速度 × 0.8) × 特殊能力係數
單位成本 = 基礎成本 × 勢力調整係數 × 稀有度係數
```

---

## 測試計劃

### 單元測試
- [ ] 每個勢力屬性驗證
- [ ] 專屬單位數值測試
- [ ] 特色機制功能測試
- [ ] 初始化流程測試

### 集成測試
- [ ] 勢力選擇界面測試
- [ ] 多人遊戲勢力同步測試
- [ ] 存檔/讀檔勢力數據測試

### 平衡性測試
- [ ] AI對戰平衡測試 (100場對戰)
- [ ] 玩家反饋收集
- [ ] 數據分析與調整

---

## 驗收標準

- [ ] 12個勢力全部完成設計
- [ ] 每個勢力包含完整屬性、機制、單位、指南
- [ ] 單元測試覆蓋率 > 90%
- [ ] 平衡性測試通過 (勝率差異 < 15%)
- [ ] 代碼審查通過
- [ ] 文檔完整

---

## 風險管理

### 高風險項目
1. **歷史準確性**: 需確保勢力設計符合歷史事實
   - **緩解**: 聘請歷史顧問審核

2. **平衡性調整**: 12個勢力難以完全平衡
   - **緩解**: 分階段測試，持續迭代

3. **內容複雜度**: 48個專屬單位設計工作量大
   - **緩解**: 使用模板化設計，重用基礎數值

---

## 文件清單

- 開發計劃: `docs/FACTION_12_DEVELOPMENT_PLAN.md`
- 設計規範: `docs/FACTION_DESIGN_GUIDELINES.md`
- API文檔: `docs/FACTION_API_REFERENCE.md`
- 平衡性報告: `docs/FACTION_BALANCE_REPORT.md`
- 歷史參考: `docs/REPUBLIC_ERA_HISTORICAL_REFERENCE.md`

---

**狀態**: 📋 計劃階段  
**負責人**: 主程式設計師 + 遊戲設計師  
**更新日期**: 2026-03-23
