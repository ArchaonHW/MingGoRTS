#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceManager.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingMemoryOptimizer;

// Performance category
UENUM(BlueprintType)
enum class EMingPerformanceCategory : uint8
{
    Rendering             UMETA(DisplayName = "Rendering"),
    Physics               UMETA(DisplayName = "Physics"),
    AI                    UMETA(DisplayName = "AI"),
    Audio                 UMETA(DisplayName = "Audio"),
    Network               UMETA(DisplayName = "Network"),
    UI                    UMETA(DisplayName = "UI"),
    Animation             UMETA(DisplayName = "Animation"),
    Input                 UMETA(DisplayName = "Input"),
    Memory                UMETA(DisplayName = "Memory"),
    Storage               UMETA(DisplayName = "Storage"),
    Custom                UMETA(DisplayName = "Custom")
};

// Performance level
UENUM(BlueprintType)
enum class EMingPerformanceLevel : uint8
{
    Low                   UMETA(DisplayName = "Low"),
    Medium                UMETA(DisplayName = "Medium"),
    High                  UMETA(DisplayName = "High"),
    Ultra                 UMETA(DisplayName = "Ultra"),
    Epic                  UMETA(DisplayName = "Epic"),
    Custom                UMETA(DisplayName = "Custom")
};

// Optimization mode
UENUM(BlueprintType)
enum class EMingOptimizationMode : uint8
{
    Manual                UMETA(DisplayName = "Manual"),
    Auto                  UMETA(DisplayName = "Auto"),
    Adaptive              UMETA(DisplayName = "Adaptive"),
    Aggressive            UMETA(DisplayName = "Aggressive"),
    Conservative          UMETA(DisplayName = "Conservative"),
    Custom                UMETA(DisplayName = "Custom")
};

// Performance metrics
USTRUCT(BlueprintType)
struct FMingPersonalPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float FrameRate;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float FrameTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float GameThreadTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float RenderThreadTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float GPUFrameTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MemoryUsedMB;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MemoryAvailableMB;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 DrawCalls;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 TrianglesRendered;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 ShadowCastingLights;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 TextureMemoryMB;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 MeshMemoryMB;

    FMingPersonalPerformanceMetrics()
        : FrameRate(60.0f)
        , FrameTime(16.67f)
        , GameThreadTime(0.0f)
        , RenderThreadTime(0.0f)
        , GPUFrameTime(0.0f)
        , MemoryUsedMB(0.0f)
        , MemoryAvailableMB(0.0f)
        , DrawCalls(0)
        , TrianglesRendered(0)
        , ShadowCastingLights(0)
        , TextureMemoryMB(0)
        , MeshMemoryMB(0)
    {}
};

// Performance settings
USTRUCT(BlueprintType)
struct FMingPerformanceSettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    EMingPerformanceLevel TargetLevel;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    EMingOptimizationMode OptimizationMode;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float TargetFrameRate;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    int32 MaxDrawCalls;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    int32 MaxShadowLights;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float TextureQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float MeshLODQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float ViewDistanceScale;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    bool bEnableDynamicResolution;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float DynamicResolutionMin;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    bool bAutoAdjustQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    int32 MaxMemoryMB;

    FMingPerformanceSettings()
        : TargetLevel(EMingPerformanceLevel::High)
        , OptimizationMode(EMingOptimizationMode::Adaptive)
        , TargetFrameRate(60.0f)
        , MaxDrawCalls(3000)
        , MaxShadowLights(4)
        , TextureQuality(1.0f)
        , MeshLODQuality(1.0f)
        , ViewDistanceScale(1.0f)
        , bEnableDynamicResolution(true)
        , DynamicResolutionMin(0.7f)
        , bAutoAdjustQuality(true)
        , MaxMemoryMB(2048)
    {}
};

// Performance warning
USTRUCT(BlueprintType)
struct FMingPerformanceWarning
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    EMingPerformanceCategory Category;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    FString WarningMessage;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float Severity;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    FString SuggestedAction;

    FMingPerformanceWarning()
        : Category(EMingPerformanceCategory::Custom)
        , WarningMessage(TEXT(""))
        , Severity(0.0f)
        , Timestamp(0.0f)
        , SuggestedAction(TEXT(""))
    {}
};

// Optimization result
USTRUCT(BlueprintType)
struct FMingOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    EMingPerformanceCategory Category;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    FString OptimizationName;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float PerformanceGain;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    bool bSuccessful;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    FString Details;

    FMingOptimizationResult()
        : Category(EMingPerformanceCategory::Custom)
        , OptimizationName(TEXT(""))
        , PerformanceGain(0.0f)
        , bSuccessful(false)
        , Details(TEXT(""))
    {}
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceWarning, const FMingPerformanceWarning&, Warning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceMetricsUpdated, const FMingPersonalPerformanceMetrics&, Metrics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptimizationApplied, const FMingOptimizationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPerformanceLevelChanged);

/**
 * ?§èƒ½ç®¡ç??? * ??Ž§?Œå„ª?–é??²æ€§èƒ½
 */
UCLASS(ClassGroup = (MingGoRTS), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingPerformanceManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceManager();

    // Initialize/Shutdown
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Initialize();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Shutdown();

    // Performance monitoring
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopPerformanceMonitoring();

    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsMonitoringActive() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    FMingPersonalPerformanceMetrics GetCurrentMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void GetPerformanceHistory(TArray<FMingPersonalPerformanceMetrics>& OutHistory, int32 MaxSamples = 100) const;

    // Settings
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPerformanceSettings(const FMingPerformanceSettings& NewSettings);

    UFUNCTION(BlueprintPure, Category = "Performance")
    FMingPerformanceSettings GetPerformanceSettings() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFrameRate(float TargetFPS);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPerformanceLevel(EMingPerformanceLevel NewLevel);

    UFUNCTION(BlueprintPure, Category = "Performance")
    EMingPerformanceLevel GetCurrentPerformanceLevel() const;

    // Optimization
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FMingOptimizationResult> RunAutoOptimization();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    FMingOptimizationResult OptimizeCategory(EMingPerformanceCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ApplyOptimization(const FString& OptimizationName);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void RevertLastOptimization();

    // Memory management
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void TriggerGarbageCollection();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void FlushMemoryPools();

    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetMemoryUsagePercent() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetMemoryBudget(int32 MaxMemoryMB);

    // Rendering optimization
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetViewDistanceScale(float Scale);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetShadowQuality(float Quality);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTextureQuality(float Quality);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetLODQuality(float Quality);

    // Adaptive quality
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void EnableAdaptiveQuality(bool bEnable);

    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsAdaptiveQualityEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetAdaptiveQualityTarget(float TargetFPS);

    // Warnings
    UFUNCTION(BlueprintPure, Category = "Performance")
    TArray<FMingPerformanceWarning> GetActiveWarnings() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ClearWarnings();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetWarningThreshold(EMingPerformanceCategory Category, float Threshold);

    // Profiling
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartProfiling(const FString& ProfileName);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopProfiling(const FString& ProfileName);

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void GetProfileReport(const FString& ProfileName, FString& OutReport) const;

    // Benchmarking
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartBenchmark();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    FMingPersonalPerformanceMetrics StopBenchmark();

    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsBenchmarking() const;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetPerformanceReport() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void LogPerformanceStats();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ResetPerformanceStats();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Performance Events")
    FOnPerformanceWarning OnPerformanceWarning;

    UPROPERTY(BlueprintAssignable, Category = "Performance Events")
    FOnPerformanceMetricsUpdated OnMetricsUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Performance Events")
    FOnOptimizationApplied OnOptimizationApplied;

    UPROPERTY(BlueprintAssignable, Category = "Performance Events")
    FOnPerformanceLevelChanged OnPerformanceLevelChanged;

private:
    UPROPERTY()
    FMingPersonalPerformanceMetrics CurrentMetrics;

    UPROPERTY()
    FMingPerformanceSettings CurrentSettings;

    UPROPERTY()
    TArray<FMingPersonalPerformanceMetrics> MetricsHistory;

    UPROPERTY()
    TArray<FMingPerformanceWarning> ActiveWarnings;

    UPROPERTY()
    TArray<FMingOptimizationResult> AppliedOptimizations;

    UPROPERTY()
    EMingPerformanceLevel CurrentLevel;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    bool bIsBenchmarking;

    UPROPERTY()
    float BenchmarkStartTime;

    UPROPERTY()
    FMingPersonalPerformanceMetrics BenchmarkStartMetrics;

    UPROPERTY()
    FTimerHandle MonitoringTimer;

    UPROPERTY()
    TMap<EMingPerformanceCategory, float> WarningThresholds;

    // Internal functions
    void UpdateMetrics();
    void CheckPerformanceWarnings();
    void ApplyPerformanceLevel(EMingPerformanceLevel Level);
    void AutoOptimizeIfNeeded();
    void RecordMetrics();
    bool ShouldTriggerOptimization() const;
    FMingOptimizationResult RunSpecificOptimization(EMingPerformanceCategory Category, const FString& OptName);
    void UpdateDynamicResolution();
    float CalculateAverageFrameRate() const;
};
