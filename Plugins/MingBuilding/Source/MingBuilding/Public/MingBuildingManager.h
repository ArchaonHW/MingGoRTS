#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingActor.h"
#include "MingBuildingManager.generated.h"

/**
 * 建�X�置結�?
 */
USTRUCT(BlueprintType)
struct FBuildingPlacementResult
{
    GENERATED_BODY()

    // ?�否?��?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ?�誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // ?�置位置
    UPROPERTY(BlueprintReadOnly)
    FVector PlacementLocation;

    // 建�?ID
    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    FBuildingPlacementResult()
        : bSuccess(false)
        , PlacementLocation(FVector::ZeroVector)
        , BuildingID(TEXT(""))
    {}
};

/**
 * 建�X��X�目
 */
USTRUCT(BlueprintType)
struct FBuildingQueueItem
{
    GENERATED_BODY()

    // 建�?類�?
    UPROPERTY(BlueprintReadOnly)
    EMingBuildingType BuildingType;

    // 建�?位置
    UPROPERTY(BlueprintReadOnly)
    FVector Position;

    // 建�X��?
    UPROPERTY(BlueprintReadOnly)
    FRotator Rotation;

    // ?��?ID
    UPROPERTY(BlueprintReadOnly)
    FString QueueID;

    // ?��?完�X��?
    UPROPERTY(BlueprintReadOnly)
    float EstimatedCompletionTime;

    // ?��X�度
    UPROPERTY(BlueprintReadOnly)
    float Progress;

    // ?�否�?��建�?    UPROPERTY(BlueprintReadOnly)
    bool bIsConstructing;

    FBuildingQueueItem()
        : BuildingType(EMingBuildingType::CommandCenter)
        , Position(FVector::ZeroVector)
        , Rotation(FRotator::ZeroRotator)
        , EstimatedCompletionTime(0.0f)
        , Progress(0.0f)
        , bIsConstructing(false)
    {}
};

/**
 * 建�?統�?信息
 */
USTRUCT(BlueprintType)
struct FBuildingStatistics
{
    GENERATED_BODY()

    // 總建築數X    UPROPERTY(BlueprintReadOnly)
    int32 TotalBuildings;

    // 活�?建�X��?
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveBuildings;

    // 建造中建�X��?
    UPROPERTY(BlueprintReadOnly)
    int32 ConstructingBuildings;

    // ?��?建�X��?
    UPROPERTY(BlueprintReadOnly)
    int32 DamagedBuildings;

    // ?��X�建築數X    UPROPERTY(BlueprintReadOnly)
    TMap<EMingBuildingType, int32> BuildingCounts;

    // 總工人數X    UPROPERTY(BlueprintReadOnly)
    int32 TotalWorkers;

    // ?�置工人?��?
    UPROPERTY(BlueprintReadOnly)
    int32 AvailableWorkers;

    // 建造�X�長�?    UPROPERTY(BlueprintReadOnly)
    int32 QueueLength;

    FBuildingStatistics()
        : TotalBuildings(0)
        , ActiveBuildings(0)
        , ConstructingBuildings(0)
        , DamagedBuildings(0)
        , TotalWorkers(0)
        , AvailableWorkers(0)
        , QueueLength(0)
    {}
};

/**
 * 建�?層管?�器
 * 管�X�地建設?��?源�X */
UCLASS(BlueprintType)
class MINGBUILDING_API UMingBuildingManager : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingManager();

    virtual void Initialize();
    virtual void Shutdown();

    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

    // === 建�X�置?�建X===
    UFUNCTION(BlueprintCallable, Category = "Building")
    FBuildingPlacementResult PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool StartConstruction(const FString& BuildingID};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool CancelConstruction(const FString& BuildingID};

    UFUNCTION(BlueprintCallable, Category = "Building")
    FString AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RemoveFromConstructionQueue(const FString& QueueID};

    // === 建�?管�? ===
    UFUNCTION(BlueprintPure, Category = "Building")
    AMingBuildingActor* GetBuilding(const FString& BuildingID) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetAllBuildings() const;

    UFUNCTION(BlueprintCallable, Category = "Building")
    TArray<AMingBuildingActor*> GetBuildingsByType(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<FBuildingQueueItem> GetConstructionQueue() const;

    UFUNCTION(BlueprintPure, Category = "Building")
    FBuildingStatistics GetBuildingStatistics() const;

    // === 建�X�置驗�? ===
    UFUNCTION(BlueprintPure, Category = "Building")
    bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    FVector GetBuildingGridSize(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintCallable, Category = "Building")
    void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation};

    UFUNCTION(BlueprintCallable, Category = "Building")
    void HidePlacementPreview();

    // === 工人管�? ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool AssignWorker(const FString& BuildingID};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RemoveWorker(const FString& BuildingID};

    UFUNCTION(BlueprintPure, Category = "Building")
    int32 GetAvailableWorkers() const;

    // === 建�X��X�維�?===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool UpgradeBuilding_Impl(const FString& BuildingID};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool DestroyBuilding(const FString& BuildingID};

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RepairBuilding(const FString& BuildingID, int32 RepairAmount};

    // === 建�X�詢 ===
    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID) const;

    // === ?��?介面 (?�容X ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConstructBuilding(const FString& BuildingType, const FVector& Position};

    UFUNCTION(BlueprintCallable, Category = "Building")
    void UpgradeBuilding_Impl(int32 BuildingId};

    // === 資�?管�? ===
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void AddResource(const FString& ResourceType, int32 Amount};

    UFUNCTION(BlueprintPure, Category = "Resource")
    int32 GetResourceAmount(const FString& ResourceType) const;

protected:
    void OnBuildingConstructed(const FString& BuildingType};
    void OnResourceUpdated(const FString& ResourceType, int32 NewAmount};

    // === ?��X�內?�方�?===
    void ProcessConstructionQueue(float DeltaTime};
    void StartNextConstruction();
    FString GenerateBuildingID();
    FString GenerateQueueID();
    bool HasEnoughResources(EMingBuildingType BuildingType) const;
    bool ConsumeConstructionResources(EMingBuildingType BuildingType};
    void RefundConstructionResources(EMingBuildingType BuildingType};
    void UpdateBuildingStatistics();
    void OnBuildingConstructionStarted(const FString& BuildingID};
    void OnBuildingConstructionCompleted(const FString& BuildingID};
    void OnBuildingDestroyed(const FString& BuildingID};
    void SaveBuildingData();
    void LoadBuildingData();
    void CleanupPlacementPreview();
    bool CheckBuildingCollision(EMingBuildingType BuildingType, const FVector& Location) const;
    bool IsTerrainSuitable(EMingBuildingType BuildingType, const FVector& Location) const;
    float CalculateBuildingEfficiency(AMingBuildingActor* Building) const;
    void UpdateSupplyChains();
    void ProcessBuildingUpgrades(float DeltaTime};

private:
    bool bIsInitialized;
    
    // === ?��?資�?存儲 (?�容X ===
    TMap<FString, int32> ResourceInventory;

    // === ?��X�建築管?�數X===
    UPROPERTY()
    TArray<TObjectPtr<AMingBuildingActor>> Buildings;

    UPROPERTY()
    TArray<FBuildingQueueItem> ConstructionQueue;

    UPROPERTY()
    TMap<FString, TObjectPtr<AMingBuildingActor>> BuildingIDMap;

    UPROPERTY()
    FString CurrentConstructionID;

    UPROPERTY(EditAnywhere, Category = "Building Manager")
    int32 TotalWorkers;

    UPROPERTY(EditAnywhere, Category = "Building Manager")
    int32 MaxConcurrentConstruction;

    UPROPERTY()
    TObjectPtr<class UStaticMeshComponent> PlacementPreview;
};

