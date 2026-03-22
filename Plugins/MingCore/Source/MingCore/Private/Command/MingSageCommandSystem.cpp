#include "MingSageCommandSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSageCommandSystem::UMingSageCommandSystem()
{
    FallWarningThreshold = 30.0f;
    YinYangBalanceThreshold = 20.0f;
    FiveElementsRotationSpeed = 1.0f;
}

void UMingSageCommandSystem::InitializeSageCommandSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化至聖者指揮學系統..."));

    // 初始化聖者檔案
    CurrentProfile = FSageProfile();
    CurrentProfile.SageType = ESageType::Sage;

    // 初始化五行輪轉
    CurrentRotation = FFiveElementsRotation();
    CurrentRotation.CurrentElement = EFiveElements::Wood;
    CurrentRotation.NextElement = EFiveElements::Fire;
    CurrentRotation.AvailableStrategies.Add(ESixStrategies::EstablishNation);

    UE_LOG(LogTemp, Log, TEXT("至聖者指揮學系統初始化完成"));
}

void UMingSageCommandSystem::ApplySagePrinciples()
{
    UE_LOG(LogTemp, Log, TEXT("應用至聖者指揮學原則..."));

    // 原則1：正邪皆兵，而聖者不墮
    // 檢查是否沉迷於某一種手段
    if (CurrentProfile.UnconventionalActionsCount > CurrentProfile.VictoriesCount * 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("警告：過度使用非常手段，有墮落風險"));
        CurrentProfile.SageType = ESageType::DemonKing;
    }

    // 原則2：陰陽平衡
    BalanceYinYang(CurrentProfile.YinBalance, CurrentProfile.YangBalance);

    // 原則3：五行輪轉
    CheckAndUpdateRotation();

    // 原則4：防墮機制
    PreventFall(CurrentProfile);

    UE_LOG(LogTemp, Log, TEXT("至聖者指揮學原則應用完成"));
}

bool UMingSageCommandSystem::CanSwitchStrategy(ESixStrategies FromStrategy, ESixStrategies ToStrategy)
{
    // 檢查策略切換的合理性
    // 正策之間可以自由切換
    if ((FromStrategy <= ESixStrategies::EstablishPeople) && (ToStrategy <= ESixStrategies::EstablishPeople))
    {
        return true;
    }

    // 逆策之間可以自由切換
    if ((FromStrategy >= ESixStrategies::BreakSituation) && (ToStrategy >= ESixStrategies::BreakSituation))
    {
        return true;
    }

    // 正逆切換需要檢查時機
    if (CurrentRotation.CurrentElement == EFiveElements::Wood || CurrentRotation.CurrentElement == EFiveElements::Fire)
    {
        // 木火階段：正為旗，邪為燃料，可以切換
        return true;
    }
    else if (CurrentRotation.CurrentElement == EFiveElements::Earth)
    {
        // 土階段：封邪、止勢、收權，限制逆策
        return ToStrategy <= ESixStrategies::EstablishPeople;
    }
    else if (CurrentRotation.CurrentElement == EFiveElements::Metal || CurrentRotation.CurrentElement == EFiveElements::Water)
    {
        // 金水階段：裁斷與不作為，極限切換
        return false;
    }

    return false;
}

float UMingSageCommandSystem::CalculateMoralCost(ESixStrategies Strategy, EYinYangNature Nature)
{
    float BaseCost = 0.0f;

    // 正策道德成本低
    switch (Strategy)
    {
    case ESixStrategies::EstablishNation:
    case ESixStrategies::EstablishSystem:
    case ESixStrategies::EstablishPeople:
        BaseCost = 0.0f;
        break;
    case ESixStrategies::BreakSituation:
        BaseCost = 30.0f;
        break;
    case ESixStrategies::BreakStructure:
        BaseCost = 50.0f;
        break;
    case ESixStrategies::PreservePeople:
        BaseCost = 10.0f;
        break;
    }

    // 陰性手段增加道德成本
    if (Nature == EYinYangNature::Yin)
    {
        BaseCost *= 1.5f;
    }

    // 根據聖者類型調整
    if (CurrentProfile.SageType == ESageType::Sage)
    {
        BaseCost *= 0.8f; // 聖者道德成本較低
    }
    else if (CurrentProfile.SageType == ESageType::DemonKing)
    {
        BaseCost *= 0.3f; // 魔王道德成本很低
    }

    return BaseCost;
}

TArray<ESixStrategies> UMingSageCommandSystem::GetRecommendedStrategies(const FSituationAnalysis& Analysis)
{
    TArray<ESixStrategies> Recommendations;

    // 根據局勢分析推薦策略
    if (Analysis.ChaosLevel > 70.0f)
    {
        // 高混亂局勢：優先考慮立制
        Recommendations.Add(ESixStrategies::EstablishSystem);
    }

    if (Analysis.PublicSupport < 30.0f)
    {
        // 低支持度：優先考慮立國
        Recommendations.Add(ESixStrategies::EstablishNation);
    }

    if (Analysis.UrgencyLevel > 80.0f)
    {
        // 高緊急性：可考慮破局
        if (CurrentProfile.SelfControlLevel > 50.0f)
        {
            Recommendations.Add(ESixStrategies::BreakSituation);
        }
    }

    // 根據五行階段推薦
    switch (CurrentRotation.CurrentElement)
    {
    case EFiveElements::Wood:
        Recommendations.Add(ESixStrategies::EstablishNation);
        break;
    case EFiveElements::Fire:
        Recommendations.Add(ESixStrategies::BreakSituation); // 燃料
        break;
    case EFiveElements::Earth:
        Recommendations.Add(ESixStrategies::EstablishSystem);
        Recommendations.Add(ESixStrategies::EstablishPeople);
        break;
    case EFiveElements::Metal:
        Recommendations.Add(ESixStrategies::BreakStructure);
        break;
    case EFiveElements::Water:
        // 水階段：留白，不推薦主動策略
        break;
    }

    return Recommendations;
}

void UMingSageCommandSystem::UpdateSageType()
{
    // 根據當前檔案更新聖者類型
    if (CurrentProfile.SelfControlLevel < FallWarningThreshold)
    {
        if (CurrentProfile.UnconventionalActionsCount > CurrentProfile.VictoriesCount)
        {
            CurrentProfile.SageType = ESageType::DemonKing;
        }
        else
        {
            CurrentProfile.SageType = ESageType::FalseSage;
        }
    }
    else
    {
        CurrentProfile.SageType = ESageType::Sage;
    }
}

void UMingSageCommandSystem::CheckAndUpdateRotation()
{
    // 檢查五行輪轉進度
    CurrentRotation.PhaseProgress += FiveElementsRotationSpeed;

    if (CurrentRotation.PhaseProgress >= CurrentRotation.TransitionThreshold)
    {
        // 轉換到下一個元素
        EFiveElements NextElement = static_cast<EFiveElements>((static_cast<int32>(CurrentRotation.CurrentElement) + 1) % 5);
        CurrentRotation.CurrentElement = NextElement;
        CurrentRotation.NextElement = static_cast<EFiveElements>((static_cast<int32>(NextElement) + 1) % 5);
        CurrentRotation.PhaseProgress = 0.0f;
        CurrentRotation.bIsTransitioning = true;

        // 更新可用策略
        UpdateAvailableStrategies();
    }
    else
    {
        CurrentRotation.bIsTransitioning = false;
    }
}

void UMingSageCommandSystem::UpdateAvailableStrategies()
{
    CurrentRotation.AvailableStrategies.Empty();

    // 根據當前五行階段更新可用策略
    switch (CurrentRotation.CurrentElement)
    {
    case EFiveElements::Wood:
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::EstablishNation);
        break;
    case EFiveElements::Fire:
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::EstablishNation);
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::BreakSituation);
        break;
    case EFiveElements::Earth:
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::EstablishSystem);
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::EstablishPeople);
        break;
    case EFiveElements::Metal:
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::BreakStructure);
        CurrentRotation.AvailableStrategies.Add(ESixStrategies::PreservePeople);
        break;
    case EFiveElements::Water:
        // 水階段：留白，不推薦主動策略
        break;
    }
}

void UMingSageCommandSystem::RecordDecision(const FCommandDecision& Decision)
{
    DecisionHistory.Add(Decision);

    // 更新聖者檔案
    if (Decision.Nature == EYinYangNature::Yin)
    {
        CurrentProfile.UnconventionalActionsCount++;
    }

    if (Decision.SuccessProbability > 70.0f)
    {
        CurrentProfile.VictoriesCount++;
    }

    // 更新自制力
    CurrentProfile.SelfControlLevel -= Decision.MoralCost * 0.1f;
    CurrentProfile.SelfControlLevel = FMath::Clamp(CurrentProfile.SelfControlLevel, 0.0f, 100.0f);

    // 檢查聖者類型
    UpdateSageType();
}

bool UMingSageCommandSystem::IsStrategyCompatible(ESixStrategies Strategy, EFiveElements Phase)
{
    // 檢查策略與五行階段的兼容性
    switch (Phase)
    {
    case EFiveElements::Wood:
        return Strategy == ESixStrategies::EstablishNation;
    case EFiveElements::Fire:
        return Strategy == ESixStrategies::EstablishNation || Strategy == ESixStrategies::BreakSituation;
    case EFiveElements::Earth:
        return Strategy == ESixStrategies::EstablishSystem || Strategy == ESixStrategies::EstablishPeople;
    case EFiveElements::Metal:
        return Strategy == ESixStrategies::BreakStructure || Strategy == ESixStrategies::PreservePeople;
    case EFiveElements::Water:
        return false; // 水階段不推薦主動策略
    }
    return false;
}
