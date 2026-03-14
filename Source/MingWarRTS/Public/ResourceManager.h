#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModifierManager.h" // Include ModifierManager
#include "ResourceManager.generated.h"

class UModifierManager; // Forward declaration

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINGWARRTS_API UResourceManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UResourceManager();

    // Reference to the ModifierManager (will be set up in PlayerController or GameState)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources")
    UModifierManager* ModifierManagerRef;

    // A map to store player's resources, e.g., "res_money": 1000
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources")
    TMap<FName, int32> PlayerResources;

    // Function to load initial resources from a CSV file
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void LoadInitialResources();

    // Check if the player has enough of multiple resources
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool HasEnoughResources(const TMap<FName, int32>& Costs) const;

    // Consume multiple resources. Returns true if successful (had enough).
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool ConsumeResources(const TMap<FName, int32>& Costs);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // To unbind delegate

    // Callback function for when a modifier is applied
    UFUNCTION()
    void OnModifierApplied(FName ModifierID, float Value);

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
