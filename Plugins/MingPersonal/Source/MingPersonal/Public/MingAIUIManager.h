#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorAnalyzed, const FString&, BehaviorType, float, Confidence);

#include "MingAIUIManager.generated.h"

UENUM(BlueprintType)
enum class EAIUserProfile: uint8 {
    NewPlayer,             // ??��??
    CasualPlayer,          // ��?X?�a
    StrategicPlayer,       // ����??�a
    PowerPlayer,           // ??��??�a
    SocialPlayer,          // ����??�a
    Explorer,              // ??��X
    StoryFocused,          // 摧毀��??
    AchievementHunter      // ??�N??�H
};

UENUM(BlueprintType)
enum class EUIAdaptationType: uint8 {
    Layout,                // �G?X???
    ColorScheme,           // ??��摧毀
    AnimationSpeed,        // ??�e??��
    InformationDensity,    // �H���K��
    InteractionPattern,    // ��??��??
    NotificationLevel,     // ??���ŧO
    TooltipFrequency,      // ??��摧毀
    AutoSaveInterval       // 摧毀�O?X???
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



 /**
 * AI UI管理器
 * 負責分析和適應用戶行為以優化UI體驗
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAIUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIUIManager();

    // ???X?AI UI�t��
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void InitializeAIUIManager(UMingPersonalUIManager* InUIManager, 
                              UMingRelationshipManager* InRelationshipManager);

    // ??��欰�l��
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackPanelUsage(EPersonalUIType PanelType, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackInteractionPattern(const FString& InteractionType, const FString& Target);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackSessionMetrics(float SessionTime, int32 ActionsPerformed);

    // AI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void AnalyzeUserBehavior();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void UpdateUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    EAIUserProfile DetermineUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GenerateAdaptationRecommendations();

    // UI摧毀
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

    // ??��??��
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ShowContextualHelp(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SuggestNextAction();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void HighlightImportantFeatures();

    // ??��?X?��
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetRecommendedQuests();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetSuggestedCharacters();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<EPersonalUIType> GetRecommendedPanels();

    // A/B����
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void StartABTest(const FString& TestName, const FString& VariantA, const FString& VariantB);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void RecordABTestResult(const FString& TestName, const FString& Variant, float Success);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    FString GetBestABTestVariant(const FString& TestName);

    // 摧毀�d??
    UFUNCTION(BlueprintPure, Category = "AI UI")
    FAIUserProfileData GetCurrentProfile() const { return CurrentProfile; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    bool IsAIEnabled() const { return bAIEnabled; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GetActiveAdaptations() const;

    UFUNCTION(BlueprintPure, Category = "AI UI")
    float GetAdaptationConfidence(EUIAdaptationType AdaptationType) const;

    // �]�m
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAIEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAdaptationSensitivity(float Sensitivity);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetLearningRate(float Rate);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ResetUserProfile();

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIProfileUpdated OnAIProfileUpdated;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnUIAdaptationApplied OnUIAdaptationApplied;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIBehaviorAnalyzed OnAIBehaviorAnalyzed;

protected:
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingPersonalUIManager> UIManager;

    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // AI??�m
    UPROPERTY()
    bool bAIEnabled = true;

    UPROPERTY()
    float AdaptationSensitivity = 0.7f;

    UPROPERTY()
    float LearningRate = 0.1f;

    UPROPERTY()
    int32 MinDataPointsForAnalysis = 10;

    // ??����??
    UPROPERTY()
    FAIUserProfileData CurrentProfile;

    // �欰摧毀
    UPROPERTY()
    TArray<FString> BehaviorHistory;

    UPROPERTY()
    TMap<FString, int32> BehaviorCounts;

    UPROPERTY()
    TMap<EPersonalUIType, float> PanelUsageTimes;

    UPROPERTY()
    TMap<FString, float> InteractionPatterns;

    // ???X?�m
    UPROPERTY()
    TArray<FAIUIAdaptationConfig> RecommendedAdaptations;

    UPROPERTY()
    TArray<FAIUIAdaptationConfig> ActiveAdaptations;

    // A/B����摧毀
    // �`??�GTMap<TMap> ����XUPROPERTY
    TMap<FString, TMap<FString, float>> ABTestData;

    // ??��??��
    void ProcessBehaviorData();
    void CalculateProfileScores();
    void GenerateAdaptations();
    void ApplyAdaptationToUI(const FAIUIAdaptationConfig& Adaptation);
    float CalculateConfidence(const TArray<float>& DataPoints) const;
    EAIUserProfile ClassifyPlayer(const FAIUserProfileData& Profile) const;
    void LogAdaptation(const FAIUIAdaptationConfig& Adaptation);

    // AI��??
    float CalculateBehaviorScore(const FString& BehaviorType) const;
    TArray<float> GetBehaviorTrend(const FString& BehaviorType, int32 WindowSize) const;
    bool IsBehaviorSignificant(const FString& BehaviorType, float Threshold) const;
    void UpdateAdaptationWeights();

private:
    // ���U??��
    void InitializeDefaultAdaptations();
    void SaveProfileData();
    void LoadProfileData();
    void CleanupOldData();
);

