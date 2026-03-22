#include "MingPerformanceBlueprintLibrary.h"
#include "MingPerformanceMonitor.h"
#include "Components/MingInstancedRenderingComponent.h"
#include "Components/MingSpatialPartitionComponent.h"
#include "Units/MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingPerformanceMonitor* UMingPerformanceBlueprintLibrary::GetPerformanceMonitor()
{
    return UMingPerformanceMonitor::Get();
}

void UMingPerformanceBlueprintLibrary::InitializePerformanceMonitor()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (!Monitor)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            Monitor = NewObject<UMingPerformanceMonitor>(World);
            Monitor->Initialize();
        }
    }
}

float UMingPerformanceBlueprintLibrary::GetCurrentFPS()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.CurrentFPS;
    }
    return 0.0f;
}

float UMingPerformanceBlueprintLibrary::GetAverageFPS()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.AverageFPS;
    }
    return 0.0f;
}

float UMingPerformanceBlueprintLibrary::GetMinFPS()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.MinFPS;
    }
    return 0.0f;
}

float UMingPerformanceBlueprintLibrary::GetMaxFPS()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.MaxFPS;
    }
    return 0.0f;
}

float UMingPerformanceBlueprintLibrary::GetFrameTimeMs()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.FrameTimeMs;
    }
    return 0.0f;
}

FString UMingPerformanceBlueprintLibrary::GetPerformanceReport()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        return Monitor->GetPerformanceReport();
    }
    return TEXT("PerformanceMonitor not initialized");
}

FString UMingPerformanceBlueprintLibrary::GetShortPerformanceText()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        return Monitor->GetShortPerformanceText();
    }
    return TEXT("N/A");
}

bool UMingPerformanceBlueprintLibrary::IsPerformanceAcceptable()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        return Monitor->IsPerformanceAcceptable();
    }
    return false;
}

void UMingPerformanceBlueprintLibrary::SetTargetFPS(float TargetFPS)
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        Monitor->SetTargetFPS(TargetFPS);
    }
}

void UMingPerformanceBlueprintLibrary::StartPerformanceRecording()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        Monitor->StartRecording();
    }
}

void UMingPerformanceBlueprintLibrary::StopPerformanceRecording()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        Monitor->StopRecording();
    }
}

bool UMingPerformanceBlueprintLibrary::ExportPerformanceReport(const FString& FilePath)
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        return Monitor->ExportReport(FilePath);
    }
    return false;
}

float UMingPerformanceBlueprintLibrary::GetMemoryUsageMB()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.UsedMemoryMB;
    }
    return 0.0f;
}

int32 UMingPerformanceBlueprintLibrary::GetRenderedInstanceCount()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.RenderedInstanceCount;
    }
    return 0;
}

int32 UMingPerformanceBlueprintLibrary::GetCulledInstanceCount()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.CulledInstanceCount;
    }
    return 0;
}

int32 UMingPerformanceBlueprintLibrary::GetActiveGridCellCount()
{
    UMingPerformanceMonitor* Monitor = UMingPerformanceMonitor::Get();
    if (Monitor)
    {
        FPerformanceMetrics Metrics = Monitor->GetCurrentMetrics();
        return Metrics.ActiveGridCellCount;
    }
    return 0;
}

void UMingPerformanceBlueprintLibrary::RegisterUnitForInstancedRendering(AMingTacticalUnit* Unit)
{
    // 這裡應該獲取場景中的InstancedRenderingComponent並註冊單位
    // 簡化處理：記錄日誌
    UE_LOG(LogTemp, Verbose, TEXT("Registering unit for instanced rendering: %s"), 
        Unit ? *Unit->GetName() : TEXT("NULL"));
}

void UMingPerformanceBlueprintLibrary::UnregisterUnitFromInstancedRendering(AMingTacticalUnit* Unit)
{
    UE_LOG(LogTemp, Verbose, TEXT("Unregistering unit from instanced rendering: %s"),
        Unit ? *Unit->GetName() : TEXT("NULL"));
}

void UMingPerformanceBlueprintLibrary::SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible)
{
    UE_LOG(LogTemp, Verbose, TEXT("Setting unit visibility: %s = %s"),
        Unit ? *Unit->GetName() : TEXT("NULL"),
        bVisible ? TEXT("Visible") : TEXT("Hidden"));
}

int32 UMingPerformanceBlueprintLibrary::GetUnitLODLevel(AMingTacticalUnit* Unit)
{
    // 簡化處理：返回預設LOD級別
    return 0;
}

void UMingPerformanceBlueprintLibrary::SetLODDistanceThresholds(const TArray<float>& Thresholds)
{
    UE_LOG(LogTemp, Log, TEXT("Setting LOD distance thresholds: %d levels"), Thresholds.Num());
}

void UMingPerformanceBlueprintLibrary::RegisterUnitForSpatialPartition(AMingTacticalUnit* Unit)
{
    UE_LOG(LogTemp, Verbose, TEXT("Registering unit for spatial partition: %s"),
        Unit ? *Unit->GetName() : TEXT("NULL"));
}

void UMingPerformanceBlueprintLibrary::UnregisterUnitFromSpatialPartition(AMingTacticalUnit* Unit)
{
    UE_LOG(LogTemp, Verbose, TEXT("Unregistering unit from spatial partition: %s"),
        Unit ? *Unit->GetName() : TEXT("NULL"));
}

TArray<AMingTacticalUnit*> UMingPerformanceBlueprintLibrary::GetUnitsInRadius(const FVector& Center, float Radius)
{
    // 簡化處理：返回空數組
    return TArray<AMingTacticalUnit*>();
}

void UMingPerformanceBlueprintLibrary::SetSpatialPartitionCellSize(float CellSize)
{
    UE_LOG(LogTemp, Log, TEXT("Setting spatial partition cell size: %.1f"), CellSize);
}

void UMingPerformanceBlueprintLibrary::SetSpatialPartitionUpdateRadius(float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Setting spatial partition update radius: %.1f"), Radius);
}

void UMingPerformanceBlueprintLibrary::GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts)
{
    // 簡化處理：返回空數組
    CellCenters.Empty();
    CellUnitCounts.Empty();
}

FString UMingPerformanceBlueprintLibrary::GetOptimizationSuggestions()
{
    FString Suggestions;
    
    Suggestions += TEXT("=== Performance Optimization Suggestions ===\n\n");
    
    float CurrentFPS = GetCurrentFPS();
    float TargetFPS = 60.0f;
    
    if (CurrentFPS < TargetFPS * 0.5f)
    {
        Suggestions += TEXT("🔴 CRITICAL: FPS is severely low!\n");
        Suggestions += TEXT("   - Consider reducing unit count\n");
        Suggestions += TEXT("   - Enable aggressive LOD\n");
        Suggestions += TEXT("   - Reduce rendering distance\n\n");
    }
    else if (CurrentFPS < TargetFPS * 0.8f)
    {
        Suggestions += TEXT("🟡 WARNING: FPS below target\n");
        Suggestions += TEXT("   - Optimize instance rendering\n");
        Suggestions += TEXT("   - Check for memory leaks\n");
        Suggestions += TEXT("   - Profile GPU usage\n\n");
    }
    else
    {
        Suggestions += TEXT("🟢 Performance is acceptable\n");
        Suggestions += TEXT("   - Continue monitoring\n");
        Suggestions += TEXT("   - Consider adding more units\n\n");
    }
    
    // 記憶體建議
    float MemoryUsage = GetMemoryUsageMB();
    if (MemoryUsage > 2048.0f)
    {
        Suggestions += TEXT("⚠️  Memory usage is high (>2GB)\n");
        Suggestions += TEXT("   - Check for memory leaks\n");
        Suggestions += TEXT("   - Optimize texture memory\n");
        Suggestions += TEXT("   - Reduce asset quality\n\n");
    }
    
    // 實例化渲染建議
    int32 RenderedCount = GetRenderedInstanceCount();
    int32 CulledCount = GetCulledInstanceCount();
    int32 TotalCount = RenderedCount + CulledCount;
    
    if (TotalCount > 0)
    {
        float CullRate = (float)CulledCount / TotalCount;
        if (CullRate < 0.1f)
        {
            Suggestions += TEXT("📊 Low culling rate detected\n");
            Suggestions += TEXT("   - Optimize view frustum culling\n");
            Suggestions += TEXT("   - Consider spatial partitioning\n\n");
        }
    }
    
    return Suggestions;
}
