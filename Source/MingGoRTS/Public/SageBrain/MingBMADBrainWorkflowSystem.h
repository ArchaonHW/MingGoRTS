#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingSageBrainConsoleAutomation.h"
#include "MingSageBrainBridge.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingRTSScriptFusionAPI.h"
#include "MingRTSSageGuidedBugFixAPI.h"
#include "MingBMADBrainWorkflowSystem.generated.h"

/**
 * BMAD大腦工作流系統
 * 根據文檔分配任務至需求單系統並按優先層級執行
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingBMADBrainWorkflowSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingBMADBrainWorkflowSystem();

    // BMAD模式啟動
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Startup", meta = (DisplayName = "啟動BMAD模式"))
    static bool StartBMADMode();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Startup", meta = (DisplayName = "初始化BMAD工作流"))
    static bool InitializeBMADWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Startup", meta = (DisplayName = "啟動自動化操作"))
    static bool EnableAutomationOperations();

    // 需求單系統
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Requirements", meta = (DisplayName = "創建需求單"))
    static bool CreateRequirement(const FString& RequirementID, const FString& Title, const FString& Description, int32 Priority);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Requirements", meta = (DisplayName = "分配任務至需求單"))
    static bool AssignTaskToRequirement(const FString& TaskID, const FString& RequirementID);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Requirements", meta = (DisplayName = "獲取需求單列表"))
    static TArray<FString> GetRequirementList();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Requirements", meta = (DisplayName = "獲取需求單詳情"))
    static FString GetRequirementDetails(const FString& RequirementID);

    // 優先層級管理
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Priority", meta = (DisplayName = "設置任務優先級"))
    static bool SetTaskPriority(const FString& TaskID, int32 Priority);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Priority", meta = (DisplayName = "獲取優先級任務"))
    static TArray<FString> GetPriorityTasks(int32 PriorityLevel);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Priority", meta = (DisplayName = "執行優先級任務"))
    static bool ExecutePriorityTasks(int32 PriorityLevel);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Priority", meta = (DisplayName = "重新排序優先級"))
    static bool ReprioritizeTasks();

    // BMAD大腦規劃
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Planning", meta = (DisplayName = "根據需求規劃BMAD"))
    static bool PlanBMADFromRequirements();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Planning", meta = (DisplayName = "分析需求文檔"))
    static bool AnalyzeRequirementDocument(const FString& DocumentPath);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Planning", meta = (DisplayName = "生成實作計劃"))
    static bool GenerateImplementationPlan();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Planning", meta = (DisplayName = "執行實作計劃"))
    static bool ExecuteImplementationPlan();

    // 自動化操作
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Automation", meta = (DisplayName = "啟動聖者大腦控制台"))
    static bool LaunchSageBrainConsole();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Automation", meta = (DisplayName = "執行BMAD工作流"))
    static bool ExecuteBMADWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Automation", meta = (DisplayName = "監控工作流進度"))
    static bool MonitorWorkflowProgress();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Automation", meta = (DisplayName = "自動修復問題"))
    static bool AutoFixIssues();

    // 智能決策
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Intelligence", meta = (DisplayName = "智能任務分配"))
    static bool IntelligentTaskAssignment();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Intelligence", meta = (DisplayName = "預測資源需求"))
    static bool PredictResourceRequirements();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Intelligence", meta = (DisplayName = "優化工作流程"))
    static bool OptimizeWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Intelligence", meta = (DisplayName = "生成BMAD建議"))
    static FString GenerateBMADRecommendations();

    // 狀態監控
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Monitoring", meta = (DisplayName = "獲取BMAD狀態"))
    static FString GetBMADStatus();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Monitoring", meta = (DisplayName = "獲取工作流狀態"))
    static TArray<FString> GetWorkflowStatus();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Monitoring", meta = (DisplayName = "導出執行日誌"))
    static bool ExportExecutionLog(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Monitoring", meta = (DisplayName = "獲取性能指標"))
    static FString GetPerformanceMetrics();

    // BMAD任務項目結構
    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Config", meta = (DisplayName = "加載BMAD配置"))
    static bool LoadBMADConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Config", meta = (DisplayName = "保存BMAD配置"))
    static bool SaveBMADConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Config", meta = (DisplayName = "重置BMAD配置"))
    static bool ResetBMADConfiguration();

    // 預設工作流
    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Presets", meta = (DisplayName = "執行開發工作流"))
    static bool ExecuteDevelopmentWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Presets", meta = (DisplayName = "執行測試工作流"))
    static bool ExecuteTestingWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Presets", meta = (DisplayName = "執行部署工作流"))
    static bool ExecuteDeploymentWorkflow();

    UFUNCTION(BlueprintCallable, Category = "BMAD Brain|Presets", meta = (DisplayName = "執行維護工作流"))
    static bool ExecuteMaintenanceWorkflow();

private:
    // 需求單結構
    USTRUCT(BlueprintType)
    struct FRequirementItem
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FString RequirementID;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FString Title;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FString Description;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        int32 Priority;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        TArray<FString> AssignedTasks;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FString Status;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FDateTime CreatedTime;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
        FDateTime LastUpdated;
    };

    // 任務項目結構
    USTRUCT(BlueprintType)
    struct FTaskItem
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString TaskID;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString TaskName;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString Description;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        int32 Priority;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString AssignedRequirementID;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString Status;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        FString Category;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        float EstimatedDuration;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
        TArray<FString> Dependencies;
    };

    // BMAD配置結構
    USTRUCT(BlueprintType)
    struct FBMADConfiguration
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        bool bEnableAutomation = true;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        bool bEnableIntelligentAssignment = true;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        bool bEnableAutoFix = true;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        bool bEnableMonitoring = true;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        int32 MaxConcurrentTasks = 4;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        float TaskTimeout = 300.0f;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        FString DefaultWorkflowPath;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Config")
        FString RequirementDocumentPath;
    };

    // 工作流狀態結構
    USTRUCT(BlueprintType)
    struct FWorkflowState
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        FString WorkflowID;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        FString CurrentPhase;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        int32 CurrentStep;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        int32 TotalSteps;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        float Progress;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        FString Status;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        FDateTime StartTime;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        TArray<FString> CompletedTasks;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workflow State")
        TArray<FString> FailedTasks;
    };

    // 靜態實例管理
    static TObjectPtr<UMingBMADBrainWorkflowSystem> BMADInstance;
    static bool bIsInitialized;

    // 數據存儲
    static TMap<FString, FRequirementItem> Requirements;
    static TMap<FString, FTaskItem> Tasks;
    static FBMADConfiguration BMADConfig;
    static FWorkflowState CurrentWorkflow;

    // 初始化函數
    static bool InitializeBMADSystem();
    static bool EnsureBMADExists();
    static void CleanupBMADSystem();

    // 需求分析
    static bool ParseRequirementDocument(const FString& DocumentContent);
    static TArray<FString> ExtractRequirementsFromText(const FString& Text);
    static int32 CalculateRequirementPriority(const FString& RequirementText);

    // 任務管理
    static bool CreateTaskFromRequirement(const FRequirementItem& Requirement);
    static TArray<FString> GenerateTasksForRequirement(const FString& RequirementID);
    static bool ValidateTaskDependencies(const FTaskItem& Task);

    // 工作流執行
    static bool ExecuteWorkflowStep(const FString& StepID);
    static bool ValidateWorkflowPrerequisites();
    static void UpdateWorkflowProgress();

    // 智能功能
    static FString GenerateTaskRecommendation(const FTaskItem& Task);
    static TArray<FString> OptimizeTaskSequence(const TArray<FString>& TaskIDs);
    static bool PredictTaskComplexity(const FTaskItem& Task);

    // 自動化功能
    static bool LaunchRequiredAutomation();
    static bool ExecuteAutomatedTask(const FTaskItem& Task);
    static bool HandleTaskFailure(const FString& TaskID);

    // 監控功能
    static void LogWorkflowEvent(const FString& Event, const FString& Details);
    static void UpdatePerformanceMetrics(const FString& TaskID, float Duration);
    static FString GenerateWorkflowReport();

    // 工具函數
    static FString GenerateUniqueID(const FString& Prefix);
    static FDateTime GetCurrentTimestamp();
    static bool SaveWorkflowState();
    static bool LoadWorkflowState();
    static void LogBMADEvent(const FString& Event, const FString& Details);

    // 預設工作流
    static TArray<FString> GetDevelopmentWorkflowSteps();
    static TArray<FString> GetTestingWorkflowSteps();
    static TArray<FString> GetDeploymentWorkflowSteps();
    static TArray<FString> GetMaintenanceWorkflowSteps();
};
