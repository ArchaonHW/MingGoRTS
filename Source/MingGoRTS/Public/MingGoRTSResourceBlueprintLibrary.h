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
    // ?≤Â?Ë≥áÊ?È°ûÂ?È°ØÁ§∫?çÁ®±
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceTypeDisplayName(EResourceType ResourceType);

    // ?≤Â?Ë≥áÊ?Á®Ä?âÂ∫¶È°ØÁ§∫?çÁ®±
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceRarityDisplayName(EResourceRarity Rarity);

    // ?≤Â?Ë≥áÊ?Ë≥™È?È°ØÁ§∫?çÁ®±
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceQualityDisplayName(EResourceQuality Quality);

    // ?≤Â??∂È??πÊ?È°ØÁ§∫?çÁ®±
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetCollectionMethodDisplayName(ECollectionMethod Method);

    // ?≤Â??Ä?âË?Ê∫êÈ???    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> GetAllResourceTypes();

    // ?≤Â??Ä?âË?Ê∫êÁ??âÂ∫¶
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceRarity> GetAllResourceRarities();

    // ?≤Â??Ä?âË?Ê∫êË≥™??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceQuality> GetAllResourceQualities();

    // ?≤Â??Ä?âÊî∂?ÜÊñπÊ≥?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<ECollectionMethod> GetAllCollectionMethods();

    // ?ºÂ??ñË?Ê∫êÊï∏?èÈ°ØÁ§?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceQuantity(float Quantity, EResourceType ResourceType);

    // ?ºÂ??ñË?Ê∫êÂÉπ?ºÈ°ØÁ§?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString FormatResourceValue(float Value);

    // Ê™¢Êü•Ë≥áÊ??ØÂê¶?∫Ê??êÁ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourcePerishable(EResourceType ResourceType);

    // Ê™¢Êü•Ë≥áÊ??ØÂê¶?ØÂ???    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsResourceStackable(EResourceType ResourceType);

    // ?≤Â?Ë≥áÊ??ñÊ?Ë∑ØÂ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceIconPath(EResourceType ResourceType);

    // ?≤Â?Ë≥áÊ??èËø∞
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceDescription(EResourceType ResourceType);

    // Ë®àÁ?Ë≥áÊ?Á∏ΩÂÉπ??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateResourceTotalValue(EResourceType ResourceType, float Quantity, EResourceQuality Quality);

    // Ê™¢Êü•Ë≥áÊ??ºÂÆπ??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool AreResourcesCompatible(EResourceType ResourceType1, EResourceType ResourceType2);

    // ?≤Â??®Ëñ¶?ÑÊî∂?ÜÊñπÊ≥?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static ECollectionMethod GetRecommendedCollectionMethod(EResourceType ResourceType);

    // ?≤Â?Ë≥áÊ??∂È??∞È?Âª∫Ë≠∞
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType);

    // ?≤Â?Ë≥áÊ?Â≠òÂÑ≤Âª∫Ë≠∞
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType);

    // ?µÂª∫Ë≥áÊ??∂È?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateResourceCollectionTask(UMingGoRTSResourceSystem* ResourceSystem, EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID);

    // ?üÂ??™Â??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StartAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, const FString& CollectorID);

    // ?úÊ≠¢?™Â??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool StopAutoCollection(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ?≤Â??∂È??≤Â∫¶
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionProgress(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // Ê™¢Êü•?∂È??ØÂê¶ÂÆåÊ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsCollectionComplete(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ?≤Â??∂È??àÁ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID);

    // ?êÂ??∂È??àÁ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ImproveCollectionEfficiency(UMingGoRTSResourceSystem* ResourceSystem, const FString& CollectionID, float ImprovementAmount);

    // ?µÂª∫Ë≥áÊ?ËΩâÊ??çÊñπ
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static FString CreateConversionRecipe(UMingGoRTSResourceSystem* ResourceSystem, const FString& RecipeName, const TMap<EResourceType, float>& InputResources, const TMap<EResourceType, float>& OutputResources);

    // Ê™¢Êü•ËΩâÊ??ØÂê¶?ØÁî®
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsConversionAvailable(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ?≤Â?ËΩâÊ??êÊú¨
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionCost(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ?≤Â?ËΩâÊ??ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float GetConversionTime(UMingGoRTSResourceSystem* ResourceSystem, const FString& ConversionID);

    // ?πÈ?Â≠òÂÑ≤Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BatchStoreResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // ?πÈ??êÂ?Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TMap<EResourceType, float> BatchRetrieveResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, const TMap<EResourceType, float>& Resources);

    // ?≤Â?Â≠òÂÑ≤ÂÆπÈ?Ë≥áË?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static void GetStorageCapacityInfo(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID, float& MaxCapacity, float& UsedCapacity, float& AvailableCapacity);

    // Ê™¢Êü•Â≠òÂÑ≤?ØÂê¶Â∑≤Êªø
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageFull(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // Ê™¢Êü•Â≠òÂÑ≤?ØÂê¶?∫Á©∫
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool IsStorageEmpty(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ?≤Â?Â≠òÂÑ≤‰∏≠Á??Ä?âË?Ê∫?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, FResourceData> GetAllStoredResources(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // Ë®àÁ?Â≠òÂÑ≤Á∏ΩÂÉπ??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static float CalculateStorageTotalValue(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ?™Â?Â≠òÂÑ≤Á©∫È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool OptimizeStorageSpace(UMingGoRTSResourceSystem* ResourceSystem, const FString& StorageID);

    // ?µÂª∫Ë≥áÊ??ÜÈ?Ë®àÂ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool CreateResourceAllocationPlan(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName, const TMap<EResourceType, float>& AllocationTargets);

    // ?∑Ë?Ë≥áÊ??ÜÈ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ExecuteResourceAllocation(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // ?≤Â?Ë≥áÊ??ÜÈ??Ä??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GetResourceAllocationStatus(UMingGoRTSResourceSystem* ResourceSystem, const FString& PlanName);

    // ?üÊ?Ë≥áÊ??±Â?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static FString GenerateResourceReport(UMingGoRTSResourceSystem* ResourceSystem);

    // ?≤Â?Ë≥áÊ?Áµ±Ë?Ë≥áÊ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TMap<EResourceType, float> GetResourceStatistics(UMingGoRTSResourceSystem* ResourceSystem);

    // ?ÜÊ?Ë≥áÊ?Ë∂®Âã¢
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static TArray<FString> AnalyzeResourceTrends(UMingGoRTSResourceSystem* ResourceSystem);

    // ?êÊ∏¨Ë≥áÊ??ÄÊ±?    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<EResourceType> PredictResourceNeeds(UMingGoRTSResourceSystem* ResourceSystem);

    // ?≤Â?Ë≥áÊ?Âª∫Ë≠∞
    UFUNCTION(BlueprintPure, Category = "Resource System")
    static TArray<FString> GetResourceRecommendations(UMingGoRTSResourceSystem* ResourceSystem);

    // È©óË?Ë≥áÊ?Á≥ªÁµ±ÂÆåÊï¥??    UFUNCTION(BlueprintPure, Category = "Resource System")
    static bool ValidateResourceSystemIntegrity(UMingGoRTSResourceSystem* ResourceSystem);

    // ‰øÆÂæ©Ë≥áÊ?Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RepairResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // ?çÁΩÆË≥áÊ?Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool ResetResourceSystem(UMingGoRTSResourceSystem* ResourceSystem);

    // ?ô‰ªΩË≥áÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool BackupResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);

    // ?¢Âæ©Ë≥áÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    static bool RestoreResourceData(UMingGoRTSResourceSystem* ResourceSystem, const FString& BackupPath);
};
