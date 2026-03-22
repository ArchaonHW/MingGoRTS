#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHierarchicalTickSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * Tick 層�?類�? | Tick Level Type
 */
UENUM(BlueprintType)
enum class ETickLevel : uint8
{
    Critical    UMETA(DisplayName = "Critical"),    // 每�X�新 | Every Frame (?�高優?��? | Highest Priority)
    High        UMETA(DisplayName = "High"),        // 每�X�新 | Every Frame
    Normal      UMETA(DisplayName = "Normal"),      // 30 FPS
    Low         UMETA(DisplayName = "Low"),         // 15 FPS
    VeryLow     UMETA(DisplayName = "Very Low"),    // 5 FPS
    Background  UMETA(DisplayName = "Background"),   // ?��X�新 | On Demand
    Paused      UMETA(DisplayName = "Paused")       // ?��X�新 | Paused
};

/**
 * Tick 組�?�?| Tick Group Definition
 */
USTRUCT()
struct MINGTACTICAL_API FTickGroup
{
    GENERATED_BODY()

    UPROPERTY()
    ETickLevel TickLevel;

    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> Units;

    UPROPERTY()
    TArray<TWeakObjectPtr<AMingCombatAI>> AIControllers;

    float TickInterval;
    float TimeSinceLastTick;
    int32 MaxUnitsPerFrame;
    int32 CurrentIndex;

    FTickGroup()
        : TickLevel(ETickLevel::Normal)
        , TickInterval(1.0f / 30.0f)
        , TimeSinceLastTick(0.0f)
        , MaxUnitsPerFrame(100)
        , CurrentIndex(0)
    {}

    void AddUnit(AMingTacticalUnit* Unit};
    void RemoveUnit(AMingTacticalUnit* Unit};
    void AddAI(AMingCombatAI* AI};
    void RemoveAI(AMingCombatAI* AI};
    bool ShouldTick(float DeltaTime};
    void ProcessTick(float DeltaTime};
    void CleanupInvalidReferences(};
    int32 GetActiveCount() const;
};

/**
 * ?��? Tick 系�?
 * 
 * 优�?大�?模�?位�? Tick ?�能�? * - ?�据?��X�态�X��XTick 频�?
 * - ?��?每帧?�新?��?位数?��X * - ?�动负载?�衡
 * 
 * ?�用�?1000+ ?��X�大规模?��X�景
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingHierarchicalTickSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHierarchicalTickSystem(};

    /**
     * ?��X�系�?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Initialize(};

    /**
     * ?�闭系�?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Shutdown(};

    /**
     * �?Tick ?�数
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Tick(float DeltaTime};

    // ==== ?��?管�? ====

    /**
     * 注�X��X�系�?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel = ETickLevel::Normal};

    /**
     * 注�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterUnit(AMingTacticalUnit* Unit};

    /**
     * 设置?��XTick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel};

    /**
     * ?��X��X��XTick 层级
     */
    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick")
    ETickLevel GetUnitTickLevel(AMingTacticalUnit* Unit) const;

    // ==== AI 管�? ====

    /**
     * 注�? AI ?�制X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel = ETickLevel::Normal};

    /**
     * 注�? AI ?�制X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterAI(AMingCombatAI* AI};

    /**
     * 设置 AI XTick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel};

    // ==== 优�?级�X====

    /**
     * 将�?位�X�到 Critical 层级
     * (例�?：被?�家?�中?��?�?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds = 5.0f};

    /**
     * 将�?位�?级到?�景层级
     * (例�?：�?幕�X��?�?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void DemoteToBackground(AMingTacticalUnit* Unit};

    /**
     * ?�据距离?�机?��?离自?��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera};

    /**
     * ?�据?��X�态�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat};

    // ==== ?�置 ====

    /**
     * 设置?��?层级XTick ?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetTickInterval(ETickLevel Level, float IntervalSeconds};

    /**
     * 设置?��?层级?��?帧�?大�X��?位数
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits};

    /**
     * ?�用/禁用?�动负载?�衡
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetAutoBalancingEnabled(bool bEnabled};

    // ==== 统计信息 ====

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetTotalRegisteredUnits() const;

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetUnitCountByLevel(ETickLevel Level) const;

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    float GetAverageTickTimeMs() const { return AverageTickTimeMs; }

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetTickedUnitsLastFrame() const { return TickedUnitsLastFrame; }

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    float GetCurrentFPS() const;

    // ==== 调�? ====

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void PrintDebugInfo(};

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void DrawDebugVisualization(};

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    FString GetDebugString() const;

private:
    // Tick 组�?�?    UPROPERTY()
    TMap<ETickLevel, FTickGroup> TickGroups;

    // ?��X��?级�X��?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, ETickLevel> UnitLevels;

    // AI ?��?级�X��?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingCombatAI>, ETickLevel> AILevels;

    // 临时?�级?��?位�X��X�间
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, float> PromotedUnits;

    // ?�置
    bool bIsInitialized;
    bool bAutoBalancingEnabled;

    // 统计
    float AverageTickTimeMs;
    int32 TickedUnitsLastFrame;
    float CurrentTickTimeMs;
    TArray<float> TickTimeHistory;
    int32 TickHistoryIndex;

    // ?��XTick �?    void InitializeTickGroups(};

    // 处�?临时?�级
    void ProcessPromotedUnits(float DeltaTime};

    // ?�动负载?�衡
    void PerformLoadBalancing(};

    // 计�X��X��?�?    ETickLevel CalculateRecommendedLevel(AMingTacticalUnit* Unit) const;

    // ?�新统计
    void UpdateTickStats(float TickTimeMs};

    // 清�X��?引用
    void CleanupInvalidReferences(};

    // ?��? Tick 组�?引用
    FTickGroup* GetTickGroup(ETickLevel Level};
    const FTickGroup* GetTickGroup(ETickLevel Level) const;

    // ?��?层级?��?认间X    static float GetDefaultInterval(ETickLevel Level};

    // ?��?层级?��?认�?帧�?大�?位数
    static int32 GetDefaultMaxUnits(ETickLevel Level};
};

