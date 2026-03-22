#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceTestManager.generated.h"

/**
 * 性能測試結果結構
 */
USTRUCT(BlueprintType)
struct FPerformanceTestResult
{
    GENERATED_BODY()

    // 測試名稱
    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    // 是否通過
    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    // 目標FPS
    UPROPERTY(BlueprintReadOnly)
    float TargetFPS;

    // 實際平均FPS
    UPROPERTY(BlueprintReadOnly)
    float ActualFPS;

    // 最低FPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // 測試單位數量
    UPROPERTY(BlueprintReadOnly)
    int32 UnitCount;

    // 測試持續時間
    UPROPERTY(BlueprintReadOnly)
    float TestDuration;

    // 錯誤信息
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
 * 性能測試管理器
 * 驗證Epic 6.4性能優化系統
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceTestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceTestManager();

    /**
     * 運行所有性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void RunAllTests();

    /**
     * 測試1: 100單位基本性能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test100Units();

    /**
     * 測試2: 500單位中等規模
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test500Units();

    /**
     * 測試3: 1000單位大規模 (Epic 6.4目標)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test1000Units();

    /**
     * 測試4: 2000單位極限測試
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult Test2000Units();

    /**
     * 測試5: 實例化渲染性能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestInstancedRendering();

    /**
     * 測試6: 空間分塊性能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestSpatialPartition();

    /**
     * 測試7: LOD系統性能
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestLODSystem();

    /**
     * 獲取所有測試結果
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    TArray<FPerformanceTestResult> GetAllResults() const { return TestResults; }

    /**
     * 生成測試報告
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    FString GenerateReport() const;

    /**
     * 是否所有測試通過
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool AllTestsPassed() const;

    /**
     * 獲取Epic 6.4完成狀態
     */
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsEpic64Complete() const;

    /**
     * 導出測試報告
     */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    bool ExportTestReport(const FString& FilePath) const;

    /**
     * 靜態獲取實例
     */
    static UMingPerformanceTestManager* Get();

private:
    // 測試結果
    UPROPERTY()
    TArray<FPerformanceTestResult> TestResults;

    // 單例實例
    static UMingPerformanceTestManager* Instance;

    // 運行通用測試
    FPerformanceTestResult RunGenericTest(const FString& TestName, int32 UnitCount, float Duration, float TargetFPS);

    // 創建測試單位
    void CreateTestUnits(int32 Count);

    // 銷毀測試單位
    void DestroyTestUnits();

    // 獲取當前FPS
    float MeasureFPS(float Duration);

    // 記錄測試結果
    void RecordResult(const FPerformanceTestResult& Result);
};
