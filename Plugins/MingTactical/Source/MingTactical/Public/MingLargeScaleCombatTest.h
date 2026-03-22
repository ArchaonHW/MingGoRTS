#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLargeScaleCombatTest.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * Â§ßË?Ê®°Êà∞È¨•ÊÄßËÉΩÊ∏¨Ë©¶
 * Ê∏¨Ë©¶1000+?Æ‰??ÑÊà∞È¨•ÊÄßËÉΩ
 */
UCLASS()
class MINGTACTICAL_API UMingLargeScaleCombatTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLargeScaleCombatTest();

    // ?ùÂ??ñÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void InitializeTest();

    // Ê∏ÖÁ?Ê∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CleanupTest();

    // ?µÂª∫Ê∏¨Ë©¶?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CreateTestUnits(int32 UnitCount = 1000);

    // ?ãÂ??ßËÉΩÊ∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StartPerformanceTest();

    // ?úÊ≠¢?ßËÉΩÊ∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StopPerformanceTest();

    // ?≤Â??ßËÉΩÁµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetAverageFPS() const { return AverageFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMinFPS() const { return MinFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMaxFPS() const { return MaxFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    int32 GetActiveUnitCount() const { return TestUnits.Num(); }

    // Ê∏¨Ë©¶ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsPerformanceTestPass() const;

protected:
    virtual void Tick(float DeltaTime);

private:
    // Ê∏¨Ë©¶?Æ‰?
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> TestUnits;

    // ?ßËÉΩ??éß
    bool bIsTestRunning;
    float TestStartTime;
    float TotalFrameTime;
    int32 FrameCount;
    
    // FPSÁµ±Ë?
    float AverageFPS;
    float MinFPS;
    float MaxFPS;
    float LastFrameTime;

    // ?∏Ê?ÁÆ°Á???    UPROPERTY()
    TObjectPtr<UMingSelectionManager> SelectionManager;

    // ?µÂª∫?Æ‰?
    AMingTacticalUnit* CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType);

    // ?¥Êñ∞?ßËÉΩÁµ±Ë?
    void UpdatePerformanceStats(float DeltaTime);

    // ?çÁΩÆ?ßËÉΩÁµ±Ë?
    void ResetPerformanceStats();

    // Ê®°Êì¨?Æ‰?Ë°åÁÇ∫
    void SimulateUnitBehavior(float DeltaTime);

    // Ê∏¨Ë©¶?Æ‰??∏Ê?
    void TestUnitSelection();

    // Ê∏¨Ë©¶?Æ‰?ÁßªÂ?
    void TestUnitMovement();

    // Ê∏¨Ë©¶?Æ‰??∞È¨•
    void TestUnitCombat();
};
