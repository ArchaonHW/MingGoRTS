#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MingRTSPredictiveAI.generated.h"

UENUM(BlueprintType)
enum class EPredictionType : uint8 {
    PlayerBehavior      UMETA(DisplayName = "Player Behavior"),
    GameOutcome         UMETA(DisplayName = "Game Outcome"),
    ResourceTrend       UMETA(DisplayName = "Resource Trend"),
    BattleResult        UMETA(DisplayName = "Battle Result"),
    EconomicTrend       UMETA(DisplayName = "Economic Trend"),
    StrategicMove       UMETA(DisplayName = "Strategic Move"),
    PlayerChurn         UMETA(DisplayName = "Player Churn"),
    SessionDuration     UMETA(DisplayName = "Session Duration"),
    ContentEngagement   UMETA(DisplayName = "Content Engagement"),
    DifficultyAdaptation UMETA(DisplayName = "Difficulty Adaptation")
};

UENUM(BlueprintType)
enum class EPredictionConfidence : uint8 {
    VeryLow             UMETA(DisplayName = "Very Low"),
    Low                 UMETA(DisplayName = "Low"),
    Medium              UMETA(DisplayName = "Medium"),
    High                UMETA(DisplayName = "High"),
    VeryHigh            UMETA(DisplayName = "Very High")
};

USTRUCT(BlueprintType)
struct FPredictionResult {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EPredictionType PredictionType;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceScore;

    UPROPERTY(BlueprintReadOnly)
    FString PredictionDescription;

    UPROPERTY(BlueprintReadOnly)
    float PredictedValue;

    UPROPERTY(BlueprintReadOnly)
    float TimeHorizon;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ContributingFactors;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendedAction;

    FPredictionResult()
        : PredictionType(EPredictionType::PlayerBehavior)
        , ConfidenceScore(0.5f)
        , PredictedValue(0.0f)
        , TimeHorizon(0.0f) {}
};

USTRUCT(BlueprintType)
struct FPlayerBehaviorPattern {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PatternID;

    UPROPERTY(BlueprintReadOnly)
    FString PatternName;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> BehaviorSequence;

    UPROPERTY(BlueprintReadOnly)
    float Frequency;

    UPROPERTY(BlueprintReadOnly)
    float ReliabilityScore;

    UPROPERTY(BlueprintReadOnly)
    float AverageDuration;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ContextTriggers;

    FPlayerBehaviorPattern()
        : Frequency(0.0f)
        , ReliabilityScore(0.5f)
        , AverageDuration(0.0f) {}
};

USTRUCT(BlueprintType)
struct FTrendAnalysisData {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TrendID;

    UPROPERTY(BlueprintReadOnly)
    FString MetricName;

    UPROPERTY(BlueprintReadOnly)
    float CurrentValue;

    UPROPERTY(BlueprintReadOnly)
    float PredictedValue;

    UPROPERTY(BlueprintReadOnly)
    float ChangeRate;

    UPROPERTY(BlueprintReadOnly)
    FString TrendDirection;

    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    int32 DataPoints;

    FTrendAnalysisData()
        : CurrentValue(0.0f)
        , PredictedValue(0.0f)
        , ChangeRate(0.0f)
        , Confidence(0.5f)
        , DataPoints(0) {}
};

USTRUCT(BlueprintType)
struct FContentPreloadRequest {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ContentID;

    UPROPERTY(BlueprintReadOnly)
    FString ContentType;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    float PredictedNeedTime;

    UPROPERTY(BlueprintReadOnly)
    FString Reason;

    FContentPreloadRequest()
        : Priority(0.5f)
        , PredictedNeedTime(0.0f) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPredictionMade, FString, PlayerID, FPredictionResult, Prediction, float, Confidence};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPatternDetected, FString, PlayerID, FPlayerBehaviorPattern, Pattern, float, Confidence};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTrendIdentified, FString, PlayerID, FTrendAnalysisData, Trend, FString, TrendType, float, Significance};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPreloadRecommended, FString, PlayerID, TArray<FContentPreloadRequest>, PreloadRequests};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRTSPredictiveAI : public UObject {
    GENERATED_BODY()

public:
    UMingRTSPredictiveAI(};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI")
    void InitializePredictiveAI(};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI")
    void ShutdownPredictiveAI(};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Prediction")
    FPredictionResult PredictPlayerBehavior(const FString& PlayerID, float TimeHorizon};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Prediction")
    FPredictionResult PredictGameOutcome(const FString& PlayerID, const FString& GameContext};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Prediction")
    FPredictionResult PredictResourceTrend(const FString& PlayerID, const FString& ResourceType, int32 TurnsAhead};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Prediction")
    FPredictionResult PredictBattleResult(const FString& PlayerID, const FString& BattleContext};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Prediction")
    TArray<FPredictionResult> GetMultiplePredictions(const FString& PlayerID, float TimeHorizon};

    UPROPERTY(BlueprintAssignable, Category = "Predictive AI Events")
    FOnPredictionMade OnPredictionMade;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Patterns")
    void StartPatternDetection(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Patterns")
    void StopPatternDetection(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Patterns")
    TArray<FPlayerBehaviorPattern> GetDetectedPatterns(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Patterns")
    FPlayerBehaviorPattern GetMostReliablePattern(const FString& PlayerID) const;

    UPROPERTY(BlueprintAssignable, Category = "Predictive AI Events")
    FOnPatternDetected OnPatternDetected;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Trends")
    FTrendAnalysisData AnalyzeMetricTrend(const FString& PlayerID, const FString& MetricName, int32 DataPoints};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Trends")
    TArray<FTrendAnalysisData> GetAllActiveTrends(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Trends")
    bool IsTrendSignificant(const FTrendAnalysisData& Trend, float Threshold) const;

    UPROPERTY(BlueprintAssignable, Category = "Predictive AI Events")
    FOnTrendIdentified OnTrendIdentified;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Preload")
    TArray<FContentPreloadRequest> GetPreloadRecommendations(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Preload")
    void PrioritizePreloadContent(const FString& PlayerID, const FString& ContentID, float Priority};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Preload")
    void ClearPreloadQueue(const FString& PlayerID};

    UPROPERTY(BlueprintAssignable, Category = "Predictive AI Events")
    FOnPreloadRecommended OnPreloadRecommended;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Player")
    void RegisterPlayer(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Player")
    void UnregisterPlayer(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Player")
    void RecordPlayerAction(const FString& PlayerID, const FString& ActionType, const FString& ActionData};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Player")
    void ClearPlayerHistory(const FString& PlayerID};

    UFUNCTION(BlueprintPure, Category = "Predictive AI Confidence")
    EPredictionConfidence GetConfidenceLevel(float Score) const;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Confidence")
    float GetSystemAccuracy(const FString& PredictionType, int32 LookbackPeriod) const;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Confidence")
    void SetMinimumConfidenceThreshold(float Threshold};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI")
    void Tick(float DeltaTime};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI")
    void ProcessRealtimeData(const FString& PlayerID, const FString& DataType, float Value};

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Analytics")
    TMap<EPredictionType, float> GetPredictionAccuracyStats() const;

    UFUNCTION(BlueprintCallable, Category = "Predictive AI Analytics")
    FString GenerateAnalyticsReport(const FString& PlayerID) const;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    float MinimumConfidenceThreshold;

    UPROPERTY()
    TMap<FString, TArray<FPlayerBehaviorPattern>> PlayerPatterns;

    UPROPERTY()
    TMap<FString, TArray<FPredictionResult>> PredictionHistory;

    UPROPERTY()
    TMap<FString, TArray<FTrendAnalysisData>> ActiveTrends;

    UPROPERTY()
    TMap<FString, TArray<FContentPreloadRequest>> PreloadQueues;

    UPROPERTY()
    TMap<EPredictionType, TArray<float>> AccuracyHistory;

    void ProcessPatternDetection(float DeltaTime};
    void UpdateTrendAnalysis(float DeltaTime};
    void EvaluatePreloadNeeds(float DeltaTime};
    float CalculateConfidence(const TArray<float>& HistoricalAccuracy) const;
    bool ShouldMakePrediction(const FString& PlayerID, EPredictionType Type) const;
    void StorePredictionResult(const FString& PlayerID, const FPredictionResult& Result};
    void UpdateAccuracyTracking(EPredictionType Type, bool bWasCorrect};

    float PredictWithNeuralNetwork(const TArray<float>& Features) const;
    float PredictWithStatisticalModel(const TArray<float>& HistoricalData) const;
    TArray<float> ExtractFeatures(const FString& PlayerID, EPredictionType Type) const;
    float CalculatePatternReliability(const FPlayerBehaviorPattern& Pattern) const;
    FString DetermineTrendDirection(float Current, float Previous, float Rate) const;
};
