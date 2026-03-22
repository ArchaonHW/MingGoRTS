// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Causality Calculator Implementation

#include "MingRTSCausalityCalculator.h"
#include "Math/UnrealMathUtility.h"

UMingRTSCausalityCalculator::UMingRTSCausalityCalculator()
{
    bCalculatorInitialized = false;
}

void UMingRTSCausalityCalculator::InitializeCausalityCalculator()
{
    bCalculatorInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Causality Calculator initialized"));
}

FCausalityAnalysisResult UMingRTSCausalityCalculator::AnalyzeCausality(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent)
{
    FCausalityAnalysisResult Result;
    Result.SourceEventID = SourceEvent.EventID;
    Result.TargetEventID = TargetEvent.EventID;
    
    // Perform analysis based on configured method
    switch (AnalysisConfig.PrimaryMethod)
    {
        case ECausalityAnalysisMethod::TemporalProximity:
            Result = PerformTemporalAnalysis(SourceEvent, TargetEvent);
            break;
        case ECausalityAnalysisMethod::SpatialProximity:
            Result = PerformSpatialAnalysis(SourceEvent, TargetEvent);
            break;
        case ECausalityAnalysisMethod::Hybrid:
            Result = PerformHybridAnalysis(SourceEvent, TargetEvent);
            break;
        default:
            Result = PerformTemporalAnalysis(SourceEvent, TargetEvent);
            break;
    }
    
    return Result;
}

FCausalityAnalysisResult UMingRTSCausalityCalculator::PerformTemporalAnalysis(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent)
{
    FCausalityAnalysisResult Result;
    Result.SourceEventID = SourceEvent.EventID;
    Result.TargetEventID = TargetEvent.EventID;
    Result.AnalysisMethod = ECausalityAnalysisMethod::TemporalProximity;
    
    // Calculate temporal difference
    FTimespan TimeDifference = TargetEvent.Timestamp - SourceEvent.Timestamp;
    float TimeDifferenceDays = TimeDifference.GetTotalDays();
    
    // Calculate temporal weight
    float TemporalWeight = CalculateTemporalWeight(TimeDifference);
    
    Result.CausalStrength = TemporalWeight;
    Result.Confidence = DetermineConfidence(Result.CausalStrength, TArray<FString>());
    Result.Explanation = FString::Printf(TEXT("Temporal analysis: %f days apart, strength: %f"), TimeDifferenceDays, TemporalWeight);
    
    return Result;
}

float UMingRTSCausalityCalculator::CalculateTemporalWeight(const FTimespan& TimeDifference) const
{
    float DaysDifference = TimeDifference.GetTotalDays();
    if (DaysDifference < 0)
    {
        return 0.0f; // Target event before source event
    }
    
    // Exponential decay function
    return FMath::Exp(-DaysDifference / AnalysisConfig.TemporalThreshold);
}
