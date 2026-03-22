#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/FMingDifficultySettings.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyManager.generated.h"

// ?å?å£°æ?
class UMingPlayerPerformanceTracker;

/**
 * ?¾åº¦?˜å?äº‹ä»¶
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
 * ?¾åº¦ç®¡ç??? * ç®¡ç?æ¸¸æ??¾åº¦?ç½®?ŒåŠ¨?è??? * 
 * ?Ÿèƒ½ï¼? * - ?¾åº¦ç­‰çº§ç®¡ç? (Easy/Normal/Hard/Expert/Custom)
 * - ?¨æ€éš¾åº¦è??? * - ?¾åº¦äº‹ä»¶è§¦å?
 * - å¹³æ?è¿‡æ¸¡ç®—æ?
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

    // ==== ?¾åº¦ç­‰çº§ç®¡ç? ====

    /** è®¾ç½®?¾åº¦ç­‰çº§ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    void SetDifficultyLevel(EDifficultyLevel NewLevel};

    /** ?·å?å½“å??¾åº¦ç­‰çº§ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    EDifficultyLevel GetCurrentDifficultyLevel() const;

    /** ?·å?å½“å??¾åº¦?¾ç¤º?ç§° */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FString GetCurrentDifficultyDisplayName() const;

    /** ?·å??€?‰å¯?¨ç??¾åº¦ç­‰çº§ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    static TArray<EDifficultyLevel> GetAvailableDifficultyLevels(};

    /** ?·å?é¢„è®¾?¾åº¦?ç½® */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level) const;

    // ==== ?¨æ€éš¾åº¦è???====

    /** ?¯ç”¨/ç¦ç”¨?¨æ€éš¾åº¦è???*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void SetDynamicAdjustmentEnabled(bool bEnabled};

    /** ?¯å¦?¯ç”¨?¨æ€è???*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDynamicAdjustmentEnabled() const;

    /** ?‹åŠ¨è§¦å??¾åº¦è¯„ä¼°?Œè???*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void ForceDifficultyEvaluation(};

    /** ?å??¾åº¦ï¼ˆé˜²æ­¢è‡ª?¨è??´ï? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void LockDifficulty(bool bLocked};

    /** ?¾åº¦?¯å¦è¢«é?å®?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDifficultyLocked() const;

    // ==== ?‚æ•°è°ƒæ•´?¥å£ ====

    /** ?·å??‚æ•°å½“å???*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    float GetParameterValue(EDifficultyParameter Parameter) const;

    /** è®¾ç½®?‚æ•°?¼ï??‚ç”¨äºè‡ªå®šä??¾åº¦ï¼?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void SetParameterValue(EDifficultyParameter Parameter, float Value};

    /** è°ƒæ•´?‚æ•°?¼ï?å¸¦å??ï? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    bool AdjustParameter(EDifficultyParameter Parameter, float Delta};

    /** ?·å??€?‰å??°è®¾ç½?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    FMingDifficultySettings GetDifficultySettings() const;

    /** åº”ç”¨å®Œæ•´è®¾ç½® */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void ApplyDifficultySettings(const FMingDifficultySettings& Settings};

    // ==== ?¹å??Ÿèƒ½è°ƒæ•´ ====

    /** è°ƒæ•´AI?¾åº¦ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    void AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence};

    /** ?·å?AI?¾åº¦ç³»æ•° */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    float GetAIDifficultyMultiplier() const;

    /** è°ƒæ•´èµ„æ??Ÿæ???*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    void AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate};

    /** ?·å?èµ„æ??¶ç?ç³»æ•° */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    float GetResourceMultiplier() const;

    /** è°ƒæ•´ä»»åŠ¡?®æ??¾åº¦ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    void AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale};

    /** ?·å?ä»»åŠ¡?¾åº¦ç³»æ•° */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    float GetMissionDifficultyMultiplier() const;

    // ==== å¹³æ?è¿‡æ¸¡ ====

    /** ?¯å¦æ­?œ¨è¿‡æ¸¡ä¸?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    bool IsTransitioning() const;

    /** ?·å?å½“å?è¿‡æ¸¡è¿›åº¦ 0-1 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    float GetTransitionProgress() const;

    /** ä¸­æ–­æ­?œ¨è¿›è??„è?æ¸?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    void AbortTransition(};

    // ==== ?šçŸ¥ç³»ç? ====

    /** è®¾ç½®?¯å¦?šçŸ¥?©å®¶?¾åº¦?˜å? */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SetPlayerNotificationEnabled(bool bEnabled};

    /** ?‘é€è‡ªå®šä??¾åº¦?šçŸ¥ */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SendDifficultyNotification(const FString& Message, float Duration = 5.0f};

    // ==== ?ä???====

    /** ä¿å?å½“å??¾åº¦?ç½® */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void SaveDifficultySettings(};

    /** ? è½½?¾åº¦?ç½® */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void LoadDifficultySettings(};

    /** ?ç½®ä¸ºé?è®¤é?ç½?*/
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void ResetToDefault(};

    // ==== è°ƒè? ====

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    void PrintDebugInfo(};

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    FString GetDebugString() const;

    // ==== äº‹ä»¶å§”æ? ====

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, const FDifficultyChangedEvent&, Event};

    UPROPERTY(BlueprintAssignable, Category = "Difficulty|Events")
    FOnDifficultyChanged OnDifficultyChanged;

protected:
    /** ?§è??¾åº¦è¯„ä¼° */
    void PerformDifficultyEvaluation(};

    /** ?¹æ®?©å®¶è¡¨ç°è®¡ç?å»ºè®®?„éš¾åº¦å???*/
    EDifficultyChangeDirection CalculateRecommendedChange() const;

    /** ?§è??¾åº¦è°ƒæ•´ */
    void ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction};

    /** å¹³æ?è¿‡æ¸¡?´æ–° */
    void UpdateTransition(float DeltaTime};

    /** å®Œæ?è¿‡æ¸¡ */
    void CompleteTransition(};

    /** è§¦å??¾åº¦?˜å?äº‹ä»¶ */
    void BroadcastDifficultyChanged(const FString& Reason};

    /** ?²æ­¢?¯è¡ï¼šæ??¥æ?è¿‘ç?è°ƒæ•´?†å² */
    bool ShouldPreventOscillation() const;

    /** è®¢é??¸å?äº‹ä»¶ */
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

    // é»˜è®¤?ç½®ç¼“å?
    static TMap<EDifficultyLevel, FMingPresetDifficultyConfig> CachedPresetConfigs;
};

