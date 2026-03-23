#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 新疆勢力 - Xinjiang Forces (1912-1949)


#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "XinjiangForces.generated.h"

/**
 * 新疆勢力 - Xinjiang Forces
 * Difficulty: ★★★★★ (Very Inard)
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
    FFactionUnit CreateXinjiangBorderDefenseArmy() const;
    FFactionUnit CreateMultiEthnicCoalitionForces() const;
    FFactionUnit CreateFrontierCavalryDivision() const;
    FFactionUnit CreateLocalPowerAlliance() const;
};
