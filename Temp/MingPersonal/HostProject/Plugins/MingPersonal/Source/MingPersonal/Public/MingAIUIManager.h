#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"
#include "MingAIUIManager.generated.h"

UENUM(BlueprintType)
enum class EAIUserProfile : uint8
{
    NewPlayer,             // 新玩家
    CasualPlayer,          // 休閒玩家
    StrategicPlayer,       // 策略玩家
    PowerPlayer,           // 重度玩家
    SocialPlayer,          // 社交玩家
    Explorer,              // 探索者
    StoryFocused,          // 劇情導向
    AchievementHunter      // 成就獵人
};

UENUM(BlueprintType)
enum class EUIAdaptationType : uint8
{
    Layout,                // 佈局適應
    ColorScheme,           // 配色方案
    AnimationSpeed,        // 動畫速度
    InformationDensity,    // 信息密度
    InteractionPattern,    // 交互模式
    NotificationLevel,     // 通知級別
    TooltipFrequency,      // 提示頻率
    AutoSaveInterval       // 自動保存間隔
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
 * AI驅動的UI管理器
 * 基於用戶行為智能調整UI體驗
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAIUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIUIManager();

    // 初始化AI UI系統
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void InitializeAIUIManager(UMingPersonalUIManager* InUIManager, 
                              UMingRelationshipManager* InRelationshipManager);

    // 用戶行為追蹤
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackPanelUsage(EPersonalUIType PanelType, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackInteractionPattern(const FString& InteractionType, const FString& Target);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackSessionMetrics(float SessionTime, int32 ActionsPerformed);

    // AI分析
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void AnalyzeUserBehavior();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void UpdateUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    EAIUserProfile DetermineUserProfile();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GenerateAdaptationRecommendations();

    // UI適應
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

    // 智能提示
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ShowContextualHelp(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SuggestNextAction();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void HighlightImportantFeatures();

    // 個性化推薦
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetRecommendedQuests();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetSuggestedCharacters();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<EPersonalUIType> GetRecommendedPanels();

    // A/B測試
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void StartABTest(const FString& TestName, const FString& VariantA, const FString& VariantB);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void RecordABTestResult(const FString& TestName, const FString& Variant, float Success);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    FString GetBestABTestVariant(const FString& TestName);

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "AI UI")
    FAIUserProfileData GetCurrentProfile() const { return CurrentProfile; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    bool IsAIEnabled() const { return bAIEnabled; }

    UFUNCTION(BlueprintPure, Category = "AI UI")
    TArray<FAIUIAdaptationConfig> GetActiveAdaptations() const;

    UFUNCTION(BlueprintPure, Category = "AI UI")
    float GetAdaptationConfidence(EUIAdaptationType AdaptationType) const;

    // 設置
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAIEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetAdaptationSensitivity(float Sensitivity);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void SetLearningRate(float Rate);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ResetUserProfile();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIProfileUpdated OnAIProfileUpdated;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnUIAdaptationApplied OnUIAdaptationApplied;

    UPROPERTY(BlueprintAssignable, Category = "AI Events")
    FOnAIBehaviorAnalyzed OnAIBehaviorAnalyzed;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingPersonalUIManager> UIManager;

    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // AI配置
    UPROPERTY()
    bool bAIEnabled = true;

    UPROPERTY()
    float AdaptationSensitivity = 0.7f;

    UPROPERTY()
    float LearningRate = 0.1f;

    UPROPERTY()
    int32 MinDataPointsForAnalysis = 10;

    // 用戶檔案
    UPROPERTY()
    FAIUserProfileData CurrentProfile;

    // 行為數據
    UPROPERTY()
    TArray<FString> BehaviorHistory;

    UPROPERTY()
    TMap<FString, int32> BehaviorCounts;

    UPROPERTY()
    TMap<EPersonalUIType, float> PanelUsageTimes;

    UPROPERTY()
    TMap<FString, float> InteractionPatterns;

    // 適應配置
    UPROPERTY()
    TArray<FAIUIAdaptationConfig> RecommendedAdaptations;

    UPROPERTY()
    TArray<FAIUIAdaptationConfig> ActiveAdaptations;

    // A/B測試數據
    // 注意：TMap<TMap> 不支持 UPROPERTY
    TMap<FString, TMap<FString, float>> ABTestData;

    // 內部函數
    void ProcessBehaviorData();
    void CalculateProfileScores();
    void GenerateAdaptations();
    void ApplyAdaptationToUI(const FAIUIAdaptationConfig& Adaptation);
    float CalculateConfidence(const TArray<float>& DataPoints) const;
    EAIUserProfile ClassifyPlayer(const FAIUserProfileData& Profile) const;
    void LogAdaptation(const FAIUIAdaptationConfig& Adaptation);

    // AI算法
    float CalculateBehaviorScore(const FString& BehaviorType) const;
    TArray<float> GetBehaviorTrend(const FString& BehaviorType, int32 WindowSize) const;
    bool IsBehaviorSignificant(const FString& BehaviorType, float Threshold) const;
    void UpdateAdaptationWeights();

private:
    // 輔助函數
    void InitializeDefaultAdaptations();
    void SaveProfileData();
    void LoadProfileData();
    void CleanupOldData();
};
