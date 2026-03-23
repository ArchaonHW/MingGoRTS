#include "MingSixConquestExecutor.h"
#include "MingSupremeSageCommandSystem.h"

UMingSixConquestExecutor::UMingSixConquestExecutor()
{
}

bool UMingSixConquestExecutor::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("六伐執行器初始化"));
    
    ExecutionHistory.Empty();
    ExecutionCounts.Empty();
    AverageEffectiveness.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("✓ 六伐執行器初始化完成"));
    
    return true;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerMind(
    const FConquestMindConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐心 - 奪其志 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("方法: %s"), *UEnum::GetDisplayValueAsText(Approach).ToString());
    
    FConquestResult Result;
    
    // 計算效果
    float Effectiveness = CalculateMindConquestEffect(Config, Target);
    
    // 根據方法調整
    if (Approach == EStrategyApproach::Reverse)
    {
        Effectiveness *= 1.2f;  // 逆道更快
        Result.MoralCost = 0.25f;
    }
    else
    {
        Result.MoralCost = 0.05f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    Result.ReputationImpact = Approach == EStrategyApproach::Reverse ? -0.1f : 0.05f;
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功動搖『%s』的意志，敵志已喪"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能有效動搖『%s』的意志"), *Target.TargetName);
    }
    
    // 副作用
    if (Config.bUseDeception && Approach == EStrategyApproach::Reverse)
    {
        Result.SideEffects.Add(TEXT("可能暴露欺騙手段"));
    }
    
    RecordExecution(ESixConquestStrategy::ConquerMind, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐心結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerSpirit(
    const FConquestSpiritConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐氣 - 挫其銳 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("士氣削弱目標: %.0f%%"), Config.MoraleReductionTarget * 100);
    
    FConquestResult Result;
    
    float Effectiveness = CalculateSpiritConquestEffect(Config, Target);
    
    if (Approach == EStrategyApproach::Reverse)
    {
        Result.MoralCost = 0.2f;
    }
    else
    {
        Result.MoralCost = 0.1f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    Result.ReputationImpact = 0.0f;
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功挫敗『%s』的銳氣，敵盛極而衰"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能有效挫敗『%s』的銳氣"), *Target.TargetName);
    }
    
    RecordExecution(ESixConquestStrategy::ConquerSpirit, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐氣結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerMomentum(
    const FConquestMomentumConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐勢 - 逆其時 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("動量干擾: %.0f%%"), Config.MomentumDisruptionLevel * 100);
    
    FConquestResult Result;
    
    float Effectiveness = CalculateMomentumConquestEffect(Config, Target);
    
    if (Approach == EStrategyApproach::Reverse)
    {
        Effectiveness *= 1.3f;  // 逆道在伐勢上特別有效
        Result.MoralCost = 0.2f;
    }
    else
    {
        Result.MoralCost = 0.1f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功逆轉『%s』的時勢，敵進退無門"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能有效逆轉『%s』的時勢"), *Target.TargetName);
    }
    
    if (Config.bReverseFlow)
    {
        Result.SideEffects.Add(TEXT("反轉的動量可能難以控制"));
    }
    
    RecordExecution(ESixConquestStrategy::ConquerMomentum, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐勢結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerLife(
    const FConquestLifeConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐命 - 斷其根 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("關鍵資源: %d個"), Config.CriticalResources.Num());
    
    FConquestResult Result;
    
    float Effectiveness = CalculateLifeConquestEffect(Config, Target);
    
    // 伐命通常成本較高
    if (Approach == EStrategyApproach::Reverse)
    {
        Effectiveness *= 1.2f;
        Result.MoralCost = 0.35f;
    }
    else
    {
        Result.MoralCost = 0.25f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功斷絕『%s』的根本，敵雖存而必亡"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能有效斷絕『%s』的根本"), *Target.TargetName);
    }
    
    Result.SideEffects.Add(TEXT("可能引發長期敵意"));
    
    RecordExecution(ESixConquestStrategy::ConquerLife, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐命結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerShadow(
    const FConquestShadowConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐影 - 亂其象 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("形象混淆: %.0f%%"), Config.ImageConfusionLevel * 100);
    
    FConquestResult Result;
    
    float Effectiveness = CalculateShadowConquestEffect(Config, Target);
    
    if (Approach == EStrategyApproach::Reverse)
    {
        Result.MoralCost = 0.3f;
    }
    else
    {
        Result.MoralCost = 0.15f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    Result.ReputationImpact = -0.1f;  // 伐影通常對聲譽有負面影響
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功混淆『%s』的形象，敵真偽不分"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能有效混淆『%s』的形象"), *Target.TargetName);
    }
    
    if (Config.bSpreadDisinformation)
    {
        Result.SideEffects.Add(TEXT"虛假信息可能被反噬"));
    }
    
    RecordExecution(ESixConquestStrategy::ConquerShadow, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐影結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquerNothing(
    const FConquestNothingConfig& Config,
    const FConquestTarget& Target,
    EStrategyApproach Approach)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行伐無 - 藏其跡 ==="));
    UE_LOG(LogTemp, Log, TEXT("目標: %s"), *Target.TargetName);
    UE_LOG(LogTemp, Log, TEXT("隱匿級別: %.0f%%"), Config.InvisibilityLevel * 100);
    
    FConquestResult Result;
    
    float Effectiveness = CalculateNothingConquestEffect(Config, Target);
    
    // 伐無通常道德成本較低（因為隱蔽）
    if (Approach == EStrategyApproach::Reverse)
    {
        Result.MoralCost = 0.15f;
    }
    else
    {
        Result.MoralCost = 0.1f;
    }
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.Effectiveness = Effectiveness;
    
    if (Result.bSuccess)
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("成功隱藏跡象於『%s』，敵攻擊無著"), *Target.TargetName);
    }
    else
    {
        Result.OutcomeDescription = FString::Printf(
            TEXT("未能完全隱藏跡象"));
    }
    
    if (Config.bSelfDestructMechanism)
    {
        Result.SideEffects.Add(TEXT("自毀機制可能誤傷"));
    }
    
    RecordExecution(ESixConquestStrategy::ConquerNothing, Result);
    
    UE_LOG(LogTemp, Log, TEXT("伐無結果: %s (效果: %.2f)"),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"),
        Result.Effectiveness);
    
    return Result;
}

FConquestResult UMingSixConquestExecutor::ExecuteConquest(
    ESixConquestStrategy Strategy,
    const FConquestExecutionPlan& Plan)
{
    // 根據策略類型調用對應執行方法
    FConquestResult Result;
    
    switch (Strategy)
    {
    case ESixConquestStrategy::ConquerMind:
        Result = ExecuteRighteousConquerMind(Plan.Target);
        break;
    case ESixConquestStrategy::ConquerSpirit:
        Result = ExecuteConquerSpirit(FConquestSpiritConfig(), Plan.Target, Plan.Approach);
        break;
    case ESixConquestStrategy::ConquerMomentum:
        Result = ExecuteConquerMomentum(FConquestMomentumConfig(), Plan.Target, Plan.Approach);
        break;
    case ESixConquestStrategy::ConquerLife:
        Result = ExecuteConquerLife(FConquestLifeConfig(), Plan.Target, Plan.Approach);
        break;
    case ESixConquestStrategy::ConquerShadow:
        Result = ExecuteConquerShadow(FConquestShadowConfig(), Plan.Target, Plan.Approach);
        break;
    case ESixConquestStrategy::ConquerNothing:
        Result = ExecuteConquerNothing(FConquestNothingConfig(), Plan.Target, Plan.Approach);
        break;
    }
    
    return Result;
}

FConquestExecutionPlan UMingSixConquestExecutor::GenerateMindConquestPlan(
    const FConquestTarget& Target,
    EStrategyApproach Approach) const
{
    FConquestExecutionPlan Plan;
    Plan.Strategy = ESixConquestStrategy::ConquerMind;
    Plan.Approach = Approach;
    Plan.Target = Target;
    Plan.ResourceRequirement = Target.StrategicValue * 100.0f;
    Plan.Duration = 5.0f;
    
    return Plan;
}

float UMingSixConquestExecutor::EvaluateStrategyFit(
    ESixConquestStrategy Strategy,
    const FConquestTarget& Target,
    const FSituationAssessment& Situation) const
{
    float Fit = 0.5f;
    
    // 根據目標弱點評估
    if (Target.WeaknessMap.Contains(TEXT("意志")))
    {
        if (Strategy == ESixConquestStrategy::ConquerMind)
        {
            Fit += 0.3f;
        }
    }
    if (Target.WeaknessMap.Contains(TEXT("士氣")))
    {
        if (Strategy == ESixConquestStrategy::ConquerSpirit)
        {
            Fit += 0.3f;
        }
    }
    if (Target.WeaknessMap.Contains(TEXT("動量")))
    {
        if (Strategy == ESixConquestStrategy::ConquerMomentum)
        {
            Fit += 0.3f;
        }
    }
    
    // 根據情境評估
    if (Situation.ChaosLevel > 0.6f)
    {
        if (Strategy == ESixConquestStrategy::ConquerMomentum)
        {
            Fit += 0.2f;
        }
    }
    
    return FMath::Clamp(Fit, 0.0f, 1.0f);
}

TArray<ESixConquestStrategy> UMingSixConquestExecutor::GetRecommendedStrategyOrder(
    const FConquestTarget& Target,
    const FSituationAssessment& Situation) const
{
    TArray<ESixConquestStrategy> Strategies;
    TArray<float> Scores;
    
    // 評估所有策略
    for (int32 i = 0; i < 6; i++)
    {
        ESixConquestStrategy Strategy = static_cast<ESixConquestStrategy>(i);
        float Score = EvaluateStrategyFit(Strategy, Target, Situation);
        
        // 插入排序
        int32 InsertIndex = 0;
        for (int32 j = 0; j < Scores.Num(); j++)
        {
            if (Score > Scores[j])
            {
                break;
            }
            InsertIndex++;
        }
        
        Strategies.Insert(Strategy, InsertIndex);
        Scores.Insert(Score, InsertIndex);
    }
    
    return Strategies;
}

FConquestExecutionPlan UMingSixConquestExecutor::CreateCombinedConquestPlan(
    const TArray<ESixConquestStrategy>& Strategies,
    const FConquestTarget& Target) const
{
    FConquestExecutionPlan Plan;
    Plan.Target = Target;
    
    // 組合多種伐術
    float TotalResource = 0.0f;
    float TotalDuration = 0.0f;
    
    for (const auto& Strategy : Strategies)
    {
        TotalResource += Target.StrategicValue * 50.0f;
        TotalDuration += 3.0f;
    }
    
    Plan.ResourceRequirement = TotalResource;
    Plan.Duration = TotalDuration;
    
    return Plan;
}

FConquestEffectivenessMetrics UMingSixConquestExecutor::EvaluateEffectiveness(
    const FConquestResult& Result,
    const FConquestExecutionPlan& Plan) const
{
    FConquestEffectivenessMetrics Metrics;
    
    Metrics.DirectDamage = Result.Effectiveness;
    Metrics.RippleEffect = Result.Effectiveness * 0.3f;
    Metrics.SustainabilityImpact = 1.0f - Result.MoralCost;
    Metrics.RecoveryDifficulty = Result.Effectiveness * 0.5f;
    Metrics.CostEfficiency = Result.bSuccess ? 
        Result.Effectiveness / FMath::Max(Plan.ResourceRequirement, 1.0f) : 0.0f;
    Metrics.SideEffectSeverity = Result.SideEffects.Num() * 0.1f;
    
    return Metrics;
}

float UMingSixConquestExecutor::CalculateMoralCost(
    ESixConquestStrategy Strategy,
    EStrategyApproach Approach,
    const FConquestTarget& Target) const
{
    float BaseCost = 0.0f;
    
    switch (Strategy)
    {
    case ESixConquestStrategy::ConquerMind:
        BaseCost = 0.15f;
        break;
    case ESixConquestStrategy::ConquerSpirit:
        BaseCost = 0.1f;
        break;
    case ESixConquestStrategy::ConquerMomentum:
        BaseCost = 0.2f;
        break;
    case ESixConquestStrategy::ConquerLife:
        BaseCost = 0.3f;
        break;
    case ESixConquestStrategy::ConquerShadow:
        BaseCost = 0.25f;
        break;
    case ESixConquestStrategy::ConquerNothing:
        BaseCost = 0.1f;
        break;
    }
    
    // 逆道增加成本
    if (Approach == EStrategyApproach::Reverse)
    {
        BaseCost *= 1.5f;
    }
    
    return FMath::Clamp(BaseCost, 0.0f, 1.0f);
}

TArray<FString> UMingSixConquestExecutor::PredictLongTermEffects(
    const FConquestResult& Result,
    int32 PredictionHorizon) const
{
    TArray<FString> Effects;
    
    if (Result.Effectiveness > 0.8f)
    {
        Effects.Add(TEXT("長期削弱敵方實力"));
    }
    if (Result.MoralCost > 0.3f)
    {
        Effects.Add(TEXT("可能引發道德爭議"));
    }
    if (Result.ReputationImpact < -0.1f)
    {
        Effects.Add(TEXT"聲譽可能持續受損"));
    }
    
    return Effects;
}

FConquestResult UMingSixConquestExecutor::ExecuteRighteousConquerMind(
    const FConquestTarget& Target)
{
    UE_LOG(LogTemp, Log, TEXT("執行正道伐心（立國）"));
    
    FConquestMindConfig Config;
    Config.bUsePropaganda = true;
    Config.bUseDeception = false;
    
    return ExecuteConquerMind(Config, Target, EStrategyApproach::Righteous);
}

FConquestResult UMingSixConquestExecutor::ExecuteReverseConquerMind(
    const FConquestTarget& Target)
{
    UE_LOG(LogTemp, Log, TEXT("執行逆道伐心（不破人）"));
    
    FConquestMindConfig Config;
    Config.bUsePropaganda = false;
    Config.bUseDeception = true;
    
    return ExecuteConquerMind(Config, Target, EStrategyApproach::Reverse);
}

FConquestResult UMingSixConquestExecutor::ExecuteRighteousConquerMomentum(
    const FConquestTarget& Target)
{
    UE_LOG(LogTemp, Log, TEXT("執行正道伐勢（立制）"));
    
    FConquestMomentumConfig Config;
    Config.bReverseFlow = false;
    Config.bCreateFalseMomentum = false;
    
    return ExecuteConquerMomentum(Config, Target, EStrategyApproach::Righteous);
}

FConquestResult UMingSixConquestExecutor::ExecuteReverseConquerMomentum(
    const FConquestTarget& Target)
{
    UE_LOG(LogTemp, Log, TEXT("執行逆道伐勢（破局）"));
    
    FConquestMomentumConfig Config;
    Config.bReverseFlow = true;
    Config.bCreateFalseMomentum = true;
    Config.MomentumDisruptionLevel = 0.9f;
    
    return ExecuteConquerMomentum(Config, Target, EStrategyApproach::Reverse);
}

float UMingSixConquestExecutor::CalculateMindConquestEffect(
    const FConquestMindConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = 0.6f;
    
    // 根據目標弱點調整
    if (Target.WeaknessMap.Contains(TEXT("意志")))
    {
        BaseEffect += Target.WeaknessMap[TEXT("意志")] * 0.3f;
    }
    
    // 根據配置調整
    if (Config.bUseDeception)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bUsePropaganda)
    {
        BaseEffect += 0.1f;
    }
    
    // 根據抵抗程度調整
    BaseEffect *= (1.0f - Target.ResistanceLevel * 0.5f);
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

float UMingSixConquestExecutor::CalculateSpiritConquestEffect(
    const FConquestSpiritConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = Config.MoraleReductionTarget;
    
    if (Config.bUseContinuousPressure)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bUseFalseHope)
    {
        BaseEffect += 0.15f;
    }
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

float UMingSixConquestExecutor::CalculateMomentumConquestEffect(
    const FConquestMomentumConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = Config.MomentumDisruptionLevel;
    
    if (Config.bReverseFlow)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bCreateFalseMomentum)
    {
        BaseEffect += 0.15f;
    }
    if (Config.AsymmetryLevel > 0.7f)
    {
        BaseEffect += 0.1f;
    }
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

float UMingSixConquestExecutor::CalculateLifeConquestEffect(
    const FConquestLifeConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = Config.SustainabilityReduction;
    
    if (Config.bCutReinforcements)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bIsolateFromAllies)
    {
        BaseEffect += 0.15f;
    }
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

float UMingSixConquestExecutor::CalculateShadowConquestEffect(
    const FConquestShadowConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = Config.ImageConfusionLevel;
    
    if (Config.bCreateFalseImage)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bSpreadDisinformation)
    {
        BaseEffect += 0.15f;
    }
    if (Config.bMimicEnemy)
    {
        BaseEffect += 0.1f;
    }
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

float UMingSixConquestExecutor::CalculateNothingConquestEffect(
    const FConquestNothingConfig& Config,
    const FConquestTarget& Target) const
{
    float BaseEffect = Config.InvisibilityLevel;
    
    if (Config.bLeaveNoTrace)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bUseDeniability)
    {
        BaseEffect += 0.1f;
    }
    if (Config.bSelfDestructMechanism)
    {
        BaseEffect += 0.05f;
    }
    
    return FMath::Clamp(BaseEffect, 0.0f, 1.0f);
}

void UMingSixConquestExecutor::RecordExecution(ESixConquestStrategy Strategy, 
    const FConquestResult& Result)
{
    ExecutionHistory.Add(Result);
    
    int32& Count = ExecutionCounts.FindOrAdd(Strategy);
    Count++;
    
    float& AvgEffect = AverageEffectiveness.FindOrAdd(Strategy);
    AvgEffect = (AvgEffect * (Count - 1) + Result.Effectiveness) / Count;
}
