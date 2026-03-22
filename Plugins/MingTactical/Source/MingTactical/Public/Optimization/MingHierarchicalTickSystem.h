#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHierarchicalTickSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * Tick �h??��?? | Tick Level Type
 */
UENUM(BlueprintType)
enum class ETickLevel: uint8 {
    Critical    UMETA(DisplayName = "Critical"),    // �C?X?�s | Every Frame (??���u摧毀 | Highest Priority)
    High        UMETA(DisplayName = "High"),        // �C?X?�s | Every Frame
    Normal      UMETA(DisplayName = "Normal"),      // 30 FPS
    Low         UMETA(DisplayName = "Low"),         // 15 FPS
    VeryLow     UMETA(DisplayName = "Very Low"),    // 5 FPS
    Background  UMETA(DisplayName = "Background"),   // ???X?�s | On Demand
    Paused      UMETA(DisplayName = "Paused")       // ???X?�s | Paused
};

/**
 * Tick ��摧毀| Tick Group Definition
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

    void AddUnit(AMingTacticalUnit* Unit);
    void RemoveUnit(AMingTacticalUnit* Unit);
    void AddAI(AMingCombatAI* AI);
    void RemoveAI(AMingCombatAI* AI);
    bool ShouldTick(float DeltaTime);
    void ProcessTick(float DeltaTime);
    void CleanupInvalidReferences();
    int32 GetActiveCount() const;
};

/**
 * 摧毀 Tick �t??
 * 
 * ɬ??�j??��??��?? Tick ??��?? * - ??�u目標數量X??XTick ???
 * - 摧毀�C???�s摧毀��摧毀X * - 摧毀???��
 * 
 * ??��??1000+ ???X?�j?��???X?��
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingHierarchicalTickSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHierarchicalTickSystem();

    /**
     * ???X?�t??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Initialize();

    /**
     * ???�t??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Shutdown();

    /**
     * ??Tick ???
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Tick(float DeltaTime);

    // ==== 摧毀��?? ====

    /**
     * �`?X??X?�t??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel = ETickLevel::Normal);

    /**
     * �`?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * ?�m???XTick ??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * 故事選項XTick ??
     */
    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick")
    ETickLevel GetUnitTickLevel(AMingTacticalUnit* Unit) const;

    // ==== AI ��?? ====

    /**
     * �`?? AI ??��X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel = ETickLevel::Normal);

    /**
     * �`?? AI ??��X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterAI(AMingCombatAI* AI);

    /**
     * ?�m AI XTick ??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel);

    // ==== ɬ摧毀X====

    /**
     * ???��?X?�� Critical ??
     * (��??�G�Q??�a??��摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds = 5.0f);

    /**
     * ???��???��??��??
     * (��??�G??��?X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void DemoteToBackground(AMingTacticalUnit* Unit);

    /**
     * ??�u�Z��??��摧毀�æ�目標數量??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera);

    /**
     * ??�u目標數量X摧毀?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat);

    // ==== ??�m ====

    /**
     * ?�m摧毀??XTick 摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetTickInterval(ETickLevel Level, float IntervalSeconds);

    /**
     * ?�m摧毀摧毀摧毀?�j?X???��?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits);

    /**
     * ??��/�T��摧毀???��
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetAutoBalancingEnabled(bool bEnabled);

    // ==== ??�H�� ====

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

    // ==== ??? ====

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void DrawDebugVisualization();

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    FString GetDebugString() const;

private:
    // Tick 摧毀?
    UPROPERTY()
    TMap<ETickLevel, FTickGroup> TickGroups;

    // 目標數量??X???
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, ETickLevel> UnitLevels;

    // AI ???目標數量
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingCombatAI>, ETickLevel> AILevels;

    // 摧毀摧毀?��?X??X??
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, float> PromotedUnits;

    // ??�m
    bool bIsInitialized;
    bool bAutoBalancingEnabled;

    // ??
    float AverageTickTimeMs;
    int32 TickedUnitsLastFrame;
    float CurrentTickTimeMs;
    TArray<float> TickTimeHistory;
    int32 TickHistoryIndex;

    // ???XTick ??
    void InitializeTickGroups();

    // 摧毀摧毀
    void ProcessPromotedUnits(float DeltaTime);

    // 摧毀???��
    void PerformLoadBalancing();

    // ??X??X摧毀?
    ETickLevel CalculateRecommendedLevel(AMingTacticalUnit* Unit) const;

    // ??�s??
    void UpdateTickStats(float TickTimeMs);

    // �M?X???�ޥ�
    void CleanupInvalidReferences();

    // 摧毀 Tick ???�ޥ�
    FTickGroup* GetTickGroup(ETickLevel Level);
    const FTickGroup* GetTickGroup(ETickLevel Level) const;

    // 摧毀摧毀摧毀X
    static float GetDefaultInterval(ETickLevel Level);

    // 摧毀摧毀摧毀摧毀�j??��?
    static int32 GetDefaultMaxUnits(ETickLevel Level);
};

