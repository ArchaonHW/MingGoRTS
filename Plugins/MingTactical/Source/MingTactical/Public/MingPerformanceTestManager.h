#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceTestManager.generated.h"

/**
 * ??յ摧毀
 */
USTRUCT(BlueprintType)
struct FPerformanceTestResult
{
    GENERATED_BODY()

    // ??
    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    // ??_摧毀
    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    // 摧毀FPS
    UPROPERTY(BlueprintReadOnly)
    float TargetFPS;

    // 摧毀FPS
    UPROPERTY(BlueprintReadOnly)
    float ActualFPS;

    // ??CFPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // 目標數量
    UPROPERTY(BlueprintReadOnly)
    int32 UnitCount;

    // 目標數量
    UPROPERTY(BlueprintReadOnly)
    float TestDuration;

    // ??~H
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FPerformanceTestResult()
        : bPassed(false)
        , TargetFPS(60.0f)
        , ActualFPS(0.0f)
        , MinFPS(0.0f)
        , UnitCount(0)
        , TestDuration(0.0f)
    {}
};

/**
 * ??պ?X * ??Epic 6.4摧毀??t
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceTestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceTestManager();

    /**
     * 目標數量ʯ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void RunAllTests();

    /**
     * 1: 100目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test100Units();

    /**
     * 2: 500摧毀??W
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test500Units();

    /**
     * 3: 1000摧毀j摧毀(Epic 6.4摧毀)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test1000Units();

    /**
     * 4: 2000摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test2000Units();

    /**
     * 5: ?X???ʯ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestInstancedRendering();

    /**
     * 6: ?X??X?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestSpatialPartition();

    /**
     * 7: LODt??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestLODSystem();

    /**
     * 目標數量?X     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    TArray<FPerformanceTestResult> GetAllResults() const { return TestResults; }

    /**
     * 摧毀摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    FString GenerateReport() const;

    /**
     * ??_摧毀ճq??
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool AllTestsPassed() const;

    /**
     * 摧毀Epic 6.4?X?X     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsEpic64Complete() const;

    /**
     * ɥX摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    bool ExportTestReport(const FString& FilePath) const;

    /**
     * 目標數量??
     */
    static UMingPerformanceTestManager* Get();

private:
    // յ??
    UPROPERTY()
    TArray<FPerformanceTestResult> TestResults;

    // 摧毀??
    static UMingPerformanceTestManager* Instance;

    // ???X?δ
    FPerformanceTestResult RunGenericTest(const FString& TestName, int32 UnitCount, float Duration, float TargetFPS);

    // ??ش摧毀
    void CreateTestUnits(int32 Count);

    // 摧毀摧毀
    void DestroyTestUnits();

    // 目標數量FPS
    float MeasureFPS(float Duration);

    // O??յ??
    void RecordResult(const FPerformanceTestResult& Result);
);

