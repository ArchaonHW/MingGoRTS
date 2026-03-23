#include "MingSupremeSageCommandSystem.h"
#include "MingThreeAuthoritiesManager.h"
#include "MingFiveElementsRotation.h"
#include "MingCorruptionPrevention.h"
#include "MingSixConquestExecutor.h"
#include "MingTwelveStrategiesExecutor.h"
#include "Engine/World.h"

UMingSupremeSageCommandSystem::UMingSupremeSageCommandSystem()
    : SystemStatus(ECommandStatus::Terminated)
    , CurrentAuthority(ESupremeAuthorityType::None)
    , CurrentPhase(EFiveElementPhase::None)
    , TotalCommandsIssued(0)
    , EvilForceUsageCount(0)
    , PhaseTransitionCount(0)
    , TotalDecisionQualityScore(0.0f)
    , AuthoritiesManager(nullptr)
    , FiveElementsRotation(nullptr)
    , CorruptionPrevention(nullptr)
    , SixConquestExecutor(nullptr)
    , TwelveStrategiesExecutor(nullptr)
{
}

bool UMingSupremeSageCommandSystem::InitializeSystem()
{
    UE_LOG(LogTemp, Log, TEXT("=== 至聖者指揮系統初始化 ==="));
    
    // 初始化三權
    if (!InitializeThreeAuthorities())
    {
        UE_LOG(LogTemp, Error, TEXT("三權初始化失敗"));
        return false;
    }
    
    // 初始化子系統
    AuthoritiesManager = NewObject<UMingThreeAuthoritiesManager>(this);
    FiveElementsRotation = NewObject<UMingFiveElementsRotation>(this);
    CorruptionPrevention = NewObject<UMingCorruptionPrevention>(this);
    SixConquestExecutor = NewObject<UMingSixConquestExecutor>(this);
    TwelveStrategiesExecutor = NewObject<UMingTwelveStrategiesExecutor>(this);
    
    if (!AuthoritiesManager || !FiveElementsRotation || !CorruptionPrevention ||
        !SixConquestExecutor || !TwelveStrategiesExecutor)
    {
        UE_LOG(LogTemp, Error, TEXT("子系統創建失敗"));
        return false;
    }
    
    AuthoritiesManager->Initialize();
    FiveElementsRotation->Initialize();
    CorruptionPrevention->Initialize();
    SixConquestExecutor->Initialize();
    TwelveStrategiesExecutor->Initialize();
    
    // 初始化權限狀態
    AuthorityStatuses.Empty();
    for (int32 i = 0; i < 3; i++)
    {
        FAuthorityStatus Status;
        Status.AuthorityType = static_cast<ESupremeAuthorityType>(i);
        Status.bIsActive = (i == 0);  // 預設道權啟動
        AuthorityStatuses.Add(Status);
    }
    
    SystemStatus = ECommandStatus::Active;
    LastAuditTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("✓ 至聖者指揮系統初始化完成"));
    LogSystemEvent(TEXT("系統初始化完成"));
    
    return true;
}

void UMingSupremeSageCommandSystem::ShutdownSystem()
{
    UE_LOG(LogTemp, Log, TEXT("=== 至聖者指揮系統關閉 ==="));
    
    // 執行最終防墮檢查
    FCorruptionCheckResult FinalCheck = PerformCorruptionCheck();
    if (FinalCheck.OverallCorruptionRisk > 0.5f)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 系統關閉時檢測到墮落風險: %.2f"), 
            FinalCheck.OverallCorruptionRisk);
    }
    
    // 封存邪兵
    if (FiveElementsRotation)
    {
        FiveElementsRotation->SealEvilForces();
    }
    
    // 生成最終審核報告
    FSelfAuditReport FinalReport = GenerateSelfAuditReport();
    UE_LOG(LogTemp, Log, TEXT("系統關閉 - 總命令數: %d, 邪兵使用: %d, 階段轉換: %d"),
        FinalReport.TotalCommandsIssued,
        FinalReport.EvilForceUsages,
        FinalReport.PhaseTransitions);
    
    SystemStatus = ECommandStatus::Terminated;
    LogSystemEvent(TEXT("系統關閉"));
}

bool UMingSupremeSageCommandSystem::InitializeThreeAuthorities()
{
    UE_LOG(LogTemp, Log, TEXT("初始化三權體系..."));
    
    // 道權 - 初始為監督狀態
    CurrentAuthority = ESupremeAuthorityType::DaoAuthority;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 三權體系初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  - 道權: 防墮監督"));
    UE_LOG(LogTemp, Log, TEXT("  - 策權: 正逆切換"));
    UE_LOG(LogTemp, Log, TEXT("  - 兵權: 執行載體"));
    
    return true;
}

FAuthorityStatus UMingSupremeSageCommandSystem::GetAuthorityStatus(
    ESupremeAuthorityType AuthorityType) const
{
    for (const auto& Status : AuthorityStatuses)
    {
        if (Status.AuthorityType == AuthorityType)
        {
            return Status;
        }
    }
    return FAuthorityStatus();
}

bool UMingSupremeSageCommandSystem::SwitchToAuthority(ESupremeAuthorityType AuthorityType)
{
    if (AuthorityType == ESupremeAuthorityType::None)
    {
        return false;
    }
    
    ESupremeAuthorityType PreviousAuthority = CurrentAuthority;
    CurrentAuthority = AuthorityType;
    
    // 更新狀態
    for (auto& Status : AuthorityStatuses)
    {
        Status.bIsActive = (Status.AuthorityType == AuthorityType);
        if (Status.bIsActive)
        {
            Status.LastAction = TEXT("接管指揮權");
            Status.LastCheckTime = FDateTime::Now();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("三權切換: %s → %s"),
        *UEnum::GetDisplayValueAsText(PreviousAuthority).ToString(),
        *UEnum::GetDisplayValueAsText(AuthorityType).ToString());
    
    OnAuthoritySwitched.Broadcast(PreviousAuthority, AuthorityType);
    LogSystemEvent(FString::Printf(TEXT("切換至%s"), 
        *UEnum::GetDisplayValueAsText(AuthorityType).ToString()));
    
    return true;
}

bool UMingSupremeSageCommandSystem::UpdateAuthorityHealth(
    ESupremeAuthorityType AuthorityType, float NewHealth)
{
    for (auto& Status : AuthorityStatuses)
    {
        if (Status.AuthorityType == AuthorityType)
        {
            Status.HealthLevel = FMath::Clamp(NewHealth, 0.0f, 1.0f);
            return true;
        }
    }
    return false;
}

bool UMingSupremeSageCommandSystem::EnterPhase(
    EFiveElementPhase Phase, const FPhaseContext& Context)
{
    if (Phase == EFiveElementPhase::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("無法進入無效階段"));
        return false;
    }
    
    // 檢查是否可以轉換
    if (!CanTransitionToPhase(Phase))
    {
        UE_LOG(LogTemp, Warning, TEXT("當前不適合轉換至%s階段"),
            *UEnum::GetDisplayValueAsText(Phase).ToString());
        return false;
    }
    
    // 結束當前階段
    if (CurrentPhase != EFiveElementPhase::None)
    {
        UE_LOG(LogTemp, Log, TEXT("結束%s階段"),
            *UEnum::GetDisplayValueAsText(CurrentPhase).ToString());
    }
    
    EFiveElementPhase PreviousPhase = CurrentPhase;
    CurrentPhase = Phase;
    CurrentPhaseContext = Context;
    CurrentPhaseContext.StartTime = FDateTime::Now();
    PhaseTransitionCount++;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入%s階段 ==="),
        *UEnum::GetDisplayValueAsText(Phase).ToString());
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Context.PhaseName);
    UE_LOG(LogTemp, Log, TEXT("主要兵種: %s"),
        *UEnum::GetDisplayValueAsText(Context.PrimaryForce).ToString());
    
    OnPhaseChanged.Broadcast(Phase);
    LogSystemEvent(FString::Printf(TEXT("進入%s階段: %s"),
        *UEnum::GetDisplayValueAsText(Phase).ToString(),
        *Context.PhaseName));
    
    return true;
}

bool UMingSupremeSageCommandSystem::TransitionToNextPhase()
{
    EFiveElementPhase NextPhase = DetermineNextPhase();
    
    if (NextPhase == EFiveElementPhase::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("無法確定下一階段"));
        return false;
    }
    
    FPhaseContext Context;
    Context.PhaseName = FString::Printf(TEXT("%s階段"),
        *UEnum::GetDisplayValueAsText(NextPhase).ToString());
    Context.PrimaryForce = GetRecommendedForcesForPhase(NextPhase)[0];
    
    return EnterPhase(NextPhase, Context);
}

EFiveElementPhase UMingSupremeSageCommandSystem::DetermineNextPhase() const
{
    // 五行相生順序: 木→火→土→金→水→木
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        return EFiveElementPhase::Fire;
    case EFiveElementPhase::Fire:
        // 檢查是否需要強制進入土階段（火太旺）
        if (FiveElementsRotation && FiveElementsRotation->IsEarthPhaseForced())
        {
            UE_LOG(LogTemp, Warning, TEXT("火太旺，強制進入土階段"));
        }
        return EFiveElementPhase::Earth;
    case EFiveElementPhase::Earth:
        return EFiveElementPhase::Metal;
    case EFiveElementPhase::Metal:
        return EFiveElementPhase::Water;
    case EFiveElementPhase::Water:
        return EFiveElementPhase::Wood;
    default:
        return EFiveElementPhase::Wood;
    }
}

bool UMingSupremeSageCommandSystem::CanTransitionToPhase(EFiveElementPhase TargetPhase) const
{
    return CheckPhaseTransitionConditions(CurrentPhase, TargetPhase);
}

bool UMingSupremeSageCommandSystem::CheckPhaseTransitionConditions(
    EFiveElementPhase From, EFiveElementPhase To) const
{
    // 基礎檢查：相生關係或特殊轉換
    if (From == EFiveElementPhase::None)
    {
        return true;  // 從無到任何階段都允許
    }
    
    // 正常相生順序
    bool bNaturalFlow = false;
    switch (From)
    {
    case EFiveElementPhase::Wood:
        bNaturalFlow = (To == EFiveElementPhase::Fire);
        break;
    case EFiveElementPhase::Fire:
        bNaturalFlow = (To == EFiveElementPhase::Earth);
        break;
    case EFiveElementPhase::Earth:
        bNaturalFlow = (To == EFiveElementPhase::Metal);
        break;
    case EFiveElementPhase::Metal:
        bNaturalFlow = (To == EFiveElementPhase::Water);
        break;
    case EFiveElementPhase::Water:
        bNaturalFlow = (To == EFiveElementPhase::Wood);
        break;
    }
    
    return bNaturalFlow;
}

TArray<EForceType> UMingSupremeSageCommandSystem::GetRecommendedForcesForPhase(
    EFiveElementPhase Phase) const
{
    TArray<EForceType> Forces;
    
    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        // 木階段：以正為主，邪為輔
        Forces.Add(EForceType::Righteous);
        Forces.Add(EForceType::Evil);
        break;
    case EFiveElementPhase::Fire:
        // 火階段：正為旗，邪為燃料
        Forces.Add(EForceType::Righteous);
        Forces.Add(EForceType::Evil);
        break;
    case EFiveElementPhase::Earth:
        // 土階段：封邪止勢，只用正兵
        Forces.Add(EForceType::Righteous);
        break;
    case EFiveElementPhase::Metal:
        // 金階段：冷裁斷，正兵為主
        Forces.Add(EForceType::Righteous);
        break;
    case EFiveElementPhase::Water:
        // 水階段：藏鋒養勢，邪兵深藏
        Forces.Add(EForceType::Righteous);
        Forces.Add(EForceType::Evil);
        break;
    default:
        Forces.Add(EForceType::Righteous);
        break;
    }
    
    return Forces;
}

bool UMingSupremeSageCommandSystem::DeployForce(EForceType ForceType, 
    const FDeploymentConfig& Config)
{
    TotalCommandsIssued++;
    
    if (ForceType == EForceType::Evil)
    {
        EvilForceUsageCount++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("部署%s: %s (隱蔽:%s)"),
        *UEnum::GetDisplayValueAsText(ForceType).ToString(),
        *Config.DeploymentName,
        Config.bIsCovert ? TEXT("是") : TEXT("否"));
    
    // 邪兵部署時檢查防墮
    if (ForceType == EForceType::Evil)
    {
        FCorruptionCheckResult Check = PerformCorruptionCheck();
        if (Check.bRationalizationOfEvil)
        {
            UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到將非常手段合理化徵象"));
            OnCorruptionSignDetected.Broadcast(ECorruptionSign::RationalizationOfEvil);
        }
    }
    
    LogSystemEvent(FString::Printf(TEXT("部署%s: %s"),
        *UEnum::GetDisplayValueAsText(ForceType).ToString(),
        *Config.DeploymentName));
    
    return true;
}

FConquestResult UMingSupremeSageCommandSystem::ExecuteSixConquest(
    ESixConquestStrategy Strategy,
    EStrategyApproach Approach,
    const FConquestTarget& Target)
{
    TotalCommandsIssued++;
    
    UE_LOG(LogTemp, Log, TEXT("執行%s: %s (%s)"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString(),
        *Target.TargetName,
        *UEnum::GetDisplayValueAsText(Approach).ToString());
    
    FConquestResult Result;
    
    if (SixConquestExecutor)
    {
        Result = SixConquestExecutor->ExecuteConquest(Strategy, 
            FConquestExecutionPlan());
    }
    else
    {
        // 基礎實現
        Result.bSuccess = true;
        Result.Effectiveness = 0.7f;
        Result.MoralCost = (Approach == EStrategyApproach::Reverse) ? 0.3f : 0.1f;
        Result.ReputationImpact = 0.0f;
        Result.OutcomeDescription = FString::Printf(TEXT("成功執行%s"),
            *UEnum::GetDisplayValueAsText(Strategy).ToString());
    }
    
    // 逆道執行增加墮落風險檢查
    if (Approach == EStrategyApproach::Reverse)
    {
        FCorruptionCheckResult Check = PerformCorruptionCheck();
        if (Check.OverallCorruptionRisk > 0.6f)
        {
            UE_LOG(LogTemp, Warning, TEXT("⚠ 逆道執行後墮落風險升高: %.2f"),
                Check.OverallCorruptionRisk);
        }
    }
    
    // 記錄決策品質
    float Quality = Result.bSuccess ? Result.Effectiveness : 0.0f;
    TotalDecisionQualityScore += Quality;
    
    LogSystemEvent(FString::Printf(TEXT("六伐執行: %s - %s"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString(),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗")));
    
    return Result;
}

TArray<ESixConquestStrategy> UMingSupremeSageCommandSystem::GetRecommendedConquestStrategies() const
{
    TArray<ESixConquestStrategy> Strategies;
    
    // 根據當前五行階段推薦策略
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        Strategies.Add(ESixConquestStrategy::ConquerMind);
        Strategies.Add(ESixConquestStrategy::ConquerSpirit);
        break;
    case EFiveElementPhase::Fire:
        Strategies.Add(ESixConquestStrategy::ConquerSpirit);
        Strategies.Add(ESixConquestStrategy::ConquerMomentum);
        break;
    case EFiveElementPhase::Earth:
        Strategies.Add(ESixConquestStrategy::ConquerLife);
        break;
    case EFiveElementPhase::Metal:
        Strategies.Add(ESixConquestStrategy::ConquerLife);
        Strategies.Add(ESixConquestStrategy::ConquerShadow);
        break;
    case EFiveElementPhase::Water:
        Strategies.Add(ESixConquestStrategy::ConquerNothing);
        break;
    default:
        Strategies.Add(ESixConquestStrategy::ConquerMind);
        break;
    }
    
    return Strategies;
}

FStrategyResult UMingSupremeSageCommandSystem::ExecuteTwelveStrategy(
    ETwelveStrategies Strategy, const FStrategyContext& Context)
{
    TotalCommandsIssued++;
    
    UE_LOG(LogTemp, Log, TEXT("執行十二策: %s"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString());
    
    FStrategyResult Result;
    
    if (TwelveStrategiesExecutor)
    {
        FStrategyExecutionContext ExecContext;
        ExecContext.Strategy = Strategy;
        ExecContext.CurrentPhase = CurrentPhase;
        Result = TwelveStrategiesExecutor->ExecuteStrategy(Strategy, Context);
    }
    else
       {
        // 基礎實現
        Result.bSuccess = true;
        Result.ExecutedStrategy = Strategy;
        Result.StrategicGain = 0.6f;
        Result.RiskLevel = 0.3f;
        Result.ResultDescription = FString::Printf(TEXT("成功執行%s"),
            *UEnum::GetDisplayValueAsText(Strategy).ToString());
    }
    
    OnStrategyExecuted.Broadcast(Result);
    
    // 記錄關鍵決策
    if (Result.RiskLevel > 0.5f || Result.StrategicGain > 0.8f)
    {
        RecordCriticalDecision(Result.ResultDescription, 
            Result.bSuccess ? Result.StrategicGain : 0.0f);
    }
    
    LogSystemEvent(FString::Printf(TEXT("十二策執行: %s - %s"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString(),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗")));
    
    return Result;
}

FCommandRecommendation UMingSupremeSageCommandSystem::GetRecommendedStrategy() const
{
    FCommandRecommendation Recommendation;
    
    // 根據當前情境推薦
    Recommendation.RecommendedPhase = CurrentPhase;
    Recommendation.ConfidenceLevel = 0.7f;
    
    // 根據當前五行階段推薦權限
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
    case EFiveElementPhase::Fire:
        Recommendation.RecommendedAuthority = ESupremeAuthorityType::StrategyAuthority;
        Recommendation.RecommendedForceType = EForceType::Hybrid;
        break;
    case EFiveElementPhase::Earth:
    case EFiveElementPhase::Metal:
        Recommendation.RecommendedAuthority = ESupremeAuthorityType::MilitaryAuthority;
        Recommendation.RecommendedForceType = EForceType::Righteous;
        break;
    case EFiveElementPhase::Water:
        Recommendation.RecommendedAuthority = ESupremeAuthorityType::DaoAuthority;
        Recommendation.RecommendedForceType = EForceType::Righteous;
        break;
    default:
        Recommendation.RecommendedAuthority = ESupremeAuthorityType::DaoAuthority;
        Recommendation.RecommendedForceType = EForceType::Righteous;
        break;
    }
    
    Recommendation.RecommendationDescription = FString::Printf(
        TEXT("建議由%s主導，使用%s，進入%s階段"),
        *UEnum::GetDisplayValueAsText(Recommendation.RecommendedAuthority).ToString(),
        *UEnum::GetDisplayValueAsText(Recommendation.RecommendedForceType).ToString(),
        *UEnum::GetDisplayValueAsText(Recommendation.RecommendedPhase).ToString()
    );
    
    return Recommendation;
}

float UMingSupremeSageCommandSystem::EvaluateStrategyFitness(
    ETwelveStrategies Strategy, const FStrategyContext& Context) const
{
    float Fitness = 0.5f;
    
    // 基於階段的適配性
    if (Context.CurrentPhase == CurrentPhase)
    {
        Fitness += 0.2f;
    }
    
    // 基於策略類型的適配性
    switch (Strategy)
    {
    case ETwelveStrategies::EstablishNation:
    case ETwelveStrategies::EstablishSystem:
    case ETwelveStrategies::EstablishPeople:
        // 正策適合木火階段
        if (CurrentPhase == EFiveElementPhase::Wood || 
            CurrentPhase == EFiveElementPhase::Fire)
        {
            Fitness += 0.2f;
        }
        break;
    case ETwelveStrategies::BreakSituation:
    case ETwelveStrategies::BreakStructure:
    case ETwelveStrategies::BreakNotPerson:
        // 逆策適合金水階段
        if (CurrentPhase == EFiveElementPhase::Metal || 
            CurrentPhase == EFiveElementPhase::Water)
        {
            Fitness += 0.2f;
        }
        break;
    }
    
    return FMath::Clamp(Fitness, 0.0f, 1.0f);
}

FCorruptionCheckResult UMingSupremeSageCommandSystem::PerformCorruptionCheck()
{
    FCorruptionCheckResult Result;
    
    if (CorruptionPrevention)
    {
        Result = CorruptionPrevention->PerformFullCorruptionCheck();
    }
    else
    {
        // 基礎檢查
        // 1. 檢查邪兵使用頻率
        float EvilUsageRatio = (TotalCommandsIssued > 0) ? 
            (float)EvilForceUsageCount / TotalCommandsIssued : 0.0f;
        Result.bRationalizationOfEvil = (EvilUsageRatio > 0.4f);
        
        // 2. 整體風險計算
        Result.OverallCorruptionRisk = CalculateCorruptionRisk();
        
        // 3. 判斷是否需要交接
        Result.bShouldTransferCommand = (Result.OverallCorruptionRisk > 0.8f);
    }
    
    // 觸發事件
    if (Result.bRationalizationOfEvil)
    {
        OnCorruptionSignDetected.Broadcast(ECorruptionSign::RationalizationOfEvil);
    }
    if (Result.bFollowerDeification)
    {
        OnCorruptionSignDetected.Broadcast(ECorruptionSign::FollowerDeification);
    }
    if (Result.bOppositionDemonization)
    {
        OnCorruptionSignDetected.Broadcast(ECorruptionSign::OppositionDemonization);
    }
    if (Result.bIAmJusticeSyndrome)
    {
        OnCorruptionSignDetected.Broadcast(ECorruptionSign::IAmJusticeSyndrome);
    }
    
    if (Result.bShouldTransferCommand)
    {
        OnCommandTransferRequired.Broadcast();
    }
    
    return Result;
}

bool UMingSupremeSageCommandSystem::IsCorruptionSignPresent(ECorruptionSign Sign) const
{
    if (CorruptionPrevention)
    {
        return CorruptionPrevention->IsCorruptionSignPresent(Sign);
    }
    return false;
}

float UMingSupremeSageCommandSystem::CalculateCorruptionRisk() const
{
    float Risk = 0.0f;
    
    // 1. 邪兵使用比例
    if (TotalCommandsIssued > 0)
    {
        float EvilRatio = (float)EvilForceUsageCount / TotalCommandsIssued;
        Risk += EvilRatio * 0.3f;
    }
    
    // 2. 決策品質趨勢
    float AvgQuality = (TotalCommandsIssued > 0) ? 
        TotalDecisionQualityScore / TotalCommandsIssued : 1.0f;
    Risk += (1.0f - AvgQuality) * 0.2f;
    
    // 3. 權限健康度
    for (const auto& Status : AuthorityStatuses)
    {
        Risk += (1.0f - Status.HealthLevel) * 0.1f;
    }
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

bool UMingSupremeSageCommandSystem::IssueWarningCommand(EWarningLevel Level, 
    const FWarningContent& Content)
{
    UE_LOG(LogTemp, Log, TEXT("發布%s戒令: %s"),
        *UEnum::GetDisplayValueAsText(Level).ToString(),
        *Content.Title);
    
    OnWarningIssued.Broadcast(Content);
    LogSystemEvent(FString::Printf(TEXT("發布%s戒令: %s"),
        *UEnum::GetDisplayValueAsText(Level).ToString(),
        *Content.Title));
    
    return true;
}

bool UMingSupremeSageCommandSystem::ShouldTransferCommand() const
{
    FCorruptionCheckResult Check = const_cast<UMingSupremeSageCommandSystem*>(this)->
        PerformCorruptionCheck();
    return Check.bShouldTransferCommand;
}

bool UMingSupremeSageCommandSystem::ExecuteCommandTransfer()
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行權力交接 ==="));
    
    // 1. 封存邪兵
    if (FiveElementsRotation)
    {
        FiveElementsRotation->SealEvilForces();
    }
    
    // 2. 生成最終報告
    FSelfAuditReport FinalReport = GenerateSelfAuditReport();
    
    // 3. 發布交接命令
    FWarningContent TransferContent;
    TransferContent.Level = EWarningLevel::Terminal;
    TransferContent.Title = TEXT("權力交接命令");
    TransferContent.Message = TEXT("根據最高戒律，執行權力交接");
    TransferContent.IssueTime = FDateTime::Now();
    
    IssueWarningCommand(EWarningLevel::Terminal, TransferContent);
    
    SystemStatus = ECommandStatus::Transferring;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 權力交接程序已啟動"));
    LogSystemEvent(TEXT("執行權力交接"));
    
    return true;
}

FSelfAuditReport UMingSupremeSageCommandSystem::GenerateSelfAuditReport()
{
    FSelfAuditReport Report;
    Report.AuditTime = FDateTime::Now();
    Report.TotalCommandsIssued = TotalCommandsIssued;
    Report.EvilForceUsages = EvilForceUsageCount;
    Report.PhaseTransitions = PhaseTransitionCount;
    Report.AverageDecisionQuality = (TotalCommandsIssued > 0) ? 
        TotalDecisionQualityScore / TotalCommandsIssued : 0.0f;
    Report.CriticalDecisions = CriticalDecisionHistory;
    Report.CorruptionStatus = PerformCorruptionCheck();
    
    // 生成建議
    FString Recommendations;
    if (Report.CorruptionStatus.OverallCorruptionRisk > 0.5f)
    {
        Recommendations += TEXT("⚠ 墮落風險較高，建議進入土階段封存邪兵\n");
    }
    if (Report.EvilForceUsages > TotalCommandsIssued * 0.3f)
    {
        Recommendations += TEXT("⚠ 邪兵使用過多，建議增加正兵比例\n");
    }
    if (Report.AverageDecisionQuality < 0.6f)
    {
        Recommendations += TEXT("⚠ 決策品質下降，建議加強道權監督\n");
    }
    
    Report.Recommendations = Recommendations;
    
    LastAuditTime = FDateTime::Now();
    
    OnSelfAuditCompleted.Broadcast(Report);
    
    UE_LOG(LogTemp, Log, TEXT("=== 自我審核報告生成 ==="));
    UE_LOG(LogTemp, Log, TEXT("總命令: %d, 邪兵使用: %d, 平均品質: %.2f"),
        Report.TotalCommandsIssued,
        Report.EvilForceUsages,
        Report.AverageDecisionQuality);
    UE_LOG(LogTemp, Log, TEXT("墮落風險: %.2f"), Report.CorruptionStatus.OverallCorruptionRisk);
    
    LogSystemEvent(TEXT("生成自我審核報告"));
    
    return Report;
}

bool UMingSupremeSageCommandSystem::RecordCriticalDecision(
    const FString& DecisionDescription, float QualityScore)
{
    CriticalDecisionHistory.Add(DecisionDescription);
    TotalDecisionQualityScore += QualityScore;
    
    UE_LOG(LogTemp, Log, TEXT("記錄關鍵決策: %s (品質: %.2f)"),
        *DecisionDescription, QualityScore);
    
    return true;
}

TMap<FString, float> UMingSupremeSageCommandSystem::GetHistoricalDecisionAnalysis() const
{
    TMap<FString, float> Analysis;
    
    Analysis.Add(TEXT("總決策數"), (float)TotalCommandsIssued);
    Analysis.Add(TEXT("邪兵使用比例"), (TotalCommandsIssued > 0) ? 
        (float)EvilForceUsageCount / TotalCommandsIssued : 0.0f);
    Analysis.Add(TEXT("平均決策品質"), (TotalCommandsIssued > 0) ? 
        TotalDecisionQualityScore / TotalCommandsIssued : 0.0f);
    Analysis.Add(TEXT("階段轉換次數"), (float)PhaseTransitionCount);
    
    return Analysis;
}

FCommandRecommendation UMingSupremeSageCommandSystem::GetCommandRecommendation() const
{
    return GetRecommendedStrategy();
}

float UMingSupremeSageCommandSystem::EvaluateCommandState() const
{
    float Score = 1.0f;
    
    // 1. 三權健康度
    for (const auto& Status : AuthorityStatuses)
    {
        Score *= Status.HealthLevel;
    }
    
    // 2. 防墮狀態
    FCorruptionCheckResult Check = const_cast<UMingSupremeSageCommandSystem*>(this)->
        PerformCorruptionCheck();
    Score *= (1.0f - Check.OverallCorruptionRisk);
    
    // 3. 系統活躍度
    if (SystemStatus != ECommandStatus::Active)
    {
        Score *= 0.5f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

void UMingSupremeSageCommandSystem::UpdateSystemStatistics()
{
    // 定期更新系統統計數據
}

void UMingSupremeSageCommandSystem::LogSystemEvent(const FString& Event)
{
    UE_LOG(LogTemp, Log, TEXT("[至聖者指揮系統] %s"), *Event);
}
