#include "SageBrain/MingFalseSageTraits.h"
#include "Engine/Engine.h"

const TMap<FString, int32> UMingFalseSageTraits::FalseSageAbilityLevelRequirements = {
    {"偽善欺騙", 1},
    {"陰謀策劃", 3},
    {"暗中操控", 5},
    {"背叛突襲", 7},
    {"虛假宣傳", 9},
    {"離間計", 12},
    {"偽裝術", 15},
    {"背後捅刀", 18}
};

const TMap<FString, FDeceptionAbility> UMingFalseSageTraits::FalseSageAbilityEffects = {
    {"偽善欺騙", FDeceptionAbility{"偽善欺騙", 65.0f, 45.0f, 400.0f, {"all", "civilian"}, "偽裝成聖者欺騙目標，獲得虛假信任", false}},
    {"陰謀策劃", FDeceptionAbility{"陰謀策劃", 75.0f, 60.0f, 600.0f, {"ally", "enemy"}, "策劃複雜陰謀，暗中操控局勢", true}},
    {"暗中操控", FDeceptionAbility{"暗中操控", 55.0f, 70.0f, 500.0f, {"ally", "enemy"}, "從暗中操控目標行動", true}},
    {"背叛突襲", FDeceptionAbility{"背叛突襲", 85.0f, 30.0f, 200.0f, {"ally", "enemy"}, "背叛盟友發動突襲，造成巨大傷害", false}},
    {"虛假宣傳", FDeceptionAbility{"虛假宣傳", 70.0f, 50.0f, 700.0f, {"civilian", "area"}, "散播虛假信息，影響輿論", false}},
    {"離間計", FDeceptionAbility{"離間計", 80.0f, 65.0f, 350.0f, {"enemy", "ally"}, "挑撥離間，破壞敵人內部關係", true}},
    {"偽裝術", FDeceptionAbility{"偽裝術", 90.0f, 80.0f, 300.0f, {"self"}, "完美偽裝成其他人，難以識破", true}},
    {"背後捅刀", FDeceptionAbility{"背後捅刀", 95.0f, 25.0f, 150.0f, {"ally", "enemy"}, "從背後攻擊，造成致命傷害", false}}
};

UMingFalseSageTraits::UMingFalseSageTraits()
{
}

FSageCharacterTraits UMingFalseSageTraits::GetFalseSageDefaultTraits()
{
    FSageCharacterTraits Traits;
    Traits.CharacterType = ESageCharacterType::FalseSage;
    Traits.MoralAuthority = 40.0f;
    Traits.StrategicThinking = 85.0f;
    Traits.MilitaryCommand = 70.0f;
    Traits.CharismaBonus = 15.0f;
    Traits.LearningSpeed = 1.3f;
    Traits.DecisionSpeed = 1.5f;
    Traits.RiskTolerance = 0.6f;
    Traits.MaxFollowers = 300;
    Traits.SpecialAbilities = {"偽善欺騙", "陰謀策劃", "暗中操控"};
    return Traits;
}

FDeceptionAbility UMingFalseSageTraits::CreateFalsePietyAbility()
{
    FDeceptionAbility Ability;
    Ability.AbilityName = "虛假虔誠";
    Ability.DeceptionPower = 70.0f;
    Ability.DetectionResistance = 50.0f;
    Ability.Duration = 450.0f;
    Ability.TargetTypes = {"all", "civilian"};
    Ability.Description = "偽裝出虔誠的外表，獲得他人的信任和尊敬";
    Ability.bCausesParanoia = false;
    return Ability;
}

FDeceptionAbility UMingFalseSageTraits::CreateConspiracyAbility()
{
    FDeceptionAbility Ability;
    Ability.AbilityName = "秘密陰謀";
    Ability.DeceptionPower = 80.0f;
    Ability.DetectionResistance = 75.0f;
    Ability.Duration = 800.0f;
    Ability.TargetTypes = {"ally", "enemy", "civilian"};
    Ability.Description = "策劃秘密陰謀，暗中操控整個局勢";
    Ability.bCausesParanoia = true;
    return Ability;
}

FDeceptionAbility UMingFalseSageTraits::CreateBetrayalAbility()
{
    FDeceptionAbility Ability;
    Ability.AbilityName = "致命背叛";
    Ability.DeceptionPower = 90.0f;
    Ability.DetectionResistance = 20.0f;
    Ability.Duration = 100.0f;
    Ability.TargetTypes = {"ally", "enemy"};
    Ability.Description = "發動致命背叛，造成巨大傷害和混亂";
    Ability.bCausesParanoia = false;
    return Ability;
}

FConspiracyEffect UMingFalseSageTraits::GetConspiracyEffect()
{
    FConspiracyEffect Effect;
    Effect.ManipulationStrength = 65.0f;
    Effect.TrustErosion = 40.0f;
    Effect.SuspicionSpread = 35.0f;
    Effect.HiddenInfluence = 50.0f;
    Effect.bUntraceable = true;
    return Effect;
}

FBackstabBonus UMingFalseSageTraits::GetBackstabBonus()
{
    FBackstabBonus Bonus;
    Bonus.SurpriseDamage = 75.0f;
    Bonus.CriticalChance = 0.4f;
    Bonus.BetrayalEffect = 60.0f;
    Bonus.ReputationDamage = 50.0f;
    Bonus.bHiddenAttack = true;
    return Bonus;
}

TArray<FString> UMingFalseSageTraits::GetFalseSageSpecialAbilities()
{
    return {"偽善欺騙", "陰謀策劃", "暗中操控", "背叛突襲", "虛假宣傳", "離間計", "偽裝術", "背後捅刀"};
}

bool UMingFalseSageTraits::CanUseFalseSageAbility(const FString& AbilityName, int32 CharacterLevel)
{
    if (const int32* RequiredLevel = FalseSageAbilityLevelRequirements.Find(AbilityName))
    {
        return CharacterLevel >= *RequiredLevel;
    }
    return false;
}

float UMingFalseSageTraits::CalculateFalseSageEffectiveness(const FSageCharacterTraits& Traits, const FString& Context)
{
    float BaseEffectiveness = 1.0f;
    
    if (Context == "deception")
    {
        BaseEffectiveness = Traits.StrategicThinking / 100.0f;
        BaseEffectiveness += (100.0f - Traits.MoralAuthority) / 200.0f;
    }
    else if (Context == "manipulation")
    {
        BaseEffectiveness = Traits.StrategicThinking / 100.0f;
        BaseEffectiveness += Traits.CharismaBonus / 100.0f;
    }
    else if (Context == "betrayal")
    {
        BaseEffectiveness = Traits.MilitaryCommand / 100.0f;
        BaseEffectiveness += Traits.RiskTolerance * 0.4f;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.0f, 2.0f);
}

void UMingFalseSageTraits::ApplyFalseSageLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel)
{
    Traits.StrategicThinking += 2.5f;
    Traits.MilitaryCommand += 1.5f;
    Traits.DecisionSpeed += 0.1f;
    Traits.RiskTolerance += 0.05f;
    
    if (NewLevel % 3 == 0)
    {
        Traits.CharismaBonus += 1.0f;
    }
    
    if (NewLevel % 6 == 0)
    {
        Traits.LearningSpeed += 0.15f;
        Traits.MaxFollowers += 25;
    }
    
    UE_LOG(LogTemp, Log, TEXT("False Sage level up bonus applied for level %d"), NewLevel);
}

FString UMingFalseSageTraits::GetFalseSageAbilityDescription(const FString& AbilityName)
{
    if (const FDeceptionAbility* Ability = FalseSageAbilityEffects.Find(AbilityName))
    {
        return Ability->Description;
    }
    return TEXT("未知偽聖者能力");
}

TArray<FString> UMingFalseSageTraits::GetFalseSageLevelUpRewards(int32 Level)
{
    TArray<FString> Rewards;
    
    if (Level == 1)
    {
        Rewards.Add("獲得基礎偽聖者能力");
        Rewards.Add("解鎖偽善欺騙");
    }
    else if (Level == 3)
    {
        Rewards.Add("解鎖陰謀策劃");
        Rewards.Add("操控能力+15%");
    }
    else if (Level == 5)
    {
        Rewards.Add("解鎖暗中操控");
        Rewards.Add("隱蔽性+20%");
    }
    else if (Level == 7)
    {
        Rewards.Add("解鎖背叛突襲");
        Rewards.Add("突襲傷害+25%");
    }
    else if (Level == 9)
    {
        Rewards.Add("解鎖虛假宣傳");
        Rewards.Add("宣傳效果+30%");
    }
    else if (Level == 10)
    {
        Rewards.Add("偽聖者進化");
        Rewards.Add("戰略思維+8，決策速度+0.2");
    }
    else if (Level == 12)
    {
        Rewards.Add("解鎖離間計");
        Rewards.Add("離間效果+35%");
    }
    else if (Level % 3 == 0)
    {
        Rewards.Add(FString::Printf(TEXT "偽聖者能力強化等級 %d", Level));
        Rewards.Add("魅力值+2");
    }
    
    return Rewards;
}

bool UMingFalseSageTraits::CanDeceiveTarget(float TargetWisdom, float DeceptionPower)
{
    float DeceptionChance = (DeceptionPower - TargetWisdom) / 100.0f;
    DeceptionChance = FMath::Clamp(DeceptionChance, 0.1f, 0.9f);
    
    return FMath::FRand() < DeceptionChance;
}

float UMingFalseSageTraits::CalculateBackstabDamage(float BaseDamage, float SurpriseFactor)
{
    float BackstabMultiplier = 1.0f + (SurpriseFactor / 100.0f) * 1.5f;
    return BaseDamage * BackstabMultiplier;
}

bool UMingFalseSageTraits::DetectConspiracy(float InvestigationLevel, float ConspiracyComplexity)
{
    float DetectionChance = InvestigationLevel / (InvestigationLevel + ConspiracyComplexity);
    return FMath::FRand() < DetectionChance;
}
