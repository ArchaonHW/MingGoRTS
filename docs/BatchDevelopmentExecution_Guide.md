# MingGoRTS 批量開發執行指南

## 快速開始

### 1. 初始化批次開發系統

```cpp
#include "TaskSystem/MingTaskBatchDevelopment.h"

// 創建管理器
UMingTaskBatchManager* Manager = NewObject<UMingTaskBatchManager>();
Manager->InitializeBatchSystem();

// 創建監控器
UMingBatchExecutionMonitor* Monitor = NewObject<UMingBatchExecutionMonitor>();
Monitor->StartMonitoring();
```

### 2. 註冊開發團隊

```cpp
// 核心系統工程師
FMingDeveloper CoreDev;
CoreDev.Name = FMingOptimizedString(TEXT("核心系統工程師"));
CoreDev.MaxConcurrentTasks = 5;
CoreDev.WeeklyHours = 40;
CoreDev.Specialization = FMingOptimizedString(TEXT("Core,Feature,Architecture"));
Manager->RegisterDeveloper(CoreDev);

// 語法修復專員
FMingDeveloper SyntaxDev;
SyntaxDev.Name = FMingOptimizedString(TEXT("語法修復專員"));
SyntaxDev.MaxConcurrentTasks = 8;
SyntaxDev.WeeklyHours = 80; // 高強度
SyntaxDev.Specialization = FMingOptimizedString(TEXT("Syntax,BugFix"));
Manager->RegisterDeveloper(SyntaxDev);

// AI 策略工程師
FMingDeveloper AIDev;
AIDev.Name = FMingOptimizedString(TEXT("AI 策略工程師"));
AIDev.MaxConcurrentTasks = 4;
AIDev.WeeklyHours = 40;
AIDev.Specialization = FMingOptimizedString(TEXT("AI,Strategy,Feature"));
Manager->RegisterDeveloper(AIDev);

// 優化測試工程師
FMingDeveloper OptDev;
OptDev.Name = FMingOptimizedString(TEXT("優化測試工程師"));
OptDev.MaxConcurrentTasks = 6;
OptDev.WeeklyHours = 40;
OptDev.Specialization = FMingOptimizedString(TEXT("Optimization,Testing"));
Manager->RegisterDeveloper(OptDev);
```

### 3. 添加開發任務

```cpp
// Phase 1: 核心系統
FMingDevelopmentTask Task001;
Task001.TaskCode = FMingOptimizedString(TEXT("SAGE-001"));
Task001.Title = FMingOptimizedString(TEXT("創建角色類型系統頭文件"));
Task001.Description = FMingOptimizedString(TEXT("定義聖者/魔王/偽聖者三種角色類型"));
Task001.EstimatedHours = 8;
Task001.Phase = 1;
Task001.Week = 1;
Task001.Priority = ETaskPriority::High;
Task001.TaskType = ETaskType::Feature;
Task001.FilePath = TEXT("Source/MingGoRTS/Public/SageCommand/MingSageCharacterTypes.h");
Manager->AddTask(Task001);

// 更多任務...
// SAGE-002 ~ SAGE-044
```

### 4. 創建批次並分配

```cpp
// 配置批次參數
FMingBatchConfig Config;
Config.BatchSize = 10;
Config.MaxHoursPerBatch = 80; // 一週工時
Config.bGroupByPhase = true;
Config.bRespectDependencies = true;
Config.bBalanceWorkload = true;
Config.MaxTasksPerDeveloper = 5;
Config.bAllowParallelInBatch = true;

// 創建批次
TArray<FMingTaskBatch> Batches = Manager->CreateBatches(Config);

// 自動分配給開發者
Manager->AutoAssignBatches(Config);

// 查看分配結果
FString Report = Manager->GenerateDeveloperWorkloadReport();
UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
```

### 5. 開始執行批次

```cpp
// 啟動第一批
Manager->StartBatch(1); // Batch ID 1

// 在 Tick 中更新進度
void AMyGameMode::Tick(float DeltaTime)
{
    Manager->ProcessBatchQueue();
    
    // 監控健康狀況
    if (!Monitor->IsBatchSystemHealthy())
    {
        TArray<int32> Blocked = Monitor->IdentifyBlockedBatches();
        // 處理阻塞
    }
}
```

### 6. 完成任務

```cpp
// 當開發者完成任務時
void OnTaskCompleted(int32 TaskId, int32 ActualHours)
{
    Manager->CompleteTask(TaskId, ActualHours);
    Monitor->RecordTaskComplete(TaskId, ActualHours);
    
    // 獲取最新進度
    FString Progress = Manager->GenerateProgressReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Progress);
}
```

### 7. 導出報告

```cpp
// 批次報告
FString BatchReport = Manager->GenerateBatchReport();
FFileHelper::SaveStringToFile(BatchReport, TEXT("BatchReport.txt"));

// 工作負載報告
FString WorkloadReport = Manager->GenerateDeveloperWorkloadReport();

// 進度報告
FString ProgressReport = Manager->GenerateProgressReport();

// CSV 格式匯出
Manager->ExportBatchPlan(TEXT("BatchPlan.csv"));
```

---

## 批次執行工作流程

### 標準開發流程

```
初始化系統 → 註冊開發者 → 添加任務 → 創建批次 → 自動分配 → 開始執行 → 監控進度 → 完成報告
```

### 語法修復並行流程

```cpp
// 語法修復可以與功能開發並行執行

// 主線程: 功能開發批次
Manager->StartBatch(1); // Phase 1 核心系統

// 並行線程: 語法修復批次
UMingIntelligentBatchPlanner* Planner = NewObject<UMingIntelligentBatchPlanner>();
TArray<FMingTaskBatch> SyntaxBatches = Planner->GenerateSyntaxFixBatches(
    414,  // 總錯誤數
    25,   // 每批錯誤數
    DeveloperRegistry  // 開發者列表
);

// 分配給語法修復專員
for (auto& Batch : SyntaxBatches)
{
    Manager->AssignBatchToDeveloper(Batch.BatchId, SyntaxDevId);
}
```

---

## 批次監控

### 健康檢查

```cpp
// 定期檢查
void CheckSystemHealth()
{
    FMingBatchExecutionMetrics Metrics = Monitor->GetCurrentMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("完成率: %.1f%%"), Metrics.CompletionPercentage);
    UE_LOG(LogTemp, Log, TEXT("阻塞任務: %d"), Metrics.BlockedTasks);
    UE_LOG(LogTemp, Log, TEXT("實際/預估工時比: %.2f"), Metrics.AverageTaskEfficiency);
    
    // 檢查超期任務
    Monitor->AlertOverdueTasks(40.0f); // 超過 40 小時警告
}
```

### 事件響應

```cpp
// 任務阻塞時
Monitor->RecordTaskBlocked(TaskId, TEXT("等待依賴任務完成"));

// 任務開始時
Monitor->RecordTaskStart(TaskId);

// 任務完成時
Monitor->RecordTaskComplete(TaskId, ActualHours);
```

---

## 批次配置建議

### 小型專案 (< 50 任務)

```cpp
FMingBatchConfig Config;
Config.BatchSize = 5;
Config.MaxHoursPerBatch = 40; // 半週
Config.bGroupByPhase = true;
Config.bBalanceWorkload = true;
```

### 中型專案 (50-200 任務)

```cpp
FMingBatchConfig Config;
Config.BatchSize = 10;
Config.MaxHoursPerBatch = 80; // 一週
Config.bGroupByPhase = true;
Config.bRespectDependencies = true;
```

### 大型專案 (> 200 任務)

```cpp
FMingBatchConfig Config;
Config.BatchSize = 25;
Config.MaxHoursPerBatch = 160; // 兩週
Config.bGroupByPhase = true;
Config.bRespectDependencies = true;
Config.bBalanceWorkload = true;
Config.MaxTasksPerDeveloper = 5;
```

---

## 常見問題

### Q: 如何處理任務依賴？

A: 在添加任務時指定依賴：

```cpp
FMingDevelopmentTask TaskB;
TaskB.DependencyTaskIds.Add(TaskA.TaskId); // 依賴 Task A
Manager->AddTask(TaskB);
```

系統會自動確保 Task A 完成後才開始 Task B。

### Q: 如何重新分配批次？

A: 使用自動分配功能：

```cpp
// 重新平衡工作負載
Manager->AutoAssignBatches(Config);

// 或手動重新分配
Manager->AssignBatchToDeveloper(BatchId, NewDeveloperId);
```

### Q: 如何暫停批次？

A: 批次狀態管理：

```cpp
// 批次會自動跟隨任務狀態
// 要暫停，可以將所有任務設為 Blocked
FMingDevelopmentTask* Task = Manager->GetTask(TaskId);
Task->Status = ETaskStatus::Blocked;

// 恢復時改回 InProgress
Task->Status = ETaskStatus::InProgress;
```

### Q: 如何追蹤實際工時？

A: 完成任務時記錄：

```cpp
int32 ActualHours = 10; // 實際花費 10 小時
Manager->CompleteTask(TaskId, ActualHours);
```

系統會自動計算效率指標。

---

## 整合範例

### 完整啟動範例

```cpp
void ABatchDevelopmentGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 1. 初始化
    BatchManager = NewObject<UMingTaskBatchManager>(this);
    BatchManager->InitializeBatchSystem();
    
    Monitor = NewObject<UMingBatchExecutionMonitor>(this);
    Monitor->StartMonitoring();
    
    // 2. 設置團隊
    SetupDevelopmentTeam();
    
    // 3. 添加任務
    AddAllDevelopmentTasks();
    
    // 4. 創建批次
    FMingBatchConfig Config;
    TArray<FMingTaskBatch> Batches = BatchManager->CreateBatches(Config);
    
    // 5. 分配
    BatchManager->AutoAssignBatches(Config);
    
    // 6. 啟動第一批
    BatchManager->StartBatch(1);
    
    // 7. 啟動定時器
    GetWorld()->GetTimerManager().SetTimer(
        BatchTickTimer,
        this,
        &ABatchDevelopmentGameMode::ProcessBatchTick,
        60.0f, // 每分鐘更新一次
        true
    );
}

void ABatchDevelopmentGameMode::ProcessBatchTick()
{
    // 更新批次隊列
    BatchManager->ProcessBatchQueue();
    
    // 檢查健康狀況
    if (!Monitor->IsBatchSystemHealthy())
    {
        HandleBlockedTasks();
    }
    
    // 輸出進度
    UE_LOG(LogTemp, Log, TEXT("%s"), *BatchManager->GenerateProgressReport());
}

void ABatchDevelopmentGameMode::HandleBlockedTasks()
{
    TArray<int32> Blocked = Monitor->IdentifyBlockedBatches();
    for (int32 BatchId : Blocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("Batch %d is blocked"), BatchId);
        // 可以發送通知給項目經理
    }
}
```

---

## 報告輸出範例

### 進度報告

```
=== 進度報告 ===

按階段進度:
  Phase 1: 8/11 (72.7%)
  Phase 2: 0/16 (0.0%)
  Phase 3: 0/11 (0.0%)
  Phase 4: 0/6 (0.0%)

整體進度: 8/44 (18.2%)

語法修復進度: 50/414 (12.1%)
```

### 批次報告

```
=== 批次開發報告 ===

總批次數: 60
已完成: 5
進行中: 8
待開始: 47

批次詳情:
  [1] Phase1_Batch1 - 3 任務, 36 小時, 狀態: InProgress
  [2] Phase1_Batch2 - 3 任務, 40 小時, 狀態: Assigned
  [3] SYNTAX-001 - 25 錯誤, 35 小時, 狀態: InProgress
  ...
```

### 工作負載報告

```
=== 開發者工作負載報告 ===

[1] 核心系統工程師
  專長: Core,Feature,Architecture
  並行任務上限: 5
  當前任務數: 3
  已完成任務數: 8
  負載: 60.0%

[2] 語法修復專員
  專長: Syntax,BugFix
  並行任務上限: 8
  當前任務數: 8
  已完成任務數: 42
  負載: 100.0%
  ...
```

---

**最後更新**: 2026-03-24
**適用版本**: MingGoRTS v1.0
