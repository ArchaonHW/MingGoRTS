#include "MingBMADBrainWorkflowSystem.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "TimerManager.h"
#include "Async/Async.h"

// 靜態成員定義
TObjectPtr<UMingBMADBrainWorkflowSystem> UMingBMADBrainWorkflowSystem::BMADInstance = nullptr;
bool UMingBMADBrainWorkflowSystem::bIsInitialized = false;
TMap<FString, UMingBMADBrainWorkflowSystem::FRequirementItem> UMingBMADBrainWorkflowSystem::Requirements;
TMap<FString, UMingBMADBrainWorkflowSystem::FTaskItem> UMingBMADBrainWorkflowSystem::Tasks;
UMingBMADBrainWorkflowSystem::FBMADConfiguration UMingBMADBrainWorkflowSystem::BMADConfig;
UMingBMADBrainWorkflowSystem::FWorkflowState UMingBMADBrainWorkflowSystem::CurrentWorkflow;

UMingBMADBrainWorkflowSystem::UMingBMADBrainWorkflowSystem()
{
}

bool UMingBMADBrainWorkflowSystem::StartBMADMode()
{
    LogBMADEvent(TEXT("啟動BMAD模式"), TEXT("開始初始化BMAD大腦工作流系統"));
    
    if (!InitializeBMADSystem())
    {
        LogBMADEvent(TEXT("BMAD模式啟動失敗"), TEXT("系統初始化失敗"));
        return false;
    }

    // 啟動聖者大腦控制台
    if (!LaunchSageBrainConsole())
    {
        LogBMADEvent(TEXT("BMAD模式啟動失敗"), TEXT("聖者大腦控制台啟動失敗"));
        return false;
    }

    // 啟動自動化操作
    if (!EnableAutomationOperations())
    {
        LogBMADEvent(TEXT("BMAD模式啟動失敗"), TEXT("自動化操作啟動失敗"));
        return false;
    }

    // 根據文檔分配任務
    if (!AnalyzeRequirementDocument(BMADConfig.RequirementDocumentPath))
    {
        LogBMADEvent(TEXT("BMAD模式啟動失敗"), TEXT("需求文檔分析失敗"));
        return false;
    }

    // 規劃BMAD大腦實作
    if (!PlanBMADFromRequirements())
    {
        LogBMADEvent(TEXT("BMAD模式啟動失敗"), TEXT("BMAD規劃失敗"));
        return false;
    }

    LogBMADEvent(TEXT("BMAD模式啟動成功"), TEXT("BMAD大腦工作流系統已完全啟動"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::InitializeBMADWorkflow()
{
    LogBMADEvent(TEXT("初始化BMAD工作流"), TEXT("開始初始化BMAD工作流程"));
    
    if (!EnsureBMADExists())
    {
        return false;
    }

    // 初始化配置
    BMADConfig = FBMADConfiguration();
    BMADConfig.bEnableAutomation = true;
    BMADConfig.bEnableIntelligentAssignment = true;
    BMADConfig.bEnableAutoFix = true;
    BMADConfig.bEnableMonitoring = true;
    BMADConfig.MaxConcurrentTasks = 4;
    BMADConfig.TaskTimeout = 300.0f;
    BMADConfig.DefaultWorkflowPath = FPaths::ProjectConfigDir() + TEXT("BMADWorkflows/");
    BMADConfig.RequirementDocumentPath = FPaths::ProjectConfigDir() + TEXT("Requirements.md");

    // 初始化工作流狀態
    CurrentWorkflow = FWorkflowState();
    CurrentWorkflow.WorkflowID = GenerateUniqueID(TEXT("Workflow"));
    CurrentWorkflow.CurrentPhase = TEXT("Initialization");
    CurrentWorkflow.CurrentStep = 0;
    CurrentWorkflow.TotalSteps = 5; // 初始化、分析、規劃、執行、監控
    CurrentWorkflow.Progress = 0.0f;
    CurrentWorkflow.Status = TEXT("Initializing");
    CurrentWorkflow.StartTime = GetCurrentTimestamp();

    LogBMADEvent(TEXT("BMAD工作流初始化完成"), TEXT("工作流狀態已初始化"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::EnableAutomationOperations()
{
    LogBMADEvent(TEXT("啟動自動化操作"), TEXT("開始啟動聖者大腦控制台自動化"));
    
    // 啟動控制台自動化系統
    if (!UMingSageBrainConsoleAutomation::StartAndInitialize())
    {
        LogBMADEvent(TEXT("自動化啟動失敗"), TEXT("控制台自動化系統啟動失敗"));
        return false;
    }

    // 設置自動化配置
    UMingSageBrainConsoleAutomation::SetErrorRetry(3, 1.0f);
    UMingSageBrainConsoleAutomation::EnableAutoRecovery();

    // 啟用智能響應
    UMingSageBrainConsoleAutomation::EnableIntelligentResponse();

    // 添加BMAD特定響應規則
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("需求"), TEXT("bridge get_requirements"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("任務"), TEXT("bridge get_tasks"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("優先級"), TEXT("bridge get_priority_tasks"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("狀態"), TEXT("bridge get_workflow_status"));

    LogBMADEvent(TEXT("自動化操作啟動成功"), TEXT("聖者大腦控制台自動化已啟動"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::CreateRequirement(const FString& RequirementID, const FString& Title, const FString& Description, int32 Priority)
{
    if (!EnsureBMADExists())
    {
        return false;
    }

    FRequirementItem NewRequirement;
    NewRequirement.RequirementID = RequirementID;
    NewRequirement.Title = Title;
    NewRequirement.Description = Description;
    NewRequirement.Priority = Priority;
    NewRequirement.Status = TEXT("Created");
    NewRequirement.CreatedTime = GetCurrentTimestamp();
    NewRequirement.LastUpdated = GetCurrentTimestamp();

    Requirements.Add(RequirementID, NewRequirement);

    LogBMADEvent(TEXT("創建需求單"), FString::Printf(TEXT("ID: %s, 標題: %s, 優先級: %d"), *RequirementID, *Title, Priority));
    return true;
}

bool UMingBMADBrainWorkflowSystem::AssignTaskToRequirement(const FString& TaskID, const FString& RequirementID)
{
    if (!Requirements.Contains(RequirementID))
    {
        LogBMADEvent(TEXT("任務分配失敗"), FString::Printf(TEXT("需求單不存在: %s"), *RequirementID));
        return false;
    }

    FRequirementItem& Requirement = Requirements[RequirementID];
    Requirement.AssignedTasks.Add(TaskID);
    Requirement.LastUpdated = GetCurrentTimestamp();

    if (Tasks.Contains(TaskID))
    {
        FTaskItem& Task = Tasks[TaskID];
        Task.AssignedRequirementID = RequirementID;
        Task.Status = TEXT("Assigned");
    }

    LogBMADEvent(TEXT("任務分配成功"), FString::Printf(TEXT("任務 %s 已分配至需求 %s"), *TaskID, *RequirementID));
    return true;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GetRequirementList()
{
    TArray<FString> RequirementList;
    for (const auto& RequirementPair : Requirements)
    {
        RequirementList.Add(RequirementPair.Key);
    }
    return RequirementList;
}

FString UMingBMADBrainWorkflowSystem::GetRequirementDetails(const FString& RequirementID)
{
    if (!Requirements.Contains(RequirementID))
    {
        return TEXT("需求單不存在");
    }

    const FRequirementItem& Requirement = Requirements[RequirementID];
    FString Details = FString::Printf(TEXT("=== 需求單詳情 ===\n"));
    Details += FString::Printf(TEXT("ID: %s\n"), *Requirement.RequirementID);
    Details += FString::Printf(TEXT("標題: %s\n"), *Requirement.Title);
    Details += FString::Printf(TEXT("描述: %s\n"), *Requirement.Description);
    Details += FString::Printf(TEXT("優先級: %d\n"), Requirement.Priority);
    Details += FString::Printf(TEXT("狀態: %s\n"), *Requirement.Status);
    Details += FString::Printf(TEXT("創建時間: %s\n"), *Requirement.CreatedTime.ToString());
    Details += FString::Printf(TEXT("最後更新: %s\n"), *Requirement.LastUpdated.ToString());
    Details += FString::Printf(TEXT("分配任務數量: %d\n"), Requirement.AssignedTasks.Num());

    return Details;
}

bool UMingBMADBrainWorkflowSystem::SetTaskPriority(const FString& TaskID, int32 Priority)
{
    if (!Tasks.Contains(TaskID))
    {
        return false;
    }

    FTaskItem& Task = Tasks[TaskID];
    Task.Priority = Priority;

    LogBMADEvent(TEXT("設置任務優先級"), FString::Printf(TEXT("任務 %s 優先級設為 %d"), *TaskID, Priority));
    return true;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GetPriorityTasks(int32 PriorityLevel)
{
    TArray<FString> PriorityTasks;
    for (const auto& TaskPair : Tasks)
    {
        if (TaskPair.Value.Priority == PriorityLevel)
        {
            PriorityTasks.Add(TaskPair.Key);
        }
    }
    return PriorityTasks;
}

bool UMingBMADBrainWorkflowSystem::ExecutePriorityTasks(int32 PriorityLevel)
{
    TArray<FString> PriorityTasks = GetPriorityTasks(PriorityLevel);
    
    if (PriorityTasks.Num() == 0)
    {
        LogBMADEvent(TEXT("執行優先級任務"), FString::Printf(TEXT("優先級 %d 沒有任務"), PriorityLevel));
        return false;
    }

    // 按優先級排序
    PriorityTasks.Sort([this](const FString& A, const FString& B) {
        if (!Tasks.Contains(A) || !Tasks.Contains(B)) return false;
        return Tasks[A].Priority < Tasks[B].Priority;
    });

    // 執行任務
    for (const FString& TaskID : PriorityTasks)
    {
        if (!ExecuteAutomatedTask(Tasks[TaskID]))
        {
            HandleTaskFailure(TaskID);
        }
        FPlatformProcess::Sleep(0.5f); // 任務間延遲
    }

    LogBMADEvent(TEXT("執行優先級任務完成"), FString::Printf(TEXT("優先級 %d 的所有任務已執行"), PriorityLevel));
    return true;
}

bool UMingBMADBrainWorkflowSystem::ReprioritizeTasks()
{
    LogBMADEvent(TEXT("重新排序優先級"), TEXT("開始智能任務優先級重排"));

    // 使用智能算法重新排序
    TArray<FString> AllTaskIDs;
    for (const auto& TaskPair : Tasks)
    {
        AllTaskIDs.Add(TaskPair.Key);
    }

    TArray<FString> OptimizedSequence = OptimizeTaskSequence(AllTaskIDs);

    // 更新任務優先級
    for (int32 i = 0; i < OptimizedSequence.Num(); i++)
    {
        if (Tasks.Contains(OptimizedSequence[i]))
        {
            FTaskItem& Task = Tasks[OptimizedSequence[i]];
            Task.Priority = i + 1; // 基於優化順序設置優先級
        }
    }

    LogBMADEvent(TEXT("優先級重排完成"), FString::Printf(TEXT("已優化 %d 個任務的優先級"), AllTaskIDs.Num()));
    return true;
}

bool UMingBMADBrainWorkflowSystem::PlanBMADFromRequirements()
{
    LogBMADEvent(TEXT("規劃BMAD實作"), TEXT("開始根據需求規劃BMAD大腦實作"));

    CurrentWorkflow.CurrentPhase = TEXT("Planning");
    CurrentWorkflow.CurrentStep = 2;
    CurrentWorkflow.Status = TEXT("Planning");

    // 為每個需求創建任務
    for (const auto& RequirementPair : Requirements)
    {
        const FRequirementItem& Requirement = RequirementPair.Value;
        
        // 根據需求生成任務
        TArray<FString> GeneratedTasks = GenerateTasksForRequirement(Requirement.RequirementID);
        
        for (const FString& TaskID : GeneratedTasks)
        {
            AssignTaskToRequirement(TaskID, Requirement.RequirementID);
        }
    }

    // 執行智能任務分配
    if (BMADConfig.bEnableIntelligentAssignment)
    {
        IntelligentTaskAssignment();
    }

    // 重新排序優先級
    ReprioritizeTasks();

    // 生成實作計劃
    if (!GenerateImplementationPlan())
    {
        LogBMADEvent(TEXT("BMAD規劃失敗"), TEXT("實作計劃生成失敗"));
        return false;
    }

    UpdateWorkflowProgress();
    LogBMADEvent(TEXT("BMAD規劃完成"), TEXT("BMAD大腦實作規劃已完成"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::AnalyzeRequirementDocument(const FString& DocumentPath)
{
    LogBMADEvent(TEXT("分析需求文檔"), FString::Printf(TEXT("文檔路徑: %s"), *DocumentPath));

    if (!FPaths::FileExists(DocumentPath))
    {
        LogBMADEvent(TEXT("文檔分析失敗"), FString::Printf(TEXT("文檔不存在: %s"), *DocumentPath));
        return false;
    }

    FString DocumentContent;
    if (!FFileHelper::LoadFileToString(DocumentContent, *DocumentPath))
    {
        LogBMADEvent(TEXT("文檔分析失敗"), FString::Printf(TEXT("無法讀取文檔: %s"), *DocumentPath));
        return false;
    }

    return ParseRequirementDocument(DocumentContent);
}

bool UMingBMADBrainWorkflowSystem::GenerateImplementationPlan()
{
    LogBMADEvent(TEXT("生成實作計劃"), TEXT("開始生成BMAD實作計劃"));

    // 創建實作計劃步驟
    TArray<FString> ImplementationSteps;
    
    // 步驟1: 驗證前置條件
    ImplementationSteps.Add(TEXT("驗證BMAD系統前置條件"));
    
    // 步驟2: 執行優先級任務
    ImplementationSteps.Add(TEXT("按優先級執行任務"));
    
    // 步驟3: 監控執行進度
    ImplementationSteps.Add(TEXT("監控任務執行進度"));
    
    // 步驟4: 處理失敗和重試
    ImplementationSteps.Add(TEXT("處理任務失敗和重試"));
    
    // 步驟5: 生成最終報告
    ImplementationSteps.Add(TEXT("生成BMAD實作報告"));

    CurrentWorkflow.CurrentPhase = TEXT("Execution");
    CurrentWorkflow.CurrentStep = 3;
    CurrentWorkflow.TotalSteps = ImplementationSteps.Num();
    CurrentWorkflow.Status = TEXT("Ready");

    LogBMADEvent(TEXT("實作計劃生成完成"), FString::Printf(TEXT("已生成 %d 個實作步驟"), ImplementationSteps.Num()));
    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteImplementationPlan()
{
    LogBMADEvent(TEXT("執行實作計劃"), TEXT("開始執行BMAD實作計劃"));

    CurrentWorkflow.Status = TEXT("Executing");
    UpdateWorkflowProgress();

    // 執行計劃步驟
    if (!ValidateWorkflowPrerequisites())
    {
        LogBMADEvent(TEXT("實作計劃執行失敗"), TEXT("前置條件驗證失敗"));
        return false;
    }

    // 執行優先級任務
    for (int32 Priority = 1; Priority <= 5; Priority++)
    {
        if (!ExecutePriorityTasks(Priority))
        {
            LogBMADEvent(TEXT("優先級任務執行失敗"), FString::Printf(TEXT("優先級 %d 任務執行失敗"), Priority));
        }
    }

    // 監控執行進度
    MonitorWorkflowProgress();

    CurrentWorkflow.Status = TEXT("Completed");
    UpdateWorkflowProgress();

    LogBMADEvent(TEXT("實作計劃執行完成"), TEXT("BMAD實作計劃已完全執行"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::LaunchSageBrainConsole()
{
    LogBMADEvent(TEXT("啟動聖者大腦控制台"), TEXT("開始啟動聖者大腦控制台系統"));
    
    return UMingSageBrainConsoleStarter::StartAndShow();
}

bool UMingBMADBrainWorkflowSystem::ExecuteBMADWorkflow()
{
    LogBMADEvent(TEXT("執行BMAD工作流"), TEXT("開始執行完整BMAD工作流"));

    // 初始化BMAD工作流
    if (!InitializeBMADWorkflow())
    {
        return false;
    }

    // 啟動自動化
    if (!EnableAutomationOperations())
    {
        return false;
    }

    // 分析需求文檔
    if (!AnalyzeRequirementDocument(BMADConfig.RequirementDocumentPath))
    {
        return false;
    }

    // 規劃BMAD實作
    if (!PlanBMADFromRequirements())
    {
        return false;
    }

    // 執行實作計劃
    if (!ExecuteImplementationPlan())
    {
        return false;
    }

    LogBMADEvent(TEXT("BMAD工作流執行完成"), TEXT("完整BMAD工作流已成功執行"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::MonitorWorkflowProgress()
{
    if (!BMADConfig.bEnableMonitoring)
    {
        return true;
    }

    LogBMADEvent(TEXT("監控工作流進度"), TEXT("開始監控BMAD工作流執行進度"));

    // 檢查任務狀態
    int32 CompletedTasks = 0;
    int32 FailedTasks = 0;
    int32 TotalTasks = Tasks.Num();

    for (const auto& TaskPair : Tasks)
    {
        const FTaskItem& Task = TaskPair.Value;
        if (Task.Status == TEXT("Completed"))
        {
            CompletedTasks++;
        }
        else if (Task.Status == TEXT("Failed"))
        {
            FailedTasks++;
        }
    }

    // 更新工作流進度
    if (TotalTasks > 0)
    {
        CurrentWorkflow.Progress = (float)CompletedTasks / TotalTasks * 100.0f;
    }

    LogBMADEvent(TEXT("工作流進度更新"), FString::Printf(TEXT("完成: %d, 失敗: %d, 總計: %d, 進度: %.1f%%"), 
        CompletedTasks, FailedTasks, TotalTasks, CurrentWorkflow.Progress));

    return true;
}

bool UMingBMADBrainWorkflowSystem::AutoFixIssues()
{
    if (!BMADConfig.bEnableAutoFix)
    {
        return true;
    }

    LogBMADEvent(TEXT("自動修復問題"), TEXT("開始自動修復BMAD系統問題"));

    // 檢查失敗的任務
    TArray<FString> FailedTasks;
    for (const auto& TaskPair : Tasks)
    {
        if (TaskPair.Value.Status == TEXT("Failed"))
        {
            FailedTasks.Add(TaskPair.Key);
        }
    }

    // 嘗試修復失敗的任務
    for (const FString& TaskID : FailedTasks)
    {
        if (HandleTaskFailure(TaskID))
        {
            LogBMADEvent(TEXT("任務修復成功"), FString::Printf(TEXT("任務 %s 已成功修復"), *TaskID));
        }
        else
        {
            LogBMADEvent(TEXT("任務修復失敗"), FString::Printf(TEXT("任務 %s 修復失敗"), *TaskID));
        }
    }

    return true;
}

bool UMingBMADBrainWorkflowSystem::IntelligentTaskAssignment()
{
    LogBMADEvent(TEXT("智能任務分配"), TEXT("開始智能分配任務至最適合的資源"));

    // 分析任務複雜度和依賴關係
    TArray<FString> HighComplexityTasks;
    TArray<FString> MediumComplexityTasks;
    TArray<FString> LowComplexityTasks;

    for (const auto& TaskPair : Tasks)
    {
        const FTaskItem& Task = TaskPair.Value;
        if (PredictTaskComplexity(Task) > 0.7f)
        {
            HighComplexityTasks.Add(TaskPair.Key);
        }
        else if (PredictTaskComplexity(Task) > 0.4f)
        {
            MediumComplexityTasks.Add(TaskPair.Key);
        }
        else
        {
            LowComplexityTasks.Add(TaskPair.Key);
        }
    }

    // 根據複雜度分配執行順序
    int32 Priority = 1;
    
    // 高複雜度任務優先處理
    for (const FString& TaskID : HighComplexityTasks)
    {
        SetTaskPriority(TaskID, Priority++);
    }

    // 中等複雜度任務
    for (const FString& TaskID : MediumComplexityTasks)
    {
        SetTaskPriority(TaskID, Priority++);
    }

    // 低複雜度任務
    for (const FString& TaskID : LowComplexityTasks)
    {
        SetTaskPriority(TaskID, Priority++);
    }

    LogBMADEvent(TEXT("智能任務分配完成"), FString::Printf(TEXT("高複雜度: %d, 中等: %d, 低: %d"), 
        HighComplexityTasks.Num(), MediumComplexityTasks.Num(), LowComplexityTasks.Num()));

    return true;
}

bool UMingBMADBrainWorkflowSystem::PredictResourceRequirements()
{
    LogBMADEvent(TEXT("預測資源需求"), TEXT("開始預測BMAD實作所需資源"));

    // 分析任務類型和資源需求
    TMap<FString, float> ResourceEstimates;
    
    for (const auto& TaskPair : Tasks)
    {
        const FTaskItem& Task = TaskPair.Value;
        
        if (Task.Category.Contains(TEXT("開發")))
        {
            ResourceEstimates.Add(TEXT("開發時間"), ResourceEstimates.FindRef(TEXT("開發時間")) + Task.EstimatedDuration);
        }
        else if (Task.Category.Contains(TEXT("測試")))
        {
            ResourceEstimates.Add(TEXT("測試時間"), ResourceEstimates.FindRef(TEXT("測試時間")) + Task.EstimatedDuration);
        }
        else if (Task.Category.Contains(TEXT("部署")))
        {
            ResourceEstimates.Add(TEXT("部署時間"), ResourceEstimates.FindRef(TEXT("部署時間")) + Task.EstimatedDuration);
        }
    }

    // 生成資源預測報告
    FString PredictionReport = TEXT("=== 資源需求預測 ===\n");
    for (const auto& ResourcePair : ResourceEstimates)
    {
        PredictionReport += FString::Printf(TEXT("%s: %.1f 小時\n"), *ResourcePair.Key, ResourcePair.Value);
    }

    LogBMADEvent(TEXT("資源需求預測完成"), PredictionReport);
    return true;
}

bool UMingBMADBrainWorkflowSystem::OptimizeWorkflow()
{
    LogBMADEvent(TEXT("優化工作流程"), TEXT("開始優化BMAD工作流程"));

    // 分析當前工作流瓶頸
    TArray<FString> Bottlenecks;
    
    for (const auto& TaskPair : Tasks)
    {
        const FTaskItem& Task = TaskPair.Value;
        if (Task.EstimatedDuration > 2.0f) // 超過2小時的任務視為瓶頸
        {
            Bottlenecks.Add(TaskPair.Key);
        }
    }

    // 生成優化建議
    FString OptimizationReport = TEXT("=== 工作流優化建議 ===\n");
    
    if (Bottlenecks.Num() > 0)
    {
        OptimizationReport += FString::Printf(TEXT("發現 %d 個瓶頸任務:\n"), Bottlenecks.Num());
        for (const FString& TaskID : Bottlenecks)
        {
            OptimizationReport += FString::Printf(TEXT("- %s (預估 %.1f 小時)\n"), *TaskID, Tasks[TaskID].EstimatedDuration);
        }
        OptimizationReport += TEXT("建議: 考慮任務分解或並行執行\n");
    }
    else
    {
        OptimizationReport += TEXT("未發現明顯瓶頸，工作流程良好\n");
    }

    LogBMADEvent(TEXT("工作流優化完成"), OptimizationReport);
    return true;
}

FString UMingBMADBrainWorkflowSystem::GenerateBMADRecommendations()
{
    FString Recommendations = TEXT("=== BMAD大腦建議 ===\n");

    // 分析當前狀態
    int32 TotalRequirements = Requirements.Num();
    int32 TotalTasks = Tasks.Num();
    int32 CompletedTasks = 0;
    int32 FailedTasks = 0;

    for (const auto& TaskPair : Tasks)
    {
        if (TaskPair.Value.Status == TEXT("Completed"))
        {
            CompletedTasks++;
        }
        else if (TaskPair.Value.Status == TEXT("Failed"))
        {
            FailedTasks++;
        }
    }

    // 生成建議
    Recommendations += FString::Printf(TEXT("需求總數: %d\n"), TotalRequirements);
    Recommendations += FString::Printf(TEXT("任務總數: %d\n"), TotalTasks);
    Recommendations += FString::Printf(TEXT("已完成任務: %d\n"), CompletedTasks);
    Recommendations += FString::Printf(TEXT("失敗任務: %d\n"), FailedTasks);
    
    if (TotalTasks > 0)
    {
        float CompletionRate = (float)CompletedTasks / TotalTasks * 100.0f;
        Recommendations += FString::Printf(TEXT("完成率: %.1f%%\n"), CompletionRate);
    }

    // 根據狀態生成建議
    if (FailedTasks > TotalTasks * 0.1f)
    {
        Recommendations += TEXT("建議: 失敗率較高，建議檢查任務依賴和資源分配\n");
    }

    if (TotalTasks - CompletedTasks > 10)
    {
        Recommendations += TEXT("建議: 待處理任務較多，建議增加並行處理或調整優先級\n");
    }

    return Recommendations;
}

// 私有函數實現
bool UMingBMADBrainWorkflowSystem::InitializeBMADSystem()
{
    if (bIsInitialized && BMADInstance)
    {
        return true;
    }

    BMADInstance = NewObject<UMingBMADBrainWorkflowSystem>();
    if (BMADInstance)
    {
        bIsInitialized = true;
        LogBMADEvent(TEXT("BMAD系統初始化"), TEXT("BMAD大腦工作流系統已初始化"));
        return true;
    }

    LogBMADEvent(TEXT("BMAD系統初始化失敗"), TEXT("無法創建BMAD實例"));
    return false;
}

bool UMingBMADBrainWorkflowSystem::EnsureBMADExists()
{
    return InitializeBMADSystem();
}

void UMingBMADBrainWorkflowSystem::CleanupBMADSystem()
{
    if (BMADInstance)
    {
        BMADInstance = nullptr;
    }
    bIsInitialized = false;
    LogBMADEvent(TEXT("清理BMAD系統"), TEXT("BMAD大腦工作流系統已清理"));
}

bool UMingBMADBrainWorkflowSystem::ParseRequirementDocument(const FString& DocumentContent)
{
    TArray<FString> RequirementLines = ExtractRequirementsFromText(DocumentContent);
    
    for (const FString& RequirementText : RequirementLines)
    {
        FString RequirementID = GenerateUniqueID(TEXT("REQ"));
        int32 Priority = CalculateRequirementPriority(RequirementText);
        
        CreateRequirement(RequirementID, RequirementText, TEXT("從文檔提取"), Priority);
        
        // 為需求生成任務
        TArray<FString> GeneratedTasks = GenerateTasksForRequirement(RequirementID);
        for (const FString& TaskID : GeneratedTasks)
        {
            AssignTaskToRequirement(TaskID, RequirementID);
        }
    }

    LogBMADEvent(TEXT("需求文檔解析完成"), FString::Printf(TEXT("解析出 %d 個需求"), RequirementLines.Num()));
    return true;
}

TArray<FString> UMingBMADBrainWorkflowSystem::ExtractRequirementsFromText(const FString& Text)
{
    TArray<FString> Requirements;
    TArray<FString> Lines;
    Text.ParseIntoArrayLines(Lines, true);

    for (const FString& Line : Lines)
    {
        FString TrimmedLine = Line.TrimStartAndEnd();
        if (TrimmedLine.IsEmpty() || TrimmedLine.StartsWith(TEXT("#")) || TrimmedLine.StartsWith(TEXT("//")))
        {
            continue;
        }

        // 簡單的需求識別邏輯
        if (TrimmedLine.Contains(TEXT("需求")) || TrimmedLine.Contains(TEXT("需要")) || 
            TrimmedLine.Contains(TEXT("應該")) || TrimmedLine.Contains(TEXT("必須")))
        {
            Requirements.Add(TrimmedLine);
        }
    }

    return Requirements;
}

int32 UMingBMADBrainWorkflowSystem::CalculateRequirementPriority(const FString& RequirementText)
{
    // 基於關鍵詞計算優先級
    if (RequirementText.Contains(TEXT("緊急")) || RequirementText.Contains(TEXT("立即")))
    {
        return 1; // 最高優先級
    }
    else if (RequirementText.Contains(TEXT("重要")) || RequirementText.Contains(TEXT("關鍵")))
    {
        return 2; // 高優先級
    }
    else if (RequirementText.Contains(TEXT("一般")) || RequirementText.Contains(TEXT("普通")))
    {
        return 4; // 低優先級
    }
    else
    {
        return 3; // 中等優先級
    }
}

bool UMingBMADBrainWorkflowSystem::CreateTaskFromRequirement(const FRequirementItem& Requirement)
{
    FString TaskID = GenerateUniqueID(TEXT("TASK"));
    FString TaskName = FString::Printf(TEXT("實現需求: %s"), *Requirement.Title);
    FString TaskDescription = Requirement.Description;
    int32 Priority = Requirement.Priority;
    FString Category = TEXT("開發");
    float EstimatedDuration = 2.0f; // 預估2小時

    FTaskItem NewTask;
    NewTask.TaskID = TaskID;
    NewTask.TaskName = TaskName;
    NewTask.Description = TaskDescription;
    NewTask.Priority = Priority;
    NewTask.AssignedRequirementID = Requirement.RequirementID;
    NewTask.Status = TEXT("Created");
    NewTask.Category = Category;
    NewTask.EstimatedDuration = EstimatedDuration;

    Tasks.Add(TaskID, NewTask);
    return true;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GenerateTasksForRequirement(const FString& RequirementID)
{
    TArray<FString> GeneratedTasks;
    
    if (!Requirements.Contains(RequirementID))
    {
        return GeneratedTasks;
    }

    const FRequirementItem& Requirement = Requirements[RequirementID];
    
    // 為每個需求生成3個基本任務：分析、設計、實現
    for (int32 i = 0; i < 3; i++)
    {
        FString TaskID = GenerateUniqueID(TEXT("TASK"));
        FString TaskName;
        FString Category;
        
        switch (i)
        {
        case 0:
            TaskName = FString::Printf(TEXT("分析需求: %s"), *Requirement.Title);
            Category = TEXT("分析");
            break;
        case 1:
            TaskName = FString::Printf(TEXT("設計方案: %s"), *Requirement.Title);
            Category = TEXT("設計");
            break;
        case 2:
            TaskName = FString::Printf(TEXT("實現功能: %s"), *Requirement.Title);
            Category = TEXT("實現");
            break;
        }

        FTaskItem NewTask;
        NewTask.TaskID = TaskID;
        NewTask.TaskName = TaskName;
        NewTask.Description = Requirement.Description;
        NewTask.Priority = Requirement.Priority;
        NewTask.AssignedRequirementID = RequirementID;
        NewTask.Status = TEXT("Created");
        NewTask.Category = Category;
        NewTask.EstimatedDuration = 1.5f; // 每個任務1.5小時

        Tasks.Add(TaskID, NewTask);
        GeneratedTasks.Add(TaskID);
    }

    return GeneratedTasks;
}

bool UMingBMADBrainWorkflowSystem::ValidateTaskDependencies(const FTaskItem& Task)
{
    // 檢查任務依賴關係
    for (const FString& DependencyID : Task.Dependencies)
    {
        if (!Tasks.Contains(DependencyID))
        {
            return false; // 依賴的任務不存在
        }
        
        const FTaskItem& DependencyTask = Tasks[DependencyID];
        if (DependencyTask.Status != TEXT("Completed"))
        {
            return false; // 依賴的任務未完成
        }
    }

    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteWorkflowStep(const FString& StepID)
{
    LogBMADEvent(TEXT("執行工作流步驟"), FString::Printf(TEXT("步驟ID: %s"), *StepID));

    // 根據步驟ID執行相應操作
    if (StepID.Contains(TEXT("Initialize")))
    {
        return InitializeBMADWorkflow();
    }
    else if (StepID.Contains(TEXT("Analyze")))
    {
        return AnalyzeRequirementDocument(BMADConfig.RequirementDocumentPath);
    }
    else if (StepID.Contains(TEXT("Plan")))
    {
        return PlanBMADFromRequirements();
    }
    else if (StepID.Contains(TEXT("Execute")))
    {
        return ExecuteImplementationPlan();
    }
    else if (StepID.Contains(TEXT("Monitor")))
    {
        return MonitorWorkflowProgress();
    }

    return false;
}

bool UMingBMADBrainWorkflowSystem::ValidateWorkflowPrerequisites()
{
    // 檢查BMAD系統前置條件
    if (!bIsInitialized)
    {
        LogBMADEvent(TEXT("前置條件驗證失敗"), TEXT("BMAD系統未初始化"));
        return false;
    }

    if (!UMingSageBrainConsoleStarter::IsConsoleRunning())
    {
        LogBMADEvent(TEXT("前置條件驗證失敗"), TEXT("聖者大腦控制台未運行"));
        return false;
    }

    if (Requirements.Num() == 0)
    {
        LogBMADEvent(TEXT("前置條件驗證失敗"), TEXT("沒有需求單"));
        return false;
    }

    return true;
}

void UMingBMADBrainWorkflowSystem::UpdateWorkflowProgress()
{
    if (CurrentWorkflow.TotalSteps > 0)
    {
        CurrentWorkflow.Progress = (float)CurrentWorkflow.CurrentStep / CurrentWorkflow.TotalSteps * 100.0f;
    }
    
    SaveWorkflowState();
}

FString UMingBMADBrainWorkflowSystem::GenerateTaskRecommendation(const FTaskItem& Task)
{
    FString Recommendation = FString::Printf(TEXT("任務建議: %s\n"), *Task.TaskName);
    
    if (Task.EstimatedDuration > 3.0f)
    {
        Recommendation += TEXT("建議: 考慮將此長時間任務分解為多個小任務\n");
    }
    
    if (Task.Dependencies.Num() > 3)
    {
        Recommendation += TEXT("建議: 此任務依賴較多，建議優先處理依賴任務\n");
    }
    
    if (Task.Priority <= 2)
    {
        Recommendation += TEXT("建議: 高優先級任務，建議分配最佳資源\n");
    }

    return Recommendation;
}

TArray<FString> UMingBMADBrainWorkflowSystem::OptimizeTaskSequence(const TArray<FString>& TaskIDs)
{
    TArray<FString> OptimizedSequence = TaskIDs;
    
    // 簡單的優化：按優先級和依賴關係排序
    OptimizedSequence.Sort([this](const FString& A, const FString& B) {
        if (!Tasks.Contains(A) || !Tasks.Contains(B)) return false;
        
        const FTaskItem& TaskA = Tasks[A];
        const FTaskItem& TaskB = Tasks[B];
        
        // 首先按優先級排序
        if (TaskA.Priority != TaskB.Priority)
        {
            return TaskA.Priority < TaskB.Priority;
        }
        
        // 優先級相同時按依賴關係排序
        return TaskA.Dependencies.Num() < TaskB.Dependencies.Num();
    });

    return OptimizedSequence;
}

bool UMingBMADBrainWorkflowSystem::PredictTaskComplexity(const FTaskItem& Task)
{
    // 基於任務屬性預測複雜度
    float Complexity = 0.5f; // 基礎複雜度
    
    if (Task.Category.Contains(TEXT("分析")))
    {
        Complexity += 0.2f;
    }
    else if (Task.Category.Contains(TEXT("設計")))
    {
        Complexity += 0.3f;
    }
    else if (Task.Category.Contains(TEXT("實現")))
    {
        Complexity += 0.4f;
    }
    
    if (Task.EstimatedDuration > 2.0f)
    {
        Complexity += 0.2f;
    }
    
    if (Task.Dependencies.Num() > 2)
    {
        Complexity += 0.1f;
    }

    return FMath::Clamp(Complexity, 0.0f, 1.0f);
}

bool UMingBMADBrainWorkflowSystem::LaunchRequiredAutomation()
{
    return UMingSageBrainConsoleAutomation::StartAndInitialize();
}

bool UMingBMADBrainWorkflowSystem::ExecuteAutomatedTask(const FTaskItem& Task)
{
    LogBMADEvent(TEXT("執行自動化任務"), FString::Printf(TEXT("任務: %s"), *Task.TaskName));

    // 根據任務類型執行相應的自動化操作
    if (Task.Category.Contains(TEXT("分析")))
    {
        return UMingSageBrainConsoleAutomation::RunTestSequence();
    }
    else if (Task.Category.Contains(TEXT("設計")))
    {
        return UMingSageBrainConsoleAutomation::RunDeveloperSequence();
    }
    else if (Task.Category.Contains(TEXT("實現")))
    {
        return UMingSageBrainConsoleAutomation::RunSupremeSageFullWorkflow();
    }

    return false;
}

bool UMingBMADBrainWorkflowSystem::HandleTaskFailure(const FString& TaskID)
{
    if (!Tasks.Contains(TaskID))
    {
        return false;
    }

    FTaskItem& Task = Tasks[TaskID];
    Task.Status = TEXT("Failed");
    CurrentWorkflow.FailedTasks.Add(TaskID);

    LogBMADEvent(TEXT("處理任務失敗"), FString::Printf(TEXT("任務 %s 執行失敗"), *TaskID));

    // 嘗試自動修復
    if (BMADConfig.bEnableAutoFix)
    {
        return AutoFixIssues();
    }

    return false;
}

void UMingBMADBrainWorkflowSystem::LogWorkflowEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[BMAD Workflow] %s: %s"), *Event, *Details);
}

void UMingBMADBrainWorkflowSystem::UpdatePerformanceMetrics(const FString& TaskID, float Duration)
{
    // 更新性能指標
    LogWorkflowEvent(TEXT("性能指標更新"), FString::Printf(TEXT("任務 %s 耗時 %.2f 秒"), *TaskID, Duration));
}

FString UMingBMADBrainWorkflowSystem::GenerateWorkflowReport()
{
    FString Report = TEXT("=== BMAD工作流報告 ===\n");
    Report += FString::Printf(TEXT("工作流ID: %s\n"), *CurrentWorkflow.WorkflowID);
    Report += FString::Printf(TEXT("當前階段: %s\n"), *CurrentWorkflow.CurrentPhase);
    Report += FString::Printf(TEXT("當前步驟: %d/%d\n"), CurrentWorkflow.CurrentStep, CurrentWorkflow.TotalSteps);
    Report += FString::Printf(TEXT("進度: %.1f%%\n"), CurrentWorkflow.Progress);
    Report += FString::Printf(TEXT("狀態: %s\n"), *CurrentWorkflow.Status);
    Report += FString::Printf(TEXT("開始時間: %s\n"), *CurrentWorkflow.StartTime.ToString());
    Report += FString::Printf(TEXT("已完成任務: %d\n"), CurrentWorkflow.CompletedTasks.Num());
    Report += FString::Printf(TEXT("失敗任務: %d\n"), CurrentWorkflow.FailedTasks.Num());

    return Report;
}

FString UMingBMADBrainWorkflowSystem::GenerateUniqueID(const FString& Prefix)
{
    return FString::Printf(TEXT("%s_%s_%d"), *Prefix, *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::RandRange(1000, 9999));
}

FDateTime UMingBMADBrainWorkflowSystem::GetCurrentTimestamp()
{
    return FDateTime::Now();
}

bool UMingBMADBrainWorkflowSystem::SaveWorkflowState()
{
    FString StateFilePath = FPaths::ProjectSavedDir() + TEXT("BMADWorkflowState.json");
    FString StateContent = GenerateWorkflowReport();
    
    return FFileHelper::SaveStringToFile(StateContent, *StateFilePath);
}

bool UMingBMADBrainWorkflowSystem::LoadWorkflowState()
{
    FString StateFilePath = FPaths::ProjectSavedDir() + TEXT("BMADWorkflowState.json");
    
    if (!FPaths::FileExists(StateFilePath))
    {
        return false;
    }

    FString StateContent;
    return FFileHelper::LoadFileToString(StateContent, *StateFilePath);
}

void UMingBMADBrainWorkflowSystem::LogBMADEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[BMAD] %s: %s"), *Event, *Details);
}

// 預設工作流實現
TArray<FString> UMingBMADBrainWorkflowSystem::GetDevelopmentWorkflowSteps()
{
    TArray<FString> Steps;
    Steps.Add(TEXT("Initialize"));
    Steps.Add(TEXT("Analyze"));
    Steps.Add(TEXT("Plan"));
    Steps.Add(TEXT("Execute"));
    Steps.Add(TEXT("Monitor"));
    return Steps;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GetTestingWorkflowSteps()
{
    TArray<FString> Steps;
    Steps.Add(TEXT("Initialize"));
    Steps.Add(TEXT("Setup Test Environment"));
    Steps.Add(TEXT("Run Unit Tests"));
    Steps.Add(TEXT("Run Integration Tests"));
    Steps.Add(TEXT("Generate Test Report"));
    return Steps;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GetDeploymentWorkflowSteps()
{
    TArray<FString> Steps;
    Steps.Add(TEXT("Initialize"));
    Steps.Add(TEXT("Build"));
    Steps.Add(TEXT("Package"));
    Steps.Add(TEXT("Deploy"));
    Steps.Add(TEXT("Verify Deployment"));
    return Steps;
}

TArray<FString> UMingBMADBrainWorkflowSystem::GetMaintenanceWorkflowSteps()
{
    TArray<FString> Steps;
    Steps.Add(TEXT("Initialize"));
    Steps.Add(TEXT("Check System Health"));
    Steps.Add(TEXT("Run Diagnostics"));
    Steps.Add(TEXT("Apply Fixes"));
    Steps.Add(TEXT("Generate Maintenance Report"));
    return Steps;
}

// 公共接口實現
FString UMingBMADBrainWorkflowSystem::GetBMADStatus()
{
    FString Status = TEXT("=== BMAD大腦系統狀態 ===\n");
    Status += FString::Printf(TEXT("初始化狀態: %s\n"), bIsInitialized ? TEXT("已初始化") : TEXT("未初始化"));
    Status += FString::Printf(TEXT("需求單數量: %d\n"), Requirements.Num());
    Status += FString::Printf(TEXT("任務數量: %d\n"), Tasks.Num());
    Status += FString::Printf(TEXT("當前工作流: %s\n"), *CurrentWorkflow.WorkflowID);
    Status += FString::Printf(TEXT("工作流階段: %s\n"), *CurrentWorkflow.CurrentPhase);
    Status += FString::Printf(TEXT("工作流進度: %.1f%%\n"), CurrentWorkflow.Progress);
    Status += FString::Printf(TEXT("自動化啟用: %s\n"), BMADConfig.bEnableAutomation ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("智能分配啟用: %s\n"), BMADConfig.bEnableIntelligentAssignment ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("自動修復啟用: %s\n"), BMADConfig.bEnableAutoFix ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("監控啟用: %s\n"), BMADConfig.bEnableMonitoring ? TEXT("是") : TEXT("否"));

    return Status;
}

FString UMingBMADBrainWorkflowSystem::GetWorkflowReport()
{
    return GenerateWorkflowReport();
}

bool UMingBMADBrainWorkflowSystem::ExportExecutionLog(const FString& FilePath)
{
    FString LogContent = GenerateWorkflowReport();
    return FFileHelper::SaveStringToFile(LogContent, *FilePath);
}

FString UMingBMADBrainWorkflowSystem::GetPerformanceMetrics()
{
    FString Metrics = TEXT("=== BMAD性能指標 ===\n");
    
    // 計算各種指標
    int32 TotalTasks = Tasks.Num();
    int32 CompletedTasks = 0;
    int32 FailedTasks = 0;
    float TotalEstimatedTime = 0.0f;

    for (const auto& TaskPair : Tasks)
    {
        const FTaskItem& Task = TaskPair.Value;
        TotalEstimatedTime += Task.EstimatedDuration;
        
        if (Task.Status == TEXT("Completed"))
        {
            CompletedTasks++;
        }
        else if (Task.Status == TEXT("Failed"))
        {
            FailedTasks++;
        }
    }

    Metrics += FString::Printf(TEXT("總任務數: %d\n"), TotalTasks);
    Metrics += FString::Printf(TEXT("已完成: %d\n"), CompletedTasks);
    Metrics += FString::Printf(TEXT("失敗: %d\n"), FailedTasks);
    Metrics += FString::Printf(TEXT("成功率: %.1f%%\n"), TotalTasks > 0 ? (float)CompletedTasks / TotalTasks * 100.0f : 0.0f);
    Metrics += FString::Printf(TEXT("預估總時間: %.1f 小時\n"), TotalEstimatedTime);

    return Metrics;
}

bool UMingBMADBrainWorkflowSystem::LoadBMADConfiguration(const FString& ConfigPath)
{
    FString ConfigContent;
    if (!FFileHelper::LoadFileToString(ConfigContent, *ConfigPath))
    {
        return false;
    }

    // 簡化的JSON解析
    if (ConfigContent.Contains(TEXT("\"bEnableAutomation\": true")))
    {
        BMADConfig.bEnableAutomation = true;
    }
    if (ConfigContent.Contains(TEXT("\"bEnableIntelligentAssignment\": true")))
    {
        BMADConfig.bEnableIntelligentAssignment = true;
    }
    if (ConfigContent.Contains(TEXT("\"bEnableAutoFix\": true")))
    {
        BMADConfig.bEnableAutoFix = true;
    }
    if (ConfigContent.Contains(TEXT("\"bEnableMonitoring\": true")))
    {
        BMADConfig.bEnableMonitoring = true;
    }

    LogBMADEvent(TEXT("BMAD配置加載完成"), FString::Printf(TEXT("從 %s 加載配置"), *ConfigPath));
    return true;
}

bool UMingBMADBrainWorkflowSystem::SaveBMADConfiguration(const FString& ConfigPath)
{
    FString JsonContent = TEXT("{\n");
    JsonContent += FString::Printf(TEXT("  \"bEnableAutomation\": %s,\n"), BMADConfig.bEnableAutomation ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEnableIntelligentAssignment\": %s,\n"), BMADConfig.bEnableIntelligentAssignment ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEnableAutoFix\": %s,\n"), BMADConfig.bEnableAutoFix ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEnableMonitoring\": %s,\n"), BMADConfig.bEnableMonitoring ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"MaxConcurrentTasks\": %d,\n"), BMADConfig.MaxConcurrentTasks);
    JsonContent += FString::Printf(TEXT("  \"TaskTimeout\": %.1f\n"), BMADConfig.TaskTimeout);
    JsonContent += TEXT("}");

    return FFileHelper::SaveStringToFile(JsonContent, *ConfigPath);
}

bool UMingBMADBrainWorkflowSystem::ResetBMADConfiguration()
{
    BMADConfig = FBMADConfiguration();
    LogBMADEvent(TEXT("BMAD配置重置"), TEXT("配置已重置為默認值"));
    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteDevelopmentWorkflow()
{
    TArray<FString> Steps = GetDevelopmentWorkflowSteps();
    for (const FString& Step : Steps)
    {
        if (!ExecuteWorkflowStep(Step))
        {
            return false;
        }
    }
    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteTestingWorkflow()
{
    TArray<FString> Steps = GetTestingWorkflowSteps();
    for (const FString& Step : Steps)
    {
        if (!ExecuteWorkflowStep(Step))
        {
            return false;
        }
    }
    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteDeploymentWorkflow()
{
    TArray<FString> Steps = GetDeploymentWorkflowSteps();
    for (const FString& Step : Steps)
    {
        if (!ExecuteWorkflowStep(Step))
        {
            return false;
        }
    }
    return true;
}

bool UMingBMADBrainWorkflowSystem::ExecuteMaintenanceWorkflow()
{
    TArray<FString> Steps = GetMaintenanceWorkflowSteps();
    for (const FString& Step : Steps)
    {
        if (!ExecuteWorkflowStep(Step))
        {
            return false;
        }
    }
    return true;
}
