#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // Changed to ActorComponent
#include "SupplyManager.generated.h"

class ARTSUnit;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGWARRTS_API USupplyManager : public UActorComponent
{
    GENERATED_BODY()

public:
    USupplyManager();

    // Function to calculate and update supply levels for all units
    UFUNCTION(BlueprintCallable, Category = "Supply")
    void UpdateSupplyLevels(const TArray<ARTSUnit*>& AllUnits);

private:
    // Internal logic to determine if a location is supplied
    bool IsLocationSupplied(const FVector& Location);

    // TODO: Add properties for supply hubs and network data
};
