#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingTacticalBlueprintLibrary.generated.h"

class UMingLargeScaleCombatTest;

/**
 * ?∞Ë?Á≥ªÁµ±?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á??∞Ë?Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?µÂª∫Â§ßË?Ê®°Êà∞È¨•ÊÄßËÉΩÊ∏¨Ë©¶ÂØ¶‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static UMingLargeScaleCombatTest* CreatePerformanceTest();

    /**
     * ?ùÂ??ñÊÄßËÉΩÊ∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void InitializePerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?µÂª∫Ê∏¨Ë©¶?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CreateTestUnits(UMingLargeScaleCombatTest* TestInstance, int32 UnitCount = 1000);

    /**
     * ?ãÂ??ßËÉΩÊ∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StartPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?úÊ≠¢?ßËÉΩÊ∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void StopPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â??ßËÉΩÊ∏¨Ë©¶ÁµêÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static bool IsPerformanceTestPassing(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â?Âπ≥Â?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetAverageFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â??Ä‰ΩéFPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMinFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â??ÄÈ´òFPS
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static float GetMaxFPS(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â?Ê¥ªË??Æ‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static int32 GetActiveUnitCount(UMingLargeScaleCombatTest* TestInstance);

    /**
     * Ê∏ÖÁ??ßËÉΩÊ∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical|Performance", CallInEditor)
    static void CleanupPerformanceTest(UMingLargeScaleCombatTest* TestInstance);

    /**
     * ?≤Â??ßËÉΩÊ∏¨Ë©¶?Ä?ãÊ?Â≠?     */
    UFUNCTION(BlueprintPure, Category = "Tactical|Performance")
    static FString GetPerformanceStatusText(UMingLargeScaleCombatTest* TestInstance);
};
