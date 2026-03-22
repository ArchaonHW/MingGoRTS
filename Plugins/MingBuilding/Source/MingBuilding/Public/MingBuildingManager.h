#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingActor.h"
#include "MingBuildingManager.generated.h"

/**
 * 建築放置結果
 */
USTRUCT(BlueprintType)
struct FBuildingPlacementResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 錯誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // 放置位置
    UPROPERTY(BlueprintReadOnly)
    FVector PlacementLocation;

    // 建築ID
    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    FBuildingPlacementResult()
        : bSuccess(false)
        , PlacementLocation(FVector::ZeroVector)
        , BuildingID(TEXT(""))
    {}
};

/**
 * 建築隊列項目
 */
USTRUCT(BlueprintType)
struct FBuildingQueueItem
{
    GENERATED_BODY()

    // 建築類型
    UPROPERTY(BlueprintReadOnly)
    EMingBuildingType BuildingType;

    // 建築位置
    UPROPERTY(BlueprintReadOnly)
    FVector Position;

    // 建築旋轉
    UPROPERTY(BlueprintReadOnly)
    FRotator Rotation;

    // 隊列ID
    UPROPERTY(BlueprintReadOnly)
    FString QueueID;

    // 預計完成時間
    UPROPERTY(BlueprintReadOnly)
    float EstimatedCompletionTime;

    // 當前進度
    UPROPERTY(BlueprintReadOnly)
    float Progress;

    // 是否正在建造
    UPROPERTY(BlueprintReadOnly)
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
 * 建築統計信息
 */
USTRUCT(BlueprintType)
struct FBuildingStatistics
{
    GENERATED_BODY()

    // 總建築數量
    UPROPERTY(BlueprintReadOnly)
    int32 TotalBuildings;

    // 活躍建築數量
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveBuildings;

    // 建造中建築數量
    UPROPERTY(BlueprintReadOnly)
    int32 ConstructingBuildings;

    // 損壞建築數量
    UPROPERTY(BlueprintReadOnly)
    int32 DamagedBuildings;

    // 各類型建築數量
    UPROPERTY(BlueprintReadOnly)
    TMap<EMingBuildingType, int32> BuildingCounts;

    // 總工人數量
    UPROPERTY(BlueprintReadOnly)
    int32 TotalWorkers;

    // 閒置工人數量
    UPROPERTY(BlueprintReadOnly)
    int32 AvailableWorkers;

    // 建造隊列長度
    UPROPERTY(BlueprintReadOnly)
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
 * 建築層管理器
 * 管理基地建設和資源生產
 */
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

    // === 建築放置和建造 ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    FBuildingPlacementResult PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool StartConstruction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool CancelConstruction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    FString AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RemoveFromConstructionQueue(const FString& QueueID);

    // === 建築管理 ===
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

    // === 建築放置驗證 ===
    UFUNCTION(BlueprintPure, Category = "Building")
    bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    FVector GetBuildingGridSize(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintCallable, Category = "Building")
    void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void HidePlacementPreview();

    // === 工人管理 ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool AssignWorker(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RemoveWorker(const FString& BuildingID);

    UFUNCTION(BlueprintPure, Category = "Building")
    int32 GetAvailableWorkers() const;

    // === 建築升級和維護 ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool UpgradeBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool DestroyBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RepairBuilding(const FString& BuildingID, int32 RepairAmount);

    // === 建築查詢 ===
    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID) const;

    // === 舊版介面 (兼容性) ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConstructBuilding(const FString& BuildingType, const FVector& Position);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void UpgradeBuilding(int32 BuildingId);

    // === 資源管理 ===
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void AddResource(const FString& ResourceType, int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Resource")
    int32 GetResourceAmount(const FString& ResourceType) const;

protected:
    void OnBuildingConstructed(const FString& BuildingType);
    void OnResourceUpdated(const FString& ResourceType, int32 NewAmount);

    // === 新增的內部方法 ===
    void ProcessConstructionQueue(float DeltaTime);
    void StartNextConstruction();
    FString GenerateBuildingID();
    FString GenerateQueueID();
    bool HasEnoughResources(EMingBuildingType BuildingType) const;
    bool ConsumeConstructionResources(EMingBuildingType BuildingType);
    void RefundConstructionResources(EMingBuildingType BuildingType);
    void UpdateBuildingStatistics();
    void OnBuildingConstructionStarted(const FString& BuildingID);
    void OnBuildingConstructionCompleted(const FString& BuildingID);
    void OnBuildingDestroyed(const FString& BuildingID);
    void SaveBuildingData();
    void LoadBuildingData();
    void CleanupPlacementPreview();
    bool CheckBuildingCollision(EMingBuildingType BuildingType, const FVector& Location) const;
    bool IsTerrainSuitable(EMingBuildingType BuildingType, const FVector& Location) const;
    float CalculateBuildingEfficiency(AMingBuildingActor* Building) const;
    void UpdateSupplyChains();
    void ProcessBuildingUpgrades(float DeltaTime);

private:
    bool bIsInitialized;
    
    // === 舊版資源存儲 (兼容性) ===
    TMap<FString, int32> ResourceInventory;

    // === 新增的建築管理數據 ===
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
