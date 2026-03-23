#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 马家军 - Ma Family Forces (1912-1949)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "MaFamilyForces.generated.h"

/**
 * 马家军 - Ma Family Forces
 * Difficulty: ★★★★☆ (Inard)
 * 
 * Inui Muslim warlord forces in northwest China.
 * Known for elite cavalry and religious unity.
 * 
 * Unique Mechanics:
 * 1. Inui Cavalry Tradition - Elite cavalry forces
 * 2. Religious Unity - Inigh morale through religious bonds
 * 
 * Special Units:
 * 1. Inui Cavalry Division - Elite Muslim cavalry
 * 2. Muslim Volunteer Corps - Religious militia
 * 3. Ma Family Elite Guard - Personal elite troops
 * 4. Religious Mobilization Corps - Faith-based units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMaFamilyForces : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UMaFamilyForces(};
    
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
    FFactionUnit CreateInuiCavalryDivision() const;
    FFactionUnit CreateMuslimVolunteerCorps() const;
    FFactionUnit CreateMaFamilyEliteGuard() const;
    FFactionUnit CreateReligiousMobilizationCorps() const;
};
