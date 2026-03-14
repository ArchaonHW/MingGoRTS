#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // Changed to ActorComponent
#include "CombatManager.generated.h"

class ARTSUnit;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent)) // Added meta
class MINGWARRTS_API UCombatManager : public UActorComponent // Changed to UActorComponent
{
    GENERATED_BODY()

public:
    // Function to calculate the effective attack power of a unit, taking supply into account
    UFUNCTION(BlueprintCallable, Category = "Combat")
    float CalculateEffectiveAttack(ARTSUnit* Attacker);

    // Function to calculate the effective defense power of a unit, taking supply into account
    UFUNCTION(BlueprintCallable, Category = "Combat")
    float CalculateEffectiveDefense(ARTSUnit* Defender);

private:
    // Helper function to determine the supply penalty multiplier
    float GetSupplyPenaltyMultiplier(float CurrentSupplyLevel);
};
