#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCompileAPI.h"
#include "MingTaskAnalyzer.generated.h"

/**
 * 任務類型枚舉
 */
UENUM(BlueprintType)
enum class EMingTaskType : uint8
{
    Debug,          // 除錯任務
    Optimization,   // 優化任務
    Feature,        // 功能任務
    Security,       // 安全任務
    Performance     // 性能任務
};

/**
 * 優先級枚舉
 */
UENUM(BlueprintType)
enum class EMingTaskPriority : uint8
{
    Critical,       // 關鍵級 - 立即處理
    High,           // 高級 - 優先處理
    Medium,         // 中級 - 正常處理
    Low,            // 低級 - 後續處理
    Deferred        // 延遲級 - 可選處理
};

/**
 * 任務狀態枚舉
 */
UENUM(BlueprintType)
enum class EMingTaskStatus : uint8
{
    New,            // 新建
    InProgress,     // 進行中
    Completed,      // 已完成
    Blocked,        // 阻塞
    Deferred        // 延遲
};

/**
 * 任務需求單結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FMingTaskRequirement
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString TaskID;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString Title;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    EMingTaskType TaskType;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    EMingTaskPriority Priority;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    EMingTaskStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString SourceFile;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    int32 LineNumber;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString LogMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString SuggestedFix;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    float EstimatedHours;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FDateTime CreatedTime;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FDateTime DueTime;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadWrite, Category = "Task Requirement")
    FString AssignedTo;

    FMingTaskRequirement()
    {
        TaskID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        TaskType = EMingTaskType::Debug;
        Priority = EMingTaskPriority::Medium;
        Status = EMingTaskStatus::New;
        SourceFile = TEXT("");
        LineNumber = 0;
        LogMessage = TEXT("");
        ErrorMessage = TEXT("");
        SuggestedFix = TEXT("");
        EstimatedHours = 0.0f;
        CreatedTime = FDateTime::Now();
        DueTime = FDateTime::Now();
        Dependencies.Empty();
        AssignedTo = TEXT("");
    }
};

/**
 * 編譯日誌分析結果
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FMingLogAnalysisResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    TArray<FMingTaskRequirement> DebugTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    TArray<FMingTaskRequirement> OptimizationTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    TArray<FMingTaskRequirement> FeatureTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    TArray<FMingTaskRequirement> SecurityTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    TArray<FMingTaskRequirement> PerformanceTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    int32 TotalWarnings;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    int32 TotalErrors;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    int32 TotalMessages;

    UPROPERTY(BlueprintReadWrite, Category = "Log Analysis")
    FDateTime AnalysisTime;

    FMingLogAnalysisResult()
    {
        TotalWarnings = 0;
        TotalErrors = 0;
        TotalMessages = 0;
        AnalysisTime = FDateTime::Now();
    }
};

/**
 * MingGoRTS 任務分析器
 * 
 * 分析編譯日誌並生成任務需求單
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Task Analyzer")
class MINGGORTS_API UMingTaskAnalyzer : public UObject
{
    GENERATED_BODY()

public:
    UMingTaskAnalyzer();

    // 初始化任務分析器
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    void InitializeTaskAnalyzer();

    // 編譯專案並生成日誌
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    FMingCompileResult CompileProjectAndGenerateLog(const FMingCompileConfiguration& Config);

    // 分析編譯日誌
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    FMingLogAnalysisResult AnalyzeCompileLog(const FString& LogPath);

    // 從文本分析日誌
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    FMingLogAnalysisResult AnalyzeLogFromText(const FString& LogText);

    // 生成任務需求單報告
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    bool GenerateTaskReport(const FMingLogAnalysisResult& AnalysisResult, const FString& ReportPath);

    // 按優先級排序任務
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    TArray<FMingTaskRequirement> SortTasksByPriority(const TArray<FMingTaskRequirement>& Tasks);

    // 獲取關鍵任務
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    TArray<FMingTaskRequirement> GetCriticalTasks(const FMingLogAnalysisResult& AnalysisResult);

    // 獲取高優先級任務
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    TArray<FMingTaskRequirement> GetHighPriorityTasks(const FMingLogAnalysisResult& AnalysisResult);

    // 計算任務統計
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    TMap<EMingTaskType, int32> CalculateTaskStatistics(const FMingLogAnalysisResult& AnalysisResult);

    // 導出任務為JSON
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    FString ExportTasksToJSON(const FMingLogAnalysisResult& AnalysisResult);

    // 導出任務為CSV
    UFUNCTION(BlueprintCallable, Category = "Task Analyzer")
    FString ExportTasksToCSV(const FMingLogAnalysisResult& AnalysisResult);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogAnalysisStarted, const FString&, LogPath);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLogAnalysisProgress, float, Progress, const FString&, CurrentTask);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogAnalysisCompleted, const FMingLogAnalysisResult&, Result);

    UPROPERTY(BlueprintAssignable, Category = "Task Analyzer")
    FOnLogAnalysisStarted OnLogAnalysisStarted;

    UPROPERTY(BlueprintAssignable, Category = "Task Analyzer")
    FOnLogAnalysisProgress OnLogAnalysisProgress;

    UPROPERTY(BlueprintAssignable, Category = "Task Analyzer")
    FOnLogAnalysisCompleted OnLogAnalysisCompleted;

protected:
    // 解析編譯錯誤
    FMingTaskRequirement ParseCompileError(const FString& ErrorLine);

    // 解析編譯警告
    FMingTaskRequirement ParseCompileWarning(const FString& WarningLine);

    // 解析性能問題
    FMingTaskRequirement ParsePerformanceIssue(const FString& PerformanceLine);

    // 解析安全問題
    FMingTaskRequirement ParseSecurityIssue(const FString& SecurityLine);

    // 確定任務優先級
    EMingTaskPriority DetermineTaskPriority(const FMingTaskRequirement& Task);

    // 估算任務時間
    float EstimateTaskHours(const FMingTaskRequirement& Task);

    // 生成建議修復方案
    FString GenerateSuggestedFix(const FMingTaskRequirement& Task);

private:
    // 編譯 API 實例
    UPROPERTY()
    UMingCompileAPI* CompileAPI;

    // 任務ID計數器
    int32 TaskIDCounter;

    // 分析規則
    TArray<FString> ErrorPatterns;
    TArray<FString> WarningPatterns;
    TArray<FString> PerformancePatterns;
    TArray<FString> SecurityPatterns;
};
