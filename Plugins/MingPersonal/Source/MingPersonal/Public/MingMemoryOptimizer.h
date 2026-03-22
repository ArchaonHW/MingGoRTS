#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMemoryOptimizer.generated.h"

// Forward declarations
class UMingPerformanceManager;

// Memory pool type
UENUM(BlueprintType)
enum class EMingMemoryPoolType : uint8
{
    Texture                UMETA(DisplayName = "Texture"),
    Mesh                   UMETA(DisplayName = "Mesh"),
    Audio                  UMETA(DisplayName = "Audio"),
    Particle               UMETA(DisplayName = "Particle"),
    Animation              UMETA(DisplayName = "Animation"),
    UI                     UMETA(DisplayName = "UI"),
    Physics                UMETA(DisplayName = "Physics"),
    AI                     UMETA(DisplayName = "AI"),
    Network                UMETA(DisplayName = "Network"),
    General                UMETA(DisplayName = "General")
};

// Memory optimization strategy
UENUM(BlueprintType)
enum class EMingMemoryStrategy : uint8
{
    Conservative           UMETA(DisplayName = "Conservative"),
    Balanced               UMETA(DisplayName = "Balanced"),
    Aggressive             UMETA(DisplayName = "Aggressive"),
    Emergency              UMETA(DisplayName = "Emergency")
};

// Memory pool stats
USTRUCT(BlueprintType)
struct FMingMemoryPoolStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    EMingMemoryPoolType PoolType;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    float UsedMemoryMB;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    float ReservedMemoryMB;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int32 NumAllocations;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    float FragmentationPercent;

    FMingMemoryPoolStats()
        : PoolType(EMingMemoryPoolType::General)
        , UsedMemoryMB(0.0f)
        , ReservedMemoryMB(0.0f)
        , NumAllocations(0)
        , FragmentationPercent(0.0f)
    {}
};

// Memory optimization result
USTRUCT(BlueprintType)
struct FMingMemoryOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    EMingMemoryStrategy Strategy;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    float MemoryFreedMB;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int32 NumObjectsFreed;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    float ProcessingTimeMs;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    bool bSuccessful;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    FString Details;

    FMingMemoryOptimizationResult()
        : Strategy(EMingMemoryStrategy::Balanced)
        , MemoryFreedMB(0.0f)
        , NumObjectsFreed(0)
        , ProcessingTimeMs(0.0f)
        , bSuccessful(false)
        , Details(TEXT(""))
    {}
};

// Cached object info
USTRUCT()
struct FMingCachedObjectInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString ObjectPath;

    UPROPERTY()
    float LastAccessTime;

    UPROPERTY()
    int32 AccessCount;

    UPROPERTY()
    float MemorySize;

    UPROPERTY()
    bool bCanUnload;

    FMingCachedObjectInfo()
        : ObjectPath(TEXT(""))
        , LastAccessTime(0.0f)
        , AccessCount(0)
        , MemorySize(0.0f)
        , bCanUnload(true)
    {}
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemoryOptimizationCompleted, FMingMemoryOptimizationResult, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMemoryWarning, float, UsedPercent, float, Threshold};

/**
 * ?��X��X * 專�?負責?��?管�X�優X */
UCLASS(ClassGroup = (MingGoRTS), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingMemoryOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingMemoryOptimizer(};

    // Initialize/Shutdown
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void Initialize(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void Shutdown(};

    // Memory optimization
    UFUNCTION(BlueprintCallable, Category = "Memory")
    FMingMemoryOptimizationResult OptimizeMemory(EMingMemoryStrategy Strategy};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    FMingMemoryOptimizationResult QuickOptimize(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    FMingMemoryOptimizationResult DeepOptimize(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    FMingMemoryOptimizationResult EmergencyCleanup(};

    // Pool management
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void FlushPool(EMingMemoryPoolType PoolType};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void FlushAllPools(};

    UFUNCTION(BlueprintPure, Category = "Memory")
    FMingMemoryPoolStats GetPoolStats(EMingMemoryPoolType PoolType) const;

    UFUNCTION(BlueprintCallable, Category = "Memory")
    TArray<FMingMemoryPoolStats> GetAllPoolStats() const;

    // Cache management
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void ClearCache(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void SetCacheSizeLimit(float MaxSizeMB};

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetCacheSize() const;

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void UnloadUnusedAssets(float UnusedTimeThreshold = 60.0f};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void CompactMemory(};

    // Object lifecycle
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void RegisterObjectForAutoCleanup(UObject* Object, float LifetimeSeconds};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void CancelAutoCleanup(UObject* Object};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void ForceDestroyObject(UObject* Object};

    // Texture management
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void SetTextureStreamingPoolSize(float SizeMB};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void FlushTextureStreaming(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void ReduceTextureResolutions(int32 MaxTextureSize};

    // Mesh management
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void FlushStaticMeshCache(};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void FlushSkeletalMeshCache(};

    // Monitoring
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void StartMemoryMonitoring(float IntervalSeconds = 5.0f};

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void StopMemoryMonitoring(};

    UFUNCTION(BlueprintPure, Category = "Memory")
    bool IsMemoryMonitoring() const;

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetTotalMemoryUsed() const;

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetTotalMemoryAvailable() const;

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetMemoryUsagePercent() const;

    // Warnings
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void SetMemoryWarningThreshold(float Percent};

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetMemoryWarningThreshold() const;

    // Reports
    UFUNCTION(BlueprintPure, Category = "Memory")
    FString GetMemoryReport() const;

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void LogMemoryStats(};

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryOptimizationCompleted OnOptimizationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryWarning OnMemoryWarning;

private:
    UPROPERTY()
    TMap<EMingMemoryPoolType, FMingMemoryPoolStats> PoolStats;

    UPROPERTY()
    TArray<FMingCachedObjectInfo> CachedObjects;

    UPROPERTY()
    float CacheSizeLimitMB;

    UPROPERTY()
    float MemoryWarningThreshold;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    FTimerHandle MonitoringTimer;

    UPROPERTY()
    TMap<UObject*, FTimerHandle> AutoCleanupTimers;

    // Internal functions
    void UpdateMemoryStats(};
    void CheckMemoryWarnings(};
    float CalculateCacheSize() const;
    void EvictLeastUsedCacheItems(float TargetSizeMB};
    void FlushRenderResourceCache(};
    void FlushAnimationCache(};
    void FlushAudioCache(};
    void FlushAICache(};
    void FlushNetworkCache(};
    void FlushUICache(};
    void PerformGarbageCollection(bool bFullPurge};
    void CompactAllocator(};
};

