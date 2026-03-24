#include "SageCommand/MingDaoAuthority.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingDaoAuthority::UMingDaoAuthority()
    : GovernanceLevel(1)
    , MoralLevel(1)
    , BalanceLevel(1)
    , HarmonyLevel(1)
    , bSystemInitialized(false)
    , bIsEnlightened(false)
{
    // 初始化道權數據數組
    DaoAuthorityDataArray.SetNum(4);
    
    // 初始化治理權
    DaoAuthorityDataArray[0].AuthorityType = EDaoAuthorityType::Governance;
    DaoAuthorityDataArray[0].AuthorityState = EDaoAuthorityState::Inactive;
    DaoAuthorityDataArray[0].AuthorityValue = 50.0f;
    DaoAuthorityDataArray[0].MaxAuthorityValue = 100.0f;
    DaoAuthorityDataArray[0].GovernanceAbility = 50.0f;
    DaoAuthorityDataArray[0].MoralInfluence = 30.0f;
    DaoAuthorityDataArray[0].BalanceIndex = 50.0f;
    DaoAuthorityDataArray[0].HarmonyLevel = 40.0f;
    DaoAuthorityDataArray[0].InfluenceRange = 100.0f;
    DaoAuthorityDataArray[0].Duration = 0.0f;
    DaoAuthorityDataArray[0].CooldownTime = 0.0f;
    
    // 初始化道德權
    DaoAuthorityDataArray[1].AuthorityType = EDaoAuthorityType::Moral;
    DaoAuthorityDataArray[1].AuthorityState = EDaoAuthorityState::Inactive;
    DaoAuthorityDataArray[1].AuthorityValue = 50.0f;
    DaoAuthorityDataArray[1].MaxAuthorityValue = 100.0f;
    DaoAuthorityDataArray[1].GovernanceAbility = 30.0f;
    DaoAuthorityDataArray[1].MoralInfluence = 60.0f;
    DaoAuthorityDataArray[1].BalanceIndex = 50.0f;
    DaoAuthorityDataArray[1].HarmonyLevel = 50.0f;
    DaoAuthorityDataArray[1].InfluenceRange = 80.0f;
    DaoAuthorityDataArray[1].Duration = 0.0f;
    DaoAuthorityDataArray[1].CooldownTime = 0.0f;
    
    // 初始化平衡權
    DaoAuthorityDataArray[2].AuthorityType = EDaoAuthorityType::Balance;
    DaoAuthorityDataArray[2].AuthorityState = EDaoAuthorityState::Inactive;
    DaoAuthorityDataArray[2].AuthorityValue = 50.0f;
    DaoAuthorityDataArray[2].MaxAuthorityValue = 100.0f;
    DaoAuthorityDataArray[2].GovernanceAbility = 40.0f;
    DaoAuthorityDataArray[2].MoralInfluence = 40.0f;
    DaoAuthorityDataArray[2].BalanceIndex = 70.0f;
    DaoAuthorityDataArray[2].HarmonyLevel = 60.0f;
    DaoAuthorityDataArray[2].InfluenceRange = 120.0f;
    DaoAuthorityDataArray[2].Duration = 0.0f;
    DaoAuthorityDataArray[2].CooldownTime = 0.0f;
    
    // 初始化和諧權
    DaoAuthorityDataArray[3].AuthorityType = EDaoAuthorityType::Harmony;
    DaoAuthorityDataArray[3].AuthorityState = EDaoAuthorityState::Inactive;
    DaoAuthorityDataArray[3].AuthorityValue = 50.0f;
    DaoAuthorityDataArray[3].MaxAuthorityValue = 100.0f;
    DaoAuthorityDataArray[3].GovernanceAbility = 35.0f;
    DaoAuthorityDataArray[3].MoralInfluence = 50.0f;
    DaoAuthorityDataArray[3].BalanceIndex = 60.0f;
    DaoAuthorityDataArray[3].HarmonyLevel = 80.0f;
    DaoAuthorityDataArray[3].InfluenceRange = 150.0f;
    DaoAuthorityDataArray[3].Duration = 0.0f;
    DaoAuthorityDataArray[3].CooldownTime = 0.0f;
}

// ============================================================================
// 系統初始化
// ============================================================================

void UMingDaoAuthority::InitializeDaoAuthority()
{
    if (bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("道權系統已初始化"));
        return;
    }

    InitializeDaoAuthorityData();

    // 計算初始效果
    CalculateDaoAuthorityEffect(EDaoAuthorityType::Governance);

    bSystemInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("道權系統初始化完e"));
}

void UMingDaoAuthority::ResetDaoAuthority()
{
    // 重置所有道權數據
    for (int32 i = 0; i < DaoAuthorityDataArray.Num(); ++i)
    {
        DaoAuthorityDataArray[i].AuthorityValue = 50.0f;
        DaoAuthorityDataArray[i].AuthorityState = EDaoAuthorityState::Inactive;
        DaoAuthorityDataArray[i].Duration = 0.0f;
        DaoAuthorityDataArray[i].CooldownTime = 0.0f;
    }

    // 重置等級
    GovernanceLevel = 1;
    MoralLevel = 1;
    BalanceLevel = 1;
    HarmonyLevel = 1;
    bIsEnlightened = false;

    UE_LOG(LogTemp, Log, TEXT("道權系統已重置"));
}

// ============================================================================
// 道權管理
// ============================================================================

bool UMingDaoAuthority::ActivateDaoAuthority(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("無效N道權類型"));
        return false;
    }

    FDaoAuthorityData& AuthorityData = DaoAuthorityDataArray[AuthorityIndex];
    
    if (AuthorityData.CooldownTime > 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("道權冷卻中，無法激活"));
        return false;
    }

    if (AuthorityData.AuthorityValue <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("道權值不足，無法激活"));
        return false;
    }

    AuthorityData.AuthorityState = EDaoAuthorityState::Active;
    AuthorityData.Duration = 45.0f; // 45秒持續時間

    // 計算並應用效果
    CalculateDaoAuthorityEffect(AuthorityType);
    ApplyDaoAuthorityTraits(AuthorityType);

    // 檢查覺醒條件
    if (CheckEnlightenment())
    {
        EnterEnlightenedState(AuthorityType);
    }

    // 觸發事件
    OnDaoAuthorityActivated.Broadcast(AuthorityType);

    UE_LOG(LogTemp, Log, TEXT("道權激活e功：%d"), (int32)AuthorityType);
    return true;
}

bool UMingDaoAuthority::DeactivateDaoAuthority(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("無效N道權類型"));
        return false;
    }

    FDaoAuthorityData& AuthorityData = DaoAuthorityDataArray[AuthorityIndex];
    AuthorityData.AuthorityState = EDaoAuthorityState::Inactive;
    AuthorityData.Duration = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("道權停用e功：%d"), (int32)AuthorityType);
    return true;
}

FDaoAuthorityData UMingDaoAuthority::GetDaoAuthorityData(EDaoAuthorityType AuthorityType) const
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex >= 0 && AuthorityIndex < DaoAuthorityDataArray.Num())
    {
        return DaoAuthorityDataArray[AuthorityIndex];
    }
    
    return FDaoAuthorityData(); // 返回默認值
}

void UMingDaoAuthority::SetDaoAuthorityData(EDaoAuthorityType AuthorityType, const FDaoAuthorityData& AuthorityData)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex >= 0 && AuthorityIndex < DaoAuthorityDataArray.Num())
    {
        DaoAuthorityDataArray[AuthorityIndex] = AuthorityData;
        UpdateDaoAuthorityState(AuthorityType);
    }
}

// ============================================================================
// 治理權系統
// ============================================================================

void UMingDaoAuthority::ApplyGovernanceEffect(const FString& TargetRegion, float GovernancePower)
{
    FDaoAuthorityData& GovernanceData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Governance];
    
    if (GovernanceData.AuthorityState != EDaoAuthorityState::Active)
    {
        UE_LOG(LogTemp, Warning, TEXT("治理權未激活"));
        return;
    }

    // 計算治理效果
    float BaseGovernance = GovernanceData.GovernanceAbility;
    float EnhancedGovernance = BaseGovernance * (1.0f + GovernancePower / 100.0f);
    
    // 覺醒狀態加e
    if (bIsEnlightened)
    {
        EnhancedGovernance *= 1.5f;
    }

    // 應用治理效果
    float GovernanceBonus = EnhancedGovernance - BaseGovernance;
    
    // 觸發事件
    OnGovernanceApplied.Broadcast(GovernanceBonus, TargetRegion);

    UE_LOG(LogTemp, Log, TEXT("治理效果已應用到 %s，加e：%.2f"), *TargetRegion, GovernanceBonus);
}

bool UMingDaoAuthority::EnhanceGovernanceAbility(float EnhancementAmount)
{
    FDaoAuthorityData& GovernanceData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Governance];
    
    float OldAbility = GovernanceData.GovernanceAbility;
    GovernanceData.GovernanceAbility = FMath::Clamp(GovernanceData.GovernanceAbility + EnhancementAmount, 0.0f, 100.0f);
    
    bool bEnhanced = GovernanceData.GovernanceAbility > OldAbility;
    
    if (bEnhanced)
    {
        UE_LOG(LogTemp, Log, TEXT("治理能力提升：%.2f -> %.2f"), OldAbility, GovernanceData.GovernanceAbility);
    }
    
    return bEnhanced;
}

// ============================================================================
// 道德權系統
// ============================================================================

void UMingDaoAuthority::ApplyMoralInfluence(const FString& TargetGroup, float MoralPower)
{
    FDaoAuthorityData& MoralData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Moral];
    
    if (MoralData.AuthorityState != EDaoAuthorityState::Active)
    {
        UE_LOG(LogTemp, Warning, TEXT("道德權未激活"));
        return;
    }

    // 計算道德影響
    float BaseMoral = MoralData.MoralInfluence;
    float EnhancedMoral = BaseMoral * (1.0f + MoralPower / 100.0f);
    
    // 覺醒狀態加e
    if (bIsEnlightened)
    {
        EnhancedMoral *= 1.4f;
    }

    UE_LOG(LogTemp, Log, TEXT("道德影響已應用到 %s，強度：%.2f"), *TargetGroup, EnhancedMoral);
}

bool UMingDaoAuthority::CheckMoralStatus()
{
    FDaoAuthorityData& MoralData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Moral];
    
    // 檢查道德狀態
    bool bHighMorality = MoralData.MoralInfluence >= 70.0f;
    bool bActiveState = MoralData.AuthorityState == EDaoAuthorityState::Active;
    bool bEnlightenedBonus = bIsEnlightened;
    
    return bHighMorality && bActiveState && bEnlightenedBonus;
}

float UMingDaoAuthority::PurifyCorruption(float CorruptionAmount)
{
    FDaoAuthorityData& MoralData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Moral];
    
    if (MoralData.AuthorityState != EDaoAuthorityState::Active)
    {
        return CorruptionAmount; // 無法淨化
    }

    // 計算淨化能力
    float PurificationPower = MoralData.MoralInfluence / 100.0f;
    
    // 覺醒狀態加e
    if (bIsEnlightened)
    {
        PurificationPower *= 2.0f; // 覺醒狀態淨化能力翻倍
    }
    
    float PurifiedAmount = CorruptionAmount * PurificationPower;
    float RemainingCorruption = FMath::Max(0.0f, CorruptionAmount - PurifiedAmount);
    
    UE_LOG(LogTemp, Log, TEXT("淨化墮落：%.2f -> %.2f"), CorruptionAmount, RemainingCorruption);
    
    return RemainingCorruption;
}

// ============================================================================
// 平衡權系統
// ============================================================================

float UMingDaoAuthority::CalculateBalanceIndex()
{
    FDaoAuthorityData& BalanceData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Balance];
    
    // 計算各權力N平衡度
    float GovernanceValue = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Governance].AuthorityValue;
    float MoralValue = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Moral].AuthorityValue;
    float HarmonyValue = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Harmony].AuthorityValue;
    
    // 計算理想平衡值
    float TotalValue = GovernanceValue + MoralValue + HarmonyValue;
    float IdealBalance = TotalValue / 3.0f;
    
    // 計算偏差
    float Deviation = FMath::Abs(GovernanceValue - IdealBalance) +
                     FMath::Abs(MoralValue - IdealBalance) +
                     FMath::Abs(HarmonyValue - IdealBalance);
    
    // 計算平衡指數
    float BalanceIndex = FMath::Max(0.0f, 100.0f - (Deviation / IdealBalance * 50.0f));
    
    // 更新平衡權數據
    BalanceData.BalanceIndex = BalanceIndex;
    
    return BalanceIndex;
}

void UMingDaoAuthority::AdjustPowerBalance()
{
    float BalanceIndex = CalculateBalanceIndex();
    
    if (BalanceIndex < 70.0f) // 平衡度低於70%需要調整
    {
        // 自動平衡調整
        float TotalValue = 0.0f;
        for (int32 i = 0; i < 3; ++i) // 不包括平衡權r身
        {
            TotalValue += DaoAuthorityDataArray[i].AuthorityValue;
        }
        
        float IdealValue = TotalValue / 3.0f;
        
        // 微調各權力值
        for (int32 i = 0; i < 3; ++i)
        {
            float Difference = IdealValue - DaoAuthorityDataArray[i].AuthorityValue;
            float Adjustment = Difference * 0.1f; // 10%調整幅度
            DaoAuthorityDataArray[i].AuthorityValue += Adjustment;
            DaoAuthorityDataArray[i].AuthorityValue = FMath::Clamp(DaoAuthorityDataArray[i].AuthorityValue, 0.0f, DaoAuthorityDataArray[i].MaxAuthorityValue);
        }
        
        UE_LOG(LogTemp, Log, TEXT("權力平衡已自動調整"));
    }
}

bool UMingDaoAuthority::IsBalanced() const
{
    return CalculateBalanceIndex() >= 70.0f;
}

// ============================================================================
// 和諧權系統
// ============================================================================

void UMingDaoAuthority::ApplyHarmonyEffect(const FString& TargetArea, float HarmonyPower)
{
    FDaoAuthorityData& HarmonyData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Harmony];
    
    if (HarmonyData.AuthorityState != EDaoAuthorityState::Active)
    {
        UE_LOG(LogTemp, Warning, TEXT("和諧權未激活"));
        return;
    }

    // 計算和諧效果
    float BaseHarmony = HarmonyData.HarmonyLevel;
    float EnhancedHarmony = BaseHarmony * (1.0f + HarmonyPower / 100.0f);
    
    // 覺醒狀態加e
    if (bIsEnlightened)
    {
        EnhancedHarmony *= 1.3f;
    }

    UE_LOG(LogTemp, Log, TEXT("和諧效果已應用到 %s，強度：%.2f"), *TargetArea, EnhancedHarmony);
}

bool UMingDaoAuthority::IncreaseHarmonyLevel(float HarmonyAmount)
{
    FDaoAuthorityData& HarmonyData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Harmony];
    
    float OldLevel = HarmonyData.HarmonyLevel;
    HarmonyData.HarmonyLevel = FMath::Clamp(HarmonyData.HarmonyLevel + HarmonyAmount, 0.0f, 100.0f);
    
    bool bIncreased = HarmonyData.HarmonyLevel > OldLevel;
    
    if (bIncreased)
    {
        HarmonyLevel++;
        UE_LOG(LogTemp, Log, TEXT("和諧度提升：%.2f -> %.2f，等級：%d"), OldLevel, HarmonyData.HarmonyLevel, HarmonyLevel);
    }
    
    return bIncreased;
}

bool UMingDaoAuthority::ResolveConflict(float ConflictIntensity)
{
    FDaoAuthorityData& HarmonyData = DaoAuthorityDataArray[(int32)EDaoAuthorityType::Harmony];
    
    if (HarmonyData.AuthorityState != EDaoAuthorityState::Active)
    {
        return false; // 無法緩解衝突
    }

    // 計算緩解能力
    float ResolutionPower = HarmonyData.HarmonyLevel / 100.0f;
    
    // 覺醒狀態加e
    if (bIsEnlightened)
    {
        ResolutionPower *= 1.8f;
    }
    
    float ReducedConflict = ConflictIntensity * (1.0f - ResolutionPower);
    bool bResolved = ReducedConflict < ConflictIntensity * 0.3f; // 衝突降低70%以上視為解決
    
    UE_LOG(LogTemp, Log, TEXT("衝突緩解：%.2f -> %.2f，解決：%s"), 
        ConflictIntensity, ReducedConflict, bResolved ? TEXT("是") : TEXT("否"));
    
    return bResolved;
}

// ============================================================================
// 覺醒系統
// ============================================================================

bool UMingDaoAuthority::CheckEnlightenment()
{
    // 檢查覺醒條件
    bool bHighGovernance = GovernanceLevel >= 5;
    bool bHighMoral = MoralLevel >= 5;
    bool bHighBalance = BalanceLevel >= 5;
    bool bHighHarmony = HarmonyLevel >= 5;
    bool bBalancedState = IsBalanced();
    bool bAllActive = true;
    
    // 檢查所有道權是否激活
    for (const FDaoAuthorityData& Data : DaoAuthorityDataArray)
    {
        if (Data.AuthorityState != EDaoAuthorityState::Active)
        {
            bAllActive = false;
            break;
        }
    }
    
    return bHighGovernance && bHighMoral && bHighBalance && bHighHarmony && bBalancedState && bAllActive;
}

void UMingDaoAuthority::EnterEnlightenedState(EDaoAuthorityType AuthorityType)
{
    if (bIsEnlightened)
    {
        return; // 已經覺醒
    }
    
    bIsEnlightened = true;
    
    // 覺醒效果：所有道權值提升
    for (FDaoAuthorityData& Data : DaoAuthorityDataArray)
    {
        Data.AuthorityValue = FMath::Min(Data.MaxAuthorityValue, Data.AuthorityValue * 1.5f);
        Data.AuthorityState = EDaoAuthorityState::Enlightened;
    }
    
    // 觸發覺醒事件
    OnDaoAuthorityEnlightened.Broadcast(AuthorityType);
    
    UE_LOG(LogTemp, Log, TEXT("進入覺醒狀態！道權類型：%d"), (int32)AuthorityType);
}

float UMingDaoAuthority::ApplyEnlightenmentBonus(float BaseValue)
{
    if (!bIsEnlightened)
    {
        return BaseValue;
    }
    
    return BaseValue * 1.5f; // 覺醒狀態50%加e
}

// ============================================================================
// 升級系統
// ============================================================================

void UMingDaoAuthority::AddDaoAuthorityExperience(EDaoAuthorityType AuthorityType, int32 Experience)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return;
    }

    // 根據道權類型增加對應等級經驗
    switch (AuthorityType)
    {
    case EDaoAuthorityType::Governance:
        if (LevelUpDaoAuthority(AuthorityType))
        {
            GovernanceLevel++;
        }
        break;
    case EDaoAuthorityType::Moral:
        if (LevelUpDaoAuthority(AuthorityType))
        {
            MoralLevel++;
        }
        break;
    case EDaoAuthorityType::Balance:
        if (LevelUpDaoAuthority(AuthorityType))
        {
            BalanceLevel++;
        }
        break;
    case EDaoAuthorityType::Harmony:
        if (LevelUpDaoAuthority(AuthorityType))
        {
            HarmonyLevel++;
        }
        break;
    }
}

bool UMingDaoAuthority::CheckDaoAuthorityLevelUp(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return false;
    }

    FDaoAuthorityData& Data = DaoAuthorityDataArray[AuthorityIndex];
    
    // 簡單N升級條件：權力值達到80%以上
    return Data.AuthorityValue >= (Data.MaxAuthorityValue * 0.8f);
}

bool UMingDaoAuthority::LevelUpDaoAuthority(EDaoAuthorityType AuthorityType)
{
    if (!CheckDaoAuthorityLevelUp(AuthorityType))
    {
        return false;
    }

    int32 AuthorityIndex = (int32)AuthorityType;
    FDaoAuthorityData& Data = DaoAuthorityDataArray[AuthorityIndex];
    
    // 提升道權特性
    Data.GovernanceAbility += 5.0f;
    Data.MoralInfluence += 5.0f;
    Data.BalanceIndex += 5.0f;
    Data.HarmonyLevel += 5.0f;
    Data.MaxAuthorityValue += 20.0f;
    
    // 觸發升級事件
    OnDaoAuthorityLevelUp.Broadcast(AuthorityType, 1); // 簡化等級顯示
    
    UE_LOG(LogTemp, Log, TEXT("道權升級：%d"), (int32)AuthorityType);
    return true;
}

// ============================================================================
// 私有方法
// ============================================================================

void UMingDaoAuthority::InitializeDaoAuthorityData()
{
    // 初始化道權數據已在構造函數中完e
}

void UMingDaoAuthority::CalculateDaoAuthorityEffect(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return;
    }

    FDaoAuthorityData& Data = DaoAuthorityDataArray[AuthorityIndex];
    
    // 計算效果
    CurrentEffect.GovernanceEffect = Data.GovernanceAbility * 0.1f;
    CurrentEffect.MoralEffect = Data.MoralInfluence * 0.1f;
    CurrentEffect.BalanceEffect = Data.BalanceIndex * 0.1f;
    CurrentEffect.HarmonyEffect = Data.HarmonyLevel * 0.1f;
    CurrentEffect.EffectRadius = Data.InfluenceRange;
    CurrentEffect.EffectIntensity = Data.AuthorityValue / 100.0f;
}

void UMingDaoAuthority::UpdateDaoAuthorityState(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return;
    }

    FDaoAuthorityData& Data = DaoAuthorityDataArray[AuthorityIndex];
    
    // 更新狀態
    if (Data.Duration <= 0.0f)
    {
        Data.AuthorityState = EDaoAuthorityState::Inactive;
    }
    else if (Data.AuthorityValue >= 80.0f)
    {
        Data.AuthorityState = bIsEnlightened ? EDaoAuthorityState::Enlightened : EDaoAuthorityState::Dominant;
    }
    else if (Data.AuthorityValue >= 30.0f)
    {
        Data.AuthorityState = EDaoAuthorityState::Active;
    }
    else
    {
        Data.AuthorityState = EDaoAuthorityState::Suppressed;
    }
}

void UMingDaoAuthority::ApplyDaoAuthorityTraits(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return;
    }

    FDaoAuthorityData& Data = DaoAuthorityDataArray[AuthorityIndex];
    
    // 根據道權類型應用特性
    switch (AuthorityType)
    {
    case EDaoAuthorityType::Governance:
        // 治理權特性：提升效率和秩序
        break;
    case EDaoAuthorityType::Moral:
        // 道德權特性：提升淨化和影響
        break;
    case EDaoAuthorityType::Balance:
        // 平衡權特性：穩定各權力
        break;
    case EDaoAuthorityType::Harmony:
        // 和諧權特性：緩解衝突
        break;
    }
}

bool UMingDaoAuthority::CheckDaoAuthorityCooldown(EDaoAuthorityType AuthorityType)
{
    int32 AuthorityIndex = (int32)AuthorityType;
    if (AuthorityIndex < 0 || AuthorityIndex >= DaoAuthorityDataArray.Num())
    {
        return false;
    }

    return DaoAuthorityDataArray[AuthorityIndex].CooldownTime <= 0.0f;
}

void UMingDaoAuthority::HandleEnlightenmentTransition(EDaoAuthorityType AuthorityType)
{
    UE_LOG(LogTemp, Log, TEXT("處理覺醒轉變：%d"), (int32)AuthorityType);
    
    // 覺醒轉變邏輯
    EnterEnlightenedState(AuthorityType);
}
