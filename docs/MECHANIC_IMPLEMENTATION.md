# MingGoRTS 特色机制程序实现文档

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Dev Team
- **状态**: 进行中

---

## 1. 机制系统架构总览

### 1.1 系统架构图

```
┌─────────────────────────────────────────────────────────┐
│                    MingRTSGameMode                       │
│                    (游戏主控制器)                         │
└──────────────────┬──────────────────────────────────────┘
                   │
    ┌──────────────┼──────────────┬──────────────┐
    │              │              │              │
┌───▼───┐    ┌────▼────┐   ┌────▼────┐   ┌────▼────┐
│Faction│    │ Mechanic │   │  Event  │   │  Unit   │
│Manager│    │ Manager  │   │ Manager │   │ Manager │
└───┬───┘    └────┬────┘   └────┬────┘   └────┬────┘
    │             │              │              │
    │    ┌────────▼────────┐    │              │
    │    │  36个特色机制    │    │              │
    │    │  - 北洋: 3个     │    │              │
    │    │  - 国民: 3个     │    │              │
    │    │  - 中共: 3个     │    │              │
    │    │  - ...          │    │              │
    │    └─────────────────┘    │              │
    │                           │              │
```

### 1.2 核心类设计

#### FMingFactionMechanic (基础机制结构)

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FMingFactionMechanic
{
    GENERATED_BODY()

    // 基础信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MechanicID;              // 机制唯一标识

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MechanicName;            // 机制名称

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MechanicDescription;     // 机制描述

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType OwningFaction;      // 所属势力

    // 机制状态
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;                  // 是否激活

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnlocked;                // 是否解锁

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnlockYear;                // 解锁年份

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;              // 当前等级

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;                  // 最大等级

    // 机制数值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> NumericValues;  // 数值参数

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> StringValues; // 字符串参数

    // 事件绑定
    UPROPERTY()
    TArray<FMechanicEffect> ActiveEffects;  // 当前生效的效果

    // 构造函数
    FMingFactionMechanic()
        : bIsActive(false)
        , bIsUnlocked(false)
        , UnlockYear(1912)
        , CurrentLevel(1)
        , MaxLevel(3)
    {}
};
```

#### UMingMechanicManager (机制管理器)

```cpp
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingMechanicManager : public UActorComponent
{
    GENERATED_BODY()

public:
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    void InitializeMechanicManager();

    // 机制注册
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    void RegisterMechanic(const FMingFactionMechanic& Mechanic);

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    void RegisterAllFactionMechanics(EFactionType Faction);

    // 机制查询
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    bool HasMechanic(const FString& MechanicID) const;

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    FMingFactionMechanic GetMechanic(const FString& MechanicID) const;

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    TArray<FMingFactionMechanic> GetFactionMechanics(EFactionType Faction) const;

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    TArray<FMingFactionMechanic> GetActiveMechanics(EFactionType Faction) const;

    // 机制激活/停用
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    bool ActivateMechanic(const FString& MechanicID);

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    bool DeactivateMechanic(const FString& MechanicID);

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    bool UnlockMechanic(const FString& MechanicID, int32 CurrentYear);

    // 机制升级
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    bool UpgradeMechanic(const FString& MechanicID);

    // 数值获取
    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    float GetMechanicNumericValue(const FString& MechanicID, const FString& Key) const;

    UFUNCTION(BlueprintCallable, Category = "Mechanic Manager")
    FString GetMechanicStringValue(const FString& MechanicID, const FString& Key) const;

    // 事件处理
    UFUNCTION()
    void OnGameYearChanged(int32 NewYear);

    UFUNCTION()
    void OnFactionTurnStart(EFactionType Faction);

    UFUNCTION()
    void OnFactionTurnEnd(EFactionType Faction);

    UFUNCTION()
    void OnBattleStarted(EBattleType BattleType, EFactionType Attacker, EFactionType Defender);

    UFUNCTION()
    void OnBattleEnded(EBattleResult Result);

    // 委托事件
    UPROPERTY(BlueprintAssignable, Category = "Mechanic Events")
    FOnMechanicActivated OnMechanicActivated;

    UPROPERTY(BlueprintAssignable, Category = "Mechanic Events")
    FOnMechanicDeactivated OnMechanicDeactivated;

    UPROPERTY(BlueprintAssignable, Category = "Mechanic Events")
    FOnMechanicUnlocked OnMechanicUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Mechanic Events")
    FOnMechanicUpgraded OnMechanicUpgraded;

protected:
    // 势力机制映射
    UPROPERTY()
    TMap<FString, FMingFactionMechanic> AllMechanics;

    UPROPERTY()
    TMap<EFactionType, TArray<FString>> FactionMechanicMap;

    UPROPERTY()
    TArray<FString> ActiveMechanics;

    // 机制效果应用
    void ApplyMechanicEffects(const FString& MechanicID);
    void RemoveMechanicEffects(const FString& MechanicID);

    // 内部辅助函数
    void InitializeBeiyangMechanics();
    void InitializeNationalistMechanics();
    void InitializeCommunistMechanics();
    void InitializeFengtianMechanics();
    void InitializeZhiliMechanics();
    void InitializeWanmeiMechanics();
    void InitializeShanxiMechanics();
    void InitializeGuangxiMechanics();
    void InitializeYunnanMechanics();
    void InitializeSichuanMechanics();
    void InitializeMajiaMechanics();
    void InitializeXinjiangMechanics();

    // 机制检查
    void CheckMechanicUnlockConditions(int32 CurrentYear);
    void UpdateMechanicEffects();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    UMingMechanicManager();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction) override;
};
```

---

## 2. 北洋政府机制实现

### 2.1 中央正统 (Central Legitimacy)

#### 机制设计

**机制ID**: `BEIYANG_CENTRAL_LEGITIMACY`

**机制效果**:
- 税收收入 +20%
- 外交好感度 +15%
- 每回合威望 +5
- 统治不稳定时触发叛乱

#### 代码实现

```cpp
void UMingMechanicManager::InitializeBeiyangMechanics()
{
    // 1. 中央正统机制
    FMingFactionMechanic CentralLegitimacy;
    CentralLegitimacy.MechanicID = TEXT("BEIYANG_CENTRAL_LEGITIMACY");
    CentralLegitimacy.MechanicName = TEXT("中央正统");
    CentralLegitimacy.MechanicDescription = TEXT("作为名义上的中央政府，获得独特的政治优势");
    CentralLegitimacy.OwningFaction = EFactionType::Beiyang;
    CentralLegitimacy.bIsActive = true;  // 游戏开始时激活
    CentralLegitimacy.bIsUnlocked = true;
    CentralLegitimacy.UnlockYear = 1912;
    CentralLegitimacy.CurrentLevel = 1;
    CentralLegitimacy.MaxLevel = 3;

    // 设置数值参数
    CentralLegitimacy.NumericValues.Add(TEXT("TaxBonus"), 0.20f);      // +20%税收
    CentralLegitimacy.NumericValues.Add(TEXT("DiplomacyBonus"), 15.0f); // +15外交好感
    CentralLegitimacy.NumericValues.Add(TEXT("PrestigePerTurn"), 5.0f);  // +5威望/回合
    CentralLegitimacy.NumericValues.Add(TEXT("RebellionThreshold"), 30.0f); // 叛乱阈值

    RegisterMechanic(CentralLegitimacy);

    // 绑定事件
    BindCentralLegitimacyEvents();
}

void UMingMechanicManager::BindCentralLegitimacyEvents()
{
    // 税收加成效果
    FMechanicEffect TaxEffect;
    TaxEffect.EffectType = EMechanicEffectType::Economic;
    TaxEffect.TargetValue = TEXT("TaxIncome");
    TaxEffect.ModifierType = EModifierType::Percentage;
    TaxEffect.Value = 0.20f;
    TaxEffect.bIsPermanent = true;

    ApplyEffectToMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"), TaxEffect);

    // 外交加成效果
    FMechanicEffect DiplomacyEffect;
    DiplomacyEffect.EffectType = EMechanicEffectType::Political;
    DiplomacyEffect.TargetValue = TEXT("DiplomacyBonus");
    DiplomacyEffect.ModifierType = EModifierType::Flat;
    DiplomacyEffect.Value = 15.0f;
    DiplomacyEffect.bIsPermanent = true;

    ApplyEffectToMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"), DiplomacyEffect);
}

// 中央正统机制特殊处理：统治不稳定检查
void UMingMechanicManager::CheckCentralLegitimacyStability()
{
    if (!IsMechanicActive(TEXT("BEIYANG_CENTRAL_LEGITIMACY")))
        return;

    // 获取当前稳定性
    float CurrentStability = GetFactionStability(EFactionType::Beiyang);
    float RebellionThreshold = GetMechanicNumericValue(TEXT("BEIYANG_CENTRAL_LEGITIMACY"), TEXT("RebellionThreshold"));

    if (CurrentStability < RebellionThreshold)
    {
        // 触发叛乱事件
        FGameEvent RebellionEvent;
        RebellionEvent.EventID = TEXT("BEIYANG_REBELLION");
        RebellionEvent.EventType = EEventType::Rebellion;
        RebellionEvent.TargetFaction = EFactionType::Beiyang;
        RebellionEvent.Probability = (RebellionThreshold - CurrentStability) / RebellionThreshold;

        TriggerEvent(RebellionEvent);
    }
}
```

#### Blueprint接口

```cpp
UFUNCTION(BlueprintCallable, Category = "北洋机制")
float GetCentralLegitimacyTaxBonus() const;

UFUNCTION(BlueprintCallable, Category = "北洋机制")
float GetCentralLegitimacyDiplomacyBonus() const;

UFUNCTION(BlueprintCallable, Category = "北洋机制")
bool IsCentralLegitimacyCausingRebellion() const;
```

### 2.2 府院之争 (Warlord Politics)

#### 机制设计

**机制ID**: `BEIYANG_FACTIONAL_STRIFE`

**机制效果**:
- 需要平衡文官派和武将派
- 派系支持度高时政策执行速度 +25%
- 派系失衡可能导致政变

#### 代码实现

```cpp
void UMingMechanicManager::InitializeBeiyangFactionalStrife()
{
    FMingFactionMechanic FactionalStrife;
    FactionalStrife.MechanicID = TEXT("BEIYANG_FACTIONAL_STRIFE");
    FactionalStrife.MechanicName = TEXT("府院之争");
    FactionalStrife.MechanicDescription = TEXT("内部派系斗争影响政府效率");
    FactionalStrife.OwningFaction = EFactionType::Beiyang;
    FactionalStrife.bIsActive = true;
    FactionalStrife.bIsUnlocked = true;
    FactionalStrife.UnlockYear = 1912;
    FactionalStrife.CurrentLevel = 1;
    FactionalStrife.MaxLevel = 1;

    // 派系平衡度 (0-100, 50为平衡)
    FactionalStrife.NumericValues.Add(TEXT("CivilFactionPower"), 50.0f);  // 文官派势力
    FactionalStrife.NumericValues.Add(TEXT("MilitaryFactionPower"), 50.0f); // 武将派势力
    FactionalStrife.NumericValues.Add(TEXT("BalanceThreshold"), 20.0f);    // 失衡阈值
    FactionalStrife.NumericValues.Add(TEXT("PolicySpeedBonus"), 0.25f);    // 政策速度加成
    FactionalStrife.NumericValues.Add(TEXT("CoupRisk"), 0.0f);           // 政变风险

    RegisterMechanic(FactionalStrife);
}

// 更新派系平衡
void UMingMechanicManager::UpdateFactionalBalance(float CivilChange, float MilitaryChange)
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(TEXT("BEIYANG_FACTIONAL_STRIFE"));
    if (!Mechanic) return;

    float& CivilPower = Mechanic->NumericValues[TEXT("CivilFactionPower")];
    float& MilitaryPower = Mechanic->NumericValues[TEXT("MilitaryFactionPower")];
    float& CoupRisk = Mechanic->NumericValues[TEXT("CoupRisk")];

    // 更新派系势力
    CivilPower = FMath::Clamp(CivilPower + CivilChange, 0.0f, 100.0f);
    MilitaryPower = FMath::Clamp(MilitaryPower + MilitaryChange, 0.0f, 100.0f);

    // 计算失衡程度
    float BalanceDiff = FMath::Abs(CivilPower - MilitaryPower);
    float Threshold = Mechanic->NumericValues[TEXT("BalanceThreshold")];

    if (BalanceDiff > Threshold)
    {
        // 派系失衡
        CoupRisk = (BalanceDiff - Threshold) / (100.0f - Threshold);

        // 检查政变
        if (CoupRisk > 0.7f && FMath::RandRange(0.0f, 1.0f) < CoupRisk * 0.1f)
        {
            TriggerCoupEvent(CivilPower > MilitaryPower ? TEXT("Civil") : TEXT("Military"));
        }
    }
    else
    {
        CoupRisk = 0.0f;
    }
}

// 获取政策执行速度加成
float UMingMechanicManager::GetPolicyExecutionSpeedBonus() const
{
    FMingFactionMechanic Mechanic = GetMechanic(TEXT("BEIYANG_FACTIONAL_STRIFE"));

    float CivilPower = Mechanic.NumericValues[TEXT("CivilFactionPower")];
    float MilitaryPower = Mechanic.NumericValues[TEXT("MilitaryFactionPower")];
    float BalanceDiff = FMath::Abs(CivilPower - MilitaryPower);
    float Threshold = Mechanic.NumericValues[TEXT("BalanceThreshold")];

    if (BalanceDiff <= Threshold)
    {
        // 派系平衡，获得加成
        return Mechanic.NumericValues[TEXT("PolicySpeedBonus")];
    }

    return 0.0f;
}
```

### 2.3 借款依赖 (Foreign Debt)

#### 机制设计

**机制ID**: `BEIYANG_FOREIGN_DEBT`

**机制效果**:
- 可向日本借款 (西原借款)
- 可向四国银行团借款 (善后借款)
- 债务过高时外国势力干预内政

#### 代码实现

```cpp
void UMingMechanicManager::InitializeBeiyangForeignDebt()
{
    FMingFactionMechanic ForeignDebt;
    ForeignDebt.MechanicID = TEXT("BEIYANG_FOREIGN_DEBT");
    ForeignDebt.MechanicName = TEXT("借款依赖");
    ForeignDebt.MechanicDescription = TEXT("依赖外国借款维持财政");
    ForeignDebt.OwningFaction = EFactionType::Beiyang;
    ForeignDebt.bIsActive = true;
    ForeignDebt.bIsUnlocked = true;
    ForeignDebt.UnlockYear = 1912;
    ForeignDebt.CurrentLevel = 1;
    ForeignDebt.MaxLevel = 1;

    // 债务数值
    ForeignDebt.NumericValues.Add(TEXT("TotalDebt"), 0.0f);           // 总债务
    ForeignDebt.NumericValues.Add(TEXT("DebtToJapan"), 0.0f);         // 对日债务
    ForeignDebt.NumericValues.Add(TEXT("DebtToBankingConsortium"), 0.0f); // 对四国银行团债务
    ForeignDebt.NumericValues.Add(TEXT("DebtServicingCost"), 0.0f);   // 偿债成本/回合
    ForeignDebt.NumericValues.Add(TEXT("InterventionThreshold"), 1000000.0f); // 干预阈值
    ForeignDebt.NumericValues.Add(TEXT("NishiharaLoanAmount"), 14500000.0f); // 西原借款额度
    ForeignDebt.NumericValues.Add(TEXT("ReorganizationLoanAmount"), 25000000.0f); // 善后借款额度

    RegisterMechanic(ForeignDebt);
}

// 申请西原借款
bool UMingMechanicManager::ApplyNishiharaLoan(float Amount)
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(TEXT("BEIYANG_FOREIGN_DEBT"));
    if (!Mechanic) return false;

    float MaxLoan = Mechanic->NumericValues[TEXT("NishiharaLoanAmount")];
    float CurrentDebtToJapan = Mechanic->NumericValues[TEXT("DebtToJapan")];

    if (Amount > MaxLoan - CurrentDebtToJapan)
    {
        UE_LOG(LogMingMechanics, Warning, TEXT("西原借款申请超额"));
        return false;
    }

    // 增加对日债务
    CurrentDebtToJapan += Amount;
    Mechanic->NumericValues[TEXT("DebtToJapan")] = CurrentDebtToJapan;

    // 获得资金
    AddFactionMoney(EFactionType::Beiyang, Amount);

    // 增加日本影响力
    IncreaseForeignInfluence(EFactionType::Japan, Amount / MaxLoan * 10.0f);

    // 检查债务危机
    CheckDebtCrisis();

    return true;
}

// 检查债务危机
void UMingMechanicManager::CheckDebtCrisis()
{
    FMingFactionMechanic Mechanic = GetMechanic(TEXT("BEIYANG_FOREIGN_DEBT"));

    float TotalDebt = Mechanic.NumericValues[TEXT("TotalDebt")];
    float Threshold = Mechanic.NumericValues[TEXT("InterventionThreshold")];

    if (TotalDebt > Threshold)
    {
        // 触发债务危机事件
        FGameEvent DebtCrisisEvent;
        DebtCrisisEvent.EventID = TEXT("BEIYANG_DEBT_CRISIS");
        DebtCrisisEvent.EventType = EEventType::Crisis;
        DebtCrisisEvent.TargetFaction = EFactionType::Beiyang;
        DebtCrisisEvent.Severity = (TotalDebt - Threshold) / Threshold;

        TriggerEvent(DebtCrisisEvent);

        // 外国干预内政
        if (Mechanic.NumericValues[TEXT("DebtToJapan")] > Threshold * 0.5f)
        {
            ApplyForeignIntervention(EFactionType::Japan);
        }
    }
}
```

---

## 3. 国民政府机制实现

### 3.1 北伐战争 (Northern Expedition)

```cpp
void UMingMechanicManager::InitializeNationalistNorthernExpedition()
{
    FMingFactionMechanic NorthernExpedition;
    NorthernExpedition.MechanicID = TEXT("NATIONALIST_NORTHERN_EXPEDITION");
    NorthernExpedition.MechanicName = TEXT("北伐战争");
    NorthernExpedition.MechanicDescription = TEXT("统一全国的军事行动");
    NorthernExpedition.OwningFaction = EFactionType::Nationalist;
    NorthernExpedition.bIsActive = false;  // 需要触发事件激活
    NorthernExpedition.bIsUnlocked = false;
    NorthernExpedition.UnlockYear = 1926;  // 1926年北伐开始
    NorthernExpedition.CurrentLevel = 1;
    NorthernExpedition.MaxLevel = 3;

    // 北伐数值
    NorthernExpedition.NumericValues.Add(TEXT("ExpeditionProgress"), 0.0f);    // 北伐进度 0-100
    NorthernExpedition.NumericValues.Add(TEXT("CombatBonus"), 0.15f);          // 北伐军战斗力 +15%
    NorthernExpedition.NumericValues.Add(TEXT("MobilizationSpeed"), 0.25f);    // 动员速度 +25%
    NorthernExpedition.NumericValues.Add(TEXT("PublicSupport"), 70.0f);         // 民众支持度
    NorthernExpedition.NumericValues.Add(TEXT("ForeignSupport"), 30.0f);        // 外国支持度

    RegisterMechanic(NorthernExpedition);
}

// 启动北伐
bool UMingMechanicManager::StartNorthernExpedition()
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(TEXT("NATIONALIST_NORTHERN_EXPEDITION"));
    if (!Mechanic) return false;

    // 检查条件
    int32 CurrentYear = GetGameYear();
    if (CurrentYear < 1926)
    {
        UE_LOG(LogMingMechanics, Warning, TEXT("北伐尚未到启动时间"));
        return false;
    }

    // 激活北伐机制
    Mechanic->bIsActive = true;
    Mechanic->bIsUnlocked = true;

    // 应用北伐效果
    ApplyNorthernExpeditionEffects();

    // 触发北伐开始事件
    FGameEvent ExpeditionStartEvent;
    ExpeditionStartEvent.EventID = TEXT("NORTHERN_EXPEDITION_STARTED");
    ExpeditionStartEvent.EventType = EEventType::MajorEvent;
    ExpeditionStartEvent.TargetFaction = EFactionType::Nationalist;

    TriggerEvent(ExpeditionStartEvent);

    return true;
}

// 更新北伐进度
void UMingMechanicManager::UpdateNorthernExpeditionProgress(float CapturedTerritory)
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(TEXT("NATIONALIST_NORTHERN_EXPEDITION"));
    if (!Mechanic || !Mechanic->bIsActive) return;

    float& Progress = Mechanic->NumericValues[TEXT("ExpeditionProgress")];
    Progress = FMath::Clamp(Progress + CapturedTerritory, 0.0f, 100.0f);

    // 北伐完成
    if (Progress >= 100.0f)
    {
        CompleteNorthernExpedition();
    }
}
```

### 3.2 黄埔系 (Whampoa Clique)

```cpp
void UMingMechanicManager::InitializeNationalistWhampoaClique()
{
    FMingFactionMechanic WhampoaClique;
    WhampoaClique.MechanicID = TEXT("NATIONALIST_WHAMPOA_CLIQUE");
    WhampoaClique.MechanicName = TEXT("黄埔系");
    WhampoaClique.MechanicDescription = TEXT("黄埔军校毕业生组成的军官团");
    WhampoaClique.OwningFaction = EFactionType::Nationalist;
    WhampoaClique.bIsActive = true;
    WhampoaClique.bIsUnlocked = true;
    WhampoaClique.UnlockYear = 1924;  // 1924年黄埔军校成立
    WhampoaClique.CurrentLevel = 1;
    WhampoaClique.MaxLevel = 3;

    // 黄埔系数值
    WhampoaClique.NumericValues.Add(TEXT("CadetCombatBonus"), 0.20f);      // 黄埔学生军战斗力 +20%
    WhampoaClique.NumericValues.Add(TEXT("OfficerLoyalty"), 0.85f);         // 军官忠诚度 85%
    WhampoaClique.NumericValues.Add(TEXT("TrainingEfficiency"), 0.30f);     // 训练效率 +30%
    WhampoaClique.NumericValues.Add(TEXT("CadetRecruitmentRate"), 100.0f);   // 每回合招募数

    RegisterMechanic(WhampoaClique);
}
```

### 3.3 民族工业 (National Industry)

```cpp
void UMingMechanicManager::InitializeNationalistIndustry()
{
    FMingFactionMechanic NationalIndustry;
    NationalIndustry.MechanicID = TEXT("NATIONALIST_NATIONAL_INDUSTRY");
    NationalIndustry.MechanicName = TEXT("民族工业");
    NationalIndustry.MechanicDescription = TEXT("发展民族资本主义工业");
    NationalIndustry.OwningFaction = EFactionType::Nationalist;
    NationalIndustry.bIsActive = true;
    NationalIndustry.bIsUnlocked = true;
    NationalIndustry.UnlockYear = 1927;
    NationalIndustry.CurrentLevel = 1;
    NationalIndustry.MaxLevel = 3;

    // 工业数值
    NationalIndustry.NumericValues.Add(TEXT("IndustrialGrowthRate"), 0.15f);    // 工业增长率 +15%
    NationalIndustry.NumericValues.Add(TEXT("FactoryEfficiency"), 0.10f);         // 工厂效率 +10%
    NationalIndustry.NumericValues.Add(TEXT("ResourceProduction"), 0.12f);        // 资源产量 +12%
    NationalIndustry.NumericValues.Add(TEXT("TechnologyProgress"), 0.08f);        // 科技进度 +8%

    RegisterMechanic(NationalIndustry);
}
```

---

## 4. 中国共产党机制实现

### 4.1 人民战争 (People's War)

```cpp
void UMingMechanicManager::InitializeCommunistPeoplesWar()
{
    FMingFactionMechanic PeoplesWar;
    PeoplesWar.MechanicID = TEXT("COMMUNIST_PEOPLES_WAR");
    PeoplesWar.MechanicName = TEXT("人民战争");
    PeoplesWar.MechanicDescription = TEXT("依靠人民群众的游击战战略");
    PeoplesWar.OwningFaction = EFactionType::Communist;
    PeoplesWar.bIsActive = true;
    PeoplesWar.bIsUnlocked = true;
    PeoplesWar.UnlockYear = 1927;
    PeoplesWar.CurrentLevel = 1;
    PeoplesWar.MaxLevel = 3;

    // 人民战争数值
    PeoplesWar.NumericValues.Add(TEXT("GuerrillaCombatBonus"), 0.40f);       // 游击战力 +40%
    PeoplesWar.NumericValues.Add(TEXT("MilitiaCombatBonus"), 0.30f);         // 民兵战力 +30%
    PeoplesWar.NumericValues.Add(TEXT("MobilizationSpeed"), 0.50f);           // 动员速度 +50%
    PeoplesWar.NumericValues.Add(TEXT("SupplyFromPeople"), 0.25f);            // 群众补给 +25%
    PeoplesWar.NumericValues.Add(TEXT("IntelNetwork"), 0.35f);                // 情报网络 +35%

    RegisterMechanic(PeoplesWar);
}
```

### 4.2 游击战术 (Guerrilla Tactics)

```cpp
void UMingMechanicManager::InitializeCommunistGuerrillaTactics()
{
    FMingFactionMechanic GuerrillaTactics;
    GuerrillaTactics.MechanicID = TEXT("COMMUNIST_GUERRILLA_TACTICS");
    GuerrillaTactics.MechanicName = TEXT("游击战术");
    GuerrillaTactics.MechanicDescription = TEXT("灵活机动的游击作战方式");
    GuerrillaTactics.OwningFaction = EFactionType::Communist;
    GuerrillaTactics.bIsActive = true;
    GuerrillaTactics.bIsUnlocked = true;
    GuerrillaTactics.UnlockYear = 1927;
    GuerrillaTactics.CurrentLevel = 1;
    GuerrillaTactics.MaxLevel = 3;

    // 游击战术数值
    GuerrillaTactics.NumericValues.Add(TEXT("MountainCombatBonus"), 0.35f);     // 山地战 +35%
    GuerrillaTactics.NumericValues.Add(TEXT("ForestCombatBonus"), 0.30f);        // 森林战 +30%
    GuerrillaTactics.NumericValues.Add(TEXT("AmbushSuccessRate"), 0.45f);        // 伏击成功率 +45%
    GuerrillaTactics.NumericValues.Add(TEXT("RetreatSuccessRate"), 0.60f);        // 撤退成功率 +60%
    GuerrillaTactics.NumericValues.Add(TEXT("SurpriseAttackBonus"), 0.25f);       // 突袭加成 +25%

    RegisterMechanic(GuerrillaTactics);
}
```

### 4.3 土地革命 (Land Revolution)

```cpp
void UMingMechanicManager::InitializeCommunistLandRevolution()
{
    FMingFactionMechanic LandRevolution;
    LandRevolution.MechanicID = TEXT("COMMUNIST_LAND_REVOLUTION");
    LandRevolution.MechanicName = TEXT("土地革命");
    LandRevolution.MechanicDescription = TEXT("打土豪分田地，获得农民支持");
    LandRevolution.OwningFaction = EFactionType::Communist;
    LandRevolution.bIsActive = true;
    LandRevolution.bIsUnlocked = true;
    LandRevolution.UnlockYear = 1927;
    LandRevolution.CurrentLevel = 1;
    LandRevolution.MaxLevel = 3;

    // 土地革命数值
    LandRevolution.NumericValues.Add(TEXT("PeasantSupport"), 0.80f);          // 农民支持度 80%
    LandRevolution.NumericValues.Add(TEXT("RecruitmentBonus"), 0.40f);        // 征兵加成 +40%
    LandRevolution.NumericValues.Add(TEXT("ResourceFromLand"), 0.20f);        // 土地资源 +20%
    LandRevolution.NumericValues.Add(TEXT("LoyaltyBonus"), 0.25f);             // 忠诚度加成 +25%
    LandRevolution.NumericValues.Add(TEXT("ForeignSanction"), 0.15f);         // 外国制裁 -15%外交

    RegisterMechanic(LandRevolution);
}
```

---

## 5. 其他势力机制实现 (简要)

### 5.1 奉系军阀

```cpp
void UMingMechanicManager::InitializeFengtianMechanics()
{
    // 东北工业
    FMingFactionMechanic NortheastIndustry;
    NortheastIndustry.MechanicID = TEXT("FENGTIAN_NORTHEAST_INDUSTRY");
    NortheastIndustry.MechanicName = TEXT("东北工业");
    NortheastIndustry.OwningFaction = EFactionType::Fengtian;
    NortheastIndustry.NumericValues.Add(TEXT("WeaponProductionBonus"), 0.25f);
    NortheastIndustry.NumericValues.Add(TEXT("IndustrialEfficiency"), 0.20f);
    RegisterMechanic(NortheastIndustry);

    // 骑兵优势
    FMingFactionMechanic CavalryAdvantage;
    CavalryAdvantage.MechanicID = TEXT("FENGTIAN_CAVALRY_ADVANTAGE");
    CavalryAdvantage.MechanicName = TEXT("骑兵优势");
    CavalryAdvantage.OwningFaction = EFactionType::Fengtian;
    CavalryAdvantage.NumericValues.Add(TEXT("CavalryCombatBonus"), 0.20f);
    CavalryAdvantage.NumericValues.Add(TEXT("CavalryRecruitmentSpeed"), 0.15f);
    RegisterMechanic(CavalryAdvantage);

    // 日本影响
    FMingFactionMechanic JapaneseInfluence;
    JapaneseInfluence.MechanicID = TEXT("FENGTIAN_JAPANESE_INFLUENCE");
    JapaneseInfluence.MechanicName = TEXT("日本影响");
    JapaneseInfluence.OwningFaction = EFactionType::Fengtian;
    JapaneseInfluence.NumericValues.Add(TEXT("JapaneseAid"), 0.30f);
    JapaneseInfluence.NumericValues.Add(TEXT("SovereigntyRisk"), 0.25f);
    RegisterMechanic(JapaneseInfluence);
}
```

### 5.2 直系军阀

```cpp
void UMingMechanicManager::InitializeZhiliMechanics()
{
    // 中原霸主
    FMingFactionMechanic CentralPlainsHegemon;
    CentralPlainsHegemon.MechanicID = TEXT("ZHILI_CENTRAL_PLAINS_HEGEMON");
    CentralPlainsHegemon.MechanicName = TEXT("中原霸主");
    CentralPlainsHegemon.OwningFaction = EFactionType::Zhili;
    CentralPlainsHegemon.NumericValues.Add(TEXT("PopulationBonus"), 0.30f);
    CentralPlainsHegemon.NumericValues.Add(TEXT("ResourceBonus"), 0.25f);
    RegisterMechanic(CentralPlainsHegemon);

    // 吴佩孚精锐
    FMingFactionMechanic WuPeifuElite;
    WuPeifuElite.MechanicID = TEXT("ZHILI_WU_PEIFU_ELITE");
    WuPeifuElite.MechanicName = TEXT("吴佩孚精锐");
    WuPeifuElite.OwningFaction = EFactionType::Zhili;
    WuPeifuElite.NumericValues.Add(TEXT("EliteCombatBonus"), 0.25f);
    WuPeifuElite.NumericValues.Add(TEXT("MoraleBonus"), 0.20f);
    RegisterMechanic(WuPeifuElite);

    // 兵力庞大
    FMingFactionMechanic LargeForce;
    LargeForce.MechanicID = TEXT("ZHILI_LARGE_FORCE");
    LargeForce.MechanicName = TEXT("兵力庞大");
    LargeForce.OwningFaction = EFactionType::Zhili;
    LargeForce.NumericValues.Add(TEXT("ArmySizeBonus"), 0.35f);
    LargeForce.NumericValues.Add(TEXT("MaintenanceCost"), 0.15f);
    RegisterMechanic(LargeForce);
}
```

### 5.3 其他势力机制 (快速定义)

```cpp
void UMingMechanicManager::InitializeAllRemainingMechanics()
{
    // 皖系军阀
    RegisterMechanic(CreateMechanic(TEXT("WANMEI_ANFU_CLIQUE"), EFactionType::Wanmei,
        {{TEXT("PoliticalInfluence"), 0.30f}, {TEXT("JapaneseAid"), 0.25f}}));
    RegisterMechanic(CreateMechanic(TEXT("WANMEI_NISHIBARA_LOAN"), EFactionType::Wanmei,
        {{TEXT("LoanAmount"), 14500000.0f}, {TEXT("PoliticalCost"), 0.20f}}));
    RegisterMechanic(CreateMechanic(TEXT("WANMEI_POLITICAL_OPERATIONS"), EFactionType::Wanmei,
        {{TEXT("PoliticalManipulation"), 0.35f}, {TEXT("ParliamentControl"), 0.40f}}));

    // 晋系军阀
    RegisterMechanic(CreateMechanic(TEXT("SHANXI_SHANXI_MODEL"), EFactionType::Shanxi,
        {{TEXT("StabilityBonus"), 0.30f}, {TEXT("EconomicEfficiency"), 0.15f}}));
    RegisterMechanic(CreateMechanic(TEXT("SHANXI_DEFENSE_SPECIALIST"), EFactionType::Shanxi,
        {{TEXT("DefenseCombatBonus"), 0.30f}, {TEXT("FortificationBonus"), 0.25f}}));
    RegisterMechanic(CreateMechanic(TEXT("SHANXI_RAILWAY_NETWORK"), EFactionType::Shanxi,
        {{TEXT("MovementSpeedBonus"), 0.50f}, {TEXT("TradeBonus"), 0.20f}}));

    // 桂系军阀
    RegisterMechanic(CreateMechanic(TEXT("GUANGXI_GUANGXI_MILITIA"), EFactionType::Guangxi,
        {{TEXT("MilitiaCombatBonus"), 0.25f}, {TEXT("MobilizationSpeed"), 0.40f}}));
    RegisterMechanic(CreateMechanic(TEXT("GUANGXI_MOUNTAIN_WARFARE"), EFactionType::Guangxi,
        {{TEXT("MountainCombatBonus"), 0.25f}, {TEXT("AmbushSuccessRate"), 0.40f}}));
    RegisterMechanic(CreateMechanic(TEXT("GUANGXI_TWO_GUANG_ALLIANCE"), EFactionType::Guangxi,
        {{TEXT("AllianceBonus"), 0.30f}, {TEXT("TradeBonus"), 0.30f}}));

    // 滇系军阀
    RegisterMechanic(CreateMechanic(TEXT("YUNNAN_NATIONAL_PROTECTION"), EFactionType::Yunnan,
        {{TEXT("AntiMonarchyBonus"), 0.25f}, {TEXT("PublicSupport"), 0.30f}}));
    RegisterMechanic(CreateMechanic(TEXT("YUNNAN_FRONTIER_EXPANSION"), EFactionType::Yunnan,
        {{TEXT("ExpansionCombatBonus"), 0.20f}, {TEXT("SupplyPenalty"), -0.30f}}));
    RegisterMechanic(CreateMechanic(TEXT("YUNNAN_ETHNIC_DIVERSITY"), EFactionType::Yunnan,
        {{TEXT("EthnicUnitBonus"), 0.25f}, {TEXT("GovernanceDifficulty"), 0.15f}}));

    // 川系军阀
    RegisterMechanic(CreateMechanic(TEXT("SICHUAN_DEFENSE_ZONE"), EFactionType::Sichuan,
        {{TEXT("ZoneAutonomy"), 0.20f}, {TEXT("InternalConflict"), 0.15f}}));
    RegisterMechanic(CreateMechanic(TEXT("SICHUAN_LAND_OF_ABUNDANCE"), EFactionType::Sichuan,
        {{TEXT("AgriculturalBonus"), 0.35f}, {TEXT("PopulationBonus"), 0.25f}}));
    RegisterMechanic(CreateMechanic(TEXT("SICHUAN_INTERNAL_WARFARE"), EFactionType::Sichuan,
        {{TEXT("WarfareExperience"), 0.20f}, {TEXT("ResourceConsumption"), 0.15f}}));

    // 马家军
    RegisterMechanic(CreateMechanic(TEXT("MAJIA_HUI_CAVALRY"), EFactionType::Majia,
        {{TEXT("CavalryCombatBonus"), 0.30f}, {TEXT("CavalryMaintenance"), -0.15f}}));
    RegisterMechanic(CreateMechanic(TEXT("MAJIA_RELIGIOUS_UNITY"), EFactionType::Majia,
        {{TEXT("MoraleBonus"), 0.30f}, {TEXT("InternalStability"), 0.25f}}));
    RegisterMechanic(CreateMechanic(TEXT("MAJIA_NORTHWEST_DOMINANCE"), EFactionType::Majia,
        {{TEXT("NorthwestControl"), 0.35f}, {TEXT("GuerrillaBonus"), 0.25f}}));

    // 新疆势力
    RegisterMechanic(CreateMechanic(TEXT("XINJIANG_FRONTIER_FORTRESS"), EFactionType::Xinjiang,
        {{TEXT("GeographicIsolation"), 0.40f}, {TEXT("SelfSufficiency"), 0.20f}}));
    RegisterMechanic(CreateMechanic(TEXT("XINJIANG_ETHNIC_COMPLEXITY"), EFactionType::Xinjiang,
        {{TEXT("EthnicUnits"), 0.30f}, {TEXT("GovernanceDifficulty"), 0.25f}}));
    RegisterMechanic(CreateMechanic(TEXT("XINJIANG_SOVIET_FACTOR"), EFactionType::Xinjiang,
        {{TEXT("SovietAid"), 0.35f}, {TEXT("PoliticalInterference"), 0.20f}}));
}
```

---

## 6. 机制效果系统

### 6.1 效果类型定义

```cpp
UENUM(BlueprintType)
enum class EMechanicEffectType : uint8
{
    Economic         UMETA(DisplayName = "经济效果"),
    Military         UMETA(DisplayName = "军事效果"),
    Political        UMETA(DisplayName = "政治效果"),
    Diplomatic       UMETA(DisplayName = "外交效果"),
    Cultural         UMETA(DisplayName = "文化效果"),
    Special          UMETA(DisplayName = "特殊效果")
};

UENUM(BlueprintType)
enum class EModifierType : uint8
{
    Flat             UMETA(DisplayName = "固定值"),
    Percentage       UMETA(DisplayName = "百分比"),
    Multiplier       UMETA(DisplayName = "乘数"),
    Override         UMETA(DisplayName = "覆盖")
};

USTRUCT(BlueprintType)
struct MINGGORTS_API FMechanicEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMechanicEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetValue;           // 目标数值名称

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EModifierType ModifierType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPermanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationTurns;           // 持续回合数 (-1为永久)

    FMechanicEffect()
        : EffectType(EMechanicEffectType::Special)
        , ModifierType(EModifierType::Flat)
        , Value(0.0f)
        , bIsPermanent(true)
        , DurationTurns(-1)
    {}
};
```

### 6.2 效果应用与移除

```cpp
void UMingMechanicManager::ApplyMechanicEffects(const FString& MechanicID)
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(MechanicID);
    if (!Mechanic) return;

    EFactionType Faction = Mechanic->OwningFaction;

    // 应用数值效果
    for (const auto& Kvp : Mechanic->NumericValues)
    {
        ApplyFactionNumericModifier(Faction, Kvp.Key, Kvp.Value);
    }

    // 应用结构效果
    for (const FMechanicEffect& Effect : Mechanic->ActiveEffects)
    {
        ApplyEffectToFaction(Faction, Effect);
    }

    // 触发效果激活事件
    OnMechanicEffectApplied.Broadcast(MechanicID);
}

void UMingMechanicManager::RemoveMechanicEffects(const FString& MechanicID)
{
    FMingFactionMechanic* Mechanic = GetMechanicPtr(MechanicID);
    if (!Mechanic) return;

    EFactionType Faction = Mechanic->OwningFaction;

    // 移除数值效果
    for (const auto& Kvp : Mechanic->NumericValues)
    {
        RemoveFactionNumericModifier(Faction, Kvp.Key, Kvp.Value);
    }

    // 移除结构效果
    for (const FMechanicEffect& Effect : Mechanic->ActiveEffects)
    {
        RemoveEffectFromFaction(Faction, Effect);
    }

    OnMechanicEffectRemoved.Broadcast(MechanicID);
}
```

---

## 7. 事件系统集成

### 7.1 机制触发事件

```cpp
void UMingMechanicManager::SetupMechanicEventBindings()
{
    // 年份变化事件
    UGameplayStatics::GetGameMode(this)->OnGameYearChanged.AddDynamic(this, &UMingMechanicManager::OnGameYearChanged);

    // 回合开始/结束事件
    UGameplayStatics::GetGameMode(this)->OnFactionTurnStart.AddDynamic(this, &UMingMechanicManager::OnFactionTurnStart);
    UGameplayStatics::GetGameMode(this)->OnFactionTurnEnd.AddDynamic(this, &UMingMechanicManager::OnFactionTurnEnd);

    // 战斗事件
    UGameplayStatics::GetGameMode(this)->OnBattleStarted.AddDynamic(this, &UMingMechanicManager::OnBattleStarted);
    UGameplayStatics::GetGameMode(this)->OnBattleEnded.AddDynamic(this, &UMingMechanicManager::OnBattleEnded);

    // 外交事件
    UGameplayStatics::GetGameMode(this)->OnDiplomacyChanged.AddDynamic(this, &UMingMechanicManager::OnDiplomacyChanged);

    // 经济事件
    UGameplayStatics::GetGameMode(this)->OnEconomicCrisis.AddDynamic(this, &UMingMechanicManager::OnEconomicCrisis);
}

void UMingMechanicManager::OnGameYearChanged(int32 NewYear)
{
    // 检查机制解锁
    CheckMechanicUnlockConditions(NewYear);

    // 更新需要时间衰减的效果
    UpdateTimeBasedEffects(NewYear);
}

void UMingMechanicManager::OnBattleStarted(EBattleType BattleType, EFactionType Attacker, EFactionType Defender)
{
    // 检查战斗相关机制
    for (const FString& MechanicID : ActiveMechanics)
    {
        FMingFactionMechanic* Mechanic = GetMechanicPtr(MechanicID);
        if (!Mechanic) continue;

        // 检查是否参与战斗的势力
        if (Mechanic->OwningFaction == Attacker || Mechanic->OwningFaction == Defender)
        {
            ApplyBattleEffects(MechanicID, BattleType, Mechanic->OwningFaction == Attacker);
        }
    }
}
```

### 7.2 特殊事件处理

```cpp
// 北洋府院之争政变事件
void UMingMechanicManager::HandleBeiyangCoupEvent(const FString& WinningFaction)
{
    // 停用当前中央正统机制
    DeactivateMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"));

    // 更换领导人
    if (WinningFaction == TEXT("Civil"))
    {
        // 文官派获胜
        ChangeFactionLeader(EFactionType::Beiyang, TEXT("CivilLeader"));
        ModifyFactionStability(EFactionType::Beiyang, -20.0f);
    }
    else
    {
        // 武将派获胜
        ChangeFactionLeader(EFactionType::Beiyang, TEXT("MilitaryLeader"));
        ModifyFactionStability(EFactionType::Beiyang, -30.0f);
    }

    // 重新激活机制
    ActivateMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"));

    // 触发政变事件
    FGameEvent CoupEvent;
    CoupEvent.EventID = TEXT("BEIYANG_COUP");
    CoupEvent.EventType = EEventType::Coup;
    CoupEvent.TargetFaction = EFactionType::Beiyang;
    CoupEvent.StringValues.Add(TEXT("WinningFaction"), WinningFaction);

    TriggerEvent(CoupEvent);
}
```

---

## 8. Blueprint接口

### 8.1 机制管理Blueprint接口

```cpp
UFUNCTION(BlueprintCallable, Category = "Mechanic System")
bool K2_ActivateMechanic(const FString& MechanicID);

UFUNCTION(BlueprintCallable, Category = "Mechanic System")
bool K2_DeactivateMechanic(const FString& MechanicID);

UFUNCTION(BlueprintCallable, Category = "Mechanic System")
bool K2_UpgradeMechanic(const FString& MechanicID);

UFUNCTION(BlueprintCallable, Category = "Mechanic System", meta = (ExpandEnumAsExecs = "ReturnValue"))
bool K2_HasMechanic(const FString& MechanicID) const;

UFUNCTION(BlueprintCallable, Category = "Mechanic System")
float K2_GetMechanicValue(const FString& MechanicID, const FString& Key) const;

UFUNCTION(BlueprintCallable, Category = "Mechanic System")
void K2_SetMechanicValue(const FString& MechanicID, const FString& Key, float Value);

UFUNCTION(BlueprintCallable, Category = "Mechanic System")
TArray<FMingFactionMechanic> K2_GetActiveMechanicsForFaction(EFactionType Faction);
```

### 8.2 势力特定Blueprint接口

```cpp
// 北洋政府接口
UFUNCTION(BlueprintCallable, Category = "北洋机制")
bool K2_ApplyNishiharaLoan(float Amount);

UFUNCTION(BlueprintCallable, Category = "北洋机制")
float K2_GetCurrentDebt() const;

UFUNCTION(BlueprintCallable, Category = "北洋机制")
void K2_UpdateFactionalBalance(float CivilChange, float MilitaryChange);

// 国民政府接口
UFUNCTION(BlueprintCallable, Category = "国民机制")
bool K2_StartNorthernExpedition();

UFUNCTION(BlueprintCallable, Category = "国民机制")
float K2_GetNorthernExpeditionProgress() const;

// 共产党接口
UFUNCTION(BlueprintCallable, Category = "中共机制")
bool K2_StartLandRedistribution();

UFUNCTION(BlueprintCallable, Category = "中共机制")
float K2_GetPeasantSupport() const;
```

---

## 9. 测试与验证

### 9.1 单元测试用例

```cpp
void UMingMechanicManagerTest::TestBeiyangCentralLegitimacy()
{
    // 测试税收加成
    float InitialTax = GetFactionTaxIncome(EFactionType::Beiyang);
    ActivateMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"));
    float ModifiedTax = GetFactionTaxIncome(EFactionType::Beiyang);

    TestEqual(TEXT("税收加成应为20%"), ModifiedTax, InitialTax * 1.20f, 0.01f);

    // 测试外交加成
    float DiplomacyBonus = GetMechanicNumericValue(TEXT("BEIYANG_CENTRAL_LEGITIMACY"), TEXT("DiplomacyBonus"));
    TestEqual(TEXT("外交加成应为15"), DiplomacyBonus, 15.0f, 0.01f);
}

void UMingMechanicManagerTest::TestNorthernExpeditionProgress()
{
    // 测试北伐启动
    int32 TestYear = 1926;
    SetGameYear(TestYear);

    bool bStarted = StartNorthernExpedition();
    TestTrue(TEXT("1926年可以启动北伐"), bStarted);

    // 测试进度更新
    UpdateNorthernExpeditionProgress(25.0f);
    float Progress = GetNorthernExpeditionProgress();
    TestEqual(TEXT("北伐进度应为25"), Progress, 25.0f, 0.01f);
}
```

### 9.2 集成测试场景

```cpp
// 测试北洋债务危机场景
void UMingMechanicManagerTest::TestBeiyangDebtCrisisScenario()
{
    // 大量借款
    ApplyNishiharaLoan(10000000.0f);
    ApplyReorganizationLoan(20000000.0f);

    // 检查债务危机触发
    bool bCrisisTriggered = CheckDebtCrisis();
    TestTrue(TEXT("高额债务应触发危机"), bCrisisTriggered);

    // 检查日本干预
    bool bJapaneseIntervention = HasForeignIntervention(EFactionType::Japan);
    TestTrue(TEXT("日本应干预内政"), bJapaneseIntervention);
}
```

---

## 10. 性能优化

### 10.1 优化策略

```cpp
// 机制效果缓存
TMap<FString, float> CachedNumericModifiers;
TMap<EFactionType, FCombinedEffect> CachedCombinedEffects;

// 批量更新
void UMingMechanicManager::BatchUpdateMechanics()
{
    // 收集所有需要更新的机制
    TArray<FString> MechanicsToUpdate;
    for (const auto& Kvp : AllMechanics)
    {
        if (Kvp.Value.bIsActive && NeedsUpdate(Kvp.Key))
        {
            MechanicsToUpdate.Add(Kvp.Key);
        }
    }

    // 批量应用效果
    ApplyMechanicEffectsBatch(MechanicsToUpdate);
}

// 异步计算
void UMingMechanicManager::AsyncCalculateMechanicEffects(const FString& MechanicID)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, MechanicID]()
    {
        CalculateMechanicEffectsInternal(MechanicID);

        // 回调主线程更新
        AsyncTask(ENamedThreads::GameThread, [this, MechanicID]()
        {
            ApplyCalculatedEffects(MechanicID);
        });
    });
}
```

---

**文档版本**: 1.0  
**最后更新**: 2026-03-23  
**作者**: MingGoRTS Dev Team  
**状态**: ✅ 已完成

**下一文档**: 历史事件设计文档
