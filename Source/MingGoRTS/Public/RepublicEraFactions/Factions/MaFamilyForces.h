// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 马家军 - Ma Family Forces (1912-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "MaFamilyForces.generated.h"

/**
 * 马家军 - Ma Family Forces
 * Difficulty: ★★★★☆ (Hard)
 * 
 * Hui Muslim warlord forces in northwest China.
 * Known for elite cavalry and religious unity.
 * 
 * Unique Mechanics:
 * 1. Hui Cavalry Tradition - Elite cavalry forces
 * 2. Religious Unity - High morale through religious bonds
 * 
 * Special Units:
 * 1. Hui Cavalry Division - Elite Muslim cavalry
 * 2. Muslim Volunteer Corps - Religious militia
 * 3. Ma Family Elite Guard - Personal elite troops
 * 4. Religious Mobilization Corps - Faith-based units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMaFamilyForces : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UMaFamilyForces();
    
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
    FFactionUnit CreateHuiCavalryDivision() const;
    FFactionUnit CreateMuslimVolunteerCorps() const;
    FFactionUnit CreateMaFamilyEliteGuard() const;
    FFactionUnit CreateReligiousMobilizationCorps() const;
};
