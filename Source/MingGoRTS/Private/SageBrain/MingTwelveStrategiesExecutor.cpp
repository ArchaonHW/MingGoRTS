#include "MingTwelveStrategiesExecutor.h"
#include "MingSupremeSageCommandSystem.h"

UMingTwelveStrategiesExecutor::UMingTwelveStrategiesExecutor()
    : RighteousStrategyCount(0)
    , ReverseStrategyCount(0)
{
}

bool UMingTwelveStrategiesExecutor::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("十二策執行器初始化"));
    
    ExecutionHistory.Empty();
    StrategyUsageCounts.Empty();
    AverageSuccessRates.Empty();
    RighteousStrategyCount = 0;
    ReverseStrategyCount = 0;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 十二策執行器初始化完成"));
    
    return true;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteEstablishNation(
    const FEstablishNationConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行立國（伐心正道） ==="));
    UE_LOG(LogTemp, Log, TEXT("國家名稱: %s"), *Config.NationName);
    UE_LOG(LogTemp, Log, TEXT("正當性目標: %.0f%%"), Config.LegitimacyTarget * 100);
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::EstablishNation;
    
    float Effectiveness = CalculateEstablishNationEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.6f;
    Result.StrategicGain = Effectiveness;
    Result.RiskLevel = 0.2f;
    Result.ResultDescription = FString::Printf(
        TEXT("立國『%s』%s，正當性達到%.0f%%"),
        *Config.NationName,
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Effectiveness * 100);
    
    RighteousStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::EstablishNation, Result.bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("立國結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteEstablishSystem(
    const FEstablishSystemConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行立制（伐勢伐命正道） ==="));
    UE_LOG(LogTemp, Log, TEXT("規則數量: %d"), Config.RulesToEstablish.Num());
    UE_LOG(LogTemp, Log, TEXT("穩定性目標: %.0f%%"), Config.SystemStabilityTarget * 100);
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::EstablishSystem;
    
    float Effectiveness = CalculateEstablishSystemEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.6f;
    Result.StrategicGain = Effectiveness;
    Result.RiskLevel = 0.25f;
    Result.ResultDescription = FString::Printf(
        TEXT("立制%s，建立%d條規則，系統穩定性%.0f%%"),
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Config.RulesToEstablish.Num(),
        Effectiveness * 100);
    
    RighteousStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::EstablishSystem, Result.bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("立制結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteEstablishPeople(
    const FEstablishPeopleConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行立人（伐氣伐影伐無正道） ==="));
    UE_LOG(LogTemp, Log, TEXT("關鍵位置: %d"), Config.KeyPositions.Num());
    UE_LOG(LogTemp, Log, TEXT("人才品質目標: %.0f%%"), Config.PeopleQualityTarget * 100);
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::EstablishPeople;
    
    float Effectiveness = CalculateEstablishPeopleEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.6f;
    Result.StrategicGain = Effectiveness;
    Result.RiskLevel = 0.15f;
    Result.ResultDescription = FString::Printf(
        TEXT("立人%s，建立%d個關鍵位置的人才體系"),
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Config.KeyPositions.Num());
    
    RighteousStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::EstablishPeople, Result.bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("立人結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteBreakSituation(
    const FBreakSituationConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行破局（伐勢伐氣逆道） ==="));
    UE_LOG(LogTemp, Log, TEXT("目標局勢: %s"), *Config.TargetSituation);
    UE_LOG(LogTemp, Log, TEXT("使用突襲: %s"), Config.bUseSurprise ? TEXT("是") : TEXT("否"));
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::BreakSituation;
    
    float Effectiveness = CalculateBreakSituationEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.StrategicGain = Effectiveness * 1.2f;  // 逆道增益
    Result.RiskLevel = 0.5f;
    Result.ResultDescription = FString::Printf(
        TEXT("破局%s，局勢轉化效果%.0f%%"),
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Effectiveness * 100);
    
    ReverseStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::BreakSituation, Result.bSuccess);
    
    // 檢查逆道使用頻率
    if (ReverseStrategyCount > 5)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 逆道使用過多，注意防墮"));
    }
    
    UE_LOG(LogTemp, Log, TEXT("破局結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteBreakStructure(
    const FBreakStructureConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行破結構（伐命伐影逆道） ==="));
    UE_LOG(LogTemp, Log, TEXT("目標結構: %s"), *Config.TargetStructure);
    UE_LOG(LogTemp, Log, TEXT("破壞目標: %.0f%%"), Config.StructuralDisruptionTarget * 100);
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::BreakStructure;
    
    float Effectiveness = CalculateBreakStructureEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.StrategicGain = Effectiveness * 1.1f;
    Result.RiskLevel = 0.6f;
    Result.ResultDescription = FString::Printf(
        TEXT("破結構%s，結構破壞程度%.0f%%"),
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Effectiveness * 100);
    
    ReverseStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::BreakStructure, Result.bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("破結構結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteBreakNotPerson(
    const FBreakNotPersonConfig& Config,
    const FStrategyExecutionContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行不破人（伐心伐無逆道） ==="));
    UE_LOG(LogTemp, Log, TEXT("保留形式: %s"), Config.bLeaveFormRemoveSubstance ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, Log, TEXT("保護率: %.0f%%"), Config.PreservationRate * 100);
    
    FStrategyResult Result;
    Result.ExecutedStrategy = ETwelveStrategies::BreakNotPerson;
    
    float Effectiveness = CalculateBreakNotPersonEffect(Config, Context);
    
    Result.bSuccess = Effectiveness > 0.5f;
    Result.StrategicGain = Effectiveness;
    Result.RiskLevel = 0.4f;
    Result.ResultDescription = FString::Printf(
        TEXT("不破人執行%s，保留%d個目標的形式"),
        Result.bSuccess ? TEXT("成功") : TEXT("未達預期"),
        Config.TargetsToPreserve.Num());
    
    ReverseStrategyCount++;
    UpdateUsageStatistics(ETwelveStrategies::BreakNotPerson, Result.bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("不破人結果: %s"), *Result.ResultDescription);
    
    return Result;
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteStrategy(
    ETwelveStrategies Strategy,
    const FStrategyContext& Context)
{
    FStrategyExecutionContext ExecContext;
    ExecContext.Strategy = Strategy;
    ExecContext.CurrentPhase = Context.CurrentPhase;
    ExecContext.UrgencyLevel = Context.UrgencyLevel;
    
    switch (Strategy)
    {
    case ETwelveStrategies::EstablishNation:
    {
        FEstablishNationConfig Config;
        Config.NationName = Context.StrategyName;
        return ExecuteEstablishNation(Config, ExecContext);
    }
    case ETwelveStrategies::EstablishSystem:
    {
        FEstablishSystemConfig Config;
        return ExecuteEstablishSystem(Config, ExecContext);
    }
    case ETwelveStrategies::EstablishPeople:
    {
        FEstablishPeopleConfig Config;
        return ExecuteEstablishPeople(Config, ExecContext);
    }
    case ETwelveStrategies::BreakSituation:
    {
        FBreakSituationConfig Config;
        return ExecuteBreakSituation(Config, ExecContext);
    }
    case ETwelveStrategies::BreakStructure:
    {
        FBreakStructureConfig Config;
        return ExecuteBreakStructure(Config, ExecContext);
    }
    case ETwelveStrategies::BreakNotPerson:
    {
        FBreakNotPersonConfig Config;
        return ExecuteBreakNotPerson(Config, ExecContext);
    }
    default:
        FStrategyResult Result;
        Result.bSuccess = false;
        Result.ResultDescription = TEXT("未知策略");
        return Result;
    }
}

FStrategyResult UMingTwelveStrategiesExecutor::ExecuteAutoSelectedStrategy(
    const FStrategyExecutionContext& Context)
{
    // 自動選擇最適合的策略
    TArray<FStrategyCompatibility> Rankings = RankStrategiesByCompatibility(Context);
    
    if (Rankings.Num() > 0)
    {
        ETwelveStrategies BestStrategy = Rankings[0].Strategy;
        FStrategyContext StrategyContext;
        StrategyContext.CurrentPhase = Context.CurrentPhase;
        StrategyContext.UrgencyLevel = Context.UrgencyLevel;
        
        return ExecuteStrategy(BestStrategy, StrategyContext);
    }
    
    // 默認執行立國
    FStrategyContext DefaultContext;
    return ExecuteStrategy(ETwelveStrategies::EstablishNation, DefaultContext);
}

FStrategyCompatibility UMingTwelveStrategiesExecutor::EvaluateStrategyCompatibility(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    FStrategyCompatibility Compatibility;
    Compatibility.Strategy = Strategy;
    
    // 階段相容性
    switch (Context.CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        if (IsRighteousStrategy(Strategy))
        {
            Compatibility.PhaseCompatibility = 0.9f;
        }
        else
        {
            Compatibility.PhaseCompatibility = 0.3f;
        }
        break;
    case EFiveElementPhase::Fire:
        if (IsRighteousStrategy(Strategy))
        {
            Compatibility.PhaseCompatibility = 0.8f;
        }
        else
        {
            Compatibility.PhaseCompatibility = 0.7f;
        }
        break;
    case EFiveElementPhase::Earth:
        Compatibility.PhaseCompatibility = IsRighteousStrategy(Strategy) ? 0.9f : 0.2f;
        break;
    case EFiveElementPhase::Metal:
        Compatibility.PhaseCompatibility = IsReverseStrategy(Strategy) ? 0.8f : 0.4f;
        break;
    case EFiveElementPhase::Water:
        Compatibility.PhaseCompatibility = IsReverseStrategy(Strategy) ? 0.9f : 0.5f;
        break;
    default:
        Compatibility.PhaseCompatibility = 0.5f;
        break;
    }
    
    // 資源相容性
    Compatibility.ResourceCompatibility = Context.ResourceAvailability;
    
    // 風險相容性
    if (Context.RiskTolerance > 0.6f && IsReverseStrategy(Strategy))
    {
        Compatibility.RiskCompatibility = 0.9f;
    }
    else if (Context.RiskTolerance < 0.3f && IsRighteousStrategy(Strategy))
    {
        Compatibility.RiskCompatibility = 0.9f;
    }
    else
    {
        Compatibility.RiskCompatibility = 0.5f;
    }
    
    // 整體評分
    Compatibility.OverallScore = 
        Compatibility.PhaseCompatibility * 0.4f +
        Compatibility.ResourceCompatibility * 0.3f +
        Compatibility.RiskCompatibility * 0.3f;
    
    return Compatibility;
}

TArray<FStrategyCompatibility> UMingTwelveStrategiesExecutor::RankStrategiesByCompatibility(
    const FStrategyExecutionContext& Context) const
{
    TArray<FStrategyCompatibility> Rankings;
    
    // 評估所有6種策略
    for (int32 i = 0; i < 6; i++)
    {
        ETwelveStrategies Strategy = static_cast<ETwelveStrategies>(i);
        FStrategyCompatibility Compatibility = EvaluateStrategyCompatibility(Strategy, Context);
        
        // 插入排序
        int32 InsertIndex = 0;
        for (int32 j = 0; j < Rankings.Num(); j++)
        {
            if (Compatibility.OverallScore > Rankings[j].OverallScore)
            {
                break;
            }
            InsertIndex++;
        }
        
        Rankings.Insert(Compatibility, InsertIndex);
    }
    
    return Rankings;
}

TArray<ETwelveStrategies> UMingTwelveStrategiesExecutor::GetRecommendedStrategiesForPhase(
    EFiveElementPhase Phase,
    const FSituationAssessment& Assessment) const
{
    TArray<ETwelveStrategies> Recommendations;
    
    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        Recommendations.Add(ETwelveStrategies::EstablishNation);
        Recommendations.Add(ETwelveStrategies::EstablishPeople);
        break;
    case EFiveElementPhase::Fire:
        Recommendations.Add(ETwelveStrategies::EstablishNation);
        Recommendations.Add(ETwelveStrategies::BreakSituation);
        break;
    case EFiveElementPhase::Earth:
        Recommendations.Add(ETwelveStrategies::EstablishSystem);
        Recommendations.Add(ETwelveStrategies::EstablishPeople);
        break;
    case EFiveElementPhase::Metal:
        Recommendations.Add(ETwelveStrategies::BreakStructure);
        Recommendations.Add(ETwelveStrategies::BreakNotPerson);
        break;
    case EFiveElementPhase::Water:
        Recommendations.Add(ETwelveStrategies::BreakSituation);
        Recommendations.Add(ETwelveStrategies::BreakNotPerson);
        break;
    default:
        Recommendations.Add(ETwelveStrategies::EstablishNation);
        break;
    }
    
    return Recommendations;
}

bool UMingTwelveStrategiesExecutor::CheckStrategyPrerequisites(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    if (IsReverseStrategy(Strategy))
    {
        // 逆策需要特定條件
        return CheckReverseActivationConditions(Strategy, Context);
    }
    else
    {
        // 正策條件較寬鬆
        return CheckRighteousPrerequisites(Strategy, Context);
    }
}

FStrategyExecutionReport UMingTwelveStrategiesExecutor::PredictStrategyOutcome(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    FStrategyExecutionReport Report;
    Report.ExecutedStrategy = Strategy;
    
    FStrategyCompatibility Compatibility = EvaluateStrategyCompatibility(Strategy, Context);
    
    Report.bSuccess = Compatibility.OverallScore > 0.6f;
    Report.StrategicGain = Compatibility.OverallScore;
    Report.ResourceCost = 100.0f * (1.0f - Context.ResourceAvailability);
    Report.TimeCost = 5.0f / Compatibility.OverallScore;
    Report.MoralImpact = IsReverseStrategy(Strategy) ? -0.2f : 0.1f;
    Report.ReputationImpact = IsReverseStrategy(Strategy) ? -0.15f : 0.2f;
    
    return Report;
}

TArray<FStrategyResult> UMingTwelveStrategiesExecutor::ExecuteRighteousSequence(
    const FEstablishNationConfig& NationConfig,
    const FEstablishSystemConfig& SystemConfig,
    const FEstablishPeopleConfig& PeopleConfig)
{
    TArray<FStrategyResult> Results;
    
    UE_LOG(LogTemp, Log, TEXT("=== 執行正六策序列 ==="));
    UE_LOG(LogTemp, Log, TEXT("立國 → 立制 → 立人"));
    
    FStrategyExecutionContext Context;
    Context.CurrentPhase = EFiveElementPhase::Wood;
    
    // 立國
    Results.Add(ExecuteEstablishNation(NationConfig, Context));
    
    // 立制
    Context.CurrentPhase = EFiveElementPhase::Earth;
    Results.Add(ExecuteEstablishSystem(SystemConfig, Context));
    
    // 立人
    Results.Add(ExecuteEstablishPeople(PeopleConfig, Context));
    
    UE_LOG(LogTemp, Log, TEXT("✓ 正六策序列執行完成"));
    
    return Results;
}

TArray<FStrategyResult> UMingTwelveStrategiesExecutor::ExecuteReverseSequence(
    const FBreakSituationConfig& SituationConfig,
    const FBreakStructureConfig& StructureConfig,
    const FBreakNotPersonConfig& NotPersonConfig)
{
    TArray<FStrategyResult> Results;
    
    UE_LOG(LogTemp, Log, TEXT("=== 執行逆六策序列 ==="));
    UE_LOG(LogTemp, Log, TEXT("破局 → 破結構 → 不破人"));
    
    FStrategyExecutionContext Context;
    Context.CurrentPhase = EFiveElementPhase::Metal;
    Context.UrgencyLevel = 0.9f;
    Context.RiskTolerance = 0.8f;
    
    // 破局
    Results.Add(ExecuteBreakSituation(SituationConfig, Context));
    
    // 破結構
    Results.Add(ExecuteBreakStructure(StructureConfig, Context));
    
    // 不破人
    Results.Add(ExecuteBreakNotPerson(NotPersonConfig, Context));
    
    UE_LOG(LogTemp, Log, TEXT("✓ 逆六策序列執行完成"));
    
    // 檢查是否需要進入土階段封存
    if (ReverseStrategyCount >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 連續使用逆策，建議立即進入土階段封存"));
    }
    
    return Results;
}

TArray<FStrategyResult> UMingTwelveStrategiesExecutor::ExecuteMixedSequence(
    const TArray<ETwelveStrategies>& StrategySequence,
    const TArray<FStrategyContext>& Contexts)
{
    TArray<FStrategyResult> Results;
    
    UE_LOG(LogTemp, Log, TEXT("=== 執行正逆混合序列 ==="));
    UE_LOG(LogTemp, Log, TEXT("策略數: %d"), StrategySequence.Num());
    
    for (int32 i = 0; i < StrategySequence.Num() && i < Contexts.Num(); i++)
    {
        Results.Add(ExecuteStrategy(StrategySequence[i], Contexts[i]));
        
        // 檢查是否需要切換階段
        if (IsReverseStrategy(StrategySequence[i]))
        {
            UE_LOG(LogTemp, Log, TEXT("使用逆策，注意階段轉換"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ 正逆混合序列執行完成"));
    
    return Results;
}

float UMingTwelveStrategiesExecutor::CalculateStrategyRisk(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    float Risk = 0.3f;
    
    if (IsReverseStrategy(Strategy))
    {
        Risk += 0.3f;
    }
    
    // 根據階段調整
    if (Context.CurrentPhase == EFiveElementPhase::Earth && IsReverseStrategy(Strategy))
    {
        Risk += 0.2f;
    }
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::AssessBackfireRisk(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    float Risk = 0.2f;
    
    if (IsReverseStrategy(Strategy))
    {
        Risk += 0.3f;
        
        // 逆策過量使用增加反噬風險
        if (ReverseStrategyCount > 3)
        {
            Risk += 0.2f;
        }
    }
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

TArray<FString> UMingTwelveStrategiesExecutor::GetStrategyMoralBoundaries(ETwelveStrategies Strategy) const
{
    TArray<FString> Boundaries;
    
    if (IsReverseStrategy(Strategy))
    {
        Boundaries.Add(TEXT("不可殘民以逞"));
        Boundaries.Add(TEXT("不可絕敵後路而致玉石俱焚"));
        Boundaries.Add(TEXT("不可壞天地大義而失天時"));
        Boundaries.Add(TEXT("不可為私利而濫用"));
    }
    else
    {
        Boundaries.Add(TEXT("不可虛立而用"));
        Boundaries.Add(TEXT("不可偏立而用"));
        Boundaries.Add(TEXT("不可逆時而用"));
    }
    
    return Boundaries;
}

FStrategyExecutionReport UMingTwelveStrategiesExecutor::GenerateExecutionReport(
    const FStrategyResult& Result,
    const FStrategyExecutionContext& Context) const
{
    FStrategyExecutionReport Report;
    Report.ExecutedStrategy = Result.ExecutedStrategy;
    Report.bSuccess = Result.bSuccess;
    Report.StrategicGain = Result.StrategicGain;
    Report.ResourceCost = 100.0f;
    Report.TimeCost = 5.0f;
    Report.MoralImpact = IsReverseStrategy(Result.ExecutedStrategy) ? -0.2f : 0.1f;
    
    if (!Result.bSuccess)
    {
        Report.UnintendedConsequences.Add(TEXT("策略未達預期效果"));
    }
    
    if (IsReverseStrategy(Result.ExecutedStrategy))
    {
        Report.RecommendedFollowUps.Add(TEXT("考慮進入土階段封存"));
    }
    else
    {
        Report.RecommendedFollowUps.Add(TEXT("可繼續執行下一正策"));
    }
    
    return Report;
}

TArray<FStrategyExecutionReport> UMingTwelveStrategiesExecutor::GetExecutionHistory() const
{
    return ExecutionHistory;
}

TMap<ETwelveStrategies, float> UMingTwelveStrategiesExecutor::AnalyzeExecutionPatterns() const
{
    TMap<ETwelveStrategies, float> Patterns;
    
    for (const auto& Pair : StrategyUsageCounts)
    {
        ETwelveStrategies Strategy = Pair.Key;
        int32 Count = Pair.Value;
        
        float Rate = AverageSuccessRates.Contains(Strategy) ? 
            AverageSuccessRates[Strategy] : 0.5f;
        
        Patterns.Add(Strategy, Rate);
    }
    
    return Patterns;
}

float UMingTwelveStrategiesExecutor::CalculateEstablishNationEffect(
    const FEstablishNationConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = Config.LegitimacyTarget;
    
    if (Config.bUsePublicCampaign)
    {
        Effect += 0.1f;
    }
    if (Config.CoreValues.Num() >= 3)
    {
        Effect += 0.1f;
    }
    
    // 階段加成
    if (Context.CurrentPhase == EFiveElementPhase::Wood)
    {
        Effect += 0.1f;
    }
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::CalculateEstablishSystemEffect(
    const FEstablishSystemConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = Config.SystemStabilityTarget;
    
    if (Config.bCreateHierarchy)
    {
        Effect += 0.05f;
    }
    if (Config.bDefineRewardsPunishments)
    {
        Effect += 0.1f;
    }
    if (Config.bAssignResponsibilities)
    {
        Effect += 0.1f;
    }
    
    Effect *= (float)Config.RulesToEstablish.Num() / 10.0f;
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::CalculateEstablishPeopleEffect(
    const FEstablishPeopleConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = Config.PeopleQualityTarget;
    
    if (Config.bBuildRoleModels)
    {
        Effect += 0.1f;
    }
    if (Config.bCreateMentorshipSystem)
    {
        Effect += 0.1f;
    }
    
    Effect *= (float)Config.KeyPositions.Num() / 5.0f;
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::CalculateBreakSituationEffect(
    const FBreakSituationConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = 0.6f;
    
    if (Config.bUseSurprise)
    {
        Effect += 0.15f;
    }
    if (Config.bUseAsymmetry)
    {
        Effect += 0.1f;
    }
    if (Config.SpeedRequirement > 0.8f)
    {
        Effect += 0.1f;
    }
    if (Config.CovertLevel > 0.7f)
    {
        Effect += 0.1f;
    }
    
    // 緊急程度加成
    Effect += Context.UrgencyLevel * 0.2f;
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::CalculateBreakStructureEffect(
    const FBreakStructureConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = Config.StructuralDisruptionTarget;
    
    if (Config.bSeverRelationships)
    {
        Effect += 0.1f;
    }
    if (Config.bUndermineFoundations)
    {
        Effect += 0.15f;
    }
    if (Config.CriticalConnections.Num() >= 3)
    {
        Effect += 0.1f;
    }
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

float UMingTwelveStrategiesExecutor::CalculateBreakNotPersonEffect(
    const FBreakNotPersonConfig& Config,
    const FStrategyExecutionContext& Context) const
{
    float Effect = 0.5f;
    
    if (Config.bLeaveFormRemoveSubstance)
    {
        Effect += 0.15f;
    }
    if (Config.bRedirectRatherThanDestroy)
    {
        Effect += 0.1f;
    }
    
    Effect *= Config.PreservationRate;
    
    return FMath::Clamp(Effect, 0.0f, 1.0f);
}

bool UMingTwelveStrategiesExecutor::CheckRighteousPrerequisites(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    // 正策需要天時地利人和
    bool bHasTiming = Context.UrgencyLevel < 0.8f;
    bool bHasResources = Context.ResourceAvailability > 0.4f;
    bool bHasSupport = true;  // 簡化
    
    return bHasTiming && bHasResources && bHasSupport;
}

bool UMingTwelveStrategiesExecutor::CheckReverseActivationConditions(
    ETwelveStrategies Strategy,
    const FStrategyExecutionContext& Context) const
{
    // 逆策需要：局已僵、敵勢盛、時窗短
    bool bStalemate = Context.UrgencyLevel > 0.7f;
    bool bStrongEnemy = true;  // 簡化
    bool bShortWindow = Context.UrgencyLevel > 0.8f;
    
    return bStalemate && bStrongEnemy && bShortWindow;
}

void UMingTwelveStrategiesExecutor::UpdateUsageStatistics(ETwelveStrategies Strategy, bool bSuccess)
{
    int32& Count = StrategyUsageCounts.FindOrAdd(Strategy);
    Count++;
    
    float& AvgSuccess = AverageSuccessRates.FindOrAdd(Strategy);
    AvgSuccess = (AvgSuccess * (Count - 1) + (bSuccess ? 1.0f : 0.0f)) / Count;
    
    // 記錄到歷史
    FStrategyExecutionReport Report;
    Report.ExecutedStrategy = Strategy;
    Report.bSuccess = bSuccess;
    Report.StrategicGain = bSuccess ? 0.7f : 0.0f;
    ExecutionHistory.Add(Report);
}

bool UMingTwelveStrategiesExecutor::IsRighteousStrategy(ETwelveStrategies Strategy) const
{
    return Strategy == ETwelveStrategies::EstablishNation ||
           Strategy == ETwelveStrategies::EstablishSystem ||
           Strategy == ETwelveStrategies::EstablishPeople;
}

bool UMingTwelveStrategiesExecutor::IsReverseStrategy(ETwelveStrategies Strategy) const
{
    return Strategy == ETwelveStrategies::BreakSituation ||
           Strategy == ETwelveStrategies::BreakStructure ||
           Strategy == ETwelveStrategies::BreakNotPerson;
}
