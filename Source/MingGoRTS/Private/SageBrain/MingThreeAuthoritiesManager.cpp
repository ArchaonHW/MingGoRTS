#include "MingThreeAuthoritiesManager.h"
#include "MingSupremeSageCommandSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingThreeAuthoritiesManager::UMingThreeAuthoritiesManager()
    : DaoAuthorityHealth(1.0f)
    , StrategyAuthorityHealth(1.0f)
    , MilitaryAuthorityHealth(1.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化三權狀態
    AuthorityStates.Add(ESupremeAuthorityType::DaoAuthority, ECommandStatus::Suspended);
    AuthorityStates.Add(ESupremeAuthorityType::StrategyAuthority, ECommandStatus::Suspended);
    AuthorityStates.Add(ESupremeAuthorityType::MilitaryAuthority, ECommandStatus::Suspended);
    
    // 默認道權優先
    CurrentActiveAuthority = ESupremeAuthorityType::DaoAuthority;
    bBalancingEnabled = true;
    BalanceCheckInterval = 5.0f;
}

void UMingThreeAuthoritiesManager::BeginPlay()
{
    Super::BeginPlay();
    
    // 啟動權力平衡檢查定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            BalanceCheckTimer,
            this,
            &UMingThreeAuthoritiesManager::CheckAuthorityBalance,
            BalanceCheckInterval,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("三權管理器初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  道權健康度: %.2f"), DaoAuthorityHealth);
    UE_LOG(LogTemp, Log, TEXT("  策權健康度: %.2f"), StrategyAuthorityHealth);
    UE_LOG(LogTemp, Log, TEXT("  兵權健康度: %.2f"), MilitaryAuthorityHealth);
}

bool UMingThreeAuthoritiesManager::Initialize()
{
    // 初始化三權健康度
    DaoAuthorityHealth = 1.0f;
    StrategyAuthorityHealth = 1.0f;
    MilitaryAuthorityHealth = 1.0f;
    
    PendingOrders.Empty();
    ExecutingOrders.Empty();
    ExecutionHistory.Empty();
    
    return true;
}

FDaoAuthorityResult UMingThreeAuthoritiesManager::EvaluateAction(const FCommandAction& Action)
{
    FDaoAuthorityResult Result;
    
    // 評估行動的道德風險
    Result.MoralRisk = CalculateMoralRisk(Action);
    Result.LongTermImpact = AssessLongTermConsequences(Action);
    Result.bIsPermitted = (Result.MoralRisk < 0.7f);
    
    if (!Result.bIsPermitted)
    {
        Result.ReasonForDenial = TEXT("道德風險過高");
    }
    
    return Result;
}

FDaoAuthorityDecision UMingThreeAuthoritiesManager::MakeDaoDecision(const FCommandSituation& Situation)
{
    FDaoAuthorityDecision Decision;
    
    // 評估道德風險
    float MoralRisk = Situation.MoralAmbiguity;
    
    // 檢查歷史模式
    if (CheckMoralRedLine(Situation.History))
    {
        Decision.Decision = EDaoAuthorityDecisionType::StopAndSeal;
        Decision.Reasoning = TEXT("觸及墮落紅線，必須立即停止並封存");
        Decision.MoralRiskAssessment = 0.9f;
        return Decision;
    }
    
    // 評估長期影響
    float LongTermImpact = AssessLongTermMoralImpact(Situation.StrategyPlan);
    
    if (MoralRisk > 0.8f || LongTermImpact < -0.5f)
    {
        Decision.Decision = EDaoAuthorityDecisionType::TransferPower;
        Decision.Reasoning = TEXT("道德風險過高，建議權力交接");
        Decision.MoralRiskAssessment = MoralRisk;
    }
    else if (MoralRisk > 0.6f)
    {
        Decision.Decision = EDaoAuthorityDecisionType::Suspend;
        Decision.Reasoning = TEXT("道德風險較高，暫停行動");
        Decision.MoralRiskAssessment = MoralRisk;
    }
    else if (MoralRisk > 0.4f)
    {
        Decision.Decision = EDaoAuthorityDecisionType::Warn;
        Decision.Reasoning = TEXT("注意道德風險");
        Decision.MoralRiskAssessment = MoralRisk;
    }
    else
    {
        Decision.Decision = EDaoAuthorityDecisionType::Allow;
        Decision.Reasoning = TEXT("道德風險在可接受範圍");
        Decision.MoralRiskAssessment = MoralRisk;
    }
    
    return Decision;
}

bool UMingThreeAuthoritiesManager::CheckMoralRedLine(const FActionHistory& History) const
{
    // 檢查三大墮落徵象
    int32 ConsecutiveEvilActions = 0;
    float TotalMoralAmbiguity = 0.0f;
    
    for (const auto& Action : History.RecentActions)
    {
        if (Action.bUsedEvilForce)
        {
            ConsecutiveEvilActions++;
        }
        TotalMoralAmbiguity += Action.MoralAmbiguity;
    }
    
    float AvgMoralAmbiguity = History.RecentActions.Num() > 0 ? 
        TotalMoralAmbiguity / History.RecentActions.Num() : 0.0f;
    
    // 紅線條件
    if (ConsecutiveEvilActions >= 5 || AvgMoralAmbiguity > 0.7f)
    {
        return true;
    }
    
    return false;
}

float UMingThreeAuthoritiesManager::AssessLongTermMoralImpact(const FStrategyPlan& Plan) const
{
    float Impact = 0.0f;
    
    // 評估計劃中的每個行動
    for (const auto& Action : Plan.Actions)
    {
        if (Action.bUsesReverseStrategy)
        {
            Impact -= 0.1f;
        }
        if (Action.bUsesEvilForce)
        {
            Impact -= 0.15f;
        }
    }
    
    // 評估計劃的可持續性
    if (Plan.bIsSustainable)
    {
        Impact += 0.2f;
    }
    
    return FMath::Clamp(Impact, -1.0f, 1.0f);
}

FStrategySwitchDecision UMingThreeAuthoritiesManager::DecideStrategySwitch(
    EFiveElementPhase CurrentPhase,
    const FSituationAssessment& Assessment)
{
    FStrategySwitchDecision Decision;
    
    // 評估當前策略狀態
    float CurrentStrategyFitness = EvaluateCurrentStrategyFitness();
    
    // 檢查是否需要切換
    if (Assessment.ChaosLevel > 0.7f && Assessment.UrgencyLevel > 0.8f)
    {
        // 緊急混亂情況：需要逆道
        Decision.bShouldSwitch = true;
        Decision.bToRighteous = false;
        Decision.SwitchUrgency = 0.9f;
        Decision.Reasoning = TEXT("局勢緊急混亂，需要逆道破局");
        Decision.Preconditions.Add(TEXT("敵勢正盛"));
        Decision.Preconditions.Add(TEXT("時間窗口短暫"));
    }
    else if (Assessment.EnemyStrength < 0.3f && Assessment.MoralSupport > 0.7f)
    {
        // 優勢明顯：轉向正道
        Decision.bShouldSwitch = true;
        Decision.bToRighteous = true;
        Decision.SwitchUrgency = 0.6f;
        Decision.Reasoning = TEXT("優勢已建立，應轉向正道鞏固");
        Decision.Preconditions.Add(TEXT("敵勢已衰"));
        Decision.Preconditions.Add(TEXT("道德支持充足"));
    }
    else if (CurrentStrategyFitness < 0.3f)
    {
        // 當前策略失效
        Decision.bShouldSwitch = true;
        Decision.bToRighteous = !IsCurrentlyRighteous();
        Decision.SwitchUrgency = 0.8f;
        Decision.Reasoning = TEXT("當前策略效果不佳，需要切換");
    }
    else
    {
        Decision.bShouldSwitch = false;
        Decision.SwitchUrgency = 0.0f;
        Decision.Reasoning = TEXT("當前策略適合，無需切換");
    }
    
    return Decision;
}

float UMingThreeAuthoritiesManager::EvaluateCurrentStrategyFitness() const
{
    // 基於歷史執行效果評估
    float TotalEffectiveness = 0.0f;
    int32 Count = 0;
    
    for (const auto& Order : ExecutionHistory)
    {
        // 使用 ExecutionHistory 中的數據
        // 這裡簡化處理
        Count++;
    }
    
    return Count > 0 ? TotalEffectiveness / Count : 0.5f;
}

ETwelveStrategies UMingThreeAuthoritiesManager::RecommendNextStrategy(
    const FStrategyContext& Context,
    const FSituationAssessment& Assessment) const
{
    // 根據情境推薦策略
    if (Assessment.UrgencyLevel > 0.7f && Assessment.ChaosLevel > 0.6f)
    {
        // 緊急情況：破局
        return ETwelveStrategies::BreakSituation;
    }
    else if (Assessment.EnemyStrength > 0.7f)
    {
        // 敵強：破結構
        return ETwelveStrategies::BreakStructure;
    }
    else if (Assessment.MoralSupport > 0.6f && Assessment.ResourceAvailability > 0.5f)
    {
        // 基礎穩固：立國
        return ETwelveStrategies::EstablishNation;
    }
    else if (Context.CurrentPhase == EFiveElementPhase::Earth)
    {
        // 土階段：立制
        return ETwelveStrategies::EstablishSystem;
    }
    
    return ETwelveStrategies::EstablishPeople;
}

bool UMingThreeAuthoritiesManager::IsSwitchTimingAppropriate() const
{
    // 檢查當前是否適合切換
    // 避免過於頻繁切換
    return true;
}

bool UMingThreeAuthoritiesManager::ExecuteOrder(const FExecutionOrder& Order)
{
    UE_LOG(LogTemp, Log, TEXT("兵權執行命令: %s"), *Order.OrderID);
    
    // 評估執行能力
    float Capability = AssessExecutionCapability(Order);
    
    if (Capability < 0.3f)
    {
        UE_LOG(LogTemp, Warning, TEXT("執行能力不足: %.2f"), Capability);
        return false;
    }
    
    // 添加到執行隊列
    PendingOrders.Add(Order);
    
    UE_LOG(LogTemp, Log, TEXT("✓ 命令已加入執行隊列"));
    
    return true;
}

float UMingThreeAuthoritiesManager::AssessExecutionCapability(const FExecutionOrder& Order) const
{
    float Capability = MilitaryAuthorityHealth;
    
    // 評估資源充足性
    Capability *= Order.Priority;
    
    // 評估複雜度
    if (Order.Targets.Num() > 5)
    {
        Capability *= 0.8f;
    }
    
    return FMath::Clamp(Capability, 0.0f, 1.0f);
}

FExecutionStatus UMingThreeAuthoritiesManager::GetExecutionStatus(const FString& OrderID) const
{
    FExecutionStatus Status;
    Status.OrderID = OrderID;
    
    // 檢查待執行隊列
    for (const auto& Order : PendingOrders)
    {
        if (Order.OrderID == OrderID)
        {
            Status.Status = EExecutionStatus::Pending;
            return Status;
        }
    }
    
    // 檢查執行中隊列
    for (const auto& Order : ExecutingOrders)
    {
        if (Order.OrderID == OrderID)
        {
            Status.Status = EExecutionStatus::Executing;
            return Status;
        }
    }
    
    // 檢查歷史記錄
    if (ExecutionHistory.Contains(OrderID))
    {
        Status.Status = EExecutionStatus::Completed;
    }
    else
    {
        Status.Status = EExecutionStatus::Unknown;
    }
    
    return Status;
}

bool UMingThreeAuthoritiesManager::CancelExecution(const FString& OrderID)
{
    // 從待執行隊列移除
    for (int32 i = PendingOrders.Num() - 1; i >= 0; i--)
    {
        if (PendingOrders[i].OrderID == OrderID)
        {
            PendingOrders.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("取消待執行命令: %s"), *OrderID);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到命令: %s"), *OrderID);
    return false;
}

bool UMingThreeAuthoritiesManager::AreAuthoritiesAligned() const
{
    // 檢查三權是否協調
    float HealthDiff = FMath::Abs(DaoAuthorityHealth - StrategyAuthorityHealth) +
                       FMath::Abs(StrategyAuthorityHealth - MilitaryAuthorityHealth) +
                       FMath::Abs(DaoAuthorityHealth - MilitaryAuthorityHealth);
    
    return HealthDiff < 0.5f;
}

bool UMingThreeAuthoritiesManager::ResolveAuthorityConflict()
{
    UE_LOG(LogTemp, Log, TEXT("解決三權衝突"));
    
    // 平均化健康度
    float AvgHealth = (DaoAuthorityHealth + StrategyAuthorityHealth + MilitaryAuthorityHealth) / 3.0f;
    DaoAuthorityHealth = FMath::Lerp(DaoAuthorityHealth, AvgHealth, 0.3f);
    StrategyAuthorityHealth = FMath::Lerp(StrategyAuthorityHealth, AvgHealth, 0.3f);
    MilitaryAuthorityHealth = FMath::Lerp(MilitaryAuthorityHealth, AvgHealth, 0.3f);
    
    UE_LOG(LogTemp, Log, TEXT("✓ 三權衝突已調和"));
    
    return true;
}

FThreeAuthoritiesReport UMingThreeAuthoritiesManager::GetAuthoritiesReport() const
{
    FThreeAuthoritiesReport Report;
    
    Report.DaoHealth = DaoAuthorityHealth;
    Report.StrategyHealth = StrategyAuthorityHealth;
    Report.MilitaryHealth = MilitaryAuthorityHealth;
    Report.bAreAligned = AreAuthoritiesAligned();
    Report.PendingOrdersCount = PendingOrders.Num();
    Report.ExecutingOrdersCount = ExecutingOrders.Num();
    Report.TotalExecuted = ExecutionHistory.Num();
    
    return Report;
}

float UMingThreeAuthoritiesManager::CalculateMoralRisk(const FCommandAction& Action) const
{
    float Risk = 0.0f;
    
    if (Action.bUsesEvilForce)
    {
        Risk += 0.4f;
    }
    if (Action.bUsesReverseStrategy)
    {
        Risk += 0.3f;
    }
    if (Action.bTargetsCivilians)
    {
        Risk += 0.3f;
    }
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

bool UMingThreeAuthoritiesManager::IsCurrentlyRighteous() const
{
    // 檢查當前是否在使用正道
    // 簡化實現
    return StrategyAuthorityHealth > 0.5f;
}
