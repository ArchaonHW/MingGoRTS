#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHierarchicalTickSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * Tick Â±§Á?È°ûÂ? | Tick Level Type
 */
UENUM(BlueprintType)
enum class ETickLevel : uint8
{
    Critical    UMETA(DisplayName = "Critical"),    // ÊØèÂ??¥Êñ∞ | Every Frame (?ÄÈ´òÂÑ™?àÁ? | Highest Priority)
    High        UMETA(DisplayName = "High"),        // ÊØèÂ??¥Êñ∞ | Every Frame
    Normal      UMETA(DisplayName = "Normal"),      // 30 FPS
    Low         UMETA(DisplayName = "Low"),         // 15 FPS
    VeryLow     UMETA(DisplayName = "Very Low"),    // 5 FPS
    Background  UMETA(DisplayName = "Background"),   // ?âÈ??¥Êñ∞ | On Demand
    Paused      UMETA(DisplayName = "Paused")       // ?´Â??¥Êñ∞ | Paused
};

/**
 * Tick ÁµÑÂ?Áæ?| Tick Group Definition
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
 * ?ÜÂ? Tick Á≥ªÁ?
 * 
 * ‰ºòÂ?Â§ßË?Ê®°Â?‰ΩçÁ? Tick ?ßËÉΩÔº? * - ?πÊçÆ?ï‰??∂ÊÄÅÂ??ç‰???Tick È¢ëÁ?
 * - ?ØÊ?ÊØèÂ∏ß?¥Êñ∞?ÑÂ?‰ΩçÊï∞?èÊ??? * - ?™Âä®Ë¥üËΩΩ?áË°°
 * 
 * ?ÇÁî®‰∫?1000+ ?ï‰??ÑÂ§ßËßÑÊ®°?òÊ??∫ÊôØ
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingHierarchicalTickSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHierarchicalTickSystem(};

    /**
     * ?ùÂ??ñÁ≥ªÁª?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Initialize(};

    /**
     * ?≥Èó≠Á≥ªÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Shutdown(};

    /**
     * ‰∏?Tick ?ΩÊï∞
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Tick(float DeltaTime};

    // ==== ?ï‰?ÁÆ°Á? ====

    /**
     * Ê≥®Â??ï‰??∞Á≥ªÁª?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel = ETickLevel::Normal};

    /**
     * Ê≥®È??ï‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterUnit(AMingTacticalUnit* Unit};

    /**
     * ËÆæÁΩÆ?ï‰???Tick Â±ÇÁ∫ß
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel};

    /**
     * ?∑Â??ï‰??ÑÂ???Tick Â±ÇÁ∫ß
     */
    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick")
    ETickLevel GetUnitTickLevel(AMingTacticalUnit* Unit) const;

    // ==== AI ÁÆ°Á? ====

    /**
     * Ê≥®Â? AI ?ßÂà∂??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel = ETickLevel::Normal};

    /**
     * Ê≥®È? AI ?ßÂà∂??     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterAI(AMingCombatAI* AI};

    /**
     * ËÆæÁΩÆ AI ??Tick Â±ÇÁ∫ß
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel};

    // ==== ‰ºòÂ?Á∫ßË???====

    /**
     * Â∞ÜÂ?‰ΩçÊ??áÂà∞ Critical Â±ÇÁ∫ß
     * (‰æãÂ?ÔºöË¢´?©ÂÆ∂?â‰∏≠?ÑÂ?‰Ω?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds = 5.0f};

    /**
     * Â∞ÜÂ?‰ΩçÈ?Á∫ßÂà∞?åÊôØÂ±ÇÁ∫ß
     * (‰æãÂ?ÔºöÂ?ÂπïÂ??ÑÂ?‰Ω?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void DemoteToBackground(AMingTacticalUnit* Unit};

    /**
     * ?πÊçÆË∑ùÁ¶ª?∏Êú∫?ÑË?Á¶ªËá™?®Ë??¥Â?Á∫?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera};

    /**
     * ?πÊçÆ?òÊ??∂ÊÄÅË??¥Â?Á∫?     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat};

    // ==== ?çÁΩÆ ====

    /**
     * ËÆæÁΩÆ?áÂ?Â±ÇÁ∫ß??Tick ?¥È?
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetTickInterval(ETickLevel Level, float IntervalSeconds};

    /**
     * ËÆæÁΩÆ?áÂ?Â±ÇÁ∫ß?ÑÊ?Â∏ßÊ?Â§ßÂ??ÜÂ?‰ΩçÊï∞
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits};

    /**
     * ?ØÁî®/Á¶ÅÁî®?™Âä®Ë¥üËΩΩ?áË°°
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetAutoBalancingEnabled(bool bEnabled};

    // ==== ÁªüËÆ°‰ø°ÊÅØ ====

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

    // ==== Ë∞ÉË? ====

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void PrintDebugInfo(};

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void DrawDebugVisualization(};

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    FString GetDebugString() const;

private:
    // Tick ÁªÑÊ?Â∞?    UPROPERTY()
    TMap<ETickLevel, FTickGroup> TickGroups;

    // ?ï‰??∞Â?Á∫ßÁ??†Â?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, ETickLevel> UnitLevels;

    // AI ?∞Â?Á∫ßÁ??†Â?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingCombatAI>, ETickLevel> AILevels;

    // ‰∏¥Êó∂?áÁ∫ß?ÑÂ?‰ΩçÂ??¢Â??∂Èó¥
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, float> PromotedUnits;

    // ?çÁΩÆ
    bool bIsInitialized;
    bool bAutoBalancingEnabled;

    // ÁªüËÆ°
    float AverageTickTimeMs;
    int32 TickedUnitsLastFrame;
    float CurrentTickTimeMs;
    TArray<float> TickTimeHistory;
    int32 TickHistoryIndex;

    // ?ùÂ???Tick Áª?    void InitializeTickGroups(};

    // Â§ÑÁ?‰∏¥Êó∂?áÁ∫ß
    void ProcessPromotedUnits(float DeltaTime};

    // ?™Âä®Ë¥üËΩΩ?áË°°
    void PerformLoadBalancing(};

    // ËÆ°Á??®Ë??ÑÂ?Á∫?    ETickLevel CalculateRecommendedLevel(AMingTacticalUnit* Unit) const;

    // ?¥Êñ∞ÁªüËÆ°
    void UpdateTickStats(float TickTimeMs};

    // Ê∏ÖÁ??†Ê?ÂºïÁî®
    void CleanupInvalidReferences(};

    // ?∑Â? Tick ÁªÑÁ?ÂºïÁî®
    FTickGroup* GetTickGroup(ETickLevel Level};
    const FTickGroup* GetTickGroup(ETickLevel Level) const;

    // ?∑Â?Â±ÇÁ∫ß?ÑÈ?ËÆ§Èó¥??    static float GetDefaultInterval(ETickLevel Level};

    // ?∑Â?Â±ÇÁ∫ß?ÑÈ?ËÆ§Ê?Â∏ßÊ?Â§ßÂ?‰ΩçÊï∞
    static int32 GetDefaultMaxUnits(ETickLevel Level};
};

