// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Trend Analysis System - B2-4
// Provides risk trend analysis and forecasting

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskTrendAnalysis.generated.h"

UENUM(BlueprintType)
enum class ETrendDirection : uint8
{
    Stable = 0, UMETA(DisplayName = "Stable"),
    Improving, UMETA(DisplayName = "Improving"),
    Degrading, UMETA(DisplayName = "Degrading"),
    Volatile, UMETA(DisplayName = "Volatile"),
    Accelerating, UMETA(DisplayName = "Accelerating"),
    Decelerating UMETA(DisplayName = "Decelerating")
};

UENUM(BlueprintType)
enum class EForecastModel : uint8
{
    Linear = 0, UMETA(DisplayName = "Linear Regression"),
    Exponential, UMETA(DisplayName = "Exponential Smoothing"),
    MovingAverage, UMETA(DisplayName = "Moving Average"),
    TrendAnalysis, UMETA(DisplayName = "Trend Analysis"),
    Seasonal, UMETA(DisplayName = "Seasonal Decomposition"),
    MachineLearning, UMETA(DisplayName = "ML Prediction")
};

UENUM(BlueprintType)
enum class EAnalysisPeriod : uint8
{
    OneHour = 0, UMETA(DisplayName = "1 Hour"),
    SixHours, UMETA(DisplayName = "6 Hours"),
    OneDay, UMETA(DisplayName = "1 Day"),
    OneWeek, UMETA(DisplayName = "1 Week"),
    OneMonth, UMETA(DisplayName = "1 Month"),
    Custom UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FRiskTrend
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    ETrendDirection Direction;

    UPROPERTY()
    float CurrentValue;

    UPROPERTY()
    float PreviousValue;

    UPROPERTY()
    float ChangeRate;

    UPROPERTY()
    float AverageValue;

    UPROPERTY()
    float MinValue;

    UPROPERTY()
    float MaxValue;

    UPROPERTY()
    float Volatility;

    UPROPERTY()
    int32 DataPoints;

    UPROPERTY()
    FString TrendDescription;

    UPROPERTY()
    FLinearColor TrendColor;

    FRiskTrend()
        : Category(ERiskCategory::General)
        , Direction(ETrendDirection::Stable)
        , CurrentValue(0.0f)
        , PreviousValue(0.0f)
        , ChangeRate(0.0f)
        , AverageValue(0.0f)
        , MinValue(0.0f)
        , MaxValue(0.0f)
        , Volatility(0.0f)
        , DataPoints(0)
        , TrendColor(FLinearColor::Gray)
    {}
};

USTRUCT(BlueprintType)
struct FTimeSeriesData
{
    GENERATED_BODY()

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    float Value;

    UPROPERTY()
    FString Label;

    UPROPERTY()
    TMap<FString, FString> Metadata;

    FTimeSeriesData()
        : Timestamp(0)
        , Value(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FRiskForecast
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    float PredictedValue;

    UPROPERTY()
    ERiskLevel PredictedLevel;

    UPROPERTY()
    uint32 PredictionTime;

    UPROPERTY()
    float ConfidenceInterval;

    UPROPERTY()
    float ConfidenceScore;

    UPROPERTY()
    TArray<float> ConfidenceRange;

    UPROPERTY()
    FString ForecastRationale;

    UPROPERTY()
    TArray<FString> ContributingFactors;

    UPROPERTY()
    TArray<FString> SuggestedActions;

    FRiskForecast()
        : Category(ERiskCategory::General)
        , PredictedValue(0.0f)
        , PredictedLevel(ERiskLevel::None)
        , PredictionTime(0)
        , ConfidenceInterval(0.0f)
        , ConfidenceScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FTrendAnalysisConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    EAnalysisPeriod DefaultPeriod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    EForecastModel DefaultModel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    int32 MinDataPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    float ForecastHorizon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    bool bEnableAutoAnalysis;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    float AnalysisInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    bool bEnableForecasting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    float TrendChangeThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    bool bStoreHistoricalData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trend Analysis")
    int32 MaxHistoricalPoints;

    FTrendAnalysisConfig()
        : DefaultPeriod(EAnalysisPeriod::OneDay)
        , DefaultModel(EForecastModel::MovingAverage)
        , MinDataPoints(10)
        , ForecastHorizon(24.0f)
        , bEnableAutoAnalysis(true)
        , AnalysisInterval(300.0f)
        , bEnableForecasting(true)
        , TrendChangeThreshold(5.0f)
        , bStoreHistoricalData(true)
        , MaxHistoricalPoints(10000)
    {}
};

USTRUCT(BlueprintType)
struct FRiskCorrelation
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskCategory SourceCategory;

    UPROPERTY()
    ERiskCategory TargetCategory;

    UPROPERTY()
    float CorrelationCoefficient;

    UPROPERTY()
    float LagTime;

    UPROPERTY()
    FString RelationshipDescription;

    UPROPERTY()
    bool bIsPositiveCorrelation;

    FRiskCorrelation()
        : SourceCategory(ERiskCategory::General)
        , TargetCategory(ERiskCategory::General)
        , CorrelationCoefficient(0.0f)
        , LagTime(0.0f)
        , bIsPositiveCorrelation(true)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrendDetected, FRiskTrend, Trend};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnForecastGenerated, FRiskForecast, Forecast};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrendDirectionChanged, FRiskTrend, ChangedTrend};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalTrendDetected, FRiskTrend, CriticalTrend};
/**
 * Risk Trend Analysis System
 * Provides risk trend analysis and forecasting
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRiskTrendAnalysis : public UObject
{
    GENERATED_BODY()

public:
    UMingRiskTrendAnalysis(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void InitializeTrendAnalysis(const FTrendAnalysisConfig& Config};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void ShutdownTrendAnalysis(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void AddDataPoint(ERiskCategory Category, uint32 Timestamp, float Value, const FString& Label = TEXT("")};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    FRiskTrend AnalyzeTrend(ERiskCategory Category, EAnalysisPeriod Period};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    TArray<FRiskTrend> AnalyzeAllTrends(EAnalysisPeriod Period};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    FRiskForecast GenerateForecast(ERiskCategory Category, float HoursAhead};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    TArray<FRiskForecast> GenerateForecastsForAllCategories(float HoursAhead};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    TArray<FTimeSeriesData> GetTimeSeriesData(ERiskCategory Category, uint32 StartTime, uint32 EndTime};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void SetForecastModel(ERiskCategory Category, EForecastModel Model};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    TArray<FRiskCorrelation> AnalyzeCorrelations(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    float CalculateCorrelation(ERiskCategory CategoryA, ERiskCategory CategoryB};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void ExportTrendData(const FString& FilePath};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void ImportTrendData(const FString& FilePath};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void ClearHistoricalData(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void ClearCategoryData(ERiskCategory Category};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void StartAutoAnalysis(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void StopAutoAnalysis(};
    UFUNCTION(BlueprintCallable, Category = "Risk Trend Analysis")
    void GenerateTrendReport(};
    UFUNCTION(BlueprintPure, Category = "Risk Trend Analysis")
    FTrendAnalysisConfig GetConfig() const { return Config; }

    UFUNCTION(BlueprintPure, Category = "Risk Trend Analysis")
    int32 GetDataPointCount(ERiskCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Risk Trend Analysis")
    bool IsAutoAnalysisActive() const;

    UPROPERTY(BlueprintAssignable, Category = "Risk Trend Events")
    FOnTrendDetected OnTrendDetected;

    UPROPERTY(BlueprintAssignable, Category = "Risk Trend Events")
    FOnForecastGenerated OnForecastGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Risk Trend Events")
    FOnTrendDirectionChanged OnTrendDirectionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Trend Events")
    FOnCriticalTrendDetected OnCriticalTrendDetected;

protected:
    UPROPERTY()
    FTrendAnalysisConfig Config;

    UPROPERTY()
    TMap<ERiskCategory, TArray<FTimeSeriesData>> HistoricalData;

    UPROPERTY()
    TMap<ERiskCategory, FRiskTrend> CurrentTrends;

    UPROPERTY()
    TMap<ERiskCategory, EForecastModel> CategoryModels;

    UPROPERTY()
    FTimerHandle AnalysisTimer;

    UPROPERTY()
    TMap<ERiskCategory, ETrendDirection> PreviousDirections;

    void ScheduleAutoAnalysis(};
    void CancelAutoAnalysis(};
    void PerformAutoAnalysis(};
    float CalculateMovingAverage(const TArray<FTimeSeriesData>& Data, int32 WindowSize};
    float CalculateExponentialSmoothing(const TArray<FTimeSeriesData>& Data, float Alpha};
    float CalculateLinearRegression(const TArray<FTimeSeriesData>& Data, uint32 TargetTime};
    float CalculateTrendSlope(const TArray<FTimeSeriesData>& Data};
    float CalculateVolatility(const TArray<FTimeSeriesData>& Data};
    ETrendDirection DetermineTrendDirection(float CurrentSlope, float PreviousSlope, float Volatility};
    FLinearColor GetTrendColor(ETrendDirection Direction};
    void DetectTrendChanges(};
    void TrimHistoricalDataIfNeeded(};
    void StoreCurrentTrends(};
    FString GenerateTrendDescription(const FRiskTrend& Trend};
    float CalculateConfidenceScore(int32 DataPoints, float Volatility};
    TArray<float> CalculateConfidenceRange(float PredictedValue, float ConfidenceScore};
    void GenerateForecastRationale(FRiskForecast& Forecast, const FRiskTrend& Trend};
    void IdentifyContributingFactors(FRiskForecast& Forecast, ERiskCategory Category};
    TArray<FString> GenerateSuggestedActions(const FRiskForecast& Forecast};
    void NotifyTrendDetected(const FRiskTrend& Trend};
    void NotifyCriticalTrend(const FRiskTrend& Trend};
    uint32 GetPeriodDuration(EAnalysisPeriod Period};
    static UMingRiskTrendAnalysis* Get(UObject* WorldContextObject};
};
