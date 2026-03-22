#include "Events/MingWinLossBlueprintLibrary.h"
#include "Engine/Engine.h"

UMingWinLossCondition* UMingWinLossBlueprintLibrary::GetWinLossConditionSystem()
{
    // 這裡應該從遊戲實例或單例獲取勝負條件系統
    // 簡化實作
    static UMingWinLossCondition* SystemInstance = nullptr;
    if (!SystemInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            SystemInstance = NewObject<UMingWinLossCondition>(World);
            SystemInstance->Initialize();
        }
    }
    
    return SystemInstance;
}

void UMingWinLossBlueprintLibrary::InitializeWinLossConditionSystem()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (System)
    {
        System->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Win-loss condition system initialized from Blueprint"));
    }
}

bool UMingWinLossBlueprintLibrary::RegisterWinLossCondition(const FWinLossCondition& Condition)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        UE_LOG(LogTemp, Error, TEXT("Win-loss condition system not available"));
        return false;
    }
    
    return System->RegisterWinLossCondition(Condition);
}

bool UMingWinLossBlueprintLibrary::UnregisterWinLossCondition(const FString& ConditionID)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return false;
    }
    
    return System->UnregisterWinLossCondition(ConditionID);
}

FWinLossEvaluationResult UMingWinLossBlueprintLibrary::EvaluateWinLoss()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        FWinLossEvaluationResult Result;
        Result.EvaluationDetails = TEXT("Win-loss condition system not available");
        return Result;
    }
    
    return System->EvaluateWinLoss();
}

bool UMingWinLossBlueprintLibrary::CheckCondition(const FString& ConditionID)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return false;
    }
    
    return System->CheckCondition(ConditionID);
}

FWinLossCondition UMingWinLossBlueprintLibrary::GetWinLossCondition(const FString& ConditionID)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return FWinLossCondition();
    }
    
    return System->GetWinLossCondition(ConditionID);
}

TArray<FWinLossCondition> UMingWinLossBlueprintLibrary::GetAllWinLossConditions()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TArray<FWinLossCondition>();
    }
    
    return System->GetAllWinLossConditions();
}

TArray<FWinLossCondition> UMingWinLossBlueprintLibrary::GetConditionsByType(EWinLossConditionType ConditionType)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TArray<FWinLossCondition>();
    }
    
    return System->GetConditionsByType(ConditionType);
}

TArray<FWinLossCondition> UMingWinLossBlueprintLibrary::GetCriticalConditions()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TArray<FWinLossCondition>();
    }
    
    return System->GetCriticalConditions();
}

TArray<FWinLossCondition> UMingWinLossBlueprintLibrary::GetSatisfiedConditions()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TArray<FWinLossCondition>();
    }
    
    return System->GetSatisfiedConditions();
}

TArray<FWinLossCondition> UMingWinLossBlueprintLibrary::GetUnsatisfiedConditions()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TArray<FWinLossCondition>();
    }
    
    return System->GetUnsatisfiedConditions();
}

void UMingWinLossBlueprintLibrary::SetGameState(const TMap<FString, FString>& GameState)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (System)
    {
        System->SetGameState(GameState);
    }
}

TMap<FString, FString> UMingWinLossBlueprintLibrary::GetGameState()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TMap<FString, FString>();
    }
    
    return System->GetGameState();
}

void UMingWinLossBlueprintLibrary::UpdateGameStateValue(const FString& Key, const FString& Value)
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (System)
    {
        System->UpdateGameStateValue(Key, Value);
    }
}

FWinLossStatistics UMingWinLossBlueprintLibrary::GetWinLossStatistics()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return FWinLossStatistics();
    }
    
    return System->GetWinLossStatistics();
}

void UMingWinLossBlueprintLibrary::ResetStatistics()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (System)
    {
        System->ResetStatistics();
        UE_LOG(LogTemp, Log, TEXT("Win-loss statistics reset from Blueprint"));
    }
}

FWinLossEvaluationResult UMingWinLossBlueprintLibrary::ForceEvaluation()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        FWinLossEvaluationResult Result;
        Result.EvaluationDetails = TEXT("Win-loss condition system not available");
        return Result;
    }
    
    return System->ForceEvaluation();
}

EWinLossResult UMingWinLossBlueprintLibrary::GetCurrentResult()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return EWinLossResult::Unknown;
    }
    
    return System->GetCurrentResult();
}

float UMingWinLossBlueprintLibrary::GetWinLossScore()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return 0.0f;
    }
    
    return System->GetWinLossScore();
}

bool UMingWinLossBlueprintLibrary::IsGameOver()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return false;
    }
    
    return System->IsGameOver();
}

FString UMingWinLossBlueprintLibrary::GetGameOverReason()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return TEXT("");
    }
    
    return System->GetGameOverReason();
}

FString UMingWinLossBlueprintLibrary::GetResultName(EWinLossResult Result)
{
    switch (Result)
    {
    case EWinLossResult::Victory:
        return TEXT("勝利");
    case EWinLossResult::Defeat:
        return TEXT("失敗");
    case EWinLossResult::Draw:
        return TEXT("平局");
    case EWinLossResult::Stalemate:
        return TEXT("僵局");
    case EWinLossResult::InProgress:
        return TEXT("進行中");
    case EWinLossResult::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知結果");
    }
}

FString UMingWinLossBlueprintLibrary::GetConditionTypeName(EWinLossConditionType ConditionType)
{
    switch (ConditionType)
    {
    case EWinLossConditionType::Military:
        return TEXT("軍事");
    case EWinLossConditionType::Political:
        return TEXT("政治");
    case EWinLossConditionType::Economic:
        return TEXT("經濟");
    case EWinLossConditionType::Territorial:
        return TEXT("領土");
    case EWinLossConditionType::TimeBased:
        return TEXT("時間");
    case EWinLossConditionType::ScoreBased:
        return TEXT("分數");
    case EWinLossConditionType::Custom:
        return TEXT("自定義");
    default:
        return TEXT("未知類型");
    }
}

FString UMingWinLossBlueprintLibrary::GetPriorityName(EWinLossPriority Priority)
{
    switch (Priority)
    {
    case EWinLossPriority::Critical:
        return TEXT("關鍵");
    case EWinLossPriority::High:
        return TEXT("高");
    case EWinLossPriority::Medium:
        return TEXT("中");
    case EWinLossPriority::Low:
        return TEXT("低");
    case EWinLossPriority::Background:
        return TEXT("背景");
    default:
        return TEXT("未知優先級");
    }
}

FString UMingWinLossBlueprintLibrary::GetStateName(EWinLossConditionState State)
{
    switch (State)
    {
    case EWinLossConditionState::Inactive:
        return TEXT("未啟用");
    case EWinLossConditionState::Active:
        return TEXT("啟用");
    case EWinLossConditionState::Satisfied:
        return TEXT("滿足");
    case EWinLossConditionState::Failed:
        return TEXT("失敗");
    case EWinLossConditionState::Disabled:
        return TEXT("禁用");
    default:
        return TEXT("未知狀態");
    }
}

FWinLossCondition UMingWinLossBlueprintLibrary::CreateWinLossCondition(
    const FString& ConditionID,
    const FString& ConditionName,
    const FString& ConditionDescription,
    EWinLossConditionType ConditionType,
    EWinLossResult ResultType,
    EWinLossPriority Priority,
    bool bIsCriticalCondition)
{
    FWinLossCondition Condition;
    Condition.ConditionID = ConditionID;
    Condition.ConditionName = ConditionName;
    Condition.ConditionDescription = ConditionDescription;
    Condition.ConditionType = ConditionType;
    Condition.ResultType = ResultType;
    Condition.Priority = Priority;
    Condition.bIsCriticalCondition = bIsCriticalCondition;
    Condition.State = EWinLossConditionState::Active;
    Condition.CheckInterval = 1.0f;
    Condition.SatisfactionScore = 0.0f;
    
    return Condition;
}

FWinLossCondition UMingWinLossBlueprintLibrary::AddConditionParameter(
    const FWinLossCondition& Condition,
    const FString& ParameterName,
    const FString& ParameterValue,
    const FString& ComparisonOperator,
    bool bIsRequired,
    float Weight)
{
    FWinLossCondition ModifiedCondition = Condition;
    
    FWinLossConditionParameter Parameter;
    Parameter.ParameterName = ParameterName;
    Parameter.ParameterValue = ParameterValue;
    Parameter.ComparisonOperator = ComparisonOperator;
    Parameter.bIsRequired = bIsRequired;
    Parameter.Weight = Weight;
    Parameter.ParameterType = TEXT("float");
    
    ModifiedCondition.Parameters.Add(Parameter);
    return ModifiedCondition;
}

FWinLossCondition UMingWinLossBlueprintLibrary::SetCheckInterval(
    const FWinLossCondition& Condition,
    float CheckInterval)
{
    FWinLossCondition ModifiedCondition = Condition;
    ModifiedCondition.CheckInterval = CheckInterval;
    return ModifiedCondition;
}

FWinLossCondition UMingWinLossBlueprintLibrary::AddConditionTag(
    const FWinLossCondition& Condition,
    const FString& Tag)
{
    FWinLossCondition ModifiedCondition = Condition;
    ModifiedCondition.ConditionTags.Add(Tag);
    return ModifiedCondition;
}

FWinLossCondition UMingWinLossBlueprintLibrary::AddRelatedEvent(
    const FWinLossCondition& Condition,
    const FString& EventID)
{
    FWinLossCondition ModifiedCondition = Condition;
    ModifiedCondition.RelatedEvents.Add(EventID);
    return ModifiedCondition;
}

TArray<bool> UMingWinLossBlueprintLibrary::BatchCheckConditions(const TArray<FString>& ConditionIDs)
{
    TArray<bool> Results;
    
    for (const FString& ConditionID : ConditionIDs)
    {
        Results.Add(CheckCondition(ConditionID));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch checked %d conditions"), Results.Num());
    return Results;
}

TArray<bool> UMingWinLossBlueprintLibrary::BatchRegisterConditions(const TArray<FWinLossCondition>& Conditions)
{
    TArray<bool> Results;
    
    for (const FWinLossCondition& Condition : Conditions)
    {
        Results.Add(RegisterWinLossCondition(Condition));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch registered %d conditions"), Results.Num());
    return Results;
}

bool UMingWinLossBlueprintLibrary::IsWinLossSystemInitialized()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    return System != nullptr;
}

FString UMingWinLossBlueprintLibrary::GetWinLossSystemVersion()
{
    return TEXT("1.0.0");
}

int32 UMingWinLossBlueprintLibrary::GetRegisteredConditionCount()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return 0;
    }
    
    return System->GetAllWinLossConditions().Num();
}

int32 UMingWinLossBlueprintLibrary::GetSatisfiedConditionCount()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return 0;
    }
    
    return System->GetSatisfiedConditions().Num();
}

int32 UMingWinLossBlueprintLibrary::GetCriticalConditionCount()
{
    UMingWinLossCondition* System = GetWinLossConditionSystem();
    if (!System)
    {
        return 0;
    }
    
    return System->GetCriticalConditions().Num();
}

float UMingWinLossBlueprintLibrary::GetConditionSatisfactionRate()
{
    int32 TotalConditions = GetRegisteredConditionCount();
    int32 SatisfiedConditions = GetSatisfiedConditionCount();
    
    if (TotalConditions == 0)
    {
        return 0.0f;
    }
    
    return (float)SatisfiedConditions / TotalConditions;
}

float UMingWinLossBlueprintLibrary::GetVictoryProgress()
{
    FWinLossEvaluationResult Result = EvaluateWinLoss();
    return Result.VictoryScore;
}

float UMingWinLossBlueprintLibrary::GetDefeatProgress()
{
    FWinLossEvaluationResult Result = EvaluateWinLoss();
    return Result.DefeatScore;
}

float UMingWinLossBlueprintLibrary::GetDrawProgress()
{
    FWinLossEvaluationResult Result = EvaluateWinLoss();
    return Result.DrawScore;
}

float UMingWinLossBlueprintLibrary::GetTimeToVictory()
{
    // 簡化實作：基於當前勝利進度估算
    float VictoryProgress = GetVictoryProgress();
    if (VictoryProgress <= 0.0f)
    {
        return -1.0f; // 無法勝利
    }
    
    // 假設線性進展
    return (1.0f - VictoryProgress) * 3600.0f; // 小時數
}

float UMingWinLossBlueprintLibrary::GetTimeToDefeat()
{
    // 簡化實作：基於當前失敗進度估算
    float DefeatProgress = GetDefeatProgress();
    if (DefeatProgress <= 0.0f)
    {
        return -1.0f; // 無法失敗
    }
    
    // 假設線性進展
    return (1.0f - DefeatProgress) * 3600.0f; // 小時數
}

float UMingWinLossBlueprintLibrary::GetVictoryProbability()
{
    // 簡化實作：基於當前分數計算概率
    float VictoryProgress = GetVictoryProgress();
    float DefeatProgress = GetDefeatProgress();
    float DrawProgress = GetDrawProgress();
    
    float TotalProgress = VictoryProgress + DefeatProgress + DrawProgress;
    if (TotalProgress > 0.0f)
    {
        return VictoryProgress / TotalProgress;
    }
    
    return 0.5f; // 默認50%概率
}

float UMingWinLossBlueprintLibrary::GetDefeatProbability()
{
    // 簡化實作：基於當前分數計算概率
    float VictoryProgress = GetVictoryProgress();
    float DefeatProgress = GetDefeatProgress();
    float DrawProgress = GetDrawProgress();
    
    float TotalProgress = VictoryProgress + DefeatProgress + DrawProgress;
    if (TotalProgress > 0.0f)
    {
        return DefeatProgress / TotalProgress;
    }
    
    return 0.5f; // 默認50%概率
}

float UMingWinLossBlueprintLibrary::GetDrawProbability()
{
    // 簡化實作：基於當前分數計算概率
    float VictoryProgress = GetVictoryProgress();
    float DefeatProgress = GetDefeatProgress();
    float DrawProgress = GetDrawProgress();
    
    float TotalProgress = VictoryProgress + DefeatProgress + DrawProgress;
    if (TotalProgress > 0.0f)
    {
        return DrawProgress / TotalProgress;
    }
    
    return 0.0f; // 默認0%概率
}

TArray<FString> UMingWinLossBlueprintLibrary::GetWinLossRecommendations()
{
    TArray<FString> Recommendations;
    
    TArray<FWinLossCondition> UnsatisfiedConditions = GetUnsatisfiedConditions();
    
    for (const FWinLossCondition& Condition : UnsatisfiedConditions)
    {
        if (Condition.Priority == EWinLossPriority::Critical || Condition.Priority == EWinLossPriority::High)
        {
            FString Recommendation = FString::Printf(TEXT("關注條件: %s - %s"), 
                *Condition.ConditionName, *Condition.ConditionDescription);
            Recommendations.Add(Recommendation);
        }
    }
    
    return Recommendations;
}

TArray<FString> UMingWinLossBlueprintLibrary::GetCriticalConditionsToWatch()
{
    TArray<FString> CriticalConditions;
    
    TArray<FWinLossCondition> Critical = GetCriticalConditions();
    
    for (const FWinLossCondition& Condition : Critical)
    {
        if (Condition.State == EWinLossConditionState::Active || Condition.State == EWinLossConditionState::Failed)
        {
            CriticalConditions.Add(Condition.ConditionID);
        }
    }
    
    return CriticalConditions;
}

TMap<FString, float> UMingWinLossBlueprintLibrary::GetConditionImpactAnalysis()
{
    TMap<FString, float> ImpactAnalysis;
    
    TArray<FWinLossCondition> AllConditions = GetAllWinLossConditions();
    
    for (const FWinLossCondition& Condition : AllConditions)
    {
        float Impact = 0.0f;
        
        // 基於優先級
        switch (Condition.Priority)
        {
        case EWinLossPriority::Critical:
            Impact += 1.0f;
            break;
        case EWinLossPriority::High:
            Impact += 0.8f;
            break;
        case EWinLossPriority::Medium:
            Impact += 0.6f;
            break;
        case EWinLossPriority::Low:
            Impact += 0.4f;
            break;
        case EWinLossPriority::Background:
            Impact += 0.2f;
            break;
        }
        
        // 基於滿足度
        Impact += Condition.SatisfactionScore * 0.5f;
        
        // 基於關鍵性
        if (Condition.bIsCriticalCondition)
        {
            Impact += 0.3f;
        }
        
        ImpactAnalysis.Add(Condition.ConditionID, FMath::Clamp(Impact, 0.0f, 1.0f));
    }
    
    return ImpactAnalysis;
}

TMap<FString, float> UMingWinLossBlueprintLibrary::GetGameStateImpact()
{
    TMap<FString, float> GameStateImpact;
    
    // 簡化實作：基於遊戲狀態值計算影響
    TMap<FString, FString> CurrentState = GetGameState();
    
    for (const auto& State : CurrentState)
    {
        float Value = FCString::Atof(*State.Value);
        float Impact = FMath::Clamp(Value / 100.0f, 0.0f, 1.0f); // 假設100為最大值
        GameStateImpact.Add(State.Key, Impact);
    }
    
    return GameStateImpact;
}

TArray<EWinLossResult> UMingWinLossBlueprintLibrary::GetWinLossTrend(int32 HistoryCount)
{
    TArray<EWinLossResult> Trend;
    
    // 簡化實作：基於當前狀態生成趨勢
    EWinLossResult CurrentResult = GetCurrentResult();
    
    for (int32 i = 0; i < HistoryCount; ++i)
    {
        // 模擬歷史趨勢
        if (CurrentResult == EWinLossResult::Victory)
        {
            Trend.Add(EWinLossResult::InProgress);
            Trend.Add(EWinLossResult::InProgress);
            Trend.Add(EWinLossResult::Victory);
        }
        else if (CurrentResult == EWinLossResult::Defeat)
        {
            Trend.Add(EWinLossResult::InProgress);
            Trend.Add(EWinLossResult::InProgress);
            Trend.Add(EWinLossResult::Defeat);
        }
        else
        {
            Trend.Add(EWinLossResult::InProgress);
        }
    }
    
    return Trend;
}

FString UMingWinLossBlueprintLibrary::GetTrendAnalysis()
{
    TArray<EWinLossResult> Trend = GetWinLossTrend(10);
    
    int32 VictoryCount = 0;
    int32 DefeatCount = 0;
    int32 DrawCount = 0;
    int32 InProgressCount = 0;
    
    for (EWinLossResult Result : Trend)
    {
        switch (Result)
        {
        case EWinLossResult::Victory:
            VictoryCount++;
            break;
        case EWinLossResult::Defeat:
            DefeatCount++;
            break;
        case EWinLossResult::Draw:
            DrawCount++;
            break;
        case EWinLossResult::InProgress:
            InProgressCount++;
            break;
        default:
            break;
        }
    }
    
    return FString::Printf(TEXT("趨勢分析: 勝利:%d, 失敗:%d, 平局:%d, 進行中:%d"), 
        VictoryCount, DefeatCount, DrawCount, InProgressCount);
}

TArray<EWinLossResult> UMingWinLossBlueprintLibrary::SimulateWinLoss(int32 SimulationCount)
{
    TArray<EWinLossResult> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        // 簡化模擬：基於當前概率
        float VictoryProb = GetVictoryProbability();
        float DefeatProb = GetDefeatProbability();
        float DrawProb = GetDrawProbability();
        
        float Random = FMath::FRand();
        
        if (Random < VictoryProb)
        {
            SimulationResults.Add(EWinLossResult::Victory);
        }
        else if (Random < VictoryProb + DefeatProb)
        {
            SimulationResults.Add(EWinLossResult::Defeat);
        }
        else
        {
            SimulationResults.Add(EWinLossResult::Draw);
        }
    }
    
    return SimulationResults;
}

TArray<FString> UMingWinLossBlueprintLibrary::SimulateConditionChanges(const FString& ConditionID, const FString& NewValue)
{
    TArray<FString> SimulationResults;
    
    // 保存原始值
    TMap<FString, FString> OriginalState = GetGameState();
    
    // 模擬狀態變化
    UpdateGameStateValue(ConditionID, NewValue);
    
    // 評估結果
    FWinLossEvaluationResult Result = EvaluateWinLoss();
    SimulationResults.Add(FString::Printf(TEXT("模擬結果: %s"), *GetResultName(Result.Result)));
    
    // 恢復原始狀態
    SetGameState(OriginalState);
    
    return SimulationResults;
}

bool UMingWinLossBlueprintLibrary::ExportWinLossData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Win-loss data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingWinLossBlueprintLibrary::ImportWinLossData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Win-loss data would be imported from: %s"), *FilePath);
    return true;
}

FString UMingWinLossBlueprintLibrary::GenerateWinLossReport()
{
    FWinLossEvaluationResult Result = EvaluateWinLoss();
    FWinLossStatistics Stats = GetWinLossStatistics();
    
    FString Report = TEXT("=== 勝負條件報告 ===\n");
    Report += FString::Printf(TEXT("當前結果: %s\n"), *GetResultName(Result.Result));
    Report += FString::Printf(TEXT("勝利分數: %.2f\n"), Result.VictoryScore);
    Report += FString::Printf(TEXT("失敗分數: %.2f\n"), Result.DefeatScore);
    Report += FString::Printf(TEXT("平局分數: %.2f\n"), Result.DrawScore);
    Report += FString::Printf(TEXT("滿足條件: %d/%d\n"), Result.SatisfiedConditions.Num(), GetRegisteredConditionCount());
    Report += FString::Printf(TEXT("總評估次數: %d\n"), Stats.TotalEvaluations);
    Report += FString::Printf(TEXT("勝率: %.2f%%\n"), Stats.WinRate * 100.0f);
    
    return Report;
}

FString UMingWinLossBlueprintLibrary::GenerateConditionReport(const FString& ConditionID)
{
    FWinLossCondition Condition = GetWinLossCondition(ConditionID);
    
    FString Report = TEXT("=== 條件詳細報告 ===\n");
    Report += FString::Printf(TEXT("條件ID: %s\n"), *Condition.ConditionID);
    Report += FString::Printf(TEXT("條件名稱: %s\n"), *Condition.ConditionName);
    Report += FString::Printf(TEXT("條件描述: %s\n"), *Condition.ConditionDescription);
    Report += FString::Printf(TEXT("條件類型: %s\n"), *GetConditionTypeName(Condition.ConditionType));
    Report += FString::Printf(TEXT("結果類型: %s\n"), *GetResultName(Condition.ResultType));
    Report += FString::Printf(TEXT("優先級: %s\n"), *GetPriorityName(Condition.Priority));
    Report += FString::Printf(TEXT("狀態: %s\n"), *GetStateName(Condition.State));
    Report += FString::Printf(TEXT("滿足度: %.2f\n"), Condition.SatisfactionScore);
    Report += FString::Printf(TEXT("關鍵條件: %s\n"), Condition.bIsCriticalCondition ? TEXT("是") : TEXT("否"));
    
    return Report;
}

FString UMingWinLossBlueprintLibrary::GenerateStatisticsReport()
{
    FWinLossStatistics Stats = GetWinLossStatistics();
    
    FString Report = TEXT("=== 統計報告 ===\n");
    Report += FString::Printf(TEXT("總評估次數: %d\n"), Stats.TotalEvaluations);
    Report += FString::Printf(TEXT("勝利次數: %d\n"), Stats.VictoryCount);
    Report += FString::Printf(TEXT("失敗次數: %d\n"), Stats.DefeatCount);
    Report += FString::Printf(TEXT("平局次數: %d\n"), Stats.DrawCount);
    Report += FString::Printf(TEXT("勝率: %.2f%%\n"), Stats.WinRate * 100.0f);
    Report += FString::Printf(TEXT("平均評估時間: %.2f秒\n"), Stats.AverageEvaluationTime);
    Report += FString::Printf(TEXT("條件滿足率: %.2f%%\n"), Stats.ConditionSatisfactionRate * 100.0f);
    Report += FString::Printf(TEXT("關鍵條件觸發: %d\n"), Stats.CriticalConditionTriggers);
    
    return Report;
}
