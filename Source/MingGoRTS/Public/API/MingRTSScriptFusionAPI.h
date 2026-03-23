#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSIntelligentScriptIntegration.h"
#include "MingRTSScriptFusionAPI.generated.h"

// ========== 腳本融合枚舉 ==========

UENUM(BlueprintType)
enum class EScriptFusionType : uint8
{
    BuildScripts UMETA(DisplayName = "構建腳本融合"),
    AutomationScripts UMETA(DisplayName = "自動化腳本融合"),
    AIScripts UMETA(DisplayName = "AI腳本融合"),
    OptimizationScripts UMETA(DisplayName = "優化腳本融合"),
    DebugScripts UMETA(DisplayName = "除錯腳本融合"),
    DeploymentScripts UMETA(DisplayName = "部署腳本融合"),
    TestingScripts UMETA(DisplayName = "測試腳本融合"),
    MaintenanceScripts UMETA(DisplayName = "維護腳本融合"),
    SecurityScripts UMETA(DisplayName = "安全腳本融合"),
    AssetScripts UMETA(DisplayName = "資產腳本融合"),
    AllScripts UMETA(DisplayName = "全部腳本融合")
};

UENUM(BlueprintType)
enum class EFusionMode : uint8
{
    Sequential UMETA(DisplayName = "順序執行"),
    Parallel UMETA(DisplayName = "並行執行"),
    Pipeline UMETA(DisplayName = "管道執行"),
    Conditional UMETA(DisplayName = "條件執行"),
    EventDriven UMETA(DisplayName = "事件驅動"),
    Intelligent UMETA(DisplayName = "智慧執行")
};

UENUM(BlueprintType)
enum class EScriptPriority : uint8
{
    Critical UMETA(DisplayName = "關鍵"),
    High UMETA(DisplayName = "高"),
    Medium UMETA(DisplayName = "中"),
    Low UMETA(DisplayName = "低"),
    Background UMETA(DisplayName = "背景")
};

// ========== 腳本融合數據結構 ==========

USTRUCT(BlueprintType)
struct FScriptFusionTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TaskID;

    UPROPERTY(BlueprintReadOnly)
    FString TaskName;

    UPROPERTY(BlueprintReadOnly)
    EScriptFusionType FusionType;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ScriptPaths;

    UPROPERTY(BlueprintReadOnly)
    EFusionMode ExecutionMode;

    UPROPERTY(BlueprintReadOnly)
    EScriptPriority Priority;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadOnly)
    FString Schedule;

    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;

    UPROPERTY(BlueprintReadOnly)
    float EstimatedDuration;

    FScriptFusionTask()
    {
        FusionType = EScriptFusionType::AllScripts;
        ExecutionMode = EFusionMode::Sequential;
        Priority = EScriptPriority::Medium;
        bEnabled = true;
        EstimatedDuration = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FScriptFusionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TaskID;

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString StatusMessage;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ExecutedScripts;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> FailedScripts;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> OutputData;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Warnings;

    FScriptFusionResult()
    {
        bSuccess = false;
        ExecutionTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FScriptFusionProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProfileID;

    UPROPERTY(BlueprintReadOnly)
    FString ProfileName;

    UPROPERTY(BlueprintReadOnly)
    EScriptFusionType FusionType;

    UPROPERTY(BlueprintReadOnly)
    TArray<FScriptFusionTask> Tasks;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> GlobalParameters;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoExecute;

    UPROPERTY(BlueprintReadOnly)
    FString TriggerCondition;

    FScriptFusionProfile()
    {
        FusionType = EScriptFusionType::AllScripts;
        bAutoExecute = false;
    }
};

/**
 * 腳本融合API - 統一整合所有現有腳本到API功能中
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSScriptFusionAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSScriptFusionAPI();

    // ========== 腳本發現和註冊 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Discovery")
    void DiscoverAllScripts();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Discovery")
    void RegisterScript(const FString& ScriptPath, EScriptCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Discovery")
    TArray<FString> GetScriptsByCategory(EScriptCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Discovery")
    void RefreshScriptRegistry();

    // ========== 腳本融合任務管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Tasks")
    FString CreateFusionTask(const FScriptFusionTask& Task);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Tasks")
    bool ExecuteFusionTask(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Tasks")
    FScriptFusionResult GetTaskResult(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Tasks")
    TArray<FScriptFusionTask> GetAllTasks();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Tasks")
    void DeleteTask(const FString& TaskID);

    // ========== 預設融合配置 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    FScriptFusionProfile CreateBuildProfile();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    FScriptFusionProfile CreateAutomationProfile();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    FScriptFusionProfile CreateOptimizationProfile();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    FScriptFusionProfile CreateTestingProfile();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    FScriptFusionProfile CreateDeploymentProfile();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Profiles")
    void ExecuteProfile(const FString& ProfileID);

    // ========== 智慧腳本融合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Intelligence")
    FScriptFusionTask SuggestOptimalFusion(const FString& Goal);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Intelligence")
    void EnableIntelligentFusion(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Intelligence")
    void OptimizeTaskExecution(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Intelligence")
    TArray<FString> PredictRequiredScripts(const FString& Context);

    // ========== 並行執行管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Parallel")
    void ExecuteParallelTasks(const TArray<FString>& TaskIDs);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Parallel")
    void SetMaxParallelTasks(int32 MaxTasks);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Parallel")
    TArray<FString> GetRunningTasks();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Parallel")
    void StopTask(const FString& TaskID);

    // ========== 管道執行系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Pipeline")
    FString CreatePipeline(const TArray<FString>& TaskIDs);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Pipeline")
    bool ExecutePipeline(const FString& PipelineID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Pipeline")
    void AddPipelineStage(const FString& PipelineID, const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Pipeline")
    TArray<FString> GetPipelineStages(const FString& PipelineID);

    // ========== 事件驅動執行 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Events")
    void RegisterEventTrigger(const FString& EventName, const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Events")
    void TriggerEvent(const FString& EventName, const TMap<FString, FString>& EventData);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Events")
    TArray<FString> GetEventTriggers(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Events")
    void UnregisterEventTrigger(const FString& EventName, const FString& TaskID);

    // ========== 調度和定時執行 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Scheduling")
    void ScheduleTask(const FString& TaskID, const FDateTime& ExecuteTime);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Scheduling")
    void ScheduleRecurringTask(const FString& TaskID, const FString& CronExpression);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Scheduling")
    TArray<FString> GetScheduledTasks();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Scheduling")
    void CancelScheduledTask(const FString& TaskID);

    // ========== 監控和日誌 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Monitoring")
    TArray<FScriptFusionResult> GetExecutionHistory(const FDateTime& StartTime, const FDateTime& EndTime);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Monitoring")
    FString GetTaskLog(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Monitoring")
    void EnableDetailedLogging(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Monitoring")
    void GenerateExecutionReport(const FString& ReportPath);

    // ========== 錯誤處理和恢復 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Error Handling")
    void EnableAutoRetry(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Error Handling")
    void SetMaxRetryAttempts(int32 MaxAttempts);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Error Handling")
    void RetryFailedTask(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Error Handling")
    TArray<FString> GetFailedTasks();

    // ========== 腳本優化 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Optimization")
    void OptimizeScript(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Optimization")
    void AnalyzeScriptPerformance(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Optimization")
    TArray<FString> GetOptimizationSuggestions();

    UFUNCTION(BlueprintCallable, Category = "Script Fusion|Optimization")
    void EnableAutoOptimization(bool bEnabled);

    // ========== 事件委託 ==========
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskStarted, const FString&, TaskID, const FScriptFusionTask&, Task);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskCompleted, const FString&, TaskID, const FScriptFusionResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskFailed, const FString&, TaskID, const FString&, ErrorMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPipelineCompleted, const FString&, PipelineID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventTriggered, const FString&, EventName, const TMap<FString, FString>&, EventData);

    UPROPERTY(BlueprintAssignable)
    FOnTaskStarted OnTaskStarted;

    UPROPERTY(BlueprintAssignable)
    FOnTaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTaskFailed OnTaskFailed;

    UPROPERTY(BlueprintAssignable)
    FOnPipelineCompleted OnPipelineCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;

private:
    // ========== 腳本註冊表 ==========
    UPROPERTY()
    TMap<EScriptCategory, TArray<FString>> ScriptRegistry;

    UPROPERTY()
    TMap<FString, FScriptFunctionality> ScriptFunctionalities;

    // ========== 任務管理 ==========
    UPROPERTY()
    TMap<FString, FScriptFusionTask> FusionTasks;

    UPROPERTY()
    TMap<FString, FScriptFusionResult> TaskResults;

    UPROPERTY()
    TArray<FString> ActiveTasks;

    // ========== 配置文件 ==========
    UPROPERTY()
    TMap<FString, FScriptFusionProfile> FusionProfiles;

    // ========== 執行狀態 ==========
    UPROPERTY()
    bool bIntelligentFusionEnabled;

    UPROPERTY()
    int32 MaxParallelTasks;

    UPROPERTY()
    bool bDetailedLoggingEnabled;

    UPROPERTY()
    bool bAutoRetryEnabled;

    UPROPERTY()
    int32 MaxRetryAttempts;

    // ========== 事件和調度 ==========
    UPROPERTY()
    TMap<FString, TArray<FString>> EventTriggers;

    UPROPERTY()
    TMap<FString, FDateTime> ScheduledTasks;

    // ========== 管道系統 ==========
    UPROPERTY()
    TMap<FString, TArray<FString>> PipelineStages;

    // ========== 內部方法 ==========
    void InitializeScriptFusionSystem();
    void ScanScriptsDirectory(const FString& DirectoryPath);
    void CategorizeScript(const FString& ScriptPath);
    void LoadScriptMetadata(const FString& ScriptPath);
    
    bool ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters, FString& Output);
    void ExecuteTaskSequential(const FScriptFusionTask& Task);
    void ExecuteTaskParallel(const FScriptFusionTask& Task);
    void ExecuteTaskPipeline(const FScriptFusionTask& Task);
    void ExecuteTaskIntelligent(const FScriptFusionTask& Task);
    
    void HandleTaskCompletion(const FString& TaskID, const FScriptFusionResult& Result);
    void HandleTaskFailure(const FString& TaskID, const FString& ErrorMessage);
    void ProcessScheduledTasks();
    void ProcessEventTriggers();
    
    FString GenerateTaskID();
    FString GeneratePipelineID();
    void SaveFusionData();
    void LoadFusionData();
    
    // ========== 腳本分析 ==========
    void AnalyzeScriptDependencies(const FString& ScriptPath);
    float EstimateScriptExecutionTime(const FString& ScriptPath);
    TArray<FString> GetScriptDependencies(const FString& ScriptPath);
    bool ValidateScriptParameters(const FString& ScriptPath, const TMap<FString, FString>& Parameters);
    
    // ========== 優化算法 ==========
    void OptimizeTaskOrder(TArray<FString>& TaskIDs);
    void OptimizeParallelExecution(TArray<FString>& TaskIDs);
    void OptimizeResourceUsage();
    void CacheFrequentTasks();
};
