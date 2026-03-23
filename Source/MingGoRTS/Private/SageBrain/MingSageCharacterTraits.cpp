#include "SageBrain/MingSageCharacterTraits.h"
#include "Engine/Engine.h"

const TMap<FString, int32> UMingSageTraits::SageAbilityLevelRequirements = {
    {"聖光加持", 1},
    {"道德感召", 3},
    {"智慧啟迪", 5},
    {"民心歸向", 8},
    {"正義審判", 12},
    {"神聖守護", 15},
    {"淨化之光", 20},
    {"天啟預言", 25}
};

const TMap<FString, FSageAbilityEffect> UMingSageTraits::SageAbilityEffects = {
    {"聖光加持", FSageAbilityEffect{"聖光加持", 25.0f, 300.0f, {"ally", "self"}, "為友軍提供聖光護盾，增加防禦力"}},
    {"道德感召", FSageAbilityEffect{"道德感召", 40.0f, 600.0f, {"all", "enemy"}, "提升友軍士氣，有機會轉化敵軍"}},
    {"智慧啟迪", FSageAbilityEffect{"智慧啟迪", 30.0f, 450.0f, {"ally"}, "提升友軍學習速度和決策能力"}},
    {"民心歸向", FSageAbilityEffect{"民心歸向", 50.0f, 900.0f, {"civilian"}, "獲得民眾支持，增加資源產出"}},
    {"正義審判", FSageAbilityEffect{"正義審判", 80.0f, 120.0f, {"enemy"}, "對邪惡敵人造成巨大傷害"}},
    {"神聖守護", FSageAbilityEffect{"神聖守護", 60.0f, 480.0f, {"ally", "self"}, "創造神聖屏障，抵禦攻擊"}},
    {"淨化之光", FSageAbilityEffect{"淨化之光", 70.0f, 180.0f, {"enemy", "area"}, "淨化區域內的敵人"}},
    {"天啟預言", FSageAbilityEffect{"天啟預言", 100.0f, 1500.0f, {"all"}, "預見未來，提供戰術優勢"}}
};

UMingSageTraits::UMingSageTraits()
{
}

FSageCharacterTraits UMingSageTraits::GetSageDefaultTraits()
{
    FSageCharacterTraits Traits;
    Traits.CharacterType = ESageCharacterType::Sage;
    Traits.MoralAuthority = 85.0f;
    Traits.StrategicThinking = 75.0f;
    Traits.MilitaryCommand = 60.0f;
    Traits.CharismaBonus = 30.0f;
    Traits.LearningSpeed = 1.5f;
    Traits.DecisionSpeed = 1.2f;
    Traits.RiskTolerance = 0.3f;
    Traits.MaxFollowers = 500;
    Traits.SpecialAbilities = {"道德感召", "智慧啟迪", "民心歸向"};
    return Traits;
}

FSageAbilityEffect UMingSageTraits::CreateMoralAuraEffect()
{
    FSageAbilityEffect Effect;
    Effect.AbilityName = "道德光環";
    Effect.EffectMagnitude = 35.0f;
    Effect.Duration = 600.0f;
    Effect.TargetTypes = {"ally", "self", "civilian"};
    Effect.Description = "聖者散發的道德光環，持續提升周圍友軍的士氣和忠誠度";
    return Effect;
}

FSageAbilityEffect UMingSageTraits::CreateWisdomBlessingEffect()
{
    FSageAbilityEffect Effect;
    Effect.AbilityName = "智慧祝福";
    Effect.EffectMagnitude = 40.0f;
    Effect.Duration = 450.0f;
    Effect.TargetTypes = {"ally", "self"};
    Effect.Description = "聖者的智慧祝福，大幅提升目標的學習能力和戰術理解";
    return Effect;
}

FSageAbilityEffect UMingSageTraits::CreateHolyJudgmentEffect()
{
    FSageAbilityEffect Effect;
    Effect.AbilityName = "聖潔審判";
    Effect.EffectMagnitude = 95.0f;
    Effect.Duration = 90.0f;
    Effect.TargetTypes = {"enemy"};
    Effect.Description = "對邪惡敵人發動的神聖審判，造成巨大傷害並附加淨化效果";
    return Effect;
}

FSageMoralInfluence UMingSageTraits::GetSageMoralInfluence()
{
    FSageMoralInfluence Influence;
    Influence.MoraleBoost = 25.0f;
    Influence.LoyaltyIncrease = 15.0f;
    Influence.ConversionChance = 0.15f;
    Influence.InfluenceRadius = 500;
    Influence.bAffectsEnemies = true;
    return Influence;
}

TArray<FString> UMingSageTraits::GetSageSpecialAbilities()
{
    return {"聖光加持", "道德感召", "智慧啟迪", "民心歸向", "正義審判", "神聖守護", "淨化之光", "天啟預言"};
}

bool UMingSageTraits::CanUseSageAbility(const FString& AbilityName, int32 CharacterLevel)
{
    if (const int32* RequiredLevel = SageAbilityLevelRequirements.Find(AbilityName))
    {
        return CharacterLevel >= *RequiredLevel;
    }
    return false;
}

float UMingSageTraits::CalculateSageEffectiveness(const FSageCharacterTraits& Traits, const FString& Context)
{
    float BaseEffectiveness = 1.0f;
    
    if (Context == "moral_influence")
    {
        BaseEffectiveness = Traits.MoralAuthority / 100.0f;
        BaseEffectiveness += Traits.CharismaBonus / 100.0f;
    }
    else if (Context == "strategic_planning")
    {
        BaseEffectiveness = Traits.StrategicThinking / 100.0f;
        BaseEffectiveness += Traits.LearningSpeed * 0.2f;
    }
    else if (Context == "military_command")
    {
        BaseEffectiveness = Traits.MilitaryCommand / 100.0f;
        BaseEffectiveness += Traits.DecisionSpeed * 0.1f;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.0f, 2.0f);
}

void UMingSageTraits::ApplySageLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel)
{
    Traits.MoralAuthority += 2.0f;
    Traits.StrategicThinking += 1.5f;
    Traits.MilitaryCommand += 1.0f;
    Traits.CharismaBonus += 0.5f;
    
    if (NewLevel % 5 == 0)
    {
        Traits.MaxFollowers += 50;
    }
    
    if (NewLevel % 10 == 0)
    {
        Traits.LearningSpeed += 0.1f;
        Traits.DecisionSpeed += 0.05f;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Sage level up bonus applied for level %d"), NewLevel);
}

FString UMingSageTraits::GetSageAbilityDescription(const FString& AbilityName)
{
    if (const FSageAbilityEffect* Effect = SageAbilityEffects.Find(AbilityName))
    {
        return Effect->Description;
    }
    return TEXT("未知聖者能力");
}

TArray<FString> UMingSageTraits::GetSageLevelUpRewards(int32 Level)
{
    TArray<FString> Rewards;
    
    if (Level == 1)
    {
        Rewards.Add("獲得基礎聖者能力");
        Rewards.Add("解鎖道德光環");
    }
    else if (Level == 3)
    {
        Rewards.Add("解鎖道德感召");
        Rewards.Add("忠誠度+10");
    }
    else if (Level == 5)
    {
        Rewards.Add("解鎖智慧啟迪");
        Rewards.Add("學習速度+15%");
    }
    else if (Level == 8)
    {
        Rewards.Add("解鎖民心歸向");
        Rewards.Add("影響範圍+20%");
    }
    else if (Level == 10)
    {
        Rewards.Add("聖者進化");
        Rewards.Add("所有屬性+5");
    }
    else if (Level == 12)
    {
        Rewards.Add("解鎖正義審判");
        Rewards.Add("對邪惡傷害+25%");
    }
    else if (Level % 5 == 0)
    {
        Rewards.Add(FString::Printf(TEXT "聖者能力強化等級 %d", Level));
        Rewards.Add("追隨者上限+25");
    }
    
    return Rewards;
}
