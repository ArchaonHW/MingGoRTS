#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingBuildingActor.h"
#include "MingTechTree.h"
#include "MingBuildingUpgradeBlueprintLibrary.generated.h"

/**
 * 建築升級 Blueprint 函數庫 | Building Upgrade Blueprint Library
 * 為 Blueprint 提供建築升級相關的功能接口
 */
UCLASS(ClassGroup = (MingBuilding), Blueprintable)
class MINGBUILDING_API UMingBuildingUpgradeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取建築當前升級等級 | Get Building Current Upgrade Level
     * @param Building 目標建築 | Target building
     * @return 當前升級等級 | Current upgrade level
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetBuildingUpgradeLevel(AMingBuildingActor* Building);

    /**
     * 檢查建築是否正在升級 | Check if Building is Upgrading
     * @param Building 目標建築 | Target building
     * @return 是否正在升級 | Is upgrading
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool IsBuildingUpgrading(AMingBuildingActor* Building);

    /**
     * 獲取建築升級進度 | Get Building Upgrade Progress
     * @param Building 目標建築 | Target building
     * @return 升級進度 (0-1) | Upgrade progress
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetBuildingUpgradeProgress(AMingBuildingActor* Building);

    /**
     * 開始建築升級 | Start Building Upgrade
     * @param Building 目標建築 | Target building
     * @param UpgradeID 升級ID | Upgrade ID
     * @return 是否成功開始 | Success
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool StartBuildingUpgrade(AMingBuildingActor* Building, const FString& UpgradeID);

    /**
     * 取消建築升級 | Cancel Building Upgrade
     * @param Building 目標建築 | Target building
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void CancelBuildingUpgrade(AMingBuildingActor* Building);

    /**
     * 獲取建築可用升級列表 | Get Available Upgrades for Building
     * @param BuildingType 建築類型 | Building type
     * @param TechTree 科技樹系統 | Tech tree system
     * @return 可用升級列表 | Available upgrades
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingBuildingUpgrade> GetAvailableUpgrades(EMingBuildingType BuildingType, UMingTechTree* TechTree);

    /**
     * 檢查是否可以升級建築 | Check if Can Upgrade Building
     * @param Building 目標建築 | Target building
     * @param UpgradeID 升級ID | Upgrade ID
     * @param TechTree 科技樹系統 | Tech tree system
     * @param AvailableResources 可用資源 | Available resources
     * @return 是否可以升級 | Can upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static bool CanUpgradeBuilding(
        AMingBuildingActor* Building, 
        const FString& UpgradeID, 
        UMingTechTree* TechTree,
        const TMap<EMingResourceType, int32>& AvailableResources);

    /**
     * 獲取升級成本 | Get Upgrade Cost
     * @param UpgradeID 升級ID | Upgrade ID
     * @param TechTree 科技樹系統 | Tech tree system
     * @return 資源成本 | Resource costs
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static TArray<FMingResourceCost> GetUpgradeCost(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * 獲取升級所需時間 | Get Upgrade Time
     * @param UpgradeID 升級ID | Upgrade ID
     * @param TechTree 科技樹系統 | Tech tree system
     * @return 所需時間(秒) | Time in seconds
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade")
    static float GetUpgradeTime(const FString& UpgradeID, UMingTechTree* TechTree);

    /**
     * 應用升級效果到建築 | Apply Upgrade Effects to Building
     * @param Building 目標建築 | Target building
     * @param UpgradeData 升級數據 | Upgrade data
     */
    UFUNCTION(BlueprintCallable, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static void ApplyUpgradeToBuilding(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * 獲取建築升級後的生命值 | Get Building Health After Upgrade
     * @param Building 目標建築 | Target building
     * @param UpgradeData 升級數據 | Upgrade data
     * @return 升級後生命值 | Health after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static int32 GetUpgradedHealth(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * 獲取建築升級後的生產效率 | Get Building Production After Upgrade
     * @param Building 目標建築 | Target building
     * @param UpgradeData 升級數據 | Upgrade data
     * @return 升級後生產效率 | Production after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedProduction(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);

    /**
     * 獲取建築升級後的防禦值 | Get Building Defense After Upgrade
     * @param Building 目標建築 | Target building
     * @param UpgradeData 升級數據 | Upgrade data
     * @return 升級後防禦值 | Defense after upgrade
     */
    UFUNCTION(BlueprintPure, Category = "Building|Upgrade", meta = (DefaultToSelf = "Building"))
    static float GetUpgradedDefense(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData);
};
