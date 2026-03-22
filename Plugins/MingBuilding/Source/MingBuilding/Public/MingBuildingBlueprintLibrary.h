#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingManager.h"
#include "MingBuildingBlueprintLibrary.generated.h"

/**
 * Âª∫Á??çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á?Âª∫Á?Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGBUILDING_API UMingBuildingBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?Âª∫Á?ÁÆ°Á???     */
    UFUNCTION(BlueprintPure, Category = "Building")
    static UMingBuildingManager* GetBuildingManager();

    /**
     * ?ùÂ??ñÂª∫ÁØâÁ≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Building")
    static void InitializeBuildingSystem();

    /**
     * ?æÁΩÆÂª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * ?ãÂ?Âª∫ÈÄ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool StartConstruction(const FString& BuildingID);

    /**
     * ?ñÊ?Âª∫ÈÄ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool CancelConstruction(const FString& BuildingID);

    /**
     * Ê∑ªÂ??∞Âª∫?†È???     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static FString AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * ÂæûÂª∫?†È??óÁßª??     */
    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    static bool RemoveFromConstructionQueue(const FString& QueueID);

    /**
     * ?≤Â?Âª∫Á?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static AMingBuildingActor* GetBuilding(const FString& BuildingID);

    /**
     * ?≤Â??Ä?âÂª∫ÁØ?     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetAllBuildings();

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÂª∫ÁØ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Management")
    static TArray<AMingBuildingActor*> GetBuildingsByType(EMingBuildingType BuildingType);

    /**
     * ?≤Â?Âª∫ÈÄ†È???     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static TArray<FBuildingQueueItem> GetConstructionQueue();

    /**
     * ?≤Â?Âª∫Á?Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Management")
    static FBuildingStatistics GetBuildingStatistics();

    /**
     * Ê™¢Êü•?æÁΩÆ‰ΩçÁΩÆ?ØÂê¶?âÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static bool IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location);

    /**
     * ?≤Â?Âª∫Á?Á∂≤Ê†ºÂ§ßÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Placement")
    static FVector GetBuildingGridSize(EMingBuildingType BuildingType);

    /**
     * È°ØÁ§∫Âª∫Á??æÁΩÆ?êË¶Ω
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation);

    /**
     * ?±Ë?Âª∫Á??æÁΩÆ?êË¶Ω
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Placement")
    static void HidePlacementPreview();

    /**
     * ?ÜÈ?Â∑•‰∫∫?∞Âª∫ÁØ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool AssignWorker(const FString& BuildingID);

    /**
     * ÂæûÂª∫ÁØâÁßª?§Â∑•‰∫?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Workers")
    static bool RemoveWorker(const FString& BuildingID);

    /**
     * ?≤Â??ØÁî®Â∑•‰∫∫?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Workers")
    static int32 GetAvailableWorkers();

    /**
     * ?áÁ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool UpgradeBuilding(const FString& BuildingID);

    /**
     * ?ßÊ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool DestroyBuilding(const FString& BuildingID);

    /**
     * ‰øÆÂæ©Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Maintenance")
    static bool RepairBuilding(const FString& BuildingID, int32 RepairAmount);

    /**
     * ?≤Â?Âª∫Á?ÂΩ±ÈüøÁØÑÂ??ßÁ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetBuildingsInRadius(const FVector& Center, float Radius);

    /**
     * ?≤Â?Âª∫Á?‰æõÊ???     */
    UFUNCTION(BlueprintCallable, Category = "Building|Query")
    static TArray<AMingBuildingActor*> GetSupplyChain(const FString& BuildingID);

    /**
     * ?≤Â?Âª∫Á??êÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static FMingBuildingCost GetBuildingCost(EMingBuildingType BuildingType);

    /**
     * Ê™¢Êü•?ØÂê¶?Ø‰ª•Âª∫ÈÄ?     */
    UFUNCTION(BlueprintPure, Category = "Building|Costs")
    static bool CanAffordBuilding(EMingBuildingType BuildingType);

    /**
     * ?≤Â?Âª∫Á??èËø∞
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingDescription(EMingBuildingType BuildingType);

    /**
     * ?≤Â?Âª∫Á?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingTypeName(EMingBuildingType BuildingType);

    /**
     * ?≤Â?Âª∫Á??Ä?ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Building|Info")
    static FString GetBuildingStateName(EMingBuildingState BuildingState);

    /**
     * ?πÈ?Âª∫ÈÄ†Âª∫ÁØ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<FString> BatchConstructBuildings(const TArray<EMingBuildingType>& BuildingTypes, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations);

    /**
     * ?πÈ??áÁ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchUpgradeBuildings(const TArray<FString>& BuildingIDs);

    /**
     * ?πÈ?‰øÆÂæ©Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Batch")
    static TArray<bool> BatchRepairBuildings(const TArray<FString>& BuildingIDs, int32 RepairAmount);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static bool IsBuildingSystemInitialized();

    /**
     * ?≤Â?Âª∫Á?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Building|System")
    static FString GetBuildingSystemVersion();

    /**
     * ?çÁΩÆÂª∫Á?Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Building|System")
    static void ResetBuildingSystem();

    /**
     * ?≤Â?Âª∫Á??æÁΩÆÂª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FVector GetOptimalPlacementLocation(EMingBuildingType BuildingType, const FVector& ReferencePoint, float SearchRadius);

    /**
     * ?™Â?Âª∫ÈÄ†Âª∫ÁØ?     */
    UFUNCTION(BlueprintCallable, Category = "Building|AI")
    static FString AutoConstructBuilding(EMingBuildingType BuildingType, const FVector& ReferencePoint);

    /**
     * ?≤Â?Âª∫Á??àÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBuildingEfficiency(const FString& BuildingID);

    /**
     * ?≤Â??∫Âú∞Á∏ΩÈ??àÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Building|Analysis")
    static float GetBaseEfficiency();

    /**
     * ?≤Â?Âª∫Á?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Analysis")
    static TArray<EMingBuildingType> GetBuildingRecommendations();

    /**
     * Ë®≠ÁΩÆÂª∫Á?Á≥ªÁµ±?ÉÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Configuration")
    static void SetBuildingSystemParameters(int32 TotalWorkers, int32 MaxConcurrentConstruction);

    /**
     * ?≤Â?Âª∫Á?Á≥ªÁµ±?ÉÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Building|Configuration")
    static void GetBuildingSystemParameters(int32& TotalWorkers, int32& MaxConcurrentConstruction);
};
