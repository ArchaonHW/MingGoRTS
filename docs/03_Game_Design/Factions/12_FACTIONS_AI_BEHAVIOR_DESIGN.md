# 12勢力AI行為設計

## AI設計總體原則

### 設計理念
- **歷史還原**: AI行為基於歷史真實策略和決策模式
- **個性化**: 每個勢力有獨特的AI行為模式和決策傾向
- **適應性**: AI能根據戰局動態調整策略
- **挑戰性**: 提供不同難度等級的AI對手

### AI能力維度
1. **戰略層**: 長期規劃、發展方向、聯盟決策
2. **戰術層**: 戰鬥指揮、單位調度、地形利用
3. **經濟層**: 資源管理、建設優先級、科技發展
4. **外交層**: 聯盟、敵對、中立決策

---

## 勢力專屬AI策略

### 1. 北洋政府 AI (Beiyang AI)

#### 戰略特徵
- **核心目標**: 維持中央統治地位
- **優先級**: 外交 > 經濟 > 軍事 > 擴張
- **風格**: 保守、權威、外交導向

#### 行為模式
```cpp
class UBeiyangAIController : public UMingRTSFactionAIController
{
    // 外交優先策略
    float GetDiplomacyPriority() override { return 0.9f; }
    
    // 傾向於與小勢力結盟以維持控制
    bool ShouldFormAlliance(AFaction* Target) override
    {
        return Target->GetStrength() < 0.4f && Target->GetRelation(this) > 0.3f;
    }
    
    // 優先使用中央軍精銳
    EUnitType GetPreferredUnitType() override { return EUnitType::BeiyangElite; }
    
    // 防守為主，進攻謹慎
    float GetAggressiveness() override { return 0.4f; }
    float GetDefensiveness() override { return 0.8f; }
};
```

#### 特殊決策邏輯
- **府院之爭事件**: 觸發內部權力鬥爭，AI可能分裂
- **稅收政策**: 優先提高稅收以維持中央政府運作
- **外交干預**: 經常干預其他勢力間的衝突

#### 歷史還原行為
- 早期積極維護統一
- 後期陷入分裂和派系鬥爭
- 對外依賴列強支持

---

### 2. 國民政府 AI (Nationalist AI)

#### 戰略特徵
- **核心目標**: 完成北伐統一中國
- **優先級**: 軍事 > 經濟 > 外交 > 建設
- **風格**: 進取、現代化、民族主義

#### 行為模式
```cpp
class UNationalistAIController : public UMingRTSFactionAIController
{
    // 軍事優先策略
    float GetMilitaryPriority() override { return 0.9f; }
    
    // 積極發動北伐戰爭
    bool ShouldLaunchNorthernExpedition() override
    {
        return GetMilitaryStrength() > 0.7f && Year >= 1926;
    }
    
    // 優先使用黃埔系部隊
    EUnitType GetPreferredUnitType() override { return EUnitType::WhampoaCadet; }
    
    // 進攻型AI
    float GetAggressiveness() override { return 0.8f; }
    
    // 現代化科技優先
    ETechType GetResearchPriority() override { return ETechType::Modernization; }
};
```

#### 特殊決策邏輯
- **北伐戰爭**: 1926年後會主動發起北伐
- **清黨事件**: 可能與共產黨決裂
- **現代化計劃**: 優先發展工業和科技

#### 歷史還原行為
- 積極進行北伐
- 與共產黨既合作又鬥爭
- 推動現代化改革

---

### 3. 中國共產黨 AI (Communist AI)

#### 戰略特徵
- **核心目標**: 建立根據地，發展人民戰爭
- **優先級**: 游擊戰 > 根據地建設 > 群眾動員 > 正規戰
- **風格**: 靈活、持久、群眾導向

#### 行為模式
```cpp
class UCommunistAIController : public UMingRTSFactionAIController
{
    // 游擊戰專精
    float GetGuerrillaWarfareBonus() override { return 0.35f; }
    
    // 優先建立根據地
    bool ShouldEstablishBase() override { return true; }
    
    // 群眾動員能力
    float GetMassMobilizationEfficiency() override { return 1.5f; }
    
    // 避開正面決戰
    bool ShouldAvoidDirectConfrontation(AFaction* Enemy) override
    {
        return GetRelativeStrength(Enemy) < 0.6f;
    }
    
    // 優先使用游擊隊
    EUnitType GetPreferredUnitType() override { return EUnitType::Guerrilla; }
    
    // 靈活機動
    float GetMobilityPreference() override { return 0.9f; }
};
```

#### 特殊決策邏輯
- **長征決策**: 危機時會考慮戰略轉移
- **統一戰線**: 會尋求與其他勢力合作抗日
- **土地政策**: 積極推行土地改革獲得農民支持

#### 歷史還原行為
- 避免正面決戰，擅長游擊戰
- 重視根據地建設
- 善於群眾動員

---

### 4. 奉系軍閥 AI (Fengtian AI)

#### 戰略特徵
- **核心目標**: 維護東北獨立，伺機進關
- **優先級**: 經濟/工業 > 軍事 > 防禦 > 擴張
- **風格**: 工業化、保守、騎兵優勢

#### 行為模式
```cpp
class UFengtianAIController : public UMingRTSFactionAIController
{
    // 工業發展優先
    float GetIndustryPriority() override { return 0.9f; }
    
    // 東北防禦強化
    float GetDefenseBonusInNortheast() override { return 0.3f; }
    
    // 騎兵專精
    float GetCavalryBonus() override { return 0.25f; }
    
    // 坦克使用 (獨特優勢)
    bool CanUseTanks() override { return true; }
    
    // 前期保守，後期進取
    float GetAggressiveness() override
    {
        return Year < 1924 ? 0.3f : 0.7f;
    }
    
    // 白俄傭兵招募
    bool ShouldRecruitWhiteRussianMercenaries() override
    {
        return GetEconomyStrength() > 0.6f;
    }
};
```

#### 特殊決策邏輯
- **皇姑屯事件**: 1928年可能觸發張作霖死亡事件
- **易幟決策**: 可能選擇歸順國民政府
- **日本干預**: 面臨日本壓力時的應對選擇

#### 歷史還原行為
- 前期鞏固東北，後期三次進關
- 重視工業建設
- 利用白俄傭兵

---

### 5. 直系軍閥 AI (Zhili AI)

#### 戰略特徵
- **核心目標**: 控制中原，問鼎中央
- **優先級**: 軍事擴張 > 兵力積累 > 聯盟 > 經濟
- **風格**: 兵力雄厚、裝甲優勢、進攻型

#### 行為模式
```cpp
class UZhiliAIController : public UMingRTSFactionAIController
{
    // 兵力擴張優先
    float GetTroopExpansionRate() override { return 1.3f; }
    
    // 裝甲列車專精
    bool ShouldBuildArmoredTrains() override { return true; }
    float GetArmoredTrainBonus() override { return 0.4f; }
    
    // 進攻型AI
    float GetAggressiveness() override { return 0.8f; }
    
    // 大規模作戰優勢
    float GetLargeScaleCombatBonus() override { return 0.15f; }
    
    // 優先數量而非質量
    bool PreferQuantityOverQuality() override { return true; }
};
```

#### 特殊決策邏輯
- **第一次直奉戰爭**: 積極對奉系作戰
- **第二次直奉戰爭**: 可能遭遇失敗
- **馮玉祥倒戈**: 內部叛亂風險

#### 歷史還原行為
- 依賴龐大兵力
- 善用裝甲列車
- 積極參與中央爭奪

---

### 6. 皖系軍閥 AI (Anhui AI)

#### 戰略特徵
- **核心目標**: 政治操控，利用外交槓桿
- **優先級**: 外交 > 政治操作 > 軍事 > 經濟
- **風格**: 陰謀、機會主義、日本依賴

#### 行為模式
```cpp
class UAnhuiAIController : public UMingRTSFactionAIController
{
    // 政治操作優先
    float GetPoliticalManeuveringBonus() override { return 0.4f; }
    
    // 日本關係利用
    float GetJapaneseSupportBonus() override { return 0.3f; }
    
    // 機會主義外交
    bool IsOpportunistic() override { return true; }
    
    // 優先使用参战軍
    EUnitType GetPreferredUnitType() override { return EUnitType::WarParticipationArmy; }
    
    // 適度進攻性
    float GetAggressiveness() override { return 0.6f; }
    
    // 擅長背刺盟友
    bool LikelyToBetrayAlliance() override
    {
        return GetStrength() > AllianceLeader->GetStrength() * 0.8f;
    }
};
```

#### 特殊決策邏輯
- **安福國會**: 政治操控事件
- **直皖戰爭**: 與直系的決戰
- **日本關係**: 過度依賴日本的風險

#### 歷史還原行為
- 擅長政治操作
- 依賴日本支持
- 機會主義外交

---

### 7. 晉系軍閥 AI (Shanxi AI)

#### 戰略特徵
- **核心目標**: 保守山西，防禦為主
- **優先級**: 防禦建設 > 經濟自足 > 中立外交 > 謹慎擴張
- **風格**: 保守、防禦、模範治理

#### 行為模式
```cpp
class UShanxiAIController : public UMingRTSFactionAIController
{
    // 防禦專精
    float GetDefenseBonus() override { return 0.4f; }
    float GetFortificationEfficiency() override { return 1.4f; }
    
    // 中立外交傾向
    float GetNeutralityPreference() override { return 0.8f; }
    
    // 經濟自給自足
    bool PreferEconomicSelfSufficiency() override { return true; }
    
    // 山西境內戰鬥加成
    float GetCombatBonusInShanxi() override { return 0.25f; }
    
    // 極低進攻性
    float GetAggressiveness() override { return 0.2f; }
    
    // 閻錫山個人領導加成
    float GetLeaderBonus() override { return 0.2f; }
};
```

#### 特殊決策邏輯
- **山西模範建設**: 積極發展內部經濟
- **中立政策**: 盡量避免卷入大規模戰爭
- **中原大戰參與**: 可能被迫選邊站

#### 歷史還原行為
- 極度重視防禦
- 保持中立立場
- 模範省建設

---

### 8. 桂系軍閥 AI (Guangxi AI)

#### 戰略特徵
- **核心目標**: 利用山地優勢，擴張兩廣
- **優先級**: 山地戰 > 民兵動員 > 聯盟 > 經濟
- **風格**: 山地游擊、全民皆兵、防禦反擊

#### 行為模式
```cpp
class UGuangxiAIController : public UMingRTSFactionAIController
{
    // 山地戰專精
    float GetMountainWarfareBonus() override { return 0.4f; }
    float GetMobilityInMountain() override { return 1.3f; }
    
    // 民兵動員
    float GetMilitiaRecruitmentRate() override { return 2.0f; }
    float GetMilitiaCombatBonus() override { return 0.3f; }
    
    // 廣西境內防禦加成
    float GetDefenseBonusInGuangxi() override { return 0.3f; }
    
    // 兩廣聯盟傾向
    bool ShouldFormLiangguangAlliance() override
    {
        return GetRelation(GuangdongFaction) > 0.5f;
    }
    
    // 防禦反擊風格
    float GetDefensiveness() override { return 0.7f; }
    float GetCounterAttackBonus() override { return 0.2f; }
};
```

#### 特殊決策邏輯
- **蔣桂戰爭**: 與蔣介石的衝突
- **兩廣聯盟**: 與廣東的合作
- **北伐參與**: 積極參與北伐戰爭

#### 歷史還原行為
- 擅長山地作戰
- 全民皆兵政策
- 靈活的外交策略

---

### 9. 滇系軍閥 AI (Yunnan AI)

#### 戰略特徵
- **核心目標**: 向外擴張，影響西南
- **優先級**: 擴張 > 山地戰 > 民族部隊 > 高原適應
- **風格**: 擴張型、邊疆勢力、民族多元

#### 行為模式
```cpp
class UYunnanAIController : public UMingRTSFactionAIController
{
    // 擴張傾向
    float GetExpansionDesire() override { return 0.8f; }
    
    // 向四川、貴州擴張加成
    float GetExpansionBonusToSichuan() override { return 0.25f; }
    float GetExpansionBonusToGuizhou() override { return 0.2f; }
    
    // 高原作戰優勢
    float GetHighlandCombatBonus() override { return 0.35f; }
    
    // 少數民族部隊
    bool CanRecruitMinorityTroops() override { return true; }
    float GetMinorityTroopBonus() override { return 0.25f; }
    
    // 護國戰爭傳承
    float GetNationalProtectionBonus() override { return 0.3f; }
    
    // 遠征補給困難
    float GetSupplyDifficultyForExpedition() override { return -0.3f; }
};
```

#### 特殊決策邏輯
- **護國戰爭**: 響應反袁稱帝
- **向外擴張**: 出兵四川、貴州
- **民族管理**: 處理多民族關係

#### 歷史還原行為
- 積極向外擴張
- 護國戰爭主力
- 民族部隊使用

---

### 10. 川系軍閥 AI (Sichuan AI)

#### 戰略特徵
- **核心目標**: 統一四川，結束防區割據
- **優先級**: 內部統一 > 防區建設 > 天府資源 > 對外防禦
- **風格**: 內戰頻繁、資源豐富、防區制

#### 行為模式
```cpp
class USichuanAIController : public UMingRTSFactionAIController
{
    // 防區制管理
    int32 GetMaxDefenseZones() override { return 5; }
    float GetDefenseZoneEfficiency() override { return 1.25f; }
    
    // 內部混戰傾向
    float GetInternalConflictChance() override { return 0.4f; }
    
    // 天府之國資源加成
    float GetResourceProductionBonus() override { return 0.35f; }
    float GetFoodProductionBonus() override { return 0.4f; }
    
    // 四川境內戰鬥加成
    float GetCombatBonusInSichuan() override { return 0.25f; }
    
    // 對外擴張保守
    float GetExternalAggressiveness() override { return 0.3f; }
    
    // 防區守備隊專精
    EUnitType GetPreferredUnitType() override { return EUnitType::DefenseZoneGarrison; }
};
```

#### 特殊決策邏輯
- **防區爭奪**: 與其他川系派系戰鬥
- **統一四川**: 最終目標是統一全省
- **出川抗日**: 統一後可能出川參戰

#### 歷史還原行為
- 內部混戰不斷
- 防區割據嚴重
- 資源豐富但消耗於內戰

---

### 11. 馬家軍 AI (MaFamily AI)

#### 戰略特徵
- **核心目標**: 維護西北統治，騎兵優勢
- **優先級**: 騎兵發展 > 宗教團結 > 西北防禦 > 游擊戰
- **風格**: 騎兵專精、宗教色彩、西北特色

#### 行為模式
```cpp
class UMaFamilyAIController : public UMingRTSFactionAIController
{
    // 騎兵專精
    float GetCavalryBonus() override { return 0.4f; }
    int32 GetMaxCavalryRatio() override { return 60; } // 最高60%騎兵
    
    // 宗教團結
    float GetReligiousUnityBonus() override { return 0.35f; }
    float GetMoraleBonusFromReligion() override { return 0.3f; }
    
    // 西北地形適應
    float GetNorthwestTerrainBonus() override { return 0.3f; }
    
    // 快速突襲戰術
    float GetRaidBonus() override { return 0.35f; }
    float GetSpeedBonus() override { return 0.25f; }
    
    // 經濟薄弱限制
    float GetEconomicWeakness() override { return -0.3f; }
    
    // 宗教動員
    bool CanUseReligiousMobilization() override { return true; }
};
```

#### 特殊決策邏輯
- **騎兵擴張**: 維持大量騎兵部隊
- **宗教政策**: 利用宗教維持團結
- **應對西征**: 對抗共產黨西征軍

#### 歷史還原行為
- 騎兵戰力強大
- 宗教維持統治
- 最終被解放軍擊敗

---

### 12. 新疆勢力 AI (Xinjiang AI)

#### 戰略特徵
- **核心目標**: 維持新疆統治，平衡各方勢力
- **優先級**: 穩定 > 民族平衡 > 蘇聯關係 > 防禦
- **風格**: 極難難度、平衡大師、多民族管理

#### 行為模式
```cpp
class UXinjiangAIController : public UMingRTSFactionAIController
{
    // 極難難度加成 (平衡用)
    float GetDifficultyBonus() override { return 0.2f; }
    
    // 多民族管理
    float GetEthnicManagementComplexity() override { return 1.5f; }
    float GetEthnicTensionRisk() override { return 0.3f; }
    
    // 蘇聯援助
    float GetSovietAidBonus() override { return 0.25f; }
    bool CanRecruitSovietAdvisors() override { return true; }
    
    // 邊疆防禦加成
    float GetBorderDefenseBonus() override { return 0.4f; }
    
    // 經濟自給壓力
    float GetEconomicPressure() override { return -0.2f; }
    
    // 外部干預風險
    float GetForeignInterventionRisk() override { return 0.4f; }
    
    // 地緣孤立
    float GetIsolationPenalty() override { return -0.25f; }
};
```

#### 特殊決策邏輯
- **民族平衡**: 維持各民族勢力平衡
- **蘇聯關係**: 謹慎處理與蘇聯關係
- **盛世才轉向**: 可能改變政治立場

#### 歷史還原行為
- 極度複雜的民族關係
- 蘇聯深度介入
- 最終回歸中央政府

---

## AI難度等級設計

### 簡單難度
- AI反應速度降低50%
- AI資源收入減少20%
- AI不會使用複雜戰術
- AI更傾向於和平

### 普通難度
- AI正常運作
- 標準資源收入
- 使用基礎戰術
- 平衡的外交政策

### 困難難度
- AI反應速度提升20%
- AI資源收入增加10%
- 使用進階戰術
- 更積極的擴張政策

### 專家難度
- AI反應速度提升40%
- AI資源收入增加20%
- 使用所有戰術組合
- 最佳決策選擇
- 會利用玩家失誤

### 極難難度
- AI反應速度提升60%
- AI資源收入增加30%
- 完美執行勢力特色策略
- 預判玩家行動
- 幾乎無失誤

---

## AI學習與適應系統

### 行為學習
```cpp
class UMingRTSFactionAILearning
{
    // 學習玩家戰術
    void LearnPlayerTactics(EGameTactics PlayerTactic, float Success);
    
    // 適應玩家策略
    void AdaptToPlayerStrategy(EPlayerStrategy Strategy);
    
    // 記憶歷史對戰
    void RecordBattleOutcome(const FBattleRecord& Record);
    
    // 調整策略權重
    void AdjustStrategyWeights();
};
```

### 動態難度調整
- 根據玩家勝率調整AI難度
- 學習玩家習慣並針對性調整
- 保持挑戰性但避免無法戰勝

---

## 測試與平衡

### AI測試場景
1. **1v1對戰測試**: 每個勢力對戰其他所有勢力
2. **自由對戰測試**: 12勢力大亂鬥
3. **歷史劇本測試**: 還原歷史戰役
4. **經濟競速測試**: AI純經濟發展能力

### AI平衡指標
- **勝率分布**: 各AI對戰勝率應接近50%
- **風格多樣性**: 確保每個AI有獨特風格
- **歷史還原度**: AI行為符合歷史定位
- **玩家體驗**: 提供有趣且公平的挑戰

---

*文檔版本*: 1.0  
*建立日期*: 2026-03-23  
*狀態*: 設計完成
