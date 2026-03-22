#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/FMingDifficultySettings.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyManager.generated.h"

// ???????
class UMingPlayerPerformanceTracker;

/**
 * ??度????事件
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
 * ??度管?X * 管??游?X?度??置??????X * 
 * ??能?? * - ??度等?管?? (Easy/Normal/Hard/Expert/Custom)
 * - ????度?X * - ??度事件触??
 * - 平???渡算??
 */
UCLASS()
class MINGTACTICAL_API UMingDifficultyManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDifficultyManager();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime};

    // ==== ??度等?管?? ====

    /** ?置??度等? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    void SetDifficultyLevel(EDifficultyLevel NewLevel};

    /** ??????X?度等? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    EDifficultyLevel GetCurrentDifficultyLevel() const;

    /** ??????X?度??示??? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FString GetCurrentDifficultyDisplayName() const;

    /** ???X???可???X?度等? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    static TArray<EDifficultyLevel> GetAvailableDifficultyLevels();

    /** ????????度??置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level) const;

    // ==== ????度?X====

    /** ??用/禁用????度?X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void SetDynamicAdjustmentEnabled(bool bEnabled};

    /** ??否??用????X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDynamicAdjustmentEnabled() const;

    /** ???触?X?度?估???X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void ForceDifficultyEvaluation();

    /** ???X?度（防止自???X??? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void LockDifficulty(bool bLocked};

    /** ??度??否被????*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDifficultyLocked() const;

    // ==== ????整??口 ====

    /** ???X????X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    float GetParameterValue(EDifficultyParameter Parameter) const;

    /** ?置??????X?用于自定?X?度??*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void SetParameterValue(EDifficultyParameter Parameter, float Value};

    /** ?整?????????X??? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    bool AdjustParameter(EDifficultyParameter Parameter, float Delta};

    /** ???X????X????*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    FMingDifficultySettings GetDifficultySettings() const;

    /** ?用完整?置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void ApplyDifficultySettings(const FMingDifficultySettings& Settings};

    // ==== ???X?能?整 ====

    /** ?整AI??度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    void AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence};

    /** ????AI??度系? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    float GetAIDifficultyMultiplier() const;

    /** ?整??X??X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    void AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate};

    /** ??????X???系? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    float GetResourceMultiplier() const;

    /** ?整任????X?度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    void AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale};

    /** ????任???度系? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    float GetMissionDifficultyMultiplier() const;

    // ==== 平???渡 ====

    /** ??否?????渡??*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    bool IsTransitioning() const;

    /** ????????渡?度 0-1 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    float GetTransitionProgress() const;

    /** 中???????X?????*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    void AbortTransition();

    // ==== ??知系?? ====

    /** ?置??否??知??家??度???? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SetPlayerNotificationEnabled(bool bEnabled};

    /** ??送自定?X?度??知 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SendDifficultyNotification(const FString& Message, float Duration = 5.0f};

    // ==== ???X====

    /** 保????X?度??置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void SaveDifficultySettings();

    /** ?????度??置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void LoadDifficultySettings();

    /** ??置????????*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void ResetToDefault();

    // ==== ??? ====

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    FString GetDebugString() const;

    // ==== 事件委?? ====

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, const FDifficultyChangedEvent&, Event};

    UPROPERTY(BlueprintAssignable, Category = "Difficulty|Events")
    FOnDifficultyChanged OnDifficultyChanged;

protected:
    /** ???X?度?估 */
    void PerformDifficultyEvaluation();

    /** ??据??家表????建????度?X*/
    EDifficultyChangeDirection CalculateRecommendedChange() const;

    /** ???X?度?整 */
    void ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction};

    /** 平???渡??新 */
    void UpdateTransition(float DeltaTime};

    /** 完???渡 */
    void CompleteTransition();

    /** 触?X?度????事件 */
    void BroadcastDifficultyChanged(const FString& Reason};

    /** ??止???：?X???近???整??史 */
    bool ShouldPreventOscillation() const;

    /** ??X???事件 */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

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

    // 默???置???
    static TMap<EDifficultyLevel, FMingPresetDifficultyConfig> CachedPresetConfigs;
};

