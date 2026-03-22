#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingTacticalBlueprintLibrary.generated.h"

class UMingLargeScaleCombatTest;

/**
 * 戰術系統藍圖函數庫
 * 提供藍圖可調用的戰術系統功能
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 創建大規模戰鬥性能測試實例
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static UMingLargeScaleCombatTest* CreatePerformanceTest();

    /**
     * 初始化性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void InitializePerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 創建測試單位
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CreateTestUnits(UMingLargeScaleCombatTest* TestInstance, int32 UnitCount = 1000);

    /**
     * 開始性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StartPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 停止性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StopPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取性能測試結果
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static bool IsPerformanceTestPassing(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取平均FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetAverageFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取最低FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMinFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取最高FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMaxFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取活躍單位數量
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static int32 GetActiveUnitCount(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 清理性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CleanupPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 獲取性能測試狀態文字
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static FString GetPerformanceStatusText(UMingLargeScaleCombatTest* TestInstance);
};
