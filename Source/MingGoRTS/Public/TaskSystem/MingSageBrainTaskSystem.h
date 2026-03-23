#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBMADProjectPlanner.h"
#include "MingTaskAnalyzer.h"
#include "MingSageBrainTaskSystem.generated.h"

/**
 * 聖者大腦任務狀態枚舉
 */
UENUM(BlueprintType)
enum class ESageBrainTaskStatus : uint8
{
    Planning,           // 規劃中
    InProgress,         // 進行中
    Testing,            // 測試中
    Review,             // 審查中
    Completed,          // 已完成
    Paused,             // 暫停
    Cancelled           // 取消
};

/**
 * 聖者大腦任務優先級枚舉
 */
UENUM(BlueprintType)
enum class ESageBrainTaskPriority : uint8
{
    Critical,           // 關鍵 - 立即執行
    High,               // 高 - 優先執行
    Medium,             // 中 - 正常執行
    Low,                // 低 - 後續執行
    Research            // 研究階段
};

/**
 * 聖者大腦任務類型枚舉
 */
UENUM(BlueprintType)
enum class ESageBrainTaskType : uint8
{
    CoreIntegration,    // 核心集成
    ThinkingSystem,     // 思考系統
    LearningAlgorithm,  // 學習算法
    APICoordination,     // API協調
    SelfAwareness,      // 自我意識
    PhilosophySystem,   // 哲學系統
    ConsciousnessEvolution, // 意識進化
    TestingSystem,      // 測試系統
    PerformanceOptimization, // 性能優化
    Documentation       // 文檔編寫
};

/**
 * 聖者大腦任務結構
 */
USTRUCT(BlueprintType)
struct FSageBrainTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FString TaskID;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FString Title;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    ESageBrainTaskType TaskType;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    ESageBrainTaskPriority Priority;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    ESageBrainTaskStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> Objectives;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> KeyMilestones;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> RequiredResources;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> RiskFactors;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    float EstimatedDuration; // 週

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    float BudgetAllocation;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> AssignedTeam;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FDateTime CreatedTime;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FDateTime TargetCompletion;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    float ProgressPercentage;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> Deliverables;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> SuccessMetrics;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FString RelatedProjectID;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadWrite, Category = "Sage Brain Task")
    FString Notes;

    FSageBrainTask()
    {
        TaskID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        TaskType = ESageBrainTaskType::CoreIntegration;
        Priority = ESageBrainTaskPriority::Medium;
        Status = ESageBrainTaskStatus::Planning;
        Objectives.Empty();
        KeyMilestones.Empty();
        RequiredResources.Empty();
        RiskFactors.Empty();
        EstimatedDuration = 2.0f;
        BudgetAllocation = 0.0f;
        AssignedTeam.Empty();
        CreatedTime = FDateTime::Now();
        TargetCompletion = FDateTime::Now();
        ProgressPercentage = 0.0f;
        Deliverables.Empty();
        SuccessMetrics.Empty();
        RelatedProjectID = TEXT("");
        Dependencies.Empty();
        Notes = TEXT("");
    }
};

/**
 * 聖者大腦任務分析結果
 */
USTRUCT(BlueprintType)
struct FSageBrainTaskAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    FString TaskID;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    float FeasibilityScore;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    float RiskAssessment;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    float ResourceRequirement;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    float TechnicalComplexity;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    TArray<FString> Strengths;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    TArray<FString> Weaknesses;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    TArray<FString> Opportunities;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    TArray<FString> Threats;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly, Category = "Task Analysis")
    FDateTime AnalysisTime;

    FSageBrainTaskAnalysis()
    {
        TaskID = TEXT("");
        FeasibilityScore = 0.0f;
        RiskAssessment = 0.0f;
        ResourceRequirement = 0.0f;
        TechnicalComplexity = 0.0f;
        Strengths.Empty();
        Weaknesses.Empty();
        Opportunities.Empty();
        Threats.Empty();
        Recommendations.Empty();
        AnalysisTime = FDateTime::Now();
    }
};

/**
 * 聖者大腦任務統計
 */
USTRUCT(BlueprintType)
struct FSageBrainTaskStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    int32 TotalTasks;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    int32 CompletedTasks;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    int32 InProgressTasks;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    int32 PlanningTasks;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    float OverallProgress;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    float AverageCompletionTime;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    float TotalBudgetUsed;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    float BudgetEfficiency;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    TArray<TMap<ESageBrainTaskType, int32>> TasksByType;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    TArray<TMap<ESageBrainTaskPriority, int32>> TasksByPriority;

    UPROPERTY(BlueprintReadOnly, Category = "Task Statistics")
    TArray<TMap<ESageBrainTaskStatus, int32>> TasksByStatus;

    FSageBrainTaskStatistics()
    {
        TotalTasks = 0;
        CompletedTasks = 0;
        InProgressTasks = 0;
        PlanningTasks = 0;
        OverallProgress = 0.0f;
        AverageCompletionTime = 0.0f;
        TotalBudgetUsed = 0.0f;
        BudgetEfficiency = 0.0f;
        TasksByType.Empty();
        TasksByPriority.Empty();
        TasksByStatus.Empty();
    }
};

/**
 * 聖者大腦任務單系統
 * 
 * 基於BMAD API分析的聖者大腦AGI系統任務管理
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Brain Task System")
class MINGGORTS_API UMingSageBrainTaskSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainTaskSystem();

    // 初始化任務系統
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    void InitializeTaskSystem();

    // 從BMAD分析創建任務清單
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    bool CreateTasksFromBMADAnalysis(const FString& AnalysisReportPath);

    // 創建新任務
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    FString CreateSageBrainTask(const FString& Title, const FString& Description, ESageBrainTaskType TaskType, ESageBrainTaskPriority Priority);

    // 分析任務可行性
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    FSageBrainTaskAnalysis AnalyzeTask(const FString& TaskID);

    // 更新任務進度
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    bool UpdateTaskProgress(const FString& TaskID, float Progress);

    // 更新任務狀態
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    bool UpdateTaskStatus(const FString& TaskID, ESageBrainTaskStatus NewStatus);

    // 獲取任務列表
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    TArray<FSageBrainTask> GetAllTasks();

    // 獲取任務統計
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    FSageBrainTaskStatistics GetTaskStatistics();

    // 獲取高優先級任務
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    TArray<FSageBrainTask> GetHighPriorityTasks();

    // 獲取逾期任務
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    TArray<FSageBrainTask> GetOverdueTasks();

    // 生成任務報告
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    FString GenerateTaskReport();

    // 導出任務數據
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    FString ExportTaskData();

    // 同步任務與BMAD系統
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    void SyncWithBMADSystem();

    // 自動任務優化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Task System")
    void OptimizeTaskSchedule();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCreated, const FString&, TaskID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskUpdated, const FString&, TaskID, ESageBrainTaskStatus, NewStatus);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCompleted, const FString&, TaskID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskAnalysisCompleted, const FSageBrainTaskAnalysis&, Analysis);

    UPROPERTY(BlueprintAssignable)
    FOnTaskCreated OnTaskCreated;

    UPROPERTY(BlueprintAssignable)
    FOnTaskUpdated OnTaskUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnTaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTaskAnalysisCompleted OnTaskAnalysisCompleted;

protected:
    // 解析BMAD分析報告
    bool ParseBMADAnalysisReport(const FString& ReportContent);

    // 創建預定義任務
    void CreatePredefinedTasks();

    // 計算任務統計
    void CalculateTaskStatistics();

    // 檢查任務依賴
    bool CheckTaskDependencies(const FString& TaskID);

    // 自動分配資源
    void AutoAssignResources(FSageBrainTask& Task);

    // 評估任務風險
    float AssessTaskRisk(const FSageBrainTask& Task);

    // 優化任務順序
    TArray<FSageBrainTask> OptimizeTaskOrder(const TArray<FSageBrainTask>& Tasks);

    // 生成任務ID
    FString GenerateUniqueTaskID();

    // 保存任務數據
    void SaveTaskData();

    // 載入任務數據
    void LoadTaskData();

    // 清理過期任務
    void CleanupExpiredTasks();

private:
    // BMAD專案規劃器
    UPROPERTY()
    TObjectPtr<UMingBMADProjectPlanner> BMADPlanner;

    // 任務分析器
    UPROPERTY()
    TObjectPtr<UMingTaskAnalyzer> TaskAnalyzer;

    // 任務存儲
    UPROPERTY()
    TMap<FString, FSageBrainTask> TaskStorage;

    // 任務分析結果
    UPROPERTY()
    TMap<FString, FSageBrainTaskAnalysis> TaskAnalyses;

    // 任務統計
    UPROPERTY()
    FSageBrainTaskStatistics TaskStats;

    // 系統配置
    UPROPERTY()
    bool bAutoOptimizationEnabled;

    UPROPERTY()
    bool bBMADSyncEnabled;

    UPROPERTY()
    float MaxConcurrentTasks;

    UPROPERTY()
    float DefaultTaskDuration;

    UPROPERTY()
    FString TaskDataFilePath;

    // 內部方法
    FString TaskTypeToString(ESageBrainTaskType TaskType);
    FString TaskPriorityToString(ESageBrainTaskPriority Priority);
    FString TaskStatusToString(ESageBrainTaskStatus Status);
    ESageBrainTaskType StringToTaskType(const FString& TypeStr);
    ESageBrainTaskPriority StringToTaskPriority(const FString& PriorityStr);
    ESageBrainTaskStatus StringToTaskStatus(const FString& StatusStr);
};
