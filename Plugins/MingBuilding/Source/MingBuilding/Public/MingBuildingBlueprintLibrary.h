#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingManager.h"
#include "MingBuildingBlueprintLibrary.generated.h"

/**
 * 建�X��X�數�? * ?��X��X�調?��?建�?系統?�能
 */
UCLASS()
class MINGBUILDING_API UMingBuildingBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��?建�?管�X     */
    UFUNCTION(BlueprintPure, Category = "Building")
    static UMingBuildingManager* GetBuildingManager();

    /**
     * ?��X�建築系�?     */
    UFUNCTION(BlueprintCallable, Category = "Building")
    static void InitializeBuildingSystem();

    /**
     * ?�置建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * ?��?建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool StartConstruction(const FString& BuildingID);

    /**
     * ?��?建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool CancelConstruction(const FString& BuildingID);

    /**
     * 添�X�建?��X     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * 從建?��X�移X     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool RemoveFromConstructionQueue(const FString& QueueID);

    /**
     * ?��?建�?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static AMingBuildingActor* GetBuilding(const FString& BuildingID);

    /**
     * ?��X�?�建�?     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetAllBuildings();

    /**
     * ?��X��?類�X�建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetBuildingsByType(EMingBuildingType BuildingType);

    /**
     * ?��?建造�X     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<FBuildingQueueItem> GetConstructionQueue();

    /**
     * ?��?建�?統�?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static FBuildingStatistics GetBuildingStatistics();

    /**
     * 檢查?�置位置?�否?��?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location};

    /**
     * ?��?建�?網格大�?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static FVector GetBuildingGridSize(EMingBuildingType BuildingType};

    /**
     * 顯示建�X�置?�覽
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation};

    /**
     * ?��?建�X�置?�覽
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void HidePlacementPreview();

    /**
     * ?��?工人?�建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool AssignWorker(const FString& BuildingID};

    /**
     * 從建築移?�工�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool RemoveWorker(const FString& BuildingID};

    /**
     * ?��X�用工人?��?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Workers")
    static int32 GetAvailableWorkers();

    /**
     * ?��?建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool UpgradeBuilding(const FString& BuildingID};

    /**
     * ?��?建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool DestroyBuilding(const FString& BuildingID};

    /**
     * 修復建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool RepairBuilding(const FString& BuildingID, int32 RepairAmount};

    /**
     * ?��?建�?影響範�X��?建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius};

    /**
     * ?��?建�?供�X     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID};

    /**
     * ?��?建�X�本
     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static FMingBuildingCost GetBuildingCost(EMingBuildingType BuildingType};

    /**
     * 檢查?�否?�以建�?     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static bool CanAffordBuilding(EMingBuildingType BuildingType};

    /**
     * ?��?建�X�述
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingDescription(EMingBuildingType BuildingType};

    /**
     * ?��?建�?類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingTypeName(EMingBuildingType BuildingType};

    /**
     * ?��?建�X�?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingStateName(EMingBuildingState BuildingState};

    /**
     * ?��?建造建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<FString> BatchConstructBuildings(const TArray<EMingBuildingType>& BuildingTypes, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations};

    /**
     * ?��X��?建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchUpgradeBuildings(const TArray<FString>& BuildingIDs};

    /**
     * ?��?修復建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchRepairBuildings(const TArray<FString>& BuildingIDs, int32 RepairAmount};

    /**
     * ?��?系統?�X     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static bool IsBuildingSystemInitialized();

    /**
     * ?��?建�?系統?�本
     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static FString GetBuildingSystemVersion();

    /**
     * ?�置建�?系統
     */
    UFUNCTION(BlueprintCallable, Category = "Building|System")
    static void ResetBuildingSystem();

    /**
     * ?��?建�X�置建議
     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FVector GetOptimalPlacementLocation(EMingBuildingType BuildingType, const FVector& ReferencePoint, float SearchRadius};

    /**
     * ?��?建造建�?     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FString AutoConstructBuilding(EMingBuildingType BuildingType, const FVector& ReferencePoint};

    /**
     * ?��?建�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBuildingEfficiency(const FString& BuildingID};

    /**
     * ?��X�地總�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBaseEfficiency();

    /**
     * ?��?建�?建議
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Analysis")
    static TArray<EMingBuildingType> GetBuildingRecommendations();

    /**
     * 設置建�?系統?�數
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Configuration")
    static void SetBuildingSystemParameters(int32 TotalWorkers, int32 MaxConcurrentConstruction};

    /**
     * ?��?建�?系統?�數
     */
    UFUNCTION(BlueprintPure, Category = "Building|Configuration")
    static void GetBuildingSystemParameters(int32& TotalWorkers, int32& MaxConcurrentConstruction};
};

