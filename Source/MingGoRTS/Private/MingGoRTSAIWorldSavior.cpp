#include "MingGoRTSAIWorldSavior.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

UMingGoRTSAIWorldSavior::UMingGoRTSAIWorldSavior()
    : bWorldSaviorActive(false)
    , bEmergencyActive(false)
    , bMonitoringActive(false)
    , CurrentThreatLevel(EWorldThreatLevel::None)
    , AIEffectiveness(0.85f)
    , MonitoringInterval(5.0f)
    , LastMonitoringTime(0.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化監控計時器
    MonitoringTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAIWorldSavior::OnMonitoringTick);
}

void UMingGoRTSAIWorldSavior::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("AI World Savior System Initialized"));
    
    // 自動啟動世界監控
    StartWorldMonitoring();
}

void UMingGoRTSAIWorldSavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (bWorldSaviorActive)
    {
        // 持續監控世界狀態
        if (bMonitoringActive)
        {
            ProcessWorldThreats();
            UpdateWorldStatus();
        }
        
        // 優化資源分配
        if (FMath::FRand() < 0.01f) // 1%機率每幀執行
        {
            OptimizeResourceAllocation();
        }
    }
}

void UMingGoRTSAIWorldSavior::StartWorldMonitoring()
{
    if (bMonitoringActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("World monitoring is already active"));
        return;
    }

    bMonitoringActive = true;
    
    // 啟動監控計時器
    if (!MonitoringTickerHandle.IsValid())
    {
        MonitoringTickerHandle = FTicker::GetCoreTicker().AddTicker(MonitoringTicker, MonitoringInterval);
    }

    UE_LOG(LogTemp, Log, TEXT("Started world monitoring"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AI World Savior: Monitoring Started"));
    }
}

void UMingGoRTSAIWorldSavior::StopWorldMonitoring()
{
    bMonitoringActive = false;
    
    // 停止監控計時器
    if (MonitoringTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(MonitoringTickerHandle);
        MonitoringTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Stopped world monitoring"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AI World Savior: Monitoring Stopped"));
    }
}

void UMingGoRTSAIWorldSavior::ScanForThreats()
{
    UE_LOG(LogTemp, Log, TEXT("Scanning for world threats..."));
    
    // 模擬威脅檢測
    if (FMath::FRand() < 0.3f) // 30%機率檢測到威脅
    {
        FWorldThreatData NewThreat;
        NewThreat.ThreatName = GenerateRandomThreatName();
        NewThreat.ThreatLevel = static_cast<EWorldThreatLevel>(FMath::RandRange(1, 5));
        NewThreat.ThreatDescription = GenerateRandomThreatDescription();
        NewThreat.ThreatLocation = FVector2D(FMath::FRand() * 1000, FMath::FRand() * 1000);
        NewThreat.ThreatRadius = FMath::FRandRange(50, 200);
        NewThreat.TimeToImpact = FMath::FRandRange(10, 300);
        NewThreat.Confidence = FMath::FRandRange(0.6, 1.0);
        NewThreat.RecommendedStrategy = GenerateRecommendedStrategy(NewThreat.ThreatLevel);
        
        DetectedThreats.Add(NewThreat);
        
        NotifyWorldThreat(NewThreat);
        
        UE_LOG(LogTemp, Warning, TEXT("Threat detected: %s (Level: %d)"), *NewThreat.ThreatName, (int32)NewThreat.ThreatLevel);
    }
}

void UMingGoRTSAIWorldSavior::GenerateSaviorStrategy(const FWorldThreatData& Threat)
{
    UE_LOG(LogTemp, Log, TEXT("Generating savior strategy for threat: %s"), *Threat.ThreatName);
    
    // 根據威脅類型和級別生成策略
    ESaviorStrategy Strategy = Threat.RecommendedStrategy;
    
    // 生成行動計劃
    GenerateActionPlan(Threat, Strategy);
}

void UMingGoRTSAIWorldSavior::GenerateActionPlan(const FWorldThreatData& Threat, ESaviorStrategy Strategy)
{
    FSaviorActionPlan NewPlan;
    NewPlan.PlanName = FString::Printf(TEXT("Response Plan for %s"), *Threat.ThreatName);
    NewPlan.Strategy = Strategy;
    NewPlan.EstimatedSuccessRate = CalculateSuccessRate(Threat, Strategy);
    NewPlan.ExecutionTime = EstimateExecutionTime(Threat, Strategy);
    
    // 生成行動步驟
    GenerateActionSteps(NewPlan, Threat, Strategy);
    
    // 計算所需資源
    CalculateRequiredResources(NewPlan, Threat);
    
    // 評估潛在風險
    AssessPotentialRisks(NewPlan, Threat);
    
    AvailablePlans.Add(NewPlan);
    
    NotifyPlanGenerated(NewPlan);
    
    UE_LOG(LogTemp, Log, TEXT("Generated action plan: %s (Success Rate: %.1f%%)"), 
        *NewPlan.PlanName, NewPlan.EstimatedSuccessRate * 100);
}

void UMingGoRTSAIWorldSavior::ExecuteActionPlan(const FSaviorActionPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing action plan: %s"), *Plan.PlanName);
    
    // 模擬執行過程
    bool bSuccess = FMath::FRand() < Plan.EstimatedSuccessRate;
    
    // 更新AI效果評分
    if (bSuccess)
    {
        AIEffectiveness = FMath::Clamp(AIEffectiveness + 0.05f, 0.0f, 1.0f);
    }
    else
    {
        AIEffectiveness = FMath::Clamp(AIEffectiveness - 0.02f, 0.0f, 1.0f);
    }
    
    NotifyMissionCompleted(bSuccess);
    
    if (GEngine)
    {
        FColor MessageColor = bSuccess ? FColor::Green : FColor::Red;
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, MessageColor, 
            FString::Printf(TEXT("Mission %s: %s"), bSuccess ? TEXT("Success") : TEXT("Failed"), *Plan.PlanName));
    }
}

void UMingGoRTSAIWorldSavior::OptimizeResources()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing global resources..."));
    
    // 分析當前資源狀態
    AnalyzeResourceStatus();
    
    // 優化分配策略
    OptimizeAllocationStrategy();
    
    // 更新資源狀態
    UpdateResourceStatus();
}

void UMingGoRTSAIWorldSavior::AllocateResources(const FString& ResourceType, float Amount, EResourcePriority Priority)
{
    FResourceOptimization ResourceData;
    ResourceData.ResourceType = ResourceType;
    ResourceData.CurrentAmount = Amount;
    ResourceData.Priority = Priority;
    ResourceData.Efficiency = FMath::FRandRange(0.7, 1.0);
    
    // 生成分配策略
    ResourceData.AllocationStrategy = GenerateAllocationStrategy(Priority);
    
    ResourceStatus.Add(ResourceData);
    
    NotifyResourceOptimized(ResourceData);
    
    UE_LOG(LogTemp, Log, TEXT("Allocated %.2f units of %s (Priority: %d)"), Amount, *ResourceType, (int32)Priority);
}

void UMingGoRTSAIWorldSavior::RedistributeResources(const TArray<FString>& CriticalNeeds)
{
    UE_LOG(LogTemp, Log, TEXT("Redistributing resources for critical needs..."));
    
    for (const FString& Need : CriticalNeeds)
    {
        // 為每個關鍵需求重新分配資源
        float AllocationAmount = CalculateOptimalAllocation(Need);
        AllocateResources(Need, AllocationAmount, EResourcePriority::Critical);
    }
}

void UMingGoRTSAIWorldSavior::PredictFutureThreats(float TimeHorizon)
{
    UE_LOG(LogTemp, Log, TEXT("Predicting threats for next %.1f time units"), TimeHorizon);
    
    PredictedThreats.Empty();
    
    // 基於歷史數據和當前趨勢預測未來威脅
    int32 PredictionCount = FMath::RandRange(1, 5);
    
    for (int32 i = 0; i < PredictionCount; ++i)
    {
        FWorldThreatData PredictedThreat;
        PredictedThreat.ThreatName = FString::Printf(TEXT("Predicted Threat %d"), i + 1);
        PredictedThreat.ThreatLevel = static_cast<EWorldThreatLevel>(FMath::RandRange(1, 4));
        PredictedThreat.ThreatDescription = TEXT("AI-predicted future threat");
        PredictedThreat.TimeToImpact = FMath::FRandRange(TimeHorizon * 0.1, TimeHorizon);
        PredictedThreat.Confidence = FMath::FRandRange(0.4, 0.8);
        
        PredictedThreats.Add(PredictedThreat);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d threat predictions"), PredictedThreats.Num());
}

void UMingGoRTSAIWorldSavior::AnalyzeThreatPatterns()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing threat patterns..."));
    
    // 分析威脅模式
    TArray<FString> Patterns;
    
    if (DetectedThreats.Num() > 2)
    {
        // 檢測重複模式
        Patterns.Add(TEXT("Increasing threat frequency detected"));
        Patterns.Add(TEXT("Geographic clustering observed"));
        Patterns.Add(TEXT("Escalating threat levels"));
    }
    
    // 更新策略基於模式分析
    for (const FString& Pattern : Patterns)
    {
        UpdateStrategyBasedOnPattern(Pattern);
    }
}

float UMingGoRTSAIWorldSavior::CalculateWorldStability() const
{
    float Stability = 1.0f;
    
    // 根據威脅級別計算穩定性
    for (const FWorldThreatData& Threat : DetectedThreats)
    {
        float ThreatImpact = (float)Threat.ThreatLevel / 5.0f * Threat.Confidence;
        Stability -= ThreatImpact;
    }
    
    // 根據資源狀態調整穩定性
    for (const FResourceOptimization& Resource : ResourceStatus)
    {
        if (Resource.Priority == EResourcePriority::Critical && Resource.CurrentAmount < Resource.RequiredAmount)
        {
            Stability -= 0.1f;
        }
    }
    
    return FMath::Clamp(Stability, 0.0f, 1.0f);
}

void UMingGoRTSAIWorldSavior::RequestDecisionSupport(const FWorldThreatData& Threat)
{
    UE_LOG(LogTemp, Log, TEXT("Requesting AI decision support for: %s"), *Threat.ThreatName);
    
    // 生成決策建議
    GenerateRecommendations();
    
    // 創建多個行動選項
    TArray<FSaviorActionPlan> Options;
    for (int32 i = 0; i < 3; ++i)
    {
        ESaviorStrategy Strategy = static_cast<ESaviorStrategy>(FMath::RandRange(0, 7));
        GenerateActionPlan(Threat, Strategy);
    }
}

void UMingGoRTSAIWorldSavior::GenerateRecommendations()
{
    Recommendations.Empty();
    
    // 基於當前狀況生成建議
    Recommendations.Add(TEXT("Increase resource allocation to critical areas"));
    Recommendations.Add(TEXT("Deploy additional monitoring units"));
    Recommendations.Add(TEXT("Strengthen diplomatic channels"));
    Recommendations.Add(TEXT("Prepare contingency plans"));
    
    if (CurrentThreatLevel >= EWorldThreatLevel::High)
    {
        Recommendations.Add(TEXT("Activate emergency protocols"));
        Recommendations.Add(TEXT("Mobilize all available resources"));
    }
}

void UMingGoRTSAIWorldSavior::EvaluatePlanEffectiveness(const FSaviorActionPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Evaluating plan effectiveness: %s"), *Plan.PlanName);
    
    // 模擬效果評估
    float Effectiveness = FMath::FRandRange(0.5, 1.0);
    
    // 更新AI學習數據
    UpdateAIEffectiveness(Effectiveness);
}

void UMingGoRTSAIWorldSavior::CreateLongTermStrategy()
{
    UE_LOG(LogTemp, Log, TEXT("Creating long-term world saving strategy..."));
    
    StrategicGoals.Empty();
    
    // 生成戰略目標
    StrategicGoals.Add(TEXT("Achieve global threat detection rate > 95%"));
    StrategicGoals.Add(TEXT("Maintain resource efficiency > 85%"));
    StrategicGoals.Add(TEXT("Reduce average response time by 50%"));
    StrategicGoals.Add(TEXT("Establish global cooperation network"));
    StrategicGoals.Add(TEXT("Develop predictive threat prevention system"));
}

void UMingGoRTSAIWorldSavior::UpdateStrategicGoals()
{
    UE_LOG(LogTemp, Log, TEXT("Updating strategic goals based on current performance..."));
    
    // 根據AI效果調整戰略目標
    if (AIEffectiveness > 0.9)
    {
        StrategicGoals.Add(TEXT("Expand monitoring coverage to global scale"));
        StrategicGoals.Add(TEXT("Implement automated response systems"));
    }
    else if (AIEffectiveness < 0.7)
    {
        StrategicGoals.Add(TEXT("Improve threat detection accuracy"));
        StrategicGoals.Add(TEXT("Enhance resource optimization algorithms"));
    }
}

void UMingGoRTSAIWorldSavior::MonitorStrategicProgress()
{
    UE_LOG(LogTemp, Log, TEXT("Monitoring strategic progress..."));
    
    // 評估戰略目標進度
    for (const FString& Goal : StrategicGoals)
    {
        float Progress = CalculateGoalProgress(Goal);
        UE_LOG(LogTemp, Log, TEXT("Goal: %s - Progress: %.1f%%"), *Goal, Progress * 100);
    }
}

void UMingGoRTSAIWorldSavior::ActivateEmergencyProtocol()
{
    if (bEmergencyActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Emergency protocol already active"));
        return;
    }
    
    bEmergencyActive = true;
    
    UE_LOG(LogTemp, Warning, TEXT("EMERGENCY PROTOCOL ACTIVATED"));
    
    // 生成緊急計劃
    GenerateEmergencyPlans();
    
    // 部署緊急資源
    DeployEmergencyResources();
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("🚨 EMERGENCY PROTOCOL ACTIVATED 🚨"));
    }
}

void UMingGoRTSAIWorldSavior::DeployEmergencyResources()
{
    UE_LOG(LogTemp, Log, TEXT("Deploying emergency resources..."));
    
    // 分配所有可用資源
    TArray<FString> EmergencyNeeds = {
        TEXT("Medical Supplies"),
        TEXT("Food and Water"),
        TEXT("Shelter"),
        TEXT("Security Forces"),
        TEXT("Communication Systems")
    };
    
    for (const FString& Need : EmergencyNeeds)
    {
        AllocateResources(Need, 1000.0f, EResourcePriority::Critical);
    }
}

void UMingGoRTSAIWorldSavior::CoordinateEmergencyResponse()
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating global emergency response..."));
    
    // 協調全球響應
    CoordinateGlobalResponse();
    
    // 更新世界狀態
    UpdateWorldStatus();
}

void UMingGoRTSAIWorldSavior::LearnFromMissionResults(bool bSuccess, const FString& MissionData)
{
    UE_LOG(LogTemp, Log, TEXT("Learning from mission results (Success: %s)"), bSuccess ? TEXT("true") : TEXT("false"));
    
    // 更新AI知識庫
    UpdateAIKnowledge();
    
    // 調整策略
    if (bSuccess)
    {
        AIEffectiveness = FMath::Clamp(AIEffectiveness + 0.02f, 0.0f, 1.0f);
    }
    else
    {
        AIEffectiveness = FMath::Clamp(AIEffectiveness - 0.01f, 0.0f, 1.0f);
        AdaptStrategies();
    }
}

void UMingGoRTSAIWorldSavior::AdaptStrategies()
{
    UE_LOG(LogTemp, Log, TEXT("Adapting strategies based on performance..."));
    
    // 基於失敗經驗調整策略
    for (FSaviorActionPlan& Plan : AvailablePlans)
    {
        Plan.EstimatedSuccessRate = FMath::Clamp(Plan.EstimatedSuccessRate * 0.9f, 0.1f, 1.0f);
    }
}

void UMingGoRTSAIWorldSavior::UpdateAIKnowledge()
{
    UE_LOG(LogTemp, Log, TEXT("Updating AI knowledge base..."));
    
    // 模擬知識更新
    AIEffectiveness = FMath::Clamp(AIEffectiveness + 0.01f, 0.0f, 1.0f);
}

void UMingGoRTSAIWorldSavior::ActivateWorldSaviorMode()
{
    if (bWorldSaviorActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("World Savior mode already active"));
        return;
    }
    
    bWorldSaviorActive = true;
    
    UE_LOG(LogTemp, Log, TEXT("🌍 AI WORLD SAVIOR MODE ACTIVATED 🌍"));
    
    // 啟動所有系統
    StartWorldMonitoring();
    OptimizeResources();
    CreateLongTermStrategy();
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("🌍 AI WORLD SAVIOR MODE ACTIVATED 🌍"));
    }
}

void UMingGoRTSAIWorldSavior::DeactivateWorldSaviorMode()
{
    bWorldSaviorActive = false;
    bEmergencyActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("World Savior mode deactivated"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Gray, TEXT("World Savior Mode Deactivated"));
    }
}

void UMingGoRTSAIWorldSavior::ResetWorldSaviorSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Resetting World Savior system..."));
    
    // 重置所有數據
    DetectedThreats.Empty();
    PredictedThreats.Empty();
    AvailablePlans.Empty();
    ResourceStatus.Empty();
    Recommendations.Empty();
    StrategicGoals.Empty();
    
    // 重置狀態
    CurrentThreatLevel = EWorldThreatLevel::None;
    AIEffectiveness = 0.85f;
    bEmergencyActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("World Savior system reset completed"));
}

// Private helper functions
void UMingGoRTSAIWorldSavior::ProcessWorldThreats()
{
    // 處理檢測到的威脅
    for (const FWorldThreatData& Threat : DetectedThreats)
    {
        if (Threat.TimeToImpact <= 0)
        {
            // 威脅即將發生，啟動緊急響應
            ActivateEmergencyProtocol();
            break;
        }
    }
}

void UMingGoRTSAIWorldSavior::UpdateWorldStatus()
{
    // 計算當前威脅級別
    EWorldThreatLevel NewThreatLevel = EWorldThreatLevel::None;
    
    for (const FWorldThreatData& Threat : DetectedThreats)
    {
        if (Threat.ThreatLevel > NewThreatLevel)
        {
            NewThreatLevel = Threat.ThreatLevel;
        }
    }
    
    if (NewThreatLevel != CurrentThreatLevel)
    {
        CurrentThreatLevel = NewThreatLevel;
        NotifyWorldStatusChange(CurrentThreatLevel, GenerateStatusMessage(CurrentThreatLevel));
    }
}

void UMingGoRTSAIWorldSavior::GenerateEmergencyPlans()
{
    // 為所有檢測到的威脅生成緊急計劃
    for (const FWorldThreatData& Threat : DetectedThreats)
    {
        if (Threat.ThreatLevel >= EWorldThreatLevel::High)
        {
            GenerateActionPlan(Threat, ESaviorStrategy::Defensive);
            GenerateActionPlan(Threat, ESaviorStrategy::Humanitarian);
        }
    }
}

void UMingGoRTSAIWorldSavior::OptimizeResourceAllocation()
{
    // 優化資源分配算法
    for (FResourceOptimization& Resource : ResourceStatus)
    {
        if (Resource.Priority == EResourcePriority::Critical)
        {
            Resource.Efficiency = FMath::Clamp(Resource.Efficiency + 0.01f, 0.0f, 1.0f);
        }
    }
}

void UMingGoRTSAIWorldSavior::AnalyzeGlobalSituation()
{
    // 分析全球狀況
    float WorldStability = CalculateWorldStability();
    
    if (WorldStability < 0.3)
    {
        ActivateEmergencyProtocol();
    }
}

void UMingGoRTSAIWorldSavior::CoordinateGlobalResponse()
{
    // 協調全球響應
    UE_LOG(LogTemp, Log, TEXT("Coordinating international response efforts..."));
}

void UMingGoRTSAIWorldSavior::UpdateAIParameters()
{
    // 更新AI參數
    AIEffectiveness = FMath::Clamp(AIEffectiveness + 0.001f, 0.0f, 1.0f);
}

bool UMingGoRTSAIWorldSavior::OnMonitoringTick(float DeltaTime)
{
    if (!bMonitoringActive)
    {
        return false;
    }
    
    // 執行監控任務
    ScanForThreats();
    AnalyzeGlobalSituation();
    
    return bMonitoringActive;
}

void UMingGoRTSAIWorldSavior::NotifyWorldThreat(const FWorldThreatData& Threat)
{
    OnWorldThreatDetected.Broadcast(Threat);
}

void UMingGoRTSAIWorldSavior::NotifyPlanGenerated(const FSaviorActionPlan& Plan)
{
    OnSaviorPlanGenerated.Broadcast(Plan);
}

void UMingGoRTSAIWorldSavior::NotifyResourceOptimized(const FResourceOptimization& Resource)
{
    OnResourceOptimized.Broadcast(Resource);
}

void UMingGoRTSAIWorldSavior::NotifyWorldStatusChange(EWorldThreatLevel NewLevel, const FString& Message)
{
    OnWorldStatusChanged.Broadcast(NewLevel, Message);
}

void UMingGoRTSAIWorldSavior::NotifyMissionCompleted(bool bSuccess)
{
    OnSaviorMissionCompleted.Broadcast(bSuccess);
}

// Helper function implementations
FString UMingGoRTSAIWorldSavior::GenerateRandomThreatName()
{
    TArray<FString> ThreatNames = {
        TEXT("Climate Crisis"),
        TEXT("Pandemic Outbreak"),
        TEXT("Natural Disaster"),
        TEXT("Resource Shortage"),
        TEXT("Technological Failure"),
        TEXT("Political Instability"),
        TEXT("Economic Collapse"),
        TEXT("Environmental Pollution")
    };
    
    return ThreatNames[FMath::RandRange(0, ThreatNames.Num() - 1)];
}

FString UMingGoRTSAIWorldSavior::GenerateRandomThreatDescription()
{
    TArray<FString> Descriptions = {
        TEXT("Critical situation requiring immediate intervention"),
        TEXT("Developing threat with potential global impact"),
        TEXT("Localized emergency with regional consequences"),
        TEXT("Systemic risk affecting multiple sectors"),
        TEXT("Cascading failure scenario detected")
    };
    
    return Descriptions[FMath::RandRange(0, Descriptions.Num() - 1)];
}

ESaviorStrategy UMingGoRTSAIWorldSavior::GenerateRecommendedStrategy(EWorldThreatLevel ThreatLevel)
{
    switch (ThreatLevel)
    {
    case EWorldThreatLevel::Low:
        return ESaviorStrategy::Diplomatic;
    case EWorldThreatLevel::Medium:
        return ESaviorStrategy::Scientific;
    case EWorldThreatLevel::High:
        return ESaviorStrategy::Defensive;
    case EWorldThreatLevel::Critical:
        return ESaviorStrategy::Humanitarian;
    case EWorldThreatLevel::Catastrophic:
        return ESaviorStrategy::Emergency;
    default:
        return ESaviorStrategy::Diplomatic;
    }
}

float UMingGoRTSAIWorldSavior::CalculateSuccessRate(const FWorldThreatData& Threat, ESaviorStrategy Strategy)
{
    float BaseRate = 0.7f;
    
    // 根據威脅級別調整
    BaseRate -= (float)Threat.ThreatLevel * 0.1f;
    
    // 根據策略調整
    switch (Strategy)
    {
    case ESaviorStrategy::Technological:
        BaseRate += 0.2f;
        break;
    case ESaviorStrategy::Scientific:
        BaseRate += 0.15f;
        break;
    case ESaviorStrategy::Diplomatic:
        BaseRate += 0.1f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseRate, 0.1f, 0.95f);
}

float UMingGoRTSAIWorldSavior::EstimateExecutionTime(const FWorldThreatData& Threat, ESaviorStrategy Strategy)
{
    float BaseTime = 60.0f; // 基礎時間（分鐘）
    
    // 根據威脅級別調整
    BaseTime *= (1.0f + (float)Threat.ThreatLevel * 0.5f);
    
    // 根據策略調整
    switch (Strategy)
    {
    case ESaviorStrategy::Technological:
        BaseTime *= 0.8f;
        break;
    case ESaviorStrategy::Diplomatic:
        BaseTime *= 1.5f;
        break;
    case ESaviorStrategy::Military:
        BaseTime *= 0.6f;
        break;
    default:
        break;
    }
    
    return BaseTime;
}

void UMingGoRTSAIWorldSavior::GenerateActionSteps(FSaviorActionPlan& Plan, const FWorldThreatData& Threat, ESaviorStrategy Strategy)
{
    Plan.ActionSteps.Empty();
    
    // 根據策略生成行動步驟
    switch (Strategy)
    {
    case ESaviorStrategy::Defensive:
        Plan.ActionSteps.Add(TEXT("Establish defensive perimeter"));
        Plan.ActionSteps.Add(TEXT("Deploy protective measures"));
        Plan.ActionSteps.Add(TEXT("Monitor threat evolution"));
        break;
    case ESaviorStrategy::Humanitarian:
        Plan.ActionSteps.Add(TEXT("Deploy medical teams"));
        Plan.ActionSteps.Add(TEXT("Establish relief centers"));
        Plan.ActionSteps.Add(TEXT("Coordinate evacuation efforts"));
        break;
    case ESaviorStrategy::Technological:
        Plan.ActionSteps.Add(TEXT("Deploy technical solutions"));
        Plan.ActionSteps.Add(TEXT("Activate automated systems"));
        Plan.ActionSteps.Add(TEXT("Monitor system performance"));
        break;
    default:
        Plan.ActionSteps.Add(TEXT("Assess situation"));
        Plan.ActionSteps.Add(TEXT("Deploy resources"));
        Plan.ActionSteps.Add(TEXT("Monitor progress"));
        break;
    }
}

void UMingGoRTSAIWorldSavior::CalculateRequiredResources(FSaviorActionPlan& Plan, const FWorldThreatData& Threat)
{
    Plan.RequiredResources.Empty();
    
    // 基於威脅級別計算所需資源
    TArray<FString> BaseResources = {
        TEXT("Personnel"),
        TEXT("Equipment"),
        TEXT("Supplies"),
        TEXT("Communication"),
        TEXT("Transportation")
    };
    
    for (const FString& Resource : BaseResources)
    {
        Plan.RequiredResources.Add(Resource);
    }
    
    // 根據威脅類型添加特定資源
    if (Threat.ThreatLevel >= EWorldThreatLevel::High)
    {
        Plan.RequiredResources.Add(TEXT("Emergency Response Teams"));
        Plan.RequiredResources.Add(TEXT("Advanced Technology"));
    }
}

void UMingGoRTSAIWorldSavior::AssessPotentialRisks(FSaviorActionPlan& Plan, const FWorldThreatData& Threat)
{
    Plan.PotentialRisks.Empty();
    Plan.MitigationStrategies.Empty();
    
    // 評估潛在風險
    Plan.PotentialRisks.Add(TEXT("Resource shortage"));
    Plan.PotentialRisks.Add(TEXT("Communication failure"));
    Plan.PotentialRisks.Add(TEXT("Unexpected escalation"));
    
    // 生成緩解策略
    Plan.MitigationStrategies.Add(TEXT("Establish backup supply lines"));
    Plan.MitigationStrategies.Add(TEXT("Deploy redundant communication systems"));
    Plan.MitigationStrategies.Add(TEXT("Prepare contingency plans"));
}

// Additional helper functions
void UMingGoRTSAIWorldSavior::AnalyzeResourceStatus()
{
    // 分析資源狀態
    for (FResourceOptimization& Resource : ResourceStatus)
    {
        Resource.Efficiency = FMath::FRandRange(0.6, 1.0);
    }
}

void UMingGoRTSAIWorldSavior::OptimizeAllocationStrategy()
{
    // 優化分配策略
    for (FResourceOptimization& Resource : ResourceStatus)
    {
        if (Resource.Priority == EResourcePriority::Critical)
        {
            Resource.AllocationStrategy = TEXT("Priority allocation with backup reserves");
        }
        else
        {
            Resource.AllocationStrategy = TEXT("Standard allocation protocol");
        }
    }
}

void UMingGoRTSAIWorldSavior::UpdateResourceStatus()
{
    // 更新資源狀態
    for (FResourceOptimization& Resource : ResourceStatus)
    {
        Resource.CurrentAmount *= FMath::FRandRange(0.95, 1.05);
    }
}

FString UMingGoRTSAIWorldSavior::GenerateAllocationStrategy(EResourcePriority Priority)
{
    switch (Priority)
    {
    case EResourcePriority::Critical:
        return TEXT("Immediate allocation with top priority");
    case EResourcePriority::High:
        return TEXT("High priority allocation");
    case EResourcePriority::Medium:
        return TEXT("Standard allocation procedure");
    case EResourcePriority::Low:
        return TEXT("Low priority allocation");
    case EResourcePriority::Optional:
        return TEXT("Optional allocation if resources available");
    default:
        return TEXT("Standard allocation");
    }
}

float UMingGoRTSAIWorldSavior::CalculateOptimalAllocation(const FString& Need)
{
    // 計算最優分配量
    return FMath::FRandRange(100, 1000);
}

void UMingGoRTSAIWorldSavior::UpdateStrategyBasedOnPattern(const FString& Pattern)
{
    UE_LOG(LogTemp, Log, TEXT("Updating strategy based on pattern: %s"), *Pattern);
    
    // 根據模式更新策略
    if (Pattern.Contains(TEXT("Increasing")))
    {
        Recommendations.Add(TEXT("Increase monitoring frequency"));
    }
    else if (Pattern.Contains(TEXT("Clustering")))
    {
        Recommendations.Add(TEXT("Focus on regional coordination"));
    }
}

void UMingGoRTSAIWorldSavior::UpdateAIEffectiveness(float Effectiveness)
{
    AIEffectiveness = FMath::Clamp((AIEffectiveness + Effectiveness) / 2.0f, 0.0f, 1.0f);
}

float UMingGoRTSAIWorldSavior::CalculateGoalProgress(const FString& Goal)
{
    // 計算目標進度
    return FMath::FRandRange(0.3, 0.9);
}

FString UMingGoRTSAIWorldSavior::GenerateStatusMessage(EWorldThreatLevel Level)
{
    switch (Level)
    {
    case EWorldThreatLevel::None:
        return TEXT("World Status: Normal - No threats detected");
    case EWorldThreatLevel::Low:
        return TEXT("World Status: Caution - Minor threats detected");
    case EWorldThreatLevel::Medium:
        return TEXT("World Status: Alert - Moderate threats detected");
    case EWorldThreatLevel::High:
        return TEXT("World Status: Warning - High threats detected");
    case EWorldThreatLevel::Critical:
        return TEXT("World Status: Critical - Severe threats detected");
    case EWorldThreatLevel::Catastrophic:
        return TEXT("World Status: Catastrophic - Extreme threats detected");
    default:
        return TEXT("World Status: Unknown");
    }
}
