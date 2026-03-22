#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalOptimizationManager.generated.h"

class UMingSpatialPartition;
class UMingHierarchicalTickSystem;
class UMingObjectPoolSystem;

/**
 * ?�能統�? | Performance Statistics
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FPerformanceStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float CurrentFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly)
    float AverageFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly)
    float MinFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly)
    float FrameTimeMs = 16.67f;

    UPROPERTY(BlueprintReadOnly)
    float GameThreadTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float GPUTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveUnitCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalUnitCount = 0;

    UPROPERTY(BlueprintReadOnly)
    float MemoryUsageMB = 0.0f;

    void Update(float DeltaTime);
};

/**
 * ?��X��?管�X| Tactical Optimization Manager
 * 
 * ?��X�管?��XEpic 2 ?�優?�系統�? | Integrate and manage all Epic 2 optimization systems:
 * - 空�X��?系統 | Spatial Partitioning
 * - ?�層 Tick 系統 | Hierarchical Tick
 * - 對象池系�?| Object Pool
 * - ?�能X�� | Performance Monitoring
 * 
 * ?��?統�X�優?��?置�X��?�口 | Provide unified optimization configuration and monitoring interface
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingTacticalOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalOptimizationManager();

    /**
     * ?��X�優?�管?�器 | Initialize Optimization Manager
     * @param WorldBounds 世�X��? (?�於空�X��?) | World bounds (for spatial partitioning)
     * @param ExpectedUnitCount ?��X��X��? | Expected unit count
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Initialize(const FBox& WorldBounds, int32 ExpectedUnitCount = 1000);

    /**
     * ?�闭优�?管�X     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Shutdown();

    /**
     * �?Tick ?�数
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Tick(float DeltaTime);

    // ==== 子系统访X====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingSpatialPartition* GetSpatialPartition() const { return SpatialPartition; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingHierarchicalTickSystem* GetTickSystem() const { return TickSystem; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingObjectPoolSystem* GetObjectPool() const { return ObjectPool; }

    // ==== ?��?管�? ====

    /**
     * 注�X��X��X��X�系�?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void RegisterUnit(class AMingTacticalUnit* Unit);

    /**
     * 从�X��X�系统注?�?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UnregisterUnit(class AMingTacticalUnit* Unit);

    /**
     * ?��?注�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchRegisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * ?��?注�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchUnregisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * ?�新?��?位置（�?步到空间?�区�?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UpdateUnitPosition(class AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * 设置?��XTick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void SetUnitTickLevel(class AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * ?��X��XCritical 层级（临?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void PromoteUnitToCritical(class AMingTacticalUnit* Unit, float DurationSeconds = 5.0f);

    // ==== AI管�? ====

    /**
     * 注�?AI?��X�系�?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void RegisterAI(class AMingCombatAI* AI);

    /**
     * 从�X�系统注?�AI
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void UnregisterAI(class AMingCombatAI* AI);

    // ==== ?�能?�控 ====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    FPerformanceStats GetPerformanceStats() const { return CurrentStats; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    float GetCurrentFPS() const { return CurrentStats.CurrentFPS; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    int32 GetActiveUnitCount() const { return CurrentStats.ActiveUnitCount; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    bool IsPerformanceCritical() const;

    // ==== ?�动优�? ====

    /**
     * ?�用/禁用?�动优�?调整
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void SetAutoOptimizationEnabled(bool bEnabled);

    /**
     * ?�据?�能?�动调整优�?级别
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void AutoAdjustOptimizationLevel();

    // ==== ?�置 ====

    /**
     * 设置?��? FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetTargetFPS(float TargetFPS);

    /**
     * 设置?�能临�X�值�?低�?此值触?�自?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetPerformanceCriticalThreshold(float FPS);

    /**
     * ?�用/禁用空间?�区
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetSpatialPartitionEnabled(bool bEnabled);

    /**
     * ?�用/禁用?��? Tick
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetHierarchicalTickEnabled(bool bEnabled);

    /**
     * ?�用/禁用对象�?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetObjectPoolEnabled(bool bEnabled);

    // ==== 调�? ====

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GetDebugString() const;

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void DrawDebugVisualization(bool bDrawSpatialGrid = true, bool bDrawTickLevels = false);

    /**
     * ?��?优�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GenerateOptimizationReport() const;

private:
    // 子系�?    UPROPERTY()
    UMingSpatialPartition* SpatialPartition;

    UPROPERTY()
    UMingHierarchicalTickSystem* TickSystem;

    UPROPERTY()
    UMingObjectPoolSystem* ObjectPool;

    // ?�能统计
    FPerformanceStats CurrentStats;
    TArray<float> FPSHistory;
    int32 FPSHistoryIndex;

    // ?�置
    bool bIsInitialized;
    bool bAutoOptimizationEnabled;
    bool bSpatialPartitionEnabled;
    bool bHierarchicalTickEnabled;
    bool bObjectPoolEnabled;
    float TargetFPS;
    float CriticalFPSThreshold;

    // ?�适�?优�?
    int32 OptimizationLevel; // 0-4，�?高�X��?激�?    float LastOptimizationAdjustTime;
    float OptimizationAdjustCooldown;

    // ?��X��?系�?
    void InitializeSubsystems(const FBox& WorldBounds, int32 ExpectedUnitCount);
    void ShutdownSubsystems();

    // ?�新?�能统计
    void UpdatePerformanceStats(float DeltaTime);

    // ?�适�?优�?调整
    void PerformAutoOptimization(float DeltaTime);

    // 调整优�?级别
    void SetOptimizationLevel(int32 NewLevel);

    // ?�据?��X��?调整池大�?    void AdjustPoolSizeForUnitCount(int32 UnitCount);
};

