// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Auto-Alert Mechanism System - B2-3
// Provides automated risk detection and alerting

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskAlertSystem.generated.h"

UENUM(BlueprintType)
enum class EAlertType : uint8
{
    Info = 0 UMETA(DisplayName = "Information"),
    Warning UMETA(DisplayName = "Warning"),
    Critical UMETA(DisplayName = "Critical"),
    Emergency UMETA(DisplayName = "Emergency"),
    Notification UMETA(DisplayName = "Notification"),
    Maintenance UMETA(DisplayName = "Maintenance"),
    Performance, UMETA(DisplayName = "Performance"),
    Security, UMETA(DisplayName = "Security"),
    Stability UMETA(DisplayName = "Stability")
};

UENUM(BlueprintType)
enum class EAlertPriority : uint8
{
    Lowest = 0 UMETA(DisplayName = "Lowest"),
    Low UMETA(DisplayName = "Low"),
    Normal UMETA(DisplayName = "Normal"),
    High, UMETA(DisplayName = "High"),
    Highest, UMETA(DisplayName = "Highest"),
    Critical UMETA(DisplayName = "Critical")
};

UENUM(BlueprintType)
enum class EAlertStatus : uint8
{
    New = 0 UMETA(DisplayName = "New"),
    Acknowledged UMETA(DisplayName = "Acknowledged"),
    InProgress UMETA(DisplayName = "In Progress"),
    Resolved, UMETA(DisplayName = "Resolved"),
    Dismissed, UMETA(DisplayName = "Dismissed"),
    Escalated UMETA(DisplayName = "Escalated")
};

UENUM(BlueprintType)
enum class ENotificationChannel : uint8
{
    InGame = 0 UMETA(DisplayName = "In-Game"),
    Email UMETA(DisplayName = "Email"),
    Push UMETA(DisplayName = "Push Notification"),
    Dashboard, UMETA(DisplayName = "Dashboard"),
    Log, UMETA(DisplayName = "Log Only"),
    Sound, UMETA(DisplayName = "Sound Alert"),
    All UMETA(DisplayName = "All Channels")
};

USTRUCT(BlueprintType)
struct FAlertRule
{
    GENERATED_BODY()

    UPROPERTY()
    FName RuleID;

    UPROPERTY()
    FString RuleName;

    UPROPERTY()
    ERiskCategory MonitoredCategory;

    UPROPERTY()
    ERiskLevel TriggerLevel;

    UPROPERTY()
    EAlertType AlertType;

    UPROPERTY()
    EAlertPriority Priority;

    UPROPERTY()
    bool bAutoTrigger;

    UPROPERTY()
    float CooldownDuration;

    UPROPERTY()
    TArray<ENotificationChannel> Channels;

    UPROPERTY()
    FString AlertTemplate;

    UPROPERTY()
    bool bRequireAcknowledgment;

    UPROPERTY()
    bool bAutoEscalate;

    UPROPERTY()
    float EscalationDelay;

    FAlertRule()
        : RuleID(NAME_None)
        , MonitoredCategory(ERiskCategory::General)
        , TriggerLevel(ERiskLevel::High)
        , AlertType(EAlertType::Warning)
        , Priority(EAlertPriority::Normal)
        , bAutoTrigger(true)
        , CooldownDuration(300.0f)
        , bRequireAcknowledgment(false)
        , bAutoEscalate(false)
        , EscalationDelay(600.0f)
    {}
};

USTRUCT(BlueprintType)
struct FRiskAlert
{
    GENERATED_BODY()

    UPROPERTY()
    FName AlertID;

    UPROPERTY()
    FName RuleID;

    UPROPERTY()
    EAlertType Type;

    UPROPERTY()
    EAlertPriority Priority;

    UPROPERTY()
    EAlertStatus Status;

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Message;

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    ERiskLevel RiskLevel;

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    uint32 AcknowledgedTime;

    UPROPERTY()
    uint32 ResolvedTime;

    UPROPERTY()
    FString AcknowledgedBy;

    UPROPERTY()
    FString ResolvedBy;

    UPROPERTY()
    TArray<FString> ActionsTaken;

    FRiskAlert()
        : AlertID(NAME_None)
        , RuleID(NAME_None)
        , Type(EAlertType::Info)
        , Priority(EAlertPriority::Normal)
        , Status(EAlertStatus::New)
        , Category(ERiskCategory::General)
        , RiskLevel(ERiskLevel::None)
        , Timestamp(0)
        , AcknowledgedTime(0)
        , ResolvedTime(0)
    {}
};

USTRUCT(BlueprintType)
struct FAlertStatistics
{
    GENERATED_BODY()

    UPROPERTY()
    int32 TotalAlerts;

    UPROPERTY()
    int32 ActiveAlerts;

    UPROPERTY()
    int32 ResolvedAlerts;

    UPROPERTY()
    int32 CriticalAlerts;

    UPROPERTY()
    int32 WarningAlerts;

    UPROPERTY()
    float AverageResolutionTime;

    UPROPERTY()
    float ResponseRate;

    UPROPERTY()
    TMap<EAlertType, int32> AlertsByType;

    UPROPERTY()
    TMap<ERiskCategory, int32> AlertsByCategory;

    FAlertStatistics()
        : TotalAlerts(0)
        , ActiveAlerts(0)
        , ResolvedAlerts(0)
        , CriticalAlerts(0)
        , WarningAlerts(0)
        , AverageResolutionTime(0.0f)
        , ResponseRate(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAlertFilter
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<EAlertType> Types;

    UPROPERTY()
    TArray<EAlertPriority> Priorities;

    UPROPERTY()
    TArray<EAlertStatus> Statuses;

    UPROPERTY()
    TArray<ERiskCategory> Categories;

    UPROPERTY()
    uint32 TimeRangeStart;

    UPROPERTY()
    uint32 TimeRangeEnd;

    UPROPERTY()
    FString SearchText;

    UPROPERTY()
    bool bShowAcknowledged;

    UPROPERTY()
    bool bShowResolved;

    FAlertFilter()
        : TimeRangeStart(0)
        , TimeRangeEnd(0)
        , bShowAcknowledged(true)
        , bShowResolved(true)
    {}
};






/**
 * Auto-Alert Mechanism System
 * Provides automated risk detection and alerting
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRiskAlertSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRiskAlertSystem();
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void InitializeAlertSystem();
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ShutdownAlertSystem();
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void RegisterAlertRule(const FAlertRule& Rule);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void UnregisterAlertRule(FName RuleID);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void EnableAlertRule(FName RuleID, bool bEnabled);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void TriggerAlert(const FRiskAlert& Alert);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void TriggerCustomAlert(const FString& Title, const FString& Message, EAlertType Type, EAlertPriority Priority);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void AcknowledgeAlert(FName AlertID, const FString& AcknowledgedBy);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ResolveAlert(FName AlertID, const FString& ResolvedBy);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void DismissAlert(FName AlertID);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void EscalateAlert(FName AlertID);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CheckRiskLevelsAndTriggerAlerts();
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    TArray<FRiskAlert> GetActiveAlerts() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    TArray<FRiskAlert> GetAlertHistory(const FAlertFilter& Filter) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    FRiskAlert GetAlert(FName AlertID) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    int32 GetActiveAlertCount() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    int32 GetActiveAlertCountByType(EAlertType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    int32 GetActiveAlertCountByPriority(EAlertPriority Priority) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    FAlertStatistics GetAlertStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void SetNotificationChannelEnabled(ENotificationChannel Channel, bool bEnabled);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void SendNotificationToChannel(ENotificationChannel Channel, const FString& Message);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ExportAlertsToFile(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CleanupOldAlerts(int32 MaxAgeHours);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ScheduleAlertDigest(float IntervalHours);
    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CancelScheduledDigest();
    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertTriggered OnAlertTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertAcknowledged OnAlertAcknowledged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertResolved OnAlertResolved;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertEscalated OnAlertEscalated;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertDismissed OnAlertDismissed;

protected:
    UPROPERTY()
    TMap<FName, FAlertRule> AlertRules;

    UPROPERTY()
    TArray<FRiskAlert> ActiveAlerts;

    UPROPERTY()
    TArray<FRiskAlert> AlertHistory;

    UPROPERTY()
    TSet<FName> DisabledRules;

    UPROPERTY()
    TSet<ENotificationChannel> EnabledChannels;

    UPROPERTY()
    FTimerHandle AlertCheckTimer;

    UPROPERTY()
    FTimerHandle DigestTimer;

    UPROPERTY()
    TMap<FName, uint32> LastAlertTimes;

    void StartAlertMonitoring();
    void StopAlertMonitoring();
    void ProcessAlertRules();
    bool ShouldTriggerAlert(const FAlertRule& Rule, const FRiskMetric& Metric);
    bool IsRuleOnCooldown(FName RuleID);
    void UpdateCooldown(FName RuleID);
    FRiskAlert CreateAlertFromRule(const FAlertRule& Rule, const FRiskMetric& Metric);
    FName GenerateAlertID();
    void NotifyChannels(const FRiskAlert& Alert);
    void ShowInGameNotification(const FRiskAlert& Alert);
    void LogAlert(const FRiskAlert& Alert);
    void PlayAlertSound(EAlertPriority Priority);
    void UpdateAlertStatistics();
    void AutoEscalateIfNeeded(FRiskAlert& Alert);
    void GenerateAlertDigest();
    static UMingRiskAlertSystem* Get(UObject* WorldContextObject);
};
