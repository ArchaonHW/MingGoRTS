#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSPerformanceMonitor.generated.h"

UENUM(BlueprintType)
enum class EPerformanceMetricType : uint8
{
    CPU,                // CPU使用率
    Memory,             // 內存使用量
    GPU,                // GPU使用率
    Network,            // 網絡延遲/帶寬
    Disk,               // 磁盤I/O
    FrameRate,          // 幀率
    RenderTime,         // 渲染時間
    GameThread,         // 遊戲線程時間
    RenderThread,       // 渲染線程時間
    Audio,              // 音頻處理時間
    Physics,            // 物理計算時間
    AI,                 // AI處理時間
    Animation,          // 動畫計算時間
    Custom              // 自定義指標
};

UENUM(BlueprintType)
enum class EPerformanceAlertLevel : uint8
{
    Normal,             // 正常範圍
    Warning,            // 警告
    Critical,           // 嚴重
    Emergency           // 緊急
};

USTRUCT(BlueprintType)
struct FPerformanceMetric
{
    GENERATED_BODY()

    UPROPERTY()
    EPerformanceMetricType MetricType;

    UPROPERTY()
    float Value;

    UPROPERTY()
    FString Unit;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    TMap<FString, FString> AdditionalData;

    FPerformanceMetric()
        : MetricType(EPerformanceMetricType::Custom)
        , Value(0.0f)
        , Timestamp(FDateTime::Now())
    {}
};

USTRUCT(BlueprintType)
struct FPerformanceThreshold
{
    GENERATED_BODY()

    UPROPERTY()
    EPerformanceMetricType MetricType;

    UPROPERTY()
    float WarningThreshold;

    UPROPERTY()
    float CriticalThreshold;

    UPROPERTY()
    float EmergencyThreshold;

    UPROPERTY()
    bool bEnabled;

    FPerformanceThreshold()
        : MetricType(EPerformanceMetricType::Custom)
        , WarningThreshold(70.0f)
        , CriticalThreshold(85.0f)
        , EmergencyThreshold(95.0f)
        , bEnabled(true)
    {}
};

USTRUCT(BlueprintType)
struct FPerformanceAlert
{
    GENERATED_BODY()

    UPROPERTY()
    EPerformanceAlertLevel AlertLevel;

    UPROPERTY()
    FString AlertMessage;

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    EPerformanceMetricType MetricType;

    UPROPERTY()
    float CurrentValue;

    UPROPERTY()
    float ThresholdValue;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    bool bAcknowledged;

    FPerformanceAlert()
        : AlertLevel(EPerformanceAlertLevel::Normal)
        , MetricType(EPerformanceMetricType::Custom)
        , CurrentValue(0.0f)
        , ThresholdValue(0.0f)
        , Timestamp(FDateTime::Now())
        , bAcknowledged(false)
    {}
};

/**
 * 系統性能監控和診斷工具
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPerformanceMonitor();

    // 監控控制
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    bool StartMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void StopMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    bool IsMonitoring() const { return bIsMonitoring; }

    // 指標收集
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void RecordMetric(EPerformanceMetricType MetricType, float Value, const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void RecordCustomMetric(const FString& MetricName, float Value, const FString& Unit, const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    TArray<FPerformanceMetric> GetMetrics(EPerformanceMetricType MetricType, int32 MaxCount = 100) const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    TArray<FPerformanceMetric> GetSystemMetrics(const FString& SystemName, int32 MaxCount = 100) const;

    // 閾值管理
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void SetThreshold(EPerformanceMetricType MetricType, float Warning, float Critical, float Emergency);

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    FPerformanceThreshold GetThreshold(EPerformanceMetricType MetricType) const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void EnableThreshold(EPerformanceMetricType MetricType, bool bEnabled);

    // 警報系統
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    TArray<FPerformanceAlert> GetActiveAlerts() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void AcknowledgeAlert(const FDateTime& AlertTimestamp);

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void ClearAlerts();

    // 性能報告
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    FString GeneratePerformanceReport(const FDateTime& StartTime, const FDateTime& EndTime) const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    FString GenerateSystemReport(const FString& SystemName) const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    bool SavePerformanceReport(const FString& FilePath, const FDateTime& StartTime, const FDateTime& EndTime) const;

    // 診斷工具
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    TArray<FString> DiagnosePerformanceIssues() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    FString GetSystemHealthScore() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    TArray<FString> GetOptimizationSuggestions() const;

    // 實時監控
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    float GetCurrentCPUUsage() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    float GetCurrentMemoryUsage() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    float GetCurrentFrameRate() const;

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    float GetCurrentGPUUsage() const;

    // 配置
    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void SetMonitoringInterval(float IntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void SetMaxMetricHistory(int32 MaxHistory);

    UFUNCTION(BlueprintCallable, Category = "Performance Monitor")
    void EnableAutoDiagnostics(bool bEnabled);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPerformanceAlert, const FPerformanceAlert&, Alert, bool, bIsNew);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMetricRecorded, EPerformanceMetricType, MetricType, float, Value, const FString&, SystemName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemHealthChanged, const FString&, HealthScore);

    UPROPERTY(BlueprintAssignable, Category = "Performance Monitor")
    FOnPerformanceAlert OnPerformanceAlert;

    UPROPERTY(BlueprintAssignable, Category = "Performance Monitor")
    FOnMetricRecorded OnMetricRecorded;

    UPROPERTY(BlueprintAssignable, Category = "Performance Monitor")
    FOnSystemHealthChanged OnSystemHealthChanged;

protected:
    // 內部方法
    virtual void BeginDestroy() override;
    void UpdateMetrics();
    void CheckThresholds();
    void ProcessAlerts();
    float CalculateHealthScore() const;

    // 系統指標收集
    void CollectSystemMetrics();
    void CollectGameMetrics();
    void CollectRenderMetrics();
    void CollectAudioMetrics();

private:
    // 監控狀態
    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    float MonitoringInterval;

    UPROPERTY()
    FTimerHandle MonitoringTimer;

    // 指標存儲
    UPROPERTY()
    TArray<FPerformanceMetric> MetricHistory;

    UPROPERTY()
    TMap<EPerformanceMetricType, FPerformanceThreshold> Thresholds;

    UPROPERTY()
    TArray<FPerformanceAlert> ActiveAlerts;

    // 配置
    UPROPERTY()
    int32 MaxMetricHistorySize;

    UPROPERTY()
    bool bAutoDiagnosticsEnabled;

    // 當前值緩存
    UPROPERTY()
    TMap<EPerformanceMetricType, float> CurrentValues;

    // 系統健康
    UPROPERTY()
    FString LastHealthScore;

    UPROPERTY()
    FDateTime LastHealthUpdate;

    // 統計數據
    UPROPERTY()
    TMap<EPerformanceMetricType, float> AverageValues;

    UPROPERTY()
    TMap<EPerformanceMetricType, float> PeakValues;

    UPROPERTY()
    TMap<EPerformanceMetricType, int32> SampleCounts;
};
