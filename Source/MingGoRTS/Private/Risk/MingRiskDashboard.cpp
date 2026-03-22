// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Monitoring Dashboard Implementation - B2-1

#include "Risk/MingRiskDashboard.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRiskDashboard, Log, All);

UMingRiskDashboard::UMingRiskDashboard()
    : CurrentView(EDashboardView::Overview)
    , OverallRiskLevel(ERiskLevel::None)
{
}

void UMingRiskDashboard::InitializeDashboard(const FDashboardConfig& Config)
{
    this->Config = Config;

    UE_LOG(LogRiskDashboard, Log, TEXT("Initializing Risk Dashboard with %d monitored categories"),
        Config.MonitoredCategories.Num());

    ScheduleUpdates();
}

void UMingRiskDashboard::ShutdownDashboard()
{
    CancelUpdates();
    RiskMetrics.Empty();
    UE_LOG(LogRiskDashboard, Log, TEXT("Risk Dashboard shutdown"));
}

void UMingRiskDashboard::UpdateDashboard()
{
    EvaluateRiskLevels();
    UpdateRiskIndicators();
    CleanupOldHistoryData();

    TArray<FRiskIndicator> Indicators = GetAllRiskIndicators();
    OnDashboardUpdated.Broadcast(Indicators);

    UE_LOG(LogRiskDashboard, Verbose, TEXT("Dashboard updated with %d indicators"), Indicators.Num());
}

void UMingRiskDashboard::SetCurrentView(EDashboardView View)
{
    CurrentView = View;
    UE_LOG(LogRiskDashboard, Log, TEXT("Dashboard view changed to: %s"), *UEnum::GetValueAsString(View));
}

void UMingRiskDashboard::RegisterRiskMetric(const FRiskMetric& Metric)
{
    RiskMetrics.Add(Metric.MetricName, Metric);
    UE_LOG(LogRiskDashboard, Log, TEXT("Registered risk metric: %s"), *Metric.MetricName.ToString());
}

void UMingRiskDashboard::UpdateRiskMetric(FName MetricName, float NewValue)
{
    if (RiskMetrics.Contains(MetricName))
    {
        FRiskMetric& Metric = RiskMetrics[MetricName];
        float OldValue = Metric.CurrentValue;
        Metric.CurrentValue = NewValue;
        Metric.LastUpdateTime = FPlatformTime::Seconds();

        // Add to history
        Metric.HistoryValues.Add(NewValue);

        // Calculate trend
        CalculateTrendForMetric(Metric);

        // Check thresholds
        ERiskLevel OldLevel = Metric.RiskLevel;
        Metric.RiskLevel = CalculateRiskLevel(NewValue, Metric.Threshold, Metric.CriticalThreshold);

        if (OldLevel != Metric.RiskLevel)
        {
            OnMetricThresholdExceeded.Broadcast(Metric);

            if (Metric.RiskLevel > ERiskLevel::Low)
            {
                OnRiskDetected.Broadcast(Metric.Category, Metric.Description);
            }
        }

        UE_LOG(LogRiskDashboard, Verbose, TEXT("Updated metric %s: %.2f -> %.2f (Level: %s)"),
            *MetricName.ToString(), OldValue, NewValue, *UEnum::GetValueAsString(Metric.RiskLevel));
    }
}

void UMingRiskDashboard::UnregisterRiskMetric(FName MetricName)
{
    RiskMetrics.Remove(MetricName);
    UE_LOG(LogRiskDashboard, Log, TEXT("Unregistered risk metric: %s"), *MetricName.ToString());
}

FRiskMetric UMingRiskDashboard::GetRiskMetric(FName MetricName) const
{
    if (RiskMetrics.Contains(MetricName))
    {
        return RiskMetrics[MetricName];
    }
    return FRiskMetric();
}

TArray<FRiskMetric> UMingRiskDashboard::GetAllRiskMetrics() const
{
    TArray<FRiskMetric> Result;
    for (const auto& Pair : RiskMetrics)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FRiskMetric> UMingRiskDashboard::GetRiskMetricsByCategory(ERiskCategory Category) const
{
    TArray<FRiskMetric> Result;
    for (const auto& Pair : RiskMetrics)
    {
        if (Pair.Value.Category == Category)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

ERiskLevel UMingRiskDashboard::GetOverallRiskLevel() const
{
    return OverallRiskLevel;
}

FRiskIndicator UMingRiskDashboard::GetRiskIndicator(ERiskCategory Category) const
{
    FRiskIndicator Indicator;
    Indicator.Category = Category;

    TArray<FRiskMetric> CategoryMetrics = GetRiskMetricsByCategory(Category);
    if (CategoryMetrics.Num() > 0)
    {
        ERiskLevel MaxLevel = ERiskLevel::None;
        float TotalScore = 0.0f;

        for (const auto& Metric : CategoryMetrics)
        {
            if (Metric.RiskLevel > MaxLevel)
            {
                MaxLevel = Metric.RiskLevel;
            }
            TotalScore += static_cast<float>(Metric.RiskLevel);
        }

        Indicator.OverallLevel = MaxLevel;
        Indicator.DisplayColor = GetRiskColor(MaxLevel);
        Indicator.ActiveRiskCount = CategoryMetrics.Num();
        Indicator.Summary = FString::Printf(TEXT("%d metrics monitored, highest level: %s"),
            CategoryMetrics.Num(), *UEnum::GetValueAsString(MaxLevel));
    }

    return Indicator;
}

TArray<FRiskIndicator> UMingRiskDashboard::GetAllRiskIndicators() const
{
    TArray<FRiskIndicator> Result;

    for (const auto& Category : Config.MonitoredCategories)
    {
        Result.Add(GetRiskIndicator(Category));
    }

    return Result;
}

void UMingRiskDashboard::SetRiskThreshold(FName MetricName, float Threshold, float CriticalThreshold)
{
    if (RiskMetrics.Contains(MetricName))
    {
        FRiskMetric& Metric = RiskMetrics[MetricName];
        Metric.Threshold = Threshold;
        Metric.CriticalThreshold = CriticalThreshold;

        // Re-evaluate with new thresholds
        Metric.RiskLevel = CalculateRiskLevel(Metric.CurrentValue, Threshold, CriticalThreshold);

        UE_LOG(LogRiskDashboard, Log, TEXT("Updated thresholds for %s: Normal < %.2f < Critical"),
            *MetricName.ToString(), Threshold);
    }
}

void UMingRiskDashboard::AcknowledgeRisk(FName MetricName)
{
    FString MetricStr = MetricName.ToString();
    if (!AcknowledgedRisks.Contains(MetricStr))
    {
        AcknowledgedRisks.Add(MetricStr);
        UE_LOG(LogRiskDashboard, Log, TEXT("Risk acknowledged: %s"), *MetricStr);
    }
}

void UMingRiskDashboard::ResolveRisk(FName MetricName)
{
    FString MetricStr = MetricName.ToString();
    if (!ResolvedRisks.Contains(MetricStr))
    {
        ResolvedRisks.Add(MetricStr);
        AcknowledgedRisks.Remove(MetricStr);
        UE_LOG(LogRiskDashboard, Log, TEXT("Risk resolved: %s"), *MetricStr);
    }

    if (RiskMetrics.Contains(MetricName))
    {
        RiskMetrics[MetricName].RiskLevel = ERiskLevel::None;
    }
}

void UMingRiskDashboard::ExportDashboardData(const FString& FilePath)
{
    UE_LOG(LogRiskDashboard, Log, TEXT("Exporting dashboard data to: %s"), *FilePath);

    FString JsonData = TEXT("{\n");
    JsonData += TEXT("  \"timestamp\": \"") + FString::Printf(TEXT("%d"), static_cast<uint32>(FPlatformTime::Seconds())) + TEXT("\",\n");
    JsonData += TEXT("  \"overall_risk_level\": \"") + UEnum::GetValueAsString(OverallRiskLevel) + TEXT("\",\n");
    JsonData += TEXT("  \"metrics\": [\n");

    int32 Index = 0;
    for (const auto& Pair : RiskMetrics)
    {
        const FRiskMetric& Metric = Pair.Value;
        JsonData += TEXT("    {\n");
        JsonData += TEXT("      \"name\": \"") + Metric.MetricName.ToString() + TEXT("\",\n");
        JsonData += TEXT("      \"value\": \"") + FString::SanitizeFloat(Metric.CurrentValue) + TEXT("\",\n");
        JsonData += TEXT("      \"level\": \"") + UEnum::GetValueAsString(Metric.RiskLevel) + TEXT("\"\n");
        JsonData += TEXT("    }") + FString(Index < RiskMetrics.Num() - 1 ? "," : "") + TEXT("\n");
        Index++;
    }

    JsonData += TEXT("  ]\n");
    JsonData += TEXT("}\n");

    FFileHelper::SaveStringToFile(JsonData, *FilePath);
}

void UMingRiskDashboard::GenerateRiskReport()
{
    UE_LOG(LogRiskDashboard, Log, TEXT("=== Risk Dashboard Report ==="));
    UE_LOG(LogRiskDashboard, Log, TEXT("Overall Risk Level: %s"), *UEnum::GetValueAsString(OverallRiskLevel));
    UE_LOG(LogRiskDashboard, Log, TEXT("Total Metrics: %d"), RiskMetrics.Num());

    TMap<ERiskLevel, int32> RiskCounts;
    for (const auto& Pair : RiskMetrics)
    {
        RiskCounts.FindOrAdd(Pair.Value.RiskLevel)++;
    }

    UE_LOG(LogRiskDashboard, Log, TEXT("Risk Distribution:"));
    for (const auto& Pair : RiskCounts)
    {
        UE_LOG(LogRiskDashboard, Log, TEXT("  %s: %d"),
            *UEnum::GetValueAsString(Pair.Key), Pair.Value);
    }

    UE_LOG(LogRiskDashboard, Log, TEXT("Active Risks: %d"), GetActiveRiskCount());
    UE_LOG(LogRiskDashboard, Log, TEXT("Critical Risks: %d"), GetCriticalRiskCount());
}

int32 UMingRiskDashboard::GetActiveRiskCount() const
{
    int32 Count = 0;
    for (const auto& Pair : RiskMetrics)
    {
        if (Pair.Value.RiskLevel > ERiskLevel::Low)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRiskDashboard::GetCriticalRiskCount() const
{
    int32 Count = 0;
    for (const auto& Pair : RiskMetrics)
    {
        if (Pair.Value.RiskLevel >= ERiskLevel::Critical)
        {
            Count++;
        }
    }
    return Count;
}

void UMingRiskDashboard::ScheduleUpdates()
{
    if (Config.bEnableRealTimeUpdates && GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            UpdateTimer,
            this,
            &UMingRiskDashboard::UpdateDashboard,
            Config.UpdateInterval,
            true
        );

        UE_LOG(LogRiskDashboard, Log, TEXT("Scheduled dashboard updates every %.1f seconds"),
            Config.UpdateInterval);
    }
}

void UMingRiskDashboard::CancelUpdates()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(UpdateTimer);
    }
}

void UMingRiskDashboard::EvaluateRiskLevels()
{
    ERiskLevel OldOverallLevel = OverallRiskLevel;
    OverallRiskLevel = ERiskLevel::None;

    for (const auto& Pair : RiskMetrics)
    {
        const FRiskMetric& Metric = Pair.Value;
        if (Metric.RiskLevel > OverallRiskLevel)
        {
            OverallRiskLevel = Metric.RiskLevel;
        }
    }

    if (OldOverallLevel != OverallRiskLevel)
    {
        NotifyRiskLevelChange(OldOverallLevel, OverallRiskLevel);
    }
}

ERiskLevel UMingRiskDashboard::CalculateRiskLevel(float Value, float Threshold, float CriticalThreshold) const
{
    if (Value >= CriticalThreshold)
    {
        return ERiskLevel::Critical;
    }
    else if (Value >= Threshold + (CriticalThreshold - Threshold) * 0.7f)
    {
        return ERiskLevel::High;
    }
    else if (Value >= Threshold + (CriticalThreshold - Threshold) * 0.3f)
    {
        return ERiskLevel::Medium;
    }
    else if (Value >= Threshold)
    {
        return ERiskLevel::Low;
    }
    return ERiskLevel::None;
}

void UMingRiskDashboard::UpdateRiskIndicators()
{
    // Risk indicators are calculated on-demand in GetRiskIndicator
}

FLinearColor UMingRiskDashboard::GetRiskColor(ERiskLevel Level) const
{
    switch (Level)
    {
    case ERiskLevel::None:
        return FLinearColor(0.0f, 1.0f, 0.0f);
    case ERiskLevel::Low:
        return FLinearColor(0.5f, 1.0f, 0.0f);
    case ERiskLevel::Medium:
        return FLinearColor(1.0f, 1.0f, 0.0f);
    case ERiskLevel::High:
        return FLinearColor(1.0f, 0.5f, 0.0f);
    case ERiskLevel::Critical:
        return FLinearColor(1.0f, 0.0f, 0.0f);
    case ERiskLevel::Emergency:
        return FLinearColor(0.5f, 0.0f, 0.0f);
    default:
        return FLinearColor::Gray;
    }
}

void UMingRiskDashboard::NotifyRiskLevelChange(ERiskLevel OldLevel, ERiskLevel NewLevel)
{
    OnRiskLevelChanged.Broadcast(NewLevel);

    UE_LOG(LogRiskDashboard, Warning, TEXT("Risk level changed from %s to %s"),
        *UEnum::GetValueAsString(OldLevel), *UEnum::GetValueAsString(NewLevel));

    LogRiskEvent(FString::Printf(TEXT("Risk level changed: %s -> %s"),
        *UEnum::GetValueAsString(OldLevel), *UEnum::GetValueAsString(NewLevel)));
}

void UMingRiskDashboard::LogRiskEvent(const FString& Event)
{
    UE_LOG(LogRiskDashboard, Log, TEXT("[RISK] %s"), *Event);
}

void UMingRiskDashboard::CleanupOldHistoryData()
{
    for (auto& Pair : RiskMetrics)
    {
        FRiskMetric& Metric = Pair.Value;
        if (Metric.HistoryValues.Num() > Config.HistoryDataPoints)
        {
            int32 RemoveCount = Metric.HistoryValues.Num() - Config.HistoryDataPoints;
            Metric.HistoryValues.RemoveAt(0, RemoveCount);
        }
    }
}

void UMingRiskDashboard::CalculateTrendForMetric(FRiskMetric& Metric)
{
    if (Metric.HistoryValues.Num() < 2) return;

    float Recent = Metric.HistoryValues.Last();
    float Previous = Metric.HistoryValues[Metric.HistoryValues.Num() - 2];

    // Simple trend calculation
    float Change = Recent - Previous;
    UE_LOG(LogRiskDashboard, VeryVerbose, TEXT("Metric %s trend: %.2f"),
        *Metric.MetricName.ToString(), Change);
}

static UMingRiskDashboard* UMingRiskDashboard::Get(UObject* WorldContextObject)
{
    static UMingRiskDashboard* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingRiskDashboard>();
        Instance->AddToRoot();
    }
    return Instance;
}
