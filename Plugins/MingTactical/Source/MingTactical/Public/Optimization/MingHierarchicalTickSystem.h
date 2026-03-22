#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHierarchicalTickSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * Tick 層??類?? | Tick Level Type
 */
UENUM(BlueprintType)
enum class ETickLevel : uint8
{
    Critical    UMETA(DisplayName = "Critical"),    // 每?X?新 | Every Frame (??高優???? | Highest Priority)
    High        UMETA(DisplayName = "High"),        // 每?X?新 | Every Frame
    Normal      UMETA(DisplayName = "Normal"),      // 30 FPS
    Low         UMETA(DisplayName = "Low"),         // 15 FPS
    VeryLow     UMETA(DisplayName = "Very Low"),    // 5 FPS
    Background  UMETA(DisplayName = "Background"),   // ???X?新 | On Demand
    Paused      UMETA(DisplayName = "Paused")       // ???X?新 | Paused
};

/**
 * Tick 組????| Tick Group Definition
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
 * ???? Tick 系??
 * 
 * 优??大??模??位?? Tick ??能?? * - ??据???X???X??XTick ???
 * - ????每???新????位????X * - ???????衡
 * 
 * ??用??1000+ ???X?大?模???X?景
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingHierarchicalTickSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHierarchicalTickSystem();

    /**
     * ???X?系??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Initialize();

    /**
     * ???系??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Shutdown();

    /**
     * ??Tick ???
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Tick(float DeltaTime);

    // ==== ????管?? ====

    /**
     * 注?X??X?系??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel = ETickLevel::Normal);

    /**
     * 注?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * ?置???XTick ??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * ???X??X??XTick ??
     */
    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick")
    ETickLevel GetUnitTickLevel(AMingTacticalUnit* Unit) const;

    // ==== AI 管?? ====

    /**
     * 注?? AI ??制X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel = ETickLevel::Normal};

    /**
     * 注?? AI ??制X     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterAI(AMingCombatAI* AI};

    /**
     * ?置 AI XTick ??
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel};

    // ==== 优????X====

    /**
     * ???位?X?到 Critical ??
     * (例??：被??家??中??????
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds = 5.0f};

    /**
     * ???位???到??景??
     * (例??：??幕?X?????
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void DemoteToBackground(AMingTacticalUnit* Unit};

    /**
     * ??据距离??机????离自???X?????     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera};

    /**
     * ??据???X???X?????     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat};

    // ==== ??置 ====

    /**
     * ?置??????XTick ????
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetTickInterval(ETickLevel Level, float IntervalSeconds};

    /**
     * ?置?????????????大?X???位?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits};

    /**
     * ??用/禁用???????衡
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetAutoBalancingEnabled(bool bEnabled};

    // ==== ??信息 ====

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
    // Tick ?????    UPROPERTY()
    TMap<ETickLevel, FTickGroup> TickGroups;

    // ???X?????X???
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, ETickLevel> UnitLevels;

    // AI ??????X???
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingCombatAI>, ETickLevel> AILevels;

    // ?????????位?X??X??
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, float> PromotedUnits;

    // ??置
    bool bIsInitialized;
    bool bAutoBalancingEnabled;

    // ??
    float AverageTickTimeMs;
    int32 TickedUnitsLastFrame;
    float CurrentTickTimeMs;
    TArray<float> TickTimeHistory;
    int32 TickHistoryIndex;

    // ???XTick ??    void InitializeTickGroups();

    // ????????
    void ProcessPromotedUnits(float DeltaTime);

    // ???????衡
    void PerformLoadBalancing();

    // ??X??X?????    ETickLevel CalculateRecommendedLevel(AMingTacticalUnit* Unit) const;

    // ??新??
    void UpdateTickStats(float TickTimeMs);

    // 清?X???引用
    void CleanupInvalidReferences();

    // ???? Tick ???引用
    FTickGroup* GetTickGroup(ETickLevel Level);
    const FTickGroup* GetTickGroup(ETickLevel Level) const;

    // ????????????X    static float GetDefaultInterval(ETickLevel Level);

    // ????????????????大??位?
    static int32 GetDefaultMaxUnits(ETickLevel Level);
};

