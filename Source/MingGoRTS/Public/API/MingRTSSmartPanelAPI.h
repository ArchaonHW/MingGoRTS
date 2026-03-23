#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSmartPanelAPI.generated.h"

UENUM(BlueprintType)
enum class EPanelType : uint8
{
    Dashboard,
    Monitoring,
    Analysis,
    Control,
    Configuration,
    Reporting,
    Alert,
    Automation
};

UENUM(BlueprintType)
enum class EPanelMode : uint8
{
    Overview,
    Detailed,
    Comparison,
    Historical,
    Predictive,
    Interactive
};

USTRUCT(BlueprintType)
struct FPanelWidget
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString WidgetId;

    UPROPERTY(BlueprintReadOnly)
    FString WidgetType;

    UPROPERTY(BlueprintReadOnly)
    FString Title;

    UPROPERTY(BlueprintReadOnly)
    FVector2D Position;

    UPROPERTY(BlueprintReadOnly)
    FVector2D Size;

    UPROPERTY(BlueprintReadOnly)
    bool bVisible;

    UPROPERTY(BlueprintReadOnly)
    bool bInteractive;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Properties;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Actions;

    FPanelWidget()
    {
        Position = FVector2D(0.0f, 0.0f);
        Size = FVector2D(100.0f, 100.0f);
        bVisible = true;
        bInteractive = true;
    }
};

USTRUCT(BlueprintType)
struct FPanelConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EPanelType PanelType;

    UPROPERTY(BlueprintReadOnly)
    EPanelMode Mode;

    UPROPERTY(BlueprintReadOnly)
    FString PanelName;

    UPROPERTY(BlueprintReadOnly)
    TArray<FPanelWidget> Widgets;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Settings;

    UPROPERTY(BlueprintReadOnly)
    FString RefreshInterval;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoRefresh;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> DataSources;

    FPanelConfiguration()
    {
        PanelType = EPanelType::Dashboard;
        Mode = EPanelMode::Overview;
        bAutoRefresh = true;
        RefreshInterval = TEXT("5s");
    }
};

USTRUCT(BlueprintType)
struct FPanelData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DataSource;

    UPROPERTY(BlueprintReadOnly)
    FString DataType;

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    UPROPERTY(BlueprintReadOnly)
    FString Unit;

    UPROPERTY(BlueprintReadOnly)
    FString Trend;

    UPROPERTY(BlueprintReadOnly)
    FString Status;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    FPanelData()
    {
        LastUpdated = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FPanelAlert
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString AlertId;

    UPROPERTY(BlueprintReadOnly)
    FString Title;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString Severity;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString Source;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Actions;

    UPROPERTY(BlueprintReadOnly)
    bool bAcknowledged;

    FPanelAlert()
    {
        Timestamp = FDateTime::Now();
        bAcknowledged = false;
    }
};

/**
 * 智慧板控API - 具備自我學習能力的控制面板系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSmartPanelAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSmartPanelAPI();

    // 核心面板功能
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API")
    FPanelConfiguration CreatePanel(EPanelType Type, const FString& PanelName);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API")
    bool UpdatePanel(const FString& PanelId, const FPanelConfiguration& Configuration);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API")
    FPanelConfiguration GetPanel(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API")
    TArray<FPanelConfiguration> GetAllPanels();

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API")
    bool DeletePanel(const FString& PanelId);

    // 組件管理
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Widgets")
    FPanelWidget AddWidget(const FString& PanelId, const FPanelWidget& Widget);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Widgets")
    bool UpdateWidget(const FString& PanelId, const FString& WidgetId, const FPanelWidget& Widget);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Widgets")
    bool RemoveWidget(const FString& PanelId, const FString& WidgetId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Widgets")
    TArray<FPanelWidget> GetPanelWidgets(const FString& PanelId);

    // 數據管理
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Data")
    void UpdatePanelData(const FString& PanelId, const FString& WidgetId, const FPanelData& Data);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Data")
    FPanelData GetPanelData(const FString& PanelId, const FString& WidgetId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Data")
    TArray<FPanelData> GetPanelAllData(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Data")
    void SetDataSource(const FString& PanelId, const FString& WidgetId, const FString& DataSource);

    // 警報系統
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Alerts")
    void CreateAlert(const FPanelAlert& Alert);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Alerts")
    void AcknowledgeAlert(const FString& AlertId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Alerts")
    TArray<FPanelAlert> GetActiveAlerts();

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Alerts")
    void ClearAlert(const FString& AlertId);

    // 自我學習功能
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Learning")
    void LearnFromUserInteraction(const FString& PanelId, const FString& Action, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Learning")
    void OptimizePanelLayout(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Learning")
    FPanelConfiguration SuggestPanelImprovements(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Learning")
    void UpdateUserPreferences(const TMap<FString, FString>& Preferences);

    // 智慧功能
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Intelligence")
    FPanelConfiguration GenerateSmartPanel(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Intelligence")
    TArray<FPanelWidget> SuggestRelevantWidgets(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Intelligence")
    void AutoConfigurePanel(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Intelligence")
    void EnablePredictiveMode(const FString& PanelId, bool bEnabled);

    // 自動化
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Automation")
    void CreateAutomationRule(const FString& RuleName, const TMap<FString, FString>& Conditions, const TArray<FString>& Actions);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Automation")
    void ExecuteAutomationRule(const FString& RuleName);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Automation")
    TArray<FString> GetAutomationRules();

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Automation")
    void DeleteAutomationRule(const FString& RuleName);

    // 跨API調用
    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Integration")
    void RequestLogData(const FString& PanelId, const FString& WidgetId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Integration")
    void RequestPerformanceMetrics(const FString& PanelId);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Integration")
    void RequestDebuggingSession(const FString& PanelId, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Smart Panel API|Integration")
    void RequestCompilationStatus(const FString& PanelId);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPanelCreated, const FString&, PanelId, const FPanelConfiguration&, Configuration);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPanelDataUpdated, const FString&, PanelId, const FString&, WidgetId, const FPanelData&, Data);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertTriggered, const FPanelAlert&, Alert);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUserInteraction, const FString&, PanelId, const FString&, Action);

    UPROPERTY(BlueprintAssignable)
    FOnPanelCreated OnPanelCreated;

    UPROPERTY(BlueprintAssignable)
    FOnPanelDataUpdated OnPanelDataUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnAlertTriggered OnAlertTriggered;

    UPROPERTY(BlueprintAssignable)
    FOnUserInteraction OnUserInteraction;

private:
    // 學習數據
    UPROPERTY()
    TArray<FPanelConfiguration> Panels;

    UPROPERTY()
    TMap<FString, TArray<FPanelData>> PanelDataMap;

    UPROPERTY()
    TArray<FPanelAlert> ActiveAlerts;

    UPROPERTY()
    TMap<FString, TMap<FString, int32>> UserInteractions;

    UPROPERTY()
    TMap<FString, FString> UserPreferences;

    UPROPERTY()
    TMap<FString, TMap<FString, FString>> AutomationRules;

    // 內部方法
    void InitializePanelDatabase();
    void SavePanelData();
    void LoadPanelData();
    void AnalyzeUserBehavior(const FString& PanelId);
    FPanelConfiguration OptimizeLayoutForUser(const FPanelConfiguration& BasePanel);
    bool EvaluateAutomationConditions(const TMap<FString, FString>& Conditions);
    void ExecuteAutomationActions(const TArray<FString>& Actions);
};
