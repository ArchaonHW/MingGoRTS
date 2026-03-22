// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 中國共產黨 - Chinese Communist Party (1927-1949)

#pragma once

#include "CoreMinimal.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "CommunistParty.generated.h"

/**
 * 中國共產黨 - Chinese Communist Party
 * Difficulty: ★★★★☆ (Hard)
 * 
 * The Communist Party of China, fighting a people's war
 * through guerrilla tactics and mass mobilization.
 * 
 * Unique Mechanics:
 * 1. People's War - Mass mobilization and guerrilla bonuses
 * 2. Land Reform - Economic bonuses from rural support
 * 
 * Special Units:
 * 1. Red Army Guerrillas - Elite guerrilla fighters
 * 2. Workers' and Peasants' Red Guards - Mass militia
 * 3. Political Commissars - Ideological enforcers
 * 4. Logistics Support Corps - Supply from the masses
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UCommunistParty : public UMingRTSFactionBase
{
    GENERATED_BODY()
    
public:
    UCommunistParty();
    
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
    FFactionUnit CreateRedArmyGuerrillas() const;
    FFactionUnit CreateWorkersPeasantsRedGuards() const;
    FFactionUnit CreatePoliticalCommissars() const;
    FFactionUnit CreateLogisticsSupportCorps() const;
};
