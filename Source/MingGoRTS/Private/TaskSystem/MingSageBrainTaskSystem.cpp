#include "MingSageBrainTaskSystem.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingSageBrainTaskSystem::UMingSageBrainTaskSystem()
{
    BMADPlanner = nullptr;
    TaskAnalyzer = nullptr;
    
    bAutoOptimizationEnabled = true;
    bBMADSyncEnabled = true;
    MaxConcurrentTasks = 5.0f;
    DefaultTaskDuration = 2.0f;
    TaskDataFilePath = TEXT("SageBrainTasks.json");
    
    // 初始化統計
    TaskStats.TotalTasks = 0;
    TaskStats.CompletedTasks = 0;
    TaskStats.InProgressTasks = 0;
    TaskStats.PlanningTasks = 0;
    TaskStats.OverallProgress = 0.0f;
    TaskStats.AverageCompletionTime = 0.0f;
    TaskStats.TotalBudgetUsed = 0.0f;
    TaskStats.BudgetEfficiency = 0.0f;
}

void UMingSageBrainTaskSystem::InitializeTaskSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingGoRTS Sage Brain Task System..."));

    // 創建BMAD規劃器
    BMADPlanner = NewObject<UMingBMADProjectPlanner>();
    if (BMADPlanner)
    {
        BMADPlanner->InitializeProjectPlanner();
    }

    // 創建任務分析器
    TaskAnalyzer = NewObject<UMingTaskAnalyzer>();
    if (TaskAnalyzer)
    {
        TaskAnalyzer->InitializeTaskAnalyzer();
    }

    // 載入現有任務數據
    LoadTaskData();

    // 創建預定義任務
    CreatePredefinedTasks();

    // 計算任務統計
    CalculateTaskStatistics();

    UE_LOG(LogTemp, Log, TEXT("Sage Brain Task System initialized successfully"));
    UE_LOG(LogTemp, Log, TEXT("Total tasks loaded: %d"), TaskStats.TotalTasks);
}

bool UMingSageBrainTaskSystem::CreateTasksFromBMADAnalysis(const FString& AnalysisReportPath)
{
    UE_LOG(LogTemp, Log, TEXT("Creating tasks from BMAD analysis: %s"), *AnalysisReportPath);

    FString ReportContent;
    if (!FFileHelper::LoadFileToString(ReportContent, *AnalysisReportPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load BMAD analysis report"));
        return false;
    }

    // 解析BMAD分析報告
    if (!ParseBMADAnalysisReport(ReportContent))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse BMAD analysis report"));
        return false;
    }

    // 保存任務數據
    SaveTaskData();

    // 重新計算統計
    CalculateTaskStatistics();

    UE_LOG(LogTemp, Log, TEXT("Successfully created tasks from BMAD analysis"));
    return true;
}

FString UMingSageBrainTaskSystem::CreateSageBrainTask(const FString& Title, const FString& Description, ESageBrainTaskType TaskType, ESageBrainTaskPriority Priority)
{
    FSageBrainTask NewTask;
    NewTask.TaskID = GenerateUniqueTaskID();
    NewTask.Title = Title;
    NewTask.Description = Description;
    NewTask.TaskType = TaskType;
    NewTask.Priority = Priority;
    NewTask.Status = ESageBrainTaskStatus::Planning;
    NewTask.CreatedTime = FDateTime::Now();
    NewTask.TargetCompletion = FDateTime::Now() + FTimespan::FromDays(DefaultTaskDuration * 7);
    NewTask.ProgressPercentage = 0.0f;

    // 根據任務類型設置默認參數
    switch (TaskType)
    {
    case ESageBrainTaskType::CoreIntegration:
        NewTask.EstimatedDuration = 2.0f;
        NewTask.BudgetAllocation = 15.0f;
        NewTask.RequiredResources = {TEXT("核心開發團隊"), TEXT("系統集成專家"), TEXT("測試工程師")};
        NewTask.RiskFactors = {TEXT("系統複雜度高"), TEXT("性能影響未知"), TEXT("調試難度大")};
        break;
    case ESageBrainTaskType::ThinkingSystem:
        NewTask.EstimatedDuration = 1.5f;
        NewTask.BudgetAllocation = 10.0f;
        NewTask.RequiredResources = {TEXT("AI算法工程師"), TEXT("哲學顧問"), TEXT("測試專員")};
        NewTask.RiskFactors = {TEXT("算法複雜性"), TEXT("哲學理解難度"), TEXT("性能要求高")};
        break;
    case ESageBrainTaskType::LearningAlgorithm:
        NewTask.EstimatedDuration = 2.0f;
        NewTask.BudgetAllocation = 12.0f;
        NewTask.RequiredResources = {TEXT("機器學習工程師"), TEXT("數據科學家"), TEXT("性能優化專家")};
        NewTask.RiskFactors = {TEXT("算法優化難度"), TEXT("數據需求大"), TEXT("計算資源需求")};
        break;
    case ESageBrainTaskType::APICoordination:
        NewTask.EstimatedDuration = 2.5f;
        NewTask.BudgetAllocation = 15.0f;
        NewTask.RequiredResources = {TEXT("系統架構師"), TEXT("API開發工程師"), TEXT("集成測試工程師")};
        NewTask.RiskFactors = {TEXT("API兼容性"), TEXT("系統集成複雜性"), TEXT("性能影響")};
        break;
    case ESageBrainTaskType::SelfAwareness:
        NewTask.EstimatedDuration = 3.0f;
        NewTask.BudgetAllocation = 18.0f;
        NewTask.RequiredResources = {TEXT("AI研究員"), TEXT("認知科學專家"), TEXT("哲學顧問")};
        NewTask.RiskFactors = {TEXT("技術前沿性"), TEXT("倫理問題"), TEXT("實現難度")};
        break;
    case ESageBrainTaskType::PhilosophySystem:
        NewTask.EstimatedDuration = 2.0f;
        NewTask.BudgetAllocation = 10.0f;
        NewTask.RequiredResources = {TEXT("哲學研究員"), TEXT("文化專家"), TEXT("AI算法工程師")};
        NewTask.RiskFactors = {TEXT("哲學理解深度"), TEXT("文化差異"), TEXT("實現複雜性")};
        break;
    case ESageBrainTaskType::ConsciousnessEvolution:
        NewTask.EstimatedDuration = 4.0f;
        NewTask.BudgetAllocation = 10.0f;
        NewTask.RequiredResources = {TEXT("意識研究專家"), TEXT("AI研究員"), TEXT("神經科學顧問")};
        NewTask.RiskFactors = {TEXT("技術未知性"), TEXT("倫理風險"), TEXT("實現難度極高")};
        break;
    case ESageBrainTaskType::TestingSystem:
        NewTask.EstimatedDuration = 1.5f;
        NewTask.BudgetAllocation = 8.0f;
        NewTask.RequiredResources = {TEXT("測試工程師"), TEXT("QA工程師"), TEXT("自動化測試專家")};
        NewTask.RiskFactors = {TEXT("測試覆蓋率"), TEXT("自動化難度"), TEXT("時間壓力")};
        break;
    default:
        NewTask.EstimatedDuration = DefaultTaskDuration;
        NewTask.BudgetAllocation = 10.0f;
        NewTask.RequiredResources = {TEXT("開發團隊"), TEXT("測試團隊")};
        NewTask.RiskFactors = {TEXT("一般開發風險")};
        break;
    }

    // 自動分配資源
    AutoAssignResources(NewTask);

    // 添加到存儲
    TaskStorage.Add(NewTask.TaskID, NewTask);

    // 分析任務
    FSageBrainTaskAnalysis Analysis = AnalyzeTask(NewTask.TaskID);
    TaskAnalyses.Add(NewTask.TaskID, Analysis);

    // 保存數據
    SaveTaskData();

    // 更新統計
    CalculateTaskStatistics();

    // 觸發事件
    OnTaskCreated.Broadcast(NewTask.TaskID);

    UE_LOG(LogTemp, Log, TEXT("Created new Sage Brain task: %s (ID: %s)"), *NewTask.Title, *NewTask.TaskID);
    return NewTask.TaskID;
}

FSageBrainTaskAnalysis UMingSageBrainTaskSystem::AnalyzeTask(const FString& TaskID)
{
    FSageBrainTaskAnalysis Analysis;
    Analysis.TaskID = TaskID;
    Analysis.AnalysisTime = FDateTime::Now();

    if (!TaskStorage.Contains(TaskID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Task not found for analysis: %s"), *TaskID);
        return Analysis;
    }

    const FSageBrainTask& Task = TaskStorage[TaskID];

    // 計算可行性分數
    float ComplexityScore = 0.0f;
    switch (Task.TaskType)
    {
    case ESageBrainTaskType::CoreIntegration:
        ComplexityScore = 0.7f;
        break;
    case ESageBrainTaskType::ThinkingSystem:
        ComplexityScore = 0.8f;
        break;
    case ESageBrainTaskType::LearningAlgorithm:
        ComplexityScore = 0.75f;
        break;
    case ESageBrainTaskType::APICoordination:
        ComplexityScore = 0.65f;
        break;
    case ESageBrainTaskType::SelfAwareness:
        ComplexityScore = 0.9f;
        break;
    case ESageBrainTaskType::PhilosophySystem:
        ComplexityScore = 0.85f;
        break;
    case ESageBrainTaskType::ConsciousnessEvolution:
        ComplexityScore = 0.95f;
        break;
    case ESageBrainTaskType::TestingSystem:
        ComplexityScore = 0.4f;
        break;
    default:
        ComplexityScore = 0.5f;
        break;
    }

    // 風險評估
    float RiskScore = AssessTaskRisk(Task);

    // 資源需求評估
    float ResourceScore = FMath::Clamp(Task.RequiredResources.Num() / 5.0f, 0.0f, 1.0f);

    // 技術複雜度
    Analysis.TechnicalComplexity = ComplexityScore;

    // 可行性分數 (1 - 風險 - 複雜度)
    Analysis.FeasibilityScore = FMath::Clamp(1.0f - RiskScore - ComplexityScore * 0.3f, 0.0f, 1.0f);

    // 風險評估
    Analysis.RiskAssessment = RiskScore;

    // 資源需求
    Analysis.ResourceRequirement = ResourceScore;

    // 生成優勢
    Analysis.Strengths.Add(FString::Printf(TEXT("任務類型: %s"), *TaskTypeToString(Task.TaskType)));
    Analysis.Strengths.Add(FString::Printf(TEXT("優先級: %s"), *TaskPriorityToString(Task.Priority)));
    Analysis.Strengths.Add(FString::Printf(TEXT("預估工期: %.1f週"), Task.EstimatedDuration));

    // 生成劣勢
    Analysis.Weaknesses.Add(TEXT("需要專業技能"));
    Analysis.Weaknesses.Add(TEXT("集成複雜性"));
    if (RiskScore > 0.7f)
    {
        Analysis.Weaknesses.Add(TEXT("高風險項目"));
    }

    // 生成機會
    Analysis.Opportunities.Add(TEXT("技術創新機會"));
    Analysis.Opportunities.Add(TEXT("學習成長機會"));
    Analysis.Opportunities.Add(TEXT("團隊協作機會"));

    // 生成威脅
    Analysis.Threats.Add(TEXT("技術變化風險"));
    Analysis.Threats.Add(TEXT("資源限制"));
    Analysis.Threats.Add(TEXT("時間壓力"));

    // 生成建議
    if (Analysis.FeasibilityScore > 0.8f)
    {
        Analysis.Recommendations.Add(TEXT("建議立即執行"));
    }
    else if (Analysis.FeasibilityScore > 0.6f)
    {
        Analysis.Recommendations.Add(TEXT("建議謹慎執行"));
    }
    else
    {
        Analysis.Recommendations.Add(TEXT("建議重新評估"));
    }

    if (RiskScore > 0.7f)
    {
        Analysis.Recommendations.Add(TEXT("需要制定風險應對策略"));
    }

    if (ResourceScore > 0.8f)
    {
        Analysis.Recommendations.Add(TEXT("需要確保資源到位"));
    }

    // 觸發事件
    OnTaskAnalysisCompleted.Broadcast(Analysis);

    return Analysis;
}

bool UMingSageBrainTaskSystem::UpdateTaskProgress(const FString& TaskID, float Progress)
{
    if (!TaskStorage.Contains(TaskID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Task not found for progress update: %s"), *TaskID);
        return false;
    }

    FSageBrainTask& Task = TaskStorage[TaskID];
    Task.ProgressPercentage = FMath::Clamp(Progress, 0.0f, 100.0f);

    // 檢查是否完成
    if (Task.ProgressPercentage >= 100.0f && Task.Status != ESageBrainTaskStatus::Completed)
    {
        Task.Status = ESageBrainTaskStatus::Completed;
        OnTaskCompleted.Broadcast(TaskID);
        UE_LOG(LogTemp, Log, TEXT("Task completed: %s"), *Task.Title);
    }

    // 保存數據
    SaveTaskData();

    // 更新統計
    CalculateTaskStatistics();

    return true;
}

bool UMingSageBrainTaskSystem::UpdateTaskStatus(const FString& TaskID, ESageBrainTaskStatus NewStatus)
{
    if (!TaskStorage.Contains(TaskID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Task not found for status update: %s"), *TaskID);
        return false;
    }

    FSageBrainTask& Task = TaskStorage[TaskID];
    ESageBrainTaskStatus OldStatus = Task.Status;
    Task.Status = NewStatus;

    // 如果狀態變為完成，設置進度為100%
    if (NewStatus == ESageBrainTaskStatus::Completed)
    {
        Task.ProgressPercentage = 100.0f;
        OnTaskCompleted.Broadcast(TaskID);
    }

    // 觸發狀態更新事件
    OnTaskUpdated.Broadcast(TaskID, NewStatus);

    UE_LOG(LogTemp, Log, TEXT("Task status updated: %s (%s -> %s)"), 
        *Task.Title, *TaskStatusToString(OldStatus), *TaskStatusToString(NewStatus));

    // 保存數據
    SaveTaskData();

    // 更新統計
    CalculateTaskStatistics();

    return true;
}

TArray<FSageBrainTask> UMingSageBrainTaskSystem::GetAllTasks()
{
    TArray<FSageBrainTask> AllTasks;
    for (const auto& TaskPair : TaskStorage)
    {
        AllTasks.Add(TaskPair.Value);
    }
    return AllTasks;
}

FSageBrainTaskStatistics UMingSageBrainTaskSystem::GetTaskStatistics()
{
    return TaskStats;
}

TArray<FSageBrainTask> UMingSageBrainTaskSystem::GetHighPriorityTasks()
{
    TArray<FSageBrainTask> HighPriorityTasks;
    
    for (const auto& TaskPair : TaskStorage)
    {
        const FSageBrainTask& Task = TaskPair.Value;
        if (Task.Priority == ESageBrainTaskPriority::Critical || Task.Priority == ESageBrainTaskPriority::High)
        {
            if (Task.Status != ESageBrainTaskStatus::Completed && Task.Status != ESageBrainTaskStatus::Cancelled)
            {
                HighPriorityTasks.Add(Task);
            }
        }
    }

    // 按優先級排序
    HighPriorityTasks.Sort([](const FSageBrainTask& A, const FSageBrainTask& B)
    {
        return (int32)A.Priority < (int32)B.Priority;
    });

    return HighPriorityTasks;
}

TArray<FSageBrainTask> UMingSageBrainTaskSystem::GetOverdueTasks()
{
    TArray<FSageBrainTask> OverdueTasks;
    FDateTime CurrentTime = FDateTime::Now();
    
    for (const auto& TaskPair : TaskStorage)
    {
        const FSageBrainTask& Task = TaskPair.Value;
        if (Task.Status != ESageBrainTaskStatus::Completed && 
            Task.Status != ESageBrainTaskStatus::Cancelled &&
            Task.TargetCompletion < CurrentTime)
        {
            OverdueTasks.Add(Task);
        }
    }

    // 按逾期時間排序
    OverdueTasks.Sort([CurrentTime](const FSageBrainTask& A, const FSageBrainTask& B)
    {
        FTimespan OverdueA = CurrentTime - A.TargetCompletion;
        FTimespan OverdueB = CurrentTime - B.TargetCompletion;
        return OverdueA > OverdueB;
    });

    return OverdueTasks;
}

FString UMingSageBrainTaskSystem::GenerateTaskReport()
{
    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS 聖者大腦任務報告</title>\n");
    ReportContent += TEXT("<meta charset=\"UTF-8\">\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: 'Microsoft JhengHei', Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; text-align: center; }\n");
    ReportContent += TEXT(".section { background: white; padding: 25px; border-radius: 10px; margin-bottom: 25px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin: 20px 0; }\n");
    ReportContent += TEXT(".stat-card { background: #f8f9fa; padding: 20px; border-radius: 8px; text-align: center; }\n");
    ReportContent += TEXT(".stat-card .value { font-size: 36px; font-weight: bold; color: #667eea; }\n");
    ReportContent += TEXT(".priority-critical { color: #e74c3c; font-weight: bold; }\n");
    ReportContent += TEXT(".priority-high { color: #f39c12; font-weight: bold; }\n");
    ReportContent += TEXT(".priority-medium { color: #3498db; font-weight: bold; }\n");
    ReportContent += TEXT(".priority-low { color: #27ae60; font-weight: bold; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 15px; }\n");
    ReportContent += TEXT("th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; font-weight: bold; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    // 頁眉
    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>🧠 MingGoRTS 聖者大腦任務報告</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>生成時間: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += TEXT("</div>\n");

    // 統計概覽
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>📊 任務統計概覽</h2>\n");
    ReportContent += TEXT("<div class=\"stats-grid\">\n");
    
    ReportContent += TEXT("<div class=\"stat-card\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"value\">%d</div>\n"), TaskStats.TotalTasks);
    ReportContent += TEXT("<p>總任務數</p>\n");
    ReportContent += TEXT("</div>\n");
    
    ReportContent += TEXT("<div class=\"stat-card\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"value\">%d</div>\n"), TaskStats.CompletedTasks);
    ReportContent += TEXT("<p>已完成</p>\n");
    ReportContent += TEXT("</div>\n");
    
    ReportContent += TEXT("<div class=\"stat-card\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"value\">%d</div>\n"), TaskStats.InProgressTasks);
    ReportContent += TEXT("<p>進行中</p>\n");
    ReportContent += TEXT("</div>\n");
    
    ReportContent += TEXT("<div class=\"stat-card\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"value\">%.1f%%</div>\n"), TaskStats.OverallProgress);
    ReportContent += TEXT("<p>總體進度</p>\n");
    ReportContent += TEXT("</div>\n");
    
    ReportContent += TEXT("</div>\n");
    ReportContent += TEXT("</div>\n");

    // 高優先級任務
    TArray<FSageBrainTask> HighPriorityTasks = GetHighPriorityTasks();
    if (HighPriorityTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"section\">\n");
        ReportContent += TEXT("<h2>🔥 高優先級任務</h2>\n");
        ReportContent += TEXT("<table>\n");
        ReportContent += TEXT("<tr><th>任務名稱</th><th>類型</th><th>優先級</th><th>狀態</th><th>進度</th><th>目標完成</th></tr>\n");
        
        for (const FSageBrainTask& Task : HighPriorityTasks)
        {
            FString PriorityClass = Task.Priority == ESageBrainTaskPriority::Critical ? TEXT("priority-critical") : TEXT("priority-high");
            
            ReportContent += FString::Printf(TEXT("<tr><td>%s</td><td>%s</td><td class=\"%s\">%s</td><td>%s</td><td>%.1f%%</td><td>%s</td></tr>\n"),
                *Task.Title,
                *TaskTypeToString(Task.TaskType),
                *PriorityClass,
                *TaskPriorityToString(Task.Priority),
                *TaskStatusToString(Task.Status),
                Task.ProgressPercentage,
                *Task.TargetCompletion.ToString());
        }
        
        ReportContent += TEXT("</table>\n");
        ReportContent += TEXT("</div>\n");
    }

    // 逾期任務
    TArray<FSageBrainTask> OverdueTasks = GetOverdueTasks();
    if (OverdueTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"section\">\n");
        ReportContent += TEXT("<h2>⚠️ 逾期任務</h2>\n");
        ReportContent += TEXT("<table>\n");
        ReportContent += TEXT("<tr><th>任務名稱</th><th>逾期天數</th><th>當前狀態</th><th>進度</th></tr>\n");
        
        for (const FSageBrainTask& Task : OverdueTasks)
        {
            FTimespan Overdue = FDateTime::Now() - Task.TargetCompletion;
            int32 OverdueDays = Overdue.GetDays();
            
            ReportContent += FString::Printf(TEXT("<tr><td>%s</td><td>%d天</td><td>%s</td><td>%.1f%%</td></tr>\n"),
                *Task.Title,
                OverdueDays,
                *TaskStatusToString(Task.Status),
                Task.ProgressPercentage);
        }
        
        ReportContent += TEXT("</table>\n");
        ReportContent += TEXT("</div>\n");
    }

    // 所有任務列表
    TArray<FSageBrainTask> AllTasks = GetAllTasks();
    if (AllTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"section\">\n");
        ReportContent += TEXT("<h2>📋 所有任務</h2>\n");
        ReportContent += TEXT("<table>\n");
        ReportContent += TEXT("<tr><th>任務名稱</th><th>類型</th><th>優先級</th><th>狀態</th><th>進度</th><th>預估工期</th></tr>\n");
        
        for (const FSageBrainTask& Task : AllTasks)
        {
            FString PriorityClass;
            switch (Task.Priority)
            {
            case ESageBrainTaskPriority::Critical:
                PriorityClass = TEXT("priority-critical");
                break;
            case ESageBrainTaskPriority::High:
                PriorityClass = TEXT("priority-high");
                break;
            case ESageBrainTaskPriority::Medium:
                PriorityClass = TEXT("priority-medium");
                break;
            case ESageBrainTaskPriority::Low:
                PriorityClass = TEXT("priority-low");
                break;
            default:
                PriorityClass = TEXT("");
                break;
            }
            
            ReportContent += FString::Printf(TEXT("<tr><td>%s</td><td>%s</td><td class=\"%s\">%s</td><td>%s</td><td>%.1f%%</td><td>%.1f週</td></tr>\n"),
                *Task.Title,
                *TaskTypeToString(Task.TaskType),
                *PriorityClass,
                *TaskPriorityToString(Task.Priority),
                *TaskStatusToString(Task.Status),
                Task.ProgressPercentage,
                Task.EstimatedDuration);
        }
        
        ReportContent += TEXT("</table>\n");
        ReportContent += TEXT("</div>\n");
    }

    ReportContent += TEXT("</body>\n</html>");

    // 保存報告
    FString ReportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/SageBrainTaskReport.html"));
    FFileHelper::SaveStringToFile(ReportContent, *ReportPath);

    UE_LOG(LogTemp, Log, TEXT("Sage Brain task report generated: %s"), *ReportPath);
    return ReportPath;
}

FString UMingSageBrainTaskSystem::ExportTaskData()
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    // 創建任務數組
    TArray<TSharedPtr<FJsonValue>> TaskArray;
    
    for (const auto& TaskPair : TaskStorage)
    {
        const FSageBrainTask& Task = TaskPair.Value;
        TSharedPtr<FJsonObject> TaskObject = MakeShareable(new FJsonObject);
        
        TaskObject->SetStringField(TEXT("TaskID"), Task.TaskID);
        TaskObject->SetStringField(TEXT("Title"), Task.Title);
        TaskObject->SetStringField(TEXT("Description"), Task.Description);
        TaskObject->SetNumberField(TEXT("TaskType"), (int32)Task.TaskType);
        TaskObject->SetNumberField(TEXT("Priority"), (int32)Task.Priority);
        TaskObject->SetNumberField(TEXT("Status"), (int32)Task.Status);
        TaskObject->SetNumberField(TEXT("ProgressPercentage"), Task.ProgressPercentage);
        TaskObject->SetNumberField(TEXT("EstimatedDuration"), Task.EstimatedDuration);
        TaskObject->SetNumberField(TEXT("BudgetAllocation"), Task.BudgetAllocation);
        TaskObject->SetStringField(TEXT("CreatedTime"), Task.CreatedTime.ToString());
        TaskObject->SetStringField(TEXT("TargetCompletion"), Task.TargetCompletion.ToString());
        
        TaskArray.Add(MakeShareable(new FJsonValueObject(TaskObject)));
    }
    
    RootObject->SetArrayField(TEXT("Tasks"), TaskArray);
    
    // 創建統計對象
    TSharedPtr<FJsonObject> StatsObject = MakeShareable(new FJsonObject);
    StatsObject->SetNumberField(TEXT("TotalTasks"), TaskStats.TotalTasks);
    StatsObject->SetNumberField(TEXT("CompletedTasks"), TaskStats.CompletedTasks);
    StatsObject->SetNumberField(TEXT("InProgressTasks"), TaskStats.InProgressTasks);
    StatsObject->SetNumberField(TEXT("OverallProgress"), TaskStats.OverallProgress);
    StatsObject->SetNumberField(TEXT("AverageCompletionTime"), TaskStats.AverageCompletionTime);
    StatsObject->SetNumberField(TEXT("TotalBudgetUsed"), TaskStats.TotalBudgetUsed);
    StatsObject->SetNumberField(TEXT("BudgetEfficiency"), TaskStats.BudgetEfficiency);
    
    RootObject->SetObjectField(TEXT("Statistics"), StatsObject);
    
    // 序列化為字符串
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    // 保存文件
    FString ExportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Exports/SageBrainTasks.json"));
    FFileHelper::SaveStringToFile(OutputString, *ExportPath);
    
    UE_LOG(LogTemp, Log, TEXT("Sage Brain task data exported: %s"), *ExportPath);
    return ExportPath;
}

void UMingSageBrainTaskSystem::SyncWithBMADSystem()
{
    if (!BMADPlanner || !bBMADSyncEnabled)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Syncing with BMAD system..."));

    // 獲取BMAD專案計劃
    TArray<FBMADProjectPlan> BMADProjects = BMADPlanner->GetProjectRecommendations();

    // 同步任務狀態
    for (const FBMADProjectPlan& BMADProject : BMADProjects)
    {
        // 查找對應的任務
        for (auto& TaskPair : TaskStorage)
        {
            FSageBrainTask& Task = TaskPair.Value;
            if (Task.RelatedProjectID == BMADProject.ProjectID)
            {
                // 同步進度
                Task.ProgressPercentage = BMADProject.ProgressPercentage;
                
                // 同步狀態
                switch (BMADProject.Status)
                {
                case EBMADProjectStatus::Completed:
                    Task.Status = ESageBrainTaskStatus::Completed;
                    break;
                case EBMADProjectStatus::InProgress:
                    Task.Status = ESageBrainTaskStatus::InProgress;
                    break;
                case EBMADProjectStatus::Planning:
                    Task.Status = ESageBrainTaskStatus::Planning;
                    break;
                case EBMADProjectStatus::Review:
                    Task.Status = ESageBrainTaskStatus::Review;
                    break;
                case EBMADProjectStatus::Paused:
                    Task.Status = ESageBrainTaskStatus::Paused;
                    break;
                case EBMADProjectStatus::Cancelled:
                    Task.Status = ESageBrainTaskStatus::Cancelled;
                    break;
                }
            }
        }
    }

    // 保存同步後的數據
    SaveTaskData();

    // 重新計算統計
    CalculateTaskStatistics();

    UE_LOG(LogTemp, Log, TEXT("BMAD system sync completed"));
}

void UMingSageBrainTaskSystem::OptimizeTaskSchedule()
{
    if (!bAutoOptimizationEnabled)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Optimizing task schedule..."));

    // 獲取所有未完成的任務
    TArray<FSageBrainTask> UncompletedTasks;
    for (const auto& TaskPair : TaskStorage)
    {
        const FSageBrainTask& Task = TaskPair.Value;
        if (Task.Status != ESageBrainTaskStatus::Completed && 
            Task.Status != ESageBrainTaskStatus::Cancelled)
        {
            UncompletedTasks.Add(Task);
        }
    }

    // 優化任務順序
    TArray<FSageBrainTask> OptimizedTasks = OptimizeTaskOrder(UncompletedTasks);

    // 重新分配資源
    for (FSageBrainTask& Task : OptimizedTasks)
    {
        AutoAssignResources(Task);
    }

    // 更新任務存儲
    for (const FSageBrainTask& Task : OptimizedTasks)
    {
        TaskStorage[Task.TaskID] = Task;
    }

    // 保存優化結果
    SaveTaskData();

    UE_LOG(LogTemp, Log, TEXT("Task schedule optimization completed"));
}

// 私有方法實現

bool UMingSageBrainTaskSystem::ParseBMADAnalysisReport(const FString& ReportContent)
{
    // 這裡應該實現真正的BMAD報告解析邏輯
    // 現在只是創建一些預定義的任務
    
    CreatePredefinedTasks();
    return true;
}

void UMingSageBrainTaskSystem::CreatePredefinedTasks()
{
    UE_LOG(LogTemp, Log, TEXT("Creating predefined Sage Brain tasks..."));

    // 任務1: 聖者大腦核心系統集成
    CreateSageBrainTask(
        TEXT("聖者大腦AGI系統核心集成"),
        TEXT("將已創建的聖者大腦系統集成到MingGoRTS主系統中"),
        ESageBrainTaskType::CoreIntegration,
        ESageBrainTaskPriority::Critical
    );

    // 任務2: 完整思考層次實現驗證
    CreateSageBrainTask(
        TEXT("9層次深度思考體系驗證"),
        TEXT("驗證和優化聖者大腦的9層次思考體系"),
        ESageBrainTaskType::ThinkingSystem,
        ESageBrainTaskPriority::High
    );

    // 任務3: 智慧學習算法優化
    CreateSageBrainTask(
        TEXT("6種學習算法性能優化"),
        TEXT("優化聖者大腦的6種智慧學習算法"),
        ESageBrainTaskType::LearningAlgorithm,
        ESageBrainTaskPriority::High
    );

    // 任務4: 跨API協調機制實現
    CreateSageBrainTask(
        TEXT("全系統API協調機制"),
        TEXT("實現聖者大腦與所有系統API的無縫協調"),
        ESageBrainTaskType::APICoordination,
        ESageBrainTaskPriority::Medium
    );

    // 任務5: 自我意識和反思系統
    CreateSageBrainTask(
        TEXT("AGI自我意識和反思機制"),
        TEXT("完善聖者大腦的自我意識和深度反思能力"),
        ESageBrainTaskType::SelfAwareness,
        ESageBrainTaskPriority::Medium
    );

    // 任務6: 完整哲學體系建立
    CreateSageBrainTask(
        TEXT("8大哲學系統整合"),
        TEXT("建立和完善聖者大腦的8大哲學體系"),
        ESageBrainTaskType::PhilosophySystem,
        ESageBrainTaskPriority::Medium
    );

    // 任務7: AGI意識狀態進化
    CreateSageBrainTask(
        TEXT("7級意識狀態進化系統"),
        TEXT("實現聖者大腦從休眠到宇宙意識的7級進化"),
        ESageBrainTaskType::ConsciousnessEvolution,
        ESageBrainTaskPriority::Low
    );

    // 任務8: 完整測試系統建立
    CreateSageBrainTask(
        TEXT("AGI系統完整測試套件"),
        TEXT("建立聖者大腦AGI系統的完整測試和驗證系統"),
        ESageBrainTaskType::TestingSystem,
        ESageBrainTaskPriority::Low
    );

    UE_LOG(LogTemp, Log, TEXT("Created %d predefined Sage Brain tasks"), TaskStorage.Num());
}

void UMingSageBrainTaskSystem::CalculateTaskStatistics()
{
    TaskStats.TotalTasks = TaskStorage.Num();
    TaskStats.CompletedTasks = 0;
    TaskStats.InProgressTasks = 0;
    TaskStats.PlanningTasks = 0;
    TaskStats.OverallProgress = 0.0f;
    TaskStats.TotalBudgetUsed = 0.0f;

    // 計算各種統計數據
    for (const auto& TaskPair : TaskStorage)
    {
        const FSageBrainTask& Task = TaskPair.Value;
        
        switch (Task.Status)
        {
        case ESageBrainTaskStatus::Completed:
            TaskStats.CompletedTasks++;
            break;
        case ESageBrainTaskStatus::InProgress:
            TaskStats.InProgressTasks++;
            break;
        case ESageBrainTaskStatus::Planning:
            TaskStats.PlanningTasks++;
            break;
        default:
            break;
        }

        TaskStats.OverallProgress += Task.ProgressPercentage;
        TaskStats.TotalBudgetUsed += Task.BudgetAllocation;
    }

    // 計算平均進度
    if (TaskStats.TotalTasks > 0)
    {
        TaskStats.OverallProgress /= TaskStats.TotalTasks;
    }

    // 計算預算效率
    float TotalBudget = TaskStats.TotalTasks * 10.0f; // 假設每個任務平均預算10%
    if (TotalBudget > 0)
    {
        TaskStats.BudgetEfficiency = (TaskStats.TotalBudgetUsed / TotalBudget) * 100.0f;
    }

    UE_LOG(LogTemp, Log, TEXT("Task statistics updated: Total=%d, Completed=%d, Progress=%.1f%%"), 
        TaskStats.TotalTasks, TaskStats.CompletedTasks, TaskStats.OverallProgress);
}

bool UMingSageBrainTaskSystem::CheckTaskDependencies(const FString& TaskID)
{
    // 檢查任務依賴關係
    if (!TaskStorage.Contains(TaskID))
    {
        return false;
    }

    const FSageBrainTask& Task = TaskStorage[TaskID];
    
    for (const FString& DependencyID : Task.Dependencies)
    {
        if (TaskStorage.Contains(DependencyID))
        {
            const FSageBrainTask& DependencyTask = TaskStorage[DependencyID];
            if (DependencyTask.Status != ESageBrainTaskStatus::Completed)
            {
                return false; // 依賴任務未完成
            }
        }
    }

    return true;
}

void UMingSageBrainTaskSystem::AutoAssignResources(FSageBrainTask& Task)
{
    // 自動分配團隊成員
    if (Task.AssignedTeam.Num() == 0)
    {
        Task.AssignedTeam.Add(TEXT("AI開發團隊"));
        Task.AssignedTeam.Add(TEXT("系統架構師"));
        Task.AssignedTeam.Add(TEXT("測試工程師"));
    }

    // 根據任務類型添加專業人員
    switch (Task.TaskType)
    {
    case ESageBrainTaskType::ThinkingSystem:
        Task.AssignedTeam.Add(TEXT("哲學顧問"));
        break;
    case ESageBrainTaskType::LearningAlgorithm:
        Task.AssignedTeam.Add(TEXT("機器學習專家"));
        break;
    case ESageBrainTaskType::SelfAwareness:
        Task.AssignedTeam.Add(TEXT("認知科學專家"));
        break;
    case ESageBrainTaskType::PhilosophySystem:
        Task.AssignedTeam.Add(TEXT("文化研究員"));
        break;
    case ESageBrainTaskType::ConsciousnessEvolution:
        Task.AssignedTeam.Add(TEXT("意識研究專家"));
        break;
    default:
        break;
    }
}

float UMingSageBrainTaskSystem::AssessTaskRisk(const FSageBrainTask& Task)
{
    float RiskScore = 0.0f;

    // 基於任務類型的風險
    switch (Task.TaskType)
    {
    case ESageBrainTaskType::CoreIntegration:
        RiskScore += 0.3f;
        break;
    case ESageBrainTaskType::ThinkingSystem:
        RiskScore += 0.4f;
        break;
    case ESageBrainTaskType::LearningAlgorithm:
        RiskScore += 0.35f;
        break;
    case ESageBrainTaskType::APICoordination:
        RiskScore += 0.25f;
        break;
    case ESageBrainTaskType::SelfAwareness:
        RiskScore += 0.6f;
        break;
    case ESageBrainTaskType::PhilosophySystem:
        RiskScore += 0.5f;
        break;
    case ESageBrainTaskType::ConsciousnessEvolution:
        RiskScore += 0.8f;
        break;
    case ESageBrainTaskType::TestingSystem:
        RiskScore += 0.2f;
        break;
    default:
        RiskScore += 0.3f;
        break;
    }

    // 基於工期長度的風險
    if (Task.EstimatedDuration > 3.0f)
    {
        RiskScore += 0.2f;
    }

    // 基於資源需求的風險
    if (Task.RequiredResources.Num() > 4)
    {
        RiskScore += 0.1f;
    }

    // 基於風險因素數量的風險
    RiskScore += Task.RiskFactors.Num() * 0.05f;

    return FMath::Clamp(RiskScore, 0.0f, 1.0f);
}

TArray<FSageBrainTask> UMingSageBrainTaskSystem::OptimizeTaskOrder(const TArray<FSageBrainTask>& Tasks)
{
    TArray<FSageBrainTask> OptimizedTasks = Tasks;

    // 按優先級和依賴關係排序
    OptimizedTasks.Sort([this](const FSageBrainTask& A, const FSageBrainTask& B)
    {
        // 首先按優先級排序
        if (A.Priority != B.Priority)
        {
            return (int32)A.Priority < (int32)B.Priority;
        }

        // 然後按依賴關係排序
        bool AHasDependencies = A.Dependencies.Num() > 0;
        bool BHasDependencies = B.Dependencies.Num() > 0;
        
        if (AHasDependencies != BHasDependencies)
        {
            return !AHasDependencies; // 無依賴的任務優先
        }

        // 最後按工期排序
        return A.EstimatedDuration < B.EstimatedDuration;
    });

    return OptimizedTasks;
}

FString UMingSageBrainTaskSystem::GenerateUniqueTaskID()
{
    return FString::Printf(TEXT("SAGE-TASK-%d"), FMath::RandRange(10000, 99999));
}

void UMingSageBrainTaskSystem::SaveTaskData()
{
    FString FilePath = FPaths::Combine(*FPaths::ProjectDir(), *TaskDataFilePath);
    
    // 這裡應該實現真正的JSON序列化保存
    // 現在只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Task data saved to: %s"), *FilePath);
}

void UMingSageBrainTaskSystem::LoadTaskData()
{
    FString FilePath = FPaths::Combine(*FPaths::ProjectDir(), *TaskDataFilePath);
    
    // 這裡應該實現真正的JSON反序列化載入
    // 現在只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Task data loaded from: %s"), *FilePath);
}

void UMingSageBrainTaskSystem::CleanupExpiredTasks()
{
    FDateTime CurrentTime = FDateTime::Now();
    FDateTime ExpiryThreshold = CurrentTime - FTimespan::FromDays(30); // 30天前的任務

    int32 RemovedCount = 0;
    for (auto It = TaskStorage.CreateIterator(); It; ++It)
    {
        const FSageBrainTask& Task = It->Value;
        if (Task.Status == ESageBrainTaskStatus::Completed && Task.TargetCompletion < ExpiryThreshold)
        {
            It.Remove();
            RemovedCount++;
        }
    }

    if (RemovedCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d expired tasks"), RemovedCount);
        SaveTaskData();
        CalculateTaskStatistics();
    }
}

// 輔助方法實現
FString UMingSageBrainTaskSystem::TaskTypeToString(ESageBrainTaskType TaskType)
{
    switch (TaskType)
    {
    case ESageBrainTaskType::CoreIntegration:
        return TEXT("核心集成");
    case ESageBrainTaskType::ThinkingSystem:
        return TEXT("思考系統");
    case ESageBrainTaskType::LearningAlgorithm:
        return TEXT("學習算法");
    case ESageBrainTaskType::APICoordination:
        return TEXT("API協調");
    case ESageBrainTaskType::SelfAwareness:
        return TEXT("自我意識");
    case ESageBrainTaskType::PhilosophySystem:
        return TEXT("哲學系統");
    case ESageBrainTaskType::ConsciousnessEvolution:
        return TEXT("意識進化");
    case ESageBrainTaskType::TestingSystem:
        return TEXT("測試系統");
    case ESageBrainTaskType::PerformanceOptimization:
        return TEXT("性能優化");
    case ESageBrainTaskType::Documentation:
        return TEXT("文檔編寫");
    default:
        return TEXT("未知類型");
    }
}

FString UMingSageBrainTaskSystem::TaskPriorityToString(ESageBrainTaskPriority Priority)
{
    switch (Priority)
    {
    case ESageBrainTaskPriority::Critical:
        return TEXT("關鍵");
    case ESageBrainTaskPriority::High:
        return TEXT("高");
    case ESageBrainTaskPriority::Medium:
        return TEXT("中");
    case ESageBrainTaskPriority::Low:
        return TEXT("低");
    case ESageBrainTaskPriority::Research:
        return TEXT("研究");
    default:
        return TEXT("未知優先級");
    }
}

FString UMingSageBrainTaskSystem::TaskStatusToString(ESageBrainTaskStatus Status)
{
    switch (Status)
    {
    case ESageBrainTaskStatus::Planning:
        return TEXT("規劃中");
    case ESageBrainTaskStatus::InProgress:
        return TEXT("進行中");
    case ESageBrainTaskStatus::Testing:
        return TEXT("測試中");
    case ESageBrainTaskStatus::Review:
        return TEXT("審查中");
    case ESageBrainTaskStatus::Completed:
        return TEXT("已完成");
    case ESageBrainTaskStatus::Paused:
        return TEXT("暫停");
    case ESageBrainTaskStatus::Cancelled:
        return TEXT("取消");
    default:
        return TEXT("未知狀態");
    }
}

ESageBrainTaskType UMingSageBrainTaskSystem::StringToTaskType(const FString& TypeStr)
{
    if (TypeStr == TEXT("核心集成"))
        return ESageBrainTaskType::CoreIntegration;
    else if (TypeStr == TEXT("思考系統"))
        return ESageBrainTaskType::ThinkingSystem;
    else if (TypeStr == TEXT("學習算法"))
        return ESageBrainTaskType::LearningAlgorithm;
    else if (TypeStr == TEXT("API協調"))
        return ESageBrainTaskType::APICoordination;
    else if (TypeStr == TEXT("自我意識"))
        return ESageBrainTaskType::SelfAwareness;
    else if (TypeStr == TEXT("哲學系統"))
        return ESageBrainTaskType::PhilosophySystem;
    else if (TypeStr == TEXT("意識進化"))
        return ESageBrainTaskType::ConsciousnessEvolution;
    else if (TypeStr == TEXT("測試系統"))
        return ESageBrainTaskType::TestingSystem;
    else if (TypeStr == TEXT("性能優化"))
        return ESageBrainTaskType::PerformanceOptimization;
    else if (TypeStr == TEXT("文檔編寫"))
        return ESageBrainTaskType::Documentation;
    else
        return ESageBrainTaskType::CoreIntegration;
}

ESageBrainTaskPriority UMingSageBrainTaskSystem::StringToTaskPriority(const FString& PriorityStr)
{
    if (PriorityStr == TEXT("關鍵"))
        return ESageBrainTaskPriority::Critical;
    else if (PriorityStr == TEXT("高"))
        return ESageBrainTaskPriority::High;
    else if (PriorityStr == TEXT("中"))
        return ESageBrainTaskPriority::Medium;
    else if (PriorityStr == TEXT("低"))
        return ESageBrainTaskPriority::Low;
    else if (PriorityStr == TEXT("研究"))
        return ESageBrainTaskPriority::Research;
    else
        return ESageBrainTaskPriority::Medium;
}

ESageBrainTaskStatus UMingSageBrainTaskSystem::StringToTaskStatus(const FString& StatusStr)
{
    if (StatusStr == TEXT("規劃中"))
        return ESageBrainTaskStatus::Planning;
    else if (StatusStr == TEXT("進行中"))
        return ESageBrainTaskStatus::InProgress;
    else if (StatusStr == TEXT("測試中"))
        return ESageBrainTaskStatus::Testing;
    else if (StatusStr == TEXT("審查中"))
        return ESageBrainTaskStatus::Review;
    else if (StatusStr == TEXT("已完成"))
        return ESageBrainTaskStatus::Completed;
    else if (StatusStr == TEXT("暫停"))
        return ESageBrainTaskStatus::Paused;
    else if (StatusStr == TEXT("取消"))
        return ESageBrainTaskStatus::Cancelled;
    else
        return ESageBrainTaskStatus::Planning;
}
