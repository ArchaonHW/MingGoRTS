#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAPISystem.h"
#include "MingRTSIntelligentAPIEnhancements.generated.h"

// ========== 智慧增強枚舉 ==========

UENUM(BlueprintType)
enum class EIntelligenceLevel : uint8
{
    Basic UMETA(DisplayName = "基礎智慧"),
    Adaptive UMETA(DisplayName = "適應性智慧"),
    Predictive UMETA(DisplayName = "預測性智慧"),
    Autonomous UMETA(DisplayName = "自主性智慧"),
    Collaborative UMETA(DisplayName = "協作性智慧")
};

UENUM(BlueprintType)
enum class ELearningAlgorithm : uint8
{
    NeuralNetwork UMETA(DisplayName = "神經網絡"),
    ReinforcementLearning UMETA(DisplayName = "強化學習"),
    GeneticAlgorithm UMETA(DisplayName = "遺傳算法"),
    DecisionTree UMETA(DisplayName = "決策樹"),
    Clustering UMETA(DisplayName = "聚類分析"),
    BayesianNetwork UMETA(DisplayName = "貝葉斯網絡"),
    EnsembleLearning UMETA(DisplayName = "集成學習"),
    DeepLearning UMETA(DisplayName = "深度學習")
};

UENUM(BlueprintType)
enum class EAutomationTrigger : uint8
{
    TimeBased UMETA(DisplayName = "時間觸發"),
    EventBased UMETA(DisplayName = "事件觸發"),
    ConditionBased UMETA(DisplayName = "條件觸發"),
    PerformanceBased UMETA(DisplayName = "性能觸發"),
    UserBased UMETA(DisplayName = "用戶觸發"),
    PredictiveBased UMETA(DisplayName = "預測觸發")
};

// ========== 智慧數據結構 ==========

USTRUCT(BlueprintType)
struct FIntelligenceProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProfileID;

    UPROPERTY(BlueprintReadOnly)
    EIntelligenceLevel IntelligenceLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<ELearningAlgorithm> EnabledAlgorithms;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> LearningWeights;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Configuration;

    UPROPERTY(BlueprintReadOnly)
    float AdaptationRate;

    UPROPERTY(BlueprintReadOnly)
    float PredictionAccuracy;

    FIntelligenceProfile()
    {
        IntelligenceLevel = EIntelligenceLevel::Adaptive;
        AdaptationRate = 0.5f;
        PredictionAccuracy = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FPredictionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PredictionID;

    UPROPERTY(BlueprintReadOnly)
    FString Context;

    UPROPERTY(BlueprintReadOnly)
    FString Prediction;

    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    FDateTime PredictedTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Factors;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    FPredictionResult()
    {
        Confidence = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAutomationWorkflow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString WorkflowID;

    UPROPERTY(BlueprintReadOnly)
    FString WorkflowName;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAPIInteraction> Steps;

    UPROPERTY(BlueprintReadOnly)
    EAutomationTrigger TriggerType;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> TriggerConditions;

    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;

    UPROPERTY(BlueprintReadOnly)
    int32 ExecutionCount;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastExecution;

    FAutomationWorkflow()
    {
        TriggerType = EAutomationTrigger::EventBased;
        bEnabled = true;
        ExecutionCount = 0;
        SuccessRate = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FInsightData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString InsightID;

    UPROPERTY(BlueprintReadOnly)
    FString Category;

    UPROPERTY(BlueprintReadOnly)
    FString Title;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    float Impact;

    UPROPERTY(BlueprintReadOnly)
    FString Severity;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedAPIs;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActionItems;

    UPROPERTY(BlueprintReadOnly)
    FDateTime GeneratedAt;

    FInsightData()
    {
        Impact = 0.0f;
        Severity = TEXT("Medium");
    }
};

// ========== 智慧增強API類 ==========

/**
 * 智慧API增強系統 - 提供高級智慧功能和自動化能力
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntelligentAPIEnhancements : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntelligentAPIEnhancements();

    // ========== 智慧配置管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Configuration")
    void InitializeIntelligenceSystem();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Configuration")
    void SetIntelligenceProfile(const FIntelligenceProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Configuration")
    FIntelligenceProfile GetIntelligenceProfile() const;

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Configuration")
    void EnableLearningAlgorithm(ELearningAlgorithm Algorithm, bool bEnabled);

    // ========== 高級預測系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Prediction")
    FPredictionResult PredictSystemBehavior(const FString& Context, const FString& TimeHorizon);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Prediction")
    TArray<FPredictionResult> PredictAPIInteractions(const FString& Scenario);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Prediction")
    FPredictionResult PredictPerformanceBottleneck(const TArray<EAPIType>& APIs);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Prediction")
    TArray<FPredictionResult> PredictUserNeeds(const FString& UserContext);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Prediction")
    float GetPredictionAccuracy(const FString& PredictionType) const;

    // ========== 智慧自動化系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Automation")
    void CreateIntelligentWorkflow(const FAutomationWorkflow& Workflow);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Automation")
    void ExecuteWorkflow(const FString& WorkflowID);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Automation")
    void EnablePredictiveAutomation(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Automation")
    FAutomationWorkflow SuggestOptimalWorkflow(const FString& Goal);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Automation")
    void OptimizeExistingWorkflow(const FString& WorkflowID);

    // ========== 深度學習集成 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Deep Learning")
    void TrainNeuralNetwork(const FString& ModelID, const TArray<FString>& TrainingData);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Deep Learning")
    FString ExecuteInference(const FString& ModelID, const FString& InputData);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Deep Learning")
    void UpdateModelWeights(const FString& ModelID, const TMap<FString, float>& Weights);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Deep Learning")
    float GetModelAccuracy(const FString& ModelID) const;

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Deep Learning")
    void EnableContinuousLearning(bool bEnabled);

    // ========== 智慧洞察系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Insights")
    TArray<FInsightData> GenerateSystemInsights();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Insights")
    TArray<FInsightData> AnalyzeAPIPatterns(const TArray<EAPIType>& APIs);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Insights")
    FInsightData GeneratePerformanceInsight(const TArray<FOptimizationMetric>& Metrics);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Insights")
    TArray<FInsightData> PredictSystemIssues(const FString& TimeFrame);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Insights")
    void ImplementInsightRecommendations(const FString& InsightID);

    // ========== 協作智慧系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Collaboration")
    void EnableCollaborativeIntelligence(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Collaboration")
    void ShareLearningData(const FString& TargetSystem, const FString& Data);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Collaboration")
    TArray<FString> ReceiveCollaborativeInsights();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Collaboration")
    void SynchronizeWithPeers(const TArray<FString>& PeerSystems);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Collaboration")
    void ParticipateInDistributedLearning(const FString& LearningTask);

    // ========== 自我進化系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Evolution")
    void EnableSelfEvolution(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Evolution")
    void EvolveSystemArchitecture();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Evolution")
    void OptimizeAPICommunication();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Evolution")
    void AdaptToNewRequirements(const TArray<FString>& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Evolution")
    float GetEvolutionProgress() const;

    // ========== 智慧監控系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Monitoring")
    void StartIntelligentMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Monitoring")
    TArray<FString> DetectAnomalies(const FString& SystemArea);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Monitoring")
    void EnableProactiveAlerts(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Monitoring")
    TArray<FString> GetSystemHealthPrediction();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Monitoring")
    void GenerateHealthReport(const FString& ReportPath);

    // ========== 智慧優化建議 ==========
    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Optimization")
    TArray<FString> GenerateOptimizationSuggestions();

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Optimization")
    void ImplementOptimizationSuggestions(const TArray<FString>& Suggestions);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Optimization")
    FOptimizationProfile GenerateAdaptiveProfile(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Optimization")
    void EnableContinuousOptimization(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Intelligent API|Optimization")
    float GetOptimizationImpact() const;

    // ========== 事件委託 ==========
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntelligenceLevelChanged, EIntelligenceLevel, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPredictionGenerated, const FPredictionResult&, Prediction);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInsightGenerated, const FInsightData&, Insight);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAutomationTriggered, const FString&, WorkflowID, const FAutomationWorkflow&, Workflow);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemEvolved, const FString&, EvolutionDescription);

    UPROPERTY(BlueprintAssignable)
    FOnIntelligenceLevelChanged OnIntelligenceLevelChanged;

    UPROPERTY(BlueprintAssignable)
    FOnPredictionGenerated OnPredictionGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnInsightGenerated OnInsightGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnAutomationTriggered OnAutomationTriggered;

    UPROPERTY(BlueprintAssignable)
    FOnSystemEvolved OnSystemEvolved;

private:
    // ========== 智慧配置 ==========
    UPROPERTY()
    FIntelligenceProfile CurrentProfile;

    UPROPERTY()
    TArray<ELearningAlgorithm> ActiveAlgorithms;

    UPROPERTY()
    bool bIntelligenceSystemInitialized;

    // ========== 預測系統 ==========
    UPROPERTY()
    TArray<FPredictionResult> PredictionHistory;

    UPROPERTY()
    TMap<FString, float> PredictionAccuracyMap;

    UPROPERTY()
    bool bPredictiveAutomationEnabled;

    // ========== 自動化系統 ==========
    UPROPERTY()
    TMap<FString, FAutomationWorkflow> AutomationWorkflows;

    UPROPERTY()
    TArray<FString> ActiveWorkflows;

    UPROPERTY()
    bool bContinuousLearningEnabled;

    // ========== 深度學習 ==========
    UPROPERTY()
    TMap<FString, TArray<FString>> NeuralNetworkModels;

    UPROPERTY()
    TMap<FString, float> ModelAccuracyMap;

    UPROPERTY()
    bool bSelfEvolutionEnabled;

    // ========== 洞察系統 ==========
    UPROPERTY()
    TArray<FInsightData> InsightHistory;

    UPROPERTY()
    TMap<FString, int32> InsightFrequency;

    // ========== 協作系統 ==========
    UPROPERTY()
    TArray<FString> ConnectedPeers;

    UPROPERTY()
    bool bCollaborativeIntelligenceEnabled;

    // ========== 監控系統 ==========
    UPROPERTY()
    TArray<FString> AnomalyHistory;

    UPROPERTY()
    bool bProactiveAlertsEnabled;

    // ========== 內部方法 ==========
    void InitializeNeuralNetworks();
    void SetupAutomationEngine();
    void ConfigurePredictionSystem();
    void InitializeInsightEngine();
    
    void ProcessLearningData();
    void UpdatePredictionModels();
    void OptimizeAutomations();
    void GenerateNewInsights();
    
    bool ValidatePrediction(const FPredictionResult& Prediction);
    void ExecuteEvolutionStep();
    void UpdateIntelligenceLevel();
    
    FString GenerateModelID();
    FString GenerateWorkflowID();
    FString GenerateInsightID();
    
    void SaveIntelligenceData();
    void LoadIntelligenceData();
    
    // ========== 智慧算法實現 ==========
    float CalculatePredictionConfidence(const FString& Context);
    TArray<FAPIInteraction> OptimizeWorkflowSteps(const TArray<FAPIInteraction>& BaseSteps);
    FInsightData AnalyzeSystemBehavior(const TArray<FAPIInteraction>& Interactions);
    void AdaptLearningWeights(const TArray<FPredictionResult>& Results);
    
    // ========== 協作功能實現 ==========
    void ShareWithPeers(const FString& Data);
    TArray<FString> AggregatePeerInsights();
    void SynchronizeLearningModels();
};
