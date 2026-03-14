#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // Changed to ActorComponent
#include "BuildingManager.generated.h"

USTRUCT(BlueprintType)
struct FBuildingData
{
    GENERATED_BODY()

    FBuildingData() : BuildTimeSeconds(0.0f) {} // Added constructor

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    // The cost to construct this building
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, int32> ConstructionCost;
    
    // Time in seconds to construct
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BuildTimeSeconds;

    // Resources this building produces per tick (e.g., "res_steel": 5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, int32> ResourceProductionRate;

    // A list of unit IDs that this building unlocks for production elsewhere
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> UnlocksUnits;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent)) // Added meta
class MINGWARRTS_API UBuildingManager : public UActorComponent // Changed to UActorComponent
{
    GENERATED_BODY()

public:
    // All available buildings loaded from JSON
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildings")
    TMap<FName, FBuildingData> AllBuildingsData;

    // A simple list to keep track of what buildings the player currently owns
    // In a real game, this would be more complex (tracking instances, locations, etc.)
    // but for our MVP, tracking the IDs is sufficient for dependency checking.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildings")
    TArray<FName> PlayerOwnedBuildings;

    // Function to load all building data from the JSON file
    UFUNCTION(BlueprintCallable, Category = "Buildings")
    void LoadAllBuildingData();

    // Check if the player owns a specific building type
    UFUNCTION(BlueprintCallable, Category = "Buildings")
    bool HasBuilding(FName BuildingID) const;
};
