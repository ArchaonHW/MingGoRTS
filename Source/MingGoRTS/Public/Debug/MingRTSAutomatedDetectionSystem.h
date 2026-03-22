// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 自動化檢測系統頭文件

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "MingRTSAutomatedDetectionSystem.generated.h"

/**
 * 檢測類型枚舉
 */
UENUM(BlueprintType)
enum class EDetectionType: uint8 {
    ProjectHealth         UMETA(DisplayName = "項目健康"),
    CodeQuality           UMETA(DisplayName = "代碼品質"),
    PerformanceMetrics    UMETA(DisplayName = "性能指標"),
    AssetIntegrity        UMETA(DisplayName = "資源完整性"),
    ConfigurationCheck    UMETA(DisplayName = "配置檢查"),
    DependencyCheck       UMETA(DisplayName = "依賴檢查"),
    SecurityScan          UMETA(DisplayName = "安全掃描"),
    LocalizationCheck     UMETA(DisplayName = "本地化檢查"),
    MemoryAnalysis        UMETA(DisplayName = "記憶體分析"),
    NetworkValidation     UMETA(DisplayName = "網絡驗證")
};

/**
 * 檢測狀態
 */
UENUM(BlueprintType)
enum class EDetectionStatus: uint8 {
    NotStarted    UMETA(DisplayName = "未開始"),
    InProgress    UMETA(DisplayName = "進行中"),
    Passed        UMETA(DisplayName = "通過"),
    Failed        UMETA(DisplayName = "失敗"),
    Warning       UMETA(DisplayName = "警告"),
    Skipped       UMETA(DisplayName = "已跳過")
};

/**
 * 檢測結果嚴重級別
 */
UENUM(BlueprintType)
enum class EDetectionSeverity: uint8 {
    Info          UMETA(DisplayName = "資訊"),
    Low           UMETA(DisplayName = "低"),
    Medium        UMETA(DisplayName = "中"),
    High          UMETA(DisplayName = "高"),
    Critical      UMETA(DisplayName = "嚴重")
};

/**
 * 檢測發現的問題
 */
USTRUCT(BlueprintType)
struct FDetectionFinding
{
    GENERATED_BODY()
    
    /** 發現ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName FindingID;
    
    /** 檢測類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionType DetectionType;
    
    /** 嚴重級別 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionSeverity Severity;
    
    /** 問題標題 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    /** 詳細描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    /** 文件/資源路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetPath;
    
    /** 建議解決方案 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Recommendation;
    
    /** 參考文檔鏈接 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DocumentationLink;
    
    /** 是否可自動修復 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAutoFixable;
    
    /** 檢測時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DetectionTime;
    
    /** 相關數據 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> Metadata;
    
    FDetectionFinding()
        : DetectionType(EDetectionType::ProjectHealth)
        , Severity(EDetectionSeverity::Info)
        , bAutoFixable(false)
    {}
};

/**
 * 檢測指標數據
 */
USTRUCT(BlueprintType)
struct FDetectionMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MetricName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentValue;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExpectedValue;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Unit;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPassed;
    
    FDetectionMetrics()
        : CurrentValue(0.0f)
        , ExpectedValue(0.0f)
        , MinThreshold(0.0f)
        , MaxThreshold(0.0f)
        , bIsPassed(false)
    {}
};

/**
 * 單個檢測任務結果
 */
USTRUCT(BlueprintType)
struct FDetectionTaskResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionType DetectionType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTimeSeconds;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDetectionFinding> Findings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDetectionMetrics> Metrics;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Summary;
    
    FDetectionTaskResult()
        : DetectionType(EDetectionType::ProjectHealth)
        , Status(EDetectionStatus::NotStarted)
        , ExecutionTimeSeconds(0.0f)
    {}
};

/**
 * 檢測配置
 */
USTRUCT(BlueprintType)
struct FDetectionConfig
{
    GENERATED_BODY()
    
    /** 檢測名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DetectionName;
    
    /** 啟用的檢測類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EDetectionType> EnabledDetections;
    
    /** 目標路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ScanPaths;
    
    /** 排除路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ExcludePaths;
    
    /** 並行檢測數量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ParallelDetectionCount;
    
    /** 檢測超時時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionTimeoutSeconds;
    
    /** 輸出報告路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OutputReportPath;
    
    /** 是否生成詳細報告 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGenerateDetailedReport;
    
    /** 是否啟用自動修復 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableAutoFix;
    
    /** 嚴重級別閾值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionSeverity MinSeverityThreshold;
    
    /** 是否發送通知 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSendNotifications;
    
    FDetectionConfig()
        : ParallelDetectionCount(4)
        , DetectionTimeoutSeconds(600.0f)
        , bGenerateDetailedReport(true)
        , bEnableAutoFix(false)
        , MinSeverityThreshold(EDetectionSeverity::Low)
        , bSendNotifications(true)
    {}
};

/**
 * 完整檢測結果
 */
USTRUCT(BlueprintType)
struct FDetectionResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DetectionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExecutionTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDetectionTaskResult> TaskResults;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalFindings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CriticalFindings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HighFindings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MediumFindings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LowFindings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AutoFixedCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallHealthScore;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ReportFilePath;
    
    FDetectionResult()
        : TotalExecutionTime(0.0f)
        , TotalFindings(0)
        , CriticalFindings(0)
        , HighFindings(0)
        , MediumFindings(0)
        , LowFindings(0)
        , AutoFixedCount(0)
        , OverallHealthScore(100.0f)
    {}
};

/**
 * 自動化檢測系統
 * 提供全面的項目健康檢測、代碼品質檢測、性能檢測等功能
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSAutomatedDetectionSystem : public UObject
{
    GENERATED_BODY()
    
public:
    /** 初始化檢測系統 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    void InitializeDetectionSystem();
    
    /** 執行完整檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionResult RunFullDetection(const FDetectionConfig& Config);
    
    /** 執行單項檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult RunSingleDetection(EDetectionType DetectionType, const TArray<FString>& TargetPaths);
    
    /** 項目健康檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectProjectHealth();
    
    /** 代碼品質檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectCodeQuality(const TArray<FString>& TargetPaths);
    
    /** 性能指標檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectPerformanceMetrics();
    
    /** 資源完整性檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectAssetIntegrity(const TArray<FString>& TargetPaths);
    
    /** 配置一致性檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectConfigurationIssues();
    
    /** 依賴關係檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectDependencyIssues();
    
    /** 安全漏洞掃描 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult PerformSecurityScan();
    
    /** 本地化完整性檢測 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult DetectLocalizationIssues();
    
    /** 記憶體使用分析 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult AnalyzeMemoryUsage();
    
    /** 網絡配置驗證 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FDetectionTaskResult ValidateNetworkConfiguration();
    
    /** 生成檢測報告 */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FString GenerateDetectionReport(const FDetectionResult& Result, bool bAsHTML);
    
    /** 導出結果到JSON */
    UFUNCTION(BlueprintCallable, Category = "Detection")
    bool ExportToJSON(const FDetectionResult& Result, const FString& FilePath);
    
    /** 獲取預設配置 */
    UFUNCTION(BlueprintPure, Category = "Detection")
    FDetectionConfig GetDefaultDetectionConfig();
    
    /** 獲取檢測統計 */
    UFUNCTION(BlueprintPure, Category = "Detection")
    TMap<EDetectionSeverity, int32> GetFindingStatistics(const FDetectionResult& Result);
    
    /** 按嚴重級別過濾發現 */
    UFUNCTION(BlueprintPure, Category = "Detection")
    TArray<FDetectionFinding> FilterFindings(const TArray<FDetectionFinding>& Findings, EDetectionSeverity MinSeverity);
    
    /** 檢查是否通過所有檢測 */
    UFUNCTION(BlueprintPure, Category = "Detection")
    bool IsAllPassed(const FDetectionResult& Result);
    
    /** 獲取健康評分 */
    UFUNCTION(BlueprintPure, Category = "Detection")
    float CalculateHealthScore(const FDetectionResult& Result);

    UPROPERTY(BlueprintAssignable, Category = "Detection")
    FOnDetectionStarted OnDetectionStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Detection")
    FOnDetectionTaskCompleted OnDetectionTaskCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Detection")
    FOnFindingDetected OnFindingDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "Detection")
    FOnDetectionCompleted OnDetectionCompleted;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectionStarted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDetectionTaskCompleted, EDetectionType, DetectionType, FDetectionTaskResult, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindingDetected, FDetectionFinding, Finding);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectionCompleted, FDetectionResult, Result);

protected:
    void ExecuteDetectionTask(EDetectionType DetectionType, const TArray<FString>& TargetPaths, FDetectionTaskResult& OutResult);
    
    TArray<FDetectionFinding> AnalyzeCodeQuality(const TArray<FString>& TargetPaths);
    TArray<FDetectionFinding> CheckAssetIntegrity(const TArray<FString>& TargetPaths);
    TArray<FDetectionFinding> ScanSecurityIssues();
    TArray<FDetectionFinding> ValidateLocalization();
    
    FDetectionMetrics CollectPerformanceMetrics();
    FDetectionMetrics CollectMemoryMetrics();
    
    void CalculateOverallScore(FDetectionResult& Result);
    FString GenerateHTMLReport(const FDetectionResult& Result);
    FString GenerateTextReport(const FDetectionResult& Result);
    
    void SendDetectionNotification(const FDetectionResult& Result);

private:
    UPROPERTY()
    bool bIsDetecting;
    
    UPROPERTY()
    FDateTime CurrentDetectionStartTime;
};
