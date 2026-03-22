#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSystemMonitorDashboard.generated.h"

UENUM(BlueprintType)
enum class EMonitorMetricType: uint8 {
    CPU,             // CPU使用率
    Memory,          // 內存使用率
    GPU,             // GPU使用率
    Network,         // 網絡使用率
    Disk,            // 磁盤使用率
    ProcessCount,    // 進程數量
    ThreadCount,     // 線程數量
    Temperature,     // 溫度
    PowerUsage,      // 功耗
    Custom          // 自定義指標
};

UENUM(BlueprintType)
enum class EAlertLevel: uint8 {
    Info,            // 信息
    Warning,         // 警告
    Critical,        // 關鍵
    Emergency        // 緊急
};

UENUM(BlueprintType)
enum class ESystemState: uint8 {
    Healthy,          // 健康
    Warning,          // 警告
    Critical,         // 關鍵
    Maintenance,      // 維護中
    Offline          // 離線
};

USTRUCT(BlueprintType)
struct FSystemMetric
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMonitorMetricType MetricType;

    UPROPERTY(BlueprintReadOnly)
    FString MetricName;

    UPROPERTY(BlueprintReadOnly)
    float CurrentValue;

    UPROPERTY(BlueprintReadOnly)
    float MinValue;

    UPROPERTY(BlueprintReadOnly)
    float MaxValue;

    UPROPERTY(BlueprintReadOnly)
    float AverageValue;

    UPROPERTY(BlueprintReadOnly)
    FString Unit;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;

    UPROPERTY(BlueprintReadOnly)
    TArray<float> HistoricalValues;

    UPROPERTY(BlueprintReadOnly)
    bool bIsHealthy;

    FSystemMetric()
    {
        MetricType = EMonitorMetricType::CPU;
        MetricName = TEXT("");
        CurrentValue = 0.0f;
        MinValue = 0.0f;
        MaxValue = 100.0f;
        AverageValue = 0.0f;
        Unit = TEXT("%");
        LastUpdated = FDateTime::Now();
        HistoricalValues.Empty();
        bIsHealthy = true;
    }
};

USTRUCT(BlueprintType)
struct FSystemAlert
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString AlertID;

    UPROPERTY(BlueprintReadOnly)
    EAlertLevel AlertLevel;

    UPROPERTY(BlueprintReadOnly)
    FString Title;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    EMonitorMetricType MetricType;

    UPROPERTY(BlueprintReadOnly)
    FString MetricName;

    UPROPERTY(BlueprintReadOnly)
    float TriggerValue;

    UPROPERTY(BlueprintReadOnly)
    float ThresholdValue;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendedAction;

    FSystemAlert()
    {
        AlertID = TEXT("");
        AlertLevel = EAlertLevel::Info;
        Title = TEXT("");
        Description = TEXT("");
        MetricType = EMonitorMetricType::CPU;
        MetricName = TEXT("");
        TriggerValue = 0.0f;
        ThresholdValue = 0.0f;
        Timestamp = FDateTime::Now();
        bIsActive = true;
        RecommendedAction = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FSystemStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESystemState OverallState;

    UPROPERTY(BlueprintReadOnly)
    FString SystemName;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastCheck;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveProcesses;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveThreads;

    UPROPERTY(BlueprintReadOnly)
    float SystemLoad;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAlerts;

    UPROPERTY(BlueprintReadOnly)
    int32 CriticalAlerts;

    UPROPERTY(BlueprintReadOnly)
    FString StatusMessage;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMonitorMetricType, FSystemMetric> Metrics;

    FSystemStatus()
    {
        OverallState = ESystemState::Healthy;
        SystemName = TEXT("MingGoRTS");
        LastCheck = FDateTime::Now();
        ActiveProcesses = 0;
        ActiveThreads = 0;
        SystemLoad = 0.0f;
        ActiveAlerts = 0;
        CriticalAlerts = 0;
        StatusMessage = TEXT("System operating normally");
        Metrics.Empty();
    }
};

USTRUCT(BlueprintType)
struct FMonitorThreshold
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMonitorMetricType MetricType;

    UPROPERTY(BlueprintReadOnly)
    float WarningThreshold;

    UPROPERTY(BlueprintReadOnly)
    float CriticalThreshold;

    UPROPERTY(BlueprintReadOnly)
    float EmergencyThreshold;

    UPROPERTY(BlueprintReadOnly)
    bool bIsEnabled;

    FMonitorThreshold()
    {
        MetricType = EMonitorMetricType::CPU;
        WarningThreshold = 70.0f;
        CriticalThreshold = 85.0f;
        EmergencyThreshold = 95.0f;
        bIsEnabled = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemAlert, const FSystemAlert&, Alert);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemStateChanged, ESystemState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMetricUpdated, EMonitorMetricType, MetricType, const FSystemMetric&, Metric);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceIssueDetected, const FString&, Issue);

/**
 * 系統監控儀表板
 * 提供實時系統狀態監控和性能分析
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSSystemMonitorDashboard : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSystemMonitorDashboard();

    // 初始化系統監控
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    bool InitializeSystemMonitor();

    // 關閉系統監控
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void ShutdownSystemMonitor();

    // 開始監控
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void StartMonitoring();

    // 停止監控
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void StopMonitoring();

    // 獲取系統狀態
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FSystemStatus GetSystemStatus() const;

    // 獲取特定指標
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FSystemMetric GetMetric(EMonitorMetricType MetricType) const;

    // 獲取所有指標
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<FSystemMetric> GetAllMetrics() const;

    // 設置監控閾值
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void SetThreshold(EMonitorMetricType MetricType, float Warning, float Critical, float Emergency);

    // 獲取監控閾值
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FMonitorThreshold GetThreshold(EMonitorMetricType MetricType) const;

    // 創建警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FString CreateAlert(EAlertLevel Level, const FString& Title, const FString& Description, EMonitorMetricType MetricType, float TriggerValue);

    // 獲取活動警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<FSystemAlert> GetActiveAlerts() const;

    // 獲取所有警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<FSystemAlert> GetAllAlerts() const;

    // 關閉警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    bool CloseAlert(const FString& AlertID);

    // 清除所有警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void ClearAllAlerts();

    // 設置監控間隔
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void SetMonitoringInterval(float IntervalSeconds);

    // 獲取監控間隔
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    float GetMonitoringInterval() const;

    // 啟用自動警報
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void SetAutoAlertEnabled(bool bEnabled);

    // 獲取自動警報狀態
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    bool IsAutoAlertEnabled() const;

    // 獲取性能報告
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FString GeneratePerformanceReport() const;

    // 獲取系統健康評分
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    float GetSystemHealthScore() const;

    // 獲取性能趨勢
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<float> GetPerformanceTrend(EMonitorMetricType MetricType, int32 TimeWindowMinutes) const;

    // 預測系統負載
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    float PredictSystemLoad(float TimeHorizonMinutes) const;

    // 檢測性能問題
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<FString> DetectPerformanceIssues() const;

    // 獲取推薦優化操作
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    TArray<FString> GetRecommendedOptimizations() const;

    // 導出監控數據
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    FString ExportMonitoringData() const;

    // 導入監控配置
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    bool ImportMonitoringConfig(const FString& Config);

    // 重置監控數據
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void ResetMonitoringData();

    // 設置警報回調
    UFUNCTION(BlueprintCallable, Category = "System Monitor")
    void SetAlertCallback(TFunction<void(const FSystemAlert&)> Callback);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "System Monitor")
    FOnSystemAlert OnSystemAlert;

    UPROPERTY(BlueprintAssignable, Category = "System Monitor")
    FOnSystemStateChanged OnSystemStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "System Monitor")
    FOnMetricUpdated OnMetricUpdated;

    UPROPERTY(BlueprintAssignable, Category = "System Monitor")
    FOnPerformanceIssueDetected OnPerformanceIssueDetected;

protected:
    // 更新系統指標
    virtual void UpdateSystemMetrics();

    // 檢查警報條件
    virtual void CheckAlertConditions();

    // 計算系統狀態
    virtual void CalculateSystemState();

    // 收集系統信息
    virtual void CollectSystemInformation();

    // 更新CPU指標
    virtual void UpdateCPUMetric();

    // 更新內存指標
    virtual void UpdateMemoryMetric();

    // 更新GPU指標
    virtual void UpdateGPUMetric();

    // 更新網絡指標
    virtual void UpdateNetworkMetric();

    // 更新磁盤指標
    virtual void UpdateDiskMetric();

    // 更新進程指標
    virtual void UpdateProcessMetric();

    // 更新線程指標
    virtual void UpdateThreadMetric();

    // 更新溫度指標
    virtual void UpdateTemperatureMetric();

    // 更新功耗指標
    virtual void UpdatePowerMetric();

    // 處理警報
    virtual void HandleAlert(const FSystemAlert& Alert);

    // 生成警報ID
    virtual FString GenerateAlertID();

    // 計算健康評分
    virtual float CalculateHealthScore();

    // 分析性能趨勢
    virtual void AnalyzePerformanceTrends();

    // 檢測異常模式
    virtual void DetectAnomalousPatterns();

    // 生成優化建議
    virtual void GenerateOptimizationRecommendations();

    // 記錄監控事件
    virtual void LogMonitoringEvent(const FString& Event, const FString& Details);

    // 保存監控數據
    virtual void SaveMonitoringData();

    // 載入監控數據
    virtual void LoadMonitoringData();

    // 清理歷史數據
    virtual void CleanupHistoricalData();

private:
    // 系統狀態
    UPROPERTY()
    FSystemStatus CurrentStatus;

    // 系統指標
    UPROPERTY()
    TMap<EMonitorMetricType, FSystemMetric> SystemMetrics;

    // 監控閾值
    UPROPERTY()
    TMap<EMonitorMetricType, FMonitorThreshold> MonitorThresholds;

    // 系統警報
    UPROPERTY()
    TArray<FSystemAlert> SystemAlerts;

    // 系統狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    bool bAutoAlertEnabled;

    // 配置參數
    UPROPERTY()
    float MonitoringInterval;

    UPROPERTY()
    int32 MaxHistoricalDataPoints;

    UPROPERTY()
    int32 MaxAlertHistory;

    // 計時器
    UPROPERTY()
    FTimerHandle MonitoringTimer;

    UPROPERTY()
    FTimerHandle AlertCheckTimer;

    UPROPERTY()
    FTimerHandle DataCleanupTimer;

    // 回調函數
    UPROPERTY()
    TFunction<void(const FSystemAlert&)> AlertCallback;

    // 性能數據
    UPROPERTY()
    TMap<EMonitorMetricType, TArray<float>> PerformanceHistory;

    UPROPERTY()
    TArray<float> SystemLoadHistory;

    UPROPERTY()
    TArray<float> HealthScoreHistory;

    // 統計數據
    UPROPERTY()
    int32 TotalAlertsGenerated;

    UPROPERTY()
    int32 PerformanceIssuesDetected;

    UPROPERTY()
    FDateTime LastSystemCheck;

    UPROPERTY()
    FDateTime LastAlertCheck;
};
