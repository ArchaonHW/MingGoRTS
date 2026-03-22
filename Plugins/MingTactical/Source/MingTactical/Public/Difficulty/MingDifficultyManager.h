#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/FMingDifficultySettings.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyManager.generated.h"

// ?��?声�?
class UMingPlayerPerformanceTracker;

/**
 * ?�度?��?事件
 */
USTRUCT(BlueprintType)
struct FDifficultyChangedEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    EDifficultyLevel OldLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    EDifficultyLevel NewLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    float OldSkillIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    float NewSkillIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    FString Reason;

    FDifficultyChangedEvent()
        : OldLevel(EDifficultyLevel::Normal)
        , NewLevel(EDifficultyLevel::Normal)
        , OldSkillIndex(50.0f)
        , NewSkillIndex(50.0f)
    {}
};

/**
 * ?�度管�X * 管�?游�X�度?�置?�动?��X * 
 * ?�能�? * - ?�度等级管�? (Easy/Normal/Hard/Expert/Custom)
 * - ?�态难度�X * - ?�度事件触�?
 * - 平�?过渡算�?
 */
UCLASS()
class MINGTACTICAL_API UMingDifficultyManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDifficultyManager(};

    virtual void Initialize(};
    virtual void Shutdown(};
    void Tick(float DeltaTime};

    // ==== ?�度等级管�? ====

    /** 设置?�度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    void SetDifficultyLevel(EDifficultyLevel NewLevel};

    /** ?��?当�X�度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    EDifficultyLevel GetCurrentDifficultyLevel() const;

    /** ?��?当�X�度?�示?�称 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FString GetCurrentDifficultyDisplayName() const;

    /** ?��X�?�可?��X�度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    static TArray<EDifficultyLevel> GetAvailableDifficultyLevels(};

    /** ?��?预设?�度?�置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level) const;

    // ==== ?�态难度�X====

    /** ?�用/禁用?�态难度�X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void SetDynamicAdjustmentEnabled(bool bEnabled};

    /** ?�否?�用?�态�X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDynamicAdjustmentEnabled() const;

    /** ?�动触�X�度评估?��X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void ForceDifficultyEvaluation(};

    /** ?��X�度（防止自?��X��? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void LockDifficulty(bool bLocked};

    /** ?�度?�否被�?�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDifficultyLocked() const;

    // ==== ?�数调整?�口 ====

    /** ?��X�数当�X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    float GetParameterValue(EDifficultyParameter Parameter) const;

    /** 设置?�数?��X�用于自定�X�度�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void SetParameterValue(EDifficultyParameter Parameter, float Value};

    /** 调整?�数?��?带�X��? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    bool AdjustParameter(EDifficultyParameter Parameter, float Delta};

    /** ?��X�?��X�设�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    FMingDifficultySettings GetDifficultySettings() const;

    /** 应用完整设置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void ApplyDifficultySettings(const FMingDifficultySettings& Settings};

    // ==== ?��X�能调整 ====

    /** 调整AI?�度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    void AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence};

    /** ?��?AI?�度系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    float GetAIDifficultyMultiplier() const;

    /** 调整资�X��X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    void AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate};

    /** ?��?资�X��?系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    float GetResourceMultiplier() const;

    /** 调整任务?��X�度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    void AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale};

    /** ?��?任务?�度系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    float GetMissionDifficultyMultiplier() const;

    // ==== 平�?过渡 ====

    /** ?�否�?��过渡�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    bool IsTransitioning() const;

    /** ?��?当�?过渡进度 0-1 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    float GetTransitionProgress() const;

    /** 中断�?��进�X��?�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    void AbortTransition(};

    // ==== ?�知系�? ====

    /** 设置?�否?�知?�家?�度?��? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SetPlayerNotificationEnabled(bool bEnabled};

    /** ?�送自定�X�度?�知 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SendDifficultyNotification(const FString& Message, float Duration = 5.0f};

    // ==== ?��X====

    /** 保�?当�X�度?�置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void SaveDifficultySettings(};

    /** ?�载?�度?�置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void LoadDifficultySettings(};

    /** ?�置为�?认�?�?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void ResetToDefault(};

    // ==== 调�? ====

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    void PrintDebugInfo(};

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    FString GetDebugString() const;

    // ==== 事件委�? ====

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, const FDifficultyChangedEvent&, Event};

    UPROPERTY(BlueprintAssignable, Category = "Difficulty|Events")
    FOnDifficultyChanged OnDifficultyChanged;

protected:
    /** ?��X�度评估 */
    void PerformDifficultyEvaluation(};

    /** ?�据?�家表现计�?建议?�难度�X*/
    EDifficultyChangeDirection CalculateRecommendedChange() const;

    /** ?��X�度调整 */
    void ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction};

    /** 平�?过渡?�新 */
    void UpdateTransition(float DeltaTime};

    /** 完�?过渡 */
    void CompleteTransition(};

    /** 触�X�度?��?事件 */
    void BroadcastDifficultyChanged(const FString& Reason};

    /** ?�止?�荡：�X��?近�?调整?�史 */
    bool ShouldPreventOscillation() const;

    /** 订�X��?事件 */
    void SetupEventSubscriptions(};
    void CleanupEventSubscriptions(};

private:
    bool bIsInitialized;
    bool bIsLocked;
    bool bIsTransitioning;

    FMingDifficultySettings CurrentSettings;
    FMingDifficultySettings TargetSettings;
    FMingDifficultySettings TransitionStartSettings;

    float TransitionProgress;
    float TimeSinceLastEvaluation;

    TArray<FDifficultyChangedEvent> AdjustmentHistory;
    int32 MaxHistorySize;

    TWeakObjectPtr<UMingPlayerPerformanceTracker> PerformanceTracker;

    // 默认?�置缓�?
    static TMap<EDifficultyLevel, FMingPresetDifficultyConfig> CachedPresetConfigs;
};

