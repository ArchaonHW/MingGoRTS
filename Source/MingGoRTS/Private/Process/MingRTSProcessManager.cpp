#include "MingRTSProcessManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingRTSProcessManager::UMingRTSProcessManager()
{
    SchedulerState = ESchedulerState::Idle;
    SchedulingAlgorithm = ESchedulingAlgorithm::RoundRobin;
    TimeSlice = 100; // 100ms
}

bool UMingRTSProcessManager::InitializeProcessManager(UMingRTSKernel* InKernel)
{
    if (!InKernel)
    {
        UE_LOG(LogTemp, Error, TEXT("Kernel reference is null"));
        return false;
    }

    Kernel = InKernel;

    if (!InitializeScheduler())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize scheduler"));
        return false;
    }

    SchedulerState = ESchedulerState::Running;
    UE_LOG(LogTemp, Log, TEXT("Process Manager initialized successfully"));
    
    // 啟動調度循環
    StartScheduling();
    
    return true;
}

void UMingRTSProcessManager::ShutdownProcessManager()
{
    if (SchedulerState == ESchedulerState::Running)
    {
        SchedulerState = ESchedulerState::Idle;
        StopScheduling();
        
        CleanupQueues();
        CleanupStatistics();
        
        UE_LOG(LogTemp, Log, TEXT("Process Manager shutdown completed"));
    }
}

FString UMingRTSProcessManager::CreateProcess(const FString& ProcessName, int32 Priority, float BurstTime)
{
    if (!Kernel || SchedulerState != ESchedulerState::Running)
    {
        return TEXT("");
    }

    // 通過內核創建進程
    FString ProcessID = Kernel->CreateProcess(ProcessName, Priority);
    if (ProcessID.IsEmpty())
    {
        return TEXT("");
    }

    // 創建進程節點
    FProcessQueueNode ProcessNode;
    ProcessNode.ProcessID = ProcessID;
    ProcessNode.Priority = Priority;
    ProcessNode.ArrivalTime = FDateTime::Now().GetTicks() / 10000.0f; // 轉換為毫秒
    ProcessNode.BurstTime = BurstTime;
    ProcessNode.TimeSlice = TimeSlice;

    // 添加到進程映射
    {
        FScopeLock Lock(&QueueCriticalSection);
        ProcessNodes.Add(ProcessID, ProcessNode);
    }

    // 添加到就緒隊列
    AddToReadyQueue(ProcessNode);

    UE_LOG(LogTemp, Log, TEXT("Process %s created and added to ready queue"), *ProcessName);
    return ProcessID;
}

bool UMingRTSProcessManager::TerminateProcess(const FString& ProcessID)
{
    if (!Kernel)
    {
        return false;
    }

    // 通過內核終止進程
    bool Success = Kernel->TerminateProcess(ProcessID);
    if (Success)
    {
        // 從隊列中移除
        RemoveFromQueue(ProcessID);
        
        // 如果是當前運行進程，需要調度下一個
        if (CurrentRunningProcess == ProcessID)
        {
            CurrentRunningProcess = TEXT("");
            ScheduleNextProcess();
        }
        
        UE_LOG(LogTemp, Log, TEXT("Process %s terminated"), *ProcessID);
    }

    return Success;
}

bool UMingRTSProcessManager::SuspendProcess(const FString& ProcessID)
{
    if (!Kernel)
    {
        return false;
    }

    bool Success = Kernel->SuspendProcess(ProcessID);
    if (Success)
    {
        // 如果是當前運行進程，需要調度下一個
        if (CurrentRunningProcess == ProcessID)
        {
            CurrentRunningProcess = TEXT("");
            ScheduleNextProcess();
        }
        
        UE_LOG(LogTemp, Log, TEXT("Process %s suspended"), *ProcessID);
    }

    return Success;
}

bool UMingRTSProcessManager::ResumeProcess(const FString& ProcessID)
{
    if (!Kernel)
    {
        return false;
    }

    bool Success = Kernel->ResumeProcess(ProcessID);
    if (Success)
    {
        // 將進程重新加入就緒隊列
        if (const FProcessQueueNode* ProcessNode = ProcessNodes.Find(ProcessID))
        {
            AddToReadyQueue(*ProcessNode);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Process %s resumed"), *ProcessID);
    }

    return Success;
}

bool UMingRTSProcessManager::ChangeProcessPriority(const FString& ProcessID, int32 NewPriority)
{
    FScopeLock Lock(&QueueCriticalSection);
    
    if (FProcessQueueNode* ProcessNode = ProcessNodes.Find(ProcessID))
    {
        int32 OldPriority = ProcessNode->Priority;
        ProcessNode->Priority = NewPriority;
        
        // 如果進程在就緒隊列中，需要重新排序
        if (IsProcessReady(ProcessID))
        {
            // 重新添加到隊列以更新優先級
            RemoveFromQueue(ProcessID);
            AddToReadyQueue(*ProcessNode);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Process %s priority changed from %d to %d"), 
               *ProcessID, OldPriority, NewPriority);
        return true;
    }

    return false;
}

void UMingRTSProcessManager::SetSchedulingAlgorithm(ESchedulingAlgorithm Algorithm)
{
    SchedulingAlgorithm = Algorithm;
    UE_LOG(LogTemp, Log, TEXT("Scheduling algorithm changed to %d"), (int32)Algorithm);
}

void UMingRTSProcessManager::SetTimeSlice(int32 TimeSliceMs)
{
    TimeSlice = TimeSliceMs;
    
    // 更新所有進程節點的時間片
    FScopeLock Lock(&QueueCriticalSection);
    for (auto& ProcessPair : ProcessNodes)
    {
        ProcessPair.Value.TimeSlice = TimeSlice;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Time slice set to %d ms"), TimeSlice);
}

void UMingRTSProcessManager::StartScheduling()
{
    if (SchedulerState == ESchedulerState::Running)
    {
        return;
    }

    SchedulerState = ESchedulerState::Running;
    
    // 啟動調度循環
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            ProcessTimeSlice();
        });
    }
    
    UE_LOG(LogTemp, Log, TEXT("Process scheduling started"));
}

void UMingRTSProcessManager::StopScheduling()
{
    SchedulerState = ESchedulerState::Idle;
    UE_LOG(LogTemp, Log, TEXT("Process scheduling stopped"));
}

void UMingRTSProcessManager::PauseScheduling()
{
    SchedulerState = ESchedulerState::Paused;
    UE_LOG(LogTemp, Log, TEXT("Process scheduling paused"));
}

TArray<FString> UMingRTSProcessManager::GetReadyProcesses() const
{
    FScopeLock Lock(&QueueCriticalSection);
    
    TArray<FString> ReadyProcesses;
    TQueue<FProcessQueueNode> TempQueue = ReadyQueue;
    
    while (!TempQueue.IsEmpty())
    {
        FProcessQueueNode Node;
        if (TempQueue.Dequeue(Node))
        {
            ReadyProcesses.Add(Node.ProcessID);
        }
    }
    
    return ReadyProcesses;
}

TArray<FProcessInfo> UMingRTSProcessManager::GetAllProcesses() const
{
    if (!Kernel)
    {
        return TArray<FProcessInfo>();
    }

    return Kernel->GetAllProcesses();
}

void UMingRTSProcessManager::ScheduleNextProcess()
{
    if (SchedulerState != ESchedulerState::Running)
    {
        return;
    }

    FString NextProcessID;
    
    // 根據調度算法選擇下一個進程
    switch (SchedulingAlgorithm)
    {
        case ESchedulingAlgorithm::FIFO:
            NextProcessID = ScheduleFIFO();
            break;
        
        case ESchedulingAlgorithm::Priority:
            NextProcessID = SchedulePriority();
            break;
        
        case ESchedulingAlgorithm::RoundRobin:
            NextProcessID = ScheduleRoundRobin();
            break;
        
        case ESchedulingAlgorithm::SJF:
            NextProcessID = ScheduleSJF();
            break;
        
        case ESchedulingAlgorithm::MLFQ:
            NextProcessID = ScheduleMLFQ();
            break;
        
        default:
            NextProcessID = ScheduleRoundRobin();
            break;
    }

    if (!NextProcessID.IsEmpty())
    {
        PerformContextSwitch(NextProcessID);
    }
}

void UMingRTSProcessManager::PerformContextSwitch(const FString& NewProcessID)
{
    FString OldProcessID = CurrentRunningProcess;
    CurrentRunningProcess = NewProcessID;

    // 更新統計
    {
        FScopeLock Lock(&StatisticsCriticalSection);
        Statistics.ContextSwitches++;
    }

    // 廣播進程調度事件
    OnProcessScheduled.Broadcast(NewProcessID, SchedulingAlgorithm);

    UE_LOG(LogTemp, Log, TEXT("Context switch: %s -> %s"), 
           OldProcessID.IsEmpty() ? TEXT("None") : *OldProcessID, *NewProcessID);
}

void UMingRTSProcessManager::UpdateStatistics()
{
    FScopeLock Lock(&StatisticsCriticalSection);
    
    // 計算平均等待時間和周轉時間
    float TotalWaitingTime = 0.0f;
    float TotalTurnaroundTime = 0.0f;
    int32 ProcessCount = 0;

    for (const auto& ProcessPair : ProcessNodes)
    {
        const FString& ProcessID = ProcessPair.Key;
        TotalWaitingTime += CalculateWaitingTime(ProcessID);
        TotalTurnaroundTime += CalculateTurnaroundTime(ProcessID);
        ProcessCount++;
    }

    if (ProcessCount > 0)
    {
        Statistics.AverageWaitingTime = TotalWaitingTime / ProcessCount;
        Statistics.AverageTurnaroundTime = TotalTurnaroundTime / ProcessCount;
    }

    Statistics.TotalProcessesScheduled = ProcessCount;
    
    // 模擬CPU利用率
    Statistics.CPUUtilization = CurrentRunningProcess.IsEmpty() ? 0.0f : 85.0f;
}

void UMingRTSProcessManager::ProcessTimeSlice()
{
    if (SchedulerState != ESchedulingState::Running)
    {
        return;
    }

    // 如果沒有當前運行進程，調度下一個
    if (CurrentRunningProcess.IsEmpty())
    {
        ScheduleNextProcess();
    }
    else
    {
        // 檢查當前進程的時間片是否用完
        if (const FProcessQueueNode* ProcessNode = ProcessNodes.Find(CurrentRunningProcess))
        {
            // 簡單的時間片輪轉邏輯
            // 在實際實現中，這裡應該跟蹤進程已運行時間
            
            // 模擬時間片用完，重新調度
            AddToReadyQueue(*ProcessNode);
            ScheduleNextProcess();
        }
    }

    // 更新統計信息
    UpdateStatistics();

    // 繼續下一個時間片
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            ProcessTimeSlice();
        });
    }
}

FString UMingRTSProcessManager::ScheduleFIFO()
{
    FScopeLock Lock(&QueueCriticalSection);
    
    FProcessQueueNode Node;
    if (ReadyQueue.Dequeue(Node))
    {
        return Node.ProcessID;
    }
    
    return TEXT("");
}

FString UMingRTSProcessManager::SchedulePriority()
{
    FScopeLock Lock(&QueueCriticalSection);
    
    FString HighestPriorityProcess;
    int32 HighestPriority = INT_MAX;
    
    TQueue<FProcessQueueNode> TempQueue = ReadyQueue;
    ReadyQueue.Empty();
    
    while (!TempQueue.IsEmpty())
    {
        FProcessQueueNode Node;
        if (TempQueue.Dequeue(Node))
        {
            if (Node.Priority > HighestPriority)
            {
                HighestPriority = Node.Priority;
                HighestPriorityProcess = Node.ProcessID;
            }
            ReadyQueue.Enqueue(Node);
        }
    }
    
    // 如果找到最高優先級進程，將其從隊列中移除
    if (!HighestPriorityProcess.IsEmpty())
    {
        TQueue<FProcessQueueNode> NewTempQueue = ReadyQueue;
        ReadyQueue.Empty();
        
        while (!NewTempQueue.IsEmpty())
        {
            FProcessQueueNode Node;
            if (NewTempQueue.Dequeue(Node))
            {
                if (Node.ProcessID != HighestPriorityProcess)
                {
                    ReadyQueue.Enqueue(Node);
                }
            }
        }
    }
    
    return HighestPriorityProcess;
}

FString UMingRTSProcessManager::ScheduleRoundRobin()
{
    return ScheduleFIFO(); // FIFO就是Round Robin的實現
}

FString UMingRTSProcessManager::ScheduleSJF()
{
    FScopeLock Lock(&QueueCriticalSection);
    
    FString ShortestProcess;
    float ShortestBurstTime = FLT_MAX;
    
    TQueue<FProcessQueueNode> TempQueue = ReadyQueue;
    ReadyQueue.Empty();
    
    while (!TempQueue.IsEmpty())
    {
        FProcessQueueNode Node;
        if (TempQueue.Dequeue(Node))
        {
            if (Node.BurstTime < ShortestBurstTime)
            {
                ShortestBurstTime = Node.BurstTime;
                ShortestProcess = Node.ProcessID;
            }
            ReadyQueue.Enqueue(Node);
        }
    }
    
    // 如果找到最短作業，將其從隊列中移除
    if (!ShortestProcess.IsEmpty())
    {
        TQueue<FProcessQueueNode> NewTempQueue = ReadyQueue;
        ReadyQueue.Empty();
        
        while (!NewTempQueue.IsEmpty())
        {
            FProcessQueueNode Node;
            if (NewTempQueue.Dequeue(Node))
            {
                if (Node.ProcessID != ShortestProcess)
                {
                    ReadyQueue.Enqueue(Node);
                }
            }
        }
    }
    
    return ShortestProcess;
}

FString UMingRTSProcessManager::ScheduleMLFQ()
{
    // 簡化的多級反饋隊列實現
    // 在實際實現中，應該有多個優先級隊列
    return SchedulePriority(); // 使用優先級調度作為簡化實現
}

bool UMingRTSProcessManager::IsProcessReady(const FString& ProcessID) const
{
    TQueue<FProcessQueueNode> TempQueue = ReadyQueue;
    
    while (!TempQueue.IsEmpty())
    {
        FProcessQueueNode Node;
        if (TempQueue.Dequeue(Node))
        {
            if (Node.ProcessID == ProcessID)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool UMingRTSProcessManager::IsProcessWaiting(const FString& ProcessID) const
{
    return WaitingProcesses.Contains(ProcessID);
}

void UMingRTSProcessManager::AddToReadyQueue(const FProcessQueueNode& ProcessNode)
{
    FScopeLock Lock(&QueueCriticalSection);
    ReadyQueue.Enqueue(ProcessNode);
}

void UMingRTSProcessManager::AddToWaitingQueue(const FString& ProcessID, const FProcessQueueNode& ProcessNode)
{
    FScopeLock Lock(&QueueCriticalSection);
    WaitingProcesses.Add(ProcessID, ProcessNode);
}

void UMingRTSProcessManager::RemoveFromQueue(const FString& ProcessID)
{
    FScopeLock Lock(&QueueCriticalSection);
    
    // 從就緒隊列中移除
    TQueue<FProcessQueueNode> TempQueue = ReadyQueue;
    ReadyQueue.Empty();
    
    while (!TempQueue.IsEmpty())
    {
        FProcessQueueNode Node;
        if (TempQueue.Dequeue(Node))
        {
            if (Node.ProcessID != ProcessID)
            {
                ReadyQueue.Enqueue(Node);
            }
        }
    }
    
    // 從等待隊列中移除
    WaitingProcesses.Remove(ProcessID);
    
    // 從進程映射中移除
    ProcessNodes.Remove(ProcessID);
}

float UMingRTSProcessManager::CalculateWaitingTime(const FString& ProcessID) const
{
    if (const FProcessQueueNode* ProcessNode = ProcessNodes.Find(ProcessID))
    {
        float CurrentTime = FDateTime::Now().GetTicks() / 10000.0f;
        return CurrentTime - ProcessNode->ArrivalTime;
    }
    
    return 0.0f;
}

float UMingRTSProcessManager::CalculateTurnaroundTime(const FString& ProcessID) const
{
    // 簡化實現，實際應該考慮完成時間
    return CalculateWaitingTime(ProcessID) + 100.0f; // 假設服務時間為100ms
}

bool UMingRTSProcessManager::InitializeScheduler()
{
    ResetStatistics();
    UE_LOG(LogTemp, Log, TEXT("Scheduler initialized"));
    return true;
}

void UMingRTSProcessManager::ResetStatistics()
{
    FScopeLock Lock(&StatisticsCriticalSection);
    Statistics = FSchedulingStatistics();
}

void UMingRTSProcessManager::CleanupQueues()
{
    FScopeLock Lock(&QueueCriticalSection);
    ReadyQueue.Empty();
    WaitingProcesses.Empty();
    ProcessNodes.Empty();
    CurrentRunningProcess = TEXT("");
}

void UMingRTSProcessManager::CleanupStatistics()
{
    FScopeLock Lock(&StatisticsCriticalSection);
    ResetStatistics();
}
