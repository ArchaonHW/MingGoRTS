# 至聖者指揮學 - 開發任務清單

## 專案概述
將「至聖者指揮學」理論體系轉化為 MingGoRTS 的遊戲系統

---

## Phase 1: 核心系統 (4週)

### Week 1-2: 角色類型系統 (SageCharacterSystem)

#### 任務 1.1: 基礎角色類型定義
- [ ] **創建 `MingSageCharacterSystem.h`**
  - 定義 `ESageCharacterType` 枚舉 (聖者/魔王/偽聖者)
  - 定義 `FSageCharacterData` 結構
  - 宣告 `UMingSageCharacterSystem` 類
- [ ] **創建 `MingSageCharacterSystem.cpp`**
  - 實現角色類型初始化
  - 實現角色特性差異化邏輯
  - 實現角色選擇介面支援

#### 任務 1.2: 角色特性系統
- [ ] **實現聖者特性**
  - 正逆策略切換無懲罰
  - 墮落閾值 150
  - 平衡型數值修正
- [ ] **實現魔王特性**
  - 逆策效果 +50%
  - 無法停止逆策機制
  - 墮落閾值 200
- [ ] **實現偽聖者特性**
  - 正策效果 +20%
  - 無法使用逆策
  - 外交加成 +30%
  - 墮落閾值 80

#### 任務 1.3: 角色選擇介面
- [ ] **設計角色選擇 UI**
  - 三種角色類型展示
  - 特性說明面板
  - 選擇確認機制

---

### Week 3-4: 三權指揮系統 (ThreePowerSystem)

#### 任務 2.1: 三權基礎架構
- [ ] **創建 `MingThreePowerSystem.h`**
  - 定義 `EThreePowerType` 枚舉 (道權/策權/兵權)
  - 定義 `FThreePowerSystem` 結構
  - 宣告三權協調機制
- [ ] **創建 `MingThreePowerSystem.cpp`**
  - 實現三權分配邏輯
  - 實現權力平衡機制
  - 實現權力轉移系統

#### 任務 2.2: 道權系統 (MingMoralAuthority)
- [ ] **創建 `MingMoralAuthority.h/cpp`**
  - 實現墮落徵象監測 (3種徵象)
  - 實現失去資格判準
  - 實現道德邊界檢查
  - 實現防墮警告系統

#### 任務 2.3: 策權系統 (MingStrategyAuthority)
- [ ] **創建 `MingStrategyAuthority.h/cpp`**
  - 實現正逆策略管理
  - 實現陰陽切換決策
  - 實現五行節奏控制
  - 實現策略推薦系統

#### 任務 2.4: 兵權系統 (MingMilitaryAuthority)
- [ ] **創建 `MingMilitaryAuthority.h/cpp`**
  - 實現部隊指揮執行
  - 實現命令發布系統
  - 實現後果承擔機制
  - 實現指揮鏈管理

---

## Phase 2: 策略系統 (4週)

### Week 5-6: 正逆六策系統 (SixStrategiesSystem)

#### 任務 3.1: 正六策實現
- [ ] **創建 `MingSixStrategiesSystem.h`**
  - 定義 `ESixStrategyType` 枚舉 (正六策 + 逆六策)
  - 定義 `FStrategyData` 結構
  - 定義 `FStrategyEffect` 結構
- [ ] **實現立國策略 (正)**
  - 效果：士氣+30%, 徵兵+20%
  - 五行對應：木
  - 持續時間：永久
- [ ] **實現立制策略 (正)**
  - 效果：經濟效率+25%
  - 五行對應：火→土
  - 持續時間：永久
- [ ] **實現立人策略 (正)**
  - 效果：將領成長+40%
  - 五行對應：水→木
  - 持續時間：永久

#### 任務 3.2: 逆六策實現
- [ ] **實現破局策略 (逆)**
  - 效果：突破包圍/逆轉戰局
  - 五行對應：水→木
  - 冷卻：30天
  - 風險：暴露後信譽-50%
- [ ] **實現破結構策略 (逆)**
  - 效果：敵軍內亂+40%
  - 五行對應：金→水
  - 冷卻：45天
  - 風險：敵人警覺後失效
- [ ] **實現不破人策略 (逆)**
  - 效果：永久策反敵將
  - 五行對應：土→金
  - 冷卻：60天
  - 風險：被發現後永久封鎖

#### 任務 3.3: 策略切換介面
- [ ] **設計策略面板 UI**
  - 正逆策略分區顯示
  - 策略效果預覽
  - 墮落風險提示
  - 冷卻時間顯示

---

### Week 7-8: 五行節奏系統 (WuXingRhythmSystem)

#### 任務 4.1: 五行系統基礎
- [ ] **創建 `MingWuXingRhythmSystem.h`**
  - 定義 `EWuXingPhase` 枚舉 (木/火/土/金/水)
  - 定義 `FWuXingRhythmData` 結構
  - 宣告五行相生相剋機制
- [ ] **創建 `MingWuXingRhythmSystem.cpp`**
  - 實現五行節奏追蹤
  - 實現相生加成機制
  - 實現相剋減益機制
  - 實現節奏轉換判定

#### 任務 4.2: 五行階段效果
- [ ] **實現木階段 (春/立名)**
  - 正策效果 +20%
  - 徵兵速度 +30%
  - 適合：立國策略
- [ ] **實現火階段 (夏/造勢)**
  - 攻擊力 +25%
  - 士氣增長 +40%
  - 適合：進攻/破局
- [ ] **實現土階段 (長夏/收權)**
  - 防禦力 +30%
  - 資源產出 +20%
  - 適合：立制/守成
- [ ] **實現金階段 (秋/裁斷)**
  - 裁斷效果 +50%
  - 清除成本 -30%
  - 適合：肅清/破結構
- [ ] **實現水階段 (冬/養機)**
  - 逆策隱蔽 +40%
  - 情報獲取 +50%
  - 適合：潛伏/不破人

#### 任務 4.3: 五行節奏 UI
- [ ] **設計五行節奏面板**
  - 當前階段顯示
  - 下一階段預測
  - 相生相剋提示
  - 階段效果說明

---

## Phase 3: 進階機制 (3週)

### Week 9-10: 防墮機制系統 (AntiFallSystem)

#### 任務 5.1: 墮落監測系統
- [ ] **創建 `MingAntiFallSystem.h/cpp`**
  - 定義 `EFallSymptomType` 枚舉 (3種徵象)
  - 定義 `FFallRiskData` 結構
  - 實現墮落值計算
  - 實現徵象檢測邏輯

#### 任務 5.2: 三大墮落徵象
- [ ] **實現「內部始疑」徵象**
  - 連續使用逆策後部隊忠誠度下降 > 20%
  - 觸發警告提示
  - 解決方案：使用正策恢復
- [ ] **實現「敵轉異常」徵象**
  - 敵人AI察覺玩家逆策模式
  - 敵人進入防禦態勢
  - 解決方案：改變策略組合
- [ ] **實現「事過露跡」徵象**
  - 過去使用的逆策被揭露機率增加
  - 信譽損失計算
  - 解決方案：情報掩蓋行動

#### 任務 5.3: 贖罪機制
- [ ] **實現贖罪任務系統**
  - 任務生成邏輯
  - 任務完成判定
  - 墮落值減少 (-50)
- [ ] **實現墮落狀態**
  - 墮落值達100 → 強制進入墮落狀態
  - 墮落狀態下無法使用正策
  - 必須完成贖罪任務才能恢復

---

### Week 11: AI適配系統

#### 任務 6.1: AI察覺逆策機制
- [ ] **實現AI逆策檢測**
  - 基於玩家行為模式分析
  - 逆策使用頻率追蹤
  - 檢測閾值設定
- [ ] **實現AI應對策略**
  - 察覺後進入防禦態勢
  - 增加情報活動
  - 反逆策機制

#### 任務 6.2: AI使用正逆策略
- [ ] **實現AI策略選擇**
  - 根據角色類型選擇策略
  - 聖者AI：平衡使用
  - 魔王AI：偏重逆策
  - 偽聖者AI：只使用正策
- [ ] **實現AI五行節奏適應**
  - AI根據當前五行階段調整策略
  - 利用相生加成
  - 避免相剋減益

---

## Phase 4: 集成測試 (1週)

### Week 12: 測試與平衡

#### 任務 7.1: 系統集成
- [ ] **整合所有子系統**
  - 角色系統 + 三權系統
  - 策略系統 + 五行系統
  - 防墮系統 + AI系統
- [ ] **實現系統間通信**
  - 事件系統綁定
  - 數據流驗證
  - 狀態同步機制

#### 任務 7.2: 測試套件
- [ ] **創建 `MingSageCommandTest.h/cpp`**
  - 角色類型測試 (3個測試用例)
  - 三權系統測試 (3個測試用例)
  - 正逆六策測試 (12個測試用例)
  - 五行節奏測試 (5個測試用例)
  - 防墮機制測試 (3個測試用例)
  - 集成測試 (5個測試用例)
- [ ] **執行自動化測試**
  - 測試覆蓋率 > 80%
  - 所有測試通過

#### 任務 7.3: 數值平衡
- [ ] **調整策略數值**
  - 效果強度平衡
  - 冷卻時間優化
  - 墮落值計算調整
- [ ] **玩家體驗優化**
  - 教學系統完善
  - UI/UX 改進
  - 反饋機制優化

---

## 文件清單

### 頭文件 (Headers)
```
Source/MingGoRTS/Public/SageCommand/
├── MingSageCharacterSystem.h
├── MingThreePowerSystem.h
├── MingMoralAuthority.h
├── MingStrategyAuthority.h
├── MingMilitaryAuthority.h
├── MingSixStrategiesSystem.h
├── MingWuXingRhythmSystem.h
├── MingAntiFallSystem.h
└── Test/MingSageCommandTest.h
```

### 源文件 (Source)
```
Source/MingGoRTS/Private/SageCommand/
├── MingSageCharacterSystem.cpp
├── MingThreePowerSystem.cpp
├── MingMoralAuthority.cpp
├── MingStrategyAuthority.cpp
├── MingMilitaryAuthority.cpp
├── MingSixStrategiesSystem.cpp
├── MingWuXingRhythmSystem.cpp
├── MingAntiFallSystem.cpp
└── Test/MingSageCommandTest.cpp
```

---

## 里程碑檢查點

### Milestone 1: 角色與三權 (Week 4)
- [ ] 三種角色類型可選擇
- [ ] 角色特性有明顯差異
- [ ] 三權系統框架完成

### Milestone 2: 策略系統 (Week 8)
- [ ] 正逆六策全部實現
- [ ] 五行節奏系統運作
- [ ] 策略效果可感知

### Milestone 3: 進階機制 (Week 11)
- [ ] 防墮機制有效運作
- [ ] AI能適當應對
- [ ] 贖罪機制完整

### Milestone 4: 完成交付 (Week 12)
- [ ] 測試覆蓋率 > 80%
- [ ] 數值平衡
- [ ] 教學系統完善

---

## 依賴關係

```
MingSageCharacterSystem
    ↓
MingThreePowerSystem (依賴角色類型)
    ↓
MingMoralAuthority / MingStrategyAuthority / MingMilitaryAuthority (並行)
    ↓
MingSixStrategiesSystem (依賴策權)
    ↓
MingWuXingRhythmSystem (與六策並行)
    ↓
MingAntiFallSystem (依賴所有策略系統)
    ↓
AI Adaptation (依賴完整系統)
```

---

## 風險與應對

| 風險 | 可能性 | 影響 | 應對措施 |
|-----|-------|------|---------|
| 系統過於複雜 | 高 | 玩家難以上手 | 設計分階段教學 |
| 數值平衡困難 | 中 | 某些策略過強 | 延長測試期 |
| 理論轉化困難 | 中 | 概念難以實現 | 與設計師密切溝通 |
| 性能問題 | 低 | 計算過度 | 優化算法 |

---

## 參考資料

- 理論原文：https://vocus.cc/article/69431022fd897800015d137b
- 開發文檔：`docs/SageCommandSystem_DesignDocument.md`
