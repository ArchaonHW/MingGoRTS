// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingWuXingRhythmSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingWuXingRhythmSystem::UMingWuXingRhythmSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize system parameters
    DefaultPhaseDuration = 120.0f; // Default 2 minutes
    RhythmUpdateInterval = 0.1f;
    MaxEventHistory = 100;
    
    // Initialize state
    bSystemInitialized = false;
    bRhythmActive = false;
    
    // Initialize rhythm data
    CurrentRhythmData.State = EWuXingRhythmState::Inactive;
    CurrentRhythmData.Pattern = EWuXingRhythmPattern::Cyclic;
    CurrentRhythmData.Intensity = EWuXingRhythmIntensity::Normal;
    CurrentRhythmData.CurrentPhase = EFiveElementPhase::None;
    CurrentRhythmData.PhaseProgress = 0.0f;
}

void UMingWuXingRhythmSystem::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeWuXingRhythmSystem();
    
    UE_LOG(LogTemp, Log, TEXT("Wu Xing Rhythm System initialized"));
}

void UMingWuXingRhythmSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized || !bRhythmActive)
    {
        return;
    }
    
    UpdateRhythmState(DeltaTime);
}

void UMingWuXingRhythmSystem::StartNewCycle()
{
    if (!bSystemInitialized)
    {
        return;
    }

    CurrentRhythmData.CurrentPhase = EFiveElementPhase::Wood;
    CurrentState.NextPhase = GetNextPhaseInCycle(EFiveElementPhase::Wood);
    CurrentState.PhaseStartTime = FDateTime::Now();
    CurrentState.PhaseEndTime = CurrentState.PhaseStartTime + FTimespan::FromSeconds(PhaseDuration);
    CurrentRhythmData.ComboCount = 0;
    CurrentState.CurrentEffectMultiplier = 1.0f;
    CurrentRhythmData.bIsInGeneratingCycle = true;

    OnPhaseChanged.Broadcast(EFiveElementPhase::None, EFiveElementPhase::Wood);
}

bool UMingWuXingRhythmSystem::AdvanceToNextPhase()
{
    if (!bSystemInitialized)
    {
        return false;
    }

    EFiveElementPhase ExpectedNext = GetNextPhaseInCycle(CurrentRhythmData.CurrentPhase);
    return SetCurrentPhaseInternal(ExpectedNext);
}

void UMingWuXingRhythmSystem::SetCurrentPhase(EFiveElementPhase NewPhase)
{
    if (!bSystemInitialized)
    {
        return;
    }

    EFiveElementPhase OldPhase = CurrentRhythmData.CurrentPhase;

    // 檢查是否v確相生
    if (IsPhaseTransitionCorrect(OldPhase, NewPhase))
    {
        CurrentRhythmData.ComboCount = FMath::Min(CurrentRhythmData.ComboCount + 1, MaxComboCount);
        CurrentRhythmData.bIsInGeneratingCycle = true;
        OnGeneratingCombo.Broadcast(CurrentRhythmData.ComboCount);
    }
    // 檢查是否相剋
    else if (IsPhaseOvercoming(OldPhase, NewPhase))
    {
        CurrentRhythmData.ComboCount = 0;
        CurrentRhythmData.bIsInGeneratingCycle = false;
        OnOvercomingOccurred.Broadcast(OldPhase, NewPhase);
    }
    else
    {
        // 既不是相生也不是相剋，中斷連擊
        CurrentRhythmData.ComboCount = 0;
        CurrentRhythmData.bIsInGeneratingCycle = false;
    }

    CurrentState.CurrentPhase = NewPhase;
    CurrentState.NextPhase = GetNextPhaseInCycle(NewPhase);
    CurrentState.PhaseStartTime = FDateTime::Now();
    CurrentState.PhaseEndTime = CurrentState.PhaseStartTime + FTimespan::FromSeconds(PhaseDuration);
    CurrentState.CurrentEffectMultiplier = CalculateComboMultiplier();

    OnPhaseChanged.Broadcast(OldPhase, NewPhase);

    // 檢查是否完e一個循環
    if (NewPhase == EFiveElementPhase::Water && OldPhase == EFiveElementPhase::Metal)
    {
        // 從金到水是v確N相生，循環即將完e
    }
    else if (CheckCycleCompletion())
    {
        OnCycleCompleted.Broadcast();
    }
}

bool UMingWuXingRhythmSystem::SetCurrentPhaseInternal(EFiveElementPhase NewPhase)
{
    if (NewPhase == EFiveElementPhase::None || NewPhase == EFiveElementPhase::Count)
    {
        return false;
    }

    EFiveElementPhase OldPhase = CurrentRhythmData.CurrentPhase;

    // 檢查是否v確相生
    if (IsPhaseTransitionCorrect(OldPhase, NewPhase))
    {
        CurrentRhythmData.ComboCount = FMath::Min(CurrentRhythmData.ComboCount + 1, MaxComboCount);
        CurrentRhythmData.bIsInGeneratingCycle = true;
        OnGeneratingCombo.Broadcast(CurrentRhythmData.ComboCount);
    }
    else
    {
        CurrentRhythmData.ComboCount = 0;
        CurrentRhythmData.bIsInGeneratingCycle = false;
    }

    CurrentState.CurrentPhase = NewPhase;
    CurrentState.NextPhase = GetNextPhaseInCycle(NewPhase);
    CurrentState.PhaseStartTime = FDateTime::Now();
    CurrentState.PhaseEndTime = CurrentState.PhaseStartTime + FTimespan::FromSeconds(PhaseDuration);
    CurrentState.CurrentEffectMultiplier = CalculateComboMultiplier();

    OnPhaseChanged.Broadcast(OldPhase, NewPhase);

    return true;
}

FWuXingEffectData UMingWuXingRhythmSystem::GetCurrentPhaseEffects() const
{
    if (!bSystemInitialized)
    {
        return FWuXingEffectData();
    }

    FWuXingEffectData Effects = GetPhaseEffectData(CurrentState.CurrentPhase);

    // 應用連擊加e
    float Multiplier = CalculateComboMultiplier();
    Effects.RighteousStrategyBonus *= Multiplier;
    Effects.EvilStrategyStealthBonus *= Multiplier;
    Effects.AttackBonus *= Multiplier;
    Effects.DefenseBonus *= Multiplier;
    Effects.MoraleGrowthBonus *= Multiplier;
    Effects.EconomicBonus *= Multiplier;
    Effects.RecruitmentBonus *= Multiplier;
    Effects.LeaderGrowthBonus *= Multiplier;
    Effects.JudgmentBonus *= Multiplier;
    Effects.IntelligenceBonus *= Multiplier;

    return Effects;
}

EFiveElementPhase UMingWuXingRhythmSystem::GetNextPhaseInCycle(EFiveElementPhase CurrentPhase) const
{
    return GetGeneratedPhase(CurrentPhase);
}

bool UMingWuXingRhythmSystem::IsPhaseTransitionCorrect(EFiveElementPhase FromPhase, EFiveElementPhase ToPhase) const
{
    if (FromPhase == EFiveElementPhase::None)
    {
        // 從無到任何階段都算v確
        return true;
    }

    return GetGeneratedPhase(FromPhase) == ToPhase;
}

bool UMingWuXingRhythmSystem::IsPhaseOvercoming(EFiveElementPhase Phase1, EFiveElementPhase Phase2) const
{
    return GetOvercomePhase(Phase1) == Phase2;
}

float UMingWuXingRhythmSystem::GetGeneratingMultiplier() const
{
    return GeneratingBaseMultiplier + (CurrentState.ComboCount * ComboBonusIncrement);
}

float UMingWuXingRhythmSystem::GetOvercomingPenalty() const
{
    return OvercomingBasePenalty;
}

FString UMingWuXingRhythmSystem::GetPhaseDescription(EFiveElementPhase Phase) const
{
    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        return TEXT("木 (春/立名)：凡開局、立旗、定名、取義、爭v當、聚人心者，皆木之節。v策效果+20%，徵兵速度+30%");
    case EFiveElementPhase::Fire:
        return TEXT("火 (夏/造勢)：凡張聲、擴熱、速傳、激情、乘勢、燒原者，皆火之節。攻擊力+25%，士氣增長+40%");
    case EFiveElementPhase::Earth:
        return TEXT("土 (長夏/收權)：凡聚權、固勢、止紛、定分、實利、安眾者，皆土之節。防禦力+30%，資源產+20%");
    case EFiveElementPhase::Metal:
        return TEXT("金 (秋/裁斷)：凡決斷、肅清、去冗、除敵、收兵、絕後患者，皆金之節。裁斷效果+50%，清除er-30%");
    case EFiveElementPhase::Water:
        return TEXT("水 (冬/養機)：凡藏鋒、養勢、留變、觀時、不動、待機者，皆水之節。逆策隱蔽+40%，情報獲取+50%");
    default:
        return TEXT("未知階段");
    }
}

FString UMingWuXingRhythmSystem::GetRecommendedStrategyForCurrentPhase() const
{
    switch (CurrentState.CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        return TEXT("木階段建議：i用立國策略，建立v當性，徵兵擴充F力。適合開局或發展初期。");
    case EFiveElementPhase::Fire:
        return TEXT("火階段建議：主動進攻，i用破局策略乘勢而為。適合攻勢或破局關鍵時刻。");
    case EFiveElementPhase::Earth:
        return TEXT("土階段建議：i用立制策略鞏固e果，加強防禦。適合守e或鞏固階段。");
    case EFiveElementPhase::Metal:
        return TEXT("金階段建議：i用破結構策略肅清敵人，或進行裁斷。適合清算或收網階段。");
    case EFiveElementPhase::Water:
        return TEXT("水階段建議：i用不破人策略暗奪敵志，或潛伏養機。適合潛伏或謀劃階段。");
    default:
        return TEXT("請先啟動五行循環");
    }
}

void UMingWuXingRhythmSystem::UpdateSystem()
{
    if (!bSystemInitialized)
    {
        return;
    }

    // 檢查是否需要自動轉換階段
    if (CurrentState.CurrentPhase != EFiveElementPhase::None)
    {
        FDateTime CurrentTime = FDateTime::Now();
        if (CurrentTime >= CurrentState.PhaseEndTime)
        {
            // 自動轉換到_一階段
            AdvanceToNextPhase();
        }
    }
}

void UMingWuXingRhythmSystem::ResetCombo()
{
    CurrentRhythmData.ComboCount = 0;
    CurrentState.CurrentEffectMultiplier = 1.0f;
    CurrentRhythmData.bIsInGeneratingCycle = false;
}

FWuXingEffectData UMingWuXingRhythmSystem::GetPhaseEffectData(EFiveElementPhase Phase) const
{
    FWuXingEffectData Effects;

    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        Effects.RighteousStrategyBonus = 0.20f;
        Effects.RecruitmentBonus = 0.30f;
        break;
    case EFiveElementPhase::Fire:
        Effects.AttackBonus = 0.25f;
        Effects.MoraleGrowthBonus = 0.40f;
        break;
    case EFiveElementPhase::Earth:
        Effects.DefenseBonus = 0.30f;
        Effects.EconomicBonus = 0.20f;
        break;
    case EFiveElementPhase::Metal:
        Effects.JudgmentBonus = 0.50f;
        Effects.AttackBonus = 0.20f; // 肅清也有攻擊性
        break;
    case EFiveElementPhase::Water:
        Effects.EvilStrategyStealthBonus = 0.40f;
        Effects.IntelligenceBonus = 0.50f;
        break;
    default:
        break;
    }

    return Effects;
}

float UMingWuXingRhythmSystem::CalculateComboMultiplier() const
{
    if (CurrentState.ComboCount <= 0)
    {
        return 1.0f;
    }

    return GeneratingBaseMultiplier + (CurrentState.ComboCount * ComboBonusIncrement);
}

bool UMingWuXingRhythmSystem::CheckCycleCompletion() const
{
    // 循環完e條件：從水回到木
    return CurrentState.CurrentPhase == EFiveElementPhase::Wood &&
           GetNextPhaseInCycle(EFiveElementPhase::Water) == EFiveElementPhase::Wood;
}

EFiveElementPhase UMingWuXingRhythmSystem::GetGeneratedPhase(EFiveElementPhase Phase) const
{
    // 五行相生順序
    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        return EFiveElementPhase::Fire;    // 木生火
    case EFiveElementPhase::Fire:
        return EFiveElementPhase::Earth;   // 火生土
    case EFiveElementPhase::Earth:
        return EFiveElementPhase::Metal;   // 土生金
    case EFiveElementPhase::Metal:
        return EFiveElementPhase::Water;   // 金生水
    case EFiveElementPhase::Water:
        return EFiveElementPhase::Wood;    // 水生木
    default:
        return EFiveElementPhase::None;
    }
}

EFiveElementPhase UMingWuXingRhythmSystem::GetOvercomePhase(EFiveElementPhase Phase) const
{
    // 五行相剋順序
    switch (Phase)
    {
    case EFiveElementPhase::Wood:
        return EFiveElementPhase::Earth;   // 木剋土
    case EFiveElementPhase::Earth:
        return EFiveElementPhase::Water;   // 土剋水
    case EFiveElementPhase::Water:
        return EFiveElementPhase::Fire;    // 水剋火
    case EFiveElementPhase::Fire:
        return EFiveElementPhase::Metal;   // 火剋金
    case EFiveElementPhase::Metal:
        return EFiveElementPhase::Wood;    // 金剋木
    default:
        return EFiveElementPhase::None;
    }
}
