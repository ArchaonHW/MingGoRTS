#include "MingRTSPerformanceProfiler.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformProcess.h"

UMingRTSPerformanceProfiler::UMingRTSPerformanceProfiler()
{
    MonitoringInterval = 1.0f; // 1秒間隔
    bEnableRealTimeMonitoring = true;
    MaxHistoricalDataPoints = 100;
    bEnableAutomaticOptimization = false;
    bIsRealTimeMonitoringActive = false;
}

void UMingRTSPerformanceProfiler::InitializeProfiler()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Initializing performance profiler..."));

    // 初始化性能閾值
    InitializePerformanceThresholds();

    // 初始化當前指標
    InitializeCurrentMetrics();

    // 設置監控定時器
    SetupMonitoringTimer();

    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Performance profiler initialized"));
}

FString UMingRTSPerformanceProfiler::StartProfilingSession(EProfilingType ProfilingType, const FString& SessionName)
{
    FString SessionID = FString::Printf(TEXT("Session_%s_%d"), *SessionName, FMath::RandRange(1000, 9999));
    
    FProfilingSession NewSession;
    NewSession.SessionID = SessionID;
    NewSession.SessionName = SessionName;
    NewSession.ProfilingType = ProfilingType;
    NewSession.StartTime = FDateTime::Now();
    NewSession.bIsActive = true;
    
    // 初始化會話指標
    InitializeSessionMetrics(NewSession, ProfilingType);
    
    // 添加到會話存儲
    ProfilingSessions.Add(SessionID, NewSession);
    ActiveSessions.Add(SessionID);
    
    // 觸發事件
    OnProfilingStarted.Broadcast(SessionID, ProfilingType);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Started profiling session %s for %s"), *SessionID, *GetProfilingTypeName(ProfilingType));
    return SessionID;
}

void UMingRTSPerformanceProfiler::StopProfilingSession(const FString& SessionID)
{
    if (!ProfilingSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Session %s not found"), *SessionID);
        return;
    }
    
    FProfilingSession& Session = ProfilingSessions[SessionID];
    Session.EndTime = FDateTime::Now();
    Session.Duration = (Session.EndTime - Session.StartTime).GetTotalSeconds();
    Session.bIsActive = false;
    
    // 從活動會話中移除
    ActiveSessions.Remove(SessionID);
    
    // 分析最終性能
    AnalyzeFinalPerformance(Session);
    
    // 生成優化建議
    Session.OptimizationSuggestions = GenerateOptimizationSuggestions(Session.ProfilingType, GetCurrentMetric(Session.ProfilingType));
    
    // 觸發事件
    OnProfilingCompleted.Broadcast(SessionID, Session);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Stopped profiling session %s after %.2f seconds"), *SessionID, Session.Duration);
}

TMap<EProfilingType, FPerformanceMetric> UMingRTSPerformanceProfiler::GetCurrentMetrics() const
{
    return CurrentMetrics;
}

TArray<FBottleneckAnalysis> UMingRTSPerformanceProfiler::AnalyzeBottlenecks(const FString& SessionID)
{
    TArray<FBottleneckAnalysis> Bottlenecks;
    
    if (!ProfilingSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Session %s not found for bottleneck analysis"), *SessionID);
        return Bottlenecks;
    }
    
    const FProfilingSession& Session = ProfilingSessions[SessionID];
    
    // 分析每種類型的性能指標
    for (const auto& MetricPair : Session.Metrics)
    {
        const FPerformanceMetric& Metric = MetricPair.Value;
        
        // 檢查是否為瓶頸
        if (Metric.PerformanceLevel <= EPerformanceLevel::Poor)
        {
            FBottleneckAnalysis Bottleneck = AnalyzeComponentPerformance(Session.ProfilingType, Metric);
            Bottlenecks.Add(Bottleneck);
            
            // 觸發瓶頸檢測事件
            OnBottleneckDetected.Broadcast(SessionID, Bottleneck);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Found %d bottlenecks in session %s"), Bottlenecks.Num(), *SessionID);
    return Bottlenecks;
}

FString UMingRTSPerformanceProfiler::GeneratePerformanceReport(const FString& SessionID)
{
    if (!ProfilingSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Session %s not found for report generation"), *SessionID);
        return FString();
    }
    
    const FProfilingSession& Session = ProfilingSessions[SessionID];
    return GeneratePerformanceReportInternal(Session);
}

void UMingRTSPerformanceProfiler::StartRealTimeMonitoring()
{
    if (bIsRealTimeMonitoringActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Real-time monitoring is already active"));
        return;
    }
    
    bIsRealTimeMonitoringActive = true;
    
    // 設置定時器
    GetWorld()->GetTimerManager().SetTimer(
        RealTimeMonitoringTimer,
        this,
        &UMingRTSPerformanceProfiler::RealTimeMonitoringLoop,
        MonitoringInterval,
        true
    );
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Started real-time monitoring with %.1f second interval"), MonitoringInterval);
}

void UMingRTSPerformanceProfiler::StopRealTimeMonitoring()
{
    if (!bIsRealTimeMonitoringActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Real-time monitoring is not active"));
        return;
    }
    
    bIsRealTimeMonitoringActive = false;
    
    // 清除定時器
    GetWorld()->GetTimerManager().ClearTimer(RealTimeMonitoringTimer);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Stopped real-time monitoring"));
}

void UMingRTSPerformanceProfiler::SetPerformanceThresholds(EProfilingType ProfilingType, float WarningThreshold, float CriticalThreshold)
{
    TTuple<float, float> Thresholds = MakeTuple(WarningThreshold, CriticalThreshold);
    PerformanceThresholds.Add(ProfilingType, Thresholds);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Set thresholds for %s - Warning: %.1f%%, Critical: %.1f%%"), 
           *GetProfilingTypeName(ProfilingType), WarningThreshold, CriticalThreshold);
}

TArray<FString> UMingRTSPerformanceProfiler::GetOptimizationSuggestions(const FString& SessionID)
{
    if (!ProfilingSessions.Contains(SessionID))
    {
        return TArray<FString>();
    }
    
    const FProfilingSession& Session = ProfilingSessions[SessionID];
    return Session.OptimizationSuggestions;
}

TMap<FString, float> UMingRTSPerformanceProfiler::CompareSessions(const FString& SessionID1, const FString& SessionID2)
{
    TMap<FString, float> Comparison;
    
    if (!ProfilingSessions.Contains(SessionID1) || !ProfilingSessions.Contains(SessionID2))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: One or both sessions not found for comparison"));
        return Comparison;
    }
    
    const FProfilingSession& Session1 = ProfilingSessions[SessionID1];
    const FProfilingSession& Session2 = ProfilingSessions[SessionID2];
    
    // 比較相同類型的指標
    for (const auto& MetricPair1 : Session1.Metrics)
    {
        const FString& MetricName = MetricPair1.Key;
        const FPerformanceMetric& Metric1 = MetricPair1.Value;
        
        if (Session2.Metrics.Contains(MetricName))
        {
            const FPerformanceMetric& Metric2 = Session2.Metrics[MetricName];
            float Improvement = (Metric1.AverageValue - Metric2.AverageValue) / Metric2.AverageValue * 100.0f;
            Comparison.Add(MetricName, Improvement);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Compared sessions %s and %s with %d metrics"), *SessionID1, *SessionID2, Comparison.Num());
    return Comparison;
}

bool UMingRTSPerformanceProfiler::ExportPerformanceData(const FString& SessionID, const FString& FilePath)
{
    if (!ProfilingSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Session %s not found for export"), *SessionID);
        return false;
    }
    
    const FProfilingSession& Session = ProfilingSessions[SessionID];
    FString ReportData = GeneratePerformanceReportInternal(Session);
    
    // 寫入文件
    bool bSuccess = FFileHelper::SaveStringToFile(ReportData, *FilePath);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Exported session %s data to %s"), *SessionID, *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MingRTSPerformanceProfiler: Failed to export session %s data"), *SessionID);
    }
    
    return bSuccess;
}

void UMingRTSPerformanceProfiler::SetMonitoringInterval(float IntervalSeconds)
{
    MonitoringInterval = FMath::Max(0.1f, IntervalSeconds);
    
    // 如果實時監控正在運行，重新啟動以應用新的間隔
    if (bIsRealTimeMonitoringActive)
    {
        StopRealTimeMonitoring();
        StartRealTimeMonitoring();
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Set monitoring interval to %.1f seconds"), MonitoringInterval);
}

TMap<EProfilingType, EPerformanceLevel> UMingRTSPerformanceProfiler::GetSystemPerformanceOverview() const
{
    TMap<EProfilingType, EPerformanceLevel> Overview;
    
    for (const auto& MetricPair : CurrentMetrics)
    {
        Overview.Add(MetricPair.Key, MetricPair.Value.PerformanceLevel);
    }
    
    return Overview;
}

void UMingRTSPerformanceProfiler::CleanupOldSessions(int32 MaxSessionsToKeep)
{
    if (ProfilingSessions.Num() <= MaxSessionsToKeep)
    {
        return;
    }
    
    // 按時間排序會話
    TArray<FString> SortedSessionIDs;
    for (const auto& SessionPair : ProfilingSessions)
    {
        SortedSessionIDs.Add(SessionPair.Key);
    }
    
    SortedSessionIDs.Sort([this](const FString& A, const FString& B) {
        return ProfilingSessions[A].StartTime < ProfilingSessions[B].StartTime;
    });
    
    // 移除最舊的會話
    int32 SessionsToRemove = ProfilingSessions.Num() - MaxSessionsToKeep;
    for (int32 i = 0; i < SessionsToRemove; i++)
    {
        FString SessionID = SortedSessionIDs[i];
        ProfilingSessions.Remove(SessionID);
        ActiveSessions.Remove(SessionID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Cleaned up %d old sessions"), SessionsToRemove);
}

// 私有方法實現

void UMingRTSPerformanceProfiler::InitializePerformanceThresholds()
{
    // CPU閾值
    PerformanceThresholds.Add(EProfilingType::CPU, MakeTuple(70.0f, 90.0f));
    
    // 內存閾值
    PerformanceThresholds.Add(EProfilingType::Memory, MakeTuple(70.0f, 90.0f));
    
    // GPU閾值
    PerformanceThresholds.Add(EProfilingType::GPU, MakeTuple(70.0f, 90.0f));
    
    // 網絡閾值
    PerformanceThresholds.Add(EProfilingType::Network, MakeTuple(100.0f, 200.0f)); // 延迟(ms)
    
    // 磁盤I/O閾值
    PerformanceThresholds.Add(EProfilingType::Disk, MakeTuple(70.0f, 90.0f));
    
    // 渲染閾值
    PerformanceThresholds.Add(EProfilingType::Rendering, MakeTuple(16.67f, 33.33f)); // FPS
    
    // 音頻閾值
    PerformanceThresholds.Add(EProfilingType::Audio, MakeTuple(70.0f, 90.0f));
    
    // 物理閾值
    PerformanceThresholds.Add(EProfilingType::Physics, MakeTuple(16.67f, 33.33f)); // FPS
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Initialized performance thresholds"));
}

void UMingRTSPerformanceProfiler::InitializeCurrentMetrics()
{
    // 初始化所有類型的當前指標
    for (int32 i = 0; i < 8; i++)
    {
        EProfilingType ProfilingType = (EProfilingType)i;
        FPerformanceMetric Metric;
        Metric.MetricName = GetProfilingTypeName(ProfilingType);
        CurrentMetrics.Add(ProfilingType, Metric);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceProfiler: Initialized current metrics"));
}

void UMingRTSPerformanceProfiler::SetupMonitoringTimer()
{
    // 設置監控定時器
    if (GetWorld() && bEnableRealTimeMonitoring)
    {
        StartRealTimeMonitoring();
    }
}

void UMingRTSPerformanceProfiler::InitializeSessionMetrics(FProfilingSession& Session, EProfilingType ProfilingType)
{
    FPerformanceMetric Metric;
    Metric.MetricName = GetProfilingTypeName(ProfilingType);
    Session.Metrics.Add(Metric.MetricName, Metric);
}

void UMingRTSPerformanceProfiler::AnalyzeFinalPerformance(FProfilingSession& Session)
{
    // 分析最終性能數據
    for (auto& MetricPair : Session.Metrics)
    {
        FPerformanceMetric& Metric = MetricPair.Value;
        
        // 計算平均值
        if (Metric.HistoricalData.Num() > 0)
        {
            float Sum = 0.0f;
            for (float Value : Metric.HistoricalData)
            {
                Sum += Value;
            }
            Metric.AverageValue = Sum / Metric.HistoricalData.Num();
        }
        
        // 更新性能等級
        if (PerformanceThresholds.Contains(Session.ProfilingType))
        {
            TTuple<float, float> Thresholds = PerformanceThresholds[Session.ProfilingType];
            Metric.PerformanceLevel = EvaluatePerformanceLevel(Metric.AverageValue, Thresholds.Get<0>(), Thresholds.Get<1>());
        }
    }
}

void UMingRTSPerformanceProfiler::UpdatePerformanceMetrics()
{
    // 更新所有性能指標
    CollectCPUMetrics();
    CollectMemoryMetrics();
    CollectGPUMetrics();
    CollectNetworkMetrics();
    CollectDiskMetrics();
    CollectRenderingMetrics();
    CollectAudioMetrics();
    CollectPhysicsMetrics();
}

void UMingRTSPerformanceProfiler::CollectCPUMetrics()
{
    float CPUUsage = CalculateCPUUsage();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::CPU];
    UpdateMetric(Metric, CPUUsage);
}

void UMingRTSPerformanceProfiler::CollectMemoryMetrics()
{
    float MemoryUsage = CalculateMemoryUsage();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Memory];
    UpdateMetric(Metric, MemoryUsage);
}

void UMingRTSPerformanceProfiler::CollectGPUMetrics()
{
    float GPUUsage = CalculateGPUUsage();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::GPU];
    UpdateMetric(Metric, GPUUsage);
}

void UMingRTSPerformanceProfiler::CollectNetworkMetrics()
{
    float NetworkLatency = CalculateNetworkLatency();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Network];
    UpdateMetric(Metric, NetworkLatency);
}

void UMingRTSPerformanceProfiler::CollectDiskMetrics()
{
    float DiskIO = CalculateDiskIO();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Disk];
    UpdateMetric(Metric, DiskIO);
}

void UMingRTSPerformanceProfiler::CollectRenderingMetrics()
{
    float RenderingPerformance = CalculateRenderingPerformance();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Rendering];
    UpdateMetric(Metric, RenderingPerformance);
}

void UMingRTSPerformanceProfiler::CollectAudioMetrics()
{
    float AudioPerformance = CalculateAudioPerformance();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Audio];
    UpdateMetric(Metric, AudioPerformance);
}

void UMingRTSPerformanceProfiler::CollectPhysicsMetrics()
{
    float PhysicsPerformance = CalculatePhysicsPerformance();
    FPerformanceMetric& Metric = CurrentMetrics[EProfilingType::Physics];
    UpdateMetric(Metric, PhysicsPerformance);
}

float UMingRTSPerformanceProfiler::CalculateCPUUsage()
{
    // 獲取CPU使用率
    return FPlatformMisc::GetCPUUsagePercentage();
}

float UMingRTSPerformanceProfiler::CalculateMemoryUsage()
{
    // 計算內存使用率
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    float UsedMemory = MemoryStats.UsedPhysical / (1024.0f * 1024.0f); // MB
    float TotalMemory = MemoryStats.TotalPhysical / (1024.0f * 1024.0f); // MB
    
    return (UsedMemory / TotalMemory) * 100.0f;
}

float UMingRTSPerformanceProfiler::CalculateGPUUsage()
{
    // 獲取GPU使用率（需要平台特定實現）
    return 0.0f; // 暫時返回0
}

float UMingRTSPerformanceProfiler::CalculateNetworkLatency()
{
    // 計算網絡延遲
    return 0.0f; // 暫時返回0
}

float UMingRTSPerformanceProfiler::CalculateDiskIO()
{
    // 計算磁盤I/O性能
    return 0.0f; // 暫時返回0
}

float UMingRTSPerformanceProfiler::CalculateRenderingPerformance()
{
    // 計算渲染性能（FPS）
    if (GEngine && GEngine->GetWorld())
    {
        return 1.0f / GEngine->GetWorld()->GetDeltaSeconds();
    }
    return 60.0f; // 默認60 FPS
}

float UMingRTSPerformanceProfiler::CalculateAudioPerformance()
{
    // 計算音頻性能
    return 0.0f; // 暫時返回0
}

float UMingRTSPerformanceProfiler::CalculatePhysicsPerformance()
{
    // 計算物理性能
    return 0.0f; // 暫時返回0
}

void UMingRTSPerformanceProfiler::DetectBottlenecks(const FString& SessionID)
{
    if (!ProfilingSessions.Contains(SessionID))
    {
        return;
    }
    
    FProfilingSession& Session = ProfilingSessions[SessionID];
    
    for (auto& MetricPair : Session.Metrics)
    {
        const FPerformanceMetric& Metric = MetricPair.Value;
        
        // 檢查性能等級
        if (Metric.PerformanceLevel <= EPerformanceLevel::Poor)
        {
            FBottleneckAnalysis Bottleneck = AnalyzeComponentPerformance(Session.ProfilingType, Metric);
            
            // 添加到會話警告
            Session.PerformanceWarnings.Add(Bottleneck.Description);
            
            // 觸發事件
            OnBottleneckDetected.Broadcast(SessionID, Bottleneck);
        }
    }
}

FBottleneckAnalysis UMingRTSPerformanceProfiler::AnalyzeComponentPerformance(EProfilingType ProfilingType, const FPerformanceMetric& Metric)
{
    FBottleneckAnalysis Analysis;
    Analysis.ComponentName = Metric.MetricName;
    Analysis.ProfilingType = ProfilingType;
    Analysis.Description = FString::Printf(TEXT("Performance issue detected in %s"), *Metric.MetricName);
    
    // 計算影響分數
    switch (Metric.PerformanceLevel)
    {
        case EPerformanceLevel::Critical:
            Analysis.ImpactScore = 1.0f;
            Analysis.PotentialImprovement = 50.0f;
            break;
        case EPerformanceLevel::Poor:
            Analysis.ImpactScore = 0.7f;
            Analysis.PotentialImprovement = 30.0f;
            break;
        case EPerformanceLevel::Average:
            Analysis.ImpactScore = 0.4f;
            Analysis.PotentialImprovement = 15.0f;
            break;
        default:
            Analysis.ImpactScore = 0.1f;
            Analysis.PotentialImprovement = 5.0f;
            break;
    }
    
    // 生成優化建議
    Analysis.SuggestedOptimizations = GenerateOptimizationSuggestions(ProfilingType, Metric);
    
    return Analysis;
}

TArray<FString> UMingRTSPerformanceProfiler::GenerateOptimizationSuggestions(EProfilingType ProfilingType, const FPerformanceMetric& Metric)
{
    TArray<FString> Suggestions;
    
    switch (ProfilingType)
    {
        case EProfilingType::CPU:
            Suggestions.Add(TEXT("Consider optimizing algorithms and reducing computational complexity"));
            Suggestions.Add(TEXT("Use multithreading for CPU-intensive tasks"));
            Suggestions.Add(TEXT("Profile and optimize hot code paths"));
            break;
            
        case EProfilingType::Memory:
            Suggestions.Add(TEXT("Implement memory pooling and object pooling"));
            Suggestions.Add(TEXT("Reduce memory allocations and deallocations"));
            Suggestions.Add(TEXT("Optimize data structures for better cache locality"));
            break;
            
        case EProfilingType::GPU:
            Suggestions.Add(TEXT("Optimize shaders and reduce draw calls"));
            Suggestions.Add(TEXT("Implement LOD systems and culling"));
            Suggestions.Add(TEXT("Use texture compression and optimization"));
            break;
            
        case EProfilingType::Network:
            Suggestions.Add(TEXT("Implement data compression for network packets"));
            Suggestions.Add(TEXT("Use prediction and interpolation for smooth gameplay"));
            Suggestions.Add(TEXT("Optimize network update frequency"));
            break;
            
        case EProfilingType::Disk:
            Suggestions.Add(TEXT("Implement asynchronous file operations"));
            Suggestions.Add(TEXT("Use asset streaming and compression"));
            Suggestions.Add(TEXT("Optimize file access patterns"));
            break;
            
        case EProfilingType::Rendering:
            Suggestions.Add(TEXT("Optimize rendering pipeline and reduce overdraw"));
            Suggestions.Add(TEXT("Implement efficient culling techniques"));
            Suggestions.Add(TEXT("Use occlusion culling and frustum culling"));
            break;
            
        case EProfilingType::Audio:
            Suggestions.Add(TEXT("Optimize audio buffer sizes and streaming"));
            Suggestions.Add(TEXT("Use audio compression and efficient formats"));
            Suggestions.Add(TEXT("Implement audio pooling and reuse"));
            break;
            
        case EProfilingType::Physics:
            Suggestions.Add(TEXT("Optimize collision detection algorithms"));
            Suggestions.Add(TEXT("Use spatial partitioning for physics objects"));
            Suggestions.Add(TEXT("Adjust physics update frequency"));
            break;
    }
    
    return Suggestions;
}

FString UMingRTSPerformanceProfiler::GeneratePerformanceReportInternal(const FProfilingSession& Session)
{
    FString Report;
    Report += FString::Printf(TEXT("=== Performance Report ===\n"));
    Report += FString::Printf(TEXT("Session: %s\n"), *Session.SessionName);
    Report += FString::Printf(TEXT("Type: %s\n"), *GetProfilingTypeName(Session.ProfilingType));
    Report += FString::Printf(TEXT("Duration: %.2f seconds\n"), Session.Duration);
    Report += FString::Printf(TEXT("Start Time: %s\n"), *Session.StartTime.ToString());
    Report += FString::Printf(TEXT("End Time: %s\n"), *Session.EndTime.ToString());
    Report += TEXT("\n=== Performance Metrics ===\n");
    
    for (const auto& MetricPair : Session.Metrics)
    {
        const FPerformanceMetric& Metric = MetricPair.Value;
        Report += FString::Printf(TEXT("%s:\n"), *Metric.MetricName);
        Report += FString::Printf(TEXT("  Current: %.2f\n"), Metric.CurrentValue);
        Report += FString::Printf(TEXT("  Average: %.2f\n"), Metric.AverageValue);
        Report += FString::Printf(TEXT("  Peak: %.2f\n"), Metric.PeakValue);
        Report += FString::Printf(TEXT("  Min: %.2f\n"), Metric.MinValue);
        Report += FString::Printf(TEXT("  Level: %s\n"), GetPerformanceLevelName(Metric.PerformanceLevel));
        Report += TEXT("\n");
    }
    
    if (Session.PerformanceWarnings.Num() > 0)
    {
        Report += TEXT("=== Performance Warnings ===\n");
        for (const FString& Warning : Session.PerformanceWarnings)
        {
            Report += FString::Printf(TEXT("- %s\n"), *Warning);
        }
        Report += TEXT("\n");
    }
    
    if (Session.OptimizationSuggestions.Num() > 0)
    {
        Report += TEXT("=== Optimization Suggestions ===\n");
        for (const FString& Suggestion : Session.OptimizationSuggestions)
        {
            Report += FString::Printf(TEXT("- %s\n"), *Suggestion);
        }
        Report += TEXT("\n");
    }
    
    Report += TEXT("=== End Report ===\n");
    
    return Report;
}

EPerformanceLevel UMingRTSPerformanceProfiler::EvaluatePerformanceLevel(float Value, float WarningThreshold, float CriticalThreshold)
{
    // 根據類型決定比較方式
    if (WarningThreshold < CriticalThreshold)
    {
        // 越低越好（如延遲）
        if (Value <= WarningThreshold)
        {
            return EPerformanceLevel::Excellent;
        }
        else if (Value <= CriticalThreshold)
        {
            return EPerformanceLevel::Good;
        }
        else if (Value <= CriticalThreshold * 1.5f)
        {
            return EPerformanceLevel::Average;
        }
        else if (Value <= CriticalThreshold * 2.0f)
        {
            return EPerformanceLevel::Poor;
        }
        else
        {
            return EPerformanceLevel::Critical;
        }
    }
    else
    {
        // 越高越好（如FPS）
        if (Value >= WarningThreshold)
        {
            return EPerformanceLevel::Excellent;
        }
        else if (Value >= CriticalThreshold)
        {
            return EPerformanceLevel::Good;
        }
        else if (Value >= CriticalThreshold * 0.7f)
        {
            return EPerformanceLevel::Average;
        }
        else if (Value >= CriticalThreshold * 0.5f)
        {
            return EPerformanceLevel::Poor;
        }
        else
        {
            return EPerformanceLevel::Critical;
        }
    }
}

void UMingRTSPerformanceProfiler::UpdateHistoricalData(FPerformanceMetric& Metric)
{
    // 添加當前值到歷史數據
    Metric.HistoricalData.Add(Metric.CurrentValue);
    
    // 限制歷史數據點數量
    if (Metric.HistoricalData.Num() > MaxHistoricalDataPoints)
    {
        Metric.HistoricalData.RemoveAt(0);
    }
    
    // 更新統計數據
    Metric.MinValue = FMath::Min(Metric.MinValue, Metric.CurrentValue);
    Metric.PeakValue = FMath::Max(Metric.PeakValue, Metric.CurrentValue);
    Metric.LastUpdateTime = FDateTime::Now();
}

void UMingRTSPerformanceProfiler::TriggerPerformanceWarning(const FString& SessionID, const FString& Warning, EPerformanceLevel Level)
{
    // 觸發性能警告事件
    OnPerformanceWarning.Broadcast(SessionID, Warning, Level);
    
    UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceProfiler: Performance warning - %s"), *Warning);
}

FString UMingRTSPerformanceProfiler::GetProfilingTypeName(EProfilingType ProfilingType) const
{
    switch (ProfilingType)
    {
        case EProfilingType::CPU: return TEXT("CPU");
        case EProfilingType::Memory: return TEXT("Memory");
        case EProfilingType::GPU: return TEXT("GPU");
        case EProfilingType::Network: return TEXT("Network");
        case EProfilingType::Disk: return TEXT("Disk");
        case EProfilingType::Rendering: return TEXT("Rendering");
        case EProfilingType::Audio: return TEXT("Audio");
        case EProfilingType::Physics: return TEXT("Physics");
        default: return TEXT("Unknown");
    }
}

void UMingRTSPerformanceProfiler::RealTimeMonitoringLoop()
{
    if (!bIsRealTimeMonitoringActive)
    {
        return;
    }
    
    // 更新性能指標
    UpdatePerformanceMetrics();
    
    // 檢查活動會話
    for (const FString& SessionID : ActiveSessions)
    {
        if (ProfilingSessions.Contains(SessionID))
        {
            FProfilingSession& Session = ProfilingSessions[SessionID];
            
            // 更新會話指標
            for (auto& MetricPair : Session.Metrics)
            {
                FString MetricName = MetricPair.Key;
                FPerformanceMetric& SessionMetric = MetricPair.Value;
                
                // 從當前指標更新會話指標
                if (CurrentMetrics.Contains(Session.ProfilingType))
                {
                    const FPerformanceMetric& CurrentMetric = CurrentMetrics[Session.ProfilingType];
                    SessionMetric.CurrentValue = CurrentMetric.CurrentValue;
                    UpdateHistoricalData(SessionMetric);
                    
                    // 檢查性能警告
                    if (PerformanceThresholds.Contains(Session.ProfilingType))
                    {
                        TTuple<float, float> Thresholds = PerformanceThresholds[Session.ProfilingType];
                        EPerformanceLevel Level = EvaluatePerformanceLevel(SessionMetric.CurrentValue, Thresholds.Get<0>(), Thresholds.Get<1>());
                        
                        if (Level <= EPerformanceLevel::Poor)
                        {
                            FString Warning = FString::Printf(TEXT("%s performance is %s (%.2f)"), 
                                                             *SessionMetric.MetricName, 
                                                             *GetPerformanceLevelName(Level), 
                                                             SessionMetric.CurrentValue);
                            TriggerPerformanceWarning(SessionID, Warning, Level);
                        }
                    }
                }
            }
            
            // 檢測瓶頸
            DetectBottlenecks(SessionID);
        }
    }
}

void UMingRTSPerformanceProfiler::UpdateMetric(FPerformanceMetric& Metric, float Value)
{
    Metric.CurrentValue = Value;
    UpdateHistoricalData(Metric);
    
    // 更新性能等級
    if (PerformanceThresholds.Contains(GetProfilingTypeFromMetric(Metric.MetricName)))
    {
        EProfilingType ProfilingType = GetProfilingTypeFromMetric(Metric.MetricName);
        TTuple<float, float> Thresholds = PerformanceThresholds[ProfilingType];
        Metric.PerformanceLevel = EvaluatePerformanceLevel(Value, Thresholds.Get<0>(), Thresholds.Get<1>());
    }
}

EProfilingType UMingRTSPerformanceProfiler::GetProfilingTypeFromMetric(const FString& MetricName) const
{
    if (MetricName == TEXT("CPU")) return EProfilingType::CPU;
    if (MetricName == TEXT("Memory")) return EProfilingType::Memory;
    if (MetricName == TEXT("GPU")) return EProfilingType::GPU;
    if (MetricName == TEXT("Network")) return EProfilingType::Network;
    if (MetricName == TEXT("Disk")) return EProfilingType::Disk;
    if (MetricName == TEXT("Rendering")) return EProfilingType::Rendering;
    if (MetricName == TEXT("Audio")) return EProfilingType::Audio;
    if (MetricName == TEXT("Physics")) return EProfilingType::Physics;
    
    return EProfilingType::CPU; // 默認
}

FString UMingRTSPerformanceProfiler::GetPerformanceLevelName(EPerformanceLevel Level) const
{
    switch (Level)
    {
        case EPerformanceLevel::Excellent: return TEXT("Excellent");
        case EPerformanceLevel::Good: return TEXT("Good");
        case EPerformanceLevel::Average: return TEXT("Average");
        case EPerformanceLevel::Poor: return TEXT("Poor");
        case EPerformanceLevel::Critical: return TEXT("Critical");
        default: return TEXT("Unknown");
    }
}

FPerformanceMetric UMingRTSPerformanceProfiler::GetCurrentMetric(EProfilingType ProfilingType) const
{
    return CurrentMetrics.FindRef(ProfilingType);
}
