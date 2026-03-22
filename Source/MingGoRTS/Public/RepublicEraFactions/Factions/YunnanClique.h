// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 滇系军阀 - Yunnan Clique (1915-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "YunnanClique.generated.h"

/**
 * 滇系军阀 - Yunnan Clique
 * Difficulty: ★★★★☆ (Hard)
 * 
 * Led by Tang Jiyao and Long Yun, inheritors of the National Protection War tradition.
 * Specialist in frontier expansion and ethnic coalition warfare.
 * 
 * Unique Mechanics:
 * 1. National Protection Tradition - High morale and legitimacy
 * 2. Frontier Expansion - Bonuses for conquering new territories
 * 
 * Special Units:
 * 1. Yunnan Border Defense Division - Expert frontier troops
 * 2. Ethnic Minority Coalition - Allied minority forces
 * 3. National Protection Elite - Veteran revolutionary troops
 * 4. Frontier Patrol Corps - Scouts and border guards
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UYunnanClique : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UYunnanClique();
    
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
    FFactionUnit CreateYunnanBorderDefenseDivision() const;
    FFactionUnit CreateEthnicMinorityCoalition() const;
    FFactionUnit CreateNationalProtectionElite() const;
    FFactionUnit CreateFrontierPatrolCorps() const;
};
