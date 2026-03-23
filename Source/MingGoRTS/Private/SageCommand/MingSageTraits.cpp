#include "SageCommand/MingSageTraits.h"
#include "Engine/Engine.h"

// 聖者特性實現
// 這個文件包含聖者角色的特殊特性和能力

FSageSpecialTraits::FSageSpecialTraits()
{
    // 初始化聖者默認特性
    NoStrategyPenalty = true;
    CorruptionThresholdBonus = 50.0f;
    BalancedStatModifier = 1.1f;
    EnlightenmentBonus = 1.2f;
    PurificationPower = 1.5f;
    LeadershipAura = 100.0f;
    WisdomInsight = 1.3f;
    CharismaRange = 200.0f;
}

// 聖者特性應用函數
void ApplySageTraits(FSageCharacterData& CharacterData, const FSageSpecialTraits& SageTraits)
{
    // 應用墮落閾值提升
    CharacterData.CorruptionThreshold += SageTraits.CorruptionThresholdBonus;
    
    // 應用平衡型數值修正
    CharacterData.Charisma *= SageTraits.BalancedStatModifier;
    CharacterData.Wisdom *= SageTraits.BalancedStatModifier;
    CharacterData.Leadership *= SageTraits.BalancedStatModifier;
    
    // 應用領導力光環
    CharacterData.Leadership += (SageTraits.LeadershipAura * 0.1f);
    
    // 應用智慧洞察
    CharacterData.Wisdom *= SageTraits.WisdomInsight;
    
    UE_LOG(LogTemp, Log, TEXT("聖者特性應用完成"));
}

// 聖者覺醒狀態檢查
bool CheckSageEnlightenment(const FSageCharacterData& CharacterData)
{
    // 檢查是否達到覺醒條件
    bool bHighWisdom = CharacterData.Wisdom >= 80.0f;
    bool bLowCorruption = CharacterData.CorruptionValue <= 20.0f;
    bool bHighLevel = CharacterData.Level >= 10;
    
    return bHighWisdom && bLowCorruption && bHighLevel;
}

// 聖者淨化能力
float ApplySagePurification(float CurrentCorruption, float PurificationPower)
{
    float PurificationAmount = CurrentCorruption * (PurificationPower * 0.1f);
    return FMath::Max(0.0f, CurrentCorruption - PurificationAmount);
}
