#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingTacticalBlueprintLibrary.generated.h"

class UMingLargeScaleCombatTest;

/**
 * ?��?系統?��X�數�? * ?��X��X�調?��X��?系統?�能
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?�建大�?模戰鬥性能測試實�?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static UMingLargeScaleCombatTest* CreatePerformanceTest(};

    /**
     * ?��X�性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void InitializePerformanceTest(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?�建測試?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CreateTestUnits(UMingLargeScaleCombatTest* TestInstance, int32 UnitCount = 1000};

    /**
     * ?��X�能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StartPerformanceTest(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?�止?�能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StopPerformanceTest(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��X�能測試結�?
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static bool IsPerformanceTestPassing(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��?平�?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetAverageFPS(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��X�低FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMinFPS(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��X�高FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMaxFPS(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��?活�X��X��?
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static int32 GetActiveUnitCount(UMingLargeScaleCombatTest* TestInstance};

    /**
     * 清�X�能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CleanupPerformanceTest(UMingLargeScaleCombatTest* TestInstance};

    /**
     * ?��X�能測試?�?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static FString GetPerformanceStatusText(UMingLargeScaleCombatTest* TestInstance};
};

