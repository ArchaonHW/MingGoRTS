#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingResourceSystem.h"
#include "MingResourceSystem.generated.h"

UENUM(BlueprintType)
enum class EMingResourceSystemType: uint8 {
    Food UMETA(DisplayName = "Food"),
    Money UMETA(DisplayName = "Money"),
    Materials UMETA(DisplayName = "Materials"),
    Fuel UMETA(DisplayName = "Fuel"),
    Ammo UMETA(DisplayName = "Ammo"),
    Manpower UMETA(DisplayName = "Manpower")
};

USTRUCT(BlueprintType)
struct FMingResourceAmount
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EMingResourceType ResourceType;

    UPROPERTY(BlueprintReadWrite)
    int32 Amount;

    FMingResourceAmount()
        : ResourceType(EMingResourceType::Money)
        , Amount(0)
    {}

    FMingResourceAmount(EMingResourceType Type, int32 InAmount)
        : ResourceType(Type)
        , Amount(InAmount)
    {}
};

USTRUCT(BlueprintType)
struct FMingResourceProduction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EMingResourceType ResourceType;

    UPROPERTY(BlueprintReadWrite)
    float ProductionRate; // Amount per second

    UPROPERTY(BlueprintReadWrite)
    float ProductionInterval;

    UPROPERTY(BlueprintReadOnly)
    float TimeSinceLastProduction;

    FMingResourceProduction()
        : ResourceType(EMingResourceType::Money)
        , ProductionRate(1.0f)
        , ProductionInterval(1.0f)
        , TimeSinceLastProduction(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingResourceCost
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EMingResourceType ResourceType;

    UPROPERTY(BlueprintReadWrite)
    int32 Cost;

    FMingResourceCost()
        : ResourceType(EMingResourceType::Money)
        , Cost(0)
    {}
};

/**
 * Resource management system for MingGoRTS
 * Handles resource storage, production, and consumption
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingResourceSystem();

    // Resource Storage
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    TMap<EMingResourceType, int32> ResourceStorage;

    UPROPERTY(EditDefaultsOnly, Category = "Limits")
    TMap<EMingResourceType, int32> ResourceCapacityLimits;

    // Production
    UPROPERTY(BlueprintReadOnly, Category = "Production")
    TArray<FMingResourceProduction> ActiveProductions;

    // Production (not exposed to Blueprint as TWeakObjectPtr is not Blueprint-supported)
    TArray<TWeakObjectPtr<class UMingResourceProducer>> ResourceProducers;

    // System Functions
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void InitializeDefaultResources();

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void AddResource(EMingResourceType Type, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool ConsumeResource(EMingResourceType Type, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool HasEnoughResources(const TArray<FMingResourceCost>& Costs) const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool ConsumeResources(const TArray<FMingResourceCost>& Costs);

    UFUNCTION(BlueprintPure, Category = "Resources")
    int32 GetResourceAmount(EMingResourceType Type) const;

    UFUNCTION(BlueprintPure, Category = "Resources")
    int32 GetResourceCapacity(EMingResourceType Type) const;

    UFUNCTION(BlueprintPure, Category = "Resources")
    float GetResourcePercentage(EMingResourceType Type) const;

    // Production Management
    UFUNCTION(BlueprintCallable, Category = "Production")
    void RegisterResourceProducer(UMingResourceProducer* Producer);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void UnregisterResourceProducer(UMingResourceProducer* Producer);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void AddProduction(const FMingResourceProduction& Production);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void RemoveProduction(EMingResourceType Type);

    // Event Delegates
    
    
    

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceChanged OnResourceChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceInsufficient OnResourceInsufficient;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceProduced OnResourceProduced;

    // Update
    UFUNCTION(BlueprintCallable, Category = "Update")
    void UpdateProduction(float DeltaTime);

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetResourceName(EMingResourceType Type);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FLinearColor GetResourceColor(EMingResourceType Type);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    TMap<EMingResourceType, int32> SerializeResources() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void DeserializeResources(const TMap<EMingResourceType, int32>& SavedResources);

protected:
    UPROPERTY()
    TWeakObjectPtr<class AMingRTSPlayerController> OwningPlayer;

    void ProcessProduction(float DeltaTime);
    void ClampResourcesToCapacity();
    void NotifyResourceChange(EMingResourceType Type, int32 NewAmount);

    // Resource change buffer for batch processing
    TArray<EMingResourceType> PendingResourceChanges;
);

