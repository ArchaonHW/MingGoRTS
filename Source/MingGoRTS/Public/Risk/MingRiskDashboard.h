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
    General = 0, UMETA(DisplayName = "General"),
    Performance, UMETA(DisplayName = "Performance"),
    Security, UMETA(DisplayName = "Security"),
    Stability, UMETA(DisplayName = "Stability"),
    Compatibility, UMETA(DisplayName = "Compatibility"),
    Resource, UMETA(DisplayName = "Resource"),
    Network, UMETA(DisplayName = "Network"),
    AI, UMETA(DisplayName = "AI System"),
    PlayerExperience, UMETA(DisplayName = "Player Experience"),
    DataIntegrity UMETA(DisplayName = "Data Integrity")
};

UENUM(BlueprintType)
enum class EDashboardView: uint8 {
    Overview = 0, UMETA(DisplayName = "Overview"),
    Performance, UMETA(DisplayName = "Performance"),
    Security, UMETA(DisplayName = "Security"),
    AI, UMETA(DisplayName = "AI Systems"),
    Network, UMETA(DisplayName = "Network"),
    Player, UMETA(DisplayName = "Player Stats"),
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
    uint32 LastUpdateTime;

    UPROPERTY()
    TArray<float> HistoryValues;

    FRiskMetric()
        : CurrentValue(0.0f)
        , Threshold(50.0f)
        , CriticalThreshold(80.0f)
        , RiskLevel(ERiskLevel::None)
        , Category(ERiskCategory::General)
        , LastUpdateTime(0)
    {}
};

USTRUCT(BlueprintType)
struct FRiskIndicator
{
    GENERATED_BODY()

    UPROPERTY()
    ERiskCategory Category;

    UPROPERTY()
    ERiskLevel OverallLevel;

    UPROPERTY()
    int32 ActiveRiskCount;

    UPROPERTY()
    int32 ResolvedRiskCount;

    UPROPERTY()
    FString Summary;

    UPROPERTY()
    FLinearColor DisplayColor;

    FRiskIndicator()
        : Category(ERiskCategory::General)
        , OverallLevel(ERiskLevel::None)
        , ActiveRiskCount(0)
        , ResolvedRiskCount(0)
        , DisplayColor(FLinearColor::Green)
    {}
};

USTRUCT(BlueprintType)
struct FDashboardConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    EDashboardView DefaultView;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    float UpdateInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    bool bEnableRealTimeUpdates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    bool bShowHistoryGraphs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    int32 HistoryDataPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    bool bEnableAlertNotifications;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashboard")
    TArray<ERiskCategory> MonitoredCategories;

    FDashboardConfig()
        : DefaultView(EDashboardView::Overview)
        , UpdateInterval(1.0f)
        , bEnableRealTimeUpdates(true)
        , bShowHistoryGraphs(true)
        , HistoryDataPoints(100)
        , bEnableAlertNotifications(true)
    {}
};





/**
 * Risk Monitoring Dashboard
 * Provides comprehensive risk monitoring and visualization
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRiskDashboard : public UObject
{
    GENERATED_BODY()

public:
    UMingRiskDashboard();
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void InitializeDashboard(const FDashboardConfig& Config);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void ShutdownDashboard();
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void UpdateDashboard();
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void SetCurrentView(EDashboardView View);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void RegisterRiskMetric(const FRiskMetric& Metric);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void UpdateRiskMetric(FName MetricName, float NewValue);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void UnregisterRiskMetric(FName MetricName);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    FRiskMetric GetRiskMetric(FName MetricName) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    TArray<FRiskMetric> GetAllRiskMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    TArray<FRiskMetric> GetRiskMetricsByCategory(ERiskCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    ERiskLevel GetOverallRiskLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    FRiskIndicator GetRiskIndicator(ERiskCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    TArray<FRiskIndicator> GetAllRiskIndicators() const;

    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void SetRiskThreshold(FName MetricName, float Threshold, float CriticalThreshold);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void AcknowledgeRisk(FName MetricName);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void ResolveRisk(FName MetricName);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void ExportDashboardData(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Risk Dashboard")
    void GenerateRiskReport();
    UFUNCTION(BlueprintPure, Category = "Risk Dashboard")
    EDashboardView GetCurrentView() const { return CurrentView; }

    UFUNCTION(BlueprintPure, Category = "Risk Dashboard")
    int32 GetActiveRiskCount() const;

    UFUNCTION(BlueprintPure, Category = "Risk Dashboard")
    int32 GetCriticalRiskCount() const;

    UFUNCTION(BlueprintPure, Category = "Risk Dashboard")
    bool IsRealTimeUpdatesEnabled() const { return Config.bEnableRealTimeUpdates; }

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnRiskLevelChanged OnRiskLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnDashboardUpdated OnDashboardUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnRiskDetected OnRiskDetected;

    UPROPERTY(BlueprintAssignable, Category = "Risk Dashboard Events")
    FOnMetricThresholdExceeded OnMetricThresholdExceeded;

protected:
    UPROPERTY()
    FDashboardConfig Config;

    UPROPERTY()
    TMap<FName, FRiskMetric> RiskMetrics;

    UPROPERTY()
    EDashboardView CurrentView;

    UPROPERTY()
    ERiskLevel OverallRiskLevel;

    UPROPERTY()
    FTimerHandle UpdateTimer;

    UPROPERTY()
    TArray<FString> AcknowledgedRisks;

    UPROPERTY()
    TArray<FString> ResolvedRisks;

    void ScheduleUpdates();
    void CancelUpdates();
    void EvaluateRiskLevels();
    ERiskLevel CalculateRiskLevel(float Value, float Threshold, float CriticalThreshold) const;
    void UpdateRiskIndicators();
    FLinearColor GetRiskColor(ERiskLevel Level) const;
    void NotifyRiskLevelChange(ERiskLevel OldLevel, ERiskLevel NewLevel);
    void LogRiskEvent(const FString& Event);
    void CleanupOldHistoryData();
    void CalculateTrendForMetric(FRiskMetric& Metric);
    static UMingRiskDashboard* Get(UObject* WorldContextObject);
};
