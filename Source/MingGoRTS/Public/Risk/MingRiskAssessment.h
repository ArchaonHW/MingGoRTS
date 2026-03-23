// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Real-Time Risk Assessment System - B2-2
// Provides real-time risk evaluation and scoring

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskAssessment.generated.h"

UENUM(BlueprintType)
enum class EAssessmentStatus : uint8 {
    Idle                UMETA(DisplayName = "Idle"),
    Assessing           UMETA(DisplayName = "Assessing"),
    Completed           UMETA(DisplayName = "Completed"),
    Failed              UMETA(DisplayName = "Failed"),
    Passed              UMETA(DisplayName = "Passed")
};

UENUM(BlueprintType)
enum class ERiskFactorType : uint8 {
    Performance         UMETA(DisplayName = "Performance"),
    Stability           UMETA(DisplayName = "Stability"),
    Security            UMETA(DisplayName = "Security"),
    Scalability         UMETA(DisplayName = "Scalability"),
    Maintainability     UMETA(DisplayName = "Maintainability"),
    Compatibility       UMETA(DisplayName = "Compatibility"),
    ResourceUse         UMETA(DisplayName = "Resource Use"),
    PlayerSatisfaction  UMETA(DisplayName = "Player Satisfaction"),
    CodeQuality         UMETA(DisplayName = "Code Quality"),
    TestCoverage        UMETA(DisplayName = "Test Coverage")
};

UENUM(BlueprintType)
enum class EAssessmentWeight : uint8 {
    VeryLow             UMETA(DisplayName = "Very Low"),
    Low                 UMETA(DisplayName = "Low"),
    Medium              UMETA(DisplayName = "Medium"),
    High                UMETA(DisplayName = "High"),
    VeryHigh            UMETA(DisplayName = "Very High"),
    Critical            UMETA(DisplayName = "Critical")
};

UENUM(BlueprintType)
enum class ERiskLevel : uint8 {
    None                UMETA(DisplayName = "No Risk"),
    Low                 UMETA(DisplayName = "Low Risk"),
    Medium              UMETA(DisplayName = "Medium Risk"),
    High                UMETA(DisplayName = "High Risk"),
    Critical            UMETA(DisplayName = "Critical Risk"),
    Emergency           UMETA(DisplayName = "Emergency")
};

UENUM(BlueprintType)
enum class ERiskCategory : uint8 {
    Technical           UMETA(DisplayName = "Technical"),
    Schedule            UMETA(DisplayName = "Schedule"),
    Budget              UMETA(DisplayName = "Budget"),
    Quality             UMETA(DisplayName = "Quality"),
    Resource            UMETA(DisplayName = "Resource"),
    External            UMETA(DisplayName = "External"),
    Security            UMETA(DisplayName = "Security"),
    Performance         UMETA(DisplayName = "Performance"),
    General             UMETA(DisplayName = "General")
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
    int32 AssessmentTime;

    UPROPERTY()
    int32 DurationMs;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    bool bEnableRealTimeAssessment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    float AssessmentInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    TArray<ERiskFactorType> EnabledFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    TMap<ERiskFactorType, EAssessmentWeight> FactorWeights;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    bool bAutoGenerateRecommendations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    bool bStoreHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    int32 MaxHistorySize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    float CriticalThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    float HighThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assessment Config")
    float MediumThreshold;

    FAssessmentConfig()
        : bEnableRealTimeAssessment(true)
        , AssessmentInterval(60.0f)
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
    FString ScoringModel;

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
    void PauseRealTimeAssessment();

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void ResumeRealTimeAssessment();

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetFactorWeight(ERiskFactorType Factor, EAssessmentWeight Weight);

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetScoringRule(const FRiskScoringRule& Rule);

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void EnableFactor(ERiskFactorType Factor, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    FRiskAssessmentResult GetLastAssessmentResult() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    TArray<FRiskAssessmentResult> GetAssessmentHistory(int32 Count = 10) const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    float GetCurrentFactorScore(ERiskFactorType Factor) const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    TArray<FRiskFactor> GetAllFactorScores() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    ERiskLevel GetCurrentRiskLevel() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    float GetOverallRiskScore() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    TArray<FString> GetRecommendations() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    TArray<FString> GenerateRecommendations(const FRiskAssessmentResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void ExportAssessmentReport(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Assessment")
    void SetCriticalThresholds(float Critical, float High, float Medium);

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    bool IsRealTimeAssessmentActive() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    EAssessmentStatus GetAssessmentStatus() const;

    UFUNCTION(BlueprintPure, Category = "Risk Assessment")
    FAssessmentConfig GetConfig() const;

protected:
    UPROPERTY()
    FAssessmentConfig Config;

    UPROPERTY()
    FRiskAssessmentResult LastResult;

    UPROPERTY()
    TMap<ERiskFactorType, FRiskScoringRule> ScoringRules;

    UPROPERTY()
    TArray<FRiskAssessmentResult> AssessmentHistory;

    UPROPERTY()
    TMap<ERiskFactorType, float> CurrentFactorScores;

    UPROPERTY()
    FDateTime LastAssessmentTime;

    UPROPERTY()
    EAssessmentStatus CurrentStatus;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssessmentCompleted, const FRiskAssessmentResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFactorScoreChanged, ERiskFactorType, Factor, float, NewScore);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRiskLevelChanged, ERiskLevel, NewLevel, ERiskLevel, OldLevel);

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnAssessmentCompleted OnAssessmentCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnFactorScoreChanged OnFactorScoreChanged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Assessment Events")
    FOnRiskLevelChanged OnRiskLevelChanged;

private:
    void PerformRealTimeAssessment();
    float CalculateFactorScore(ERiskFactorType Factor);
    float CalculateOverallRiskScore(const TArray<FRiskFactor>& Factors);
    ERiskLevel DetermineRiskLevel(float Score);
    void UpdateFactorScores();
    void StoreAssessmentResult(const FRiskAssessmentResult& Result);
    void CleanupOldHistory();
    FString GenerateAssessmentSummary(const FRiskAssessmentResult& Result);
    TArray<FString> GenerateDefaultRecommendations(const FRiskAssessmentResult& Result);
};
