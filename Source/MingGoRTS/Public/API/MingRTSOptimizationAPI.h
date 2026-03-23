#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSOptimizationAPI.generated.h"

UENUM(BlueprintType)
enum class EOptimizationType : uint8
{
    Performance,
    Memory,
    Network,
    Rendering,
    AI,
    Audio,
    UI,
    FileIO,
    Compilation,
    Overall
};

UENUM(BlueprintType)
enum class EOptimizationStrategy : uint8
{
    Conservative,
    Balanced,
    Aggressive,
    Custom,
    Adaptive,
    Predictive
};

USTRUCT(BlueprintType)
struct FOptimizationMetric
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString MetricName;

    UPROPERTY(BlueprintReadOnly)
    float CurrentValue;

    UPROPERTY(BlueprintReadOnly)
    float TargetValue;

    UPROPERTY(BlueprintReadOnly)
    float ImprovementPercentage;

    UPROPERTY(BlueprintReadOnly)
    EOptimizationType Type;

    UPROPERTY(BlueprintReadOnly)
    FString Unit;

    FOptimizationMetric()
    {
        CurrentValue = 0.0f;
        TargetValue = 0.0f;
        ImprovementPercentage = 0.0f;
        Type = EOptimizationType::Performance;
    }
};

USTRUCT(BlueprintType)
struct FOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    EOptimizationType Type;

    UPROPERTY(BlueprintReadOnly)
    EOptimizationStrategy Strategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FOptimizationMetric> Metrics;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AppliedOptimizations;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    float OptimizationTime;

    UPROPERTY(BlueprintReadOnly)
    float PerformanceImprovement;

    UPROPERTY(BlueprintReadOnly)
    float MemoryReduction;

    FOptimizationResult()
    {
        bSuccess = false;
        Type = EOptimizationType::Performance;
        Strategy = EOptimizationStrategy::Balanced;
        OptimizationTime = 0.0f;
        PerformanceImprovement = 0.0f;
        MemoryReduction = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FOptimizationProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProfileName;

    UPROPERTY(BlueprintReadOnly)
    EOptimizationStrategy Strategy;

    UPROPERTY(BlueprintReadOnly)
    TMap<EOptimizationType, float> Weights;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Settings;

    UPROPERTY(BlueprintReadOnly)
    TArray<FOptimizationMetric> TargetMetrics;

    FOptimizationProfile()
    {
        Strategy = EOptimizationStrategy::Balanced;
    }
};

/**
 * 智慧優化API - 具備自我學習能力的優化系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationAPI();

    // 核心優化功能
    UFUNCTION(BlueprintCallable, Category = "Optimization API")
    FOptimizationResult OptimizePerformance(const FOptimizationProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Optimization API")
    FOptimizationResult OptimizeMemory(const FOptimizationProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Optimization API")
    FOptimizationResult OptimizeNetwork(const FOptimizationProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Optimization API")
    FOptimizationResult OptimizeRendering(const FOptimizationProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Optimization API")
    FOptimizationResult OptimizeAI(const FOptimizationProfile& Profile);

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "Optimization API|Monitoring")
    TArray<FOptimizationMetric> GetCurrentMetrics();

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Monitoring")
    TArray<FOptimizationMetric> GetHistoricalMetrics(const FDateTime& StartTime, const FDateTime& EndTime);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Monitoring")
    void StartMetricTracking(EOptimizationType Type);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Monitoring")
    void StopMetricTracking(EOptimizationType Type);

    // 自我學習功能
    UFUNCTION(BlueprintCallable, Category = "Optimization API|Learning")
    void LearnFromOptimizationResult(const FOptimizationResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Learning")
    void UpdateOptimizationStrategy(const FString& Context, EOptimizationStrategy Strategy, float SuccessRate);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Learning")
    FOptimizationProfile GenerateOptimalProfile(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Learning")
    void AdaptToSystemChanges(const TArray<FOptimizationMetric>& CurrentMetrics);

    // 智慧預測
    UFUNCTION(BlueprintCallable, Category = "Optimization API|Prediction")
    TArray<FOptimizationMetric> PredictPerformanceTrends(const FString& TimeHorizon);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Prediction")
    TArray<FString> PredictBottlenecks();

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Prediction")
    FOptimizationProfile SuggestProactiveOptimizations();

    // 自動優化
    UFUNCTION(BlueprintCallable, Category = "Optimization API|Automation")
    void EnableAutoOptimization(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Automation")
    void SetOptimizationSchedule(const FString& Schedule);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Automation")
    void RunScheduledOptimizations();

    // 跨API調用
    UFUNCTION(BlueprintCallable, Category = "Optimization API|Integration")
    void RequestLogAnalysis(const TArray<FOptimizationMetric>& Metrics);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Integration")
    void RequestDebuggingSession(const FOptimizationResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Optimization API|Integration")
    void RequestCompilationOptimization(const FOptimizationProfile& Profile);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptimizationStarted, EOptimizationType, Type);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOptimizationProgress, EOptimizationType, Type, float, Progress);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptimizationCompleted, const FOptimizationResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMetricsUpdated, const TArray<FOptimizationMetric>&, Metrics);

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationStarted OnOptimizationStarted;

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationProgress OnOptimizationProgress;

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationCompleted OnOptimizationCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnMetricsUpdated OnMetricsUpdated;

private:
    // 學習數據
    UPROPERTY()
    TArray<FOptimizationResult> OptimizationHistory;

    UPROPERTY()
    TMap<FString, EOptimizationStrategy> ContextStrategies;

    UPROPERTY()
    TMap<EOptimizationType, FOptimizationMetric> CurrentMetrics;

    UPROPERTY()
    TArray<FOptimizationProfile> SavedProfiles;

    UPROPERTY()
    bool bAutoOptimizationEnabled;

    // 內部方法
    void InitializeOptimizationDatabase();
    void SaveOptimizationData();
    void LoadOptimizationData();
    float CalculateOptimizationScore(const FOptimizationResult& Result);
    void UpdateLearningAlgorithms(const FOptimizationResult& Result);
    FOptimizationProfile AdaptProfileToContext(const FOptimizationProfile& BaseProfile, const FString& Context);
};
