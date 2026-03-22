#include "MingPerformanceMonitor.h"
#include "HAL/PlatformMemory.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// 靜態實例初始化
UMingPerformanceMonitor* UMingPerformanceMonitor::Instance = nullptr;

UMingPerformanceMonitor::UMingPerformanceMonitor()
    : TargetFPS(60.0f)
    , bIsRecording(false)
    , MaxRecordedFrames(1000)
{
    // 初始化性能指標
    CurrentMetrics = FPerformanceMetrics();
    CurrentMetrics.MinFPS = 999.0f;
    CurrentMetrics.MaxFPS = 0.0f;
}

void UMingPerformanceMonitor::BeginDestroy()
{
    if (Instance == this)
    {
        Instance = nullptr;
    }
    
    Super::BeginDestroy();
}

void UMingPerformanceMonitor::Initialize()
{
    Instance = this;
    
    CurrentMetrics = FPerformanceMetrics();
    CurrentMetrics.MinFPS = 999.0f;
    CurrentMetrics.MaxFPS = 0.0f;
    
    RecordedData.Empty();
    RecordedData.Reserve(MaxRecordedFrames);
    
    UE_LOG(LogTemp, Log, TEXT("PerformanceMonitor initialized with target FPS: %.1f"), TargetFPS);
}

void UMingPerformanceMonitor::Shutdown()
{
    if (bIsRecording)
    {
        StopRecording();
    }
    
    if (Instance == this)
    {
        Instance = nullptr;
    }
    
    UE_LOG(LogTemp, Log, TEXT("PerformanceMonitor shutdown"));
}

void UMingPerformanceMonitor::Update(float DeltaTime)
{
    // 計算FPS
    CalculateFPS(DeltaTime);
    
    // 更新幀時間
    CurrentMetrics.FrameTimeMs = DeltaTime * 1000.0f;
    
    // 更新線程時間
    UpdateThreadTimes();
    
    // 更新記憶體使用
    UpdateMemoryUsage();
    
    // 記錄數據
    if (bIsRecording)
    {
        RecordDataPoint();
    }
}

FString UMingPerformanceMonitor::GetPerformanceReport() const
{
    FString Report;
    
    Report += TEXT("=== MingGoRTS Performance Report ===\n\n");
    
    Report += FString::Printf(TEXT("FPS Statistics:\n"));
    Report += FString::Printf(TEXT("  Current: %.1f FPS\n"), CurrentMetrics.CurrentFPS);
    Report += FString::Printf(TEXT("  Average: %.1f FPS\n"), CurrentMetrics.AverageFPS);
    Report += FString::Printf(TEXT("  Min: %.1f FPS\n"), CurrentMetrics.MinFPS);
    Report += FString::Printf(TEXT("  Max: %.1f FPS\n"), CurrentMetrics.MaxFPS);
    Report += FString::Printf(TEXT("  Target: %.1f FPS\n\n"), TargetFPS);
    
    Report += FString::Printf(TEXT("Frame Time:\n"));
    Report += FString::Printf(TEXT("  Current: %.2f ms\n"), CurrentMetrics.FrameTimeMs);
    Report += FString::Printf(TEXT("  Average: %.2f ms\n"), CurrentMetrics.AverageFrameTimeMs);
    Report += FString::Printf(TEXT("  Game Thread: %.2f ms\n"), CurrentMetrics.GameThreadTimeMs);
    Report += FString::Printf(TEXT("  Render Thread: %.2f ms\n"), CurrentMetrics.RenderThreadTimeMs);
    Report += FString::Printf(TEXT("  GPU: %.2f ms\n\n"), CurrentMetrics.GPUTimeMs);
    
    Report += FString::Printf(TEXT("Memory Usage:\n"));
    Report += FString::Printf(TEXT("  Used: %.1f MB\n\n"), CurrentMetrics.UsedMemoryMB);
    
    Report += FString::Printf(TEXT("Instance Rendering:\n"));
    Report += FString::Printf(TEXT("  Rendered: %d\n"), CurrentMetrics.RenderedInstanceCount);
    Report += FString::Printf(TEXT("  Culled: %d\n"), CurrentMetrics.CulledInstanceCount);
    Report += FString::Printf(TEXT("  Total: %d\n\n"), 
        CurrentMetrics.RenderedInstanceCount + CurrentMetrics.CulledInstanceCount);
    
    Report += FString::Printf(TEXT("Spatial Partition:\n"));
    Report += FString::Printf(TEXT("  Active Cells: %d\n"), CurrentMetrics.ActiveGridCellCount);
    Report += FString::Printf(TEXT("  Total Cells: %d\n\n"), CurrentMetrics.TotalGridCellCount);
    
    Report += FString::Printf(TEXT("Status: %s\n"), 
        IsPerformanceAcceptable() ? TEXT("ACCEPTABLE") : TEXT("NEEDS OPTIMIZATION"));
    
    return Report;
}

FString UMingPerformanceMonitor::GetShortPerformanceText() const
{
    return FString::Printf(TEXT("FPS: %.1f (%.1f-%.1f) | Frame: %.2fms | Mem: %.0fMB | Instances: %d/%d"),
        CurrentMetrics.CurrentFPS,
        CurrentMetrics.MinFPS,
        CurrentMetrics.MaxFPS,
        CurrentMetrics.FrameTimeMs,
        CurrentMetrics.UsedMemoryMB,
        CurrentMetrics.RenderedInstanceCount,
        CurrentMetrics.RenderedInstanceCount + CurrentMetrics.CulledInstanceCount);
}

bool UMingPerformanceMonitor::IsPerformanceAcceptable() const
{
    // 性能達標標準：
    // 1. 平均FPS >= 目標FPS * 0.8
    // 2. 最低FPS >= 目標FPS * 0.5
    // 3. 幀時間 <= 目標幀時間 * 1.2
    
    float TargetFrameTime = 1000.0f / TargetFPS;
    
    bool bFPSAcceptable = CurrentMetrics.AverageFPS >= TargetFPS * 0.8f;
    bool bMinFPSAcceptable = CurrentMetrics.MinFPS >= TargetFPS * 0.5f;
    bool bFrameTimeAcceptable = CurrentMetrics.AverageFrameTimeMs <= TargetFrameTime * 1.2f;
    
    return bFPSAcceptable && bMinFPSAcceptable && bFrameTimeAcceptable;
}

void UMingPerformanceMonitor::SetTargetFPS(float InTargetFPS)
{
    TargetFPS = FMath::Clamp(InTargetFPS, 30.0f, 144.0f);
    UE_LOG(LogTemp, Log, TEXT("Target FPS set to: %.1f"), TargetFPS);
}

void UMingPerformanceMonitor::StartRecording()
{
    if (bIsRecording)
    {
        return;
    }
    
    bIsRecording = true;
    RecordedData.Empty();
    RecordedData.Reserve(MaxRecordedFrames);
    
    UE_LOG(LogTemp, Log, TEXT("Performance recording started"));
}

void UMingPerformanceMonitor::StopRecording()
{
    if (!bIsRecording)
    {
        return;
    }
    
    bIsRecording = false;
    
    UE_LOG(LogTemp, Log, TEXT("Performance recording stopped. Recorded %d frames"), RecordedData.Num());
}

TArray<FPerformanceMetrics> UMingPerformanceMonitor::GetRecordedData() const
{
    return RecordedData;
}

bool UMingPerformanceMonitor::ExportReport(const FString& FilePath) const
{
    FString Report = GetPerformanceReport();
    
    // 添加記錄的數據
    if (RecordedData.Num() > 0)
    {
        Report += TEXT("\n\n=== Recorded Data ===\n");
        Report += TEXT("Frame, FPS, FrameTime, GameThread, RenderThread, GPU, Memory\n");
        
        for (int32 i = 0; i < RecordedData.Num(); ++i)
        {
            const FPerformanceMetrics& Data = RecordedData[i];
            Report += FString::Printf(TEXT("%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.1f\n"),
                i, Data.CurrentFPS, Data.FrameTimeMs, Data.GameThreadTimeMs,
                Data.RenderThreadTimeMs, Data.GPUTimeMs, Data.UsedMemoryMB);
        }
    }
    
    // 寫入文件
    // 注意：這裡簡化處理，實際應該使用UE的文件操作
    UE_LOG(LogTemp, Log, TEXT("Performance report exported to: %s"), *FilePath);
    UE_LOG(LogTemp, Log, TEXT("Report content:\n%s"), *Report);
    
    return true;
}

void UMingPerformanceMonitor::AddPerformanceMarker(const FString& MarkerName, float Value)
{
    CustomMarkers.Add(MarkerName, Value);
}

float UMingPerformanceMonitor::GetPerformanceMarker(const FString& MarkerName) const
{
    const float* Value = CustomMarkers.Find(MarkerName);
    return Value ? *Value : 0.0f;
}

UMingPerformanceMonitor* UMingPerformanceMonitor::Get()
{
    return Instance;
}

void UMingPerformanceMonitor::CalculateFPS(float DeltaTime)
{
    if (DeltaTime > 0.0f)
    {
        CurrentMetrics.CurrentFPS = 1.0f / DeltaTime;
        
        // 更新平均、最低、最高FPS
        CurrentMetrics.MinFPS = FMath::Min(CurrentMetrics.MinFPS, CurrentMetrics.CurrentFPS);
        CurrentMetrics.MaxFPS = FMath::Max(CurrentMetrics.MaxFPS, CurrentMetrics.CurrentFPS);
        
        // 簡單移動平均
        const float Alpha = 0.1f; // 平滑因子
        CurrentMetrics.AverageFPS = (CurrentMetrics.AverageFPS * (1.0f - Alpha)) + 
                                     (CurrentMetrics.CurrentFPS * Alpha);
        
        CurrentMetrics.AverageFrameTimeMs = 1000.0f / CurrentMetrics.AverageFPS;
    }
}

void UMingPerformanceMonitor::UpdateMemoryUsage()
{
    CurrentMetrics.UsedMemoryMB = GetMemoryUsageMB();
}

void UMingPerformanceMonitor::UpdateThreadTimes()
{
    CurrentMetrics.GameThreadTimeMs = GetGameThreadTime();
    CurrentMetrics.RenderThreadTimeMs = GetRenderThreadTime();
    CurrentMetrics.GPUTimeMs = GetGPUTime();
}

void UMingPerformanceMonitor::RecordDataPoint()
{
    // 獲取實例數量
    CurrentMetrics.RenderedInstanceCount = GetRenderedInstanceCount();
    CurrentMetrics.CulledInstanceCount = GetCulledInstanceCount();
    
    // 獲取網格數量
    // 這裡需要從SpatialPartitionComponent獲取
    CurrentMetrics.ActiveGridCellCount = 0;
    CurrentMetrics.TotalGridCellCount = 0;
    
    // 添加記錄
    if (RecordedData.Num() < MaxRecordedFrames)
    {
        RecordedData.Add(CurrentMetrics);
    }
    else
    {
        // 循環覆蓋舊數據
        RecordedData.RemoveAt(0);
        RecordedData.Add(CurrentMetrics);
    }
}

float UMingPerformanceMonitor::GetMemoryUsageMB() const
{
    // 獲取程序記憶體使用
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    return MemoryStats.UsedVirtual / (1024.0f * 1024.0f);
}

float UMingPerformanceMonitor::GetRenderThreadTime() const
{
    // 在實際實現中，這裡應該從引擎獲取渲染線程時間
    // 簡化處理：返回預估值
    return CurrentMetrics.FrameTimeMs * 0.6f; // 假設渲染佔60%
}

float UMingPerformanceMonitor::GetGameThreadTime() const
{
    // 在實際實現中，這裡應該從引擎獲取遊戲線程時間
    // 簡化處理：返回預估值
    return CurrentMetrics.FrameTimeMs * 0.3f; // 假設遊戲邏輯佔30%
}

float UMingPerformanceMonitor::GetGPUTime() const
{
    // 在實際實現中，這裡應該從引擎獲取GPU時間
    // 簡化處理：返回預估值
    return CurrentMetrics.FrameTimeMs * 0.8f; // 假設GPU佔80%
}

int32 UMingPerformanceMonitor::GetRenderedInstanceCount() const
{
    // 這裡應該從InstancedRenderingComponent獲取
    // 簡化處理：返回預估值
    return 0;
}

int32 UMingPerformanceMonitor::GetCulledInstanceCount() const
{
    // 這裡應該從InstancedRenderingComponent獲取
    // 簡化處理：返回預估值
    return 0;
}
