// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Real-Time Risk Assessment System Implementation - B2-2

#include "Risk/MingRiskAssessment.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRiskAssessment, Log, All);

UMingRiskAssessment::UMingRiskAssessment()
    : CurrentStatus(EAssessmentStatus::Idle)
{
}

void UMingRiskAssessment::InitializeAssessment(const FAssessmentConfig& Config)
{
    this->Config = Config;
    CurrentStatus = EAssessmentStatus::Idle;

    UE_LOG(LogRiskAssessment, Log, TEXT("Risk Assessment initialized with %d enabled factors"),
        Config.EnabledFactors.Num());

    // Initialize scores
    for (const auto& Factor : Config.EnabledFactors)
    {
        CurrentScores.Add(Factor, 0.0f);
    }

    // Initialize default scoring rules
    for (const auto& Factor : Config.EnabledFactors)
    {
        FRiskScoringRule Rule;
        Rule.FactorType = Factor;
        ScoringRules.Add(Factor, Rule);
    }
}

void UMingRiskAssessment::ShutdownAssessment()
{
    StopRealTimeAssessment();
    CurrentStatus = EAssessmentStatus::Idle;
    UE_LOG(LogRiskAssessment, Log, TEXT("Risk Assessment shutdown"));
}

FRiskAssessmentResult UMingRiskAssessment::PerformAssessment()
{
    EAssessmentStatus OldStatus = CurrentStatus;
    CurrentStatus = EAssessmentStatus::Assessing;
    OnAssessmentStatusChanged.Broadcast(CurrentStatus);

    uint32 StartTime = FPlatformTime::Cycles();
    CurrentAssessmentID = GenerateAssessmentID();

    FRiskAssessmentResult Result;
    Result.AssessmentID = CurrentAssessmentID;
    Result.AssessmentTime = FPlatformTime::Seconds();

    TArray<FRiskFactor> Factors;

    // Evaluate each enabled factor
    for (const auto& FactorType : Config.EnabledFactors)
    {
        FRiskFactor Factor = EvaluateFactor(FactorType);
        Factors.Add(Factor);
        CurrentScores.Add(FactorType, Factor.Score);
    }

    Result.Factors = Factors;
    Result.OverallRiskScore = CalculateWeightedScore(Factors);
    Result.OverallLevel = DetermineRiskLevel(Result.OverallRiskScore);

    // Generate category scores
    for (const auto& Factor : Factors)
    {
        Result.CategoryScores.Add(Factor.Type, Factor.Score);
    }

    // Generate recommendations
    if (Config.bAutoGenerateRecommendations)
    {
        Result.Recommendations = GenerateRecommendations(Result);
    }

    Result.DurationMs = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles() - StartTime);
    Result.Summary = FString::Printf(TEXT("Overall Risk: %.1f%% (%s)"),
        Result.OverallRiskScore, *UEnum::GetValueAsString(Result.OverallLevel));

    LastResult = Result;
    StoreAssessmentResult(Result);

    CurrentStatus = EAssessmentStatus::Completed;
    OnAssessmentCompleted.Broadcast(Result);
    OnAssessmentStatusChanged.Broadcast(CurrentStatus);

    // Check for critical risk
    if (Result.OverallLevel >= ERiskLevel::Critical)
    {
        NotifyCriticalRisk(Result);
    }

    UE_LOG(LogRiskAssessment, Log, TEXT("Assessment completed: %s (%.1f%%) in %.2f ms"),
        *Result.AssessmentID, Result.OverallRiskScore, Result.DurationMs);

    return Result;
}

void UMingRiskAssessment::StartRealTimeAssessment()
{
    if (Config.bEnableRealTimeAssessment && CurrentStatus != EAssessmentStatus::Assessing)
    {
        if (GEngine && GEngine->GetCurrentWorldContext())
        {
            GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
                AssessmentTimer,
                this,
                &UMingRiskAssessment::PerformAssessment,
                Config.AssessmentInterval,
                true
            );

            UE_LOG(LogRiskAssessment, Log, TEXT("Real-time assessment started (interval: %.1f s)"),
                Config.AssessmentInterval);

            PerformAssessment();
        }
    }
}

void UMingRiskAssessment::StopRealTimeAssessment()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(AssessmentTimer);
    }

    UE_LOG(LogRiskAssessment, Log, TEXT("Real-time assessment stopped"));
}

void UMingRiskAssessment::PauseAssessment()
{
    if (CurrentStatus == EAssessmentStatus::Assessing)
    {
        CurrentStatus = EAssessmentStatus::Paused;
        OnAssessmentStatusChanged.Broadcast(CurrentStatus);
        UE_LOG(LogRiskAssessment, Log, TEXT("Assessment paused"));
    }
}

void UMingRiskAssessment::ResumeAssessment()
{
    if (CurrentStatus == EAssessmentStatus::Paused)
    {
        CurrentStatus = EAssessmentStatus::Idle;
        OnAssessmentStatusChanged.Broadcast(CurrentStatus);
        UE_LOG(LogRiskAssessment, Log, TEXT("Assessment resumed"));
    }
}

void UMingRiskAssessment::SetFactorWeight(ERiskFactorType Factor, float Weight)
{
    Config.CustomWeights.Add(Factor, Weight);
    UE_LOG(LogRiskAssessment, Log, TEXT("Set weight for factor %s: %.2f"),
        *UEnum::GetValueAsString(Factor), Weight);
}

void UMingRiskAssessment::EnableFactor(ERiskFactorType Factor, bool bEnabled)
{
    if (bEnabled)
    {
        if (!Config.EnabledFactors.Contains(Factor))
        {
            Config.EnabledFactors.Add(Factor);
            UE_LOG(LogRiskAssessment, Log, TEXT("Enabled factor: %s"), *UEnum::GetValueAsString(Factor));
        }
    }
    else
    {
        Config.EnabledFactors.Remove(Factor);
        UE_LOG(LogRiskAssessment, Log, TEXT("Disabled factor: %s"), *UEnum::GetValueAsString(Factor));
    }
}

void UMingRiskAssessment::SetScoringRule(ERiskFactorType Factor, const FRiskScoringRule& Rule)
{
    ScoringRules.Add(Factor, Rule);
    UE_LOG(LogRiskAssessment, Log, TEXT("Set scoring rule for factor %s"),
        *UEnum::GetValueAsString(Factor));
}

FRiskAssessmentResult UMingRiskAssessment::GetLastAssessmentResult() const
{
    return LastResult;
}

TArray<FRiskAssessmentResult> UMingRiskAssessment::GetAssessmentHistory(int32 Count) const
{
    int32 StartIndex = FMath::Max(0, AssessmentHistory.Num() - Count);
    TArray<FRiskAssessmentResult> Result;

    for (int32 i = StartIndex; i < AssessmentHistory.Num(); ++i)
    {
        Result.Add(AssessmentHistory[i]);
    }

    return Result;
}

float UMingRiskAssessment::GetCurrentFactorScore(ERiskFactorType Factor) const
{
    if (CurrentScores.Contains(Factor))
    {
        return CurrentScores[Factor];
    }
    return 0.0f;
}

TMap<ERiskFactorType, float> UMingRiskAssessment::GetAllFactorScores() const
{
    return CurrentScores;
}

ERiskLevel UMingRiskAssessment::GetCurrentOverallRiskLevel() const
{
    return LastResult.OverallLevel;
}

TArray<FString> UMingRiskAssessment::GenerateRecommendations(const FRiskAssessmentResult& Result)
{
    TArray<FString> Recommendations;

    for (const auto& Factor : Result.Factors)
    {
        if (Factor.Score >= Config.HighThreshold)
        {
            FString Rec = FString::Printf(TEXT("Address %s: Score %.1f%% - %s"),
                *UEnum::GetValueAsString(Factor.Type),
                Factor.Score,
                *Factor.Description);
            Recommendations.Add(Rec);
        }
    }

    if (Recommendations.Num() == 0)
    {
        Recommendations.Add(TEXT("All risk factors within acceptable ranges."));
    }

    return Recommendations;
}

void UMingRiskAssessment::ExportAssessmentReport(const FString& FilePath)
{
    UE_LOG(LogRiskAssessment, Log, TEXT("Exporting assessment report to: %s"), *FilePath);

    FString Report = TEXT("MingGoRTS Risk Assessment Report\n");
    Report += TEXT("=============================\n\n");
    Report += FString::Printf(TEXT("Assessment ID: %s\n"), *LastResult.AssessmentID);
    Report += FString::Printf(TEXT("Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Duration: %.2f ms\n\n"), LastResult.DurationMs);

    Report += FString::Printf(TEXT("Overall Risk Score: %.1f%%\n"), LastResult.OverallRiskScore);
    Report += FString::Printf(TEXT("Risk Level: %s\n\n"), *UEnum::GetValueAsString(LastResult.OverallLevel));

    Report += TEXT("Factor Breakdown:\n");
    Report += TEXT("----------------\n");

    for (const auto& Factor : LastResult.Factors)
    {
        Report += FString::Printf(TEXT("- %s: %.1f%% (Weight: %.2f)\n"),
            *UEnum::GetValueAsString(Factor.Type),
            Factor.Score,
            Factor.Weight);
        if (!Factor.Description.IsEmpty())
        {
            Report += FString::Printf(TEXT("  Description: %s\n"), *Factor.Description);
        }
    }

    Report += TEXT("\nRecommendations:\n");
    Report += TEXT("----------------\n");

    for (const auto& Rec : LastResult.Recommendations)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Rec);
    }

    FFileHelper::SaveStringToFile(Report, *FilePath);
}

void UMingRiskAssessment::SetCriticalThresholds(float Critical, float High, float Medium)
{
    Config.CriticalThreshold = Critical;
    Config.HighThreshold = High;
    Config.MediumThreshold = Medium;

    UE_LOG(LogRiskAssessment, Log, TEXT("Updated thresholds: Critical=%.1f, High=%.1f, Medium=%.1f"),
        Critical, High, Medium);
}

bool UMingRiskAssessment::IsRealTimeAssessmentActive() const
{
    return CurrentStatus == EAssessmentStatus::Assessing;
}

float UMingRiskAssessment::CalculateWeightedScore(const TArray<FRiskFactor>& Factors)
{
    float TotalWeight = 0.0f;
    float WeightedSum = 0.0f;

    for (const auto& Factor : Factors)
    {
        float Weight = Factor.Weight;

        // Apply custom weight if configured
        if (Config.CustomWeights.Contains(Factor.Type))
        {
            Weight = Config.CustomWeights[Factor.Type];
        }

        WeightedSum += Factor.Score * Weight;
        TotalWeight += Weight;
    }

    return TotalWeight > 0.0f ? WeightedSum / TotalWeight : 0.0f;
}

ERiskLevel UMingRiskAssessment::DetermineRiskLevel(float Score)
{
    if (Score >= Config.CriticalThreshold)
    {
        return ERiskLevel::Critical;
    }
    else if (Score >= Config.HighThreshold)
    {
        return ERiskLevel::High;
    }
    else if (Score >= Config.MediumThreshold)
    {
        return ERiskLevel::Medium;
    }
    else if (Score > 0.0f)
    {
        return ERiskLevel::Low;
    }
    return ERiskLevel::None;
}

FRiskFactor UMingRiskAssessment::EvaluateFactor(ERiskFactorType FactorType)
{
    FRiskFactor Factor;
    Factor.Type = FactorType;

    switch (FactorType)
    {
    case ERiskFactorType::Performance:
        Factor.Score = CalculatePerformanceScore();
        Factor.Description = TEXT("System performance metrics");
        break;
    case ERiskFactorType::Stability:
        Factor.Score = CalculateStabilityScore();
        Factor.Description = TEXT("System stability assessment");
        break;
    case ERiskFactorType::Security:
        Factor.Score = CalculateSecurityScore();
        Factor.Description = TEXT("Security risk evaluation");
        break;
    case ERiskFactorType::Scalability:
        Factor.Score = CalculateScalabilityScore();
        Factor.Description = TEXT("Scalability capacity analysis");
        break;
    case ERiskFactorType::Maintainability:
        Factor.Score = CalculateMaintainabilityScore();
        Factor.Description = TEXT("Code maintainability assessment");
        break;
    case ERiskFactorType::Compatibility:
        Factor.Score = CalculateCompatibilityScore();
        Factor.Description = TEXT("Compatibility check results");
        break;
    case ERiskFactorType::ResourceUsage:
        Factor.Score = CalculateResourceUsageScore();
        Factor.Description = TEXT("Resource utilization analysis");
        break;
    case ERiskFactorType::PlayerSatisfaction:
        Factor.Score = CalculatePlayerSatisfactionScore();
        Factor.Description = TEXT("Player satisfaction metrics");
        break;
    case ERiskFactorType::CodeQuality:
        Factor.Score = CalculateCodeQualityScore();
        Factor.Description = TEXT("Code quality evaluation");
        break;
    case ERiskFactorType::TestCoverage:
        Factor.Score = CalculateTestCoverageScore();
        Factor.Description = TEXT("Test coverage analysis");
        break;
    default:
        Factor.Score = 0.0f;
        Factor.Description = TEXT("Unknown factor");
        break;
    }

    Factor.Weight = 1.0f;
    Factor.Importance = EAssessmentWeight::Medium;

    return Factor;
}

float UMingRiskAssessment::CalculatePerformanceScore()
{
    // Placeholder: Would evaluate actual performance metrics
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateStabilityScore()
{
    // Placeholder: Would evaluate system stability
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateSecurityScore()
{
    // Placeholder: Would evaluate security status
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateScalabilityScore()
{
    // Placeholder: Would evaluate scalability capacity
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateMaintainabilityScore()
{
    // Placeholder: Would evaluate code maintainability
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateCompatibilityScore()
{
    // Placeholder: Would evaluate compatibility
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateResourceUsageScore()
{
    // Placeholder: Would evaluate resource usage
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculatePlayerSatisfactionScore()
{
    // Placeholder: Would evaluate player satisfaction
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateCodeQualityScore()
{
    // Placeholder: Would evaluate code quality
    return FMath::RandRange(0.0f, 100.0f);
}

float UMingRiskAssessment::CalculateTestCoverageScore()
{
    // Placeholder: Would evaluate test coverage
    return FMath::RandRange(0.0f, 100.0f);
}

void UMingRiskAssessment::StoreAssessmentResult(const FRiskAssessmentResult& Result)
{
    if (Config.bStoreHistory)
    {
        AssessmentHistory.Add(Result);
        TrimHistoryIfNeeded();
    }
}

void UMingRiskAssessment::TrimHistoryIfNeeded()
{
    while (AssessmentHistory.Num() > Config.MaxHistorySize)
    {
        AssessmentHistory.RemoveAt(0);
    }
}

FString UMingRiskAssessment::GenerateAssessmentID()
{
    return FString::Printf(TEXT("RA-%d-%d"), static_cast<int32>(FPlatformTime::Seconds()), FMath::RandRange(1000, 9999));
}

void UMingRiskAssessment::NotifyCriticalRisk(const FRiskAssessmentResult& Result)
{
    OnCriticalRiskDetected.Broadcast(Result);
    UE_LOG(LogRiskAssessment, Error, TEXT("CRITICAL RISK DETECTED: Score %.1f%%"), Result.OverallRiskScore);
}

void UMingRiskAssessment::UpdateFactorScores()
{
    for (const auto& FactorType : Config.EnabledFactors)
    {
        FRiskFactor Factor = EvaluateFactor(FactorType);
        float NewScore = Factor.Score;

        if (CurrentScores.Contains(FactorType))
        {
            float OldScore = CurrentScores[FactorType];
            if (FMath::Abs(NewScore - OldScore) > 1.0f)
            {
                OnFactorScoreChanged.Broadcast(FactorType, NewScore);
            }
        }

        CurrentScores.Add(FactorType, NewScore);
    }
}

float UMingRiskAssessment::ApplyScoringRule(float RawValue, const FRiskScoringRule& Rule)
{
    if (Rule.bInverseScoring)
    {
        return 100.0f - RawValue;
    }
    return RawValue;
}

static UMingRiskAssessment* UMingRiskAssessment::Get(UObject* WorldContextObject)
{
    static UMingRiskAssessment* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingRiskAssessment>();
        Instance->AddToRoot();
    }
    return Instance;
}
