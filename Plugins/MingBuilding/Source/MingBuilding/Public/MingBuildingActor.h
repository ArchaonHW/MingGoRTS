#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingResourceSystem.h"
#include "MingBuildingActor.generated.h"

UENUM(BlueprintType)
enum class EMingBuildingType : uint8
{
    CommandCenter UMETA(DisplayName = "Command Center"),
    Barracks UMETA(DisplayName = "Barracks"),
    Factory UMETA(DisplayName = "Factory"),
    SupplyDepot UMETA(DisplayName = "Supply Depot"),
    ResourceGatherer UMETA(DisplayName = "Resource Gatherer"),
    DefenseTower UMETA(DisplayName = "Defense Tower")
};

UENUM(BlueprintType)
enum class EMingBuildingState : uint8
{
    Planned UMETA(DisplayName = "Planned"),
    Constructing UMETA(DisplayName = "Constructing"),
    Active UMETA(DisplayName = "Active"),
    Damaged UMETA(DisplayName = "Damaged"),
    Destroyed UMETA(DisplayName = "Destroyed")
};

USTRUCT(BlueprintType)
struct FMingBuildingCost
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EMingBuildingType BuildingType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FMingResourceCost> ResourceCosts;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BuildTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxHealth;

    FMingBuildingCost()
        : BuildingType(EMingBuildingType::CommandCenter)
        , BuildTime(10.0f)
        , MaxHealth(1000)
    {}
};

/**
 * Building Actor class for MingGoRTS
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API AMingBuildingActor : public AActor
{
    GENERATED_BODY()

public:
    AMingBuildingActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EMingBuildingType BuildingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString BuildingName;

    UPROPERTY(BlueprintReadOnly, Category = "Building")
    EMingBuildingState BuildingState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
    FMingBuildingCost BuildingCost;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    int32 CurrentHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    int32 MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Construction")
    float ConstructionProgress;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Construction")
    float ConstructionSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Construction")
    bool bIsUnderConstruction;

    UPROPERTY(BlueprintReadOnly, Category = "Construction")
    int32 AssignedWorkers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Construction")
    int32 MaxWorkers;

    UPROPERTY(BlueprintReadOnly, Category = "Functionality")
    bool bIsOperational;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
    bool bProvidesSupply;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
    int32 SupplyCapacity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UStaticMeshComponent> BuildingMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UBoxComponent> ConstructionZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UMingResourceProducer> ResourceProducer;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void StartConstruction();

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void UpdateConstruction(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void CompleteConstruction();

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void CancelConstruction();

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void AssignWorker();

    UFUNCTION(BlueprintCallable, Category = "Construction")
    void RemoveWorker();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(int32 DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Repair(int32 RepairAmount);

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercentage() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void DestroyBuilding();

    UFUNCTION(BlueprintPure, Category = "Utility")
    bool IsConstructing() const { return bIsUnderConstruction; }

    UFUNCTION(BlueprintPure, Category = "Utility")
    bool IsActive() const { return BuildingState == EMingBuildingState::Active; }

    UFUNCTION(BlueprintCallable, Category = "Production")
    void EnableResourceProduction();

    UFUNCTION(BlueprintCallable, Category = "Production")
    void DisableResourceProduction();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionStarted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConstructionProgress, float, Progress);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingDamaged, int32, CurrentHealth, int32, MaxHealth);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingDestroyed);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingActivated);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnConstructionStarted OnConstructionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnConstructionProgress OnConstructionProgress;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnConstructionCompleted OnConstructionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingDamaged OnBuildingDamaged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingDestroyed OnBuildingDestroyed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingActivated OnBuildingActivated;

    UFUNCTION(BlueprintPure, Category = "Costs", meta = (StaticFunction = "true"))
    static FMingBuildingCost GetBuildingCost(EMingBuildingType Type);

    UFUNCTION(BlueprintPure, Category = "Costs")
    static bool CanAffordBuilding(EMingBuildingType Type, const TMap<EMingResourceType, int32>& AvailableResources);

    // 建築升級相關方法 | Building Upgrade Methods
    UFUNCTION(BlueprintCallable, Category = "Upgrade")
    void StartUpgrade(const FString& UpgradeID);

    UFUNCTION(BlueprintCallable, Category = "Upgrade")
    void CancelUpgrade();

    UFUNCTION(BlueprintPure, Category = "Upgrade")
    bool IsUpgrading() const { return bIsBeingUpgraded; }

    UFUNCTION(BlueprintPure, Category = "Upgrade")
    float GetUpgradeProgress() const { return UpgradeProgress; }

    UFUNCTION(BlueprintCallable, Category = "Upgrade")
    void ApplyUpgrade(const FMingBuildingUpgrade& UpgradeData);

    UFUNCTION(BlueprintPure, Category = "Upgrade")
    int32 GetCurrentUpgradeLevel() const { return CurrentUpgradeLevel; }

    UFUNCTION(BlueprintCallable, Category = "Upgrade")
    void SetUpgradeLevel(int32 NewLevel);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingUpgradeStarted, const FString&, UpgradeID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingUpgradeProgress, float, Progress);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingUpgradeCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingUpgradeCancelled);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgradeStarted OnBuildingUpgradeStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgradeProgress OnBuildingUpgradeProgress;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgradeCompleted OnBuildingUpgradeCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgradeCancelled OnBuildingUpgradeCancelled;

protected:
    UPROPERTY()
    TWeakObjectPtr<class UMingResourceSystem> ResourceSystem;

    void UpdateVisualState();
    void OnConstructionComplete_Internal();
    void OnDestroyed_Internal();

    TMap<EMingResourceType, int32> AccumulatedResources;

    // 升級相關屬性 | Upgrade Properties
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade")
    bool bIsBeingUpgraded;

    UPROPERTY(BlueprintReadOnly, Category = "Upgrade")
    float UpgradeProgress;

    UPROPERTY(BlueprintReadOnly, Category = "Upgrade")
    int32 CurrentUpgradeLevel;

    UPROPERTY()
    FString CurrentUpgradeID;

    UPROPERTY()
    float UpgradeTimeRemaining;

    void UpdateUpgrade(float DeltaTime);
    void CompleteUpgrade();
    void OnUpgradeComplete_Internal();
};
