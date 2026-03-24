#include "SageCommand/MingSageCommandSystem.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingYinYangFiveElements.h"
#include "SageCommand/MingSixStrategies.h"
#include "SageCommand/MingCommandAuthority.h"
#include "SageCommand/MingFiveElementsRotation.h"
#include "Engine/World.h"
#include "TimerManager.h"

// 五行相生相克表定義
const TMap<EFiveElements, EFiveElements> UMingSageCommandSystem::GenerationCycle = {
    {EFiveElements::Metal, EFiveElements::Water},
    {EFiveElements::Water, EFiveElements::Wood},
    {EFiveElements::Wood, EFiveElements::Fire},
    {EFiveElements::Fire, EFiveElements::Earth},
    {EFiveElements::Earth, EFiveElements::Metal}
};

const TMap<EFiveElements, EFiveElements> UMingSageCommandSystem::DestructionCycle = {
    {EFiveElements::Metal, EFiveElements::Wood},
    {EFiveElements::Wood, EFiveElements::Earth},
    {EFiveElements::Earth, EFiveElements::Water},
    {EFiveElements::Water, EFiveElements::Fire},
    {EFiveElements::Fire, EFiveElements::Metal}
};

UMingSageCommandSystem::UMingSageCommandSystem()
{
    MoralAuthoritySystem = nullptr;
    YinYangSystem = nullptr;
    SixStrategiesSystem = nullptr;
    AuthoritySystem = nullptr;
    RotationSystem = nullptr;
    
    bSystemActive = false;
    SystemHealth = 100.0f;
    
    DecisionsMade = 0;
    AverageDecisionTime = 0.0f;
    LastDecisionTime = 0.0f;
}

bool UMingSageCommandSystem::InitializeSageCommandSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化至聖者指揮學系統..."));
    
    // 初始化子系統
    InitializeSubSystems();
    
    // 設置初始狀態
    CurrentRotation.CurrentElement = EFiveElements::Earth;
    CurrentRotation.NextElement = EFiveElements::Metal;
    CurrentRotation.RotationProgress = 0.0f;
    CurrentRotation.RotationSpeed = RotationBaseSpeed;
    
    // 初始化道德權威指標
    CurrentMoralAuthority.MoralIntegrity = 50.0f;
    CurrentMoralAuthority.AuthorityLegitimacy = 50.0f;
    CurrentMoralAuthority.PublicSupport = 50.0f;
    CurrentMoralAuthority.RighteousnessLevel = 50.0f;
    CurrentMoralAuthority.EthicalBalance = 50.0f;
    
    // 設置定時器更新輪轉
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            RotationTimerHandle,
            this,
            &UMingSageCommandSystem::UpdateElementRotation,
            0.1f,
            true
        );
    }
    
    bSystemActive = true;
    SystemHealth = 100.0f;
    
    UE_LOG(LogTemp, Log, TEXT("至聖者指揮學系統初始化完成"));
    return true;
}

void UMingSageCommandSystem::ShutdownSageCommandSystem()
{
    UE_LOG(LogTemp, Log, TEXT("關閉至聖者指揮學系統..."));
    
    bSystemActive = false;
    
    // 清理定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RotationTimerHandle);
    }
    
    // 清理子系統
    if (MoralAuthoritySystem)
    {
        MoralAuthoritySystem->Cleanup();
    }
    
    if (YinYangSystem)
    {
        YinYangSystem->Cleanup();
    }
    
    if (SixStrategiesSystem)
    {
        SixStrategiesSystem->Cleanup();
    }
    
    if (AuthoritySystem)
    {
        AuthoritySystem->Cleanup();
    }
    
    if (RotationSystem)
    {
        RotationSystem->Cleanup();
    }
    
    UE_LOG(LogTemp, Log, TEXT("至聖者指揮學系統已關閉"));
}

FStrategicDecision UMingSageCommandSystem::MakeStrategicDecision(const FString& Context, EStrategyType StrategyType)
{
    if (!bSystemActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("指揮學系統未激活，無法做決策"));
        return FStrategicDecision();
    }
    
    double StartTime = FPlatformTime::Seconds();
    
    FStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.StrategyType = StrategyType;
    Decision.Priority = EDecisionPriority::Medium;
    Decision.Description = FString::Printf(TEXT("基於 %s 的戰略決策"), *Context);
    
    // 分析當前局勢
    FStrategicSituation Situation = AnalyzeCurrentSituation();
    
    // 選擇最佳策略
    Decision.StrategyMethod = SelectOptimalStrategy(Context);
    
    // 設定五行親和性
    Decision.ElementAffinity = GetCurrentElement();
    
    // 計算成功概率
    Decision.SuccessProbability = CalculateSuccessProbability(Decision);
    
    // 記錄決策
    DecisionHistory.Add(Decision);
    DecisionsMade++;
    
    // 更新性能指標
    LastDecisionTime = FPlatformTime::Seconds() - StartTime;
    AverageDecisionTime = (AverageDecisionTime * (DecisionsMade - 1) + LastDecisionTime) / DecisionsMade;
    
    // 處理決策效果
    ProcessDecisionEffects(Decision);
    
    // 廣播決策事件
    OnStrategicDecisionMade.Broadcast(Decision);
    
    UE_LOG(LogTemp, Log, TEXT("做戰略決策: %s (成功率: %.1f%%)"), *Decision.Description, Decision.SuccessProbability * 100);
    
    return Decision;
}

bool UMingSageCommandSystem::ExecuteStrategicDecision(const FStrategicDecision& Decision)
{
    if (!bSystemActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("指揮學系統未激活，無法執行決策"));
        return false;
    }
    
    // 檢查道德合法性
    if (!IsDecisionMoral(Decision))
    {
        UE_LOG(LogTemp, Warning, TEXT("決策不符合道德標準，執行被拒絕: %s"), *Decision.Description);
        return false;
    }
    
    // 檢查權威是否足夠
    if (!CanIssueCommand(Decision.Description))
    {
        UE_LOG(LogTemp, Warning, TEXT("權威不足，無法執行決策: %s"), *Decision.Description);
        return false;
    }
    
    // 執行決策
    bool bSuccess = false;
    
    switch (Decision.StrategyType)
    {
        case EStrategyType::Defensive:
            bSuccess = ExecuteDefensiveStrategy(Decision);
            break;
        case EStrategyType::Offensive:
            bSuccess = ExecuteOffensiveStrategy(Decision);
            break;
        case EStrategyType::Diplomatic:
            bSuccess = ExecuteDiplomaticStrategy(Decision);
            break;
        case EStrategyType::Economic:
            bSuccess = ExecuteEconomicStrategy(Decision);
            break;
        case EStrategyType::Cultural:
            bSuccess = ExecuteCulturalStrategy(Decision);
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("未知的策略類型"));
            return false;
    }
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("成功執行決策: %s"), *Decision.Description);
        
        // 更新系統狀態
        UpdateSystemStatus();
        
        // 廣播系統更新
        BroadcastSystemUpdates();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("決策執行失敗: %s"), *Decision.Description);
    }
    
    return bSuccess;
}

FStrategicSituation UMingSageCommandSystem::AnalyzeCurrentSituation()
{
    FStrategicSituation Situation;
    
    // 獲取道德權威指標
    Situation.MoralAuthority = GetMoralAuthorityMetrics();
    
    // 計算軍事實力
    Situation.MilitaryStrength = CalculateMilitaryStrength();
    
    // 計算經濟實力
    Situation.EconomicPower = CalculateEconomicPower();
    
    // 計算政治影響
    Situation.PoliticalInfluence = CalculatePoliticalInfluence();
    
    // 計算文化威望
    Situation.CulturalPrestige = CalculateCulturalPrestige();
    
    // 計算總體優勢
    Situation.OverallAdvantage = (
        Situation.MilitaryStrength * 0.3f +
        Situation.EconomicPower * 0.25f +
        Situation.PoliticalInfluence * 0.2f +
        Situation.CulturalPrestige * 0.15f +
        Situation.MoralAuthority.MoralIntegrity * 0.1f
    ) * 100.0f;
    
    CurrentSituation = Situation;
    OnStrategicSituationUpdated.Broadcast(Situation);
    
    return Situation;
}

FMoralAuthorityMetrics UMingSageCommandSystem::GetMoralAuthorityMetrics() const
{
    if (MoralAuthoritySystem)
    {
        return MoralAuthoritySystem->GetMoralAuthorityMetrics();
    }
    return CurrentMoralAuthority;
}

bool UMingSageCommandSystem::UpdateMoralAuthority(float MoralChange, const FString& Reason)
{
    if (!MoralAuthoritySystem)
    {
        return false;
    }
    
    bool bSuccess = MoralAuthoritySystem->UpdateMoralAuthority(MoralChange, Reason);
    
    if (bSuccess)
    {
        CurrentMoralAuthority = MoralAuthoritySystem->GetMoralAuthorityMetrics();
        OnMoralAuthorityChanged.Broadcast(CurrentMoralAuthority);
        
        UE_LOG(LogTemp, Log, TEXT("道德權威更新: %.1f (%s)"), MoralChange, *Reason);
    }
    
    return bSuccess;
}

bool UMingSageCommandSystem::IsDecisionMoral(const FStrategicDecision& Decision)
{
    if (!MoralAuthoritySystem)
    {
        return false;
    }
    
    return MoralAuthoritySystem->IsDecisionMoral(Decision);
}

EFiveElements UMingSageCommandSystem::GetCurrentElement() const
{
    return CurrentRotation.CurrentElement;
}

FElementRotation UMingSageCommandSystem::GetElementRotation() const
{
    return CurrentRotation;
}

bool UMingSageCommandSystem::SetElementAffinity(EFiveElements Element)
{
    if (!YinYangSystem)
    {
        return false;
    }
    
    bool bSuccess = YinYangSystem->SetElementAffinity(Element);
    
    if (bSuccess)
    {
        CurrentRotation.CurrentElement = Element;
        CurrentRotation.NextElement = GetNextElement(Element);
        CurrentRotation.RotationProgress = 0.0f;
        
        OnElementRotationChanged.Broadcast(CurrentRotation);
    }
    
    return bSuccess;
}

float UMingSageCommandSystem::GetElementalAdvantage(EFiveElements Attacker, EFiveElements Defender) const
{
    // 檢查相克關係
    if (DestructionCycle.Contains(Attacker) && DestructionCycle[Attacker] == Defender)
    {
        return 1.5f; // 50% 加成
    }
    
    // 檢查相生關係
    if (GenerationCycle.Contains(Attacker) && GenerationCycle[Attacker] == Defender)
    {
        return 0.8f; // 20% 減益
    }
    
    return 1.0f; // 無加成
}

ESixStrategyType UMingSageCommandSystem::SelectOptimalStrategy(const FString& Context)
{
    if (!SixStrategiesSystem)
    {
        return ESixStrategyType::DirectAttack;
    }
    
    return SixStrategiesSystem->SelectOptimalStrategy(Context);
}

TArray<ESixStrategyType> UMingSageCommandSystem::GetAvailableStrategies() const
{
    if (!SixStrategiesSystem)
    {
        return TArray<ESixStrategyType>();
    }
    
    return SixStrategiesSystem->GetAvailableStrategies();
}

float UMingSageCommandSystem::GetStrategyEffectiveness(ESixStrategyType Strategy, const FString& Context) const
{
    if (!SixStrategiesSystem)
    {
        return 0.0f;
    }
    
    return SixStrategiesSystem->GetStrategyEffectiveness(Strategy, Context);
}

float UMingSageCommandSystem::GetCommandAuthority() const
{
    if (AuthoritySystem)
    {
        return AuthoritySystem->GetCommandAuthority();
    }
    
    return CurrentMoralAuthority.AuthorityLegitimacy;
}

bool UMingSageCommandSystem::EnhanceAuthority(EAuthoritySource Source, float Amount)
{
    if (!AuthoritySystem)
    {
        return false;
    }
    
    bool bSuccess = AuthoritySystem->EnhanceAuthority(Source, Amount);
    
    if (bSuccess)
    {
        UpdateSystemStatus();
    }
    
    return bSuccess;
}

bool UMingSageCommandSystem::CanIssueCommand(const FString& Command) const
{
    if (!AuthoritySystem)
    {
        return false;
    }
    
    return AuthoritySystem->CanIssueCommand(Command);
}

void UMingSageCommandSystem::UpdateElementRotation(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 更新輪轉進度
    CurrentRotation.RotationProgress += CurrentRotation.RotationSpeed * DeltaTime;
    
    // 檢查是否需要輪轉到下一元素
    if (CurrentRotation.RotationProgress >= 1.0f)
    {
        CurrentRotation.RotationProgress = 0.0f;
        CurrentRotation.CurrentElement = CurrentRotation.NextElement;
        CurrentRotation.NextElement = GetNextElement(CurrentRotation.CurrentElement);
        
        OnElementRotationChanged.Broadcast(CurrentRotation);
        
        UE_LOG(LogTemp, Log, TEXT("五行輪轉: %s -> %s"), 
               *GetElementName(CurrentRotation.CurrentElement),
               *GetElementName(CurrentRotation.NextElement));
    }
}

void UMingSageCommandSystem::SetRotationSpeed(float Speed)
{
    CurrentRotation.RotationSpeed = FMath::Clamp(Speed, 0.1f, 5.0f);
}

EFiveElements UMingSageCommandSystem::PredictNextElement() const
{
    return CurrentRotation.NextElement;
}

TArray<FStrategicDecision> UMingSageCommandSystem::GenerateStrategicOptions(const FString& Context)
{
    TArray<FStrategicDecision> Options;
    
    // 分析當前局勢
    FStrategicSituation Situation = AnalyzeCurrentSituation();
    
    // 生成不同策略類型的選項
    for (int32 i = 0; i < 5; ++i)
    {
        FStrategicDecision Option;
        Option.DecisionID = FGuid::NewGuid().ToString();
        Option.StrategyType = static_cast<EStrategyType>(i);
        Option.StrategyMethod = SelectOptimalStrategy(Context);
        Option.ElementAffinity = GetCurrentElement();
        Option.Priority = CalculateDecisionPriority(Option);
        Option.SuccessProbability = CalculateSuccessProbability(Option);
        Option.Description = FString::Printf(TEXT("戰略選項 %d: %s"), i + 1, *Context);
        
        Options.Add(Option);
    }
    
    // 按成功概率排序
    Options.Sort([](const FStrategicDecision& A, const FStrategicDecision& B)
    {
        return A.SuccessProbability > B.SuccessProbability;
    });
    
    return Options;
}

FDecisionQualityMetrics UMingSageCommandSystem::EvaluateDecisionQuality(const FStrategicDecision& Decision)
{
    FDecisionQualityMetrics Metrics;
    
    // 計算戰略準確性
    Metrics.StrategicAccuracy = CalculateStrategicAccuracy(Decision);
    
    // 計算戰術效率
    Metrics.TacticalEfficiency = CalculateTacticalEfficiency(Decision);
    
    // 計算資源利用率
    Metrics.ResourceUtilization = CalculateResourceUtilization(Decision);
    
    // 計算風險管理能力
    Metrics.RiskManagement = CalculateRiskManagement(Decision);
    
    // 計算適應性
    Metrics.Adaptability = CalculateAdaptability(Decision);
    
    return Metrics;
}

float UMingSageCommandSystem::CalculateSuccessProbability(const FStrategicDecision& Decision) const
{
    float BaseProbability = 0.5f;
    
    // 根據當前局勢調整
    FStrategicSituation Situation = CurrentSituation;
    BaseProbability += Situation.OverallAdvantage * 0.002f; // 每1點優勢增加0.2%成功率
    
    // 根據道德權威調整
    BaseProbability += Situation.MoralAuthority.MoralIntegrity * 0.003f; // 每1點道德增加0.3%成功率
    
    // 根據五行親和性調整
    float ElementalBonus = GetElementalBonus(Decision.ElementAffinity);
    BaseProbability += ElementalBonus * 0.1f;
    
    // 根據策略類型調整
    float StrategyBonus = GetStrategyBonus(Decision.StrategyMethod);
    BaseProbability += StrategyBonus * 0.1f;
    
    return FMath::Clamp(BaseProbability, 0.0f, 1.0f);
}

// 私有方法實現
void UMingSageCommandSystem::InitializeSubSystems()
{
    // 創建子系統實例
    MoralAuthoritySystem = NewObject<UMingMoralAuthority>();
    YinYangSystem = NewObject<UMingYinYangFiveElements>();
    SixStrategiesSystem = NewObject<UMingSixStrategies>();
    AuthoritySystem = NewObject<UMingCommandAuthority>();
    RotationSystem = NewObject<UMingFiveElementsRotation>();
    
    // 初始化子系統
    if (MoralAuthoritySystem)
    {
        MoralAuthoritySystem->Initialize();
    }
    
    if (YinYangSystem)
    {
        YinYangSystem->Initialize();
    }
    
    if (SixStrategiesSystem)
    {
        SixStrategiesSystem->Initialize();
    }
    
    if (AuthoritySystem)
    {
        AuthoritySystem->Initialize();
    }
    
    if (RotationSystem)
    {
        RotationSystem->Initialize();
    }
}

void UMingSageCommandSystem::UpdateSystemStatus()
{
    // 計算系統健康度
    float MoralHealth = CurrentMoralAuthority.MoralIntegrity;
    float AuthorityHealth = GetCommandAuthority();
    float RotationHealth = 100.0f; // 輪轉系統總是健康的
    
    SystemHealth = (MoralHealth + AuthorityHealth + RotationHealth) / 3.0f;
    
    // 應用權威衰減
    if (AuthoritySystem)
    {
        AuthoritySystem->ApplyAuthorityDecay(AuthorityDecayRate);
    }
}

void UMingSageCommandSystem::ProcessDecisionEffects(const FStrategicDecision& Decision)
{
    // 根據決策類型處理效果
    switch (Decision.StrategyType)
    {
        case EStrategyType::Defensive:
            UpdateMoralAuthority(2.0f, TEXT("防禦決策"));
            break;
        case EStrategyType::Offensive:
            UpdateMoralAuthority(-1.0f, TEXT("進攻決策"));
            break;
        case EStrategyType::Diplomatic:
            UpdateMoralAuthority(3.0f, TEXT("外交決策"));
            break;
        case EStrategyType::Economic:
            UpdateMoralAuthority(1.0f, TEXT("經濟決策"));
            break;
        case EStrategyType::Cultural:
            UpdateMoralAuthority(2.0f, TEXT("文化決策"));
            break;
    }
}

void UMingSageCommandSystem::BroadcastSystemUpdates()
{
    OnStrategicSituationUpdated.Broadcast(CurrentSituation);
    OnMoralAuthorityChanged.Broadcast(CurrentMoralAuthority);
    OnElementRotationChanged.Broadcast(CurrentRotation);
}

// 輔助方法
EFiveElements UMingSageCommandSystem::GetNextElement(EFiveElements CurrentElement) const
{
    switch (CurrentElement)
    {
        case EFiveElements::Metal: return EFiveElements::Water;
        case EFiveElements::Water: return EFiveElements::Wood;
        case EFiveElements::Wood: return EFiveElements::Fire;
        case EFiveElements::Fire: return EFiveElements::Earth;
        case EFiveElements::Earth: return EFiveElements::Metal;
        default: return EFiveElements::Earth;
    }
}

FString UMingSageCommandSystem::GetElementName(EFiveElements Element) const
{
    switch (Element)
    {
        case EFiveElements::Metal: return TEXT("金");
        case EFiveElements::Water: return TEXT("水");
        case EFiveElements::Wood: return TEXT("木");
        case EFiveElements::Fire: return TEXT("火");
        case EFiveElements::Earth: return TEXT("土");
        default: return TEXT("未知");
    }
}

float UMingSageCommandSystem::CalculateMilitaryStrength() const
{
    // 這裡應該從實際的軍事系統獲取數據
    return 60.0f; // 示例值
}

float UMingSageCommandSystem::CalculateEconomicPower() const
{
    // 這裡應該從實際的經濟系統獲取數據
    return 70.0f; // 示例值
}

float UMingSageCommandSystem::CalculatePoliticalInfluence() const
{
    // 這裡應該從實際的政治系統獲取數據
    return 55.0f; // 示例值
}

float UMingSageCommandSystem::CalculateCulturalPrestige() const
{
    // 這裡應該從實際的文化系統獲取數據
    return 65.0f; // 示例值
}

EDecisionPriority UMingSageCommandSystem::CalculateDecisionPriority(const FStrategicDecision& Decision) const
{
    if (Decision.SuccessProbability > 0.8f)
    {
        return EDecisionPriority::High;
    }
    else if (Decision.SuccessProbability > 0.6f)
    {
        return EDecisionPriority::Medium;
    }
    else if (Decision.SuccessProbability > 0.4f)
    {
        return EDecisionPriority::Low;
    }
    else
    {
        return EDecisionPriority::Critical;
    }
}

float UMingSageCommandSystem::CalculateStrategicAccuracy(const FStrategicDecision& Decision) const
{
    // 基於當前局勢和決策類型計算戰略準確性
    return 0.7f; // 示例值
}

float UMingSageCommandSystem::CalculateTacticalEfficiency(const FStrategicDecision& Decision) const
{
    // 基於策略方法計算戰術效率
    return 0.8f; // 示例值
}

float UMingSageCommandSystem::CalculateResourceUtilization(const FStrategicDecision& Decision) const
{
    // 計算資源利用率
    return 0.6f; // 示例值
}

float UMingSageCommandSystem::CalculateRiskManagement(const FStrategicDecision& Decision) const
{
    // 計算風險管理能力
    return 0.7f; // 示例值
}

float UMingSageCommandSystem::CalculateAdaptability(const FStrategicDecision& Decision) const
{
    // 計算適應性
    return 0.8f; // 示例值
}

float UMingSageCommandSystem::GetElementalBonus(EFiveElements Element) const
{
    // 根據當前輪轉狀態計算元素加成
    if (Element == CurrentRotation.CurrentElement)
    {
        return 1.0f; // 當前元素有最大加成
    }
    else if (Element == CurrentRotation.NextElement)
    {
        return 0.5f; // 下一元素有中等加成
    }
    else
    {
        return 0.0f; // 其他元素無加成
    }
}

float UMingSageCommandSystem::GetStrategyBonus(ESixStrategyType Strategy) const
{
    // 根據策略類型計算加成
    switch (Strategy)
    {
        case ESixStrategyType::DirectAttack: return 0.2f;
        case ESixStrategyType::DirectDefense: return 0.3f;
        case ESixStrategyType::DirectDiplomacy: return 0.4f;
        case ESixStrategyType::IndirectAttack: return 0.3f;
        case ESixStrategyType::IndirectDefense: return 0.2f;
        case ESixStrategyType::IndirectDiplomacy: return 0.5f;
        default: return 0.0f;
    }
}

bool UMingSageCommandSystem::ExecuteDefensiveStrategy(const FStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("執行防禦策略: %s"), *Decision.Description);
    // 這裡應該調用實際的防禦系統
    return true;
}

bool UMingSageCommandSystem::ExecuteOffensiveStrategy(const FStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("執行進攻策略: %s"), *Decision.Description);
    // 這裡應該調用實際的進攻系統
    return true;
}

bool UMingSageCommandSystem::ExecuteDiplomaticStrategy(const FStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("執行外交策略: %s"), *Decision.Description);
    // 這裡應該調用實際的外交系統
    return true;
}

bool UMingSageCommandSystem::ExecuteEconomicStrategy(const FStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("執行經濟策略: %s"), *Decision.Description);
    // 這裡應該調用實際的經濟系統
    return true;
}

bool UMingSageCommandSystem::ExecuteCulturalStrategy(const FStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("執行文化策略: %s"), *Decision.Description);
    // 這裡應該調用實際的文化系統
    return true;
}
