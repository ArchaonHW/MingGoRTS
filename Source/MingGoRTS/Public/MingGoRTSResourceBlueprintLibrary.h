#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSResourceBlueprintLibrary.generated.h"

UCLASS()
class MINGRTS_API UMingGoRTSResourceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // 摧毀��??��??���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceTypeDisplayName(EResourceType ResourceType);

    // 摧毀��??�}??�����??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceRarityDisplayName(EResourceRarity Rarity);

    // 摧毀��??��??���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceQualityDisplayName(EResourceQuality Quality);

    // 整理背包物品���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetCollectionMethodDisplayName(ECollectionMethod Method);

    // 目標數量??��?X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> GetAllResourceTypes();

    // 目標數量??��?X?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceRarity> GetAllResourceRarities();

    // 目標數量??����X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceQuality> GetAllResourceQualities();

    // 目標數量��??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<ECollectionMethod> GetAllCollectionMethods();

    // 目標數量����??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceQuantity(float Quantity, EResourceType ResourceType);

    // 目標數量����??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceValue(float Value);

    // �ˬd��?X?�_目標數量
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourcePerishable(EResourceType ResourceType);

    // �ˬd��?X?�_???X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourceStackable(EResourceType ResourceType);

    // 摧毀��?X???��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceIconPath(EResourceType ResourceType);

    // 摧毀��?X?�z
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceDescription(EResourceType ResourceType);

    // �p??��??�`��X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateResourceTotalValue(EResourceType ResourceType, float Quantity, EResourceQuality Quality);

    // �ˬd��?X?�eX
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool AreResourcesCompatible(EResourceType ResourceType1, EResourceType ResourceType2);

    // ???X?��??��??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static ECollectionMethod GetRecommendedCollectionMethod(EResourceType ResourceType);

    // 摧毀��?X??X???��ĳ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType);

    // 摧毀��??�s�x��ĳ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType);

    // ??�ظ�?X???��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateResourceCollectionTask(UMingGoRTSResourceSystem* ResourceSystem, EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID);

    // 整理背包物品
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StartAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, const FString& CollectorID);

    // ??��目標數量
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StopAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 故事重要性?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionProgress(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // �ˬd???X?�_��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsCollectionComplete(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 整理背包物品
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // 整理背包物品
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ImproveCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, float ImprovementAmount);

    // ??�ظ�??��?X?��
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateConversionRecipe(UMingGoRTSResourceSystem* ResourceSystem, const FString& RecipeName, const TMap<EResourceType, float>& InputResources, const TMap<EResourceType, float>& OutputResources);

    // �ˬd��?X?�_??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsConversionAvailable(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 摧毀��?X?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionCost(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 摧毀��?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionTime(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // 摧毀�s�x��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BatchStoreResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TMap<EResourceType, float> BatchRetrieveResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // 摧毀�s�x�e??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static void GetStorageCapacityInfo(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, float& MaxCapacity, float& UsedCapacity, float& AvailableCapacity);

    // �ˬd�s�x??�_�w��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageFull(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // �ˬd�s�x??�_??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageEmpty(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 摧毀�s�x��?X摧毀???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, FResourceData> GetAllStoredResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // �p??�s�x�`��X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateStorageTotalValue(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // 摧毀�s�x��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool OptimizeStorageSpace(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ??�ظ�?X???�p??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool CreateResourceAllocationPlan(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName, const TMap<EResourceType, float>& AllocationTargets);

    // 摧毀��?X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ExecuteResourceAllocation(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // 摧毀��?X??X?X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceAllocationStatus(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // 摧毀��?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GenerateResourceReport(UMingGoRTSResourceSystem* ResourceSystem);

    // 摧毀��??��??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, float> GetResourceStatistics(UMingGoRTSResourceSystem* ResourceSystem);

    // 摧毀��??�Ͷ�
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TArray<FString> AnalyzeResourceTrends(UMingGoRTSResourceSystem* ResourceSystem);

    // ??����?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> PredictResourceNeeds(UMingGoRTSResourceSystem* ResourceSystem);

    // 摧毀��??��ĳ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceRecommendations(UMingGoRTSResourceSystem* ResourceSystem);

    // ��??��??�t�Χ���X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool ValidateResourceSystemIntegrity(UMingGoRTSResourceSystem* ResourceSystem);

    // �״_��??�t��
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RepairResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // ??�m��??�t��
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ResetResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // ??����?X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BackupResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);

    // ??�_��?X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RestoreResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);
);

