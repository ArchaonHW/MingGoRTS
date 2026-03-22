#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPredictiveAI.generated.h"

// Prediction Types
UENUM(BlueprintType)
enum class EPredictionType : uint8
{
    PlayerBehavior      UMETA(DisplayName = "玩家行为"),
    GameOutcome         UMETA(DisplayName = "游戏结果"),
    ResourceTrend       UMETA(DisplayName = "资源趋势"),
    BattleResult        UMETA(DisplayName = "战斗结果"),
    EconomicTrend       UMETA(DisplayName = "经济趋势"),
    StrategicMove       UMETA(DisplayName = "战略行动"),
    PlayerChurn         UMETA(DisplayName = "玩家流失"),
    SessionDuration     UMETA(DisplayName = "会话时长"),
    ContentEngagement   UMETA(DisplayName = "内容参与度"),
    DifficultyAdaptation UMETA(DisplayName = "难度适应")
};

// Prediction Confidence Level
UENUM(BlueprintType)
enum class EPredictionConfidence : uint8
{
    VeryLow             UMETA(DisplayName = "非常低"),
    Low                 UMETA(DisplayName = "低"),
    Medium              UMETA(DisplayName = "中等"),
    High                UMETA(DisplayName = "高"),
    VeryHigh            UMETA(DisplayName = "非常高")
};

// Player Behavior Patterns
UENUM(BlueprintType)
enum class EPlayerBehaviorPattern : uint8
{
    AggressiveRush      UMETA(DisplayName = "激进快攻"),
    DefensiveTurtle     UMETA(DisplayName = "防御龟缩"),
    EconomicFocus       UMETA(DisplayName = "经济导向"),
    BalancedApproach    UMETA(DisplayName = "平衡发展"),
    Micromanagement     UMETA(DisplayName = "微操密集"),
    Macromanagement     UMETA(DisplayName = "宏操密集"),
    AdaptiveStrategy    UMETA(DisplayName = "适应性战略"),
    ExplorationFocus    UMETA(DisplayName = "探索导向"),
    CombatFocus         UMETA(DisplayName = "战斗导向"),
    DiplomacyFocus      UMETA(DisplayName = "外交导向"),
    Unknown             UMETA(DisplayName = "未知")
};

// Trend Direction
UENUM(BlueprintType)
enum class ETrendDirection : uint8
{
    Increasing          UMETA(DisplayName = "上升"),
    Decreasing          UMETA(DisplayName = "下降"),
    Stable              UMETA(DisplayName = "稳定"),
    Volatile            UMETA(DisplayName = "波动"),
    Peak                UMETA(DisplayName = "峰值"),
    Trough              UMETA(DisplayName = "谷值")
};

// Prediction Result Structure
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FPredictionResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString PredictionID;
    
    UPROPERTY(BlueprintReadOnly)
    EPredictionType PredictionType;
    
    UPROPERTY(BlueprintReadOnly)
    FString TargetID;
    
    UPROPERTY(BlueprintReadOnly)
    float PredictedValue;
    
    UPROPERTY(BlueprintReadOnly)
    float ConfidenceScore;
    
    UPROPERTY(BlueprintReadOnly)
    EPredictionConfidence ConfidenceLevel;
    
    UPROPERTY(BlueprintReadOnly)
    float PredictionTime;
    
    UPROPERTY(BlueprintReadOnly)
    float ValidUntil;
    
    UPROPERTY(BlueprintReadOnly)
    FString PredictionContext;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ContributingFactors;
    
    UPROPERTY(BlueprintReadOnly)
    FString RecommendedAction;
    
    FPredictionResult()
        : PredictionType(EPredictionType::PlayerBehavior)
        , PredictedValue(0.0f)
        , ConfidenceScore(0.5f)
        , ConfidenceLevel(EPredictionConfidence::Medium)
        , PredictionTime(0.0f)
        , ValidUntil(60.0f)
    {}
};

// Player Behavior Data
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FPlayerBehaviorData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    FVector PlayerLocation;
    
    UPROPERTY(BlueprintReadOnly)
    EPlayerBehaviorPattern CurrentPattern;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ActionHistory;
    
    UPROPERTY(BlueprintReadOnly)
    float AggressionScore;
    
    UPROPERTY(BlueprintReadOnly)
    float EconomyScore;
    
    UPROPERTY(BlueprintReadOnly)
    float DefenseScore;
    
    UPROPERTY(BlueprintReadOnly)
    float ExplorationScore;
    
    UPROPERTY(BlueprintReadOnly)
    float APM; // Actions Per Minute
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceGatheringRate;
    
    UPROPERTY(BlueprintReadOnly)
    float UnitProductionRate;
    
    FPlayerBehaviorData()
        : CurrentPattern(EPlayerBehaviorPattern::Unknown)
        , AggressionScore(0.5f)
        , EconomyScore(0.5f)
        , DefenseScore(0.5f)
        , ExplorationScore(0.5f)
        , APM(0.0f)
        , ResourceGatheringRate(0.0f)
        , UnitProductionRate(0.0f)
    {}
};

// Trend Analysis Data
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FTrendAnalysis
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString TrendID;
    
    UPROPERTY(BlueprintReadOnly)
    FString MetricName;
    
    UPROPERTY(BlueprintReadOnly)
    ETrendDirection Direction;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentValue;
    
    UPROPERTY(BlueprintReadOnly)
    float PredictedValue;
    
    UPROPERTY(BlueprintReadOnly)
    float ChangeRate;
    
    UPROPERTY(BlueprintReadOnly)
    float Volatility;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<float> HistoricalValues;
    
    UPROPERTY(BlueprintReadOnly)
    float TrendStrength;
    
    UPROPERTY(BlueprintReadOnly)
    float PredictionHorizon;
    
    FTrendAnalysis()
        : Direction(ETrendDirection::Stable)
        , CurrentValue(0.0f)
        , PredictedValue(0.0f)
        , ChangeRate(0.0f)
        , Volatility(0.0f)
        , TrendStrength(0.5f)
        , PredictionHorizon(300.0f)
    {}
};

// Churn Risk Assessment
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FChurnRiskAssessment
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;
    
    UPROPERTY(BlueprintReadOnly)
    float ChurnProbability;
    
    UPROPERTY(BlueprintReadOnly)
    EPredictionConfidence Confidence;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskFactors;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RetentionRecommendations;
    
    UPROPERTY(BlueprintReadOnly)
    int32 DaysSinceLastPlay;
    
    UPROPERTY(BlueprintReadOnly)
    float EngagementScore;
    
    UPROPERTY(BlueprintReadOnly)
    float SatisfactionScore;
    
    FChurnRiskAssessment()
        : ChurnProbability(0.0f)
        , Confidence(EPredictionConfidence::Medium)
        , DaysSinceLastPlay(0)
        , EngagementScore(0.5f)
        , SatisfactionScore(0.5f)
    {}
};

// Game Outcome Prediction
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGameOutcomePrediction
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString MatchID;
    
    UPROPERTY(BlueprintReadOnly)
    FString PredictedWinner;
    
    UPROPERTY(BlueprintReadOnly)
    float WinProbability;
    
    UPROPERTY(BlueprintReadOnly)
    EPredictionConfidence Confidence;
    
    UPROPERTY(BlueprintReadOnly)
    float PredictedDuration;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> PlayerWinProbabilities;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> KeyFactors;
    
    UPROPERTY(BlueprintReadOnly)
    FString RecommendedStrategy;
    
    FGameOutcomePrediction()
        : WinProbability(0.5f)
        , Confidence(EPredictionConfidence::Medium)
        , PredictedDuration(600.0f)
    {}
};

// Prediction Model Configuration
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FPredictionModelConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinDataPoints;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeWindow;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UpdateFrequency;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseMachineLearning;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConfidenceThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PredictionHorizon;
    
    FPredictionModelConfig()
        : MinDataPoints(10)
        , TimeWindow(300.0f)
        , UpdateFrequency(1.0f)
        , bUseMachineLearning(true)
        , ConfidenceThreshold(0.6f)
        , PredictionHorizon(600)
    {}
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPredictionGenerated, FString, TargetID, FPredictionResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBehaviorPatternDetected, FString, PlayerID, EPlayerBehaviorPattern, Pattern);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrendIdentified, FString, TrendID, FTrendAnalysis, Analysis);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChurnRiskDetected, FChurnRiskAssessment, RiskAssessment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOutcomePredicted, FGameOutcomePrediction, Outcome);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRTSPredictiveAI : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSPredictiveAI();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Predictive")
    void InitializePredictiveAI(const FPredictionModelConfig& Config);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Predictive")
    void ShutdownPredictiveAI();
    
    // Player Behavior Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
    void RecordPlayerAction(const FString& PlayerID, const FString& Action, float Value);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
    EPlayerBehaviorPattern DetectBehaviorPattern(const FString& PlayerID);
    
    UFUNCTION(BlueprintPure, Category = "AI|Behavior")
    FPlayerBehaviorData GetPlayerBehaviorData(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
    TArray<EPredictionType> PredictNextActions(const FString& PlayerID, int32 NumPredictions);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
    float PredictPlayerPerformance(const FString& PlayerID, float TimeHorizon);
    
    // Game Outcome Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|Outcome")
    FGameOutcomePrediction PredictGameOutcome(const FString& MatchID, const TArray<FString>& PlayerIDs);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Outcome")
    void UpdateGameState(const FString& MatchID, const FString& StateData);
    
    UFUNCTION(BlueprintPure, Category = "AI|Outcome")
    float GetWinProbability(const FString& MatchID, const FString& PlayerID) const;
    
    // Trend Analysis
    UFUNCTION(BlueprintCallable, Category = "AI|Trend")
    FTrendAnalysis AnalyzeTrend(const FString& MetricName, const TArray<float>& HistoricalData);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Trend")
    void UpdateTrendData(const FString& MetricName, float NewValue);
    
    UFUNCTION(BlueprintPure, Category = "AI|Trend")
    TArray<FTrendAnalysis> GetAllActiveTrends() const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Trend")
    ETrendDirection PredictTrendDirection(const FString& MetricName, float Horizon);
    
    // Churn Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|Churn")
    FChurnRiskAssessment AssessChurnRisk(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Churn")
    void UpdatePlayerEngagement(const FString& PlayerID, float EngagementScore);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Churn")
    void RecordPlayerSession(const FString& PlayerID, float Duration, bool bCompleted);
    
    UFUNCTION(BlueprintPure, Category = "AI|Churn")
    TArray<FChurnRiskAssessment> GetHighRiskPlayers(float Threshold) const;
    
    // Resource and Economic Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|Economy")
    float PredictResourceNeeds(const FString& PlayerID, EResourceType ResourceType, float TimeHorizon);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Economy")
    float PredictEconomicGrowth(const FString& PlayerID, float TimeHorizon);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Economy")
    TMap<EResourceType, float> PredictResourceShortages(const FString& PlayerID, float TimeHorizon);
    
    // Battle Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|Battle")
    float PredictBattleOutcome(const FString& AttackerID, const FString& DefenderID, 
                               int32 AttackerForces, int32 DefenderForces);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Battle")
    TArray<FString> PredictStrategicTargets(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Battle")
    float EstimateBattleDuration(const FString& AttackerID, const FString& DefenderID);
    
    // General Prediction
    UFUNCTION(BlueprintCallable, Category = "AI|General")
    FPredictionResult GeneratePrediction(EPredictionType Type, const FString& TargetID, 
                                        const FString& Context);
    
    UFUNCTION(BlueprintPure, Category = "AI|General")
    TArray<FPredictionResult> GetActivePredictions(const FString& TargetID) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|General")
    void InvalidatePrediction(const FString& PredictionID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|General")
    float ValidatePrediction(const FString& PredictionID, float ActualValue);
    
    // Model Management
    UFUNCTION(BlueprintCallable, Category = "AI|Model")
    void RetrainModel(EPredictionType ModelType);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Model")
    void SetModelConfiguration(const FPredictionModelConfig& NewConfig);
    
    UFUNCTION(BlueprintPure, Category = "AI|Model")
    FPredictionModelConfig GetModelConfiguration() const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Model")
    float GetModelAccuracy(EPredictionType ModelType) const;
    
    // Analytics
    UFUNCTION(BlueprintPure, Category = "AI|Analytics")
    TMap<EPredictionType, float> GetPredictionAccuracyStats() const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Analytics")
    int32 GetTotalPredictionsMade() const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Analytics")
    float GetAverageConfidence() const;
    
    // Tick Update
    UFUNCTION(BlueprintCallable, Category = "AI")
    void Tick(float DeltaTime);
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "AI|Events")
    FOnPredictionGenerated OnPredictionGenerated;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Events")
    FOnBehaviorPatternDetected OnBehaviorPatternDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Events")
    FOnTrendIdentified OnTrendIdentified;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Events")
    FOnChurnRiskDetected OnChurnRiskDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Events")
    FOnGameOutcomePredicted OnGameOutcomePredicted;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float CurrentGameTime;
    
    UPROPERTY()
    FPredictionModelConfig ModelConfig;
    
    UPROPERTY()
    TMap<FString, FPlayerBehaviorData> PlayerBehaviorHistory;
    
    UPROPERTY()
    TMap<FString, TArray<float>> TrendData;
    
    UPROPERTY()
    TMap<FString, FTrendAnalysis> ActiveTrends;
    
    UPROPERTY()
    TMap<FString, FChurnRiskAssessment> ChurnAssessments;
    
    UPROPERTY()
    TMap<FString, FGameOutcomePrediction> GamePredictions;
    
    UPROPERTY()
    TMap<FString, FPredictionResult> ActivePredictions;
    
    UPROPERTY()
    TMap<FString, TMap<FString, float>> PlayerActionHistory;
    
    UPROPERTY()
    TMap<EPredictionType, float> ModelAccuracy;
    
    UPROPERTY()
    int32 TotalPredictions;
    
    UPROPERTY()
    float AccumulatedConfidence;
    
    // Internal processing
    void ProcessBehaviorData(float DeltaTime);
    void UpdateTrendPredictions(float DeltaTime);
    void CheckChurnRisks(float DeltaTime);
    void UpdateGamePredictions(float DeltaTime);
    void CleanupOldPredictions();
    void ValidatePredictionsAgainstReality();
    
    // Prediction algorithms
    float CalculateBehaviorPatternConfidence(const FString& PlayerID, EPlayerBehaviorPattern Pattern);
    float PredictUsingLinearRegression(const TArray<float>& Data, int32 Horizon);
    float PredictUsingMovingAverage(const TArray<float>& Data, int32 Window);
    float PredictUsingExponentialSmoothing(const TArray<float>& Data, float Alpha);
    float PredictUsingNeuralNetwork(const TArray<float>& Data, EPredictionType Type);
    
    // Helper functions
    EPredictionConfidence ScoreToConfidence(float Score) const;
    void UpdateModelAccuracy(EPredictionType Type, float Prediction, float Actual);
    float CalculateWinProbability(const FString& PlayerID, const FString& MatchID);
    TMap<FString, float> ExtractFeatures(const FString& PlayerID);
    float CalculateEngagementDecline(const FString& PlayerID);
    float CalculateSatisfactionScore(const FString& PlayerID);
    
    // Pattern detection
    EPlayerBehaviorPattern DetectPatternFromActions(const TMap<FString, float>& Actions);
    float CalculatePatternMatchScore(const TMap<FString, float>& Actions, EPlayerBehaviorPattern Pattern);
    
    // Trend analysis
    float CalculateTrendStrength(const TArray<float>& Data) const;
    float CalculateVolatility(const TArray<float>& Data) const;
    ETrendDirection DetermineTrendDirection(float Current, float Previous, float Rate);
    
    // Churn prediction
    TArray<FString> IdentifyChurnRiskFactors(const FString& PlayerID);
    TArray<FString> GenerateRetentionRecommendations(float RiskLevel, const TArray<FString>& Factors);
    
    // ID generation
    FString GeneratePredictionID() const;
    FString GenerateTrendID(const FString& MetricName) const;
};
