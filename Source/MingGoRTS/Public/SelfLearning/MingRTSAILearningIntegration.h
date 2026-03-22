// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 8.1: AI Learning Integration

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SelfLearning/MingRTSSelfLearningSystem.h"
#include "MingRTSAILearningIntegration.generated.h"

// Player strategy patterns
UENUM(BlueprintType)
enum class EPlayerStrategyPattern : uint8
{
    AggressiveRush,      // Fast unit production, early attacks
    DefensiveTurtle,     // Strong defenses, late game economy
    EconomicBoom,        // Focus on resource gathering
    BalancedApproach,    // Mixed strategy
    MicroIntensive,      // Focus on unit control
    MacroIntensive,      // Focus on economy management
    Adaptive,            // Changes strategy based on opponent
    Unknown              // Unidentified pattern
};

// AI learning confidence levels
UENUM(BlueprintType)
enum class EAILearningConfidence : uint8
{
    VeryLow,     // < 30% confidence
    Low,         // 30-50% confidence
    Medium,      // 50-70% confidence
    High,        // 70-85% confidence
    VeryHigh     // > 85% confidence
};

// Player behavior analysis result
USTRUCT(BlueprintType)
struct FPlayerBehaviorAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    EPlayerStrategyPattern PrimaryStrategy;

    UPROPERTY(BlueprintReadOnly)
    EPlayerStrategyPattern SecondaryStrategy;

    UPROPERTY(BlueprintReadOnly)
    EAILearningConfidence Confidence;

    UPROPERTY(BlueprintReadOnly)
    float AggressionScore;        // 0.0 = very defensive, 1.0 = very aggressive

    UPROPERTY(BlueprintReadOnly)
    float EconomicFocus;          // 0.0 = military focus, 1.0 = economic focus

    UPROPERTY(BlueprintReadOnly)
    float MicroSkillLevel;        // 0.0 = beginner, 1.0 = expert

    UPROPERTY(BlueprintReadOnly)
    float MacroSkillLevel;        // 0.0 = beginner, 1.0 = expert

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreferredUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreferredTactics;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> StrategyWeights;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;
};

// AI adaptation parameters
USTRUCT(BlueprintType)
struct FAIAdaptationParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float AggressionAdjustment;        // Adjust AI aggression

    UPROPERTY(BlueprintReadWrite)
    float EconomicAdjustment;          // Adjust AI economic focus

    UPROPERTY(BlueprintReadWrite)
    float DifficultyScaling;           // Overall difficulty adjustment

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> UnitCounters;  // Counter specific units

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> StrategyWeights; // Adjust strategy weights

    UPROPERTY(BlueprintReadWrite)
    float ResponseTime;                // AI response time adjustment

    UPROPERTY(BlueprintReadWrite)
    float RiskTolerance;               // AI risk tolerance level

    UPROPERTY(BlueprintReadWrite)
    bool bAdaptToPlayerStyle;          // Enable style adaptation

    UPROPERTY(BlueprintReadWrite)
    bool bLearnFromMatches;            // Enable match learning
};

/**
 * AI Learning Integration System
 * 
 * This system integrates the self-learning capabilities with the AI controller,
 * enabling the AI to learn from player strategies and adapt its behavior accordingly.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (MingGoRTS))
class MINGGORTS_API UMingRTSAILearningIntegration : public UObject
{
    GENERATED_BODY()

public:
    // Constructor and initialization
    UMingRTSAILearningIntegration();
    virtual void BeginDestroy() override;

    // System initialization
    UFUNCTION(BlueprintCallable, Category = "AI Learning")
    void InitializeAILearning(UMingRTSSelfLearningSystem* SelfLearningSystem};

    UFUNCTION(BlueprintCallable, Category = "AI Learning")
    void ShutdownAILearning();

    // Player behavior analysis
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    void AnalyzePlayerBehavior(const FString& PlayerID, const TMap<FString, FString>& MatchData};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    FPlayerBehaviorAnalysis GetPlayerAnalysis(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    TArray<FPlayerBehaviorAnalysis> GetAllPlayerAnalyses() const;

    // AI adaptation
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Adaptation")
    void AdaptAIToPlayer(const FString& PlayerID};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Adaptation")
    void ApplyAIAdaptations(const FAIAdaptationParameters& Parameters};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Adaptation")
    FAIAdaptationParameters GetRecommendedAdaptations(const FString& PlayerID) const;

    // Learning model management
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Models")
    bool CreatePlayerStrategyModel(const FString& ModelName};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Models")
    bool TrainStrategyModel();

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Models")
    EPlayerStrategyPattern PredictPlayerStrategy(const FString& PlayerID, const TMap<FString, FString>& CurrentState};

    // Match data collection
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Data")
    void RecordMatchStart(const FString& PlayerID, const FString& MatchID};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Data")
    void RecordPlayerAction(const FString& PlayerID, const FString& Action, const TMap<FString, FString>& Context};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Data")
    void RecordMatchEnd(const FString& PlayerID, const FString& MatchID, bool bVictory, const TMap<FString, FString>& Results};

    // Strategy pattern recognition
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Patterns")
    EPlayerStrategyPattern IdentifyStrategyPattern(const TArray<TMap<FString, FString>>& ActionHistory};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Patterns")
    float CalculateAggressionScore(const TArray<TMap<FString, FString>>& ActionHistory};

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Patterns")
    float CalculateEconomicFocus(const TArray<TMap<FString, FString>>& ActionHistory};

    // Performance monitoring
    UFUNCTION(BlueprintCallable, Category = "AI Learning|Performance")
    float GetPredictionAccuracy(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Performance")
    EAILearningConfidence GetLearningConfidence(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Performance")
    void ResetLearningData(const FString& PlayerID};

    // Events and delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerAnalyzed, const FString&, PlayerID, const FPlayerBehaviorAnalysis&, Analysis};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIAdapted, const FString&, PlayerID, const FAIAdaptationParameters&, Adaptations};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStrategyPredicted, const FString&, PlayerID, EPlayerStrategyPattern, Strategy, float, Confidence};

    UPROPERTY(BlueprintAssignable)
    FOnPlayerAnalyzed OnPlayerAnalyzed;

    UPROPERTY(BlueprintAssignable)
    FOnAIAdapted OnAIAdapted;

    UPROPERTY(BlueprintAssignable)
    FOnStrategyPredicted OnStrategyPredicted;

protected:
    // Analysis algorithms
    void AnalyzeAggressionPattern(const FString& PlayerID};
    void AnalyzeEconomicPattern(const FString& PlayerID};
    void AnalyzeUnitPreferences(const FString& PlayerID};
    void AnalyzeTimingPatterns(const FString& PlayerID};
    void AnalyzeMicroMacroBalance(const FString& PlayerID};

    // Strategy identification
    EPlayerStrategyPattern ClassifyPlayerStrategy(const FPlayerBehaviorAnalysis& Analysis};
    void UpdateStrategyWeights(FPlayerBehaviorAnalysis& Analysis};
    void ValidateAnalysis(FPlayerBehaviorAnalysis& Analysis};

    // AI adaptation logic
    FAIAdaptationParameters GenerateAdaptations(const FPlayerBehaviorAnalysis& Analysis};
    void ApplyCounterStrategies(FAIAdaptationParameters& Parameters, const FPlayerBehaviorAnalysis& Analysis};
    void AdjustDifficulty(FAIAdaptationParameters& Parameters, const FPlayerBehaviorAnalysis& Analysis};

    // Data processing
    void ProcessMatchData(const FString& PlayerID};
    void ExtractFeatures(const TMap<FString, FString>& RawData, TArray<float>& Features};
    void NormalizeFeatures(TArray<float>& Features};

private:
    // Core system reference
    UPROPERTY()
    TObjectPtr<UMingRTSSelfLearningSystem> SelfLearningSystem;

    // Player analysis storage
    UPROPERTY()
    TMap<FString, FPlayerBehaviorAnalysis> PlayerAnalyses;

    // Match data storage
    UPROPERTY()
    TMap<FString, TArray<TMap<FString, FString>>> PlayerMatchData;

    // Current match tracking
    UPROPERTY()
    TMap<FString, FString> CurrentMatches;

    // AI adaptation storage
    UPROPERTY()
    TMap<FString, FAIAdaptationParameters> PlayerAdaptations;

    // Learning models
    UPROPERTY()
    TArray<FString> StrategyModelNames;

    // Performance metrics
    UPROPERTY()
    TMap<FString, float> PredictionAccuracies;

    UPROPERTY()
    TMap<FString, EAILearningConfidence> LearningConfidences;

    // Configuration
    UPROPERTY()
    bool bSystemInitialized;

    UPROPERTY()
    bool bEnableAdaptiveAI;

    UPROPERTY()
    bool bEnableStrategyLearning;

    UPROPERTY()
    int32 MinMatchesForAnalysis;

    UPROPERTY()
    float ConfidenceThreshold;

    // Statistics
    UPROPERTY()
    int32 TotalAnalyzedPlayers;

    UPROPERTY()
    int32 TotalMatchesRecorded;

    UPROPERTY()
    int32 TotalAdaptationsApplied;
};

