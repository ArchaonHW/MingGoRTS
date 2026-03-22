#include "MingFiveElementsSystem.h"
#include "Engine/World.h"

UMingFiveElementsSystem::UMingFiveElementsSystem()
{
    RotationSpeed = 1.0f;
    TransitionThreshold = 80.0f;
    RhythmThreshold = 0.5f;
}

void UMingFiveElementsSystem::InitializeFiveElementsSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化五行系統..."));

    // 初始化五行狀態
    CurrentState = FFiveElementsState();
    CurrentState.CurrentPhase = EFiveElementsPhase::Wood;
    CurrentState.NextPhase = EFiveElementsPhase::Fire;
    CurrentState.PhaseProgress = 0.0f;
    CurrentState.TransitionThreshold = TransitionThreshold;
    CurrentState.bIsTransitioning = false;
    CurrentState.CycleCount = 0.0f;

    // 初始化五行節奏
    CurrentRhythm = FFiveElementsRhythm();
    CurrentRhythm.WoodRhythm = 1.0f;
    CurrentRhythm.FireRhythm = 1.0f;
    CurrentRhythm.EarthRhythm = 1.0f;
    CurrentRhythm.MetalRhythm = 1.0f;
    CurrentRhythm.WaterRhythm = 1.0f;
    CurrentRhythm.OverallRhythm = 1.0f;
    CurrentRhythm.bIsHarmonious = true;

    // 初始化階段策略
    InitializePhaseStrategies();

    UE_LOG(LogTemp, Log, TEXT("五行系統初始化完成"));
}

FFiveElementsState UMingFiveElementsSystem::ExecuteRotation()
{
    // 更新當前階段進度
    CurrentState.PhaseProgress += RotationSpeed;

    // 檢查是否可以轉換
    if (CanTransitionToNext())
    {
        ExecutePhaseTransition();
    }

    // 更新節奏
    UpdateRhythm();

    return CurrentState;
}

TArray<FFiveElementsStrategy> UMingFiveElementsSystem::GetCurrentPhaseStrategies()
{
    if (PhaseStrategies.Contains(CurrentState.CurrentPhase))
    {
        return PhaseStrategies[CurrentState.CurrentPhase];
    }

    return TArray<FFiveElementsStrategy>();
}

EFiveElementsRelation UMingFiveElementsSystem::CheckElementsRelation(EFiveElementsPhase Element1, EFiveElementsPhase Element2)
{
    // 五行相生關係
    if ((Element1 == EFiveElementsPhase::Wood && Element2 == EFiveElementsPhase::Fire) ||
        (Element1 == EFiveElementsPhase::Fire && Element2 == EFiveElementsPhase::Earth) ||
        (Element1 == EFiveElementsPhase::Earth && Element2 == EFiveElementsPhase::Metal) ||
        (Element1 == EFiveElementsPhase::Metal && Element2 == EFiveElementsPhase::Water) ||
        (Element1 == EFiveElementsPhase::Water && Element2 == EFiveElementsPhase::Wood))
    {
        return EFiveElementsRelation::Generating;
    }

    // 五行相剋關係
    if ((Element1 == EFiveElementsPhase::Wood && Element2 == EFiveElementsPhase::Earth) ||
        (Element1 == EFiveElementsPhase::Fire && Element2 == EFiveElementsPhase::Metal) ||
        (Element1 == EFiveElementsPhase::Earth && Element2 == EFiveElementsPhase::Water) ||
        (Element1 == EFiveElementsPhase::Metal && Element2 == EFiveElementsPhase::Wood) ||
        (Element1 == EFiveElementsPhase::Water && Element2 == EFiveElementsPhase::Fire))
    {
        return EFiveElementsRelation::Overcoming;
    }

    // 五行相侮關係（反向相剋）
    if ((Element1 == EFiveElementsPhase::Earth && Element2 == EFiveElementsPhase::Wood) ||
        (Element1 == EFiveElementsPhase::Metal && Element2 == EFiveElementsPhase::Fire) ||
        (Element1 == EFiveElementsPhase::Water && Element2 == EFiveElementsPhase::Earth) ||
        (Element1 == EFiveElementsPhase::Wood && Element2 == EFiveElementsPhase::Metal) ||
        (Element1 == EFiveElementsPhase::Fire && Element2 == EFiveElementsPhase::Water))
    {
        return EFiveElementsRelation::Insulting;
    }

    // 扶助關係
    return EFiveElementsRelation::Supporting;
}

FFiveElementsRhythm UMingFiveElementsSystem::CalculateRhythm()
{
    // 計算各階段節奏
    CurrentRhythm.WoodRhythm = CalculatePhaseEfficiency(EFiveElementsPhase::Wood);
    CurrentRhythm.FireRhythm = CalculatePhaseEfficiency(EFiveElementsPhase::Fire);
    CurrentRhythm.EarthRhythm = CalculatePhaseEfficiency(EFiveElementsPhase::Earth);
    CurrentRhythm.MetalRhythm = CalculatePhaseEfficiency(EFiveElementsPhase::Metal);
    CurrentRhythm.WaterRhythm = CalculatePhaseEfficiency(EFiveElementsPhase::Water);

    // 計算整體節奏
    CurrentRhythm.OverallRhythm = (CurrentRhythm.WoodRhythm + CurrentRhythm.FireRhythm + 
                                   CurrentRhythm.EarthRhythm + CurrentRhythm.MetalRhythm + 
                                   CurrentRhythm.WaterRhythm) / 5.0f;

    // 檢查和諧性
    float Variance = FMath::Pow(CurrentRhythm.WoodRhythm - CurrentRhythm.OverallRhythm, 2) +
                     FMath::Pow(CurrentRhythm.FireRhythm - CurrentRhythm.OverallRhythm, 2) +
                     FMath::Pow(CurrentRhythm.EarthRhythm - CurrentRhythm.OverallRhythm, 2) +
                     FMath::Pow(CurrentRhythm.MetalRhythm - CurrentRhythm.OverallRhythm, 2) +
                     FMath::Pow(CurrentRhythm.WaterRhythm - CurrentRhythm.OverallRhythm, 2);

    CurrentRhythm.bIsHarmonious = (Variance < RhythmThreshold);

    return CurrentRhythm;
}

bool UMingFiveElementsSystem::CanTransitionToNext()
{
    // 檢查轉換條件
    if (CurrentState.PhaseProgress < CurrentState.TransitionThreshold)
    {
        return false;
    }

    // 檢查轉換是否有效
    return IsTransitionValid(CurrentState.CurrentPhase, CurrentState.NextPhase);
}

bool UMingFiveElementsSystem::ExecutePhaseTransition()
{
    // 記錄當前階段
    CurrentState.CompletedPhases.Add(CurrentState.CurrentPhase);

    // 執行轉換
    EFiveElementsPhase PreviousPhase = CurrentState.CurrentPhase;
    CurrentState.CurrentPhase = CurrentState.NextPhase;
    CurrentState.NextPhase = GetRecommendedNextPhase();
    CurrentState.PhaseProgress = 0.0f;
    CurrentState.bIsTransitioning = true;

    // 更新循環計數
    if (CurrentState.CurrentPhase == EFiveElementsPhase::Wood)
    {
        CurrentState.CycleCount++;
    }

    // 記錄轉換
    RecordPhaseTransition(PreviousPhase, CurrentState.CurrentPhase);

    UE_LOG(LogTemp, Log, TEXT("五行階段轉換：%s -> %s"), 
        *GetPhaseName(PreviousPhase), *GetPhaseName(CurrentState.CurrentPhase));

    return true;
}

EFiveElementsPhase UMingFiveElementsSystem::GetRecommendedNextPhase()
{
    // 按照相生順序推薦下一階段
    switch (CurrentState.CurrentPhase)
    {
    case EFiveElementsPhase::Wood:
        return EFiveElementsPhase::Fire;
    case EFiveElementsPhase::Fire:
        return EFiveElementsPhase::Earth;
    case EFiveElementsPhase::Earth:
        return EFiveElementsPhase::Metal;
    case EFiveElementsPhase::Metal:
        return EFiveElementsPhase::Water;
    case EFiveElementsPhase::Water:
        return EFiveElementsPhase::Wood;
    default:
        return EFiveElementsPhase::Wood;
    }
}

bool UMingFiveElementsSystem::IsElementsOrderDisrupted()
{
    // 檢查五行秩序是否被破壞
    // 主要檢查節奏和諧性和轉換有效性
    if (!CurrentRhythm.bIsHarmonious)
    {
        return true;
    }

    // 檢查是否有錯誤的轉換
    if (!IsTransitionValid(CurrentState.CurrentPhase, CurrentState.NextPhase))
    {
        return true;
    }

    return false;
}

bool UMingFiveElementsSystem::RestoreElementsOrder()
{
    // 修復五行秩序
    if (!IsElementsOrderDisrupted())
    {
        return true; // 秩序正常
    }

    // 重置到木階段
    CurrentState.CurrentPhase = EFiveElementsPhase::Wood;
    CurrentState.NextPhase = EFiveElementsPhase::Fire;
    CurrentState.PhaseProgress = 0.0f;
    CurrentState.bIsTransitioning = false;

    // 重新計算節奏
    UpdateRhythm();

    UE_LOG(LogTemp, Log, TEXT("五行秩序已修復"));
    return true;
}

FString UMingFiveElementsSystem::GetPhaseName(EFiveElementsPhase Phase)
{
    switch (Phase)
    {
    case EFiveElementsPhase::Wood:
        return TEXT("木");
    case EFiveElementsPhase::Fire:
        return TEXT("火");
    case EFiveElementsPhase::Earth:
        return TEXT("土");
    case EFiveElementsPhase::Metal:
        return TEXT("金");
    case EFiveElementsPhase::Water:
        return TEXT("水");
    default:
        return TEXT("未知");
    }
}

FString UMingFiveElementsSystem::GetPhaseDescription(EFiveElementsPhase Phase)
{
    switch (Phase)
    {
    case EFiveElementsPhase::Wood:
        return TEXT("木：立名與正當性，開局、立旗、定名、取義、爭正當、聚人心");
    case EFiveElementsPhase::Fire:
        return TEXT("火：造勢與加速，張聲、擴熱、速傳、激情、乘勢、燒原");
    case EFiveElementsPhase::Earth:
        return TEXT("土：收權與止亂，聚權、固勢、止紛、定分、實利、安眾");
    case EFiveElementsPhase::Metal:
        return TEXT("金：裁斷與清除，決斷、肅清、去冗、除敵、收兵、絕後患");
    case EFiveElementsPhase::Water:
        return TEXT("水：留白與不動，藏鋒、養勢、留變、觀時、不動、待機");
    default:
        return TEXT("未知階段");
    }
}

void UMingFiveElementsSystem::InitializePhaseStrategies()
{
    PhaseStrategies.Empty();

    // 木階段策略
    TArray<FFiveElementsStrategy> WoodStrategies;
    FFiveElementsStrategy WoodStrategy;
    WoodStrategy.TargetPhase = EFiveElementsPhase::Wood;
    WoodStrategy.StrategyName = TEXT("立國");
    WoodStrategy.Description = TEXT("立大義、立旗幟、立共同之志");
    WoodStrategy.ExecutionTime = 10.0f;
    WoodStrategy.SuccessRate = 85.0f;
    WoodStrategy.bIsAvailable = true;
    WoodStrategies.Add(WoodStrategy);
    PhaseStrategies.Add(EFiveElementsPhase::Wood, WoodStrategies);

    // 火階段策略
    TArray<FFiveElementsStrategy> FireStrategies;
    FFiveElementsStrategy FireStrategy;
    FireStrategy.TargetPhase = EFiveElementsPhase::Fire;
    FireStrategy.StrategyName = TEXT("造勢");
    FireStrategy.Description = TEXT("張聲、擴熱、速傳、激情、乘勢");
    FireStrategy.ExecutionTime = 5.0f;
    FireStrategy.SuccessRate = 75.0f;
    FireStrategy.bIsAvailable = true;
    FireStrategies.Add(FireStrategy);
    PhaseStrategies.Add(EFiveElementsPhase::Fire, FireStrategies);

    // 土階段策略
    TArray<FFiveElementsStrategy> EarthStrategies;
    FFiveElementsStrategy EarthStrategy;
    EarthStrategy.TargetPhase = EFiveElementsPhase::Earth;
    EarthStrategy.StrategyName = TEXT("收權");
    EarthStrategy.Description = TEXT("聚權、固勢、止紛、定分、實利");
    EarthStrategy.ExecutionTime = 15.0f;
    EarthStrategy.SuccessRate = 90.0f;
    EarthStrategy.bIsAvailable = true;
    EarthStrategies.Add(EarthStrategy);
    PhaseStrategies.Add(EFiveElementsPhase::Earth, EarthStrategies);

    // 金階段策略
    TArray<FFiveElementsStrategy> MetalStrategies;
    FFiveElementsStrategy MetalStrategy;
    MetalStrategy.TargetPhase = EFiveElementsPhase::Metal;
    MetalStrategy.StrategyName = TEXT("裁斷");
    MetalStrategy.Description = TEXT("決斷、肅清、去冗、除敵、收兵");
    MetalStrategy.ExecutionTime = 8.0f;
    MetalStrategy.SuccessRate = 80.0f;
    MetalStrategy.bIsAvailable = true;
    MetalStrategies.Add(MetalStrategy);
    PhaseStrategies.Add(EFiveElementsPhase::Metal, MetalStrategies);

    // 水階段策略
    TArray<FFiveElementsStrategy> WaterStrategies;
    FFiveElementsStrategy WaterStrategy;
    WaterStrategy.TargetPhase = EFiveElementsPhase::Water;
    WaterStrategy.StrategyName = TEXT("留白");
    WaterStrategy.Description = TEXT("藏鋒、養勢、留變、觀時、不動");
    WaterStrategy.ExecutionTime = 20.0f;
    WaterStrategy.SuccessRate = 95.0f;
    WaterStrategy.bIsAvailable = true;
    WaterStrategies.Add(WaterStrategy);
    PhaseStrategies.Add(EFiveElementsPhase::Water, WaterStrategies);
}

void UMingFiveElementsSystem::UpdateRhythm()
{
    CalculateRhythm();
}

bool UMingFiveElementsSystem::IsTransitionValid(EFiveElementsPhase From, EFiveElementsPhase To)
{
    // 檢查轉換是否遵循相生順序
    EFiveElementsRelation Relation = CheckElementsRelation(From, To);
    return (Relation == EFiveElementsRelation::Generating);
}

void UMingFiveElementsSystem::RecordPhaseTransition(EFiveElementsPhase From, EFiveElementsPhase To)
{
    // 記錄階段轉換歷史
    UE_LOG(LogTemp, Log, TEXT("記錄五行轉換：%s -> %s"), 
        *GetPhaseName(From), *GetPhaseName(To));
}

float UMingFiveElementsSystem::CalculatePhaseEfficiency(EFiveElementsPhase Phase)
{
    // 計算階段效率
    // 基於當前階段進度和歷史表現
    float BaseEfficiency = 1.0f;

    // 根據階段調整效率
    switch (Phase)
    {
    case EFiveElementsPhase::Wood:
        BaseEfficiency = 0.8f; // 木階段較慢但穩定
        break;
    case EFiveElementsPhase::Fire:
        BaseEfficiency = 1.2f; // 火階段快速但風險高
        break;
    case EFiveElementsPhase::Earth:
        BaseEfficiency = 1.0f; // 土階段平衡
        break;
    case EFiveElementsPhase::Metal:
        BaseEfficiency = 0.9f; // 金階段精確但耗時
        break;
    case EFiveElementsPhase::Water:
        BaseEfficiency = 0.7f; // 水階段緩慢但安全
        break;
    }

    // 根據當前進度調整
    if (Phase == CurrentState.CurrentPhase)
    {
        BaseEfficiency *= (1.0f + CurrentState.PhaseProgress / 100.0f);
    }

    return BaseEfficiency;
}
