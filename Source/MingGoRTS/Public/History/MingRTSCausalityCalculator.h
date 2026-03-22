// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Causality Calculator - Analyzes and calculates causal relationships between events

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.h"
#include "MingRTSCausalityCalculator.generated.h"

// Causality Analysis Methods
UENUM(BlueprintType)
enum class ECausalityAnalysisMethod : uint8
{
    TemporalProximity UMETA(DisplayName = "時間接近性"),
    SpatialProximity UMETA(DisplayName = "空間接近性"),
    LogicalInference UMETA(DisplayName = "邏輯推理"),
    StatisticalCorrelation UMETA(DisplayName = "統計相關性"),
    PatternMatching UMETA(DisplayName = "模式匹配"),
    BayesianInference UMETA(DisplayName = "貝葉斯推理"),
    NeuralNetwork UMETA(DisplayName = "神經網絡"),
    Hybrid UMETA(DisplayName = "混合方法")
};

// Causality Confidence Levels
UENUM(BlueprintType)
enum class ECausalityConfidence : uint8
{
    VeryLow UMETA(DisplayName = "很低"),
    Low UMETA(DisplayName = "低"),
    Medium UMETA(DisplayName = "中等"),
    High UMETA(DisplayName = "高"),
    VeryHigh UMETA(DisplayName = "很高"),
    Certain UMETA(DisplayName = "確定")
};

// Causality Analysis Result
USTRUCT(BlueprintType)
struct MINGGORTS_API FCausalityAnalysisResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    FString SourceEventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    FString TargetEventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    ECausalityType CausalityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    float CausalStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    ECausalityConfidence Confidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    ECausalityAnalysisMethod AnalysisMethod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    TArray<FString> SupportingEvidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    TArray<FString> ConflictingEvidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    FString Explanation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Analysis")
    FDateTime AnalysisTimestamp;

    FCausalityAnalysisResult()
    {
        CausalityType = ECausalityType::Direct;
        CausalStrength = 0.0f;
        Confidence = ECausalityConfidence::Low;
        AnalysisMethod = ECausalityAnalysisMethod::TemporalProximity;
        AnalysisTimestamp = FDateTime::Now();
    }
};

// Causality Network Node
USTRUCT(BlueprintType)
struct MINGGORTS_API FCausalityNetworkNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    TArray<FString> Causes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    TArray<FString> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    float CentralityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    float InfluenceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Network")
    int32 CausalDepth;

    FCausalityNetworkNode()
    {
        CentralityScore = 0.0f;
        InfluenceScore = 0.0f;
        CausalDepth = 0;
    }
};

// Causality Analysis Configuration
USTRUCT(BlueprintType)
struct MINGGORTS_API FCausalityAnalysisConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    ECausalityAnalysisMethod PrimaryMethod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    float TemporalThreshold = 7.0f; // Days

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    float SpatialThreshold = 500.0f; // Units

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    float MinimumCausalStrength = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    float ConfidenceThreshold = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    bool bEnableStatisticalAnalysis = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    bool bEnablePatternRecognition = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    bool bEnableMachineLearning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causality Config")
    int32 MaxAnalysisDepth = 5;

    FCausalityAnalysisConfig()
    {
        PrimaryMethod = ECausalityAnalysisMethod::Hybrid;
        TemporalThreshold = 7.0f;
        SpatialThreshold = 500.0f;
        MinimumCausalStrength = 0.3f;
        ConfidenceThreshold = 0.6f;
        bEnableStatisticalAnalysis = true;
        bEnablePatternRecognition = true;
        bEnableMachineLearning = false;
        MaxAnalysisDepth = 5;
    }
};

/**
 * Causality Calculator - Analyzes and calculates causal relationships between historical events
 * 
 * This system uses multiple analysis methods to determine causal relationships
 * between events, providing confidence scores and explanations for each relationship.
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCausalityCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCausalityCalculator();

    // Causality Analysis
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    void InitializeCausalityCalculator();

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    FCausalityAnalysisResult AnalyzeCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FCausalityAnalysisResult> AnalyzeEventCausality(const FString& EventID, const TArray<FHistoricalEventData>& AllEvents);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FCausalityAnalysisResult> AnalyzeTimelineCausality(const FString& TimelineID, const TMap<FString, FHistoricalEventData>& AllEvents);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    void UpdateCausalityNetwork(const TArray<FCausalityAnalysisResult>& NewResults);

    // Causality Network Management
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TMap<FString, FCausalityNetworkNode> GetCausalityNetwork() const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    FCausalityNetworkNode GetNetworkNode(const FString& EventID) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> GetUpstreamEvents(const FString& EventID, int32 MaxDepth = 3) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> GetDownstreamEvents(const FString& EventID, int32 MaxDepth = 3) const;

    // Causality Prediction
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> PredictCausalEffects(const FString& EventID, const TArray<FHistoricalEventData>& AllEvents);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> PredictCausalCauses(const FString& EventID, const TArray<FHistoricalEventData>& AllEvents);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float CalculateCausalProbability(const FString& SourceEventID, const FString& TargetEventID) const;

    // Causality Analysis Methods
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float AnalyzeTemporalCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float AnalyzeSpatialCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float AnalyzeLogicalCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float AnalyzeStatisticalCausality(const FHistoricalEventData& SourceEvent, const TArray<FHistoricalEventData>& TargetEvents) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float AnalyzePatternCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    void SetAnalysisConfig(const FCausalityAnalysisConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    FCausalityAnalysisConfig GetAnalysisConfig() const { return AnalysisConfig; }

    // Network Analytics
    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> GetMostInfluentialEvents(int32 TopCount = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    TArray<FString> GetCriticalPathEvents(const FString& StartEventID, const FString& EndEventID) const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float CalculateNetworkDensity() const;

    UFUNCTION(BlueprintCallable, Category = "Causality Calculator")
    float CalculateAveragePathLength() const;

protected:
    // Analysis method implementations
    FCausalityAnalysisResult PerformTemporalAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformSpatialAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformLogicalAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformStatisticalAnalysis(const FHistoricalEventData& SourceEvent, const TArray<FHistoricalEventData>& TargetEvents);
    FCausalityAnalysisResult PerformPatternAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformBayesianAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformNeuralNetworkAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);
    FCausalityAnalysisResult PerformHybridAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent);

    // Network analysis
    void CalculateNetworkMetrics();
    float CalculateNodeCentrality(const FString& EventID) const;
    float CalculateNodeInfluence(const FString& EventID) const;
    int32 CalculateNodeDepth(const FString& EventID) const;

    // Pattern recognition
    TArray<FString> IdentifyCausalPatterns(const TArray<FHistoricalEventData>& Events) const;
    bool MatchesCausalPattern(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent, const FString& Pattern) const;

    // Statistical analysis
    float CalculateCorrelationCoefficient(const TArray<float>& VariableA, const TArray<float>& VariableB) const;
    float CalculatePValue(const float Correlation, int32 SampleSize) const;

    // Machine learning (simplified)
    void TrainCausalModel(const TArray<FCausalityAnalysisResult>& TrainingData);
    float PredictCausalStrength(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;

private:
    UPROPERTY()
    TMap<FString, FCausalityNetworkNode> CausalityNetwork;

    UPROPERTY()
    FCausalityAnalysisConfig AnalysisConfig;

    UPROPERTY()
    TArray<FCausalityAnalysisResult> AnalysisHistory;

    UPROPERTY()
    bool bCalculatorInitialized;

    // Helper methods
    ECausalityConfidence DetermineConfidence(float CausalStrength, const TArray<FString>& Evidence) const;
    FString GenerateExplanation(const FCausalityAnalysisResult& Result) const;
    void UpdateNetworkNode(const FString& EventID);
    void ValidateNetworkIntegrity();
    float CalculateTemporalWeight(const FTimespan& TimeDifference) const;
    float CalculateSpatialWeight(const float Distance) const;
    float CalculateLogicalWeight(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const;
};
