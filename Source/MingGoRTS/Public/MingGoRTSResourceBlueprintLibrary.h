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
    // ????��??��??���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceTypeDisplayName(EResourceType ResourceType);

    // ????��??�}??�����??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceRarityDisplayName(EResourceRarity Rarity);

    // ????��??��??���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceQualityDisplayName(EResourceQuality Quality);

    // ???X??X???���??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetCollectionMethodDisplayName(ECollectionMethod Method);

    // ???X?????��?X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> GetAllResourceTypes();

    // ???X?????��?X?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceRarity> GetAllResourceRarities();

    // ???X?????����X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceQuality> GetAllResourceQualities();

    // ???X???��??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<ECollectionMethod> GetAllCollectionMethods();

    // ???X???����??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceQuantity(float Quantity, EResourceType ResourceType);

    // ???X???����??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceValue(float Value);

    // �ˬd��?X?�_???X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourcePerishable(EResourceType ResourceType);

    // �ˬd��?X?�_???X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourceStackable(EResourceType ResourceType);

    // ????��?X???��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceIconPath(EResourceType ResourceType);

    // ????��?X?�z
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

    // ????��?X??X???��ĳ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType);

    // ????��??�s�x��ĳ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType);

    // ??�ظ�?X???��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateResourceCollectionTask(UMingGoRTSResourceSystem* ResourceSystem, EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID);

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StartAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, const FString& CollectorID);

    // ??��???X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StopAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ???X??X?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionProgress(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // �ˬd???X?�_��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsCollectionComplete(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ???X??X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ImproveCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, float ImprovementAmount);

    // ??�ظ�??��?X?��
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateConversionRecipe(UMingGoRTSResourceSystem* ResourceSystem, const FString& RecipeName, const TMap<EResourceType, float>& InputResources, const TMap<EResourceType, float>& OutputResources);

    // �ˬd��?X?�_??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsConversionAvailable(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ????��?X?��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionCost(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ????��?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionTime(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ????�s�x��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BatchStoreResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // ???X???��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TMap<EResourceType, float> BatchRetrieveResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // ????�s�x�e??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static void GetStorageCapacityInfo(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, float& MaxCapacity, float& UsedCapacity, float& AvailableCapacity);

    // �ˬd�s�x??�_�w��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageFull(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // �ˬd�s�x??�_??��
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageEmpty(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ????�s�x��?X???????
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, FResourceData> GetAllStoredResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // �p??�s�x�`��X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateStorageTotalValue(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ????�s�x��??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool OptimizeStorageSpace(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ??�ظ�?X???�p??
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool CreateResourceAllocationPlan(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName, const TMap<EResourceType, float>& AllocationTargets);

    // ????��?X???
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ExecuteResourceAllocation(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // ????��?X??X?X
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceAllocationStatus(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // ????��?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GenerateResourceReport(UMingGoRTSResourceSystem* ResourceSystem);

    // ????��??��??��??
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, float> GetResourceStatistics(UMingGoRTSResourceSystem* ResourceSystem);

    // ????��??�Ͷ�
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TArray<FString> AnalyzeResourceTrends(UMingGoRTSResourceSystem* ResourceSystem);

    // ??����?X???
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> PredictResourceNeeds(UMingGoRTSResourceSystem* ResourceSystem);

    // ????��??��ĳ
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

