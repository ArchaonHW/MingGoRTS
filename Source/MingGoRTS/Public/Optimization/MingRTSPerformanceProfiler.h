#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPerformanceProfiler.generated.h"

UENUM(BlueprintType)
enum class EProfilingType : uint8
{
    CPU,             // CPU?ßËÉΩ?ÜÊ?
    Memory,          // ?ßÂ??ÜÊ?
    GPU,             // GPU?ßËÉΩ?ÜÊ?
    Network,         // Á∂≤Áµ°?ÜÊ?
    Disk,            // Á£ÅÁõ§I/O?ÜÊ?
    Rendering,       // Ê∏≤Ê??ßËÉΩ?ÜÊ?
    Audio,           // ?≥È†ª?ßËÉΩ?ÜÊ?
    Physics          // ?©Á??ßËÉΩ?ÜÊ?
};

UENUM(BlueprintType)
enum class EPerformanceLevel : uint8
{
    Excellent,       // ?™Á? (>90%)
    Good,            // ?ØÂ•Ω (70-90%)
    Average,         // ‰∏Ä??(50-70%)
    Poor,            // ËºÉÂ∑Æ (30-50%)
    Critical         // ?®Á? (<30%)
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
 * ?ßËÉΩ?ÜÊ??®Á≥ªÁµ?- ÂØ¶Ê??ßËÉΩ??éß?åÂ??êÁ≥ªÁµ? * ?ê‰??®Èù¢?ÑÊÄßËÉΩ?ÜÊ??ÅÁì∂?∏Ê™¢Ê∏¨Â??™Â?Âª∫Ë≠∞
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSPerformanceProfiler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceProfiler(};

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void InitializeProfiler(};

    // ?ãÂ??ßËÉΩ?ÜÊ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    FString StartProfilingSession(EProfilingType ProfilingType, const FString& SessionName};

    // ?úÊ≠¢?ßËÉΩ?ÜÊ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StopProfilingSession(const FString& SessionID};

    // ?≤Â??∂Â??ßËÉΩ?áÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<EProfilingType, FPerformanceMetric> GetCurrentMetrics() const;

    // ?ÜÊ??ßËÉΩ?∂È†∏
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TArray<FBottleneckAnalysis> AnalyzeBottlenecks(const FString& SessionID};

    // ?üÊ??ßËÉΩ?±Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    FString GeneratePerformanceReport(const FString& SessionID};

    // ÂØ¶Ê??ßËÉΩ??éß
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StartRealTimeMonitoring(};

    // ?úÊ≠¢ÂØ¶Ê???éß
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void StopRealTimeMonitoring(};

    // Ë®≠ÁΩÆ?ßËÉΩ?æÂÄ?    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void SetPerformanceThresholds(EProfilingType ProfilingType, float WarningThreshold, float CriticalThreshold};

    // ?≤Â??™Â?Âª∫Ë≠∞
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TArray<FString> GetOptimizationSuggestions(const FString& SessionID};

    // ÊØîË??ßËÉΩ?ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<FString, float> CompareSessions(const FString& SessionID1, const FString& SessionID2};

    // Â∞éÂá∫?ßËÉΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    bool ExportPerformanceData(const FString& SessionID, const FString& FilePath};

    // Ë®≠ÁΩÆ??éß?ìÈ?
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void SetMonitoringInterval(float IntervalSeconds};

    // ?≤Â?Á≥ªÁµ±?ßËÉΩÊ¶ÇË¶Ω
    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    TMap<EProfilingType, EPerformanceLevel> GetSystemPerformanceOverview() const;

    // Ê∏ÖÁ??äÊ?Ë©?    UFUNCTION(BlueprintCallable, Category = "Performance Profiler")
    void CleanupOldSessions(int32 MaxSessionsToKeep};

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnProfilingStarted OnProfilingStarted;

    UPROPERTY(BlueprintAssignable)
    FOnProfilingCompleted OnProfilingCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceWarning OnPerformanceWarning;

    UPROPERTY(BlueprintAssignable)
    FOnBottleneckDetected OnBottleneckDetected;

protected:
    // ??éß?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    float MonitoringInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    bool bEnableRealTimeMonitoring;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    int32 MaxHistoricalDataPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Parameters")
    bool bEnableAutomaticOptimization;

    // ?ßËÉΩ?ÉË©±Â≠òÂÑ≤
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TMap<FString, FProfilingSession> ProfilingSessions;

    // ?∂Â?Ê¥ªÂ??ÉË©±
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TArray<FString> ActiveSessions;

    // ?ßËÉΩ?æÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling Data")
    TMap<EProfilingType, TTuple<float, float>> PerformanceThresholds;

    // ?∂Â??ßËÉΩ?áÊ?
    UPROPERTY(BlueprintReadOnly, Category = "Profiling Data")
    TMap<EProfilingType, FPerformanceMetric> CurrentMetrics;

private:
    // ?ßÈÉ®?ßËÉΩ?ÜÊ??èËºØ
    void UpdatePerformanceMetrics(};
    void CollectCPUMetrics(};
    void CollectMemoryMetrics(};
    void CollectGPUMetrics(};
    void CollectNetworkMetrics(};
    void CollectDiskMetrics(};
    void CollectRenderingMetrics(};
    void CollectAudioMetrics(};
    void CollectPhysicsMetrics(};
    
    // ?ßËÉΩ?ÜÊ?ÁÆóÊ?
    float CalculateCPUUsage(};
    float CalculateMemoryUsage(};
    float CalculateGPUUsage(};
    float CalculateNetworkLatency(};
    float CalculateDiskIO(};
    float CalculateRenderingPerformance(};
    float CalculateAudioPerformance(};
    float CalculatePhysicsPerformance(};
    
    // ?∂È†∏Ê™¢Ê∏¨
    void DetectBottlenecks(const FString& SessionID};
    FBottleneckAnalysis AnalyzeComponentPerformance(EProfilingType ProfilingType, const FPerformanceMetric& Metric};
    
    // ?™Â?Âª∫Ë≠∞?üÊ?
    TArray<FString> GenerateOptimizationSuggestions(EProfilingType ProfilingType, const FPerformanceMetric& Metric};
    FString GeneratePerformanceReportInternal(const FProfilingSession& Session};
    
    // ËºîÂä©?ΩÊï∏
    EPerformanceLevel EvaluatePerformanceLevel(float Value, float WarningThreshold, float CriticalThreshold};
    void UpdateHistoricalData(FPerformanceMetric& Metric};
    void TriggerPerformanceWarning(const FString& SessionID, const FString& Warning, EPerformanceLevel Level};
    FString GetProfilingTypeName(EProfilingType ProfilingType) const;
    
    // ÂØ¶Ê???éß
    void RealTimeMonitoringLoop(};
    bool bIsRealTimeMonitoringActive;
    FTimerHandle RealTimeMonitoringTimer;
};

