#include "Performance/MingRTSPerformanceSystem.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformMemory.h"

UMingRTSPerformanceSystem::UMingRTSPerformanceSystem()
    : bMonitoringEnabled(false)
    , bDynamicResolutionEnabled(true)
    , TargetFPS(60)
    , DrawDistance(10000.0f)
    , MonitoringInterval(1.0f)
{
}

void UMingRTSPerformanceSystem::InitializePerformanceSystem()
{
    // 設置默認性能預算
    Budget = FPerformanceBudget();
    
    UE_LOG(LogTemp, Log, TEXT("Performance System Initialized. Target FPS: %d"), TargetFPS);
}

void UMingRTSPerformanceSystem::StartPerformanceMonitoring()
{
    if (bMonitoringEnabled)
    {
        return;
    }
    
    bMonitoringEnabled = true;
    StartMonitoringTimer();
    
    UE_LOG(LogTemp, Log, TEXT("Performance monitoring started"));
}

void UMingRTSPerformanceSystem::StopPerformanceMonitoring()
{
    bMonitoringEnabled = false;
    StopMonitoringTimer();
    
    UE_LOG(LogTemp, Log, TEXT("Performance monitoring stopped"));
}

FPerformanceMetrics UMingRTSPerformanceSystem::GetCurrentMetrics() const
{
    return CurrentMetrics;
}

FPerformanceBudget UMingRTSPerformanceSystem::GetPerformanceBudget() const
{
    return Budget;
}

void UMingRTSPerformanceSystem::SetPerformanceBudget(const FPerformanceBudget& InBudget)
{
    Budget = InBudget;
    UE_LOG(LogTemp, Log, TEXT("Performance budget updated. Target FPS: %d"), Budget.TargetFPS);
}

void UMingRTSPerformanceSystem::ApplyOptimizationPreset(EOptimizationPreset Preset)
{
    switch (Preset)
    {
    case EOptimizationPreset::Ultra:
        TargetFPS = 120;
        DrawDistance = 20000.0f;
        SetShadowQuality(5);
        SetTextureQuality(3);
        SetEffectsQuality(5);
        SetViewDistance(4);
        break;
        
    case EOptimizationPreset::High:
        TargetFPS = 60;
        DrawDistance = 15000.0f;
        SetShadowQuality(4);
        SetTextureQuality(2);
        SetEffectsQuality(4);
        SetViewDistance(3);
        break;
        
    case EOptimizationPreset::Medium:
        TargetFPS = 60;
        DrawDistance = 10000.0f;
        SetShadowQuality(3);
        SetTextureQuality(1);
        SetEffectsQuality(3);
        SetViewDistance(2);
        break;
        
    case EOptimizationPreset::Low:
        TargetFPS = 30;
        DrawDistance = 5000.0f;
        SetShadowQuality(2);
        SetTextureQuality(0);
        SetEffectsQuality(2);
        SetViewDistance(1);
        break;
        
    case EOptimizationPreset::Minimal:
        TargetFPS = 30;
        DrawDistance = 2000.0f;
        SetShadowQuality(1);
        SetTextureQuality(0);
        SetEffectsQuality(1);
        SetViewDistance(0);
        break;
        
    default:
        break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied optimization preset: %d"), (int32)Preset);
    OnOptimizationApplied.Broadcast();
}

TArray<FOptimizationSuggestion> UMingRTSPerformanceSystem::GetOptimizationSuggestions() const
{
    TArray<FOptimizationSuggestion> Suggestions;
    
    // 檢查FPS
    if (CurrentMetrics.CurrentFPS < Budget.MinFPS)
    {
        FOptimizationSuggestion Suggestion;
        Suggestion.Category = EPerformanceCategory::FPS;
        Suggestion.Description = FString::Printf(TEXT("FPS低於目標 (當前: %.1f, 目標: %d)"), CurrentMetrics.CurrentFPS, Budget.MinFPS);
        Suggestion.SuggestedAction = TEXT("降低渲染設置或減少同屏單位數量");
        Suggestion.Priority = EPerformanceLevel::Poor;
        Suggestion.ExpectedImprovement = 10.0f;
        Suggestions.Add(Suggestion);
    }
    
    // 檢查內存
    if (CurrentMetrics.MemoryUsageMB > Budget.MaxMemoryUsageMB)
    {
        FOptimizationSuggestion Suggestion;
        Suggestion.Category = EPerformanceCategory::Memory;
        Suggestion.Description = FString::Printf(TEXT("內存使用過高 (當前: %.1f MB, 限制: %.1f MB)"), CurrentMetrics.MemoryUsageMB, Budget.MaxMemoryUsageMB);
        Suggestion.SuggestedAction = TEXT("執行垃圾回收或降低紋理質量");
        Suggestion.Priority = EPerformanceLevel::Critical;
        Suggestion.ExpectedImprovement = 20.0f;
        Suggestions.Add(Suggestion);
    }
    
    // 檢查Draw Calls
    if (CurrentMetrics.DrawCalls > Budget.MaxDrawCalls)
    {
        FOptimizationSuggestion Suggestion;
        Suggestion.Category = EPerformanceCategory::Rendering;
        Suggestion.Description = FString::Printf(TEXT("Draw Calls過多 (當前: %d, 限制: %d)"), CurrentMetrics.DrawCalls, Budget.MaxDrawCalls);
        Suggestion.SuggestedAction = TEXT("啟用剔除或合併網格");
        Suggestion.Priority = EPerformanceLevel::Poor;
        Suggestion.ExpectedImprovement = 15.0f;
        Suggestions.Add(Suggestion);
    }
    
    // 檢查CPU負載
    if (CurrentMetrics.CPULoadPercent > Budget.MaxCPULoadPercent)
    {
        FOptimizationSuggestion Suggestion;
        Suggestion.Category = EPerformanceCategory::CPU;
        Suggestion.Description = FString::Printf(TEXT("CPU負載過高 (當前: %.1f%%)"), CurrentMetrics.CPULoadPercent);
        Suggestion.SuggestedAction = TEXT("優化AI更新頻率或減少物理計算");
        Suggestion.Priority = EPerformanceLevel::Poor;
        Suggestion.ExpectedImprovement = 12.0f;
        Suggestions.Add(Suggestion);
    }
    
    return Suggestions;
}

void UMingRTSPerformanceSystem::AutoOptimize()
{
    UE_LOG(LogTemp, Log, TEXT("Running auto optimization..."));
    
    TArray<FOptimizationSuggestion> Suggestions = GetOptimizationSuggestions();
    
    for (const FOptimizationSuggestion& Suggestion : Suggestions)
    {
        if (Suggestion.Priority == EPerformanceLevel::Critical || Suggestion.Priority == EPerformanceLevel::Poor)
        {
            UE_LOG(LogTemp, Log, TEXT("Applying optimization: %s"), *Suggestion.SuggestedAction);
        }
    }
    
    ApplyAutomaticOptimizations();
    
    OnOptimizationApplied.Broadcast();
}

void UMingRTSPerformanceSystem::SetTargetFPS(int32 InTargetFPS)
{
    TargetFPS = FMath::Clamp(InTargetFPS, 15, 240);
    Budget.TargetFPS = TargetFPS;
    UE_LOG(LogTemp, Log, TEXT("Target FPS set to: %d"), TargetFPS);
}

int32 UMingRTSPerformanceSystem::GetTargetFPS() const
{
    return TargetFPS;
}

void UMingRTSPerformanceSystem::SetDrawDistance(float Distance)
{
    DrawDistance = FMath::Clamp(Distance, 1000.0f, 50000.0f);
    UE_LOG(LogTemp, Log, TEXT("Draw distance set to: %.1f"), DrawDistance);
}

float UMingRTSPerformanceSystem::GetDrawDistance() const
{
    return DrawDistance;
}

void UMingRTSPerformanceSystem::SetShadowQuality(int32 Quality)
{
    Quality = FMath::Clamp(Quality, 0, 5);
    UE_LOG(LogTemp, Log, TEXT("Shadow quality set to: %d"), Quality);
}

void UMingRTSPerformanceSystem::SetAntiAliasing(int32 Level)
{
    Level = FMath::Clamp(Level, 0, 4);
    UE_LOG(LogTemp, Log, TEXT("Anti-aliasing set to: %d"), Level);
}

void UMingRTSPerformanceSystem::SetPostProcessQuality(int32 Quality)
{
    Quality = FMath::Clamp(Quality, 0, 5);
    UE_LOG(LogTemp, Log, TEXT("Post process quality set to: %d"), Quality);
}

void UMingRTSPerformanceSystem::SetTextureQuality(int32 Quality)
{
    Quality = FMath::Clamp(Quality, 0, 3);
    UE_LOG(LogTemp, Log, TEXT("Texture quality set to: %d"), Quality);
}

void UMingRTSPerformanceSystem::SetEffectsQuality(int32 Quality)
{
    Quality = FMath::Clamp(Quality, 0, 5);
    UE_LOG(LogTemp, Log, TEXT("Effects quality set to: %d"), Quality);
}

void UMingRTSPerformanceSystem::SetViewDistance(int32 Quality)
{
    Quality = FMath::Clamp(Quality, 0, 4);
    UE_LOG(LogTemp, Log, TEXT("View distance quality set to: %d"), Quality);
}

void UMingRTSPerformanceSystem::SetFoliageDensity(float Density)
{
    Density = FMath::Clamp(Density, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("Foliage density set to: %.2f"), Density);
}

void UMingRTSPerformanceSystem::ForceGarbageCollection()
{
    // 執行垃圾回收
    UE_LOG(LogTemp, Log, TEXT("Forcing garbage collection..."));
    // 這裡應該調用實際的垃圾回收功能
}

void UMingRTSPerformanceSystem::FlushUnusedResources()
{
    UE_LOG(LogTemp, Log, TEXT("Flushing unused resources..."));
    // 這裡應該清理未使用的資源
}

TArray<FPerformanceHistory> UMingRTSPerformanceSystem::GetPerformanceHistory(float HoursBack) const
{
    TArray<FPerformanceHistory> Result;
    
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan Lookback = FTimespan::FromHours(HoursBack);
    
    for (const FPerformanceHistory& Record : PerformanceHistory)
    {
        FDateTime RecordTime;
        if (FDateTime::Parse(Record.Timestamp, RecordTime))
        {
            if (CurrentTime - RecordTime <= Lookback)
            {
                Result.Add(Record);
            }
        }
    }
    
    return Result;
}

bool UMingRTSPerformanceSystem::ExportPerformanceReport(const FString& FilePath) const
{
    FString Report = TEXT("=== MingGoRTS Performance Report ===\n\n");
    Report += FString::Printf(TEXT("Generated: %s\n\n"), *FDateTime::Now().ToString());
    
    Report += TEXT("Current Metrics:\n");
    Report += FString::Printf(TEXT("  FPS: %.1f (Target: %d)\n"), CurrentMetrics.CurrentFPS, Budget.TargetFPS);
    Report += FString::Printf(TEXT("  Frame Time: %.2f ms\n"), CurrentMetrics.FrameTime);
    Report += FString::Printf(TEXT("  Memory: %.1f MB\n"), CurrentMetrics.MemoryUsageMB);
    Report += FString::Printf(TEXT("  Draw Calls: %d\n"), CurrentMetrics.DrawCalls);
    Report += FString::Printf(TEXT("  Triangles: %d\n"), CurrentMetrics.TriangleCount);
    
    Report += TEXT("\nPerformance Budget:\n");
    Report += FString::Printf(TEXT("  Max Draw Calls: %d\n"), Budget.MaxDrawCalls);
    Report += FString::Printf(TEXT("  Max Memory: %.1f MB\n"), Budget.MaxMemoryUsageMB);
    Report += FString::Printf(TEXT("  Max AI Agents: %d\n"), Budget.MaxAIAgents);
    
    // 這裡應該實際寫入文件
    UE_LOG(LogTemp, Log, TEXT("Performance report exported to: %s"), *FilePath);
    return true;
}

void UMingRTSPerformanceSystem::SetDynamicResolutionEnabled(bool bEnabled)
{
    bDynamicResolutionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Dynamic resolution %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingRTSPerformanceSystem::HasPerformanceIssues() const
{
    return CurrentMetrics.CurrentFPS < Budget.MinFPS ||
           CurrentMetrics.MemoryUsageMB > Budget.MaxMemoryUsageMB ||
           CurrentMetrics.CPULoadPercent > Budget.MaxCPULoadPercent ||
           CurrentMetrics.DrawCalls > Budget.MaxDrawCalls;
}

EPerformanceLevel UMingRTSPerformanceSystem::GetPerformanceLevel() const
{
    return CalculatePerformanceLevel(CurrentMetrics);
}

void UMingRTSPerformanceSystem::StartMonitoringTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MonitoringTimerHandle);
        
        if (bMonitoringEnabled && MonitoringInterval > 0)
        {
            World->GetTimerManager().SetTimer(MonitoringTimerHandle, this, 
                &UMingRTSPerformanceSystem::OnMonitoringTick, MonitoringInterval, true);
        }
    }
}

void UMingRTSPerformanceSystem::StopMonitoringTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MonitoringTimerHandle);
    }
}

void UMingRTSPerformanceSystem::OnMonitoringTick()
{
    if (!bMonitoringEnabled)
    {
        return;
    }
    
    UpdateMetrics();
    CheckBudgetViolation();
    RecordHistory();
}

void UMingRTSPerformanceSystem::UpdateMetrics()
{
    // 模擬性能數據更新
    // 實際應該從引擎獲取真實數據
    
    // 更新FPS統計
    float CurrentFrameFPS = 60.0f; // 這應該從引擎獲取
    CurrentMetrics.CurrentFPS = CurrentFrameFPS;
    CurrentMetrics.MinFPS = FMath::Min(CurrentMetrics.MinFPS, CurrentFrameFPS);
    CurrentMetrics.MaxFPS = FMath::Max(CurrentMetrics.MaxFPS, CurrentFrameFPS);
    
    // 簡化的平均FPS計算
    CurrentMetrics.AverageFPS = (CurrentMetrics.AverageFPS * 0.9f) + (CurrentFrameFPS * 0.1f);
    CurrentMetrics.FrameTime = 1000.0f / CurrentFrameFPS;
    
    // 更新內存使用
    CurrentMetrics.MemoryUsageMB = FPlatformMemory::GetStats().UsedVirtual / (1024.0f * 1024.0f);
    
    // 計算內存使用百分比
    int64 TotalMemory = FPlatformMemory::GetStats().AvailableVirtual + FPlatformMemory::GetStats().UsedVirtual;
    CurrentMetrics.MemoryUsagePercent = (CurrentMetrics.MemoryUsageMB * 1024.0f * 1024.0f / TotalMemory) * 100.0f;
    
    // 模擬其他數據
    CurrentMetrics.CPULoadPercent = FMath::Clamp(30.0f + FMath::RandRange(-10.0f, 20.0f), 0.0f, 100.0f);
    CurrentMetrics.GPULoadPercent = FMath::Clamp(40.0f + FMath::RandRange(-10.0f, 30.0f), 0.0f, 100.0f);
    CurrentMetrics.DrawCalls = 1500 + FMath::RandRange(-200, 500);
    CurrentMetrics.TriangleCount = 250000 + FMath::RandRange(-50000, 100000);
}

void UMingRTSPerformanceSystem::RecordHistory()
{
    FPerformanceHistory Record;
    Record.Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    Record.Metrics = CurrentMetrics;
    Record.LevelName = GetLevelName();
    GetObjectCounts(Record.AIUnitCount, Record.BuildingCount);
    
    PerformanceHistory.Add(Record);
    
    // 限制歷史記錄數量
    if (PerformanceHistory.Num() > 1000)
    {
        PerformanceHistory.RemoveAt(0, PerformanceHistory.Num() - 1000);
    }
}

void UMingRTSPerformanceSystem::CheckBudgetViolation()
{
    // 檢查FPS
    if (CurrentMetrics.CurrentFPS < Budget.MinFPS)
    {
        OnFPSDropped.Broadcast(CurrentMetrics.CurrentFPS);
        
        // 如果啟用了動態分辨率，嘗試降低
        if (bDynamicResolutionEnabled)
        {
            ApplyAutomaticOptimizations();
        }
    }
    
    // 檢查內存
    if (CurrentMetrics.MemoryUsageMB > Budget.MaxMemoryUsageMB * 0.9f)
    {
        OnMemoryWarning.Broadcast(CurrentMetrics.MemoryUsageMB);
    }
    
    // 檢查性能級別變化
    EPerformanceLevel CurrentLevel = CalculatePerformanceLevel(CurrentMetrics);
    static EPerformanceLevel LastLevel = EPerformanceLevel::Excellent;
    
    if (CurrentLevel != LastLevel)
    {
        OnPerformanceLevelChanged.Broadcast(CurrentLevel);
        LastLevel = CurrentLevel;
    }
}

EPerformanceLevel UMingRTSPerformanceSystem::CalculatePerformanceLevel(const FPerformanceMetrics& Metrics) const
{
    if (Metrics.CurrentFPS >= Budget.TargetFPS)
    {
        return EPerformanceLevel::Excellent;
    }
    else if (Metrics.CurrentFPS >= Budget.TargetFPS * 0.9f)
    {
        return EPerformanceLevel::Good;
    }
    else if (Metrics.CurrentFPS >= Budget.MinFPS)
    {
        return EPerformanceLevel::Acceptable;
    }
    else if (Metrics.CurrentFPS >= Budget.MinFPS * 0.8f)
    {
        return EPerformanceLevel::Poor;
    }
    else
    {
        return EPerformanceLevel::Critical;
    }
}

void UMingRTSPerformanceSystem::ApplyAutomaticOptimizations()
{
    UE_LOG(LogTemp, Log, TEXT("Applying automatic optimizations..."));
    
    // 根據性能問題應用不同的優化
    if (CurrentMetrics.CurrentFPS < Budget.MinFPS * 0.8f)
    {
        ReduceDrawDistance();
        ReduceEffects();
    }
    
    if (CurrentMetrics.DrawCalls > Budget.MaxDrawCalls)
    {
        ReduceShadowQuality();
    }
    
    if (CurrentMetrics.CPULoadPercent > Budget.MaxCPULoadPercent)
    {
        OptimizeAI();
    }
    
    if (CurrentMetrics.MemoryUsageMB > Budget.MaxMemoryUsageMB)
    {
        ReduceTextureQuality();
        ForceGarbageCollection();
    }
}

void UMingRTSPerformanceSystem::ReduceDrawDistance()
{
    DrawDistance *= 0.8f;
    UE_LOG(LogTemp, Log, TEXT("Reduced draw distance to: %.1f"), DrawDistance);
}

void UMingRTSPerformanceSystem::ReduceShadowQuality()
{
    UE_LOG(LogTemp, Log, TEXT("Reducing shadow quality..."));
}

void UMingRTSPerformanceSystem::ReduceEffects()
{
    UE_LOG(LogTemp, Log, TEXT("Reducing effects quality..."));
}

void UMingRTSPerformanceSystem::EnableLevelStreaming()
{
    UE_LOG(LogTemp, Log, TEXT("Enabling level streaming..."));
}

void UMingRTSPerformanceSystem::OptimizeAI()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing AI updates..."));
}

void UMingRTSPerformanceSystem::ReduceTextureQuality()
{
    UE_LOG(LogTemp, Log, TEXT("Reducing texture quality..."));
}

FString UMingRTSPerformanceSystem::GetLevelName() const
{
    return TEXT("CurrentLevel");
}

void UMingRTSPerformanceSystem::GetObjectCounts(int32& OutAIUnits, int32& OutBuildings) const
{
    OutAIUnits = 0;
    OutBuildings = 0;
}
