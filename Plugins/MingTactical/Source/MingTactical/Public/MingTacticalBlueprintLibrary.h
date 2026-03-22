#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingTacticalBlueprintLibrary.generated.h"

class UMingLargeScaleCombatTest;

/**
 * 摧毀t目標數量 * 故事重要性?目標數量t??
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ??ؤj??Ҿ԰ʯչ??
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static UMingLargeScaleCombatTest* CreatePerformanceTest();

    /**
     * ???X?ʯ
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void InitializePerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ??ش摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CreateTestUnits(UMingLargeScaleCombatTest* TestInstance, int32 UnitCount = 1000);

    /**
     * ???X?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StartPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StopPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ???X?յ??
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static bool IsPerformanceTestPassing(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 摧毀??FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetAverageFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ???X?CFPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMinFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ???X?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMaxFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 摧毀?X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static int32 GetActiveUnitCount(UMingLargeScaleCombatTest* TestInstance);

    /**
     * M?X?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CleanupPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * 目標數量摧毀??     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static FString GetPerformanceStatusText(UMingLargeScaleCombatTest* TestInstance);
};

