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
 * MingGoRTS 增強性能優化和調試系統
 * 處理遊戲性能監控、優化、調試和分析等功能
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSPerformanceEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceEnhancedSystem();

    // 初始化增強性能優化系統
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void InitializePerformanceEnhancedSystem(UWorld* World);

    // 更新增強性能優化系統（每幀調用）
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void UpdatePerformanceEnhancedSystem(float DeltaTime);

    // 獲取當前性能指標
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCurrentMetrics() const;

    // 獲取特定類別的性能指標
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics GetCategoryMetrics(EPerformanceCategory Category) const;

    // 開始性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString StartProfiling(const FString& ProfileName, EPerformanceLevel TargetLevel, EOptimizationMode Mode);

    // 停止性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool StopProfiling(const FString& ProfileID);

    // 暫停性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool PauseProfiling(const FString& ProfileID);

    // 恢復性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ResumeProfiling(const FString& ProfileID);

    // 獲取性能分析結果
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceProfile GetProfile(const FString& ProfileID) const;

    // 獲取所有性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FPerformanceProfile> GetAllProfiles() const;

    // 自動性能優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AutoOptimize(EPerformanceCategory Category, EPerformanceLevel TargetLevel);

    // 批量性能優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BatchOptimize(const TArray<EPerformanceCategory>& Categories, EPerformanceLevel TargetLevel);

    // 手動性能優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ManualOptimize(EPerformanceCategory Category, const TMap<FString, FString>& Settings);

    // 獲取優化建議
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FOptimizationResult> GetOptimizationSuggestions() const;

    // 應用優化設置
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ApplyOptimizationSettings(const TMap<FString, FString>& Settings);

    // 重置優化設置
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ResetOptimizationSettings();

    // 設置調試級別
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetDebugLevel(EDebugLevel Level);

    // 獲取調試級別
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    EDebugLevel GetDebugLevel() const;

    // 添加調試消息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void AddDebugMessage(const FString& Category, const FString& Message, EDebugLevel Level, const FString& Context = TEXT(""));

    // 獲取調試信息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FDebugInfo> GetDebugInfo(const FString& Category = TEXT("")) const;

    // 清除調試信息
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearDebugInfo(const FString& Category = TEXT(""));

    // 導出性能數據
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ExportPerformanceData(const FString& FilePath, const FString& ProfileID = TEXT(""));

    // 導入性能數據
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ImportPerformanceData(const FString& FilePath);

    // 獲取性能統計
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<FString, float> GetPerformanceStatistics() const;

    // 獲取性能報告
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FString GeneratePerformanceReport(const FString& ProfileID = TEXT("")) const;

    // 設置性能警告閾值
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceAlertThresholds(const TMap<EPerformanceCategory, float>& Thresholds);

    // 獲取性能警告
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<FString> GetPerformanceAlerts() const;

    // 清除性能警告
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ClearPerformanceAlerts();

    // 啟用/禁用性能監控
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetPerformanceMonitoringEnabled(bool bEnabled);

    // 獲取性能監控狀態
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool IsPerformanceMonitoringEnabled() const;

    // 設置監控間隔
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void SetMonitoringInterval(float Interval);

    // 獲取監控間隔
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float GetMonitoringInterval() const;

public:
    // 事件委託
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
    // 處理性能監控
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceMonitoring(float DeltaTime);

    // 處理性能分析
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessProfiling(float DeltaTime);

    // 處理自動優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessAutoOptimization(float DeltaTime);

    // 處理性能警告
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void ProcessPerformanceAlerts(float DeltaTime);

    // 收集性能指標
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FPerformanceMetrics CollectPerformanceMetrics() const;

    // 分析性能瓶頸
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TArray<EPerformanceCategory> AnalyzePerformanceBottlenecks() const;

    // 生成優化建議
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult GenerateOptimizationSuggestion(EPerformanceCategory Category) const;

    // 應用渲染優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyRenderingOptimization(const TMap<FString, FString>& Settings);

    // 應用物理優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyPhysicsOptimization(const TMap<FString, FString>& Settings);

    // 應用AI優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAIOptimization(const TMap<FString, FString>& Settings);

    // 應用音頻優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAudioOptimization(const TMap<FString, FString>& Settings);

    // 應用網絡優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyNetworkOptimization(const TMap<FString, FString>& Settings);

    // 應用UI優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyUIOptimization(const TMap<FString, FString>& Settings);

    // 應用動畫優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyAnimationOptimization(const TMap<FString, FString>& Settings);

    // 應用內存優化
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    FOptimizationResult ApplyMemoryOptimization(const TMap<FString, FString>& Settings);

    // 驗證優化結果
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    bool ValidateOptimizationResult(const FOptimizationResult& Result) const;

    // 記錄性能數據
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void RecordPerformanceData(const FPerformanceMetrics& Metrics);

    // 分析性能趨勢
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    TMap<EPerformanceCategory, float> AnalyzePerformanceTrends() const;

    // 預測性能負載
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    float PredictPerformanceLoad() const;

    // 平衡性能負載
    UFUNCTION(BlueprintCallable, Category = "Performance Enhanced System")
    void BalancePerformanceLoad();

protected:
    // 當前世界引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // 當前性能指標
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // 性能分析映射
    UPROPERTY()
    TMap<FString, FPerformanceProfile> PerformanceProfiles;

    // 調試信息映射
    UPROPERTY()
    TArray<FDebugInfo> DebugInfo;

    // 性能警告列表
    UPROPERTY()
    TArray<FString> PerformanceAlerts;

    // 性能警告閾值
    UPROPERTY()
    TMap<EPerformanceCategory, float> AlertThresholds;

    // 當前分析ID
    UPROPERTY()
    FString CurrentProfileID;

    // 性能更新間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float PerformanceUpdateInterval;

    // 上次性能更新時間
    UPROPERTY()
    float LastPerformanceUpdateTime;

    // 監控間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    float MonitoringInterval;

    // 上次監控時間
    UPROPERTY()
    float LastMonitoringTime;

    // 調試級別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    EDebugLevel DebugLevel;

    // 是否啟用性能監控
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceMonitoringEnabled;

    // 是否啟用自動優化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bAutoOptimizationEnabled;

    // 是否啟用性能警告
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    bool bPerformanceAlertsEnabled;

    // 最大調試信息數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxDebugInfoCount;

    // 最大性能分析數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Enhanced System")
    int32 MaxProfileCount;

private:
    // 獲取性能類別名稱
    FString GetPerformanceCategoryName(EPerformanceCategory Category) const;

    // 獲取性能級別名稱
    FString GetPerformanceLevelName(EPerformanceLevel Level) const;

    // 獲取優化模式名稱
    FString GetOptimizationModeName(EOptimizationMode Mode) const;

    // 獲取調試級別名稱
    FString GetDebugLevelName(EDebugLevel Level) const;

    // 生成唯一ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // 驗證性能指標
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics) const;

    // 驗證性能分析
    bool ValidatePerformanceProfile(const FPerformanceProfile& Profile) const;

    // 處理性能錯誤
    void HandlePerformanceError(const FString& Operation, const FString& ErrorMessage);

    // 清理無效性能數據
    void CleanupInvalidPerformanceData();

    // 獲取性能指標模板
    FPerformanceMetrics GetPerformanceMetricsTemplate() const;

    // 獲取性能分析模板
    FPerformanceProfile GetPerformanceProfileTemplate() const;

    // 獲取優化結果模板
    FOptimizationResult GetOptimizationResultTemplate() const;

    // 記錄性能統計
    void RecordPerformanceStatistics();

    // 處理性能瓶頸
    void HandlePerformanceBottlenecks();

    // 優化渲染性能
    void OptimizeRenderingPerformance();

    // 優化物理性能
    void OptimizePhysicsPerformance();

    // 優化AI性能
    void OptimizeAIPerformance();

    // 優化音頻性能
    void OptimizeAudioPerformance();

    // 優化網絡性能
    void OptimizeNetworkPerformance();

    // 優化UI性能
    void OptimizeUIPerformance();

    // 優化動畫性能
    void OptimizeAnimationPerformance();

    // 優化內存性能
    void OptimizeMemoryPerformance();

    // 獲取默認性能設置
    TMap<FString, FString> GetDefaultPerformanceSettings() const;

    // 應用控制台命令
    void ApplyConsoleCommand(const FString& Command);

    // 獲取控制台變量值
    FString GetConsoleVariable(const FString& VariableName) const;

    // 設置控制台變量值
    void SetConsoleVariable(const FString& VariableName, const FString& Value);
};
