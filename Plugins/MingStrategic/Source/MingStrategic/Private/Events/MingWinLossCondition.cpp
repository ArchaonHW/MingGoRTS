#include "Events/MingWinLossCondition.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingWinLossCondition::UMingWinLossCondition()
    : bIsInitialized(false)
{
    // 預分配容量
    RegisteredConditions.Reserve(50);
    Statistics = FWinLossStatistics();
    CurrentEvaluation = FWinLossEvaluationResult();
}

void UMingWinLossCondition::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化遊戲狀態
    GameState.Add(TEXT("game_time"), TEXT("0"));
    GameState.Add(TEXT("player_team"), TEXT("1"));
    GameState.Add(TEXT("enemy_team"), TEXT("2"));
    GameState.Add(TEXT("victory_score"), TEXT("0"));
    GameState.Add(TEXT("defeat_score"), TEXT("0"));
    GameState.Add(TEXT("draw_score"), TEXT("0"));
    
    // 初始化民國歷史勝負條件
    InitializeHistoricalWinLossConditions();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingWinLossCondition initialized"));
    UE_LOG(LogTemp, Log, TEXT("Loaded %d win-loss conditions"), RegisteredConditions.Num());
}

void UMingWinLossCondition::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    RegisteredConditions.Empty();
    ConditionMap.Empty();
    GameState.Empty();
    Statistics = FWinLossStatistics();
    CurrentEvaluation = FWinLossEvaluationResult();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingWinLossCondition shutdown"));
}

bool UMingWinLossCondition::RegisterWinLossCondition(const FWinLossCondition& Condition)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("WinLossCondition not initialized"));
        return false;
    }

    if (Condition.ConditionID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Condition ID cannot be empty"));
        return false;
    }

    // 檢查是否已存在
    if (ConditionMap.Contains(Condition.ConditionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Condition %s already registered"), *Condition.ConditionID);
        return false;
    }

    // 驗證條件配置
    if (!ValidateConditionConfig(Condition))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid condition configuration: %s"), *Condition.ConditionID);
        return false;
    }

    // 添加到註冊列表
    RegisteredConditions.Add(Condition);
    ConditionMap.Add(Condition.ConditionID, Condition);
    
    UE_LOG(LogTemp, Log, TEXT("Win-loss condition registered: %s - %s"), 
        *Condition.ConditionID, *Condition.ConditionName);
    
    return true;
}

bool UMingWinLossCondition::UnregisterWinLossCondition(const FString& ConditionID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (!ConditionMap.Contains(ConditionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Condition %s not found"), *ConditionID);
        return false;
    }

    // 從註冊列表移除
    for (int32 i = 0; i < RegisteredConditions.Num(); ++i)
    {
        if (RegisteredConditions[i].ConditionID == ConditionID)
        {
            RegisteredConditions.RemoveAt(i);
            break;
        }
    }
    
    ConditionMap.Remove(ConditionID);
    
    UE_LOG(LogTemp, Log, TEXT("Win-loss condition unregistered: %s"), *ConditionID);
    
    return true;
}

FWinLossEvaluationResult UMingWinLossCondition::EvaluateWinLoss()
{
    FWinLossEvaluationResult Result;
    Result.EvaluationTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        Result.EvaluationDetails = TEXT("WinLossCondition not initialized");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Evaluating win-loss conditions..."));

    // 檢查所有條件
    for (FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.State == EWinLossConditionState::Active)
        {
            bool bSatisfied = CheckCondition(Condition.ConditionID);
            
            if (bSatisfied)
            {
                Condition.State = EWinLossConditionState::Satisfied;
                Condition.SatisfactionScore = EvaluateConditionSatisfaction(Condition);
                Result.SatisfiedConditions.Add(Condition.ConditionID);
                
                // 記錄勝負分數
                if (Condition.ResultType == EWinLossResult::Victory)
                {
                    Result.VictoryScore += Condition.SatisfactionScore;
                }
                else if (Condition.ResultType == EWinLossResult::Defeat)
                {
                    Result.DefeatScore += Condition.SatisfactionScore;
                }
                else if (Condition.ResultType == EWinLossResult::Draw)
                {
                    Result.DrawScore += Condition.SatisfactionScore;
                }
            }
            else
            {
                Condition.State = EWinLossConditionState::Failed;
                Condition.SatisfactionScore = 0.0f;
                Result.UnsatisfiedConditions.Add(Condition.ConditionID);
            }
            
            // 更新關鍵條件狀態
            if (Condition.bIsCriticalCondition)
            {
                Result.CriticalConditionStates.Add(Condition.ConditionID, Condition.State);
            }
        }
    }

    // 計算總分
    CalculateWinLossScores(Result);
    
    // 確定最終結果
    Result.Result = DetermineFinalResult(Result);
    
    // 更新統計
    UpdateStatistics(Result);
    
    // 通知勝負變化
    if (CurrentEvaluation.Result != Result.Result)
    {
        NotifyWinLossChange(CurrentEvaluation.Result, Result.Result);
    }
    
    // 處理遊戲結束
    if (Result.Result != EWinLossResult::Unknown && Result.Result != EWinLossResult::InProgress)
    {
        FString Reason = FString::Printf(TEXT("Game ended: %s"), *UEnum::GetValueAsString(Result.Result));
        HandleGameOver(Result.Result, Reason);
    }
    
    // 更新當前評估
    CurrentEvaluation = Result;
    
    UE_LOG(LogTemp, Log, TEXT("Win-loss evaluation completed: %s (V:%.2f, D:%.2f, D:%.2f)"), 
        *UEnum::GetValueAsString(Result.Result), 
        Result.VictoryScore, Result.DefeatScore, Result.DrawScore);
    
    return Result;
}

bool UMingWinLossCondition::CheckCondition(const FString& ConditionID)
{
    const FWinLossCondition* Condition = ConditionMap.Find(ConditionID);
    if (!Condition)
    {
        UE_LOG(LogTemp, Error, TEXT("Condition %s not found"), *ConditionID);
        return false;
    }

    // 檢查間隔
    float CurrentTime = FPlatformTime::Seconds();
    if (CurrentTime - Condition->LastCheckTime < Condition->CheckInterval)
    {
        return Condition->State == EWinLossConditionState::Satisfied;
    }

    Condition->LastCheckTime = CurrentTime;

    // 根據條件類型檢查
    bool bSatisfied = false;
    switch (Condition->ConditionType)
    {
    case EWinLossConditionType::Military:
        bSatisfied = CheckMilitaryCondition(*Condition);
        break;
    case EWinLossConditionType::Political:
        bSatisfied = CheckPoliticalCondition(*Condition);
        break;
    case EWinLossConditionType::Economic:
        bSatisfied = CheckEconomicCondition(*Condition);
        break;
    case EWinLossConditionType::Territorial:
        bSatisfied = CheckTerritorialCondition(*Condition);
        break;
    case EWinLossConditionType::TimeBased:
        bSatisfied = CheckTimeBasedCondition(*Condition);
        break;
    case EWinLossConditionType::ScoreBased:
        bSatisfied = CheckScoreBasedCondition(*Condition);
        break;
    case EWinLossConditionType::Custom:
        bSatisfied = CheckCustomCondition(*Condition);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown condition type: %d"), static_cast<int32>(Condition->ConditionType));
        break;
    }

    return bSatisfied;
}

FWinLossCondition UMingWinLossCondition::GetWinLossCondition(const FString& ConditionID) const
{
    const FWinLossCondition* Condition = ConditionMap.Find(ConditionID);
    return Condition ? *Condition : FWinLossCondition();
}

TArray<FWinLossCondition> UMingWinLossCondition::GetAllWinLossConditions() const
{
    return RegisteredConditions;
}

TArray<FWinLossCondition> UMingWinLossCondition::GetConditionsByType(EWinLossConditionType ConditionType) const
{
    TArray<FWinLossCondition> Result;
    
    for (const FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.ConditionType == ConditionType)
        {
            Result.Add(Condition);
        }
    }
    
    return Result;
}

TArray<FWinLossCondition> UMingWinLossCondition::GetCriticalConditions() const
{
    TArray<FWinLossCondition> Result;
    
    for (const FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.bIsCriticalCondition)
        {
            Result.Add(Condition);
        }
    }
    
    return Result;
}

TArray<FWinLossCondition> UMingWinLossCondition::GetSatisfiedConditions() const
{
    TArray<FWinLossCondition> Result;
    
    for (const FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.State == EWinLossConditionState::Satisfied)
        {
            Result.Add(Condition);
        }
    }
    
    return Result;
}

TArray<FWinLossCondition> UMingWinLossCondition::GetUnsatisfiedConditions() const
{
    TArray<FWinLossCondition> Result;
    
    for (const FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.State == EWinLossConditionState::Failed)
        {
            Result.Add(Condition);
        }
    }
    
    return Result;
}

void UMingWinLossCondition::SetGameState(const TMap<FString, FString>& NewGameState)
{
    GameState = NewGameState;
    UE_LOG(LogTemp, Verbose, TEXT("Game state updated with %d values"), GameState.Num());
}

TMap<FString, FString> UMingWinLossCondition::GetGameState() const
{
    return GameState;
}

void UMingWinLossCondition::UpdateGameStateValue(const FString& Key, const FString& Value)
{
    GameState.Add(Key, Value);
    UE_LOG(LogTemp, Verbose, TEXT("Game state value updated: %s = %s"), *Key, *Value);
}

FWinLossStatistics UMingWinLossCondition::GetWinLossStatistics() const
{
    return Statistics;
}

void UMingWinLossCondition::ResetStatistics()
{
    Statistics = FWinLossStatistics();
    UE_LOG(LogTemp, Log, TEXT("Win-loss statistics reset"));
}

FWinLossEvaluationResult UMingWinLossCondition::ForceEvaluation()
{
    return EvaluateWinLoss();
}

EWinLossResult UMingWinLossCondition::GetCurrentResult() const
{
    return CurrentEvaluation.Result;
}

float UMingWinLossCondition::GetWinLossScore() const
{
    return CurrentEvaluation.TotalScore;
}

bool UMingWinLossCondition::IsGameOver() const
{
    return CurrentEvaluation.Result != EWinLossResult::Unknown && 
           CurrentEvaluation.Result != EWinLossResult::InProgress;
}

FString UMingWinLossCondition::GetGameOverReason() const
{
    return CurrentEvaluation.EvaluationDetails;
}

// === 內部方法實現 ===

void UMingWinLossCondition::InitializeHistoricalWinLossConditions()
{
    CreateMilitaryConditions();
    CreatePoliticalConditions();
    CreateEconomicConditions();
    CreateTerritorialConditions();
    CreateTimeBasedConditions();
    CreateScoreBasedConditions();
}

void UMingWinLossCondition::CreateMilitaryConditions()
{
    // 敵方軍事勝利條件
    FWinLossCondition PlayerMilitaryVictory;
    PlayerMilitaryVictory.ConditionID = TEXT("player_military_victory");
    PlayerMilitaryVictory.ConditionName = TEXT("玩家軍事勝利");
    PlayerMilitaryVictory.ConditionDescription = TEXT("玩家軍事力量完全擊敗敵方");
    PlayerMilitaryVictory.ConditionType = EWinLossConditionType::Military;
    PlayerMilitaryVictory.ResultType = EWinLossResult::Victory;
    PlayerMilitaryVictory.Priority = EWinLossPriority::Critical;
    PlayerMilitaryVictory.bIsCriticalCondition = true;
    PlayerMilitaryVictory.CheckInterval = 5.0f;
    
    FWinLossConditionParameter EnemyUnits;
    EnemyUnits.ParameterName = TEXT("enemy_units");
    EnemyUnits.ParameterValue = TEXT("0");
    EnemyUnits.ComparisonOperator = TEXT("<=");
    EnemyUnits.bIsRequired = true;
    EnemyUnits.Weight = 1.0f;
    
    PlayerMilitaryVictory.Parameters.Add(EnemyUnits);
    
    RegisterWinLossCondition(PlayerMilitaryVictory);

    // 敵方軍事失敗條件
    FWinLossCondition PlayerMilitaryDefeat;
    PlayerMilitaryDefeat.ConditionID = TEXT("player_military_defeat");
    PlayerMilitaryDefeat.ConditionName = TEXT("玩家軍事失敗");
    PlayerMilitaryDefeat.ConditionDescription = TEXT("玩家軍事力量被完全消滅");
    PlayerMilitaryDefeat.ConditionType = EWinLossConditionType::Military;
    PlayerMilitaryDefeat.ResultType = EWinLossResult::Defeat;
    PlayerMilitaryDefeat.Priority = EWinLossPriority::Critical;
    PlayerMilitaryDefeat.bIsCriticalCondition = true;
    PlayerMilitaryDefeat.CheckInterval = 5.0f;
    
    FWinLossConditionParameter PlayerUnits;
    PlayerUnits.ParameterName = TEXT("player_units");
    PlayerUnits.ParameterValue = TEXT("0");
    PlayerUnits.ComparisonOperator = TEXT("<=");
    PlayerUnits.bIsRequired = true;
    PlayerUnits.Weight = 1.0f;
    
    PlayerMilitaryDefeat.Parameters.Add(PlayerUnits);
    
    RegisterWinLossCondition(PlayerMilitaryDefeat);
}

void UMingWinLossCondition::CreatePoliticalConditions()
{
    // 政治統一條件
    FWinLossCondition PoliticalUnification;
    PoliticalUnification.ConditionID = TEXT("political_unification");
    PoliticalUnification.ConditionName = TEXT("政治統一");
    PoliticalUnification.ConditionDescription = TEXT("實現政治統一，控制所有地區");
    PoliticalUnification.ConditionType = EWinLossConditionType::Political;
    PoliticalUnification.ResultType = EWinLossResult::Victory;
    PoliticalUnification.Priority = EWinLossPriority::High;
    PoliticalUnification.bIsCriticalCondition = false;
    PoliticalUnification.CheckInterval = 10.0f;
    
    FWinLossConditionParameter PoliticalControl;
    PoliticalControl.ParameterName = TEXT("political_control");
    PoliticalControl.ParameterValue = TEXT("100");
    PoliticalControl.ComparisonOperator(TEXT(">="));
    PoliticalControl.bIsRequired = true;
    PoliticalControl.Weight = 1.0f;
    
    PoliticalUnification.Parameters.Add(PoliticalControl);
    
    RegisterWinLossCondition(PoliticalUnification);
}

void UMingWinLossCondition::CreateEconomicConditions()
{
    // 經濟勝利條件
    FWinLossCondition EconomicVictory;
    EconomicVictory.ConditionID = TEXT("economic_victory");
    EconomicVictory.ConditionName = TEXT("經濟勝利");
    EconomicVictory.ConditionDescription = TEXT("經濟實力遠超敵方");
    EconomicVictory.ConditionType = EWinLossConditionType::Economic;
    EconomicVictory.ResultType = EWinLossResult::Victory;
    EconomicVictory.Priority = EWinLossPriority::Medium;
    EconomicVictory.bIsCriticalCondition = false;
    EconomicVictory.CheckInterval = 15.0f;
    
    FWinLossConditionParameter EconomicAdvantage;
    EconomicAdvantage.ParameterName = TEXT("economic_advantage");
    EconomicAdvantage.ParameterValue = TEXT("200");
    EconomicAdvantage.ComparisonOperator(TEXT(">"));
    EconomicAdvantage.bIsRequired = true;
    EconomicAdvantage.Weight = 1.0f;
    
    EconomicVictory.Parameters.Add(EconomicAdvantage);
    
    RegisterWinLossCondition(EconomicVictory);
}

void UMingWinLossCondition::CreateTerritorialConditions()
{
    // 領土控制勝利條件
    FWinLossCondition TerritorialVictory;
    TerritorialVictory.ConditionID = TEXT("territorial_victory");
    TerritorialVictory.ConditionName = TEXT("領土控制勝利");
    TerritorialVictory.ConditionDescription = TEXT("控制大部分領土");
    TerritorialVictory.ConditionType = EWinLossConditionType::Territorial;
    TerritorialVictory.ResultType = EWinLossResult::Victory;
    TerritorialVictory.Priority = EWinLossPriority::High;
    TerritorialVictory.bIsCriticalCondition = false;
    TerritorialVictory.CheckInterval = 10.0f;
    
    FWinLossConditionParameter TerritoryControl;
    TerritoryControl.ParameterName = TEXT("territory_control");
    TerritoryControl.ParameterValue = TEXT("75");
    TerritoryControl.ComparisonOperator(TEXT(">="));
    TerritoryControl.bIsRequired = true;
    TerritoryControl.Weight = 1.0f;
    
    TerritorialVictory.Parameters.Add(TerritoryControl);
    
    RegisterWinLossCondition(TerritorialVictory);
}

void UMingWinLossCondition::CreateTimeBasedConditions()
{
    // 時間限制條件
    FWinLossCondition TimeLimit;
    TimeLimit.ConditionID = TEXT("time_limit");
    TimeLimit.ConditionName = TEXT("時間限制");
    TimeLimit.ConditionDescription = TEXT("超過時間限制");
    TimeLimit.ConditionType = EWinLossConditionType::TimeBased;
    TimeLimit.ResultType = EWinLossResult::Draw;
    TimeLimit.Priority = EWinLossPriority::Low;
    TimeLimit.bIsCriticalCondition = false;
    TimeLimit.CheckInterval = 30.0f;
    
    FWinLossConditionParameter GameTime;
    GameTime.ParameterName = TEXT("game_time");
    GameTime.ParameterValue = TEXT("3600"); // 1小時
    GameTime.ComparisonOperator(TEXT(">"));
    GameTime.bIsRequired = true;
    GameTime.Weight = 1.0f;
    
    TimeLimit.Parameters.Add(GameTime);
    
    RegisterWinLossCondition(TimeLimit);
}

void UMingWinLossCondition::CreateScoreBasedConditions()
{
    // 分數勝利條件
    FWinLossCondition ScoreVictory;
    ScoreVictory.ConditionID = TEXT("score_victory");
    ScoreVictory.ConditionName = TEXT("分數勝利");
    ScoreVictory.ConditionDescription = TEXT("達到勝利分數");
    ScoreVictory.ConditionType = EWinLossConditionType::ScoreBased;
    ScoreVictory.ResultType = EWinLossResult::Victory;
    ScoreVictory.Priority = EWinLossPriority::Medium;
    ScoreVictory.bIsCriticalCondition = false;
    ScoreVictory.CheckInterval = 5.0f;
    
    FWinLossConditionParameter VictoryScore;
    VictoryScore.ParameterName = TEXT("victory_score");
    VictoryScore.ParameterValue = TEXT("1000");
    VictoryScore.ComparisonOperator(TEXT(">="));
    VictoryScore.bIsRequired = true;
    VictoryScore.Weight = 1.0f;
    
    ScoreVictory.Parameters.Add(VictoryScore);
    
    RegisterWinLossCondition(ScoreVictory);
}

// === 條件檢查方法 ===

bool UMingWinLossCondition::CheckMilitaryCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckPoliticalCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckEconomicCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckTerritorialCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckTimeBasedCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckScoreBasedCondition(const FWinLossCondition& Condition)
{
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UMingWinLossCondition::CheckCustomCondition(const FWinLossCondition& Condition)
{
    // 簡化實作：基於參數檢查
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        FString Value = GetGameStateValue(Parameter.ParameterName);
        
        if (!CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
        {
            if (Parameter.bIsRequired)
            {
                return false;
            }
        }
    }
    
    return true;
}

float UMingWinLossCondition::EvaluateConditionSatisfaction(const FWinLossCondition& Condition)
{
    float Satisfaction = 0.0f;
    int32 RequiredCount = 0;
    int32 SatisfiedCount = 0;
    
    for (const FWinLossConditionParameter& Parameter : Condition.Parameters)
    {
        if (Parameter.bIsRequired)
        {
            RequiredCount++;
            
            FString Value = GetGameStateValue(Parameter.ParameterName);
            if (CompareValues(Value, Parameter.ComparisonOperator, Parameter.ParameterValue))
            {
                SatisfiedCount++;
                Satisfaction += Parameter.Weight;
            }
        }
    }
    
    if (RequiredCount > 0)
    {
        Satisfaction /= RequiredCount;
    }
    
    return FMath::Clamp(Satisfaction, 0.0f, 1.0f);
}

void UMingWinLossCondition::CalculateWinLossScores(FWinLossEvaluationResult& Result)
{
    // 計算總分
    Result.TotalScore = Result.VictoryScore + Result.DefeatScore + Result.DrawScore;
    
    // 正規化分數
    float TotalScore = Result.TotalScore;
    if (TotalScore > 0.0f)
    {
        Result.VictoryScore /= TotalScore;
        Result.DefeatScore /= TotalScore;
        Result.DrawScore /= TotalScore;
    }
}

EWinLossResult UMingWinLossCondition::DetermineFinalResult(const FWinLossEvaluationResult& Result)
{
    // 檢查關鍵條件
    if (CheckCriticalConditions())
    {
        // 如果有關鍵條件滿足，根據類型確定結果
        for (const FString& ConditionID : Result.SatisfiedConditions)
        {
            const FWinLossCondition* Condition = ConditionMap.Find(ConditionID);
            if (Condition && Condition->bIsCriticalCondition)
            {
                return Condition->ResultType;
            }
        }
    }
    
    // 基於分數確定結果
    if (Result.VictoryScore >= 0.7f)
    {
        return EWinLossResult::Victory;
    }
    else if (Result.DefeatScore >= 0.7f)
    {
        return EWinLossResult::Defeat;
    }
    else if (Result.DrawScore >= 0.5f)
    {
        return EWinLossResult::Draw;
    }
    else
    {
        return EWinLossResult::InProgress;
    }
}

void UMingWinLossCondition::UpdateStatistics(const FWinLossEvaluationResult& Result)
{
    Statistics.TotalEvaluations++;
    
    switch (Result.Result)
    {
    case EWinLossResult::Victory:
        Statistics.VictoryCount++;
        break;
    case EWinLossResult::Defeat:
        Statistics.DefeatCount++;
        break;
    case EWinLossResult::Draw:
        Statistics.DrawCount++;
        break;
    default:
        break;
    }
    
    // 計算勝率
    if (Statistics.TotalEvaluations > 0)
    {
        Statistics.WinRate = (float)Statistics.VictoryCount / Statistics.TotalEvaluations;
    }
    
    // 計算平均評估時間
    float TotalTime = Statistics.AverageEvaluationTime * (Statistics.TotalEvaluations - 1) + 
                      (FPlatformTime::Seconds() - Result.EvaluationTime);
    Statistics.AverageEvaluationTime = TotalTime / Statistics.TotalEvaluations;
    
    // 計算條件滿足率
    int32 TotalConditions = RegisteredConditions.Num();
    int32 SatisfiedConditions = Result.SatisfiedConditions.Num();
    if (TotalConditions > 0)
    {
        Statistics.ConditionSatisfactionRate = (float)SatisfiedConditions / TotalConditions;
    }
}

void UMingWinLossCondition::NotifyWinLossChange(EWinLossResult OldResult, EWinLossResult NewResult)
{
    UE_LOG(LogTemp, Log, TEXT("Win-loss result changed: %s -> %s"), 
        *UEnum::GetValueAsString(OldResult), *UEnum::GetValueAsString(NewResult));
    
    // 這裡可以添加事件通知邏輯
}

FString UMingWinLossCondition::GetGameStateValue(const FString& Key) const
{
    FString* Value = GameState.Find(Key);
    return Value ? *Value : TEXT("0");
}

bool UMingWinLossCondition::CompareValues(const FString& Value1, const FString& Operator, const FString& Value2) const
{
    float NumValue1 = FCString::Atof(*Value1);
    float NumValue2 = FCString::Atof(*Value2);
    
    if (Operator == TEXT(">"))
        return NumValue1 > NumValue2;
    else if (Operator == TEXT("<"))
        return NumValue1 < NumValue2;
    else if (Operator == TEXT("=="))
        return FMath::IsNearlyEqual(NumValue1, NumValue2);
    else if (Operator == TEXT(">="))
        return NumValue1 >= NumValue2;
    else if (Operator == TEXT("<="))
        return NumValue1 <= NumValue2;
    else if (Operator == TEXT("contains"))
        return Value1.Contains(Value2);
    
    return false;
}

bool UMingWinLossCondition::ValidateConditionConfig(const FWinLossCondition& Condition) const
{
    if (Condition.ConditionID.IsEmpty())
    {
        return false;
    }
    
    if (Condition.Parameters.Num() == 0)
    {
        return false;
    }
    
    return true;
}

void UMingWinLossCondition::LogWinLossEvent(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[WinLoss] %s"), *Message);
}

bool UMingWinLossCondition::CheckCriticalConditions()
{
    for (const FWinLossCondition& Condition : RegisteredConditions)
    {
        if (Condition.bIsCriticalCondition && Condition.State == EWinLossConditionState::Satisfied)
        {
            return true;
        }
    }
    
    return false;
}

void UMingWinLossCondition::HandleGameOver(EWinLossResult Result, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("Game Over: %s - %s"), *UEnum::GetValueAsString(Result), *Reason);
    
    // 這裡可以添加遊戲結束處理邏輯
}
