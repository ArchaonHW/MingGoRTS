#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceBenchmark.generated.h"

UENUM(BlueprintType)
enum class EMingBenchmarkType : uint8
{
    LoadTime UMETA(DisplayName = "Load Time"),
    MemoryUsage UMETA(DisplayName = "Memory Usage"),
    FPS UMETA(DisplayName = "FPS"),
    CPU UMETA(DisplayName = "CPU"),
    GPU UMETA(DisplayName = "GPU"),
    Network UMETA(DisplayName = "Network"),
    DiskIO UMETA(DisplayName = "Disk I/O"),
    EventLatency UMETA(DisplayName = "Event Latency")
};

USTRUCT(BlueprintType)
struct FMingBenchmarkResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BenchmarkName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingBenchmarkType BenchmarkType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MeasuredValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Unit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTargetMet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PerformanceRatio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TestSamples;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardDeviation;

    FMingBenchmarkResult()
        : BenchmarkType(EMingBenchmarkType::LoadTime)
        , MeasuredValue(0.0f)
        , TargetValue(0.0f)
        , bTargetMet(false)
        , PerformanceRatio(0.0f)
        , AverageValue(0.0f)
        , MinValue(0.0f)
        , MaxValue(0.0f)
        , StandardDeviation(0.0f)
    {}
};

/**
 * Performance Benchmark for MingGoRTS
 * Measures and analyzes system performance metrics
 */
UCLASS(ClassGroup = (Performance), Blueprintable)
class MINGINTEGRATION_API UMingPerformanceBenchmark : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceBenchmark();

    // Benchmark Execution
    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunAllBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunLoadTimeBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunMemoryUsageBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunFPSBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunCPUBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunGPUBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunNetworkBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunDiskIOBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Benchmark")
    void RunEventLatencyBenchmark();

    // Continuous Monitoring
    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void StartContinuousMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void StopContinuousMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void UpdateMonitoringData();

    UFUNCTION(BlueprintPure, Category = "Monitoring")
    bool IsMonitoringActive() const;

    // Results Analysis
    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingBenchmarkResult> GetAllResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingBenchmarkResult> GetPassedResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingBenchmarkResult> GetFailedResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    float GetOverallPerformanceScore() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    bool MeetsAllTargets() const;

    UFUNCTION(BlueprintCallable, Category = "Results")
    void GeneratePerformanceReport();

    UFUNCTION(BlueprintCallable, Category = "Results")
    void ExportBenchmarkResults(const FString& FilePath);

    // Target Management
    UFUNCTION(BlueprintCallable, Category = "Targets")
    void SetBenchmarkTarget(EMingBenchmarkType BenchmarkType, float TargetValue);

    UFUNCTION(BlueprintPure, Category = "Targets")
    float GetBenchmarkTarget(EMingBenchmarkType BenchmarkType) const;

    UFUNCTION(BlueprintCallable, Category = "Targets")
    void LoadDefaultTargets();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetBenchmarkTypeName(EMingBenchmarkType BenchmarkType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveBenchmarkData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadBenchmarkData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingBenchmarkResult> BenchmarkResults;

    UPROPERTY()
    TMap<EMingBenchmarkType, float> BenchmarkTargets;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeBenchmarkTargets();
    void SetupMonitoringEnvironment();

    // Benchmark Execution
    void ExecuteLoadTimeTest();
    void ExecuteMemoryUsageTest();
    void ExecuteFPSTest();
    void ExecuteCPUTest();
    void ExecuteGPUTest();
    void ExecuteNetworkTest();
    void ExecuteDiskIOTest();
    void ExecuteEventLatencyTest();

    // Data Collection
    void CollectPerformanceSample(EMingBenchmarkType BenchmarkType);
    void ProcessBenchmarkData(EMingBenchmarkType BenchmarkType);
    void CalculateStatistics(FMingBenchmarkResult& Result);
    void EvaluatePerformance(FMingBenchmarkResult& Result);

    // Monitoring
    void CollectMonitoringData();
    void UpdateMonitoringStatistics();
    void CheckPerformanceThresholds();

    // Analysis
    void AnalyzePerformanceTrends();
    void IdentifyPerformanceBottlenecks();
    void GenerateOptimizationSuggestions();
    void CreatePerformanceSummary();

    // Helpers
    FMingBenchmarkResult* FindBenchmarkResult(const FString& BenchmarkName);
    void AddBenchmarkResult(const FMingBenchmarkResult& Result);
    void LogBenchmarkEvent(const FString& Event, const FString& Details);
    float CalculatePerformanceScore(const FMingBenchmarkResult& Result) const;
};
