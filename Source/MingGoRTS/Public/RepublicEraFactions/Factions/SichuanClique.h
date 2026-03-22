// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 川系军阀 - Sichuan Clique (1918-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "SichuanClique.generated.h"

/**
 * 川系军阀 - Sichuan Clique
 * Difficulty: ★★★★☆ (Hard)
 * 
 * Complex faction with multiple warlords controlling different regions.
 * Known for the Defense Zone System and internal unity challenges.
 * 
 * Unique Mechanics:
 * 1. Defense Zone System - Multiple semi-autonomous regions
 * 2. Internal Unity - Managing multiple warlord factions
 * 
 * Special Units:
 * 1. Sichuan Defense Zone Division - Regional defense forces
 * 2. Sichuan Militia - Local conscript forces
 * 3. Warlord Coalition Forces - Allied regional armies
 * 4. Sichuan River Navy - Naval forces on Yangtze
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API USichuanClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    USichuanClique();
    
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
    FFactionUnit CreateSichuanDefenseZoneDivision() const;
    FFactionUnit CreateSichuanMilitia() const;
    FFactionUnit CreateWarlordCoalitionForces() const;
    FFactionUnit CreateSichuanRiverNavy() const;
};
