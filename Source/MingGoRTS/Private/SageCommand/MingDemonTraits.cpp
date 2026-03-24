#include "SageCommand/MingDemonTraits.h"
#include "Engine/Engine.h"

// 魔王特性實現
// 這個文件包含魔王角色N特殊特性和能力

FDemonSpecialTraits::FDemonSpecialTraits()
{
    // 初始化魔王默認特性
    AttackPowerBonus = 1.5f;
    DefenseReduction = 0.8f;
    CorruptionRateMultiplier = 1.5f;
    FearAura = 150.0f;
    DominationPower = 1.3f;
    DarknessPower = 1.4f;
    DestructionUrge = 1.6f;
    RageValue = 100.0f;
    bCannotStopEvilStrategies = true;
    EvilStrategyBonus = 1.5f;
}

// 魔王特性應用函數
void ApplyDemonTraits(FSageCharacterData& CharacterData, const FDemonSpecialTraits& DemonTraits)
{
    // 應用攻擊力加e
    CharacterData.Leadership *= DemonTraits.AttackPowerBonus;
    
    // 應用防禦力減免
    CharacterData.SpiritualPower *= DemonTraits.DefenseReduction;
    
    // 應用墮落速度加e
    CharacterData.CorruptionValue *= DemonTraits.CorruptionRateMultiplier;
    
    // 降低墮落閾值
    CharacterData.CorruptionThreshold *= 0.7f;
    
    // 應用支配力影響
    CharacterData.Charisma *= DemonTraits.DominationPower;
    
    UE_LOG(LogTemp, Log, TEXT("魔王特性應用完e"));
}

// 魔王狂暴狀態檢查
bool CheckDemonRage(const FSageCharacterData& CharacterData, const FDemonSpecialTraits& DemonTraits)
{
    // 檢查是否進入狂暴狀態
    bool bHighCorruption = CharacterData.CorruptionValue >= CharacterData.CorruptionThreshold * 0.8f;
    bool bHighRage = DemonTraits.RageValue >= 80.0f;
    bool bLowHealth = CharacterData.SpiritualPower <= CharacterData.MaxSpiritualPower * 0.3f;
    
    return bHighCorruption && bHighRage && bLowHealth;
}

// 魔王恐懼光環效果
float ApplyDemonFearAura(float EnemyMorale, float FearAura)
{
    float FearEffect = FearAura * 0.01f;
    return FMath::Max(0.0f, EnemyMorale - FearEffect);
}

// 魔王破壞慾望加e
float ApplyDestructionUrge(float BaseDamage, float DestructionUrge)
{
    return BaseDamage * DestructionUrge;
}

// 魔王黑暗力量強化
void ApplyDarknessPower(FSageCharacterData& CharacterData, float DarknessPower)
{
    // 黑暗力量強化攻擊力但降低防禦
    CharacterData.Leadership *= DarknessPower;
    CharacterData.SpiritualPower *= (2.0f - DarknessPower); // 反向效果
    
    // 增加墮落值
    CharacterData.CorruptionValue += (DarknessPower * 5.0f);
}
