#include "SageBrain/MingDemonKingTraits.h"
#include "Engine/Engine.h"

const TMap<FString, int32> UMingDemonKingTraits::DemonAbilityLevelRequirements = {
    {"魔威震懾", 1},
    {"恐懼散播", 2},
    {"黑暗統治", 4},
    {"邪惡腐化", 6},
    {"毀滅指令", 8},
    {"地獄召喚", 12},
    {"靈魂支配", 15},
    {"終末審判", 20}
};

const TMap<FString, FDemonAbilityEffect> UMingDemonKingTraits::DemonAbilityEffects = {
    {"魔威震懾", FDemonAbilityEffect{"魔威震懾", 45.0f, 15.0f, 400.0f, {"enemy", "area"}, "釋放魔王的威壓氣場，震懾所有敵人", true}},
    {"恐懼散播", FDemonAbilityEffect{"恐懼散播", 60.0f, 10.0f, 600.0f, {"enemy", "area"}, "散播恐懼瘟疫，持續降低敵軍士氣", true}},
    {"黑暗統治", FDemonAbilityEffect{"黑暗統治", 35.0f, 25.0f, 800.0f, {"all", "area"}, "建立黑暗統治領域，壓制所有反抗", false}},
    {"邪惡腐化", FDemonAbilityEffect{"邪惡腐化", 25.0f, 20.0f, 500.0f, {"enemy", "ally"}, "腐化目標，有機會轉化為魔軍", false}},
    {"毀滅指令", FDemonAbilityEffect{"毀滅指令", 80.0f, 50.0f, 150.0f, {"enemy", "structure"}, "下令毀滅目標，造成巨大破壞", false}},
    {"地獄召喚", FDemonAbilityEffect{"地獄召喚", 70.0f, 30.0f, 300.0f, {"area"}, "召喚地獄生物協助作戰", true}},
    {"靈魂支配", FDemonAbilityEffect{"靈魂支配", 90.0f, 40.0f, 200.0f, {"enemy"}, "支配敵人靈魂，暫時控制行動", true}},
    {"終末審判", FDemonAbilityEffect{"終末審判", 100.0f, 80.0f, 100.0f, {"all", "area"}, "釋放終末審判，毀滅範圍內一切", true}}
};

UMingDemonKingTraits::UMingDemonKingTraits()
{
}

FSageCharacterTraits UMingDemonKingTraits::GetDemonKingDefaultTraits()
{
    FSageCharacterTraits Traits;
    Traits.CharacterType = ESageCharacterType::DemonKing;
    Traits.MoralAuthority = 20.0f;
    Traits.StrategicThinking = 80.0f;
    Traits.MilitaryCommand = 90.0f;
    Traits.CharismaBonus = -10.0f;
    Traits.LearningSpeed = 1.0f;
    Traits.DecisionSpeed = 1.8f;
    Traits.RiskTolerance = 0.8f;
    Traits.MaxFollowers = 1000;
    Traits.SpecialAbilities = {"魔威震懾", "恐懼散播", "黑暗統治"};
    return Traits;
}

FDemonAbilityEffect UMingDemonKingTraits::CreateTerrorAuraEffect()
{
    FDemonAbilityEffect Effect;
    Effect.AbilityName = "恐懼光環";
    Effect.FearMagnitude = 55.0f;
    Effect.DamageBonus = 20.0f;
    Effect.Duration = 500.0f;
    Effect.TargetTypes = {"enemy", "area"};
    Effect.Description = "魔王散發的恐懼光環，持續對敵人造成心理壓力和傷害";
    Effect.bCausesTerror = true;
    return Effect;
}

FDemonAbilityEffect UMingDemonKingTraits::CreateDarkCommandEffect()
{
    FDemonAbilityEffect Effect;
    Effect.AbilityName = "黑暗指令";
    Effect.FearMagnitude = 40.0f;
    Effect.DamageBonus = 35.0f;
    Effect.Duration = 250.0f;
    Effect.TargetTypes = {"ally", "enemy"};
    Effect.Description = "發出黑暗指令，強化友軍戰力並威懾敵人";
    Effect.bCausesTerror = false;
    return Effect;
}

FDemonAbilityEffect UMingDemonKingTraits::CreateDestructionOrderEffect()
{
    FDemonAbilityEffect Effect;
    Effect.AbilityName = "毀滅命令";
    Effect.FearMagnitude = 85.0f;
    Effect.DamageBonus = 75.0f;
    Effect.Duration = 100.0f;
    Effect.TargetTypes = {"enemy", "structure", "area"};
    Effect.Description = "下達毀滅命令，對目標造成毀滅性打擊";
    Effect.bCausesTerror = true;
    return Effect;
}

FDominanceAura UMingDemonKingTraits::GetDemonDominanceAura()
{
    FDominanceAura Aura;
    Aura.SuppressionRadius = 600;
    Aura.EnemyMoraleDrain = 30.0f;
    Aura.AllyCompliance = 25.0f;
    Aura.RebellionSuppression = 40.0f;
    Aura.bAffectsAllUnits = true;
    return Aura;
}

FCorruptionEffect UMingDemonKingTraits::GetCorruptionEffect()
{
    FCorruptionEffect Effect;
    Effect.CorruptionStrength = 45.0f;
    Effect.ConversionChance = 0.25f;
    Effect.LoyaltyDecay = 20.0f;
    Effect.PropagationSpeed = 1.5f;
    Effect.bPermanent = false;
    return Effect;
}

TArray<FString> UMingDemonKingTraits::GetDemonKingSpecialAbilities()
{
    return {"魔威震懾", "恐懼散播", "黑暗統治", "邪惡腐化", "毀滅指令", "地獄召喚", "靈魂支配", "終末審判"};
}

bool UMingDemonKingTraits::CanUseDemonAbility(const FString& AbilityName, int32 CharacterLevel)
{
    if (const int32* RequiredLevel = DemonAbilityLevelRequirements.Find(AbilityName))
    {
        return CharacterLevel >= *RequiredLevel;
    }
    return false;
}

float UMingDemonKingTraits::CalculateDemonEffectiveness(const FSageCharacterTraits& Traits, const FString& Context)
{
    float BaseEffectiveness = 1.0f;
    
    if (Context == "fear_aura")
    {
        BaseEffectiveness = (100.0f - Traits.MoralAuthority) / 100.0f;
        BaseEffectiveness += Traits.MilitaryCommand / 200.0f;
    }
    else if (Context == "corruption")
    {
        BaseEffectiveness = Traits.StrategicThinking / 100.0f;
        BaseEffectiveness += Traits.RiskTolerance * 0.3f;
    }
    else if (Context == "destruction")
    {
        BaseEffectiveness = Traits.MilitaryCommand / 100.0f;
        BaseEffectiveness += Traits.DecisionSpeed * 0.2f;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.0f, 2.5f);
}

void UMingDemonKingTraits::ApplyDemonKingLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel)
{
    Traits.MilitaryCommand += 3.0f;
    Traits.StrategicThinking += 2.0f;
    Traits.MoralAuthority -= 1.0f;
    Traits.RiskTolerance += 0.1f;
    
    if (NewLevel % 4 == 0)
    {
        Traits.MaxFollowers += 100;
    }
    
    if (NewLevel % 8 == 0)
    {
        Traits.DecisionSpeed += 0.15f;
        Traits.CharismaBonus -= 2.0f;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Demon King level up bonus applied for level %d"), NewLevel);
}

FString UMingDemonKingTraits::GetDemonAbilityDescription(const FString& AbilityName)
{
    if (const FDemonAbilityEffect* Effect = DemonAbilityEffects.Find(AbilityName))
    {
        return Effect->Description;
    }
    return TEXT("未知魔王能力");
}

TArray<FString> UMingDemonKingTraits::GetDemonKingLevelUpRewards(int32 Level)
{
    TArray<FString> Rewards;
    
    if (Level == 1)
    {
        Rewards.Add("獲得基礎魔王能力");
        Rewards.Add("解鎖魔威震懾");
    }
    else if (Level == 2)
    {
        Rewards.Add("解鎖恐懼散播");
        Rewards.Add("恐懼效果+15%");
    }
    else if (Level == 4)
    {
        Rewards.Add("解鎖黑暗統治");
        Rewards.Add("控制範圍+20%");
    }
    else if (Level == 6)
    {
        Rewards.Add("解鎖邪惡腐化");
        Rewards.Add("腐化機率+10%");
    }
    else if (Level == 8)
    {
        Rewards.Add("解鎖毀滅指令");
        Rewards.Add("破壞力+25%");
    }
    else if (Level == 10)
    {
        Rewards.Add("魔王進化");
        Rewards.Add("軍事指揮+10，戰略思維+5");
    }
    else if (Level == 12)
    {
        Rewards.Add("解鎖地獄召喚");
        Rewards.Add("召喚物強度+30%");
    }
    else if (Level % 4 == 0)
    {
        Rewards.Add(FString::Printf(TEXT "魔王能力強化等級 %d", Level));
        Rewards.Add("追隨者上限+50");
    }
    
    return Rewards;
}

bool UMingDemonKingTraits::CanCorruptTarget(float TargetMoralAuthority, float DemonPower)
{
    float CorruptionChance = (DemonPower - TargetMoralAuthority) / 100.0f;
    CorruptionChance = FMath::Clamp(CorruptionChance, 0.0f, 0.8f);
    
    return FMath::FRand() < CorruptionChance;
}

float UMingDemonKingTraits::CalculateFearDamage(float BaseDamage, float FearLevel)
{
    float FearMultiplier = 1.0f + (FearLevel / 100.0f) * 0.5f;
    return BaseDamage * FearMultiplier;
}
