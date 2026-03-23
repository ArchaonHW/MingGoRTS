#include "MingPerformanceOptimization.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"

// ============================================================================
// 分層更新管理器實現
// ============================================================================

void UMingHierarchicalTickManager::RegisterTickableObject(IMingTickableObject* Object)
{
    if (!Object) return;
    
    EUpdateFrequency Frequency = Object->GetUpdateFrequency();
    TickBuckets.FindOrAdd(Frequency).Objects.Add(Object);
}

void UMingHierarchicalTickManager::UnregisterTickableObject(IMingTickableObject* Object)
{
    if (!Object) return;
    
    for (auto& Pair : TickBuckets)
    {
        Pair.Value.Objects.Remove(Object);
    }
}

void UMingHierarchicalTickManager::ProcessTick(float DeltaTime)
{
    ++GlobalFrameCounter;
    
    for (auto& Pair : TickBuckets)
    {
        EUpdateFrequency Frequency = Pair.Key;
        FTickBucket& Bucket = Pair.Value;
        
        if (!ShouldUpdateThisFrame(Frequency))
        {
            continue;
        }
        
        for (IMingTickableObject* Object : Bucket.Objects)
        {
            if (Object && !Object->ShouldSkipUpdate())
            {
                Object->Tick(DeltaTime);
            }
        }
    }
}

void UMingHierarchicalTickManager::SetPlayerLocation(FVector Location)
{
    // 根據玩家位置調整更新頻率
    FVector Delta = Location - LastPlayerLocation;
    if (Delta.SizeSquared() > 10000.0f) // 100 units
    {
        DistributeObjectsToBuckets();
        LastPlayerLocation = Location;
    }
}

int32 UMingHierarchicalTickManager::GetActiveObjectCount() const
{
    int32 Count = 0;
    for (const auto& Pair : TickBuckets)
    {
        Count += Pair.Value.Objects.Num();
    }
    return Count;
}

void UMingHierarchicalTickManager::DistributeObjectsToBuckets()
{
    // 根據距離重新分配更新頻率
    // 實際實現會根據物件與玩家的距離調整頻率
}

bool UMingHierarchicalTickManager::ShouldUpdateThisFrame(EUpdateFrequency Frequency) const
{
    switch (Frequency)
    {
        case EUpdateFrequency::EveryFrame:
            return true;
        case EUpdateFrequency::Every2Frames:
            return (GlobalFrameCounter % 2) == 0;
        case EUpdateFrequency::Every4Frames:
            return (GlobalFrameCounter % 4) == 0;
        case EUpdateFrequency::Every8Frames:
            return (GlobalFrameCounter % 8) == 0;
        case EUpdateFrequency::Every16Frames:
            return (GlobalFrameCounter % 16) == 0;
        case EUpdateFrequency::Every32Frames:
            return (GlobalFrameCounter % 32) == 0;
        case EUpdateFrequency::AsNeeded:
            return false; // 需要顯式調用
        default:
            return true;
    }
}

// ============================================================================
// 批次處理管理器實現
// ============================================================================

void UMingBatchProcessingManager::InitializeBatch(int32 ExpectedOperationCount)
{
    PendingOperations.Empty();
    OperationCount = 0;
}

void UMingBatchProcessingManager::AddOperation(const FMingOptimizedString& OperationType, 
    const FMingOptimizedString& Target, float Value)
{
    if (OperationCount >= 128)
    {
        // 批次已滿，先執行
        ExecuteBatch();
    }
    
    FBatchOperation Op;
    Op.OperationType = OperationType;
    Op.Target = Target;
    Op.Value = Value;
    
    PendingOperations.Add(Op);
    ++OperationCount;
}

void UMingBatchProcessingManager::ExecuteBatch()
{
    if (OperationCount == 0) return;
    
    UE_LOG(LogTemp, Log, TEXT("[BatchProcessing] Executing %d operations"), OperationCount);
    
    // 按操作類型分組處理
    TMingFixedMap<FMingOptimizedString, TArray<FBatchOperation*, 64>, 16> GroupedOps;
    
    for (int32 i = 0; i < OperationCount; ++i)
    {
        FBatchOperation* Op = &PendingOperations[i];
        // 將操作添加到對應組別
    }
    
    // 執行批次操作
    // 實際實現會根據操作類型進行優化處理
    
    // 清空批次
    PendingOperations.Empty();
    OperationCount = 0;
}

void UMingBatchProcessingManager::ClearBatch()
{
    PendingOperations.Empty();
    OperationCount = 0;
}

int32 UMingBatchProcessingManager::GetPendingOperationCount() const
{
    return OperationCount;
}

bool UMingBatchProcessingManager::IsBatchFull() const
{
    return OperationCount >= 128;
}

// ============================================================================
// 效能監控實現
// ============================================================================

static bool bIsMonitoring = false;
static TArray<FPerformanceMetrics> MetricsHistory;
static float MonitoringStartTime = 0.0f;
static int32 TargetFrameRate = 60;
static bool bDynamicResolutionEnabled = false;

void UMingPerformanceMonitor::StartMonitoring()
{
    bIsMonitoring = true;
    MetricsHistory.Empty();
    MonitoringStartTime = FPlatformTime::Seconds();
    UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] Started monitoring"));
}

void UMingPerformanceMonitor::StopMonitoring()
{
    bIsMonitoring = false;
    
    float Duration = FPlatformTime::Seconds() - MonitoringStartTime;
    UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] Stopped. Duration: %.2f seconds, Samples: %d"),
        Duration, MetricsHistory.Num());
}

FPerformanceMetrics UMingPerformanceMonitor::GetCurrentMetrics()
{
    FPerformanceMetrics Metrics;
    
    // 獲取引擎統計
    if (GEngine)
    {
        // 這裡可以從引擎獲取實際數據
        Metrics.FrameTime = 1000.0f / FMath::Max(1.0f, GEngine->GetAverageFPS());
        Metrics.GameThreadTime = 0.0f; // 需要實際測量
        Metrics.RenderThreadTime = 0.0f;
        Metrics.GPUFrameTime = 0.0f;
        Metrics.DrawCalls = 0;
        Metrics.TrianglesRendered = 0;
    }
    
    if (bIsMonitoring)
    {
        MetricsHistory.Add(Metrics);
    }
    
    return Metrics;
}

bool UMingPerformanceMonitor::IsPerformanceCritical()
{
    FPerformanceMetrics Metrics = GetCurrentMetrics();
    
    // 如果幀時間超過目標的150%，視為危急
    float TargetFrameTime = 1000.0f / TargetFrameRate;
    return Metrics.FrameTime > TargetFrameTime * 1.5f;
}

void UMingPerformanceMonitor::ApplyEmergencyOptimizations()
{
    UE_LOG(LogTemp, Warning, TEXT("[PerformanceMonitor] Applying emergency optimizations"));
    
    // 1. 降低AI更新頻率
    // 2. 減少粒子效果
    // 3. 降低LOD距離
    // 4. 啟用動態解析度
    EnableDynamicResolution(true);
    
    // 5. 觸發垃圾回收
    GEngine->ForceGarbageCollection(true);
}

FString UMingPerformanceMonitor::GetPerformanceReport()
{
    if (MetricsHistory.Num() == 0)
    {
        return TEXT("No performance data available.");
    }
    
    float AvgFrameTime = 0.0f;
    float MinFrameTime = FLT_MAX;
    float MaxFrameTime = 0.0f;
    
    for (const auto& Metrics : MetricsHistory)
    {
        AvgFrameTime += Metrics.FrameTime;
        MinFrameTime = FMath::Min(MinFrameTime, Metrics.FrameTime);
        MaxFrameTime = FMath::Max(MaxFrameTime, Metrics.FrameTime);
    }
    
    AvgFrameTime /= MetricsHistory.Num();
    
    return FString::Printf(TEXT(
        "Performance Report:\n"
        "Sample Count: %d\n"
        "Average Frame Time: %.2f ms (%.1f FPS)\n"
        "Min Frame Time: %.2f ms (%.1f FPS)\n"
        "Max Frame Time: %.2f ms (%.1f FPS)\n"
        "Target FPS: %d\n"),
        MetricsHistory.Num(),
        AvgFrameTime, 1000.0f / AvgFrameTime,
        MinFrameTime, 1000.0f / MinFrameTime,
        MaxFrameTime, 1000.0f / MaxFrameTime,
        TargetFrameRate);
}

void UMingPerformanceMonitor::SetTargetFrameRate(int32 TargetFPS)
{
    TargetFrameRate = TargetFPS;
    
    if (GEngine)
    {
        GEngine->SetMaxFPS(TargetFPS);
    }
}

void UMingPerformanceMonitor::EnableDynamicResolution(bool bEnable)
{
    bDynamicResolutionEnabled = bEnable;
    
    // 實際實現會調整動態解析度設置
    UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] Dynamic resolution %s"),
        bEnable ? TEXT("enabled") : TEXT("disabled"));
}

// ============================================================================
// 自動優化系統
// ============================================================================

/**
 * 自動根據效能調整品質設置
 */
void AutoOptimizeQualitySettings()
{
    FPerformanceMetrics Metrics = UMingPerformanceMonitor::GetCurrentMetrics();
    float TargetFrameTime = 1000.0f / 60.0f; // 目標 60 FPS
    
    if (Metrics.FrameTime > TargetFrameTime * 1.2f)
    {
        // 效能不佳，降低品質
        // 減少陰影品質
        // 降低粒子密度
        // 減少視野距離
        UE_LOG(LogTemp, Warning, TEXT("[AutoOptimize] Reducing quality settings"));
    }
    else if (Metrics.FrameTime < TargetFrameTime * 0.8f)
    {
        // 效能良好，可以略微提升品質
        UE_LOG(LogTemp, Log, TEXT("[AutoOptimize] Performance good, maintaining settings"));
    }
}
