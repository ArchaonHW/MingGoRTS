// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Trend Analysis System Implementation - B2-4

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

    UE_LOG(LogRiskTrend, Log, TEXT("Risk Trend Analysis initialized"));
    UE_LOG(LogRiskTrend, Log, TEXT("Analysis period: %s, Model: %s"),
        *UEnum::GetValueAsString(Config.DefaultPeriod),
        *UEnum::GetValueAsString(Config.DefaultModel));

    // Initialize historical data containers for all risk categories
    for (int32 i = 0; i <= static_cast<int32>(ERiskCategory::DataIntegrity); ++i)
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        HistoricalData.Add(Category, TArray<FTimeSeriesData>());
        CategoryModels.Add(Category, Config.DefaultModel);
        PreviousDirections.Add(Category, ETrendDirection::Stable);
    }

    if (Config.bEnableAutoAnalysis)
    {
        StartAutoAnalysis();
    }
}

void UMingRiskTrendAnalysis::ShutdownTrendAnalysis()
{
    StopAutoAnalysis();
    UE_LOG(LogRiskTrend, Log, TEXT("Risk Trend Analysis shutdown"));
}

void UMingRiskTrendAnalysis::AddDataPoint(ERiskCategory Category, uint32 Timestamp, float Value, const FString& Label)
{
    if (!HistoricalData.Contains(Category))
    {
        HistoricalData.Add(Category, TArray<FTimeSeriesData>());
    }

    FTimeSeriesData DataPoint;
    DataPoint.Timestamp = Timestamp;
    DataPoint.Value = Value;
    DataPoint.Label = Label;

    HistoricalData[Category].Add(DataPoint);

    // Trim if needed
    if (Config.bStoreHistoricalData)
    {
        TrimHistoricalDataIfNeeded();
    }

    UE_LOG(LogRiskTrend, VeryVerbose, TEXT("Added data point for %s: %.2f at %d"),
        *UEnum::GetValueAsString(Category), Value, Timestamp);
}

FRiskTrend UMingRiskTrendAnalysis::AnalyzeTrend(ERiskCategory Category, EAnalysisPeriod Period)
{
    FRiskTrend Trend;
    Trend.Category = Category;

    if (!HistoricalData.Contains(Category) || HistoricalData[Category].Num() < Config.MinDataPoints)
    {
        Trend.TrendDescription = TEXT("Insufficient data for trend analysis");
        Trend.Direction = ETrendDirection::Stable;
        return Trend;
    }

    const TArray<FTimeSeriesData>& Data = HistoricalData[Category];

    uint32 PeriodDuration = GetPeriodDuration(Period);
    uint32 CurrentTime = FPlatformTime::Seconds();
    uint32 PeriodStartTime = CurrentTime - PeriodDuration;

    // Filter data for the specified period
    TArray<FTimeSeriesData> PeriodData;
    for (const auto& Point : Data)
    {
        if (Point.Timestamp >= PeriodStartTime)
        {
            PeriodData.Add(Point);
        }
    }

    if (PeriodData.Num() < Config.MinDataPoints)
    {
        PeriodData = Data;
        if (PeriodData.Num() > 100)
        {
            PeriodData = TArray<FTimeSeriesData>(PeriodData.GetData() + PeriodData.Num() - 100, 100);
        }
    }

    // Calculate statistics
    Trend.DataPoints = PeriodData.Num();
    Trend.CurrentValue = PeriodData.Last().Value;
    Trend.PreviousValue = PeriodData.Num() > 1 ? PeriodData[PeriodData.Num() - 2].Value : Trend.CurrentValue;

    float Sum = 0.0f;
    Trend.MinValue = PeriodData[0].Value;
    Trend.MaxValue = PeriodData[0].Value;

    for (const auto& Point : PeriodData)
    {
        Sum += Point.Value;
        Trend.MinValue = FMath::Min(Trend.MinValue, Point.Value);
        Trend.MaxValue = FMath::Max(Trend.MaxValue, Point.Value);
    }

    Trend.AverageValue = Sum / PeriodData.Num();
    Trend.Volatility = CalculateVolatility(PeriodData);

    // Calculate trend direction
    float Slope = CalculateTrendSlope(PeriodData);
    float PreviousSlope = 0.0f;

    if (PreviousDirections.Contains(Category))
    {
        ETrendDirection PrevDir = PreviousDirections[Category];
        PreviousSlope = (PrevDir == ETrendDirection::Improving || PrevDir == ETrendDirection::Accelerating) ? 1.0f : -1.0f;
    }

    Trend.Direction = DetermineTrendDirection(Slope, PreviousSlope, Trend.Volatility);
    Trend.ChangeRate = Slope;
    Trend.TrendColor = GetTrendColor(Trend.Direction);

    // Generate description
    Trend.TrendDescription = GenerateTrendDescription(Trend);

    // Store current trend
    CurrentTrends.Add(Category, Trend);
    PreviousDirections.Add(Category, Trend.Direction);

    // Notify if significant trend change
    if (Trend.Direction == ETrendDirection::Degrading || Trend.Direction == ETrendDirection::Accelerating)
    {
        if (Trend.CurrentValue > 60.0f)
        {
            NotifyCriticalTrend(Trend);
        }
    }

    NotifyTrendDetected(Trend);

    UE_LOG(LogRiskTrend, Verbose, TEXT("Trend analyzed for %s: %s (change rate: %.2f)"),
        *UEnum::GetValueAsString(Category),
        *UEnum::GetValueAsString(Trend.Direction),
        Trend.ChangeRate);

    return Trend;
}

TArray<FRiskTrend> UMingRiskTrendAnalysis::AnalyzeAllTrends(EAnalysisPeriod Period)
{
    TArray<FRiskTrend> Results;

    for (const auto& Pair : HistoricalData)
    {
        if (Pair.Value.Num() >= Config.MinDataPoints)
        {
            Results.Add(AnalyzeTrend(Pair.Key, Period));
        }
    }

    return Results;
}

FRiskForecast UMingRiskTrendAnalysis::GenerateForecast(ERiskCategory Category, float HoursAhead)
{
    FRiskForecast Forecast;
    Forecast.Category = Category;
    Forecast.PredictionTime = FPlatformTime::Seconds() + HoursAhead * 3600;

    if (!HistoricalData.Contains(Category) || HistoricalData[Category].Num() < Config.MinDataPoints)
    {
        Forecast.ForecastRationale = TEXT("Insufficient data for forecasting");
        return Forecast;
    }

    const TArray<FTimeSeriesData>& Data = HistoricalData[Category];

    // Use appropriate model
    EForecastModel Model = Config.DefaultModel;
    if (CategoryModels.Contains(Category))
    {
        Model = CategoryModels[Category];
    }

    float PredictedValue = 0.0f;

    switch (Model)
    {
    case EForecastModel::MovingAverage:
        PredictedValue = CalculateMovingAverage(Data, 10);
        break;
    case EForecastModel::Exponential:
        PredictedValue = CalculateExponentialSmoothing(Data, 0.3f);
        break;
    case EForecastModel::Linear:
        PredictedValue = CalculateLinearRegression(Data, Forecast.PredictionTime);
        break;
    case EForecastModel::TrendAnalysis:
        {
            FRiskTrend Trend = AnalyzeTrend(Category, EAnalysisPeriod::OneDay);
            PredictedValue = Trend.CurrentValue + Trend.ChangeRate * HoursAhead;
        }
        break;
    default:
        PredictedValue = Data.Last().Value;
        break;
    }

    Forecast.PredictedValue = FMath::Clamp(PredictedValue, 0.0f, 100.0f);

    // Determine risk level based on predicted value
    if (Forecast.PredictedValue >= 80.0f)
    {
        Forecast.PredictedLevel = ERiskLevel::Critical;
    }
    else if (Forecast.PredictedValue >= 60.0f)
    {
        Forecast.PredictedLevel = ERiskLevel::High;
    }
    else if (Forecast.PredictedValue >= 40.0f)
    {
        Forecast.PredictedLevel = ERiskLevel::Medium;
    }
    else if (Forecast.PredictedValue >= 20.0f)
    {
        Forecast.PredictedLevel = ERiskLevel::Low;
    }
    else
    {
        Forecast.PredictedLevel = ERiskLevel::None;
    }

    // Calculate confidence
    Forecast.ConfidenceScore = CalculateConfidenceScore(Data.Num(), 0.0f);
    Forecast.ConfidenceInterval = 10.0f * (1.0f - Forecast.ConfidenceScore / 100.0f);
    Forecast.ConfidenceRange = CalculateConfidenceRange(Forecast.PredictedValue, Forecast.ConfidenceScore);

    // Generate rationale
    GenerateForecastRationale(Forecast, CurrentTrends.Contains(Category) ? CurrentTrends[Category] : FRiskTrend());

    // Identify contributing factors
    IdentifyContributingFactors(Forecast, Category);

    // Generate suggestions
    Forecast.SuggestedActions = GenerateSuggestedActions(Forecast);

    OnForecastGenerated.Broadcast(Forecast);

    UE_LOG(LogRiskTrend, Log, TEXT("Forecast for %s (%.1f hours): %.1f%% (confidence: %.1f%%)"),
        *UEnum::GetValueAsString(Category), HoursAhead, Forecast.PredictedValue, Forecast.ConfidenceScore);

    return Forecast;
}

TArray<FRiskForecast> UMingRiskTrendAnalysis::GenerateForecastsForAllCategories(float HoursAhead)
{
    TArray<FRiskForecast> Results;

    for (const auto& Pair : HistoricalData)
    {
        if (Pair.Value.Num() >= Config.MinDataPoints)
        {
            Results.Add(GenerateForecast(Pair.Key, HoursAhead));
        }
    }

    return Results;
}

TArray<FTimeSeriesData> UMingRiskTrendAnalysis::GetTimeSeriesData(ERiskCategory Category, uint32 StartTime, uint32 EndTime)
{
    TArray<FTimeSeriesData> Result;

    if (HistoricalData.Contains(Category))
    {
        for (const auto& Point : HistoricalData[Category])
        {
            if (Point.Timestamp >= StartTime && Point.Timestamp <= EndTime)
            {
                Result.Add(Point);
            }
        }
    }

    return Result;
}

void UMingRiskTrendAnalysis::SetForecastModel(ERiskCategory Category, EForecastModel Model)
{
    CategoryModels.Add(Category, Model);
    UE_LOG(LogRiskTrend, Log, TEXT("Set forecast model for %s to %s"),
        *UEnum::GetValueAsString(Category), *UEnum::GetValueAsString(Model));
}

TArray<FRiskCorrelation> UMingRiskTrendAnalysis::AnalyzeCorrelations()
{
    TArray<FRiskCorrelation> Correlations;

    // Calculate correlations between all category pairs
    for (const auto& PairA : HistoricalData)
    {
        for (const auto& PairB : HistoricalData)
        {
            if (PairA.Key != PairB.Key)
            {
                float Correlation = CalculateCorrelation(PairA.Key, PairB.Key);

                if (FMath::Abs(Correlation) > 0.3f) // Only significant correlations
                {
                    FRiskCorrelation RiskCorr;
                    RiskCorr.SourceCategory = PairA.Key;
                    RiskCorr.TargetCategory = PairB.Key;
                    RiskCorr.CorrelationCoefficient = Correlation;
                    RiskCorr.bIsPositiveCorrelation = Correlation > 0;
                    RiskCorr.RelationshipDescription = FString::Printf(TEXT("Correlation: %.2f"), Correlation);

                    Correlations.Add(RiskCorr);
                }
            }
        }
    }

    return Correlations;
}

float UMingRiskTrendAnalysis::CalculateCorrelation(ERiskCategory CategoryA, ERiskCategory CategoryB)
{
    if (!HistoricalData.Contains(CategoryA) || !HistoricalData.Contains(CategoryB))
    {
        return 0.0f;
    }

    const TArray<FTimeSeriesData>& DataA = HistoricalData[CategoryA];
    const TArray<FTimeSeriesData>& DataB = HistoricalData[CategoryB];

    // Simple correlation calculation
    if (DataA.Num() < 2 || DataB.Num() < 2)
    {
        return 0.0f;
    }

    float MeanA = 0.0f, MeanB = 0.0f;
    for (const auto& Point : DataA) MeanA += Point.Value;
    for (const auto& Point : DataB) MeanB += Point.Value;
    MeanA /= DataA.Num();
    MeanB /= DataB.Num();

    float Numerator = 0.0f;
    float DenomA = 0.0f;
    float DenomB = 0.0f;

    int32 MinCount = FMath::Min(DataA.Num(), DataB.Num());
    for (int32 i = 0; i < MinCount; ++i)
    {
        float DiffA = DataA[i].Value - MeanA;
        float DiffB = DataB[i].Value - MeanB;

        Numerator += DiffA * DiffB;
        DenomA += DiffA * DiffA;
        DenomB += DiffB * DiffB;
    }

    float Denominator = FMath::Sqrt(DenomA * DenomB);
    if (Denominator < 0.001f) return 0.0f;

    return FMath::Clamp(Numerator / Denominator, -1.0f, 1.0f);
}

void UMingRiskTrendAnalysis::ExportTrendData(const FString& FilePath)
{
    UE_LOG(LogRiskTrend, Log, TEXT("Exporting trend data to: %s"), *FilePath);

    FString JsonData = TEXT("{\n");
    JsonData += TEXT("  \"categories\": [\n");

    int32 CatIndex = 0;
    for (const auto& Pair : HistoricalData)
    {
        JsonData += TEXT("    {\n");
        JsonData += TEXT("      \"category\": \"") + UEnum::GetValueAsString(Pair.Key) + TEXT("\",\n");
        JsonData += TEXT("      \"data_points\": [\n");

        for (int32 i = 0; i < Pair.Value.Num(); ++i)
        {
            const auto& Point = Pair.Value[i];
            JsonData += FString::Printf(TEXT("        {\"time\": %d, \"value\": %.2f}"),
                Point.Timestamp, Point.Value);
            JsonData += (i < Pair.Value.Num() - 1) ? TEXT(",\n") : TEXT("\n");
        }

        JsonData += TEXT("      ]\n");
        JsonData += TEXT("    }") + FString(CatIndex < HistoricalData.Num() - 1 ? "," : "") + TEXT("\n");
        CatIndex++;
    }

    JsonData += TEXT("  ]\n");
    JsonData += TEXT("}\n");

    FFileHelper::SaveStringToFile(JsonData, *FilePath);
}

void UMingRiskTrendAnalysis::ImportTrendData(const FString& FilePath)
{
    UE_LOG(LogRiskTrend, Log, TEXT("Importing trend data from: %s"), *FilePath);
    // Implementation would parse JSON and populate HistoricalData
}

void UMingRiskTrendAnalysis::ClearHistoricalData()
{
    for (auto& Pair : HistoricalData)
    {
        Pair.Value.Empty();
    }
    CurrentTrends.Empty();
    UE_LOG(LogRiskTrend, Log, TEXT("Historical data cleared"));
}

void UMingRiskTrendAnalysis::ClearCategoryData(ERiskCategory Category)
{
    if (HistoricalData.Contains(Category))
    {
        HistoricalData[Category].Empty();
        CurrentTrends.Remove(Category);
        UE_LOG(LogRiskTrend, Log, TEXT("Data cleared for category: %s"), *UEnum::GetValueAsString(Category));
    }
}

void UMingRiskTrendAnalysis::StartAutoAnalysis()
{
    if (Config.bEnableAutoAnalysis && GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            AnalysisTimer,
            this,
            &UMingRiskTrendAnalysis::PerformAutoAnalysis,
            Config.AnalysisInterval,
            true
        );

        UE_LOG(LogRiskTrend, Log, TEXT("Auto-analysis started (interval: %.1f seconds)"), Config.AnalysisInterval);
    }
}

void UMingRiskTrendAnalysis::StopAutoAnalysis()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(AnalysisTimer);
    }

    UE_LOG(LogRiskTrend, Log, TEXT("Auto-analysis stopped"));
}

void UMingRiskTrendAnalysis::GenerateTrendReport()
{
    UE_LOG(LogRiskTrend, Log, TEXT("=== Risk Trend Analysis Report ==="));

    for (const auto& Pair : CurrentTrends)
    {
        const FRiskTrend& Trend = Pair.Value;
        UE_LOG(LogRiskTrend, Log, TEXT("%s: %s (%.1f -> %.1f, rate: %.2f)"),
            *UEnum::GetValueAsString(Trend.Category),
            *UEnum::GetValueAsString(Trend.Direction),
            Trend.PreviousValue,
            Trend.CurrentValue,
            Trend.ChangeRate);
    }
}

int32 UMingRiskTrendAnalysis::GetDataPointCount(ERiskCategory Category) const
{
    if (HistoricalData.Contains(Category))
    {
        return HistoricalData[Category].Num();
    }
    return 0;
}

bool UMingRiskTrendAnalysis::IsAutoAnalysisActive() const
{
    return Config.bEnableAutoAnalysis;
}

void UMingRiskTrendAnalysis::ScheduleAutoAnalysis()
{
    // Timer is set in StartAutoAnalysis
}

void UMingRiskTrendAnalysis::CancelAutoAnalysis()
{
    StopAutoAnalysis();
}

void UMingRiskTrendAnalysis::PerformAutoAnalysis()
{
    for (const auto& Pair : HistoricalData)
    {
        if (Pair.Value.Num() >= Config.MinDataPoints)
        {
            AnalyzeTrend(Pair.Key, Config.DefaultPeriod);
        }
    }

    DetectTrendChanges();
}

float UMingRiskTrendAnalysis::CalculateMovingAverage(const TArray<FTimeSeriesData>& Data, int32 WindowSize)
{
    if (Data.Num() < WindowSize) return 0.0f;

    float Sum = 0.0f;
    for (int32 i = Data.Num() - WindowSize; i < Data.Num(); ++i)
    {
        Sum += Data[i].Value;
    }
    return Sum / WindowSize;
}

float UMingRiskTrendAnalysis::CalculateExponentialSmoothing(const TArray<FTimeSeriesData>& Data, float Alpha)
{
    if (Data.Num() == 0) return 0.0f;

    float Smoothed = Data[0].Value;
    for (int32 i = 1; i < Data.Num(); ++i)
    {
        Smoothed = Alpha * Data[i].Value + (1.0f - Alpha) * Smoothed;
    }
    return Smoothed;
}

float UMingRiskTrendAnalysis::CalculateLinearRegression(const TArray<FTimeSeriesData>& Data, uint32 TargetTime)
{
    if (Data.Num() < 2) return 0.0f;

    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumXX = 0.0f;
    int32 N = Data.Num();

    for (const auto& Point : Data)
    {
        float X = static_cast<float>(Point.Timestamp);
        SumX += X;
        SumY += Point.Value;
        SumXY += X * Point.Value;
        SumXX += X * X;
    }

    float Denominator = N * SumXX - SumX * SumX;
    if (FMath::Abs(Denominator) < 0.001f) return Data.Last().Value;

    float Slope = (N * SumXY - SumX * SumY) / Denominator;
    float Intercept = (SumY - Slope * SumX) / N;

    return Slope * TargetTime + Intercept;
}

float UMingRiskTrendAnalysis::CalculateTrendSlope(const TArray<FTimeSeriesData>& Data)
{
    if (Data.Num() < 2) return 0.0f;

    // Simple linear regression for slope
    int32 N = Data.Num();
    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumXX = 0.0f;

    for (int32 i = 0; i < N; ++i)
    {
        float X = static_cast<float>(i);
        SumX += X;
        SumY += Data[i].Value;
        SumXY += X * Data[i].Value;
        SumXX += X * X;
    }

    float Denominator = N * SumXX - SumX * SumX;
    if (FMath::Abs(Denominator) < 0.001f) return 0.0f;

    return (N * SumXY - SumX * SumY) / Denominator;
}

float UMingRiskTrendAnalysis::CalculateVolatility(const TArray<FTimeSeriesData>& Data)
{
    if (Data.Num() < 2) return 0.0f;

    float Mean = 0.0f;
    for (const auto& Point : Data)
    {
        Mean += Point.Value;
    }
    Mean /= Data.Num();

    float Variance = 0.0f;
    for (const auto& Point : Data)
    {
        Variance += FMath::Pow(Point.Value - Mean, 2.0f);
    }
    Variance /= Data.Num();

    return FMath::Sqrt(Variance);
}

ETrendDirection UMingRiskTrendAnalysis::DetermineTrendDirection(float CurrentSlope, float PreviousSlope, float Volatility)
{
    float AbsSlope = FMath::Abs(CurrentSlope);
    float Threshold = Config.TrendChangeThreshold;

    if (AbsSlope < Threshold)
    {
        return ETrendDirection::Stable;
    }

    if (Volatility > Threshold * 3)
    {
        return ETrendDirection::Volatile;
    }

    bool IsImproving = CurrentSlope < 0; // Assuming lower risk values are better

    if (IsImproving)
    {
        if (FMath::Abs(CurrentSlope) > FMath::Abs(PreviousSlope) * 1.5f)
        {
            return ETrendDirection::Accelerating;
        }
        return ETrendDirection::Improving;
    }
    else
    {
        if (FMath::Abs(CurrentSlope) > FMath::Abs(PreviousSlope) * 1.5f)
        {
            return ETrendDirection::Accelerating;
        }
        return ETrendDirection::Degrading;
    }
}

FLinearColor UMingRiskTrendAnalysis::GetTrendColor(ETrendDirection Direction)
{
    switch (Direction)
    {
    case ETrendDirection::Stable:
        return FLinearColor(0.5f, 0.5f, 0.5f); // Gray
    case ETrendDirection::Improving:
        return FLinearColor(0.0f, 1.0f, 0.0f); // Green
    case ETrendDirection::Degrading:
        return FLinearColor(1.0f, 0.0f, 0.0f); // Red
    case ETrendDirection::Volatile:
        return FLinearColor(1.0f, 1.0f, 0.0f); // Yellow
    case ETrendDirection::Accelerating:
        return FLinearColor(1.0f, 0.5f, 0.0f); // Orange
    case ETrendDirection::Decelerating:
        return FLinearColor(0.0f, 1.0f, 1.0f); // Cyan
    default:
        return FLinearColor::Gray;
    }
}

void UMingRiskTrendAnalysis::DetectTrendChanges()
{
    for (const auto& Pair : CurrentTrends)
    {
        ERiskCategory Category = Pair.Key;
        const FRiskTrend& CurrentTrend = Pair.Value;

        if (PreviousDirections.Contains(Category))
        {
            ETrendDirection PreviousDirection = PreviousDirections[Category];

            if (PreviousDirection != CurrentTrend.Direction)
            {
                OnTrendDirectionChanged.Broadcast(CurrentTrend);
                UE_LOG(LogRiskTrend, Log, TEXT("Trend direction changed for %s: %s -> %s"),
                    *UEnum::GetValueAsString(Category),
                    *UEnum::GetValueAsString(PreviousDirection),
                    *UEnum::GetValueAsString(CurrentTrend.Direction));
            }
        }
    }
}

void UMingRiskTrendAnalysis::TrimHistoricalDataIfNeeded()
{
    for (auto& Pair : HistoricalData)
    {
        if (Pair.Value.Num() > Config.MaxHistoricalPoints)
        {
            int32 RemoveCount = Pair.Value.Num() - Config.MaxHistoricalPoints;
            Pair.Value.RemoveAt(0, RemoveCount);
        }
    }
}

void UMingRiskTrendAnalysis::StoreCurrentTrends()
{
    // Trends are stored in CurrentTrends map
}

FString UMingRiskTrendAnalysis::GenerateTrendDescription(const FRiskTrend& Trend)
{
    FString DirectionStr = UEnum::GetValueAsString(Trend.Direction);
    FString ChangeStr = FString::Printf(TEXT("%.1f%%"), FMath::Abs(Trend.ChangeRate));

    return FString::Printf(TEXT("%s trend detected with %s change rate (volatility: %.1f)"),
        *DirectionStr, *ChangeStr, Trend.Volatility);
}

float UMingRiskTrendAnalysis::CalculateConfidenceScore(int32 DataPoints, float Volatility)
{
    float BaseConfidence = FMath::Clamp(static_cast<float>(DataPoints) / 100.0f * 100.0f, 0.0f, 100.0f);
    float VolatilityPenalty = FMath::Clamp(Volatility / 10.0f, 0.0f, 30.0f);

    return FMath::Clamp(BaseConfidence - VolatilityPenalty, 50.0f, 95.0f);
}

TArray<float> UMingRiskTrendAnalysis::CalculateConfidenceRange(float PredictedValue, float ConfidenceScore)
{
    float Range = (1.0f - ConfidenceScore / 100.0f) * 20.0f;
    return { FMath::Max(0.0f, PredictedValue - Range), FMath::Min(100.0f, PredictedValue + Range) };
}

void UMingRiskTrendAnalysis::GenerateForecastRationale(FRiskForecast& Forecast, const FRiskTrend& Trend)
{
    Forecast.ForecastRationale = FString::Printf(TEXT("Based on %s trend with %.1f%% confidence. "),
        *UEnum::GetValueAsString(Trend.Direction), Forecast.ConfidenceScore);

    Forecast.ForecastRationale += FString::Printf(TEXT("Current trajectory suggests %.1f%% risk level."),
        Forecast.PredictedValue);
}

void UMingRiskTrendAnalysis::IdentifyContributingFactors(FRiskForecast& Forecast, ERiskCategory Category)
{
    // Add factors that contribute to the forecast
    if (CurrentTrends.Contains(Category))
    {
        const FRiskTrend& Trend = CurrentTrends[Category];
        if (Trend.Volatility > 10.0f)
        {
            Forecast.ContributingFactors.Add(TEXT("High volatility"));
        }
        if (Trend.ChangeRate > 5.0f)
        {
            Forecast.ContributingFactors.Add(TEXT("Rapid value changes"));
        }
        if (Trend.DataPoints < 50)
        {
            Forecast.ContributingFactors.Add(TEXT("Limited data points"));
        }
    }
}

TArray<FString> UMingRiskTrendAnalysis::GenerateSuggestedActions(const FRiskForecast& Forecast)
{
    TArray<FString> Actions;

    switch (Forecast.PredictedLevel)
    {
    case ERiskLevel::Critical:
        Actions.Add(TEXT("Immediate risk mitigation required"));
        Actions.Add(TEXT("Escalate to management"));
        Actions.Add(TEXT("Implement emergency procedures"));
        break;
    case ERiskLevel::High:
        Actions.Add(TEXT("Schedule preventive maintenance"));
        Actions.Add(TEXT("Increase monitoring frequency"));
        Actions.Add(TEXT("Prepare contingency plans"));
        break;
    case ERiskLevel::Medium:
        Actions.Add(TEXT("Review current practices"));
        Actions.Add(TEXT("Schedule routine check"));
        break;
    default:
        Actions.Add(TEXT("Continue normal operations"));
        Actions.Add(TEXT("Schedule regular review"));
        break;
    }

    return Actions;
}

void UMingRiskTrendAnalysis::NotifyTrendDetected(const FRiskTrend& Trend)
{
    OnTrendDetected.Broadcast(Trend);
}

void UMingRiskTrendAnalysis::NotifyCriticalTrend(const FRiskTrend& Trend)
{
    OnCriticalTrendDetected.Broadcast(Trend);
    UE_LOG(LogRiskTrend, Warning, TEXT("CRITICAL TREND detected for %s: %s at %.1f%%"),
        *UEnum::GetValueAsString(Trend.Category),
        *UEnum::GetValueAsString(Trend.Direction),
        Trend.CurrentValue);
}

uint32 UMingRiskTrendAnalysis::GetPeriodDuration(EAnalysisPeriod Period)
{
    switch (Period)
    {
    case EAnalysisPeriod::OneHour:
        return 3600;
    case EAnalysisPeriod::SixHours:
        return 21600;
    case EAnalysisPeriod::OneDay:
        return 86400;
    case EAnalysisPeriod::OneWeek:
        return 604800;
    case EAnalysisPeriod::OneMonth:
        return 2592000;
    default:
        return 86400;
    }
}

static UMingRiskTrendAnalysis* UMingRiskTrendAnalysis::Get(UObject* WorldContextObject)
{
    static UMingRiskTrendAnalysis* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingRiskTrendAnalysis>();
        Instance->AddToRoot();
    }
    return Instance;
}
