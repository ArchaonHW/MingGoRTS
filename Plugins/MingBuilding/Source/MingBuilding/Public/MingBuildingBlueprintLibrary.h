#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingManager.h"
#include "MingBuildingBlueprintLibrary.generated.h"

/**
 * 建築藍圖函數庫
 * 提供藍圖可調用的建築系統功能
 */
UCLASS()
class MINGBUILDING_API UMingBuildingBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取建築管理器
     */
    UFUNCTION(BlueprintPure, Category = "Building")
    static UMingBuildingManager* GetBuildingManager();

    /**
     * 初始化建築系統
     */
    UFUNCTION(BlueprintCallable, Category = "Building")
    static void InitializeBuildingSystem();

    /**
     * 放置建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * 開始建造
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool StartConstruction(const FString& BuildingID);

    /**
     * 取消建造
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool CancelConstruction(const FString& BuildingID);

    /**
     * 添加到建造隊列
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * 從建造隊列移除
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool RemoveFromConstructionQueue(const FString& QueueID);

    /**
     * 獲取建築
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static AMingBuildingActor* GetBuilding(const FString& BuildingID);

    /**
     * 獲取所有建築
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetAllBuildings();

    /**
     * 獲取指定類型的建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetBuildingsByType(EMingBuildingType BuildingType);

    /**
     * 獲取建造隊列
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<FBuildingQueueItem> GetConstructionQueue();

    /**
     * 獲取建築統計
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static FBuildingStatistics GetBuildingStatistics();

    /**
     * 檢查放置位置是否有效
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location);

    /**
     * 獲取建築網格大小
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static FVector GetBuildingGridSize(EMingBuildingType BuildingType);

    /**
     * 顯示建築放置預覽
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * 隱藏建築放置預覽
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void HidePlacementPreview();

    /**
     * 分配工人到建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool AssignWorker(const FString& BuildingID);

    /**
     * 從建築移除工人
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool RemoveWorker(const FString& BuildingID);

    /**
     * 獲取可用工人數量
     */
    UFUNCTION(BlueprintPure, Category = "Building|Workers")
    static int32 GetAvailableWorkers();

    /**
     * 升級建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool UpgradeBuilding(const FString& BuildingID);

    /**
     * 摧毀建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool DestroyBuilding(const FString& BuildingID);

    /**
     * 修復建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool RepairBuilding(const FString& BuildingID, int32 RepairAmount);

    /**
     * 獲取建築影響範圍內的建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius);

    /**
     * 獲取建築供應鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID);

    /**
     * 獲取建築成本
     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static FMingBuildingCost GetBuildingCost(EMingBuildingType BuildingType);

    /**
     * 檢查是否可以建造
     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static bool CanAffordBuilding(EMingBuildingType BuildingType);

    /**
     * 獲取建築描述
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingDescription(EMingBuildingType BuildingType);

    /**
     * 獲取建築類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingTypeName(EMingBuildingType BuildingType);

    /**
     * 獲取建築狀態名稱
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingStateName(EMingBuildingState BuildingState);

    /**
     * 批量建造建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<FString> BatchConstructBuildings(const TArray<EMingBuildingType>& BuildingTypes, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations);

    /**
     * 批量升級建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchUpgradeBuildings(const TArray<FString>& BuildingIDs);

    /**
     * 批量修復建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchRepairBuildings(const TArray<FString>& BuildingIDs, int32 RepairAmount);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static bool IsBuildingSystemInitialized();

    /**
     * 獲取建築系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static FString GetBuildingSystemVersion();

    /**
     * 重置建築系統
     */
    UFUNCTION(BlueprintCallable, Category = "Building|System")
    static void ResetBuildingSystem();

    /**
     * 獲取建築放置建議
     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FVector GetOptimalPlacementLocation(EMingBuildingType BuildingType, const FVector& ReferencePoint, float SearchRadius);

    /**
     * 自動建造建築
     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FString AutoConstructBuilding(EMingBuildingType BuildingType, const FVector& ReferencePoint);

    /**
     * 獲取建築效率
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBuildingEfficiency(const FString& BuildingID);

    /**
     * 獲取基地總體效率
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBaseEfficiency();

    /**
     * 獲取建築建議
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Analysis")
    static TArray<EMingBuildingType> GetBuildingRecommendations();

    /**
     * 設置建築系統參數
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Configuration")
    static void SetBuildingSystemParameters(int32 TotalWorkers, int32 MaxConcurrentConstruction);

    /**
     * 獲取建築系統參數
     */
    UFUNCTION(BlueprintPure, Category = "Building|Configuration")
    static void GetBuildingSystemParameters(int32& TotalWorkers, int32& MaxConcurrentConstruction);
};
