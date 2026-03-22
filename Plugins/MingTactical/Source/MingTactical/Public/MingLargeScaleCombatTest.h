#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLargeScaleCombatTest.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * 大�?模戰鬥性能測試
 * 測試1000+?��X�戰鬥性能
 */
UCLASS()
class MINGTACTICAL_API UMingLargeScaleCombatTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLargeScaleCombatTest(};

    // ?��X�測�?    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void InitializeTest(};

    // 清�?測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CleanupTest(};

    // ?�建測試?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CreateTestUnits(int32 UnitCount = 1000};

    // ?��X�能測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StartPerformanceTest(};

    // ?�止?�能測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StopPerformanceTest(};

    // ?��X�能統�?
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetAverageFPS() const { return AverageFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMinFPS() const { return MinFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMaxFPS() const { return MaxFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    int32 GetActiveUnitCount() const { return TestUnits.Num(}; }

    // 測試結�?
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsPerformanceTestPass() const;

protected:
    virtual void Tick(float DeltaTime};

private:
    // 測試?��?
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> TestUnits;

    // ?�能X��
    bool bIsTestRunning;
    float TestStartTime;
    float TotalFrameTime;
    int32 FrameCount;
    
    // FPS統�?
    float AverageFPS;
    float MinFPS;
    float MaxFPS;
    float LastFrameTime;

    // ?��?管�X    UPROPERTY()
    TObjectPtr<UMingSelectionManager> SelectionManager;

    // ?�建?��?
    AMingTacticalUnit* CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType};

    // ?�新?�能統�?
    void UpdatePerformanceStats(float DeltaTime};

    // ?�置?�能統�?
    void ResetPerformanceStats(};

    // 模擬?��?行為
    void SimulateUnitBehavior(float DeltaTime};

    // 測試?��X��?
    void TestUnitSelection(};

    // 測試?��?移�?
    void TestUnitMovement(};

    // 測試?��X�鬥
    void TestUnitCombat(};
};

