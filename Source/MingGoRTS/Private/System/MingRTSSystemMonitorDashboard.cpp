#include "MingRTSSystemMonitorDashboard.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformMisc.h"
#include "Misc/ScopeLock.h"

UMingRTSSystemMonitorDashboard::UMingRTSSystemMonitorDashboard()
{
    bIsInitialized = false;
    bIsMonitoring = false;
    bAutoAlertEnabled = true;
    MonitoringInterval = 1.0f;
    MaxHistoricalDataPoints = 1000;
    MaxAlertHistory = 100;
    TotalAlertsGenerated = 0;
    PerformanceIssuesDetected = 0;
    LastSystemCheck = FDateTime::Now();
    LastAlertCheck = FDateTime::Now();
}

bool UMingRTSSystemMonitorDashboard::InitializeSystemMonitor()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("System Monitor already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing System Monitor Dashboard..."));

    // 初始化系統狀態
    CurrentStatus = FSystemStatus();
    CurrentStatus.SystemName = TEXT("MingGoRTS System");
    CurrentStatus.OverallState = ESystemState::Healthy;

    // 初始化系統指標
    SystemMetrics.Empty();
    
    // 初始化CPU指標
    FSystemMetric CPUMetric;
    CPUMetric.MetricType = EMonitorMetricType::CPU;
    CPUMetric.MetricName = TEXT("CPU Usage");
    CPUMetric.Unit = TEXT("%");
    CPUMetric.MaxValue = 100.0f;
    SystemMetrics.Add(EMonitorMetricType::CPU, CPUMetric);

    // 初始化內存指標
    FSystemMetric MemoryMetric;
    MemoryMetric.MetricType = EMonitorMetricType::Memory;
    MemoryMetric.MetricName = TEXT("Memory Usage");
    MemoryMetric.Unit = TEXT("%");
    MemoryMetric.MaxValue = 100.0f;
    SystemMetrics.Add(EMonitorMetricType::Memory, MemoryMetric);

    // 初始化GPU指標
    FSystemMetric GPUMetric;
    GPUMetric.MetricType = EMonitorMetricType::GPU;
    GPUMetric.MetricName = TEXT("GPU Usage");
    GPUMetric.Unit = TEXT("%");
    GPUMetric.MaxValue = 100.0f;
    SystemMetrics.Add(EMonitorMetricType::GPU, GPUMetric);

    // 初始化網絡指標
    FSystemMetric NetworkMetric;
    NetworkMetric.MetricType = EMonitorMetricType::Network;
    NetworkMetric.MetricName = TEXT("Network Usage");
    NetworkMetric.Unit = TEXT("Mbps");
    NetworkMetric.MaxValue = 1000.0f;
    SystemMetrics.Add(EMonitorMetricType::Network, NetworkMetric);

    // 初始化磁盤指標
    FSystemMetric DiskMetric;
    DiskMetric.MetricType = EMonitorMetricType::Disk;
    DiskMetric.MetricName = TEXT("Disk Usage");
    DiskMetric.Unit = TEXT("%");
    DiskMetric.MaxValue = 100.0f;
    SystemMetrics.Add(EMonitorMetricType::Disk, DiskMetric);

    // 初始化進程指標
    FSystemMetric ProcessMetric;
    ProcessMetric.MetricType = EMonitorMetricType::ProcessCount;
    ProcessMetric.MetricName = TEXT("Process Count");
    ProcessMetric.Unit = TEXT("count");
    ProcessMetric.MaxValue = 1000.0f;
    SystemMetrics.Add(EMonitorMetricType::ProcessCount, ProcessMetric);

    // 初始化線程指標
    FSystemMetric ThreadMetric;
    ThreadMetric.MetricType = EMonitorMetricType::ThreadCount;
    ThreadMetric.MetricName = TEXT("Thread Count");
    ThreadMetric.Unit = TEXT("count");
    ThreadMetric.MaxValue = 10000.0f;
    SystemMetrics.Add(EMonitorMetricType::ThreadCount, ThreadMetric);

    // 初始化溫度指標
    FSystemMetric TemperatureMetric;
    TemperatureMetric.MetricType = EMonitorMetricType::Temperature;
    TemperatureMetric.MetricName = TEXT("System Temperature");
    TemperatureMetric.Unit = TEXT("°C");
    TemperatureMetric.MaxValue = 100.0f;
    SystemMetrics.Add(EMonitorMetricType::Temperature, TemperatureMetric);

    // 初始化功耗指標
    FSystemMetric PowerMetric;
    PowerMetric.MetricType = EMonitorMetricType::PowerUsage;
    PowerMetric.MetricName = TEXT("Power Usage");
    PowerMetric.Unit = TEXT("W");
    PowerMetric.MaxValue = 500.0f;
    SystemMetrics.Add(EMonitorMetricType::PowerUsage, PowerMetric);

    // 初始化監控閾值
    MonitorThresholds.Empty();
    
    // CPU閾值
    FMonitorThreshold CPUThreshold;
    CPUThreshold.MetricType = EMonitorMetricType::CPU;
    CPUThreshold.WarningThreshold = 70.0f;
    CPUThreshold.CriticalThreshold = 85.0f;
    CPUThreshold.EmergencyThreshold = 95.0f;
    CPUThreshold.bIsEnabled = true;
    MonitorThresholds.Add(EMonitorMetricType::CPU, CPUThreshold);

    // 內存閾值
    FMonitorThreshold MemoryThreshold;
    MemoryThreshold.MetricType = EMonitorMetricType::Memory;
    MemoryThreshold.WarningThreshold = 75.0f;
    MemoryThreshold.CriticalThreshold = 90.0f;
    MemoryThreshold.EmergencyThreshold = 95.0f;
    MemoryThreshold.bIsEnabled = true;
    MonitorThresholds.Add(EMonitorMetricType::Memory, MemoryThreshold);

    // GPU閾值
    FMonitorThreshold GPUThreshold;
    GPUThreshold.MetricType = EMonitorMetricType::GPU;
    GPUThreshold.WarningThreshold = 80.0f;
    GPUThreshold.CriticalThreshold = 90.0f;
    GPUThreshold.EmergencyThreshold = 98.0f;
    GPUThreshold.bIsEnabled = true;
    MonitorThresholds.Add(EMonitorMetricType::GPU, GPUThreshold);

    // 初始化性能歷史
    PerformanceHistory.Empty();
    for (int32 i = 0; i < 10; ++i)
    {
        EMonitorMetricType MetricType = static_cast<EMonitorMetricType>(i);
        PerformanceHistory.Add(MetricType, TArray<float>());
    }

    SystemLoadHistory.Empty();
    HealthScoreHistory.Empty();

    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        // 監控定時器
        World->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRTSSystemMonitorDashboard::UpdateSystemMetrics,
            MonitoringInterval,
            true
        );

        // 警報檢查定時器
        World->GetTimerManager().SetTimer(
            AlertCheckTimer,
            this,
            &UMingRTSSystemMonitorDashboard::CheckAlertConditions,
            5.0f,
            true
        );

        // 數據清理定時器
        World->GetTimerManager().SetTimer(
            DataCleanupTimer,
            this,
            &UMingRTSSystemMonitorDashboard::CleanupHistoricalData,
            300.0f,
            true
        );
    }

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("System Monitor Dashboard initialized successfully"));
    return true;
}

void UMingRTSSystemMonitorDashboard::ShutdownSystemMonitor()
{
    if (!bIsInitialized)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down System Monitor Dashboard..."));

    // 停止監控
    StopMonitoring();

    // 清除定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MonitoringTimer);
        World->GetTimerManager().ClearTimer(AlertCheckTimer);
        World->GetTimerManager().ClearTimer(DataCleanupTimer);
    }

    // 保存監控數據
    SaveMonitoringData();

    // 清空數據結構
    SystemMetrics.Empty();
    MonitorThresholds.Empty();
    SystemAlerts.Empty();
    PerformanceHistory.Empty();
    SystemLoadHistory.Empty();
    HealthScoreHistory.Empty();

    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("System Monitor Dashboard shutdown complete"));
}

void UMingRTSSystemMonitorDashboard::StartMonitoring()
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("System Monitor not initialized"));
        return;
    }

    if (bIsMonitoring)
    {
        UE_LOG(LogTemp, Warning, TEXT("Monitoring already started"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting system monitoring..."));

    bIsMonitoring = true;
    LastSystemCheck = FDateTime::Now();

    // 立即執行一次更新
    UpdateSystemMetrics();

    UE_LOG(LogTemp, Log, TEXT("System monitoring started"));
}

void UMingRTSSystemMonitorDashboard::StopMonitoring()
{
    if (!bIsMonitoring)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Stopping system monitoring..."));

    bIsMonitoring = false;

    UE_LOG(LogTemp, Log, TEXT("System monitoring stopped"));
}

FSystemStatus UMingRTSSystemMonitorDashboard::GetSystemStatus() const
{
    return CurrentStatus;
}

FSystemMetric UMingRTSSystemMonitorDashboard::GetMetric(EMonitorMetricType MetricType) const
{
    const FSystemMetric* Metric = SystemMetrics.Find(MetricType);
    return Metric ? *Metric : FSystemMetric();
}

TArray<FSystemMetric> UMingRTSSystemMonitorDashboard::GetAllMetrics() const
{
    TArray<FSystemMetric> Metrics;
    for (const auto& MetricPair : SystemMetrics)
    {
        Metrics.Add(MetricPair.Value);
    }
    return Metrics;
}

void UMingRTSSystemMonitorDashboard::SetThreshold(EMonitorMetricType MetricType, float Warning, float Critical, float Emergency)
{
    FMonitorThreshold Threshold;
    Threshold.MetricType = MetricType;
    Threshold.WarningThreshold = Warning;
    Threshold.CriticalThreshold = Critical;
    Threshold.EmergencyThreshold = Emergency;
    Threshold.bIsEnabled = true;
    
    MonitorThresholds.Add(MetricType, Threshold);
    
    UE_LOG(LogTemp, Log, TEXT("Thresholds set for metric type %d: W=%.1f, C=%.1f, E=%.1f"), 
        (int32)MetricType, Warning, Critical, Emergency);
}

FMonitorThreshold UMingRTSSystemMonitorDashboard::GetThreshold(EMonitorMetricType MetricType) const
{
    const FMonitorThreshold* Threshold = MonitorThresholds.Find(MetricType);
    return Threshold ? *Threshold : FMonitorThreshold();
}

FString UMingRTSSystemMonitorDashboard::CreateAlert(EAlertLevel Level, const FString& Title, const FString& Description, EMonitorMetricType MetricType, float TriggerValue)
{
    FSystemAlert Alert;
    Alert.AlertID = GenerateAlertID();
    Alert.AlertLevel = Level;
    Alert.Title = Title;
    Alert.Description = Description;
    Alert.MetricType = MetricType;
    Alert.TriggerValue = TriggerValue;
    Alert.Timestamp = FDateTime::Now();
    Alert.bIsActive = true;

    // 獲取閾值
    const FMonitorThreshold* Threshold = MonitorThresholds.Find(MetricType);
    if (Threshold)
    {
        Alert.ThresholdValue = Threshold->WarningThreshold;
    }

    // 獲取指標名稱
    const FSystemMetric* Metric = SystemMetrics.Find(MetricType);
    if (Metric)
    {
        Alert.MetricName = Metric->MetricName;
    }

    // 生成推薦操作
    switch (Level)
    {
    case EAlertLevel::Info:
        Alert.RecommendedAction = TEXT("Monitor the situation");
        break;
    case EAlertLevel::Warning:
        Alert.RecommendedAction = TEXT("Investigate and consider optimization");
        break;
    case EAlertLevel::Critical:
        Alert.RecommendedAction = TEXT("Immediate action required");
        break;
    case EAlertLevel::Emergency:
        Alert.RecommendedAction = TEXT("System intervention required");
        break;
    }

    SystemAlerts.Add(Alert);
    TotalAlertsGenerated++;

    // 處理警報
    HandleAlert(Alert);

    UE_LOG(LogTemp, Warning, TEXT("Alert created: %s - %s"), *Alert.AlertID, *Alert.Title);
    return Alert.AlertID;
}

TArray<FSystemAlert> UMingRTSSystemMonitorDashboard::GetActiveAlerts() const
{
    TArray<FSystemAlert> ActiveAlerts;
    for (const FSystemAlert& Alert : SystemAlerts)
    {
        if (Alert.bIsActive)
        {
            ActiveAlerts.Add(Alert);
        }
    }
    return ActiveAlerts;
}

TArray<FSystemAlert> UMingRTSSystemMonitorDashboard::GetAllAlerts() const
{
    return SystemAlerts;
}

bool UMingRTSSystemMonitorDashboard::CloseAlert(const FString& AlertID)
{
    for (FSystemAlert& Alert : SystemAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.bIsActive = false;
            UE_LOG(LogTemp, Log, TEXT("Alert closed: %s"), *AlertID);
            return true;
        }
    }
    return false;
}

void UMingRTSSystemMonitorDashboard::ClearAllAlerts()
{
    int32 AlertCount = SystemAlerts.Num();
    SystemAlerts.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared %d alerts"), AlertCount);
}

void UMingRTSSystemMonitorDashboard::SetMonitoringInterval(float IntervalSeconds)
{
    MonitoringInterval = IntervalSeconds;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRTSSystemMonitorDashboard::UpdateSystemMetrics,
            MonitoringInterval,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("Monitoring interval set to %.2f seconds"), IntervalSeconds);
}

float UMingRTSSystemMonitorDashboard::GetMonitoringInterval() const
{
    return MonitoringInterval;
}

void UMingRTSSystemMonitorDashboard::SetAutoAlertEnabled(bool bEnabled)
{
    bAutoAlertEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto alert %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingRTSSystemMonitorDashboard::IsAutoAlertEnabled() const
{
    return bAutoAlertEnabled;
}

FString UMingRTSSystemMonitorDashboard::GeneratePerformanceReport() const
{
    FString Report = TEXT("=== System Performance Report ===\n\n");
    Report += FString::Printf(TEXT("Generated: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("System State: %s\n"), *StaticEnum<ESystemState>()->GetValueAsString(CurrentStatus.OverallState));
    Report += FString::Printf(TEXT("System Load: %.1f%%\n"), CurrentStatus.SystemLoad);
    Report += FString::Printf(TEXT("Active Processes: %d\n"), CurrentStatus.ActiveProcesses);
    Report += FString::Printf(TEXT("Active Threads: %d\n"), CurrentStatus.ActiveThreads);
    Report += FString::Printf(TEXT("Active Alerts: %d\n"), CurrentStatus.ActiveAlerts);
    Report += FString::Printf(TEXT("Critical Alerts: %d\n\n"), CurrentStatus.CriticalAlerts);

    Report += TEXT("=== Metrics ===\n");
    for (const auto& MetricPair : SystemMetrics)
    {
        const FSystemMetric& Metric = MetricPair.Value;
        Report += FString::Printf(TEXT("%s: %.2f %s (Avg: %.2f)\n"), 
            *Metric.MetricName, Metric.CurrentValue, *Metric.Unit, Metric.AverageValue);
    }

    Report += TEXT("\n=== Recent Alerts ===\n");
    TArray<FSystemAlert> RecentAlerts = GetActiveAlerts();
    for (const FSystemAlert& Alert : RecentAlerts)
    {
        Report += FString::Printf(TEXT("[%s] %s: %s\n"), 
            *StaticEnum<EAlertLevel>()->GetValueAsString(Alert.AlertLevel), 
            *Alert.Title, *Alert.Description);
    }

    return Report;
}

float UMingRTSSystemMonitorDashboard::GetSystemHealthScore() const
{
    return CalculateHealthScore();
}

TArray<float> UMingRTSSystemMonitorDashboard::GetPerformanceTrend(EMonitorMetricType MetricType, int32 TimeWindowMinutes) const
{
    const TArray<float>* History = PerformanceHistory.Find(MetricType);
    if (!History || History->Num() == 0)
    {
        return TArray<float>();
    }

    // 計算需要的數據點數量
    int32 DataPointsPerMinute = 60; // 假設每秒一個數據點
    int32 RequiredPoints = TimeWindowMinutes * DataPointsPerMinute;
    
    // 返回最近的數據點
    TArray<float> Trend;
    int32 StartIndex = FMath::Max(0, History->Num() - RequiredPoints);
    for (int32 i = StartIndex; i < History->Num(); ++i)
    {
        Trend.Add((*History)[i]);
    }
    
    return Trend;
}

float UMingRTSSystemMonitorDashboard::PredictSystemLoad(float TimeHorizonMinutes) const
{
    const TArray<float> LoadHistory = SystemLoadHistory;
    if (LoadHistory.Num() < 2)
    {
        return CurrentStatus.SystemLoad;
    }

    // 簡單線性回歸預測
    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumX2 = 0.0f;
    int32 N = FMath::Min(LoadHistory.Num(), 60); // 使用最近60個數據點

    for (int32 i = 0; i < N; ++i)
    {
        float X = (float)i;
        float Y = LoadHistory[LoadHistory.Num() - N + i];
        SumX += X;
        SumY += Y;
        SumXY += X * Y;
        SumX2 += X * X;
    }

    float Slope = (N * SumXY - SumX * SumY) / (N * SumX2 - SumX * SumX);
    float Intercept = (SumY - Slope * SumX) / N;

    float PredictedLoad = Slope * (float)(N + TimeHorizonMinutes) + Intercept;
    return FMath::Clamp(PredictedLoad, 0.0f, 100.0f);
}

TArray<FString> UMingRTSSystemMonitorDashboard::DetectPerformanceIssues() const
{
    TArray<FString> Issues;

    // 檢查高CPU使用率
    const FSystemMetric* CPUMetric = SystemMetrics.Find(EMonitorMetricType::CPU);
    if (CPUMetric && CPUMetric->CurrentValue > 85.0f)
    {
        Issues.Add(TEXT("High CPU usage detected"));
    }

    // 檢查高內存使用率
    const FSystemMetric* MemoryMetric = SystemMetrics.Find(EMonitorMetricType::Memory);
    if (MemoryMetric && MemoryMetric->CurrentValue > 90.0f)
    {
        Issues.Add(TEXT("High memory usage detected"));
    }

    // 檢查高GPU使用率
    const FSystemMetric* GPUMetric = SystemMetrics.Find(EMonitorMetricType::GPU);
    if (GPUMetric && GPUMetric->CurrentValue > 90.0f)
    {
        Issues.Add(TEXT("High GPU usage detected"));
    }

    // 檢查系統負載
    if (CurrentStatus.SystemLoad > 80.0f)
    {
        Issues.Add(TEXT("High system load detected"));
    }

    // 檢查過多進程
    if (CurrentStatus.ActiveProcesses > 500)
    {
        Issues.Add(TEXT("Excessive process count detected"));
    }

    // 檢查過多線程
    if (CurrentStatus.ActiveThreads > 2000)
    {
        Issues.Add(TEXT("Excessive thread count detected"));
    }

    return Issues;
}

TArray<FString> UMingRTSSystemMonitorDashboard::GetRecommendedOptimizations() const
{
    TArray<FString> Recommendations;

    // 基於當前狀態生成建議
    if (CurrentStatus.SystemLoad > 70.0f)
    {
        Recommendations.Add(TEXT("Consider reducing system load by optimizing background processes"));
    }

    const FSystemMetric* CPUMetric = SystemMetrics.Find(EMonitorMetricType::CPU);
    if (CPUMetric && CPUMetric->CurrentValue > 75.0f)
    {
        Recommendations.Add(TEXT("Optimize CPU-intensive operations or upgrade hardware"));
    }

    const FSystemMetric* MemoryMetric = SystemMetrics.Find(EMonitorMetricType::Memory);
    if (MemoryMetric && MemoryMetric->CurrentValue > 80.0f)
    {
        Recommendations.Add(TEXT("Free up memory by closing unused applications or adding more RAM"));
    }

    const FSystemMetric* GPUMetric = SystemMetrics.Find(EMonitorMetricType::GPU);
    if (GPUMetric && GPUMetric->CurrentValue > 80.0f)
    {
        Recommendations.Add(TEXT("Reduce graphics quality settings or upgrade GPU"));
    }

    if (CurrentStatus.ActiveAlerts > 5)
    {
        Recommendations.Add(TEXT("Address active alerts to improve system stability"));
    }

    return Recommendations;
}

FString UMingRTSSystemMonitorDashboard::ExportMonitoringData() const
{
    FString Data = TEXT("{\n");
    Data += FString::Printf(TEXT("  \"SystemName\": \"%s\",\n"), *CurrentStatus.SystemName);
    Data += FString::Printf(TEXT("  \"SystemState\": \"%s\",\n"), *StaticEnum<ESystemState>()->GetValueAsString(CurrentStatus.OverallState));
    Data += FString::Printf(TEXT("  \"SystemLoad\": %.2f,\n"), CurrentStatus.SystemLoad);
    Data += FString::Printf(TEXT("  \"ActiveProcesses\": %d,\n"), CurrentStatus.ActiveProcesses);
    Data += FString::Printf(TEXT("  \"ActiveThreads\": %d,\n"), CurrentStatus.ActiveThreads);
    Data += FString::Printf(TEXT("  \"HealthScore\": %.2f,\n"), GetSystemHealthScore());
    Data += TEXT("  \"Metrics\": {\n");

    for (const auto& MetricPair : SystemMetrics)
    {
        const FSystemMetric& Metric = MetricPair.Value;
        Data += FString::Printf(TEXT("    \"%s\": {\n"), *Metric.MetricName);
        Data += FString::Printf(TEXT("      \"CurrentValue\": %.2f,\n"), Metric.CurrentValue);
        Data += FString::Printf(TEXT("      \"AverageValue\": %.2f,\n"), Metric.AverageValue);
        Data += FString::Printf(TEXT("      \"Unit\": \"%s\"\n"), *Metric.Unit);
        Data += TEXT("    },\n");
    }

    Data += TEXT("  }\n");
    Data += TEXT("}");

    return Data;
}

bool UMingRTSSystemMonitorDashboard::ImportMonitoringConfig(const FString& Config)
{
    // 簡化的配置導入
    if (Config.Contains(TEXT("\"WarningThreshold\"")))
    {
        // 解析並設置閾值
        UE_LOG(LogTemp, Log, TEXT("Monitoring configuration imported"));
        return true;
    }
    return false;
}

void UMingRTSSystemMonitorDashboard::ResetMonitoringData()
{
    // 重置所有指標
    for (auto& MetricPair : SystemMetrics)
    {
        FSystemMetric& Metric = MetricPair.Value;
        Metric.CurrentValue = 0.0f;
        Metric.AverageValue = 0.0f;
        Metric.HistoricalValues.Empty();
    }

    // 清空警報
    SystemAlerts.Empty();

    // 重置歷史數據
    for (auto& HistoryPair : PerformanceHistory)
    {
        HistoryPair.Value.Empty();
    }
    SystemLoadHistory.Empty();
    HealthScoreHistory.Empty();

    // 重置統計
    TotalAlertsGenerated = 0;
    PerformanceIssuesDetected = 0;

    UE_LOG(LogTemp, Log, TEXT("Monitoring data reset"));
}

void UMingRTSSystemMonitorDashboard::SetAlertCallback(TFunction<void(const FSystemAlert&)> Callback)
{
    AlertCallback = Callback;
}

void UMingRTSSystemMonitorDashboard::UpdateSystemMetrics()
{
    if (!bIsMonitoring)
    {
        return;
    }

    LastSystemCheck = FDateTime::Now();

    // 更新各項指標
    UpdateCPUMetric();
    UpdateMemoryMetric();
    UpdateGPUMetric();
    UpdateNetworkMetric();
    UpdateDiskMetric();
    UpdateProcessMetric();
    UpdateThreadMetric();
    UpdateTemperatureMetric();
    UpdatePowerMetric();

    // 計算系統狀態
    CalculateSystemState();

    // 更新系統負載歷史
    SystemLoadHistory.Add(CurrentStatus.SystemLoad);
    if (SystemLoadHistory.Num() > MaxHistoricalDataPoints)
    {
        SystemLoadHistory.RemoveAt(0);
    }

    // 更新健康評分歷史
    float HealthScore = CalculateHealthScore();
    HealthScoreHistory.Add(HealthScore);
    if (HealthScoreHistory.Num() > MaxHistoricalDataPoints)
    {
        HealthScoreHistory.RemoveAt(0);
    }

    // 分析性能趨勢
    AnalyzePerformanceTrends();

    // 檢測異常模式
    DetectAnomalousPatterns();
}

void UMingRTSSystemMonitorDashboard::CheckAlertConditions()
{
    if (!bAutoAlertEnabled || !bIsMonitoring)
    {
        return;
    }

    LastAlertCheck = FDateTime::Now();

    // 檢查每個指標的閾值
    for (const auto& MetricPair : SystemMetrics)
    {
        EMonitorMetricType MetricType = MetricPair.Key;
        const FSystemMetric& Metric = MetricPair.Value;

        const FMonitorThreshold* Threshold = MonitorThresholds.Find(MetricType);
        if (!Threshold || !Threshold->bIsEnabled)
        {
            continue;
        }

        // 檢查緊急閾值
        if (Metric.CurrentValue >= Threshold->EmergencyThreshold)
        {
            CreateAlert(
                EAlertLevel::Emergency,
                FString::Printf(TEXT("Emergency: %s"), *Metric.MetricName),
                FString::Printf(TEXT("%s has reached emergency level: %.2f %s"), 
                    *Metric.MetricName, Metric.CurrentValue, *Metric.Unit),
                MetricType,
                Metric.CurrentValue
            );
        }
        // 檢查關鍵閾值
        else if (Metric.CurrentValue >= Threshold->CriticalThreshold)
        {
            CreateAlert(
                EAlertLevel::Critical,
                FString::Printf(TEXT("Critical: %s"), *Metric.MetricName),
                FString::Printf(TEXT("%s has reached critical level: %.2f %s"), 
                    *Metric.MetricName, Metric.CurrentValue, *Metric.Unit),
                MetricType,
                Metric.CurrentValue
            );
        }
        // 檢查警告閾值
        else if (Metric.CurrentValue >= Threshold->WarningThreshold)
        {
            CreateAlert(
                EAlertLevel::Warning,
                FString::Printf(TEXT("Warning: %s"), *Metric.MetricName),
                FString::Printf(TEXT("%s has reached warning level: %.2f %s"), 
                    *Metric.MetricName, Metric.CurrentValue, *Metric.Unit),
                MetricType,
                Metric.CurrentValue
            );
        }
    }

    // 更新活動警報數量
    CurrentStatus.ActiveAlerts = GetActiveAlerts().Num();
    CurrentStatus.CriticalAlerts = 0;
    for (const FSystemAlert& Alert : SystemAlerts)
    {
        if (Alert.bIsActive && (Alert.AlertLevel == EAlertLevel::Critical || Alert.AlertLevel == EAlertLevel::Emergency))
        {
            CurrentStatus.CriticalAlerts++;
        }
    }
}

void UMingRTSSystemMonitorDashboard::CalculateSystemState()
{
    int32 CriticalIssues = 0;
    int32 WarningIssues = 0;

    // 檢查警報級別
    for (const FSystemAlert& Alert : SystemAlerts)
    {
        if (Alert.bIsActive)
        {
            if (Alert.AlertLevel == EAlertLevel::Critical || Alert.AlertLevel == EAlertLevel::Emergency)
            {
                CriticalIssues++;
            }
            else if (Alert.AlertLevel == EAlertLevel::Warning)
            {
                WarningIssues++;
            }
        }
    }

    // 檢查指標健康狀態
    for (const auto& MetricPair : SystemMetrics)
    {
        const FSystemMetric& Metric = MetricPair.Value;
        if (!Metric.bIsHealthy)
        {
            CriticalIssues++;
        }
    }

    // 確定系統狀態
    ESystemState OldState = CurrentStatus.OverallState;
    
    if (CriticalIssues > 0)
    {
        CurrentStatus.OverallState = ESystemState::Critical;
        CurrentStatus.StatusMessage = FString::Printf(TEXT("System has %d critical issues"), CriticalIssues);
    }
    else if (WarningIssues > 3)
    {
        CurrentStatus.OverallState = ESystemState::Warning;
        CurrentStatus.StatusMessage = FString::Printf(TEXT("System has %d warning issues"), WarningIssues);
    }
    else
    {
        CurrentStatus.OverallState = ESystemState::Healthy;
        CurrentStatus.StatusMessage = TEXT("System operating normally");
    }

    // 更新系統負載
    CurrentStatus.SystemLoad = GetSystemHealthScore();

    // 廣播狀態變化
    if (OldState != CurrentStatus.OverallState)
    {
        OnSystemStateChanged.Broadcast(CurrentStatus.OverallState);
    }
}

void UMingRTSSystemMonitorDashboard::CollectSystemInformation()
{
    // 收集系統基本信息
    CurrentStatus.LastCheck = FDateTime::Now();
    
    // 這裡可以添加更多系統信息收集
    // 例如：操作系統版本、硬件信息等
}

void UMingRTSSystemMonitorDashboard::UpdateCPUMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::CPU);
    if (!Metric) return;

    // 模擬CPU使用率（實際應使用系統API）
    static float CPUUsage = 0.0f;
    CPUUsage += (FMath::RandRange(-5.0f, 5.0f));
    CPUUsage = FMath::Clamp(CPUUsage, 0.0f, 100.0f);

    Metric->CurrentValue = CPUUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(CPUUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::CPU);
    if (History)
    {
        History->Add(CPUUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    // 廣播指標更新
    OnMetricUpdated.Broadcast(EMonitorMetricType::CPU, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateMemoryMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::Memory);
    if (!Metric) return;

    // 模擬內存使用率
    static float MemoryUsage = 60.0f;
    MemoryUsage += (FMath::RandRange(-2.0f, 2.0f));
    MemoryUsage = FMath::Clamp(MemoryUsage, 0.0f, 100.0f);

    Metric->CurrentValue = MemoryUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(MemoryUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::Memory);
    if (History)
    {
        History->Add(MemoryUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::Memory, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateGPUMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::GPU);
    if (!Metric) return;

    // 模擬GPU使用率
    static float GPUUsage = 30.0f;
    GPUUsage += (FMath::RandRange(-10.0f, 10.0f));
    GPUUsage = FMath::Clamp(GPUUsage, 0.0f, 100.0f);

    Metric->CurrentValue = GPUUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(GPUUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::GPU);
    if (History)
    {
        History->Add(GPUUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::GPU, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateNetworkMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::Network);
    if (!Metric) return;

    // 模擬網絡使用率
    static float NetworkUsage = 50.0f;
    NetworkUsage += (FMath::RandRange(-20.0f, 20.0f));
    NetworkUsage = FMath::Clamp(NetworkUsage, 0.0f, Metric->MaxValue);

    Metric->CurrentValue = NetworkUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(NetworkUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::Network);
    if (History)
    {
        History->Add(NetworkUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::Network, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateDiskMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::Disk);
    if (!Metric) return;

    // 模擬磁盤使用率
    static float DiskUsage = 45.0f;
    DiskUsage += (FMath::RandRange(-1.0f, 1.0f));
    DiskUsage = FMath::Clamp(DiskUsage, 0.0f, 100.0f);

    Metric->CurrentValue = DiskUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(DiskUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::Disk);
    if (History)
    {
        History->Add(DiskUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::Disk, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateProcessMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::ProcessCount);
    if (!Metric) return;

    // 模擬進程數量
    static int32 ProcessCount = 150;
    ProcessCount += FMath::RandRange(-5, 5);
    ProcessCount = FMath::Clamp(ProcessCount, 50, 500);

    Metric->CurrentValue = (float)ProcessCount;
    Metric->LastUpdated = FDateTime::Now();
    CurrentStatus.ActiveProcesses = ProcessCount;

    // 更新歷史數據
    Metric->HistoricalValues.Add((float)ProcessCount);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    OnMetricUpdated.Broadcast(EMonitorMetricType::ProcessCount, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateThreadMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::ThreadCount);
    if (!Metric) return;

    // 模擬線程數量
    static int32 ThreadCount = 800;
    ThreadCount += FMath::RandRange(-50, 50);
    ThreadCount = FMath::Clamp(ThreadCount, 100, 2000);

    Metric->CurrentValue = (float)ThreadCount;
    Metric->LastUpdated = FDateTime::Now();
    CurrentStatus.ActiveThreads = ThreadCount;

    // 更新歷史數據
    Metric->HistoricalValues.Add((float)ThreadCount);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    OnMetricUpdated.Broadcast(EMonitorMetricType::ThreadCount, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdateTemperatureMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::Temperature);
    if (!Metric) return;

    // 模擬系統溫度
    static float Temperature = 45.0f;
    Temperature += (FMath::RandRange(-2.0f, 2.0f));
    Temperature = FMath::Clamp(Temperature, 20.0f, 80.0f);

    Metric->CurrentValue = Temperature;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(Temperature);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::Temperature);
    if (History)
    {
        History->Add(Temperature);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::Temperature, *Metric);
}

void UMingRTSSystemMonitorDashboard::UpdatePowerMetric()
{
    FSystemMetric* Metric = SystemMetrics.Find(EMonitorMetricType::PowerUsage);
    if (!Metric) return;

    // 模擬功耗
    static float PowerUsage = 200.0f;
    PowerUsage += (FMath::RandRange(-20.0f, 20.0f));
    PowerUsage = FMath::Clamp(PowerUsage, 50.0f, 400.0f);

    Metric->CurrentValue = PowerUsage;
    Metric->LastUpdated = FDateTime::Now();

    // 更新歷史數據
    Metric->HistoricalValues.Add(PowerUsage);
    if (Metric->HistoricalValues.Num() > MaxHistoricalDataPoints)
    {
        Metric->HistoricalValues.RemoveAt(0);
    }

    // 計算平均值
    float Sum = 0.0f;
    for (float Value : Metric->HistoricalValues)
    {
        Sum += Value;
    }
    Metric->AverageValue = Sum / Metric->HistoricalValues.Num();

    // 更新性能歷史
    TArray<float>* History = PerformanceHistory.Find(EMonitorMetricType::PowerUsage);
    if (History)
    {
        History->Add(PowerUsage);
        if (History->Num() > MaxHistoricalDataPoints)
        {
            History->RemoveAt(0);
        }
    }

    OnMetricUpdated.Broadcast(EMonitorMetricType::PowerUsage, *Metric);
}

void UMingRTSSystemMonitorDashboard::HandleAlert(const FSystemAlert& Alert)
{
    // 廣播警報事件
    OnSystemAlert.Broadcast(Alert);

    // 調用自定義回調
    if (AlertCallback)
    {
        AlertCallback(Alert);
    }

    // 記錄警報
    LogMonitoringEvent(TEXT("Alert Generated"), FString::Printf(TEXT("%s: %s"), *Alert.Title, *Alert.Description));
}

FString UMingRTSSystemMonitorDashboard::GenerateAlertID()
{
    return FString::Printf(TEXT("ALERT_%lld"), FDateTime::Now().GetTicks());
}

float UMingRTSSystemMonitorDashboard::CalculateHealthScore() const
{
    float TotalScore = 0.0f;
    int32 MetricCount = 0;

    // 基於各項指標計算健康分數
    for (const auto& MetricPair : SystemMetrics)
    {
        const FSystemMetric& Metric = MetricPair.Value;
        float MetricScore = 100.0f;

        // 根據指標類型計算分數
        if (Metric.MetricType == EMonitorMetricType::CPU || 
            Metric.MetricType == EMonitorMetricType::Memory || 
            Metric.MetricType == EMonitorMetricType::GPU)
        {
            // 使用率類型指標：值越低越好
            MetricScore = 100.0f - Metric.CurrentValue;
        }
        else if (Metric.MetricType == EMonitorMetricType::Temperature)
        {
            // 溫度：理想範圍30-60度
            if (Metric.CurrentValue >= 30.0f && Metric.CurrentValue <= 60.0f)
            {
                MetricScore = 100.0f;
            }
            else
            {
                MetricScore = FMath::Max(0.0f, 100.0f - FMath::Abs(Metric.CurrentValue - 45.0f) * 2.0f);
            }
        }
        else
        {
            // 其他指標：假設當前值在正常範圍內
            MetricScore = 100.0f;
        }

        TotalScore += MetricScore;
        MetricCount++;
    }

    // 扣除警報影響
    int32 ActiveAlerts = GetActiveAlerts().Num();
    float AlertPenalty = ActiveAlerts * 5.0f;

    float FinalScore = MetricCount > 0 ? (TotalScore / MetricCount) - AlertPenalty : 0.0f;
    return FMath::Clamp(FinalScore, 0.0f, 100.0f);
}

void UMingRTSSystemMonitorDashboard::AnalyzePerformanceTrends()
{
    // 分析性能趨勢
    for (const auto& HistoryPair : PerformanceHistory)
    {
        EMonitorMetricType MetricType = HistoryPair.Key;
        const TArray<float>& History = HistoryPair.Value;

        if (History.Num() < 10) continue;

        // 簡單趨勢分析：比較最近10個數據點的平均值與之前10個
        float RecentSum = 0.0f, PreviousSum = 0.0f;
        int32 StartIndex = History.Num() - 10;

        for (int32 i = 0; i < 10; ++i)
        {
            RecentSum += History[StartIndex + i];
        }

        for (int32 i = 0; i < 10 && StartIndex - 1 - i >= 0; ++i)
        {
            PreviousSum += History[StartIndex - 1 - i];
        }

        float RecentAvg = RecentSum / 10.0f;
        float PreviousAvg = PreviousSum / FMath::Min(10, StartIndex);
        float Trend = RecentAvg - PreviousAvg;

        // 檢測顯著趨勢
        if (FMath::Abs(Trend) > 10.0f)
        {
            FString TrendDirection = Trend > 0 ? TEXT("increasing") : TEXT("decreasing");
            LogMonitoringEvent(TEXT("Performance Trend Detected"), 
                FString::Printf(TEXT("%s trend detected for metric %d: %.2f"), 
                    *TrendDirection, (int32)MetricType, Trend));
        }
    }
}

void UMingRTSSystemMonitorDashboard::DetectAnomalousPatterns()
{
    // 檢測異常模式
    TArray<FString> Issues = DetectPerformanceIssues();
    
    if (Issues.Num() > PerformanceIssuesDetected)
    {
        for (const FString& Issue : Issues)
        {
            OnPerformanceIssueDetected.Broadcast(Issue);
        }
        PerformanceIssuesDetected = Issues.Num();
    }
}

void UMingRTSSystemMonitorDashboard::GenerateOptimizationRecommendations()
{
    // 生成優化建議
    TArray<FString> Recommendations = GetRecommendedOptimizations();
    
    for (const FString& Recommendation : Recommendations)
    {
        LogMonitoringEvent(TEXT("Optimization Recommendation"), Recommendation);
    }
}

void UMingRTSSystemMonitorDashboard::LogMonitoringEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("System Monitor Event: %s - %s"), *Event, *Details);
}

void UMingRTSSystemMonitorDashboard::SaveMonitoringData()
{
    // 保存監控數據到文件
    FString Data = ExportMonitoringData();
    FString FileName = FString::Printf(TEXT("SystemMonitorData_%s.json"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    // 實際應使用文件系統API保存數據
    UE_LOG(LogTemp, Log, TEXT("Monitoring data saved to: %s"), *FileName);
}

void UMingRTSSystemMonitorDashboard::LoadMonitoringData()
{
    // 載入監控數據
    UE_LOG(LogTemp, Log, TEXT("Monitoring data loaded"));
}

void UMingRTSSystemMonitorDashboard::CleanupHistoricalData()
{
    // 清理歷史數據
    int32 RemovedPoints = 0;

    for (auto& MetricPair : SystemMetrics)
    {
        FSystemMetric& Metric = MetricPair.Value;
        int32 OriginalCount = Metric.HistoricalValues.Num();
        
        // 保留最近的數據點
        while (Metric.HistoricalValues.Num() > MaxHistoricalDataPoints)
        {
            Metric.HistoricalValues.RemoveAt(0);
            RemovedPoints++;
        }
        
        // 重新計算平均值
        if (Metric.HistoricalValues.Num() > 0)
        {
            float Sum = 0.0f;
            for (float Value : Metric.HistoricalValues)
            {
                Sum += Value;
            }
            Metric.AverageValue = Sum / Metric.HistoricalValues.Num();
        }
    }

    // 清理性能歷史
    for (auto& HistoryPair : PerformanceHistory)
    {
        TArray<float>& History = HistoryPair.Value;
        while (History.Num() > MaxHistoricalDataPoints)
        {
            History.RemoveAt(0);
            RemovedPoints++;
        }
    }

    // 清理系統負載歷史
    while (SystemLoadHistory.Num() > MaxHistoricalDataPoints)
    {
        SystemLoadHistory.RemoveAt(0);
        RemovedPoints++;
    }

    // 清理健康評分歷史
    while (HealthScoreHistory.Num() > MaxHistoricalDataPoints)
    {
        HealthScoreHistory.RemoveAt(0);
        RemovedPoints++;
    }

    // 清理舊警報
    int32 AlertCount = SystemAlerts.Num();
    while (SystemAlerts.Num() > MaxAlertHistory)
    {
        SystemAlerts.RemoveAt(0);
        RemovedPoints++;
    }

    if (RemovedPoints > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Historical data cleanup completed. Removed %d data points"), RemovedPoints);
    }
}
