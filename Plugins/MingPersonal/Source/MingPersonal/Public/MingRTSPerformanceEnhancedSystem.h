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
enum class EPerformanceCategory: uint8 {
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
enum class EPerformanceLevel: uint8 {
    Low            UMETA(DisplayName = "Low"),
    Medium         UMETA(DisplayName = "Medium"),
    High           UMETA(DisplayName = "High"),
    Ultra          UMETA(DisplayName = "Ultra"),
    Epic           UMETA(DisplayName = "Epic"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EOptimizationMode: uint8 {
    Manual         UMETA(DisplayName = "Manual"),
    Automatic      UMETA(DisplayName = "Automatic"),
    Adaptive       UMETA(DisplayName = "Adaptive"),
    Aggressive     UMETA(DisplayName = "Aggressive"),
    Conservative   UMETA(DisplayName = "Conservative"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EDebugLevel: uint8 {
    None           UMETA(DisplayName = "None"),
    Basic          UMETA(DisplayName = "Basic"),
    Standard       UMETA(DisplayName = "Standard"),
    Verbose        UMETA(DisplayName = "Verbose"),
    Extreme        UMETA(DisplayName = "Extreme"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EProfilingState: uint8 {
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
        Context = TEXT(""); }
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
        Description = TEXT(""); }
};






DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceAlert, const FString&, AlertMessage);

/**
 * MingGoRTS �W�j??��???X?�ոըt?? * ???X?��??��X摧毀�u??�B�ո�?X???��?X */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSPerformanceEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceEnhancedSystem();

    // 目標數量�j�ʯ�摧毀�t��
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void InitializePerformanceEnhancedSystem(UWorld* World);

    // ??�s�W�j??��摧毀�t�Ρ]??�V�ե�??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void UpdatePerformanceEnhancedSystem(float DeltaTime);

    // 故事重要性?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCurrentMetrics() const;

    // 目標數量���O??�ʯ�摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCategoryMetrics(EPerformanceCategory Category) const;

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString StartProfiling(const FString& ProfileName, EPerformanceLevel TargetLevel, EOptimizationMode Mode);

    // ??��??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool StopProfiling(const FString& ProfileID);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool PauseProfiling(const FString& ProfileID);

    // ??�_??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ResumeProfiling(const FString& ProfileID);

    // ???X?��摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceProfile GetProfile(const FString& ProfileID) const;

    // 目標數量�ʯ�摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FPerformanceProfile> GetAllProfiles() const;

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AutoOptimize(EPerformanceCategory Category, EPerformanceLevel TargetLevel);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BatchOptimize(const TArray<EPerformanceCategory>& Categories, EPerformanceLevel TargetLevel);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ManualOptimize(EPerformanceCategory Category, const TMap<FString, FString>& Settings);

    // 目標數量��ĳ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FOptimizationResult> GetOptimizationSuggestions() const;

    // ??��摧毀�]�m
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ApplyOptimizationSettings(const TMap<FString, FString>& Settings);

    // ??�m摧毀�]�m
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ResetOptimizationSettings();

    // �]�m�ոկŧO
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetDebugLevel(EDebugLevel Level);

    // 摧毀�ոկŧO
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    EDebugLevel GetDebugLevel() const;

    // �K??�ոծ���
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AddDebugMessage(const FString& Category, const FString& Message, EDebugLevel Level, const FString& Context = TEXT("")};

    // 摧毀�ոիH��
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FDebugInfo> GetDebugInfo(const FString& Category = TEXT("")) const;

    // �M���ոիH��
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearDebugInfo(const FString& Category = TEXT("")};

    // �ɥX??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ExportPerformanceData(const FString& FilePath, const FString& ProfileID = TEXT("")};

    // �ɤJ??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ImportPerformanceData(const FString& FilePath);

    // ???X?���??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<FString, float> GetPerformanceStatistics() const;

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString GeneratePerformanceReport(const FString& ProfileID = TEXT("")) const;

    // �]�m??��ĵ?X???
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceAlertThresholds(const TMap<EPerformanceCategory, float>& Thresholds);

    // ???X?��ĵ??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FString> GetPerformanceAlerts() const;

    // �M��??��ĵ??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearPerformanceAlerts();

    // ??��/�T��??��X??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceMonitoringEnabled(bool bEnabled);

    // ???X?��X摧毀X
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool IsPerformanceMonitoringEnabled() const;

    // �]�mX摧毀??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetMonitoringInterval(float Interval);

    // 目標數量???
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float GetMonitoringInterval() const;

public:
    // �ƥ�e??
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
    // ???X?��X??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceMonitoring(float DeltaTime);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessProfiling(float DeltaTime);

    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessAutoOptimization(float DeltaTime);

    // ???X?��ĵ??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceAlerts(float DeltaTime);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics CollectPerformanceMetrics() const;

    // ???X?��??�V
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<EPerformanceCategory> AnalyzePerformanceBottlenecks() const;

    // 目標數量��ĳ
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult GenerateOptimizationSuggestion(EPerformanceCategory Category) const;

    // ??�δ�?X???
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyRenderingOptimization(const TMap<FString, FString>& Settings);

    // ??��目標數量
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyPhysicsOptimization(const TMap<FString, FString>& Settings);

    // ??��AI摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAIOptimization(const TMap<FString, FString>& Settings);

    // ??��??�W摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAudioOptimization(const TMap<FString, FString>& Settings);

    // ??�κ��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyNetworkOptimization(const TMap<FString, FString>& Settings);

    // ??��UI摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyUIOptimization(const TMap<FString, FString>& Settings);

    // ??��??�e摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAnimationOptimization(const TMap<FString, FString>& Settings);

    // ??��目標數量
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyMemoryOptimization(const TMap<FString, FString>& Settings);

    // ��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ValidateOptimizationResult(const FOptimizationResult& Result) const;

    // �O?X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void RecordPerformanceData(const FPerformanceMetrics& Metrics);

    // ???X?���Ͷ�
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<EPerformanceCategory, float> AnalyzePerformanceTrends() const;

    // ??��??��t??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float PredictPerformanceLoad() const;

    // ����??��t??
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BalancePerformanceLoad();

protected:
    // 摧毀�@??�ޥ�
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // ???X?��摧毀
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // ??��目標數量
    UPROPERTY()
    TMap<FString, FPerformanceProfile> PerformanceProfiles;

    // �ոիH��摧毀
    UPROPERTY()
    TArray<FDebugInfo> DebugInfo;

    // ??��ĵ?X?��
    UPROPERTY()
    TArray<FString> PerformanceAlerts;

    // ??��ĵ?X???
    UPROPERTY()
    TMap<EPerformanceCategory, float> AlertThresholds;

    // 目標數量ID
    UPROPERTY()
    FString CurrentProfileID;

    // ??��??�s摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float PerformanceUpdateInterval;

    // �W��??��??�s摧毀
    UPROPERTY()
    float LastPerformanceUpdateTime;

    // X摧毀??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float MonitoringInterval;

    // �W��X摧毀??
    UPROPERTY()
    float LastMonitoringTime;

    // �ոկŧO
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    EDebugLevel DebugLevel;

    // ??�_??��??��X??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceMonitoringEnabled;

    // ??�_??��目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bAutoOptimizationEnabled;

    // ??�_??��??��ĵ??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceAlertsEnabled;

    // ??�j�ոիH??��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxDebugInfoCount;

    // ??�j�ʯ�目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxProfileCount;

private:
    // ???X?�����O??��
    FString GetPerformanceCategoryName(EPerformanceCategory Category) const;

    // ???X?��ŧO??��
    FString GetPerformanceLevelName(EPerformanceLevel Level) const;

    // 目標數量��?X?��
    FString GetOptimizationModeName(EOptimizationMode Mode) const;

    // 摧毀�ոկŧO??��
    FString GetDebugLevelName(EDebugLevel Level) const;

    // 目標數量ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // ��?X?��摧毀
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics) const;

    // ��?X?��摧毀
    bool ValidatePerformanceProfile(const FPerformanceProfile& Profile) const;

    // ???X?��??�~
    void HandlePerformanceError(const FString& Operation, const FString& ErrorMessage);

    // �M?X??X?��摧毀
    void CleanupInvalidPerformanceData();

    // ???X?��摧毀�ҪO
    FPerformanceMetrics GetPerformanceMetricsTemplate() const;

    // ???X?��摧毀�ҪO
    FPerformanceProfile GetPerformanceProfileTemplate() const;

    // 目標數量��??�ҪO
    FOptimizationResult GetOptimizationResultTemplate() const;

    // �O?X?���??
    void RecordPerformanceStatistics();

    // ???X?��??�V
    void HandlePerformanceBottlenecks();

    // 摧毀��?X?��
    void OptimizeRenderingPerformance();

    // 故事重要性?��
    void OptimizePhysicsPerformance();

    // 摧毀AI??��
    void OptimizeAIPerformance();

    // ???X?�W??��
    void OptimizeAudioPerformance();

    // 摧毀���??��
    void OptimizeNetworkPerformance();

    // 摧毀UI??��
    void OptimizeUIPerformance();

    // ???X?�e??��
    void OptimizeAnimationPerformance();

    // 故事重要性?��
    void OptimizeMemoryPerformance();

    // 摧毀�q?X?��]�m
    TMap<FString, FString> GetDefaultPerformanceSettings() const;

    // ??��??��??�R??
    void ApplyConsoleCommand(const FString& Command);

    // ???X?��目標數量
    FString GetConsoleVariable(const FString& VariableName) const;

    // �]�m??��目標數量
    void SetConsoleVariable(const FString& VariableName, const FString& Value);
);

