// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Auto-Alert Mechanism System - B2-3
// Provides automated risk detection and alerting

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskAlertSystem.generated.h"

UENUM(BlueprintType)
enum class EAlertType : uint8 {
    Info                UMETA(DisplayName = "Information"),
    Warning             UMETA(DisplayName = "Warning"),
    Critical            UMETA(DisplayName = "Critical"),
    Emergency           UMETA(DisplayName = "Emergency"),
    Notification        UMETA(DisplayName = "Notification"),
    Maintenance         UMETA(DisplayName = "Maintenance"),
    Security            UMETA(DisplayName = "Security"),
    Stability           UMETA(DisplayName = "Stability")
};

UENUM(BlueprintType)
enum class EAlertPriority : uint8 {
    Lowest              UMETA(DisplayName = "Lowest"),
    Low                 UMETA(DisplayName = "Low"),
    Normal              UMETA(DisplayName = "Normal"),
    High                UMETA(DisplayName = "High"),
    Highest             UMETA(DisplayName = "Highest"),
    Critical            UMETA(DisplayName = "Critical")
};

UENUM(BlueprintType)
enum class EAlertStatus : uint8 {
    New                 UMETA(DisplayName = "New"),
    Acknowledged        UMETA(DisplayName = "Acknowledged"),
    InProgress          UMETA(DisplayName = "In Progress"),
    Resolved            UMETA(DisplayName = "Resolved"),
    Dismissed           UMETA(DisplayName = "Dismissed"),
    Escalated           UMETA(DisplayName = "Escalated")
};

UENUM(BlueprintType)
enum class ENotificationChannel : uint8 {
    InGame              UMETA(DisplayName = "In-Game"),
    Email               UMETA(DisplayName = "Email"),
    Push                UMETA(DisplayName = "Push Notification"),
    Dashboard           UMETA(DisplayName = "Dashboard"),
    Log                 UMETA(DisplayName = "Log Only"),
    Sound               UMETA(DisplayName = "Sound Alert"),
    All                 UMETA(DisplayName = "All Channels")
};

USTRUCT(BlueprintType)
struct FAlertRule
{
    GENERATED_BODY()

    UPROPERTY()
    FString RuleID;

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
        , TriggerLevel(ERiskLevel::None)
        , AlertType(EAlertType::Info)
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
    FString AlertID;

    UPROPERTY()
    FString RuleID;

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
    int32 Timestamp;

    UPROPERTY()
    int32 AcknowledgedTimestamp;

    UPROPERTY()
    int32 ResolvedTimestamp;

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
        , AcknowledgedTimestamp(0)
        , ResolvedTimestamp(0)
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
    int32 TimeRangeStart;

    UPROPERTY()
    int32 TimeRangeEnd;

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
    void UnregisterAlertRule(const FString& RuleID);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void EnableAlertRule(const FString& RuleID, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void TriggerAlert(const FRiskAlert& Alert);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void TriggerCustomAlert(const FString& Title, const FString& Message, EAlertType Type, EAlertPriority Priority, ERiskCategory Category, ERiskLevel RiskLevel);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void AcknowledgeAlert(const FString& AlertID, const FString& AcknowledgedBy);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ResolveAlert(const FString& AlertID, const FString& ResolvedBy);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void DismissAlert(const FString& AlertID);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void EscalateAlert(const FString& AlertID);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CheckRiskLevelsAndTriggerAlerts();

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    TArray<FRiskAlert> GetActiveAlerts() const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    TArray<FRiskAlert> GetAlertHistory(const FAlertFilter& Filter) const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    FRiskAlert GetAlert(const FString& AlertID) const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    int32 GetActiveAlertCount() const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    int32 GetActiveAlertCountByType(EAlertType Type) const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    int32 GetActiveAlertCountByPriority(EAlertPriority Priority) const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    int32 GetActiveAlertCountByCategory(ERiskCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Risk Alert System")
    FAlertStatistics GetAlertStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void SetNotificationChannelEnabled(ENotificationChannel Channel, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void SendNotificationToChannel(ENotificationChannel Channel, const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ExportAlertsToFile(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CleanOldAlerts(int32 MaxAgeHours);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void ScheduleAlertDigest(float IntervalHours);

    UFUNCTION(BlueprintCallable, Category = "Risk Alert System")
    void CancelScheduledDigest();

protected:
    UPROPERTY()
    TMap<FString, FAlertRule> AlertRules;

    UPROPERTY()
    TArray<FRiskAlert> ActiveAlerts;

    UPROPERTY()
    TArray<FRiskAlert> AlertHistory;

    UPROPERTY()
    TMap<ENotificationChannel, bool> ChannelEnabled;

    UPROPERTY()
    FAlertStatistics Statistics;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlertTriggered, const FRiskAlert&, Alert, const FAlertRule&, Rule);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlertAcknowledged, const FString&, AlertID, const FString&, AcknowledgedBy);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlertResolved, const FString&, AlertID, const FString&, ResolvedBy);

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertTriggered OnAlertTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertAcknowledged OnAlertAcknowledged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Alert Events")
    FOnAlertResolved OnAlertResolved;

private:
    void ProcessAlertRule(const FAlertRule& Rule);
    bool ShouldTriggerAlert(const FAlertRule& Rule, ERiskLevel CurrentLevel);
    void UpdateStatistics();
    void SendNotification(const FRiskAlert& Alert);
    FString GenerateAlertMessage(const FAlertRule& Rule, const FRiskAlert& Alert) const;
};
