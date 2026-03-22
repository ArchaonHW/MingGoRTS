// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 自動化批量除錯系統頭文件

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "MingRTSAutomatedBatchDebugger.generated.h"

/**
 * 除錯任務類型枚舉
 */
UENUM(BlueprintType)
enum class EDebugTaskType : uint8
{
    CompilationCheck      UMETA(DisplayName = "編譯檢查"),
    StaticAnalysis        UMETA(DisplayName = "靜態分析"),
    UnitTestExecution     UMETA(DisplayName = "單元測試"),
    IntegrationTest       UMETA(DisplayName = "集成測試"),
    MemoryLeakDetection   UMETA(DisplayName = "記憶體洩漏檢測"),
    PerformanceProfiling  UMETA(DisplayName = "性能分析"),
    CodeStyleCheck        UMETA(DisplayName = "代碼風格檢查"),
    BlueprintValidation   UMETA(DisplayName = "藍圖驗證"),
    AssetValidation       UMETA(DisplayName = "資源驗證"),
    AutoFixIssues         UMETA(DisplayName = "自動修復問題")
};

/**
 * 問題嚴重程度
 */
UENUM(BlueprintType)
enum class EIssueSeverity : uint8
{
    Critical    UMETA(DisplayName = "嚴重"),
    Error       UMETA(DisplayName = "錯誤"),
    Warning     UMETA(DisplayName = "警告"),
    Info        UMETA(DisplayName = "資訊"),
    Suggestion  UMETA(DisplayName = "建議")
};

/**
 * 除錯結果狀態
 */
UENUM(BlueprintType)
enum class EDebugResultStatus : uint8
{
    Pending     UMETA(DisplayName = "待處理"),
    Running     UMETA(DisplayName = "執行中"),
    Success     UMETA(DisplayName = "成功"),
    Failed      UMETA(DisplayName = "失敗"),
    Partial     UMETA(DisplayName = "部分成功"),
    Skipped     UMETA(DisplayName = "已跳過")
};

/**
 * 檢測到的問題結構
 */
USTRUCT(BlueprintType)
struct FDebugIssue
{
    GENERATED_BODY()
    
    /** 問題ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName IssueID;
    
    /** 問題類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDebugTaskType TaskType;
    
    /** 嚴重程度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EIssueSeverity Severity;
    
    /** 問題標題 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    /** 詳細描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    /** 文件路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FilePath;
    
    /** 行號 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LineNumber;
    
    /** 建議修復方案 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SuggestedFix;
    
    /** 是否可以自動修復 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanAutoFix;
    
    /** 修復腳本/命令 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FixCommand;
    
    /** 發現時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DetectedTime;
    
    FDebugIssue()
        : TaskType(EDebugTaskType::CompilationCheck)
        , Severity(EIssueSeverity::Info)
        , LineNumber(0)
        , bCanAutoFix(false)
    {}
};

/**
 * 批次除錯任務配置
 */
USTRUCT(BlueprintType)
struct FBatchDebugConfig
{
    GENERATED_BODY()
    
    /** 任務名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BatchName;
    
    /** 啟用的除錯任務 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EDebugTaskType> EnabledTasks;
    
    /** 目標模塊/路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TargetPaths;
    
    /** 是否啟用自動修復 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableAutoFix;
    
    /** 自動修復的最大嚴重級別 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EIssueSeverity AutoFixMaxSeverity;
    
    /** 並行執行數量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ParallelTaskCount;
    
    /** 超時時間（秒） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TaskTimeoutSeconds;
    
    /** 輸出報告路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OutputReportPath;
    
    /** 是否生成HTML報告 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGenerateHTMLReport;
    
    /** 是否發送通知 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSendNotifications;
    
    FBatchDebugConfig()
        : bEnableAutoFix(false)
        , AutoFixMaxSeverity(EIssueSeverity::Warning)
        , ParallelTaskCount(4)
        , TaskTimeoutSeconds(300.0f)
        , bGenerateHTMLReport(true)
        , bSendNotifications(true)
    {}
};

/**
 * 除錯任務結果
 */
USTRUCT(BlueprintType)
struct FDebugTaskResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDebugTaskType TaskType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDebugResultStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTimeSeconds;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDebugIssue> Issues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FilesProcessed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AutoFixedCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SummaryMessage;
    
    FDebugTaskResult()
        : TaskType(EDebugTaskType::CompilationCheck)
        , Status(EDebugResultStatus::Pending)
        , ExecutionTimeSeconds(0.0f)
        , FilesProcessed(0)
        , AutoFixedCount(0)
    {}
};

/**
 * 批次除錯完整結果
 */
USTRUCT(BlueprintType)
struct FBatchDebugResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BatchName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExecutionTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDebugTaskResult> TaskResults;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalIssues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CriticalIssues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Errors;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Warnings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AutoFixed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ReportFilePath;
    
    FBatchDebugResult()
        : TotalExecutionTime(0.0f)
        , TotalIssues(0)
        , CriticalIssues(0)
        , Errors(0)
        , Warnings(0)
        , AutoFixed(0)
    {}
};

/**
 * 自動化批次除錯系統
 * 提供完整的批次除錯、問題檢測和自動修復功能
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSAutomatedBatchDebugger : public UObject
{
    GENERATED_BODY()
    
public:
    /** 初始化除錯系統 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    void InitializeDebugger();
    
    /** 執行批次除錯 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FBatchDebugResult ExecuteBatchDebug(const FBatchDebugConfig& Config);
    
    /** 執行單個除錯任務 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult ExecuteSingleTask(EDebugTaskType TaskType, const TArray<FString>& TargetPaths);
    
    /** 批次編譯檢查 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult PerformCompilationCheck(const TArray<FString>& TargetPaths);
    
    /** 靜態代碼分析 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult PerformStaticAnalysis(const TArray<FString>& TargetPaths);
    
    /** 執行單元測試 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult ExecuteUnitTests(const TArray<FString>& TargetPaths);
    
    /** 執行集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult ExecuteIntegrationTests();
    
    /** 記憶體洩漏檢測 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult PerformMemoryLeakDetection();
    
    /** 性能分析 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult PerformPerformanceProfiling();
    
    /** 代碼風格檢查 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult PerformCodeStyleCheck(const TArray<FString>& TargetPaths);
    
    /** 藍圖驗證 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult ValidateBlueprints(const TArray<FString>& TargetPaths);
    
    /** 資源驗證 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FDebugTaskResult ValidateAssets(const TArray<FString>& TargetPaths);
    
    /** 自動修復問題 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    int32 AutoFixIssues(const TArray<FDebugIssue>& Issues, EIssueSeverity MaxSeverity);
    
    /** 修復單個問題 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    bool FixSingleIssue(const FDebugIssue& Issue);
    
    /** 生成報告 */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FString GenerateReport(const FBatchDebugResult& Result, bool bAsHTML);
    
    /** 導出結果到JSON */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    bool ExportResultsToJSON(const FBatchDebugResult& Result, const FString& FilePath);
    
    /** 導入結果從JSON */
    UFUNCTION(BlueprintCallable, Category = "Debug|Batch")
    FBatchDebugResult ImportResultsFromJSON(const FString& FilePath);
    
    /** 獲取預設配置 */
    UFUNCTION(BlueprintPure, Category = "Debug|Batch")
    FBatchDebugConfig GetDefaultConfig();
    
    /** 獲取問題統計 */
    UFUNCTION(BlueprintPure, Category = "Debug|Batch")
    TMap<EIssueSeverity, int32> GetIssueStatistics(const FBatchDebugResult& Result);
    
    /** 按嚴重程度過濾問題 */
    UFUNCTION(BlueprintPure, Category = "Debug|Batch")
    TArray<FDebugIssue> FilterIssuesBySeverity(const TArray<FDebugIssue>& Issues, EIssueSeverity MinSeverity);
    
    /** 檢查是否有嚴重問題 */
    UFUNCTION(BlueprintPure, Category = "Debug|Batch")
    bool HasCriticalIssues(const FBatchDebugResult& Result);
    
    /** 獲取所有可自動修復的問題 */
    UFUNCTION(BlueprintPure, Category = "Debug|Batch")
    TArray<FDebugIssue> GetAutoFixableIssues(const TArray<FDebugIssue>& Issues);

    UPROPERTY(BlueprintAssignable, Category = "Debug|Batch")
    FOnDebugTaskStarted OnDebugTaskStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Debug|Batch")
    FOnDebugTaskCompleted OnDebugTaskCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Debug|Batch")
    FOnIssueFound OnIssueFound;
    
    UPROPERTY(BlueprintAssignable, Category = "Debug|Batch")
    FOnBatchDebugCompleted OnBatchDebugCompleted;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugTaskStarted, EDebugTaskType, TaskType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugTaskCompleted, EDebugTaskType, TaskType, FDebugTaskResult, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIssueFound, FDebugIssue, Issue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchDebugCompleted, FBatchDebugResult, Result);

protected:
    /** 內部任務執行 */
    void ExecuteTaskInternal(EDebugTaskType TaskType, const TArray<FString>& TargetPaths, FDebugTaskResult& OutResult);
    
    /** 分析編譯日誌 */
    TArray<FDebugIssue> ParseCompilationErrors(const FString& LogOutput);
    
    /** 執行靜態分析 */
    TArray<FDebugIssue> RunStaticCodeAnalysis(const TArray<FString>& TargetPaths);
    
    /** 檢查常見問題模式 */
    TArray<FDebugIssue> DetectCommonPatterns(const FString& FilePath, const FString& FileContent);
    
    /** 執行自動修復 */
    bool ExecuteAutoFix(const FDebugIssue& Issue, FString& OutMessage);
    
    /** 生成HTML報告 */
    FString GenerateHTMLReport(const FBatchDebugResult& Result);
    
    /** 生成文本報告 */
    FString GenerateTextReport(const FBatchDebugResult& Result);
    
    /** 發送通知 */
    void SendNotification(const FBatchDebugResult& Result);

private:
    UPROPERTY()
    TArray<FString> ProcessedFiles;
    
    UPROPERTY()
    bool bIsRunning;
    
    UPROPERTY()
    FDateTime CurrentBatchStartTime;
    
    /** 修復缺失的分號 */
    bool FixMissingSemicolon(const FString& FilePath, int32 LineNumber);
    
    /** 修復縮進問題 */
    bool FixIndentation(const FString& FilePath, int32 LineNumber);
    
    /** 修復未使用的變數 */
    bool FixUnusedVariable(const FString& FilePath, int32 LineNumber, const FString& VariableName);
    
    /** 修復缺失的Include */
    bool FixMissingInclude(const FString& FilePath, const FString& MissingType);
};
