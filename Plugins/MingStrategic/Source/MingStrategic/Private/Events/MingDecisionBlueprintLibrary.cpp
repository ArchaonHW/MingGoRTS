#include "Events/MingDecisionBlueprintLibrary.h"
#include "Engine/Engine.h"

UMingDecisionConsequenceCalculator* UMingDecisionBlueprintLibrary::GetDecisionConsequenceCalculator()
{
    // 這裡應該從遊戲實例或單例獲取決策後果計算器
    // 簡化實作
    static UMingDecisionConsequenceCalculator* CalculatorInstance = nullptr;
    if (!CalculatorInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            CalculatorInstance = NewObject<UMingDecisionConsequenceCalculator>(World);
            CalculatorInstance->Initialize();
        }
    }
    
    return CalculatorInstance;
}

void UMingDecisionBlueprintLibrary::InitializeDecisionConsequenceCalculator()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (Calculator)
    {
        Calculator->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Decision consequence calculator initialized from Blueprint"));
    }
}

bool UMingDecisionBlueprintLibrary::RegisterDecisionConsequence(const FDecisionConsequence& Consequence)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        UE_LOG(LogTemp, Error, TEXT("Decision consequence calculator not available"));
        return false;
    }
    
    return Calculator->RegisterDecisionConsequence(Consequence);
}

bool UMingDecisionBlueprintLibrary::UnregisterDecisionConsequence(const FString& ConsequenceID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return false;
    }
    
    return Calculator->UnregisterDecisionConsequence(ConsequenceID);
}

FConsequenceCalculationResult UMingDecisionBlueprintLibrary::CalculateDecisionConsequences(const FDecisionContext& Context)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        FConsequenceCalculationResult Result;
        Result.ErrorMessage = TEXT("Decision consequence calculator not available");
        return Result;
    }
    
    return Calculator->CalculateDecisionConsequences(Context);
}

FDecisionConsequence UMingDecisionBlueprintLibrary::GetDecisionConsequence(const FString& ConsequenceID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return FDecisionConsequence();
    }
    
    return Calculator->GetDecisionConsequence(ConsequenceID);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetAllDecisionConsequences()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetAllDecisionConsequences();
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesByType(EDecisionConsequenceType ConsequenceType)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesByType(ConsequenceType);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesByScope(EConsequenceImpactScope ImpactScope)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesByScope(ImpactScope);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesBySeverity(EConsequenceSeverity Severity)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesBySeverity(Severity);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesByImpactType(EConsequenceImpactType ImpactType)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesByImpactType(ImpactType);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesForDecision(const FString& DecisionID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesForDecision(DecisionID);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesForCharacter(const FString& CharacterID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesForCharacter(CharacterID);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequencesForEvent(const FString& EventID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->GetConsequencesForEvent(EventID);
}

bool UMingDecisionBlueprintLibrary::TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return false;
    }
    
    return Calculator->TriggerConsequence(ConsequenceID, Context);
}

bool UMingDecisionBlueprintLibrary::ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return false;
    }
    
    return Calculator->ApplyConsequenceEffects(ConsequenceID, Context);
}

bool UMingDecisionBlueprintLibrary::ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return false;
    }
    
    return Calculator->ReverseConsequence(ConsequenceID, Context);
}

TMap<FString, int32> UMingDecisionBlueprintLibrary::GetConsequenceStatistics()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TMap<FString, int32>();
    }
    
    return Calculator->GetConsequenceStatistics();
}

TMap<FString, float> UMingDecisionBlueprintLibrary::GetConsequenceImpactAnalysis(const FString& DecisionID)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TMap<FString, float>();
    }
    
    return Calculator->GetConsequenceImpactAnalysis(DecisionID);
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FDecisionConsequence>();
    }
    
    return Calculator->PredictDecisionConsequences(Context, PredictionCount);
}

TArray<FConsequenceCalculationResult> UMingDecisionBlueprintLibrary::SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return TArray<FConsequenceCalculationResult>();
    }
    
    return Calculator->SimulateDecisionConsequences(Context, SimulationCount);
}

FString UMingDecisionBlueprintLibrary::GetConsequenceTypeName(EDecisionConsequenceType ConsequenceType)
{
    switch (ConsequenceType)
    {
    case EDecisionConsequenceType::Immediate:
        return TEXT("即時");
    case EDecisionConsequenceType::Delayed:
        return TEXT("延迟");
    case EDecisionConsequenceType::Cumulative:
        return TEXT("累積");
    case EDecisionConsequenceType::Conditional:
        return TEXT("條件");
    case EDecisionConsequenceType::Random:
        return TEXT("隨機");
    case EDecisionConsequenceType::Hidden:
        return TEXT("隱藏");
    case EDecisionConsequenceType::Permanent:
        return TEXT("永久");
    case EDecisionConsequenceType::Temporary:
        return TEXT("臨時");
    default:
        return TEXT("未知類型");
    }
}

FString UMingDecisionBlueprintLibrary::GetImpactScopeName(EConsequenceImpactScope ImpactScope)
{
    switch (ImpactScope)
    {
    case EConsequenceImpactScope::Personal:
        return TEXT("個人");
    case EConsequenceImpactScope::Local:
        return TEXT("本地");
    case EConsequenceImpactScope::Regional:
        return TEXT("區域");
    case EConsequenceImpactScope::National:
        return TEXT("全國");
    case EConsequenceImpactScope::International:
        return TEXT("國際");
    case EConsequenceImpactScope::Global:
        return TEXT("全球");
    default:
        return TEXT("未知範圍");
    }
}

FString UMingDecisionBlueprintLibrary::GetSeverityName(EConsequenceSeverity Severity)
{
    switch (Severity)
    {
    case EConsequenceSeverity::Trivial:
        return TEXT("微不足道");
    case EConsequenceSeverity::Minor:
        return TEXT("輕微");
    case EConsequenceSeverity::Moderate:
        return TEXT("中等");
    case EConsequenceSeverity::Major:
        return TEXT("重大");
    case EConsequenceSeverity::Critical:
        return TEXT("關鍵");
    case EConsequenceSeverity::Catastrophic:
        return TEXT("災難性");
    default:
        return TEXT("未知嚴重程度");
    }
}

FString UMingDecisionBlueprintLibrary::GetDurationName(EConsequenceDuration Duration)
{
    switch (Duration)
    {
    case EConsequenceDuration::Instant:
        return TEXT("瞬時");
    case EConsequenceDuration::Short:
        return TEXT("短期");
    case EConsequenceDuration::Medium:
        return TEXT("中期");
    case EConsequenceDuration::Long:
        return TEXT("長期");
    case EConsequenceDuration::Permanent:
        return TEXT("永久");
    case EConsequenceDuration::Indefinite:
        return TEXT("無限期");
    default:
        return TEXT("未知持續時間");
    }
}

FString UMingDecisionBlueprintLibrary::GetImpactTypeName(EConsequenceImpactType ImpactType)
{
    switch (ImpactType)
    {
    case EConsequenceImpactType::Positive:
        return TEXT("正面");
    case EConsequenceImpactType::Negative:
        return TEXT("負面");
    case EConsequenceImpactType::Mixed:
        return TEXT("混合");
    case EConsequenceImpactType::Neutral:
        return TEXT("中性");
    case EConsequenceImpactType::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知影響類型");
    }
}

FDecisionContext UMingDecisionBlueprintLibrary::CreateDecisionContext(
    const FString& DecisionID,
    const FString& DecisionType,
    const FString& DecisionOption,
    const FString& DecisionMakerID,
    const FString& RepublicEra,
    int32 CurrentYear,
    const FString& PlayerFaction,
    int32 DifficultyLevel)
{
    FDecisionContext Context;
    Context.DecisionID = DecisionID;
    Context.DecisionTime = FPlatformTime::Seconds();
    Context.DecisionType = DecisionType;
    Context.DecisionOption = DecisionOption;
    Context.DecisionMakerID = DecisionMakerID;
    Context.RepublicEra = RepublicEra;
    Context.CurrentYear = CurrentYear;
    Context.PlayerFaction = PlayerFaction;
    Context.DifficultyLevel = DifficultyLevel;
    
    return Context;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetGameState(const FDecisionContext& Context, const TMap<FString, FString>& GameState)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.CurrentGameState = GameState;
    return ModifiedContext;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetCharacterStates(const FDecisionContext& Context, const TMap<FString, FString>& CharacterStates)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.CharacterStates = CharacterStates;
    return ModifiedContext;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetEventStates(const FDecisionContext& Context, const TMap<FString, FString>& EventStates)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.EventStates = EventStates;
    return ModifiedContext;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetResourceStates(const FDecisionContext& Context, const TMap<FString, int32>& ResourceStates)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.ResourceStates = ResourceStates;
    return ModifiedContext;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetBuildingStates(const FDecisionContext& Context, const TMap<FString, FString>& BuildingStates)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.BuildingStates = BuildingStates;
    return ModifiedContext;
}

FDecisionContext UMingDecisionBlueprintLibrary::SetUnitStates(const FDecisionContext& Context, const TMap<FString, FString>& UnitStates)
{
    FDecisionContext ModifiedContext = Context;
    ModifiedContext.UnitStates = UnitStates;
    return ModifiedContext;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::CreateDecisionConsequence(
    const FString& ConsequenceID,
    const FString& ConsequenceName,
    const FString& ConsequenceDescription,
    EDecisionConsequenceType ConsequenceType,
    EConsequenceImpactScope ImpactScope,
    EConsequenceSeverity Severity,
    EConsequenceDuration Duration,
    EConsequenceImpactType ImpactType,
    const FString& RelatedDecisionID,
    float Probability,
    float DelayTime,
    bool bIsReversible)
{
    FDecisionConsequence Consequence;
    Consequence.ConsequenceID = ConsequenceID;
    Consequence.ConsequenceName = ConsequenceName;
    Consequence.ConsequenceDescription = ConsequenceDescription;
    Consequence.ConsequenceType = ConsequenceType;
    Consequence.ImpactScope = ImpactScope;
    Consequence.Severity = Severity;
    Consequence.Duration = Duration;
    Consequence.ImpactType = ImpactType;
    Consequence.RelatedDecisionID = RelatedDecisionID;
    Consequence.Probability = FMath::Clamp(Probability, 0.0f, 1.0f);
    Consequence.DelayTime = FMath::Max(0.0f, DelayTime);
    Consequence.bIsReversible = bIsReversible;
    Consequence.bIsTriggered = false;
    Consequence.TriggerTime = 0.0f;
    
    return Consequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddConsequenceEffect(const FDecisionConsequence& Consequence, const FString& Effect)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.Effects.Add(Effect);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddConsequenceParameter(
    const FDecisionConsequence& Consequence,
    const FString& ParameterName,
    const FString& ParameterValue,
    const FString& ParameterType,
    bool bIsRequired,
    const FString& DefaultValue,
    const FString& Description)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    
    FConsequenceParameter Parameter;
    Parameter.ParameterName = ParameterName;
    Parameter.ParameterValue = ParameterValue;
    Parameter.ParameterType = ParameterType;
    Parameter.bIsRequired = bIsRequired;
    Parameter.DefaultValue = DefaultValue;
    Parameter.Description = Description;
    
    ModifiedConsequence.Parameters.Add(Parameter);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddTriggerCondition(const FDecisionConsequence& Consequence, const FString& Condition)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.TriggerConditions.Add(Condition);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddRelatedCharacter(const FDecisionConsequence& Consequence, const FString& CharacterID)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.RelatedCharacterIDs.Add(CharacterID);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddRelatedEvent(const FDecisionConsequence& Consequence, const FString& EventID)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.RelatedEventIDs.Add(EventID);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddReversalCondition(const FDecisionConsequence& Consequence, const FString& Condition)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.ReversalConditions.Add(Condition);
    return ModifiedConsequence;
}

FDecisionConsequence UMingDecisionBlueprintLibrary::AddConsequenceTag(const FDecisionConsequence& Consequence, const FString& Tag)
{
    FDecisionConsequence ModifiedConsequence = Consequence;
    ModifiedConsequence.ConsequenceTags.Add(Tag);
    return ModifiedConsequence;
}

TArray<FConsequenceCalculationResult> UMingDecisionBlueprintLibrary::BatchCalculateConsequences(const TArray<FDecisionContext>& Contexts)
{
    TArray<FConsequenceCalculationResult> Results;
    
    for (const FDecisionContext& Context : Contexts)
    {
        Results.Add(CalculateDecisionConsequences(Context));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch calculated %d decision consequences"), Results.Num());
    return Results;
}

TArray<bool> UMingDecisionBlueprintLibrary::BatchTriggerConsequences(const TArray<FString>& ConsequenceIDs, const FDecisionContext& Context)
{
    TArray<bool> Results;
    
    for (const FString& ConsequenceID : ConsequenceIDs)
    {
        Results.Add(TriggerConsequence(ConsequenceID, Context));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch triggered %d consequences"), Results.Num());
    return Results;
}

bool UMingDecisionBlueprintLibrary::IsDecisionConsequenceCalculatorInitialized()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    return Calculator != nullptr;
}

FString UMingDecisionBlueprintLibrary::GetDecisionConsequenceCalculatorVersion()
{
    return TEXT("1.0.0");
}

int32 UMingDecisionBlueprintLibrary::GetRegisteredConsequenceCount()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return 0;
    }
    
    return Calculator->GetAllDecisionConsequences().Num();
}

int32 UMingDecisionBlueprintLibrary::GetTriggeredConsequenceCount()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return 0;
    }
    
    TArray<FDecisionConsequence> AllConsequences = Calculator->GetAllDecisionConsequences();
    int32 TriggeredCount = 0;
    
    for (const FDecisionConsequence& Consequence : AllConsequences)
    {
        if (Consequence.bIsTriggered)
        {
            TriggeredCount++;
        }
    }
    
    return TriggeredCount;
}

FString UMingDecisionBlueprintLibrary::GetConsequenceDescription(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    return Consequence.ConsequenceDescription;
}

bool UMingDecisionBlueprintLibrary::IsConsequenceTriggered(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    return Consequence.bIsTriggered;
}

bool UMingDecisionBlueprintLibrary::IsConsequenceReversible(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    return Consequence.bIsReversible;
}

float UMingDecisionBlueprintLibrary::GetConsequenceProbability(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    return Consequence.Probability;
}

float UMingDecisionBlueprintLibrary::GetConsequenceDelay(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    return Consequence.DelayTime;
}

float UMingDecisionBlueprintLibrary::GetConsequenceImpactScore(const FString& ConsequenceID, const FDecisionContext& Context)
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (!Calculator)
    {
        return 0.0f;
    }
    
    FDecisionConsequence Consequence = Calculator->GetDecisionConsequence(ConsequenceID);
    
    // 簡化實作：基於嚴重程度和影響範圍計算
    float Impact = 0.0f;
    
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
    
    return FMath::Clamp(Impact, 0.0f, 1.0f);
}

float UMingDecisionBlueprintLibrary::GetDecisionOverallImpact(const FString& DecisionID)
{
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    float OverallImpact = 0.0f;
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Positive)
        {
            OverallImpact += 0.5f;
        }
        else if (Consequence.ImpactType == EConsequenceImpactType::Negative)
        {
            OverallImpact -= 0.5f;
        }
        else if (Consequence.ImpactType == EConsequenceImpactType::Mixed)
        {
            OverallImpact += 0.0f;
        }
    }
    
    return FMath::Clamp(OverallImpact, -1.0f, 1.0f);
}

float UMingDecisionBlueprintLibrary::GetDecisionPositiveImpact(const FString& DecisionID)
{
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    float PositiveImpact = 0.0f;
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Positive)
        {
            PositiveImpact += 1.0f;
        }
        else if (Consequence.ImpactType == EConsequenceImpactType::Mixed)
        {
            PositiveImpact += 0.5f;
        }
    }
    
    return PositiveImpact;
}

float UMingDecisionBlueprintLibrary::GetDecisionNegativeImpact(const FString& DecisionID)
{
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    float NegativeImpact = 0.0f;
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Negative)
        {
            NegativeImpact += 1.0f;
        }
        else if (Consequence.ImpactType == EConsequenceImpactType::Mixed)
        {
            NegativeImpact += 0.5f;
        }
    }
    
    return NegativeImpact;
}

float UMingDecisionBlueprintLibrary::GetDecisionRiskAssessment(const FString& DecisionID)
{
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    float RiskScore = 0.0f;
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        // 基於嚴重程度計算風險
        switch (Consequence.Severity)
        {
        case EConsequenceSeverity::Trivial:
            RiskScore += 0.1f;
            break;
        case EConsequenceSeverity::Minor:
            RiskScore += 0.2f;
            break;
        case EConsequenceSeverity::Moderate:
            RiskScore += 0.4f;
            break;
        case EConsequenceSeverity::Major:
            RiskScore += 0.6f;
            break;
        case EConsequenceSeverity::Critical:
            RiskScore += 0.8f;
            break;
        case EConsequenceSeverity::Catastrophic:
            RiskScore += 1.0f;
            break;
        }
        
        // 負面影響增加風險
        if (Consequence.ImpactType == EConsequenceImpactType::Negative)
        {
            RiskScore *= 1.5f;
        }
        else if (Consequence.ImpactType == EConsequenceImpactType::Mixed)
        {
            RiskScore *= 1.2f;
        }
    }
    
    return FMath::Clamp(RiskScore / Consequences.Num(), 0.0f, 1.0f);
}

TArray<FString> UMingDecisionBlueprintLibrary::GetDecisionRecommendations(const FString& DecisionID)
{
    TArray<FString> Recommendations;
    
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Positive)
        {
            FString Recommendation = FString::Printf(TEXT("建議: %s - %s"), 
                *Consequence.ConsequenceName, *Consequence.ConsequenceDescription);
            Recommendations.Add(Recommendation);
        }
    }
    
    return Recommendations;
}

TArray<FString> UMingDecisionBlueprintLibrary::GetDecisionWarnings(const FString& DecisionID)
{
    TArray<FString> Warnings;
    
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Negative)
        {
            FString Warning = FString::Printf(TEXT("警告: %s - %s"), 
                *Consequence.ConsequenceName, *Consequence.ConsequenceDescription);
            Warnings.Add(Warning);
        }
    }
    
    return Warnings;
}

TArray<FString> UMingDecisionBlueprintLibrary::GetDecisionOpportunities(const FString& DecisionID)
{
    TArray<FString> Opportunities;
    
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        if (Consequence.ImpactType == EConsequenceImpactType::Positive && Consequence.Severity >= EConsequenceSeverity::Major)
        {
            FString Opportunity = FString::Printf(TEXT("機會: %s - %s"), 
                *Consequence.ConsequenceName, *Consequence.ConsequenceDescription);
            Opportunities.Add(Opportunity);
        }
    }
    
    return Opportunities;
}

TArray<FString> UMingDecisionBlueprintLibrary::GetDecisionAlternatives(const FString& DecisionID)
{
    TArray<FString> Alternatives;
    
    // 簡化實作：基於決策類型提供替代方案
    if (DecisionID.Contains(TEXT("military")))
    {
        Alternatives.Add(TEXT("外交解決方案"));
        Alternatives.Add(TEXT("經濟制裁方案"));
        Alternatives.Add(TEXT("政治談判方案"));
    }
    else if (DecisionID.Contains(TEXT("economic")))
    {
        Alternatives.Add(TEXT("軍事威懾方案"));
        Alternatives.Add(TEXT("外交協商方案"));
        Alternatives.Add(TEXT("社會改革方案"));
    }
    else if (DecisionID.Contains(TEXT("political")))
    {
        Alternatives.Add(TEXT("經濟誘導方案"));
        Alternatives.Add(TEXT("社會運動方案"));
        Alternatives.Add(TEXT("文化宣傳方案"));
    }
    
    return Alternatives;
}

TArray<FConsequenceCalculationResult> UMingDecisionBlueprintLibrary::GetDecisionHistory(const FString& DecisionID)
{
    // 簡化實作：返回空數組，實際應該從決策系統獲取
    return TArray<FConsequenceCalculationResult>();
}

TArray<float> UMingDecisionBlueprintLibrary::GetDecisionTimeline(const FString& DecisionID)
{
    TArray<float> Timeline;
    
    // 簡化實作：返回模擬時間線
    Timeline.Add(0.0f); // 決策時間
    Timeline.Add(3600.0f); // 1小時後
    Timeline.Add(7200.0f); // 2小時後
    Timeline.Add(86400.0f); // 1天後
    Timeline.Add(604800.0f); // 1週後
    Timeline.Add(2592000.0f); // 1月後
    
    return Timeline;
}

TMap<FString, TArray<FString>> UMingDecisionBlueprintLibrary::GetDecisionImpactNetwork(const FString& DecisionID)
{
    TMap<FString, TArray<FString>> Network;
    
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        TArray<FString> AffectedItems;
        
        // 添加相關人物
        for (const FString& CharacterID : Consequence.RelatedCharacterIDs)
        {
            AffectedItems.Add(CharacterID);
        }
        
        // 添加相關事件
        for (const FString& EventID : Consequence.RelatedEventIDs)
        {
            AffectedItems.Add(EventID);
        }
        
        Network.Add(Consequence.ConsequenceID, AffectedItems);
    }
    
    return Network;
}

TArray<FString> UMingDecisionBlueprintLibrary::GetDecisionDependencies(const FString& DecisionID)
{
    TArray<FString> Dependencies;
    
    // 簡化實作：基於決策類型推斷依賴關係
    if (DecisionID.Contains(TEXT("revolution")))
    {
        Dependencies.Add(TEXT("political_unrest"));
        Dependencies.Add(TEXT("economic_crisis"));
        Dependencies.Add(TEXT("social_discontent"));
    }
    else if (DecisionID.Contains(TEXT("reform")))
    {
        Dependencies.Add(TEXT("public_support"));
        Dependencies.Add(TEXT("political_stability"));
        Dependencies.Add(TEXT("economic_resources"));
    }
    
    return Dependencies;
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequenceChain(const FString& ConsequenceID)
{
    TArray<FDecisionConsequence> Chain;
    
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    if (!Consequence.ConsequenceID.IsEmpty())
    {
        Chain.Add(Consequence);
    }
    
    // 簡化實作：基於後果類型推斷鏈條
    if (Consequence.ConsequenceType == EDecisionConsequenceType::Conditional)
    {
        // 添加相關條件後果
        TArray<FDecisionConsequence> ConditionalConsequences = GetConsequencesByType(EDecisionConsequenceType::Conditional);
        for (const FDecisionConsequence& ConditionalConsequence : ConditionalConsequences)
        {
            if (ConditionalConsequence.RelatedDecisionID == Consequence.RelatedDecisionID)
            {
                Chain.Add(ConditionalConsequence);
            }
        }
    }
    
    return Chain;
}

TArray<FDecisionConsequence> UMingDecisionBlueprintLibrary::GetConsequenceBranches(const FString& ConsequenceID)
{
    TArray<FDecisionConsequence> Branches;
    
    // 簡化實作：基於後果類型推斷分支
    TArray<FDecisionConsequence> RandomConsequences = GetConsequencesByType(EDecisionConsequenceType::Random);
    for (const FDecisionConsequence& RandomConsequence : RandomConsequences)
    {
        if (RandomConsequence.RelatedDecisionID == GetDecisionConsequence(ConsequenceID).RelatedDecisionID)
        {
            Branches.Add(RandomConsequence);
        }
    }
    
    return Branches;
}

bool UMingDecisionBlueprintLibrary::ExportDecisionData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Decision data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingDecisionBlueprintLibrary::ImportDecisionData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Decision data would be imported from: %s"), *FilePath);
    return true;
}

FString UMingDecisionBlueprintLibrary::GenerateDecisionReport(const FString& DecisionID)
{
    TArray<FDecisionConsequence> Consequences = GetConsequencesForDecision(DecisionID);
    
    FString Report = TEXT("=== 決策報告 ===\n");
    Report += FString::Printf(TEXT("決策ID: %s\n"), *DecisionID);
    Report += FString::Printf(TEXT("後果數量: %d\n\n"), Consequences.Num());
    
    Report += TEXT("=== 後果詳情 ===\n");
    for (const FDecisionConsequence& Consequence : Consequences)
    {
        Report += FString::Printf(TEXT("- %s (%s)\n"), *Consequence.ConsequenceName, *GetConsequenceTypeName(Consequence.ConsequenceType));
        Report += FString::Printf(TEXT("  %s\n"), *Consequence.ConsequenceDescription);
        Report += FString::Printf(TEXT("  嚴重程度: %s\n"), *GetSeverityName(Consequence.Severity));
        Report += FString::Printf(TEXT("  影響範圍: %s\n"), *GetImpactScopeName(Consequence.ImpactScope));
        Report += FString::Printf(TEXT("  影響類型: %s\n"), *GetImpactTypeName(Consequence.ImpactType));
        Report += FString::Printf(TEXT("  發生概率: %.1f%%\n"), Consequence.Probability * 100.0f);
        Report += FString::Printf(TEXT("  延迟時間: %.1f秒\n"), Consequence.DelayTime);
        Report += FString::Printf(TEXT("  是否可逆轉: %s\n"), Consequence.bIsReversible ? TEXT("是") : TEXT("否"));
        Report += FString::Printf(TEXT("  效果數量: %d\n"), Consequence.Effects.Num());
        Report += TEXT("\n");
    }
    
    return Report;
}

FString UMingDecisionBlueprintLibrary::GenerateConsequenceReport(const FString& ConsequenceID)
{
    FDecisionConsequence Consequence = GetDecisionConsequence(ConsequenceID);
    
    FString Report = TEXT("=== 後果報告 ===\n");
    Report += FString::Printf(TEXT("後果ID: %s\n"), *Consequence.ConsequenceID);
    Report += FString::Printf(TEXT("後果名稱: %s\n"), *Consequence.ConsequenceName);
    Report += FString::Printf(TEXT("後果描述: %s\n"), *Consequence.ConsequenceDescription);
    Report += FString::Printf(TEXT("後果類型: %s\n"), *GetConsequenceTypeName(Consequence.ConsequenceType));
    Report += FString::Printf(TEXT("影響範圍: %s\n"), *GetImpactScopeName(Consequence.ImpactScope));
    Report += FString::Printf(TEXT("嚴重程度: %s\n"), *GetSeverityName(Consequence.Severity));
    Report += FString::Printf(TEXT("持續時間: %s\n"), *GetDurationName(Consequence.Duration));
    Report += FString::Printf(TEXT("影響類型: %s\n"), *GetImpactTypeName(Consequence.ImpactType));
    Report += FString::Printf(TEXT("相關決策: %s\n"), *Consequence.RelatedDecisionID);
    Report += FString::Printf(TEXT("發生概率: %.1f%%\n"), Consequence.Probability * 100.0f);
    Report += FString::Printf(TEXT("延迟時間: %.1f秒\n"), Consequence.DelayTime);
    Report += FString::Printf(TEXT("冷卻時間: %.1f秒\n"), Consequence.CooldownTime);
    Report += FString::Printf(TEXT("是否已觸發: %s\n"), Consequence.bIsTriggered ? TEXT("是") : TEXT("否"));
    Report += FString::Printf(TEXT("是否可逆轉: %s\n"), Consequence.bIsReversible ? TEXT("是") : TEXT("否"));
    Report += FString::Printf(TEXT("觸發時間: %.1f\n"), Consequence.TriggerTime);
    Report += FString::Printf(TEXT("參數數量: %d\n"), Consequence.Parameters.Num());
    Report += FString::Printf(TEXT("觸發條件數量: %d\n"), Consequence.TriggerConditions.Num());
    Report += FString::Printf(TEXT("效果數量: %d\n"), Consequence.Effects.Num());
    Report += FString::Printf(TEXT("相關人物數量: %d\n"), Consequence.RelatedCharacterIDs.Num());
    Report += FString::Printf(TEXT("相關事件數量: %d\n"), Consequence.RelatedEventIDs.Num());
    Report += FString::Printf(TEXT("逆轉條件數量: %d\n"), Consequence.ReversalConditions.Num());
    Report += FString::Printf(TEXT("標籤數量: %d\n"), Consequence.ConsequenceTags.Num());
    
    return Report;
}

FString UMingDecisionBlueprintLibrary::GenerateStatisticsReport()
{
    TMap<FString, int32> Stats = GetConsequenceStatistics();
    
    FString Report = TEXT("=== 決策後果統計報告 ===\n");
    Report += FString::Printf(TEXT("註冊後果數量: %d\n"), GetRegisteredConsequenceCount());
    Report += FString::Printf(TEXT("已觸發後果數量: %d\n"), GetTriggeredConsequenceCount());
    
    Report += TEXT("\n=== 統計詳情 ===\n");
    for (const auto& Stat : Stats)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *Stat.Key, Stat.Value);
    }
    
    return Report;
}

FString UMingDecisionBlueprintLibrary::GenerateImpactAnalysisReport(const FString& DecisionID)
{
    TMap<FString, float> ImpactAnalysis = GetConsequenceImpactAnalysis(DecisionID);
    
    FString Report = TEXT("=== 影響分析報告 ===\n");
    Report += FString::Printf(TEXT("決策ID: %s\n"), *DecisionID);
    Report += FString::Printf(TEXT("分析項目數量: %d\n\n"), ImpactAnalysis.Num());
    
    Report += TEXT("=== 影響詳情 ===\n");
    for (const auto& Impact : ImpactAnalysis)
    {
        Report += FString::Printf(TEXT("- %s: %.2f\n"), *Impact.Key, Impact.Value);
    }
    
    return Report;
}

FString UMingDecisionBlueprintLibrary::GenerateRiskAssessmentReport(const FString& DecisionID)
{
    float RiskScore = GetDecisionRiskAssessment(DecisionID);
    
    FString Report = TEXT("=== 風險評估報告 ===\n");
    Report += FString::Printf(TEXT("決策ID: %s\n"), *DecisionID);
    Report += FString::Printf(TEXT("風險評分: %.2f\n"), RiskScore);
    
    if (RiskScore <= 0.2f)
    {
        Report += TEXT("風險等級: 低風險\n");
        Report += TEXT("建議: 可以執行\n");
    }
    else if (RiskScore <= 0.5f)
    {
        Report += TEXT("風險等級: 中等風險\n");
        Report += TEXT("建議: 謹慎考慮\n");
    }
    else if (RiskScore <= 0.8f)
    {
        Report += TEXT("風險等級: 高風險\n");
        Report += TEXT("建議: 需要準備應對措施\n");
    }
    else
    {
        Report += TEXT("風險等級: 極高風險\n");
        Report += TEXT("建議: 避免執行或尋找替代方案\n");
    }
    
    return Report;
}

void UMingDecisionBlueprintLibrary::ResetDecisionSystem()
{
    UMingDecisionConsequenceCalculator* Calculator = GetDecisionConsequenceCalculator();
    if (Calculator)
    {
        Calculator->Shutdown();
        Calculator->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Decision system reset"));
    }
}

void UMingDecisionBlueprintLibrary::ClearDecisionHistory()
{
    // 簡化實作：記錄清除操作
    UE_LOG(LogTemp, Log, TEXT("Decision history cleared"));
}

bool UMingDecisionBlueprintLibrary::BackupDecisionData(const FString& BackupPath)
{
    // 簡化實作：記錄備份操作
    UE_LOG(LogTemp, Log, TEXT("Decision data would be backed up to: %s"), *BackupPath);
    return true;
}

bool UMingDecisionBlueprintLibrary::RestoreDecisionData(const FString& BackupPath)
{
    // 簡化實作：記錄恢復操作
    UE_LOG(LogTemp, Log, TEXT("Decision data would be restored from: %s"), *BackupPath);
    return true;
}
