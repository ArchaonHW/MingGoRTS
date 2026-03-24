#include "SageCommand/MingFalseSageTraits.h"
#include "Engine/Engine.h"

// 偽聖者特性實現
// 這個文件包含偽聖者角色N特殊特性和能力

FFalseSageSpecialTraits::FFalseSageSpecialTraits()
{
    // 初始化偽聖者默認特性
    DeceptionSuccessRate = 0.7f;
    BetrayalChance = 0.3f;
    bHideTrueIdentity = true;
    InfiltrationAbility = 1.4f;
    ManipulationPower = 1.3f;
    StealthEffectiveness = 1.5f;
    SabotagePower = 1.2f;
    DisguiseDuration = 300.0f;
    BetrayalDamageBonus = 2.0f;
    IdentityRevealPenalty = 1.5f;
    ChaosSpread = 1.6f;
}

// 偽聖者特性應用函數
void ApplyFalseSageTraits(FSageCharacterData& CharacterData, const FFalseSageSpecialTraits& FalseSageTraits)
{
    // 應用魅力提升（欺騙需要）
    CharacterData.Charisma *= 1.2f;
    
    // 應用智慧提升（操控需要）
    CharacterData.Wisdom *= 1.1f;
    
    // 應用領導力略降（不擅長直接領導）
    CharacterData.Leadership *= 0.9f;
    
    // 調整墮落閾值（中等）
    CharacterData.CorruptionThreshold *= 0.85f;
    
    UE_LOG(LogTemp, Log, TEXT("偽聖者特性應用完e"));
}

// 偽聖者欺騙檢查
bool CheckFalseSageDeception(float TargetSuspicion, float DeceptionSuccessRate)
{
    float DeceptionRoll = FMath::RandRange(0.0f, 1.0f);
    float EffectiveSuccessRate = DeceptionSuccessRate * (1.0f - TargetSuspicion);
    
    return DeceptionRoll <= EffectiveSuccessRate;
}

// 偽聖者背叛判定
bool CheckFalseSageBetrayal(const FFalseSageSpecialTraits& FalseSageTraits, float OpportunityLevel)
{
    float BetrayalRoll = FMath::RandRange(0.0f, 1.0f);
    float EffectiveBetrayalChance = FalseSageTraits.BetrayalChance * OpportunityLevel;
    
    return BetrayalRoll <= EffectiveBetrayalChance;
}

// 偽聖者身份隱藏效果
float ApplyStealthEffect(float DetectionChance, float StealthEffectiveness)
{
    return DetectionChance / StealthEffectiveness;
}

// 偽聖者滲透能力
float ApplyInfiltrationBonus(float BaseInfiltrationChance, float InfiltrationAbility)
{
    return FMath::Min(1.0f, BaseInfiltrationChance * InfiltrationAbility);
}

// 偽聖者操控影響
float ApplyManipulationPower(float TargetMorale, float ManipulationPower)
{
    float ManipulationEffect = ManipulationPower * 0.2f;
    return TargetMorale * (1.0f - ManipulationEffect);
}

// 偽聖者破壞效果
float ApplySabotageDamage(float BaseDamage, float SabotagePower)
{
    return BaseDamage * SabotagePower;
}

// 偽聖者混亂傳播
void ApplyChaosSpread(FSageCharacterData& CharacterData, float ChaosSpread)
{
    // 混亂傳播影響周圍單位
    CharacterData.CorruptionValue += (ChaosSpread * 10.0f);
    
    // 降低自身穩定性
    CharacterData.SpiritualPower *= (1.0f - ChaosSpread * 0.1f);
}

// 偽聖者身份揭露處理
void HandleIdentityReveal(FSageCharacterData& CharacterData, const FFalseSageSpecialTraits& FalseSageTraits)
{
    // 身份揭露時N懲罰
    CharacterData.Charisma *= 0.5f; // 魅力j幅_降
    CharacterData.CorruptionValue += 50.0f; // 墮落值增加
    CharacterData.CorruptionThreshold *= FalseSageTraits.IdentityRevealPenalty; // 墮落閾值調整
    
    UE_LOG(LogTemp, Warning, TEXT("偽聖者身份已揭露，受到懲罰"));
}
