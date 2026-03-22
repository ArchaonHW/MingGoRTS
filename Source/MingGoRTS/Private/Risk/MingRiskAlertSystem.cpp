// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Auto-Alert Mechanism System Implementation - B2-3

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
    UE_LOG(LogRiskAlert, Log, TEXT("Risk Alert System initialized"));
    StartAlertMonitoring();
}

void UMingRiskAlertSystem::ShutdownAlertSystem()
{
    StopAlertMonitoring();
    ActiveAlerts.Empty();
    AlertHistory.Empty();
    UE_LOG(LogRiskAlert, Log, TEXT("Risk Alert System shutdown"));
}

void UMingRiskAlertSystem::RegisterAlertRule(const FAlertRule& Rule)
{
    AlertRules.Add(Rule.RuleID, Rule);
    UE_LOG(LogRiskAlert, Log, TEXT("Registered alert rule: %s (%s)"),
        *Rule.RuleName, *Rule.RuleID.ToString());
}

void UMingRiskAlertSystem::UnregisterAlertRule(FName RuleID)
{
    AlertRules.Remove(RuleID);
    UE_LOG(LogRiskAlert, Log, TEXT("Unregistered alert rule: %s"), *RuleID.ToString());
}

void UMingRiskAlertSystem::EnableAlertRule(FName RuleID, bool bEnabled)
{
    if (bEnabled)
    {
        DisabledRules.Remove(RuleID);
        UE_LOG(LogRiskAlert, Log, TEXT("Enabled alert rule: %s"), *RuleID.ToString());
    }
    else
    {
        DisabledRules.Add(RuleID);
        UE_LOG(LogRiskAlert, Log, TEXT("Disabled alert rule: %s"), *RuleID.ToString());
    }
}

void UMingRiskAlertSystem::TriggerAlert(const FRiskAlert& Alert)
{
    FRiskAlert NewAlert = Alert;
    if (NewAlert.AlertID.IsNone())
    {
        NewAlert.AlertID = GenerateAlertID();
    }
    NewAlert.Timestamp = FPlatformTime::Seconds();
    NewAlert.Status = EAlertStatus::New;

    ActiveAlerts.Add(NewAlert);
    AlertHistory.Add(NewAlert);

    // Notify channels
    NotifyChannels(NewAlert);

    // Auto-escalate if configured
    if (AlertRules.Contains(NewAlert.RuleID))
    {
        const FAlertRule& Rule = AlertRules[NewAlert.RuleID];
        if (Rule.bAutoEscalate && Rule.bRequireAcknowledgment)
        {
            // Escalation will be handled by timer
        }
    }

    OnAlertTriggered.Broadcast(NewAlert);

    UE_LOG(LogRiskAlert, Warning, TEXT("Alert triggered: [%s] %s - %s"),
        *UEnum::GetValueAsString(NewAlert.Type),
        *NewAlert.Title,
        *NewAlert.Message);
}

void UMingRiskAlertSystem::TriggerCustomAlert(const FString& Title, const FString& Message,
    EAlertType Type, EAlertPriority Priority)
{
    FRiskAlert Alert;
    Alert.AlertID = GenerateAlertID();
    Alert.Type = Type;
    Alert.Priority = Priority;
    Alert.Title = Title;
    Alert.Message = Message;
    Alert.Category = ERiskCategory::General;
    Alert.RiskLevel = Priority >= EAlertPriority::High ? ERiskLevel::High : ERiskLevel::Medium;

    TriggerAlert(Alert);
}

void UMingRiskAlertSystem::AcknowledgeAlert(FName AlertID, const FString& AcknowledgedBy)
{
    for (auto& Alert : ActiveAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Status = EAlertStatus::Acknowledged;
            Alert.AcknowledgedTime = FPlatformTime::Seconds();
            Alert.AcknowledgedBy = AcknowledgedBy;

            OnAlertAcknowledged.Broadcast(Alert);

            UE_LOG(LogRiskAlert, Log, TEXT("Alert acknowledged by %s: %s"),
                *AcknowledgedBy, *AlertID.ToString());
            return;
        }
    }
}

void UMingRiskAlertSystem::ResolveAlert(FName AlertID, const FString& ResolvedBy)
{
    for (int32 i = ActiveAlerts.Num() - 1; i >= 0; --i)
    {
        if (ActiveAlerts[i].AlertID == AlertID)
        {
            FRiskAlert Alert = ActiveAlerts[i];
            Alert.Status = EAlertStatus::Resolved;
            Alert.ResolvedTime = FPlatformTime::Seconds();
            Alert.ResolvedBy = ResolvedBy;

            // Move to history
            ActiveAlerts.RemoveAt(i);

            // Update in history
            for (auto& HistAlert : AlertHistory)
            {
                if (HistAlert.AlertID == AlertID)
                {
                    HistAlert = Alert;
                    break;
                }
            }

            OnAlertResolved.Broadcast(Alert);

            UE_LOG(LogRiskAlert, Log, TEXT("Alert resolved by %s: %s"),
                *ResolvedBy, *AlertID.ToString());
            return;
        }
    }
}

void UMingRiskAlertSystem::DismissAlert(FName AlertID)
{
    for (int32 i = ActiveAlerts.Num() - 1; i >= 0; --i)
    {
        if (ActiveAlerts[i].AlertID == AlertID)
        {
            FRiskAlert Alert = ActiveAlerts[i];
            Alert.Status = EAlertStatus::Dismissed;

            ActiveAlerts.RemoveAt(i);
            OnAlertDismissed.Broadcast(Alert);

            UE_LOG(LogRiskAlert, Log, TEXT("Alert dismissed: %s"), *AlertID.ToString());
            return;
        }
    }
}

void UMingRiskAlertSystem::EscalateAlert(FName AlertID)
{
    for (auto& Alert : ActiveAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.Status = EAlertStatus::Escalated;

            // Increase priority
            if (Alert.Priority < EAlertPriority::Critical)
            {
                Alert.Priority = static_cast<EAlertPriority>(static_cast<int32>(Alert.Priority) + 1);
            }

            // Re-notify with higher priority
            NotifyChannels(Alert);
            OnAlertEscalated.Broadcast(Alert);

            UE_LOG(LogRiskAlert, Warning, TEXT("Alert escalated: %s"), *AlertID.ToString());
            return;
        }
    }
}

void UMingRiskAlertSystem::CheckRiskLevelsAndTriggerAlerts()
{
    // This would integrate with the risk dashboard to check current metrics
    // and trigger alerts based on configured rules
    UE_LOG(LogRiskAlert, Verbose, TEXT("Checking risk levels for alert conditions"));
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetActiveAlerts() const
{
    return ActiveAlerts;
}

TArray<FRiskAlert> UMingRiskAlertSystem::GetAlertHistory(const FAlertFilter& Filter) const
{
    TArray<FRiskAlert> Result;

    for (const auto& Alert : AlertHistory)
    {
        // Apply filters
        if (Filter.Types.Num() > 0 && !Filter.Types.Contains(Alert.Type))
        {
            continue;
        }

        if (Filter.Priorities.Num() > 0 && !Filter.Priorities.Contains(Alert.Priority))
        {
            continue;
        }

        if (Filter.Statuses.Num() > 0 && !Filter.Statuses.Contains(Alert.Status))
        {
            continue;
        }

        if (Filter.Categories.Num() > 0 && !Filter.Categories.Contains(Alert.Category))
        {
            continue;
        }

        if (Filter.TimeRangeStart > 0 && Alert.Timestamp < Filter.TimeRangeStart)
        {
            continue;
        }

        if (Filter.TimeRangeEnd > 0 && Alert.Timestamp > Filter.TimeRangeEnd)
        {
            continue;
        }

        if (!Filter.SearchText.IsEmpty())
        {
            if (!Alert.Title.Contains(Filter.SearchText) && !Alert.Message.Contains(Filter.SearchText))
            {
                continue;
            }
        }

        if (!Filter.bShowAcknowledged && Alert.Status == EAlertStatus::Acknowledged)
        {
            continue;
        }

        if (!Filter.bShowResolved && Alert.Status == EAlertStatus::Resolved)
        {
            continue;
        }

        Result.Add(Alert);
    }

    return Result;
}

FRiskAlert UMingRiskAlertSystem::GetAlert(FName AlertID) const
{
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            return Alert;
        }
    }

    for (const auto& Alert : AlertHistory)
    {
        if (Alert.AlertID == AlertID)
        {
            return Alert;
        }
    }

    return FRiskAlert();
}

int32 UMingRiskAlertSystem::GetActiveAlertCount() const
{
    return ActiveAlerts.Num();
}

int32 UMingRiskAlertSystem::GetActiveAlertCountByType(EAlertType Type) const
{
    int32 Count = 0;
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.Type == Type)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRiskAlertSystem::GetActiveAlertCountByPriority(EAlertPriority Priority) const
{
    int32 Count = 0;
    for (const auto& Alert : ActiveAlerts)
    {
        if (Alert.Priority == Priority)
        {
            Count++;
        }
    }
    return Count;
}

FAlertStatistics UMingRiskAlertSystem::GetAlertStatistics() const
{
    FAlertStatistics Stats;

    Stats.TotalAlerts = AlertHistory.Num();
    Stats.ActiveAlerts = ActiveAlerts.Num();

    for (const auto& Alert : AlertHistory)
    {
        Stats.AlertsByType.FindOrAdd(Alert.Type)++;
        Stats.AlertsByCategory.FindOrAdd(Alert.Category)++;

        if (Alert.Status == EAlertStatus::Resolved)
        {
            Stats.ResolvedAlerts++;
        }

        if (Alert.Type == EAlertType::Critical)
        {
            Stats.CriticalAlerts++;
        }
        else if (Alert.Type == EAlertType::Warning)
        {
            Stats.WarningAlerts++;
        }
    }

    // Calculate average resolution time
    float TotalResolutionTime = 0.0f;
    int32 ResolvedCount = 0;
    for (const auto& Alert : AlertHistory)
    {
        if (Alert.Status == EAlertStatus::Resolved && Alert.ResolvedTime > Alert.Timestamp)
        {
            TotalResolutionTime += (Alert.ResolvedTime - Alert.Timestamp) / 60.0f; // minutes
            ResolvedCount++;
        }
    }

    Stats.AverageResolutionTime = ResolvedCount > 0 ? TotalResolutionTime / ResolvedCount : 0.0f;
    Stats.ResponseRate = Stats.TotalAlerts > 0 ? (static_cast<float>(Stats.ResolvedAlerts) / Stats.TotalAlerts) : 0.0f;

    return Stats;
}

void UMingRiskAlertSystem::SetNotificationChannelEnabled(ENotificationChannel Channel, bool bEnabled)
{
    if (bEnabled)
    {
        EnabledChannels.Add(Channel);
    }
    else
    {
        EnabledChannels.Remove(Channel);
    }

    UE_LOG(LogRiskAlert, Log, TEXT("Notification channel %s: %s"),
        *UEnum::GetValueAsString(Channel),
        bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMingRiskAlertSystem::SendNotificationToChannel(ENotificationChannel Channel, const FString& Message)
{
    if (!EnabledChannels.Contains(Channel))
    {
        return;
    }

    switch (Channel)
    {
    case ENotificationChannel::InGame:
        ShowInGameNotification(FRiskAlert());
        break;
    case ENotificationChannel::Log:
        LogAlert(FRiskAlert());
        break;
    case ENotificationChannel::Sound:
        PlayAlertSound(EAlertPriority::Normal);
        break;
    default:
        UE_LOG(LogRiskAlert, Log, TEXT("[%s] %s"), *UEnum::GetValueAsString(Channel), *Message);
        break;
    }
}

void UMingRiskAlertSystem::ExportAlertsToFile(const FString& FilePath)
{
    UE_LOG(LogRiskAlert, Log, TEXT("Exporting alerts to: %s"), *FilePath);

    FString JsonData = TEXT("[\n");

    for (int32 i = 0; i < AlertHistory.Num(); ++i)
    {
        const auto& Alert = AlertHistory[i];
        JsonData += TEXT("  {\n");
        JsonData += TEXT("    \"id\": \"") + Alert.AlertID.ToString() + TEXT("\",\n");
        JsonData += TEXT("    \"type\": \"") + UEnum::GetValueAsString(Alert.Type) + TEXT("\",\n");
        JsonData += TEXT("    \"priority\": \"") + UEnum::GetValueAsString(Alert.Priority) + TEXT("\",\n");
        JsonData += TEXT("    \"status\": \"") + UEnum::GetValueAsString(Alert.Status) + TEXT("\",\n");
        JsonData += TEXT("    \"title\": \"") + Alert.Title + TEXT("\",\n");
        JsonData += TEXT("    \"message\": \"") + Alert.Message + TEXT("\"\n");
        JsonData += TEXT("  }") + FString(i < AlertHistory.Num() - 1 ? "," : "") + TEXT("\n");
    }

    JsonData += TEXT("]\n");

    FFileHelper::SaveStringToFile(JsonData, *FilePath);
}

void UMingRiskAlertSystem::CleanupOldAlerts(int32 MaxAgeHours)
{
    uint32 CurrentTime = FPlatformTime::Seconds();
    uint32 MaxAgeSeconds = MaxAgeHours * 3600;

    for (int32 i = AlertHistory.Num() - 1; i >= 0; --i)
    {
        if (CurrentTime - AlertHistory[i].Timestamp > MaxAgeSeconds)
        {
            AlertHistory.RemoveAt(i);
        }
    }

    UE_LOG(LogRiskAlert, Log, TEXT("Cleaned up alerts older than %d hours"), MaxAgeHours);
}

void UMingRiskAlertSystem::ScheduleAlertDigest(float IntervalHours)
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            DigestTimer,
            this,
            &UMingRiskAlertSystem::GenerateAlertDigest,
            IntervalHours * 3600.0f,
            true
        );

        UE_LOG(LogRiskAlert, Log, TEXT("Scheduled alert digest every %.1f hours"), IntervalHours);
    }
}

void UMingRiskAlertSystem::CancelScheduledDigest()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(DigestTimer);
    }
}

void UMingRiskAlertSystem::StartAlertMonitoring()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            AlertCheckTimer,
            this,
            &UMingRiskAlertSystem::ProcessAlertRules,
            10.0f,
            true
        );

        UE_LOG(LogRiskAlert, Log, TEXT("Alert monitoring started"));
    }
}

void UMingRiskAlertSystem::StopAlertMonitoring()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(AlertCheckTimer);
    }
}

void UMingRiskAlertSystem::ProcessAlertRules()
{
    // Process configured alert rules
    for (const auto& Pair : AlertRules)
    {
        const FAlertRule& Rule = Pair.Value;

        if (DisabledRules.Contains(Rule.RuleID))
        {
            continue;
        }

        // Check cooldown
        if (IsRuleOnCooldown(Rule.RuleID))
        {
            continue;
        }

        // Alert rule processing would integrate with risk metrics
        // For now, this is a placeholder
    }

    // Check for alerts requiring escalation
    for (auto& Alert : ActiveAlerts)
    {
        if (Alert.Status == EAlertStatus::New && AlertRules.Contains(Alert.RuleID))
        {
            const FAlertRule& Rule = AlertRules[Alert.RuleID];
            if (Rule.bRequireAcknowledgment && Rule.bAutoEscalate)
            {
                uint32 ElapsedTime = FPlatformTime::Seconds() - Alert.Timestamp;
                if (ElapsedTime > Rule.EscalationDelay)
                {
                    AutoEscalateIfNeeded(Alert);
                }
            }
        }
    }
}

bool UMingRiskAlertSystem::ShouldTriggerAlert(const FAlertRule& Rule, const FRiskMetric& Metric)
{
    return Metric.RiskLevel >= Rule.TriggerLevel;
}

bool UMingRiskAlertSystem::IsRuleOnCooldown(FName RuleID)
{
    if (!LastAlertTimes.Contains(RuleID))
    {
        return false;
    }

    uint32 CurrentTime = FPlatformTime::Seconds();
    uint32 LastTime = LastAlertTimes[RuleID];

    const FAlertRule& Rule = AlertRules[RuleID];
    return (CurrentTime - LastTime) < Rule.CooldownDuration;
}

void UMingRiskAlertSystem::UpdateCooldown(FName RuleID)
{
    LastAlertTimes.Add(RuleID, FPlatformTime::Seconds());
}

FRiskAlert UMingRiskAlertSystem::CreateAlertFromRule(const FAlertRule& Rule, const FRiskMetric& Metric)
{
    FRiskAlert Alert;
    Alert.AlertID = GenerateAlertID();
    Alert.RuleID = Rule.RuleID;
    Alert.Type = Rule.AlertType;
    Alert.Priority = Rule.Priority;
    Alert.Category = Rule.MonitoredCategory;
    Alert.RiskLevel = Metric.RiskLevel;

    // Format message from template
    Alert.Title = Rule.RuleName;
    Alert.Message = Rule.AlertTemplate;
    Alert.Message.ReplaceInline(TEXT("{metric}"), *Metric.Description);
    Alert.Message.ReplaceInline(TEXT("{value}"), *FString::Printf(TEXT("%.1f"), Metric.CurrentValue));
    Alert.Message.ReplaceInline(TEXT("{level}"), *UEnum::GetValueAsString(Metric.RiskLevel));

    return Alert;
}

FName UMingRiskAlertSystem::GenerateAlertID()
{
    return FName(*FString::Printf(TEXT("ALT-%d-%d"), static_cast<int32>(FPlatformTime::Seconds()), FMath::RandRange(1000, 9999)));
}

void UMingRiskAlertSystem::NotifyChannels(const FRiskAlert& Alert)
{
    if (EnabledChannels.Contains(ENotificationChannel::InGame))
    {
        ShowInGameNotification(Alert);
    }

    if (EnabledChannels.Contains(ENotificationChannel::Dashboard))
    {
        // Dashboard notification
    }

    if (EnabledChannels.Contains(ENotificationChannel::Log))
    {
        LogAlert(Alert);
    }

    if (EnabledChannels.Contains(ENotificationChannel::Sound))
    {
        PlayAlertSound(Alert.Priority);
    }
}

void UMingRiskAlertSystem::ShowInGameNotification(const FRiskAlert& Alert)
{
    // Would integrate with UI system to show in-game notification
    UE_LOG(LogRiskAlert, Verbose, TEXT("In-game notification: [%s] %s"),
        *Alert.Title, *Alert.Message);
}

void UMingRiskAlertSystem::LogAlert(const FRiskAlert& Alert)
{
    UE_LOG(LogRiskAlert, Log, TEXT("[ALERT-%s] %s: %s"),
        *UEnum::GetValueAsString(Alert.Type),
        *Alert.Title,
        *Alert.Message);
}

void UMingRiskAlertSystem::PlayAlertSound(EAlertPriority Priority)
{
    // Would play appropriate alert sound based on priority
    UE_LOG(LogRiskAlert, Verbose, TEXT("Playing alert sound for priority: %s"),
        *UEnum::GetValueAsString(Priority));
}

void UMingRiskAlertSystem::UpdateAlertStatistics()
{
    // Statistics are calculated on-demand
}

void UMingRiskAlertSystem::AutoEscalateIfNeeded(FRiskAlert& Alert)
{
    uint32 CurrentTime = FPlatformTime::Seconds();
    uint32 ElapsedSeconds = CurrentTime - Alert.Timestamp;

    if (AlertRules.Contains(Alert.RuleID))
    {
        const FAlertRule& Rule = AlertRules[Alert.RuleID];
        if (ElapsedSeconds > Rule.EscalationDelay && Alert.Status == EAlertStatus::New)
        {
            EscalateAlert(Alert.AlertID);
        }
    }
}

void UMingRiskAlertSystem::GenerateAlertDigest()
{
    FAlertStatistics Stats = GetAlertStatistics();

    UE_LOG(LogRiskAlert, Log, TEXT("=== Alert Digest ==="));
    UE_LOG(LogRiskAlert, Log, TEXT("Total Alerts: %d"), Stats.TotalAlerts);
    UE_LOG(LogRiskAlert, Log, TEXT("Active Alerts: %d"), Stats.ActiveAlerts);
    UE_LOG(LogRiskAlert, Log, TEXT("Critical: %d, Warning: %d"), Stats.CriticalAlerts, Stats.WarningAlerts);
    UE_LOG(LogRiskAlert, Log, TEXT("Response Rate: %.1f%%"), Stats.ResponseRate * 100.0f);
}

static UMingRiskAlertSystem* UMingRiskAlertSystem::Get(UObject* WorldContextObject)
{
    static UMingRiskAlertSystem* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingRiskAlertSystem>();
        Instance->AddToRoot();
    }
    return Instance;
}
