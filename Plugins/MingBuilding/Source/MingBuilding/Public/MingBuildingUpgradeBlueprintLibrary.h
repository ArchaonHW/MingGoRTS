#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingActor.h"
#include "MingTechTree.h"
#include "MingBuildingUpgradeBlueprintLibrary.generated.h"

/**
 * 建�X��? Blueprint ?�數�?| Building Upgrade Blueprint Library
 * XBlueprint ?��?建�X��X��X��X�接X */
UCLASS(ClassGroup = (MingBuilding), Blueprintable)
class MINGBUILDING_API UMingBuildingUpgradeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��?建�X��X��?等�? | Get Building Current Upgrade Level
     * @param Building ?��?建�? | Target building
     * @return ?��X��?等�? | Current upgrade level
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetBuildingUpgradeLevel(AMingBuildingActor* Building);

    /**
     * 檢查建�X�否�?��?��? | Check if Building is Upgrading
     * @param Building ?��?建�? | Target building
     * @return ?�否�?��?��? | Is upgrading
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool IsBuildingUpgrading(AMingBuildingActor* Building);

    /**
     * ?��?建�X��X�度 | Get Building Upgrade Progress
     * @param Building ?��?建�? | Target building
     * @return ?��X�度 (0-1) | Upgrade progress
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetBuildingUpgradeProgress(AMingBuildingActor* Building);

    /**
     * ?��?建�X��? | Start Building Upgrade
     * @param Building ?��?建�? | Target building
     * @param UpgradeID ?��?ID | Upgrade ID
     * @return ?�否?��X��? | Success
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool StartBuildingUpgrade(AMingBuildingActor* Building, const FString& UpgradeID);

    /**
     * ?��?建�X��? | Cancel Building Upgrade
     * @param Building ?��?建�? | Target building
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void CancelBuildingUpgrade(AMingBuildingActor* Building);

    /**
     * ?��?建�X�用?��X�表 | Get Available Upgrades for Building
     * @param BuildingType 建�?類�? | Building type
     * @param TechTree 科�?樹系�?| Tech tree system
     * @return ?�用?��X�表 | Available upgrades
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingBuildingUpgrade> GetAvailableUpgrades(EMingBuildingType BuildingType, UMingTechTree* TechTree);

    /**
     * 檢查?�否?�以?��?建�? | Check if Can Upgrade Building
     * @param Building ?��?建�? | Target building
     * @param UpgradeID ?��?ID | Upgrade ID
     * @param TechTree 科�?樹系�?| Tech tree system
     * @param AvailableResources ?�用資�? | Available resources
     * @return ?�否?�以?��? | Can upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool CanUpgradeBuilding(
        AMingBuildingActor* Building, 
        const FString& UpgradeID, 
        UMingTechTree* TechTree,
        const TMap<EMingResourceType, int32>& AvailableResources);

    /**
     * ?��X��X�本 | Get Upgrade Cost
     * @param UpgradeID ?��?ID | Upgrade ID
     * @param TechTree 科�?樹系�?| Tech tree system
     * @return 資�X�本 | Resource costs
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingResourceCost> GetUpgradeCost(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * ?��X��X�?�?��? | Get Upgrade Time
     * @param UpgradeID ?��?ID | Upgrade ID
     * @param TechTree 科�?樹系�?| Tech tree system
     * @return ?�?�?��?(�? | Time in seconds
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static float GetUpgradeTime(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * ?�用?��X��X�建�?| Apply Upgrade Effects to Building
     * @param Building ?��?建�? | Target building
     * @param UpgradeData ?��X��? | Upgrade data
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void ApplyUpgradeToBuilding(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?��?建�X��?後�X�命X| Get Building Health After Upgrade
     * @param Building ?��?建�? | Target building
     * @param UpgradeData ?��X��? | Upgrade data
     * @return ?��?後�X��?| Health after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetUpgradedHealth(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?��?建�X��?後�X�產?��? | Get Building Production After Upgrade
     * @param Building ?��?建�? | Target building
     * @param UpgradeData ?��X��? | Upgrade data
     * @return ?��?後�X��X| Production after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedProduction(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?��?建�X��?後�X�禦X| Get Building Defense After Upgrade
     * @param Building ?��?建�? | Target building
     * @param UpgradeData ?��X��? | Upgrade data
     * @return ?��?後防禦�?| Defense after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedDefense(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);
);

