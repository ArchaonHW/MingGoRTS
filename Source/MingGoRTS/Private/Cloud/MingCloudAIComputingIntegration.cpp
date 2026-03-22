// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud AI Computing Integration Implementation - C2-2

#include "Cloud/MingCloudAIComputingIntegration.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Async/Async.h"
#include "Misc/Guid.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

DEFINE_LOG_CATEGORY_STATIC(LogCloudAI, Log, All);

UMingCloudAIComputingIntegration::UMingCloudAIComputingIntegration()
    : ActiveProvider(EAIProvider::Local)
    , bCloudAvailable(false)
    , bProcessingPaused(false)
    , CostAlertThreshold(0.8f)
    , ActiveTaskCount(0)
    , CompletedTaskCount(0)
    , FailedTaskCount(0)
{
}

void UMingCloudAIComputingIntegration::InitializeCloudAI(const FCloudAIComputingConfig& InConfig)
{
    Config = InConfig;
    
    for (const auto& ProviderConfig : Config.ProviderConfigs)
    {
        ProviderConfigurations.Add(ProviderConfig.Provider, ProviderConfig);
    }
    
    if (Config.ProviderConfigs.Num() > 0)
    {
        ActiveProvider = Config.ProviderConfigs[0].Provider;
    }
    
    bCloudAvailable = Config.bEnableCloudProcessing;
    
    TestProviderConnection(ActiveProvider);
    
    LogCloudAI(TEXT("Cloud AI Computing Integration initialized"));
}

void UMingCloudAIComputingIntegration::ShutdownCloudAI()
{
    ClearQueue();
    ResultCache.Empty();
    
    LogCloudAI(TEXT("Cloud AI Computing Integration shutdown"));
}

FString UMingCloudAIComputingIntegration::SubmitTask(const FCloudAITaskRequest& Request)
{
    FString TaskID = FGuid::NewGuid().ToString();
    
    FCloudAITaskRequest NewRequest = Request;
    NewRequest.TaskID = TaskID;
    
    PendingTasks.Add(TaskID, NewRequest);
    
    FQueuedTaskInfo QueueInfo;
    QueueInfo.TaskID = TaskID;
    QueueInfo.TaskType = NewRequest.TaskType;
    QueueInfo.Priority = NewRequest.Priority;
    QueueInfo.Status = ECloudAITaskStatus::Queued;
    QueueInfo.QueueTimestamp = FDateTime::Now().ToUnixTimestamp();
    QueueInfo.EstimatedProcessingTimeMs = 1000;
    
    TaskQueue.Add(QueueInfo);
    TaskQueue.Sort([](const FQueuedTaskInfo& A, const FQueuedTaskInfo& B)
    {
        return CalculatePriorityScore(A.Priority) > CalculatePriorityScore(B.Priority);
    });
    
    OnQueueUpdated.Broadcast(TaskQueue.Num());
    OnTaskStarted.Broadcast(TaskID);
    
    LogCloudAI(FString::Printf(TEXT("Task submitted: %s (Type: %d)"), *TaskID, static_cast<int32>(NewRequest.TaskType)));
    
    ProcessTaskQueue();
    
    return TaskID;
}

bool UMingCloudAIComputingIntegration::SubmitTaskAsync(const FCloudAITaskRequest& Request)
{
    SubmitTask(Request);
    return true;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::GetTaskResult(const FString& TaskID)
{
    if (CompletedTasks.Contains(TaskID))
    {
        return CompletedTasks[TaskID];
    }
    
    FCloudAITaskResult EmptyResult;
    EmptyResult.TaskID = TaskID;
    EmptyResult.Status = ECloudAITaskStatus::Pending;
    EmptyResult.ErrorMessage = TEXT("Task not found or not completed");
    
    return EmptyResult;
}

bool UMingCloudAIComputingIntegration::CancelTask(const FString& TaskID)
{
    if (PendingTasks.Contains(TaskID))
    {
        PendingTasks.Remove(TaskID);
        
        for (int32 i = TaskQueue.Num() - 1; i >= 0; --i)
        {
            if (TaskQueue[i].TaskID == TaskID)
            {
                TaskQueue[i].Status = ECloudAITaskStatus::Cancelled;
                TaskQueue.RemoveAt(i);
                break;
            }
        }
        
        OnQueueUpdated.Broadcast(TaskQueue.Num());
        LogCloudAI(FString::Printf(TEXT("Task cancelled: %s"), *TaskID));
        return true;
    }
    
    return false;
}

bool UMingCloudAIComputingIntegration::IsTaskComplete(const FString& TaskID)
{
    return CompletedTasks.Contains(TaskID);
}

ECloudAITaskStatus UMingCloudAIComputingIntegration::GetTaskStatus(const FString& TaskID)
{
    if (CompletedTasks.Contains(TaskID))
    {
        return CompletedTasks[TaskID].Status;
    }
    
    for (const auto& Task : TaskQueue)
    {
        if (Task.TaskID == TaskID)
        {
            return Task.Status;
        }
    }
    
    return ECloudAITaskStatus::Pending;
}

TArray<FQueuedTaskInfo> UMingCloudAIComputingIntegration::GetQueuedTasks()
{
    return TaskQueue;
}

int32 UMingCloudAIComputingIntegration::GetQueueLength() const
{
    return TaskQueue.Num();
}

void UMingCloudAIComputingIntegration::ClearQueue()
{
    for (auto& Task : TaskQueue)
    {
        if (Task.Status == ECloudAITaskStatus::Queued || Task.Status == ECloudAITaskStatus::Pending)
        {
            Task.Status = ECloudAITaskStatus::Cancelled;
        }
    }
    
    TaskQueue.Empty();
    PendingTasks.Empty();
    
    OnQueueUpdated.Broadcast(0);
    LogCloudAI(TEXT("Task queue cleared"));
}

bool UMingCloudAIComputingIntegration::SetTaskPriority(const FString& TaskID, ECloudAITaskPriority NewPriority)
{
    for (auto& Task : TaskQueue)
    {
        if (Task.TaskID == TaskID && Task.Status == ECloudAITaskStatus::Queued)
        {
            Task.Priority = NewPriority;
            
            TaskQueue.Sort([](const FQueuedTaskInfo& A, const FQueuedTaskInfo& B)
            {
                return CalculatePriorityScore(A.Priority) > CalculatePriorityScore(B.Priority);
            });
            
            return true;
        }
    }
    
    return false;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::ProcessTaskLocally(const FCloudAITaskRequest& Request)
{
    uint32 StartTime = FDateTime::Now().GetTicks();
    
    FCloudAITaskResult Result = CallLocalProcessing(Request);
    
    uint32 EndTime = FDateTime::Now().GetTicks();
    Result.ProcessingTimeMs = (EndTime - StartTime) / 10000;
    Result.ProviderUsed = TEXT("Local");
    
    UpdateUsageStats(Result);
    
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::ProcessTaskInCloud(const FCloudAITaskRequest& Request)
{
    EAIProvider Provider = Config.bSmartProviderSelection ? SelectBestProvider(Request) : ActiveProvider;
    
    return CallCloudProvider(Request, Provider);
}

bool UMingCloudAIComputingIntegration::IsCloudProcessingEnabled() const
{
    return Config.bEnableCloudProcessing;
}

void UMingCloudAIComputingIntegration::EnableCloudProcessing(bool bEnable)
{
    Config.bEnableCloudProcessing = bEnable;
    bCloudAvailable = bEnable;
    
    LogCloudAI(FString::Printf(TEXT("Cloud processing %s"), bEnable ? TEXT("enabled") : TEXT("disabled")));
}

bool UMingCloudAIComputingIntegration::IsCloudAIAvailable() const
{
    return bCloudAvailable;
}

void UMingCloudAIComputingIntegration::AddProviderConfig(const FAIProviderConfig& ProviderConfig)
{
    ProviderConfigurations.Add(ProviderConfig.Provider, ProviderConfig);
    
    if (!Config.ProviderConfigs.ContainsByPredicate([&](const FAIProviderConfig& Config)
    {
        return Config.Provider == ProviderConfig.Provider;
    }))
    {
        Config.ProviderConfigs.Add(ProviderConfig);
    }
}

void UMingCloudAIComputingIntegration::RemoveProviderConfig(EAIProvider Provider)
{
    ProviderConfigurations.Remove(Provider);
    Config.ProviderConfigs.RemoveAll([Provider](const FAIProviderConfig& Config)
    {
        return Config.Provider == Provider;
    });
}

void UMingCloudAIComputingIntegration::SetActiveProvider(EAIProvider Provider)
{
    if (ProviderConfigurations.Contains(Provider))
    {
        ActiveProvider = Provider;
        LogCloudAI(FString::Printf(TEXT("Active provider set to: %d"), static_cast<int32>(Provider)));
    }
}

EAIProvider UMingCloudAIComputingIntegration::GetActiveProvider() const
{
    return ActiveProvider;
}

bool UMingCloudAIComputingIntegration::TestProviderConnection(EAIProvider Provider)
{
    LogCloudAI(FString::Printf(TEXT("Testing connection to provider: %d"), static_cast<int32>(Provider)));
    
    bCloudAvailable = true;
    OnProviderAvailable.Broadcast();
    
    return true;
}

float UMingCloudAIComputingIntegration::GetProviderLatency(EAIProvider Provider)
{
    return 100.0f;
}

void UMingCloudAIComputingIntegration::SetDailyBudget(float Budget)
{
    Config.DailyBudget = Budget;
}

float UMingCloudAIComputingIntegration::GetDailyBudget() const
{
    return Config.DailyBudget;
}

float UMingCloudAIComputingIntegration::GetRemainingBudget() const
{
    return FMath::Max(0.0f, Config.DailyBudget - UsageStats.TotalCostToday);
}

FCloudAIUsageStats UMingCloudAIComputingIntegration::GetUsageStats() const
{
    return UsageStats;
}

void UMingCloudAIComputingIntegration::ResetUsageStats()
{
    UsageStats = FCloudAIUsageStats();
    UsageStats.LastResetTimestamp = FDateTime::Now().ToUnixTimestamp();
}

bool UMingCloudAIComputingIntegration::IsWithinBudget() const
{
    return UsageStats.TotalCostToday < Config.DailyBudget;
}

void UMingCloudAIComputingIntegration::EnableCostTracking(bool bEnable)
{
    Config.bEnableCostTracking = bEnable;
}

bool UMingCloudAIComputingIntegration::IsCostTrackingEnabled() const
{
    return Config.bEnableCostTracking;
}

void UMingCloudAIComputingIntegration::SetCostAlertThreshold(float Threshold)
{
    CostAlertThreshold = FMath::Clamp(Threshold, 0.0f, 1.0f);
}

float UMingCloudAIComputingIntegration::GetAverageRequestCost() const
{
    if (UsageStats.TotalRequestsToday > 0)
    {
        return UsageStats.TotalCostToday / UsageStats.TotalRequestsToday;
    }
    return 0.0f;
}

float UMingCloudAIComputingIntegration::GetEstimatedCostForTask(ECloudAITaskType TaskType) const
{
    switch (TaskType)
    {
    case ECloudAITaskType::ContentGeneration:
        return 0.002f;
    case ECloudAITaskType::ImageProcessing:
        return 0.02f;
    case ECloudAITaskType::AudioSynthesis:
        return 0.015f;
    case ECloudAITaskType::BehaviorPrediction:
        return 0.001f;
    default:
        return 0.005f;
    }
}

TArray<FString> UMingCloudAIComputingIntegration::SubmitBatchTasks(const TArray<FCloudAITaskRequest>& Requests)
{
    TArray<FString> TaskIDs;
    
    for (const auto& Request : Requests)
    {
        TaskIDs.Add(SubmitTask(Request));
    }
    
    return TaskIDs;
}

TArray<FCloudAITaskResult> UMingCloudAIComputingIntegration::GetBatchResults(const TArray<FString>& TaskIDs)
{
    TArray<FCloudAITaskResult> Results;
    
    for (const auto& TaskID : TaskIDs)
    {
        Results.Add(GetTaskResult(TaskID));
    }
    
    return Results;
}

bool UMingCloudAIComputingIntegration::CacheTaskResult(const FString& TaskID, const FCloudAITaskResult& Result)
{
    ResultCache.Add(TaskID, Result);
    
    while (ResultCache.Num() > 100)
    {
        auto It = ResultCache.CreateIterator();
        if (It)
        {
            It.RemoveCurrent();
        }
    }
    
    return true;
}

bool UMingCloudAIComputingIntegration::HasCachedResult(const FString& TaskKey) const
{
    return ResultCache.Contains(TaskKey);
}

FCloudAITaskResult UMingCloudAIComputingIntegration::GetCachedResult(const FString& TaskKey)
{
    if (ResultCache.Contains(TaskKey))
    {
        return ResultCache[TaskKey];
    }
    
    FCloudAITaskResult EmptyResult;
    EmptyResult.Status = ECloudAITaskStatus::Failed;
    EmptyResult.ErrorMessage = TEXT("Cache miss");
    return EmptyResult;
}

void UMingCloudAIComputingIntegration::ClearCache()
{
    ResultCache.Empty();
}

int32 UMingCloudAIComputingIntegration::GetCacheSize() const
{
    return ResultCache.Num();
}

FCloudAITaskResult UMingCloudAIComputingIntegration::GenerateContent(const FString& Prompt, ECloudAITaskPriority Priority)
{
    FCloudAITaskRequest Request;
    Request.TaskType = ECloudAITaskType::ContentGeneration;
    Request.Priority = Priority;
    Request.InputParameters.Add(TEXT("prompt"), Prompt);
    
    FString TaskID = SubmitTask(Request);
    
    ExecuteTask(TaskID);
    
    return GetTaskResult(TaskID);
}

FCloudAITaskResult UMingCloudAIComputingIntegration::AnalyzeImage(const TArray<uint8>& ImageData, const FString& AnalysisType)
{
    FCloudAITaskRequest Request;
    Request.TaskType = ECloudAITaskType::ImageProcessing;
    Request.BinaryInputs.Add(TEXT("image"), ImageData);
    Request.InputParameters.Add(TEXT("analysis_type"), AnalysisType);
    
    FString TaskID = SubmitTask(Request);
    ExecuteTask(TaskID);
    
    return GetTaskResult(TaskID);
}

FCloudAITaskResult UMingCloudAIComputingIntegration::SynthesizeAudio(const FString& Text, const FString& VoiceType)
{
    FCloudAITaskRequest Request;
    Request.TaskType = ECloudAITaskType::AudioSynthesis;
    Request.InputParameters.Add(TEXT("text"), Text);
    Request.InputParameters.Add(TEXT("voice"), VoiceType);
    
    FString TaskID = SubmitTask(Request);
    ExecuteTask(TaskID);
    
    return GetTaskResult(TaskID);
}

FCloudAITaskResult UMingCloudAIComputingIntegration::PredictPlayerBehavior(const TMap<FString, FString>& PlayerData)
{
    FCloudAITaskRequest Request;
    Request.TaskType = ECloudAITaskType::BehaviorPrediction;
    Request.InputParameters = PlayerData;
    
    FString TaskID = SubmitTask(Request);
    ExecuteTask(TaskID);
    
    return GetTaskResult(TaskID);
}

FCloudAITaskResult UMingCloudAIComputingIntegration::OptimizeStrategy(const TMap<FString, FString>& GameState)
{
    FCloudAITaskRequest Request;
    Request.TaskType = ECloudAITaskType::StrategyOptimization;
    Request.InputParameters = GameState;
    
    FString TaskID = SubmitTask(Request);
    ExecuteTask(TaskID);
    
    return GetTaskResult(TaskID);
}

void UMingCloudAIComputingIntegration::PauseProcessing()
{
    bProcessingPaused = true;
    LogCloudAI(TEXT("Processing paused"));
}

void UMingCloudAIComputingIntegration::ResumeProcessing()
{
    bProcessingPaused = false;
    ProcessTaskQueue();
    LogCloudAI(TEXT("Processing resumed"));
}

bool UMingCloudAIComputingIntegration::IsProcessingPaused() const
{
    return bProcessingPaused;
}

int32 UMingCloudAIComputingIntegration::GetActiveTaskCount() const
{
    return ActiveTaskCount;
}

int32 UMingCloudAIComputingIntegration::GetCompletedTaskCount() const
{
    return CompletedTaskCount;
}

int32 UMingCloudAIComputingIntegration::GetFailedTaskCount() const
{
    return FailedTaskCount;
}

float UMingCloudAIComputingIntegration::GetAverageProcessingTime() const
{
    return UsageStats.AverageResponseTimeMs;
}

void UMingCloudAIComputingIntegration::ProcessTaskQueue()
{
    if (bProcessingPaused || TaskQueue.Num() == 0)
    {
        return;
    }
    
    if (ActiveTaskCount >= Config.MaxConcurrentRequests)
    {
        return;
    }
    
    for (auto& Task : TaskQueue)
    {
        if (Task.Status == ECloudAITaskStatus::Queued)
        {
            ExecuteTask(Task.TaskID);
            break;
        }
    }
}

void UMingCloudAIComputingIntegration::ExecuteTask(const FString& TaskID)
{
    if (!PendingTasks.Contains(TaskID))
    {
        return;
    }
    
    FCloudAITaskRequest Request = PendingTasks[TaskID];
    
    for (auto& Task : TaskQueue)
    {
        if (Task.TaskID == TaskID)
        {
            Task.Status = ECloudAITaskStatus::Processing;
            Task.StartTimestamp = FDateTime::Now().ToUnixTimestamp();
            break;
        }
    }
    
    ActiveTaskCount++;
    
    FCloudAITaskResult Result;
    
    if (ShouldUseCache(Request))
    {
        FString CacheKey = GenerateCacheKey(Request);
        if (HasCachedResult(CacheKey))
        {
            Result = GetCachedResult(CacheKey);
            Result.TaskID = TaskID;
            CompleteTask(TaskID, Result);
            return;
        }
    }
    
    if (!Config.bEnableCloudProcessing || !bCloudAvailable)
    {
        Result = ProcessTaskLocally(Request);
    }
    else
    {
        Result = ProcessTaskInCloud(Request);
    }
    
    Result.TaskID = TaskID;
    
    if (Config.bEnableCaching && Result.Status == ECloudAITaskStatus::Completed)
    {
        CacheTaskResult(GenerateCacheKey(Request), Result);
    }
    
    if (Result.Status == ECloudAITaskStatus::Completed)
    {
        CompleteTask(TaskID, Result);
    }
    else
    {
        FailTask(TaskID, Result.ErrorMessage);
    }
}

void UMingCloudAIComputingIntegration::CompleteTask(const FString& TaskID, const FCloudAITaskResult& Result)
{
    CompletedTasks.Add(TaskID, Result);
    PendingTasks.Remove(TaskID);
    
    for (int32 i = TaskQueue.Num() - 1; i >= 0; --i)
    {
        if (TaskQueue[i].TaskID == TaskID)
        {
            TaskQueue.RemoveAt(i);
            break;
        }
    }
    
    ActiveTaskCount--;
    CompletedTaskCount++;
    
    OnTaskCompleted.Broadcast(Result);
    OnQueueUpdated.Broadcast(TaskQueue.Num());
    
    ProcessTaskQueue();
}

void UMingCloudAIComputingIntegration::FailTask(const FString& TaskID, const FString& Error)
{
    FCloudAITaskResult Result;
    Result.TaskID = TaskID;
    Result.Status = ECloudAITaskStatus::Failed;
    Result.ErrorMessage = Error;
    
    CompletedTasks.Add(TaskID, Result);
    PendingTasks.Remove(TaskID);
    
    for (int32 i = TaskQueue.Num() - 1; i >= 0; --i)
    {
        if (TaskQueue[i].TaskID == TaskID)
        {
            TaskQueue.RemoveAt(i);
            break;
        }
    }
    
    ActiveTaskCount--;
    FailedTaskCount++;
    
    OnTaskFailed.Broadcast(TaskID, Error);
    OnQueueUpdated.Broadcast(TaskQueue.Num());
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallCloudProvider(const FCloudAITaskRequest& Request, EAIProvider Provider)
{
    switch (Provider)
    {
    case EAIProvider::OpenAI:
        return CallOpenAI(Request);
    case EAIProvider::Anthropic:
        return CallAnthropic(Request);
    case EAIProvider::GoogleAI:
        return CallGoogleAI(Request);
    case EAIProvider::AzureML:
        return CallAzureML(Request);
    case EAIProvider::AWSBedrock:
        return CallAWSBedrock(Request);
    default:
        return CallLocalProcessing(Request);
    }
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallOpenAI(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("OpenAI");
    Result.ModelUsed = TEXT("gpt-4");
    Result.GeneratedContent = TEXT("Generated content from OpenAI");
    Result.ConfidenceScore = 0.95f;
    Result.TokensUsed = 150;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallAnthropic(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("Anthropic");
    Result.ModelUsed = TEXT("claude-3");
    Result.GeneratedContent = TEXT("Generated content from Anthropic");
    Result.ConfidenceScore = 0.93f;
    Result.TokensUsed = 200;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallGoogleAI(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("GoogleAI");
    Result.ModelUsed = TEXT("gemini-pro");
    Result.GeneratedContent = TEXT("Generated content from Google AI");
    Result.ConfidenceScore = 0.90f;
    Result.TokensUsed = 180;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallAzureML(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("AzureML");
    Result.ModelUsed = TEXT("azure-openai");
    Result.GeneratedContent = TEXT("Generated content from Azure ML");
    Result.ConfidenceScore = 0.92f;
    Result.TokensUsed = 160;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallAWSBedrock(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("AWSBedrock");
    Result.ModelUsed = TEXT("anthropic.claude");
    Result.GeneratedContent = TEXT("Generated content from AWS Bedrock");
    Result.ConfidenceScore = 0.91f;
    Result.TokensUsed = 170;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

FCloudAITaskResult UMingCloudAIComputingIntegration::CallLocalProcessing(const FCloudAITaskRequest& Request)
{
    FCloudAITaskResult Result;
    Result.Status = ECloudAITaskStatus::Completed;
    Result.ProviderUsed = TEXT("Local");
    Result.ModelUsed = TEXT("local-model");
    
    switch (Request.TaskType)
    {
    case ECloudAITaskType::ContentGeneration:
        Result.GeneratedContent = FString::Printf(TEXT("Locally generated content for: %s"), 
            *Request.InputParameters.FindRef(TEXT("prompt")));
        break;
    case ECloudAITaskType::BehaviorPrediction:
        Result.GeneratedContent = TEXT("{\"predicted_action\": \"continue_play\", \"confidence\": 0.85}");
        break;
    case ECloudAITaskType::StrategyOptimization:
        Result.GeneratedContent = TEXT("{\"recommended_strategy\": \"balanced\", \"priority_units\": [\"infantry\", \"archers\"]}");
        break;
    default:
        Result.GeneratedContent = TEXT("Local processing result");
    }
    
    Result.ConfidenceScore = 0.75f;
    Result.TokensUsed = 50;
    Result.CompletedTimestamp = FDateTime::Now().ToUnixTimestamp();
    return Result;
}

EAIProvider UMingCloudAIComputingIntegration::SelectBestProvider(const FCloudAITaskRequest& Request)
{
    float BestScore = -1.0f;
    EAIProvider BestProvider = EAIProvider::Local;
    
    for (const auto& Pair : ProviderConfigurations)
    {
        float Latency = GetProviderLatency(Pair.Key);
        float Score = 1.0f / (Latency + 1.0f);
        
        if (Score > BestScore)
        {
            BestScore = Score;
            BestProvider = Pair.Key;
        }
    }
    
    return BestProvider;
}

int32 UMingCloudAIComputingIntegration::CalculatePriorityScore(ECloudAITaskPriority Priority)
{
    switch (Priority)
    {
    case ECloudAITaskPriority::Critical:
        return 100;
    case ECloudAITaskPriority::High:
        return 75;
    case ECloudAITaskPriority::Normal:
        return 50;
    case ECloudAITaskPriority::Low:
        return 25;
    case ECloudAITaskPriority::Background:
        return 10;
    default:
        return 50;
    }
}

void UMingCloudAIComputingIntegration::UpdateUsageStats(const FCloudAITaskResult& Result)
{
    if (!Config.bEnableCostTracking)
    {
        return;
    }
    
    UsageStats.TotalRequestsToday++;
    UsageStats.TotalTokensUsed += Result.TokensUsed;
    
    if (Result.Status == ECloudAITaskStatus::Completed)
    {
        UsageStats.SuccessfulRequests++;
    }
    else
    {
        UsageStats.FailedRequests++;
    }
    
    float Cost = GetEstimatedCostForTask(ECloudAITaskType::ContentGeneration);
    UsageStats.TotalCostToday += Cost;
    
    if (UsageStats.RequestsByType.Contains(Result.Status == ECloudAITaskStatus::Completed ? ECloudAITaskType::ContentGeneration : ECloudAITaskType::BatchProcessing))
    {
        UsageStats.RequestsByType[Result.Status == ECloudAITaskStatus::Completed ? ECloudAITaskType::ContentGeneration : ECloudAITaskType::BatchProcessing]++;
    }
    else
    {
        UsageStats.RequestsByType.Add(Result.Status == ECloudAITaskStatus::Completed ? ECloudAITaskType::ContentGeneration : ECloudAITaskType::BatchProcessing, 1);
    }
    
    EAIProvider Provider = EAIProvider::Local;
    if (Result.ProviderUsed == TEXT("OpenAI"))
        Provider = EAIProvider::OpenAI;
    else if (Result.ProviderUsed == TEXT("Anthropic"))
        Provider = EAIProvider::Anthropic;
    
    if (UsageStats.RequestsByProvider.Contains(Provider))
    {
        UsageStats.RequestsByProvider[Provider]++;
    }
    else
    {
        UsageStats.RequestsByProvider.Add(Provider, 1);
    }
    
    float TotalTime = UsageStats.AverageResponseTimeMs * (UsageStats.TotalRequestsToday - 1) + Result.ProcessingTimeMs;
    UsageStats.AverageResponseTimeMs = TotalTime / UsageStats.TotalRequestsToday;
    
    CheckBudgetThreshold();
}

bool UMingCloudAIComputingIntegration::ShouldUseCache(const FCloudAITaskRequest& Request)
{
    return Config.bEnableCaching && Request.Priority != ECloudAITaskPriority::Critical;
}

FString UMingCloudAIComputingIntegration::GenerateCacheKey(const FCloudAITaskRequest& Request)
{
    FString Key = FString::Printf(TEXT("%d_%d_%s"), 
        static_cast<int32>(Request.TaskType),
        static_cast<int32>(Request.Priority),
        *Request.ContextData.Left(50));
    return FMD5::HashAnsiString(*Key);
}

void UMingCloudAIComputingIntegration::CheckBudgetThreshold()
{
    if (Config.DailyBudget > 0)
    {
        float BudgetUsedRatio = UsageStats.TotalCostToday / Config.DailyBudget;
        if (BudgetUsedRatio >= CostAlertThreshold)
        {
            OnBudgetThreshold.Broadcast();
            LogCloudAI(TEXT("Budget threshold reached"));
        }
    }
}

void UMingCloudAIComputingIntegration::CleanupOldTasks()
{
    const int32 MaxCompletedTasks = 100;
    
    while (CompletedTasks.Num() > MaxCompletedTasks)
    {
        auto It = CompletedTasks.CreateIterator();
        if (It)
        {
            It.RemoveCurrent();
        }
    }
}

void UMingCloudAIComputingIntegration::LogCloudAI(const FString& Message)
{
    UE_LOG(LogCloudAI, Log, TEXT("[CloudAI] %s"), *Message);
}

UMingCloudAIComputingIntegration* UMingCloudAIComputingIntegration::Get(UObject* WorldContextObject)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
    {
        static UMingCloudAIComputingIntegration* Instance = nullptr;
        if (!Instance)
        {
            Instance = NewObject<UMingCloudAIComputingIntegration>();
        }
        return Instance;
    }
    return nullptr;
}
