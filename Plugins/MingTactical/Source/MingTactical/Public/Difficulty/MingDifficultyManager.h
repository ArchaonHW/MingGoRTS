#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/FMingDifficultySettings.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyManager.generated.h"

// 摧毀???
class UMingPlayerPerformanceTracker;

/**
 * ??��摧毀�ƥ�
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
 * ??�׺�?X * ��??��?X?��??�m摧毀??X * 
 * ??��?? * - ??�׵�?��?? (Easy/Normal/Hard/Expert/Custom)
 * - 摧毀��?X * - ??�רƥ��D??
 * - ��???���??
 */
UCLASS()
class MINGTACTICAL_API UMingDifficultyManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDifficultyManager();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime);

    // ==== ??�׵�?��?? ====

    /** ?�m??�׵�? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    void SetDifficultyLevel(EDifficultyLevel NewLevel);

    /** 摧毀??X?�׵�? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    EDifficultyLevel GetCurrentDifficultyLevel() const;

    /** 摧毀??X?��??��??? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FString GetCurrentDifficultyDisplayName() const;

    /** 目標數量�i???X?�׵�? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    static TArray<EDifficultyLevel> GetAvailableDifficultyLevels();

    /** 摧毀摧毀��??�m */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level) const;

    // ==== 摧毀��?X====

    /** ??��/�T��摧毀��?X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void SetDynamicAdjustmentEnabled(bool bEnabled);

    /** ??�_??��摧毀X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDynamicAdjustmentEnabled() const;

    /** ???�D?X?��?��???X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void ForceDifficultyEvaluation();

    /** ???X?�ס]�����目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void LockDifficulty(bool bLocked);

    /** ??��??�_�Q摧毀*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDifficultyLocked() const;

    // ==== 摧毀��??�f ====

    /** 目標數量?X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    float GetParameterValue(EDifficultyParameter Parameter) const;

    /** ?�m摧毀??X?�Τ_�۩w?X?��??*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void SetParameterValue(EDifficultyParameter Parameter, float Value);

    /** ?��摧毀??目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    bool AdjustParameter(EDifficultyParameter Parameter, float Delta);

    /** 目標數量?X摧毀*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    FMingDifficultySettings GetDifficultySettings() const;

    /** ?�Χ���?�m */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void ApplyDifficultySettings(const FMingDifficultySettings& Settings);

    // ==== ???X?��?�� ====

    /** ?��AI??�� */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    void AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence);

    /** 摧毀AI??�רt? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    float GetAIDifficultyMultiplier() const;

    /** ?��??X??X*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    void AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate);

    /** ???目標數量�t? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    float GetResourceMultiplier() const;

    /** ?���摧毀X?�� */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    void AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale);

    /** 摧毀��???�רt? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    float GetMissionDifficultyMultiplier() const;

    // ==== ��???�� ====

    /** ??�_摧毀?��??*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    bool IsTransitioning() const;

    /** 摧毀摧毀��?�� 0-1 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    float GetTransitionProgress() const;

    /** ��摧毀目標數量??*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    void AbortTransition();

    // ==== ??���t?? ====

    /** ?�m??�_??��??�a??��摧毀 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SetPlayerNotificationEnabled(bool bEnabled);

    /** ??�e�۩w?X?��??�� */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SendDifficultyNotification(const FString& Message, float Duration = 5.0f);

    // ==== ???X====

    /** �O摧毀X?��??�m */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void SaveDifficultySettings();

    /** 摧毀?��??�m */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void LoadDifficultySettings();

    /** ??�m摧毀摧毀*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void ResetToDefault();

    // ==== ??? ====

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    FString GetDebugString() const;

    // ==== �ƥ�e?? ====

    

    UPROPERTY(BlueprintAssignable, Category = "Difficulty|Events")
    FOnDifficultyChanged OnDifficultyChanged;

protected:
    /** ???X?��?�� */
    void PerformDifficultyEvaluation();

    /** ??�u??�a��摧毀��摧毀��?X*/
    EDifficultyChangeDirection CalculateRecommendedChange() const;

    /** ???X?��?�� */
    void ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction);

    /** ��???��??�s */
    void UpdateTransition(float DeltaTime);

    /** ��???�� */
    void CompleteTransition();

    /** �D?X?��摧毀�ƥ� */
    void BroadcastDifficultyChanged(const FString& Reason);

    /** ??��???�G?X???��???��??�v */
    bool ShouldPreventOscillation() const;

    /** ??X???�ƥ� */
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

    // �q???�m???
    static TMap<EDifficultyLevel, FMingPresetDifficultyConfig> CachedPresetConfigs;
};

