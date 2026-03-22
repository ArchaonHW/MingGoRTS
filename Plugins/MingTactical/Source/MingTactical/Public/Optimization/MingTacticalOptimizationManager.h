#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalOptimizationManager.generated.h"

class UMingSpatialPartition;
class UMingHierarchicalTickSystem;
class UMingObjectPoolSystem;

/**
 * ?ßËÉΩÁµ±Ë? | Performance Statistics
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
 * ?∞Ë??™Â?ÁÆ°Á???| Tactical Optimization Manager
 * 
 * ?¥Â??åÁÆ°?ÜÊ???Epic 2 ?ÑÂÑ™?ñÁ≥ªÁµ±Ô? | Integrate and manage all Epic 2 optimization systems:
 * - Á©∫È??ÜÂ?Á≥ªÁµ± | Spatial Partitioning
 * - ?ÜÂ±§ Tick Á≥ªÁµ± | Hierarchical Tick
 * - Â∞çË±°Ê±†Á≥ªÁµ?| Object Pool
 * - ?ßËÉΩ??éß | Performance Monitoring
 * 
 * ?ê‰?Áµ±‰??ÑÂÑ™?ñÈ?ÁΩÆÂ???éß?•Âè£ | Provide unified optimization configuration and monitoring interface
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingTacticalOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalOptimizationManager();

    /**
     * ?ùÂ??ñÂÑ™?ñÁÆ°?ÜÂô® | Initialize Optimization Manager
     * @param WorldBounds ‰∏ñÁ??äÁ? (?®ÊñºÁ©∫È??ÜÂ?) | World bounds (for spatial partitioning)
     * @param ExpectedUnitCount ?êÊ??Æ‰??∏È? | Expected unit count
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Initialize(const FBox& WorldBounds, int32 ExpectedUnitCount = 1000);

    /**
     * ?≥Èó≠‰ºòÂ?ÁÆ°Á???     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Shutdown();

    /**
     * ‰∏?Tick ?ΩÊï∞
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization")
    void Tick(float DeltaTime);

    // ==== Â≠êÁ≥ªÁªüËÆø??====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingSpatialPartition* GetSpatialPartition() const { return SpatialPartition; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingHierarchicalTickSystem* GetTickSystem() const { return TickSystem; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Systems")
    UMingObjectPoolSystem* GetObjectPool() const { return ObjectPool; }

    // ==== ?ï‰?ÁÆ°Á? ====

    /**
     * Ê≥®Â??ï‰??∞Ê??â‰??ñÁ≥ªÁª?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void RegisterUnit(class AMingTacticalUnit* Unit);

    /**
     * ‰ªéÊ??â‰??ñÁ≥ªÁªüÊ≥®?Ä?ï‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UnregisterUnit(class AMingTacticalUnit* Unit);

    /**
     * ?πÈ?Ê≥®Â??ï‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchRegisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * ?πÈ?Ê≥®È??ï‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void BatchUnregisterUnits(const TArray<class AMingTacticalUnit*>& Units);

    /**
     * ?¥Êñ∞?ï‰?‰ΩçÁΩÆÔºàÂ?Ê≠•Âà∞Á©∫Èó¥?ÜÂå∫Ôº?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void UpdateUnitPosition(class AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * ËÆæÁΩÆ?ï‰???Tick Â±ÇÁ∫ß
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void SetUnitTickLevel(class AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * ?êÂ??ï‰???Critical Â±ÇÁ∫ßÔºà‰∏¥?∂Ô?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Units")
    void PromoteUnitToCritical(class AMingTacticalUnit* Unit, float DurationSeconds = 5.0f);

    // ==== AIÁÆ°Á? ====

    /**
     * Ê≥®Â?AI?∞‰??ñÁ≥ªÁª?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void RegisterAI(class AMingCombatAI* AI);

    /**
     * ‰ªé‰??ñÁ≥ªÁªüÊ≥®?ÄAI
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|AI")
    void UnregisterAI(class AMingCombatAI* AI);

    // ==== ?ßËÉΩ?ëÊéß ====

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    FPerformanceStats GetPerformanceStats() const { return CurrentStats; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    float GetCurrentFPS() const { return CurrentStats.CurrentFPS; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    int32 GetActiveUnitCount() const { return CurrentStats.ActiveUnitCount; }

    UFUNCTION(BlueprintPure, Category = "Tactical Optimization|Performance")
    bool IsPerformanceCritical() const;

    // ==== ?™Âä®‰ºòÂ? ====

    /**
     * ?ØÁî®/Á¶ÅÁî®?™Âä®‰ºòÂ?Ë∞ÉÊï¥
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void SetAutoOptimizationEnabled(bool bEnabled);

    /**
     * ?πÊçÆ?ßËÉΩ?™Âä®Ë∞ÉÊï¥‰ºòÂ?Á∫ßÂà´
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Auto")
    void AutoAdjustOptimizationLevel();

    // ==== ?çÁΩÆ ====

    /**
     * ËÆæÁΩÆ?ÆÊ? FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetTargetFPS(float TargetFPS);

    /**
     * ËÆæÁΩÆ?ßËÉΩ‰∏¥Á??àÂÄºÔ?‰Ωé‰?Ê≠§ÂÄºËß¶?ëËá™?®‰??ñÔ?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetPerformanceCriticalThreshold(float FPS);

    /**
     * ?ØÁî®/Á¶ÅÁî®Á©∫Èó¥?ÜÂå∫
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetSpatialPartitionEnabled(bool bEnabled);

    /**
     * ?ØÁî®/Á¶ÅÁî®?ÜÂ? Tick
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetHierarchicalTickEnabled(bool bEnabled);

    /**
     * ?ØÁî®/Á¶ÅÁî®ÂØπË±°Ê±?     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Config")
    void SetObjectPoolEnabled(bool bEnabled);

    // ==== Ë∞ÉË? ====

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GetDebugString() const;

    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    void DrawDebugVisualization(bool bDrawSpatialGrid = true, bool bDrawTickLevels = false);

    /**
     * ?üÊ?‰ºòÂ??•Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Tactical Optimization|Debug")
    FString GenerateOptimizationReport() const;

private:
    // Â≠êÁ≥ªÁª?    UPROPERTY()
    UMingSpatialPartition* SpatialPartition;

    UPROPERTY()
    UMingHierarchicalTickSystem* TickSystem;

    UPROPERTY()
    UMingObjectPoolSystem* ObjectPool;

    // ?ßËÉΩÁªüËÆ°
    FPerformanceStats CurrentStats;
    TArray<float> FPSHistory;
    int32 FPSHistoryIndex;

    // ?çÁΩÆ
    bool bIsInitialized;
    bool bAutoOptimizationEnabled;
    bool bSpatialPartitionEnabled;
    bool bHierarchicalTickEnabled;
    bool bObjectPoolEnabled;
    float TargetFPS;
    float CriticalFPSThreshold;

    // ?™ÈÄÇÂ?‰ºòÂ?
    int32 OptimizationLevel; // 0-4ÔºåË?È´ò‰??ñË?ÊøÄËø?    float LastOptimizationAdjustTime;
    float OptimizationAdjustCooldown;

    // ?ùÂ??ñÂ?Á≥ªÁ?
    void InitializeSubsystems(const FBox& WorldBounds, int32 ExpectedUnitCount);
    void ShutdownSubsystems();

    // ?¥Êñ∞?ßËÉΩÁªüËÆ°
    void UpdatePerformanceStats(float DeltaTime);

    // ?™ÈÄÇÂ?‰ºòÂ?Ë∞ÉÊï¥
    void PerformAutoOptimization(float DeltaTime);

    // Ë∞ÉÊï¥‰ºòÂ?Á∫ßÂà´
    void SetOptimizationLevel(int32 NewLevel);

    // ?πÊçÆ?ï‰??∞È?Ë∞ÉÊï¥Ê±†Â§ßÂ∞?    void AdjustPoolSizeForUnitCount(int32 UnitCount);
};
