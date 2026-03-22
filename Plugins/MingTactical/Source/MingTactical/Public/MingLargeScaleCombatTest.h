#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLargeScaleCombatTest.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * ??家驹矮┦代刚
 * 代刚1000+???X?驹矮┦
 */
UCLASS()
class MINGTACTICAL_API UMingLargeScaleCombatTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLargeScaleCombatTest();

    // ???X?代??    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void InitializeTest();

    // 睲??代刚
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CleanupTest();

    // ??代刚????
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CreateTestUnits(int32 UnitCount = 1000};

    // ???X?代刚
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StartPerformanceTest();

    // ??ゎ??代刚
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StopPerformanceTest();

    // ???X?参??
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetAverageFPS() const { return AverageFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMinFPS() const { return MinFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMaxFPS() const { return MaxFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    int32 GetActiveUnitCount() const { return TestUnits.Num(); }

    // 代刚挡??
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsPerformanceTestPass() const;

protected:
    virtual void Tick(float DeltaTime};

private:
    // 代刚????
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> TestUnits;

    // ??X??
    bool bIsTestRunning;
    float TestStartTime;
    float TotalFrameTime;
    int32 FrameCount;
    
    // FPS参??
    float AverageFPS;
    float MinFPS;
    float MaxFPS;
    float LastFrameTime;

    // ????恨?X    UPROPERTY()
    TObjectPtr<UMingSelectionManager> SelectionManager;

    // ??????
    AMingTacticalUnit* CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType};

    // ??穝??参??
    void UpdatePerformanceStats(float DeltaTime};

    // ??竚??参??
    void ResetPerformanceStats();

    // 家览????︽
    void SimulateUnitBehavior(float DeltaTime};

    // 代刚???X???
    void TestUnitSelection();

    // 代刚????簿??
    void TestUnitMovement();

    // 代刚???X?矮
    void TestUnitCombat();
};

