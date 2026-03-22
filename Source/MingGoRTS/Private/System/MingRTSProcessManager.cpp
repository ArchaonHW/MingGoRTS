#include "MingRTSProcessManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"

int32 UMingRTSProcessManager::ProcessIDCounter = 0;

UMingRTSProcessManager::UMingRTSProcessManager()
    : MaxProcessCount(100)
    , SchedulingInterval(1.0f)
    , bAutoSchedulingEnabled(true)
{
}

void UMingRTSProcessManager::InitializeProcessManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingRTS Process Manager"));
    
    // 初始化系統資源使用情況
    SystemResourceUsage.Empty();
    SystemResourceUsage.Add(TEXT("TotalCPU"), 0.0f);
    SystemResourceUsage.Add(TEXT("TotalMemory"), 0.0f);
    SystemResourceUsage.Add(TEXT("TotalThreads"), 0.0f);
    
    // 記錄初始化時間
    LastSchedulingTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Process Manager initialized with max %d processes"), MaxProcessCount);
}

FString UMingRTSProcessManager::CreateProcess(const FProcessCreationParams& CreationParams)
{
    // 驗證創建參數
    if (!ValidateProcessCreationParams(CreationParams))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid process creation parameters"));
        return FString();
    }
    
    // 檢查進程數量限制
    if (ActiveProcesses.Num() >= MaxProcessCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum process count reached: %d"), MaxProcessCount);
        return FString();
    }
    
    // 檢查依賴關係
    if (!CheckDependencies(CreationParams.RequiredDependencies))
    {
        UE_LOG(LogTemp, Error, TEXT("Required dependencies not satisfied"));
        return FString();
    }
    
    // 生成進程ID
    FString ProcessID = GenerateProcessID();
    
    // 創建進程信息
    FProcessInfo ProcessInfo;
    ProcessInfo.ProcessID = ProcessID;
    ProcessInfo.ProcessName = CreationParams.ProcessName;
    ProcessInfo.OwnerSystem = CreationParams.OwnerSystem;
    ProcessInfo.State = EProcessState::Created;
    ProcessInfo.Priority = CreationParams.Priority;
    ProcessInfo.CPUUsage = 0.0f;
    ProcessInfo.MemoryUsage = 0.0f;
    ProcessInfo.ThreadCount = 1;
    ProcessInfo.CreationTime = FDateTime::Now();
    ProcessInfo.LastActiveTime = FDateTime::Now();
    ProcessInfo.ExecutionTime = 0.0f;
    ProcessInfo.Dependencies = CreationParams.RequiredDependencies;
    ProcessInfo.ProcessData = CreationParams.InitialData;
    
    // 分配系統資源
    if (!AllocateSystemResources(ProcessID, CreationParams))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to allocate system resources for process: %s"), *ProcessID);
        return FString();
    }
    
    // 添加到活躍進程列表
    ActiveProcesses.Add(ProcessID, ProcessInfo);
    
    // 更新進程狀態為運行中
    UpdateProcessState(ProcessID, EProcessState::Running);
    
    // 觸發進程創建事件
    OnProcessCreated.Broadcast(ProcessID, ProcessInfo);
    
    UE_LOG(LogTemp, Log, TEXT("Process created successfully: %s (%s)"), *ProcessID, *ProcessInfo.ProcessName);
    
    return ProcessID;
}

bool UMingRTSProcessManager::TerminateProcess(const FString& ProcessID)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not found for termination: %s"), *ProcessID);
        return false;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    
    // 檢查進程狀態
    if (ProcessInfo.State == EProcessState::Terminated || ProcessInfo.State == EProcessState::Crashed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Process already terminated: %s"), *ProcessID);
        return false;
    }
    
    // 更新進程狀態
    ProcessInfo.State = EProcessState::Terminated;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    // 釋放系統資源
    ReleaseSystemResources(ProcessID);
    
    // 添加到終止列表
    TerminatedProcesses.Add(ProcessID);
    
    // 觸發進程終止事件
    OnProcessTerminated.Broadcast(ProcessID, true);
    
    UE_LOG(LogTemp, Log, TEXT("Process terminated successfully: %s"), *ProcessID);
    
    return true;
}

bool UMingRTSProcessManager::SuspendProcess(const FString& ProcessID)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not found for suspension: %s"), *ProcessID);
        return false;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    
    if (ProcessInfo.State != EProcessState::Running)
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not running, cannot suspend: %s"), *ProcessID);
        return false;
    }
    
    // 更新進程狀態
    ProcessInfo.State = EProcessState::Suspended;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    // 釋放CPU資源但保留內存
    SystemResourceUsage[TEXT("TotalCPU")] -= ProcessInfo.CPUUsage;
    
    // 觸發進程狀態變化事件
    OnProcessStateChanged.Broadcast(ProcessID, EProcessState::Suspended);
    
    UE_LOG(LogTemp, Log, TEXT("Process suspended: %s"), *ProcessID);
    
    return true;
}

bool UMingRTSProcessManager::ResumeProcess(const FString& ProcessID)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not found for resumption: %s"), *ProcessID);
        return false;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    
    if (ProcessInfo.State != EProcessState::Suspended)
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not suspended, cannot resume: %s"), *ProcessID);
        return false;
    }
    
    // 更新進程狀態
    ProcessInfo.State = EProcessState::Running;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    // 重新分配CPU資源
    ProcessInfo.CPUUsage = FMath::FRandRange(5.0f, 15.0f); // 模擬CPU使用
    SystemResourceUsage[TEXT("TotalCPU")] += ProcessInfo.CPUUsage;
    
    // 觸發進程狀態變化事件
    OnProcessStateChanged.Broadcast(ProcessID, EProcessState::Running);
    
    UE_LOG(LogTemp, Log, TEXT("Process resumed: %s"), *ProcessID);
    
    return true;
}

FProcessInfo UMingRTSProcessManager::GetProcessInfo(const FString& ProcessID) const
{
    if (ActiveProcesses.Contains(ProcessID))
    {
        return ActiveProcesses[ProcessID];
    }
    
    return FProcessInfo(); // 返回空的進程信息
}

TArray<FProcessInfo> UMingRTSProcessManager::GetAllProcesses() const
{
    TArray<FProcessInfo> AllProcesses;
    
    for (const auto& ProcessPair : ActiveProcesses)
    {
        AllProcesses.Add(ProcessPair.Value);
    }
    
    return AllProcesses;
}

TArray<FProcessInfo> UMingRTSProcessManager::GetProcessesByState(EProcessState State) const
{
    TArray<FProcessInfo> FilteredProcesses;
    
    for (const auto& ProcessPair : ActiveProcesses)
    {
        if (ProcessPair.Value.State == State)
        {
            FilteredProcesses.Add(ProcessPair.Value);
        }
    }
    
    return FilteredProcesses;
}

TArray<FProcessInfo> UMingRTSProcessManager::GetProcessesByPriority(EProcessPriority Priority) const
{
    TArray<FProcessInfo> FilteredProcesses;
    
    for (const auto& ProcessPair : ActiveProcesses)
    {
        if (ProcessPair.Value.Priority == Priority)
        {
            FilteredProcesses.Add(ProcessPair.Value);
        }
    }
    
    return FilteredProcesses;
}

TArray<FProcessInfo> UMingRTSProcessManager::GetProcessesByOwner(const FString& OwnerSystem) const
{
    TArray<FProcessInfo> FilteredProcesses;
    
    for (const auto& ProcessPair : ActiveProcesses)
    {
        if (ProcessPair.Value.OwnerSystem == OwnerSystem)
        {
            FilteredProcesses.Add(ProcessPair.Value);
        }
    }
    
    return FilteredProcesses;
}

bool UMingRTSProcessManager::SetProcessPriority(const FString& ProcessID, EProcessPriority NewPriority)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not found for priority change: %s"), *ProcessID);
        return false;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    ProcessInfo.Priority = NewPriority;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Process priority updated: %s -> %s"), 
        *ProcessID, *UEnum::GetDisplayValueAsText(NewPriority).ToString());
    
    return true;
}

bool UMingRTSProcessManager::UpdateProcessState(const FString& ProcessID, EProcessState NewState)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Process not found for state update: %s"), *ProcessID);
        return false;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    EProcessState PreviousState = ProcessInfo.State;
    ProcessInfo.State = NewState;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    // 更新執行時間
    if (NewState == EProcessState::Running && PreviousState != EProcessState::Running)
    {
        ProcessInfo.ExecutionTime += CalculateExecutionTime(ProcessInfo);
    }
    
    // 觸發進程狀態變化事件
    OnProcessStateChanged.Broadcast(ProcessID, NewState);
    
    UE_LOG(LogTemp, Log, TEXT("Process state updated: %s -> %s"), 
        *UEnum::GetDisplayValueAsText(PreviousState).ToString(),
        *UEnum::GetDisplayValueAsText(NewState).ToString());
    
    return true;
}

bool UMingRTSProcessManager::DoesProcessExist(const FString& ProcessID) const
{
    return ActiveProcesses.Contains(ProcessID);
}

TMap<FString, float> UMingRTSProcessManager::GetProcessPerformanceStats(const FString& ProcessID) const
{
    TMap<FString, float> Stats;
    
    if (!ActiveProcesses.Contains(ProcessID))
    {
        return Stats;
    }
    
    const FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    
    Stats.Add(TEXT("CPUUsage"), ProcessInfo.CPUUsage);
    Stats.Add(TEXT("MemoryUsage"), ProcessInfo.MemoryUsage);
    Stats.Add(TEXT("ThreadCount"), static_cast<float>(ProcessInfo.ThreadCount));
    Stats.Add(TEXT("ExecutionTime"), ProcessInfo.ExecutionTime);
    Stats.Add(TEXT("AgeInSeconds"), (FDateTime::Now() - ProcessInfo.CreationTime).GetTotalSeconds());
    
    return Stats;
}

TMap<EProcessState, int32> UMingRTSProcessManager::GetSystemProcessStats() const
{
    TMap<EProcessState, int32> Stats;
    
    // 初始化所有狀態計數器
    Stats.Add(EProcessState::Created, 0);
    Stats.Add(EProcessState::Running, 0);
    Stats.Add(EProcessState::Suspended, 0);
    Stats.Add(EProcessState::Terminated, 0);
    Stats.Add(EProcessState::Crashed, 0);
    
    // 統計各狀態進程數量
    for (const auto& ProcessPair : ActiveProcesses)
    {
        EProcessState State = ProcessPair.Value.State;
        int32* Count = Stats.Find(State);
        if (Count)
        {
            (*Count)++;
        }
    }
    
    return Stats;
}

int32 UMingRTSProcessManager::CleanupTerminatedProcesses()
{
    int32 CleanedCount = 0;
    
    for (int32 i = TerminatedProcesses.Num() - 1; i >= 0; --i)
    {
        const FString& ProcessID = TerminatedProcesses[i];
        
        // 檢查進程是否可以清理（超過5分鐘）
        if (ActiveProcesses.Contains(ProcessID))
        {
            const FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
            FTimespan TimeSinceTermination = FDateTime::Now() - ProcessInfo.LastActiveTime;
            
            if (TimeSinceTermination.GetTotalSeconds() > 300.0f) // 5分鐘
            {
                ActiveProcesses.Remove(ProcessID);
                TerminatedProcesses.RemoveAt(i);
                CleanedCount++;
            }
        }
    }
    
    if (CleanedCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d terminated processes"), CleanedCount);
    }
    
    return CleanedCount;
}

void UMingRTSProcessManager::SetMaxProcessCount(int32 MaxCount)
{
    MaxProcessCount = FMath::Max(1, MaxCount);
    UE_LOG(LogTemp, Log, TEXT("Maximum process count set to: %d"), MaxProcessCount);
}

int32 UMingRTSProcessManager::GetCurrentProcessCount() const
{
    return ActiveProcesses.Num();
}

FString UMingRTSProcessManager::GenerateProcessReport() const
{
    FString Report;
    Report += TEXT("=== 進程管理報告 ===\n\n");
    
    // 基本信息
    Report += FString::Printf(TEXT("報告生成時間: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("當前進程數量: %d / %d\n"), ActiveProcesses.Num(), MaxProcessCount);
    Report += FString::Printf(TEXT("已終止進程數量: %d\n"), TerminatedProcesses.Num());
    Report += FString::Printf(TEXT("自動調度: %s\n\n"), bAutoSchedulingEnabled ? TEXT("啟用") : TEXT("禁用"));
    
    // 系統資源使用情況
    Report += TEXT("=== 系統資源使用 ===\n");
    for (const auto& ResourcePair : SystemResourceUsage)
    {
        Report += FString::Printf(TEXT("%s: %.2f\n"), *ResourcePair.Key, ResourcePair.Value);
    }
    Report += TEXT("\n");
    
    // 進程狀態統計
    Report += TEXT("=== 進程狀態統計 ===\n");
    TMap<EProcessState, int32> StateStats = GetSystemProcessStats();
    for (const auto& StatePair : StateStats)
    {
        FString StateName = UEnum::GetDisplayValueAsText(StatePair.Key).ToString();
        Report += FString::Printf(TEXT("%s: %d\n"), *StateName, StatePair.Value);
    }
    Report += TEXT("\n");
    
    // 活躍進程詳情
    Report += TEXT("=== 活躍進程詳情 ===\n");
    TArray<FProcessInfo> AllProcesses = GetAllProcesses();
    for (const FProcessInfo& ProcessInfo : AllProcesses)
    {
        Report += FString::Printf(TEXT("\n進程ID: %s\n"), *ProcessInfo.ProcessID);
        Report += FString::Printf(TEXT("名稱: %s\n"), *ProcessInfo.ProcessName);
        Report += FString::Printf(TEXT("所有者: %s\n"), *ProcessInfo.OwnerSystem);
        Report += FString::Printf(TEXT("狀態: %s\n"), *UEnum::GetDisplayValueAsText(ProcessInfo.State).ToString());
        Report += FString::Printf(TEXT("優先級: %s\n"), *UEnum::GetDisplayValueAsText(ProcessInfo.Priority).ToString());
        Report += FString::Printf(TEXT("CPU使用率: %.1f%%\n"), ProcessInfo.CPUUsage);
        Report += FString::Printf(TEXT("內存使用: %.1f MB\n"), ProcessInfo.MemoryUsage);
        Report += FString::Printf(TEXT("線程數: %d\n"), ProcessInfo.ThreadCount);
        Report += FString::Printf(TEXT("執行時間: %.2f 秒\n"), ProcessInfo.ExecutionTime);
        Report += FString::Printf(TEXT("創建時間: %s\n"), *ProcessInfo.CreationTime.ToString());
        
        if (ProcessInfo.Dependencies.Num() > 0)
        {
            Report += TEXT("依賴: ");
            for (const FString& Dependency : ProcessInfo.Dependencies)
            {
                Report += FString::Printf(TEXT("%s "), *Dependency);
            }
            Report += TEXT("\n");
        }
    }
    
    return Report;
}

FString UMingRTSProcessManager::GenerateProcessID() const
{
    return FString::Printf(TEXT("PROC_%08d"), ++ProcessIDCounter);
}

bool UMingRTSProcessManager::ValidateProcessCreationParams(const FProcessCreationParams& Params) const
{
    if (Params.ProcessName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Process name cannot be empty"));
        return false;
    }
    
    if (Params.OwnerSystem.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Owner system cannot be empty"));
        return false;
    }
    
    if (Params.MemoryLimit < 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("Memory limit cannot be negative"));
        return false;
    }
    
    if (Params.CPULimit < 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("CPU limit cannot be negative"));
        return false;
    }
    
    if (Params.MaxThreads < 1)
    {
        UE_LOG(LogTemp, Error, TEXT("Max threads must be at least 1"));
        return false;
    }
    
    return true;
}

bool UMingRTSProcessManager::CheckDependencies(const TArray<FString>& RequiredDependencies) const
{
    for (const FString& Dependency : RequiredDependencies)
    {
        bool bDependencyFound = false;
        
        // 檢查依賴是否為活躍進程
        for (const auto& ProcessPair : ActiveProcesses)
        {
            if (ProcessPair.Value.ProcessName == Dependency && 
                (ProcessPair.Value.State == EProcessState::Running || ProcessPair.Value.State == EProcessState::Suspended))
            {
                bDependencyFound = true;
                break;
            }
        }
        
        if (!bDependencyFound)
        {
            UE_LOG(LogTemp, Warning, TEXT("Dependency not found: %s"), *Dependency);
            return false;
        }
    }
    
    return true;
}

bool UMingRTSProcessManager::AllocateSystemResources(const FString& ProcessID, const FProcessCreationParams& Params)
{
    // 模擬資源分配
    float RequiredCPU = FMath::Min(Params.CPULimit, 20.0f); // 最大20% CPU
    float RequiredMemory = FMath::Min(Params.MemoryLimit, 512.0f); // 最大512MB 內存
    
    // 檢查系統資源是否足夠
    float AvailableCPU = 100.0f - SystemResourceUsage[TEXT("TotalCPU")];
    float AvailableMemory = 2048.0f - SystemResourceUsage[TEXT("TotalMemory")]; // 假設2GB總內存
    
    if (AvailableCPU < RequiredCPU || AvailableMemory < RequiredMemory)
    {
        UE_LOG(LogTemp, Error, TEXT("Insufficient system resources for process: %s"), *ProcessID);
        return false;
    }
    
    // 分配資源
    SystemResourceUsage[TEXT("TotalCPU")] += RequiredCPU;
    SystemResourceUsage[TEXT("TotalMemory")] += RequiredMemory;
    SystemResourceUsage[TEXT("TotalThreads")] += Params.MaxThreads;
    
    // 更新進程資源使用情況
    if (ActiveProcesses.Contains(ProcessID))
    {
        FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
        ProcessInfo.CPUUsage = RequiredCPU;
        ProcessInfo.MemoryUsage = RequiredMemory;
        ProcessInfo.ThreadCount = Params.MaxThreads;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Resources allocated for process %s: CPU=%.1f%%, Memory=%.1fMB, Threads=%d"), 
        *ProcessID, RequiredCPU, RequiredMemory, Params.MaxThreads);
    
    return true;
}

void UMingRTSProcessManager::ReleaseSystemResources(const FString& ProcessID)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        return;
    }
    
    const FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    
    // 釋放資源
    SystemResourceUsage[TEXT("TotalCPU")] -= ProcessInfo.CPUUsage;
    SystemResourceUsage[TEXT("TotalMemory")] -= ProcessInfo.MemoryUsage;
    SystemResourceUsage[TEXT("TotalThreads")] -= ProcessInfo.ThreadCount;
    
    // 確保資源使用不為負數
    SystemResourceUsage[TEXT("TotalCPU")] = FMath::Max(0.0f, SystemResourceUsage[TEXT("TotalCPU")]);
    SystemResourceUsage[TEXT("TotalMemory")] = FMath::Max(0.0f, SystemResourceUsage[TEXT("TotalMemory")]);
    SystemResourceUsage[TEXT("TotalThreads")] = FMath::Max(0.0f, SystemResourceUsage[TEXT("TotalThreads")]);
    
    UE_LOG(LogTemp, Log, TEXT("Resources released for process %s: CPU=%.1f%%, Memory=%.1fMB, Threads=%d"), 
        *ProcessID, ProcessInfo.CPUUsage, ProcessInfo.MemoryUsage, ProcessInfo.ThreadCount);
}

void UMingRTSProcessManager::UpdateProcessPerformanceData()
{
    FDateTime CurrentTime = FDateTime::Now();
    
    for (auto& ProcessPair : ActiveProcesses)
    {
        FProcessInfo& ProcessInfo = ProcessPair.Value;
        
        if (ProcessInfo.State == EProcessState::Running)
        {
            // 模擬CPU使用率變化
            ProcessInfo.CPUUsage += FMath::FRandRange(-2.0f, 2.0f);
            ProcessInfo.CPUUsage = FMath::Clamp(ProcessInfo.CPUUsage, 1.0f, 25.0f);
            
            // 模擬內存使用率變化
            ProcessInfo.MemoryUsage += FMath::FRandRange(-5.0f, 5.0f);
            ProcessInfo.MemoryUsage = FMath::Clamp(ProcessInfo.MemoryUsage, 10.0f, 600.0f);
            
            // 更新執行時間
            ProcessInfo.ExecutionTime += (CurrentTime - ProcessInfo.LastActiveTime).GetTotalSeconds();
            ProcessInfo.LastActiveTime = CurrentTime;
        }
    }
}

void UMingRTSProcessManager::PerformProcessScheduling()
{
    if (!bAutoSchedulingEnabled)
    {
        return;
    }
    
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan TimeSinceLastScheduling = CurrentTime - LastSchedulingTime;
    
    if (TimeSinceLastScheduling.GetTotalSeconds() < SchedulingInterval)
    {
        return; // 還沒到調度時間
    }
    
    // 更新進程性能數據
    UpdateProcessPerformanceData();
    
    // 檢查進程健康狀態
    CheckProcessHealth();
    
    // 執行優先級調度
    TArray<FProcessInfo> RunningProcesses = GetProcessesByState(EProcessState::Running);
    
    // 按優先級排序
    RunningProcesses.Sort([](const FProcessInfo& A, const FProcessInfo& B)
    {
        return static_cast<int32>(A.Priority) > static_cast<int32>(B.Priority);
    });
    
    // 分配CPU時間片（簡化實現）
    float TotalCPU = 100.0f;
    for (FProcessInfo& ProcessInfo : RunningProcesses)
    {
        float CPUSlice = 0.0f;
        
        switch (ProcessInfo.Priority)
        {
        case EProcessPriority::Critical:
            CPUSlice = 25.0f;
            break;
        case EProcessPriority::High:
            CPUSlice = 20.0f;
            break;
        case EProcessPriority::Normal:
            CPUSlice = 15.0f;
            break;
        case EProcessPriority::Low:
            CPUSlice = 10.0f;
            break;
        case EProcessPriority::Idle:
            CPUSlice = 5.0f;
            break;
        }
        
        CPUSlice = FMath::Min(CPUSlice, TotalCPU);
        ProcessInfo.CPUUsage = CPUSlice;
        TotalCPU -= CPUSlice;
        
        if (TotalCPU <= 0.0f)
        {
            break;
        }
    }
    
    LastSchedulingTime = CurrentTime;
}

void UMingRTSProcessManager::CheckProcessHealth()
{
    FDateTime CurrentTime = FDateTime::Now();
    
    for (auto& ProcessPair : ActiveProcesses)
    {
        FProcessInfo& ProcessInfo = ProcessPair.Value;
        
        // 檢查進程是否響應
        FTimespan TimeSinceLastActive = CurrentTime - ProcessInfo.LastActiveTime;
        
        if (ProcessInfo.State == EProcessState::Running && TimeSinceLastActive.GetTotalSeconds() > 60.0f)
        {
            // 進程可能無響應，標記為崩潰
            UE_LOG(LogTemp, Warning, TEXT("Process appears unresponsive: %s"), *ProcessInfo.ProcessID);
            HandleProcessCrash(ProcessInfo.ProcessID);
        }
        
        // 檢查內存洩漏
        if (ProcessInfo.MemoryUsage > 800.0f) // 超過800MB
        {
            UE_LOG(LogTemp, Warning, TEXT("Process using excessive memory: %s (%.1f MB)"), 
                *ProcessInfo.ProcessID, ProcessInfo.MemoryUsage);
            
            if (ProcessInfo.MemoryUsage > 1200.0f) // 超過1.2GB，終止進程
            {
                UE_LOG(LogTemp, Error, TEXT("Terminating process due to memory leak: %s"), *ProcessInfo.ProcessID);
                TerminateProcess(ProcessInfo.ProcessID);
            }
        }
    }
}

void UMingRTSProcessManager::HandleProcessCrash(const FString& ProcessID)
{
    if (!ActiveProcesses.Contains(ProcessID))
    {
        return;
    }
    
    FProcessInfo& ProcessInfo = ActiveProcesses[ProcessID];
    ProcessInfo.State = EProcessState::Crashed;
    ProcessInfo.LastActiveTime = FDateTime::Now();
    
    // 釋放系統資源
    ReleaseSystemResources(ProcessID);
    
    // 添加到終止列表
    TerminatedProcesses.Add(ProcessID);
    
    // 觸發進程崩潰事件
    OnProcessCrashed.Broadcast(ProcessID);
    
    UE_LOG(LogTemp, Error, TEXT("Process crashed: %s"), *ProcessID);
}

float UMingRTSProcessManager::CalculateExecutionTime(const FProcessInfo& ProcessInfo) const
{
    if (ProcessInfo.State == EProcessState::Running)
    {
        return (FDateTime::Now() - ProcessInfo.LastActiveTime).GetTotalSeconds();
    }
    
    return 0.0f;
}
