#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSUtilityAPI.generated.h"

/**
 * 工具函數API系統
 * 專門負責各種工具和輔助功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSUtilityAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUtilityAPI();

    // 初始化工具系統
    UFUNCTION(BlueprintCallable, Category = "Utility API")
    bool InitializeUtilitySystem();

    // 編碼問題修復
    UFUNCTION(BlueprintCallable, Category = "Utility|Encoding")
    bool FixEncodingIssues();

    UFUNCTION(BlueprintCallable, Category = "Utility|Encoding")
    bool FixFileEncoding(const FString& FilePath, const FString& TargetEncoding = TEXT("UTF-8"));

    UFUNCTION(BlueprintCallable, Category = "Utility|Encoding")
    TArray<FString> ScanEncodingIssues(const FString& DirectoryPath);

    UFUNCTION(BlueprintCallable, Category = "Utility|Encoding")
    bool ConvertEncodingBatch(const TArray<FString>& FilePaths, const FString& TargetEncoding);

    UFUNCTION(BlueprintCallable, Category = "Utility|Encoding")
    FString DetectFileEncoding(const FString& FilePath);

    // 項目健康檢查
    UFUNCTION(BlueprintCallable, Category = "Utility|Health")
    bool CheckProjectHealth();

    UFUNCTION(BlueprintCallable, Category = "Utility|Health")
    FProjectHealthReport GenerateHealthReport();

    UFUNCTION(BlueprintCallable, Category = "Utility|Health")
    TArray<FString> GetHealthIssues();

    UFUNCTION(BlueprintCallable, Category = "Utility|Health")
    bool FixHealthIssue(const FString& IssueID);

    UFUNCTION(BlueprintCallable, Category = "Utility|Health")
    bool ScheduleHealthCheck(const FDateTime& CheckTime);

    // 性能優化
    UFUNCTION(BlueprintCallable, Category = "Utility|Performance")
    bool OptimizePerformance();

    UFUNCTION(BlueprintCallable, Category = "Utility|Performance")
    FPerformanceReport AnalyzePerformance();

    UFUNCTION(BlueprintCallable, Category = "Utility|Performance")
    TArray<FString> GetOptimizationSuggestions();

    UFUNCTION(BlueprintCallable, Category = "Utility|Performance")
    bool ApplyOptimization(const FString& OptimizationID);

    UFUNCTION(BlueprintCallable, Category = "Utility|Performance")
    bool OptimizeMemoryUsage();

    // 文件系統工具
    UFUNCTION(BlueprintCallable, Category = "Utility|File System")
    bool CleanTemporaryFiles();

    UFUNCTION(BlueprintCallable, Category = "Utility|File System")
    bool OrganizeProjectFiles();

    UFUNCTION(BlueprintCallable, Category = "Utility|File System")
    TArray<FString> FindDuplicateFiles();

    UFUNCTION(BlueprintCallable, Category = "Utility|File System")
    bool RemoveDuplicateFiles(const TArray<FString>& DuplicatePaths);

    UFUNCTION(BlueprintCallable, Category = "Utility|File System")
    bool ValidateFileStructure();

    // 代碼質量工具
    UFUNCTION(BlueprintCallable, Category = "Utility|Code Quality")
    bool AnalyzeCodeQuality();

    UFUNCTION(BlueprintCallable, Category = "Utility|Code Quality")
    FCodeQualityReport GetCodeQualityReport();

    UFUNCTION(BlueprintCallable, Category = "Utility|Code Quality")
    TArray<FString> GetCodeQualityIssues();

    UFUNCTION(BlueprintCallable, Category = "Utility|Code Quality")
    bool FixCodeQualityIssue(const FString& IssueID);

    UFUNCTION(BlueprintCallable, Category = "Utility|Code Quality")
    bool CheckNamingConventions();

    // 版本控制工具
    UFUNCTION(BlueprintCallable, Category = "Utility|Version Control")
    bool CheckVersionControlStatus();

    UFUNCTION(BlueprintCallable, Category = "Utility|Version Control")
    bool SyncVersionControl();

    UFUNCTION(BlueprintCallable, Category = "Utility|Version Control")
    TArray<FString> GetModifiedFiles();

    UFUNCTION(BlueprintCallable, Category = "Utility|Version Control")
    bool CreateVersionTag(const FString& TagName, const FString& Description);

    // 構建工具
    UFUNCTION(BlueprintCallable, Category = "Utility|Build")
    bool CleanBuildCache();

    UFUNCTION(BlueprintCallable, Category = "Utility|Build")
    bool ValidateBuildEnvironment();

    UFUNCTION(BlueprintCallable, Category = "Utility|Build")
    TArray<FString> GetBuildDependencies();

    UFUNCTION(BlueprintCallable, Category = "Utility|Build")
    bool CheckBuildRequirements();

    // 調試工具
    UFUNCTION(BlueprintCallable, Category = "Utility|Debug")
    bool GenerateDebugReport();

    UFUNCTION(BlueprintCallable, Category = "Utility|Debug")
    bool EnableDebugMode();

    UFUNCTION(BlueprintCallable, Category = "Utility|Debug")
    bool DisableDebugMode();

    UFUNCTION(BlueprintCallable, Category = "Utility|Debug")
    TArray<FString> GetDebugLogs();

    UFUNCTION(BlueprintCallable, Category = "Utility|Debug")
    bool ClearDebugLogs();

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Utility|Configuration")
    bool LoadConfiguration();

    UFUNCTION(BlueprintCallable, Category = "Utility|Configuration")
    bool SaveConfiguration();

    UFUNCTION(BlueprintCallable, Category = "Utility|Configuration")
    bool ResetConfiguration();

    UFUNCTION(BlueprintCallable, Category = "Utility|Configuration")
    bool ValidateConfiguration();

    UFUNCTION(BlueprintCallable, Category = "Utility|Configuration")
    TMap<FString, FString> GetConfigurationSettings();

    // 備份和恢復
    UFUNCTION(BlueprintCallable, Category = "Utility|Backup")
    bool CreateBackup(const FString& BackupPath);

    UFUNCTION(BlueprintCallable, Category = "Utility|Backup")
    bool RestoreBackup(const FString& BackupPath);

    UFUNCTION(BlueprintCallable, Category = "Utility|Backup")
    TArray<FString> GetAvailableBackups();

    UFUNCTION(BlueprintCallable, Category = "Utility|Backup")
    bool ScheduleBackup(const FDateTime& BackupTime);

    // 日志管理
    UFUNCTION(BlueprintCallable, Category = "Utility|Logging")
    bool ConfigureLogging(const FLoggingConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Utility|Logging")
    TArray<FString> GetSystemLogs();

    UFUNCTION(BlueprintCallable, Category = "Utility|Logging")
    bool ClearSystemLogs();

    UFUNCTION(BlueprintCallable, Category = "Utility|Logging")
    bool ExportLogs(const FString& ExportPath);

    // 系統監控
    UFUNCTION(BlueprintCallable, Category = "Utility|Monitoring")
    FSystemMetrics GetSystemMetrics();

    UFUNCTION(BlueprintCallable, Category = "Utility|Monitoring")
    bool StartSystemMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Utility|Monitoring")
    bool StopSystemMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Utility|Monitoring")
    TArray<FSystemAlert> GetSystemAlerts();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUtilityOperationCompleted, const FString&, OperationName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnUtilityOperationFailed, const FString&, OperationName, const FString&, ErrorMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthIssueDetected, const FString&, IssueDescription);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceIssueDetected, const FString&, IssueDescription);

    UPROPERTY(BlueprintAssignable, Category = "Utility|Events")
    FOnUtilityOperationCompleted OnUtilityOperationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Utility|Events")
    FOnUtilityOperationFailed OnUtilityOperationFailed;

    UPROPERTY(BlueprintAssignable, Category = "Utility|Events")
    FOnHealthIssueDetected OnHealthIssueDetected;

    UPROPERTY(BlueprintAssignable, Category = "Utility|Events")
    FOnPerformanceIssueDetected OnPerformanceIssueDetected;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Utility|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Utility|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Utility|Status")
    TArray<FString> GetAvailableFunctions() const;

    UFUNCTION(BlueprintCallable, Category = "Utility|Status")
    TMap<FString, FString> GetUtilityStatistics() const;

private:
    bool bIsInitialized;
    
    // 配置數據
    TSharedPtr<FJsonObject> UtilityConfiguration;
    
    // 系統監控數據
    TArray<FSystemMetrics> MetricsHistory;
    
    // 健康檢查結果
    TArray<FProjectHealthReport> HealthReports;
    
    // 當前操作狀態
    TMap<FString, bool> OperationStatus;

    // 內部輔助函數
    bool LoadUtilityConfiguration();
    bool InitializeMonitoring();
    
    // 編碼相關
    bool DetectAndFixEncoding(const FString& FilePath);
    TArray<FString> ScanDirectoryForEncodingIssues(const FString& DirectoryPath);
    
    // 文件系統相關
    bool ValidateFilePath(const FString& FilePath) const;
    TArray<FString> FindFilesByPattern(const FString& Pattern);
    
    // 性能分析相關
    void CollectSystemMetrics();
    float CalculateCPULoad();
    float CalculateMemoryUsage();
    
    // 代碼質量相關
    bool AnalyzeFileCodeQuality(const FString& FilePath);
    TArray<FString> CheckFileNamingConvention(const FString& FilePath);
    
    // 數據持久化
    TSharedPtr<FJsonObject> SaveUtilityData() const;
    bool LoadUtilityData(TSharedPtr<FJsonObject> UtilityData);
};

// 項目健康狀態枚舉
UENUM(BlueprintType)
enum class EProjectHealthStatus : uint8
{
    Excellent   UMETA(DisplayName = "Excellent"),
    Good        UMETA(DisplayName = "Good"),
    Fair        UMETA(DisplayName = "Fair"),
    Poor        UMETA(DisplayName = "Poor"),
    Critical    UMETA(DisplayName = "Critical")
};

// 優化類型枚舉
UENUM(BlueprintType)
enum class EOptimizationType : uint8
{
    Memory      UMETA(DisplayName = "Memory"),
    CPU         UMETA(DisplayName = "CPU"),
    Disk        UMETA(DisplayName = "Disk"),
    Network     UMETA(DisplayName = "Network"),
    General     UMETA(DisplayName = "General")
};

// 代碼質量級別枚舉
UENUM(BlueprintType)
enum class ECodeQualityLevel : uint8
{
    Excellent   UMETA(DisplayName = "Excellent"),
    Good        UMETA(DisplayName = "Good"),
    Average     UMETA(DisplayName = "Average"),
    Poor        UMETA(DisplayName = "Poor"),
    Critical    UMETA(DisplayName = "Critical")
};

// 項目健康報告結構
USTRUCT(BlueprintType)
struct FPROJECT_HEALTH_REPORT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    EProjectHealthStatus OverallStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    TArray<FString> Issues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    TArray<FString> Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    TArray<FString> Recommendations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    FDateTime ReportTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Health Report")
    TMap<FString, float> HealthScores;

    FPROJECT_HEALTH_REPORT()
    {
        OverallStatus = EProjectHealthStatus::Good;
    }
};

// 性能報告結構
USTRUCT(BlueprintType)
struct FPERFORMANCE_REPORT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    float CPULoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    float DiskUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    float NetworkUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    TArray<FString> Bottlenecks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    TArray<FOptimizationSuggestion> Suggestions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Report")
    FDateTime ReportTime;

    FPERFORMANCE_REPORT()
    {
        CPULoad = 0.0f;
        MemoryUsage = 0.0f;
        DiskUsage = 0.0f;
        NetworkUsage = 0.0f;
    }
};

// 代碼質量報告結構
USTRUCT(BlueprintType)
struct FCODE_QUALITY_REPORT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Report")
    ECodeQualityLevel OverallQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Report")
    TArray<FCodeQualityIssue> Issues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Report")
    TArray<FString> Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Report")
    TMap<FString, int32> QualityMetrics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Report")
    FDateTime ReportTime;

    FCODE_QUALITY_REPORT()
    {
        OverallQuality = ECodeQualityLevel::Good;
    }
};

// 系統指標結構
USTRUCT(BlueprintType)
struct FSYSTEM_METRICS
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    float CPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    float DiskUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    float NetworkActivity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    int32 ActiveProcesses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Metrics")
    FDateTime Timestamp;

    FSYSTEM_METRICS()
    {
        CPUUsage = 0.0f;
        MemoryUsage = 0.0f;
        DiskUsage = 0.0f;
        NetworkActivity = 0.0f;
        ActiveProcesses = 0;
    }
};

// 系統警報結構
USTRUCT(BlueprintType)
struct FSYSTEM_ALERT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Alert")
    FString AlertID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Alert")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Alert")
    FString Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Alert")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Alert")
    bool bResolved;

    FSYSTEM_ALERT()
    {
        AlertID = TEXT("");
        Message = TEXT("");
        Severity = TEXT("Medium");
        bResolved = false;
    }
};

// 優化建議結構
USTRUCT(BlueprintType)
struct FOPTIMIZATION_SUGGESTION
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Suggestion")
    FString SuggestionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Suggestion")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Suggestion")
    EOptimizationType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Suggestion")
    float ExpectedImprovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Suggestion")
    FString ImplementationSteps;

    FOPTIMIZATION_SUGGESTION()
    {
        SuggestionID = TEXT("");
        Description = TEXT("");
        Type = EOptimizationType::General;
        ExpectedImprovement = 0.0f;
        ImplementationSteps = TEXT("");
    }
};

// 代碼質量問題結構
USTRUCT(BlueprintType)
struct FCODE_QUALITY_ISSUE
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    FString IssueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    int32 LineNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    FString Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality Issue")
    FString Category;

    FCODE_QUALITY_ISSUE()
    {
        IssueID = TEXT("");
        Description = TEXT("");
        FilePath = TEXT("");
        LineNumber = 0;
        Severity = TEXT("Medium");
        Category = TEXT("General");
    }
};

// 日志配置結構
USTRUCT(BlueprintType)
struct FLOGGING_CONFIG
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    bool bEnableVerboseLogging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    bool bEnableFileLogging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    bool bEnableConsoleLogging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    FString LogLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    FString LogDirectory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging Config")
    int32 MaxLogFiles;

    FLOGGING_CONFIG()
    {
        bEnableVerboseLogging = false;
        bEnableFileLogging = true;
        bEnableConsoleLogging = true;
        LogLevel = TEXT("Info");
        LogDirectory = TEXT("");
        MaxLogFiles = 10;
    }
};
