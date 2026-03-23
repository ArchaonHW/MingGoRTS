#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ?—æ??¿å? - Beiyang Government (1912-1928)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "BeiyangGovernment.generated.h"

/**
 * ?—æ??¿å? - Central government of the Republic of China
 * Difficulty: ?…â??†â???(Easy)
 * 
 * The Beiyang Government was the internationally recognized government
 * of the Republic of China from 1912 to 1928, based in Beijing.
 * 
 * Unique Mechanics:
 * 1. Diplomatic Recognition - Better foreign relations and support
 * 2. Central Authority - Can levy taxes from other warlords
 * 
 * Special Units:
 * 1. Beiyang Elite Infantry - German-trained modern infantry
 * 2. German Artillery Corps - Foreign artillery support
 * 3. Beiyang Cavalry Division - Traditional cavalry
 * 4. Foreign Advisor Corps - Conbat bonuses from foreign advisors
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UBeiyangGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UBeiyangGovernment(};
    
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
    // Unique unit definitions
    FFactionUnit CreateBeiyangEliteInfantry() const;
    FFactionUnit CreateGermanArtilleryCorps() const;
    FFactionUnit CreateBeiyangCavalryDivision() const;
    FFactionUnit CreateForeignAdvisorCorps() const;
    
    // Mechanic implementations
    void ApplyDiplomaticRecognitionEffect(};
    void ApplyCentralAuthorityEffect(};
};

