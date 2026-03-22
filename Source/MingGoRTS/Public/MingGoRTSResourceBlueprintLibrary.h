#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSResourceBlueprintLibrary.generated.h"

UCLASS()
class MINGGORTS_API UMingGoRTSResourceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // 獲取資源類型顯示名稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceTypeDisplayName(EResourceType ResourceType);

    // 獲取資源稀有度顯示名稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceRarityDisplayName(EResourceRarity Rarity);

    // 獲取資源質量顯示名稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceQualityDisplayName(EResourceQuality Quality);

    // 獲取收集方法顯示名稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetCollectionMethodDisplayName(ECollectionMethod Method);

    // 獲取所有資源類型
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> GetAllResourceTypes();

    // 獲取所有資源稀有度
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceRarity> GetAllResourceRarities();

    // 獲取所有資源質量
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceQuality> GetAllResourceQualities();

    // 獲取所有收集方法
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<ECollectionMethod> GetAllCollectionMethods();

    // 格式化資源數量顯示
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceQuantity(float Quantity, EResourceType ResourceType);

    // 格式化資源價值顯示
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceValue(float Value);

    // 檢查資源是否為易腐爛
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourcePerishable(EResourceType ResourceType);

    // 檢查資源是否可堆疊
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourceStackable(EResourceType ResourceType);

    // 獲取資源圖標路徑
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceIconPath(EResourceType ResourceType);

    // 獲取資源描述
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceDescription(EResourceType ResourceType);

    // 計算資源總價值
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateResourceTotalValue(EResourceType ResourceType, float Quantity, EResourceQuality Quality);

    // 檢查資源兼容性
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool AreResourcesCompatible(EResourceType ResourceType1, EResourceType ResourceType2);

    // 獲取推薦的收集方法
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static ECollectionMethod GetRecommendedCollectionMethod(EResourceType ResourceType);

    // 獲取資源收集地點建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType);

    // 獲取資源存儲建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType);

    // 創建資源收集任務
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateResourceCollectionTask(UMingGoRTSResourceSystem* ResourceSystem, EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID);

    // 啟動自動收集
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StartAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, const FString& CollectorID);

    // 停止自動收集
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StopAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 獲取收集進度
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionProgress(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 檢查收集是否完成
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsCollectionComplete(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 獲取收集效率
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 提升收集效率
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ImproveCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, float ImprovementAmount);

    // 創建資源轉換配方
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateConversionRecipe(UMingGoRTSResourceSystem* ResourceSystem, const FString& RecipeName, const TMap<EResourceType, float>& InputResources, const TMap<EResourceType, float>& OutputResources);

    // 檢查轉換是否可用
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsConversionAvailable(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 獲取轉換成本
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionCost(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 獲取轉換時間
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionTime(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 批量存儲資源
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BatchStoreResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // 批量提取資源
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TMap<EResourceType, float> BatchRetrieveResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // 獲取存儲容量資訊
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static void GetStorageCapacityInfo(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, float& MaxCapacity, float& UsedCapacity, float& AvailableCapacity);

    // 檢查存儲是否已滿
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageFull(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 檢查存儲是否為空
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageEmpty(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 獲取存儲中的所有資源
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, FResourceData> GetAllStoredResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 計算存儲總價值
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateStorageTotalValue(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 優化存儲空間
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool OptimizeStorageSpace(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 創建資源分配計劃
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool CreateResourceAllocationPlan(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName, const TMap<EResourceType, float>& AllocationTargets);

    // 執行資源分配
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ExecuteResourceAllocation(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // 獲取資源分配狀態
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceAllocationStatus(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // 生成資源報告
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GenerateResourceReport(UMingGoRTSResourceSystem* ResourceSystem);

    // 獲取資源統計資料
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, float> GetResourceStatistics(UMingGoRTSResourceSystem* ResourceSystem);

    // 分析資源趨勢
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TArray<FString> AnalyzeResourceTrends(UMingGoRTSResourceSystem* ResourceSystem);

    // 預測資源需求
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> PredictResourceNeeds(UMingGoRTSResourceSystem* ResourceSystem);

    // 獲取資源建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceRecommendations(UMingGoRTSResourceSystem* ResourceSystem);

    // 驗證資源系統完整性
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool ValidateResourceSystemIntegrity(UMingGoRTSResourceSystem* ResourceSystem);

    // 修復資源系統
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RepairResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // 重置資源系統
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ResetResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // 備份資源數據
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BackupResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);

    // 恢復資源數據
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RestoreResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);
};
