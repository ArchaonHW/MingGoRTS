#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingActor.h"
#include "MingBuildingManager.generated.h"

/**
 * Âª∫Á??æÁΩÆÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FBuildingPlacementResult
{
    GENERATED_BODY()

    // ?ØÂê¶?êÂ?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ?ØË™§‰ø°ÊÅØ
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // ?æÁΩÆ‰ΩçÁΩÆ
    UPROPERTY(BlueprintReadOnly)
    FVector PlacementLocation;

    // Âª∫Á?ID
    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    FBuildingPlacementResult()
        : bSuccess(false)
        , PlacementLocation(FVector::ZeroVector)
        , BuildingID(TEXT(""))
    {}
};

/**
 * Âª∫Á??äÂ??ÖÁõÆ
 */
USTRUCT(BlueprintType)
struct FBuildingQueueItem
{
    GENERATED_BODY()

    // Âª∫Á?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EMingBuildingType BuildingType;

    // Âª∫Á?‰ΩçÁΩÆ
    UPROPERTY(BlueprintReadOnly)
    FVector Position;

    // Âª∫Á??ãË?
    UPROPERTY(BlueprintReadOnly)
    FRotator Rotation;

    // ?äÂ?ID
    UPROPERTY(BlueprintReadOnly)
    FString QueueID;

    // ?êË?ÂÆåÊ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float EstimatedCompletionTime;

    // ?∂Â??≤Â∫¶
    UPROPERTY(BlueprintReadOnly)
    float Progress;

    // ?ØÂê¶Ê≠?ú®Âª∫ÈÄ?    UPROPERTY(BlueprintReadOnly)
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
 * Âª∫Á?Áµ±Ë?‰ø°ÊÅØ
 */
USTRUCT(BlueprintType)
struct FBuildingStatistics
{
    GENERATED_BODY()

    // Á∏ΩÂª∫ÁØâÊï∏??    UPROPERTY(BlueprintReadOnly)
    int32 TotalBuildings;

    // Ê¥ªË?Âª∫Á??∏È?
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveBuildings;

    // Âª∫ÈÄ†‰∏≠Âª∫Á??∏È?
    UPROPERTY(BlueprintReadOnly)
    int32 ConstructingBuildings;

    // ?çÂ?Âª∫Á??∏È?
    UPROPERTY(BlueprintReadOnly)
    int32 DamagedBuildings;

    // ?ÑÈ??ãÂª∫ÁØâÊï∏??    UPROPERTY(BlueprintReadOnly)
    TMap<EMingBuildingType, int32> BuildingCounts;

    // Á∏ΩÂ∑•‰∫∫Êï∏??    UPROPERTY(BlueprintReadOnly)
    int32 TotalWorkers;

    // ?íÁΩÆÂ∑•‰∫∫?∏È?
    UPROPERTY(BlueprintReadOnly)
    int32 AvailableWorkers;

    // Âª∫ÈÄ†È??óÈï∑Â∫?    UPROPERTY(BlueprintReadOnly)
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
 * Âª∫Á?Â±§ÁÆ°?ÜÂô®
 * ÁÆ°Á??∫Âú∞Âª∫Ë®≠?åË?Ê∫êÁ??? */
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

    // === Âª∫Á??æÁΩÆ?åÂª∫??===
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

    // === Âª∫Á?ÁÆ°Á? ===
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

    // === Âª∫Á??æÁΩÆÈ©óË? ===
    UFUNCTION(BlueprintPure, Category = "Building")
    bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    FVector GetBuildingGridSize(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintCallable, Category = "Building")
    void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void HidePlacementPreview();

    // === Â∑•‰∫∫ÁÆ°Á? ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool AssignWorker(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RemoveWorker(const FString& BuildingID);

    UFUNCTION(BlueprintPure, Category = "Building")
    int32 GetAvailableWorkers() const;

    // === Âª∫Á??áÁ??åÁ∂≠Ë≠?===
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool UpgradeBuilding_Impl(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool DestroyBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool RepairBuilding(const FString& BuildingID, int32 RepairAmount);

    // === Âª∫Á??•Ë©¢ ===
    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius) const;

    UFUNCTION(BlueprintPure, Category = "Building")
    TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID) const;

    // === ?äÁ?‰ªãÈù¢ (?ºÂÆπ?? ===
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConstructBuilding(const FString& BuildingType, const FVector& Position);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void UpgradeBuilding_Impl(int32 BuildingId);

    // === Ë≥áÊ?ÁÆ°Á? ===
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void AddResource(const FString& ResourceType, int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Resource")
    int32 GetResourceAmount(const FString& ResourceType) const;

protected:
    void OnBuildingConstructed(const FString& BuildingType);
    void OnResourceUpdated(const FString& ResourceType, int32 NewAmount);

    // === ?∞Â??ÑÂÖß?®ÊñπÊ≥?===
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
    
    // === ?äÁ?Ë≥áÊ?Â≠òÂÑ≤ (?ºÂÆπ?? ===
    TMap<FString, int32> ResourceInventory;

    // === ?∞Â??ÑÂª∫ÁØâÁÆ°?ÜÊï∏??===
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
