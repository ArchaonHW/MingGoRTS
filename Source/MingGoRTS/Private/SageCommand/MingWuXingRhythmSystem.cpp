// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingWuXingRhythmSystem.h"

UMingWuXingRhythmSystem::UMingWuXingRhythmSystem()
    : PhaseDuration(120.0f)
    , GeneratingBaseMultiplier(1.2f)
    , OvercomingBasePenalty(0.8f)
    , ComboBonusIncrement(0.1f)
    , MaxComboCount(10)
    , bIsInitialized(false)
{
}

void UMingWuXingRhythmSystem::InitializeWuXingSystem()
{
    if (bIsInitialized)
    {
        return;
    }

    CurrentState = FWuXingRhythmState();
    bIsInitialized = true;
}

void UMingWuXingRhythmSystem::StartNewCycle()
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentState.CurrentPhase = EWuXingPhase::Wood;
    CurrentState.NextPhase = GetNextPhaseInCycle(EWuXingPhase::Wood);
    CurrentState.PhaseStartTime = FDateTime::Now();
    CurrentState.PhaseEndTime = CurrentState.PhaseStartTime + FTimespan::FromSeconds(PhaseDuration);
    CurrentState.ComboCount = 0;
    CurrentState.CurrentEffectMultiplier = 1.0f;
    CurrentState.bIsInGeneratingCycle = true;

    OnPhaseChanged.Broadcast(EWuXingPhase::None, EWuXingPhase::Wood);
}

bool UMingWuXingRhythmSystem::AdvanceToNextPhase()
{
    if (!bIsInitialized)
    {
        return false;
    }

    EWuXingPhase ExpectedNext = GetNextPhaseInCycle(CurrentState.CurrentPhase);
    return SetCurrentPhaseInternal(ExpectedNext);
}

void UMingWuXingRhythmSystem::SetCurrentPhase(EWuXingPhase NewPhase)
{
    if (!bIsInitialized)
    {
        return;
    }

    EWuXingPhase OldPhase = CurrentState.CurrentPhase;

    // 檢查是否正確相生
    if (IsPhaseTransitionCorrect(OldPhase, NewPhase))
    {
        CurrentState.ComboCount = FMath::Min(CurrentState.ComboCount + 1, MaxComboCount);
        CurrentState.bIsInGeneratingCycle = true;
        OnGeneratingCombo.Broadcast(CurrentState.ComboCount);
    }
    // 檢查是否相剋
    else if (IsPhaseOvercoming(OldPhase, NewPhase))
    {
        CurrentState.ComboCount = 0;
        CurrentState.bIsInGeneratingCycle = false;
        OnOvercomingOccured.Broadcast(OldPhase, NewPhase);
    }
    else
    {
        // 既不是相生也不是相剋，中斷連擊
        CurrentState.ComboCount = 0;
        CurrentState.bIsInGeneratingCycle = false;
    }

    CurrentState.CurrentPhase = NewPhase;
    CurrentState.NextPhase = GetNextPhaseInCycle(NewPhase);
    CurrentState.PhaseStartTime = FDateTime::Now();
    CurrentState.PhaseEndTime = CurrentState.PhaseStartTime + FTimespan::FromSeconds(PhaseDuration);
    CurrentState.CurrentEffectMultiplier = CalculateComboMultiplier();

    OnPhaseChanged.Broadcast(OldPhase, NewPhase);

    // 檢查是否完成一個循環
    if (NewPhase == EWuXingPhase::Water && OldPhase == EWuXingPhase::Metal)
    {
        // 從金到水是正確的相生，循環即將完成
    }
    else if (CheckCycleCompletion())
    {
        OnCycleCompleted.Broadcast();
    }
}

bool UMingWuXingRhythmSystem::SetCurrentPhaseInternal(EWuXingPhase NewPhase)
{
    if (NewPhase == EWuXingPhase::None || NewPhase == EWuXingPhase::Count)
    {
        return false;
    }

    EWuXingPhase OldPhase = CurrentState.CurrentPhase;

    // 檢查是否正確相生
    if (IsPhaseTransitionCorrect(OldPhase, NewPhase))
    {
        CurrentState.ComboCount = FMath::Min(CurrentState.ComboCount + 1, MaxComboCount);
        CurrentState.bIsInGeneratingCycle = true;
        OnGeneratingCombo.Broadcast(CurrentState.ComboCount);
    }
    else
    {
        CurrentState.ComboCount = 0;
        CurrentState.bIsInGeneratingCycle = false;
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
    if (!bIsInitialized)
    {
        return FWuXingEffectData();
    }

    FWuXingEffectData Effects = GetPhaseEffectData(CurrentState.CurrentPhase);

    // 應用連擊加成
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

EWuXingPhase UMingWuXingRhythmSystem::GetNextPhaseInCycle(EWuXingPhase CurrentPhase) const
{
    return GetGeneratedPhase(CurrentPhase);
}

bool UMingWuXingRhythmSystem::IsPhaseTransitionCorrect(EWuXingPhase FromPhase, EWuXingPhase ToPhase) const
{
    if (FromPhase == EWuXingPhase::None)
    {
        // 從無到任何階段都算正確
        return true;
    }

    return GetGeneratedPhase(FromPhase) == ToPhase;
}

bool UMingWuXingRhythmSystem::IsPhaseOvercoming(EWuXingPhase Phase1, EWuXingPhase Phase2) const
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

FString UMingWuXingRhythmSystem::GetPhaseDescription(EWuXingPhase Phase) const
{
    switch (Phase)
    {
    case EWuXingPhase::Wood:
        return TEXT("木 (春/立名)：凡開局、立旗、定名、取義、爭正當、聚人心者，皆木之節。正策效果+20%，徵兵速度+30%");
    case EWuXingPhase::Fire:
        return TEXT("火 (夏/造勢)：凡張聲、擴熱、速傳、激情、乘勢、燒原者，皆火之節。攻擊力+25%，士氣增長+40%");
    case EWuXingPhase::Earth:
        return TEXT("土 (長夏/收權)：凡聚權、固勢、止紛、定分、實利、安眾者，皆土之節。防禦力+30%，資源產出+20%");
    case EWuXingPhase::Metal:
        return TEXT("金 (秋/裁斷)：凡決斷、肅清、去冗、除敵、收兵、絕後患者，皆金之節。裁斷效果+50%，清除成本-30%");
    case EWuXingPhase::Water:
        return TEXT("水 (冬/養機)：凡藏鋒、養勢、留變、觀時、不動、待機者，皆水之節。逆策隱蔽+40%，情報獲取+50%");
    default:
        return TEXT("未知階段");
    }
}

FString UMingWuXingRhythmSystem::GetRecommendedStrategyForCurrentPhase() const
{
    switch (CurrentState.CurrentPhase)
    {
    case EWuXingPhase::Wood:
        return TEXT("木階段建議：使用立國策略，建立正當性，徵兵擴充軍力。適合開局或發展初期。");
    case EWuXingPhase::Fire:
        return TEXT("火階段建議：主動進攻，使用破局策略乘勢而為。適合攻勢或破局關鍵時刻。");
    case EWuXingPhase::Earth:
        return TEXT("土階段建議：使用立制策略鞏固成果，加強防禦。適合守成或鞏固階段。");
    case EWuXingPhase::Metal:
        return TEXT("金階段建議：使用破結構策略肅清敵人，或進行裁斷。適合清算或收網階段。");
    case EWuXingPhase::Water:
        return TEXT("水階段建議：使用不破人策略暗奪敵志，或潛伏養機。適合潛伏或謀劃階段。");
    default:
        return TEXT("請先啟動五行循環");
    }
}

void UMingWuXingRhythmSystem::UpdateSystem()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 檢查是否需要自動轉換階段
    if (CurrentState.CurrentPhase != EWuXingPhase::None)
    {
        FDateTime CurrentTime = FDateTime::Now();
        if (CurrentTime >= CurrentState.PhaseEndTime)
        {
            // 自動轉換到下一階段
            AdvanceToNextPhase();
        }
    }
}

void UMingWuXingRhythmSystem::ResetCombo()
{
    CurrentState.ComboCount = 0;
    CurrentState.CurrentEffectMultiplier = 1.0f;
    CurrentState.bIsInGeneratingCycle = false;
}

FWuXingEffectData UMingWuXingRhythmSystem::GetPhaseEffectData(EWuXingPhase Phase) const
{
    FWuXingEffectData Effects;

    switch (Phase)
    {
    case EWuXingPhase::Wood:
        Effects.RighteousStrategyBonus = 0.20f;
        Effects.RecruitmentBonus = 0.30f;
        break;
    case EWuXingPhase::Fire:
        Effects.AttackBonus = 0.25f;
        Effects.MoraleGrowthBonus = 0.40f;
        break;
    case EWuXingPhase::Earth:
        Effects.DefenseBonus = 0.30f;
        Effects.EconomicBonus = 0.20f;
        break;
    case EWuXingPhase::Metal:
        Effects.JudgmentBonus = 0.50f;
        Effects.AttackBonus = 0.20f; // 肅清也有攻擊性
        break;
    case EWuXingPhase::Water:
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
    // 循環完成條件：從水回到木
    return CurrentState.CurrentPhase == EWuXingPhase::Wood &&
           GetNextPhaseInCycle(EWuXingPhase::Water) == EWuXingPhase::Wood;
}

EWuXingPhase UMingWuXingRhythmSystem::GetGeneratedPhase(EWuXingPhase Phase) const
{
    // 五行相生順序
    switch (Phase)
    {
    case EWuXingPhase::Wood:
        return EWuXingPhase::Fire;    // 木生火
    case EWuXingPhase::Fire:
        return EWuXingPhase::Earth;   // 火生土
    case EWuXingPhase::Earth:
        return EWuXingPhase::Metal;   // 土生金
    case EWuXingPhase::Metal:
        return EWuXingPhase::Water;   // 金生水
    case EWuXingPhase::Water:
        return EWuXingPhase::Wood;    // 水生木
    default:
        return EWuXingPhase::None;
    }
}

EWuXingPhase UMingWuXingRhythmSystem::GetOvercomePhase(EWuXingPhase Phase) const
{
    // 五行相剋順序
    switch (Phase)
    {
    case EWuXingPhase::Wood:
        return EWuXingPhase::Earth;   // 木剋土
    case EWuXingPhase::Earth:
        return EWuXingPhase::Water;   // 土剋水
    case EWuXingPhase::Water:
        return EWuXingPhase::Fire;    // 水剋火
    case EWuXingPhase::Fire:
        return EWuXingPhase::Metal;   // 火剋金
    case EWuXingPhase::Metal:
        return EWuXingPhase::Wood;    // 金剋木
    default:
        return EWuXingPhase::None;
    }
}
