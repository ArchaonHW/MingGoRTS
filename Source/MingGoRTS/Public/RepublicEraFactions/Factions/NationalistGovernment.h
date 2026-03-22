// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 國民政府 - Nationalist Government (1925-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "NationalistGovernment.generated.h"

/**
 * 國民政府 - Nationalist Government
 * Difficulty: ★★★☆☆ (Normal)
 * 
 * The Nationalist Government led by the Kuomintang (KMT),
 * based in Guangzhou then Nanjing after the Northern Expedition.
 * 
 * Unique Mechanics:
 * 1. Nationalism - Morale boost and recruitment bonuses
 * 2. Modernization Drive - Technology research bonuses
 * 
 * Special Units:
 * 1. Whampoa Cadets - Elite officers from Whampoa Military Academy
 * 2. German-Equipped Divisions - Modernized infantry with German arms
 * 3. Air Support Group - Early Chinese air force
 * 4. Political Work Teams - Propaganda and morale units
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UNationalistGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UNationalistGovernment();
    
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
    FFactionUnit CreateWhampoaCadets() const;
    FFactionUnit CreateGermanEquippedDivisions() const;
    FFactionUnit CreateAirSupportGroup() const;
    FFactionUnit CreatePoliticalWorkTeams() const;
};
