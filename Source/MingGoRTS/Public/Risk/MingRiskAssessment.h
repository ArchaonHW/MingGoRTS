// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Real-Time Risk Assessment System - B2-2
// Provides real-time risk evaluation and scoring

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskAssessment.generated.h"

UENUM(BlueprintType)
enum class EAssessmentStatus: uint8 {
    Idle = 0, UMETA(DisplayName = "Idle"),
    Assessing, UMETA(DisplayName = "Assessing"),
    Completed, UMETA(DisplayName = "Completed"),
    Failed, UMETA(DisplayName = "Failed"),
    Paused UMETA(DisplayName = "Paused")
};

UENUM(BlueprintType)
enum class ERiskFactorType: uint8 {
    Performance = 0, UMETA(DisplayName = "Performance"),
    Stability, UMETA(DisplayName = "Stability"),
    Security, UMETA(DisplayName = "Security"),
    Scalability, UMETA(DisplayName = "Scalability"),
    Maintainability, UMETA(DisplayName = "Maintainability"),
    Compatibility, UMETA(DisplayName = "Compatibility"),
    ResourceUsage, UMETA(DisplayName = "Resource Usage"),
    PlayerSatisfaction, UMETA(DisplayName = "Player Satisfaction"),
    CodeQuality, UMETA(DisplayName = "Code Quality"),
    TestCoverage UMETA(DisplayName = "Test Coverage")
};

UENUM(BlueprintType)
enum class EAssessmentWeight: uint8 {
    VeryLow = 0, UMETA(DisplayName = "Very Low"),
    Low, UMETA(DisplayName = "Low"),
    Medium, UMETA(DisplayName = "Medium"),
    High, UMETA(DisplayName = "High"),
    VeryHigh, UMETA(DisplayName = "Very High"),
    Critical UMETA(DisplayName = "Critical")
};

USTRUCT(BlueprintType)
struct FRiskFactor
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskFactorType Type;

    UPROPERTY()
    float Score;

    UPROPERTY()
    float Weight;

    UPROPERTY()
    EAssessmentWeight Importance;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    TArray<FString> ContributingFactors;

    UPROPERTY()
    TArray<FString> MitigationStrategies;

    FRiskFactor()
        : Type(ERiskFactorType::Performance)
        , Score(0.0f)
        , Weight(1.0f)
        , Importance(EAssessmentWeight::Medium)
    {}
};

USTRUCT(BlueprintType)
struct FRiskAssessmentResult
{
    GENERATED_BODY()

    UPROPERTY()
    float OverallRiskScore;

    UPROPERTY()
    ERiskLevel OverallLevel;

    UPROPERTY()
    TArray<FRiskFactor> Factors;

    UPROPERTY()
    FString AssessmentID;

    UPROPERTY()
    uint32 AssessmentTime;

    UPROPERTY()
    uint32 DurationMs;

    UPROPERTY()
    FString Summary;

    UPROPERTY()
    TArray<FString> Recommendations;

    UPROPERTY()
    TMap<ERiskFactorType, float> CategoryScores;

    FRiskAssessmentResult()
        : OverallRiskScore(0.0f)
        , OverallLevel(ERiskLevel::None)
        , AssessmentTime(0)
        , DurationMs(0)
    {}
};

USTRUCT(BlueprintType)
struct FAssessmentConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    bool bEnableRealTimeAssessment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    float AssessmentInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    TArray<ERiskFactorType> EnabledFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    TMap<ERiskFactorType, float> CustomWeights;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    bool bAutoGenerateRecommendations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    bool bStoreHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    int32 MaxHistorySize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    float CriticalThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    float HighThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment")
    float MediumThreshold;

    FAssessmentConfig()
        : bEnableRealTimeAssessment(true)
        , AssessmentInterval(5.0f)
        , bAutoGenerateRecommendations(true)
        , bStoreHistory(true)
        , MaxHistorySize(1000)
        , CriticalThreshold(80.0f)
        , HighThreshold(60.0f)
        , MediumThreshold(40.0f)
    {}
};

USTRUCT(BlueprintType)
struct FRiskScoringRule
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskFactorType FactorType;

    UPROPERTY()
    float ThresholdLow;

    UPROPERTY()
    float ThresholdMedium;

    UPROPERTY()
    float ThresholdHigh;

    UPROPERTY()
    float ThresholdCritical;

    UPROPERTY()
    FString ScoringFormula;

    UPROPERTY()
    bool bInverseScoring;

    FRiskScoringRule()
        : FactorType(ERiskFactorType::Performance)
        , ThresholdLow(20.0f)
        , ThresholdMedium(40.0f)
        , ThresholdHigh(60.0f)
        , ThresholdCritical(80.0f)
        , bInverseScoring(false)
    {}
};





/**
 * Real-Time Risk Assessment System
 * Provides real-time risk evaluation and scoring
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRiskAssessment : public UObject
{
    GENERATED_BODY()

public:
    UMingRiskAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void InitializeAssessment(const FAssessmentConfig& Config);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void ShutdownAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    FRiskAssessmentResult PerformAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void StartRealTimeAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void StopRealTimeAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void PauseAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void ResumeAssessment();
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetFactorWeight(ERiskFactorType Factor, float Weight);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void EnableFactor(ERiskFactorType Factor, bool bEnabled);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetScoringRule(ERiskFactorType Factor, const FRiskScoringRule& Rule);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    FRiskAssessmentResult GetLastAssessmentResult() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    TArray<FRiskAssessmentResult> GetAssessmentHistory(int32 Count = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    float GetCurrentFactorScore(ERiskFactorType Factor) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    TMap<ERiskFactorType, float> GetAllFactorScores() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    ERiskLevel GetCurrentOverallRiskLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    TArray<FString> GenerateRecommendations(const FRiskAssessmentResult& Result);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void ExportAssessmentReport(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetCriticalThresholds(float Critical, float High, float Medium);
    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    EAssessmentStatus GetAssessmentStatus() const { return CurrentStatus; }

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    bool IsRealTimeAssessmentActive() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    FAssessmentConfig GetConfig() const { return Config; }

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnAssessmentCompleted OnAssessmentCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnFactorScoreChanged OnFactorScoreChanged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnCriticalRiskDetected OnCriticalRiskDetected;

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnAssessmentStatusChanged OnAssessmentStatusChanged;

protected:
    UPROPERTY()
    FAssessmentConfig Config;

    UPROPERTY()
    EAssessmentStatus CurrentStatus;

    UPROPERTY()
    FRiskAssessmentResult LastResult;

    UPROPERTY()
    TArray<FRiskAssessmentResult> AssessmentHistory;

    UPROPERTY()
    TMap<ERiskFactorType, FRiskScoringRule> ScoringRules;

    UPROPERTY()
    TMap<ERiskFactorType, float> CurrentScores;

    UPROPERTY()
    FTimerHandle AssessmentTimer;

    UPROPERTY()
    FString CurrentAssessmentID;

    float CalculateWeightedScore(const TArray<FRiskFactor>& Factors);
    ERiskLevel DetermineRiskLevel(float Score);
    FRiskFactor EvaluateFactor(ERiskFactorType FactorType);
    float CalculatePerformanceScore();
    float CalculateStabilityScore();
    float CalculateSecurityScore();
    float CalculateScalabilityScore();
    float CalculateMaintainabilityScore();
    float CalculateCompatibilityScore();
    float CalculateResourceUsageScore();
    float CalculatePlayerSatisfactionScore();
    float CalculateCodeQualityScore();
    float CalculateTestCoverageScore();
    void StoreAssessmentResult(const FRiskAssessmentResult& Result);
    void TrimHistoryIfNeeded();
    FString GenerateAssessmentID();
    void NotifyCriticalRisk(const FRiskAssessmentResult& Result);
    void UpdateFactorScores();
    float ApplyScoringRule(float RawValue, const FRiskScoringRule& Rule);
    static UMingRiskAssessment* Get(UObject* WorldContextObject);
);
