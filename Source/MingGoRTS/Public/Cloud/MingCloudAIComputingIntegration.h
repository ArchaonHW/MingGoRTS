// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud AI Computing Integration - C2-2
// Provides cloud-based AI processing and distributed computing capabilities

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingCloudAIComputingIntegration.generated.h"

UENUM(BlueprintType)
enum class ECloudAITaskType: uint8 {
    ContentGeneration = 0, UMETA(DisplayName = "Content Generation"),
    ImageProcessing, UMETA(DisplayName = "Image Processing"),
    AudioSynthesis, UMETA(DisplayName = "Audio Synthesis"),
    TextAnalysis, UMETA(DisplayName = "Text Analysis"),
    BehaviorPrediction, UMETA(DisplayName = "Behavior Prediction"),
    StrategyOptimization, UMETA(DisplayName = "Strategy Optimization"),
    QualityAssessment, UMETA(DisplayName = "Quality Assessment"),
    PatternRecognition, UMETA(DisplayName = "Pattern Recognition"),
    NaturalLanguage, UMETA(DisplayName = "Natural Language"),
    ProceduralGeneration, UMETA(DisplayName = "Procedural Generation"),
    ModelTraining, UMETA(DisplayName = "Model Training"),
    BatchProcessing UMETA(DisplayName = "Batch Processing")
};

UENUM(BlueprintType)
enum class ECloudAITaskPriority: uint8 {
    Critical = 0, UMETA(DisplayName = "Critical"),
    High, UMETA(DisplayName = "High"),
    Normal, UMETA(DisplayName = "Normal"),
    Low, UMETA(DisplayName = "Low"),
    Background UMETA(DisplayName = "Background")
};

UENUM(BlueprintType)
enum class ECloudAITaskStatus: uint8 {
    Pending = 0, UMETA(DisplayName = "Pending"),
    Queued, UMETA(DisplayName = "Queued"),
    Processing, UMETA(DisplayName = "Processing"),
    Completed, UMETA(DisplayName = "Completed"),
    Failed, UMETA(DisplayName = "Failed"),
    Cancelled, UMETA(DisplayName = "Cancelled"),
    Timeout UMETA(DisplayName = "Timeout")
};

UENUM(BlueprintType)
enum class EAIProvider: uint8 {
    Local = 0, UMETA(DisplayName = "Local"),
    OpenAI, UMETA(DisplayName = "OpenAI"),
    Anthropic, UMETA(DisplayName = "Anthropic"),
    GoogleAI, UMETA(DisplayName = "Google AI"),
    AzureML, UMETA(DisplayName = "Azure ML"),
    AWSBedrock, UMETA(DisplayName = "AWS Bedrock"),
    CustomEndpoint, UMETA(DisplayName = "Custom Endpoint"),
    Hybrid UMETA(DisplayName = "Hybrid")
};

USTRUCT(BlueprintType)
struct FCloudAITaskRequest
{
    GENERATED_BODY()

    UPROPERTY()
    FString TaskID;

    UPROPERTY()
    ECloudAITaskType TaskType;

    UPROPERTY()
    ECloudAITaskPriority Priority;

    UPROPERTY()
    TMap<FString, FString> InputParameters;

    UPROPERTY()
    TMap<FString, TArray<uint8>> BinaryInputs;

    UPROPERTY()
    FString ContextData;

    UPROPERTY()
    float TimeoutSeconds;

    UPROPERTY()
    bool bRequireHighQuality;

    UPROPERTY()
    int32 MaxRetries;

    UPROPERTY()
    FString PreferredModel;

    UPROPERTY()
    TMap<FString, FString> CustomHeaders;

    FCloudAITaskRequest()
        : TaskType(ECloudAITaskType::ContentGeneration)
        , Priority(ECloudAITaskPriority::Normal)
        , TimeoutSeconds(30.0f)
        , bRequireHighQuality(false)
        , MaxRetries(3)
    {}
};

USTRUCT(BlueprintType)
struct FCloudAITaskResult
{
    GENERATED_BODY()

    UPROPERTY()
    FString TaskID;

    UPROPERTY()
    ECloudAITaskStatus Status;

    UPROPERTY()
    TMap<FString, FString> OutputParameters;

    UPROPERTY()
    TMap<FString, TArray<uint8>> BinaryOutputs;

    UPROPERTY()
    FString GeneratedContent;

    UPROPERTY()
    float ConfidenceScore;

    UPROPERTY()
    int32 ProcessingTimeMs;

    UPROPERTY()
    int32 TokensUsed;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    FString ProviderUsed;

    UPROPERTY()
    FString ModelUsed;

    UPROPERTY()
    uint32 CompletedTimestamp;

    FCloudAITaskResult()
        : Status(ECloudAITaskStatus::Pending)
        , ConfidenceScore(0.0f)
        , ProcessingTimeMs(0)
        , TokensUsed(0)
        , CompletedTimestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FAIProviderConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    EAIProvider Provider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    FString EndpointURL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    FString APIKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    FString ModelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    int32 MaxTokens;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float Temperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float TopP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    int32 MaxConcurrentRequests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float RequestTimeoutSeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bEnableRetry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bEnableCaching;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float CostPerRequest;

    FAIProviderConfig()
        : Provider(EAIProvider::Local)
        , MaxTokens(2048)
        , Temperature(0.7f)
        , TopP(0.9f)
        , MaxConcurrentRequests(5)
        , RequestTimeoutSeconds(30.0f)
        , bEnableRetry(true)
        , bEnableCaching(true)
        , CostPerRequest(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FCloudAIComputingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bEnableCloudProcessing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bFallbackToLocal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float LocalProcessingThresholdMs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    int32 MaxQueueSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    int32 BatchSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    float DailyBudget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bEnableCostTracking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bEnableUsageAnalytics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    TArray<FAIProviderConfig> ProviderConfigs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud AI")
    bool bSmartProviderSelection;

    FCloudAIComputingConfig()
        : bEnableCloudProcessing(true)
        , bFallbackToLocal(true)
        , LocalProcessingThresholdMs(100.0f)
        , MaxQueueSize(100)
        , BatchSize(1)
        , DailyBudget(10.0f)
        , bEnableCostTracking(true)
        , bEnableUsageAnalytics(true)
        , bSmartProviderSelection(true)
    {}
};

USTRUCT(BlueprintType)
struct FCloudAIUsageStats
{
    GENERATED_BODY()

    UPROPERTY()
    int32 TotalRequestsToday;

    UPROPERTY()
    int32 SuccessfulRequests;

    UPROPERTY()
    int32 FailedRequests;

    UPROPERTY()
    float TotalCostToday;

    UPROPERTY()
    int32 TotalTokensUsed;

    UPROPERTY()
    float AverageResponseTimeMs;

    UPROPERTY()
    TMap<ECloudAITaskType, int32> RequestsByType;

    UPROPERTY()
    TMap<EAIProvider, int32> RequestsByProvider;

    UPROPERTY()
    uint32 LastResetTimestamp;

    FCloudAIUsageStats()
        : TotalRequestsToday(0)
        , SuccessfulRequests(0)
        , FailedRequests(0)
        , TotalCostToday(0.0f)
        , TotalTokensUsed(0)
        , AverageResponseTimeMs(0.0f)
        , LastResetTimestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FQueuedTaskInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString TaskID;

    UPROPERTY()
    ECloudAITaskType TaskType;

    UPROPERTY()
    ECloudAITaskPriority Priority;

    UPROPERTY()
    ECloudAITaskStatus Status;

    UPROPERTY()
    uint32 QueueTimestamp;

    UPROPERTY()
    uint32 StartTimestamp;

    UPROPERTY()
    int32 EstimatedProcessingTimeMs;

    UPROPERTY()
    float Progress;

    FQueuedTaskInfo()
        : TaskType(ECloudAITaskType::ContentGeneration)
        , Priority(ECloudAITaskPriority::Normal)
        , Status(ECloudAITaskStatus::Pending)
        , QueueTimestamp(0)
        , StartTimestamp(0)
        , EstimatedProcessingTimeMs(0)
        , Progress(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudAITaskStarted, FString, TaskID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudAITaskProgress, FString, TaskID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudAITaskCompleted, FCloudAITaskResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudAITaskFailed, FString, TaskID, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloudAIProviderAvailable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloudAIBudgetThreshold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudAIQueueUpdated, int32, QueueLength);

/**
 * Cloud AI Computing Integration
 * Provides cloud-based AI processing and distributed computing capabilities
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingCloudAIComputingIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingCloudAIComputingIntegration();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void InitializeCloudAI(const FCloudAIComputingConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void ShutdownCloudAI();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FString SubmitTask(const FCloudAITaskRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool SubmitTaskAsync(const FCloudAITaskRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult GetTaskResult(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool CancelTask(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsTaskComplete(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    ECloudAITaskStatus GetTaskStatus(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    TArray<FQueuedTaskInfo> GetQueuedTasks();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    int32 GetQueueLength() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void ClearQueue();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool SetTaskPriority(const FString& TaskID, ECloudAITaskPriority NewPriority);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult ProcessTaskLocally(const FCloudAITaskRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult ProcessTaskInCloud(const FCloudAITaskRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsCloudProcessingEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void EnableCloudProcessing(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsCloudAIAvailable() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void AddProviderConfig(const FAIProviderConfig& ProviderConfig);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void RemoveProviderConfig(EAIProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void SetActiveProvider(EAIProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    EAIProvider GetActiveProvider() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool TestProviderConnection(EAIProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    float GetProviderLatency(EAIProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void SetDailyBudget(float Budget);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    float GetDailyBudget() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    float GetRemainingBudget() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAIUsageStats GetUsageStats() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void ResetUsageStats();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsWithinBudget() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void EnableCostTracking(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsCostTrackingEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void SetCostAlertThreshold(float Threshold);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    float GetAverageRequestCost() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    float GetEstimatedCostForTask(ECloudAITaskType TaskType) const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    TArray<FString> SubmitBatchTasks(const TArray<FCloudAITaskRequest>& Requests);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    TArray<FCloudAITaskResult> GetBatchResults(const TArray<FString>& TaskIDs);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool CacheTaskResult(const FString& TaskID, const FCloudAITaskResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool HasCachedResult(const FString& TaskKey) const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult GetCachedResult(const FString& TaskKey);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    int32 GetCacheSize() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult GenerateContent(const FString& Prompt, ECloudAITaskPriority Priority);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult AnalyzeImage(const TArray<uint8>& ImageData, const FString& AnalysisType);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult SynthesizeAudio(const FString& Text, const FString& VoiceType);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult PredictPlayerBehavior(const TMap<FString, FString>& PlayerData);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FCloudAITaskResult OptimizeStrategy(const TMap<FString, FString>& GameState);

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void PauseProcessing();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void ResumeProcessing();

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    bool IsProcessingPaused() const;

    UFUNCTION(BlueprintPure, Category = "Cloud AI")
    int32 GetActiveTaskCount() const;

    UFUNCTION(BlueprintPure, Category = "Cloud AI")
    int32 GetCompletedTaskCount() const;

    UFUNCTION(BlueprintPure, Category = "Cloud AI")
    int32 GetFailedTaskCount() const;

    UFUNCTION(BlueprintPure, Category = "Cloud AI")
    float GetAverageProcessingTime() const;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAITaskStarted OnTaskStarted;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAITaskProgress OnTaskProgress;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAITaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAITaskFailed OnTaskFailed;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAIProviderAvailable OnProviderAvailable;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAIBudgetThreshold OnBudgetThreshold;

    UPROPERTY(BlueprintAssignable, Category = "Cloud AI Events")
    FOnCloudAIQueueUpdated OnQueueUpdated;

protected:
    UPROPERTY()
    FCloudAIComputingConfig Config;

    UPROPERTY()
    TMap<FString, FCloudAITaskRequest> PendingTasks;

    UPROPERTY()
    TMap<FString, FCloudAITaskResult> CompletedTasks;

    UPROPERTY()
    TArray<FQueuedTaskInfo> TaskQueue;

    UPROPERTY()
    TMap<EAIProvider, FAIProviderConfig> ProviderConfigurations;

    UPROPERTY()
    EAIProvider ActiveProvider;

    UPROPERTY()
    FCloudAIUsageStats UsageStats;

    UPROPERTY()
    bool bCloudAvailable;

    UPROPERTY()
    bool bProcessingPaused;

    UPROPERTY()
    float CostAlertThreshold;

    UPROPERTY()
    int32 ActiveTaskCount;

    UPROPERTY()
    int32 CompletedTaskCount;

    UPROPERTY()
    int32 FailedTaskCount;

    UPROPERTY()
    TMap<FString, FCloudAITaskResult> ResultCache;

    void ProcessTaskQueue();
    void ExecuteTask(const FString& TaskID);
    void CompleteTask(const FString& TaskID, const FCloudAITaskResult& Result);
    void FailTask(const FString& TaskID, const FString& Error);
    FCloudAITaskResult CallCloudProvider(const FCloudAITaskRequest& Request, EAIProvider Provider);
    FCloudAITaskResult CallOpenAI(const FCloudAITaskRequest& Request);
    FCloudAITaskResult CallAnthropic(const FCloudAITaskRequest& Request);
    FCloudAITaskResult CallGoogleAI(const FCloudAITaskRequest& Request);
    FCloudAITaskResult CallAzureML(const FCloudAITaskRequest& Request);
    FCloudAITaskResult CallAWSBedrock(const FCloudAITaskRequest& Request);
    FCloudAITaskResult CallLocalProcessing(const FCloudAITaskRequest& Request);
    EAIProvider SelectBestProvider(const FCloudAITaskRequest& Request);
    int32 CalculatePriorityScore(ECloudAITaskPriority Priority);
    void UpdateUsageStats(const FCloudAITaskResult& Result);
    bool ShouldUseCache(const FCloudAITaskRequest& Request);
    FString GenerateCacheKey(const FCloudAITaskRequest& Request);
    void CheckBudgetThreshold();
    void CleanupOldTasks();
    void LogCloudAI(const FString& Message);
    static UMingCloudAIComputingIntegration* Get(UObject* WorldContextObject);
};
