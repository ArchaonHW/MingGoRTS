// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Monitoring Dashboard Implementation - B2-1
// Provides comprehensive risk monitoring and visualization

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
    CurrentView = EDashboardView::Overview;
    
    // Initialize risk metrics
    InitializeRiskMetrics();
    
    // Start real-time monitoring
    if (Config.bEnableRealTimeMonitoring)
    {
        StartRealTimeMonitoring();
    }
    
    UE_LOG(LogRiskDashboard, Log, TEXT("Risk Dashboard initialized with %d metrics"), 
        RiskMetrics.Num());
}

void UMingRiskDashboard::ShutdownDashboard()
{
    StopRealTimeMonitoring();
    UE_LOG(LogRiskDashboard, Log, TEXT("Risk Dashboard shutdown"));
}

void UMingRiskDashboard::UpdateRiskMetric(const FName& MetricName, float Value, ERiskCategory Category)
{
    if (FRiskMetric* Metric = RiskMetrics.Find(MetricName))
    {
        Metric->CurrentValue = Value;
        Metric->Category = Category;
        Metric->LastUpdateTime = FDateTime::Now().GetTicks();
        
        // Update history
        Metric->HistoryValues.Add(Value);
        if (Metric->HistoryValues.Num() > Config.MaxHistoryPoints)
        {
            Metric->HistoryValues.RemoveAt(0);
        }
        
        // Check thresholds
        CheckMetricThresholds(*Metric);
        
        // Update overall risk level
        UpdateOverallRiskLevel();
        
        // Broadcast update
        OnRiskMetricUpdated.Broadcast(MetricName, *Metric);
    }
    else
    {
        UE_LOG(LogRiskDashboard, Warning, TEXT("Metric '%s' not found"), *MetricName.ToString());
    }
}

FRiskMetric UMingRiskDashboard::GetRiskMetric(const FName& MetricName) const
{
    if (const FRiskMetric* Metric = RiskMetrics.Find(MetricName))
    {
        return *Metric;
    }
    
    // Return empty metric if not found
    FRiskMetric EmptyMetric;
    EmptyMetric.MetricName = MetricName;
    return EmptyMetric;
}

TArray<FRiskMetric> UMingRiskDashboard::GetAllRiskMetrics() const
{
    TArray<FRiskMetric> Metrics;
    for (const auto& MetricPair : RiskMetrics)
    {
        Metrics.Add(MetricPair.Value);
    }
    return Metrics;
}

TArray<FRiskMetric> UMingRiskDashboard::GetRiskMetricsByCategory(ERiskCategory Category) const
{
    TArray<FRiskMetric> CategoryMetrics;
    for (const auto& MetricPair : RiskMetrics)
    {
        if (MetricPair.Value.Category == Category)
        {
            CategoryMetrics.Add(MetricPair.Value);
        }
    }
    return CategoryMetrics;
}

TArray<FRiskAlert> UMingRiskDashboard::GetActiveAlerts() const
{
    TArray<FRiskAlert> ActiveAlerts;
    for (const auto& Alert : RiskAlerts)
    {
        if (Alert.Status == EAlertStatus::New || Alert.Status == EAlertStatus::InProgress)
        {
            ActiveAlerts.Add(Alert);
        }
    }
    return ActiveAlerts;
}

void UMingRiskDashboard::CreateAlert(const FString& Title, const FString& Message, 
    EAlertType Type, EAlertPriority Priority, ERiskCategory Category)
{
    FRiskAlert NewAlert;
    NewAlert.AlertID = FGuid::NewGuid().ToString();
    NewAlert.Title = Title;
    NewAlert.Message = Message;
    NewAlert.Type = Type;
    NewAlert.Priority = Priority;
    NewAlert.Category = Category;
    NewAlert.Status = EAlertStatus::New;
    NewAlert.Timestamp = FDateTime::Now();
    
    RiskAlerts.Add(NewAlert);
    
    // Broadcast alert
    OnRiskAlertCreated.Broadcast(NewAlert);
    
    UE_LOG(LogRiskDashboard, Log, TEXT("Alert created: %s"), *Title);
}

void UMingRiskDashboard::AcknowledgeAlert(const FString& AlertID)
{
    for (auto& Alert : RiskAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Status = EAlertStatus::Acknowledged;
            OnRiskAlertUpdated.Broadcast(Alert);
            break;
        }
    }
}

void UMingRiskDashboard::ResolveAlert(const FString& AlertID)
{
    for (auto& Alert : RiskAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Status = EAlertStatus::Resolved;
            OnRiskAlertUpdated.Broadcast(Alert);
            break;
        }
    }
}

void UMingRiskDashboard::SetDashboardView(EDashboardView View)
{
    CurrentView = View;
    OnDashboardViewChanged.Broadcast(View);
}

EDashboardView UMingRiskDashboard::GetCurrentView() const
{
    return CurrentView;
}

ERiskLevel UMingRiskDashboard::GetOverallRiskLevel() const
{
    return OverallRiskLevel;
}

void UMingRiskDashboard::StartRealTimeMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRiskDashboard::PerformMonitoringCycle,
            Config.MonitoringInterval,
            true);

        UE_LOG(LogRiskDashboard, Log, TEXT("Real-time monitoring started (interval: %.1f s)"), 
            Config.MonitoringInterval);
    }
}

void UMingRiskDashboard::StopRealTimeMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().ClearTimer(MonitoringTimer);
    }
    
    UE_LOG(LogRiskDashboard, Log, TEXT("Real-time monitoring stopped"));
}

void UMingRiskDashboard::RefreshDashboard()
{
    // Update all metrics
    for (auto& MetricPair : RiskMetrics)
    {
        UpdateMetricValue(MetricPair.Value);
    }
    
    // Update overall risk level
    UpdateOverallRiskLevel();
    
    // Check for new alerts
    CheckForAlerts();
    
    OnDashboardRefreshed.Broadcast();
}

void UMingRiskDashboard::ExportDashboardData(const FString& FilePath) const
{
    UE_LOG(LogRiskDashboard, Log, TEXT("Exporting dashboard data to: %s"), *FilePath);
    
    FString Report = TEXT("MingGoRTS Risk Dashboard Report\n");
    Report += TEXT("===================================\n\n");
    Report += FString::Printf(TEXT("Export Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Overall Risk Level: %s\n\n"), *UEnum::GetValueAsString(OverallRiskLevel));
    
    Report += TEXT("Risk Metrics:\n");
    Report += TEXT("-------------\n");
    
    for (const auto& MetricPair : RiskMetrics)
    {
        const FRiskMetric& Metric = MetricPair.Value;
        Report += FString::Printf(TEXT("- %s: %.2f (%s)\n"), 
            *Metric.MetricName.ToString(),
            Metric.CurrentValue,
            *UEnum::GetValueAsString(Metric.RiskLevel));
    }
    
    Report += TEXT("\nActive Alerts:\n");
    Report += TEXT("-------------\n");
    
    for (const FRiskAlert& Alert : RiskAlerts)
    {
        if (Alert.Status == EAlertStatus::New || Alert.Status == EAlertStatus::InProgress)
        {
            Report += FString::Printf(TEXT("- %s: %s\n"), *Alert.Title, *Alert.Message);
        }
    }
    
    // In a real implementation, you would save this to a file
    UE_LOG(LogRiskDashboard, Log, TEXT("Report generated:\n%s"), *Report);
}

// Private helper functions

void UMingRiskDashboard::InitializeRiskMetrics()
{
    // Initialize default metrics
    AddRiskMetric(TEXT("CPUUsage"), ERiskCategory::Performance, 0.0f, 70.0f, 90.0f);
    AddRiskMetric(TEXT("MemoryUsage"), ERiskCategory::Performance, 0.0f, 75.0f, 95.0f);
    AddRiskMetric(TEXT("NetworkLatency"), ERiskCategory::Network, 0.0f, 100.0f, 200.0f);
    AddRiskMetric(TEXT("ErrorRate"), ERiskCategory::Stability, 0.0f, 1.0f, 5.0f);
    AddRiskMetric(TEXT("SecurityScore"), ERiskCategory::Security, 100.0f, 70.0f, 50.0f);
}

void UMingRiskDashboard::AddRiskMetric(const FName& Name, ERiskCategory Category, 
    float DefaultValue, float Threshold, float CriticalThreshold)
{
    FRiskMetric Metric;
    Metric.MetricName = Name;
    Metric.Category = Category;
    Metric.CurrentValue = DefaultValue;
    Metric.Threshold = Threshold;
    Metric.CriticalThreshold = CriticalThreshold;
    Metric.RiskLevel = ERiskLevel::None;
    Metric.LastUpdateTime = FDateTime::Now().GetTicks();
    
    RiskMetrics.Add(Name, Metric);
}

void UMingRiskDashboard::CheckMetricThresholds(FRiskMetric& Metric)
{
    if (Metric.CurrentValue >= Metric.CriticalThreshold)
    {
        Metric.RiskLevel = ERiskLevel::Critical;
        CreateAlert(
            FString::Printf(TEXT("Critical: %s"), *Metric.MetricName.ToString()),
            FString::Printf(TEXT("Metric %s has reached critical level: %.2f"), 
                *Metric.MetricName.ToString(), Metric.CurrentValue),
            EAlertType::Critical,
            EAlertPriority::Highest,
            Metric.Category);
    }
    else if (Metric.CurrentValue >= Metric.Threshold)
    {
        if (Metric.RiskLevel < ERiskLevel::High)
        {
            Metric.RiskLevel = ERiskLevel::High;
            CreateAlert(
                FString::Printf(TEXT("Warning: %s"), *Metric.MetricName.ToString()),
                FString::Printf(TEXT("Metric %s has exceeded threshold: %.2f"), 
                    *Metric.MetricName.ToString(), Metric.CurrentValue),
                EAlertType::Warning,
                EAlertPriority::High,
                Metric.Category);
        }
    }
    else
    {
        Metric.RiskLevel = ERiskLevel::None;
    }
}

void UMingRiskDashboard::UpdateOverallRiskLevel()
{
    ERiskLevel MaxLevel = ERiskLevel::None;
    
    for (const auto& MetricPair : RiskMetrics)
    {
        if (MetricPair.Value.RiskLevel > MaxLevel)
        {
            MaxLevel = MetricPair.Value.RiskLevel;
        }
    }
    
    if (OverallRiskLevel != MaxLevel)
    {
        ERiskLevel OldLevel = OverallRiskLevel;
        OverallRiskLevel = MaxLevel;
        OnOverallRiskLevelChanged.Broadcast(OverallRiskLevel, OldLevel);
    }
}

void UMingRiskDashboard::PerformMonitoringCycle()
{
    RefreshDashboard();
}

void UMingRiskDashboard::UpdateMetricValue(FRiskMetric& Metric)
{
    // In a real implementation, this would query actual system values
    // For now, we'll use placeholder logic
    if (Metric.MetricName == TEXT("CPUUsage"))
    {
        Metric.CurrentValue = FMath::RandRange(20.0f, 80.0f);
    }
    else if (Metric.MetricName == TEXT("MemoryUsage"))
    {
        Metric.CurrentValue = FMath::RandRange(30.0f, 85.0f);
    }
    else if (Metric.MetricName == TEXT("NetworkLatency"))
    {
        Metric.CurrentValue = FMath::RandRange(10.0f, 150.0f);
    }
    else if (Metric.MetricName == TEXT("ErrorRate"))
    {
        Metric.CurrentValue = FMath::RandRange(0.0f, 3.0f);
    }
    else if (Metric.MetricName == TEXT("SecurityScore"))
    {
        Metric.CurrentValue = FMath::RandRange(60.0f, 100.0f);
    }
    
    Metric.LastUpdateTime = FDateTime::Now().GetTicks();
    
    // Update history
    Metric.HistoryValues.Add(Metric.CurrentValue);
    if (Metric.HistoryValues.Num() > Config.MaxHistoryPoints)
    {
        Metric.HistoryValues.RemoveAt(0);
    }
}

void UMingRiskDashboard::CheckForAlerts()
{
    // Check for any conditions that should generate alerts
    // This is a placeholder implementation
    int32 ActiveAlertCount = 0;
    for (const FRiskAlert& Alert : RiskAlerts)
    {
        if (Alert.Status == EAlertStatus::New || Alert.Status == EAlertStatus::InProgress)
        {
            ActiveAlertCount++;
        }
    }
    
    if (ActiveAlertCount > Config.MaxActiveAlerts)
    {
        CreateAlert(
            TEXT("Alert Limit Exceeded"),
            FString::Printf(TEXT("Too many active alerts: %d"), ActiveAlertCount),
            EAlertType::Warning,
            EAlertPriority::High,
            ERiskCategory::General);
    }
}
