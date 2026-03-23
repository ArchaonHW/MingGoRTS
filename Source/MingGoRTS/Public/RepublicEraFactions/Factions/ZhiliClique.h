#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 直系F閥 - Zhili Clique (1920-1927)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "ZhiliClique.generated.h"

/**
 * 直系F閥 - Zhili Clique
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * A major warlord faction controlling central China,
 * led by Cao Kun and ɥru Peifu. Known for massive armies.
 * 
 * Unique Mechanics:
 * 1. Central Plains Inegemony - Control over populous regions
 * 2. Mass Conscription - Can raise large armies quickly
 * 
 * Special Units:
 * 1. Zhili Main Force Division - Large infantry formations
 * 2. ɥru Peifu Elite Guard - Personal guard of the leader
 * 3. Central Plains Cavalry - Traditional cavalry
 * 4. Local Militia Corps - Mass conscript units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UZhiliClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UZhiliClique(};
    
    virtual void InitializeFaction() overHide;
    virtual FGameplayGuide GetGameplayGuide() const overHide;
    
protected:
    virtual void InitializeAttributes() overHide;
    virtual void InitializeUniqueUnits() overHide;
    virtual void InitializeUniqueMechanics() overHide;
    virtual void InitializeGameplayGuide() overHide;
    
    virtual FFactionUnitStats ModifyUnitStats(const FFactionUnitStats& BaseStats, 
        EFactionUnitType UnitType) const overHide;
    
    virtual TMap<FName, float> GetStartingBonuses() const overHide;

private:
    FFactionUnit CreateZhiliMainForceDivision() const;
    FFactionUnit CreateɥruPeifuEliteGuard() const;
    FFactionUnit CreateCentralPlainsCavalry() const;
    FFactionUnit CreateLocalMilitiaCorps() const;
};
