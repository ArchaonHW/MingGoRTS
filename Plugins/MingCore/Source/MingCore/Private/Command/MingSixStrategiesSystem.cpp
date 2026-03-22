#include "MingSixStrategiesSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSixStrategiesSystem::UMingSixStrategiesSystem()
{
    ExecutionThreshold = 50.0f;
    MoralCostThreshold = 70.0f;
    SuccessRateThreshold = 60.0f;
}

void UMingSixStrategiesSystem::InitializeSixStrategiesSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化六策系統..."));

    // 初始化策略檔案
    InitializeStrategyProfiles();

    // 初始化策略組合
    InitializeStrategyCombinations();

    // 清空當前執行
    CurrentExecutions.Empty();

    UE_LOG(LogTemp, Log, TEXT("六策系統初始化完成"));
}

FSixStrategyProfile UMingSixStrategiesSystem::GetStrategyProfile(ESixStrategyType StrategyType)
{
    if (StrategyProfiles.Contains(StrategyType))
    {
        return StrategyProfiles[StrategyType];
    }

    return FSixStrategyProfile();
}

bool UMingSixStrategiesSystem::ExecuteStrategy(ESixStrategyType StrategyType)
{
    if (!StrategyProfiles.Contains(StrategyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("策略檔案不存在"));
        return false;
    }

    FSixStrategyProfile Profile = StrategyProfiles[StrategyType];

    // 檢查是否可以執行
    if (!CanExecuteStrategy(Profile))
    {
        UE_LOG(LogTemp, Warning, TEXT("策略執行條件不滿足"));
        return false;
    }

    // 創建執行實例
    FSixStrategyExecution Execution;
    Execution.Profile = Profile;
    Execution.ExecutionProgress = 0.0f;
    Execution.bIsExecuting = true;
    Execution.bIsCompleted = false;
    Execution.StartTime = GetWorld()->GetTimeSeconds();
    Execution.ElapsedTime = 0.0f;

    // 添加到當前執行列表
    CurrentExecutions.Add(Execution);

    UE_LOG(LogTemp, Log, TEXT("開始執行策略：%s"), *Profile.StrategyName);

    return true;
}

bool UMingSixStrategiesSystem::IsStrategyAvailable(ESixStrategyType StrategyType)
{
    if (!StrategyProfiles.Contains(StrategyType))
    {
        return false;
    }

    FSixStrategyProfile Profile = StrategyProfiles[StrategyType];

    // 檢查道德成本
    if (Profile.MoralCost > MoralCostThreshold)
    {
        return false;
    }

    // 檢查成功率
    if (Profile.SuccessProbability < SuccessRateThreshold)
    {
        return false;
    }

    // 檢查是否已經在執行
    for (const FSixStrategyExecution& Execution : CurrentExecutions)
    {
        if (Execution.Profile.StrategyType == StrategyType && Execution.bIsExecuting)
        {
            return false;
        }
    }

    return true;
}

float UMingSixStrategiesSystem::CalculateStrategySuccess(ESixStrategyType StrategyType)
{
    if (!StrategyProfiles.Contains(StrategyType))
    {
        return 0.0f;
    }

    FSixStrategyProfile Profile = StrategyProfiles[StrategyType];
    float BaseSuccess = Profile.SuccessProbability;

    // 根據當前執行中的策略調整成功率
    for (const FSixStrategyExecution& Execution : CurrentExecutions)
    {
        if (Execution.bIsExecuting)
        {
            // 檢查兼容性
            if (AreStrategiesCompatible(StrategyType, Execution.Profile.StrategyType))
            {
                BaseSuccess += 10.0f; // 兼容策略提升成功率
            }
            else
            {
                BaseSuccess -= 15.0f; // 不兼容策略降低成功率
            }
        }
    }

    return FMath::Clamp(BaseSuccess, 0.0f, 100.0f);
}

float UMingSixStrategiesSystem::CalculateStrategyMoralCost(ESixStrategyType StrategyType)
{
    if (!StrategyProfiles.Contains(StrategyType))
    {
        return 100.0f;
    }

    FSixStrategyProfile Profile = StrategyProfiles[StrategyType];
    float BaseCost = Profile.MoralCost;

    // 正策道德成本較低
    if (Profile.StrategyNature == ESixStrategyNature::Orthodox)
    {
        BaseCost *= 0.5f;
    }
    // 逆策道德成本較高
    else if (Profile.StrategyNature == ESixStrategyNature::Unconventional)
    {
        BaseCost *= 1.5f;
    }

    return FMath::Clamp(BaseCost, 0.0f, 100.0f);
}

TArray<ESixStrategyType> UMingSixStrategiesSystem::GetRecommendedStrategies()
{
    TArray<ESixStrategyType> Recommendations;

    // 根據當前局勢推薦策略
    for (const auto& StrategyPair : StrategyProfiles)
    {
        ESixStrategyType StrategyType = StrategyPair.Key;
        FSixStrategyProfile Profile = StrategyPair.Value;

        if (IsStrategyAvailable(StrategyType))
        {
            float SuccessRate = CalculateStrategySuccess(StrategyType);
            float MoralCost = CalculateStrategyMoralCost(StrategyType);

            // 推薦成功率高且道德成本可接受的策略
            if (SuccessRate > SuccessRateThreshold && MoralCost < MoralCostThreshold)
            {
                Recommendations.Add(StrategyType);
            }
        }
    }

    // 按成功率排序
    Recommendations.Sort([this](ESixStrategyType A, ESixStrategyType B)
    {
        return CalculateStrategySuccess(A) > CalculateStrategySuccess(B);
    });

    return Recommendations;
}

FSixStrategyCombination UMingSixStrategiesSystem::AnalyzeStrategyCombination(const TArray<ESixStrategyType>& Strategies)
{
    FSixStrategyCombination Combination;
    Combination.StrategySequence = Strategies;
    Combination.OverallSuccessProbability = 0.0f;
    Combination.OverallMoralCost = 0.0f;
    Combination.bIsValidCombination = true;

    // 檢查每個策略的可用性
    for (ESixStrategyType StrategyType : Strategies)
    {
        if (!IsStrategyAvailable(StrategyType))
        {
            Combination.bIsValidCombination = false;
            break;
        }
    }

    if (!Combination.bIsValidCombination)
    {
        return Combination;
    }

    // 計算整體成功率和道德成本
    float TotalSuccess = 0.0f;
    float TotalMoralCost = 0.0f;

    for (ESixStrategyType StrategyType : Strategies)
    {
        TotalSuccess += CalculateStrategySuccess(StrategyType);
        TotalMoralCost += CalculateStrategyMoralCost(StrategyType);
    }

    // 計算協效獎勵
    float SynergyBonus = CalculateSynergyBonus(Strategies);

    Combination.OverallSuccessProbability = (TotalSuccess / Strategies.Num()) + SynergyBonus;
    Combination.OverallMoralCost = TotalMoralCost;

    // 生成組合名稱和描述
    Combination.CombinationName = TEXT("策略組合");
    for (ESixStrategyType StrategyType : Strategies)
    {
        Combination.CombinationName += TEXT(" + ") + GetStrategyName(StrategyType);
    }

    Combination.Description = TEXT("包含") + FString::FromInt(Strategies.Num()) + TEXT("個策略的組合");

    return Combination;
}

bool UMingSixStrategiesSystem::ExecuteStrategyCombination(const TArray<ESixStrategyType>& Strategies)
{
    FSixStrategyCombination Combination = AnalyzeStrategyCombination(Strategies);

    if (!Combination.bIsValidCombination)
    {
        UE_LOG(LogTemp, Warning, TEXT("策略組合無效"));
        return false;
    }

    // 按順序執行策略
    for (ESixStrategyType StrategyType : Strategies)
    {
        if (!ExecuteStrategy(StrategyType))
        {
            UE_LOG(LogTemp, Warning, TEXT("策略組合執行失敗：%s"), *GetStrategyName(StrategyType));
            return false;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("策略組合執行成功：%s"), *Combination.CombinationName);
    return true;
}

bool UMingSixStrategiesSystem::AreStrategiesCompatible(ESixStrategyType Strategy1, ESixStrategyType Strategy2)
{
    // 檢查策略兼容性
    // 正策之間通常兼容
    bool IsStrategy1Orthodox = (Strategy1 <= ESixStrategyType::EstablishPeople);
    bool IsStrategy2Orthodox = (Strategy2 <= ESixStrategyType::EstablishPeople);

    if (IsStrategy1Orthodox && IsStrategy2Orthodox)
    {
        return true;
    }

    // 逆策之間需要檢查
    bool IsStrategy1Unconventional = (Strategy1 >= ESixStrategyType::BreakSituation);
    bool IsStrategy2Unconventional = (Strategy2 >= ESixStrategyType::BreakSituation);

    if (IsStrategy1Unconventional && IsStrategy2Unconventional)
    {
        // 不破人策略與其他逆策兼容
        if (Strategy1 == ESixStrategyType::PreservePeople || Strategy2 == ESixStrategyType::PreservePeople)
        {
            return true;
        }
        return false; // 其他逆策組合可能衝突
    }

    // 正逆策略組合需要具體分析
    return true; // 暫時認為兼容
}

TArray<FSixStrategyExecution> UMingSixStrategiesSystem::GetCurrentExecutions()
{
    return CurrentExecutions;
}

bool UMingSixStrategiesSystem::StopStrategyExecution(ESixStrategyType StrategyType)
{
    for (int32 i = 0; i < CurrentExecutions.Num(); i++)
    {
        if (CurrentExecutions[i].Profile.StrategyType == StrategyType && CurrentExecutions[i].bIsExecuting)
        {
            CurrentExecutions[i].bIsExecuting = false;
            CurrentExecutions[i].bIsCompleted = false;

            UE_LOG(LogTemp, Log, TEXT("停止策略執行：%s"), *CurrentExecutions[i].Profile.StrategyName);
            return true;
        }
    }

    return false;
}

FString UMingSixStrategiesSystem::GetStrategyName(ESixStrategyType StrategyType)
{
    switch (StrategyType)
    {
    case ESixStrategyType::EstablishNation:
        return TEXT("立國");
    case ESixStrategyType::EstablishSystem:
        return TEXT("立制");
    case ESixStrategyType::EstablishPeople:
        return TEXT("立人");
    case ESixStrategyType::BreakSituation:
        return TEXT("破局");
    case ESixStrategyType::BreakStructure:
        return TEXT("破結構");
    case ESixStrategyType::PreservePeople:
        return TEXT("不破人");
    default:
        return TEXT("未知策略");
    }
}

FString UMingSixStrategiesSystem::GetStrategyDescription(ESixStrategyType StrategyType)
{
    switch (StrategyType)
    {
    case ESixStrategyType::EstablishNation:
        return TEXT("立大義、立旗幟、立共同之志，使敵心生疑、我心生歸");
    case ESixStrategyType::EstablishSystem:
        return TEXT("立規則、立秩序、立賞罰、立權責，使勢有常軌、命有定分");
    case ESixStrategyType::EstablishPeople:
        return TEXT("立賢才、立榜樣、立信望、立傳承，使氣有依歸、影有真象");
    case ESixStrategyType::BreakSituation:
        return TEXT("用非常之手段破局，速決速收，不戀戰，不貪果");
    case ESixStrategyType::BreakStructure:
        return TEXT("隱蔽破壞敵方結構，單線直達，不留痕跡");
    case ESixStrategyType::PreservePeople:
        return TEXT("堅持道德底線，不傷害無辜，保持人性光輝");
    default:
        return TEXT("未知策略描述");
    }
}

void UMingSixStrategiesSystem::InitializeStrategyProfiles()
{
    StrategyProfiles.Empty();

    // 立國策略
    FSixStrategyProfile EstablishNation;
    EstablishNation.StrategyType = ESixStrategyType::EstablishNation;
    EstablishNation.StrategyNature = ESixStrategyNature::Orthodox;
    EstablishNation.TargetType = ESixStrategyTarget::Heart;
    EstablishNation.StrategyName = TEXT("立國");
    EstablishNation.Description = TEXT("立大義、立旗幟、立共同之志");
    EstablishNation.ExecutionCost = 80.0f;
    EstablishNation.SuccessProbability = 85.0f;
    EstablishNation.MoralCost = 10.0f;
    EstablishNation.TimeRequired = 20.0f;
    EstablishNation.bIsReversible = true;
    EstablishNation.bRequiresPublicSupport = true;
    StrategyProfiles.Add(ESixStrategyType::EstablishNation, EstablishNation);

    // 立制策略
    FSixStrategyProfile EstablishSystem;
    EstablishSystem.StrategyType = ESixStrategyType::EstablishSystem;
    EstablishSystem.StrategyNature = ESixStrategyNature::Orthodox;
    EstablishSystem.TargetType = ESixStrategyTarget::Power;
    EstablishSystem.StrategyName = TEXT("立制");
    EstablishSystem.Description = TEXT("立規則、立秩序、立賞罰、立權責");
    EstablishSystem.ExecutionCost = 90.0f;
    EstablishSystem.SuccessProbability = 90.0f;
    EstablishSystem.MoralCost = 15.0f;
    EstablishSystem.TimeRequired = 25.0f;
    EstablishSystem.bIsReversible = true;
    EstablishSystem.bRequiresPublicSupport = true;
    StrategyProfiles.Add(ESixStrategyType::EstablishSystem, EstablishSystem);

    // 立人策略
    FSixStrategyProfile EstablishPeople;
    EstablishPeople.StrategyType = ESixStrategyType::EstablishPeople;
    EstablishPeople.StrategyNature = ESixStrategyNature::Orthodox;
    EstablishPeople.TargetType = ESixStrategyTarget::Energy;
    EstablishPeople.StrategyName = TEXT("立人");
    EstablishPeople.Description = TEXT("立賢才、立榜樣、立信望、立傳承");
    EstablishPeople.ExecutionCost = 70.0f;
    EstablishPeople.SuccessProbability = 80.0f;
    EstablishPeople.MoralCost = 5.0f;
    EstablishPeople.TimeRequired = 30.0f;
    EstablishPeople.bIsReversible = true;
    EstablishPeople.bRequiresPublicSupport = true;
    StrategyProfiles.Add(ESixStrategyType::EstablishPeople, EstablishPeople);

    // 破局策略
    FSixStrategyProfile BreakSituation;
    BreakSituation.StrategyType = ESixStrategyType::BreakSituation;
    BreakSituation.StrategyNature = ESixStrategyNature::Unconventional;
    BreakSituation.TargetType = ESixStrategyTarget::Power;
    BreakSituation.StrategyName = TEXT("破局");
    BreakSituation.Description = TEXT("用非常之手段破局，速決速收");
    BreakSituation.ExecutionCost = 60.0f;
    BreakSituation.SuccessProbability = 75.0f;
    BreakSituation.MoralCost = 40.0f;
    BreakSituation.TimeRequired = 5.0f;
    BreakSituation.bIsReversible = false;
    BreakSituation.bRequiresPublicSupport = false;
    StrategyProfiles.Add(ESixStrategyType::BreakSituation, BreakSituation);

    // 破結構策略
    FSixStrategyProfile BreakStructure;
    BreakStructure.StrategyType = ESixStrategyType::BreakStructure;
    BreakStructure.StrategyNature = ESixStrategyNature::Unconventional;
    BreakStructure.TargetType = ESixStrategyTarget::Shadow;
    BreakStructure.StrategyName = TEXT("破結構");
    BreakStructure.Description = TEXT("隱蔽破壞敵方結構，單線直達");
    BreakStructure.ExecutionCost = 50.0f;
    BreakStructure.SuccessProbability = 70.0f;
    BreakStructure.MoralCost = 50.0f;
    BreakStructure.TimeRequired = 8.0f;
    BreakStructure.bIsReversible = false;
    BreakStructure.bRequiresPublicSupport = false;
    StrategyProfiles.Add(ESixStrategyType::BreakStructure, BreakStructure);

    // 不破人策略
    FSixStrategyProfile PreservePeople;
    PreservePeople.StrategyType = ESixStrategyType::PreservePeople;
    PreservePeople.StrategyNature = ESixStrategyNature::Hybrid;
    PreservePeople.TargetType = ESixStrategyTarget::Life;
    PreservePeople.StrategyName = TEXT("不破人");
    PreservePeople.Description = TEXT("堅持道德底線，不傷害無辜");
    PreservePeople.ExecutionCost = 20.0f;
    PreservePeople.SuccessProbability = 95.0f;
    PreservePeople.MoralCost = 0.0f;
    PreservePeople.TimeRequired = 1.0f;
    PreservePeople.bIsReversible = true;
    PreservePeople.bRequiresPublicSupport = true;
    StrategyProfiles.Add(ESixStrategyType::PreservePeople, PreservePeople);
}

void UMingSixStrategiesSystem::InitializeStrategyCombinations()
{
    StrategyCombinations.Empty();

    // 正策組合
    FSixStrategyCombination OrthodoxCombination;
    OrthodoxCombination.StrategySequence.Add(ESixStrategyType::EstablishNation);
    OrthodoxCombination.StrategySequence.Add(ESixStrategyType::EstablishSystem);
    OrthodoxCombination.StrategySequence.Add(ESixStrategyType::EstablishPeople);
    OrthodoxCombination.CombinationName = TEXT("正策全開");
    OrthodoxCombination.Description = TEXT("完整的正策組合，建立穩固基礎");
    OrthodoxCombination.OverallSuccessProbability = 85.0f;
    OrthodoxCombination.OverallMoralCost = 30.0f;
    OrthodoxCombination.bIsValidCombination = true;
    StrategyCombinations.Add(OrthodoxCombination);

    // 逆策組合
    FSixStrategyCombination UnconventionalCombination;
    UnconventionalCombination.StrategySequence.Add(ESixStrategyType::BreakSituation);
    UnconventionalCombination.StrategySequence.Add(ESixStrategyType::BreakStructure);
    UnconventionalCombination.StrategySequence.Add(ESixStrategyType::PreservePeople);
    UnconventionalCombination.CombinationName = TEXT("逆策精準");
    UnconventionalCombination.Description = TEXT("精準的逆策組合，快速破局");
    UnconventionalCombination.OverallSuccessProbability = 75.0f;
    UnconventionalCombination.OverallMoralCost = 90.0f;
    UnconventionalCombination.bIsValidCombination = true;
    StrategyCombinations.Add(UnconventionalCombination);
}

bool UMingSixStrategiesSystem::CanExecuteStrategy(const FSixStrategyProfile& Profile)
{
    // 檢查執行條件
    if (Profile.ExecutionCost > ExecutionThreshold)
    {
        return false;
    }

    if (Profile.MoralCost > MoralCostThreshold)
    {
        return false;
    }

    if (Profile.SuccessProbability < SuccessRateThreshold)
    {
        return false;
    }

    return true;
}

void UMingSixStrategiesSystem::UpdateExecutionProgress()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    for (int32 i = 0; i < CurrentExecutions.Num(); i++)
    {
        FSixStrategyExecution& Execution = CurrentExecutions[i];
        
        if (Execution.bIsExecuting)
        {
            Execution.ElapsedTime = CurrentTime - Execution.StartTime;
            Execution.ExecutionProgress = (Execution.ElapsedTime / Execution.Profile.TimeRequired) * 100.0f;

            if (Execution.ExecutionProgress >= 100.0f)
            {
                Execution.bIsExecuting = false;
                Execution.bIsCompleted = true;
                RecordStrategyExecution(Execution);
            }
        }
    }
}

void UMingSixStrategiesSystem::RecordStrategyExecution(const FSixStrategyExecution& Execution)
{
    ExecutionHistory.Add(Execution);

    // 限制歷史記錄數量
    if (ExecutionHistory.Num() > 100)
    {
        ExecutionHistory.RemoveAt(0);
    }
}

float UMingSixStrategiesSystem::CalculateSynergyBonus(const TArray<ESixStrategyType>& Strategies)
{
    // 計算策略協效獎勵
    float Bonus = 0.0f;

    // 正策組合有協效
    int32 OrthodoxCount = 0;
    int32 UnconventionalCount = 0;

    for (ESixStrategyType StrategyType : Strategies)
    {
        if (StrategyType <= ESixStrategyType::EstablishPeople)
        {
            OrthodoxCount++;
        }
        else if (StrategyType >= ESixStrategyType::BreakSituation)
        {
            UnconventionalCount++;
        }
    }

    // 全正策組合
    if (OrthodoxCount == Strategies.Num() && Strategies.Num() >= 2)
    {
        Bonus += 15.0f;
    }

    // 全逆策組合
    if (UnconventionalCount == Strategies.Num() && Strategies.Num() >= 2)
    {
        Bonus += 10.0f;
    }

    // 平衡組合
    if (OrthodoxCount > 0 && UnconventionalCount > 0)
    {
        Bonus += 5.0f;
    }

    return Bonus;
}
