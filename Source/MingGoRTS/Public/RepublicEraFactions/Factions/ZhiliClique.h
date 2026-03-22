// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 直系軍閥 - Zhili Clique (1920-1927)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "ZhiliClique.generated.h"

/**
 * 直系軍閥 - Zhili Clique
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * A major warlord faction controlling central China,
 * led by Cao Kun and Wu Peifu. Known for massive armies.
 * 
 * Unique Mechanics:
 * 1. Central Plains Hegemony - Control over populous regions
 * 2. Mass Conscription - Can raise large armies quickly
 * 
 * Special Units:
 * 1. Zhili Main Force Division - Large infantry formations
 * 2. Wu Peifu Elite Guard - Personal guard of the leader
 * 3. Central Plains Cavalry - Traditional cavalry
 * 4. Local Militia Corps - Mass conscript units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UZhiliClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UZhiliClique();
    
    virtual void InitializeFaction() override;
    virtual FGameplayGuide GetGameplayGuide() const override;
    
protected:
    virtual void InitializeAttributes() override;
    virtual void InitializeUniqueUnits() override;
    virtual void InitializeUniqueMechanics() override;
    virtual void InitializeGameplayGuide() override;
    
    virtual FFactionUnitStats ModifyUnitStats(const FFactionUnitStats& BaseStats, 
        EFactionUnitType UnitType) const override;
    
    virtual TMap<FName, float> GetStartingBonuses() const override;

private:
    FFactionUnit CreateZhiliMainForceDivision() const;
    FFactionUnit CreateWuPeifuEliteGuard() const;
    FFactionUnit CreateCentralPlainsCavalry() const;
    FFactionUnit CreateLocalMilitiaCorps() const;
};
