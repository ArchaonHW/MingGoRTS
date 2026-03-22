#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalOptimizationManager.generated.h"

class UMingSpatialPartition;
class UMingHierarchicalTickSystem;
class UMingObjectPoolSystem;

/**
 * 性能統計 | Performance Statistics
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
 * 戰術優化管理器 | Tactical Optimization Manager
 * 
 * 整合和管理所有 Epic 2 的優化系統： | Integrate and manage all Epic 2 optimization systems:
 * - 空間分區系統 | Spatial Partitioning
 * - 分層 Tick 系統 | Hierarchical Tick
 * - 對象池系統 | Object Pool
 * - 性能監控 | Performance Monitoring
 * 
 * 提供統一的優化配置和監控接口 | Provide unified optimization configuration and monitoring interface
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingTacticalOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalOptimizationManager();

    /**
     * 初始化優化管理器 | Initialize Optimization Manager
     * @param WorldBounds 世界邊界 (用於空間分區) | World bounds (for spatial partitioning)
     * @param ExpectedUnitCount 預期單位數量 | Expected unit count
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Initialize(const FBox& WorldBounds, int32 ExpectedUnitCount = 1000);

    /**
     * 关闭优化管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Shutdown();

    /**
     * 主 Tick 函数
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Tick(float DeltaTime);

    // ==== 子系统访问 ====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingSpatialPartition* GetSpatialPartition() const { return SpatialPartition; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingHierarchicalTickSystem* GetTickSystem() const { return TickSystem; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingObjectPoolSystem* GetObjectPool() const { return ObjectPool; }

    // ==== 单位管理 ====

    /**
     * 注册单位到所有优化系统
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void RegisterUnit(class AMingTacticalUnit* Unit);

    /**
     * 从所有优化系统注销单位
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UnregisterUnit(class AMingTacticalUnit* Unit);

    /**
     * 批量注册单位
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchRegisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * 批量注销单位
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchUnregisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * 更新单位位置（同步到空间分区）
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UpdateUnitPosition(class AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * 设置单位的 Tick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void SetUnitTickLevel(class AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * 提升单位到 Critical 层级（临时）
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void PromoteUnitToCritical(class AMingTacticalUnit* Unit, float DurationSeconds = 5.0f);

    // ==== AI管理 ====

    /**
     * 注册AI到优化系统
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void RegisterAI(class AMingCombatAI* AI);

    /**
     * 从优化系统注销AI
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void UnregisterAI(class AMingCombatAI* AI);

    // ==== 性能监控 ====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    FPerformanceStats GetPerformanceStats() const { return CurrentStats; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    float GetCurrentFPS() const { return CurrentStats.CurrentFPS; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    int32 GetActiveUnitCount() const { return CurrentStats.ActiveUnitCount; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    bool IsPerformanceCritical() const;

    // ==== 自动优化 ====

    /**
     * 启用/禁用自动优化调整
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void SetAutoOptimizationEnabled(bool bEnabled);

    /**
     * 根据性能自动调整优化级别
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void AutoAdjustOptimizationLevel();

    // ==== 配置 ====

    /**
     * 设置目标 FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetTargetFPS(float TargetFPS);

    /**
     * 设置性能临界阈值（低于此值触发自动优化）
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetPerformanceCriticalThreshold(float FPS);

    /**
     * 启用/禁用空间分区
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetSpatialPartitionEnabled(bool bEnabled);

    /**
     * 启用/禁用分层 Tick
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetHierarchicalTickEnabled(bool bEnabled);

    /**
     * 启用/禁用对象池
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetObjectPoolEnabled(bool bEnabled);

    // ==== 调试 ====

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GetDebugString() const;

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void DrawDebugVisualization(bool bDrawSpatialGrid = true, bool bDrawTickLevels = false);

    /**
     * 生成优化报告
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GenerateOptimizationReport() const;

private:
    // 子系统
    UPROPERTY()
    UMingSpatialPartition* SpatialPartition;

    UPROPERTY()
    UMingHierarchicalTickSystem* TickSystem;

    UPROPERTY()
    UMingObjectPoolSystem* ObjectPool;

    // 性能统计
    FPerformanceStats CurrentStats;
    TArray<float> FPSHistory;
    int32 FPSHistoryIndex;

    // 配置
    bool bIsInitialized;
    bool bAutoOptimizationEnabled;
    bool bSpatialPartitionEnabled;
    bool bHierarchicalTickEnabled;
    bool bObjectPoolEnabled;
    float TargetFPS;
    float CriticalFPSThreshold;

    // 自适应优化
    int32 OptimizationLevel; // 0-4，越高优化越激进
    float LastOptimizationAdjustTime;
    float OptimizationAdjustCooldown;

    // 初始化子系统
    void InitializeSubsystems(const FBox& WorldBounds, int32 ExpectedUnitCount);
    void ShutdownSubsystems();

    // 更新性能统计
    void UpdatePerformanceStats(float DeltaTime);

    // 自适应优化调整
    void PerformAutoOptimization(float DeltaTime);

    // 调整优化级别
    void SetOptimizationLevel(int32 NewLevel);

    // 根据单位数量调整池大小
    void AdjustPoolSizeForUnitCount(int32 UnitCount);
};
