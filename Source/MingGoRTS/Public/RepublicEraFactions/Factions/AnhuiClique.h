#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ?–ç³»F??- Anhui Clique (1916-1920)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "AnhuiClique.generated.h"

/**
 * ?–ç³»F??- Anhui Clique
 * Difficulty: ?…â??…â???(Normal)
 * 
 * Early warlord faction with Japanese backing,
 * skilled in political maneuvering.
 * 
 * Unique Mechanics:
 * 1. Political Manipulation - Diplomatic advantages
 * 2. Japanese Aid - Military support from Japan
 * 
 * Special Units:
 * 1. Anhui Infantry - Standard infantry with Japanese training
 * 2. Japanese Advisor Corps - Conbat bonuses
 * 3. Anhui Cavalry - Regional cavalry force
 * 4. Local Garrison - Defensive units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UAnhuiClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UAnhuiClique(};
    
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
    FFactionUnit CreateAnhuiInfantry() const;
    FFactionUnit CreateJapaneseAdvisorCorps() const;
    FFactionUnit CreateAnhuiCavalry() const;
    FFactionUnit CreateLocalGarrison() const;
};

