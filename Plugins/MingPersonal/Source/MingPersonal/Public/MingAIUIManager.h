#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"
#include "MingAIUIManager.generated.h"

UENUM(BlueprintType)
enum class EAIUserProfile : uint8
{
    NewPlayer,             // ?∞Áé©ÂÆ?    CasualPlayer,          // ‰ºëÈ??©ÂÆ∂
    StrategicPlayer,       // Á≠ñÁï•?©ÂÆ∂
    PowerPlayer,           // ?çÂ∫¶?©ÂÆ∂
    SocialPlayer,          // Á§æ‰∫§?©ÂÆ∂
    Explorer,              // ?¢Á¥¢??    StoryFocused,          // ?áÊ?Â∞éÂ?
    AchievementHunter      // ?êÂ∞±?µ‰∫∫
};

UENUM(BlueprintType)
enum class EUIAdaptationType : uint8
{
    Layout,                // ‰ΩàÂ??©Ê?
    ColorScheme,           // ?çËâ≤?πÊ?
    AnimationSpeed,        // ?ïÁï´?üÂ∫¶
    InformationDensity,    // ‰ø°ÊÅØÂØÜÂ∫¶
    InteractionPattern,    // ‰∫§‰?Ê®°Â?
    NotificationLevel,     // ?öÁü•Á¥öÂà•
    TooltipFrequency,      // ?êÁ§∫?ªÁ?
    AutoSaveInterval       // ?™Â?‰øùÂ??ìÈ?
};

USTRUCT(BlueprintType)
struct FAIUserProfileData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    EAIUserProfile ProfileType;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    float PlayTimeHours;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    int32 QuestsCompleted;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    int32 RelationshipsFormed;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    float CombatWinRate;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    float SocialInteractionScore;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    float ExplorationScore;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    TArray<EPersonalUIType> FrequentlyUsedPanels;

    UPROPERTY(BlueprintReadOnly, Category = "AI Profile")
    TMap<EUIAdaptationType, float> AdaptationPreferences;

    FAIUserProfileData()
    {
        ProfileType = EAIUserProfile::NewPlayer;
        PlayTimeHours = 0.0f;
        QuestsCompleted = 0;
        RelationshipsFormed = 0;
        CombatWinRate = 0.0f;
        SocialInteractionScore = 0.0f;
        ExplorationScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAIUIAdaptationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Adaptation")
    EUIAdaptationType AdaptationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Adaptation")
    float AdaptationValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Adaptation")
    bool bIsEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Adaptation")
    float ConfidenceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Adaptation")
    FString Reason;

    FAIUIAdaptationConfig()
    {
        AdaptationType = EUIAdaptationType::Layout;
        AdaptationValue = 0.0f;
        bIsEnabled = false;
        ConfidenceScore = 0.0f;
        Reason = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIProfileUpdated, const FAIUserProfileData&, ProfileData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIAdaptationApplied, const FAIUIAdaptationConfig&, Adaptation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorAnalyzed, const FString&, BehaviorType, float, Confidence);

/**
 * AIÈ©ÖÂ??ÑUIÁÆ°Á??? * ?∫Êñº?®Êà∂Ë°åÁÇ∫?∫ËÉΩË™øÊï¥UIÈ´îÈ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAIUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIUIManager();

    // ?ùÂ??ñAI UIÁ≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void InitializeAIUIManager(UMingPersonalUIManager* InUIManager, 
                              UMingRelationshipManager* InRelationshipManager);

    // ?®Êà∂Ë°åÁÇ∫ËøΩËπ§
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackPanelUsage(EPersonalUIType PanelType, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackInteractionPattern(const FString& InteractionType, const FString& Target);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackSessionMetrics(float SessionTime, int32 ActionsPerformed);

    // AI?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void AnalyzeUserBehavior();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void UpdateUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    EAIUserProfile DetermineUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GenerateAdaptationRecommendations();

    // UI?©Ê?
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ApplyAdaptation(const FAIUIAdaptationConfig& Adaptation);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ApplyAllRecommendedAdaptations();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void OptimizeLayoutForProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void AdjustColorScheme();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAnimationSpeed();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void AdjustInformationDensity();

    // ?∫ËÉΩ?êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ShowContextualHelp(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SuggestNextAction();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void HighlightImportantFeatures();

    // ?ãÊÄßÂ??®Ëñ¶
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetRecommendedQuests();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetSuggestedCharacters();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<EPersonalUIType> GetRecommendedPanels();

    // A/BÊ∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void StartABTest(const FString& TestName, const FString& VariantA, const FString& VariantB);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void RecordABTestResult(const FString& TestName, const FString& Variant, float Success);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    FString GetBestABTestVariant(const FString& TestName);

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "AI UI")
    FAIUserProfileData GetCurrentProfile() const { return CurrentProfile; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    bool IsAIEnabled() const { return bAIEnabled; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GetActiveAdaptations() const;

    UFUNCTION(BlueprintPure, Category = "AI UI")
    float GetAdaptationConfidence(EUIAdaptationType AdaptationType) const;

    // Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAIEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAdaptationSensitivity(float Sensitivity);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetLearningRate(float Rate);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ResetUserProfile();

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIProfileUpdated OnAIProfileUpdated;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnUIAdaptationApplied OnUIAdaptationApplied;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIBehaviorAnalyzed OnAIBehaviorAnalyzed;

protected:
    // Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingPersonalUIManager> UIManager;

    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // AI?çÁΩÆ
    UPROPERTY()
    bool bAIEnabled = true;

    UPROPERTY()
    float AdaptationSensitivity = 0.7f;

    UPROPERTY()
    float LearningRate = 0.1f;

    UPROPERTY()
    int32 MinDataPointsForAnalysis = 10;

    // ?®Êà∂Ê™îÊ?
    UPROPERTY()
    FAIUserProfileData CurrentProfile;

    // Ë°åÁÇ∫?∏Ê?
    UPROPERTY()
    TArray<FString> BehaviorHistory;

    UPROPERTY()
    TMap<FString, int32> BehaviorCounts;

    UPROPERTY()
    TMap<EPersonalUIType, float> PanelUsageTimes;

    UPROPERTY()
    TMap<FString, float> InteractionPatterns;

    // ?©Ê??çÁΩÆ
    UPROPERTY()
    TArray<FAIUIAdaptationConfig> RecommendedAdaptations;

    UPROPERTY()
    TArray<FAIUIAdaptationConfig> ActiveAdaptations;

    // A/BÊ∏¨Ë©¶?∏Ê?
    // Ê≥®Ê?ÔºöTMap<TMap> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TMap<FString, float>> ABTestData;

    // ?ßÈÉ®?ΩÊï∏
    void ProcessBehaviorData();
    void CalculateProfileScores();
    void GenerateAdaptations();
    void ApplyAdaptationToUI(const FAIUIAdaptationConfig& Adaptation);
    float CalculateConfidence(const TArray<float>& DataPoints) const;
    EAIUserProfile ClassifyPlayer(const FAIUserProfileData& Profile) const;
    void LogAdaptation(const FAIUIAdaptationConfig& Adaptation);

    // AIÁÆóÊ?
    float CalculateBehaviorScore(const FString& BehaviorType) const;
    TArray<float> GetBehaviorTrend(const FString& BehaviorType, int32 WindowSize) const;
    bool IsBehaviorSignificant(const FString& BehaviorType, float Threshold) const;
    void UpdateAdaptationWeights();

private:
    // ËºîÂä©?ΩÊï∏
    void InitializeDefaultAdaptations();
    void SaveProfileData();
    void LoadProfileData();
    void CleanupOldData();
};
