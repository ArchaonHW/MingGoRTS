#include "MingRTSKernel.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingRTSKernel::UMingRTSKernel()
{
    KernelState = EKernelState::Uninitialized;
    TotalCPUUsage = 0.0f;
    TotalMemoryUsage = 0;
}

bool UMingRTSKernel::InitializeKernel()
{
    if (KernelState != EKernelState::Uninitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Kernel is already initialized or in invalid state"));
        return false;
    }

    KernelState = EKernelState::Initializing;

    // 初始化各個子系統
    if (!InitializeProcessManager())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize process manager"));
        KernelState = EKernelState::Error;
        return false;
    }

    if (!InitializeMemoryManager())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize memory manager"));
        KernelState = EKernelState::Error;
        return false;
    }

    if (!InitializeFileSystem())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize file system"));
        KernelState = EKernelState::Error;
        return false;
    }

    if (!InitializeInterruptSystem())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize interrupt system"));
        KernelState = EKernelState::Error;
        return false;
    }

    KernelState = EKernelState::Running;
    UE_LOG(LogTemp, Log, TEXT("MingRTS Kernel initialized successfully"));

    // 啟動系統調用處理定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            ProcessSystemCalls();
        });
    }

    return true;
}

void UMingRTSKernel::ShutdownKernel()
{
    if (KernelState == EKernelState::Running)
    {
        KernelState = EKernelState::ShuttingDown;

        // 清理所有進程
        CleanupProcesses();

        // 清理系統服務
        CleanupServices();

        // 清理中斷處理器
        CleanupInterrupts();

        KernelState = EKernelState::Uninitialized;
        UE_LOG(LogTemp, Log, TEXT("MingRTS Kernel shutdown completed"));
    }
}

FString UMingRTSKernel::ExecuteSystemCall(const FSystemCallParams& Params)
{
    if (KernelState != EKernelState::Running)
    {
        return TEXT("Kernel not running");
    }

    // 將系統調用加入隊列
    SystemCallQueue.Enqueue(Params);

    return TEXT("System call queued");
}

bool UMingRTSKernel::RegisterSystemService(const FString& ServiceName, UObject* Service)
{
    if (!Service || ServiceName.IsEmpty())
    {
        return false;
    }

    FScopeLock Lock(&ServiceCriticalSection);
    
    if (SystemServices.Contains(ServiceName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service %s already registered"), *ServiceName);
        return false;
    }

    SystemServices.Add(ServiceName, Service);
    UE_LOG(LogTemp, Log, TEXT("Service %s registered successfully"), *ServiceName);
    return true;
}

void UMingRTSKernel::UnregisterSystemService(const FString& ServiceName)
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    if (SystemServices.Contains(ServiceName))
    {
        SystemServices.Remove(ServiceName);
        UE_LOG(LogTemp, Log, TEXT("Service %s unregistered"), *ServiceName);
    }
}

FString UMingRTSKernel::CreateProcess(const FString& ProcessName, int32 Priority)
{
    if (KernelState != EKernelState::Running)
    {
        return TEXT("");
    }

    FScopeLock Lock(&ProcessCriticalSection);

    FString ProcessID = GenerateProcessID();
    
    FProcessInfo ProcessInfo;
    ProcessInfo.ProcessID = ProcessID;
    ProcessInfo.ProcessName = ProcessName;
    ProcessInfo.State = EProcessState::Created;
    ProcessInfo.Priority = Priority;
    ProcessInfo.CreationTime = FDateTime::Now();

    Processes.Add(ProcessID, ProcessInfo);

    UE_LOG(LogTemp, Log, TEXT("Process %s created with ID %s"), *ProcessName, *ProcessID);
    return ProcessID;
}

bool UMingRTSKernel::TerminateProcess(const FString& ProcessID)
{
    if (!IsValidProcessID(ProcessID))
    {
        return false;
    }

    FScopeLock Lock(&ProcessCriticalSection);

    FProcessInfo* ProcessInfo = Processes.Find(ProcessID);
    if (ProcessInfo)
    {
        ProcessInfo->State = EProcessState::Terminated;
        UE_LOG(LogTemp, Log, TEXT("Process %s terminated"), *ProcessID);
        return true;
    }

    return false;
}

bool UMingRTSKernel::SuspendProcess(const FString& ProcessID)
{
    if (!IsValidProcessID(ProcessID))
    {
        return false;
    }

    FScopeLock Lock(&ProcessCriticalSection);

    FProcessInfo* ProcessInfo = Processes.Find(ProcessID);
    if (ProcessInfo && ProcessInfo->State == EProcessState::Running)
    {
        ProcessInfo->State = EProcessState::Suspended;
        UE_LOG(LogTemp, Log, TEXT("Process %s suspended"), *ProcessID);
        return true;
    }

    return false;
}

bool UMingRTSKernel::ResumeProcess(const FString& ProcessID)
{
    if (!IsValidProcessID(ProcessID))
    {
        return false;
    }

    FScopeLock Lock(&ProcessCriticalSection);

    FProcessInfo* ProcessInfo = Processes.Find(ProcessID);
    if (ProcessInfo && ProcessInfo->State == EProcessState::Suspended)
    {
        ProcessInfo->State = EProcessState::Ready;
        UE_LOG(LogTemp, Log, TEXT("Process %s resumed"), *ProcessID);
        return true;
    }

    return false;
}

TArray<FProcessInfo> UMingRTSKernel::GetAllProcesses() const
{
    FScopeLock Lock(&ProcessCriticalSection);
    
    TArray<FProcessInfo> ProcessList;
    for (const auto& ProcessPair : Processes)
    {
        ProcessList.Add(ProcessPair.Value);
    }
    
    return ProcessList;
}

FProcessInfo UMingRTSKernel::GetProcessInfo(const FString& ProcessID) const
{
    FScopeLock Lock(&ProcessCriticalSection);
    
    if (const FProcessInfo* ProcessInfo = Processes.Find(ProcessID))
    {
        return *ProcessInfo;
    }
    
    return FProcessInfo();
}

void UMingRTSKernel::RegisterInterruptHandler(EInterruptType InterruptType, const FOnInterrupt& Handler)
{
    FScopeLock Lock(&InterruptCriticalSection);
    
    InterruptHandlers.Add(InterruptType, Handler);
    UE_LOG(LogTemp, Log, TEXT("Interrupt handler registered for type %d"), (int32)InterruptType);
}

void UMingRTSKernel::TriggerInterrupt(EInterruptType InterruptType)
{
    HandleInterrupt(InterruptType);
}

void UMingRTSKernel::ProcessSystemCalls()
{
    if (KernelState != EKernelState::Running)
    {
        return;
    }

    // 處理系統調用隊列
    while (!SystemCallQueue.IsEmpty())
    {
        FSystemCallParams Params;
        if (SystemCallQueue.Dequeue(Params))
        {
            // 根據調用類型處理
            switch (Params.CallType)
            {
                case ESystemCallType::Process:
                    // 處理進程相關調用
                    break;
                
                case ESystemCallType::Memory:
                    // 處理內存相關調用
                    break;
                
                case ESystemCallType::FileSystem:
                    // 處理文件系統相關調用
                    break;
                
                default:
                    UE_LOG(LogTemp, Warning, TEXT("Unknown system call type: %d"), (int32)Params.CallType);
                    break;
            }
        }
    }

    // 更新進程狀態
    UpdateProcessStates();

    // 繼續下一輪處理
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            ProcessSystemCalls();
        });
    }
}

void UMingRTSKernel::UpdateProcessStates()
{
    FScopeLock Lock(&ProcessCriticalSection);

    // 更新進程狀態和統計信息
    for (auto& ProcessPair : Processes)
    {
        FProcessInfo& ProcessInfo = ProcessPair.Value;
        
        // 模擬CPU使用率變化
        if (ProcessInfo.State == EProcessState::Running)
        {
            ProcessInfo.CPUUsage = FMath::FRand() * 100.0f;
            TotalCPUUsage = (TotalCPUUsage + ProcessInfo.CPUUsage) / 2.0f;
        }
        
        // 模擬內存使用變化
        ProcessInfo.MemoryUsage = FMath::RandRange(1024, 8192);
        TotalMemoryUsage = (TotalMemoryUsage + ProcessInfo.MemoryUsage) / 2;
    }
}

void UMingRTSKernel::HandleInterrupt(EInterruptType InterruptType)
{
    FScopeLock Lock(&InterruptCriticalSection);

    if (const FOnInterrupt* Handler = InterruptHandlers.Find(InterruptType))
    {
        Handler->ExecuteIfBound(InterruptType);
    }

    // 廣播中斷事件
    OnInterrupt.Broadcast(InterruptType);
    
    UE_LOG(LogTemp, Log, TEXT("Interrupt %d handled"), (int32)InterruptType);
}

FString UMingRTSKernel::GenerateProcessID() const
{
    return FString::Printf(TEXT("PROC_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSKernel::IsValidProcessID(const FString& ProcessID) const
{
    return !ProcessID.IsEmpty() && Processes.Contains(ProcessID);
}

bool UMingRTSKernel::InitializeProcessManager()
{
    UE_LOG(LogTemp, Log, TEXT("Process manager initialized"));
    return true;
}

bool UMingRTSKernel::InitializeMemoryManager()
{
    UE_LOG(LogTemp, Log, TEXT("Memory manager initialized"));
    return true;
}

bool UMingRTSKernel::InitializeFileSystem()
{
    UE_LOG(LogTemp, Log, TEXT("File system initialized"));
    return true;
}

bool UMingRTSKernel::InitializeInterruptSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Interrupt system initialized"));
    return true;
}

void UMingRTSKernel::CleanupProcesses()
{
    FScopeLock Lock(&ProcessCriticalSection);
    
    // 終止所有運行中的進程
    for (auto& ProcessPair : Processes)
    {
        if (ProcessPair.Value.State == EProcessState::Running || 
            ProcessPair.Value.State == EProcessState::Ready ||
            ProcessPair.Value.State == EProcessState::Suspended)
        {
            ProcessPair.Value.State = EProcessState::Terminated;
        }
    }
    
    Processes.Empty();
    UE_LOG(LogTemp, Log, TEXT("All processes cleaned up"));
}

void UMingRTSKernel::CleanupServices()
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    SystemServices.Empty();
    UE_LOG(LogTemp, Log, TEXT("All system services cleaned up"));
}

void UMingRTSKernel::CleanupInterrupts()
{
    FScopeLock Lock(&InterruptCriticalSection);
    
    InterruptHandlers.Empty();
    UE_LOG(LogTemp, Log, TEXT("All interrupt handlers cleaned up"));
}
