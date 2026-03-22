// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 奉系軍閥 - Fengtian Clique (1916-1928)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "FengtianClique.generated.h"

/**
 * 奉系軍閥 - Fengtian Clique
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * The dominant warlord faction in Manchuria, led by the Zhang family.
 * Known for Japanese support and strong cavalry forces.
 * 
 * Unique Mechanics:
 * 1. Manchurian Industry - Economic bonuses from industrial development
 * 2. Japanese Support - Military aid from Japan
 * 
 * Special Units:
 * 1. Northeast Army Infantry - Industrial-era infantry
 * 2. White Russian Mercenaries - Foreign veteran soldiers
 * 3. Fengtian Armored Train - Mobile fortress
 * 4. Cavalry Brigade - Mongolian-style cavalry
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UFengtianClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UFengtianClique();
    
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
    FFactionUnit CreateNortheastArmyInfantry() const;
    FFactionUnit CreateWhiteRussianMercenaries() const;
    FFactionUnit CreateFengtianArmoredTrain() const;
    FFactionUnit CreateCavalryBrigade() const;
};
