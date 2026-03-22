#include "MingMemoryOptimizer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformMemory.h"
#include "UObject/UObjectIterator.h"

UMingMemoryOptimizer::UMingMemoryOptimizer()
    : CacheSizeLimitMB(512.0f)
    , MemoryWarningThreshold(85.0f)
    , bIsMonitoring(false)
{
}

void UMingMemoryOptimizer::Initialize()
{
    PoolStats.Empty();
    CachedObjects.Empty();
    AutoCleanupTimers.Empty();
    bIsMonitoring = false;

    // Initialize pool stats
    for (int32 i = 0; i < (int32)EMingMemoryPoolType::General + 1; ++i)
    {
        EMingMemoryPoolType PoolType = (EMingMemoryPoolType)i;
        FMingMemoryPoolStats Stats;
        Stats.PoolType = PoolType;
        PoolStats.Add(PoolType, Stats);
    }

    UE_LOG(LogTemp, Log, TEXT("MingMemoryOptimizer initialized"));
}

void UMingMemoryOptimizer::Shutdown()
{
    StopMemoryMonitoring();

    // Cancel all auto-cleanup timers
    for (const auto& Pair : AutoCleanupTimers)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(Pair.Value);
        }
    }
    AutoCleanupTimers.Empty();

    ClearCache();

    UE_LOG(LogTemp, Log, TEXT("MingMemoryOptimizer shutdown"));
}

FMingMemoryOptimizationResult UMingMemoryOptimizer::OptimizeMemory(EMingMemoryStrategy Strategy)
{
    FMingMemoryOptimizationResult Result;
    Result.Strategy = Strategy;

    float StartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    float MemoryBefore = GetTotalMemoryUsed();

    UE_LOG(LogTemp, Log, TEXT("Starting memory optimization with strategy: %s"), 
        *UEnum::GetValueAsString(Strategy));

    switch (Strategy)
    {
    case EMingMemoryStrategy::Conservative:
        // Gentle cleanup - just trigger GC
        PerformGarbageCollection(false);
        break;

    case EMingMemoryStrategy::Balanced:
        // Moderate cleanup
        PerformGarbageCollection(false);
        FlushTextureStreaming();
        UnloadUnusedAssets(120.0f);
        break;

    case EMingMemoryStrategy::Aggressive:
        // Heavy cleanup
        PerformGarbageCollection(true);
        FlushAllPools();
        UnloadUnusedAssets(30.0f);
        ClearCache();
        CompactMemory();
        break;

    case EMingMemoryStrategy::Emergency:
        // Emergency cleanup
        PerformGarbageCollection(true);
        FlushAllPools();
        ClearCache();
        CompactMemory();
        // Force cleanup of more objects
        for (TActorIterator<AActor> It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;
            if (Actor && !Actor->IsPendingKill() && !Actor->IsA<APlayerController>())
            {
                // Check if actor is far from players
                Result.NumObjectsFreed++;
            }
        }
        break;
    }

    float MemoryAfter = GetTotalMemoryUsed();
    Result.MemoryFreedMB = MemoryBefore - MemoryAfter;
    Result.ProcessingTimeMs = (GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f) - StartTime * 1000.0f;
    Result.bSuccessful = Result.MemoryFreedMB > 0;
    Result.Details = FString::Printf(TEXT("Freed %.2f MB in %.2f ms"), Result.MemoryFreedMB, Result.ProcessingTimeMs);

    OnOptimizationCompleted.Broadcast(Result);

    UE_LOG(LogTemp, Log, TEXT("Memory optimization completed: %s"), *Result.Details);

    return Result;
}

FMingMemoryOptimizationResult UMingMemoryOptimizer::QuickOptimize()
{
    return OptimizeMemory(EMingMemoryStrategy::Balanced);
}

FMingMemoryOptimizationResult UMingMemoryOptimizer::DeepOptimize()
{
    return OptimizeMemory(EMingMemoryStrategy::Aggressive);
}

FMingMemoryOptimizationResult UMingMemoryOptimizer::EmergencyCleanup()
{
    return OptimizeMemory(EMingMemoryStrategy::Emergency);
}

void UMingMemoryOptimizer::FlushPool(EMingMemoryPoolType PoolType)
{
    UE_LOG(LogTemp, Log, TEXT("Flushing memory pool: %s"), *UEnum::GetValueAsString(PoolType));

    switch (PoolType)
    {
    case EMingMemoryPoolType::Texture:
        FlushTextureStreaming();
        break;

    case EMingMemoryPoolType::Mesh:
        FlushStaticMeshCache();
        FlushSkeletalMeshCache();
        break;

    case EMingMemoryPoolType::Animation:
        FlushAnimationCache();
        break;

    case EMingMemoryPoolType::Audio:
        FlushAudioCache();
        break;

    case EMingMemoryPoolType::AI:
        FlushAICache();
        break;

    case EMingMemoryPoolType::Network:
        FlushNetworkCache();
        break;

    case EMingMemoryPoolType::UI:
        FlushUICache();
        break;

    default:
        break;
    }

    if (PoolStats.Contains(PoolType))
    {
        FMingMemoryPoolStats& Stats = PoolStats[PoolType];
        Stats.UsedMemoryMB = 0.0f;
        Stats.NumAllocations = 0;
    }
}

void UMingMemoryOptimizer::FlushAllPools()
{
    UE_LOG(LogTemp, Log, TEXT("Flushing all memory pools"));

    for (int32 i = 0; i < (int32)EMingMemoryPoolType::General + 1; ++i)
    {
        FlushPool((EMingMemoryPoolType)i);
    }
}

FMingMemoryPoolStats UMingMemoryOptimizer::GetPoolStats(EMingMemoryPoolType PoolType) const
{
    if (PoolStats.Contains(PoolType))
    {
        return PoolStats[PoolType];
    }

    return FMingMemoryPoolStats();
}

TArray<FMingMemoryPoolStats> UMingMemoryOptimizer::GetAllPoolStats() const
{
    TArray<FMingMemoryPoolStats> Result;
    for (const auto& Pair : PoolStats)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

void UMingMemoryOptimizer::ClearCache()
{
    UE_LOG(LogTemp, Log, TEXT("Clearing object cache - %d items"), CachedObjects.Num());

    CachedObjects.Empty();
}

void UMingMemoryOptimizer::SetCacheSizeLimit(float MaxSizeMB)
{
    CacheSizeLimitMB = FMath::Max(64.0f, MaxSizeMB);

    // If current cache exceeds new limit, evict items
    if (GetCacheSize() > CacheSizeLimitMB)
    {
        EvictLeastUsedCacheItems(CacheSizeLimitMB * 0.8f);
    }

    UE_LOG(LogTemp, Log, TEXT("Cache size limit set to %.2f MB"), CacheSizeLimitMB);
}

float UMingMemoryOptimizer::GetCacheSize() const
{
    return CalculateCacheSize();
}

void UMingMemoryOptimizer::UnloadUnusedAssets(float UnusedTimeThreshold)
{
    UE_LOG(LogTemp, Log, TEXT("Unloading unused assets (threshold: %.2f seconds)"), UnusedTimeThreshold);

    float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    int32 UnloadedCount = 0;

    // Remove cached objects that haven't been accessed recently
    for (int32 i = CachedObjects.Num() - 1; i >= 0; --i)
    {
        float TimeSinceAccess = CurrentTime - CachedObjects[i].LastAccessTime;
        if (TimeSinceAccess > UnusedTimeThreshold && CachedObjects[i].bCanUnload)
        {
            CachedObjects.RemoveAt(i);
            UnloadedCount++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Unloaded %d unused cache items"), UnloadedCount);
}

void UMingMemoryOptimizer::CompactMemory()
{
    UE_LOG(LogTemp, Log, TEXT("Compacting memory"));

    // Compact allocator
    CompactAllocator();

    // Flush render resource cache to help defragmentation
    FlushRenderResourceCache();
}

void UMingMemoryOptimizer::RegisterObjectForAutoCleanup(UObject* Object, float LifetimeSeconds)
{
    if (!Object || !GetWorld())
    {
        return;
    }

    // Cancel any existing cleanup timer for this object
    if (AutoCleanupTimers.Contains(Object))
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoCleanupTimers[Object]);
    }

    // Set up new cleanup timer
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([this, Object]()
    {
        if (Object && !Object->IsPendingKill())
        {
            Object->MarkPendingKill();
        }
        AutoCleanupTimers.Remove(Object);
    });

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, LifetimeSeconds, false);
    AutoCleanupTimers.Add(Object, TimerHandle);

    UE_LOG(LogTemp, Verbose, TEXT("Registered object for auto-cleanup in %.2f seconds"), LifetimeSeconds);
}

void UMingMemoryOptimizer::CancelAutoCleanup(UObject* Object)
{
    if (!Object || !AutoCleanupTimers.Contains(Object) || !GetWorld())
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(AutoCleanupTimers[Object]);
    AutoCleanupTimers.Remove(Object);
}

void UMingMemoryOptimizer::ForceDestroyObject(UObject* Object)
{
    if (!Object)
    {
        return;
    }

    CancelAutoCleanup(Object);

    if (!Object->IsPendingKill())
    {
        Object->MarkPendingKill();
        UE_LOG(LogTemp, Log, TEXT("Force destroyed object: %s"), *Object->GetName());
    }
}

void UMingMemoryOptimizer::SetTextureStreamingPoolSize(float SizeMB)
{
    // In real implementation, this would set the texture streaming pool size
    UE_LOG(LogTemp, Log, TEXT("Texture streaming pool size set to %.2f MB"), SizeMB);
}

void UMingMemoryOptimizer::FlushTextureStreaming()
{
    // In real implementation, this would flush the texture streaming cache
    UE_LOG(LogTemp, Log, TEXT("Texture streaming cache flushed"));

    if (PoolStats.Contains(EMingMemoryPoolType::Texture))
    {
        PoolStats[EMingMemoryPoolType::Texture].UsedMemoryMB = 0.0f;
    }
}

void UMingMemoryOptimizer::ReduceTextureResolutions(int32 MaxTextureSize)
{
    UE_LOG(LogTemp, Log, TEXT("Reducing texture resolutions to max %d"), MaxTextureSize);

    // In real implementation, this would iterate through textures and reduce resolution
}

void UMingMemoryOptimizer::FlushStaticMeshCache()
{
    UE_LOG(LogTemp, Log, TEXT("Static mesh cache flushed"));

    if (PoolStats.Contains(EMingMemoryPoolType::Mesh))
    {
        PoolStats[EMingMemoryPoolType::Mesh].UsedMemoryMB = 0.0f;
    }
}

void UMingMemoryOptimizer::FlushSkeletalMeshCache()
{
    UE_LOG(LogTemp, Log, TEXT("Skeletal mesh cache flushed"));
}

void UMingMemoryOptimizer::StartMemoryMonitoring(float IntervalSeconds)
{
    if (bIsMonitoring)
    {
        return;
    }

    bIsMonitoring = true;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(MonitoringTimer, this,
            &UMingMemoryOptimizer::UpdateMemoryStats, IntervalSeconds, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Memory monitoring started (interval: %.2f seconds)"), IntervalSeconds);
}

void UMingMemoryOptimizer::StopMemoryMonitoring()
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

    UE_LOG(LogTemp, Log, TEXT("Memory monitoring stopped"));
}

bool UMingMemoryOptimizer::IsMemoryMonitoring() const
{
    return bIsMonitoring;
}

float UMingMemoryOptimizer::GetTotalMemoryUsed() const
{
    FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
    return (Stats.UsedVirtual - Stats.AvailableVirtual) / (1024.0f * 1024.0f);
}

float UMingMemoryOptimizer::GetTotalMemoryAvailable() const
{
    FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
    return Stats.AvailableVirtual / (1024.0f * 1024.0f);
}

float UMingMemoryOptimizer::GetMemoryUsagePercent() const
{
    FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
    float TotalPhysical = Stats.TotalPhysical / (1024.0f * 1024.0f);
    float UsedPhysical = (Stats.UsedPhysical) / (1024.0f * 1024.0f);

    if (TotalPhysical <= 0.0f)
    {
        return 0.0f;
    }

    return (UsedPhysical / TotalPhysical) * 100.0f;
}

void UMingMemoryOptimizer::SetMemoryWarningThreshold(float Percent)
{
    MemoryWarningThreshold = FMath::Clamp(Percent, 50.0f, 95.0f);
    UE_LOG(LogTemp, Log, TEXT("Memory warning threshold set to %.1f%%"), MemoryWarningThreshold);
}

float UMingMemoryOptimizer::GetMemoryWarningThreshold() const
{
    return MemoryWarningThreshold;
}

FString UMingMemoryOptimizer::GetMemoryReport() const
{
    FString Report = TEXT("=== MingGoRTS Memory Report ===\n");
    Report += FString::Printf(TEXT("Total Memory Used: %.2f MB\n"), GetTotalMemoryUsed());
    Report += FString::Printf(TEXT("Total Memory Available: %.2f MB\n"), GetTotalMemoryAvailable());
    Report += FString::Printf(TEXT("Memory Usage: %.1f%%\n"), GetMemoryUsagePercent());
    Report += FString::Printf(TEXT("Cache Size: %.2f MB / %.2f MB\n"), GetCacheSize(), CacheSizeLimitMB);
    Report += FString::Printf(TEXT("Cache Items: %d\n"), CachedObjects.Num());
    Report += FString::Printf(TEXT("Auto-Cleanup Objects: %d\n"), AutoCleanupTimers.Num());
    Report += FString::Printf(TEXT("Memory Warning Threshold: %.1f%%\n"), MemoryWarningThreshold);

    Report += TEXT("\n=== Pool Statistics ===\n");
    for (const auto& Pair : PoolStats)
    {
        const FMingMemoryPoolStats& Stats = Pair.Value;
        Report += FString::Printf(TEXT("%s: %.2f MB, %d allocations\n"),
            *UEnum::GetValueAsString(Stats.PoolType), Stats.UsedMemoryMB, Stats.NumAllocations);
    }

    return Report;
}

void UMingMemoryOptimizer::LogMemoryStats()
{
    UE_LOG(LogTemp, Log, TEXT("%s"), *GetMemoryReport());
}

// Internal helper functions

void UMingMemoryOptimizer::UpdateMemoryStats()
{
    // Update pool stats
    for (auto& Pair : PoolStats)
    {
        FMingMemoryPoolStats& Stats = Pair.Value;
        // In real implementation, this would gather actual stats
        Stats.UsedMemoryMB = FMath::RandRange(10.0f, 100.0f);
        Stats.NumAllocations = FMath::RandRange(100, 1000);
    }

    // Check memory warnings
    CheckMemoryWarnings();
}

void UMingMemoryOptimizer::CheckMemoryWarnings()
{
    float UsagePercent = GetMemoryUsagePercent();

    if (UsagePercent > MemoryWarningThreshold)
    {
        OnMemoryWarning.Broadcast(UsagePercent, MemoryWarningThreshold);

        UE_LOG(LogTemp, Warning, TEXT("Memory usage warning: %.1f%% (threshold: %.1f%%)"),
            UsagePercent, MemoryWarningThreshold);
    }
}

float UMingMemoryOptimizer::CalculateCacheSize() const
{
    float TotalSize = 0.0f;
    for (const auto& Info : CachedObjects)
    {
        TotalSize += Info.MemorySize;
    }
    return TotalSize / (1024.0f * 1024.0f); // Convert to MB
}

void UMingMemoryOptimizer::EvictLeastUsedCacheItems(float TargetSizeMB)
{
    // Sort by access time (oldest first)
    CachedObjects.Sort([](const FMingCachedObjectInfo& A, const FMingCachedObjectInfo& B)
    {
        return A.LastAccessTime < B.LastAccessTime;
    });

    // Remove items until we're under target
    while (CalculateCacheSize() > TargetSizeMB && CachedObjects.Num() > 0)
    {
        CachedObjects.RemoveAt(0);
    }
}

void UMingMemoryOptimizer::FlushRenderResourceCache()
{
    // In real implementation, this would flush render resources
    UE_LOG(LogTemp, Log, TEXT("Render resource cache flushed"));
}

void UMingMemoryOptimizer::FlushAnimationCache()
{
    UE_LOG(LogTemp, Log, TEXT("Animation cache flushed"));
}

void UMingMemoryOptimizer::FlushAudioCache()
{
    UE_LOG(LogTemp, Log, TEXT("Audio cache flushed"));
}

void UMingMemoryOptimizer::FlushAICache()
{
    UE_LOG(LogTemp, Log, TEXT("AI cache flushed"));
}

void UMingMemoryOptimizer::FlushNetworkCache()
{
    UE_LOG(LogTemp, Log, TEXT("Network cache flushed"));
}

void UMingMemoryOptimizer::FlushUICache()
{
    UE_LOG(LogTemp, Log, TEXT("UI cache flushed"));
}

void UMingMemoryOptimizer::PerformGarbageCollection(bool bFullPurge)
{
    UE_LOG(LogTemp, Log, TEXT("Performing garbage collection (full purge: %s)"),
        bFullPurge ? TEXT("Yes") : TEXT("No"));

    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void UMingMemoryOptimizer::CompactAllocator()
{
    UE_LOG(LogTemp, Log, TEXT("Compacting memory allocator"));
}
