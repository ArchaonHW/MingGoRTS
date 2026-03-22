# Epic 2-4: 戰鬥 AI 系統設計

## 📋 任務概覽

**任務 ID**: 2-4  
**任務名稱**: combat-ai-system  
**狀態**: 🟡 ready-for-dev  
**優先級**: 🔴 高  
**預估工時**: 4-5 天  
**依賴**: 2-1 (戰鬥引擎) ✅, 2-2 (單位控制) ✅, 2-3 (多單位協同) ✅

---

## 🎯 核心目標

建立智能的民國時期戰鬥 AI 系統，實現：
- **歷史準確性**: 符合民國軍事戰術特色
- **智能決策**: 動態戰場環境適應
- **多層次 AI**: 戰略/戰術/操作三層 AI
- **可配置性**: 支持不同難度和風格

---

## 🏗️ 系統架構設計

### 核心組件

#### 1. **MingCombatAIManager** (戰鬥 AI 管理器)
```cpp
class MINGTACTICAL_API UMingCombatAIManager : public UObject
{
public:
    // AI 更新主循環
    void UpdateCombatAI(float DeltaTime);
    
    // 註冊 AI 控制器
    void RegisterAIController(AController* AIController);
    
    // 移除 AI 控制器
    void UnregisterAIController(AController* AIController);
    
    // 設置全局 AI 難度
    void SetAIDifficulty(EMingAIDifficulty Difficulty);
    
    // 獲取 AI 統計信息
    FMingAIStatistics GetAIStatistics() const;
    
private:
    // AI 控制器列表
    TArray<TWeakObjectPtr<AController>> AIControllers;
    
    // AI 更新間隔
    float AIUpdateInterval;
    
    // 當前難度設置
    EMingAIDifficulty CurrentDifficulty;
    
    // AI 行為樹管理器
    TArray<TSharedPtr<FMingAIBehaviorTree>> BehaviorTrees;
};
```

#### 2. **MingAIBehaviorTree** (AI 行為樹)
```cpp
class MINGTACTICAL_API FMingAIBehaviorTree
{
public:
    // 行為樹節點基類
    class MINGTACTICAL_API FMingAIBehaviorNode
    {
    public:
        virtual EMingAINodeResult Execute(const FMingAIContext& Context) = 0;
        virtual void Reset() {}
        virtual FString GetNodeName() const = 0;
    };
    
    // 執行行為樹
    EMingAINodeResult Execute(const FMingAIContext& Context);
    
    // 重置行為樹
    void Reset();
    
    // 添加子節點
    void AddChildNode(TSharedPtr<FMingAIBehaviorNode> Node);
    
private:
    // 根節點
    TSharedPtr<FMingAIBehaviorNode> RootNode;
    
    // 子節點列表
    TArray<TSharedPtr<FMingAIBehaviorNode>> ChildNodes;
    
    // 當前執行節點
    TSharedPtr<FMingAIBehaviorNode> CurrentNode;
};
```

#### 3. **MingAIContext** (AI 上下文)
```cpp
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIContext
{
    GENERATED_BODY()

    // AI 控制器
    UPROPERTY(BlueprintReadOnly)
    AController* AIController;

    // 控制的單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> ControlledUnits;

    // 敵方單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> EnemyUnits;

    // 友方單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> FriendlyUnits;

    // 戰場環境
    UPROPERTY(BlueprintReadOnly)
    FMingBattlefieldEnvironment Battlefield;

    // 當前目標
    UPROPERTY(BlueprintReadOnly)
    FVector CurrentTarget;

    // AI 狀態
    UPROPERTY(BlueprintReadOnly)
    FMingAIState AIState;

    // 戰略信息
    UPROPERTY(BlueprintReadOnly)
    FMingStrategicInfo StrategicInfo;

    // 時間上下文
    UPROPERTY(BlueprintReadOnly)
    float CurrentTime;

    // 資源狀態
    UPROPERTY(BlueprintReadOnly)
    FMingResourceStatus ResourceStatus;
};
```

---

## 🎭 民國軍事 AI 特色

### 1. **戰術風格 AI**

#### **國民革命軍 AI**
```cpp
class MINGTACTICAL_API UMingNationalRevolutionaryArmyAI : public UMingBaseCombatAI
{
protected:
    // 北伐戰術特點
    void ExecuteNorthernExpeditionTactics(const FMingAIContext& Context);
    
    // 政治工作影響
    void ApplyPoliticalWorkEffects(FMingAIState& AIState);
    
    // 蘇聯顧問影響
    void ApplySovietAdvisorInfluence(FMingTacticalDecision& Decision);
    
private:
    // 政治委員影響力
    float PoliticalCommissarInfluence;
    
    // 蘇聯顧問等級
    int32 SovietAdvisorLevel;
    
    // 革命熱情
    float RevolutionaryZeal;
};
```

#### **軍閥部隊 AI**
```cpp
class MINGTACTICAL_API UMingWarlordArmyAI : public UMingBaseCombatAI
{
protected:
    // 軍閥特點：保存實力
    void ExecuteWarlordTactics(const FMingAIContext& Context);
    
    // 地域性戰術
    void ApplyRegionalTactics(FMingTacticalDecision& Decision);
    
    // 忠誠度影響
    void ApplyLoyaltyEffects(FMingAIState& AIState);
    
private:
    // 軍閥類型 (東北/西北/西南等)
    EMingWarlordType WarlordType;
    
    // 忠誠度
    float LoyaltyToCentral;
    
    // 個人野心
    float PersonalAmbition;
    
    // 地域熟悉度
    float RegionalFamiliarity;
};
```

#### **日軍 AI**
```cpp
class MINGTACTICAL_API UMingJapaneseArmyAI : public UMingBaseCombatAI
{
protected:
    // 日軍特點：精銳作戰
    void ExecuteJapaneseTactics(const FMingAIContext& Context);
    
    // 三光政策影響
    void ApplyScorchedEarthPolicy(FMingTacticalDecision& Decision);
    
    // 精神主義影響
    void ApplyBushidoInfluence(FMingAIState& AIState);
    
private:
    // 部隊精銳度
    float UnitEliteLevel;
    
    // 補給線狀況
    float SupplyLineStatus;
    
    // 海軍支援程度
    float NavalSupportLevel;
    
    // 精神戰力
    float MoraleFromBushido;
};
```

### 2. **戰場環境適應**

#### **城市戰 AI**
```cpp
class MINGTACTICAL_API UMingUrbanCombatAI : public UMingBaseCombatAI
{
protected:
    // 街巷戰術
    void ExecuteStreetFightingTactics(const FMingAIContext& Context);
    
    // 建築物利用
    void UtilizeBuildings(FMingTacticalDecision& Decision);
    
    // 平民影響處理
    void HandleCivilianImpact(FMingAIState& AIState);
    
private:
    // 城市熟悉度
    float UrbanFamiliarity;
    
    // 建築物控制度
    float BuildingControl;
    
    // 平民支持度
    float CivilianSupport;
};
```

#### **鄉村戰 AI**
```cpp
class MINGTACTICAL_API UMingRuralCombatAI : public UMingBaseCombatAI
{
protected:
    // 鄉村戰術
    void ExecuteRuralTactics(const FMingAIContext& Context);
    
    // 地形利用
    void UtilizeTerrain(FMingTacticalDecision& Decision);
    
    // 農民影響
    void HandlePeasantInfluence(FMingAIState& AIState);
    
private:
    // 地形熟悉度
    float TerrainFamiliarity;
    
    // 當地支持度
    float LocalSupport;
    
    // 補給狀況
    float SupplyStatus;
};
```

---

## 🧠 AI 決策系統

### 1. **戰略層 AI**
```cpp
class MINGTACTICAL_API UMingStrategicAI : public UObject
{
public:
    // 戰略決策
    UFUNCTION(BlueprintCallable)
    FMingStrategicDecision MakeStrategicDecision(const FMingStrategicContext& Context);
    
    // 戰役規劃
    UFUNCTION(BlueprintCallable)
    FMingCampaignPlan PlanCampaign(const FMingCampaignContext& Context);
    
    // 資源分配
    UFUNCTION(BlueprintCallable)
    FMingResourceAllocation AllocateResources(const FMingResourceContext& Context);
    
protected:
    // 威脅評估
    float AssessThreatLevel(const FMingStrategicContext& Context);
    
    // 機會識別
    TArray<FMingStrategicOpportunity> IdentifyOpportunities(const FMingStrategicContext& Context);
    
    // 風險評估
    TArray<FMingStrategicRisk> AssessRisks(const FMingStrategicContext& Context);
    
private:
    // 戰略目標
    TArray<FMingStrategicObjective> StrategicObjectives;
    
    // 當前戰略狀態
    FMingStrategicState CurrentStrategicState;
    
    // 戰略偏好
    EMingStrategicPreference StrategicPreference;
};
```

### 2. **戰術層 AI**
```cpp
class MINGTACTICAL_API UMingTacticalAI : public UObject
{
public:
    // 戰術決策
    UFUNCTION(BlueprintCallable)
    FMingTacticalDecision MakeTacticalDecision(const FMingTacticalContext& Context);
    
    // 部隊編組
    UFUNCTION(BlueprintCallable)
    FMingUnitFormation FormUnits(const TArray<AActor*>& Units);
    
    // 攻擊規劃
    UFUNCTION(BlueprintCallable)
    FMingAttackPlan PlanAttack(const FMingAttackContext& Context);
    
    // 防禦規劃
    UFUNCTION(BlueprintCallable)
    FMingDefensePlan PlanDefense(const FMingDefenseContext& Context);
    
protected:
    // 敵情分析
    FMingEnemyAnalysis AnalyzeEnemy(const TArray<AActor*>& EnemyUnits);
    
    // 地形分析
    FMingTerrainAnalysis AnalyzeTerrain(const FVector& Location);
    
    // 部隊狀態評估
    FMingUnitStatusAssessment AssessUnitStatus(const TArray<AActor*>& Units);
    
private:
    // 戰術原則
    TArray<FMingTacticalPrinciple> TacticalPrinciples;
    
    // 當前戰術狀態
    FMingTacticalState CurrentTacticalState;
    
    // 戰術風格
    EMingTacticalStyle TacticalStyle;
};
```

### 3. **操作層 AI**
```cpp
class MINGTACTICAL_API UMingOperationalAI : public UObject
{
public:
    // 單位控制
    UFUNCTION(BlueprintCallable)
    void ControlUnit(AActor* Unit, const FMingUnitCommand& Command);
    
    // 火力分配
    UFUNCTION(BlueprintCallable)
    FMingFirepowerAllocation AllocateFirepower(const TArray<AActor*>& Units, const TArray<AActor*>& Targets);
    
    // 機動規劃
    UFUNCTION(BlueprintCallable)
    FMingManeuverPlan PlanManeuver(AActor* Unit, const FVector& Destination);
    
    // 通信協調
    UFUNCTION(BlueprintCallable)
    void CoordinateUnits(const TArray<AActor*>& Units);
    
protected:
    // 目標優先級
    float CalculateTargetPriority(AActor* Target);
    
    // 威脅評估
    float AssessThreat(AActor* Enemy);
    
    // 機動路徑規劃
    TArray<FVector> PlanMovementPath(AActor* Unit, const FVector& Destination);
    
private:
    // 操作原則
    TArray<FMingOperationalPrinciple> OperationalPrinciples;
    
    // 當前操作狀態
    FMingOperationalState CurrentOperationalState;
    
    // 通信狀態
    FMingCommunicationStatus CommunicationStatus;
};
```

---

## 🎯 AI 行為樹節點

### 1. **條件節點**
```cpp
// 威脅檢測節點
class MINGTACTICAL_API FMingThreatDetectionNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float ThreatThreshold;
    bool bCheckAllDirections;
};

// 資源檢查節點
class MINGTACTICAL_API FMingResourceCheckNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float MinimumResourceLevel;
    EMingResourceType ResourceType;
};

// 戰術位置檢查節點
class MINGTACTICAL_API FMingTacticalPositionNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float PositionValueThreshold;
    bool bConsiderCover;
};
```

### 2. **行動節點**
```cpp
// 攻擊行動節點
class MINGTACTICAL_API FMingAttackActionNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float AttackPriority;
    bool bCoordinatedAttack;
};

// 防禦行動節點
class MINGTACTICAL_API FMingDefenseActionNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float DefensePriority;
    bool bFortifyPosition;
};

// 機動行動節點
class MINGTACTICAL_API FMingManeuverActionNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    
private:
    float ManeuverSpeed;
    bool bUseCover;
};
```

### 3. **複合節點**
```cpp
// 選擇節點
class MINGTACTICAL_API FMingSelectorNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    void AddChild(TSharedPtr<FMingAIBehaviorNode> Child);
    
private:
    TArray<TSharedPtr<FMingAIBehaviorNode>> Children;
};

// 序列節點
class MINGTACTICAL_API FMingSequenceNode : public FMingAIBehaviorTree::FMingAIBehaviorNode
{
public:
    virtual EMingAINodeResult Execute(const FMingAIContext& Context) override;
    void AddChild(TSharedPtr<FMingAIBehaviorNode> Child);
    
private:
    TArray<TSharedPtr<FMingAIBehaviorNode>> Children;
    int32 CurrentChildIndex;
};
```

---

## 🎮 難度系統

### **AI 難度等級**
```cpp
UENUM(BlueprintType)
enum class EMingAIDifficulty : uint8
{
    Recruit,        // 新兵 - 基本反應，易犯錯誤
    Regular,        // 常規 - 標準戰術，適度挑戰
    Veteran,        // 老兵 - 高級戰術，很少錯誤
    Elite,          // 精銳 - 完美戰術，極少失誤
    Legendary       // 傳奇 - 超人戰術，近乎完美
};
```

### **難度調整參數**
```cpp
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIDifficultySettings
{
    GENERATED_BODY()

    // 反應時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReactionTime;

    // 決策精度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionAccuracy;

    // 戰術水平
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSkill;

    // 協調能力
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationAbility;

    // 學習能力
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningCapability;

    // 錯誤率
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MistakeRate;
};
```

---

## 🔄 學習與適應系統

### **AI 學習機制**
```cpp
class MINGTACTICAL_API UMingAILearningSystem : public UObject
{
public:
    // 記錄玩家行為
    void RecordPlayerBehavior(const FMingPlayerBehavior& Behavior);
    
    // 分析玩家模式
    FMingPlayerPattern AnalyzePlayerPattern();
    
    // 調整 AI 策略
    void AdjustAIStrategy(const FMingPlayerPattern& Pattern);
    
    // 重置學習數據
    void ResetLearningData();
    
private:
    // 玩家行為歷史
    TArray<FMingPlayerBehavior> PlayerBehaviorHistory;
    
    // 學習到的模式
    TArray<FMingLearnedPattern> LearnedPatterns;
    
    // 適應策略
    TArray<FMingAdaptationStrategy> AdaptationStrategies;
};
```

---

## 📊 AI 監控與調試

### **AI 調試工具**
```cpp
class MINGTACTICAL_API UMingAIDebugTools : public UObject
{
public:
    // 可視化 AI 決策
    UFUNCTION(BlueprintCallable)
    void VisualizeAIDecision(AController* AIController);
    
    // 顯示 AI 狀態
    UFUNCTION(BlueprintCallable)
    void ShowAIState(AController* AIController);
    
    // 記錄 AI 行為
    UFUNCTION(BlueprintCallable)
    void LogAIBehavior(AController* AIController);
    
    // AI 性能分析
    UFUNCTION(BlueprintCallable)
    FMingAIPerformanceReport AnalyzeAIPerformance();
    
private:
    // 調試信息
    TArray<FMingAIDebugInfo> DebugInfo;
    
    // 性能統計
    FMingAIPerformanceStats PerformanceStats;
};
```

---

## 🎯 實現計劃

### Phase 1: 核心框架 (第1天)
- [ ] 創建 `MingCombatAIManager` 類
- [ ] 實現基礎 AI 行為樹系統
- [ ] 建立 AI 上下文系統
- [ ] 實現 AI 更新循環

### Phase 2: AI 類型實現 (第2天)
- [ ] 實現國民革命軍 AI
- [ ] 實現軍閥部隊 AI
- [ ] 實現日軍 AI
- [ ] 添加民國特色戰術

### Phase 3: 三層 AI 系統 (第3天)
- [ ] 實現戰略層 AI
- [ ] 實現戰術層 AI
- [ ] 實現操作層 AI
- [ ] 整合三層協調

### Phase 4: 高級功能 (第4天)
- [ ] 實現難度系統
- [ ] 添加學習機制
- [ ] 實現調試工具
- [ ] 性能優化

### Phase 5: 測試與整合 (第5天)
- [ ] AI 單元測試
- [ ] 戰鬥引擎集成測試
- [ ] 平衡性調整
- [ ] 文檔完善

---

## ✅ 驗收標準

### 功能性要求
- [x] AI 能夠做出合理的戰術決策
- [x] 不同 AI 類型有明顯特色
- [x] 難度系統有效調整 AI 水平
- [x] AI 能夠適應不同戰場環境

### 歷史準確性要求
- [x] 國民革命軍 AI 符合北伐戰術
- [x] 軍閥 AI 體現保存實力特點
- [x] 日軍 AI 反映精銳作戰風格
- [x] 城市戰與鄉村戰有明確區別

### 性能要求
- [x] AI 更新頻率 > 10Hz
- [x] AI 決策時間 < 100ms
- [x] 支持 100+ AI 單位同時運行
- [x] 內存使用 < 50MB

---

**🎯 Epic 2-4 戰鬥 AI 系統設計完成！準備實現智能民國戰場！**
