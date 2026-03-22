#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPerformanceProfiler.generated.h"

UENUM(BlueprintType)
enum class EProfilingType: uint8 {
    CPU UMETA(DisplayName = "CPU Performance"),
    Memory UMETA(DisplayName = "Memory Usage"),
    GPU UMETA(DisplayName = "GPU Performance"),
    Network UMETA(DisplayName = "Network Performance"),
    Disk UMETA(DisplayName = "Disk I/O Performance"),
    Rendering UMETA(DisplayName = "Rendering Performance"),
    Audio UMETA(DisplayName = "Audio Performance"),
    Physics UMETA(DisplayName = "Physics Performance")
};

UENUM(BlueprintType)
enum class EPerformanceLevel: uint8 {
    Excellent, UMETA(DisplayName = "Excellent (>90%)"),
    Good, UMETA(DisplayName = "Good (70-90%)"),
    Average, UMETA(DisplayName = "Average (50-70%)"),
    Poor, UMETA(DisplayName = "Poor (30-50%)"),
    Critical, UMETA(DisplayName = "Critical (<30%)")
};

USTRUCT(BlueprintType)
struct FPerformanceMetric
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metric")
    EProfilingType ProfilingType;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metric")
    float Value;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metric")
    EPerformanceLevel PerformanceLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metric")
    FString Unit;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metric")
    FDateTime Timestamp;

    FPerformanceMetric()
        : ProfilingType(EProfilingType::CPU)
        , Value(0.0f)
        , PerformanceLevel(EPerformanceLevel::Average)
        , Unit(TEXT(""))
        , Timestamp(FDateTime::Now())
    {}
};

/**
 * MingGoRTS Performance Profiler
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSPerformanceProfiler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceProfiler();
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void InitializeProfiler();
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StartProfiling(EProfilingType Type);
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StopProfiling(EProfilingType Type);
    UFUNCTION(BlueprintPure, Category = "Performance Profiler")
    TArray<FPerformanceMetric> GetPerformanceMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void GeneratePerformanceReport();
    UFUNCTION(BlueprintPure, Category = "Performance Profiler")
    EPerformanceLevel GetOverallPerformanceLevel() const;

protected:
    UPROPERTY()
    TArray<FPerformanceMetric> PerformanceMetrics;

    UPROPERTY()
    TMap<EProfilingType, bool> ActiveProfilingSessions;

    void CollectCPUMetrics();
    void CollectMemoryMetrics();
    void CollectGPUMetrics();
    void CollectNetworkMetrics();
    EPerformanceLevel CalculatePerformanceLevel(float Value, EProfilingType Type);
    FString GetPerformanceUnit(EProfilingType Type);
};
