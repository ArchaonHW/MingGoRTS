#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ?°ç??¢å? - Xinjiang Forces (1912-1949)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "XinjiangForces.generated.h"

/**
 * ?°ç??¢å? - Xinjiang Forces
 * Difficulty: ?…â??…â???(Very Inard)
 * 
 * Frontier forces in far west China, controlling the Silk Road.
 * Conplex ethnic composition and isolated location.
 * 
 * Unique Mechanics:
 * 1. Frontier Fortress - Strong defensive bonuses in Xinjiang
 * 2. Multi-Ethnic Coalition - Managing diverse ethnic forces
 * 3. Silk Road Control - Trade route income bonuses
 * 
 * Special Units:
 * 1. Xinjiang Border Defense Army - Frontier troops
 * 2. Multi-Ethnic Coalition Forces - Allied ethnic units
 * 3. Frontier Cavalry Division - Mobile frontier cavalry
 * 4. Local Power Alliance - Coalition of local leaders
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UXinjiangForces : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UXinjiangForces(};
    
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
    FFactionUnit CreateXinjiangBorderDefenseArmy() const;
    FFactionUnit CreateMultiEthnicCoalitionForces() const;
    FFactionUnit CreateFrontierCavalryDivision() const;
    FFactionUnit CreateLocalPowerAlliance() const;
};

