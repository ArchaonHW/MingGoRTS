#include "MingAITaskIntegration.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingAITaskIntegration::UMingAITaskIntegration()
    : TaskPersonalizationThreshold(0.6f)
    , PredictionConfidenceThreshold(0.5f)
    , MaxInsightHistory(10)
    , bEnableRealTimePrediction(true)
{
}

void UMingAITaskIntegration::InitializeAITaskIntegration()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing AI Task Integration System..."));
    
    // Create and initialize core AI systems
    TaskGenerator = NewObject<UMingDynamicTaskGenerator>();
    BehaviorPredictor = NewObject<UMingPlayerBehaviorPredictor>();
    
    if (TaskGenerator)
    {
        TaskGenerator->InitializeTaskGenerator();
    }
    
    if (BehaviorPredictor)
    {
        BehaviorPredictor->InitializePredictor();
    }
    
    // Initialize cache
    InsightCache.Empty();
    LastInsightUpdate.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("AI Task Integration System initialized successfully"));
}

FDynamicTask UMingAITaskIntegration::ProcessTaskGenerationRequest(const FTaskGenerationRequest& Request)
{
    if (!TaskGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("TaskGenerator is not initialized"));
        return FDynamicTask();
    }
    
    FDynamicTask GeneratedTask;
    
    // Check if we should personalize the task
    if (Request.bForcePersonalization || ShouldPersonalizeTask(Request.PlayerID))
    {
        // Get player behavior profile
        FPlayerBehaviorProfile Profile = CreateDefaultProfile(Request.PlayerID);
        
        // Generate personalized task
        GeneratedTask = TaskGenerator->GeneratePersonalizedTask(Request.PlayerID, Profile);
        
        UE_LOG(LogTemp, Log, TEXT("Generated personalized task %s for player %s"), 
               *GeneratedTask.TaskID, *Request.PlayerID);
    }
    else
    {
        // Generate contextual task
        ETaskType PreferredType = GetPreferredTaskType(Request.PlayerID);
        GeneratedTask = TaskGenerator->GenerateContextualTask(Request.Context, PreferredType);
        
        UE_LOG(LogTemp, Log, TEXT("Generated contextual task %s for player %s"), 
               *GeneratedTask.TaskID, *Request.PlayerID);
    }
    
    // Broadcast task generation event
    OnTaskGenerated.Broadcast(GeneratedTask);
    
    return GeneratedTask;
}

TArray<FPredictionResult> UMingAITaskIntegration::ProcessBehaviorAnalysisRequest(const FBehaviorAnalysisRequest& Request)
{
    if (!BehaviorPredictor)
    {
        UE_LOG(LogTemp, Error, TEXT("BehaviorPredictor is not initialized"));
        return TArray<FPredictionResult>();
    }
    
    TArray<FPredictionResult> Predictions;
    
    if (Request.bIncludePredictions)
    {
        Predictions = BehaviorPredictor->PredictActionsInTimeWindow(Request.PlayerID, Request.AnalysisWindowMinutes);
    }
    else
    {
        // Just get the next action prediction
        FPredictionResult NextPrediction = BehaviorPredictor->PredictNextAction(Request.PlayerID);
        Predictions.Add(NextPrediction);
    }
    
    // Broadcast prediction events
    for (const FPredictionResult& Prediction : Predictions)
    {
        OnBehaviorPredicted.Broadcast(Prediction);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d behavior predictions for player %s"), 
           Predictions.Num(), *Request.PlayerID);
    
    return Predictions;
}

FTaskBehaviorInsight UMingAITaskIntegration::GetPlayerInsight(const FString& PlayerID)
{
    // Check cache first
    FDateTime CurrentTime = FDateTime::Now();
    if (LastInsightUpdate.Contains(PlayerID))
    {
        FDateTime LastUpdate = LastInsightUpdate[PlayerID];
        if ((CurrentTime - LastUpdate).GetTotalMinutes() < 5.0f) // Cache for 5 minutes
        {
            return GetCachedInsight(PlayerID);
        }
    }
    
    FTaskBehaviorInsight Insight;
    Insight.PlayerID = PlayerID;
    
    // Get task recommendations
    Insight.RecommendedTasks = GetAdaptiveTaskRecommendations(PlayerID, 3);
    
    // Get behavior predictions
    Insight.BehaviorPredictions = GetRealTimePredictions(PlayerID, 10.0f);
    
    // Get content preload requests
    Insight.PreloadRequests = GetContentPreloadRecommendations(PlayerID);
    
    // Calculate engagement score
    Insight.OverallEngagementScore = CalculateEngagementScore(PlayerID);
    
    // Generate insight summary
    Insight.InsightSummary = GenerateInsightSummary(Insight);
    
    // Cache the insight
    CacheInsight(PlayerID, Insight);
    
    // Broadcast insight generation
    OnInsightGenerated.Broadcast(Insight);
    
    UE_LOG(LogTemp, Log, TEXT("Generated player insight for %s: %s"), *PlayerID, *Insight.InsightSummary);
    
    return Insight;
}

void UMingAITaskIntegration::RecordPlayerAction(const FString& PlayerID, const FPlayerAction& Action)
{
    if (!BehaviorPredictor)
    {
        UE_LOG(LogTemp, Error, TEXT("BehaviorPredictor is not initialized"));
        return;
    }
    
    // Record the action
    BehaviorPredictor->RecordPlayerAction(PlayerID, Action);
    
    // Process real-time updates if enabled
    if (bEnableRealTimePrediction)
    {
        ProcessRealTimeUpdates(PlayerID);
    }
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Recorded action %d for player %s"), (int32)Action.ActionType, *PlayerID);
}

TArray<FDynamicTask> UMingAITaskIntegration::GetAdaptiveTaskRecommendations(const FString& PlayerID, int32 MaxTasks)
{
    if (!TaskGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("TaskGenerator is not initialized"));
        return TArray<FDynamicTask>();
    }
    
    // Get player behavior profile
    FPlayerBehaviorProfile Profile = CreateDefaultProfile(PlayerID);
    
    // Update profile with current data
    UpdatePlayerBehaviorProfile(PlayerID, Profile);
    
    // Get recommended tasks
    TArray<FDynamicTask> RecommendedTasks = TaskGenerator->GetRecommendedTasks(PlayerID, MaxTasks);
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d adaptive task recommendations for player %s"), 
           RecommendedTasks.Num(), *PlayerID);
    
    return RecommendedTasks;
}

void UMingAITaskIntegration::UpdatePlayerBehaviorProfile(const FString& PlayerID, const FPlayerBehaviorProfile& Profile)
{
    if (!TaskGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("TaskGenerator is not initialized"));
        return;
    }
    
    TaskGenerator->UpdatePlayerProfile(PlayerID, Profile);
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Updated behavior profile for player %s"), *PlayerID);
}

TArray<FContentPreloadRequest> UMingAITaskIntegration::GetContentPreloadRecommendations(const FString& PlayerID)
{
    if (!BehaviorPredictor)
    {
        UE_LOG(LogTemp, Error, TEXT("BehaviorPredictor is not initialized"));
        return TArray<FContentPreloadRequest>();
    }
    
    TArray<FContentPreloadRequest> PreloadRequests = BehaviorPredictor->GetContentPreloadRequests(PlayerID);
    
    // Filter by confidence threshold
    TArray<FContentPreloadRequest> FilteredRequests;
    for (const FContentPreloadRequest& Request : PreloadRequests)
    {
        if (Request.Priority >= PredictionConfidenceThreshold)
        {
            FilteredRequests.Add(Request);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d content preload recommendations for player %s"), 
           FilteredRequests.Num(), *PlayerID);
    
    return FilteredRequests;
}

void UMingAITaskIntegration::AnalyzeTaskCompletionImpact(const FString& PlayerID, const FDynamicTask& CompletedTask, float CompletionTime)
{
    if (!TaskGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("TaskGenerator is not initialized"));
        return;
    }
    
    // Analyze task completion
    TaskGenerator->AnalyzeTaskCompletion(PlayerID, CompletedTask, CompletionTime);
    
    // Update behavior predictor with task completion action
    FPlayerAction TaskCompletionAction;
    TaskCompletionAction.ActionType = EPlayerActionType::Idle; // Placeholder
    TaskCompletionAction.ActionData = FString::Printf(TEXT("CompletedTask:%s"), *CompletedTask.TaskID);
    TaskCompletionAction.Timestamp = FDateTime::Now();
    TaskCompletionAction.Duration = CompletionTime;
    
    RecordPlayerAction(PlayerID, TaskCompletionAction);
    
    UE_LOG(LogTemp, Log, TEXT("Analyzed task completion impact for player %s"), *PlayerID);
}

TArray<FPredictionResult> UMingAITaskIntegration::GetRealTimePredictions(const FString& PlayerID, float TimeWindowMinutes)
{
    if (!BehaviorPredictor)
    {
        UE_LOG(LogTemp, Error, TEXT("BehaviorPredictor is not initialized"));
        return TArray<FPredictionResult>();
    }
    
    TArray<FPredictionResult> Predictions = BehaviorPredictor->PredictActionsInTimeWindow(PlayerID, TimeWindowMinutes);
    
    // Filter by confidence threshold
    TArray<FPredictionResult> FilteredPredictions;
    for (const FPredictionResult& Prediction : Predictions)
    {
        if (Prediction.Probability >= PredictionConfidenceThreshold)
        {
            FilteredPredictions.Add(Prediction);
        }
    }
    
    return FilteredPredictions;
}

FPlayerBehaviorProfile UMingAITaskIntegration::CreateDefaultProfile(const FString& PlayerID)
{
    FPlayerBehaviorProfile Profile;
    
    // Initialize with default values
    Profile.CombatPreference = 0.5f;
    Profile.ExplorationPreference = 0.5f;
    Profile.ConstructionPreference = 0.5f;
    Profile.DiplomacyPreference = 0.5f;
    Profile.ResourceManagementPreference = 0.5f;
    Profile.StoryPreference = 0.5f;
    Profile.PlayerSkillLevel = 10; // Default skill level
    Profile.AverageCompletionTime = 300.0f; // 5 minutes default
    
    return Profile;
}

float UMingAITaskIntegration::CalculateEngagementScore(const FString& PlayerID)
{
    if (!BehaviorPredictor)
    {
        return 0.5f; // Default engagement score
    }
    
    // Get session summary
    FString SessionSummary = BehaviorPredictor->GetSessionSummary(PlayerID);
    
    // Calculate engagement based on action diversity and frequency
    float EngagementScore = 0.5f; // Base score
    
    // Add points for different types of actions
    if (SessionSummary.Contains(TEXT("Combat")))
    {
        EngagementScore += 0.1f;
    }
    if (SessionSummary.Contains(TEXT("Build")))
    {
        EngagementScore += 0.1f;
    }
    if (SessionSummary.Contains(TEXT("Explore")))
    {
        EngagementScore += 0.1f;
    }
    if (SessionSummary.Contains(TEXT("Diplomacy")))
    {
        EngagementScore += 0.15f;
    }
    
    // Cap the score
    EngagementScore = FMath::Min(EngagementScore, 1.0f);
    
    return EngagementScore;
}

FString UMingAITaskIntegration::GenerateInsightSummary(const FTaskBehaviorInsight& Insight)
{
    FString Summary = FString::Printf(TEXT("Player %s Analysis:\n"), *Insight.PlayerID);
    Summary += FString::Printf(TEXT("Engagement Score: %.2f\n"), Insight.OverallEngagementScore);
    Summary += FString::Printf(TEXT("Recommended Tasks: %d\n"), Insight.RecommendedTasks.Num());
    Summary += FString::Printf(TEXT("Behavior Predictions: %d\n"), Insight.BehaviorPredictions.Num());
    Summary += FString::Printf(TEXT("Preload Requests: %d\n"), Insight.PreloadRequests.Num());
    
    if (Insight.BehaviorPredictions.Num() > 0)
    {
        Summary += FString::Printf(TEXT("Next Likely Action: %s (Confidence: %.2f)\n"), 
                                *GetActionName(Insight.BehaviorPredictions[0].PredictedAction),
                                Insight.BehaviorPredictions[0].Probability);
    }
    
    return Summary;
}

void UMingAITaskIntegration::ProcessRealTimeUpdates(const FString& PlayerID)
{
    // Get current prediction
    if (BehaviorPredictor)
    {
        FPredictionResult CurrentPrediction = BehaviorPredictor->PredictNextAction(PlayerID);
        
        // Broadcast if confidence is high enough
        if (CurrentPrediction.Confidence >= EPredictionConfidence::Medium)
        {
            OnBehaviorPredicted.Broadcast(CurrentPrediction);
        }
    }
}

void UMingAITaskIntegration::CacheInsight(const FString& PlayerID, const FTaskBehaviorInsight& Insight)
{
    InsightCache.Add(PlayerID, Insight);
    LastInsightUpdate.Add(PlayerID, FDateTime::Now());
    
    // Maintain cache size
    if (InsightCache.Num() > MaxInsightHistory)
    {
        // Remove oldest entry
        FString OldestPlayerID;
        FDateTime OldestTime = FDateTime::MaxValue();
        
        for (const auto& CacheEntry : LastInsightUpdate)
        {
            if (CacheEntry.Value < OldestTime)
            {
                OldestTime = CacheEntry.Value;
                OldestPlayerID = CacheEntry.Key;
            }
        }
        
        if (!OldestPlayerID.IsEmpty())
        {
            InsightCache.Remove(OldestPlayerID);
            LastInsightUpdate.Remove(OldestPlayerID);
        }
    }
}

FTaskBehaviorInsight UMingAITaskIntegration::GetCachedInsight(const FString& PlayerID)
{
    if (InsightCache.Contains(PlayerID))
    {
        return InsightCache[PlayerID];
    }
    
    // Return empty insight if not found
    FTaskBehaviorInsight EmptyInsight;
    EmptyInsight.PlayerID = PlayerID;
    return EmptyInsight;
}

bool UMingAITaskIntegration::ShouldPersonalizeTask(const FString& PlayerID)
{
    if (!BehaviorPredictor)
    {
        return false;
    }
    
    // Get prediction confidence
    FPredictionResult Prediction = BehaviorPredictor->PredictNextAction(PlayerID);
    
    // Personalize if we have enough data (confidence above threshold)
    return Prediction.Probability >= TaskPersonalizationThreshold;
}

ETaskType UMingAITaskIntegration::GetPreferredTaskType(const FString& PlayerID)
{
    if (!BehaviorPredictor)
    {
        return ETaskType::Combat; // Default
    }
    
    // Get next action prediction
    FPredictionResult Prediction = BehaviorPredictor->PredictNextAction(PlayerID);
    
    // Convert action type to task type
    switch (Prediction.PredictedAction)
    {
        case EPlayerActionType::Attack:
            return ETaskType::Combat;
        case EPlayerActionType::MoveUnit:
            return ETaskType::Exploration;
        case EPlayerActionType::Build:
            return ETaskType::Construction;
        case EPlayerActionType::GatherResources:
            return ETaskType::Resource;
        case EPlayerActionType::Trade:
        case EPlayerActionType::Diplomacy:
            return ETaskType::Diplomacy;
        default:
            return ETaskType::Combat;
    }
}

FString UMingAITaskIntegration::GetActionName(EPlayerActionType ActionType)
{
    switch (ActionType)
    {
        case EPlayerActionType::MoveUnit:
            return TEXT("Move Unit");
        case EPlayerActionType::Attack:
            return TEXT("Attack");
        case EPlayerActionType::Build:
            return TEXT("Build");
        case EPlayerActionType::GatherResources:
            return TEXT("Gather Resources");
        case EPlayerActionType::Research:
            return TEXT("Research");
        case EPlayerActionType::Trade:
            return TEXT("Trade");
        case EPlayerActionType::Diplomacy:
            return TEXT("Diplomacy");
        case EPlayerActionType::SaveGame:
            return TEXT("Save Game");
        case EPlayerActionType::LoadGame:
            return TEXT("Load Game");
        case EPlayerActionType::Idle:
        default:
            return TEXT("Idle");
    }
}
