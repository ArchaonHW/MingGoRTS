#include "SageBrain/MingSixStrategies.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingSixStrategies::UMingSixStrategies()
{
    bIsInitialized = false;
    MaxStrategyPower = 100.0f;
    TotalStrategyExecutions = 0;
    
    InitializeDefaultStrategies();
}

void UMingSixStrategies::InitializeSixStrategies()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Six Strategies already initialized"));
        return;
    }

    // 初始化所有六策狀態
    StrategyStatusMap.Empty();
    
    for (uint8 i = static_cast<uint8>(ESixStrategyType::EstablishNation); 
         i <= static_cast<uint8>(ESixStrategyType::PreservePeople); ++i)
    {
        ESixStrategyType StrategyType = static_cast<ESixStrategyType>(i);
        FStrategyStatus Status;
        Status.StrategyType = StrategyType;
        Status.Direction = (i <= static_cast<uint8>(ESixStrategyType::EstablishPeople)) ? 
                          EStrategyDirection::Positive : EStrategyDirection::Negative;
        Status.PowerLevel = 50.0f;
        Status.Effectiveness = 0.5f;
        
        StrategyStatusMap.Add(StrategyType, Status);
    }
    
    ActiveCombinations.Empty();
    TotalStrategyExecutions = 0;
    
    bIsInitialized = true;
    
    OnSixStrategiesInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Six Strategies system initialized successfully"));
}

FStrategyExecutionResult UMingSixStrategies::ExecuteStrategy(ESixStrategyType StrategyType, const TArray<FString>& Targets)
{
    FStrategyExecutionResult Result;
    Result.StrategyType = StrategyType;
    
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Six Strategies not initialized"));
        Result.ExecutionLog = TEXT("System not initialized");
        return Result;
    }

    if (!CanExecuteStrategy(StrategyType))
    {
        Result.ExecutionLog = TEXT("Cannot execute strategy");
        return Result;
    }

    // 計算執行效果
    float Effectiveness = CalculateBaseEffectiveness(StrategyType);
    float ImpactScore = CalculateImpactScore(StrategyType, Targets);
    
    // 判斷成功與否
    Result.bSuccess = (FMath::FRand() < Effectiveness);
    Result.ImpactScore = Result.bSuccess ? ImpactScore : ImpactScore * 0.3f;
    Result.AffectedTargets = Targets;
    
    if (Result.bSuccess)
    {
        Result.ExecutionLog = FString::Printf(TEXT("Strategy %d executed successfully"), static_cast<int32>(StrategyType));
    }
    else
    {
        Result.ExecutionLog = FString::Printf(TEXT("Strategy %d failed to execute"), static_cast<int32>(StrategyType));
    }
    
    // 記錄執行
    RecordStrategyExecution(StrategyType, Result.bSuccess);
    
    // 觸發事件
    OnStrategyExecuted.Broadcast(Result);
    OnStrategyExecutionCompleted(Result);
    
    return Result;
}

bool UMingSixStrategies::IsValidStrategyCombination(const TArray<ESixStrategyType>& Strategies) const
{
    if (Strategies.Num() < 2 || Strategies.Num() > 3)
    {
        return false;
    }

    // 檢查是否包含正策和逆策的混合（不允許）
    bool HasPositive = false;
    bool HasNegative = false;
    
    for (ESixStrategyType Strategy : Strategies)
    {
        if (static_cast<uint8>(Strategy) <= static_cast<uint8>(ESixStrategyType::EstablishPeople))
        {
            HasPositive = true;
        }
        else
        {
            HasNegative = true;
        }
    }
    
    // 正策和逆策不能混用
    if (HasPositive && HasNegative)
    {
        return false;
    }
    
    // 檢查是否包含重複策略
    for (int32 i = 0; i < Strategies.Num(); ++i)
    {
        for (int32 j = i + 1; j < Strategies.Num(); ++j)
        {
            if (Strategies[i] == Strategies[j])
            {
                return false;
            }
        }
    }
    
    return true;
}

FSixStrategyCombination UMingSixStrategies::ActivateStrategyCombination(const TArray<ESixStrategyType>& Strategies)
{
    FSixStrategyCombination Combination;
    Combination.ActiveStrategies = Strategies;
    
    if (!IsValidStrategyCombination(Strategies))
    {
        Combination.bIsValidCombination = false;
        return Combination;
    }
    
    Combination.bIsValidCombination = true;
    
    // 計算協同效果
    Combination.SynergyBonus = CalculateSynergyEffect(Strategies);
    
    // 計算總威力
    float TotalPower = 0.0f;
    for (ESixStrategyType Strategy : Strategies)
    {
        if (const FStrategyStatus* Status = StrategyStatusMap.Find(Strategy))
        {
            TotalPower += Status->PowerLevel;
        }
    }
    Combination.TotalPower = TotalPower * (1.0f + Combination.SynergyBonus);
    
    // 設置組合名稱
    if (Strategies.Num() == 3)
    {
        Combination.CombinationName = TEXT("完全組合");
    }
    else if (Strategies.Num() == 2)
    {
        Combination.CombinationName = TEXT("雙策組合");
    }
    
    ActiveCombinations.Add(Combination);
    
    OnStrategyCombinationActivated.Broadcast(Combination);
    
    UE_LOG(LogTemp, Log, TEXT("Strategy combination activated: %s (Power: %.2f)"), 
           *Combination.CombinationName, Combination.TotalPower);
    
    return Combination;
}

FStrategyStatus UMingSixStrategies::GetStrategyStatus(ESixStrategyType StrategyType) const
{
    if (const FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType))
    {
        return *Status;
    }
    
    return FStrategyStatus();
}

void UMingSixStrategies::UpdateStrategyPower(ESixStrategyType StrategyType, float NewPower)
{
    if (!bIsInitialized)
    {
        return;
    }

    FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType);
    if (Status)
    {
        Status->PowerLevel = FMath::Clamp(NewPower, 0.0f, MaxStrategyPower);
        UE_LOG(LogTemp, Log, TEXT("Strategy %d power updated to %.2f"), 
               static_cast<int32>(StrategyType), Status->PowerLevel);
    }
}

TArray<ESixStrategyType> UMingSixStrategies::GetRecommendedStrategies(EStrategyDirection Direction) const
{
    TArray<ESixStrategyType> Recommendations;
    
    for (const auto& Pair : StrategyStatusMap)
    {
        if (Pair.Value.Direction == Direction && Pair.Value.Effectiveness > 0.6f)
        {
            Recommendations.Add(Pair.Key);
        }
    }
    
    // 按效果排序
    Recommendations.Sort([this](const ESixStrategyType& A, const ESixStrategyType& B)
    {
        const FStrategyStatus* StatusA = StrategyStatusMap.Find(A);
        const FStrategyStatus* StatusB = StrategyStatusMap.Find(B);
        
        if (StatusA && StatusB)
        {
            return StatusA->Effectiveness > StatusB->Effectiveness;
        }
        return false;
    });
    
    return Recommendations;
}

float UMingSixStrategies::CalculateSynergyEffect(const TArray<ESixStrategyType>& Strategies) const
{
    if (Strategies.Num() < 2)
    {
        return 0.0f;
    }
    
    float BaseSynergy = 0.1f * (Strategies.Num() - 1);
    
    // 檢查特殊組合加成
    bool HasEstablishNation = Strategies.Contains(ESixStrategyType::EstablishNation);
    bool HasEstablishSystem = Strategies.Contains(ESixStrategyType::EstablishSystem);
    bool HasEstablishPeople = Strategies.Contains(ESixStrategyType::EstablishPeople);
    
    // 正三策完整組合額外加成
    if (HasEstablishNation && HasEstablishSystem && HasEstablishPeople)
    {
        BaseSynergy += 0.3f;
    }
    
    bool HasBreakSituation = Strategies.Contains(ESixStrategyType::BreakSituation);
    bool HasBreakStructure = Strategies.Contains(ESixStrategyType::BreakStructure);
    bool HasPreservePeople = Strategies.Contains(ESixStrategyType::PreservePeople);
    
    // 逆三策完整組合額外加成
    if (HasBreakSituation && HasBreakStructure && HasPreservePeople)
    {
        BaseSynergy += 0.25f;
    }
    
    return FMath::Min(BaseSynergy, 0.5f);
}

// 正策執行
FStrategyExecutionResult UMingSixStrategies::ExecuteEstablishNation(const FString& NationName)
{
    TArray<FString> Targets;
    Targets.Add(NationName);
    return ExecuteStrategy(ESixStrategyType::EstablishNation, Targets);
}

FStrategyExecutionResult UMingSixStrategies::ExecuteEstablishSystem(const FString& SystemType)
{
    TArray<FString> Targets;
    Targets.Add(SystemType);
    return ExecuteStrategy(ESixStrategyType::EstablishSystem, Targets);
}

FStrategyExecutionResult UMingSixStrategies::ExecuteEstablishPeople(const TArray<FString>& PeopleList)
{
    return ExecuteStrategy(ESixStrategyType::EstablishPeople, PeopleList);
}

// 逆策執行
FStrategyExecutionResult UMingSixStrategies::ExecuteBreakSituation(const FString& TargetSituation)
{
    TArray<FString> Targets;
    Targets.Add(TargetSituation);
    return ExecuteStrategy(ESixStrategyType::BreakSituation, Targets);
}

FStrategyExecutionResult UMingSixStrategies::ExecuteBreakStructure(const FString& TargetStructure)
{
    TArray<FString> Targets;
    Targets.Add(TargetStructure);
    return ExecuteStrategy(ESixStrategyType::BreakStructure, Targets);
}

FStrategyExecutionResult UMingSixStrategies::ExecutePreservePeople(const FString& TargetNation)
{
    TArray<FString> Targets;
    Targets.Add(TargetNation);
    return ExecuteStrategy(ESixStrategyType::PreservePeople, Targets);
}

// 私有方法
void UMingSixStrategies::InitializeDefaultStrategies()
{
    // 默認已經在構造函數中初始化
}

float UMingSixStrategies::CalculateBaseEffectiveness(ESixStrategyType StrategyType) const
{
    const FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType);
    if (Status)
    {
        return Status->Effectiveness + (Status->PowerLevel / MaxStrategyPower) * 0.3f;
    }
    return 0.5f;
}

bool UMingSixStrategies::CanExecuteStrategy(ESixStrategyType StrategyType) const
{
    const FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType);
    if (!Status)
    {
        return false;
    }
    
    return Status->PowerLevel > 10.0f;
}

void UMingSixStrategies::RecordStrategyExecution(ESixStrategyType StrategyType, bool bSuccess)
{
    FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType);
    if (Status)
    {
        Status->ExecutionCount++;
        
        // 更新成功率
        float TotalExecutions = static_cast<float>(Status->ExecutionCount);
        float CurrentSuccesses = Status->SuccessRate * (TotalExecutions - 1.0f);
        if (bSuccess)
        {
            CurrentSuccesses += 1.0f;
        }
        Status->SuccessRate = CurrentSuccesses / TotalExecutions;
        
        // 提升效果
        if (bSuccess)
        {
            Status->Effectiveness = FMath::Min(Status->Effectiveness + 0.02f, 1.0f);
        }
    }
    
    TotalStrategyExecutions++;
    
    // 記錄到歷史
    FString Timestamp = FDateTime::Now().ToString();
    FString HistoryEntry = FString::Printf(TEXT("%s - %s"), *Timestamp, bSuccess ? TEXT("Success") : TEXT("Failed"));
    
    TArray<FString>* History = StrategyHistory.Find(StrategyType);
    if (History)
    {
        History->Add(HistoryEntry);
    }
    else
    {
        TArray<FString> NewHistory;
        NewHistory.Add(HistoryEntry);
        StrategyHistory.Add(StrategyType, NewHistory);
    }
}

float UMingSixStrategies::CalculateImpactScore(ESixStrategyType StrategyType, const TArray<FString>& Targets) const
{
    float BaseImpact = 10.0f;
    
    const FStrategyStatus* Status = StrategyStatusMap.Find(StrategyType);
    if (Status)
    {
        BaseImpact *= (Status->PowerLevel / 50.0f);
        BaseImpact *= Status->Effectiveness;
    }
    
    // 根據目標數量調整
    float TargetMultiplier = 1.0f + (Targets.Num() - 1) * 0.2f;
    
    return BaseImpact * TargetMultiplier;
}
