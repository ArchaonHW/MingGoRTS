#include "SageCommand/MingSixStrategies.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSixStrategies::UMingSixStrategies()
{
    // 初始化策略狀態
    StrategyStates.Add(ESixStrategyType::HeavenStrategy, EStrategyState::Planning);
    StrategyStates.Add(ESixStrategyType::EarthStrategy, EStrategyState::Planning);
    StrategyStates.Add(ESixStrategyType::HumanStrategy, EStrategyState::Planning);
    StrategyStates.Add(ESixStrategyType::HeavenEarthStrategy, EStrategyState::Planning);
    StrategyStates.Add(ESixStrategyType::HeavenHumanStrategy, EStrategyState::Planning);
    StrategyStates.Add(ESixStrategyType::EarthHumanStrategy, EStrategyState::Planning);
}

bool UMingSixStrategies::Initialize()
{
    if (bSystemActive)
    {
        return true;
    }

    // 初始化系統狀態
    bSystemActive = true;
    SystemStability = 100.0f;

    // 設置執行更新定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ExecutionUpdateTimer,
            this,
            &UMingSixStrategies::UpdateStrategyExecution,
            0.1f,
            true
        );
    }

    return true;
}

void UMingSixStrategies::Cleanup()
{
    bSystemActive = false;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ExecutionUpdateTimer);
    }
    
    StrategyStates.Empty();
    ExecutionPlans.Empty();
    StrategyHistory.Empty();
    ActiveStrategies.Empty();
}

FStrategyEvaluation UMingSixStrategies::EvaluateStrategy(ESixStrategyType StrategyType, const FString& Context)
{
    FStrategyEvaluation Evaluation;
    Evaluation.StrategyType = StrategyType;
    
    // 評估策略的各個維度
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        Evaluation.SuccessProbability = EvaluateHeavenStrategy(Context);
        break;
    case ESixStrategyType::EarthStrategy:
        Evaluation.SuccessProbability = EvaluateEarthStrategy(Context);
        break;
    case ESixStrategyType::HumanStrategy:
        Evaluation.SuccessProbability = EvaluateHumanStrategy(Context);
        break;
    case ESixStrategyType::HeavenEarthStrategy:
        Evaluation.SuccessProbability = EvaluateHeavenEarthStrategy(Context);
        break;
    case ESixStrategyType::HeavenHumanStrategy:
        Evaluation.SuccessProbability = EvaluateHeavenHumanStrategy(Context);
        break;
    case ESixStrategyType::EarthHumanStrategy:
        Evaluation.SuccessProbability = EvaluateEarthHumanStrategy(Context);
        break;
    }
    
    // 計算其他評估指標
    Evaluation.RiskLevel = CalculateStrategyRisk(StrategyType, Context);
    Evaluation.ResourceCost = CalculateResourceRequirements(StrategyType);
    Evaluation.TimeRequired = CalculateTimeRequirements(StrategyType);
    Evaluation.StrategicValue = Evaluation.SuccessProbability * (100.0f - Evaluation.RiskLevel) / 100.0f;
    
    // 生成建議
    if (Evaluation.SuccessProbability > 70.0f && Evaluation.RiskLevel < 30.0f)
    {
        Evaluation.Recommendation = TEXT("推薦執行：成功概率高，風險較低");
    }
    else if (Evaluation.SuccessProbability > 50.0f)
    {
        Evaluation.Recommendation = TEXT("可考慮執行：需要謹慎評估風險");
    }
    else
    {
        Evaluation.Recommendation = TEXT("不推薦執行：成功概率較低，風險較高");
    }
    
    // 觸發事件
    OnStrategyEvaluated.Broadcast(Evaluation);
    
    return Evaluation;
}

TArray<FStrategyEvaluation> UMingSixStrategies::EvaluateAllStrategies(const FString& Context)
{
    TArray<FStrategyEvaluation> Evaluations;
    
    // 評估所有六種策略
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::HeavenStrategy, Context));
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::EarthStrategy, Context));
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::HumanStrategy, Context));
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::HeavenEarthStrategy, Context));
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::HeavenHumanStrategy, Context));
    Evaluations.Add(EvaluateStrategy(ESixStrategyType::EarthHumanStrategy, Context));
    
    return Evaluations;
}

ESixStrategyType UMingSixStrategies::GetOptimalStrategy(const FString& Context)
{
    TArray<FStrategyEvaluation> Evaluations = EvaluateAllStrategies(Context);
    
    ESixStrategyType OptimalStrategy = ESixStrategyType::HeavenStrategy;
    float MaxStrategicValue = 0.0f;
    
    for (const FStrategyEvaluation& Evaluation : Evaluations)
    {
        if (Evaluation.StrategicValue > MaxStrategicValue)
        {
            MaxStrategicValue = Evaluation.StrategicValue;
            OptimalStrategy = Evaluation.StrategyType;
        }
    }
    
    return OptimalStrategy;
}

TArray<FString> UMingSixStrategies::GetStrategyRecommendations(const FString& Context)
{
    TArray<FString> Recommendations;
    TArray<FStrategyEvaluation> Evaluations = EvaluateAllStrategies(Context);
    
    // 按戰略價值排序
    Evaluations.Sort([](const FStrategyEvaluation& A, const FStrategyEvaluation& B)
    {
        return A.StrategicValue > B.StrategicValue;
    });
    
    // 生成建議
    for (int32 i = 0; i < Evaluations.Num() && i < 3; ++i)
    {
        const FStrategyEvaluation& Evaluation = Evaluations[i];
        Recommendations.Add(FString::Printf(TEXT("%d. %s - %s"), 
            i + 1, *GetStrategyName(Evaluation.StrategyType), *Evaluation.Recommendation));
    }
    
    return Recommendations;
}

bool UMingSixStrategies::ExecuteStrategy(ESixStrategyType StrategyType, const FString& Context)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 檢查策略是否已經在執行中
    if (ActiveStrategies.Contains(StrategyType))
    {
        return false;
    }
    
    // 評估策略
    FStrategyEvaluation Evaluation = EvaluateStrategy(StrategyType, Context);
    
    // 如果成功概率太低，拒絕執行
    if (Evaluation.SuccessProbability < 30.0f)
    {
        return false;
    }
    
    // 創建執行計劃
    FStrategyExecutionPlan Plan = CreateExecutionPlan(StrategyType, Context);
    
    // 開始執行
    return StartStrategyExecution(Plan);
}

FStrategyExecutionPlan UMingSixStrategies::CreateExecutionPlan(ESixStrategyType StrategyType, const FString& Context)
{
    FStrategyExecutionPlan Plan;
    Plan.StrategyType = StrategyType;
    Plan.CurrentState = EStrategyState::Preparing;
    Plan.ProgressPercentage = 0.0f;
    
    // 根據策略類型生成執行步驟
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        Plan.ExecutionSteps.Add(TEXT("分析天時變化"));
        Plan.ExecutionSteps.Add(TEXT("選擇最佳時機"));
        Plan.ExecutionSteps.Add(TEXT("利用自然現象"));
        Plan.ExecutionSteps.Add(TEXT("執行天道策略"));
        break;
    case ESixStrategyType::EarthStrategy:
        Plan.ExecutionSteps.Add(TEXT("勘察地形地貌"));
        Plan.ExecutionSteps.Add(TEXT("佔據有利位置"));
        Plan.ExecutionSteps.Add(TEXT("設置防禦工事"));
        Plan.ExecutionSteps.Add(TEXT("執行地道策略"));
        break;
    case ESixStrategyType::HumanStrategy:
        Plan.ExecutionSteps.Add(TEXT("分析敵我人心"));
        Plan.ExecutionSteps.Add(TEXT("制定心理戰術"));
        Plan.ExecutionSteps.Add(TEXT("分化敵方內部"));
        Plan.ExecutionSteps.Add(TEXT("執行人道策略"));
        break;
    case ESixStrategyType::HeavenEarthStrategy:
        Plan.ExecutionSteps.Add(TEXT("分析天時地利"));
        Plan.ExecutionSteps.Add(TEXT("協調天地因素"));
        Plan.ExecutionSteps.Add(TEXT("制定綜合方案"));
        Plan.ExecutionSteps.Add(TEXT("執行天地策略"));
        break;
    case ESixStrategyType::HeavenHumanStrategy:
        Plan.ExecutionSteps.Add(TEXT("分析天時人心"));
        Plan.ExecutionSteps.Add(TEXT("協調天人關係"));
        Plan.ExecutionSteps.Add(TEXT("制定精神戰術"));
        Plan.ExecutionSteps.Add(TEXT("執行天人策略"));
        break;
    case ESixStrategyType::EarthHumanStrategy:
        Plan.ExecutionSteps.Add(TEXT("分析地利人心"));
        Plan.ExecutionSteps.Add(TEXT("協調地人關係"));
        Plan.ExecutionSteps.Add(TEXT("制定持久戰術"));
        Plan.ExecutionSteps.Add(TEXT("執行地人策略"));
        break;
    }
    
    // 計算所需資源
    Plan.RequiredResources.Add(TEXT("兵力"));
    Plan.RequiredResources.Add(TEXT("物資"));
    Plan.RequiredResources.Add(TEXT("時間"));
    
    // 識別風險因素
    Plan.RiskFactors.Add(TEXT("敵方反制"));
    Plan.RiskFactors.Add(TEXT("環境變化"));
    Plan.RiskFactors.Add(TEXT("內部不穩"));
    
    // 計算預計持續時間
    Plan.EstimatedDuration = CalculateTimeRequirements(StrategyType);
    
    return Plan;
}

bool UMingSixStrategies::StartStrategyExecution(const FStrategyExecutionPlan& Plan)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    ESixStrategyType StrategyType = Plan.StrategyType;
    
    // 保存執行計劃
    ExecutionPlans.Add(StrategyType, Plan);
    
    // 設置策略狀態
    StrategyStates[StrategyType] = EStrategyState::Executing;
    
    // 添加到活動策略列表
    ActiveStrategies.Add(StrategyType);
    
    // 記錄事件
    RecordStrategyEvent(FString::Printf(TEXT("開始執行策略：%s"), *GetStrategyName(StrategyType)), 
                       StrategyType, EStrategyState::Executing, 50.0f);
    
    // 觸發事件
    OnStrategyExecutionStarted.Broadcast(Plan);
    
    return true;
}

bool UMingSixStrategies::PauseStrategyExecution(ESixStrategyType StrategyType)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return false;
    }
    
    StrategyStates[StrategyType] = EStrategyState::Planning;
    
    // 記錄事件
    RecordStrategyEvent(FString::Printf(TEXT("暫停執行策略：%s"), *GetStrategyName(StrategyType)), 
                       StrategyType, EStrategyState::Planning, 25.0f);
    
    return true;
}

bool UMingSixStrategies::ResumeStrategyExecution(ESixStrategyType StrategyType)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return false;
    }
    
    StrategyStates[StrategyType] = EStrategyState::Executing;
    
    // 記錄事件
    RecordStrategyEvent(FString::Printf(TEXT("恢復執行策略：%s"), *GetStrategyName(StrategyType)), 
                       StrategyType, EStrategyState::Executing, 35.0f);
    
    return true;
}

bool UMingSixStrategies::CancelStrategyExecution(ESixStrategyType StrategyType)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return false;
    }
    
    StrategyStates[StrategyType] = EStrategyState::Failed;
    ActiveStrategies.Remove(StrategyType);
    ExecutionPlans.Remove(StrategyType);
    
    // 記錄事件
    RecordStrategyEvent(FString::Printf(TEXT("取消執行策略：%s"), *GetStrategyName(StrategyType)), 
                       StrategyType, EStrategyState::Failed, 20.0f);
    
    return true;
}

EStrategyState UMingSixStrategies::GetStrategyState(ESixStrategyType StrategyType) const
{
    if (StrategyStates.Contains(StrategyType))
    {
        return StrategyStates[StrategyType];
    }
    return EStrategyState::Planning;
}

float UMingSixStrategies::GetStrategyProgress(ESixStrategyType StrategyType) const
{
    if (ExecutionPlans.Contains(StrategyType))
    {
        return ExecutionPlans[StrategyType].ProgressPercentage;
    }
    return 0.0f;
}

TArray<FString> UMingSixStrategies::GetActiveStrategies() const
{
    TArray<FString> ActiveStrategyNames;
    
    for (ESixStrategyType StrategyType : ActiveStrategies)
    {
        ActiveStrategyNames.Add(GetStrategyName(StrategyType));
    }
    
    return ActiveStrategyNames;
}

bool UMingSixStrategies::IsStrategyExecuting(ESixStrategyType StrategyType) const
{
    return ActiveStrategies.Contains(StrategyType) && 
           StrategyStates.Contains(StrategyType) && 
           StrategyStates[StrategyType] == EStrategyState::Executing;
}

TArray<ESixStrategyType> UMingSixStrategies::GetCompatibleStrategies(ESixStrategyType StrategyType) const
{
    return GetSynergisticStrategies(StrategyType);
}

TArray<ESixStrategyType> UMingSixStrategies::GetConflictingStrategies(ESixStrategyType StrategyType) const
{
    return GetAntagonisticStrategies(StrategyType);
}

float UMingSixStrategies::CalculateStrategySynergy(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const
{
    return CalculateCompatibilityScore(Strategy1, Strategy2);
}

EStrategyComplexity UMingSixStrategies::GetStrategyComplexity(ESixStrategyType StrategyType) const
{
    return DetermineStrategyComplexity(StrategyType);
}

float UMingSixStrategies::CalculateStrategicAdvantage(ESixStrategyType StrategyType) const
{
    // 基於策略複雜度和當前狀態計算戰略優勢
    EStrategyComplexity Complexity = GetStrategyComplexity(StrategyType);
    float ComplexityBonus = 0.0f;
    
    switch (Complexity)
    {
    case EStrategyComplexity::Simple:
        ComplexityBonus = 10.0f;
        break;
    case EStrategyComplexity::Moderate:
        ComplexityBonus = 25.0f;
        break;
    case EStrategyComplexity::Complex:
        ComplexityBonus = 40.0f;
        break;
    case EStrategyComplexity::Master:
        ComplexityBonus = 60.0f;
        break;
    }
    
    return 50.0f + ComplexityBonus; // 基礎50分 + 複雜度加成
}

TArray<FString> UMingSixStrategies::GetStrategyEffects(ESixStrategyType StrategyType) const
{
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        return GetHeavenStrategyEffects();
    case ESixStrategyType::EarthStrategy:
        return GetEarthStrategyEffects();
    case ESixStrategyType::HumanStrategy:
        return GetHumanStrategyEffects();
    case ESixStrategyType::HeavenEarthStrategy:
        return GetHeavenEarthStrategyEffects();
    case ESixStrategyType::HeavenHumanStrategy:
        return GetHeavenHumanStrategyEffects();
    case ESixStrategyType::EarthHumanStrategy:
        return GetEarthHumanStrategyEffects();
    default:
        return TArray<FString>();
    }
}

TArray<FString> UMingSixStrategies::GetStrategyRequirements(ESixStrategyType StrategyType) const
{
    TArray<FString> Requirements;
    
    Requirements.Add(TEXT("足夠的兵力"));
    Requirements.Add(TEXT("充分的物資"));
    Requirements.Add(TEXT("適當的時機"));
    
    // 根據策略類型添加特殊要求
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        Requirements.Add(TEXT("有利的天時"));
        break;
    case ESixStrategyType::EarthStrategy:
        Requirements.Add(TEXT("有利的地形"));
        break;
    case ESixStrategyType::HumanStrategy:
        Requirements.Add(TEXT("深入了解敵我人心"));
        break;
    case ESixStrategyType::HeavenEarthStrategy:
        Requirements.Add(TEXT("天時地利的協調"));
        break;
    case ESixStrategyType::HeavenHumanStrategy:
        Requirements.Add(TEXT("天人合一的境界"));
        break;
    case ESixStrategyType::EarthHumanStrategy:
        Requirements.Add(TEXT("地人協調的能力"));
        break;
    }
    
    return Requirements;
}

TArray<FSixStrategyEvent> UMingSixStrategies::GetStrategyHistory() const
{
    return StrategyHistory;
}

FSixStrategyEvent UMingSixStrategies::GetLastStrategyEvent() const
{
    if (StrategyHistory.Num() > 0)
    {
        return StrategyHistory.Last();
    }
    return FSixStrategyEvent();
}

void UMingSixStrategies::ClearStrategyHistory()
{
    StrategyHistory.Empty();
}

// 私有方法實現

float UMingSixStrategies::EvaluateHeavenStrategy(const FString& Context)
{
    float BaseScore = 50.0f;
    
    // 檢查上下文中的天時因素
    if (Context.Contains("天時") || Context.Contains("天氣") || Context.Contains("季節"))
    {
        BaseScore += 20.0f;
    }
    
    if (Context.Contains("夜晚") || Context.Contains("月光"))
    {
        BaseScore += 15.0f;
    }
    
    if (Context.Contains("風雨") || Context.Contains("雷電"))
    {
        BaseScore += 10.0f;
    }
    
    return FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

float UMingSixStrategies::EvaluateEarthStrategy(const FString& Context)
{
    float BaseScore = 50.0f;
    
    // 檢查上下文中的地利因素
    if (Context.Contains("地形") || Context.Contains("地貌") || Context.Contains("位置"))
    {
        BaseScore += 20.0f;
    }
    
    if (Context.Contains("高地") || Context.Contains("要塞"))
    {
        BaseScore += 15.0f;
    }
    
    if (Context.Contains("防禦") || Context.Contains("守護"))
    {
        BaseScore += 10.0f;
    }
    
    return FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

float UMingSixStrategies::EvaluateHumanStrategy(const FString& Context)
{
    float BaseScore = 50.0f;
    
    // 檢查上下文中的人和因素
    if (Context.Contains("人心") || Context.Contains("士氣") || Context.Contains("忠誠"))
    {
        BaseScore += 20.0f;
    }
    
    if (Context.Contains("心理") || Context.Contains("情感"))
    {
        BaseScore += 15.0f;
    }
    
    if (Context.Contains("分化") || Context.Contains("離間"))
    {
        BaseScore += 10.0f;
    }
    
    return FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

float UMingSixStrategies::EvaluateHeavenEarthStrategy(const FString& Context)
{
    // 天地策略是天道和地道策略的結合
    float HeavenScore = EvaluateHeavenStrategy(Context);
    float EarthScore = EvaluateEarthStrategy(Context);
    
    return (HeavenScore + EarthScore) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMingSixStrategies::EvaluateHeavenHumanStrategy(const FString& Context)
{
    // 天人策略是天道和人道策略的結合
    float HeavenScore = EvaluateHeavenStrategy(Context);
    float HumanScore = EvaluateHumanStrategy(Context);
    
    return (HeavenScore + HumanScore) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMingSixStrategies::EvaluateEarthHumanStrategy(const FString& Context)
{
    // 地人策略是地道和人道策略的結合
    float EarthScore = EvaluateEarthStrategy(Context);
    float HumanScore = EvaluateHumanStrategy(Context);
    
    return (EarthScore + HumanScore) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMingSixStrategies::CalculateStrategyRisk(ESixStrategyType StrategyType, const FString& Context)
{
    float BaseRisk = 30.0f; // 基礎風險
    
    // 根據策略複雜度調整風險
    EStrategyComplexity Complexity = DetermineStrategyComplexity(StrategyType);
    switch (Complexity)
    {
    case EStrategyComplexity::Simple:
        BaseRisk -= 10.0f;
        break;
    case EStrategyComplexity::Moderate:
        BaseRisk += 0.0f;
        break;
    case EStrategyComplexity::Complex:
        BaseRisk += 15.0f;
        break;
    case EStrategyComplexity::Master:
        BaseRisk += 30.0f;
        break;
    }
    
    // 根據上下文調整風險
    if (Context.Contains("危險") || Context.Contains("風險"))
    {
        BaseRisk += 20.0f;
    }
    
    if (Context.Contains("安全") || Context.Contains("穩定"))
    {
        BaseRisk -= 15.0f;
    }
    
    return FMath::Clamp(BaseRisk, 0.0f, 100.0f);
}

float UMingSixStrategies::CalculateResourceRequirements(ESixStrategyType StrategyType)
{
    // 根據策略類型計算資源需求
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        return 40.0f; // 主要依賴時機，資源需求較低
    case ESixStrategyType::EarthStrategy:
        return 60.0f; // 需要佔據地利，資源需求中等
    case ESixStrategyType::HumanStrategy:
        return 50.0f; // 主要依賴心理戰，資源需求中等
    case ESixStrategyType::HeavenEarthStrategy:
        return 70.0f; // 天地結合，資源需求較高
    case ESixStrategyType::HeavenHumanStrategy:
        return 65.0f; // 天人結合，資源需求較高
    case ESixStrategyType::EarthHumanStrategy:
        return 75.0f; // 地人結合，資源需求最高
    default:
        return 50.0f;
    }
}

float UMingSixStrategies::CalculateTimeRequirements(ESixStrategyType StrategyType)
{
    // 根據策略類型計算時間需求（返回分鐘數）
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        return 30.0f; // 依賴時機，時間較短
    case ESixStrategyType::EarthStrategy:
        return 60.0f; // 需要佔據地利，時間中等
    case ESixStrategyType::HumanStrategy:
        return 45.0f; // 心理戰需要時間，時間中等
    case ESixStrategyType::HeavenEarthStrategy:
        return 75.0f; // 天地結合，時間較長
    case ESixStrategyType::HeavenHumanStrategy:
        return 90.0f; // 天人結合，時間較長
    case ESixStrategyType::EarthHumanStrategy:
        return 120.0f; // 地人結合，時間最長
    default:
        return 60.0f;
    }
}

void UMingSixStrategies::UpdateStrategyExecution(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 更新所有執行中的策略
    for (ESixStrategyType StrategyType : ActiveStrategies)
    {
        if (StrategyStates[StrategyType] == EStrategyState::Executing)
        {
            UpdateStrategyProgress(StrategyType, DeltaTime);
            CheckStrategyCompletion(StrategyType);
        }
    }
}

void UMingSixStrategies::UpdateStrategyProgress(ESixStrategyType StrategyType, float DeltaTime)
{
    if (!ExecutionPlans.Contains(StrategyType))
    {
        return;
    }
    
    FStrategyExecutionPlan& Plan = ExecutionPlans[StrategyType];
    
    // 更新進度
    float ProgressIncrement = (StrategyExecutionSpeed * DeltaTime / Plan.EstimatedDuration) * 100.0f;
    Plan.ProgressPercentage = FMath::Clamp(Plan.ProgressPercentage + ProgressIncrement, 0.0f, 100.0f);
}

void UMingSixStrategies::CheckStrategyCompletion(ESixStrategyType StrategyType)
{
    if (!ExecutionPlans.Contains(StrategyType))
    {
        return;
    }
    
    const FStrategyExecutionPlan& Plan = ExecutionPlans[StrategyType];
    
    if (Plan.ProgressPercentage >= 100.0f)
    {
        // 策略執行完成
        StrategyStates[StrategyType] = EStrategyState::Completed;
        ActiveStrategies.Remove(StrategyType);
        
        // 記錄事件
        RecordStrategyEvent(FString::Printf(TEXT("策略執行完成：%s"), *GetStrategyName(StrategyType)), 
                           StrategyType, EStrategyState::Completed, 80.0f);
    }
}

TArray<ESixStrategyType> UMingSixStrategies::GetSynergisticStrategies(ESixStrategyType StrategyType) const
{
    TArray<ESixStrategyType> SynergisticStrategies;
    
    // 定義策略間的協同關係
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy:
        SynergisticStrategies.Add(ESixStrategyType::HeavenEarthStrategy);
        SynergisticStrategies.Add(ESixStrategyType::HeavenHumanStrategy);
        break;
    case ESixStrategyType::EarthStrategy:
        SynergisticStrategies.Add(ESixStrategyType::HeavenEarthStrategy);
        SynergisticStrategies.Add(ESixStrategyType::EarthHumanStrategy);
        break;
    case ESixStrategyType::HumanStrategy:
        SynergisticStrategies.Add(ESixStrategyType::HeavenHumanStrategy);
        SynergisticStrategies.Add(ESixStrategyType::EarthHumanStrategy);
        break;
    case ESixStrategyType::HeavenEarthStrategy:
        SynergisticStrategies.Add(ESixStrategyType::HeavenStrategy);
        SynergisticStrategies.Add(ESixStrategyType::EarthStrategy);
        break;
    case ESixStrategyType::HeavenHumanStrategy:
        SynergisticStrategies.Add(ESixStrategyType::HeavenStrategy);
        SynergisticStrategies.Add(ESixStrategyType::HumanStrategy);
        break;
    case ESixStrategyType::EarthHumanStrategy:
        SynergisticStrategies.Add(ESixStrategyType::EarthStrategy);
        SynergisticStrategies.Add(ESixStrategyType::HumanStrategy);
        break;
    }
    
    return SynergisticStrategies;
}

TArray<ESixStrategyType> UMingSixStrategies::GetAntagonisticStrategies(ESixStrategyType StrategyType) const
{
    // 六策之間沒有直接的對抗關係，更多是互補
    return TArray<ESixStrategyType>();
}

float UMingSixStrategies::CalculateCompatibilityScore(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const
{
    // 如果是協同策略，返回高分數
    TArray<ESixStrategyType> SynergisticStrategies = GetSynergisticStrategies(Strategy1);
    if (SynergisticStrategies.Contains(Strategy2))
    {
        return 0.8f;
    }
    
    // 如果是相同策略，返回中等分數
    if (Strategy1 == Strategy2)
    {
        return 0.5f;
    }
    
    // 其他情況返回低分數
    return 0.2f;
}

void UMingSixStrategies::RecordStrategyEvent(const FString& Description, ESixStrategyType StrategyType, EStrategyState State, float Impact)
{
    FSixStrategyEvent Event;
    Event.EventID = FString::Printf(TEXT("STRATEGY_%lld"), FDateTime::Now().GetTicks());
    Event.Description = Description;
    Event.StrategyType = StrategyType;
    Event.EventState = State;
    Event.ImpactLevel = Impact;
    Event.Timestamp = FDateTime::Now();
    
    StrategyHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (StrategyHistory.Num() > 1000)
    {
        StrategyHistory.RemoveAt(0);
    }
}

FString UMingSixStrategies::GetStrategyName(ESixStrategyType StrategyType) const
{
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy: return TEXT("天道策略");
    case ESixStrategyType::EarthStrategy: return TEXT("地道策略");
    case ESixStrategyType::HumanStrategy: return TEXT("人道策略");
    case ESixStrategyType::HeavenEarthStrategy: return TEXT("天地策略");
    case ESixStrategyType::HeavenHumanStrategy: return TEXT("天人策略");
    case ESixStrategyType::EarthHumanStrategy: return TEXT("地人策略");
    default: return TEXT("未知策略");
    }
}

FString UMingSixStrategies::GetStateName(EStrategyState State) const
{
    switch (State)
    {
    case EStrategyState::Planning: return TEXT("計劃中");
    case EStrategyState::Preparing: return TEXT("準備中");
    case EStrategyState::Executing: return TEXT("執行中");
    case EStrategyState::Monitoring: return TEXT("監控中");
    case EStrategyState::Completed: return TEXT("已完成");
    case EStrategyState::Failed: return TEXT("失敗");
    default: return TEXT("未知狀態");
    }
}

FString UMingSixStrategies::GetComplexityName(EStrategyComplexity Complexity) const
{
    switch (Complexity)
    {
    case EStrategyComplexity::Simple: return TEXT("簡單");
    case EStrategyComplexity::Moderate: return TEXT("中等");
    case EStrategyComplexity::Complex: return TEXT("複雜");
    case EStrategyComplexity::Master: return TEXT("大師級");
    default: return TEXT("未知");
    }
}

EStrategyComplexity UMingSixStrategies::DetermineStrategyComplexity(ESixStrategyType StrategyType) const
{
    switch (StrategyType)
    {
    case ESixStrategyType::HeavenStrategy: return EStrategyComplexity::Moderate;
    case ESixStrategyType::EarthStrategy: return EStrategyComplexity::Moderate;
    case ESixStrategyType::HumanStrategy: return EStrategyComplexity::Complex;
    case ESixStrategyType::HeavenEarthStrategy: return EStrategyComplexity::Complex;
    case ESixStrategyType::HeavenHumanStrategy: return EStrategyComplexity::Master;
    case ESixStrategyType::EarthHumanStrategy: return EStrategyComplexity::Master;
    default: return EStrategyComplexity::Simple;
    }
}

TArray<FString> UMingSixStrategies::GetHeavenStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("利用天時變化獲得優勢"));
    Effects.Add(TEXT("在特定時間點發動攻擊"));
    Effects.Add(TEXT("利用自然現象掩護行動"));
    Effects.Add(TEXT("提高部隊士氣和戰鬥力"));
    return Effects;
}

TArray<FString> UMingSixStrategies::GetEarthStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("佔據有利地形獲得防禦優勢"));
    Effects.Add(TEXT("利用地形特點設置陷阱"));
    Effects.Add(TEXT("建立穩固的防禦工事"));
    Effects.Add(TEXT("控制關鍵戰略位置"));
    return Effects;
}

TArray<FString> UMingSixStrategies::GetHumanStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("分化敵方內部，削弱士氣"));
    Effects.Add(TEXT("收買敵方重要人物"));
    Effects.Add(TEXT("散播謠言動搖敵軍心"));
    Effects.Add(TEXT("提升己方部隊忠誠度"));
    return Effects;
}

TArray<FString> UMingSixStrategies::GetHeavenEarthStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("協調天時地利形成絕對優勢"));
    Effects.Add(TEXT("在最佳時機利用地形特點"));
    Effects.Add(TEXT("建立天地一體的防禦體系"));
    Effects.Add(TEXT("提高整體戰略協調性"));
    return Effects;
}

TArray<FString> UMingSixStrategies::GetHeavenHumanStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("達到天人合一的精神境界"));
    Effects.Add(TEXT("利用天時進行心理戰"));
    Effects.Add(TEXT("提升部隊的精神戰鬥力"));
    Effects.Add(TEXT("獲得超常的戰略洞察力"));
    return Effects;
}

TArray<FString> UMingSixStrategies::GetEarthHumanStrategyEffects() const
{
    TArray<FString> Effects;
    Effects.Add(TEXT("實現地人協調的持久作戰"));
    Effects.Add(TEXT("利用地形進行長期消耗戰"));
    Effects.Add(TEXT("建立穩固的後方基地"));
    Effects.Add(TEXT("獲得長期的戰略主動權"));
    return Effects;
}
