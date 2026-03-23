// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Real-Time Risk Assessment System - B2-2

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

    int32 StartTime = FPlatformTime::Cycles();
    CurrentAssessmentID = GenerateAssessmentID();

    FRiskAssessmentResult Result;
    Result.AssessmentID = CurrentAssessmentID;
    Result.AssessmentTime = FDateTime::Now();

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
        if (GEngine && GEngine->GetWorldFromContextObject(this))
        {
            GEngine->GetWorldFromContextObject(this)->GetTimerManager().SetTimer(
                AssessmentTimer,
                this,
                &UMingRiskAssessment::PerformAssessment,
                Config.AssessmentInterval,
                true);

            UE_LOG(LogRiskAssessment, Log, TEXT("Real-time assessment started (interval: %.1f s)"), 
                Config.AssessmentInterval);
        }
    }
}

void UMingRiskAssessment::StopRealTimeAssessment()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().ClearTimer(AssessmentTimer);
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
            CurrentScores.Add(Factor, 0.0f);
        }
    }
    else
    {
        Config.EnabledFactors.Remove(Factor);
        CurrentScores.Remove(Factor);
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
        Recommendations.Add(TEXT("All risk factors are within acceptable ranges."));
    }

    return Recommendations;
}

void UMingRiskAssessment::ExportAssessmentReport(const FString& FilePath) const
{
    UE_LOG(LogRiskAssessment, Log, TEXT("Exporting assessment report to: %s"), *FilePath);

    FString Report = TEXT("MingGoRTS Risk Assessment Report\n");
    Report += TEXT("=====================================\n\n");
    Report += FString::Printf(TEXT("Assessment ID: %s\n"), *LastResult.AssessmentID);
    Report += FString::Printf(TEXT("Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Duration: %.2f ms\n\n"), LastResult.DurationMs);

    Report += FString::Printf(TEXT("Overall Risk Score: %.1f%%\n"), LastResult.OverallRiskScore);
    Report += FString::Printf(TEXT("Risk Level: %s\n\n"), *UEnum::GetValueAsString(LastResult.OverallLevel));

    Report += TEXT("Factor Breakdown:\n");
    Report += TEXT("-----------------\n");

    for (const auto& Factor : LastResult.Factors)
    {
        Report += FString::Printf(TEXT("- %s: %.1f%% (weight: %.2f)\n"), 
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

    for (const FString& Rec : LastResult.Recommendations)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Rec);
    }

    // In a real implementation, you would save this to a file
    UE_LOG(LogRiskAssessment, Log, TEXT("Report generated:\n%s"), *Report);
}

// Private helper functions

FRiskFactor UMingRiskAssessment::EvaluateFactor(ERiskFactorType FactorType)
{
    FRiskFactor Factor;
    Factor.Type = FactorType;
    Factor.Weight = GetFactorWeight(FactorType);
    
    // In a real implementation, this would perform actual risk factor evaluation
    // For now, we'll use a simple heuristic based on factor type
    switch (FactorType)
    {
        case ERiskFactorType::Performance:
            Factor.Score = EvaluatePerformanceRisk();
            break;
        case ERiskFactorType::Stability:
            Factor.Score = EvaluateStabilityRisk();
            break;
        case ERiskFactorType::Security:
            Factor.Score = EvaluateSecurityRisk();
            break;
        default:
            Factor.Score = 25.0f; // Default moderate risk
            break;
    }

    Factor.Description = GenerateFactorDescription(FactorType, Factor.Score);
    return Factor;
}

float UMingRiskAssessment::GetFactorWeight(ERiskFactorType Factor) const
{
    if (Config.CustomWeights.Contains(Factor))
    {
        return Config.CustomWeights[Factor];
    }
    
    // Default weights
    switch (Factor)
    {
        case ERiskFactorType::Performance: return 0.3f;
        case ERiskFactorType::Stability: return 0.25f;
        case ERiskFactorType::Security: return 0.2f;
        case ERiskFactorType::Scalability: return 0.15f;
        case ERiskFactorType::Maintainability: return 0.1f;
        default: return 0.1f;
    }
}

float UMingRiskAssessment::CalculateWeightedScore(const TArray<FRiskFactor>& Factors)
{
    float TotalScore = 0.0f;
    float TotalWeight = 0.0f;

    for (const auto& Factor : Factors)
    {
        TotalScore += Factor.Score * Factor.Weight;
        TotalWeight += Factor.Weight;
    }

    return TotalWeight > 0.0f ? (TotalScore / TotalWeight) : 0.0f;
}

ERiskLevel UMingRiskAssessment::DetermineRiskLevel(float Score) const
{
    if (Score >= 80.0f) return ERiskLevel::Emergency;
    if (Score >= 60.0f) return ERiskLevel::Critical;
    if (Score >= 40.0f) return ERiskLevel::High;
    if (Score >= 20.0f) return ERiskLevel::Medium;
    if (Score > 0.0f) return ERiskLevel::Low;
    return ERiskLevel::None;
}

FString UMingRiskAssessment::GenerateAssessmentID() const
{
    return FString::Printf(TEXT("RA_%lld"), FDateTime::Now().GetTicks());
}

void UMingRiskAssessment::StoreAssessmentResult(const FRiskAssessmentResult& Result)
{
    AssessmentHistory.Add(Result);
    
    // Keep only the last 100 results
    if (AssessmentHistory.Num() > 100)
    {
        AssessmentHistory.RemoveAt(0);
    }
}

void UMingRiskAssessment::NotifyCriticalRisk(const FRiskAssessmentResult& Result)
{
    FString Message = FString::Printf(TEXT("Critical Risk Detected: %s (%.1f%%)"), 
        *Result.AssessmentID, Result.OverallRiskScore);
    
    UE_LOG(LogRiskAssessment, Warning, TEXT("%s"), *Message);
    
    // In a real implementation, this would trigger alerts, notifications, etc.
}

float UMingRiskAssessment::EvaluatePerformanceRisk()
{
    // Placeholder implementation - would analyze actual performance metrics
    return FMath::RandRange(10.0f, 70.0f);
}

float UMingRiskAssessment::EvaluateStabilityRisk()
{
    // Placeholder implementation - would analyze crash rates, error rates, etc.
    return FMath::RandRange(5.0f, 50.0f);
}

float UMingRiskAssessment::EvaluateSecurityRisk()
{
    // Placeholder implementation - would analyze security vulnerabilities
    return FMath::RandRange(15.0f, 60.0f);
}

FString UMingRiskAssessment::GenerateFactorDescription(ERiskFactorType Factor, float Score) const
{
    switch (Factor)
    {
        case ERiskFactorType::Performance:
            return FString::Printf(TEXT("Performance risk at %.1f%% - Monitor frame rates and response times"), Score);
        case ERiskFactorType::Stability:
            return FString::Printf(TEXT("Stability risk at %.1f%% - Check for crashes and memory leaks"), Score);
        case ERiskFactorType::Security:
            return FString::Printf(TEXT("Security risk at %.1f%% - Review authentication and data protection"), Score);
        default:
            return FString::Printf(TEXT("Risk factor at %.1f%%"), Score);
    }
}
