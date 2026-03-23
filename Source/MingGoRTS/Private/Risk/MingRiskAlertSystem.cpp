// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Auto-Alert Mechanism System Implementation - B2-3
// Provides automated risk detection and alerting

#include "Risk/MingRiskAlertSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRiskAlert, Log, All);

UMingRiskAlertSystem::UMingRiskAlertSystem()
{
    // Enable all notification channels by default
    EnabledChannels.Add(ENotificationChannel::InGame);
    EnabledChannels.Add(ENotificationChannel::Dashboard);
    EnabledChannels.Add(ENotificationChannel::Log);
}

void UMingRiskAlertSystem::InitializeAlertSystem()
{
    // Initialize default alert rules
    InitializeDefaultRules();
    
    // Start monitoring if enabled
    if (bEnableMonitoring)
    {
        StartMonitoring();
    }
    
    UE_LOG(LogRiskAlert, Log, TEXT("Risk Alert System initialized with %d rules"), 
        AlertRules.Num());
}

void UMingRiskAlertSystem::ShutdownAlertSystem()
{
    StopMonitoring();
    UE_LOG(LogRiskAlert, Log, TEXT("Risk Alert System shutdown"));
}

void UMingRiskAlertSystem::CreateAlert(const FString& Title, const FString& Message, 
    EAlertType Type, EAlertPriority Priority, ERiskCategory Category)
{
    FRiskAlert Alert;
    Alert.AlertID = FGuid::NewGuid().ToString();
    Alert.Title = Title;
    Alert.Message = Message;
    Alert.Type = Type;
    Alert.Priority = Priority;
    Alert.Category = Category;
    Alert.Status = EAlertStatus::New;
    Alert.Timestamp = FDateTime::Now();
    
    // Apply alert rules
    ApplyAlertRules(Alert);
    
    // Store alert
    ActiveAlerts.Add(Alert);
    AlertHistory.Add(Alert);
    
    // Send notifications
    SendAlertNotifications(Alert);
    
    // Broadcast event
    OnAlertCreated.Broadcast(Alert);
    
    UE_LOG(LogRiskAlert, Log, TEXT("Alert created: %s [%s]"), *Title, *UEnum::GetValueAsString(Type));
}

void UMingRiskAlertSystem::CreateThresholdAlert(const FName& MetricName, float CurrentValue, 
    float Threshold, ERiskCategory Category)
{
    FString Title = FString::Printf(TEXT("Threshold Exceeded: %s"), *MetricName.ToString());
    FString Message = FString::Printf(TEXT("Metric '%s' has exceeded threshold: %.2f > %.2f"), 
        *MetricName.ToString(), CurrentValue, Threshold);
    
    EAlertType AlertType = (CurrentValue >= Threshold * 1.5f) ? EAlertType::Critical : EAlertType::Warning;
    EAlertPriority Priority = (CurrentValue >= Threshold * 1.5f) ? EAlertPriority::Highest : EAlertPriority::High;
    
    CreateAlert(Title, Message, AlertType, Priority, Category);
}

void UMingRiskAlertSystem::AcknowledgeAlert(const FString& AlertID)
{
    for (auto& Alert : ActiveAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Status = EAlertStatus::Acknowledged;
            OnAlertStatusChanged.Broadcast(Alert);
            break;
        }
    }
}

void UMingRiskAlertSystem::ResolveAlert(const FString& AlertID, const FString& Resolution)
{
    for (int32 i = 0; i < ActiveAlerts.Num(); ++i)
    {
        if (ActiveAlerts[i].AlertID == AlertID)
        {
            ActiveAlerts[i].Status = EAlertStatus::Resolved;
            ActiveAlerts[i].Resolution = Resolution;
            ActiveAlerts[i].ResolvedTime = FDateTime::Now();
            
            OnAlertStatusChanged.Broadcast(ActiveAlerts[i]);
            ActiveAlerts.RemoveAt(i);
            break;
        }
    }
}

void UMingRiskAlertSystem::EscalateAlert(const FString& AlertID, EAlertPriority NewPriority)
{
    for (auto& Alert : ActiveAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Priority = NewPriority;
            Alert.Status = EAlertStatus::Escalated;
            
            // Send escalation notifications
            SendEscalationNotifications(Alert);
            
            OnAlertStatusChanged.Broadcast(Alert);
            break;
        }
    }
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetActiveAlerts() const
{
    return ActiveAlerts;
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetAlertsByType(EAlertType Type) const
{
    TArray<FRiskAlert> TypeAlerts;
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.Type == Type)
        {
            TypeAlerts.Add(Alert);
        }
    }
    return TypeAlerts;
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetAlertsByPriority(EAlertPriority Priority) const
{
    TArray<FRiskAlert> PriorityAlerts;
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.Priority == Priority)
        {
            PriorityAlerts.Add(Alert);
        }
    }
    return PriorityAlerts;
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetAlertsByCategory(ERiskCategory Category) const
{
    TArray<FRiskAlert> CategoryAlerts;
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.Category == Category)
        {
            CategoryAlerts.Add(Alert);
        }
    }
    return CategoryAlerts;
}

void UMingRiskAlertSystem::AddAlertRule(const FAlertRule& Rule)
{
    AlertRules.Add(Rule);
    UE_LOG(LogRiskAlert, Log, TEXT("Alert rule added: %s"), *Rule.RuleName.ToString());
}

void UMingRiskAlertSystem::RemoveAlertRule(const FName& RuleName)
{
    AlertRules.RemoveAll([&](const FAlertRule& Rule) {
        return Rule.RuleName == RuleName;
    });
    
    UE_LOG(LogRiskAlert, Log, TEXT("Alert rule removed: %s"), *RuleName.ToString());
}

void UMingRiskAlertSystem::EnableNotificationChannel(ENotificationChannel Channel)
{
    if (!EnabledChannels.Contains(Channel))
    {
        EnabledChannels.Add(Channel);
        UE_LOG(LogRiskAlert, Log, TEXT("Notification channel enabled: %s"), *UEnum::GetValueAsString(Channel));
    }
}

void UMingRiskAlertSystem::DisableNotificationChannel(ENotificationChannel Channel)
{
    EnabledChannels.Remove(Channel);
    UE_LOG(LogRiskAlert, Log, TEXT("Notification channel disabled: %s"), *UEnum::GetValueAsString(Channel));
}

void UMingRiskAlertSystem::StartMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRiskAlertSystem::PerformMonitoringCycle,
            MonitoringInterval,
            true);

        UE_LOG(LogRiskAlert, Log, TEXT("Alert monitoring started (interval: %.1f s)"), 
            MonitoringInterval);
    }
}

void UMingRiskAlertSystem::StopMonitoring()
{
    if (GEngine && GEngine->GetWorldFromContextObject(this))
    {
        GEngine->GetWorldFromContextObject(this)->GetTimerManager().ClearTimer(MonitoringTimer);
    }
    
    UE_LOG(LogRiskAlert, Log, TEXT("Alert monitoring stopped"));
}

void UMingRiskAlertSystem::SetMonitoringInterval(float Interval)
{
    MonitoringInterval = Interval;
    
    // Restart monitoring with new interval
    if (bEnableMonitoring)
    {
        StopMonitoring();
        StartMonitoring();
    }
}

FAlertStatistics UMingRiskAlertSystem::GetAlertStatistics() const
{
    FAlertStatistics Stats;
    
    // Count alerts by type
    for (const auto& Alert : AlertHistory)
    {
        switch (Alert.Type)
        {
            case EAlertType::Info: Stats.InfoCount++; break;
            case EAlertType::Warning: Stats.WarningCount++; break;
            case EAlertType::Critical: Stats.CriticalCount++; break;
            case EAlertType::Emergency: Stats.EmergencyCount++; break;
            case EAlertType::Notification: Stats.NotificationCount++; break;
            case EAlertType::Maintenance: Stats.MaintenanceCount++; break;
            case EAlertType::Security: Stats.SecurityCount++; break;
            case EAlertType::Stability: Stats.StabilityCount++; break;
        }
        
        Stats.TotalAlerts++;
        
        // Calculate resolution time
        if (Alert.Status == EAlertStatus::Resolved && Alert.ResolvedTime != FDateTime::MinValue())
        {
            FTimespan ResolutionTime = Alert.ResolvedTime - Alert.Timestamp;
            Stats.AverageResolutionTime += ResolutionTime.GetTotalSeconds();
            Stats.ResolvedCount++;
        }
    }
    
    // Calculate average resolution time
    if (Stats.ResolvedCount > 0)
    {
        Stats.AverageResolutionTime /= Stats.ResolvedCount;
    }
    
    // Calculate active alert count
    Stats.ActiveAlertCount = ActiveAlerts.Num();
    
    return Stats;
}

void UMingRiskAlertSystem::ClearAllAlerts()
{
    int32 ClearedCount = ActiveAlerts.Num();
    ActiveAlerts.Empty();
    
    UE_LOG(LogRiskAlert, Log, TEXT("Cleared %d active alerts"), ClearedCount);
    OnAllAlertsCleared.Broadcast();
}

void UMingRiskAlertSystem::ExportAlertHistory(const FString& FilePath) const
{
    UE_LOG(LogRiskAlert, Log, TEXT("Exporting alert history to: %s"), *FilePath);
    
    FString Report = TEXT("MingGoRTS Alert History Report\n");
    Report += TEXT("===============================\n\n");
    Report += FString::Printf(TEXT("Export Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Total Alerts: %d\n"), AlertHistory.Num());
    Report += FString::Printf(TEXT("Active Alerts: %d\n\n"), ActiveAlerts.Num());
    
    Report += TEXT("Alert Statistics:\n");
    Report += TEXT("-----------------\n");
    
    FAlertStatistics Stats = GetAlertStatistics();
    Report += FString::Printf(TEXT("- Total: %d\n"), Stats.TotalAlerts);
    Report += FString::Printf(TEXT("- Info: %d\n"), Stats.InfoCount);
    Report += FString::Printf(TEXT("- Warning: %d\n"), Stats.WarningCount);
    Report += FString::Printf(TEXT("- Critical: %d\n"), Stats.CriticalCount);
    Report += FString::Printf(TEXT("- Emergency: %d\n"), Stats.EmergencyCount);
    Report += FString::Printf(TEXT("- Resolved: %d\n"), Stats.ResolvedCount);
    Report += FString::Printf(TEXT("- Average Resolution Time: %.1f seconds\n\n"), Stats.AverageResolutionTime);
    
    Report += TEXT("Recent Alerts:\n");
    Report += TEXT("-------------\n");
    
    int32 RecentCount = FMath::Min(50, AlertHistory.Num());
    for (int32 i = AlertHistory.Num() - RecentCount; i < AlertHistory.Num(); ++i)
    {
        const FRiskAlert& Alert = AlertHistory[i];
        Report += FString::Printf(TEXT("- [%s] %s: %s\n"), 
            *Alert.Timestamp.ToString(),
            *Alert.Title,
            *Alert.Message);
    }
    
    // In a real implementation, you would save this to a file
    UE_LOG(LogRiskAlert, Log, TEXT("Report generated:\n%s"), *Report);
}

// Private helper functions

void UMingRiskAlertSystem::InitializeDefaultRules()
{
    // Default CPU usage rule
    FAlertRule CPURule;
    CPURule.RuleName = TEXT("HighCPUUsage");
    CPURule.MetricName = TEXT("CPUUsage");
    CPURule.Threshold = 80.0f;
    CPURule.CriticalThreshold = 95.0f;
    CPURule.AlertType = EAlertType::Warning;
    CPURule.Priority = EAlertPriority::High;
    CPURule.Category = ERiskCategory::Performance;
    CPURule.bEnabled = true;
    AlertRules.Add(CPURule);
    
    // Default memory usage rule
    FAlertRule MemoryRule;
    MemoryRule.RuleName = TEXT("HighMemoryUsage");
    MemoryRule.MetricName = TEXT("MemoryUsage");
    MemoryRule.Threshold = 85.0f;
    MemoryRule.CriticalThreshold = 98.0f;
    MemoryRule.AlertType = EAlertType::Warning;
    MemoryRule.Priority = EAlertPriority::High;
    MemoryRule.Category = ERiskCategory::Performance;
    MemoryRule.bEnabled = true;
    AlertRules.Add(MemoryRule);
    
    // Default error rate rule
    FAlertRule ErrorRule;
    ErrorRule.RuleName = TEXT("HighErrorRate");
    ErrorRule.MetricName = TEXT("ErrorRate");
    ErrorRule.Threshold = 2.0f;
    ErrorRule.CriticalThreshold = 5.0f;
    ErrorRule.AlertType = EAlertType::Critical;
    ErrorRule.Priority = EAlertPriority::Highest;
    ErrorRule.Category = ERiskCategory::Stability;
    ErrorRule.bEnabled = true;
    AlertRules.Add(ErrorRule);
}

void UMingRiskAlertSystem::ApplyAlertRules(FRiskAlert& Alert)
{
    for (const FAlertRule& Rule : AlertRules)
    {
        if (Rule.bEnabled && Rule.RuleName == Alert.Title)
        {
            // Apply rule modifications
            if (Rule.AlertType != EAlertType::Info)
            {
                Alert.Type = Rule.AlertType;
            }
            if (Rule.Priority != EAlertPriority::Normal)
            {
                Alert.Priority = Rule.Priority;
            }
            Alert.Category = Rule.Category;
            break;
        }
    }
}

void UMingRiskAlertSystem::SendAlertNotifications(const FRiskAlert& Alert)
{
    for (ENotificationChannel Channel : EnabledChannels)
    {
        switch (Channel)
        {
            case ENotificationChannel::InGame:
                SendInGameNotification(Alert);
                break;
            case ENotificationChannel::Dashboard:
                SendDashboardNotification(Alert);
                break;
            case ENotificationChannel::Log:
                SendLogNotification(Alert);
                break;
            case ENotificationChannel::Email:
                SendEmailNotification(Alert);
                break;
            case ENotificationChannel::Push:
                SendPushNotification(Alert);
                break;
        }
    }
}

void UMingRiskAlertSystem::SendInGameNotification(const FRiskAlert& Alert)
{
    // In a real implementation, this would display an in-game notification
    UE_LOG(LogRiskAlert, Log, TEXT("In-game notification: %s"), *Alert.Title);
}

void UMingRiskAlertSystem::SendDashboardNotification(const FRiskAlert& Alert)
{
    // In a real implementation, this would update the dashboard
    UE_LOG(LogRiskAlert, Log, TEXT("Dashboard notification: %s"), *Alert.Title);
}

void UMingRiskAlertSystem::SendLogNotification(const FRiskAlert& Alert)
{
    // Log the alert with appropriate severity
    switch (Alert.Priority)
    {
        case EAlertPriority::Highest:
        case EAlertPriority::Critical:
            UE_LOG(LogRiskAlert, Error, TEXT("[%s] %s: %s"), 
                *UEnum::GetValueAsString(Alert.Type), *Alert.Title, *Alert.Message);
            break;
        case EAlertPriority::High:
            UE_LOG(LogRiskAlert, Warning, TEXT("[%s] %s: %s"), 
                *UEnum::GetValueAsString(Alert.Type), *Alert.Title, *Alert.Message);
            break;
        default:
            UE_LOG(LogRiskAlert, Log, TEXT("[%s] %s: %s"), 
                *UEnum::GetValueAsString(Alert.Type), *Alert.Title, *Alert.Message);
            break;
    }
}

void UMingRiskAlertSystem::SendEmailNotification(const FRiskAlert& Alert)
{
    // In a real implementation, this would send an email
    UE_LOG(LogRiskAlert, Log, TEXT("Email notification: %s"), *Alert.Title);
}

void UMingRiskAlertSystem::SendPushNotification(const FRiskAlert& Alert)
{
    // In a real implementation, this would send a push notification
    UE_LOG(LogRiskAlert, Log, TEXT("Push notification: %s"), *Alert.Title);
}

void UMingRiskAlertSystem::SendEscalationNotifications(const FRiskAlert& Alert)
{
    // Send special notifications for escalated alerts
    UE_LOG(LogRiskAlert, Warning, TEXT("ALERT ESCALATED: %s [%s]"), 
        *Alert.Title, *UEnum::GetValueAsString(Alert.Priority));
}

void UMingRiskAlertSystem::PerformMonitoringCycle()
{
    // Check all alert rules
    for (const FAlertRule& Rule : AlertRules)
    {
        if (Rule.bEnabled)
        {
            CheckAlertRule(Rule);
        }
    }
    
    // Clean up old alerts
    CleanupOldAlerts();
}

void UMingRiskAlertSystem::CheckAlertRule(const FAlertRule& Rule)
{
    // In a real implementation, this would query actual metric values
    // For now, we'll use placeholder logic
    float CurrentValue = 0.0f;
    
    if (Rule.MetricName == TEXT("CPUUsage"))
    {
        CurrentValue = FMath::RandRange(20.0f, 90.0f);
    }
    else if (Rule.MetricName == TEXT("MemoryUsage"))
    {
        CurrentValue = FMath::RandRange(30.0f, 95.0f);
    }
    else if (Rule.MetricName == TEXT("ErrorRate"))
    {
        CurrentValue = FMath::RandRange(0.0f, 6.0f);
    }
    
    // Check if threshold is exceeded
    if (CurrentValue >= Rule.Threshold)
    {
        // Check if we already have an active alert for this rule
        bool bHasActiveAlert = false;
        for (const FRiskAlert& Alert : ActiveAlerts)
        {
            if (Alert.Title == Rule.RuleName)
            {
                bHasActiveAlert = true;
                break;
            }
        }
        
        // Create new alert if none exists
        if (!bHasActiveAlert)
        {
            EAlertType AlertType = (CurrentValue >= Rule.CriticalThreshold) ? 
                EAlertType::Critical : Rule.AlertType;
            EAlertPriority Priority = (CurrentValue >= Rule.CriticalThreshold) ? 
                EAlertPriority::Highest : Rule.Priority;
            
            CreateAlert(Rule.RuleName.ToString(), 
                FString::Printf(TEXT("Metric '%s' has exceeded threshold: %.2f"), 
                    *Rule.MetricName.ToString(), CurrentValue),
                AlertType, Priority, Rule.Category);
        }
    }
}

void UMingRiskAlertSystem::CleanupOldAlerts()
{
    // Remove alerts older than the retention period
    FDateTime CutoffTime = FDateTime::Now() - FTimespan::FromDays(AlertRetentionDays);
    
    AlertHistory.RemoveAll([&](const FRiskAlert& Alert) {
        return Alert.Timestamp < CutoffTime;
    });
}
