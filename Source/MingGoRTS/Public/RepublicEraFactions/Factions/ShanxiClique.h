// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 晋系军阀 - Shanxi Clique (1911-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "ShanxiClique.generated.h"

/**
 * 晋系军阀 - Shanxi Clique
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * Led by Yan Xishan, known for the "Shanxi Model" of governance.
 * Defensive specialist with strong provincial defense.
 * 
 * Unique Mechanics:
 * 1. Shanxi Model - Economic and administrative efficiency
 * 2. Defensive Specialist - Strong fortifications and defense
 * 
 * Special Units:
 * 1. Shanxi Defense Division - Expert defensive infantry
 * 2. Shanxi Engineer Corps - Fortification builders
 * 3. Shanxi Merchant Guards - Economic protection units
 * 4. Local Security Forces - Provincial defense troops
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UShanxiClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UShanxiClique();
    
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
    FFactionUnit CreateShanxiDefenseDivision() const;
    FFactionUnit CreateShanxiEngineerCorps() const;
    FFactionUnit CreateShanxiMerchantGuards() const;
    FFactionUnit CreateLocalSecurityForces() const;
};
