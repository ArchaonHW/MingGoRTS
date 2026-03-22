#include "AI/MingRTSPredictiveAI.h"
#include "Math/UnrealMathUtility.h"

UMingRTSPredictiveAI::UMingRTSPredictiveAI()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
    , TotalPredictions(0)
    , AccumulatedConfidence(0.0f)
{
}

void UMingRTSPredictiveAI::InitializePredictiveAI(const FPredictionModelConfig& Config)
{
    if (bIsInitialized)
    {
        return;
    }
    
    ModelConfig = Config;
    CurrentGameTime = 0.0f;
    TotalPredictions = 0;
    AccumulatedConfidence = 0.0f;
    
    // Initialize accuracy tracking
    for (int32 i = 0; i < static_cast<int32>(EPredictionType::DifficultyAdaptation) + 1; ++i)
    {
        ModelAccuracy.Add(static_cast<EPredictionType>(i), 0.5f);
    }
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPredictiveAI: Initialized with ML: %s"),
        ModelConfig.bUseMachineLearning ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMingRTSPredictiveAI::ShutdownPredictiveAI()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    PlayerBehaviorHistory.Empty();
    TrendData.Empty();
    ActiveTrends.Empty();
    ChurnAssessments.Empty();
    GamePredictions.Empty();
    ActivePredictions.Empty();
    PlayerActionHistory.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPredictiveAI: Shutdown"));
}

void UMingRTSPredictiveAI::RecordPlayerAction(const FString& PlayerID, const FString& Action, float Value)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    float Timestamp = CurrentGameTime;
    
    // Update action history
    if (!PlayerActionHistory.Contains(PlayerID))
    {
        PlayerActionHistory.Add(PlayerID, TMap<FString, float>());
    }
    
    PlayerActionHistory[PlayerID].Add(Action, Value);
    
    // Update behavior data
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        FPlayerBehaviorData NewData;
        NewData.PlayerID = PlayerID;
        NewData.Timestamp = Timestamp;
        PlayerBehaviorHistory.Add(PlayerID, NewData);
    }
    
    FPlayerBehaviorData& BehaviorData = PlayerBehaviorHistory[PlayerID];
    BehaviorData.Timestamp = Timestamp;
    BehaviorData.ActionHistory.Add(Action, Value);
    
    // Update specific scores based on action
    if (Action.Contains(TEXT("Attack")) || Action.Contains(TEXT("Combat")))
    {
        BehaviorData.AggressionScore = FMath::Min(1.0f, BehaviorData.AggressionScore + Value * 0.1f);
    }
    else if (Action.Contains(TEXT("Gather")) || Action.Contains(TEXT("Resource")) || Action.Contains(TEXT("Economy")))
    {
        BehaviorData.EconomyScore = FMath::Min(1.0f, BehaviorData.EconomyScore + Value * 0.1f);
    }
    else if (Action.Contains(TEXT("Build")) || Action.Contains(TEXT("Defend")))
    {
        BehaviorData.DefenseScore = FMath::Min(1.0f, BehaviorData.DefenseScore + Value * 0.1f);
    }
    else if (Action.Contains(TEXT("Explore")) || Action.Contains(TEXT("Scout")))
    {
        BehaviorData.ExplorationScore = FMath::Min(1.0f, BehaviorData.ExplorationScore + Value * 0.1f);
    }
    
    // Calculate APM
    static TMap<FString, int32> ActionCounts;
    static TMap<FString, float> LastResetTime;
    
    if (!LastResetTime.Contains(PlayerID) || CurrentGameTime - LastResetTime[PlayerID] > 60.0f)
    {
        ActionCounts.Add(PlayerID, 0);
        LastResetTime.Add(PlayerID, CurrentGameTime);
    }
    
    ActionCounts[PlayerID]++;
    BehaviorData.APM = ActionCounts[PlayerID] / (CurrentGameTime - LastResetTime[PlayerID]) * 60.0f;
    
    UE_LOG(LogTemp, Verbose, TEXT("MingRTSPredictiveAI: Recorded action %s for player %s"), *Action, *PlayerID);
}

EPlayerBehaviorPattern UMingRTSPredictiveAI::DetectBehaviorPattern(const FString& PlayerID)
{
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        return EPlayerBehaviorPattern::Unknown;
    }
    
    const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
    
    // Use pattern detection algorithm
    EPlayerBehaviorPattern DetectedPattern = DetectPatternFromActions(Data.ActionHistory);
    
    // Update stored pattern
    if (DetectedPattern != EPlayerBehaviorPattern::Unknown)
    {
        FPlayerBehaviorData& MutableData = PlayerBehaviorHistory[PlayerID];
        MutableData.CurrentPattern = DetectedPattern;
        
        OnBehaviorPatternDetected.Broadcast(PlayerID, DetectedPattern);
    }
    
    return DetectedPattern;
}

FPlayerBehaviorData UMingRTSPredictiveAI::GetPlayerBehaviorData(const FString& PlayerID) const
{
    if (PlayerBehaviorHistory.Contains(PlayerID))
    {
        return PlayerBehaviorHistory[PlayerID];
    }
    return FPlayerBehaviorData();
}

TArray<EPredictionType> UMingRTSPredictiveAI::PredictNextActions(const FString& PlayerID, int32 NumPredictions)
{
    TArray<EPredictionType> Predictions;
    
    if (!PlayerBehaviorHistory.Contains(PlayerID) || NumPredictions <= 0)
    {
        return Predictions;
    }
    
    const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
    
    // Generate predictions based on current pattern
    switch (Data.CurrentPattern)
    {
    case EPlayerBehaviorPattern::AggressiveRush:
        Predictions.Add(EPredictionType::BattleResult);
        Predictions.Add(EPredictionType::StrategicMove);
        break;
        
    case EPlayerBehaviorPattern::EconomicFocus:
        Predictions.Add(EPredictionType::ResourceTrend);
        Predictions.Add(EPredictionType::EconomicTrend);
        break;
        
    case EPlayerBehaviorPattern::DefensiveTurtle:
        Predictions.Add(EPredictionType::BattleResult);
        break;
        
    case EPlayerBehaviorPattern::ExplorationFocus:
        Predictions.Add(EPredictionType::StrategicMove);
        break;
        
    default:
        // Balanced predictions
        Predictions.Add(EPredictionType::PlayerBehavior);
        Predictions.Add(EPredictionType::GameOutcome);
        break;
    }
    
    // Fill remaining slots
    while (Predictions.Num() < NumPredictions)
    {
        Predictions.Add(EPredictionType::PlayerBehavior);
    }
    
    return Predictions;
}

float UMingRTSPredictiveAI::PredictPlayerPerformance(const FString& PlayerID, float TimeHorizon)
{
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        return 0.5f;
    }
    
    const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
    
    // Calculate performance based on multiple factors
    float Performance = 0.0f;
    
    // APM factor (normalize to 0-1, assuming 300 APM is high)
    float APMFactor = FMath::Clamp(Data.APM / 300.0f, 0.0f, 1.0f) * 0.2f;
    
    // Balance of scores
    float BalanceFactor = (Data.AggressionScore + Data.EconomyScore + Data.DefenseScore + Data.ExplorationScore) / 4.0f * 0.3f;
    
    // Consistency factor (based on pattern stability)
    float ConsistencyFactor = 0.5f; // Default
    
    Performance = APMFactor + BalanceFactor + ConsistencyFactor;
    
    // Adjust for time horizon (longer horizon = more uncertainty)
    float UncertaintyFactor = FMath::Clamp(TimeHorizon / 600.0f, 0.0f, 0.3f);
    Performance *= (1.0f - UncertaintyFactor);
    
    return FMath::Clamp(Performance, 0.0f, 1.0f);
}

FGameOutcomePrediction UMingRTSPredictiveAI::PredictGameOutcome(const FString& MatchID, const TArray<FString>& PlayerIDs)
{
    FGameOutcomePrediction Prediction;
    Prediction.MatchID = MatchID;
    Prediction.PlayerWinProbabilities.Empty();
    
    if (PlayerIDs.Num() == 0)
    {
        return Prediction;
    }
    
    // Calculate win probability for each player
    for (const FString& PlayerID : PlayerIDs)
    {
        float WinProb = CalculateWinProbability(PlayerID, MatchID);
        Prediction.PlayerWinProbabilities.Add(PlayerID, WinProb);
    }
    
    // Find predicted winner
    FString BestPlayer;
    float BestProbability = -1.0f;
    
    for (const auto& Pair : Prediction.PlayerWinProbabilities)
    {
        if (Pair.Value > BestProbability)
        {
            BestProbability = Pair.Value;
            BestPlayer = Pair.Key;
        }
    }
    
    Prediction.PredictedWinner = BestPlayer;
    Prediction.WinProbability = BestProbability;
    Prediction.Confidence = ScoreToConfidence(BestProbability);
    
    // Estimate duration based on player patterns
    float TotalAggression = 0.0f;
    for (const FString& PlayerID : PlayerIDs)
    {
        if (PlayerBehaviorHistory.Contains(PlayerID))
        {
            TotalAggression += PlayerBehaviorHistory[PlayerID].AggressionScore;
        }
    }
    
    float AverageAggression = TotalAggression / PlayerIDs.Num();
    // More aggression = shorter game
    Prediction.PredictedDuration = 600.0f * (1.0f - AverageAggression * 0.5f);
    
    // Generate key factors
    Prediction.KeyFactors.Empty();
    if (BestProbability > 0.7f)
    {
        Prediction.KeyFactors.Add(TEXT("Strong player performance"));
    }
    if (AverageAggression > 0.7f)
    {
        Prediction.KeyFactors.Add(TEXT("Aggressive playstyle detected"));
    }
    
    // Store prediction
    GamePredictions.Add(MatchID, Prediction);
    
    OnGameOutcomePredicted.Broadcast(Prediction);
    
    return Prediction;
}

void UMingRTSPredictiveAI::UpdateGameState(const FString& MatchID, const FString& StateData)
{
    // Update existing prediction with new state data
    if (GamePredictions.Contains(MatchID))
    {
        // In a real implementation, this would parse state data and update prediction
        UE_LOG(LogTemp, Verbose, TEXT("MingRTSPredictiveAI: Updated game state for match %s"), *MatchID);
    }
}

float UMingRTSPredictiveAI::GetWinProbability(const FString& MatchID, const FString& PlayerID) const
{
    if (GamePredictions.Contains(MatchID))
    {
        const FGameOutcomePrediction& Prediction = GamePredictions[MatchID];
        if (Prediction.PlayerWinProbabilities.Contains(PlayerID))
        {
            return Prediction.PlayerWinProbabilities[PlayerID];
        }
    }
    return 0.5f;
}

FTrendAnalysis UMingRTSPredictiveAI::AnalyzeTrend(const FString& MetricName, const TArray<float>& HistoricalData)
{
    FTrendAnalysis Analysis;
    Analysis.TrendID = GenerateTrendID(MetricName);
    Analysis.MetricName = MetricName;
    
    if (HistoricalData.Num() < 2)
    {
        Analysis.Direction = ETrendDirection::Stable;
        return Analysis;
    }
    
    Analysis.HistoricalValues = HistoricalData;
    Analysis.CurrentValue = HistoricalData.Last();
    
    // Calculate trend direction
    float PreviousValue = HistoricalData[HistoricalData.Num() - 2];
    float Change = Analysis.CurrentValue - PreviousValue;
    float Rate = Change / PreviousValue;
    
    Analysis.ChangeRate = Rate;
    Analysis.Direction = DetermineTrendDirection(Analysis.CurrentValue, PreviousValue, Rate);
    
    // Calculate trend strength
    Analysis.TrendStrength = CalculateTrendStrength(HistoricalData);
    
    // Calculate volatility
    Analysis.Volatility = CalculateVolatility(HistoricalData);
    
    // Predict future value
    if (ModelConfig.bUseMachineLearning && HistoricalData.Num() >= ModelConfig.MinDataPoints)
    {
        Analysis.PredictedValue = PredictUsingNeuralNetwork(HistoricalData, EPredictionType::ResourceTrend);
    }
    else
    {
        Analysis.PredictedValue = PredictUsingLinearRegression(HistoricalData, 1);
    }
    
    // Store trend
    ActiveTrends.Add(Analysis.TrendID, Analysis);
    
    OnTrendIdentified.Broadcast(Analysis.TrendID, Analysis);
    
    return Analysis;
}

void UMingRTSPredictiveAI::UpdateTrendData(const FString& MetricName, float NewValue)
{
    if (!TrendData.Contains(MetricName))
    {
        TrendData.Add(MetricName, TArray<float>());
    }
    
    TrendData[MetricName].Add(NewValue);
    
    // Keep only relevant history
    int32 MaxHistory = FMath::Max(ModelConfig.MinDataPoints * 2, 100);
    if (TrendData[MetricName].Num() > MaxHistory)
    {
        TrendData[MetricName].RemoveAt(0);
    }
    
    // Re-analyze trend if enough data
    if (TrendData[MetricName].Num() >= ModelConfig.MinDataPoints)
    {
        AnalyzeTrend(MetricName, TrendData[MetricName]);
    }
}

TArray<FTrendAnalysis> UMingRTSPredictiveAI::GetAllActiveTrends() const
{
    TArray<FTrendAnalysis> Trends;
    for (const auto& Pair : ActiveTrends)
    {
        Trends.Add(Pair.Value);
    }
    return Trends;
}

ETrendDirection UMingRTSPredictiveAI::PredictTrendDirection(const FString& MetricName, float Horizon)
{
    if (!ActiveTrends.Contains(GenerateTrendID(MetricName)))
    {
        return ETrendDirection::Stable;
    }
    
    const FTrendAnalysis& Analysis = ActiveTrends[GenerateTrendID(MetricName)];
    
    // Project forward
    float ProjectedChange = Analysis.ChangeRate * Horizon;
    float FutureValue = Analysis.CurrentValue * (1.0f + ProjectedChange);
    
    return DetermineTrendDirection(FutureValue, Analysis.CurrentValue, ProjectedChange);
}

FChurnRiskAssessment UMingRTSPredictiveAI::AssessChurnRisk(const FString& PlayerID)
{
    FChurnRiskAssessment Assessment;
    Assessment.PlayerID = PlayerID;
    
    // Gather data
    float EngagementDecline = CalculateEngagementDecline(PlayerID);
    float SatisfactionScore = CalculateSatisfactionScore(PlayerID);
    
    // Calculate churn probability
    Assessment.ChurnProbability = FMath::Clamp(
        (EngagementDecline * 0.5f + (1.0f - SatisfactionScore) * 0.5f), 0.0f, 1.0f);
    
    Assessment.EngagementScore = 1.0f - EngagementDecline;
    Assessment.SatisfactionScore = SatisfactionScore;
    
    Assessment.Confidence = ScoreToConfidence(1.0f - Assessment.ChurnProbability);
    
    // Identify risk factors
    Assessment.RiskFactors = IdentifyChurnRiskFactors(PlayerID);
    
    // Generate recommendations
    Assessment.RetentionRecommendations = GenerateRetentionRecommendations(
        Assessment.ChurnProbability, Assessment.RiskFactors);
    
    // Store assessment
    ChurnAssessments.Add(PlayerID, Assessment);
    
    // Alert if high risk
    if (Assessment.ChurnProbability > 0.7f)
    {
        OnChurnRiskDetected.Broadcast(Assessment);
    }
    
    return Assessment;
}

void UMingRTSPredictiveAI::UpdatePlayerEngagement(const FString& PlayerID, float EngagementScore)
{
    // Record engagement score in trend data
    UpdateTrendData(PlayerID + TEXT("_Engagement"), EngagementScore);
}

void UMingRTSPredictiveAI::RecordPlayerSession(const FString& PlayerID, float Duration, bool bCompleted)
{
    // Update session history
    FString SessionKey = PlayerID + TEXT("_SessionDuration");
    UpdateTrendData(SessionKey, Duration);
    
    // Record completion rate
    FString CompletionKey = PlayerID + TEXT("_CompletionRate");
    float CompletionValue = bCompleted ? 1.0f : 0.0f;
    
    if (TrendData.Contains(CompletionKey) && TrendData[CompletionKey].Num() > 0)
    {
        // Moving average
        float CurrentAvg = TrendData[CompletionKey].Last();
        float NewAvg = CurrentAvg * 0.9f + CompletionValue * 0.1f;
        UpdateTrendData(CompletionKey, NewAvg);
    }
    else
    {
        UpdateTrendData(CompletionKey, CompletionValue);
    }
}

TArray<FChurnRiskAssessment> UMingRTSPredictiveAI::GetHighRiskPlayers(float Threshold) const
{
    TArray<FChurnRiskAssessment> HighRiskPlayers;
    
    for (const auto& Pair : ChurnAssessments)
    {
        if (Pair.Value.ChurnProbability >= Threshold)
        {
            HighRiskPlayers.Add(Pair.Value);
        }
    }
    
    // Sort by risk level
    HighRiskPlayers.Sort([](const FChurnRiskAssessment& A, const FChurnRiskAssessment& B)
    {
        return A.ChurnProbability > B.ChurnProbability;
    });
    
    return HighRiskPlayers;
}

float UMingRTSPredictiveAI::PredictResourceNeeds(const FString& PlayerID, EResourceType ResourceType, float TimeHorizon)
{
    FString Key = PlayerID + TEXT("_Resource_") + FString::FromInt(static_cast<int32>(ResourceType));
    
    if (!TrendData.Contains(Key) || TrendData[Key].Num() < ModelConfig.MinDataPoints)
    {
        return 100.0f; // Default prediction
    }
    
    const TArray<float>& Data = TrendData[Key];
    
    float PredictedValue;
    if (ModelConfig.bUseMachineLearning)
    {
        PredictedValue = PredictUsingNeuralNetwork(Data, EPredictionType::ResourceTrend);
    }
    else
    {
        PredictedValue = PredictUsingLinearRegression(Data, FMath::CeilToInt(TimeHorizon / 60.0f));
    }
    
    return FMath::Max(0.0f, PredictedValue);
}

float UMingRTSPredictiveAI::PredictEconomicGrowth(const FString& PlayerID, float TimeHorizon)
{
    FString Key = PlayerID + TEXT("_Economy");
    
    if (!TrendData.Contains(Key) || TrendData[Key].Num() < ModelConfig.MinDataPoints)
    {
        return 0.0f; // No growth data
    }
    
    const TArray<float>& Data = TrendData[Key];
    
    float GrowthRate = 0.0f;
    if (Data.Num() >= 2)
    {
        float Recent = Data.Last();
        float Previous = Data[Data.Num() - 2];
        
        if (Previous > 0)
        {
            GrowthRate = (Recent - Previous) / Previous;
        }
    }
    
    // Project forward
    float ProjectedGrowth = GrowthRate * (TimeHorizon / 60.0f);
    
    return ProjectedGrowth;
}

TMap<EResourceType, float> UMingRTSPredictiveAI::PredictResourceShortages(const FString& PlayerID, float TimeHorizon)
{
    TMap<EResourceType, float> Shortages;
    
    // Check all resource types
    for (int32 i = 0; i < static_cast<int32>(EResourceType::Max); ++i)
    {
        EResourceType ResourceType = static_cast<EResourceType>(i);
        
        float PredictedNeed = PredictResourceNeeds(PlayerID, ResourceType, TimeHorizon);
        
        // Predicted shortage threshold
        if (PredictedNeed < 50.0f)
        {
            Shortages.Add(ResourceType, PredictedNeed);
        }
    }
    
    return Shortages;
}

float UMingRTSPredictiveAI::PredictBattleOutcome(const FString& AttackerID, const FString& DefenderID,
    int32 AttackerForces, int32 DefenderForces)
{
    // Base probability from force ratio
    float TotalForces = AttackerForces + DefenderForces;
    if (TotalForces == 0)
    {
        return 0.5f;
    }
    
    float ForceRatio = AttackerForces / TotalForces;
    
    // Adjust for player behavior
    float AttackerBonus = 0.0f;
    float DefenderBonus = 0.0f;
    
    if (PlayerBehaviorHistory.Contains(AttackerID))
    {
        AttackerBonus = PlayerBehaviorHistory[AttackerID].AggressionScore * 0.1f;
    }
    
    if (PlayerBehaviorHistory.Contains(DefenderID))
    {
        DefenderBonus = PlayerBehaviorHistory[DefenderID].DefenseScore * 0.1f;
    }
    
    float WinProbability = ForceRatio + AttackerBonus - DefenderBonus;
    
    return FMath::Clamp(WinProbability, 0.0f, 1.0f);
}

TArray<FString> UMingRTSPredictiveAI::PredictStrategicTargets(const FString& PlayerID)
{
    TArray<FString> Targets;
    
    // This would integrate with game state to identify strategic targets
    // For now, return empty list
    
    return Targets;
}

float UMingRTSPredictiveAI::EstimateBattleDuration(const FString& AttackerID, const FString& DefenderID)
{
    float BaseDuration = 60.0f; // Base 60 seconds
    
    // Adjust based on player styles
    float SpeedFactor = 1.0f;
    
    if (PlayerBehaviorHistory.Contains(AttackerID))
    {
        SpeedFactor += PlayerBehaviorHistory[AttackerID].APM / 300.0f * 0.5f;
    }
    
    if (PlayerBehaviorHistory.Contains(DefenderID))
    {
        SpeedFactor -= PlayerBehaviorHistory[DefenderID].DefenseScore * 0.3f;
    }
    
    return BaseDuration / FMath::Max(0.5f, SpeedFactor);
}

FPredictionResult UMingRTSPredictiveAI::GeneratePrediction(EPredictionType Type, const FString& TargetID,
    const FString& Context)
{
    FPredictionResult Result;
    Result.PredictionID = GeneratePredictionID();
    Result.PredictionType = Type;
    Result.TargetID = TargetID;
    Result.PredictionTime = CurrentGameTime;
    Result.ValidUntil = CurrentGameTime + ModelConfig.PredictionHorizon;
    Result.PredictionContext = Context;
    
    // Generate prediction based on type
    switch (Type)
    {
    case EPredictionType::PlayerBehavior:
        if (PlayerBehaviorHistory.Contains(TargetID))
        {
            Result.PredictedValue = PredictPlayerPerformance(TargetID, 300.0f);
            Result.ConfidenceScore = 0.7f;
        }
        break;
        
    case EPredictionType::ResourceTrend:
        {
            FString Key = TargetID + TEXT("_Resource");
            if (TrendData.Contains(Key) && TrendData[Key].Num() > 0)
            {
                Result.PredictedValue = TrendData[Key].Last();
                Result.ConfidenceScore = 0.6f;
            }
        }
        break;
        
    case EPredictionType::GameOutcome:
        // Would need match data
        Result.PredictedValue = 0.5f;
        Result.ConfidenceScore = 0.5f;
        break;
        
    default:
        Result.PredictedValue = 0.5f;
        Result.ConfidenceScore = 0.5f;
        break;
    }
    
    Result.ConfidenceLevel = ScoreToConfidence(Result.ConfidenceScore);
    
    // Store prediction
    ActivePredictions.Add(Result.PredictionID, Result);
    
    // Update stats
    TotalPredictions++;
    AccumulatedConfidence += Result.ConfidenceScore;
    
    OnPredictionGenerated.Broadcast(TargetID, Result);
    
    return Result;
}

TArray<FPredictionResult> UMingRTSPredictiveAI::GetActivePredictions(const FString& TargetID) const
{
    TArray<FPredictionResult> Results;
    
    for (const auto& Pair : ActivePredictions)
    {
        if (Pair.Value.TargetID == TargetID)
        {
            Results.Add(Pair.Value);
        }
    }
    
    return Results;
}

void UMingRTSPredictiveAI::InvalidatePrediction(const FString& PredictionID)
{
    ActivePredictions.Remove(PredictionID);
}

float UMingRTSPredictiveAI::ValidatePrediction(const FString& PredictionID, float ActualValue)
{
    if (!ActivePredictions.Contains(PredictionID))
    {
        return 0.0f;
    }
    
    FPredictionResult& Prediction = ActivePredictions[PredictionID];
    
    // Calculate accuracy
    float Accuracy = 1.0f - FMath::Abs(Prediction.PredictedValue - ActualValue);
    
    // Update model accuracy
    UpdateModelAccuracy(Prediction.PredictionType, Prediction.PredictedValue, ActualValue);
    
    // Remove validated prediction
    ActivePredictions.Remove(PredictionID);
    
    return Accuracy;
}

void UMingRTSPredictiveAI::RetrainModel(EPredictionType ModelType)
{
    // In a real implementation, this would retrain ML models
    UE_LOG(LogTemp, Log, TEXT("MingRTSPredictiveAI: Retraining model for type %d"), 
        static_cast<int32>(ModelType));
}

void UMingRTSPredictiveAI::SetModelConfiguration(const FPredictionModelConfig& NewConfig)
{
    ModelConfig = NewConfig;
}

FPredictionModelConfig UMingRTSPredictiveAI::GetModelConfiguration() const
{
    return ModelConfig;
}

float UMingRTSPredictiveAI::GetModelAccuracy(EPredictionType ModelType) const
{
    if (ModelAccuracy.Contains(ModelType))
    {
        return ModelAccuracy[ModelType];
    }
    return 0.5f;
}

TMap<EPredictionType, float> UMingRTSPredictiveAI::GetPredictionAccuracyStats() const
{
    return ModelAccuracy;
}

int32 UMingRTSPredictiveAI::GetTotalPredictionsMade() const
{
    return TotalPredictions;
}

float UMingRTSPredictiveAI::GetAverageConfidence() const
{
    if (TotalPredictions == 0)
    {
        return 0.0f;
    }
    return AccumulatedConfidence / TotalPredictions;
}

void UMingRTSPredictiveAI::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    // Process all subsystems
    ProcessBehaviorData(DeltaTime);
    UpdateTrendPredictions(DeltaTime);
    CheckChurnRisks(DeltaTime);
    UpdateGamePredictions(DeltaTime);
    CleanupOldPredictions();
}

void UMingRTSPredictiveAI::ProcessBehaviorData(float DeltaTime)
{
    // Periodically detect patterns
    static float LastPatternDetectionTime = 0.0f;
    
    if (CurrentGameTime - LastPatternDetectionTime > 60.0f) // Every minute
    {
        for (const auto& Pair : PlayerBehaviorHistory)
        {
            DetectBehaviorPattern(Pair.Key);
        }
        LastPatternDetectionTime = CurrentGameTime;
    }
}

void UMingRTSPredictiveAI::UpdateTrendPredictions(float DeltaTime)
{
    // Update active trends
    for (auto& Pair : ActiveTrends)
    {
        FTrendAnalysis& Trend = Pair.Value;
        
        // Update prediction horizon
        Trend.PredictionHorizon -= DeltaTime;
        
        if (Trend.PredictionHorizon <= 0)
        {
            // Regenerate prediction
            if (TrendData.Contains(Trend.MetricName))
            {
                Trend = AnalyzeTrend(Trend.MetricName, TrendData[Trend.MetricName]);
            }
        }
    }
}

void UMingRTSPredictiveAI::CheckChurnRisks(float DeltaTime)
{
    static float LastChurnCheckTime = 0.0f;
    
    if (CurrentGameTime - LastChurnCheckTime > 300.0f) // Every 5 minutes
    {
        for (const auto& Pair : PlayerBehaviorHistory)
        {
            AssessChurnRisk(Pair.Key);
        }
        LastChurnCheckTime = CurrentGameTime;
    }
}

void UMingRTSPredictiveAI::UpdateGamePredictions(float DeltaTime)
{
    // Update game outcome predictions
    for (auto& Pair : GamePredictions)
    {
        FGameOutcomePrediction& Prediction = Pair.Value;
        
        // Reduce prediction confidence over time
        Prediction.WinProbability = FMath::Lerp(Prediction.WinProbability, 0.5f, DeltaTime * 0.001f);
    }
}

void UMingRTSPredictiveAI::CleanupOldPredictions()
{
    // Remove expired predictions
    TArray<FString> ExpiredPredictions;
    
    for (const auto& Pair : ActivePredictions)
    {
        if (CurrentGameTime > Pair.Value.ValidUntil)
        {
            ExpiredPredictions.Add(Pair.Key);
        }
    }
    
    for (const FString& ID : ExpiredPredictions)
    {
        ActivePredictions.Remove(ID);
    }
}

void UMingRTSPredictiveAI::ValidatePredictionsAgainstReality()
{
    // In a real implementation, this would compare predictions with actual outcomes
}

float UMingRTSPredictiveAI::CalculateBehaviorPatternConfidence(const FString& PlayerID, EPlayerBehaviorPattern Pattern)
{
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        return 0.0f;
    }
    
    const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
    return CalculatePatternMatchScore(Data.ActionHistory, Pattern);
}

float UMingRTSPredictiveAI::PredictUsingLinearRegression(const TArray<float>& Data, int32 Horizon)
{
    if (Data.Num() < 2)
    {
        return Data.Num() > 0 ? Data.Last() : 0.0f;
    }
    
    // Simple linear regression: y = mx + b
    int32 N = Data.Num();
    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumX2 = 0.0f;
    
    for (int32 i = 0; i < N; ++i)
    {
        float X = static_cast<float>(i);
        float Y = Data[i];
        SumX += X;
        SumY += Y;
        SumXY += X * Y;
        SumX2 += X * X;
    }
    
    float Denominator = N * SumX2 - SumX * SumX;
    if (Denominator == 0)
    {
        return Data.Last();
    }
    
    float Slope = (N * SumXY - SumX * SumY) / Denominator;
    float Intercept = (SumY - Slope * SumX) / N;
    
    float Predicted = Intercept + Slope * (N - 1 + Horizon);
    
    return Predicted;
}

float UMingRTSPredictiveAI::PredictUsingMovingAverage(const TArray<float>& Data, int32 Window)
{
    if (Data.Num() == 0)
    {
        return 0.0f;
    }
    
    int32 EffectiveWindow = FMath::Min(Window, Data.Num());
    float Sum = 0.0f;
    
    for (int32 i = Data.Num() - EffectiveWindow; i < Data.Num(); ++i)
    {
        Sum += Data[i];
    }
    
    return Sum / EffectiveWindow;
}

float UMingRTSPredictiveAI::PredictUsingExponentialSmoothing(const TArray<float>& Data, float Alpha)
{
    if (Data.Num() == 0)
    {
        return 0.0f;
    }
    
    float Smoothed = Data[0];
    
    for (int32 i = 1; i < Data.Num(); ++i)
    {
        Smoothed = Alpha * Data[i] + (1.0f - Alpha) * Smoothed;
    }
    
    return Smoothed;
}

float UMingRTSPredictiveAI::PredictUsingNeuralNetwork(const TArray<float>& Data, EPredictionType Type)
{
    // Placeholder for neural network prediction
    // In a real implementation, this would use a trained NN model
    
    // For now, use a combination of other methods
    float Linear = PredictUsingLinearRegression(Data, 1);
    float EMA = PredictUsingExponentialSmoothing(Data, 0.3f);
    float MA = PredictUsingMovingAverage(Data, 5);
    
    // Weighted combination
    return Linear * 0.4f + EMA * 0.4f + MA * 0.2f;
}

EPredictionConfidence UMingRTSPredictiveAI::ScoreToConfidence(float Score) const
{
    if (Score >= 0.9f)
    {
        return EPredictionConfidence::VeryHigh;
    }
    else if (Score >= 0.75f)
    {
        return EPredictionConfidence::High;
    }
    else if (Score >= 0.5f)
    {
        return EPredictionConfidence::Medium;
    }
    else if (Score >= 0.25f)
    {
        return EPredictionConfidence::Low;
    }
    else
    {
        return EPredictionConfidence::VeryLow;
    }
}

void UMingRTSPredictiveAI::UpdateModelAccuracy(EPredictionType Type, float Prediction, float Actual)
{
    float Error = FMath::Abs(Prediction - Actual);
    float NewAccuracy = 1.0f - Error;
    
    // Exponential moving average
    float CurrentAccuracy = ModelAccuracy.Contains(Type) ? ModelAccuracy[Type] : 0.5f;
    ModelAccuracy[Type] = CurrentAccuracy * 0.9f + NewAccuracy * 0.1f;
}

float UMingRTSPredictiveAI::CalculateWinProbability(const FString& PlayerID, const FString& MatchID)
{
    float BaseProbability = 0.5f;
    
    if (PlayerBehaviorHistory.Contains(PlayerID))
    {
        const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
        
        // Adjust based on behavior scores
        BaseProbability += (Data.AggressionScore - 0.5f) * 0.1f;
        BaseProbability += (Data.EconomyScore - 0.5f) * 0.1f;
        BaseProbability += (Data.APM / 300.0f - 0.5f) * 0.1f;
    }
    
    return FMath::Clamp(BaseProbability, 0.1f, 0.9f);
}

TMap<FString, float> UMingRTSPredictiveAI::ExtractFeatures(const FString& PlayerID)
{
    TMap<FString, float> Features;
    
    if (PlayerBehaviorHistory.Contains(PlayerID))
    {
        const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
        
        Features.Add(TEXT("Aggression"), Data.AggressionScore);
        Features.Add(TEXT("Economy"), Data.EconomyScore);
        Features.Add(TEXT("Defense"), Data.DefenseScore);
        Features.Add(TEXT("Exploration"), Data.ExplorationScore);
        Features.Add(TEXT("APM"), Data.APM / 300.0f);
    }
    
    return Features;
}

float UMingRTSPredictiveAI::CalculateEngagementDecline(const FString& PlayerID)
{
    FString Key = PlayerID + TEXT("_Engagement");
    
    if (!TrendData.Contains(Key) || TrendData[Key].Num() < 2)
    {
        return 0.0f;
    }
    
    const TArray<float>& Data = TrendData[Key];
    
    float Recent = Data.Last();
    float Previous = Data[0];
    
    if (Previous > 0)
    {
        float Decline = (Previous - Recent) / Previous;
        return FMath::Max(0.0f, Decline);
    }
    
    return 0.0f;
}

float UMingRTSPredictiveAI::CalculateSatisfactionScore(const FString& PlayerID)
{
    float Score = 0.5f;
    
    // Based on session completion rate
    FString CompletionKey = PlayerID + TEXT("_CompletionRate");
    if (TrendData.Contains(CompletionKey) && TrendData[CompletionKey].Num() > 0)
    {
        Score = TrendData[CompletionKey].Last();
    }
    
    // Adjust based on engagement
    float Engagement = 1.0f - CalculateEngagementDecline(PlayerID);
    Score = Score * 0.6f + Engagement * 0.4f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

EPlayerBehaviorPattern UMingRTSPredictiveAI::DetectPatternFromActions(const TMap<FString, float>& Actions)
{
    // Calculate scores for each pattern
    TMap<EPlayerBehaviorPattern, float> PatternScores;
    
    for (int32 i = 0; i < static_cast<int32>(EPlayerBehaviorPattern::Unknown); ++i)
    {
        EPlayerBehaviorPattern Pattern = static_cast<EPlayerBehaviorPattern>(i);
        PatternScores.Add(Pattern, CalculatePatternMatchScore(Actions, Pattern));
    }
    
    // Find best match
    EPlayerBehaviorPattern BestPattern = EPlayerBehaviorPattern::Unknown;
    float BestScore = 0.3f; // Threshold
    
    for (const auto& Pair : PatternScores)
    {
        if (Pair.Value > BestScore)
        {
            BestScore = Pair.Value;
            BestPattern = Pair.Key;
        }
    }
    
    return BestPattern;
}

float UMingRTSPredictiveAI::CalculatePatternMatchScore(const TMap<FString, float>& Actions, EPlayerBehaviorPattern Pattern)
{
    float Score = 0.0f;
    float TotalWeight = 0.0f;
    
    switch (Pattern)
    {
    case EPlayerBehaviorPattern::AggressiveRush:
        if (Actions.Contains(TEXT("Attack"))) { Score += Actions[TEXT("Attack")] * 2.0f; TotalWeight += 2.0f; }
        if (Actions.Contains(TEXT("Combat"))) { Score += Actions[TEXT("Combat")] * 1.5f; TotalWeight += 1.5f; }
        if (Actions.Contains(TEXT("Build"))) { Score += (1.0f - Actions[TEXT("Build")]) * 0.5f; TotalWeight += 0.5f; }
        break;
        
    case EPlayerBehaviorPattern::EconomicFocus:
        if (Actions.Contains(TEXT("Gather"))) { Score += Actions[TEXT("Gather")] * 2.0f; TotalWeight += 2.0f; }
        if (Actions.Contains(TEXT("Resource"))) { Score += Actions[TEXT("Resource")] * 1.5f; TotalWeight += 1.5f; }
        if (Actions.Contains(TEXT("Economy"))) { Score += Actions[TEXT("Economy")] * 1.5f; TotalWeight += 1.5f; }
        break;
        
    case EPlayerBehaviorPattern::DefensiveTurtle:
        if (Actions.Contains(TEXT("Build"))) { Score += Actions[TEXT("Build")] * 2.0f; TotalWeight += 2.0f; }
        if (Actions.Contains(TEXT("Defend"))) { Score += Actions[TEXT("Defend")] * 2.0f; TotalWeight += 2.0f; }
        if (Actions.Contains(TEXT("Attack"))) { Score += (1.0f - Actions[TEXT("Attack")]) * 0.5f; TotalWeight += 0.5f; }
        break;
        
    case EPlayerBehaviorPattern::ExplorationFocus:
        if (Actions.Contains(TEXT("Explore"))) { Score += Actions[TEXT("Explore")] * 2.0f; TotalWeight += 2.0f; }
        if (Actions.Contains(TEXT("Scout"))) { Score += Actions[TEXT("Scout")] * 1.5f; TotalWeight += 1.5f; }
        break;
        
    default:
        break;
    }
    
    return TotalWeight > 0 ? Score / TotalWeight : 0.0f;
}

float UMingRTSPredictiveAI::CalculateTrendStrength(const TArray<float>& Data) const
{
    if (Data.Num() < 2)
    {
        return 0.0f;
    }
    
    // Calculate coefficient of determination (R-squared)
    float Mean = 0.0f;
    for (float Value : Data)
    {
        Mean += Value;
    }
    Mean /= Data.Num();
    
    float SST = 0.0f; // Total sum of squares
    float SSE = 0.0f; // Error sum of squares
    
    for (int32 i = 0; i < Data.Num(); ++i)
    {
        float Predicted = PredictUsingLinearRegression(Data, i - Data.Num() + 1);
        SST += FMath::Square(Data[i] - Mean);
        SSE += FMath::Square(Data[i] - Predicted);
    }
    
    float RSquared = SST > 0 ? 1.0f - (SSE / SST) : 0.0f;
    
    return FMath::Max(0.0f, RSquared);
}

float UMingRTSPredictiveAI::CalculateVolatility(const TArray<float>& Data) const
{
    if (Data.Num() < 2)
    {
        return 0.0f;
    }
    
    float Mean = 0.0f;
    for (float Value : Data)
    {
        Mean += Value;
    }
    Mean /= Data.Num();
    
    float Variance = 0.0f;
    for (float Value : Data)
    {
        Variance += FMath::Square(Value - Mean);
    }
    Variance /= Data.Num();
    
    return FMath::Sqrt(Variance);
}

ETrendDirection UMingRTSPredictiveAI::DetermineTrendDirection(float Current, float Previous, float Rate)
{
    float Threshold = 0.05f; // 5% change threshold
    
    if (FMath::Abs(Rate) < Threshold)
    {
        return ETrendDirection::Stable;
    }
    else if (Rate > 0)
    {
        if (Rate > 0.3f) return ETrendDirection::Peak;
        return ETrendDirection::Increasing;
    }
    else
    {
        if (Rate < -0.3f) return ETrendDirection::Trough;
        return ETrendDirection::Decreasing;
    }
}

TArray<FString> UMingRTSPredictiveAI::IdentifyChurnRiskFactors(const FString& PlayerID)
{
    TArray<FString> Factors;
    
    float EngagementDecline = CalculateEngagementDecline(PlayerID);
    if (EngagementDecline > 0.3f)
    {
        Factors.Add(TEXT("Declining engagement"));
    }
    
    if (PlayerBehaviorHistory.Contains(PlayerID))
    {
        const FPlayerBehaviorData& Data = PlayerBehaviorHistory[PlayerID];
        
        if (Data.APM < 50.0f)
        {
            Factors.Add(TEXT("Low activity level"));
        }
        
        if (Data.SatisfactionScore < 0.3f)
        {
            Factors.Add(TEXT("Low satisfaction"));
        }
    }
    
    FString SessionKey = PlayerID + TEXT("_SessionDuration");
    if (TrendData.Contains(SessionKey) && TrendData[SessionKey].Num() > 0)
    {
        float LastSession = TrendData[SessionKey].Last();
        if (LastSession < 300.0f) // Less than 5 minutes
        {
            Factors.Add(TEXT("Short play sessions"));
        }
    }
    
    return Factors;
}

TArray<FString> UMingRTSPredictiveAI::GenerateRetentionRecommendations(float RiskLevel, const TArray<FString>& Factors)
{
    TArray<FString> Recommendations;
    
    if (RiskLevel > 0.5f)
    {
        Recommendations.Add(TEXT("Send personalized offer"));
    }
    
    for (const FString& Factor : Factors)
    {
        if (Factor.Contains(TEXT("Declining engagement")))
        {
            Recommendations.Add(TEXT("Introduce new content"));
            Recommendations.Add(TEXT("Send re-engagement notification"));
        }
        else if (Factor.Contains(TEXT("Low activity")))
        {
            Recommendations.Add(TEXT("Offer tutorial assistance"));
            Recommendations.Add(TEXT("Simplify early game experience"));
        }
        else if (Factor.Contains(TEXT("Low satisfaction")))
        {
            Recommendations.Add(TEXT("Request feedback"));
            Recommendations.Add(TEXT("Offer compensation"));
        }
    }
    
    return Recommendations;
}

FString UMingRTSPredictiveAI::GeneratePredictionID() const
{
    return FString::Printf(TEXT("PRED_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSPredictiveAI::GenerateTrendID(const FString& MetricName) const
{
    return FString::Printf(TEXT("TREND_%s_%s"), *MetricName, *FGuid::NewGuid().ToString().Left(8));
}
