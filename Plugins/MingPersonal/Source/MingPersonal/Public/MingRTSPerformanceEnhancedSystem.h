#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "MingRTSPerformanceEnhancedSystem.generated.h"

UENUM(BlueprintType)
enum class EPerformanceCategory : uint8
{
    Rendering      UMETA(DisplayName = "Rendering"),
    Physics        UMETA(DisplayName = "Physics"),
    AI             UMETA(DisplayName = "AI"),
    Audio          UMETA(DisplayName = "Audio"),
    Network        UMETA(DisplayName = "Network"),
    UI             UMETA(DisplayName = "UI"),
    Animation      UMETA(DisplayName = "Animation"),
    Input          UMETA(DisplayName = "Input"),
    Memory         UMETA(DisplayName = "Memory"),
    Storage        UMETA(DisplayName = "Storage"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EPerformanceLevel : uint8
{
    Low            UMETA(DisplayName = "Low"),
    Medium         UMETA(DisplayName = "Medium"),
    High           UMETA(DisplayName = "High"),
    Ultra          UMETA(DisplayName = "Ultra"),
    Epic           UMETA(DisplayName = "Epic"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EOptimizationMode : uint8
{
    Manual         UMETA(DisplayName = "Manual"),
    Automatic      UMETA(DisplayName = "Automatic"),
    Adaptive       UMETA(DisplayName = "Adaptive"),
    Aggressive     UMETA(DisplayName = "Aggressive"),
    Conservative   UMETA(DisplayName = "Conservative"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EDebugLevel : uint8
{
    None           UMETA(DisplayName = "None"),
    Basic          UMETA(DisplayName = "Basic"),
    Standard       UMETA(DisplayName = "Standard"),
    Verbose        UMETA(DisplayName = "Verbose"),
    Extreme        UMETA(DisplayName = "Extreme"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EProfilingState : uint8
{
    Stopped        UMETA(DisplayName = "Stopped"),
    Starting       UMETA(DisplayName = "Starting"),
    Running        UMETA(DisplayName = "Running"),
    Paused         UMETA(DisplayName = "Paused"),
    Stopping       UMETA(DisplayName = "Stopping"),
    Saving         UMETA(DisplayName = "Saving"),
    Custom         UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FMingPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    EPerformanceCategory Category;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float FrameRate;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float FrameTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float CPUUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float GPUUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int64 MemoryUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int64 MemoryAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float DrawCalls;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float Triangles;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float TextureMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float NetworkLatency;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float NetworkBandwidth;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int32 ActorCount;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int32 ComponentCount;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float Timestamp;

    FPerformanceMetrics()
    {
        Category = EPerformanceCategory::Rendering;
        FrameRate = 0.0f;
        FrameTime = 0.0f;
        CPUUsage = 0.0f;
        GPUUsage = 0.0f;
        MemoryUsage = 0;
        MemoryAvailable = 0;
        DrawCalls = 0.0f;
        Triangles = 0.0f;
        TextureMemory = 0.0f;
        NetworkLatency = 0.0f;
        NetworkBandwidth = 0.0f;
        ActorCount = 0;
        ComponentCount = 0;
        Timestamp = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FPerformanceProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    FString ProfileID;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    FString ProfileName;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    EPerformanceLevel TargetLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    EOptimizationMode OptimizationMode;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    TMap<EPerformanceCategory, float> CategoryTargets;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    TMap<FString, FString> Settings;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    float StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    float Duration;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Profile")
    TArray<FPerformanceMetrics> Metrics;

    FPerformanceProfile()
    {
        ProfileID = TEXT(""};
        ProfileName = TEXT(""};
        TargetLevel = EPerformanceLevel::Medium;
        OptimizationMode = EOptimizationMode::Automatic;
        bIsActive = false;
        StartTime = 0.0f;
        Duration = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FDebugInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    FString Category;

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    EDebugLevel Level;

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    FString Context;

    UPROPERTY(BlueprintReadOnly, Category = "Debug Info")
    TMap<FString, FString> Metadata;

    FDebugInfo()
    {
        Category = TEXT(""};
        Message = TEXT(""};
        Level = EDebugLevel::Basic;
        Timestamp = 0.0f;
        Context = TEXT(""};
    }
};

USTRUCT(BlueprintType)
struct FOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    EPerformanceCategory Category;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    FString OptimizationType;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    float BeforeValue;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    float AfterValue;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    float Improvement;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    bool bSuccessful;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Result")
    TArray<FString> AppliedChanges;

    FOptimizationResult()
    {
        Category = EPerformanceCategory::Rendering;
        OptimizationType = TEXT(""};
        BeforeValue = 0.0f;
        AfterValue = 0.0f;
        Improvement = 0.0f;
        bSuccessful = false;
        Description = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceMetricsUpdated, const FPerformanceMetrics&, Metrics};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptimizationCompleted, const FOptimizationResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugMessage, const FDebugInfo&, DebugInfo};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileStarted, const FString&, ProfileID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileCompleted, const FString&, ProfileID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceAlert, const FString&, AlertMessage};

/**
 * MingGoRTS 增強?�能?��X�調試系�? * ?��X�戲?�能X��?�優?�、調試�X��?等�X */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSPerformanceEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceEnhancedSystem(};

    // ?��X��?強性能?��?系統
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void InitializePerformanceEnhancedSystem(UWorld* World};

    // ?�新增強?�能?��?系統（�?幀調用�?    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void UpdatePerformanceEnhancedSystem(float DeltaTime};

    // ?��X��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCurrentMetrics() const;

    // ?��X��?類別?�性能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCategoryMetrics(EPerformanceCategory Category) const;

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString StartProfiling(const FString& ProfileName, EPerformanceLevel TargetLevel, EOptimizationMode Mode};

    // ?�止?�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool StopProfiling(const FString& ProfileID};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool PauseProfiling(const FString& ProfileID};

    // ?�復?�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ResumeProfiling(const FString& ProfileID};

    // ?��X�能?��?結�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceProfile GetProfile(const FString& ProfileID) const;

    // ?��X�?�性能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FPerformanceProfile> GetAllProfiles() const;

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AutoOptimize(EPerformanceCategory Category, EPerformanceLevel TargetLevel};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BatchOptimize(const TArray<EPerformanceCategory>& Categories, EPerformanceLevel TargetLevel};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ManualOptimize(EPerformanceCategory Category, const TMap<FString, FString>& Settings};

    // ?��X��?建議
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FOptimizationResult> GetOptimizationSuggestions() const;

    // ?�用?��?設置
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ApplyOptimizationSettings(const TMap<FString, FString>& Settings};

    // ?�置?��?設置
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ResetOptimizationSettings(};

    // 設置調試級別
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetDebugLevel(EDebugLevel Level};

    // ?��?調試級別
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    EDebugLevel GetDebugLevel() const;

    // 添�?調試消息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AddDebugMessage(const FString& Category, const FString& Message, EDebugLevel Level, const FString& Context = TEXT("")};

    // ?��?調試信息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FDebugInfo> GetDebugInfo(const FString& Category = TEXT("")) const;

    // 清除調試信息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearDebugInfo(const FString& Category = TEXT("")};

    // 導出?�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ExportPerformanceData(const FString& FilePath, const FString& ProfileID = TEXT("")};

    // 導入?�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ImportPerformanceData(const FString& FilePath};

    // ?��X�能統�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<FString, float> GetPerformanceStatistics() const;

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString GeneratePerformanceReport(const FString& ProfileID = TEXT("")) const;

    // 設置?�能警�X��?    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceAlertThresholds(const TMap<EPerformanceCategory, float>& Thresholds};

    // ?��X�能警�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FString> GetPerformanceAlerts() const;

    // 清除?�能警�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearPerformanceAlerts(};

    // ?�用/禁用?�能X��
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceMonitoringEnabled(bool bEnabled};

    // ?��X�能X��?�X    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool IsPerformanceMonitoringEnabled() const;

    // 設置X��?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetMonitoringInterval(float Interval};

    // ?��X��?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float GetMonitoringInterval() const;

public:
    // 事件委�?
    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnPerformanceMetricsUpdated OnPerformanceMetricsUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnOptimizationCompleted OnOptimizationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnDebugMessage OnDebugMessage;

    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnProfileStarted OnProfileStarted;

    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnProfileCompleted OnProfileCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Performance Enhanced System Events")
    FOnPerformanceAlert OnPerformanceAlert;

protected:
    // ?��X�能X��
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceMonitoring(float DeltaTime};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessProfiling(float DeltaTime};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessAutoOptimization(float DeltaTime};

    // ?��X�能警�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceAlerts(float DeltaTime};

    // ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics CollectPerformanceMetrics() const;

    // ?��X�能?�頸
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<EPerformanceCategory> AnalyzePerformanceBottlenecks() const;

    // ?��X��?建議
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult GenerateOptimizationSuggestion(EPerformanceCategory Category) const;

    // ?�用渲�X��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyRenderingOptimization(const TMap<FString, FString>& Settings};

    // ?�用?��X��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyPhysicsOptimization(const TMap<FString, FString>& Settings};

    // ?�用AI?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAIOptimization(const TMap<FString, FString>& Settings};

    // ?�用?�頻?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAudioOptimization(const TMap<FString, FString>& Settings};

    // ?�用網絡?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyNetworkOptimization(const TMap<FString, FString>& Settings};

    // ?�用UI?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyUIOptimization(const TMap<FString, FString>& Settings};

    // ?�用?�畫?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAnimationOptimization(const TMap<FString, FString>& Settings};

    // ?�用?��X��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyMemoryOptimization(const TMap<FString, FString>& Settings};

    // 驗�X��?結�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ValidateOptimizationResult(const FOptimizationResult& Result) const;

    // 記�X�能?��?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void RecordPerformanceData(const FPerformanceMetrics& Metrics};

    // ?��X�能趨勢
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<EPerformanceCategory, float> AnalyzePerformanceTrends() const;

    // ?�測?�能負�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float PredictPerformanceLoad() const;

    // 平衡?�能負�?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BalancePerformanceLoad(};

protected:
    // ?��?世�?引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // ?��X�能?��?
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // ?�能?��X��?
    UPROPERTY()
    TMap<FString, FPerformanceProfile> PerformanceProfiles;

    // 調試信息?��?
    UPROPERTY()
    TArray<FDebugInfo> DebugInfo;

    // ?�能警�X�表
    UPROPERTY()
    TArray<FString> PerformanceAlerts;

    // ?�能警�X��?    UPROPERTY()
    TMap<EPerformanceCategory, float> AlertThresholds;

    // ?��X��?ID
    UPROPERTY()
    FString CurrentProfileID;

    // ?�能?�新?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float PerformanceUpdateInterval;

    // 上次?�能?�新?��?
    UPROPERTY()
    float LastPerformanceUpdateTime;

    // X��?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float MonitoringInterval;

    // 上次X��?��?
    UPROPERTY()
    float LastMonitoringTime;

    // 調試級別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    EDebugLevel DebugLevel;

    // ?�否?�用?�能X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceMonitoringEnabled;

    // ?�否?�用?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bAutoOptimizationEnabled;

    // ?�否?�用?�能警�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceAlertsEnabled;

    // ?�大調試信?�數X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxDebugInfoCount;

    // ?�大性能?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxProfileCount;

private:
    // ?��X�能類別?�稱
    FString GetPerformanceCategoryName(EPerformanceCategory Category) const;

    // ?��X�能級別?�稱
    FString GetPerformanceLevelName(EPerformanceLevel Level) const;

    // ?��X��?模�X�稱
    FString GetOptimizationModeName(EOptimizationMode Mode) const;

    // ?��?調試級別?�稱
    FString GetDebugLevelName(EDebugLevel Level) const;

    // ?��X��?ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // 驗�X�能?��?
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics) const;

    // 驗�X�能?��?
    bool ValidatePerformanceProfile(const FPerformanceProfile& Profile) const;

    // ?��X�能?�誤
    void HandlePerformanceError(const FString& Operation, const FString& ErrorMessage};

    // 清�X��X�能?��?
    void CleanupInvalidPerformanceData(};

    // ?��X�能?��?模板
    FPerformanceMetrics GetPerformanceMetricsTemplate() const;

    // ?��X�能?��?模板
    FPerformanceProfile GetPerformanceProfileTemplate() const;

    // ?��X��?結�?模板
    FOptimizationResult GetOptimizationResultTemplate() const;

    // 記�X�能統�?
    void RecordPerformanceStatistics(};

    // ?��X�能?�頸
    void HandlePerformanceBottlenecks(};

    // ?��?渲�X�能
    void OptimizeRenderingPerformance(};

    // ?��X��X�能
    void OptimizePhysicsPerformance(};

    // ?��?AI?�能
    void OptimizeAIPerformance(};

    // ?��X�頻?�能
    void OptimizeAudioPerformance(};

    // ?��?網絡?�能
    void OptimizeNetworkPerformance(};

    // ?��?UI?�能
    void OptimizeUIPerformance(};

    // ?��X�畫?�能
    void OptimizeAnimationPerformance(};

    // ?��X��X�能
    void OptimizeMemoryPerformance(};

    // ?��?默�X�能設置
    TMap<FString, FString> GetDefaultPerformanceSettings() const;

    // ?�用?�制?�命�?    void ApplyConsoleCommand(const FString& Command};

    // ?��X�制?��X��?    FString GetConsoleVariable(const FString& VariableName) const;

    // 設置?�制?��X��?    void SetConsoleVariable(const FString& VariableName, const FString& Value};
};

