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
    // ?��?資�?類�?顯示?�稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceTypeDisplayName(EResourceType ResourceType};

    // ?��?資�?稀?�度顯示?�稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceRarityDisplayName(EResourceRarity Rarity};

    // ?��?資�?質�?顯示?�稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceQualityDisplayName(EResourceQuality Quality};

    // ?��X��X��?顯示?�稱
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetCollectionMethodDisplayName(ECollectionMethod Method};

    // ?��X�?��?源�X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> GetAllResourceTypes(};

    // ?��X�?��?源�X�度
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceRarity> GetAllResourceRarities(};

    // ?��X�?��?源質X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceQuality> GetAllResourceQualities(};

    // ?��X�?�收?�方�?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<ECollectionMethod> GetAllCollectionMethods(};

    // ?��X��?源數?�顯�?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceQuantity(float Quantity, EResourceType ResourceType};

    // ?��X��?源價?�顯�?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceValue(float Value};

    // 檢查資�X�否?��X��?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourcePerishable(EResourceType ResourceType};

    // 檢查資�X�否?��X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourceStackable(EResourceType ResourceType};

    // ?��?資�X��?路�?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceIconPath(EResourceType ResourceType};

    // ?��?資�X�述
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceDescription(EResourceType ResourceType};

    // 計�?資�?總價X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateResourceTotalValue(EResourceType ResourceType, float Quantity, EResourceQuality Quality};

    // 檢查資�X�容X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool AreResourcesCompatible(EResourceType ResourceType1, EResourceType ResourceType2};

    // ?��X�薦?�收?�方�?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static ECollectionMethod GetRecommendedCollectionMethod(EResourceType ResourceType};

    // ?��?資�X��X��?建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType};

    // ?��?資�?存儲建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType};

    // ?�建資�X��?任�?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateResourceCollectionTask(UMingGoRTSResourceSystem* ResourceSystem, EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StartAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, const FString& CollectorID};

    // ?�止?��X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StopAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID};

    // ?��X��X�度
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionProgress(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID};

    // 檢查?��X�否完�?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsCollectionComplete(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID};

    // ?��X��X��?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ImproveCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, float ImprovementAmount};

    // ?�建資�?轉�X�方
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateConversionRecipe(UMingGoRTSResourceSystem* ResourceSystem, const FString& RecipeName, const TMap<EResourceType, float>& InputResources, const TMap<EResourceType, float>& OutputResources};

    // 檢查轉�X�否?�用
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsConversionAvailable(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID};

    // ?��?轉�X�本
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionCost(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID};

    // ?��?轉�X��?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionTime(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID};

    // ?��?存儲資�?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BatchStoreResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources};

    // ?��X��?資�?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TMap<EResourceType, float> BatchRetrieveResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources};

    // ?��?存儲容�?資�?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static void GetStorageCapacityInfo(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, float& MaxCapacity, float& UsedCapacity, float& AvailableCapacity};

    // 檢查存儲?�否已滿
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageFull(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID};

    // 檢查存儲?�否?�空
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageEmpty(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID};

    // ?��?存儲中�X�?��?�?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, FResourceData> GetAllStoredResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID};

    // 計�?存儲總價X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateStorageTotalValue(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID};

    // ?��?存儲空�?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool OptimizeStorageSpace(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID};

    // ?�建資�X��?計�?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool CreateResourceAllocationPlan(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName, const TMap<EResourceType, float>& AllocationTargets};

    // ?��?資�X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ExecuteResourceAllocation(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName};

    // ?��?資�X��X�X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceAllocationStatus(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName};

    // ?��?資�X��?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GenerateResourceReport(UMingGoRTSResourceSystem* ResourceSystem};

    // ?��?資�?統�?資�?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, float> GetResourceStatistics(UMingGoRTSResourceSystem* ResourceSystem};

    // ?��?資�?趨勢
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TArray<FString> AnalyzeResourceTrends(UMingGoRTSResourceSystem* ResourceSystem};

    // ?�測資�X��?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> PredictResourceNeeds(UMingGoRTSResourceSystem* ResourceSystem};

    // ?��?資�?建議
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceRecommendations(UMingGoRTSResourceSystem* ResourceSystem};

    // 驗�?資�?系統完整X    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool ValidateResourceSystemIntegrity(UMingGoRTSResourceSystem* ResourceSystem};

    // 修復資�?系統
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RepairResourceSystem(UMingGoRTSResourceSystem* ResourceSystem};

    // ?�置資�?系統
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ResetResourceSystem(UMingGoRTSResourceSystem* ResourceSystem};

    // ?�份資�X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BackupResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath};

    // ?�復資�X��?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RestoreResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath};
};

