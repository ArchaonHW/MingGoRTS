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
        ProfileID = TEXT("");
        ProfileName = TEXT("");
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
        Category = TEXT("");
        Message = TEXT("");
        Level = EDebugLevel::Basic;
        Timestamp = 0.0f;
        Context = TEXT("");
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
        OptimizationType = TEXT("");
        BeforeValue = 0.0f;
        AfterValue = 0.0f;
        Improvement = 0.0f;
        bSuccessful = false;
        Description = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceMetricsUpdated, const FPerformanceMetrics&, Metrics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptimizationCompleted, const FOptimizationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugMessage, const FDebugInfo&, DebugInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileStarted, const FString&, ProfileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileCompleted, const FString&, ProfileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceAlert, const FString&, AlertMessage);

/**
 * MingGoRTS Â¢ûÂº∑?ßËÉΩ?™Â??åË™øË©¶Á≥ªÁµ? * ?ïÁ??äÊà≤?ßËÉΩ??éß?ÅÂÑ™?ñ„ÄÅË™øË©¶Â??ÜÊ?Á≠âÂ??? */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSPerformanceEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceEnhancedSystem();

    // ?ùÂ??ñÂ?Âº∑ÊÄßËÉΩ?™Â?Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void InitializePerformanceEnhancedSystem(UWorld* World);

    // ?¥Êñ∞Â¢ûÂº∑?ßËÉΩ?™Â?Á≥ªÁµ±ÔºàÊ?ÂπÄË™øÁî®Ôº?    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void UpdatePerformanceEnhancedSystem(float DeltaTime);

    // ?≤Â??∂Â??ßËÉΩ?áÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCurrentMetrics() const;

    // ?≤Â??πÂ?È°ûÂà•?ÑÊÄßËÉΩ?áÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCategoryMetrics(EPerformanceCategory Category) const;

    // ?ãÂ??ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString StartProfiling(const FString& ProfileName, EPerformanceLevel TargetLevel, EOptimizationMode Mode);

    // ?úÊ≠¢?ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool StopProfiling(const FString& ProfileID);

    // ?´Â??ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool PauseProfiling(const FString& ProfileID);

    // ?¢Âæ©?ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ResumeProfiling(const FString& ProfileID);

    // ?≤Â??ßËÉΩ?ÜÊ?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceProfile GetProfile(const FString& ProfileID) const;

    // ?≤Â??Ä?âÊÄßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FPerformanceProfile> GetAllProfiles() const;

    // ?™Â??ßËÉΩ?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AutoOptimize(EPerformanceCategory Category, EPerformanceLevel TargetLevel);

    // ?πÈ??ßËÉΩ?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BatchOptimize(const TArray<EPerformanceCategory>& Categories, EPerformanceLevel TargetLevel);

    // ?ãÂ??ßËÉΩ?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ManualOptimize(EPerformanceCategory Category, const TMap<FString, FString>& Settings);

    // ?≤Â??™Â?Âª∫Ë≠∞
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FOptimizationResult> GetOptimizationSuggestions() const;

    // ?âÁî®?™Â?Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ApplyOptimizationSettings(const TMap<FString, FString>& Settings);

    // ?çÁΩÆ?™Â?Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ResetOptimizationSettings();

    // Ë®≠ÁΩÆË™øË©¶Á¥öÂà•
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetDebugLevel(EDebugLevel Level);

    // ?≤Â?Ë™øË©¶Á¥öÂà•
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    EDebugLevel GetDebugLevel() const;

    // Ê∑ªÂ?Ë™øË©¶Ê∂àÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AddDebugMessage(const FString& Category, const FString& Message, EDebugLevel Level, const FString& Context = TEXT(""));

    // ?≤Â?Ë™øË©¶‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FDebugInfo> GetDebugInfo(const FString& Category = TEXT("")) const;

    // Ê∏ÖÈô§Ë™øË©¶‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearDebugInfo(const FString& Category = TEXT(""));

    // Â∞éÂá∫?ßËÉΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ExportPerformanceData(const FString& FilePath, const FString& ProfileID = TEXT(""));

    // Â∞éÂÖ•?ßËÉΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ImportPerformanceData(const FString& FilePath);

    // ?≤Â??ßËÉΩÁµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<FString, float> GetPerformanceStatistics() const;

    // ?≤Â??ßËÉΩ?±Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString GeneratePerformanceReport(const FString& ProfileID = TEXT("")) const;

    // Ë®≠ÁΩÆ?ßËÉΩË≠¶Â??æÂÄ?    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceAlertThresholds(const TMap<EPerformanceCategory, float>& Thresholds);

    // ?≤Â??ßËÉΩË≠¶Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FString> GetPerformanceAlerts() const;

    // Ê∏ÖÈô§?ßËÉΩË≠¶Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearPerformanceAlerts();

    // ?üÁî®/Á¶ÅÁî®?ßËÉΩ??éß
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceMonitoringEnabled(bool bEnabled);

    // ?≤Â??ßËÉΩ??éß?Ä??    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool IsPerformanceMonitoringEnabled() const;

    // Ë®≠ÁΩÆ??éß?ìÈ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetMonitoringInterval(float Interval);

    // ?≤Â???éß?ìÈ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float GetMonitoringInterval() const;

public:
    // ‰∫ã‰ª∂ÂßîË?
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
    // ?ïÁ??ßËÉΩ??éß
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceMonitoring(float DeltaTime);

    // ?ïÁ??ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessProfiling(float DeltaTime);

    // ?ïÁ??™Â??™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessAutoOptimization(float DeltaTime);

    // ?ïÁ??ßËÉΩË≠¶Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceAlerts(float DeltaTime);

    // ?∂È??ßËÉΩ?áÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics CollectPerformanceMetrics() const;

    // ?ÜÊ??ßËÉΩ?∂È†∏
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<EPerformanceCategory> AnalyzePerformanceBottlenecks() const;

    // ?üÊ??™Â?Âª∫Ë≠∞
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult GenerateOptimizationSuggestion(EPerformanceCategory Category) const;

    // ?âÁî®Ê∏≤Ê??™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyRenderingOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®?©Á??™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyPhysicsOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®AI?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAIOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®?≥È†ª?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAudioOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®Á∂≤Áµ°?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyNetworkOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®UI?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyUIOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®?ïÁï´?™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAnimationOptimization(const TMap<FString, FString>& Settings);

    // ?âÁî®?ßÂ??™Â?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyMemoryOptimization(const TMap<FString, FString>& Settings);

    // È©óË??™Â?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ValidateOptimizationResult(const FOptimizationResult& Result) const;

    // Ë®òÈ??ßËÉΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void RecordPerformanceData(const FPerformanceMetrics& Metrics);

    // ?ÜÊ??ßËÉΩË∂®Âã¢
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<EPerformanceCategory, float> AnalyzePerformanceTrends() const;

    // ?êÊ∏¨?ßËÉΩË≤†Ë?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float PredictPerformanceLoad() const;

    // Âπ≥Ë°°?ßËÉΩË≤†Ë?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BalancePerformanceLoad();

protected:
    // ?∂Â?‰∏ñÁ?ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // ?∂Â??ßËÉΩ?áÊ?
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // ?ßËÉΩ?ÜÊ??†Â?
    UPROPERTY()
    TMap<FString, FPerformanceProfile> PerformanceProfiles;

    // Ë™øË©¶‰ø°ÊÅØ?†Â?
    UPROPERTY()
    TArray<FDebugInfo> DebugInfo;

    // ?ßËÉΩË≠¶Â??óË°®
    UPROPERTY()
    TArray<FString> PerformanceAlerts;

    // ?ßËÉΩË≠¶Â??æÂÄ?    UPROPERTY()
    TMap<EPerformanceCategory, float> AlertThresholds;

    // ?∂Â??ÜÊ?ID
    UPROPERTY()
    FString CurrentProfileID;

    // ?ßËÉΩ?¥Êñ∞?ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float PerformanceUpdateInterval;

    // ‰∏äÊ¨°?ßËÉΩ?¥Êñ∞?ÇÈ?
    UPROPERTY()
    float LastPerformanceUpdateTime;

    // ??éß?ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float MonitoringInterval;

    // ‰∏äÊ¨°??éß?ÇÈ?
    UPROPERTY()
    float LastMonitoringTime;

    // Ë™øË©¶Á¥öÂà•
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    EDebugLevel DebugLevel;

    // ?ØÂê¶?üÁî®?ßËÉΩ??éß
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceMonitoringEnabled;

    // ?ØÂê¶?üÁî®?™Â??™Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bAutoOptimizationEnabled;

    // ?ØÂê¶?üÁî®?ßËÉΩË≠¶Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceAlertsEnabled;

    // ?ÄÂ§ßË™øË©¶‰ø°?ØÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxDebugInfoCount;

    // ?ÄÂ§ßÊÄßËÉΩ?ÜÊ??∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxProfileCount;

private:
    // ?≤Â??ßËÉΩÈ°ûÂà•?çÁ®±
    FString GetPerformanceCategoryName(EPerformanceCategory Category) const;

    // ?≤Â??ßËÉΩÁ¥öÂà•?çÁ®±
    FString GetPerformanceLevelName(EPerformanceLevel Level) const;

    // ?≤Â??™Â?Ê®°Â??çÁ®±
    FString GetOptimizationModeName(EOptimizationMode Mode) const;

    // ?≤Â?Ë™øË©¶Á¥öÂà•?çÁ®±
    FString GetDebugLevelName(EDebugLevel Level) const;

    // ?üÊ??Ø‰?ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // È©óË??ßËÉΩ?áÊ?
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics) const;

    // È©óË??ßËÉΩ?ÜÊ?
    bool ValidatePerformanceProfile(const FPerformanceProfile& Profile) const;

    // ?ïÁ??ßËÉΩ?ØË™§
    void HandlePerformanceError(const FString& Operation, const FString& ErrorMessage);

    // Ê∏ÖÁ??°Ê??ßËÉΩ?∏Ê?
    void CleanupInvalidPerformanceData();

    // ?≤Â??ßËÉΩ?áÊ?Ê®°Êùø
    FPerformanceMetrics GetPerformanceMetricsTemplate() const;

    // ?≤Â??ßËÉΩ?ÜÊ?Ê®°Êùø
    FPerformanceProfile GetPerformanceProfileTemplate() const;

    // ?≤Â??™Â?ÁµêÊ?Ê®°Êùø
    FOptimizationResult GetOptimizationResultTemplate() const;

    // Ë®òÈ??ßËÉΩÁµ±Ë?
    void RecordPerformanceStatistics();

    // ?ïÁ??ßËÉΩ?∂È†∏
    void HandlePerformanceBottlenecks();

    // ?™Â?Ê∏≤Ê??ßËÉΩ
    void OptimizeRenderingPerformance();

    // ?™Â??©Á??ßËÉΩ
    void OptimizePhysicsPerformance();

    // ?™Â?AI?ßËÉΩ
    void OptimizeAIPerformance();

    // ?™Â??≥È†ª?ßËÉΩ
    void OptimizeAudioPerformance();

    // ?™Â?Á∂≤Áµ°?ßËÉΩ
    void OptimizeNetworkPerformance();

    // ?™Â?UI?ßËÉΩ
    void OptimizeUIPerformance();

    // ?™Â??ïÁï´?ßËÉΩ
    void OptimizeAnimationPerformance();

    // ?™Â??ßÂ??ßËÉΩ
    void OptimizeMemoryPerformance();

    // ?≤Â?ÈªòË??ßËÉΩË®≠ÁΩÆ
    TMap<FString, FString> GetDefaultPerformanceSettings() const;

    // ?âÁî®?ßÂà∂?∞ÂëΩ‰ª?    void ApplyConsoleCommand(const FString& Command);

    // ?≤Â??ßÂà∂?∞Ë??èÂÄ?    FString GetConsoleVariable(const FString& VariableName) const;

    // Ë®≠ÁΩÆ?ßÂà∂?∞Ë??èÂÄ?    void SetConsoleVariable(const FString& VariableName, const FString& Value);
};
