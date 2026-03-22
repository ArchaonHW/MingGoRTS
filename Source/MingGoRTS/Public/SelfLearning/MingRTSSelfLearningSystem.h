// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 8.1: Self-Learning System Implementation
// 
// This system provides self-learning capabilities for MingGoRTS, enabling the game
// to learn from user behavior, game data, and environment to continuously improve
// the player experience through adaptive AI, personalized content, and intelligent
// recommendations.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Containers/CircularBuffer.h"
#include "Math/Vector.h"
#include "MingRTSSelfLearningSystem.generated.h"

// Forward declarations
class UMingRTSAIController;
class UMingRTSCulturalAdaptationSystem;
class UMingRTSAudioEnhancedSystem;
class UMingRTSUIEnhancedSystem;
class UMingAIGeneratedContentSystem;

// Learning data types
UENUM(BlueprintType)
enum class ELearningDataType: uint8 {
    UserBehavior,        // User interaction patterns
    GameplayMetrics,     // Game performance metrics
    CulturalPreference,  // Cultural adaptation preferences
    AudioPreference,     // Audio and music preferences
    UIPreference,        // UI interaction preferences
    ContentPreference,   // Content generation preferences
    AIPlayerStrategy,    // Player strategy patterns
    SystemPerformance,   // System performance metrics
    Custom               // Custom learning data
};

// Learning algorithm types
UENUM(BlueprintType)
enum class ELearningAlgorithm: uint8 {
    ReinforcementLearning,  // RL for adaptive behavior
    NeuralNetwork,          // Deep learning for pattern recognition
    Clustering,             // Unsupervised learning for grouping
    DecisionTree,           // Rule-based learning
    Bayesian,               // Probabilistic learning
    GeneticAlgorithm,       // Evolutionary learning
    CollaborativeFiltering, // Recommendation systems
    Custom                  // Custom algorithm
};

// Learning confidence levels
UENUM(BlueprintType)
enum class ELearningConfidence: uint8 {
    VeryLow,     // 0-20% confidence
    Low,         // 20-40% confidence
    Medium,      // 40-60% confidence
    High,        // 60-80% confidence
    VeryHigh     // 80-100% confidence
};

// User behavior data structure
USTRUCT(BlueprintType)
struct FUserBehaviorData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString UserID;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString ActionType;  // UI click, unit selection, etc.

    UPROPERTY(BlueprintReadOnly)
    FString Context;     // Game state, location, etc.

    UPROPERTY(BlueprintReadOnly)
    float Duration;      // Time spent on action

    UPROPERTY(BlueprintReadOnly)
    bool bSuccessful;    // Was the action successful?

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;  // Additional context data
};

// Gameplay metrics data structure
USTRUCT(BlueprintType)
struct FGameplayMetricsData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString UserID;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    float WinRate;

    UPROPERTY(BlueprintReadOnly)
    float AverageGameTime;

    UPROPERTY(BlueprintReadOnly)
    int32 UnitsCommanded;

    UPROPERTY(BlueprintReadOnly)
    int32 ResourcesCollected;

    UPROPERTY(BlueprintReadOnly)
    int32 BuildingsConstructed;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreferredUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreferredStrategies;
};

// Learning model configuration
USTRUCT(BlueprintType)
struct FLearningModelConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    ELearningAlgorithm Algorithm;

    UPROPERTY(BlueprintReadWrite)
    ELearningDataType TargetDataType;

    UPROPERTY(BlueprintReadWrite)
    float LearningRate;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxIterations;

    UPROPERTY(BlueprintReadWrite)
    float ConvergenceThreshold;

    UPROPERTY(BlueprintReadWrite)
    bool bEnableRegularization;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> Hyperparameters;
};

// Learning result structure
USTRUCT(BlueprintType)
struct FLearningResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ELearningDataType DataType;

    UPROPERTY(BlueprintReadOnly)
    ELearningConfidence Confidence;

    UPROPERTY(BlueprintReadOnly)
    float Accuracy;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> Predictions;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    FString ModelVersion;

    UPROPERTY(BlueprintReadOnly)
    FDateTime GeneratedAt;
};

// Adaptive parameters for system adjustment
USTRUCT(BlueprintType)
struct FAdaptiveParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> AIStrategyWeights;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> CulturalPreferences;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> AudioPreferences;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> UIPreferences;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> ContentPreferences;

    UPROPERTY(BlueprintReadWrite)
    float DifficultyAdjustment;

    UPROPERTY(BlueprintReadWrite)
    float ContentComplexityAdjustment;
};

/**
 * Self-Learning System Core Class
 * 
 * This system provides comprehensive self-learning capabilities for MingGoRTS,
 * enabling adaptive AI, personalized content, and intelligent recommendations
 * based on user behavior and game data analysis.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (MingGoRTS))
class MINGGORTS_API UMingRTSSelfLearningSystem : public UObject
{
    GENERATED_BODY()

public:
    // Constructor and initialization
    UMingRTSSelfLearningSystem();
    virtual void BeginDestroy() override;

    // System initialization and lifecycle
    UFUNCTION(BlueprintCallable, Category = "Self-Learning")
    void InitializeSelfLearningSystem();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning")
    void ShutdownSelfLearningSystem();

    // Data collection interface
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Data")
    void CollectUserBehaviorData(const FUserBehaviorData& BehaviorData);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Data")
    void CollectGameplayMetrics(const FGameplayMetricsData& MetricsData);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Data")
    void CollectCustomData(ELearningDataType DataType, const TMap<FString, FString>& Data);

    // Learning model management
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Models")
    bool CreateLearningModel(const FString& ModelName, const FLearningModelConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Models")
    bool TrainLearningModel(const FString& ModelName);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Models")
    FLearningResult GetLearningPrediction(const FString& ModelName, const TMap<FString, FString>& InputData);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Models")
    bool DeleteLearningModel(const FString& ModelName);

    // Adaptive system integration
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Adaptation")
    void ApplyAdaptiveParameters(const FAdaptiveParameters& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Adaptation")
    FAdaptiveParameters GetRecommendedAdaptations(const FString& UserID);

    // System integration interfaces
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Integration")
    void IntegrateWithAISystem(UMingRTSAIController* AIController);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Integration")
    void IntegrateWithCulturalSystem(UMingRTSCulturalAdaptationSystem* CulturalSystem);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Integration")
    void IntegrateWithAudioSystem(UMingRTSAudioEnhancedSystem* AudioSystem);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Integration")
    void IntegrateWithUISystem(UMingRTSUIEnhancedSystem* UISystem);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Integration")
    void IntegrateWithContentSystem(UMingAIGeneratedContentSystem* ContentSystem);

    // Learning analytics and monitoring
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Analytics")
    TMap<FString, FLearningResult> GetAllLearningResults();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Analytics")
    float GetLearningAccuracy(const FString& ModelName);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Analytics")
    ELearningConfidence GetLearningConfidence(const FString& ModelName);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Analytics")
    void ExportLearningData(const FString& FilePath);

    // Configuration and settings
    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Config")
    void SetLearningRate(float Rate);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Config")
    void SetMaxDataPoints(int32 MaxPoints);

    UFUNCTION(BlueprintCallable, Category = "Self-Learning|Config")
    void EnableAutoLearning(bool bEnabled);

    // Events and delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningCompleted, const FString&, ModelName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAdaptationRecommended, const FString&, UserID, const FAdaptiveParameters&, Parameters);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningDataCollected, ELearningDataType, DataType);

    UPROPERTY(BlueprintAssignable)
    FOnLearningCompleted OnLearningCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnAdaptationRecommended OnAdaptationRecommended;

    UPROPERTY(BlueprintAssignable)
    FOnLearningDataCollected OnLearningDataCollected;

protected:
    // Core learning algorithms
    void ProcessReinforcementLearning(const FString& ModelName);
    void ProcessNeuralNetwork(const FString& ModelName);
    void ProcessClustering(const FString& ModelName);
    void ProcessDecisionTree(const FString& ModelName);
    void ProcessBayesianLearning(const FString& ModelName);
    void ProcessGeneticAlgorithm(const FString& ModelName);
    void ProcessCollaborativeFiltering(const FString& ModelName);

    // Data processing utilities
    void PreprocessData(ELearningDataType DataType);
    void ValidateData(const TMap<FString, FString>& Data);
    void NormalizeData(TMap<FString, float>& Data);
    void FeatureExtraction(TMap<FString, FString>& Data, TArray<float>& Features);

    // Model management utilities
    bool ValidateModelConfig(const FLearningModelConfig& Config);
    void SaveModel(const FString& ModelName);
    void LoadModel(const FString& ModelName);
    void UpdateModelMetrics(const FString& ModelName, const FLearningResult& Result);

    // Adaptive system integration
    void AdaptAISystem(const FAdaptiveParameters& Parameters);
    void AdaptCulturalSystem(const FAdaptiveParameters& Parameters);
    void AdaptAudioSystem(const FAdaptiveParameters& Parameters);
    void AdaptUISystem(const FAdaptiveParameters& Parameters);
    void AdaptContentSystem(const FAdaptiveParameters& Parameters);

    // Background processing
    void StartBackgroundLearning();
    void StopBackgroundLearning();
    void ProcessLearningQueue();

private:
    // Core data storage
    UPROPERTY()
    TMap<ELearningDataType, TArray<TMap<FString, FString>>> LearningData;

    UPROPERTY()
    TMap<FString, FLearningModelConfig> LearningModels;

    UPROPERTY()
    TMap<FString, FLearningResult> LearningResults;

    UPROPERTY()
    TMap<FString, FAdaptiveParameters> UserAdaptations;

    // System integration references
    UPROPERTY()
    TObjectPtr<UMingRTSAIController> IntegratedAIController;

    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> IntegratedCulturalSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSAudioEnhancedSystem> IntegratedAudioSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSUIEnhancedSystem> IntegratedUISystem;

    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> IntegratedContentSystem;

    // Configuration parameters
    UPROPERTY()
    float GlobalLearningRate;

    UPROPERTY()
    int32 MaxDataPoints;

    UPROPERTY()
    bool bAutoLearningEnabled;

    UPROPERTY()
    bool bSystemInitialized;

    // Background processing
    UPROPERTY()
    class FTimerHandle BackgroundLearningTimer;

    UPROPERTY()
    TArray<FString> LearningQueue;

    // Performance metrics
    UPROPERTY()
    TMap<FString, float> ModelAccuracies;

    UPROPERTY()
    TMap<FString, ELearningConfidence> ModelConfidences;

    UPROPERTY()
    int32 TotalDataPointsCollected;

    UPROPERTY()
    int32 TotalLearningIterations;
};

