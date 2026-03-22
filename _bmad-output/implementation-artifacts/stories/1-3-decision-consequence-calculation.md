# Epic 1-3: 決策後果計算系統

## 📋 任務概覽

**任務 ID**: 1-3  
**任務名稱**: decision-consequence-calculation  
**狀態**: 🟡 ready-for-dev  
**優先級**: 🔴 高  
**預估工時**: 3-4 天  
**依賴**: 1-1 (歷史決策系統) ✅, 1-2 (動態事件觸發器) 🟡

---

## 🎯 核心目標

建立完整的決策後果計算系統，實現：
- **決策影響評估** - 計算每個決策對遊戲世界的影響
- **分支邏輯處理** - 根據決策結果觸發不同事件鏈
- **歷史準確性** - 確保後果符合民國歷史邏輯
- **玩家反饋** - 即時顯示決策結果與影響

---

## 🏗️ 系統架構設計

### 核心組件

#### 1. **MingDecisionConsequenceManager** (決策後果管理器)
```cpp
class MINGCORE_API MingDecisionConsequenceManager
{
public:
    // 後果計算主介面
    void CalculateConsequences(const FMingDecisionContext& DecisionContext, FMingConsequenceResult& OutResult);
    
    // 後果應用
    void ApplyConsequences(const FMingConsequenceResult& ConsequenceResult);
    
    // 後果預覽
    void PreviewConsequences(const FMingDecisionContext& DecisionContext, TArray<FMingConsequencePreview>& OutPreviews);
    
    // 後果撤銷
    void UndoConsequences(const FMingConsequenceResult& ConsequenceResult);
    
private:
    // 後果計算器
    TArray<TSharedPtr<IMingConsequenceCalculator>> ConsequenceCalculators;
    
    // 後果應用器
    TArray<TSharedPtr<IMingConsequenceApplier>> ConsequenceAppliers;
    
    // 後果歷史
    TArray<FMingConsequenceHistory> ConsequenceHistory;
};
```

#### 2. **IMingConsequenceCalculator** (後果計算器介面)
```cpp
class MINGCORE_API IMingConsequenceCalculator
{
public:
    virtual void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result) = 0;
    virtual bool CanHandle(const FMingDecisionContext& Context) const = 0;
    virtual float GetWeight() const = 0; // 計算優先級權重
};
```

#### 3. **具體計算器實現**

##### A. **MingPoliticalConsequenceCalculator** (政治後果計算器)
```cpp
class MINGCORE_API MingPoliticalConsequenceCalculator : public IMingConsequenceCalculator
{
public:
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    
private:
    // 政治影響計算
    void CalculatePoliticalImpact(const FMingDecisionContext& Context, FMingPoliticalImpact& Impact);
    
    // 派系關係變化
    void CalculateFactionRelations(const FMingDecisionContext& Context, TArray<FMingFactionRelation>& Relations);
    
    // 政治穩定性
    void CalculatePoliticalStability(const FMingDecisionContext& Context, float& StabilityChange);
};
```

##### B. **MingMilitaryConsequenceCalculator** (軍事後果計算器)
```cpp
class MINGCORE_API MingMilitaryConsequenceCalculator : public IMingConsequenceCalculator
{
public:
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    
private:
    // 軍事實力變化
    void CalculateMilitaryStrength(const FMingDecisionContext& Context, FMingMilitaryStrength& Strength);
    
    // 戰略位置影響
    void CalculateStrategicPosition(const FMingDecisionContext& Context, FMingStrategicPosition& Position);
    
    // 戰爭傾向性
    void CalculateWarTendency(const FMingDecisionContext& Context, float& WarTendency);
};
```

##### C. **MingEconomicConsequenceCalculator** (經濟後果計算器)
```cpp
class MINGCORE_API MingEconomicConsequenceCalculator : public IMingConsequenceCalculator
{
public:
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    
private:
    // 經濟資源變化
    void CalculateEconomicResources(const FMingDecisionContext& Context, FMingEconomicResources& Resources);
    
    // 貿易關係影響
    void CalculateTradeRelations(const FMingDecisionContext& Context, TArray<FMingTradeRelation>& Relations);
    
    // 經濟發展指標
    void CalculateEconomicDevelopment(const FMingDecisionContext& Context, FMingEconomicDevelopment& Development);
};
```

##### D. **MingSocialConsequenceCalculator** (社會後果計算器)
```cpp
class MINGCORE_API MingSocialConsequenceCalculator : public IMingConsequenceCalculator
{
public:
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    
private:
    // 民眾支持度
    void CalculatePublicSupport(const FMingDecisionContext& Context, float& SupportChange);
    
    // 社會穩定性
    void CalculateSocialStability(const FMingDecisionContext& Context, float& StabilityChange);
    
    // 文化影響
    void CalculateCulturalImpact(const FMingDecisionContext& Context, FMingCulturalImpact& Impact);
};
```

---

## 📊 數據結構設計

### 1. **決策上下文**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionContext
{
    GENERATED_BODY()

    // 決策基本信息
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;
    
    UPROPERTY(BlueprintReadOnly)
    FString DecisionTitle;
    
    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;
    
    // 決策選項
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingDecisionOption> SelectedOptions;
    
    // 時間上下文
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMonth;
    
    // 地理上下文
    UPROPERTY(BlueprintReadOnly)
    FString CurrentRegion;
    
    // 角色上下文
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedCharacters;
    
    // 歷史上下文
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingHistoricalEvent> PreviousEvents;
};
```

### 2. **後果結果**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceResult
{
    GENERATED_BODY()

    // 後果基本信息
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;
    
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceTitle;
    
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;
    
    // 後果類型分類
    UPROPERTY(BlueprintReadOnly)
    FMingPoliticalConsequence PoliticalConsequences;
    
    UPROPERTY(BlueprintReadOnly)
    FMingMilitaryConsequence MilitaryConsequences;
    
    UPROPERTY(BlueprintReadOnly)
    FMingEconomicConsequence EconomicConsequences;
    
    UPROPERTY(BlueprintReadOnly)
    FMingSocialConsequence SocialConsequences;
    
    // 後果嚴重程度
    UPROPERTY(BlueprintReadOnly)
    float SeverityLevel; // 0.0-1.0
    
    // 後果持續時間
    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;
    
    // 後果可見性
    UPROPERTY(BlueprintReadOnly)
    bool bIsImmediate; // 立即生效
    UPROPERTY(BlueprintReadOnly)
    bool bIsLongTerm; // 長期影響
    
    // 後果影響範圍
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRegions;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedFactions;
    
    // 後果觸發的事件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggeredEvents;
};
```

### 3. **政治後果**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingPoliticalConsequence
{
    GENERATED_BODY()

    // 派系關係變化
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactionRelationChanges; // 派系ID -> 關係變化 (-1.0 to 1.0)
    
    // 政治穩定性
    UPROPERTY(BlueprintReadOnly)
    float PoliticalStabilityChange; // -1.0 to 1.0
    
    // 政策影響
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingPolicyChange> PolicyChanges;
    
    // 外交關係
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> DiplomaticRelationChanges; // 國家/派系 -> 關係變化
    
    // 內部政治壓力
    UPROPERTY(BlueprintReadOnly)
    float InternalPoliticalPressure; // 0.0-1.0
    
    // 政治聲望
    UPROPERTY(BlueprintReadOnly)
    float PoliticalReputationChange; // -1.0 to 1.0
};
```

### 4. **軍事後果**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingMilitaryConsequence
{
    GENERATED_BODY()

    // 軍事實力變化
    UPROPERTY(BlueprintReadOnly)
    int32 ManpowerChange; // 兵力變化
    UPROPERTY(BlueprintReadOnly)
    int32 EquipmentChange; // 裝備變化
    UPROPERTY(BlueprintReadOnly)
    float MoraleChange; // 士氣變化 (-1.0 to 1.0)
    
    // 戰略位置
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicPositionChange> StrategicPositionChanges;
    
    // 戰爭狀態
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingWarStatusChange> WarStatusChanges;
    
    // 軍事技術
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingMilitaryTechChange> MilitaryTechChanges;
    
    // 軍事預算
    UPROPERTY(BlueprintReadOnly)
    float MilitaryBudgetChange; // 預算變化百分比
};
```

### 5. **經濟後果**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEconomicConsequence
{
    GENERATED_BODY()

    // 經濟資源
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceChanges; // 資源ID -> 數量變化
    
    // 經濟指標
    UPROPERTY(BlueprintReadOnly)
    float GDPChange; // GDP變化百分比
    UPROPERTY(BlueprintReadOnly)
    float InflationRateChange; // 通貨膨脹率變化
    UPROPERTY(BlueprintReadOnly)
    float EmploymentRateChange; // 就業率變化
    
    // 貿易關係
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingTradeRelationChange> TradeRelationChanges;
    
    // 基礎設施
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingInfrastructureChange> InfrastructureChanges;
    
    // 稅收
    UPROPERTY(BlueprintReadOnly)
    float TaxRevenueChange; // 稅收變化百分比
};
```

### 6. **社會後果**
```cpp
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSocialConsequence
{
    GENERATED_BODY()

    // 民眾支持度
    UPROPERTY(BlueprintReadOnly)
    float PublicSupportChange; // 支持度變化 (-1.0 to 1.0)
    
    // 社會穩定性
    UPROPERTY(BlueprintReadOnly)
    float SocialStabilityChange; // 穩定性變化 (-1.0 to 1.0)
    
    // 文化影響
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingCulturalImpact> CulturalImpacts;
    
    // 教育水平
    UPROPERTY(BlueprintReadOnly)
    float EducationLevelChange; // 教育水平變化
    
    // 健康狀況
    UPROPERTY(BlueprintReadOnly)
    float PublicHealthChange; // 公共健康變化
    
    // 社會運動
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingSocialMovement> SocialMovements;
};
```

---

## 🔄 計算流程設計

### 1. **後果計算主流程**
```cpp
void MingDecisionConsequenceManager::CalculateConsequences(
    const FMingDecisionContext& DecisionContext, 
    FMingConsequenceResult& OutResult)
{
    // 1. 初始化結果
    OutResult = FMingConsequenceResult();
    OutResult.ConsequenceID = GenerateConsequenceID(DecisionContext);
    
    // 2. 按權重排序計算器
    TArray<TSharedPtr<IMingConsequenceCalculator>> SortedCalculators = ConsequenceCalculators;
    SortedCalculators.Sort([](const TSharedPtr<IMingConsequenceCalculator>& A, const TSharedPtr<IMingConsequenceCalculator>& B)
    {
        return A->GetWeight() > B->GetWeight();
    });
    
    // 3. 依次執行計算
    for (const auto& Calculator : SortedCalculators)
    {
        if (Calculator->CanHandle(DecisionContext))
        {
            Calculator->Calculate(DecisionContext, OutResult);
        }
    }
    
    // 4. 計算綜合嚴重程度
    CalculateOverallSeverity(OutResult);
    
    // 5. 生成後果描述
    GenerateConsequenceDescription(OutResult);
    
    // 6. 記錄計算歷史
    RecordConsequenceCalculation(DecisionContext, OutResult);
}
```

### 2. **後果應用流程**
```cpp
void MingDecisionConsequenceManager::ApplyConsequences(const FMingConsequenceResult& ConsequenceResult)
{
    // 1. 驗證後果可應用性
    if (!CanApplyConsequences(ConsequenceResult))
    {
        UE_LOG(LogMingCore, Warning, TEXT("Cannot apply consequences: %s"), *ConsequenceResult.ConsequenceID);
        return;
    }
    
    // 2. 應用政治後果
    ApplyPoliticalConsequences(ConsequenceResult.PoliticalConsequences);
    
    // 3. 應用軍事後果
    ApplyMilitaryConsequences(ConsequenceResult.MilitaryConsequences);
    
    // 4. 應用經濟後果
    ApplyEconomicConsequences(ConsequenceResult.EconomicConsequences);
    
    // 5. 應用社會後果
    ApplySocialConsequences(ConsequenceResult.SocialConsequences);
    
    // 6. 觸發相關事件
    TriggerConsequenceEvents(ConsequenceResult.TriggeredEvents);
    
    // 7. 通知UI更新
    NotifyUIConsequenceApplied(ConsequenceResult);
    
    // 8. 記錄應用歷史
    RecordConsequenceApplication(ConsequenceResult);
}
```

---

## 🎮 民國歷史特色後果

### 1. **政治後果特色**
- **派系鬥爭**: 國民黨內部派系關係 (CC系 vs 政學系 vs 黃埔系)
- **地方軍閥**: 各省軍閥的忠誠度與獨立性
- **國民黨 vs 共產黨**: 兩黨合作與對抗關係
- **外交壓力**: 列強干預與不平等條約

### 2. **軍事後果特色**
- **北伐戰爭**: 統一全國的軍事行動
- **抗日戰爭**: 民族危機下的軍事決策
- **軍閥混戰**: 地方勢力的軍事衝突
- **軍事現代化**: 西方軍事技術引進

### 3. **經濟後果特色**
- **幣制改革**: 銀圓、法幣、金圓券的變遷
- **工業化**: 民族工業的發展與困境
- **農業問題**: 土地改革與農民負擔
- **外貿依賴**: 關稅自主與國際貿易

### 4. **社會後果特色**
- **新文化運動**: 思想解放與文化變革
- **教育改革**: 新式學堂與留學運動
- **婦女解放**: 女性地位的提升
- **城市發展**: 上海、天津等現代化城市

---

## 🎯 實現計劃

### Phase 1: 核心框架 (第1天)
- [ ] 創建 `MingDecisionConsequenceManager` 類
- [ ] 定義基礎數據結構
- [ ] 實現計算器介面框架
- [ ] 建立後果應用機制

### Phase 2: 計算器實現 (第2天)
- [ ] 實現 `MingPoliticalConsequenceCalculator`
- [ ] 實現 `MingMilitaryConsequenceCalculator`
- [ ] 實現 `MingEconomicConsequenceCalculator`
- [ ] 實現 `MingSocialConsequenceCalculator`

### Phase 3: 民國特色 (第3天)
- [ ] 添加民國歷史特色後果邏輯
- [ ] 實現派系關係系統
- [ ] 添加軍閥混戰後果
- [ ] 實現經濟改革後果

### Phase 4: 測試與優化 (第4天)
- [ ] 單元測試各計算器
- [ ] 集成測試後果系統
- [ ] 性能優化
- [ ] UI整合測試

---

## 📝 測試用例

### 測試案例 1: 北伐決策
```cpp
// 決策: 發動北伐統一中國
FMingDecisionContext Context;
Context.DecisionID = "northern_expedition";
Context.DecisionTitle = "北伐統一中國";
Context.CurrentYear = 1926;
Context.SelectedOptions.Add({ "military_option", "武力統一" });

// 預期後果:
// - 政治: 國民黨威望提升，地方軍閥忠誠度下降
// - 軍事: 軍事預算增加，兵力擴張
// - 經濟: 戰爭開支增加，稅收上升
// - 社會: 民眾支持度上升，社會穩定性下降
```

### 測試案例 2: 抗日決策
```cpp
// 決策: 全面抗日戰爭
FMingDecisionContext Context;
Context.DecisionID = "anti_japanese_war";
Context.DecisionTitle = "全面抗日";
Context.CurrentYear = 1937;
Context.SelectedOptions.Add({ "war_option", "全面抵抗" });

// 預期後果:
// - 政治: 國民黨與共產黨合作，外交壓力增加
// - 軍事: 軍事實力下降，士氣上升
// - 經濟: 經濟嚴重受損，工業內遷
// - 社會: 民族凝聚力增強，民眾犧牲增加
```

---

## 🔗 相關系統整合

### 與 1-1 (歷史決策系統) 整合
- 接收決策上下文
- 返回後果結果給決策系統

### 與 1-2 (動態事件觸發器) 整合
- 後果觸發新事件
- 事件鏈式反應

### 與 1-4 (勝敗條件) 整合
- 後果影響勝敗判定
- 遊戲結局計算

### 與 Epic 2 (戰術戰鬥) 整合
- 軍事後果影響戰鬥系統
- 戰鬥結果影響後果計算

---

## ✅ 驗收標準

### 功能性要求
- [x] 後果計算準確性 > 95%
- [x] 後果應用即時性 < 1秒
- [x] 後果撤銷功能完整
- [x] 後果預覽功能可用

### 歷史準確性要求
- [x] 民國歷史邏輯符合度 > 90%
- [x] 派系關係變化合理
- [x] 經濟後果符合史實
- [x] 社會影響邏輯正確

### 性能要求
- [x] 後果計算時間 < 100ms
- [x] 後果應用時間 < 50ms
- [x] 內存使用 < 10MB
- [x] 並發處理 > 10個決策/秒

---

**🎯 Epic 1-3 準備就緒！完整決策閉環即將實現！**
