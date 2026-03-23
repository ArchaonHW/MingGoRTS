#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSIntelligentAPIEnhancements.h"
#include "MingRTSAdvancedAPIFeatures.generated.h"

// ========== 高級API功能枚舉 ==========

UENUM(BlueprintType)
enum class EAPIFeature : uint8
{
    NaturalLanguageProcessing UMETA(DisplayName = "自然語言處理"),
    ComputerVision UMETA(DisplayName = "計算機視覺"),
    VoiceRecognition UMETA(DisplayName = "語音識別"),
    SentimentAnalysis UMETA(DisplayName = "情感分析"),
    BehaviorAnalysis UMETA(DisplayName = "行為分析"),
    AnomalyDetection UMETA(DisplayName = "異常檢測"),
    AutoHealing UMETA(DisplayName = "自動修復"),
    ContextualHelp UMETA(DisplayName = "情境幫助"),
    SmartNotifications UMETA(DisplayName = "智能通知"),
    PredictiveMaintenance UMETA(DisplayName = "預測性維護")
};

UENUM(BlueprintType)
enum class EDataMiningType : uint8
{
    PatternRecognition UMETA(DisplayName = "模式識別"),
    TrendAnalysis UMETA(DisplayName = "趨勢分析"),
    CorrelationAnalysis UMETA(DisplayName = "相關性分析"),
    Clustering UMETA(DisplayName = "聚類分析"),
    Classification UMETA(DisplayName = "分類分析"),
    RegressionAnalysis UMETA(DisplayName = "回歸分析"),
    TimeSeriesAnalysis UMETA(DisplayName = "時間序列分析"),
    AssociationRules UMETA(DisplayName = "關聯規則")
};

UENUM(BlueprintType)
enum class ESecurityLevel : uint8
{
    Basic UMETA(DisplayName = "基礎安全"),
    Standard UMETA(DisplayName = "標準安全"),
    Enhanced UMETA(DisplayName = "增強安全"),
    Advanced UMETA(DisplayName = "高級安全"),
    Military UMETA(DisplayName = "軍事級安全"),
    Quantum UMETA(DisplayName = "量子級安全")
};

// ========== 高級功能數據結構 ==========

USTRUCT(BlueprintType)
struct FAPIFeature
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAPIFeature FeatureType;

    UPROPERTY(BlueprintReadOnly)
    FString FeatureName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;

    UPROPERTY(BlueprintReadOnly)
    float Accuracy;

    UPROPERTY(BlueprintReadOnly)
    float Performance;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Configuration;

    FAPIFeature()
    {
        bEnabled = true;
        Accuracy = 0.0f;
        Performance = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FDataMiningResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ResultID;

    UPROPERTY(BlueprintReadOnly)
    EDataMiningType MiningType;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Patterns;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Insights;

    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> Metrics;

    UPROPERTY(BlueprintReadOnly)
    FDateTime GeneratedAt;

    FDataMiningResult()
    {
        Confidence = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSecurityThreat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ThreatID;

    UPROPERTY(BlueprintReadOnly)
    FString ThreatType;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    ESecurityLevel Severity;

    UPROPERTY(BlueprintReadOnly)
    FString Source;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedAPIs;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MitigationSteps;

    UPROPERTY(BlueprintReadOnly)
    FDateTime DetectedAt;

    FSecurityThreat()
    {
        Severity = ESecurityLevel::Basic;
    }
};

USTRUCT(BlueprintType)
struct FContextualHelp
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString HelpID;

    UPROPERTY(BlueprintReadOnly)
    FString Context;

    UPROPERTY(BlueprintReadOnly)
    FString Question;

    UPROPERTY(BlueprintReadOnly)
    FString Answer;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedTopics;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StepByStepGuide;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CodeExamples;

    UPROPERTY(BlueprintReadOnly)
    float RelevanceScore;

    FContextualHelp()
    {
        RelevanceScore = 0.0f;
    }
};

// ========== 高級API功能類 ==========

/**
 * 高級API功能系統 - 提供前沿的AI和數據科學功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAdvancedAPIFeatures : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAdvancedAPIFeatures();

    // ========== 自然語言處理功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    FString ProcessNaturalLanguage(const FString& Input, const FString& Language);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    TArray<FString> ExtractEntities(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    FString TranslateText(const FString& Text, const FString& SourceLanguage, const FString& TargetLanguage);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    float AnalyzeSentiment(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    FString GenerateSummary(const FString& LongText, int32 MaxLength);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|NLP")
    TArray<FString> ExtractKeywords(const FString& Text);

    // ========== 計算機視覺功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Computer Vision")
    TArray<FString> AnalyzeImage(const FString& ImagePath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Computer Vision")
    FString DetectObjects(const FString& ImagePath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Computer Vision")
    FString RecognizeFaces(const FString& ImagePath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Computer Vision")
    FString AnalyzeVideo(const FString& VideoPath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Computer Vision")
    FString GenerateImageDescription(const FString& ImagePath);

    // ========== 語音識別功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Voice Recognition")
    FString RecognizeSpeech(const FString& AudioPath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Voice Recognition")
    FString IdentifySpeaker(const FString& AudioPath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Voice Recognition")
    float AnalyzeEmotionInVoice(const FString& AudioPath);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Voice Recognition")
    FString ConvertSpeechToText(const FString& AudioPath, const FString& Language);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Voice Recognition")
    FString GenerateVoiceCommand(const FString& Command);

    // ========== 數據挖掘功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Data Mining")
    FDataMiningResult MineDataPatterns(const TArray<FString>& Data, EDataMiningType MiningType);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Data Mining")
    TArray<FString> DiscoverTrends(const TArray<FString>& HistoricalData);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Data Mining")
    TMap<FString, float> FindCorrelations(const TArray<FString>& Variables);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Data Mining")
    TArray<FString> ClassifyData(const TArray<FString>& DataPoints, const FString& ClassificationType);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Data Mining")
    FString PredictNextValue(const TArray<FString>& TimeSeriesData);

    // ========== 行為分析功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Behavior Analysis")
    TArray<FString> AnalyzeUserBehavior(const TArray<FAPIInteraction>& Interactions);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Behavior Analysis")
    FString DetectUserIntent(const FString& Context, const TArray<FString>& Actions);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Behavior Analysis")
    TArray<FString> PredictUserActions(const FString& UserID);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Behavior Analysis")
    float CalculateUserSatisfaction(const FString& UserID);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Behavior Analysis")
    void OptimizeUserExperience(const FString& UserID);

    // ========== 異常檢測功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Anomaly Detection")
    TArray<FString> DetectSystemAnomalies(const TArray<FOptimizationMetric>& Metrics);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Anomaly Detection")
    FString DetectSecurityThreat(const TArray<FAPIInteraction>& APIInteractions);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Anomaly Detection")
    TArray<FString> DetectPerformanceAnomalies(const FString& SystemArea);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Anomaly Detection")
    bool IsBehaviorAnomalous(const FString& UserID, const FString& Action);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Anomaly Detection")
    void EnableRealTimeAnomalyDetection(bool bEnabled);

    // ========== 自動修復功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Auto Healing")
    bool AttemptAutoHeal(const FString& IssueID);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Auto Healing")
    void EnableAutoHealing(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Auto Healing")
    TArray<FString> GetHealingStrategies(const FString& IssueType);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Auto Healing")
    void ApplyHealingStrategy(const FString& StrategyID);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Auto Healing")
    float GetHealingSuccessRate() const;

    // ========== 情境幫助功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Contextual Help")
    FContextualHelp GenerateContextualHelp(const FString& Context, const FString& UserQuestion);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Contextual Help")
    TArray<FContextualHelp> GetRelevantHelpTopics(const FString& CurrentTask);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Contextual Help")
    FString GenerateStepByStepGuide(const FString& Problem);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Contextual Help")
    TArray<FString> GenerateCodeExamples(const FString& Task);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Contextual Help")
    void EnableProactiveHelp(bool bEnabled);

    // ========== 智能通知功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Smart Notifications")
    void SendSmartNotification(const FString& Message, const FString& Priority, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Smart Notifications")
    void SetNotificationPreferences(const TMap<FString, FString>& Preferences);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Smart Notifications")
    TArray<FString> GetPendingNotifications();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Smart Notifications")
    void OptimizeNotificationTiming();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Smart Notifications")
    void EnableIntelligentFiltering(bool bEnabled);

    // ========== 預測性維護功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Predictive Maintenance")
    TArray<FString> PredictMaintenanceNeeds();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Predictive Maintenance")
    void SchedulePreventiveMaintenance(const FString& ComponentID, const FDateTime& ScheduledTime);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Predictive Maintenance")
    float CalculateSystemHealthScore();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Predictive Maintenance")
    void OptimizeMaintenanceSchedule();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Predictive Maintenance")
    void EnablePredictiveMaintenance(bool bEnabled);

    // ========== 安全增強功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Security")
    FSecurityThreat DetectSecurityThreat(const FString& SystemArea);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Security")
    void SetSecurityLevel(ESecurityLevel Level);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Security")
    void EnableAdvancedEncryption(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Security")
    void ImplementSecurityMitigation(const FString& ThreatID);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Security")
    TArray<FSecurityThreat> GetActiveThreats();

    // ========== 功能管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Advanced API|Management")
    void EnableFeature(EAPIFeature Feature, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Management")
    FAPIFeature GetFeatureStatus(EAPIFeature Feature);

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Management")
    TArray<FAPIFeature> GetAllFeatures();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Management")
    void OptimizeFeaturePerformance();

    UFUNCTION(BlueprintCallable, Category = "Advanced API|Management")
    void UpdateFeatureConfiguration(EAPIFeature Feature, const TMap<FString, FString>& Config);

    // ========== 事件委託 ==========
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNaturalLanguageProcessed, const FString&, ProcessedText);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnomalyDetected, const FString&, AnomalyDescription);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecurityThreatDetected, const FSecurityThreat&, Threat);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoHealingCompleted, const FString&, IssueID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextualHelpGenerated, const FContextualHelp&, Help);

    UPROPERTY(BlueprintAssignable)
    FOnNaturalLanguageProcessed OnNaturalLanguageProcessed;

    UPROPERTY(BlueprintAssignable)
    FOnAnomalyDetected OnAnomalyDetected;

    UPROPERTY(BlueprintAssignable)
    FOnSecurityThreatDetected OnSecurityThreatDetected;

    UPROPERTY(BlueprintAssignable)
    FOnAutoHealingCompleted OnAutoHealingCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnContextualHelpGenerated OnContextualHelpGenerated;

private:
    // ========== 功能狀態管理 ==========
    UPROPERTY()
    TMap<EAPIFeature, FAPIFeature> FeatureStates;

    UPROPERTY()
    TArray<EAPIFeature> ActiveFeatures;

    // ========== 數據挖掘存儲 ==========
    UPROPERTY()
    TArray<FDataMiningResult> MiningHistory;

    UPROPERTY()
    TMap<EDataMiningType, TArray<FString>> MiningPatterns;

    // ========== 安全系統 ==========
    UPROPERTY()
    ESecurityLevel CurrentSecurityLevel;

    UPROPERTY()
    TArray<FSecurityThreat> ActiveThreats;

    UPROPERTY()
    bool bAdvancedEncryptionEnabled;

    // ========== 行為分析數據 ==========
    UPROPERTY()
    TMap<FString, TArray<FAPIInteraction>> UserBehaviorHistory;

    UPROPERTY()
    TMap<FString, TArray<FString>> UserIntentPatterns;

    // ========== 自動修復系統 ==========
    UPROPERTY()
    bool bAutoHealingEnabled;

    UPROPERTY()
    TMap<FString, TArray<FString>> HealingStrategies;

    UPROPERTY()
    float HealingSuccessRate;

    // ========== 通知系統 ==========
    UPROPERTY()
    TArray<FString> PendingNotifications;

    UPROPERTY()
    TMap<FString, FString> NotificationPreferences;

    UPROPERTY()
    bool bIntelligentFilteringEnabled;

    // ========== 內部方法 ==========
    void InitializeAdvancedFeatures();
    void SetupNaturalLanguageProcessor();
    void ConfigureComputerVision();
    void InitializeVoiceRecognition();
    void SetupDataMiningEngine();
    void ConfigureSecuritySystem();
    
    void ProcessFeatureRequest(EAPIFeature Feature);
    void UpdateFeatureMetrics(EAPIFeature Feature, float Performance);
    void HandleSecurityEvent(const FSecurityThreat& Threat);
    void ExecuteAutoHealingProcedure(const FString& IssueID);
    
    FString GenerateFeatureID();
    FString AnalyzeTextSentiment(const FString& Text);
    TArray<FString> ExtractImageFeatures(const FString& ImagePath);
    void LearnFromUserInteraction(const FString& UserID, const FString& Action);
    
    void SaveAdvancedFeatureData();
    void LoadAdvancedFeatureData();
};
