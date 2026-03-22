#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPerformanceProfiler.generated.h"

UENUM(BlueprintType)
enum class EProfilingType : uint8
{
    CPU,             // CPU?�能?��?
    Memory,          // ?��X��?
    GPU,             // GPU?�能?��?
    Network,         // 網絡?��?
    Disk,            // 磁盤I/O?��?
    Rendering,       // 渲�X�能?��?
    Audio,           // ?�頻?�能?��?
    Physics          // ?��X�能?��?
};

UENUM(BlueprintType)
enum class EPerformanceLevel : uint8
{
    Excellent,       // ?��? (>90%)
    Good,            // ?�好 (70-90%)
    Average,         // 一X(50-70%)
    Poor,            // 較差 (30-50%)
    Critical         // ?��? (<30%)
};

USTRUCT(BlueprintType)
struct FPerformanceMetric
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MetricName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PeakValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPerformanceLevel PerformanceLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> HistoricalData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    FPerformanceMetric()
    {
        MetricName = TEXT(""};
        CurrentValue = 0.0f;
        AverageValue = 0.0f;
        PeakValue = 0.0f;
        MinValue = FLT_MAX;
        PerformanceLevel = EPerformanceLevel::Average;
        LastUpdateTime = FDateTime::Now(};
    }
};

USTRUCT(BlueprintType)
struct FProfilingSession
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EProfilingType ProfilingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FPerformanceMetric> Metrics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PerformanceWarnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> OptimizationSuggestions;

    FProfilingSession()
    {
        SessionID = TEXT(""};
        SessionName = TEXT(""};
        ProfilingType = EProfilingType::CPU;
        Duration = 0.0f;
        bIsActive = false;
    }
};

USTRUCT(BlueprintType)
struct FBottleneckAnalysis
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ComponentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EProfilingType ProfilingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImpactScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SuggestedOptimizations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PotentialImprovement;

    FBottleneckAnalysis()
    {
        ComponentName = TEXT(""};
        ProfilingType = EProfilingType::CPU;
        ImpactScore = 0.0f;
        PotentialImprovement = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProfilingStarted, const FString&, SessionID, EProfilingType, ProfilingType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProfilingCompleted, const FString&, SessionID, const FProfilingSession&, Session};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPerformanceWarning, const FString&, SessionID, const FString&, Warning, EPerformanceLevel, Level};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBottleneckDetected, const FString&, SessionID, const FBottleneckAnalysis&, Bottleneck};

/**
 * ?�能?��X�系�?- 實�X�能X��?��X�系�? * ?��X�面?�性能?��X�瓶?�檢測�X��?建議
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSPerformanceProfiler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceProfiler(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void InitializeProfiler(};

    // ?��X�能?��X�話
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    FString StartProfilingSession(EProfilingType ProfilingType, const FString& SessionName};

    // ?�止?�能?��X�話
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StopProfilingSession(const FString& SessionID};

    // ?��X��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<EProfilingType, FPerformanceMetric> GetCurrentMetrics() const;

    // ?��X�能?�頸
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TArray<FBottleneckAnalysis> AnalyzeBottlenecks(const FString& SessionID};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    FString GeneratePerformanceReport(const FString& SessionID};

    // 實�X�能X��
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StartRealTimeMonitoring(};

    // ?�止實�X��
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StopRealTimeMonitoring(};

    // 設置?�能?��?    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void SetPerformanceThresholds(EProfilingType ProfilingType, float WarningThreshold, float CriticalThreshold};

    // ?��X��?建議
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TArray<FString> GetOptimizationSuggestions(const FString& SessionID};

    // 比�X�能?�話
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<FString, float> CompareSessions(const FString& SessionID1, const FString& SessionID2};

    // 導出?�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    bool ExportPerformanceData(const FString& SessionID, const FString& FilePath};

    // 設置X��?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void SetMonitoringInterval(float IntervalSeconds};

    // ?��?系統?�能概覽
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<EProfilingType, EPerformanceLevel> GetSystemPerformanceOverview() const;

    // 清�X��?�?    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void CleanupOldSessions(int32 MaxSessionsToKeep};

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnProfilingStarted OnProfilingStarted;

    UPROPERTY(BlueprintAssignable)
    FOnProfilingCompleted OnProfilingCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceWarning OnPerformanceWarning;

    UPROPERTY(BlueprintAssignable)
    FOnBottleneckDetected OnBottleneckDetected;

protected:
    // X��?�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    float MonitoringInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    bool bEnableRealTimeMonitoring;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    int32 MaxHistoricalDataPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    bool bEnableAutomaticOptimization;

    // ?�能?�話存儲
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TMap<FString, FProfilingSession> ProfilingSessions;

    // ?��?活�X�話
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TArray<FString> ActiveSessions;

    // ?�能?��?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Data")
    TMap<EProfilingType, TTuple<float, float>> PerformanceThresholds;

    // ?��X�能?��?
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TMap<EProfilingType, FPerformanceMetric> CurrentMetrics;

private:
    // ?�部?�能?��X�輯
    void UpdatePerformanceMetrics(};
    void CollectCPUMetrics(};
    void CollectMemoryMetrics(};
    void CollectGPUMetrics(};
    void CollectNetworkMetrics(};
    void CollectDiskMetrics(};
    void CollectRenderingMetrics(};
    void CollectAudioMetrics(};
    void CollectPhysicsMetrics(};
    
    // ?�能?��?算�?
    float CalculateCPUUsage(};
    float CalculateMemoryUsage(};
    float CalculateGPUUsage(};
    float CalculateNetworkLatency(};
    float CalculateDiskIO(};
    float CalculateRenderingPerformance(};
    float CalculateAudioPerformance(};
    float CalculatePhysicsPerformance(};
    
    // ?�頸檢測
    void DetectBottlenecks(const FString& SessionID};
    FBottleneckAnalysis AnalyzeComponentPerformance(EProfilingType ProfilingType, const FPerformanceMetric& Metric};
    
    // ?��?建議?��?
    TArray<FString> GenerateOptimizationSuggestions(EProfilingType ProfilingType, const FPerformanceMetric& Metric};
    FString GeneratePerformanceReportInternal(const FProfilingSession& Session};
    
    // 輔助?�數
    EPerformanceLevel EvaluatePerformanceLevel(float Value, float WarningThreshold, float CriticalThreshold};
    void UpdateHistoricalData(FPerformanceMetric& Metric};
    void TriggerPerformanceWarning(const FString& SessionID, const FString& Warning, EPerformanceLevel Level};
    FString GetProfilingTypeName(EProfilingType ProfilingType) const;
    
    // 實�X��
    void RealTimeMonitoringLoop(};
    bool bIsRealTimeMonitoringActive;
    FTimerHandle RealTimeMonitoringTimer;
};

