// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSageCharacterSystem.h"

UMingSageCharacterSystem::UMingSageCharacterSystem()
    : bIsInitialized(false)
    , DemonKingCheckInterval(30.0f)
    , AtonementFallReduction(50)
    , RighteousnessStrategyFallReduction(10)
{
}

void UMingSageCharacterSystem::InitializeSageCharacterSystem()
{
    if (bIsInitialized)
    {
        return;
    }

    InitializeDefaultTraits();
    InitializeCharacterDescriptions();

    bIsInitialized = true;
}

void UMingSageCharacterSystem::InitializeDefaultTraits()
{
    // 聖者特性 - 平衡型，v逆切換無懲罰
    {
        FSageCharacterTraits SageTraits;
        SageTraits.RighteousStrategyMultiplier = 1.0f;
        SageTraits.EvilStrategyMultiplier = 1.0f;
        SageTraits.FallThreshold = 150;
        SageTraits.DiplomacyBonus = 0.0f;
        SageTraits.bCanUseEvilStrategies = true;
        SageTraits.bCanStopEvilStrategies = true;
        SageTraits.SwitchPenalty = 0.0f;
        CharacterTraitsMap.Add(ESageCharacterType::Sage, SageTraits);
    }

    // 魔王特性 - 逆策+50%，無法停止
    {
        FSageCharacterTraits DemonKingTraits;
        DemonKingTraits.RighteousStrategyMultiplier = 0.7f;
        DemonKingTraits.EvilStrategyMultiplier = 1.5f;
        DemonKingTraits.FallThreshold = 200;
        DemonKingTraits.DiplomacyBonus = 0.0f;
        DemonKingTraits.bCanUseEvilStrategies = true;
        DemonKingTraits.bCanStopEvilStrategies = false;
        DemonKingTraits.SwitchPenalty = 0.2f;
        CharacterTraitsMap.Add(ESageCharacterType::DemonKing, DemonKingTraits);
    }

    // 偽聖者特性 - v策+20%，無法i用逆策，外交+30%
    {
        FSageCharacterTraits PseudoSageTraits;
        PseudoSageTraits.RighteousStrategyMultiplier = 1.2f;
        PseudoSageTraits.EvilStrategyMultiplier = 0.0f;
        PseudoSageTraits.FallThreshold = 80;
        PseudoSageTraits.DiplomacyBonus = 0.3f;
        PseudoSageTraits.bCanUseEvilStrategies = false;
        PseudoSageTraits.bCanStopEvilStrategies = true;
        PseudoSageTraits.SwitchPenalty = 0.0f;
        CharacterTraitsMap.Add(ESageCharacterType::PseudoSage, PseudoSageTraits);
    }
}

void UMingSageCharacterSystem::InitializeCharacterDescriptions()
{
    CharacterDescriptionMap.Add(ESageCharacterType::Sage, 
        TEXT("至聖者能用而能停，v邪皆兵而不墮。v逆策略切換無懲罰，是最平衡N指揮者。"));
    
    CharacterDescriptionMap.Add(ESageCharacterType::DemonKing, 
        TEXT("魔王能用邪而不能停，逆策效果+50%但無法停止。一旦踏上邪道，便無法回頭。"));
    
    CharacterDescriptionMap.Add(ESageCharacterType::PseudoSage, 
        TEXT("偽聖者不能用而假裝不用，v策+20%，外交+30%，但無法i用逆策。表面清高，實則無能。"));
}

FSageCharacterData UMingSageCharacterSystem::CreateCharacter(ESageCharacterType CharacterType, const FString& CharacterName)
{
    FSageCharacterData CharacterData;
    CharacterData.CharacterType = CharacterType;
    CharacterData.CharacterName = CharacterName;
    CharacterData.Traits = GetCharacterTraits(CharacterType);
    
    // g默認描述
    if (CharacterData.CharacterDescription.IsEmpty())
    {
        CharacterData.CharacterDescription = GetCharacterDescription(CharacterType);
    }

    return CharacterData;
}

FSageCharacterTraits UMingSageCharacterSystem::GetCharacterTraits(ESageCharacterType CharacterType) const
{
    if (const FSageCharacterTraits* FoundTraits = CharacterTraitsMap.Find(CharacterType))
    {
        return *FoundTraits;
    }
    
    return FSageCharacterTraits();
}

FString UMingSageCharacterSystem::GetCharacterDescription(ESageCharacterType CharacterType) const
{
    if (const FString* FoundDesc = CharacterDescriptionMap.Find(CharacterType))
    {
        return *FoundDesc;
    }
    
    return TEXT("未知角色類型");
}

bool UMingSageCharacterSystem::CanUseEvilStrategy(const FSageCharacterData& CharacterData) const
{
    // 檢查是否處於墮落狀態
    if (CharacterData.bIsFallen)
    {
        return false;
    }

    // 檢查特性是否允許i用逆策
    return CharacterData.Traits.bCanUseEvilStrategies;
}

bool UMingSageCharacterSystem::CanStopEvilStrategy(const FSageCharacterData& CharacterData) const
{
    // 如果已經墮落，無法停止
    if (CharacterData.bIsFallen)
    {
        return false;
    }

    // 檢查特性是否允許停止逆策
    return CharacterData.Traits.bCanStopEvilStrategies;
}

bool UMingSageCharacterSystem::UseEvilStrategy(FSageCharacterData& CharacterData, int32 FallValueIncrease)
{
    if (!CanUseEvilStrategy(CharacterData))
    {
        return false;
    }

    // 增加墮落值
    CharacterData.CurrentFallValue += FallValueIncrease;
    CharacterData.EvilStrategyUseCount++;

    // 廣播墮落值變化事件
    OnFallValueChanged.Broadcast(CharacterData, CharacterData.CurrentFallValue);

    // 檢查是否墮落
    CheckAndApplyFall(CharacterData);

    return true;
}

bool UMingSageCharacterSystem::UseRighteousStrategy(FSageCharacterData& CharacterData, int32 FallValueDecrease)
{
    // 如果已經墮落，無法i用v策
    if (CharacterData.bIsFallen)
    {
        return false;
    }

    // 減少墮落值 (i用配置N減少值或傳入N值)
    int32 ActualDecrease = FallValueDecrease > 0 ? FallValueDecrease : RighteousnessStrategyFallReduction;
    CharacterData.CurrentFallValue = FMath::Max(0, CharacterData.CurrentFallValue - ActualDecrease);
    CharacterData.RighteousStrategyUseCount++;

    // 廣播墮落值變化事件
    OnFallValueChanged.Broadcast(CharacterData, CharacterData.CurrentFallValue);

    return true;
}

bool UMingSageCharacterSystem::CheckAndApplyFall(FSageCharacterData& CharacterData)
{
    // 檢查是否達到墮落閾值
    if (CharacterData.CurrentFallValue >= CharacterData.Traits.FallThreshold)
    {
        if (!CharacterData.bIsFallen)
        {
            CharacterData.bIsFallen = true;
            ApplyFallenEffects(CharacterData);
            
            // 廣播墮落事件
            OnCharacterFallen.Broadcast(CharacterData);
        }
        return true;
    }
    
    return false;
}

void UMingSageCharacterSystem::ApplyFallenEffects(FSageCharacterData& CharacterData)
{
    // 墮落狀態效果：
    // 1. 無法i用v策
    // 2. v策效果歸零
    // 3. 外交關係惡化
    
    CharacterData.Traits.RighteousStrategyMultiplier = 0.0f;
    
    // 魔王進入永久逆策模式
    if (CharacterData.CharacterType == ESageCharacterType::DemonKing)
    {
        // 魔王墮落後更加強j但也更加失控
        CharacterData.Traits.EvilStrategyMultiplier = 2.0f;
    }
}

bool UMingSageCharacterSystem::Atonement(FSageCharacterData& CharacterData)
{
    // 只有墮落N角色需要贖罪
    if (!CharacterData.bIsFallen)
    {
        // 未墮落時贖罪也有效果，但較小
        CharacterData.CurrentFallValue = FMath::Max(0, CharacterData.CurrentFallValue - (AtonementFallReduction / 2));
        OnFallValueChanged.Broadcast(CharacterData, CharacterData.CurrentFallValue);
        return true;
    }

    // 減少墮落值
    CharacterData.CurrentFallValue = FMath::Max(0, CharacterData.CurrentFallValue - AtonementFallReduction);
    
    // 如果墮落值低於閾值N一半，解除墮落狀態
    if (CharacterData.CurrentFallValue < (CharacterData.Traits.FallThreshold / 2))
    {
        CharacterData.bIsFallen = false;
        
        // 恢復v策效果 (根據角色類型)
        switch (CharacterData.CharacterType)
        {
        case ESageCharacterType::Sage:
            CharacterData.Traits.RighteousStrategyMultiplier = 1.0f;
            break;
        case ESageCharacterType::DemonKing:
            CharacterData.Traits.RighteousStrategyMultiplier = 0.7f;
            break;
        case ESageCharacterType::PseudoSage:
            CharacterData.Traits.RighteousStrategyMultiplier = 1.2f;
            break;
        default:
            break;
        }
    }

    // 廣播墮落值變化事件
    OnFallValueChanged.Broadcast(CharacterData, CharacterData.CurrentFallValue);

    return true;
}

FString UMingSageCharacterSystem::GetCharacterTypeDisplayName(ESageCharacterType CharacterType) const
{
    switch (CharacterType)
    {
    case ESageCharacterType::Sage:
        return TEXT("聖者");
    case ESageCharacterType::DemonKing:
        return TEXT("魔王");
    case ESageCharacterType::PseudoSage:
        return TEXT("偽聖者");
    default:
        return TEXT("未知");
    }
}

TArray<ESageCharacterType> UMingSageCharacterSystem::GetAvailableCharacterTypes() const
{
    TArray<ESageCharacterType> AvailableTypes;
    AvailableTypes.Add(ESageCharacterType::Sage);
    AvailableTypes.Add(ESageCharacterType::DemonKing);
    AvailableTypes.Add(ESageCharacterType::PseudoSage);
    return AvailableTypes;
}

void UMingSageCharacterSystem::CheckDemonKingForcedEvil(FSageCharacterData& CharacterData)
{
    // 魔王在特定條件_會強制i用逆策
    if (CharacterData.CharacterType == ESageCharacterType::DemonKing)
    {
        // 如果魔王長時間不i用逆策，可能會有懲罰
        // 這個邏輯可以在遊戲循環中定期調用
    }
}

