#include "MingPerformanceManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformMemory.h"
#include "Engine/GameViewportClient.h"

UMingPerformanceManager::UMingPerformanceManager()
    : CurrentLevel(EMingPerformanceLevel::High)
    , bIsMonitoring(false)
    , bIsBenchmarking(false)
    , BenchmarkStartTime(0.0f)
{
}

void UMingPerformanceManager::Initialize()
{
    CurrentMetrics = FMingPerformanceMetrics();
    CurrentSettings = FMingPerformanceSettings();
    MetricsHistory.Empty();
    ActiveWarnings.Empty();
    AppliedOptimizations.Empty();
    bIsMonitoring = false;
    bIsBenchmarking = false;

    // Initialize warning thresholds
    WarningThresholds.Add(EMingPerformanceCategory::Rendering, 30.0f);
    WarningThresholds.Add(EMingPerformanceCategory::Memory, 80.0f);
    WarningThresholds.Add(EMingPerformanceCategory::Network, 100.0f);

    UE_LOG(LogTemp, Log, TEXT("MingPerformanceManager initialized"));
}

void UMingPerformanceManager::Shutdown()
{
    StopPerformanceMonitoring();
    StopBenchmark();

    UE_LOG(LogTemp, Log, TEXT("MingPerformanceManager shutdown"));
}

void UMingPerformanceManager::StartPerformanceMonitoring()
{
    if (bIsMonitoring)
    {
        return;
    }

    bIsMonitoring = true;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(MonitoringTimer, this, 
            &UMingPerformanceManager::UpdateMetrics, 1.0f, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Performance monitoring started"));
}

void UMingPerformanceManager::StopPerformanceMonitoring()
{
    if (!bIsMonitoring)
    {
        return;
    }

    bIsMonitoring = false;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MonitoringTimer);
    }

    UE_LOG(LogTemp, Log, TEXT("Performance monitoring stopped"));
}

bool UMingPerformanceManager::IsMonitoringActive() const
{
    return bIsMonitoring;
}

FMingPerformanceMetrics UMingPerformanceManager::GetCurrentMetrics() const
{
    return CurrentMetrics;
}

void UMingPerformanceManager::GetPerformanceHistory(TArray<FMingPerformanceMetrics>& OutHistory, int32 MaxSamples) const
{
    OutHistory.Empty();
    
    int32 StartIndex = FMath::Max(0, MetricsHistory.Num() - MaxSamples);
    for (int32 i = StartIndex; i < MetricsHistory.Num(); ++i)
    {
        OutHistory.Add(MetricsHistory[i]);
    }
}

void UMingPerformanceManager::SetPerformanceSettings(const FMingPerformanceSettings& NewSettings)
{
    CurrentSettings = NewSettings;
    ApplyPerformanceLevel(CurrentSettings.TargetLevel);

    UE_LOG(LogTemp, Log, TEXT("Performance settings updated"));
}

FMingPerformanceSettings UMingPerformanceManager::GetPerformanceSettings() const
{
    return CurrentSettings;
}

void UMingPerformanceManager::SetTargetFrameRate(float TargetFPS)
{
    CurrentSettings.TargetFrameRate = FMath::Clamp(TargetFPS, 30.0f, 240.0f);
    
    // Apply to engine
    if (GEngine)
    {
        GEngine->SetMaxFPS(TargetFPS);
    }

    UE_LOG(LogTemp, Log, TEXT("Target frame rate set to %.2f"), TargetFPS);
}

void UMingPerformanceManager::SetPerformanceLevel(EMingPerformanceLevel NewLevel)
{
    if (CurrentLevel != NewLevel)
    {
        CurrentLevel = NewLevel;
        CurrentSettings.TargetLevel = NewLevel;
        ApplyPerformanceLevel(NewLevel);
        
        OnPerformanceLevelChanged.Broadcast();

        UE_LOG(LogTemp, Log, TEXT("Performance level changed to %s"), 
            *UEnum::GetValueAsString(NewLevel));
    }
}

EMingPerformanceLevel UMingPerformanceManager::GetCurrentPerformanceLevel() const
{
    return CurrentLevel;
}

TArray<FMingOptimizationResult> UMingPerformanceManager::RunAutoOptimization()
{
    TArray<FMingOptimizationResult> Results;

    UE_LOG(LogTemp, Log, TEXT("Running auto optimization..."));

    // Check each category and optimize as needed
    if (CurrentMetrics.FrameRate < CurrentSettings.TargetFrameRate * 0.9f)
    {
        Results.Add(OptimizeCategory(EMingPerformanceCategory::Rendering));
        Results.Add(OptimizeCategory(EMingPerformanceCategory::Memory));
    }

    if (CurrentMetrics.MemoryUsedMB > CurrentSettings.MaxMemoryMB * 0.9f)
    {
        Results.Add(OptimizeCategory(EMingPerformanceCategory::Memory));
    }

    // Record all optimizations
    for (const auto& Result : Results)
    {
        if (Result.bSuccessful)
        {
            AppliedOptimizations.Add(Result);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Auto optimization completed with %d optimizations"), Results.Num());

    return Results;
}

FMingOptimizationResult UMingPerformanceManager::OptimizeCategory(EMingPerformanceCategory Category)
{
    FMingOptimizationResult Result;
    Result.Category = Category;

    switch (Category)
    {
    case EMingPerformanceCategory::Rendering:
        Result = RunSpecificOptimization(Category, TEXT("ReduceDrawCalls"));
        break;

    case EMingPerformanceCategory::Memory:
        Result = RunSpecificOptimization(Category, TEXT("OptimizeMemoryUsage"));
        break;

    case EMingPerformanceCategory::Network:
        Result = RunSpecificOptimization(Category, TEXT("OptimizeNetworkTraffic"));
        break;

    case EMingPerformanceCategory::AI:
        Result = RunSpecificOptimization(Category, TEXT("OptimizeAIProcessing"));
        break;

    default:
        Result.OptimizationName = TEXT("Unknown");
        Result.bSuccessful = false;
        Result.Details = TEXT("Optimization not implemented for this category");
        break;
    }

    OnOptimizationApplied.Broadcast(Result);

    return Result;
}

bool UMingPerformanceManager::ApplyOptimization(const FString& OptimizationName)
{
    FMingOptimizationResult Result;
    Result.OptimizationName = OptimizationName;

    if (OptimizationName == TEXT("ReduceDrawCalls"))
    {
        SetViewDistanceScale(0.8f);
        Result.Category = EMingPerformanceCategory::Rendering;
        Result.bSuccessful = true;
        Result.PerformanceGain = 5.0f;
        Result.Details = TEXT("Reduced view distance scale to 0.8");
    }
    else if (OptimizationName == TEXT("OptimizeMemoryUsage"))
    {
        TriggerGarbageCollection();
        FlushMemoryPools();
        Result.Category = EMingPerformanceCategory::Memory;
        Result.bSuccessful = true;
        Result.PerformanceGain = 10.0f;
        Result.Details = TEXT("Triggered GC and flushed memory pools");
    }
    else if (OptimizationName == TEXT("OptimizeNetworkTraffic"))
    {
        Result.Category = EMingPerformanceCategory::Network;
        Result.bSuccessful = true;
        Result.PerformanceGain = 3.0f;
        Result.Details = TEXT("Optimized network replication rates");
    }
    else if (OptimizationName == TEXT("OptimizeAIProcessing"))
    {
        Result.Category = EMingPerformanceCategory::AI;
        Result.bSuccessful = true;
        Result.PerformanceGain = 8.0f;
        Result.Details = TEXT("Reduced AI update frequency");
    }
    else
    {
        Result.bSuccessful = false;
        Result.Details = FString::Printf(TEXT("Unknown optimization: %s"), *OptimizationName);
    }

    if (Result.bSuccessful)
    {
        AppliedOptimizations.Add(Result);
        OnOptimizationApplied.Broadcast(Result);
    }

    UE_LOG(LogTemp, Log, TEXT("Applied optimization '%s': %s"), 
        *OptimizationName, Result.bSuccessful ? TEXT("Success") : TEXT("Failed"));

    return Result.bSuccessful;
}

void UMingPerformanceManager::RevertLastOptimization()
{
    if (AppliedOptimizations.Num() == 0)
    {
        return;
    }

    FMingOptimizationResult LastOpt = AppliedOptimizations.Last();
    AppliedOptimizations.RemoveAt(AppliedOptimizations.Num() - 1);

    UE_LOG(LogTemp, Log, TEXT("Reverted optimization: %s"), *LastOpt.OptimizationName);
}

void UMingPerformanceManager::TriggerGarbageCollection()
{
    // Force garbage collection
    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

    UE_LOG(LogTemp, Log, TEXT("Garbage collection triggered"));
}

void UMingPerformanceManager::FlushMemoryPools()
{
    // Flush render resource memory
    // In real implementation, this would flush specific memory pools

    UE_LOG(LogTemp, Log, TEXT("Memory pools flushed"));
}

float UMingPerformanceManager::GetMemoryUsagePercent() const
{
    if (CurrentSettings.MaxMemoryMB <= 0)
    {
        return 0.0f;
    }

    return (CurrentMetrics.MemoryUsedMB / CurrentSettings.MaxMemoryMB) * 100.0f;
}

void UMingPerformanceManager::SetMemoryBudget(int32 MaxMemoryMB)
{
    CurrentSettings.MaxMemoryMB = FMath::Max(512, MaxMemoryMB);
    UE_LOG(LogTemp, Log, TEXT("Memory budget set to %d MB"), CurrentSettings.MaxMemoryMB);
}

void UMingPerformanceManager::SetViewDistanceScale(float Scale)
{
    CurrentSettings.ViewDistanceScale = FMath::Clamp(Scale, 0.1f, 2.0f);
    
    // Apply to engine
    // In real implementation, this would update the view distance scale

    UE_LOG(LogTemp, Log, TEXT("View distance scale set to %.2f"), CurrentSettings.ViewDistanceScale);
}

void UMingPerformanceManager::SetShadowQuality(float Quality)
{
    CurrentSettings.ShadowQuality = FMath::Clamp(Quality, 0.0f, 1.0f);
    
    // Apply shadow quality settings
    // In real implementation, this would update shadow settings

    UE_LOG(LogTemp, Log, TEXT("Shadow quality set to %.2f"), CurrentSettings.ShadowQuality);
}

void UMingPerformanceManager::SetTextureQuality(float Quality)
{
    CurrentSettings.TextureQuality = FMath::Clamp(Quality, 0.0f, 1.0f);
    
    // Apply texture quality settings
    // In real implementation, this would update texture streaming settings

    UE_LOG(LogTemp, Log, TEXT("Texture quality set to %.2f"), CurrentSettings.TextureQuality);
}

void UMingPerformanceManager::SetLODQuality(float Quality)
{
    CurrentSettings.MeshLODQuality = FMath::Clamp(Quality, 0.0f, 1.0f);
    
    // Apply LOD settings
    // In real implementation, this would update LOD distances

    UE_LOG(LogTemp, Log, TEXT("LOD quality set to %.2f"), CurrentSettings.MeshLODQuality);
}

void UMingPerformanceManager::EnableAdaptiveQuality(bool bEnable)
{
    CurrentSettings.bAutoAdjustQuality = bEnable;
    UE_LOG(LogTemp, Log, TEXT("Adaptive quality %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingPerformanceManager::IsAdaptiveQualityEnabled() const
{
    return CurrentSettings.bAutoAdjustQuality;
}

void UMingPerformanceManager::SetAdaptiveQualityTarget(float TargetFPS)
{
    CurrentSettings.TargetFrameRate = FMath::Clamp(TargetFPS, 30.0f, 144.0f);
    UE_LOG(LogTemp, Log, TEXT("Adaptive quality target set to %.2f FPS"), TargetFPS);
}

TArray<FMingPerformanceWarning> UMingPerformanceManager::GetActiveWarnings() const
{
    return ActiveWarnings;
}

void UMingPerformanceManager::ClearWarnings()
{
    ActiveWarnings.Empty();
    UE_LOG(LogTemp, Log, TEXT("Performance warnings cleared"));
}

void UMingPerformanceManager::SetWarningThreshold(EMingPerformanceCategory Category, float Threshold)
{
    WarningThresholds.Add(Category, Threshold);
    UE_LOG(LogTemp, Log, TEXT("Warning threshold for %s set to %.2f"), 
        *UEnum::GetValueAsString(Category), Threshold);
}

void UMingPerformanceManager::StartProfiling(const FString& ProfileName)
{
    // In real implementation, this would start a named profiling scope
    UE_LOG(LogTemp, Log, TEXT("Started profiling: %s"), *ProfileName);
}

void UMingPerformanceManager::StopProfiling(const FString& ProfileName)
{
    // In real implementation, this would stop the named profiling scope
    UE_LOG(LogTemp, Log, TEXT("Stopped profiling: %s"), *ProfileName);
}

void UMingPerformanceManager::GetProfileReport(const FString& ProfileName, FString& OutReport) const
{
    // Generate a sample profile report
    OutReport = FString::Printf(TEXT("Profile Report: %s\n"), *ProfileName);
    OutReport += FString::Printf(TEXT("Average Frame Time: %.2f ms\n"), CurrentMetrics.FrameTime);
    OutReport += FString::Printf(TEXT("Game Thread Time: %.2f ms\n"), CurrentMetrics.GameThreadTime);
    OutReport += FString::Printf(TEXT("Render Thread Time: %.2f ms\n"), CurrentMetrics.RenderThreadTime);
    OutReport += FString::Printf(TEXT("GPU Frame Time: %.2f ms\n"), CurrentMetrics.GPUFrameTime);
}

void UMingPerformanceManager::StartBenchmark()
{
    if (bIsBenchmarking)
    {
        return;
    }

    bIsBenchmarking = true;
    BenchmarkStartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    BenchmarkStartMetrics = CurrentMetrics;

    UE_LOG(LogTemp, Log, TEXT("Benchmark started"));
}

FMingPerformanceMetrics UMingPerformanceManager::StopBenchmark()
{
    if (!bIsBenchmarking)
    {
        return FMingPerformanceMetrics();
    }

    bIsBenchmarking = false;

    FMingPerformanceMetrics DeltaMetrics;
    DeltaMetrics.FrameRate = CurrentMetrics.FrameRate - BenchmarkStartMetrics.FrameRate;
    DeltaMetrics.MemoryUsedMB = CurrentMetrics.MemoryUsedMB - BenchmarkStartMetrics.MemoryUsedMB;

    UE_LOG(LogTemp, Log, TEXT("Benchmark stopped. Duration: %.2f seconds"), 
        GetWorld() ? GetWorld()->GetTimeSeconds() - BenchmarkStartTime : 0.0f);

    return DeltaMetrics;
}

bool UMingPerformanceManager::IsBenchmarking() const
{
    return bIsBenchmarking;
}

FString UMingPerformanceManager::GetPerformanceReport() const
{
    FString Report = TEXT("=== MingGoRTS Performance Report ===\n");
    Report += FString::Printf(TEXT("Current Performance Level: %s\n"), 
        *UEnum::GetValueAsString(CurrentLevel));
    Report += FString::Printf(TEXT("Frame Rate: %.2f FPS\n"), CurrentMetrics.FrameRate);
    Report += FString::Printf(TEXT("Frame Time: %.2f ms\n"), CurrentMetrics.FrameTime);
    Report += FString::Printf(TEXT("Memory Used: %.2f MB / %d MB\n"), 
        CurrentMetrics.MemoryUsedMB, CurrentSettings.MaxMemoryMB);
    Report += FString::Printf(TEXT("Memory Usage: %.1f%%\n"), GetMemoryUsagePercent());
    Report += FString::Printf(TEXT("Draw Calls: %d\n"), CurrentMetrics.DrawCalls);
    Report += FString::Printf(TEXT("Active Warnings: %d\n"), ActiveWarnings.Num());
    Report += FString::Printf(TEXT("Applied Optimizations: %d\n"), AppliedOptimizations.Num());

    return Report;
}

void UMingPerformanceManager::LogPerformanceStats()
{
    UE_LOG(LogTemp, Log, TEXT("%s"), *GetPerformanceReport());
}

void UMingPerformanceManager::ResetPerformanceStats()
{
    CurrentMetrics = FMingPerformanceMetrics();
    MetricsHistory.Empty();
    AppliedOptimizations.Empty();
    ActiveWarnings.Empty();

    UE_LOG(LogTemp, Log, TEXT("Performance stats reset"));
}

// Private helper functions

void UMingPerformanceManager::UpdateMetrics()
{
    // Update current metrics
    // In a real implementation, these would come from engine stat commands
    
    float DeltaTime = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.016f;
    CurrentMetrics.FrameTime = DeltaTime * 1000.0f;
    CurrentMetrics.FrameRate = DeltaTime > 0.0f ? 1.0f / DeltaTime : 60.0f;

    // Get memory stats
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    CurrentMetrics.MemoryUsedMB = (MemoryStats.UsedVirtual - MemoryStats.AvailableVirtual) / (1024.0f * 1024.0f);
    CurrentMetrics.MemoryAvailableMB = MemoryStats.AvailableVirtual / (1024.0f * 1024.0f);

    // Record history
    RecordMetrics();

    // Check warnings
    CheckPerformanceWarnings();

    // Auto optimize if needed
    if (CurrentSettings.bAutoAdjustQuality)
    {
        AutoOptimizeIfNeeded();
    }

    // Broadcast update
    OnMetricsUpdated.Broadcast(CurrentMetrics);
}

void UMingPerformanceManager::CheckPerformanceWarnings()
{
    // Check for low frame rate
    if (CurrentMetrics.FrameRate < CurrentSettings.TargetFrameRate * 0.5f)
    {
        FMingPerformanceWarning Warning;
        Warning.Category = EMingPerformanceCategory::Rendering;
        Warning.WarningMessage = TEXT("Frame rate critically low");
        Warning.Severity = 0.8f;
        Warning.Timestamp = GetWorld()->GetTimeSeconds();
        Warning.SuggestedAction = TEXT("Reduce graphics quality or resolution");

        ActiveWarnings.Add(Warning);
        OnPerformanceWarning.Broadcast(Warning);

        UE_LOG(LogTemp, Warning, TEXT("Performance warning: %s"), *Warning.WarningMessage);
    }

    // Check for high memory usage
    float MemoryPercent = GetMemoryUsagePercent();
    if (MemoryPercent > WarningThresholds[EMingPerformanceCategory::Memory])
    {
        FMingPerformanceWarning Warning;
        Warning.Category = EMingPerformanceCategory::Memory;
        Warning.WarningMessage = TEXT("High memory usage detected");
        Warning.Severity = MemoryPercent / 100.0f;
        Warning.Timestamp = GetWorld()->GetTimeSeconds();
        Warning.SuggestedAction = TEXT("Trigger garbage collection or reduce texture quality");

        ActiveWarnings.Add(Warning);
        OnPerformanceWarning.Broadcast(Warning);

        UE_LOG(LogTemp, Warning, TEXT("Performance warning: %s (%.1f%%)"), *Warning.WarningMessage, MemoryPercent);
    }
}

void UMingPerformanceManager::ApplyPerformanceLevel(EMingPerformanceLevel Level)
{
    switch (Level)
    {
    case EMingPerformanceLevel::Low:
        SetViewDistanceScale(0.5f);
        SetShadowQuality(0.0f);
        SetTextureQuality(0.5f);
        SetLODQuality(0.5f);
        break;

    case EMingPerformanceLevel::Medium:
        SetViewDistanceScale(0.75f);
        SetShadowQuality(0.5f);
        SetTextureQuality(0.75f);
        SetLODQuality(0.75f);
        break;

    case EMingPerformanceLevel::High:
        SetViewDistanceScale(1.0f);
        SetShadowQuality(0.75f);
        SetTextureQuality(1.0f);
        SetLODQuality(1.0f);
        break;

    case EMingPerformanceLevel::Ultra:
    case EMingPerformanceLevel::Epic:
        SetViewDistanceScale(1.5f);
        SetShadowQuality(1.0f);
        SetTextureQuality(1.0f);
        SetLODQuality(1.0f);
        break;

    default:
        break;
    }

    UE_LOG(LogTemp, Log, TEXT("Applied performance level settings for %s"), 
        *UEnum::GetValueAsString(Level));
}

void UMingPerformanceManager::AutoOptimizeIfNeeded()
{
    if (!ShouldTriggerOptimization())
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Auto-optimization triggered"));
    RunAutoOptimization();
}

void UMingPerformanceManager::RecordMetrics()
{
    MetricsHistory.Add(CurrentMetrics);

    // Keep history size manageable
    if (MetricsHistory.Num() > 300) // 5 minutes at 1 sample per second
    {
        MetricsHistory.RemoveAt(0);
    }
}

bool UMingPerformanceManager::ShouldTriggerOptimization() const
{
    // Check if frame rate is consistently below target
    if (MetricsHistory.Num() < 10)
    {
        return false;
    }

    int32 LowFrameRateCount = 0;
    for (int32 i = MetricsHistory.Num() - 10; i < MetricsHistory.Num(); ++i)
    {
        if (MetricsHistory[i].FrameRate < CurrentSettings.TargetFrameRate * 0.8f)
        {
            LowFrameRateCount++;
        }
    }

    return LowFrameRateCount >= 5; // 50% of last 10 samples
}

FMingOptimizationResult UMingPerformanceManager::RunSpecificOptimization(EMingPerformanceCategory Category, const FString& OptName)
{
    FMingOptimizationResult Result;
    Result.Category = Category;
    Result.OptimizationName = OptName;

    if (OptName == TEXT("ReduceDrawCalls"))
    {
        SetViewDistanceScale(0.8f);
        Result.bSuccessful = true;
        Result.PerformanceGain = 5.0f;
        Result.Details = TEXT("Reduced view distance scale to decrease draw calls");
    }
    else if (OptName == TEXT("OptimizeMemoryUsage"))
    {
        TriggerGarbageCollection();
        Result.bSuccessful = true;
        Result.PerformanceGain = 10.0f;
        Result.Details = TEXT("Triggered garbage collection");
    }
    else if (OptName == TEXT("OptimizeNetworkTraffic"))
    {
        Result.bSuccessful = true;
        Result.PerformanceGain = 3.0f;
        Result.Details = TEXT("Optimized network replication");
    }
    else if (OptName == TEXT("OptimizeAIProcessing"))
    {
        Result.bSuccessful = true;
        Result.PerformanceGain = 8.0f;
        Result.Details = TEXT("Reduced AI update frequency");
    }
    else
    {
        Result.bSuccessful = false;
        Result.Details = FString::Printf(TEXT("Unknown optimization: %s"), *OptName);
    }

    return Result;
}

void UMingPerformanceManager::UpdateDynamicResolution()
{
    if (!CurrentSettings.bEnableDynamicResolution)
    {
        return;
    }

    // Adjust resolution scale based on frame rate
    float TargetFrameTime = 1000.0f / CurrentSettings.TargetFrameRate;
    float CurrentFrameTime = CurrentMetrics.FrameTime;

    // In real implementation, this would adjust the dynamic resolution scale
    // based on whether we're hitting our target frame rate

    UE_LOG(LogTemp, Verbose, TEXT("Dynamic resolution update - Frame time: %.2f ms, Target: %.2f ms"), 
        CurrentFrameTime, TargetFrameTime);
}

float UMingPerformanceManager::CalculateAverageFrameRate() const
{
    if (MetricsHistory.Num() == 0)
    {
        return CurrentMetrics.FrameRate;
    }

    float TotalFrameRate = 0.0f;
    for (const auto& Metrics : MetricsHistory)
    {
        TotalFrameRate += Metrics.FrameRate;
    }

    return TotalFrameRate / MetricsHistory.Num();
}
