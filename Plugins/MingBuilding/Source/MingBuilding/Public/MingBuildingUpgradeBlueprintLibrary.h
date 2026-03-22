#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingActor.h"
#include "MingTechTree.h"
#include "MingBuildingUpgradeBlueprintLibrary.generated.h"

/**
 * å»ºç??‡ç? Blueprint ?½æ•¸åº?| Building Upgrade Blueprint Library
 * ??Blueprint ?ä?å»ºç??‡ç??¸é??„å??½æ¥?? */
UCLASS(ClassGroup = (MingBuilding), Blueprintable)
class MINGBUILDING_API UMingBuildingUpgradeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?²å?å»ºç??¶å??‡ç?ç­‰ç? | Get Building Current Upgrade Level
     * @param Building ?®æ?å»ºç? | Target building
     * @return ?¶å??‡ç?ç­‰ç? | Current upgrade level
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetBuildingUpgradeLevel(AMingBuildingActor* Building);

    /**
     * æª¢æŸ¥å»ºç??¯å¦æ­?œ¨?‡ç? | Check if Building is Upgrading
     * @param Building ?®æ?å»ºç? | Target building
     * @return ?¯å¦æ­?œ¨?‡ç? | Is upgrading
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool IsBuildingUpgrading(AMingBuildingActor* Building);

    /**
     * ?²å?å»ºç??‡ç??²åº¦ | Get Building Upgrade Progress
     * @param Building ?®æ?å»ºç? | Target building
     * @return ?‡ç??²åº¦ (0-1) | Upgrade progress
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetBuildingUpgradeProgress(AMingBuildingActor* Building);

    /**
     * ?‹å?å»ºç??‡ç? | Start Building Upgrade
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeID ?‡ç?ID | Upgrade ID
     * @return ?¯å¦?å??‹å? | Success
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool StartBuildingUpgrade(AMingBuildingActor* Building, const FString& UpgradeID);

    /**
     * ?–æ?å»ºç??‡ç? | Cancel Building Upgrade
     * @param Building ?®æ?å»ºç? | Target building
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void CancelBuildingUpgrade(AMingBuildingActor* Building);

    /**
     * ?²å?å»ºç??¯ç”¨?‡ç??—è¡¨ | Get Available Upgrades for Building
     * @param BuildingType å»ºç?é¡å? | Building type
     * @param TechTree ç§‘æ?æ¨¹ç³»çµ?| Tech tree system
     * @return ?¯ç”¨?‡ç??—è¡¨ | Available upgrades
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingBuildingUpgrade> GetAvailableUpgrades(EMingBuildingType BuildingType, UMingTechTree* TechTree);

    /**
     * æª¢æŸ¥?¯å¦?¯ä»¥?‡ç?å»ºç? | Check if Can Upgrade Building
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeID ?‡ç?ID | Upgrade ID
     * @param TechTree ç§‘æ?æ¨¹ç³»çµ?| Tech tree system
     * @param AvailableResources ?¯ç”¨è³‡æ? | Available resources
     * @return ?¯å¦?¯ä»¥?‡ç? | Can upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool CanUpgradeBuilding(
        AMingBuildingActor* Building, 
        const FString& UpgradeID, 
        UMingTechTree* TechTree,
        const TMap<EMingResourceType, int32>& AvailableResources);

    /**
     * ?²å??‡ç??æœ¬ | Get Upgrade Cost
     * @param UpgradeID ?‡ç?ID | Upgrade ID
     * @param TechTree ç§‘æ?æ¨¹ç³»çµ?| Tech tree system
     * @return è³‡æ??æœ¬ | Resource costs
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingResourceCost> GetUpgradeCost(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * ?²å??‡ç??€?€?‚é? | Get Upgrade Time
     * @param UpgradeID ?‡ç?ID | Upgrade ID
     * @param TechTree ç§‘æ?æ¨¹ç³»çµ?| Tech tree system
     * @return ?€?€?‚é?(ç§? | Time in seconds
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static float GetUpgradeTime(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * ?‰ç”¨?‡ç??ˆæ??°å»ºç¯?| Apply Upgrade Effects to Building
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeData ?‡ç??¸æ? | Upgrade data
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void ApplyUpgradeToBuilding(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?²å?å»ºç??‡ç?å¾Œç??Ÿå‘½??| Get Building Health After Upgrade
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeData ?‡ç??¸æ? | Upgrade data
     * @return ?‡ç?å¾Œç??½å€?| Health after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetUpgradedHealth(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?²å?å»ºç??‡ç?å¾Œç??Ÿç”¢?ˆç? | Get Building Production After Upgrade
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeData ?‡ç??¸æ? | Upgrade data
     * @return ?‡ç?å¾Œç??¢æ???| Production after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedProduction(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * ?²å?å»ºç??‡ç?å¾Œç??²ç¦¦??| Get Building Defense After Upgrade
     * @param Building ?®æ?å»ºç? | Target building
     * @param UpgradeData ?‡ç??¸æ? | Upgrade data
     * @return ?‡ç?å¾Œé˜²ç¦¦å€?| Defense after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedDefense(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);
};
