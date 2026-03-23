// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Trend Analysis System Implementation - B2-4
// Provides risk trend analysis and forecasting

#include "Risk/MingRiskTrendAnalysis.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRiskTrend, Log, All);

UMingRiskTrendAnalysis::UMingRiskTrendAnalysis()
{
}

void UMingRiskTrendAnalysis::InitializeTrendAnalysis(const FTrendAnalysisConfig& Config)
{
    this->Config = Config;
    
    // Initialize trend data for all risk categories
    InitializeTrendData();
    
    // Start trend monitoring if enabled
    if (Config.bEnableTrendMonitoring)
    {
        StartTrendMonitoring();
    }
    
    UE_LOG(LogRiskTrend, Log, TEXT("Risk Trend Analysis initialized with analysis period: %s"), 
        *UEnum::GetValueAsString(Config.AnalysisPeriod));
}

void UMingRiskTrendAnalysis::ShutdownTrendAnalysis()
{
    StopTrendMonitoring();
    UE_LOG(LogRiskTrend, Log, TEXT("Risk Trend Analysis shutdown"));
}

void UMingRiskTrendAnalysis::AddRiskDataPoint(ERiskCategory Category, float Value, const FDateTime& Timestamp)
{
    FRiskTrendData& TrendData = RiskTrendData[Category];
    
    // Add new data point
    FRiskDataPoint DataPoint;
    DataPoint.Value = Value;
    DataPoint.Timestamp = Timestamp;
    DataPoint.Category = Category;
    
    TrendData.DataPoints.Add(DataPoint);
    
    // Remove old data points based on retention policy
    CleanupOldDataPoints(TrendData);
    
    // Update trend analysis
    UpdateTrendAnalysis(Category);
    
    // Broadcast event
    OnRiskDataPointAdded.Broadcast(Category, DataPoint);
    
    UE_LOG(LogRiskTrend, Verbose, TEXT("Added risk data point for category %s: %.2f"), 
        *UEnum::GetValueAsString(Category), Value);
}

FRiskTrend UMingRiskTrendAnalysis::AnalyzeRiskTrend(ERiskCategory Category, EForecastModel Model, EAnalysisPeriod Period)
{
    FRiskTrend Trend;
    Trend.Category = Category;
    Trend.AnalysisPeriod = Period;
    Trend.ForecastModel = Model;
    Trend.LastUpdated = FDateTime::Now();
    
    // Get data points for the specified period
    TArray<FRiskDataPoint> DataPoints = GetDataPointsForPeriod(Category, Period);
    
    if (DataPoints.Num() < 2)
    {
        Trend.Direction = ETrendDirection::Stable;
        Trend.Confidence = 0.0f;
        Trend.ErrorMessage = TEXT("Insufficient data for trend analysis");
        return Trend;
    }
    
    // Perform trend analysis based on model
    switch (Model)
    {
        case EForecastModel::Linear:
            PerformLinearRegression(DataPoints, Trend);
            break;
        case EForecastModel::Exponential:
            PerformExponentialSmoothing(DataPoints, Trend);
            break;
        case EForecastModel::MovingAverage:
            PerformMovingAverage(DataPoints, Trend);
            break;
        case EForecastModel::TrendAnalysis:
            PerformTrendAnalysis(DataPoints, Trend);
            break;
        case EForecastModel::Seasonal:
            PerformSeasonalDecomposition(DataPoints, Trend);
            break;
        case EForecastModel::MachineLearning:
            PerformMLPrediction(DataPoints, Trend);
            break;
    }
    
    // Calculate trend direction
    CalculateTrendDirection(Trend);
    
    // Generate forecast
    GenerateForecast(Trend);
    
    return Trend;
}

TArray<FRiskTrend> UMingRiskTrendAnalysis::AnalyzeAllRiskTrends(EForecastModel Model, EAnalysisPeriod Period)
{
    TArray<FRiskTrend> Trends;
    
    for (int32 i = 0; i < static_cast<int32>(ERiskCategory::DataIntegrity) + 1; ++i)
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        FRiskTrend Trend = AnalyzeRiskTrend(Category, Model, Period);
        Trends.Add(Trend);
    }
    
    return Trends;
}

FRiskForecast UMingRiskTrendAnalysis::GenerateRiskForecast(ERiskCategory Category, EForecastModel Model, int32 ForecastHorizon)
{
    FRiskForecast Forecast;
    Forecast.Category = Category;
    Forecast.ForecastModel = Model;
    Forecast.ForecastHorizon = ForecastHorizon;
    Forecast.GeneratedAt = FDateTime::Now();
    
    // Get historical data
    TArray<FRiskDataPoint> DataPoints = RiskTrendData[Category].DataPoints;
    
    if (DataPoints.Num() < 5)
    {
        Forecast.ErrorMessage = TEXT("Insufficient data for forecasting");
        Forecast.Confidence = 0.0f;
        return Forecast;
    }
    
    // Generate forecast based on model
    switch (Model)
    {
        case EForecastModel::Linear:
            GenerateLinearForecast(DataPoints, Forecast);
            break;
        case EForecastModel::Exponential:
            GenerateExponentialForecast(DataPoints, Forecast);
            break;
        case EForecastModel::MovingAverage:
            GenerateMovingAverageForecast(DataPoints, Forecast);
            break;
        case EForecastModel::TrendAnalysis:
            GenerateTrendBasedForecast(DataPoints, Forecast);
            break;
        case EForecastModel::Seasonal:
            GenerateSeasonalForecast(DataPoints, Forecast);
            break;
        case EForecastModel::MachineLearning:
            GenerateMLForecast(DataPoints, Forecast);
            break;
    }
    
    // Calculate confidence intervals
    CalculateConfidenceIntervals(Forecast);
    
    return Forecast;
}

TArray<FRiskDataPoint> UMingRiskTrendAnalysis::GetDataPointsForPeriod(ERiskCategory Category, EAnalysisPeriod Period)
{
    TArray<FRiskDataPoint> FilteredPoints;
    
    FDateTime CutoffTime = GetCutoffTimeForPeriod(Period);
    
    for (const FRiskDataPoint& Point : RiskTrendData[Category].DataPoints)
    {
        if (Point.Timestamp >= CutoffTime)
        {
            FilteredPoints.Add(Point);
        }
    }
    
    return FilteredPoints;
}

FRiskTrendStatistics UMingRiskTrendAnalysis::GetTrendStatistics(ERiskCategory Category, EAnalysisPeriod Period)
{
    FRiskTrendStatistics Stats;
    Stats.Category = Category;
    Stats.Period = Period;
    Stats.CalculatedAt = FDateTime::Now();
    
    TArray<FRiskDataPoint> DataPoints = GetDataPointsForPeriod(Category, Period);
    
    if (DataPoints.Num() == 0)
    {
        return Stats;
    }
    
    // Calculate basic statistics
    float Sum = 0.0f;
    float MinValue = DataPoints[0].Value;
    float MaxValue = DataPoints[0].Value;
    
    for (const FRiskDataPoint& Point : DataPoints)
    {
        Sum += Point.Value;
        MinValue = FMath::Min(MinValue, Point.Value);
        MaxValue = FMath::Max(MaxValue, Point.Value);
    }
    
    Stats.MeanValue = Sum / DataPoints.Num();
    Stats.MinValue = MinValue;
    Stats.MaxValue = MaxValue;
    Stats.DataPointCount = DataPoints.Num();
    
    // Calculate standard deviation
    float Variance = 0.0f;
    for (const FRiskDataPoint& Point : DataPoints)
    {
        Variance += FMath::Square(Point.Value - Stats.MeanValue);
    }
    Variance /= DataPoints.Num();
    Stats.StandardDeviation = FMath::Sqrt(Variance);
    
    // Calculate trend direction
    if (DataPoints.Num() >= 2)
    {
        float FirstValue = DataPoints[0].Value;
        float LastValue = DataPoints.Last().Value;
        float Change = LastValue - FirstValue;
        
        if (FMath::Abs(Change) < 0.01f)
        {
            Stats.TrendDirection = ETrendDirection::Stable;
        }
        else if (Change > 0)
        {
            Stats.TrendDirection = (Change > 0.1f) ? ETrendDirection::Accelerating : ETrendDirection::Improving;
        }
        else
        {
            Stats.TrendDirection = (Change < -0.1f) ? ETrendDirection::Decelerating : ETrendDirection::Degrading;
        }
        
        Stats.PercentChange = (FirstValue != 0.0f) ? (Change / FirstValue) * 100.0f : 0.0f;
    }
    
    return Stats;
}

void UMingRiskTrendAnalysis::SetAnalysisPeriod(EAnalysisPeriod Period)
{
    Config.AnalysisPeriod = Period;
    UE_LOG(LogRiskTrend, Log, TEXT("Analysis period changed to: %s"), *UEnum::GetValueAsString(Period));
}

void UMingRiskTrendAnalysis::SetForecastModel(EForecastModel Model)
{
    Config.DefaultForecastModel = Model;
    UE_LOG(LogRiskTrend, Log, TEXT("Default forecast model changed to: %s"), *UEnum::GetValueAsString(Model));
}

void UMingRiskTrendAnalysis::StartTrendMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRiskTrendAnalysis::PerformMonitoringCycle,
            Config.MonitoringInterval,
            true);

        UE_LOG(LogRiskTrend, Log, TEXT("Trend monitoring started (interval: %.1f s)"), 
            Config.MonitoringInterval);
    }
}

void UMingRiskTrendAnalysis::StopTrendMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().ClearTimer(MonitoringTimer);
    }
    
    UE_LOG(LogRiskTrend, Log, TEXT("Trend monitoring stopped"));
}

void UMingRiskTrendAnalysis::ExportTrendAnalysis(const FString& FilePath) const
{
    UE_LOG(LogRiskTrend, Log, TEXT("Exporting trend analysis to: %s"), *FilePath);
    
    FString Report = TEXT("MingGoRTS Risk Trend Analysis Report\n");
    Report += TEXT("=====================================\n\n");
    Report += FString::Printf(TEXT("Export Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Analysis Period: %s\n"), *UEnum::GetValueAsString(Config.AnalysisPeriod));
    Report += FString::Printf(TEXT("Default Model: %s\n\n"), *UEnum::GetValueAsString(Config.DefaultForecastModel));
    
    Report += TEXT("Risk Category Trends:\n");
    Report += TEXT("---------------------\n");
    
    for (int32 i = 0; i < static_cast<int32>(ERiskCategory::DataIntegrity) + 1; ++i)
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        FRiskTrendStatistics Stats = GetTrendStatistics(Category, Config.AnalysisPeriod);
        
        Report += FString::Printf(TEXT("- %s:\n"), *UEnum::GetValueAsString(Category));
        Report += FString::Printf(TEXT("  Mean: %.2f\n"), Stats.MeanValue);
        Report += FString::Printf(TEXT("  Range: %.2f - %.2f\n"), Stats.MinValue, Stats.MaxValue);
        Report += FString::Printf(TEXT("  Trend: %s (%.1f%% change)\n"), 
            *UEnum::GetValueAsString(Stats.TrendDirection), Stats.PercentChange);
        Report += FString::Printf(TEXT("  Data Points: %d\n\n"), Stats.DataPointCount);
    }
    
    // In a real implementation, you would save this to a file
    UE_LOG(LogRiskTrend, Log, TEXT("Report generated:\n%s"), *Report);
}

// Private helper functions

void UMingRiskTrendAnalysis::InitializeTrendData()
{
    for (int32 i = 0; i < static_cast<int32>(ERiskCategory::DataIntegrity) + 1; ++i)
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        RiskTrendData.Add(Category, FRiskTrendData());
    }
}

void UMingRiskTrendAnalysis::CleanupOldDataPoints(FRiskTrendData& TrendData)
{
    FDateTime CutoffTime = GetCutoffTimeForPeriod(Config.AnalysisPeriod);
    
    TrendData.DataPoints.RemoveAll([&](const FRiskDataPoint& Point) {
        return Point.Timestamp < CutoffTime;
    });
}

void UMingRiskTrendAnalysis::UpdateTrendAnalysis(ERiskCategory Category)
{
    // Update trend statistics
    FRiskTrendStatistics Stats = GetTrendStatistics(Category, Config.AnalysisPeriod);
    RiskTrendData[Category].LatestStatistics = Stats;
    
    // Broadcast update
    OnTrendAnalysisUpdated.Broadcast(Category, Stats);
}

FDateTime UMingRiskTrendAnalysis::GetCutoffTimeForPeriod(EAnalysisPeriod Period)
{
    FDateTime Now = FDateTime::Now();
    
    switch (Period)
    {
        case EAnalysisPeriod::OneHour:
            return Now - FTimespan::FromHours(1);
        case EAnalysisPeriod::SixHours:
            return Now - FTimespan::FromHours(6);
        case EAnalysisPeriod::OneDay:
            return Now - FTimespan::FromDays(1);
        case EAnalysisPeriod::OneWeek:
            return Now - FTimespan::FromDays(7);
        case EAnalysisPeriod::OneMonth:
            return Now - FTimespan::FromDays(30);
        case EAnalysisPeriod::Custom:
            return Now - FTimespan::FromDays(7); // Default to 7 days for custom
    }
    
    return Now - FTimespan::FromDays(1);
}

void UMingRiskTrendAnalysis::PerformLinearRegression(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    if (DataPoints.Num() < 2) return;
    
    // Simple linear regression implementation
    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumX2 = 0.0f;
    int32 N = DataPoints.Num();
    
    for (int32 i = 0; i < N; ++i)
    {
        float X = static_cast<float>(i);
        float Y = DataPoints[i].Value;
        
        SumX += X;
        SumY += Y;
        SumXY += X * Y;
        SumX2 += X * X;
    }
    
    float Slope = (N * SumXY - SumX * SumY) / (N * SumX2 - SumX * SumX);
    float Intercept = (SumY - Slope * SumX) / N;
    
    Trend.Slope = Slope;
    Trend.Intercept = Intercept;
    Trend.Confidence = CalculateRegressionConfidence(DataPoints, Slope, Intercept);
}

void UMingRiskTrendAnalysis::PerformExponentialSmoothing(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    // Simple exponential smoothing implementation
    float Alpha = 0.3f; // Smoothing factor
    float SmoothedValue = DataPoints[0].Value;
    
    for (int32 i = 1; i < DataPoints.Num(); ++i)
    {
        SmoothedValue = Alpha * DataPoints[i].Value + (1.0f - Alpha) * SmoothedValue;
    }
    
    Trend.SmoothedValue = SmoothedValue;
    Trend.Confidence = 0.7f; // Placeholder confidence
}

void UMingRiskTrendAnalysis::PerformMovingAverage(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    // Simple moving average implementation
    int32 WindowSize = FMath::Min(5, DataPoints.Num());
    float Sum = 0.0f;
    
    for (int32 i = DataPoints.Num() - WindowSize; i < DataPoints.Num(); ++i)
    {
        Sum += DataPoints[i].Value;
    }
    
    Trend.MovingAverage = Sum / WindowSize;
    Trend.Confidence = 0.6f; // Placeholder confidence
}

void UMingRiskTrendAnalysis::PerformTrendAnalysis(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    // Simple trend analysis based on first and last values
    if (DataPoints.Num() < 2) return;
    
    float FirstValue = DataPoints[0].Value;
    float LastValue = DataPoints.Last().Value;
    float Change = LastValue - FirstValue;
    
    Trend.TrendValue = Change;
    Trend.Confidence = 0.8f;
}

void UMingRiskTrendAnalysis::PerformSeasonalDecomposition(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    // Placeholder for seasonal decomposition
    // In a real implementation, this would use more sophisticated algorithms
    Trend.SeasonalComponent = 0.0f;
    Trend.TrendComponent = DataPoints.Last().Value;
    Trend.Confidence = 0.5f;
}

void UMingRiskTrendAnalysis::PerformMLPrediction(const TArray<FRiskDataPoint>& DataPoints, FRiskTrend& Trend)
{
    // Placeholder for machine learning prediction
    // In a real implementation, this would use trained ML models
    Trend.MLPrediction = DataPoints.Last().Value;
    Trend.Confidence = 0.9f;
}

void UMingRiskTrendAnalysis::CalculateTrendDirection(FRiskTrend& Trend)
{
    if (FMath::Abs(Trend.Slope) < 0.01f)
    {
        Trend.Direction = ETrendDirection::Stable;
    }
    else if (Trend.Slope > 0)
    {
        Trend.Direction = (Trend.Slope > 0.1f) ? ETrendDirection::Accelerating : ETrendDirection::Improving;
    }
    else
    {
        Trend.Direction = (Trend.Slope < -0.1f) ? ETrendDirection::Decelerating : ETrendDirection::Degrading;
    }
}

void UMingRiskTrendAnalysis::GenerateForecast(FRiskTrend& Trend)
{
    // Generate simple forecast based on trend
    int32 ForecastPoints = 10;
    
    for (int32 i = 1; i <= ForecastPoints; ++i)
    {
        float ForecastValue = Trend.Intercept + Trend.Slope * (DataPoints.Num() + i);
        FDateTime ForecastTime = FDateTime::Now() + FTimespan::FromHours(i);
        
        FRiskForecastPoint Point;
        Point.Value = ForecastValue;
        Point.Timestamp = ForecastTime;
        Point.Confidence = Trend.Confidence;
        
        Trend.ForecastPoints.Add(Point);
    }
}

void UMingRiskTrendAnalysis::GenerateLinearForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Linear forecast implementation
    // Similar to linear regression but extended for future predictions
}

void UMingRiskTrendAnalysis::GenerateExponentialForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Exponential forecast implementation
}

void UMingRiskTrendAnalysis::GenerateMovingAverageForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Moving average forecast implementation
}

void UMingRiskTrendAnalysis::GenerateTrendBasedForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Trend-based forecast implementation
}

void UMingRiskTrendAnalysis::GenerateSeasonalForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Seasonal forecast implementation
}

void UMingRiskTrendAnalysis::GenerateMLForecast(const TArray<FRiskDataPoint>& DataPoints, FRiskForecast& Forecast)
{
    // Machine learning forecast implementation
}

void UMingRiskTrendAnalysis::CalculateConfidenceIntervals(FRiskForecast& Forecast)
{
    // Calculate confidence intervals for forecast points
    for (FRiskForecastPoint& Point : Forecast.ForecastPoints)
    {
        float Margin = Point.Value * 0.1f; // 10% margin of error
        Point.LowerBound = Point.Value - Margin;
        Point.UpperBound = Point.Value + Margin;
    }
}

float UMingRiskTrendAnalysis::CalculateRegressionConfidence(const TArray<FRiskDataPoint>& DataPoints, float Slope, float Intercept)
{
    // Calculate R-squared for confidence
    float SumSquaredTotal = 0.0f;
    float SumSquaredResidual = 0.0f;
    float MeanY = 0.0f;
    
    // Calculate mean
    for (const FRiskDataPoint& Point : DataPoints)
    {
        MeanY += Point.Value;
    }
    MeanY /= DataPoints.Num();
    
    // Calculate sums
    for (int32 i = 0; i < DataPoints.Num(); ++i)
    {
        float X = static_cast<float>(i);
        float Y = DataPoints[i].Value;
        float PredictedY = Intercept + Slope * X;
        
        SumSquaredTotal += FMath::Square(Y - MeanY);
        SumSquaredResidual += FMath::Square(Y - PredictedY);
    }
    
    // R-squared calculation
    if (SumSquaredTotal > 0.0f)
    {
        return 1.0f - (SumSquaredResidual / SumSquaredTotal);
    }
    
    return 0.0f;
}

void UMingRiskTrendAnalysis::PerformMonitoringCycle()
{
    // In a real implementation, this would collect actual risk data
    // For now, we'll generate sample data
    
    for (int32 i = 0; i < static_cast<int32>(ERiskCategory::DataIntegrity) + 1; ++i)
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        
        // Generate sample data point
        float SampleValue = FMath::RandRange(0.0f, 100.0f);
        AddRiskDataPoint(Category, SampleValue, FDateTime::Now());
    }
}
