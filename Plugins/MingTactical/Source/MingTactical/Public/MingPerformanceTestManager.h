#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceTestManager.generated.h"

/**
 * ??能測試結??結??
 */
USTRUCT(BlueprintType)
struct FPerformanceTestResult
{
    GENERATED_BODY()

    // 測試??稱
    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    // ??否????
    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    // ????FPS
    UPROPERTY(BlueprintReadOnly)
    float TargetFPS;

    // 實??平??FPS
    UPROPERTY(BlueprintReadOnly)
    float ActualFPS;

    // ??低FPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // 測試???X???
    UPROPERTY(BlueprintReadOnly)
    int32 UnitCount;

    // 測試???X???
    UPROPERTY(BlueprintReadOnly)
    float TestDuration;

    // ??誤信息
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
 * ??能測試管?X * 驗??Epic 6.4??能????系統
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceTestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceTestManager();

    /**
     * ???X???性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void RunAllTests();

    /**
     * 測試1: 100???X?本??能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test100Units();

    /**
     * 測試2: 500????中??規模
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test500Units();

    /**
     * 測試3: 1000????大????(Epic 6.4????)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test1000Units();

    /**
     * 測試4: 2000????極??測試
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test2000Units();

    /**
     * 測試5: 實?X?渲??性能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestInstancedRendering();

    /**
     * 測試6: 空?X??X?能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestSpatialPartition();

    /**
     * 測試7: LOD系統??能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestLODSystem();

    /**
     * ???X???測試?X     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    TArray<FPerformanceTestResult> GetAllResults() const { return TestResults; }

    /**
     * ????測試????
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    FString GenerateReport() const;

    /**
     * ??否????測試通??
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool AllTestsPassed() const;

    /**
     * ????Epic 6.4完?X?X     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsEpic64Complete() const;

    /**
     * 導出測試????
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    bool ExportTestReport(const FString& FilePath) const;

    /**
     * ???X???實??
     */
    static UMingPerformanceTestManager* Get();

private:
    // 測試結??
    UPROPERTY()
    TArray<FPerformanceTestResult> TestResults;

    // ????實??
    static UMingPerformanceTestManager* Instance;

    // ???X?用測試
    FPerformanceTestResult RunGenericTest(const FString& TestName, int32 UnitCount, float Duration, float TargetFPS};

    // ??建測試????
    void CreateTestUnits(int32 Count};

    // ????測試????
    void DestroyTestUnits();

    // ???X???FPS
    float MeasureFPS(float Duration};

    // 記??測試結??
    void RecordResult(const FPerformanceTestResult& Result};
};

