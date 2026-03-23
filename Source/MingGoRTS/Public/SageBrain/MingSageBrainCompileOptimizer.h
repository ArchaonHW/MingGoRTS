// SageBrain 編譯優化與任務管理整合系統
// 啟用聖者大腦並制定最佳編譯策略

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingSageBrainIntegratedAPI.h"
#include "MingSageBrainCommandAPI.h"
#include "MingTaskAnalyzer.h"
#include "MingSageBrainTaskSystem.h"
#include "MingSageBrainCompileOptimizer.generated.h"

// 編譯策略類型
UENUM(BlueprintType)
enum class ECompileStrategy : uint8
{
    Fastest,        // 最快速 - 增量編譯，最小清理
    Balanced,       // 平衡 - 部分清理，智能緩存
    Reliable,       // 最可靠 - 完整清理，完整驗證
    Parallel,       // 並行 - 多線程編譯
    Distributed     // 分布式 - 多機編譯
};

// 編譯階段
UENUM(BlueprintType)
enum class ECompilePhase : uint8
{
    Preparation,    // 準備階段
    Validation,     // 驗證階段
    CleanUp,        // 清理階段
    Generation,     // 生成專案檔案
    Compilation,    // 編譯階段
    Linking,        // 連結階段
    Verification,   // 驗證階段
    Optimization    // 優化階段
};

// 編譯優化設定
USTRUCT(BlueprintType)
struct MINGGORTS_API FCompileOptimizationConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    ECompileStrategy Strategy;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    bool bEnableIncrementalBuild;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    bool bEnableParallelCompilation;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    int32 MaxParallelJobs;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    bool bEnableCache;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    bool bSkipValidation;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    bool bAutoFixErrors;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    float TimeoutMinutes;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    FString TargetPlatform;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Optimization")
    FString BuildConfiguration;

    FCompileOptimizationConfig()
    {
        Strategy = ECompileStrategy::Balanced;
        bEnableIncrementalBuild = true;
        bEnableParallelCompilation = true;
        MaxParallelJobs = 8;
        bEnableCache = true;
        bSkipValidation = false;
        bAutoFixErrors = true;
        TimeoutMinutes = 30.0f;
        TargetPlatform = TEXT("Win64");
        BuildConfiguration = TEXT("Development");
    }
};

// 編譯階段結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FCompilePhaseResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    ECompilePhase Phase;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    float DurationSeconds;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    FString LogOutput;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    TArray<FString> Errors;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Phase")
    TArray<FString> Warnings;

    FCompilePhaseResult()
    {
        Phase = ECompilePhase::Preparation;
        bSuccess = false;
        DurationSeconds = 0.0f;
        LogOutput = TEXT("");
    }
};

// 編譯優化報告
USTRUCT(BlueprintType)
struct MINGGORTS_API FCompileOptimizationReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    FString StrategyName;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    float TotalDuration;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    TArray<FCompilePhaseResult> PhaseResults;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    int32 TotalErrors;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    int32 TotalWarnings;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    bool bOverallSuccess;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    FString RecommendedNextSteps;

    UPROPERTY(BlueprintReadOnly, Category = "Compile Report")
    TArray<FMingTaskRequirement> GeneratedTasks;

    FCompileOptimizationReport()
    {
        StrategyName = TEXT("");
        TotalDuration = 0.0f;
        TotalErrors = 0;
        TotalWarnings = 0;
        bOverallSuccess = false;
        RecommendedNextSteps = TEXT("");
    }
};

// 委派定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSageBrainEnabled, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompileStrategySelected, ECompileStrategy, Strategy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompilePhaseStarted, ECompilePhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompilePhaseCompleted, const FCompilePhaseResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompileOptimizationCompleted, const FCompileOptimizationReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTasksCreatedFromLogs, int32, TaskCount);

/**
 * 聖者大腦編譯優化器
 * 
 * 整合聖者大腦智慧與編譯系統，提供最佳編譯策略
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Brain|Compile Optimizer")
class MINGGORTS_API UMingSageBrainCompileOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainCompileOptimizer();

    // ========================================
    // 1. 啟用聖者大腦
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool EnableSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool IsSageBrainEnabled() const { return bSageBrainEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FString GetSageBrainStatus() const;

    // ========================================
    // 2. 制定最佳編譯策略
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    ECompileStrategy DetermineOptimalStrategy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FCompileOptimizationConfig GenerateOptimizationConfig(ECompileStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FString GetStrategyDescription(ECompileStrategy Strategy);

    // ========================================
    // 3. 執行優化編譯
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FCompileOptimizationReport ExecuteOptimizedCompile(const FCompileOptimizationConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool ExecuteFastCompile();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool ExecuteReliableCompile();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool ExecuteParallelCompile(int32 JobCount);

    // ========================================
    // 4. 蒐集與分析 Log
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FString CollectCompileLogs();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    FMingLogAnalysisResult AnalyzeCompileLogs(const FString& LogContent);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    TArray<FMingTaskRequirement> ExtractTasksFromLogs(const FMingLogAnalysisResult& AnalysisResult);

    // ========================================
    // 5. 建立任務至需求單系統
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool CreateTasksInTicketSystem(const TArray<FMingTaskRequirement>& Tasks);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool CreateSageBrainTasksFromCompileReport(const FCompileOptimizationReport& Report);

    // ========================================
    // 6. 優先層級排序
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    TArray<FMingTaskRequirement> SortTasksByPriorityAndImpact(const TArray<FMingTaskRequirement>& Tasks);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    TArray<FSageBrainTask> GetPrioritizedSageBrainTasks();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    void AutoPrioritizeAllTasks();

    // ========================================
    // 7. 完整工作流程
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool RunCompleteCompileOptimizationWorkflow();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Compile Optimizer")
    bool RunSmartCompileAndTaskGeneration();

    // 事件委派
    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnSageBrainEnabled OnSageBrainEnabled;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnCompileStrategySelected OnCompileStrategySelected;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnCompilePhaseStarted OnCompilePhaseStarted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnCompilePhaseCompleted OnCompilePhaseCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnCompileOptimizationCompleted OnCompileOptimizationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Compile Optimizer|Events")
    FOnTasksCreatedFromLogs OnTasksCreatedFromLogs;

protected:
    // 編譯階段執行
    FCompilePhaseResult ExecutePreparationPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteValidationPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteCleanUpPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteGenerationPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteCompilationPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteLinkingPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteVerificationPhase(const FCompileOptimizationConfig& Config);
    FCompilePhaseResult ExecuteOptimizationPhase(const FCompileOptimizationConfig& Config);

    // 智慧決策
    bool ShouldUseIncrementalBuild();
    bool ShouldUseParallelCompilation();
    int32 CalculateOptimalJobCount();
    bool ShouldAutoFix();

    // 任務創建輔助
    FMingTaskRequirement CreateTaskFromError(const FString& Error, EMingTaskPriority Priority);
    FMingTaskRequirement CreateTaskFromWarning(const FString& Warning, EMingTaskPriority Priority);
    FSageBrainTask CreateSageBrainTaskFromRequirement(const FMingTaskRequirement& Requirement);

    // 日誌分析
    TArray<FString> ParseErrorsFromLog(const FString& LogContent);
    TArray<FString> ParseWarningsFromLog(const FString& LogContent);
    EMingTaskPriority DeterminePriorityFromError(const FString& Error);

    // 內部輔助
    void LogOptimizerActivity(const FString& Activity);
    float CalculatePhaseEfficiency(ECompilePhase Phase, float Duration);
    void UpdateSageBrainWithCompileResults(const FCompileOptimizationReport& Report);

private:
    // 核心系統組件
    UPROPERTY()
    TObjectPtr<UMingSageBrainIntegratedAPI> SageBrainAPI;

    UPROPERTY()
    TObjectPtr<UMingSageBrainCommandAPI> CommandAPI;

    UPROPERTY()
    TObjectPtr<UMingTaskAnalyzer> TaskAnalyzer;

    UPROPERTY()
    TObjectPtr<UMingSageBrainTaskSystem> TaskSystem;

    // 系統狀態
    UPROPERTY()
    bool bSageBrainEnabled;

    UPROPERTY()
    bool bIsCompiling;

    UPROPERTY()
    ECompileStrategy CurrentStrategy;

    UPROPERTY()
    FCompileOptimizationReport LastReport;

    UPROPERTY()
    TArray<FString> CompileLogBuffer;

    UPROPERTY()
    float CompileStartTime;
};
