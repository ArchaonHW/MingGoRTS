# MingGoRTS 程序实现 - 特色机制系统

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Programming Team
- **状态**: 实施中

---

## 1. 系统架构

### 1.1 机制系统架构图

```
┌─────────────────────────────────────────────────────────────┐
│                    MingGoRTS Game Instance                   │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────────┐    ┌─────────────────────────────┐  │
│  │  UMingMechanicManager │    │   UMingEventManager        │  │
│  │  - FactionMechanics   │    │   - EventDatabase          │  │
│  │  - GlobalModifiers    │    │   - ActiveEvents           │  │
│  │  - MechanicRegistry   │    │   - EventQueue             │  │
│  └──────────┬──────────┘    └──────────────┬──────────────┘  │
│             │                                │               │
│     ┌───────┴───────┐                ┌───────┴───────┐       │
│     ▼               ▼                ▼               ▼       │
│ ┌──────────┐   ┌──────────┐    ┌──────────┐   ┌──────────┐  │
│ │MechanicA │   │MechanicB │    │EventA    │   │EventB    │  │
│ │(北洋)   │   │(国民政府)│    │(袁世凯称帝)│   │(北伐战争) │  │
│ └──────────┘   └──────────┘    └──────────┘   └──────────┘  │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 核心类设计

```cpp
// 机制管理器 - 统一入口
UCLASS()
class MINGGORTS_API UMingMechanicManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 初始化/关闭
    void InitializeManager();
    void ShutdownManager();
    
    // 势力机制管理
    void RegisterFactionMechanic(EFactionType Faction, UMingFactionMechanic* Mechanic);
    UMingFactionMechanic* GetFactionMechanic(EFactionType Faction);
    
    // 机制效果应用
    void ApplyMechanicEffect(EFactionType Faction, const FMechanicEffect& Effect);
    void RemoveMechanicEffect(EFactionType Faction, const FGuid& EffectID);
    
    // 全局修改器
    void AddGlobalModifier(const FGlobalModifier& Modifier);
    void RemoveGlobalModifier(const FGuid& ModifierID);
    
    // 事件委托
    UPROPERTY(BlueprintAssignable)
    FOnMechanicActivated OnMechanicActivated;
    
    UPROPERTY(BlueprintAssignable)
    FOnMechanicDeactivated OnMechanicDeactivated;

private:
    UPROPERTY()
    TMap<EFactionType, TObjectPtr<UMingFactionMechanic>> FactionMechanics;
    
    UPROPERTY()
    TArray<FGlobalModifier> ActiveGlobalModifiers;
    
    UPROPERTY()
    TObjectPtr<UMingMechanicEffectProcessor> EffectProcessor;
};
```

---

## 2. 势力特色机制实现

### 2.1 北洋政府机制

#### 机制1: 军阀妥协 (Warlord Compromise)

```cpp
UCLASS()
class MINGGORTS_API UMingBeiyangWarlordMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    virtual void TickMechanic(float DeltaTime) override;
    virtual void OnMechanicActivated() override;
    virtual void OnMechanicDeactivated() override;

    // 妥协点数管理
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Warlord")
    void AddCompromisePoints(float Points);
    
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Warlord")
    bool SpendCompromisePoints(float Points);
    
    UFUNCTION(BlueprintPure, Category = "Beiyang|Warlord")
    float GetCompromisePoints() const { return CompromisePoints; }
    
    // 临时增益效果
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Warlord")
    void ActivateTemporaryBoost(ETemporaryBoostType BoostType);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxCompromisePoints = 100.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float PassiveRegenRate = 2.0f; // 每秒恢复点数
    
    UPROPERTY()
    float CompromisePoints = 0.0f;
    
    // 激活的增益效果
    UPROPERTY()
    TArray<FActiveBoost> ActiveBoosts;
    
    void ProcessPassiveRegeneration(float DeltaTime);
    void UpdateBoostEffects(float DeltaTime);
};

// 实现文件 (.cpp)
void UMingBeiyangWarlordMechanic::InitializeMechanic()
{
    Super::InitializeMechanic();
    
    // 初始化妥协点数
    CompromisePoints = MaxCompromisePoints * 0.5f; // 初始50%
    
    // 注册效果处理器
    RegisterEffectHandler(TEXT("CompromiseRegen"), 
        FMechanicEffectDelegate::CreateUObject(this, &UMingBeiyangWarlordMechanic::ProcessPassiveRegeneration));
}

void UMingBeiyangWarlordMechanic::AddCompromisePoints(float Points)
{
    CompromisePoints = FMath::Clamp(CompromisePoints + Points, 0.0f, MaxCompromisePoints);
    
    // 广播点数变化
    OnCompromisePointsChanged.Broadcast(CompromisePoints, MaxCompromisePoints);
    
    // 检查阈值触发的特殊效果
    if (CompromisePoints >= MaxCompromisePoints * 0.8f)
    {
        TriggerHighCompromiseBonus();
    }
}

void UMingBeiyangWarlordMechanic::ActivateTemporaryBoost(ETemporaryBoostType BoostType)
{
    float Cost = GetBoostCost(BoostType);
    
    if (CompromisePoints < Cost)
    {
        UE_LOG(LogMingMechanics, Warning, TEXT("Insufficient compromise points for boost"));
        return;
    }
    
    // 扣除点数
    SpendCompromisePoints(Cost);
    
    // 创建增益效果
    FActiveBoost NewBoost;
    NewBoost.BoostType = BoostType;
    NewBoost.Duration = GetBoostDuration(BoostType);
    NewBoost.RemainingTime = NewBoost.Duration;
    NewBoost.EffectID = FGuid::NewGuid();
    
    // 应用效果
    ApplyBoostEffect(NewBoost);
    
    ActiveBoosts.Add(NewBoost);
    
    UE_LOG(LogMingMechanics, Log, TEXT("Activated boost: %s"), 
        *UEnum::GetValueAsString(BoostType));
}
```

#### 机制2: 德国军事顾问

```cpp
UCLASS()
class MINGGORTS_API UMingBeiyangGermanAdvisorMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    UFUNCTION(BlueprintCallable, Category = "Beiyang|GermanAdvisor")
    void HireGermanAdvisor(EGermanAdvisorType AdvisorType);
    
    UFUNCTION(BlueprintCallable, Category = "Beiyang|GermanAdvisor")
    void DismissAdvisor(FGuid AdvisorID);
    
    // 战术模板系统
    UFUNCTION(BlueprintCallable, Category = "Beiyang|GermanAdvisor")
    void ActivateTacticalTemplate(ETacticalTemplate Template);

private:
    UPROPERTY()
    TArray<FActiveAdvisor> ActiveAdvisors;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    int32 MaxAdvisors = 2;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float BaseAdvisorCost = 500.0f;
    
    // 顾问效果配置
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    TMap<EGermanAdvisorType, FAdvisorConfig> AdvisorConfigs;
};
```

#### 机制3: 北洋正统

```cpp
UCLASS()
class MINGGORTS_API UMingBeiyangLegitimacyMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    // 正统度管理
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Legitimacy")
    void AddLegitimacy(float Amount);
    
    UFUNCTION(BlueprintPure, Category = "Beiyang|Legitimacy")
    float GetLegitimacyPercentage() const;
    
    // 外交效果
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Legitimacy")
    float GetDiplomaticBonus() const;
    
    // 势力召唤
    UFUNCTION(BlueprintCallable, Category = "Beiyang|Legitimacy")
    bool CanSummonAlliedFaction(EFactionType Faction) const;

private:
    UPROPERTY()
    float LegitimacyPoints = 50.0f; // 初始50
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxLegitimacy = 100.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float DecayRate = 1.0f; // 自然衰减
    
    // 影响正统度的因素
    UPROPERTY()
    TMap<ELegitimacySource, float> LegitimacySources;
};
```

### 2.2 国民政府机制

#### 机制1: 黄埔军校

```cpp
UCLASS()
class MINGGORTS_API UMingGMDWhampoaMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    // 学员训练
    UFUNCTION(BlueprintCallable, Category = "GMD|Whampoa")
    void StartCadetTraining(EWhampoaSpecialization Specialization);
    
    UFUNCTION(BlueprintCallable, Category = "GMD|Whampoa")
    void GraduateCadets(int32 Count);
    
    // 精英单位生产
    UFUNCTION(BlueprintCallable, Category = "GMD|Whampoa")
    AUnit* ProduceEliteUnit(EWhampoaSpecialization Type);
    
    // 训练加成
    UFUNCTION(BlueprintPure, Category = "GMD|Whampoa")
    float GetTrainingSpeedBonus() const;

private:
    UPROPERTY()
    TArray<FCadetTraining> ActiveTrainings;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    int32 MaxConcurrentTrainings = 3;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float BaseTrainingSpeed = 1.0f;
    
    // 毕业学员累计数量（解锁进阶特殊化）
    UPROPERTY()
    TMap<EWhampoaSpecialization, int32> GraduateCounts;
};
```

#### 机制2: 新生活运动

```cpp
UCLASS()
class MINGGORTS_API UMingGMDNewLifeMovementMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    UFUNCTION(BlueprintCallable, Category = "GMD|NewLife")
    void PromoteNewLifeValue(ENewLifeValue Value);
    
    UFUNCTION(BlueprintPure, Category = "GMD|NewLife")
    float GetPublicMorale() const { return PublicMorale; }
    
    // 资源转换
    UFUNCTION(BlueprintCallable, Category = "GMD|NewLife")
    float ConvertCultureToProduction(float CultureAmount);

private:
    UPROPERTY()
    float PublicMorale = 50.0f;
    
    UPROPERTY()
    TMap<ENewLifeValue, float> ValuePromotions;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxMoraleBonus = 0.3f; // 30%生产效率加成
};
```

#### 机制3: 德械师装备

```cpp
UCLASS()
class MINGGORTS_API UMingGMDGermanEquipmentMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    // 装备分配
    UFUNCTION(BlueprintCallable, Category = "GMD|GermanEquipment")
    void AllocateEquipmentToUnit(AUnit* Unit, EEquipmentType Type);
    
    // 装备采购
    UFUNCTION(BlueprintCallable, Category = "GMD|GermanEquipment")
    bool PurchaseEquipment(EEquipmentType Type, int32 Quantity);
    
    UFUNCTION(BlueprintPure, Category = "GMD|GermanEquipment")
    int32 GetEquipmentStock(EEquipmentType Type) const;

private:
    UPROPERTY()
    TMap<EEquipmentType, int32> EquipmentStock;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    int32 MaxEquipmentStock = 100;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    TMap<EEquipmentType, float> EquipmentStats;
};
```

### 2.3 中共机制

#### 机制1: 游击战

```cpp
UCLASS()
class MINGGORTS_API UMingCCPGuerrillaWarfareMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    UFUNCTION(BlueprintCallable, Category = "CCP|Guerrilla")
    void EnterGuerrillaMode(AUnit* Unit);
    
    UFUNCTION(BlueprintCallable, Category = "CCP|Guerrilla")
    void ExitGuerrillaMode(AUnit* Unit);
    
    // 埋伏系统
    UFUNCTION(BlueprintCallable, Category = "CCP|Guerrilla")
    void SetupAmbush(const FVector& Location, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "CCP|Guerrilla")
    void TriggerAmbush(const FVector& EnemyLocation);
    
    // 地形优势
    UFUNCTION(BlueprintPure, Category = "CCP|Guerrilla")
    float GetTerrainBonus(const FVector& Location) const;

private:
    UPROPERTY()
    TArray<FAmbushPoint> ActiveAmbushes;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxAmbushBonus = 2.0f; // 最大2倍伤害
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float StealthDuration = 30.0f;
    
    // 地形类型加成表
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    TMap<ETerrainType, float> TerrainBonuses;
};
```

#### 机制2: 土地改革

```cpp
UCLASS()
class MINGGORTS_API UMingCCPLandReformMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    UFUNCTION(BlueprintCallable, Category = "CCP|LandReform")
    void ImplementLandReform(const FRegionData& Region);
    
    UFUNCTION(BlueprintPure, Category = "CCP|LandReform")
    float GetPeasantSupport(const FRegionData& Region) const;
    
    // 兵员征召
    UFUNCTION(BlueprintCallable, Category = "CCP|LandReform")
    int32 RecruitPeasantSoldiers(const FRegionData& Region, int32 MaxCount);

private:
    UPROPERTY()
    TMap<FRegionID, FLandReformStatus> ReformStatus;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float BaseSupportGain = 10.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float RecruitRatio = 0.15f; // 15%支持度可转化为兵员
};
```

#### 机制3: 统一战线

```cpp
UCLASS()
class MINGGORTS_API UMingCCPUnitedFrontMechanic : public UMingFactionMechanic
{
    GENERATED_BODY()

public:
    virtual void InitializeMechanic() override;
    
    UFUNCTION(BlueprintCallable, Category = "CCP|UnitedFront")
    void ProposeAlliance(EFactionType TargetFaction);
    
    UFUNCTION(BlueprintCallable, Category = "CCP|UnitedFront")
    void BreakAlliance(EFactionType AllyFaction);
    
    UFUNCTION(BlueprintPure, Category = "CCP|UnitedFront")
    bool HasActiveAlliance() const;
    
    // 联盟共享
    UFUNCTION(BlueprintCallable, Category = "CCP|UnitedFront")
    void ShareResourcesWithAlly(EFactionType Ally, FResourceAmount Amount);

private:
    UPROPERTY()
    TArray<FUnitedFrontAlliance> ActiveAlliances;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    int32 MaxAlliances = 2;
    
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float ResourceSharingEfficiency = 0.8f; // 80%效率
};
```

---

## 3. 历史事件系统实现

### 3.1 事件管理器

```cpp
UCLASS()
class MINGGORTS_API UMingEventManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Event System")
    void InitializeEventSystem();
    
    // 事件触发
    UFUNCTION(BlueprintCallable, Category = "Event System")
    void TriggerEvent(const FName& EventID);
    
    UFUNCTION(BlueprintCallable, Category = "Event System")
    void TriggerEventWithChoices(const FName& EventID, const FEventChoices& Choices);
    
    // 事件查询
    UFUNCTION(BlueprintPure, Category = "Event System")
    bool IsEventAvailable(const FName& EventID) const;
    
    UFUNCTION(BlueprintPure, Category = "Event System")
    TArray<FAvailableEvent> GetAvailableEvents() const;
    
    // 事件链
    UFUNCTION(BlueprintCallable, Category = "Event System")
    void StartEventChain(const FName& ChainID);
    
    // 时间线推进
    UFUNCTION(BlueprintCallable, Category = "Event System")
    void AdvanceGameTime(float Days);

    // 事件委托
    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;
    
    UPROPERTY(BlueprintAssignable)
    FOnEventCompleted OnEventCompleted;

private:
    UPROPERTY()
    TObjectPtr<UMingEventDatabase> EventDatabase;
    
    UPROPERTY()
    TArray<FActiveEvent> ActiveEvents;
    
    UPROPERTY()
    TArray<FEventChain> ActiveChains;
    
    UPROPERTY()
    FGameTime CurrentGameTime;
    
    void CheckTimeBasedEvents();
    void CheckConditionBasedEvents();
    void ProcessEventEffects(const FHistoricalEvent& Event);
};
```

### 3.2 核心历史事件

#### 袁世凯称帝事件

```cpp
// 事件数据定义
USTRUCT()
struct FHistoricalEvent_YuanShikaiEmperor : public FHistoricalEvent
{
    GENERATED_BODY()

    FHistoricalEvent_YuanShikaiEmperor()
    {
        EventID = TEXT("EVENT_YUAN_SHIKAI_EMPEROR");
        EventName = NSLOCTEXT("Events", "YuanEmperor", "袁世凯称帝");
        
        // 触发时间: 1915年12月
        TriggerTime.GameYear = 1915;
        TriggerTime.GameMonth = 12;
        
        // 触发条件
        FEventTriggerCondition Condition;
        Condition.ConditionType = ETriggerConditionType::FactionExists;
        Condition.FactionRequired = EFactionType::BeiyangGovernment;
        TriggerConditions.Add(Condition);
        
        // 事件效果
        FEventEffect Effect1;
        Effect1.EffectType = EEventEffectType::FactionLegitimacy;
        Effect1.TargetFaction = EFactionType::BeiyangGovernment;
        Effect1.FloatValue = 30.0f; // 正统度+30
        EventEffects.Add(Effect1);
        
        FEventEffect Effect2;
        Effect2.EffectType = EEventEffectType::DiplomaticRelation;
        Effect2.TargetFaction = EFactionType::BeiyangGovernment;
        Effect2.RelationModifier = -20.0f; // 外交关系-20
        EventEffects.Add(Effect2);
        
        FEventEffect Effect3;
        Effect3.EffectType = EEventEffectType::SpawnEvent;
        Effect3.EventToSpawn = TEXT("EVENT_NATIONAL_PROTECTION_WAR");
        Effect3.DelayDays = 30; // 30天后触发护国战争
        EventEffects.Add(Effect3);
        
        // 玩家选择
        EventChoices.Reset();
        
        FEventChoice Choice1;
        Choice1.ChoiceID = TEXT("SUPPORT_EMPEROR");
        Choice1.ChoiceText = NSLOCTEXT("Events", "SupportEmperor", "支持袁世凯称帝");
        Choice1.Consequences.Add({EFactionType::BeiyangGovernment, 20.0f, -10.0f});
        EventChoices.Add(Choice1);
        
        FEventChoice Choice2;
        Choice2.ChoiceID = TEXT("OPPOSE_EMPEROR");
        Choice2.ChoiceText = NSLOCTEXT("Events", "OpposeEmperor", "公开反对称帝");
        Choice2.Consequences.Add({EFactionType::BeiyangGovernment, -30.0f, 0.0f});
        Choice2.bTriggersWar = true;
        EventChoices.Add(Choice2);
        
        FEventChoice Choice3;
        Choice3.ChoiceID = TEXT("REMAIN_NEUTRAL");
        Choice3.ChoiceText = NSLOCTEXT("Events", "RemainNeutral", "保持中立观望");
        Choice3.Consequences.Add({EFactionType::BeiyangGovernment, -5.0f, -5.0f});
        EventChoices.Add(Choice3);
    }
};
```

#### 北伐战争事件链

```cpp
UCLASS()
class MINGGORTS_API UMingNorthernExpeditionChain : public UMingEventChain
{
    GENERATED_BODY()

public:
    virtual void InitializeChain() override;
    
    // 北伐阶段推进
    UFUNCTION(BlueprintCallable, Category = "Event Chain|NorthernExpedition")
    void AdvanceExpeditionPhase();
    
    UFUNCTION(BlueprintPure, Category = "Event Chain|NorthernExpedition")
    EExpeditionPhase GetCurrentPhase() const;

private:
    UPROPERTY()
    EExpeditionPhase CurrentPhase = EExpeditionPhase::Preparation;
    
    UPROPERTY()
    TArray<FExpeditionTarget> ExpeditionTargets;
    
    // 北伐阶段事件序列
    void SetupPhaseEvents();
    
    UFUNCTION()
    void OnPhase1Complete();
    
    UFUNCTION()
    void OnPhase2Complete();
    
    UFUNCTION()
    void OnPhase3Complete();
};

void UMingNorthernExpeditionChain::InitializeChain()
{
    Super::InitializeChain();
    
    ChainID = TEXT("CHAIN_NORTHERN_EXPEDITION");
    ChainName = NSLOCTEXT("EventChains", "NorthernExpedition", "北伐战争");
    
    // 设置北伐目标
    ExpeditionTargets = {
        {EFactionType::ZhiliClique, TEXT("消灭直系军阀")},
        {EFactionType::AnhuiClique, TEXT("消灭皖系军阀")},
        {EFactionType::FengtianClique, TEXT("驱逐奉系势力")},
        {TEXT("统一长江流域"), TEXT("控制长江中下游")},
        {TEXT("攻克北京"), TEXT("占领首都")}
    };
    
    // 注册阶段事件
    SetupPhaseEvents();
}

void UMingNorthernExpeditionChain::SetupPhaseEvents()
{
    // 阶段1: 誓师出征
    FChainEvent Phase1Event;
    Phase1Event.EventID = TEXT("EXPEDITION_PHASE1");
    Phase1Event.TriggerCondition = [this]() -> bool {
        return CurrentPhase == EExpeditionPhase::Preparation;
    };
    Phase1Event.OnComplete = FChainEventDelegate::CreateUObject(this, &UMingNorthernExpeditionChain::OnPhase1Complete);
    
    // 阶段2: 湖南战役
    FChainEvent Phase2Event;
    Phase2Event.EventID = TEXT("EXPEDITION_PHASE2");
    Phase2Event.TriggerCondition = [this]() -> bool {
        return CurrentPhase == EExpeditionPhase::HunanCampaign;
    };
    Phase2Event.OnComplete = FChainEventDelegate::CreateUObject(this, &UMingNorthernExpeditionChain::OnPhase2Complete);
    
    // 阶段3-5... (略)
    
    ChainEvents.Add(Phase1Event);
    ChainEvents.Add(Phase2Event);
}
```

### 3.3 事件效果处理器

```cpp
UCLASS()
class MINGGORTS_API UMingEventEffectProcessor : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UMingMechanicManager* InMechanicManager);
    
    UFUNCTION()
    void ProcessEffect(const FEventEffect& Effect);
    
    // 各类效果处理
    void ProcessFactionLegitimacyEffect(const FEventEffect& Effect);
    void ProcessDiplomaticRelationEffect(const FEventEffect& Effect);
    void ProcessResourceEffect(const FEventEffect& Effect);
    void ProcessUnitEffect(const FEventEffect& Effect);
    void ProcessTerritoryEffect(const FEventEffect& Effect);
    void ProcessTechnologyEffect(const FEventEffect& Effect);

private:
    UPROPERTY()
    TObjectPtr<UMingMechanicManager> MechanicManager;
    
    UPROPERTY()
    TObjectPtr<UMingFactionManager> FactionManager;
    
    UPROPERTY()
    TObjectPtr<UMingResourceManager> ResourceManager;
};

void UMingEventEffectProcessor::ProcessEffect(const FEventEffect& Effect)
{
    switch (Effect.EffectType)
    {
        case EEventEffectType::FactionLegitimacy:
            ProcessFactionLegitimacyEffect(Effect);
            break;
            
        case EEventEffectType::DiplomaticRelation:
            ProcessDiplomaticRelationEffect(Effect);
            break;
            
        case EEventEffectType::ResourceChange:
            ProcessResourceEffect(Effect);
            break;
            
        case EEventEffectType::SpawnUnit:
            ProcessUnitEffect(Effect);
            break;
            
        case EEventEffectType::TerritoryChange:
            ProcessTerritoryEffect(Effect);
            break;
            
        case EEventEffectType::TechnologyUnlock:
            ProcessTechnologyEffect(Effect);
            break;
            
        default:
            UE_LOG(LogMingEvents, Warning, TEXT("Unknown event effect type: %d"), 
                (int32)Effect.EffectType);
    }
}

void UMingEventEffectProcessor::ProcessFactionLegitimacyEffect(const FEventEffect& Effect)
{
    if (!MechanicManager)
        return;
        
    UMingFactionMechanic* FactionMechanic = 
        MechanicManager->GetFactionMechanic(Effect.TargetFaction);
        
    if (UMingBeiyangLegitimacyMechanic* LegitimacyMechanic = 
        Cast<UMingBeiyangLegitimacyMechanic>(FactionMechanic))
    {
        LegitimacyMechanic->AddLegitimacy(Effect.FloatValue);
        
        UE_LOG(LogMingEvents, Log, TEXT("Applied legitimacy effect to %s: %f"),
            *UEnum::GetValueAsString(Effect.TargetFaction),
            Effect.FloatValue);
    }
}
```

---

## 4. 配置系统集成

### 4.1 数据驱动配置

```cpp
// 机制配置数据资产
UCLASS()
class MINGGORTS_API UMingMechanicConfigData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Faction Mechanics")
    TMap<EFactionType, FFactionMechanicConfig> FactionMechanicConfigs;
    
    UPROPERTY(EditDefaultsOnly, Category = "Global Mechanics")
    TArray<FGlobalMechanicConfig> GlobalMechanics;
    
    UPROPERTY(EditDefaultsOnly, Category = "Mechanic Modifiers")
    TArray<FMechanicModifierConfig> MechanicModifiers;
};

USTRUCT()
struct FFactionMechanicConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly)
    EFactionType FactionType;
    
    UPROPERTY(EditDefaultsOnly)
    TArray<FMechanicDefinition> Mechanics;
    
    UPROPERTY(EditDefaultsOnly)
    FMechanicBalanceConfig BalanceConfig;
};
```

### 4.2 CSV数据表

```csv
// Mechanics_Config.csv
MechanicID,Faction,MechanicType,BaseValue,MaxValue,RegenRate,UnlockCondition
BY_WARLORD,BeiyangGovernment,Compromise,100,100,2,Start
BY_GERMAN_ADVISOR,BeiyangGovernment,Advisor,2,2,0,Technology_Unlocked
BY_LEGITIMACY,BeiyangGovernment,Legitimacy,50,100,1,Start
GMD_WHAMPOA,NationalistGovernment,Training,3,3,0.5,Building_Constructed
GMD_NEW_LIFE,NationalistGovernment,Morale,50,100,0,Event_Triggered
GMD_EQUIPMENT,NationalistGovernment,Equipment,0,100,0,Technology_Imported
CCP_GUERRILLA,ChineseCommunistParty,Stealth,100,100,5,Start
CCP_LAND_REFORM,ChineseCommunistParty,Reform,0,100,2,Territory_Controlled
CCP_UNITED_FRONT,ChineseCommunistParty,Alliance,0,2,0,Event_Available
```

---

## 5. 调试与测试

### 5.1 机制调试工具

```cpp
UCLASS()
class MINGGORTS_API UMingMechanicDebugger : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(Exec)
    void DebugMechanic(const FString& MechanicName);
    
    UFUNCTION(Exec)
    void SetMechanicValue(const FString& MechanicName, float Value);
    
    UFUNCTION(Exec)
    void TriggerTestEvent(const FString& EventID);
    
    UFUNCTION(Exec)
    void ListActiveMechanics();
    
    UFUNCTION(Exec)
    void ExportMechanicReport();
};
```

### 5.2 自动化测试

```cpp
UCLASS()
class MINGGORTS_API UMingMechanicTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FTestResult RunMechanicActivationTest();
    
    UFUNCTION(BlueprintCallable)
    FTestResult RunMechanicEffectTest();
    
    UFUNCTION(BlueprintCallable)
    FTestResult RunEventChainTest();
    
    UFUNCTION(BlueprintCallable)
    FTestResult RunBalanceValidationTest();
};
```

---

## 6. 附录

### 6.1 参考文档

- 势力详细设计: `docs/12_FACTIONS_DETAILED_DESIGN.md`
- 机制实现设计: `docs/MECHANIC_IMPLEMENTATION.md`
- 历史事件设计: `docs/HISTORICAL_EVENTS_DESIGN.md`

### 6.2 代码规范

- 所有机制类继承自 `UMingFactionMechanic`
- 使用 `UFUNCTION(BlueprintCallable)` 暴露给蓝图
- 配置数据使用 `UDataAsset` 便于编辑器配置
- 事件系统使用委托模式实现松耦合

---

*文档结束*
