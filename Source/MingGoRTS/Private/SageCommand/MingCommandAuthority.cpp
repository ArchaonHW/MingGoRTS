#include "SageCommand/MingCommandAuthority.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingCommandAuthority::UMingCommandAuthority()
{
    // 初始化合法性來源
    LegitimacySources.Add(EAuthorityLegitimacySource::DivineRight);
    LegitimacySources.Add(EAuthorityLegitimacySource::PopularSupport);
    
    // 初始化當前指標
    CurrentMetrics.PrimaryAuthority = ECommandAuthorityType::MoralAuthority;
    CurrentMetrics.CurrentState = EAuthorityState::Stable;
    CurrentMetrics.AuthorityLevel = 50.0f;
    CurrentMetrics.LegitimacyScore = 50.0f;
    CurrentMetrics.ComplianceRate = 50.0f;
    CurrentMetrics.InfluenceRadius = 50.0f;
    CurrentMetrics.CommandEffectiveness = 50.0f;
}

bool UMingCommandAuthority::Initialize()
{
    if (bSystemActive)
    {
        return true;
    }

    // 初始化系統狀態
    bSystemActive = true;
    SystemStability = 100.0f;

    // 設置衰減定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            DecayTimerHandle,
            this,
            &UMingCommandAuthority::ApplyAuthorityDecay,
            1.0f,
            true
        );
    }

    return true;
}

void UMingCommandAuthority::Cleanup()
{
    bSystemActive = false;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(DecayTimerHandle);
    }
    
    AuthorityHistory.Empty();
    CommandEffectivenessHistory.Empty();
}

FAuthorityMetrics UMingCommandAuthority::GetAuthorityMetrics() const
{
    return CurrentMetrics;
}

bool UMingCommandAuthority::SetPrimaryAuthority(ECommandAuthorityType AuthorityType)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentMetrics.PrimaryAuthority = AuthorityType;
    
    // 根據權威類型調整相關指標
    switch (AuthorityType)
    {
    case ECommandAuthorityType::MoralAuthority:
        CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel + 10.0f, 0.0f, 100.0f);
        break;
    case ECommandAuthorityType::LegalAuthority:
        CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore + 10.0f, 0.0f, 100.0f);
        break;
    case ECommandAuthorityType::CharismaticAuthority:
        CurrentMetrics.ComplianceRate = FMath::Clamp(CurrentMetrics.ComplianceRate + 10.0f, 0.0f, 100.0f);
        break;
    case ECommandAuthorityType::TraditionalAuthority:
        CurrentMetrics.InfluenceRadius = FMath::Clamp(CurrentMetrics.InfluenceRadius + 10.0f, 0.0f, 100.0f);
        break;
    case ECommandAuthorityType::ExpertAuthority:
        CurrentMetrics.CommandEffectiveness = FMath::Clamp(CurrentMetrics.CommandEffectiveness + 10.0f, 0.0f, 100.0f);
        break;
    case ECommandAuthorityType::SituationalAuthority:
        // 情境權威給予所有指標小幅提升
        CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel + 5.0f, 0.0f, 100.0f);
        CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore + 5.0f, 0.0f, 100.0f);
        CurrentMetrics.ComplianceRate = FMath::Clamp(CurrentMetrics.ComplianceRate + 5.0f, 0.0f, 100.0f);
        CurrentMetrics.InfluenceRadius = FMath::Clamp(CurrentMetrics.InfluenceRadius + 5.0f, 0.0f, 100.0f);
        CurrentMetrics.CommandEffectiveness = FMath::Clamp(CurrentMetrics.CommandEffectiveness + 5.0f, 0.0f, 100.0f);
        break;
    }
    
    // 記錄事件
    RecordAuthorityEvent(FString::Printf(TEXT("設置主要權威類型：%s"), *GetAuthorityName(AuthorityType)), 
                        AuthorityType, 10.0f, 5.0f);
    
    // 觸發事件
    OnAuthorityChanged.Broadcast(CurrentMetrics);
    
    return true;
}

ECommandAuthorityType UMingCommandAuthority::GetPrimaryAuthority() const
{
    return CurrentMetrics.PrimaryAuthority;
}

bool UMingCommandAuthority::EnhanceAuthority(ECommandAuthorityType AuthorityType, float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    switch (AuthorityType)
    {
    case ECommandAuthorityType::MoralAuthority:
        return EnhanceMoralAuthority(Amount);
    case ECommandAuthorityType::LegalAuthority:
        return EnhanceLegalAuthority(Amount);
    case ECommandAuthorityType::CharismaticAuthority:
        return EnhanceCharismaticAuthority(Amount);
    case ECommandAuthorityType::TraditionalAuthority:
        return EnhanceTraditionalAuthority(Amount);
    case ECommandAuthorityType::ExpertAuthority:
        return EnhanceExpertAuthority(Amount);
    case ECommandAuthorityType::SituationalAuthority:
        return EnhanceSituationalAuthority(Amount);
    default:
        return false;
    }
}

float UMingCommandAuthority::GetAuthorityLevel() const
{
    return CurrentMetrics.AuthorityLevel;
}

float UMingCommandAuthority::GetLegitimacyScore() const
{
    return CurrentMetrics.LegitimacyScore;
}

bool UMingCommandAuthority::EnhanceLegitimacy(EAuthorityLegitimacySource Source, float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    float EnhancementAmount = 0.0f;
    
    switch (Source)
    {
    case EAuthorityLegitimacySource::DivineRight:
        EnhancementAmount = EvaluateDivineRightLegitimacy() * Amount / 100.0f;
        break;
    case EAuthorityLegitimacySource::PopularSupport:
        EnhancementAmount = EvaluatePopularSupportLegitimacy() * Amount / 100.0f;
        break;
    case EAuthorityLegitimacySource::LegalFramework:
        EnhancementAmount = EvaluateLegalFrameworkLegitimacy() * Amount / 100.0f;
        break;
    case EAuthorityLegitimacySource::HistoricalTradition:
        EnhancementAmount = EvaluateHistoricalTraditionLegitimacy() * Amount / 100.0f;
        break;
    case EAuthorityLegitimacySource::MilitarySuccess:
        EnhancementAmount = EvaluateMilitarySuccessLegitimacy() * Amount / 100.0f;
        break;
    case EAuthorityLegitimacySource::EconomicProsperity:
        EnhancementAmount = EvaluateEconomicProsperityLegitimacy() * Amount / 100.0f;
        break;
    }
    
    CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore + EnhancementAmount, 0.0f, 100.0f);
    
    // 記錄事件
    RecordAuthorityEvent(FString::Printf(TEXT("增強合法性：%s"), *GetLegitimacySourceName(Source)), 
                        CurrentMetrics.PrimaryAuthority, EnhancementAmount * 0.5f, EnhancementAmount);
    
    // 觸發事件
    OnLegitimacyChanged.Broadcast(CurrentMetrics.LegitimacyScore);
    
    return true;
}

TArray<EAuthorityLegitimacySource> UMingCommandAuthority::GetLegitimacySources() const
{
    return LegitimacySources;
}

bool UMingCommandAuthority::IsLegitimate() const
{
    return CurrentMetrics.LegitimacyScore >= 50.0f;
}

FCommandEffectiveness UMingCommandAuthority::EvaluateCommandEffectiveness(const FString& CommandID, const FString& CommandDescription)
{
    FCommandEffectiveness Effectiveness;
    Effectiveness.CommandID = CommandID;
    Effectiveness.CommandDescription = CommandDescription;
    
    // 計算各個維度的有效性
    Effectiveness.ExecutionSpeed = CurrentMetrics.AuthorityLevel * 0.8f + FMath::FRandRange(0.0f, 20.0f);
    Effectiveness.ComplianceLevel = CurrentMetrics.ComplianceRate * 0.9f + FMath::FRandRange(0.0f, 10.0f);
    Effectiveness.ResultQuality = CurrentMetrics.CommandEffectiveness * 0.85f + FMath::FRandRange(0.0f, 15.0f);
    Effectiveness.ResourceEfficiency = (CurrentMetrics.AuthorityLevel + CurrentMetrics.ComplianceRate) / 2.0f * 0.8f + FMath::FRandRange(0.0f, 20.0f);
    
    // 計算整體有效性
    Effectiveness.OverallEffectiveness = (Effectiveness.ExecutionSpeed + Effectiveness.ComplianceLevel + 
                                       Effectiveness.ResultQuality + Effectiveness.ResourceEfficiency) / 4.0f;
    
    // 保存到歷史記錄
    CommandEffectivenessHistory.Add(Effectiveness);
    
    // 限制歷史記錄數量
    if (CommandEffectivenessHistory.Num() > 500)
    {
        CommandEffectivenessHistory.RemoveAt(0);
    }
    
    // 觸發事件
    OnCommandEffectivenessEvaluated.Broadcast(Effectiveness);
    
    return Effectiveness;
}

float UMingCommandAuthority::GetCommandEffectiveness() const
{
    return CurrentMetrics.CommandEffectiveness;
}

bool UMingCommandAuthority::ImproveCommandEffectiveness(float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentMetrics.CommandEffectiveness = FMath::Clamp(CurrentMetrics.CommandEffectiveness + Amount, 0.0f, 100.0f);
    
    // 記錄事件
    RecordAuthorityEvent(TEXT("提升指揮有效性"), CurrentMetrics.PrimaryAuthority, Amount * 0.3f, Amount * 0.2f);
    
    return true;
}

TArray<FString> UMingCommandAuthority::GetCommandRecommendations() const
{
    TArray<FString> Recommendations;
    
    if (CurrentMetrics.AuthorityLevel < 30.0f)
    {
        Recommendations.Add(TEXT("權威水平過低，建議增強道德權威或魅力權威"));
    }
    
    if (CurrentMetrics.LegitimacyScore < 30.0f)
    {
        Recommendations.Add(TEXT("合法性不足，建議增強民心支持或法律框架"));
    }
    
    if (CurrentMetrics.ComplianceRate < 30.0f)
    {
        Recommendations.Add(TEXT("服從率偏低，建議提升個人魅力或專業權威"));
    }
    
    if (CurrentMetrics.CommandEffectiveness < 30.0f)
    {
        Recommendations.Add(TEXT("指揮有效性不足，建議增強專業權威或情境權威"));
    }
    
    if (CurrentMetrics.InfluenceRadius < 30.0f)
    {
        Recommendations.Add(TEXT("影響範圍有限，建議增強傳統權威或道德權威"));
    }
    
    return Recommendations;
}

float UMingCommandAuthority::GetComplianceRate() const
{
    return CurrentMetrics.ComplianceRate;
}

bool UMingCommandAuthority::ImproveCompliance(float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentMetrics.ComplianceRate = FMath::Clamp(CurrentMetrics.ComplianceRate + Amount, 0.0f, 100.0f);
    
    // 記錄事件
    RecordAuthorityEvent(TEXT("提升服從率"), CurrentMetrics.PrimaryAuthority, Amount * 0.4f, Amount * 0.3f);
    
    return true;
}

TArray<FString> UMingCommandAuthority::GetComplianceFactors() const
{
    TArray<FString> Factors;
    
    Factors.Add(FString::Printf(TEXT("權威水平：%.1f%%"), CurrentMetrics.AuthorityLevel));
    Factors.Add(FString::Printf(TEXT("合法性分數：%.1f%%"), CurrentMetrics.LegitimacyScore));
    Factors.Add(FString::Printf(TEXT("影響範圍：%.1f%%"), CurrentMetrics.InfluenceRadius));
    Factors.Add(FString::Printf(TEXT("指揮有效性：%.1f%%"), CurrentMetrics.CommandEffectiveness));
    
    return Factors;
}

bool UMingCommandAuthority::IsCommandComplied(const FString& Command) const
{
    // 簡化的服從性檢查
    float ComplianceChance = CurrentMetrics.ComplianceRate / 100.0f;
    return FMath::FRand() < ComplianceChance;
}

float UMingCommandAuthority::GetInfluenceRadius() const
{
    return CurrentMetrics.InfluenceRadius;
}

bool UMingCommandAuthority::ExpandInfluence(float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentMetrics.InfluenceRadius = FMath::Clamp(CurrentMetrics.InfluenceRadius + Amount, 0.0f, 100.0f);
    
    // 記錄事件
    RecordAuthorityEvent(TEXT("擴展影響範圍"), CurrentMetrics.PrimaryAuthority, Amount * 0.2f, Amount * 0.1f);
    
    return true;
}

TArray<FString> UMingCommandAuthority::GetInfluencedRegions() const
{
    TArray<FString> Regions;
    
    // 根據影響範圍返回受影響的地區
    if (CurrentMetrics.InfluenceRadius >= 20.0f)
    {
        Regions.Add(TEXT("核心地區"));
    }
    if (CurrentMetrics.InfluenceRadius >= 40.0f)
    {
        Regions.Add(TEXT("周邊地區"));
    }
    if (CurrentMetrics.InfluenceRadius >= 60.0f)
    {
        Regions.Add(TEXT("遠程地區"));
    }
    if (CurrentMetrics.InfluenceRadius >= 80.0f)
    {
        Regions.Add(TEXT("邊境地區"));
    }
    
    return Regions;
}

bool UMingCommandAuthority::CanInfluenceRegion(const FString& Region) const
{
    TArray<FString> InfluencedRegions = GetInfluencedRegions();
    return InfluencedRegions.Contains(Region);
}

bool UMingCommandAuthority::HandleAuthorityChallenge(const FString& Challenger, float ChallengeStrength)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 分析挑戰
    bool bAnalyzed = AnalyzeChallenge(Challenger, ChallengeStrength);
    if (!bAnalyzed)
    {
        return false;
    }
    
    // 檢查是否需要防禦
    if (ChallengeStrength > ChallengeThreshold)
    {
        // 選擇防禦策略
        ECommandAuthorityType DefenseType = CurrentMetrics.PrimaryAuthority;
        
        // 執行防禦
        bool bDefended = DefendAuthority(DefenseType);
        
        if (bDefended)
        {
            // 記錄事件
            RecordAuthorityEvent(FString::Printf(TEXT("成功應對權威挑戰：%s"), *Challenger), 
                                CurrentMetrics.PrimaryAuthority, -ChallengeStrength * 0.3f, -ChallengeStrength * 0.2f);
        }
        
        return bDefended;
    }
    
    return true; // 挑戰強度不足，無需特別處理
}

bool UMingCommandAuthority::DefendAuthority(ECommandAuthorityType DefenseType)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 執行防禦策略
    return ExecuteDefenseStrategy();
}

TArray<FString> UMingCommandAuthority::GetThreatsToAuthority() const
{
    TArray<FString> Threats;
    
    if (CurrentMetrics.AuthorityLevel < ChallengeThreshold)
    {
        Threats.Add(TEXT("權威水平過低，面臨挑戰威脅"));
    }
    
    if (CurrentMetrics.LegitimacyScore < 40.0f)
    {
        Threats.Add(TEXT("合法性不足，面臨質疑威脅"));
    }
    
    if (CurrentMetrics.ComplianceRate < 40.0f)
    {
        Threats.Add(TEXT("服從率偏低，面臨抗命威脅"));
    }
    
    if (SystemStability < 50.0f)
    {
        Threats.Add(TEXT("系統穩定性不足，面臨崩潰威脅"));
    }
    
    return Threats;
}

bool UMingCommandAuthority::IsAuthorityUnderThreat() const
{
    TArray<FString> Threats = GetThreatsToAuthority();
    return Threats.Num() > 0;
}

TArray<FAuthorityEvent> UMingCommandAuthority::GetAuthorityHistory() const
{
    return AuthorityHistory;
}

FAuthorityEvent UMingCommandAuthority::GetLastAuthorityEvent() const
{
    if (AuthorityHistory.Num() > 0)
    {
        return AuthorityHistory.Last();
    }
    return FAuthorityEvent();
}

void UMingCommandAuthority::ClearAuthorityHistory()
{
    AuthorityHistory.Empty();
}

// 私有方法實現

float UMingCommandAuthority::CalculateAuthorityLevel() const
{
    // 基於多個因素計算權威水平
    float MoralFactor = 0.0f;
    float LegalFactor = 0.0f;
    float CharismaticFactor = 0.0f;
    float TraditionalFactor = 0.0f;
    float ExpertFactor = 0.0f;
    float SituationalFactor = 0.0f;
    
    // 根據主要權威類型給予不同權重
    switch (CurrentMetrics.PrimaryAuthority)
    {
    case ECommandAuthorityType::MoralAuthority:
        MoralFactor = 1.5f;
        LegalFactor = 0.8f;
        CharismaticFactor = 1.0f;
        TraditionalFactor = 1.2f;
        ExpertFactor = 0.9f;
        SituationalFactor = 0.7f;
        break;
    case ECommandAuthorityType::LegalAuthority:
        MoralFactor = 0.8f;
        LegalFactor = 1.5f;
        CharismaticFactor = 0.7f;
        TraditionalFactor = 1.3f;
        ExpertFactor = 1.1f;
        SituationalFactor = 0.6f;
        break;
    case ECommandAuthorityType::CharismaticAuthority:
        MoralFactor = 1.0f;
        LegalFactor = 0.7f;
        CharismaticFactor = 1.5f;
        TraditionalFactor = 0.8f;
        ExpertFactor = 0.9f;
        SituationalFactor = 1.1f;
        break;
    case ECommandAuthorityType::TraditionalAuthority:
        MoralFactor = 1.2f;
        LegalFactor = 1.3f;
        CharismaticFactor = 0.8f;
        TraditionalFactor = 1.5f;
        ExpertFactor = 0.7f;
        SituationalFactor = 0.5f;
        break;
    case ECommandAuthorityType::ExpertAuthority:
        MoralFactor = 0.9f;
        LegalFactor = 1.1f;
        CharismaticFactor = 0.9f;
        TraditionalFactor = 0.7f;
        ExpertFactor = 1.5f;
        SituationalFactor = 0.9f;
        break;
    case ECommandAuthorityType::SituationalAuthority:
        MoralFactor = 0.7f;
        LegalFactor = 0.6f;
        CharismaticFactor = 1.1f;
        TraditionalFactor = 0.5f;
        ExpertFactor = 0.9f;
        SituationalFactor = 1.5f;
        break;
    }
    
    // 計算加權平均
    float WeightedSum = (MoralFactor + LegalFactor + CharismaticFactor + TraditionalFactor + ExpertFactor + SituationalFactor) * 10.0f;
    return FMath::Clamp(WeightedSum, 0.0f, 100.0f);
}

float UMingCommandAuthority::CalculateLegitimacyScore() const
{
    // 基於合法性來源計算合法性分數
    float Score = 0.0f;
    
    for (EAuthorityLegitimacySource Source : LegitimacySources)
    {
        switch (Source)
        {
        case EAuthorityLegitimacySource::DivineRight:
            Score += EvaluateDivineRightLegitimacy();
            break;
        case EAuthorityLegitimacySource::PopularSupport:
            Score += EvaluatePopularSupportLegitimacy();
            break;
        case EAuthorityLegitimacySource::LegalFramework:
            Score += EvaluateLegalFrameworkLegitimacy();
            break;
        case EAuthorityLegitimacySource::HistoricalTradition:
            Score += EvaluateHistoricalTraditionLegitimacy();
            break;
        case EAuthorityLegitimacySource::MilitarySuccess:
            Score += EvaluateMilitarySuccessLegitimacy();
            break;
        case EAuthorityLegitimacySource::EconomicProsperity:
            Score += EvaluateEconomicProsperityLegitimacy();
            break;
        }
    }
    
    return FMath::Clamp(Score / LegitimacySources.Num(), 0.0f, 100.0f);
}

float UMingCommandAuthority::CalculateComplianceRate() const
{
    // 基於權威水平和合法性計算服從率
    return (CurrentMetrics.AuthorityLevel * 0.6f + CurrentMetrics.LegitimacyScore * 0.4f);
}

float UMingCommandAuthority::CalculateInfluenceRadius() const
{
    // 基於權威水平和影響力計算影響範圍
    return (CurrentMetrics.AuthorityLevel * 0.7f + CurrentMetrics.ComplianceRate * 0.3f);
}

float UMingCommandAuthority::CalculateCommandEffectiveness() const
{
    // 基於多個因素計算指揮有效性
    return (CurrentMetrics.AuthorityLevel * 0.3f + CurrentMetrics.LegitimacyScore * 0.2f + 
            CurrentMetrics.ComplianceRate * 0.3f + CurrentMetrics.InfluenceRadius * 0.2f);
}

float UMingCommandAuthority::EvaluateDivineRightLegitimacy() const
{
    // 評估天命所歸的合法性
    return CurrentMetrics.AuthorityLevel * 0.8f + FMath::FRandRange(10.0f, 30.0f);
}

float UMingCommandAuthority::EvaluatePopularSupportLegitimacy() const
{
    // 評估民心所向的合法性
    return CurrentMetrics.ComplianceRate * 0.9f + FMath::FRandRange(5.0f, 25.0f);
}

float UMingCommandAuthority::EvaluateLegalFrameworkLegitimacy() const
{
    // 評估法理依據的合法性
    return CurrentMetrics.LegitimacyScore * 0.7f + FMath::FRandRange(15.0f, 35.0f);
}

float UMingCommandAuthority::EvaluateHistoricalTraditionLegitimacy() const
{
    // 評估歷史傳統的合法性
    return CurrentMetrics.InfluenceRadius * 0.8f + FMath::FRandRange(10.0f, 30.0f);
}

float UMingCommandAuthority::EvaluateMilitarySuccessLegitimacy() const
{
    // 評估軍事成就的合法性
    return CurrentMetrics.CommandEffectiveness * 0.9f + FMath::FRandRange(5.0f, 25.0f);
}

float UMingCommandAuthority::EvaluateEconomicProsperityLegitimacy() const
{
    // 評估經濟繁榮的合法性
    return SystemStability * 0.8f + FMath::FRandRange(10.0f, 30.0f);
}

bool UMingCommandAuthority::EnhanceMoralAuthority(float Amount)
{
    CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel + Amount, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::EnhanceLegalAuthority(float Amount)
{
    CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore + Amount, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::EnhanceCharismaticAuthority(float Amount)
{
    CurrentMetrics.ComplianceRate = FMath::Clamp(CurrentMetrics.ComplianceRate + Amount, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::EnhanceTraditionalAuthority(float Amount)
{
    CurrentMetrics.InfluenceRadius = FMath::Clamp(CurrentMetrics.InfluenceRadius + Amount, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::EnhanceExpertAuthority(float Amount)
{
    CurrentMetrics.CommandEffectiveness = FMath::Clamp(CurrentMetrics.CommandEffectiveness + Amount, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::EnhanceSituationalAuthority(float Amount)
{
    // 情境權威增強所有指標
    CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel + Amount * 0.5f, 0.0f, 100.0f);
    CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore + Amount * 0.5f, 0.0f, 100.0f);
    CurrentMetrics.ComplianceRate = FMath::Clamp(CurrentMetrics.ComplianceRate + Amount * 0.5f, 0.0f, 100.0f);
    CurrentMetrics.InfluenceRadius = FMath::Clamp(CurrentMetrics.InfluenceRadius + Amount * 0.5f, 0.0f, 100.0f);
    CurrentMetrics.CommandEffectiveness = FMath::Clamp(CurrentMetrics.CommandEffectiveness + Amount * 0.5f, 0.0f, 100.0f);
    return true;
}

bool UMingCommandAuthority::AnalyzeChallenge(const FString& Challenger, float ChallengeStrength)
{
    // 簡化的挑戰分析
    return ChallengeStrength > 0.0f;
}

bool UMingCommandAuthority::SelectDefenseStrategy(ECommandAuthorityType DefenseType)
{
    // 選擇防禦策略
    CurrentMetrics.CurrentState = EAuthorityState::Challenged;
    return true;
}

bool UMingCommandAuthority::ExecuteDefenseStrategy()
{
    // 執行防禦策略
    if (CurrentMetrics.CurrentState == EAuthorityState::Challenged)
    {
        // 成功防禦，恢復狀態
        CurrentMetrics.CurrentState = EAuthorityState::Stable;
        
        // 給予少量權威提升作為防禦成功的獎勵
        CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel + 5.0f, 0.0f, 100.0f);
        
        return true;
    }
    
    return false;
}

void UMingCommandAuthority::RecordAuthorityEvent(const FString& Description, ECommandAuthorityType AuthorityType, float AuthorityChange, float LegitimacyImpact)
{
    FAuthorityEvent Event;
    Event.EventID = FString::Printf(TEXT("AUTHORITY_%lld"), FDateTime::Now().GetTicks());
    Event.Description = Description;
    Event.AuthorityType = AuthorityType;
    Event.AuthorityChange = AuthorityChange;
    Event.LegitimacyImpact = LegitimacyImpact;
    Event.Timestamp = FDateTime::Now();
    
    AuthorityHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (AuthorityHistory.Num() > 1000)
    {
        AuthorityHistory.RemoveAt(0);
    }
}

void UMingCommandAuthority::UpdateAuthorityMetrics()
{
    // 更新所有指標
    CurrentMetrics.AuthorityLevel = CalculateAuthorityLevel();
    CurrentMetrics.LegitimacyScore = CalculateLegitimacyScore();
    CurrentMetrics.ComplianceRate = CalculateComplianceRate();
    CurrentMetrics.InfluenceRadius = CalculateInfluenceRadius();
    CurrentMetrics.CommandEffectiveness = CalculateCommandEffectiveness();
}

FString UMingCommandAuthority::GetAuthorityName(ECommandAuthorityType AuthorityType) const
{
    switch (AuthorityType)
    {
    case ECommandAuthorityType::MoralAuthority: return TEXT("道德權威");
    case ECommandAuthorityType::LegalAuthority: return TEXT("法理權威");
    case ECommandAuthorityType::CharismaticAuthority: return TEXT("魅力權威");
    case ECommandAuthorityType::TraditionalAuthority: return TEXT("傳統權威");
    case ECommandAuthorityType::ExpertAuthority: return TEXT("專業權威");
    case ECommandAuthorityType::SituationalAuthority: return TEXT("情境權威");
    default: return TEXT("未知權威");
    }
}

FString UMingCommandAuthority::GetStateName(EAuthorityState State) const
{
    switch (State)
    {
    case EAuthorityState::Strong: return TEXT("強大");
    case EAuthorityState::Stable: return TEXT("穩定");
    case EAuthorityState::Weakening: return TEXT("衰弱");
    case EAuthorityState::Challenged: return TEXT("受挑戰");
    case EAuthorityState::Collapsing: return TEXT("崩潰");
    case EAuthorityState::Restoring: return TEXT("恢復");
    default: return TEXT("未知狀態");
    }
}

FString UMingCommandAuthority::GetLegitimacySourceName(EAuthorityLegitimacySource Source) const
{
    switch (Source)
    {
    case EAuthorityLegitimacySource::DivineRight: return TEXT("天命所歸");
    case EAuthorityLegitimacySource::PopularSupport: return TEXT("民心所向");
    case EAuthorityLegitimacySource::LegalFramework: return TEXT("法理依據");
    case EAuthorityLegitimacySource::HistoricalTradition: return TEXT("歷史傳統");
    case EAuthorityLegitimacySource::MilitarySuccess: return TEXT("軍事成就");
    case EAuthorityLegitimacySource::EconomicProsperity: return TEXT("經濟繁榮");
    default: return TEXT("未知來源");
    }
}

void UMingCommandAuthority::ApplyAuthorityDecay(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 應用權威衰減
    float AuthorityDecay = AuthorityDecayRate * DeltaTime;
    CurrentMetrics.AuthorityLevel = FMath::Clamp(CurrentMetrics.AuthorityLevel - AuthorityDecay, 0.0f, 100.0f);
    
    // 應用合法性衰減
    float LegitimacyDecay = LegitimacyDecayRate * DeltaTime;
    CurrentMetrics.LegitimacyScore = FMath::Clamp(CurrentMetrics.LegitimacyScore - LegitimacyDecay, 0.0f, 100.0f);
    
    // 更新其他指標
    UpdateAuthorityMetrics();
    
    // 檢查狀態變化
    if (CurrentMetrics.AuthorityLevel < 20.0f)
    {
        CurrentMetrics.CurrentState = EAuthorityState::Collapsing;
    }
    else if (CurrentMetrics.AuthorityLevel < 40.0f)
    {
        CurrentMetrics.CurrentState = EAuthorityState::Weakening;
    }
    else if (CurrentMetrics.AuthorityLevel < 60.0f)
    {
        CurrentMetrics.CurrentState = EAuthorityState::Stable;
    }
    else
    {
        CurrentMetrics.CurrentState = EAuthorityState::Strong;
    }
}
