# 聖者大腦任務單系統 - 使用指南

## 📋 系統概述

基於BMAD API分析的聖者大腦AGI系統任務管理系統，為MingGoRTS項目提供完整的任務規劃、執行和監控功能。

## 🚀 快速開始

### 第一步：包含頭文件
```cpp
#include "TaskSystem/MingSageBrainTaskSystem.h"
```

### 第二步：創建任務系統
```cpp
// 在您的類中添加
UPROPERTY()
TObjectPtr<UMingSageBrainTaskSystem> SageBrainTaskSystem;

// 初始化
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 創建任務系統
    SageBrainTaskSystem = NewObject<UMingSageBrainTaskSystem>();
    SageBrainTaskSystem->InitializeTaskSystem();
    
    // 綁定事件
    SageBrainTaskSystem->OnTaskCreated.AddDynamic(this, &AMyGameMode::OnTaskCreated);
    SageBrainTaskSystem->OnTaskUpdated.AddDynamic(this, &AMyGameMode::OnTaskUpdated);
    SageBrainTaskSystem->OnTaskCompleted.AddDynamic(this, &AMyGameMode::OnTaskCompleted);
}
```

### 第三步：從BMAD分析創建任務
```cpp
void AMyGameMode::CreateTasksFromBMADAnalysis()
{
    if (SageBrainTaskSystem)
    {
        // 從BMAD分析報告創建任務
        bool bSuccess = SageBrainTaskSystem->CreateTasksFromBMADAnalysis(
            TEXT("docs/BMAD_SAGE_BRAIN_ANALYSIS_REPORT.md"));
        
        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully created tasks from BMAD analysis"));
        }
    }
}
```

## 🎯 核心功能使用

### 創建新任務
```cpp
void AMyGameMode::CreateNewTask()
{
    if (SageBrainTaskSystem)
    {
        // 創建核心集成任務
        FString TaskID = SageBrainTaskSystem->CreateSageBrainTask(
            TEXT("聖者大腦核心系統集成"),
            TEXT("將聖者大腦集成到主系統"),
            ESageBrainTaskType::CoreIntegration,
            ESageBrainTaskPriority::Critical
        );
        
        UE_LOG(LogTemp, Log, TEXT("Created task: %s"), *TaskID);
    }
}
```

### 分析任務可行性
```cpp
void AMyGameMode::AnalyzeTask(const FString& TaskID)
{
    if (SageBrainTaskSystem)
    {
        FSageBrainTaskAnalysis Analysis = SageBrainTaskSystem->AnalyzeTask(TaskID);
        
        UE_LOG(LogTemp, Log, TEXT("Task Analysis for %s:"), *TaskID);
        UE_LOG(LogTemp, Log, TEXT("  Feasibility: %.2f"), Analysis.FeasibilityScore);
        UE_LOG(LogTemp, Log, TEXT("  Risk: %.2f"), Analysis.RiskAssessment);
        UE_LOG(LogTemp, Log, TEXT("  Complexity: %.2f"), Analysis.TechnicalComplexity);
        
        for (const FString& Recommendation : Analysis.Recommendations)
        {
            UE_LOG(LogTemp, Log, TEXT("  Recommendation: %s"), *Recommendation);
        }
    }
}
```

### 更新任務進度
```cpp
void AMyGameMode::UpdateTaskProgress(const FString& TaskID, float Progress)
{
    if (SageBrainTaskSystem)
    {
        bool bSuccess = SageBrainTaskSystem->UpdateTaskProgress(TaskID, Progress);
        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("Updated task progress: %s - %.1f%%"), *TaskID, Progress);
        }
    }
}
```

### 獲取任務統計
```cpp
void AMyGameMode::DisplayTaskStatistics()
{
    if (SageBrainTaskSystem)
    {
        FSageBrainTaskStatistics Stats = SageBrainTaskSystem->GetTaskStatistics();
        
        UE_LOG(LogTemp, Log, TEXT("=== 聖者大腦任務統計 ==="));
        UE_LOG(LogTemp, Log, TEXT("總任務數: %d"), Stats.TotalTasks);
        UE_LOG(LogTemp, Log, TEXT("已完成: %d"), Stats.CompletedTasks);
        UE_LOG(LogTemp, Log, TEXT("進行中: %d"), Stats.InProgressTasks);
        UE_LOG(LogTemp, Log, TEXT("總體進度: %.1f%%"), Stats.OverallProgress);
        UE_LOG(LogTemp, Log, TEXT("預算效率: %.1f%%"), Stats.BudgetEfficiency);
    }
}
```

### 獲取高優先級任務
```cpp
void AMyGameMode::ShowHighPriorityTasks()
{
    if (SageBrainTaskSystem)
    {
        TArray<FSageBrainTask> HighPriorityTasks = SageBrainTaskSystem->GetHighPriorityTasks();
        
        UE_LOG(LogTemp, Log, TEXT("=== 高優先級任務 ==="));
        for (const FSageBrainTask& Task : HighPriorityTasks)
        {
            UE_LOG(LogTemp, Log, TEXT("%s - %s (%s) - %.1f%%"), 
                *Task.Title, 
                *SageBrainTaskSystem->TaskPriorityToString(Task.Priority),
                *SageBrainTaskSystem->TaskStatusToString(Task.Status),
                Task.ProgressPercentage);
        }
    }
}
```

### 生成任務報告
```cpp
void AMyGameMode::GenerateTaskReport()
{
    if (SageBrainTaskSystem)
    {
        FString ReportPath = SageBrainTaskSystem->GenerateTaskReport();
        UE_LOG(LogTemp, Log, TEXT("Task report generated: %s"), *ReportPath);
        
        // 可以在瀏覽器中打開報告
        FPlatformProcess::LaunchURL(*ReportPath);
    }
}
```

## 🔧 事件處理

### 任務事件處理
```cpp
// 任務創建事件
UFUNCTION()
void AMyGameMode::OnTaskCreated(const FString& TaskID)
{
    UE_LOG(LogTemp, Log, TEXT("Task created: %s"), *TaskID);
    
    // 可以在這裡添加通知、UI更新等邏輯
    ShowTaskNotification(TaskID, TEXT("新任務已創建"));
}

// 任務更新事件
UFUNCTION()
void AMyGameMode::OnTaskUpdated(const FString& TaskID, ESageBrainTaskStatus NewStatus)
{
    UE_LOG(LogTemp, Log, TEXT("Task updated: %s - %s"), 
        *TaskID, *SageBrainTaskSystem->TaskStatusToString(NewStatus));
    
    // 更新UI顯示
    UpdateTaskUI(TaskID, NewStatus);
}

// 任務完成事件
UFUNCTION()
void AMyGameMode::OnTaskCompleted(const FString& TaskID)
{
    UE_LOG(LogTemp, Log, TEXT("Task completed: %s"), *TaskID);
    
    // 慶祝任務完成
    CelebrateTaskCompletion(TaskID);
    
    // 檢查是否有新任務可以開始
    CheckForNewTasks();
}
```

## 📊 任務類型說明

### 核心任務類型
- **CoreIntegration** - 核心集成任務
- **ThinkingSystem** - 思考系統任務
- **LearningAlgorithm** - 學習算法任務
- **APICoordination** - API協調任務
- **SelfAwareness** - 自我意識任務
- **PhilosophySystem** - 哲學系統任務
- **ConsciousnessEvolution** - 意識進化任務
- **TestingSystem** - 測試系統任務
- **PerformanceOptimization** - 性能優化任務
- **Documentation** - 文檔編寫任務

### 優先級說明
- **Critical** - 關鍵任務，立即執行
- **High** - 高優先級，優先執行
- **Medium** - 中優先級，正常執行
- **Low** - 低優先級，後續執行
- **Research** - 研究階段，需要評估

### 狀態說明
- **Planning** - 規劃中
- **InProgress** - 進行中
- **Testing** - 測試中
- **Review** - 審查中
- **Completed** - 已完成
- **Paused** - 暫停
- **Cancelled** - 取消

## 🎯 實際使用場景

### 場景1：項目啟動
```cpp
void AMyGameMode::StartSageBrainProject()
{
    if (SageBrainTaskSystem)
    {
        // 從BMAD分析創建任務
        SageBrainTaskSystem->CreateTasksFromBMADAnalysis(
            TEXT("docs/BMAD_SAGE_BRAIN_ANALYSIS_REPORT.md"));
        
        // 顯示任務統計
        DisplayTaskStatistics();
        
        // 顯示高優先級任務
        ShowHighPriorityTasks();
        
        // 生成初始報告
        GenerateTaskReport();
    }
}
```

### 場景2：任務監控
```cpp
void AMyGameMode::MonitorTasks()
{
    if (SageBrainTaskSystem)
    {
        // 獲取逾期任務
        TArray<FSageBrainTask> OverdueTasks = SageBrainTaskSystem->GetOverdueTasks();
        
        if (OverdueTasks.Num() > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found %d overdue tasks:"), OverdueTasks.Num());
            for (const FSageBrainTask& Task : OverdueTasks)
            {
                UE_LOG(LogTemp, Warning, TEXT("  %s - %s"), *Task.Title, *Task.TargetCompletion.ToString());
            }
        }
        
        // 獲取任務統計
        FSageBrainTaskStatistics Stats = SageBrainTaskSystem->GetTaskStatistics();
        
        // 如果進度低於預期，觸發優化
        if (Stats.OverallProgress < 50.0f)
        {
            SageBrainTaskSystem->OptimizeTaskSchedule();
        }
    }
}
```

### 場景3：任務完成處理
```cpp
void AMyGameMode::HandleTaskCompletion(const FString& TaskID)
{
    if (SageBrainTaskSystem)
    {
        // 分析任務完成質量
        FSageBrainTaskAnalysis Analysis = SageBrainTaskSystem->AnalyzeTask(TaskID);
        
        // 如果完成質量高，給予獎勵
        if (Analysis.FeasibilityScore > 0.8f)
        {
            AwardTeamBonus(TaskID);
        }
        
        // 檢查依賴任務
        TArray<FSageBrainTask> AllTasks = SageBrainTaskSystem->GetAllTasks();
        for (const FSageBrainTask& Task : AllTasks)
        {
            if (Task.Dependencies.Contains(TaskID) && Task.Status == ESageBrainTaskStatus::Planning)
            {
                // 激活等待的任務
                SageBrainTaskSystem->UpdateTaskStatus(Task.TaskID, ESageBrainTaskStatus::InProgress);
            }
        }
        
        // 更新統計
        DisplayTaskStatistics();
    }
}
```

## 🔧 高級功能

### 自動任務優化
```cpp
void AMyGameMode::EnableAutoOptimization()
{
    if (SageBrainTaskSystem)
    {
        // 啟用自動優化
        SageBrainTaskSystem->OptimizeTaskSchedule();
        
        // 同步BMAD系統
        SageBrainTaskSystem->SyncWithBMADSystem();
        
        UE_LOG(LogTemp, Log, TEXT("Auto optimization enabled"));
    }
}
```

### 任務數據導出
```cpp
void AMyGameMode::ExportTaskData()
{
    if (SageBrainTaskSystem)
    {
        FString ExportPath = SageBrainTaskSystem->ExportTaskData();
        UE_LOG(LogTemp, Log, TEXT("Task data exported: %s"), *ExportPath);
        
        // 可以將數據發送到其他系統
        SendDataToAnalytics(ExportPath);
    }
}
```

### 批量任務操作
```cpp
void AMyGameMode::BatchUpdateTasks()
{
    if (SageBrainTaskSystem)
    {
        TArray<FSageBrainTask> AllTasks = SageBrainTaskSystem->GetAllTasks();
        
        for (FSageBrainTask& Task : AllTasks)
        {
            // 批量更新進度
            if (Task.Status == ESageBrainTaskStatus::InProgress)
            {
                // 模擬進度更新
                float NewProgress = FMath::Clamp(Task.ProgressPercentage + FMath::RandRange(5.0f, 15.0f), 0.0f, 100.0f);
                SageBrainTaskSystem->UpdateTaskProgress(Task.TaskID, NewProgress);
            }
        }
    }
}
```

## 📈 性能監控

### 監控系統性能
```cpp
void AMyGameMode::MonitorSystemPerformance()
{
    if (SageBrainTaskSystem)
    {
        FSageBrainTaskStatistics Stats = SageBrainTaskSystem->GetTaskStatistics();
        
        // 檢查性能指標
        if (Stats.OverallProgress < 30.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Project progress is low: %.1f%%"), Stats.OverallProgress);
        }
        
        if (Stats.BudgetEfficiency > 120.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Budget overruns detected: %.1f%%"), Stats.BudgetEfficiency);
        }
        
        if (Stats.InProgressTasks > 10)
        {
            UE_LOG(LogTemp, Warning, TEXT("Too many concurrent tasks: %d"), Stats.InProgressTasks);
        }
    }
}
```

### 生成性能報告
```cpp
void AMyGameMode::GeneratePerformanceReport()
{
    if (SageBrainTaskSystem)
    {
        // 生成任務報告
        FString TaskReportPath = SageBrainTaskSystem->GenerateTaskReport();
        
        // 生成性能分析
        FString PerformanceReport = GenerateCustomPerformanceReport();
        
        // 合併報告
        FString CombinedReport = CombineReports(TaskReportPath, PerformanceReport);
        
        UE_LOG(LogTemp, Log, TEXT("Performance report generated"));
    }
}
```

## 🎨 UI集成

### 任務列表UI
```cpp
void AMyGameMode::UpdateTaskListUI()
{
    if (SageBrainTaskSystem)
    {
        TArray<FSageBrainTask> AllTasks = SageBrainTaskSystem->GetAllTasks();
        
        // 更新UI任務列表
        for (const FSageBrainTask& Task : AllTasks)
        {
            UpdateTaskListItem(Task);
        }
        
        // 更新統計信息
        FSageBrainTaskStatistics Stats = SageBrainTaskSystem->GetTaskStatistics();
        UpdateStatisticsUI(Stats);
    }
}
```

### 任務詳情UI
```cpp
void AMyGameMode::ShowTaskDetails(const FString& TaskID)
{
    if (SageBrainTaskSystem)
    {
        TArray<FSageBrainTask> AllTasks = SageBrainTaskSystem->GetAllTasks();
        
        for (const FSageBrainTask& Task : AllTasks)
        {
            if (Task.TaskID == TaskID)
            {
                // 顯示任務詳情
                ShowTaskDetailsDialog(Task);
                
                // 顯示任務分析
                FSageBrainTaskAnalysis Analysis = SageBrainTaskSystem->AnalyzeTask(TaskID);
                ShowTaskAnalysisDialog(Analysis);
                
                break;
            }
        }
    }
}
```

## 🔍 故障排除

### 常見問題解決

#### 問題1：任務創建失敗
```cpp
void AMyGameMode::DebugTaskCreation()
{
    if (!SageBrainTaskSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("SageBrainTaskSystem is null"));
        return;
    }
    
    // 檢查系統是否已初始化
    if (TaskStorage.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Task system not initialized, initializing now..."));
        SageBrainTaskSystem->InitializeTaskSystem();
    }
}
```

#### 問題2：任務進度不更新
```cpp
void AMyGameMode::DebugTaskProgress()
{
    if (SageBrainTaskSystem)
    {
        TArray<FSageBrainTask> AllTasks = SageBrainTaskSystem->GetAllTasks();
        
        for (const FSageBrainTask& Task : AllTasks)
        {
            UE_LOG(LogTemp, Log, TEXT("Task %s: Status=%s, Progress=%.1f%%"), 
                *Task.Title,
                *SageBrainTaskSystem->TaskStatusToString(Task.Status),
                Task.ProgressPercentage);
        }
    }
}
```

#### 問題3：事件不觸發
```cpp
void AMyGameMode::DebugEventBinding()
{
    if (SageBrainTaskSystem)
    {
        // 檢查事件是否正確綁定
        UE_LOG(LogTemp, Log, TEXT("Event binding check:"));
        UE_LOG(LogTemp, Log, TEXT("  OnTaskCreated bound: %s"), 
            SageBrainTaskSystem->OnTaskCreated.IsBound() ? TEXT("Yes") : TEXT("No"));
        UE_LOG(LogTemp, Log, TEXT("  OnTaskUpdated bound: %s"), 
            SageBrainTaskSystem->OnTaskUpdated.IsBound() ? TEXT("Yes") : TEXT("No"));
        UE_LOG(LogTemp, Log, TEXT("  OnTaskCompleted bound: %s"), 
            SageBrainTaskSystem->OnTaskCompleted.IsBound() ? TEXT("Yes") : TEXT("No"));
    }
}
```

## 📚 最佳實踐

### 1. 任務創建最佳實踐
- 使用清晰的任務標題和描述
- 選擇合適的任務類型和優先級
- 設置合理的工期和預算
- 定義明確的目標和里程碑

### 2. 進度管理最佳實踐
- 定期更新任務進度
- 監控逾期任務
- 及時處理風險和問題
- 保持任務依賴關係清晰

### 3. 系統集成最佳實踐
- 正確綁定所有事件
- 定期同步BMAD系統
- 啟用自動優化功能
- 定期導出和分析數據

### 4. 性能優化最佳實踐
- 監控系統性能指標
- 避免同時進行太多任務
- 定期清理過期任務
- 優化任務執行順序

## 🎯 總結

聖者大腦任務單系統為MingGoRTS項目提供了：

- **完整的任務管理** - 從創建到完成的完整生命周期
- **智能分析功能** - 可行性評估和風險分析
- **自動化優化** - 任務調度和資源分配
- **實時監控** - 進度跟踪和性能監控
- **靈活的集成** - 與BMAD系統無縫集成
- **豐富的事件** - 完整的事件系統支持

通過這個系統，您可以高效地管理聖者大腦AGI系統的開發過程，確保項目按時、按質、按預算完成。

**🧠 立即開始您的聖者大腦任務管理之旅！**
