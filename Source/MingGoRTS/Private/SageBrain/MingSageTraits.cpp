#include "SageBrain/MingSageTraits.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingSageTraits::UMingSageTraits()
{
    bIsInitialized = false;
    SageLevel = 1;
    CurrentXP = 0.0f;
    MaxMoralPurity = 100.0f;
    MaxWisdomLevel = 100.0f;
    MaxFollowers = 1000;
    MoralRestoreRate = 0.5f;
    WisdomGrowthRate = 0.3f;
    
    InitializeSageAbilities();
}

void UMingSageTraits::InitializeSageTraits()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sage traits already initialized"));
        return;
    }

    // 初始化聖者狀態
    CurrentSageStatus = FSageStatus();
    CurrentSageStatus.MoralPurity = 80.0f;
    CurrentSageStatus.WisdomLevel = 70.0f;
    CurrentSageStatus.DivineConnection = 60.0f;
    CurrentSageStatus.RighteousPower = 75.0f;
    CurrentSageStatus.EnlightenmentProgress = 0.0f;
    CurrentSageStatus.FollowerCount = 0;
    
    // 解鎖初始能力
    UnlockAbility(ESageAbilityType::MoralGuidance);
    
    bIsInitialized = true;
    
    // 觸發初始化事件
    OnSageTraitsInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Sage traits initialized successfully at level %d"), SageLevel);
}

bool UMingSageTraits::ActivateSageAbility(ESageAbilityType AbilityType, const FString& Target)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sage traits not initialized"));
        return false;
    }

    // 檢查能力是否可用
    if (!IsAbilityAvailable(AbilityType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability not available: %s"), *UEnum::GetValueAsString(AbilityType));
        return false;
    }

    // 檢查冷卻時間
    if (AbilityCooldowns.Contains(AbilityType) && AbilityCooldowns[AbilityType] > 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability on cooldown: %s"), *UEnum::GetValueAsString(AbilityType));
        return false;
    }

    FSageAbility Ability = SageAbilitiesMap[AbilityType];
    
    // 檢查能力條件
    if (!CheckAbilityConditions(Ability))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability conditions not met: %s"), *Ability.AbilityName);
        return false;
    }

    // 計算效果
    float Effect = CalculateAbilityEffect(AbilityType);
    
    // 應用能力效果
    switch (AbilityType)
    {
        case ESageAbilityType::MoralGuidance:
            ApplyMoralGuidance(Target, Effect);
            break;
        case ESageAbilityType::WisdomBoost:
            ApplyWisdomBoost(Effect);
            break;
        case ESageAbilityType::UnityCommand:
            ApplyUnityCommand(Effect);
            break;
        case ESageAbilityType::RighteousJudgment:
            ExecuteRighteousJudgment(Target, Effect);
            break;
        case ESageAbilityType::DivineBlessing:
            BestowDivineBlessing(Target, Effect);
            break;
        case ESageAbilityType::Enlightenment:
            ApplyEnlightenment(Effect);
            break;
        default:
            break;
    }

    // 設置冷卻時間
    AbilityCooldowns.Add(AbilityType, Ability.CooldownTime);
    
    // 增加經驗
    CurrentXP += Ability.PowerCost * 0.1f;
    
    // 檢查升級
    if (CurrentXP >= CalculateXPRequirement())
    {
        LevelUpSage();
    }
    
    // 觸發能力激活事件
    OnSageAbilityActivated.Broadcast(AbilityType, Effect);
    
    UE_LOG(LogTemp, Log, TEXT("Activated sage ability: %s on target: %s with effect: %.2f"), 
           *Ability.AbilityName, *Target, Effect);
    
    return true;
}

TArray<FSageAbility> UMingSageTraits::GetAvailableAbilities() const
{
    TArray<FSageAbility> AvailableAbilities;
    
    for (const auto& AbilityPair : SageAbilitiesMap)
    {
        const FSageAbility& Ability = AbilityPair.Value;
        if (IsAbilityAvailable(Ability.AbilityType) && CheckAbilityConditions(Ability))
        {
            AvailableAbilities.Add(Ability);
        }
    }
    
    return AvailableAbilities;
}

bool UMingSageTraits::ApplyMoralGuidance(const FString& Target, float GuidanceStrength)
{
    // 增加道德純度
    IncreaseMoralPurity(GuidanceStrength * 0.1f);
    
    // 感召追隨者
    int32 NewFollowers = FMath::RoundToInt(GuidanceStrength * 0.5f);
    CurrentSageStatus.FollowerCount = FMath::Min(CurrentSageStatus.FollowerCount + NewFollowers, MaxFollowers);
    
    OnFollowerInspired.Broadcast(CurrentSageStatus.FollowerCount, TEXT("Moral Guidance"));
    
    UE_LOG(LogTemp, Log, TEXT("Applied moral guidance to %s, gained %d followers"), 
           *Target, NewFollowers);
    
    return true;
}

bool UMingSageTraits::ApplyWisdomBoost(float BoostAmount)
{
    float OldWisdom = CurrentSageStatus.WisdomLevel;
    CurrentSageStatus.WisdomLevel = FMath::Clamp(
        CurrentSageStatus.WisdomLevel + BoostAmount * WisdomGrowthRate, 
        0.0f, MaxWisdomLevel
    );
    
    OnWisdomGained.Broadcast(CurrentSageStatus.WisdomLevel - OldWisdom);
    
    UE_LOG(LogTemp, Log, TEXT("Wisdom boosted by %.2f (from %.2f to %.2f)"), 
           BoostAmount * WisdomGrowthRate, OldWisdom, CurrentSageStatus.WisdomLevel);
    
    return true;
}

bool UMingSageTraits::ApplyUnityCommand(float CommandStrength)
{
    // 提升追隨者士氣和忠誠度
    CurrentSageStatus.RighteousPower = FMath::Clamp(
        CurrentSageStatus.RighteousPower + CommandStrength * 0.2f, 
        0.0f, 100.0f
    );
    
    UE_LOG(LogTemp, Log, TEXT("Applied unity command with strength %.2f"), CommandStrength);
    
    return true;
}

bool UMingSageTraits::ExecuteRighteousJudgment(const FString& Target, float JudgmentPower)
{
    // 執行正義審判，對邪惡目標造成巨大影響
    float Effectiveness = CalculateSageEffectiveness();
    float ActualPower = JudgmentPower * Effectiveness;
    
    // 增加道德純度
    IncreaseMoralPurity(JudgmentPower * 0.05f);
    
    UE_LOG(LogTemp, Log, TEXT("Executed righteous judgment on %s with power %.2f"), 
           *Target, ActualPower);
    
    return true;
}

bool UMingSageTraits::BestowDivineBlessing(const FString& Target, float BlessingPower)
{
    // 施加神聖祝福
    float Effectiveness = CalculateSageEffectiveness();
    float ActualBlessing = BlessingPower * Effectiveness;
    
    // 增加神聖連接
    CurrentSageStatus.DivineConnection = FMath::Clamp(
        CurrentSageStatus.DivineConnection + BlessingPower * 0.1f, 
        0.0f, 100.0f
    );
    
    UE_LOG(LogTemp, Log, TEXT("Bestowed divine blessing on %s with power %.2f"), 
           *Target, ActualBlessing);
    
    return true;
}

bool UMingSageTraits::ApplyEnlightenment(float EnlightenmentAmount)
{
    // 應用啟迪之光
    CurrentSageStatus.EnlightenmentProgress += EnlightenmentAmount;
    
    // 檢查是否達到完全啟迪
    if (CurrentSageStatus.EnlightenmentProgress >= 100.0f)
    {
        CurrentSageStatus.EnlightenmentProgress = 100.0f;
        OnEnlightenmentReached(100.0f);
        
        // 獲得大量智慧
        ApplyWisdomBoost(20.0f);
    }
    else
    {
        OnEnlightenmentReached(CurrentSageStatus.EnlightenmentProgress);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Enlightenment progress: %.2f%%"), CurrentSageStatus.EnlightenmentProgress);
    
    return true;
}

bool UMingSageTraits::InspireFollowers(const FString& Message, float InspirationPower)
{
    // 感召追隨者
    int32 NewFollowers = FMath::RoundToInt(InspirationPower * CalculateSageEffectiveness());
    CurrentSageStatus.FollowerCount = FMath::Min(CurrentSageStatus.FollowerCount + NewFollowers, MaxFollowers);
    
    // 增加智慧
    ApplyWisdomBoost(InspirationPower * 0.05f);
    
    OnFollowerInspired.Broadcast(CurrentSageStatus.FollowerCount, Message);
    
    UE_LOG(LogTemp, Log, TEXT("Inspired %d followers with message: %s"), 
           NewFollowers, *Message);
    
    return true;
}

void UMingSageTraits::IncreaseMoralPurity(float Amount)
{
    float OldPurity = CurrentSageStatus.MoralPurity;
    CurrentSageStatus.MoralPurity = FMath::Clamp(
        CurrentSageStatus.MoralPurity + Amount * MoralRestoreRate, 
        0.0f, MaxMoralPurity
    );
    
    OnMoralPurityChanged.Broadcast(CurrentSageStatus.MoralPurity);
    
    UE_LOG(LogTemp, Log, TEXT("Moral purity increased from %.2f to %.2f"), 
           OldPurity, CurrentSageStatus.MoralPurity);
}

float UMingSageTraits::CalculateSageEffectiveness() const
{
    float MoralBonus = CurrentSageStatus.MoralPurity / 100.0f * 0.3f;
    float WisdomBonus = CurrentSageStatus.WisdomLevel / 100.0f * 0.25f;
    float DivineBonus = CurrentSageStatus.DivineConnection / 100.0f * 0.2f;
    float RighteousBonus = CurrentSageStatus.RighteousPower / 100.0f * 0.15f;
    float EnlightenmentBonus = CurrentSageStatus.EnlightenmentProgress / 100.0f * 0.1f;
    
    float BaseEffectiveness = 1.0f + MoralBonus + WisdomBonus + DivineBonus + RighteousBonus + EnlightenmentBonus;
    
    // 應用修飾符
    for (const auto& Modifier : CurrentSageStatus.SageModifiers)
    {
        BaseEffectiveness *= Modifier.Value;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.5f, 3.0f);
}

bool UMingSageTraits::IsAbilityAvailable(ESageAbilityType AbilityType) const
{
    // 檢查是否已解鎖
    if (!CurrentSageStatus.UnlockedAbilities.Contains(AbilityType))
    {
        return false;
    }
    
    // 檢查是否在冷卻中
    if (AbilityCooldowns.Contains(AbilityType) && AbilityCooldowns[AbilityType] > 0.0f)
    {
        return false;
    }
    
    return true;
}

void UMingSageTraits::ApplySageModifier(const FString& ModifierName, float ModifierValue)
{
    CurrentSageStatus.SageModifiers.Add(ModifierName, ModifierValue);
    UpdateSageStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Applied sage modifier %s with value %.2f"), 
           *ModifierName, ModifierValue);
}

void UMingSageTraits::RemoveSageModifier(const FString& ModifierName)
{
    if (CurrentSageStatus.SageModifiers.Remove(ModifierName) > 0)
    {
        UpdateSageStatus();
        UE_LOG(LogTemp, Log, TEXT("Removed sage modifier %s"), *ModifierName);
    }
}

int32 UMingSageTraits::GetSageLevel() const
{
    return SageLevel;
}

void UMingSageTraits::LevelUpSage()
{
    SageLevel++;
    CurrentXP = 0.0f;
    
    // 提升基礎屬性
    CurrentSageStatus.MoralPurity = FMath::Clamp(CurrentSageStatus.MoralPurity + 5.0f, 0.0f, MaxMoralPurity);
    CurrentSageStatus.WisdomLevel = FMath::Clamp(CurrentSageStatus.WisdomLevel + 3.0f, 0.0f, MaxWisdomLevel);
    CurrentSageStatus.DivineConnection = FMath::Clamp(CurrentSageStatus.DivineConnection + 2.0f, 0.0f, 100.0f);
    CurrentSageStatus.RighteousPower = FMath::Clamp(CurrentSageStatus.RighteousPower + 4.0f, 0.0f, 100.0f);
    
    // 解鎖新能力
    switch (SageLevel)
    {
        case 2:
            UnlockAbility(ESageAbilityType::WisdomBoost);
            break;
        case 3:
            UnlockAbility(ESageAbilityType::UnityCommand);
            break;
        case 5:
            UnlockAbility(ESageAbilityType::RighteousJudgment);
            break;
        case 7:
            UnlockAbility(ESageAbilityType::DivineBlessing);
            break;
        case 10:
            UnlockAbility(ESageAbilityType::Enlightenment);
            break;
        default:
            break;
    }
    
    // 觸發升級事件
    OnSageLevelUp(SageLevel);
    
    UE_LOG(LogTemp, Log, TEXT("Sage leveled up to %d"), SageLevel);
}

FString UMingSageTraits::GetSageTitle() const
{
    switch (SageLevel)
    {
        case 1:
            return TEXT("見習聖者");
        case 2:
            return TEXT("初階聖者");
        case 3:
        case 4:
            return TEXT("中階聖者");
        case 5:
        case 6:
            return TEXT("高階聖者");
        case 7:
        case 8:
        case 9:
            return TEXT("大聖者");
        case 10:
            return TEXT("聖者導師");
        default:
            if (SageLevel > 10)
            {
                return FString::Printf(TEXT("傳奇聖者 LV%d"), SageLevel);
            }
            return TEXT("無名聖者");
    }
}

void UMingSageTraits::InitializeSageAbilities()
{
    // 道德引導
    FSageAbility MoralGuidance;
    MoralGuidance.AbilityName = TEXT("道德引導");
    MoralGuidance.AbilityType = ESageAbilityType::MoralGuidance;
    MoralGuidance.PowerCost = 15.0f;
    MoralGuidance.EffectStrength = 25.0f;
    MoralGuidance.CooldownTime = 30.0f;
    MoralGuidance.RequiredLevel = 1;
    MoralGuidance.RequiredConditions = {TEXT("MinMoralPurity:60")};
    SageAbilitiesMap.Add(ESageAbilityType::MoralGuidance, MoralGuidance);

    // 智慧提升
    FSageAbility WisdomBoost;
    WisdomBoost.AbilityName = TEXT("智慧提升");
    WisdomBoost.AbilityType = ESageAbilityType::WisdomBoost;
    WisdomBoost.PowerCost = 20.0f;
    WisdomBoost.EffectStrength = 30.0f;
    WisdomBoost.CooldownTime = 45.0f;
    WisdomBoost.RequiredLevel = 2;
    WisdomBoost.RequiredConditions = {TEXT("MinWisdom:50")};
    SageAbilitiesMap.Add(ESageAbilityType::WisdomBoost, WisdomBoost);

    // 團結指揮
    FSageAbility UnityCommand;
    UnityCommand.AbilityName = TEXT("團結指揮");
    UnityCommand.AbilityType = ESageAbilityType::UnityCommand;
    UnityCommand.PowerCost = 25.0f;
    UnityCommand.EffectStrength = 35.0f;
    UnityCommand.CooldownTime = 60.0f;
    UnityCommand.RequiredLevel = 3;
    UnityCommand.RequiredConditions = {TEXT("MinFollowers:20")};
    SageAbilitiesMap.Add(ESageAbilityType::UnityCommand, UnityCommand);

    // 正義審判
    FSageAbility RighteousJudgment;
    RighteousJudgment.AbilityName = TEXT("正義審判");
    RighteousJudgment.AbilityType = ESageAbilityType::RighteousJudgment;
    RighteousJudgment.PowerCost = 40.0f;
    RighteousJudgment.EffectStrength = 50.0f;
    RighteousJudgment.CooldownTime = 90.0f;
    RighteousJudgment.RequiredLevel = 5;
    RighteousJudgment.RequiredConditions = {TEXT("MinMoralPurity:75"), TEXT("MinRighteousPower:60")};
    SageAbilitiesMap.Add(ESageAbilityType::RighteousJudgment, RighteousJudgment);

    // 神聖祝福
    FSageAbility DivineBlessing;
    DivineBlessing.AbilityName = TEXT("神聖祝福");
    DivineBlessing.AbilityType = ESageAbilityType::DivineBlessing;
    DivineBlessing.PowerCost = 35.0f;
    DivineBlessing.EffectStrength = 45.0f;
    DivineBlessing.CooldownTime = 75.0f;
    DivineBlessing.RequiredLevel = 7;
    DivineBlessing.RequiredConditions = {TEXT("MinDivineConnection:70")};
    SageAbilitiesMap.Add(ESageAbilityType::DivineBlessing, DivineBlessing);

    // 啟迪之光
    FSageAbility Enlightenment;
    Enlightenment.AbilityName = TEXT("啟迪之光");
    Enlightenment.AbilityType = ESageAbilityType::Enlightenment;
    Enlightenment.PowerCost = 50.0f;
    Enlightenment.EffectStrength = 60.0f;
    Enlightenment.CooldownTime = 120.0f;
    Enlightenment.RequiredLevel = 10;
    Enlightenment.RequiredConditions = {TEXT("MinWisdom:80"), TEXT("MinEnlightenmentProgress:50")};
    SageAbilitiesMap.Add(ESageAbilityType::Enlightenment, Enlightenment);
}

float UMingSageTraits::CalculateAbilityEffect(ESageAbilityType AbilityType) const
{
    float BaseEffect = 1.0f;
    
    switch (AbilityType)
    {
        case ESageAbilityType::MoralGuidance:
            BaseEffect = CurrentSageStatus.MoralPurity / 100.0f * 30.0f;
            break;
        case ESageAbilityType::WisdomBoost:
            BaseEffect = CurrentSageStatus.WisdomLevel / 100.0f * 25.0f;
            break;
        case ESageAbilityType::UnityCommand:
            BaseEffect = (CurrentSageStatus.FollowerCount / 100.0f) * 20.0f + CurrentSageStatus.RighteousPower / 100.0f * 15.0f;
            break;
        case ESageAbilityType::RighteousJudgment:
            BaseEffect = CurrentSageStatus.MoralPurity / 100.0f * 25.0f + CurrentSageStatus.RighteousPower / 100.0f * 25.0f;
            break;
        case ESageAbilityType::DivineBlessing:
            BaseEffect = CurrentSageStatus.DivineConnection / 100.0f * 40.0f;
            break;
        case ESageAbilityType::Enlightenment:
            BaseEffect = CurrentSageStatus.EnlightenmentProgress / 100.0f * 50.0f + CurrentSageStatus.WisdomLevel / 100.0f * 10.0f;
            break;
        default:
            break;
    }
    
    return BaseEffect * CalculateSageEffectiveness();
}

bool UMingSageTraits::CheckAbilityConditions(const FSageAbility& Ability) const
{
    // 檢查等級要求
    if (SageLevel < Ability.RequiredLevel)
    {
        return false;
    }
    
    // 檢查其他條件
    for (const FString& Condition : Ability.RequiredConditions)
    {
        TArray<FString> Parts;
        Condition.ParseIntoArray(Parts, TEXT(":"));
        
        if (Parts.Num() == 2)
        {
            FString ConditionType = Parts[0];
            float RequiredValue = FCString::Atof(*Parts[1]);
            
            if (ConditionType == TEXT("MinMoralPurity"))
            {
                if (CurrentSageStatus.MoralPurity < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinWisdom"))
            {
                if (CurrentSageStatus.WisdomLevel < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinDivineConnection"))
            {
                if (CurrentSageStatus.DivineConnection < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinRighteousPower"))
            {
                if (CurrentSageStatus.RighteousPower < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinFollowers"))
            {
                if (CurrentSageStatus.FollowerCount < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinEnlightenmentProgress"))
            {
                if (CurrentSageStatus.EnlightenmentProgress < RequiredValue)
                    return false;
            }
        }
    }
    
    return true;
}

void UMingSageTraits::UnlockAbility(ESageAbilityType AbilityType)
{
    if (!CurrentSageStatus.UnlockedAbilities.Contains(AbilityType))
    {
        CurrentSageStatus.UnlockedAbilities.Add(AbilityType);
        
        OnAbilityUnlocked(AbilityType);
        
        UE_LOG(LogTemp, Log, TEXT("Unlocked sage ability: %s"), *UEnum::GetValueAsString(AbilityType));
    }
}

void UMingSageTraits::UpdateSageStatus()
{
    // 應用修飾符
    for (const auto& Modifier : CurrentSageStatus.SageModifiers)
    {
        CurrentSageStatus.MoralPurity *= Modifier.Value;
        CurrentSageStatus.WisdomLevel *= Modifier.Value;
        CurrentSageStatus.DivineConnection *= Modifier.Value;
        CurrentSageStatus.RighteousPower *= Modifier.Value;
    }
    
    // 限制數值範圍
    CurrentSageStatus.MoralPurity = FMath::Clamp(CurrentSageStatus.MoralPurity, 0.0f, MaxMoralPurity);
    CurrentSageStatus.WisdomLevel = FMath::Clamp(CurrentSageStatus.WisdomLevel, 0.0f, MaxWisdomLevel);
    CurrentSageStatus.DivineConnection = FMath::Clamp(CurrentSageStatus.DivineConnection, 0.0f, 100.0f);
    CurrentSageStatus.RighteousPower = FMath::Clamp(CurrentSageStatus.RighteousPower, 0.0f, 100.0f);
}

float UMingSageTraits::CalculateXPRequirement() const
{
    // 經驗需求隨等級增加
    return 100.0f * FMath::Pow(1.5f, SageLevel - 1);
}
