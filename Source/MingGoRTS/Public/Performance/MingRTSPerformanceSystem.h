#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPerformanceSystem.generated.h"

/**
 * 性能監控類別
 */
UENUM(BlueprintType)
enum class EPerformanceCategory: uint8 {
    FPS                   UMETA(DisplayName = "幀率"),
    Memory                UMETA(DisplayName = "內存"),
    CPU                   UMETA(DisplayName = "CPU"),
    GPU                   UMETA(DisplayName = "GPU"),
    Rendering             UMETA(DisplayName = "渲染"),
    Physics               UMETA(DisplayName = "物理"),
    AI                    UMETA(DisplayName = "AI"),
    Audio                 UMETA(DisplayName = "音頻"),
    Network               UMETA(DisplayName = "網絡"),
    Loading               UMETA(DisplayName = "加載")
};

/**
 * 性能級別
 */
UENUM(BlueprintType)
enum class EPerformanceLevel: uint8 {
    Excellent             UMETA(DisplayName = "優秀"),
    Good                  UMETA(DisplayName = "良好"),
    Acceptable            UMETA(DisplayName = "可接受"),
    Poor                  UMETA(DisplayName = "較差"),
    Critical              UMETA(DisplayName = "嚴重")
};

/**
 * 優化設置
 */
UENUM(BlueprintType)
enum class EOptimizationPreset: uint8 {
    Ultra                 UMETA(DisplayName = "極致"),
    High                  UMETA(DisplayName = "高"),
    Medium                UMETA(DisplayName = "中"),
    Low                   UMETA(DisplayName = "低"),
    Minimal               UMETA(DisplayName = "最低"),
    Custom                UMETA(DisplayName = "自定義")
};

/**
 * 性能指標數據
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FrameTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsageMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsagePercent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CPULoadPercent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GPULoadPercent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DrawCalls;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriangleCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MeshCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TextureMemoryMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PendingGCObjects;
    
    FPerformanceMetrics()
        : CurrentFPS(60.0f)
        , AverageFPS(60.0f)
        , MinFPS(60.0f)
        , MaxFPS(60.0f)
        , FrameTime(16.67f)
        , MemoryUsageMB(0.0f)
        , MemoryUsagePercent(0.0f)
        , CPULoadPercent(0.0f)
        , GPULoadPercent(0.0f)
        , DrawCalls(0)
        , TriangleCount(0)
        , MeshCount(0)
        , TextureMemoryMB(0)
        , PendingGCObjects(0)
    {}
};

/**
 * 性能預算設置
 */
USTRUCT(BlueprintType)
struct FPerformanceBudget
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxDrawCalls;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxTriangleCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxMemoryUsageMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxTextureMemoryMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAIAgents;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxParticles;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCPULoadPercent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxGPULoadPercent;
    
    FPerformanceBudget()
        : TargetFPS(60)
        , MinFPS(30)
        , MaxDrawCalls(3000)
        , MaxTriangleCount(500000)
        , MaxMemoryUsageMB(4096.0f)
        , MaxTextureMemoryMB(1024)
        , MaxAIAgents(100)
        , MaxParticles(10000)
        , MaxCPULoadPercent(80.0f)
        , MaxGPULoadPercent(90.0f)
    {}
};

/**
 * 優化建議
 */
USTRUCT(BlueprintType)
struct FOptimizationSuggestion
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPerformanceCategory Category;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SuggestedAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPerformanceLevel Priority;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExpectedImprovement;
    
    FOptimizationSuggestion()
        : Category(EPerformanceCategory::FPS)
        , Priority(EPerformanceLevel::Good)
        , ExpectedImprovement(0.0f)
    {}
};

/**
 * 性能歷史記錄
 */
USTRUCT(BlueprintType)
struct FPerformanceHistory
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPerformanceMetrics Metrics;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PlayerCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AIUnitCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BuildingCount;
    
    FPerformanceHistory()
        : PlayerCount(0)
        , AIUnitCount(0)
        , BuildingCount(0)
    {}
};

/**
 * 性能優化系統
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSPerformanceSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSPerformanceSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void InitializePerformanceSystem();
    
    // 開始性能監控
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartPerformanceMonitoring();
    
    // 停止性能監控
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopPerformanceMonitoring();
    
    // 獲取當前性能指標
    UFUNCTION(BlueprintCallable, Category = "Performance")
    FPerformanceMetrics GetCurrentMetrics() const;
    
    // 獲取性能預算
    UFUNCTION(BlueprintCallable, Category = "Performance")
    FPerformanceBudget GetPerformanceBudget() const;
    
    // 設置性能預算
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPerformanceBudget(const FPerformanceBudget& Budget);
    
    // 應用優化預設
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ApplyOptimizationPreset(EOptimizationPreset Preset);
    
    // 獲取優化建議
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FOptimizationSuggestion> GetOptimizationSuggestions() const;
    
    // 自動優化
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AutoOptimize();
    
    // 設置目標幀率
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(int32 TargetFPS);
    
    // 獲取目標幀率
    UFUNCTION(BlueprintCallable, Category = "Performance")
    int32 GetTargetFPS() const;
    
    // 設置渲染距離
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetDrawDistance(float Distance);
    
    // 獲取渲染距離
    UFUNCTION(BlueprintCallable, Category = "Performance")
    float GetDrawDistance() const;
    
    // 設置陰影質量
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetShadowQuality(int32 Quality);
    
    // 設置抗鋸齒
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetAntiAliasing(int32 Level);
    
    // 設置後處理質量
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPostProcessQuality(int32 Quality);
    
    // 設置紋理質量
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTextureQuality(int32 Quality);
    
    // 設置效果質量
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetEffectsQuality(int32 Quality);
    
    // 設置視野距離
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetViewDistance(int32 Quality);
    
    // 設置植被密度
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetFoliageDensity(float Density);
    
    // 執行垃圾回收
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ForceGarbageCollection();
    
    // 清理未使用資源
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void FlushUnusedResources();
    
    // 獲取性能歷史
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FPerformanceHistory> GetPerformanceHistory(float HoursBack) const;
    
    // 導出性能報告
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ExportPerformanceReport(const FString& FilePath) const;
    
    // 設置動態分辨率
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetDynamicResolutionEnabled(bool bEnabled);
    
    // 檢查性能問題
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool HasPerformanceIssues() const;
    
    // 獲取性能級別
    UFUNCTION(BlueprintCallable, Category = "Performance")
    EPerformanceLevel GetPerformanceLevel() const;
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceLevelChanged, EPerformanceLevel, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFPSDropped, float, CurrentFPS);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemoryWarning, float, MemoryUsageMB);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptimizationApplied);
    
    UPROPERTY(BlueprintAssignable, Category = "Performance|Events")
    FOnPerformanceLevelChanged OnPerformanceLevelChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Performance|Events")
    FOnFPSDropped OnFPSDropped;
    
    UPROPERTY(BlueprintAssignable, Category = "Performance|Events")
    FOnMemoryWarning OnMemoryWarning;
    
    UPROPERTY(BlueprintAssignable, Category = "Performance|Events")
    FOnOptimizationApplied OnOptimizationApplied;
    
private:
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;
    
    UPROPERTY()
    FPerformanceBudget Budget;
    
    UPROPERTY()
    TArray<FPerformanceHistory> PerformanceHistory;
    
    UPROPERTY()
    bool bMonitoringEnabled;
    
    UPROPERTY()
    bool bDynamicResolutionEnabled;
    
    UPROPERTY()
    int32 TargetFPS;
    
    UPROPERTY()
    float DrawDistance;
    
    UPROPERTY()
    float MonitoringInterval;
    
    FTimerHandle MonitoringTimerHandle;
    
    void StartMonitoringTimer();
    void StopMonitoringTimer();
    void OnMonitoringTick();
    void UpdateMetrics();
    void RecordHistory();
    void CheckBudgetViolation();
    EPerformanceLevel CalculatePerformanceLevel(const FPerformanceMetrics& Metrics) const;
    void ApplyAutomaticOptimizations();
    void ReduceDrawDistance();
    void ReduceShadowQuality();
    void ReduceEffects();
    void EnableLevelStreaming();
    void OptimizeAI();
    void ReduceTextureQuality();
    FString GetLevelName() const;
    void GetObjectCounts(int32& OutAIUnits, int32& OutBuildings) const;
};
