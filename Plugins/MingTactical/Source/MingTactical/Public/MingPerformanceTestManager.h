#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceTestManager.generated.h"

/**
 * ?ßËÉΩÊ∏¨Ë©¶ÁµêÊ?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FPerformanceTestResult
{
    GENERATED_BODY()

    // Ê∏¨Ë©¶?çÁ®±
    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    // ?ØÂê¶?öÈ?
    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    // ?ÆÊ?FPS
    UPROPERTY(BlueprintReadOnly)
    float TargetFPS;

    // ÂØ¶È?Âπ≥Â?FPS
    UPROPERTY(BlueprintReadOnly)
    float ActualFPS;

    // ?Ä‰ΩéFPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // Ê∏¨Ë©¶?Æ‰??∏È?
    UPROPERTY(BlueprintReadOnly)
    int32 UnitCount;

    // Ê∏¨Ë©¶?ÅÁ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float TestDuration;

    // ?ØË™§‰ø°ÊÅØ
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
 * ?ßËÉΩÊ∏¨Ë©¶ÁÆ°Á??? * È©óË?Epic 6.4?ßËÉΩ?™Â?Á≥ªÁµ±
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceTestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceTestManager();

    /**
     * ?ãË??Ä?âÊÄßËÉΩÊ∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void RunAllTests();

    /**
     * Ê∏¨Ë©¶1: 100?Æ‰??∫Êú¨?ßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test100Units();

    /**
     * Ê∏¨Ë©¶2: 500?Æ‰?‰∏≠Á?Ë¶èÊ®°
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test500Units();

    /**
     * Ê∏¨Ë©¶3: 1000?Æ‰?Â§ßË?Ê®?(Epic 6.4?ÆÊ?)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test1000Units();

    /**
     * Ê∏¨Ë©¶4: 2000?Æ‰?Ê•µÈ?Ê∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test2000Units();

    /**
     * Ê∏¨Ë©¶5: ÂØ¶‰??ñÊ∏≤?ìÊÄßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestInstancedRendering();

    /**
     * Ê∏¨Ë©¶6: Á©∫È??ÜÂ??ßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestSpatialPartition();

    /**
     * Ê∏¨Ë©¶7: LODÁ≥ªÁµ±?ßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestLODSystem();

    /**
     * ?≤Â??Ä?âÊ∏¨Ë©¶Á???     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    TArray<FPerformanceTestResult> GetAllResults() const { return TestResults; }

    /**
     * ?üÊ?Ê∏¨Ë©¶?±Â?
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    FString GenerateReport() const;

    /**
     * ?ØÂê¶?Ä?âÊ∏¨Ë©¶ÈÄöÈ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool AllTestsPassed() const;

    /**
     * ?≤Â?Epic 6.4ÂÆåÊ??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsEpic64Complete() const;

    /**
     * Â∞éÂá∫Ê∏¨Ë©¶?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    bool ExportTestReport(const FString& FilePath) const;

    /**
     * ?úÊ??≤Â?ÂØ¶‰?
     */
    static UMingPerformanceTestManager* Get();

private:
    // Ê∏¨Ë©¶ÁµêÊ?
    UPROPERTY()
    TArray<FPerformanceTestResult> TestResults;

    // ?Æ‰?ÂØ¶‰?
    static UMingPerformanceTestManager* Instance;

    // ?ãË??öÁî®Ê∏¨Ë©¶
    FPerformanceTestResult RunGenericTest(const FString& TestName, int32 UnitCount, float Duration, float TargetFPS);

    // ?µÂª∫Ê∏¨Ë©¶?Æ‰?
    void CreateTestUnits(int32 Count);

    // ?∑Ê?Ê∏¨Ë©¶?Æ‰?
    void DestroyTestUnits();

    // ?≤Â??∂Â?FPS
    float MeasureFPS(float Duration);

    // Ë®òÈ?Ê∏¨Ë©¶ÁµêÊ?
    void RecordResult(const FPerformanceTestResult& Result);
};
