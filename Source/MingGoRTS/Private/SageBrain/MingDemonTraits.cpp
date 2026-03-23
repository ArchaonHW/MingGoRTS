#include "SageBrain/MingDemonTraits.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingDemonTraits::UMingDemonTraits()
{
    bIsInitialized = false;
    DemonLevel = 1;
    CurrentXP = 0.0f;
    MaxFearPower = 100.0f;
    MaxDarkEnergy = 100.0f;
    MaxMinions = 500;
    DarkEnergyRestoreRate = 0.8f;
    FearGrowthRate = 0.6f;
    
    InitializeDemonAbilities();
}

void UMingDemonTraits::InitializeDemonTraits()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Demon traits already initialized"));
        return;
    }

    CurrentDemonStatus = FDemonStatus();
    CurrentDemonStatus.FearPower = 70.0f;
    CurrentDemonStatus.DarkEnergy = 80.0f;
    CurrentDemonStatus.ChaosLevel = 60.0f;
    CurrentDemonStatus.DestructionPower = 75.0f;
    CurrentDemonStatus.MinionCount = 0;
    CurrentDemonStatus.TerrorRadius = 100.0f;
    
    UnlockAbility(EDemonAbilityType::FearAura);
    
    bIsInitialized = true;
    
    OnDemonTraitsInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Demon traits initialized successfully at level %d"), DemonLevel);
}

bool UMingDemonTraits::ActivateDemonAbility(EDemonAbilityType AbilityType, const FString& Target)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Demon traits not initialized"));
        return false;
    }

    if (!IsAbilityAvailable(AbilityType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability not available: %s"), *UEnum::GetValueAsString(AbilityType));
        return false;
    }

    if (AbilityCooldowns.Contains(AbilityType) && AbilityCooldowns[AbilityType] > 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability on cooldown: %s"), *UEnum::GetValueAsString(AbilityType));
        return false;
    }

    FDemonAbility Ability = DemonAbilitiesMap[AbilityType];
    
    if (!CheckAbilityConditions(Ability))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability conditions not met: %s"), *Ability.AbilityName);
        return false;
    }

    float Effect = CalculateAbilityEffect(AbilityType);
    
    switch (AbilityType)
    {
        case EDemonAbilityType::FearAura:
            ApplyFearAura(Effect, CurrentDemonStatus.TerrorRadius);
            break;
        case EDemonAbilityType::DarkCommand:
            ApplyDarkCommand(Effect);
            break;
        case EDemonAbilityType::ChaosTactics:
            ApplyChaosTactics(Effect);
            break;
        case EDemonAbilityType::Intimidation:
            ApplyIntimidation(Target, Effect);
            break;
        case EDemonAbilityType::Corruption:
            ApplyCorruption(Target, Effect);
            break;
        case EDemonAbilityType::Destruction:
            ApplyDestruction(Target, Effect);
            break;
        default:
            break;
    }

    AbilityCooldowns.Add(AbilityType, Ability.CooldownTime);
    CurrentXP += Ability.DarkEnergyCost * 0.1f;
    
    if (CurrentXP >= CalculateXPRequirement())
    {
        LevelUpDemon();
    }
    
    OnDemonAbilityActivated.Broadcast(AbilityType, Effect);
    
    UE_LOG(LogTemp, Log, TEXT("Activated demon ability: %s on target: %s with effect: %.2f"), 
           *Ability.AbilityName, *Target, Effect);
    
    return true;
}

TArray<FDemonAbility> UMingDemonTraits::GetAvailableAbilities() const
{
    TArray<FDemonAbility> AvailableAbilities;
    
    for (const auto& AbilityPair : DemonAbilitiesMap)
    {
        const FDemonAbility& Ability = AbilityPair.Value;
        if (IsAbilityAvailable(Ability.AbilityType) && CheckAbilityConditions(Ability))
        {
            AvailableAbilities.Add(Ability);
        }
    }
    
    return AvailableAbilities;
}

bool UMingDemonTraits::ApplyFearAura(float AuraStrength, float Radius)
{
    CurrentDemonStatus.FearPower = FMath::Clamp(
        CurrentDemonStatus.FearPower + AuraStrength * 0.2f, 
        0.0f, MaxFearPower
    );
    
    CurrentDemonStatus.TerrorRadius = FMath::Clamp(
        CurrentDemonStatus.TerrorRadius + Radius * 0.1f, 
        0.0f, 500.0f
    );
    
    OnFearPowerChanged.Broadcast(CurrentDemonStatus.FearPower);
    OnTerrorRadiusChanged(CurrentDemonStatus.TerrorRadius);
    
    UE_LOG(LogTemp, Log, TEXT("Applied fear aura with strength %.2f and radius %.2f"), 
           AuraStrength, Radius);
    
    return true;
}

bool UMingDemonTraits::ApplyDarkCommand(float CommandStrength)
{
    CurrentDemonStatus.DarkEnergy = FMath::Clamp(
        CurrentDemonStatus.DarkEnergy + CommandStrength * 0.3f, 
        0.0f, MaxDarkEnergy
    );
    
    OnDarkEnergyChanged.Broadcast(CurrentDemonStatus.DarkEnergy);
    
    UE_LOG(LogTemp, Log, TEXT("Applied dark command with strength %.2f"), CommandStrength);
    
    return true;
}

bool UMingDemonTraits::ApplyChaosTactics(float ChaosStrength)
{
    CurrentDemonStatus.ChaosLevel = FMath::Clamp(
        CurrentDemonStatus.ChaosLevel + ChaosStrength * 0.25f, 
        0.0f, 100.0f
    );
    
    UE_LOG(LogTemp, Log, TEXT("Applied chaos tactics with strength %.2f"), ChaosStrength);
    
    return true;
}

bool UMingDemonTraits::ApplyIntimidation(const FString& Target, float IntimidationPower)
{
    float Effectiveness = CalculateDemonEffectiveness();
    float ActualPower = IntimidationPower * Effectiveness;
    
    CurrentDemonStatus.FearPower = FMath::Clamp(
        CurrentDemonStatus.FearPower + IntimidationPower * 0.1f, 
        0.0f, MaxFearPower
    );
    
    OnFearPowerChanged.Broadcast(CurrentDemonStatus.FearPower);
    
    UE_LOG(LogTemp, Log, TEXT("Applied intimidation to %s with power %.2f"), 
           *Target, ActualPower);
    
    return true;
}

bool UMingDemonTraits::ApplyCorruption(const FString& Target, float CorruptionPower)
{
    float Effectiveness = CalculateDemonEffectiveness();
    float ActualCorruption = CorruptionPower * Effectiveness;
    
    CurrentDemonStatus.DarkEnergy = FMath::Clamp(
        CurrentDemonStatus.DarkEnergy + CorruptionPower * 0.15f, 
        0.0f, MaxDarkEnergy
    );
    
    OnDarkEnergyChanged.Broadcast(CurrentDemonStatus.DarkEnergy);
    
    UE_LOG(LogTemp, Log, TEXT("Applied corruption to %s with power %.2f"), 
           *Target, ActualCorruption);
    
    return true;
}

bool UMingDemonTraits::ApplyDestruction(const FString& Target, float DestructionPower)
{
    float Effectiveness = CalculateDemonEffectiveness();
    float ActualDestruction = DestructionPower * Effectiveness;
    
    CurrentDemonStatus.DestructionPower = FMath::Clamp(
        CurrentDemonStatus.DestructionPower + DestructionPower * 0.2f, 
        0.0f, 100.0f
    );
    
    UE_LOG(LogTemp, Log, TEXT("Applied destruction to %s with power %.2f"), 
           *Target, ActualDestruction);
    
    return true;
}

bool UMingDemonTraits::SummonMinions(int32 Count, const FString& MinionType)
{
    int32 ActualCount = FMath::Min(Count, MaxMinions - CurrentDemonStatus.MinionCount);
    CurrentDemonStatus.MinionCount += ActualCount;
    
    OnMinionSummoned.Broadcast(CurrentDemonStatus.MinionCount, MinionType);
    
    UE_LOG(LogTemp, Log, TEXT("Summoned %d minions of type %s"), 
           ActualCount, *MinionType);
    
    return ActualCount > 0;
}

void UMingDemonTraits::IncreaseFearPower(float Amount)
{
    float OldPower = CurrentDemonStatus.FearPower;
    CurrentDemonStatus.FearPower = FMath::Clamp(
        CurrentDemonStatus.FearPower + Amount * FearGrowthRate, 
        0.0f, MaxFearPower
    );
    
    OnFearPowerChanged.Broadcast(CurrentDemonStatus.FearPower);
    
    UE_LOG(LogTemp, Log, TEXT("Fear power increased from %.2f to %.2f"), 
           OldPower, CurrentDemonStatus.FearPower);
}

float UMingDemonTraits::CalculateDemonEffectiveness() const
{
    float FearBonus = CurrentDemonStatus.FearPower / 100.0f * 0.3f;
    float DarkBonus = CurrentDemonStatus.DarkEnergy / 100.0f * 0.25f;
    float ChaosBonus = CurrentDemonStatus.ChaosLevel / 100.0f * 0.2f;
    float DestructionBonus = CurrentDemonStatus.DestructionPower / 100.0f * 0.15f;
    float MinionBonus = (CurrentDemonStatus.MinionCount / 100.0f) * 0.1f;
    
    float BaseEffectiveness = 1.0f + FearBonus + DarkBonus + ChaosBonus + DestructionBonus + MinionBonus;
    
    for (const auto& Modifier : CurrentDemonStatus.DemonModifiers)
    {
        BaseEffectiveness *= Modifier.Value;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.5f, 3.0f);
}

bool UMingDemonTraits::IsAbilityAvailable(EDemonAbilityType AbilityType) const
{
    if (!CurrentDemonStatus.UnlockedAbilities.Contains(AbilityType))
    {
        return false;
    }
    
    if (AbilityCooldowns.Contains(AbilityType) && AbilityCooldowns[AbilityType] > 0.0f)
    {
        return false;
    }
    
    return true;
}

void UMingDemonTraits::ApplyDemonModifier(const FString& ModifierName, float ModifierValue)
{
    CurrentDemonStatus.DemonModifiers.Add(ModifierName, ModifierValue);
    UpdateDemonStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Applied demon modifier %s with value %.2f"), 
           *ModifierName, ModifierValue);
}

void UMingDemonTraits::RemoveDemonModifier(const FString& ModifierName)
{
    if (CurrentDemonStatus.DemonModifiers.Remove(ModifierName) > 0)
    {
        UpdateDemonStatus();
        UE_LOG(LogTemp, Log, TEXT("Removed demon modifier %s"), *ModifierName);
    }
}

int32 UMingDemonTraits::GetDemonLevel() const
{
    return DemonLevel;
}

void UMingDemonTraits::LevelUpDemon()
{
    DemonLevel++;
    CurrentXP = 0.0f;
    
    CurrentDemonStatus.FearPower = FMath::Clamp(CurrentDemonStatus.FearPower + 5.0f, 0.0f, MaxFearPower);
    CurrentDemonStatus.DarkEnergy = FMath::Clamp(CurrentDemonStatus.DarkEnergy + 8.0f, 0.0f, MaxDarkEnergy);
    CurrentDemonStatus.ChaosLevel = FMath::Clamp(CurrentDemonStatus.ChaosLevel + 4.0f, 0.0f, 100.0f);
    CurrentDemonStatus.DestructionPower = FMath::Clamp(CurrentDemonStatus.DestructionPower + 6.0f, 0.0f, 100.0f);
    
    switch (DemonLevel)
    {
        case 2:
            UnlockAbility(EDemonAbilityType::DarkCommand);
            break;
        case 3:
            UnlockAbility(EDemonAbilityType::ChaosTactics);
            break;
        case 5:
            UnlockAbility(EDemonAbilityType::Intimidation);
            break;
        case 7:
            UnlockAbility(EDemonAbilityType::Corruption);
            break;
        case 10:
            UnlockAbility(EDemonAbilityType::Destruction);
            break;
        default:
            break;
    }
    
    OnDemonLevelUp(DemonLevel);
    
    UE_LOG(LogTemp, Log, TEXT("Demon leveled up to %d"), DemonLevel);
}

FString UMingDemonTraits::GetDemonTitle() const
{
    switch (DemonLevel)
    {
        case 1:
            return TEXT("見習魔王");
        case 2:
            return TEXT("初階魔王");
        case 3:
        case 4:
            return TEXT("中階魔王");
        case 5:
        case 6:
            return TEXT("高階魔王");
        case 7:
        case 8:
        case 9:
            return TEXT("大魔王");
        case 10:
            return TEXT("魔王君主");
        default:
            if (DemonLevel > 10)
            {
                return FString::Printf(TEXT("傳奇魔王 LV%d"), DemonLevel);
            }
            return TEXT("無名魔王");
    }
}

void UMingDemonTraits::InitializeDemonAbilities()
{
    FDemonAbility FearAura;
    FearAura.AbilityName = TEXT("恐懼光環");
    FearAura.AbilityType = EDemonAbilityType::FearAura;
    FearAura.DarkEnergyCost = 15.0f;
    FearAura.EffectStrength = 30.0f;
    FearAura.CooldownTime = 30.0f;
    FearAura.RequiredLevel = 1;
    FearAura.RequiredConditions = {TEXT("MinFearPower:50")};
    DemonAbilitiesMap.Add(EDemonAbilityType::FearAura, FearAura);

    FDemonAbility DarkCommand;
    DarkCommand.AbilityName = TEXT("黑暗統治");
    DarkCommand.AbilityType = EDemonAbilityType::DarkCommand;
    DarkCommand.DarkEnergyCost = 25.0f;
    DarkCommand.EffectStrength = 35.0f;
    DarkCommand.CooldownTime = 45.0f;
    DarkCommand.RequiredLevel = 2;
    DarkCommand.RequiredConditions = {TEXT("MinDarkEnergy:60")};
    DemonAbilitiesMap.Add(EDemonAbilityType::DarkCommand, DarkCommand);

    FDemonAbility ChaosTactics;
    ChaosTactics.AbilityName = TEXT("混沌戰術");
    ChaosTactics.AbilityType = EDemonAbilityType::ChaosTactics;
    ChaosTactics.DarkEnergyCost = 20.0f;
    ChaosTactics.EffectStrength = 40.0f;
    ChaosTactics.CooldownTime = 50.0f;
    ChaosTactics.RequiredLevel = 3;
    ChaosTactics.RequiredConditions = {TEXT("MinChaosLevel:40")};
    DemonAbilitiesMap.Add(EDemonAbilityType::ChaosTactics, ChaosTactics);

    FDemonAbility Intimidation;
    Intimidation.AbilityName = TEXT("威懾");
    Intimidation.AbilityType = EDemonAbilityType::Intimidation;
    Intimidation.DarkEnergyCost = 30.0f;
    Intimidation.EffectStrength = 45.0f;
    Intimidation.CooldownTime = 60.0f;
    Intimidation.RequiredLevel = 5;
    Intimidation.RequiredConditions = {TEXT("MinFearPower:65"), TEXT("MinDarkEnergy:70")};
    DemonAbilitiesMap.Add(EDemonAbilityType::Intimidation, Intimidation);

    FDemonAbility Corruption;
    Corruption.AbilityName = TEXT("腐化");
    Corruption.AbilityType = EDemonAbilityType::Corruption;
    Corruption.DarkEnergyCost = 35.0f;
    Corruption.EffectStrength = 50.0f;
    Corruption.CooldownTime = 75.0f;
    Corruption.RequiredLevel = 7;
    Corruption.RequiredConditions = {TEXT("MinDarkEnergy:80")};
    DemonAbilitiesMap.Add(EDemonAbilityType::Corruption, Corruption);

    FDemonAbility Destruction;
    Destruction.AbilityName = TEXT("毀滅之力");
    Destruction.AbilityType = EDemonAbilityType::Destruction;
    Destruction.DarkEnergyCost = 50.0f;
    Destruction.EffectStrength = 60.0f;
    Destruction.CooldownTime = 120.0f;
    Destruction.RequiredLevel = 10;
    Destruction.RequiredConditions = {TEXT("MinDestructionPower:70"), TEXT("MinChaosLevel:60")};
    DemonAbilitiesMap.Add(EDemonAbilityType::Destruction, Destruction);
}

float UMingDemonTraits::CalculateAbilityEffect(EDemonAbilityType AbilityType) const
{
    float BaseEffect = 1.0f;
    
    switch (AbilityType)
    {
        case EDemonAbilityType::FearAura:
            BaseEffect = CurrentDemonStatus.FearPower / 100.0f * 35.0f;
            break;
        case EDemonAbilityType::DarkCommand:
            BaseEffect = CurrentDemonStatus.DarkEnergy / 100.0f * 30.0f;
            break;
        case EDemonAbilityType::ChaosTactics:
            BaseEffect = CurrentDemonStatus.ChaosLevel / 100.0f * 40.0f;
            break;
        case EDemonAbilityType::Intimidation:
            BaseEffect = (CurrentDemonStatus.FearPower / 100.0f * 25.0f) + (CurrentDemonStatus.DarkEnergy / 100.0f * 20.0f);
            break;
        case EDemonAbilityType::Corruption:
            BaseEffect = CurrentDemonStatus.DarkEnergy / 100.0f * 45.0f;
            break;
        case EDemonAbilityType::Destruction:
            BaseEffect = (CurrentDemonStatus.DestructionPower / 100.0f * 50.0f) + (CurrentDemonStatus.ChaosLevel / 100.0f * 10.0f);
            break;
        default:
            break;
    }
    
    return BaseEffect * CalculateDemonEffectiveness();
}

bool UMingDemonTraits::CheckAbilityConditions(const FDemonAbility& Ability) const
{
    if (DemonLevel < Ability.RequiredLevel)
    {
        return false;
    }
    
    for (const FString& Condition : Ability.RequiredConditions)
    {
        TArray<FString> Parts;
        Condition.ParseIntoArray(Parts, TEXT(":"));
        
        if (Parts.Num() == 2)
        {
            FString ConditionType = Parts[0];
            float RequiredValue = FCString::Atof(*Parts[1]);
            
            if (ConditionType == TEXT("MinFearPower"))
            {
                if (CurrentDemonStatus.FearPower < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinDarkEnergy"))
            {
                if (CurrentDemonStatus.DarkEnergy < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinChaosLevel"))
            {
                if (CurrentDemonStatus.ChaosLevel < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinDestructionPower"))
            {
                if (CurrentDemonStatus.DestructionPower < RequiredValue)
                    return false;
            }
        }
    }
    
    return true;
}

void UMingDemonTraits::UnlockAbility(EDemonAbilityType AbilityType)
{
    if (!CurrentDemonStatus.UnlockedAbilities.Contains(AbilityType))
    {
        CurrentDemonStatus.UnlockedAbilities.Add(AbilityType);
        
        OnAbilityUnlocked(AbilityType);
        
        UE_LOG(LogTemp, Log, TEXT("Unlocked demon ability: %s"), *UEnum::GetValueAsString(AbilityType));
    }
}

void UMingDemonTraits::UpdateDemonStatus()
{
    for (const auto& Modifier : CurrentDemonStatus.DemonModifiers)
    {
        CurrentDemonStatus.FearPower *= Modifier.Value;
        CurrentDemonStatus.DarkEnergy *= Modifier.Value;
        CurrentDemonStatus.ChaosLevel *= Modifier.Value;
        CurrentDemonStatus.DestructionPower *= Modifier.Value;
    }
    
    CurrentDemonStatus.FearPower = FMath::Clamp(CurrentDemonStatus.FearPower, 0.0f, MaxFearPower);
    CurrentDemonStatus.DarkEnergy = FMath::Clamp(CurrentDemonStatus.DarkEnergy, 0.0f, MaxDarkEnergy);
    CurrentDemonStatus.ChaosLevel = FMath::Clamp(CurrentDemonStatus.ChaosLevel, 0.0f, 100.0f);
    CurrentDemonStatus.DestructionPower = FMath::Clamp(CurrentDemonStatus.DestructionPower, 0.0f, 100.0f);
}

float UMingDemonTraits::CalculateXPRequirement() const
{
    return 100.0f * FMath::Pow(1.5f, DemonLevel - 1);
}
