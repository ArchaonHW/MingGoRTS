#include "MingPlayerBehaviorPredictor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

UMingPlayerBehaviorPredictor::UMingPlayerBehaviorPredictor()
    : MaxActionHistorySize(1000)
    , PatternRecognitionThreshold(0.7f)
    , PredictionConfidenceThreshold(0.6f)
    , PreloadTimeWindowMinutes(5.0f)
{
}

void UMingPlayerBehaviorPredictor::InitializePredictor()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Player Behavior Predictor..."));
    
    // Initialize internal data structures
    PlayerActionHistory.Empty();
    PlayerBehaviorPatterns.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Player Behavior Predictor initialized"));
}

void UMingPlayerBehaviorPredictor::RecordPlayerAction(const FString& PlayerID, const FPlayerAction& Action)
{
    // Add action to player history
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        PlayerActionHistory.Add(PlayerID, TArray<FPlayerAction>());
    }
    
    TArray<FPlayerAction>& ActionHistory = PlayerActionHistory[PlayerID];
    ActionHistory.Add(Action);
    
    // Maintain history size limit
    if (ActionHistory.Num() > MaxActionHistorySize)
    {
        ActionHistory.RemoveAt(0);
    }
    
    // Clean up old actions periodically
    CleanupOldActions(PlayerID);
    
    // Update behavior patterns
    UpdatePredictionModel(PlayerID);
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Recorded action %d for player %s"), (int32)Action.ActionType, *PlayerID);
}

FPredictionResult UMingPlayerBehaviorPredictor::PredictNextAction(const FString& PlayerID)
{
    FPredictionResult Prediction;
    
    if (!PlayerActionHistory.Contains(PlayerID) || !PlayerBehaviorPatterns.Contains(PlayerID))
    {
        Prediction.PredictedAction = EPlayerActionType::Idle;
        Prediction.Confidence = EPredictionConfidence::VeryLow;
        Prediction.Probability = 0.0f;
        Prediction.PredictedTime = FDateTime::Now();
        return Prediction;
    }
    
    const TArray<FPlayerAction>& ActionHistory = PlayerActionHistory[PlayerID];
    const TArray<FBehaviorPattern>& Patterns = PlayerBehaviorPatterns[PlayerID];
    
    if (ActionHistory.Num() < 3 || Patterns.Num() == 0)
    {
        Prediction.PredictedAction = EPlayerActionType::Idle;
        Prediction.Confidence = EPredictionConfidence::Low;
        Prediction.Probability = 0.2f;
        Prediction.PredictedTime = FDateTime::Now();
        return Prediction;
    }
    
    // Find the most likely pattern
    float MaxProbability = 0.0f;
    EPlayerActionType MostLikelyAction = EPlayerActionType::Idle;
    
    for (const FBehaviorPattern& Pattern : Patterns)
    {
        if (Pattern.PatternStrength < PatternRecognitionThreshold)
        {
            continue;
        }
        
        // Check if recent actions match this pattern
        if (ActionHistory.Num() >= Pattern.ActionSequence.Num())
        {
            TArray<EPlayerActionType> RecentActions;
            for (int32 i = ActionHistory.Num() - Pattern.ActionSequence.Num() + 1; i < ActionHistory.Num(); ++i)
            {
                RecentActions.Add(ActionHistory[i].ActionType);
            }
            
            float Similarity = CalculatePatternSimilarity(Pattern.ActionSequence, RecentActions);
            if (Similarity > MaxProbability)
            {
                MaxProbability = Similarity;
                // Predict the next action in the pattern
                if (Pattern.ActionSequence.Num() > 0)
                {
                    MostLikelyAction = Pattern.ActionSequence[0]; // Simplified prediction
                }
            }
        }
    }
    
    Prediction.PredictedAction = MostLikelyAction;
    Prediction.Confidence = CalculateConfidence(MaxProbability);
    Prediction.Probability = MaxProbability;
    Prediction.PredictedTime = FDateTime::Now() + FTimespan::FromSeconds(Patterns[0].AverageInterval);
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Predicted action %d for player %s with confidence %d"), 
           (int32)Prediction.PredictedAction, *PlayerID, (int32)Prediction.Confidence);
    
    return Prediction;
}

TArray<FPredictionResult> UMingPlayerBehaviorPredictor::PredictActionsInTimeWindow(const FString& PlayerID, float TimeWindowMinutes)
{
    TArray<FPredictionResult> Predictions;
    
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        return Predictions;
    }
    
    // Generate multiple predictions for the time window
    FDateTime CurrentTime = FDateTime::Now();
    FDateTime EndTime = CurrentTime + FTimespan::FromMinutes(TimeWindowMinutes);
    
    // Predict at regular intervals
    float IntervalMinutes = TimeWindowMinutes / 5.0f; // 5 predictions in the window
    
    for (int32 i = 0; i < 5; ++i)
    {
        FPredictionResult Prediction = PredictNextAction(PlayerID);
        Prediction.PredictedTime = CurrentTime + FTimespan::FromMinutes(IntervalMinutes * i);
        Predictions.Add(Prediction);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d predictions for player %s in %.1f minute window"), 
           Predictions.Num(), *PlayerID, TimeWindowMinutes);
    
    return Predictions;
}

TArray<FBehaviorPattern> UMingPlayerBehaviorPredictor::AnalyzeBehaviorPatterns(const FString& PlayerID)
{
    TArray<FBehaviorPattern> Patterns;
    
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        return Patterns;
    }
    
    const TArray<FPlayerAction>& ActionHistory = PlayerActionHistory[PlayerID];
    
    if (ActionHistory.Num() < 5)
    {
        return Patterns;
    }
    
    // Extract patterns of different lengths
    for (int32 PatternLength = 2; PatternLength <= 5; ++PatternLength)
    {
        TMap<TArray<EPlayerActionType>, int32> PatternCounts;
        
        // Count occurrences of each pattern
        for (int32 i = 0; i <= ActionHistory.Num() - PatternLength; ++i)
        {
            FBehaviorPattern Pattern = ExtractPattern(ActionHistory, i, PatternLength);
            
            TArray<EPlayerActionType> PatternKey = Pattern.ActionSequence;
            if (PatternCounts.Contains(PatternKey))
            {
                PatternCounts[PatternKey]++;
            }
            else
            {
                PatternCounts.Add(PatternKey, 1);
            }
        }
        
        // Create pattern objects for significant patterns
        for (const auto& PatternCount : PatternCounts)
        {
            if (PatternCount.Value >= 2) // Pattern must appear at least twice
            {
                FBehaviorPattern SignificantPattern;
                SignificantPattern.ActionSequence = PatternCount.Key;
                SignificantPattern.Frequency = (float)PatternCount.Value / (ActionHistory.Num() - PatternLength + 1);
                
                // Calculate average interval
                float TotalInterval = 0.0f;
                int32 IntervalCount = 0;
                
                for (int32 i = 0; i <= ActionHistory.Num() - PatternLength; ++i)
                {
                    bool Matches = true;
                    for (int32 j = 0; j < PatternLength; ++j)
                    {
                        if (ActionHistory[i + j].ActionType != SignificantPattern.ActionSequence[j])
                        {
                            Matches = false;
                            break;
                        }
                    }
                    
                    if (Matches)
                    {
                        if (i > 0)
                        {
                            TotalInterval += (ActionHistory[i].Timestamp - ActionHistory[i - 1].Timestamp).GetTotalSeconds();
                            IntervalCount++;
                        }
                    }
                }
                
                SignificantPattern.AverageInterval = IntervalCount > 0 ? TotalInterval / IntervalCount : 60.0f;
                SignificantPattern.PatternStrength = SignificantPattern.Frequency * PatternCount.Value;
                
                Patterns.Add(SignificantPattern);
            }
        }
    }
    
    // Sort patterns by strength
    Patterns.Sort([](const FBehaviorPattern& A, const FBehaviorPattern& B)
    {
        return A.PatternStrength > B.PatternStrength;
    });
    
    UE_LOG(LogTemp, Log, TEXT("Analyzed %d behavior patterns for player %s"), Patterns.Num(), *PlayerID);
    
    return Patterns;
}

TArray<FContentPreloadRequest> UMingPlayerBehaviorPredictor::GetContentPreloadRequests(const FString& PlayerID)
{
    TArray<FContentPreloadRequest> PreloadRequests;
    
    // Get predictions for the next few minutes
    TArray<FPredictionResult> Predictions = PredictActionsInTimeWindow(PlayerID, PreloadTimeWindowMinutes);
    
    for (const FPredictionResult& Prediction : Predictions)
    {
        if (Prediction.Confidence >= EPredictionConfidence::Medium)
        {
            FContentPreloadRequest Request;
            Request.ContentType = GetContentTypeForAction(Prediction.PredictedAction);
            Request.ContentID = GenerateContentID(Prediction.PredictedAction);
            Request.Priority = (float)Prediction.Confidence / 4.0f; // Normalize to 0-1
            Request.RequestTime = FDateTime::Now();
            
            PreloadRequests.Add(Request);
        }
    }
    
    // Sort by priority
    PreloadRequests.Sort([](const FContentPreloadRequest& A, const FContentPreloadRequest& B)
    {
        return A.Priority > B.Priority;
    });
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d content preload requests for player %s"), 
           PreloadRequests.Num(), *PlayerID);
    
    return PreloadRequests;
}

void UMingPlayerBehaviorPredictor::UpdatePredictionModel(const FString& PlayerID)
{
    // Analyze current behavior patterns
    TArray<FBehaviorPattern> Patterns = AnalyzeBehaviorPatterns(PlayerID);
    
    // Update stored patterns
    PlayerBehaviorPatterns.Add(PlayerID, Patterns);
    
    // Save player data
    SavePlayerData(PlayerID);
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Updated prediction model for player %s"), *PlayerID);
}

FString UMingPlayerBehaviorPredictor::GetSessionSummary(const FString& PlayerID)
{
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        return TEXT("No session data available");
    }
    
    const TArray<FPlayerAction>& ActionHistory = PlayerActionHistory[PlayerID];
    
    // Count action types
    TMap<EPlayerActionType, int32> ActionCounts;
    for (const FPlayerAction& Action : ActionHistory)
    {
        if (ActionCounts.Contains(Action.ActionType))
        {
            ActionCounts[Action.ActionType]++;
        }
        else
        {
            ActionCounts.Add(Action.ActionType, 1);
        }
    }
    
    // Create summary string
    FString Summary = FString::Printf(TEXT("Session Summary for %s:\n"), *PlayerID);
    Summary += FString::Printf(TEXT("Total Actions: %d\n"), ActionHistory.Num());
    
    for (const auto& ActionCount : ActionCounts)
    {
        FString ActionName = GetActionName(ActionCount.Key);
        Summary += FString::Printf(TEXT("%s: %d\n"), *ActionName, ActionCount.Value);
    }
    
    if (PlayerBehaviorPatterns.Contains(PlayerID))
    {
        const TArray<FBehaviorPattern>& Patterns = PlayerBehaviorPatterns[PlayerID];
        Summary += FString::Printf(TEXT("Behavior Patterns: %d\n"), Patterns.Num());
    }
    
    return Summary;
}

FBehaviorPattern UMingPlayerBehaviorPredictor::ExtractPattern(const TArray<FPlayerAction>& Actions, int32 StartIndex, int32 PatternLength)
{
    FBehaviorPattern Pattern;
    
    for (int32 i = 0; i < PatternLength && (StartIndex + i) < Actions.Num(); ++i)
    {
        Pattern.ActionSequence.Add(Actions[StartIndex + i].ActionType);
    }
    
    return Pattern;
}

float UMingPlayerBehaviorPredictor::CalculatePatternSimilarity(const TArray<EPlayerActionType>& Pattern1, const TArray<EPlayerActionType>& Pattern2)
{
    if (Pattern1.Num() != Pattern2.Num())
    {
        return 0.0f;
    }
    
    int32 MatchingActions = 0;
    for (int32 i = 0; i < Pattern1.Num(); ++i)
    {
        if (Pattern1[i] == Pattern2[i])
        {
            MatchingActions++;
        }
    }
    
    return (float)MatchingActions / Pattern1.Num();
}

EPredictionConfidence UMingPlayerBehaviorPredictor::CalculateConfidence(float Probability)
{
    if (Probability >= 0.9f)
    {
        return EPredictionConfidence::VeryHigh;
    }
    else if (Probability >= 0.7f)
    {
        return EPredictionConfidence::High;
    }
    else if (Probability >= 0.5f)
    {
        return EPredictionConfidence::Medium;
    }
    else if (Probability >= 0.3f)
    {
        return EPredictionConfidence::Low;
    }
    else
    {
        return EPredictionConfidence::VeryLow;
    }
}

void UMingPlayerBehaviorPredictor::CleanupOldActions(const FString& PlayerID)
{
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        return;
    }
    
    TArray<FPlayerAction>& ActionHistory = PlayerActionHistory[PlayerID];
    FDateTime CutoffTime = FDateTime::Now() - FTimespan::FromDays(7); // Keep 7 days of history
    
    // Remove old actions
    for (int32 i = ActionHistory.Num() - 1; i >= 0; --i)
    {
        if (ActionHistory[i].Timestamp < CutoffTime)
        {
            ActionHistory.RemoveAt(i);
        }
    }
}

void UMingPlayerBehaviorPredictor::SavePlayerData(const FString& PlayerID)
{
    // In a real implementation, this would save to a file or database
    // For now, we'll just log the save action
    UE_LOG(LogTemp, VeryVerbose, TEXT("Saved behavior data for player %s"), *PlayerID);
}

void UMingPlayerBehaviorPredictor::LoadPlayerData(const FString& PlayerID)
{
    // In a real implementation, this would load from a file or database
    // For now, we'll just log the load action
    UE_LOG(LogTemp, VeryVerbose, TEXT("Loaded behavior data for player %s"), *PlayerID);
}

FString UMingPlayerBehaviorPredictor::GetContentTypeForAction(EPlayerActionType ActionType)
{
    switch (ActionType)
    {
        case EPlayerActionType::MoveUnit:
            return TEXT("UnitMovement");
        case EPlayerActionType::Attack:
            return TEXT("CombatAssets");
        case EPlayerActionType::Build:
            return TEXT("ConstructionAssets");
        case EPlayerActionType::GatherResources:
            return TEXT("ResourceAssets");
        case EPlayerActionType::Research:
            return TEXT("ResearchAssets");
        case EPlayerActionType::Trade:
            return TEXT("TradeAssets");
        case EPlayerActionType::Diplomacy:
            return TEXT("DiplomacyAssets");
        case EPlayerActionType::SaveGame:
            return TEXT("SaveGameAssets");
        case EPlayerActionType::LoadGame:
            return TEXT("LoadGameAssets");
        case EPlayerActionType::Idle:
        default:
            return TEXT("GeneralAssets");
    }
}

FString UMingPlayerBehaviorPredictor::GenerateContentID(EPlayerActionType ActionType)
{
    return FString::Printf(TEXT("%s_%lld"), *GetContentTypeForAction(ActionType), FDateTime::Now().GetTicks());
}

FString UMingPlayerBehaviorPredictor::GetActionName(EPlayerActionType ActionType)
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
