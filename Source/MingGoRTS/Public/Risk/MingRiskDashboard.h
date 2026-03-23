// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Risk Monitoring Dashboard - B2-1
// Provides comprehensive risk monitoring and visualization

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRiskDashboard.generated.h"

UENUM(BlueprintType)
enum class ERiskLevel: uint8 {
    None UMETA(DisplayName = "No Risk"),
    Low UMETA(DisplayName = "Low Risk"),
    Medium UMETA(DisplayName = "Medium Risk"),
    High UMETA(DisplayName = "High Risk"),
    Critical UMETA(DisplayName = "Critical Risk"),
    Emergency UMETA(DisplayName = "Emergency")
};

UENUM(BlueprintType)
enum class ERiskCategory: uint8 {
    General = 0 UMETA(DisplayName = "General"),
    Performance UMETA(DisplayName = "Performance"),
    Security UMETA(DisplayName = "Security"),
    Stability UMETA(DisplayName = "Stability"),
    Compatibility UMETA(DisplayName = "Compatibility"),
    Resource UMETA(DisplayName = "Resource"),
    Network UMETA(DisplayName = "Network"),
    AI UMETA(DisplayName = "AI Systems"),
    PlayerExperience UMETA(DisplayName = "Player Experience"),
    DataIntegrity UMETA(DisplayName = "Data Integrity")
};

UENUM(BlueprintType)
enum class EDashboardView: uint8 {
    Overview = 0 UMETA(DisplayName = "Overview"),
    Performance UMETA(DisplayName = "Performance"),
    Security UMETA(DisplayName = "Security"),
    AI UMETA(DisplayName = "AI Systems"),
    Network UMETA(DisplayName = "Network"),
    Player UMETA(DisplayName = "Player Stats"),
    Custom UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FRiskMetric
{
    GENERATED_BODY()

    UPROPERTY()
    FName MetricName;

    UPROPERTY()
    float CurrentValue;

    UPROPERTY()
    float Threshold;

    UPROPERTY()
    float CriticalThreshold;

    UPROPERTY()
    ERiskLevel RiskLevel;

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    int32 LastUpdateTime;

    UPROPERTY()
    TArray<float> HistoryValues;

    FRiskMetric()
        : CurrentValue(0.0f)
        , Threshold(0.0f)
        , CriticalThreshold(0.0f)
        , RiskLevel(ERiskLevel::None)
        , Category(ERiskCategory::General)
        , LastUpdateTime(0)
    {}
};

USTRUCT(BlueprintType)
struct FRiskAlert
{
    GENERATED_BODY()

    UPROPERTY()
    FString AlertID;

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ERiskLevel Level;

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    bool bIsAcknowledged;

    UPROPERTY()
    FString RecommendedAction;

    FRiskAlert()
        : Level(ERiskLevel::None)
        , Category(ERiskCategory::General)
        , bIsAcknowledged(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRiskLevelChanged, ERiskLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRiskAlertTriggered, FRiskAlert, Alert);

/**
 * Risk Dashboard System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRiskDashboard : public UObject
{
    GENERATED_BODY()

public:
    UMingRiskDashboard();

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void InitializeDashboard();

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void UpdateRiskMetrics();

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    ERiskLevel GetOverallRiskLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    TArray<FRiskMetric> GetRiskMetricsByCategory(ERiskCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void AddRiskMetric(const FRiskMetric& Metric);

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void UpdateRiskMetric(const FName& MetricName, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void RemoveRiskMetric(const FName& MetricName);

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    TArray<FRiskAlert> GetActiveAlerts() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void AcknowledgeAlert(const FString& AlertID);

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void SetDashboardView(EDashboardView View);

    UFUNCTION(BlueprintPure, Category = "Risk Dashboard")
    EDashboardView GetCurrentView() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void ExportRiskReport(const FString& FilePath);

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnRiskLevelChanged OnRiskLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnRiskAlertTriggered OnRiskAlertTriggered;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    ERiskLevel CurrentOverallRisk;

    UPROPERTY()
    EDashboardView CurrentView;

    UPROPERTY()
    TArray<FRiskMetric> RiskMetrics;

    UPROPERTY()
    TArray<FRiskAlert> ActiveAlerts;

    UPROPERTY()
    TMap<ERiskCategory, ERiskLevel> CategoryRiskLevels;

    void EvaluateRiskLevels();
    void GenerateAlert(const FRiskMetric& Metric);
    void ClearResolvedAlerts();
    FString GenerateAlertID() const;
};
