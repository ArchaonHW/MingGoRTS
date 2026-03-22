#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "RiskMonitoringDashboard.generated.h"

UENUM(BlueprintType)
enum class ERiskLevel : uint8
{
    Critical    UMETA(DisplayName = "Critical"),
    High        UMETA(DisplayName = "High"),
    Medium      UMETA(DisplayName = "Medium"),
    Low         UMETA(DisplayName = "Low"),
    Minimal     UMETA(DisplayName = "Minimal")
};

UENUM(BlueprintType)
enum class ERiskCategory : uint8
{
    Technical   UMETA(DisplayName = "Technical"),
    Schedule    UMETA(DisplayName = "Schedule"),
    Budget      UMETA(DisplayName = "Budget"),
    Quality     UMETA(DisplayName = "Quality"),
    Resource    UMETA(DisplayName = "Resource"),
    External    UMETA(DisplayName = "External"),
    Security    UMETA(DisplayName = "Security"),
    Performance UMETA(DisplayName = "Performance")
};

UENUM(BlueprintType)
enum class ERiskStatus : uint8
{
    Active      UMETA(DisplayName = "Active"),
    Mitigated   UMETA(DisplayName = "Mitigated"),
    Accepted    UMETA(DisplayName = "Accepted"),
    Transferred UMETA(DisplayName = "Transferred"),
    Closed      UMETA(DisplayName = "Closed")
};

USTRUCT(BlueprintType)
struct FRiskItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FString RiskID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    ERiskCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    ERiskLevel Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    ERiskStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    float Probability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    float Impact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    float RiskScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FString MitigationStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FString Owner;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FDateTime IdentifiedDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    FDateTime LastUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    TArray<FString> RelatedTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Item")
    TArray<FString> Tags;
};

USTRUCT(BlueprintType)
struct FRiskMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    int32 TotalRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    int32 CriticalRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    int32 HighRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    int32 MediumRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    int32 LowRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    float AverageRiskScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    float RiskTrend;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    TMap<ERiskCategory, int32> CategoryDistribution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    TMap<ERiskStatus, int32> StatusDistribution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Metrics")
    FDateTime LastCalculated;
};

USTRUCT(BlueprintType)
struct FRiskAlert
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FString AlertID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FString RiskID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FString AlertTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FString AlertMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    ERiskLevel Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    bool bIsRead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Risk Alert")
    FString ActionRequired;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRiskAdded, const FRiskItem&, NewRisk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRiskUpdated, const FRiskItem&, UpdatedRisk, ERiskLevel, PreviousLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRiskAlert, const FRiskAlert&, Alert);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRiskMetricsUpdated, const FRiskMetrics&, NewMetrics);

/**
 * 風險監控儀表板
 * 提供實時風險評估、監控和預警功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API URiskMonitoringDashboard : public UObject
{
    GENERATED_BODY()

public:
    URiskMonitoringDashboard();

    // 初始化風險監控儀表板
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void InitializeDashboard();

    // 添加風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    bool AddRiskItem(const FRiskItem& RiskItem);

    // 更新風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    bool UpdateRiskItem(const FString& RiskID, const FRiskItem& UpdatedRisk);

    // 刪除風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    bool RemoveRiskItem(const FString& RiskID);

    // 獲取風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    FRiskItem GetRiskItem(const FString& RiskID) const;

    // 獲取所有風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetAllRiskItems() const;

    // 按類別獲取風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetRisksByCategory(ERiskCategory Category) const;

    // 按級別獲取風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetRisksByLevel(ERiskLevel Level) const;

    // 按狀態獲取風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetRisksByStatus(ERiskStatus Status) const;

    // 計算風險指標
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    FRiskMetrics CalculateRiskMetrics();

    // 獲取當前風險指標
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    FRiskMetrics GetCurrentRiskMetrics() const { return CurrentMetrics; }

    // 創建風險預警
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void CreateRiskAlert(const FString& RiskID, const FString& AlertMessage, ERiskLevel Severity);

    // 獲取風險預警
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskAlert> GetRiskAlerts(bool bUnreadOnly = false) const;

    // 標記預警為已讀
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void MarkAlertAsRead(const FString& AlertID);

    // 清除預警
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void ClearAlert(const FString& AlertID);

    // 清除所有預警
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void ClearAllAlerts();

    // 自動風險評估
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void PerformAutomaticRiskAssessment();

    // 生成風險報告
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    FString GenerateRiskReport() const;

    // 獲取風險趨勢分析
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<float> GetRiskTrendAnalysis(int32 Days = 30) const;

    // 獲取高風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetHighRiskItems() const;

    // 獲取需要立即關注的風險
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> GetCriticalRisks() const;

    // 設置風險閾值
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void SetRiskThreshold(ERiskLevel Level, float Threshold);

    // 獲取風險閾值
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    float GetRiskThreshold(ERiskLevel Level) const;

    // 導出風險數據
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    bool ExportRiskData(const FString& FilePath) const;

    // 導入風險數據
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    bool ImportRiskData(const FString& FilePath);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Risk Monitoring Dashboard")
    FOnRiskAdded OnRiskAdded;

    UPROPERTY(BlueprintAssignable, Category = "Risk Monitoring Dashboard")
    FOnRiskUpdated OnRiskUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Risk Monitoring Dashboard")
    FOnRiskAlert OnRiskAlert;

    UPROPERTY(BlueprintAssignable, Category = "Risk Monitoring Dashboard")
    FOnRiskMetricsUpdated OnRiskMetricsUpdated;

protected:
    // 初始化默認風險項目
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void InitializeDefaultRisks();

    // 計算風險評分
    float CalculateRiskScore(float Probability, float Impact) const;

    // 檢查風險閾值
    UFUNCTION(BlueprintCallable, Category = "Risk Monitoring Dashboard")
    void CheckRiskThresholds();

    // 更新風險趨勢
    void UpdateRiskTrend();

    // 生成預警消息
    FString GenerateAlertMessage(const FRiskItem& RiskItem) const;

    // 驗證風險項目
    bool ValidateRiskItem(const FRiskItem& RiskItem) const;

    // 風險項目列表
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    TArray<FRiskItem> RiskItems;

    // 風險預警列表
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    TArray<FRiskAlert> RiskAlerts;

    // 當前風險指標
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    FRiskMetrics CurrentMetrics;

    // 風險閾值
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    TMap<ERiskLevel, float> RiskThresholds;

    // 風險趨勢數據
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    TArray<float> RiskTrendData;

    // 自動評估間隔（秒）
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    float AutoAssessmentInterval;

    // 最後評估時間
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    FDateTime LastAssessmentTime;

    // 是否啟用自動評估
    UPROPERTY(BlueprintReadOnly, Category = "Risk Monitoring Dashboard")
    bool bAutoAssessmentEnabled;

private:
};
