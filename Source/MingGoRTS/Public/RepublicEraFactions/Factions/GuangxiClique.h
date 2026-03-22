// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 桂系军阀 - Guangxi Clique (1924-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "GuangxiClique.generated.h"

/**
 * 桂系军阀 - Guangxi Clique
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * Led by Li Zongren and Bai Chongxi, known for Guangxi militia system.
 * Specialist in mountain warfare and guerrilla tactics.
 * 
 * Unique Mechanics:
 * 1. Guangxi Militia System - Mass mobilization of civilians
 * 2. Mountain Warfare - Superior combat in rough terrain
 * 
 * Special Units:
 * 1. Guangxi Mountain Division - Elite mountain troops
 * 2. Guangxi Militia - Mass conscript forces
 * 3. Mountain Guerrillas - Irregular mountain fighters
 * 4. Local Alliance Forces - Coalition of local powers
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UGuangxiClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UGuangxiClique();
    
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
    FFactionUnit CreateGuangxiMountainDivision() const;
    FFactionUnit CreateGuangxiMilitia() const;
    FFactionUnit CreateMountainGuerrillas() const;
    FFactionUnit CreateLocalAllianceForces() const;
};
