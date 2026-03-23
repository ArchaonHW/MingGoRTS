#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCompilationManager.generated.h"

// 前置聲明
UENUM(BlueprintType)
enum class ECompilationTaskType : uint8
{
    FullBuild,
    IncrementalBuild,
    CleanBuild,
    SyntaxCheck,
    ProjectHealthCheck,
    BuildTimeAnalysis,
    GenerateProjectFiles,
    VerifyCompilation,
    FixSyntaxErrors,
    OptimizeBuild
};

USTRUCT(BlueprintType)
struct FCompilationTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    ECompilationTaskType TaskType;

    UPROPERTY(BlueprintReadWrite)
    FString TaskName;

    UPROPERTY(BlueprintReadWrite)
    FString Description;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TargetFiles;

    UPROPERTY(BlueprintReadWrite)
    bool bAsync = false;

    UPROPERTY(BlueprintReadWrite)
    int32 Priority = 5;
};

USTRUCT(BlueprintType)
struct FSyntaxIssue
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber = 0;

    UPROPERTY(BlueprintReadOnly)
    FString IssueType; // MissingGENERATED_BODY, BraceMismatch, MissingPragmaOnce, MissingSemicolon

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoFixable = false;
};

USTRUCT(BlueprintType)
struct FProjectHealthReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalFiles = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 HeaderFiles = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 SourceFiles = 0;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSyntaxIssue> Issues;

    UPROPERTY(BlueprintReadOnly)
    int32 ErrorCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 WarningCount = 0;

    UPROPERTY(BlueprintReadOnly)
    FString ReportTimestamp;
};

USTRUCT(BlueprintType)
struct FBuildTimeStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float AverageBuildTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float LastBuildTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalBuilds = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulBuilds = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedBuilds = 0;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ModuleBuildTimes;
};

/**
 * 統一編譯管理器 - 整合所有編譯相關功能
 * 整合來源:
 * - QuickCompile.cmd (快速編譯)
 * - BuildTimeAnalyzer.cmd (編譯時間分析)
 * - VerifyCompilation.cmd (編譯驗證)
 * - ProjectHealthCheck.py (項目健康檢查)
 * - comprehensive_syntax_fix.py (語法修復)
 * - CheckNamingConventions.py (命名規範檢查)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCompilationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCompilationManager();

    // ========== 核心編譯功能 (來自 QuickCompile.cmd) ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool BuildDevelopment();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool BuildDebug();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool BuildShipping();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool BuildEditor();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool FastIncrementalBuild();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Build")
    bool CleanAndRebuild();

    // ========== 項目健康檢查 (來自 ProjectHealthCheck.py) ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Health Check")
    FProjectHealthReport PerformHealthCheck();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Health Check")
    TArray<FSyntaxIssue> ScanForSyntaxIssues();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Health Check")
    bool FixSyntaxIssues(const TArray<FSyntaxIssue>& Issues);

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Health Check")
    bool AutoFixAllIssues();

    // ========== 編譯時間分析 (來自 BuildTimeAnalyzer.cmd) ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Analysis")
    FBuildTimeStats AnalyzeBuildTime();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Analysis")
    bool GenerateBuildReport();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Analysis")
    TArray<FString> GetBuildOptimizationSuggestions();

    // ========== 編譯驗證 (來自 VerifyCompilation.cmd) ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Verification")
    bool VerifyProjectStructure();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Verification")
    bool VerifyCompilationEnvironment();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Verification")
    bool VerifyFixedFiles();

    // ========== 項目文件生成 ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Project Files")
    bool GenerateProjectFiles();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Project Files")
    bool RefreshProjectFiles();

    // ========== 命名規範檢查 (來自 CheckNamingConventions.py) ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Naming")
    bool CheckNamingConventions();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Naming")
    TArray<FString> GetNamingViolations();

    // ========== 批量任務管理 ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Tasks")
    bool QueueTask(const FCompilationTask& Task);

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Tasks")
    bool ExecuteTaskQueue();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Tasks")
    bool ClearTaskQueue();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Tasks")
    bool IsProcessingTasks() const { return bIsProcessing; }

    // ========== 統計與報告 ==========
    
    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Stats")
    FBuildTimeStats GetCompilationStats() const { return CompilationStats; }

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Stats")
    FString ExportReportToHTML();

    UFUNCTION(BlueprintCallable, Category = "Compilation Manager|Stats")
    FString ExportReportToJSON();

    // ========== 事件委託 ==========
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskStarted, ECompilationTaskType, TaskType, FString, TaskName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTaskProgress, ECompilationTaskType, TaskType, float, Progress, FString, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskCompleted, ECompilationTaskType, TaskType, bool, bSuccess);

    UPROPERTY(BlueprintAssignable)
    FOnTaskStarted OnTaskStarted;

    UPROPERTY(BlueprintAssignable)
    FOnTaskProgress OnTaskProgress;

    UPROPERTY(BlueprintAssignable)
    FOnTaskCompleted OnTaskCompleted;

private:
    // 內部狀態
    UPROPERTY()
    bool bIsProcessing = false;

    UPROPERTY()
    TArray<FCompilationTask> TaskQueue;

    UPROPERTY()
    FBuildTimeStats CompilationStats;

    UPROPERTY()
    FString LastReportPath;

    // 配置路徑
    FString GetEnginePath() const;
    FString GetProjectPath() const;
    FString GetUBTPath() const;
    FString GetBuildBatPath() const;
    FString GetGenerateProjectBatPath() const;

    // 內部實現方法
    bool ExecuteBuildCommand(const FString& Configuration, const FString& AdditionalFlags);
    TArray<FSyntaxIssue> CheckFileForIssues(const FString& FilePath);
    bool FixIssueInFile(const FSyntaxIssue& Issue);
    bool DeleteDirectoryRecursive(const FString& DirPath);
    int32 CountFilesInDirectory(const FString& DirPath, const FString& Extension);
    FString ExecuteCommandAndGetOutput(const FString& Command, int32& OutExitCode);
    void UpdateBuildStats(bool bSuccess, float BuildTime);
    void LogAction(const FString& Action, const FString& Message);
    
    // 任務處理
    bool ProcessSingleTask(const FCompilationTask& Task);
    void ProcessTaskQueueAsync();
};
