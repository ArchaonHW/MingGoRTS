#include "Events/MingDecisionConsequenceCalculator.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingDecisionConsequenceCalculator::UMingDecisionConsequenceCalculator()
    : bIsInitialized(false)
{
    // 預分配容量
    RegisteredConsequences.Reserve(100);
    ConsequenceStatistics.Reserve(50);
}

void UMingDecisionConsequenceCalculator::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化民國歷史決策後果庫
    InitializeHistoricalDecisionConsequences();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingDecisionConsequenceCalculator initialized"));
    UE_LOG(LogTemp, Log, TEXT("Loaded %d decision consequences"), RegisteredConsequences.Num());
}

void UMingDecisionConsequenceCalculator::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    RegisteredConsequences.Empty();
    ConsequenceMap.Empty();
    ConsequenceStatistics.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingDecisionConsequenceCalculator shutdown"));
}

bool UMingDecisionConsequenceCalculator::RegisterDecisionConsequence(const FDecisionConsequence& Consequence)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("DecisionConsequenceCalculator not initialized"));
        return false;
    }

    if (Consequence.ConsequenceID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Consequence ID cannot be empty"));
        return false;
    }

    // 檢查是否已存在
    if (ConsequenceMap.Contains(Consequence.ConsequenceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Consequence %s already registered"), *Consequence.ConsequenceID);
        return false;
    }

    // 驗證後果配置
    if (!ValidateConsequenceConfig(Consequence))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid consequence configuration: %s"), *Consequence.ConsequenceID);
        return false;
    }

    // 添加到註冊列表
    RegisteredConsequences.Add(Consequence);
    ConsequenceMap.Add(Consequence.ConsequenceID, Consequence);
    
    UE_LOG(LogTemp, Log, TEXT("Decision consequence registered: %s - %s"), 
        *Consequence.ConsequenceID, *Consequence.ConsequenceName);
    
    return true;
}

bool UMingDecisionConsequenceCalculator::UnregisterDecisionConsequence(const FString& ConsequenceID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (!ConsequenceMap.Contains(ConsequenceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Consequence %s not found"), *ConsequenceID);
        return false;
    }

    // 從註冊列表移除
    for (int32 i = 0; i < RegisteredConsequences.Num(); ++i)
    {
        if (RegisteredConsequences[i].ConsequenceID == ConsequenceID)
        {
            RegisteredConsequences.RemoveAt(i);
            break;
        }
    }
    
    ConsequenceMap.Remove(ConsequenceID);
    
    UE_LOG(LogTemp, Log, TEXT("Decision consequence unregistered: %s"), *ConsequenceID);
    
    return true;
}

FConsequenceCalculationResult UMingDecisionConsequenceCalculator::CalculateDecisionConsequences(const FDecisionContext& Context)
{
    FConsequenceCalculationResult Result;
    Result.CalculationTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        Result.ErrorMessage = TEXT("DecisionConsequenceCalculator not initialized");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Calculating decision consequences for: %s"), *Context.DecisionID);

    // 獲取相關後果
    TArray<FDecisionConsequence> RelevantConsequences = GetConsequencesForDecision(Context.DecisionID);
    
    // 根據上下文篩選後果
    TArray<FDecisionConsequence> FilteredConsequences;
    for (const FDecisionConsequence& Consequence : RelevantConsequences)
    {
        if (CheckConsequenceConditions(Consequence, Context))
        {
            FilteredConsequences.Add(Consequence);
        }
    }

    // 排序後果
    SortConsequencesByPriority(FilteredConsequences);

    // 計算各種類型的後果
    CalculateImmediateConsequences(Context, Result);
    CalculateDelayedConsequences(Context, Result);
    CalculateCumulativeConsequences(Context, Result);
    CalculateConditionalConsequences(Context, Result);
    CalculateRandomConsequences(Context, Result);
    CalculateHiddenConsequences(Context, Result);
    CalculatePermanentConsequences(Context, Result);
    CalculateTemporaryConsequences(Context, Result);

    // 計算總體影響分數
    CalculateOverallImpact(Result);

    // 更新統計
    UpdateConsequenceStatistics(Context.DecisionID);

    UE_LOG(LogTemp, Log, TEXT("Decision consequences calculated: %d consequences, impact: %.2f"), 
        Result.CalculatedConsequences.Num(), Result.OverallImpactScore);
    
    return Result;
}

FDecisionConsequence UMingDecisionConsequenceCalculator::GetDecisionConsequence(const FString& ConsequenceID) const
{
    const FDecisionConsequence* Consequence = ConsequenceMap.Find(ConsequenceID);
    return Consequence ? *Consequence : FDecisionConsequence();
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetAllDecisionConsequences() const
{
    return RegisteredConsequences;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesByType(EDecisionConsequenceType ConsequenceType) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.ConsequenceType == ConsequenceType)
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesByScope(EConsequenceImpactScope ImpactScope) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.ImpactScope == ImpactScope)
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesBySeverity(EConsequenceSeverity Severity) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.Severity == Severity)
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesByImpactType(EConsequenceImpactType ImpactType) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.ImpactType == ImpactType)
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesForDecision(const FString& DecisionID) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.RelatedDecisionID == DecisionID)
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesForCharacter(const FString& CharacterID) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.RelatedCharacterIDs.Contains(CharacterID))
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::GetConsequencesForEvent(const FString& EventID) const
{
    TArray<FDecisionConsequence> Result;
    
    for (const FDecisionConsequence& Consequence : RegisteredConsequences)
    {
        if (Consequence.RelatedEventIDs.Contains(EventID))
        {
            Result.Add(Consequence);
        }
    }
    
    return Result;
}

bool UMingDecisionConsequenceCalculator::TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context)
{
    FDecisionConsequence* Consequence = ConsequenceMap.Find(ConsequenceID);
    if (!Consequence)
    {
        UE_LOG(LogTemp, Error, TEXT("Consequence %s not found"), *ConsequenceID);
        return false;
    }

    if (Consequence->bIsTriggered)
    {
        UE_LOG(LogTemp, Warning, TEXT("Consequence %s already triggered"), *ConsequenceID);
        return false;
    }

    if (!CheckConsequenceConditions(*Consequence, Context))
    {
        UE_LOG(LogTemp, Warning, TEXT("Consequence %s conditions not met"), *ConsequenceID);
        return false;
    }

    // 計算概率
    float Probability = CalculateConsequenceProbability(*Consequence, Context);
    if (FMath::FRand() > Probability)
    {
        UE_LOG(LogTemp, Log, TEXT("Consequence %s probability check failed"), *ConsequenceID);
        return false;
    }

    // 計算延迟
    float Delay = CalculateConsequenceDelay(*Consequence, Context);
    if (Delay > 0.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("Consequence %s delayed by %.2f seconds"), *ConsequenceID, Delay);
        // 這裡應該設置定時器來延迟觸發
    }

    // 標記為已觸發
    Consequence->bIsTriggered = true;
    Consequence->TriggerTime = FPlatformTime::Seconds();

    // 應用後果效果
    bool bSuccess = ApplyConsequenceEffects(ConsequenceID, Context);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Consequence %s triggered successfully"), *ConsequenceID);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to apply consequence %s effects"), *ConsequenceID);
    }

    return bSuccess;
}

bool UMingDecisionConsequenceCalculator::ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context)
{
    const FDecisionConsequence* Consequence = ConsequenceMap.Find(ConsequenceID);
    if (!Consequence)
    {
        return false;
    }

    // 應用各種效果
    for (const FString& Effect : Consequence->Effects)
    {
        // 簡化實作：記錄效果
        UE_LOG(LogTemp, Log, TEXT("Applying consequence effect: %s"), *Effect);
    }

    return true;
}

bool UMingDecisionConsequenceCalculator::ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context)
{
    const FDecisionConsequence* Consequence = ConsequenceMap.Find(ConsequenceID);
    if (!Consequence)
    {
        return false;
    }

    if (!Consequence->bIsReversible)
    {
        UE_LOG(LogTemp, Warning, TEXT("Consequence %s is not reversible"), *ConsequenceID);
        return false;
    }

    // 檢查逆轉條件
    for (const FString& ReversalCondition : Consequence->ReversalConditions)
    {
        // 簡化實作：檢查條件
        UE_LOG(LogTemp, Log, TEXT("Checking reversal condition: %s"), *ReversalCondition);
    }

    UE_LOG(LogTemp, Log, TEXT("Consequence %s reversed"), *ConsequenceID);
    return true;
}

TMap<FString, int32> UMingDecisionConsequenceCalculator::GetConsequenceStatistics() const
{
    return ConsequenceStatistics;
}

TMap<FString, float> UMingDecisionConsequenceCalculator::GetConsequenceImpactAnalysis(const FString& DecisionID) const
{
    TMap<FString, float> ImpactAnalysis;
    
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        float Impact = 0.0f;
        
        // 基於嚴重程度
        switch (Consequence.Severity)
        {
        case EConsequenceSeverity::Trivial:
            Impact += 0.1f;
            break;
        case EConsequenceSeverity::Minor:
            Impact += 0.3f;
            break;
        case EConsequenceSeverity::Moderate:
            Impact += 0.5f;
            break;
        case EConsequenceSeverity::Major:
            Impact += 0.7f;
            break;
        case EConsequenceSeverity::Critical:
            Impact += 0.9f;
            break;
        case EConsequenceSeverity::Catastrophic:
            Impact += 1.0f;
            break;
        }
        
        // 基於影響範圍
        switch (Consequence.ImpactScope)
        {
        case EConsequenceImpactScope::Personal:
            Impact += 0.2f;
            break;
        case EConsequenceImpactScope::Local:
            Impact += 0.4f;
            break;
        case EConsequenceImpactScope::Regional:
            Impact += 0.6f;
            break;
        case EConsequenceImpactScope::National:
            Impact += 0.8f;
            break;
        case EConsequenceImpactScope::International:
            Impact += 0.9f;
            break;
        case EConsequenceImpactScope::Global:
            Impact += 1.0f;
            break;
        }
        
        ImpactAnalysis.Add(Consequence.ConsequenceID, Impact);
    }
    
    return ImpactAnalysis;
}

TArray<FDecisionConsequence> UMingDecisionConsequenceCalculator::PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount)
{
    TArray<FDecisionConsequence> PredictedConsequences;
    
    // 獲取相關後果
    TArray<FDecisionConsequence> RelevantConsequences = GetConsequencesForDecision(Context.DecisionID);
    
    // 根據概率和條件預測
    for (const FDecisionConsequence& Consequence : RelevantConsequences)
    {
        if (CheckConsequenceConditions(Consequence, Context))
        {
            float Probability = CalculateConsequenceProbability(Consequence, Context);
            
            // 如果概率足夠高，加入預測
            if (Probability >= 0.3f)
            {
                PredictedConsequences.Add(Consequence);
            }
        }
    }
    
    // 限制預測數量
    if (PredictedConsequences.Num() > PredictionCount)
    {
        PredictedConsequences.SetNum(PredictionCount);
    }
    
    return PredictedConsequences;
}

TArray<FConsequenceCalculationResult> UMingDecisionConsequenceCalculator::SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount)
{
    TArray<FConsequenceCalculationResult> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FConsequenceCalculationResult Result = CalculateDecisionConsequences(Context);
        SimulationResults.Add(Result);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d decision consequences"), SimulationCount);
    
    return SimulationResults;
}

// === 內部方法實現 ===

void UMingDecisionConsequenceCalculator::InitializeHistoricalDecisionConsequences()
{
    CreatePoliticalDecisionConsequences();
    CreateMilitaryDecisionConsequences();
    CreateEconomicDecisionConsequences();
    CreateSocialDecisionConsequences();
    CreateCulturalDecisionConsequences();
    CreateDiplomaticDecisionConsequences();
}

void UMingDecisionConsequenceCalculator::CreatePoliticalDecisionConsequences()
{
    // 辛亥革命成功後果
    FDecisionConsequence XinhaiRevolutionConsequence;
    XinhaiRevolutionConsequence.ConsequenceID = TEXT("xinhai_revolution_success");
    XinhaiRevolutionConsequence.ConsequenceName = TEXT("辛亥革命成功後果");
    XinhaiRevolutionConsequence.ConsequenceDescription = TEXT("辛亥革命成功，建立中華民國");
    XinhaiRevolutionConsequence.ConsequenceType = EDecisionConsequenceType::Immediate;
    XinhaiRevolutionConsequence.ImpactScope = EConsequenceImpactScope::National;
    XinhaiRevolutionConsequence.Severity = EConsequenceSeverity::Critical;
    XinhaiRevolutionConsequence.Duration = EConsequenceDuration::Permanent;
    XinhaiRevolutionConsequence.ImpactType = EConsequenceImpactType::Positive;
    XinhaiRevolutionConsequence.RelatedDecisionID = TEXT("xinhai_revolution");
    XinhaiRevolutionConsequence.Probability = 0.9f;
    XinhaiRevolutionConsequence.DelayTime = 0.0f;
    XinhaiRevolutionConsequence.bIsReversible = false;
    XinhaiRevolutionConsequence.Effects.Add(TEXT("establish_republic"));
    XinhaiRevolutionConsequence.Effects.Add(TEXT("overthrow_monarchy"));
    XinhaiRevolutionConsequence.Effects.Add(TEXT("national_unity"));
    XinhaiRevolutionConsequence.ConsequenceTags.Add(TEXT("revolution"));
    XinhaiRevolutionConsequence.ConsequenceTags.Add(TEXT("founding"));
    
    RegisterDecisionConsequence(XinhaiRevolutionConsequence);

    // 北伐統一後果
    FDecisionConsequence NorthernExpeditionConsequence;
    NorthernExpeditionConsequence.ConsequenceID = TEXT("northern_expedition_success");
    NorthernExpeditionConsequence.ConsequenceName = TEXT("北伐統一後果");
    NorthernExpeditionConsequence.ConsequenceDescription = TEXT("北伐成功，統一中國");
    NorthernExpeditionConsequence.ConsequenceType = EDecisionConsequenceType::Immediate;
    NorthernExpeditionConsequence.ImpactScope = EConsequenceImpactScope::National;
    NorthernExpeditionConsequence.Severity = EConsequenceSeverity::Major;
    NorthernExpeditionConsequence.Duration = EConsequenceDuration::Permanent;
    NorthernExpeditionConsequence.ImpactType = EConsequenceImpactType::Positive;
    NorthernExpeditionConsequence.RelatedDecisionID = TEXT("northern_expedition");
    NorthernExpeditionConsequence.Probability = 0.8f;
    NorthernExpeditionConsequence.DelayTime = 0.0f;
    NorthernExpeditionConsequence.bIsReversible = false;
    NorthernExpeditionConsequence.Effects.Add(TEXT("unify_china"));
    NorthernExpeditionConsequence.Effects.Add(TEXT("end_warlord_era"));
    NorthernExpeditionConsequence.Effects.Add(TEXT("strengthen_central_government"));
    NorthernExpeditionConsequence.ConsequenceTags.Add(TEXT("unification"));
    NorthernExpeditionConsequence.ConsequenceTags.Add(TEXT("military"));
    
    RegisterDecisionConsequence(NorthernExpeditionConsequence);
}

void UMingDecisionConsequenceCalculator::CreateMilitaryDecisionConsequences()
{
    // 抗戰決定後果
    FDecisionConsequence AntiJapaneseWarConsequence;
    AntiJapaneseWarConsequence.ConsequenceID = TEXT("anti_japanese_war_decision");
    AntiJapaneseWarConsequence.ConsequenceName = TEXT("抗戰決定後果");
    AntiJapaneseWarConsequence.ConsequenceDescription = TEXT("決定全面抗戰，抵抗日本侵略");
    AntiJapaneseWarConsequence.ConsequenceType = EDecisionConsequenceType::Immediate;
    AntiJapaneseWarConsequence.ImpactScope = EConsequenceImpactScope::National;
    AntiJapaneseWarConsequence.Severity = EConsequenceSeverity::Critical;
    AntiJapaneseWarConsequence.Duration = EConsequenceDuration::Long;
    AntiJapaneseWarConsequence.ImpactType = EConsequenceImpactType::Mixed;
    AntiJapaneseWarConsequence.RelatedDecisionID = TEXT("anti_japanese_war");
    AntiJapaneseWarConsequence.Probability = 0.95f;
    AntiJapaneseWarConsequence.DelayTime = 0.0f;
    AntiJapaneseWarConsequence.bIsReversible = false;
    AntiJapaneseWarConsequence.Effects.Add(TEXT("national_unity"));
    AntiJapaneseWarConsequence.Effects.Add(TEXT("massive_casualties"));
    AntiJapaneseWarConsequence.Effects.Add(TEXT("economic_devastation"));
    AntiJapaneseWarConsequence.Effects.Add(TEXT("international_support"));
    AntiJapaneseWarConsequence.ConsequenceTags.Add(TEXT("war"));
    AntiJapaneseWarConsequence.ConsequenceTags.Add(TEXT("resistance"));
    
    RegisterDecisionConsequence(AntiJapaneseWarConsequence);
}

void UMingDecisionConsequenceCalculator::CreateEconomicDecisionConsequences()
{
    // 經濟改革後果
    FDecisionConsequence EconomicReformConsequence;
    EconomicReformConsequence.ConsequenceID = TEXT("economic_reform_decision");
    EconomicReformConsequence.ConsequenceName = TEXT("經濟改革後果");
    EconomicReformConsequence.ConsequenceDescription = TEXT("實施經濟改革，發展現代工業");
    EconomicReformConsequence.ConsequenceType = EDecisionConsequenceType::Delayed;
    EconomicReformConsequence.ImpactScope = EConsequenceImpactScope::National;
    EconomicReformConsequence.Severity = EConsequenceSeverity::Major;
    EconomicReformConsequence.Duration = EConsequenceDuration::Long;
    EconomicReformConsequence.ImpactType = EConsequenceImpactType::Positive;
    EconomicReformConsequence.RelatedDecisionID = TEXT("economic_reform");
    EconomicReformConsequence.Probability = 0.7f;
    EconomicReformConsequence.DelayTime = 3600.0f; // 1小時後
    EconomicReformConsequence.bIsReversible = true;
    EconomicReformConsequence.Effects.Add(TEXT("industrial_development"));
    EconomicReformConsequence.Effects.Add(TEXT("economic_growth"));
    EconomicReformConsequence.Effects.Add(TEXT("social_change"));
    EconomicReformConsequence.ReversalConditions.Add(TEXT("political_opposition"));
    EconomicReformConsequence.ConsequenceTags.Add(TEXT("economy"));
    EconomicReformConsequence.ConsequenceTags.Add(TEXT("reform"));
    
    RegisterDecisionConsequence(EconomicReformConsequence);
}

void UMingDecisionConsequenceCalculator::CreateSocialDecisionConsequences()
{
    // 教育改革後果
    FDecisionConsequence EducationReformConsequence;
    EducationReformConsequence.ConsequenceID = TEXT("education_reform_decision");
    EducationReformConsequence.ConsequenceName = TEXT("教育改革後果");
    EducationReformConsequence.ConsequenceDescription = TEXT("推動教育改革，提高國民素質");
    EducationReformConsequence.ConsequenceType = EDecisionConsequenceType::Cumulative;
    EducationReformConsequence.ImpactScope = EConsequenceImpactScope::National;
    EducationReformConsequence.Severity = EConsequenceSeverity::Moderate;
    EducationReformConsequence.Duration = EConsequenceDuration::Long;
    EducationReformConsequence.ImpactType = EConsequenceImpactType::Positive;
    EducationReformConsequence.RelatedDecisionID = TEXT("education_reform");
    EducationReformConsequence.Probability = 0.8f;
    EducationReformConsequence.DelayTime = 1800.0f; // 30分鐘後
    EducationReformConsequence.bIsReversible = false;
    EducationReformConsequence.Effects.Add(TEXT("literacy_improvement"));
    EducationReformConsequence.Effects.Add(TEXT("cultural_development"));
    EducationReformConsequence.Effects.Add(TEXT("talent_cultivation"));
    EducationReformConsequence.ConsequenceTags.Add(TEXT("education"));
    EducationReformConsequence.ConsequenceTags.Add(TEXT("social"));
    
    RegisterDecisionConsequence(EducationReformConsequence);
}

void UMingDecisionConsequenceCalculator::CreateCulturalDecisionConsequences()
{
    // 新文化運動後果
    FDecisionConsequence NewCultureMovementConsequence;
    NewCultureMovementConsequence.ConsequenceID = TEXT("new_culture_movement_decision");
    NewCultureMovementConsequence.ConsequenceName = TEXT("新文化運動後果");
    NewCultureMovementConsequence.ConsequenceDescription = TEXT("推動新文化運動，促進思想解放");
    NewCultureMovementConsequence.ConsequenceType = EDecisionConsequenceType::Conditional;
    NewCultureMovementConsequence.ImpactScope = EConsequenceImpactScope::National;
    NewCultureMovementConsequence.Severity = EConsequenceSeverity::Moderate;
    NewCultureMovementConsequence.Duration = EConsequenceDuration::Permanent;
    NewCultureMovementConsequence.ImpactType = EConsequenceImpactType::Mixed;
    NewCultureMovementConsequence.RelatedDecisionID = TEXT("new_culture_movement");
    NewCultureMovementConsequence.Probability = 0.6f;
    NewCultureMovementConsequence.DelayTime = 0.0f;
    NewCultureMovementConsequence.bIsReversible = false;
    NewCultureMovementConsequence.Effects.Add(TEXT("ideological_change"));
    NewCultureMovementConsequence.Effects.Add(TEXT("literary_innovation"));
    NewCultureMovementConsequence.Effects.Add(TEXT("social_debate"));
    NewCultureMovementConsequence.TriggerConditions.Add(TEXT("intellectual_support"));
    NewCultureMovementConsequence.TriggerConditions.Add(TEXT("political_tolerance"));
    NewCultureMovementConsequence.ConsequenceTags.Add(TEXT("culture"));
    NewCultureMovementConsequence.ConsequenceTags.Add(TEXT("movement"));
    
    RegisterDecisionConsequence(NewCultureMovementConsequence);
}

void UMingDecisionConsequenceCalculator::CreateDiplomaticDecisionConsequences()
{
    // 外交政策後果
    FDecisionConsequence ForeignPolicyConsequence;
    ForeignPolicyConsequence.ConsequenceID = TEXT("foreign_policy_decision");
    ForeignPolicyConsequence.ConsequenceName = TEXT("外交政策後果");
    ForeignPolicyConsequence.ConsequenceDescription = TEXT("制定外交政策，處理國際關係");
    ForeignPolicyConsequence.ConsequenceType = EDecisionConsequenceType::Immediate;
    ForeignPolicyConsequence.ImpactScope = EConsequenceImpactScope::International;
    ForeignPolicyConsequence.Severity = EConsequenceSeverity::Major;
    ForeignPolicyConsequence.Duration = EConsequenceDuration::Medium;
    ForeignPolicyConsequence.ImpactType = EConsequenceImpactType::Mixed;
    ForeignPolicyConsequence.RelatedDecisionID = TEXT("foreign_policy");
    ForeignPolicyConsequence.Probability = 0.85f;
    ForeignPolicyConsequence.DelayTime = 0.0f;
    ForeignPolicyConsequence.bIsReversible = true;
    ForeignPolicyConsequence.Effects.Add(TEXT("international_relations"));
    ForeignPolicyConsequence.Effects.Add(TEXT("foreign_aid"));
    ForeignPolicyConsequence.Effects.Add(TEXT("trade_agreements"));
    ForeignPolicyConsequence.ReversalConditions.Add(TEXT("policy_change"));
    ForeignPolicyConsequence.ConsequenceTags.Add(TEXT("diplomacy"));
    ForeignPolicyConsequence.ConsequenceTags.Add(TEXT("foreign"));
    
    RegisterDecisionConsequence(ForeignPolicyConsequence);
}

// === 計算方法實現 ===

void UMingDecisionConsequenceCalculator::CalculateImmediateConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> ImmediateConsequences = GetConsequencesByType(EDecisionConsequenceType::Immediate);
    
    for (const FDecisionConsequence& Consequence : ImmediateConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID && CheckConsequenceConditions(Consequence, Context))
        {
            float Probability = CalculateConsequenceProbability(Consequence, Context);
            if (FMath::FRand() <= Probability)
            {
                Result.CalculatedConsequences.Add(Consequence);
                
                // 應用效果到各個系統
                ApplyConsequenceToGameState(Consequence, Context, Result.AffectedGameState);
                ApplyConsequenceToCharacters(Consequence, Context, Result.AffectedCharacters);
                ApplyConsequenceToEvents(Consequence, Context, Result.AffectedEvents);
                ApplyConsequenceToResources(Consequence, Context, Result.AffectedResources);
                ApplyConsequenceToBuildings(Consequence, Context, Result.AffectedBuildings);
                ApplyConsequenceToUnits(Consequence, Context, Result.AffectedUnits);
                
                // 計算影響分數
                float Impact = CalculateConsequenceImpact(Consequence, Context);
                if (Consequence.ImpactType == EConsequenceImpactType::Positive)
                {
                    Result.PositiveImpactScore += Impact;
                }
                else if (Consequence.ImpactType == EConsequenceImpactType::Negative)
                {
                    Result.NegativeImpactScore += Impact;
                }
            }
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateDelayedConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> DelayedConsequences = GetConsequencesByType(EDecisionConsequenceType::Delayed);
    
    for (const FDecisionConsequence& Consequence : DelayedConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID && CheckConsequenceConditions(Consequence, Context))
        {
            float Delay = CalculateConsequenceDelay(Consequence, Context);
            if (Delay > 0.0f)
            {
                // 這裡應該設置定時器來延迟觸發
                UE_LOG(LogTemp, Log, TEXT("Delayed consequence %s scheduled for %.2f seconds"), *Consequence.ConsequenceID, Delay);
                Result.CalculatedConsequences.Add(Consequence);
            }
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateCumulativeConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> CumulativeConsequences = GetConsequencesByType(EDecisionConsequenceType::Cumulative);
    
    for (const FDecisionConsequence& Consequence : CumulativeConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID && CheckConsequenceConditions(Consequence, Context))
        {
            // 累積效果需要基於歷史決策
            UE_LOG(LogTemp, Log, TEXT("Cumulative consequence %s calculated"), *Consequence.ConsequenceID);
            Result.CalculatedConsequences.Add(Consequence);
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateConditionalConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> ConditionalConsequences = GetConsequencesByType(EDecisionConsequenceType::Conditional);
    
    for (const FDecisionConsequence& Consequence : ConditionalConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID)
        {
            if (CheckConsequenceConditions(Consequence, Context))
            {
                Result.CalculatedConsequences.Add(Consequence);
            }
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateRandomConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> RandomConsequences = GetConsequencesByType(EDecisionConsequenceType::Random);
    
    for (const FDecisionConsequence& Consequence : RandomConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID)
        {
            float RandomProbability = FMath::FRand();
            if (RandomProbability <= 0.3f) // 30%概率
            {
                Result.CalculatedConsequences.Add(Consequence);
                UE_LOG(LogTemp, Log, TEXT("Random consequence %s triggered"), *Consequence.ConsequenceID);
            }
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateHiddenConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> HiddenConsequences = GetConsequencesByType(EDecisionConsequenceType::Hidden);
    
    for (const FDecisionConsequence& Consequence : HiddenConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID)
        {
            // 隱藏後果不會立即顯示
            UE_LOG(LogTemp, Verbose, TEXT("Hidden consequence %s calculated"), *Consequence.ConsequenceID);
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculatePermanentConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> PermanentConsequences = GetConsequencesByType(EDecisionConsequenceType::Permanent);
    
    for (const FDecisionConsequence& Consequence : PermanentConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID && CheckConsequenceConditions(Consequence, Context))
        {
            Result.CalculatedConsequences.Add(Consequence);
            UE_LOG(LogTemp, Log, TEXT("Permanent consequence %s applied"), *Consequence.ConsequenceID);
        }
    }
}

void UMingDecisionConsequenceCalculator::CalculateTemporaryConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result)
{
    TArray<FDecisionConsequence> TemporaryConsequences = GetConsequencesByType(EDecisionConsequenceType::Temporary);
    
    for (const FDecisionConsequence& Consequence : TemporaryConsequences)
    {
        if (Consequence.RelatedDecisionID == Context.DecisionID && CheckConsequenceConditions(Consequence, Context))
        {
            float Duration = CalculateConsequenceDuration(Consequence, Context);
            if (Duration > 0.0f)
            {
                Result.CalculatedConsequences.Add(Consequence);
                UE_LOG(LogTemp, Log, TEXT("Temporary consequence %s applied for %.2f seconds"), *Consequence.ConsequenceID, Duration);
            }
        }
    }
}

float UMingDecisionConsequenceCalculator::CalculateConsequenceImpact(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const
{
    float Impact = 0.0f;
    
    // 基於嚴重程度
    switch (Consequence.Severity)
    {
    case EConsequenceSeverity::Trivial:
        Impact += 0.1f;
        break;
    case EConsequenceSeverity::Minor:
        Impact += 0.3f;
        break;
    case EConsequenceSeverity::Moderate:
        Impact += 0.5f;
        break;
    case EConsequenceSeverity::Major:
        Impact += 0.7f;
        break;
    case EConsequenceSeverity::Critical:
        Impact += 0.9f;
        break;
    case EConsequenceSeverity::Catastrophic:
        Impact += 1.0f;
        break;
    }
    
    // 基於影響範圍
    switch (Consequence.ImpactScope)
    {
    case EConsequenceImpactScope::Personal:
        Impact += 0.1f;
        break;
    case EConsequenceImpactScope::Local:
        Impact += 0.3f;
        break;
    case EConsequenceImpactScope::Regional:
        Impact += 0.5f;
        break;
    case EConsequenceImpactScope::National:
        Impact += 0.7f;
        break;
    case EConsequenceImpactScope::International:
        Impact += 0.9f;
        break;
    case EConsequenceImpactScope::Global:
        Impact += 1.0f;
        break;
    }
    
    // 基於持續時間
    switch (Consequence.Duration)
    {
    case EConsequenceDuration::Instant:
        Impact += 0.1f;
        break;
    case EConsequenceDuration::Short:
        Impact += 0.3f;
        break;
    case EConsequenceDuration::Medium:
        Impact += 0.5f;
        break;
    case EConsequenceDuration::Long:
        Impact += 0.7f;
        break;
    case EConsequenceDuration::Permanent:
        Impact += 1.0f;
        break;
    case EConsequenceDuration::Indefinite:
        Impact += 0.8f;
        break;
    }
    
    return FMath::Clamp(Impact, 0.0f, 1.0f);
}

void UMingDecisionConsequenceCalculator::CalculateOverallImpact(FConsequenceCalculationResult& Result) const
{
    Result.OverallImpactScore = Result.PositiveImpactScore - Result.NegativeImpactScore;
}

bool UMingDecisionConsequenceCalculator::CheckConsequenceConditions(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const
{
    for (const FString& Condition : Consequence.TriggerConditions)
    {
        // 簡化實作：檢查條件
        if (Condition == TEXT("intellectual_support"))
        {
            // 檢查是否有知識分子支持
            if (!Context.CharacterStates.Contains(TEXT("scholar_support")))
            {
                return false;
            }
        }
        else if (Condition == TEXT("political_tolerance"))
        {
            // 檢查政治容忍度
            FString* Tolerance = Context.CurrentGameState.Find(TEXT("political_tolerance"));
            if (!Tolerance || FCString::Atof(*Tolerance) < 0.5f)
            {
                return false;
            }
        }
    }
    
    return true;
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToGameState(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, FString>& AffectedStates) const
{
    for (const FString& Effect : Consequence.Effects)
    {
        if (Effect == TEXT("establish_republic"))
        {
            AffectedStates.Add(TEXT("government_type"), TEXT("republic"));
        }
        else if (Effect == TEXT("overthrow_monarchy"))
        {
            AffectedStates.Add(TEXT("monarchy"), TEXT("overthrown"));
        }
        else if (Effect == TEXT("national_unity"))
        {
            AffectedStates.Add(TEXT("unity_level"), TEXT("high"));
        }
        else if (Effect == TEXT("unify_china"))
        {
            AffectedStates.Add(TEXT("unification_status"), TEXT("unified"));
        }
        else if (Effect == TEXT("end_warlord_era"))
        {
            AffectedStates.Add(TEXT("warlord_era"), TEXT("ended"));
        }
        else if (Effect == TEXT("strengthen_central_government"))
        {
            AffectedStates.Add(TEXT("central_government_power"), TEXT("strong"));
        }
    }
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToCharacters(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedCharacters) const
{
    // 添加相關人物到受影響列表
    for (const FString& CharacterID : Consequence.RelatedCharacterIDs)
    {
        AffectedCharacters.Add(CharacterID);
    }
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToEvents(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedEvents) const
{
    // 添加相關事件到受影響列表
    for (const FString& EventID : Consequence.RelatedEventIDs)
    {
        AffectedEvents.Add(EventID);
    }
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToResources(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, int32>& AffectedResources) const
{
    // 簡化實作：基於效果影響資源
    for (const FString& Effect : Consequence.Effects)
    {
        if (Effect == TEXT("economic_devastation"))
        {
            AffectedResources.Add(TEXT("economy"), -50);
        }
        else if (Effect == TEXT("economic_growth"))
        {
            AffectedResources.Add(TEXT("economy"), 30);
        }
        else if (Effect == TEXT("massive_casualties"))
        {
            AffectedResources.Add(TEXT("population"), -20);
        }
        else if (Effect == TEXT("international_support"))
        {
            AffectedResources.Add(TEXT("foreign_aid"), 40);
        }
    }
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToBuildings(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedBuildings) const
{
    // 簡化實作：基於效果影響建築
    for (const FString& Effect : Consequence.Effects)
    {
        if (Effect == TEXT("industrial_development"))
        {
            AffectedBuildings.Add(TEXT("factory"));
            AffectedBuildings.Add(TEXT("power_plant"));
        }
    }
}

void UMingDecisionConsequenceCalculator::ApplyConsequenceToUnits(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedUnits) const
{
    // 簡化實作：基於效果影響單位
    for (const FString& Effect : Consequence.Effects)
    {
        if (Effect == TEXT("military_development"))
        {
            AffectedUnits.Add(TEXT("soldier"));
            AffectedUnits.Add(TEXT("officer"));
        }
    }
}

void UMingDecisionConsequenceCalculator::UpdateConsequenceStatistics(const FString& DecisionID)
{
    int32* Count = ConsequenceStatistics.Find(DecisionID);
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        ConsequenceStatistics.Add(DecisionID, 1);
    }
}

bool UMingDecisionConsequenceCalculator::ValidateConsequenceConfig(const FDecisionConsequence& Consequence) const
{
    if (Consequence.ConsequenceID.IsEmpty())
    {
        return false;
    }
    
    if (Consequence.ConsequenceName.IsEmpty())
    {
        return false;
    }
    
    if (Consequence.Effects.Num() == 0)
    {
        return false;
    }
    
    return true;
}

void UMingDecisionConsequenceCalculator::LogConsequenceEvent(const FString& ConsequenceID, const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Consequence %s] %s"), *ConsequenceID, *Message);
}

float UMingDecisionConsequenceCalculator::CalculateConsequenceProbability(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const
{
    float BaseProbability = Consequence.Probability;
    
    // 基於難度調整
    float DifficultyModifier = 1.0f - (Context.DifficultyLevel - 1) * 0.1f;
    
    // 基於玩家陣營調整
    float FactionModifier = 1.0f;
    if (Context.PlayerFaction == TEXT("nationalist"))
    {
        FactionModifier = 1.1f;
    }
    else if (Context.PlayerFaction == TEXT("communist"))
    {
        FactionModifier = 0.9f;
    }
    
    return FMath::Clamp(BaseProbability * DifficultyModifier * FactionModifier, 0.0f, 1.0f);
}

float UMingDecisionConsequenceCalculator::CalculateConsequenceDelay(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const
{
    float BaseDelay = Consequence.DelayTime;
    
    // 基於難度調整
    float DifficultyModifier = (Context.DifficultyLevel - 1) * 0.2f;
    
    return BaseDelay + DifficultyModifier * 3600.0f; // 難度每增加1級，延迟增加1小時
}

float UMingDecisionConsequenceCalculator::CalculateConsequenceDuration(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const
{
    float BaseDuration = 0.0f;
    
    switch (Consequence.Duration)
    {
    case EConsequenceDuration::Instant:
        BaseDuration = 0.0f;
        break;
    case EConsequenceDuration::Short:
        BaseDuration = 1800.0f; // 30分鐘
        break;
    case EConsequenceDuration::Medium:
        BaseDuration = 7200.0f; // 2小時
        break;
    case EConsequenceDuration::Long:
        BaseDuration = 28800.0f; // 8小時
        break;
    case EConsequenceDuration::Permanent:
        BaseDuration = -1.0f; // 永久
        break;
    case EConsequenceDuration::Indefinite:
        BaseDuration = 86400.0f; // 24小時
        break;
    }
    
    return BaseDuration;
}

int32 UMingDecisionConsequenceCalculator::GetConsequencePriority(const FDecisionConsequence& Consequence) const
{
    int32 Priority = 0;
    
    // 基於嚴重程度
    switch (Consequence.Severity)
    {
    case EConsequenceSeverity::Catastrophic:
        Priority += 100;
        break;
    case EConsequenceSeverity::Critical:
        Priority += 80;
        break;
    case EConsequenceSeverity::Major:
        Priority += 60;
        break;
    case EConsequenceSeverity::Moderate:
        Priority += 40;
        break;
    case EConsequenceSeverity::Minor:
        Priority += 20;
        break;
    case EConsequenceSeverity::Trivial:
        Priority += 10;
        break;
    }
    
    // 基於影響範圍
    switch (Consequence.ImpactScope)
    {
    case EConsequenceImpactScope::Global:
        Priority += 50;
        break;
    case EConsequenceImpactScope::International:
        Priority += 40;
        break;
    case EConsequenceImpactScope::National:
        Priority += 30;
        break;
    case EConsequenceImpactScope::Regional:
        Priority += 20;
        break;
    case EConsequenceImpactScope::Local:
        Priority += 10;
        break;
    case EConsequenceImpactScope::Personal:
        Priority += 5;
        break;
    }
    
    return Priority;
}

void UMingDecisionConsequenceCalculator::SortConsequencesByPriority(TArray<FDecisionConsequence>& Consequences) const
{
    Consequences.Sort([this](const FDecisionConsequence& A, const FDecisionConsequence& B)
    {
        return GetConsequencePriority(A) > GetConsequencePriority(B);
    });
}
