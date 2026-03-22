#include "MingRTSResourceScheduler.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingRTSResourceScheduler::UMingRTSResourceScheduler()
{
    bIsInitialized = false;
    bIsShutdown = false;
    bAutoOptimizationEnabled = true;
}

bool UMingRTSResourceScheduler::InitializeResourceScheduler()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Resource Scheduler already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing Resource Scheduler..."));

    // 初始化資源池
    ResourcePools.Empty();
    
    // CPU資源池
    FResourcePool CPUPool;
    CPUPool.ResourceType = EResourceType::CPU;
    CPUPool.TotalCapacity = 100.0f; // 100% CPU
    CPUPool.AvailableCapacity = 100.0f;
    ResourcePools.Add(EResourceType::CPU, CPUPool);

    // 內存資源池
    FResourcePool MemoryPool;
    MemoryPool.ResourceType = EResourceType::Memory;
    MemoryPool.TotalCapacity = 8192.0f; // 8GB
    MemoryPool.AvailableCapacity = 8192.0f;
    ResourcePools.Add(EResourceType::Memory, MemoryPool);

    // GPU資源池
    FResourcePool GPUPool;
    GPUPool.ResourceType = EResourceType::GPU;
    GPUPool.TotalCapacity = 100.0f; // 100% GPU
    GPUPool.AvailableCapacity = 100.0f;
    ResourcePools.Add(EResourceType::GPU, GPUPool);

    // 網絡資源池
    FResourcePool NetworkPool;
    NetworkPool.ResourceType = EResourceType::Network;
    NetworkPool.TotalCapacity = 1000.0f; // 1000 Mbps
    NetworkPool.AvailableCapacity = 1000.0f;
    ResourcePools.Add(EResourceType::Network, NetworkPool);

    // 存儲資源池
    FResourcePool StoragePool;
    StoragePool.ResourceType = EResourceType::Storage;
    StoragePool.TotalCapacity = 102400.0f; // 100GB
    StoragePool.AvailableCapacity = 102400.0f;
    ResourcePools.Add(EResourceType::Storage, StoragePool);

    // 初始化調度策略
    SchedulingPolicies.Empty();
    SchedulingPolicies.Add(EResourceType::CPU, ESchedulingPolicy::Adaptive);
    SchedulingPolicies.Add(EResourceType::Memory, ESchedulingPolicy::FairShare);
    SchedulingPolicies.Add(EResourceType::GPU, ESchedulingPolicy::Priority);
    SchedulingPolicies.Add(EResourceType::Network, ESchedulingPolicy::FIFO);
    SchedulingPolicies.Add(EResourceType::Storage, ESchedulingPolicy::RoundRobin);

    // 初始化指標
    SchedulingMetrics.Empty();
    for (int32 i = 0; i < 6; ++i)
    {
        EResourceType ResourceType = static_cast<EResourceType>(i);
        FSchedulingMetrics Metrics;
        SchedulingMetrics.Add(ResourceType, Metrics);
        
        // 初始化歷史數據
        ResponseTimeHistory.Add(ResourceType, TArray<float>());
        UtilizationHistory.Add(ResourceType, TArray<float>());
        RoundRobinCounters.Add(ResourceType, 0);
    }

    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        // 每秒更新指標
        World->GetTimerManager().SetTimer(
            MetricsUpdateTimer,
            this,
            &UMingRTSResourceScheduler::UpdateResourceUtilization,
            1.0f,
            true
        );

        // 每5秒檢查超時
        World->GetTimerManager().SetTimer(
            TimeoutCheckTimer,
            this,
            &UMingRTSResourceScheduler::HandleTimeoutRequests,
            5.0f,
            true
        );

        // 每30秒自動優化
        World->GetTimerManager().SetTimer(
            AutoOptimizationTimer,
            this,
            &UMingRTSResourceScheduler::PerformAutoOptimization,
            30.0f,
            true
        );
    }

    bIsInitialized = true;
    bIsShutdown = false;

    UE_LOG(LogTemp, Log, TEXT("Resource Scheduler initialized successfully"));
    return true;
}

void UMingRTSResourceScheduler::ShutdownResourceScheduler()
{
    if (!bIsInitialized || bIsShutdown)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down Resource Scheduler..."));

    // 清除定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MetricsUpdateTimer);
        World->GetTimerManager().ClearTimer(TimeoutCheckTimer);
        World->GetTimerManager().ClearTimer(AutoOptimizationTimer);
    }

    // 釋放所有資源分配
    for (auto& AllocationPair : ResourceAllocations)
    {
        ExecuteResourceRelease(AllocationPair.Key);
    }

    // 清空數據結構
    ResourcePools.Empty();
    ResourceAllocations.Empty();
    PendingRequests.Empty();
    SchedulingPolicies.Empty();
    SchedulingMetrics.Empty();
    ResponseTimeHistory.Empty();
    UtilizationHistory.Empty();
    RoundRobinCounters.Empty();

    bIsShutdown = true;
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("Resource Scheduler shutdown complete"));
}

FString UMingRTSResourceScheduler::RequestResource(const FResourceRequest& Request)
{
    if (!bIsInitialized || bIsShutdown)
    {
        UE_LOG(LogTemp, Error, TEXT("Resource Scheduler not initialized"));
        return TEXT("");
    }

    if (!ValidateResourceRequest(Request))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid resource request"));
        return TEXT("");
    }

    // 生成請求ID
    FString RequestID = GenerateRequestID();
    
    // 創建請求副本並設置ID
    FResourceRequest NewRequest = Request;
    NewRequest.RequestID = RequestID;

    // 添加到待處理隊列
    PendingRequests.Add(NewRequest);

    // 立即處理請求
    ProcessResourceRequest(RequestID);

    return RequestID;
}

bool UMingRTSResourceScheduler::ReleaseResource(const FString& AllocationID)
{
    if (!bIsInitialized || bIsShutdown)
    {
        return false;
    }

    return ExecuteResourceRelease(AllocationID);
}

bool UMingRTSResourceScheduler::AdjustResourceAllocation(const FString& AllocationID, float NewAmount)
{
    if (!bIsInitialized || bIsShutdown)
    {
        return false;
    }

    FResourceAllocation* Allocation = ResourceAllocations.Find(AllocationID);
    if (!Allocation)
    {
        UE_LOG(LogTemp, Warning, TEXT("Allocation not found: %s"), *AllocationID);
        return false;
    }

    EResourceType ResourceType = Allocation->ResourceType;
    FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (!Pool)
    {
        return false;
    }

    // 檢查新分配量是否可用
    float Difference = NewAmount - Allocation->AllocatedAmount;
    if (Difference > 0 && Pool->AvailableCapacity < Difference)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient resources for adjustment"));
        return false;
    }

    // 調整分配
    Pool->AvailableCapacity += (Allocation->AllocatedAmount - NewAmount);
    Pool->UtilizedCapacity += (NewAmount - Allocation->AllocatedAmount);
    
    Allocation->AllocatedAmount = NewAmount;
    Allocation->LastUpdateTime = FDateTime::Now();

    UE_LOG(LogTemp, Log, TEXT("Resource allocation adjusted: %s to %.2f"), *AllocationID, NewAmount);
    return true;
}

FResourceAllocation UMingRTSResourceScheduler::GetResourceAllocation(const FString& AllocationID) const
{
    const FResourceAllocation* Allocation = ResourceAllocations.Find(AllocationID);
    return Allocation ? *Allocation : FResourceAllocation();
}

FResourcePool UMingRTSResourceScheduler::GetResourcePool(EResourceType ResourceType) const
{
    const FResourcePool* Pool = ResourcePools.Find(ResourceType);
    return Pool ? *Pool : FResourcePool();
}

TArray<FResourcePool> UMingRTSResourceScheduler::GetAllResourcePools() const
{
    TArray<FResourcePool> Pools;
    for (const auto& PoolPair : ResourcePools)
    {
        Pools.Add(PoolPair.Value);
    }
    return Pools;
}

void UMingRTSResourceScheduler::SetSchedulingPolicy(EResourceType ResourceType, ESchedulingPolicy Policy)
{
    SchedulingPolicies.Add(ResourceType, Policy);
    UE_LOG(LogTemp, Log, TEXT("Scheduling policy set for resource type %d: %d"), (int32)ResourceType, (int32)Policy);
}

ESchedulingPolicy UMingRTSResourceScheduler::GetSchedulingPolicy(EResourceType ResourceType) const
{
    const ESchedulingPolicy* Policy = SchedulingPolicies.Find(ResourceType);
    return Policy ? *Policy : ESchedulingPolicy::FIFO;
}

bool UMingRTSResourceScheduler::SetResourcePoolCapacity(EResourceType ResourceType, float Capacity)
{
    FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (!Pool)
    {
        return false;
    }

    float OldCapacity = Pool->TotalCapacity;
    Pool->TotalCapacity = Capacity;
    Pool->AvailableCapacity = Capacity - Pool->UtilizedCapacity - Pool->ReservedCapacity;

    UE_LOG(LogTemp, Log, TEXT("Resource pool capacity changed from %.2f to %.2f"), OldCapacity, Capacity);
    return true;
}

bool UMingRTSResourceScheduler::ReserveResource(EResourceType ResourceType, float Amount)
{
    FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (!Pool || Pool->AvailableCapacity < Amount)
    {
        return false;
    }

    Pool->AvailableCapacity -= Amount;
    Pool->ReservedCapacity += Amount;

    UE_LOG(LogTemp, Log, TEXT("Resource reserved: %.2f of type %d"), Amount, (int32)ResourceType);
    return true;
}

bool UMingRTSResourceScheduler::ReleaseReservedResource(EResourceType ResourceType, float Amount)
{
    FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (!Pool || Pool->ReservedCapacity < Amount)
    {
        return false;
    }

    Pool->AvailableCapacity += Amount;
    Pool->ReservedCapacity -= Amount;

    UE_LOG(LogTemp, Log, TEXT("Reserved resource released: %.2f of type %d"), Amount, (int32)ResourceType);
    return true;
}

FSchedulingMetrics UMingRTSResourceScheduler::GetSchedulingMetrics(EResourceType ResourceType) const
{
    const FSchedulingMetrics* Metrics = SchedulingMetrics.Find(ResourceType);
    return Metrics ? *Metrics : FSchedulingMetrics();
}

TMap<EResourceType, FSchedulingMetrics> UMingRTSResourceScheduler::GetAllSchedulingMetrics() const
{
    return SchedulingMetrics;
}

void UMingRTSResourceScheduler::ResetMetrics(EResourceType ResourceType)
{
    FSchedulingMetrics NewMetrics;
    SchedulingMetrics.Add(ResourceType, NewMetrics);
    
    ResponseTimeHistory.Find(ResourceType)->Empty();
    UtilizationHistory.Find(ResourceType)->Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Metrics reset for resource type %d"), (int32)ResourceType);
}

void UMingRTSResourceScheduler::ResetAllMetrics()
{
    for (int32 i = 0; i < 6; ++i)
    {
        EResourceType ResourceType = static_cast<EResourceType>(i);
        ResetMetrics(ResourceType);
    }
}

void UMingRTSResourceScheduler::ForceGarbageCollection()
{
    // 釋放非活動分配
    TArray<FString> InactiveAllocations;
    for (auto& AllocationPair : ResourceAllocations)
    {
        if (!AllocationPair.Value.bIsActive)
        {
            InactiveAllocations.Add(AllocationPair.Key);
        }
    }

    for (const FString& AllocationID : InactiveAllocations)
    {
        ExecuteResourceRelease(AllocationID);
    }

    UE_LOG(LogTemp, Log, TEXT("Garbage collection completed. Released %d inactive allocations"), InactiveAllocations.Num());
}

float UMingRTSResourceScheduler::GetSystemLoad() const
{
    float TotalLoad = 0.0f;
    int32 PoolCount = 0;

    for (const auto& PoolPair : ResourcePools)
    {
        const FResourcePool& Pool = PoolPair.Value;
        if (Pool.TotalCapacity > 0.0f)
        {
            TotalLoad += (Pool.UtilizedCapacity / Pool.TotalCapacity) * 100.0f;
            PoolCount++;
        }
    }

    return PoolCount > 0 ? TotalLoad / PoolCount : 0.0f;
}

float UMingRTSResourceScheduler::GetResourceUtilization(EResourceType ResourceType) const
{
    const FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (!Pool || Pool->TotalCapacity <= 0.0f)
    {
        return 0.0f;
    }

    return (Pool->UtilizedCapacity / Pool->TotalCapacity) * 100.0f;
}

float UMingRTSResourceScheduler::PredictResourceDemand(EResourceType ResourceType, float TimeHorizon)
{
    TArray<float>* History = UtilizationHistory.Find(ResourceType);
    if (!History || History->Num() < 2)
    {
        return GetResourceUtilization(ResourceType);
    }

    // 簡單線性回歸預測
    float SumX = 0.0f, SumY = 0.0f, SumXY = 0.0f, SumX2 = 0.0f;
    int32 N = FMath::Min(History->Num(), 10); // 使用最近10個數據點

    for (int32 i = 0; i < N; ++i)
    {
        float X = (float)i;
        float Y = (*History)[History->Num() - N + i];
        SumX += X;
        SumY += Y;
        SumXY += X * Y;
        SumX2 += X * X;
    }

    float Slope = (N * SumXY - SumX * SumY) / (N * SumX2 - SumX * SumX);
    float Intercept = (SumY - Slope * SumX) / N;

    float PredictedUtilization = Slope * (float)(N + TimeHorizon) + Intercept;
    return FMath::Clamp(PredictedUtilization, 0.0f, 100.0f);
}

void UMingRTSResourceScheduler::OptimizeResourceAllocations()
{
    UE_LOG(LogTemp, Log, TEXT("Starting resource allocation optimization..."));

    // 分析當前分配模式
    TMap<EResourceType, TArray<FString>> LowUtilizationAllocations;
    TMap<EResourceType, TArray<FString>> HighPriorityWaiting;

    for (const auto& AllocationPair : ResourceAllocations)
    {
        const FString& AllocationID = AllocationPair.Key;
        const FResourceAllocation& Allocation = AllocationPair.Value;
        
        float Utilization = Allocation.AllocatedAmount > 0.0f ? 
            (Allocation.UtilizedAmount / Allocation.AllocatedAmount) * 100.0f : 0.0f;

        if (Utilization < 20.0f) // 低利用率
        {
            LowUtilizationAllocations.FindOrAdd(Allocation.ResourceType).Add(AllocationID);
        }
    }

    // 檢查待處理的高優先級請求
    for (const FResourceRequest& Request : PendingRequests)
    {
        if (Request.Priority == EResourcePriority::Critical || Request.Priority == EResourcePriority::High)
        {
            HighPriorityWaiting.FindOrAdd(Request.ResourceType).Add(Request.RequestID);
        }
    }

    // 重新分配低利用率的資源給高優先級請求
    for (auto& LowUtilPair : LowUtilizationAllocations)
    {
        EResourceType ResourceType = LowUtilPair.Key;
        const TArray<FString>* HighPriorityRequests = HighPriorityWaiting.Find(ResourceType);
        
        if (HighPriorityRequests && HighPriorityRequests->Num() > 0)
        {
            for (const FString& AllocationID : LowUtilPair.Value)
            {
                if (HighPriorityRequests->Num() == 0) break;

                // 釋放低利用率分配
                ExecuteResourceRelease(AllocationID);

                // 處理高優先級請求
                const FString& RequestID = (*HighPriorityRequests)[0];
                ProcessResourceRequest(RequestID);
                HighPriorityWaiting.Find(ResourceType)->RemoveAt(0);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Resource allocation optimization completed"));
}

void UMingRTSResourceScheduler::SetAutoOptimizationEnabled(bool bEnabled)
{
    bAutoOptimizationEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto optimization %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingRTSResourceScheduler::IsAutoOptimizationEnabled() const
{
    return bAutoOptimizationEnabled;
}

void UMingRTSResourceScheduler::ProcessResourceRequest(const FString& RequestID)
{
    // 找到對應的請求
    FResourceRequest* Request = nullptr;
    for (FResourceRequest& PendingRequest : PendingRequests)
    {
        if (PendingRequest.RequestID == RequestID)
        {
            Request = &PendingRequest;
            break;
        }
    }

    if (!Request)
    {
        UE_LOG(LogTemp, Warning, TEXT("Request not found: %s"), *RequestID);
        return;
    }

    EResourceType ResourceType = Request->ResourceType;
    ESchedulingPolicy Policy = GetSchedulingPolicy(ResourceType);

    // 根據調度策略處理請求
    TArray<FResourceRequest> TypeSpecificRequests;
    for (const FResourceRequest& PendingRequest : PendingRequests)
    {
        if (PendingRequest.ResourceType == ResourceType)
        {
            TypeSpecificRequests.Add(PendingRequest);
        }
    }

    FString AllocationID = ExecuteSchedulingAlgorithm(ResourceType, TypeSpecificRequests);
    
    if (!AllocationID.IsEmpty())
    {
        // 從待處理隊列中移除
        PendingRequests.RemoveAll([RequestID](const FResourceRequest& Req) { return Req.RequestID == RequestID; });
    }
}

bool UMingRTSResourceScheduler::CheckResourceAvailability(EResourceType ResourceType, float Amount)
{
    const FResourcePool* Pool = ResourcePools.Find(ResourceType);
    return Pool && Pool->AvailableCapacity >= Amount;
}

FString UMingRTSResourceScheduler::ExecuteResourceAllocation(const FResourceRequest& Request)
{
    if (!CheckResourceAvailability(Request.ResourceType, Request.RequiredAmount))
    {
        return TEXT("");
    }

    FResourcePool* Pool = ResourcePools.Find(Request.ResourceType);
    if (!Pool)
    {
        return TEXT("");
    }

    // 創建分配
    FResourceAllocation Allocation;
    Allocation.AllocationID = GenerateAllocationID();
    Allocation.RequestID = Request.RequestID;
    Allocation.ResourceType = Request.ResourceType;
    Allocation.AllocatedAmount = Request.RequiredAmount;
    Allocation.UtilizedAmount = 0.0f;
    Allocation.AllocationTime = FDateTime::Now();
    Allocation.LastUpdateTime = FDateTime::Now();
    Allocation.bIsActive = true;

    // 更新資源池
    Pool->AvailableCapacity -= Request.RequiredAmount;
    Pool->UtilizedCapacity += Request.RequiredAmount;
    Pool->ActiveAllocations++;

    // 添加到分配列表
    ResourceAllocations.Add(Allocation.AllocationID, Allocation);

    // 記錄指標
    FSchedulingMetrics* Metrics = SchedulingMetrics.Find(Request.ResourceType);
    if (Metrics)
    {
        Metrics->TotalRequestsProcessed++;
        Metrics->SuccessfulAllocations++;
        
        float ResponseTime = (FDateTime::Now() - Request.RequestTime).GetTotalSeconds();
        TArray<float>* ResponseHistory = ResponseTimeHistory.Find(Request.ResourceType);
        if (ResponseHistory)
        {
            ResponseHistory->Add(ResponseTime);
            if (ResponseHistory->Num() > 100)
            {
                ResponseHistory->RemoveAt(0);
            }
        }
    }

    // 廣播事件
    OnResourceAllocated.Broadcast(Allocation);

    UE_LOG(LogTemp, Log, TEXT("Resource allocated: %s (%.2f)"), *Allocation.AllocationID, Request.RequiredAmount);
    return Allocation.AllocationID;
}

bool UMingRTSResourceScheduler::ExecuteResourceRelease(const FString& AllocationID)
{
    FResourceAllocation* Allocation = ResourceAllocations.Find(AllocationID);
    if (!Allocation)
    {
        return false;
    }

    FResourcePool* Pool = ResourcePools.Find(Allocation->ResourceType);
    if (!Pool)
    {
        return false;
    }

    // 更新資源池
    Pool->AvailableCapacity += Allocation->AllocatedAmount;
    Pool->UtilizedCapacity -= Allocation->AllocatedAmount;
    Pool->ActiveAllocations--;

    // 移除分配
    ResourceAllocations.Remove(AllocationID);

    // 廣播事件
    OnResourceReleased.Broadcast(AllocationID);

    UE_LOG(LogTemp, Log, TEXT("Resource released: %s"), *AllocationID);
    return true;
}

void UMingRTSResourceScheduler::UpdateResourceUtilization()
{
    for (auto& PoolPair : ResourcePools)
    {
        EResourceType ResourceType = PoolPair.Key;
        FResourcePool& Pool = PoolPair.Value;

        // 計算當前利用率
        float CurrentUtilization = Pool.TotalCapacity > 0.0f ? 
            (Pool.UtilizedCapacity / Pool.TotalCapacity) * 100.0f : 0.0f;

        // 添加到歷史記錄
        TArray<float>* History = UtilizationHistory.Find(ResourceType);
        if (History)
        {
            History->Add(CurrentUtilization);
            if (History->Num() > 100)
            {
                History->RemoveAt(0);
            }
        }

        // 計算指標
        CalculateSchedulingMetrics(ResourceType);

        // 廣播利用率變化
        OnResourceUtilizationChanged.Broadcast(ResourceType, CurrentUtilization);
    }
}

void UMingRTSResourceScheduler::HandleTimeoutRequests()
{
    FDateTime CurrentTime = FDateTime::Now();
    TArray<FString> TimedOutRequests;

    for (const FResourceRequest& Request : PendingRequests)
    {
        float ElapsedTime = (CurrentTime - Request.RequestTime).GetTotalSeconds();
        if (ElapsedTime > Request.TimeoutSeconds)
        {
            TimedOutRequests.Add(Request.RequestID);
        }
    }

    for (const FString& RequestID : TimedOutRequests)
    {
        PendingRequests.RemoveAll([RequestID](const FResourceRequest& Req) { return Req.RequestID == RequestID; });
        OnResourceRequestFailed.Broadcast(RequestID);
        
        // 記錄超時
        for (auto& MetricsPair : SchedulingMetrics)
        {
            MetricsPair.Value.Timeouts++;
        }
    }

    if (TimedOutRequests.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Handled %d timed out requests"), TimedOutRequests.Num());
    }
}

FString UMingRTSResourceScheduler::ExecuteSchedulingAlgorithm(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    ESchedulingPolicy Policy = GetSchedulingPolicy(ResourceType);

    switch (Policy)
    {
    case ESchedulingPolicy::FIFO:
        return ExecuteFIFOScheduling(ResourceType, PendingRequests);
    case ESchedulingPolicy::Priority:
        return ExecutePriorityScheduling(ResourceType, PendingRequests);
    case ESchedulingPolicy::RoundRobin:
        return ExecuteRoundRobinScheduling(ResourceType, PendingRequests);
    case ESchedulingPolicy::FairShare:
        return ExecuteFairShareScheduling(ResourceType, PendingRequests);
    case ESchedulingPolicy::RealTime:
        return ExecuteRealTimeScheduling(ResourceType, PendingRequests);
    case ESchedulingPolicy::Adaptive:
        return ExecuteAdaptiveScheduling(ResourceType, PendingRequests);
    default:
        return ExecuteFIFOScheduling(ResourceType, PendingRequests);
    }
}

FString UMingRTSResourceScheduler::ExecuteFIFOScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // FIFO: 處理最早的請求
    const FResourceRequest& FirstRequest = PendingRequests[0];
    return ExecuteResourceAllocation(FirstRequest);
}

FString UMingRTSResourceScheduler::ExecutePriorityScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // 優先級調度: 找到最高優先級的請求
    const FResourceRequest* HighestPriorityRequest = nullptr;
    float HighestWeight = -1.0f;

    for (const FResourceRequest& Request : PendingRequests)
    {
        float Weight = GetPriorityWeight(Request.Priority);
        if (Weight > HighestWeight)
        {
            HighestWeight = Weight;
            HighestPriorityRequest = &Request;
        }
    }

    return HighestPriorityRequest ? ExecuteResourceAllocation(*HighestPriorityRequest) : TEXT("");
}

FString UMingRTSResourceScheduler::ExecuteRoundRobinScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // 輪轉調度
    int32* Counter = RoundRobinCounters.Find(ResourceType);
    int32 Index = Counter ? (*Counter % PendingRequests.Num()) : 0;
    
    if (Counter)
    {
        (*Counter)++;
    }

    return ExecuteResourceAllocation(PendingRequests[Index]);
}

FString UMingRTSResourceScheduler::ExecuteFairShareScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // 公平分享調度: 簡化實現，優先考慮資源使用量少的進程
    TMap<FString, float> ProcessUsage;
    
    // 計算每個進程的資源使用量
    for (const auto& AllocationPair : ResourceAllocations)
    {
        if (AllocationPair.Value.ResourceType == ResourceType)
        {
            float& Usage = ProcessUsage.FindOrAdd(AllocationPair.Value.ProcessID, 0.0f);
            Usage += AllocationPair.Value.AllocatedAmount;
        }
    }

    // 找到使用量最少的進程的請求
    const FResourceRequest* FairestRequest = nullptr;
    float MinUsage = FLT_MAX;

    for (const FResourceRequest& Request : PendingRequests)
    {
        float Usage = ProcessUsage.FindRef(Request.ProcessID);
        if (Usage < MinUsage)
        {
            MinUsage = Usage;
            FairestRequest = &Request;
        }
    }

    return FairestRequest ? ExecuteResourceAllocation(*FairestRequest) : TEXT("");
}

FString UMingRTSResourceScheduler::ExecuteRealTimeScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // 實時調度: 優先處理關鍵和高優先級請求
    for (const FResourceRequest& Request : PendingRequests)
    {
        if (Request.Priority == EResourcePriority::Critical)
        {
            return ExecuteResourceAllocation(Request);
        }
    }

    for (const FResourceRequest& Request : PendingRequests)
    {
        if (Request.Priority == EResourcePriority::High)
        {
            return ExecuteResourceAllocation(Request);
        }
    }

    // 如果沒有高優先級請求，使用FIFO
    return ExecuteFIFOScheduling(ResourceType, PendingRequests);
}

FString UMingRTSResourceScheduler::ExecuteAdaptiveScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests)
{
    if (PendingRequests.Num() == 0) return TEXT("");

    // 自適應調度: 根據系統負載動態選擇策略
    float SystemLoad = GetSystemLoad();
    
    if (SystemLoad > 80.0f)
    {
        // 高負載時使用實時調度
        return ExecuteRealTimeScheduling(ResourceType, PendingRequests);
    }
    else if (SystemLoad > 50.0f)
    {
        // 中等負載時使用優先級調度
        return ExecutePriorityScheduling(ResourceType, PendingRequests);
    }
    else
    {
        // 低負載時使用公平分享調度
        return ExecuteFairShareScheduling(ResourceType, PendingRequests);
    }
}

void UMingRTSResourceScheduler::PerformAutoOptimization()
{
    if (!bAutoOptimizationEnabled)
    {
        return;
    }

    OptimizeResourceAllocations();
}

void UMingRTSResourceScheduler::CalculateSchedulingMetrics(EResourceType ResourceType)
{
    FSchedulingMetrics* Metrics = SchedulingMetrics.Find(ResourceType);
    if (!Metrics)
    {
        return;
    }

    // 計算平均響應時間
    TArray<float>* ResponseHistory = ResponseTimeHistory.Find(ResourceType);
    if (ResponseHistory && ResponseHistory->Num() > 0)
    {
        float Sum = 0.0f;
        for (float Time : *ResponseHistory)
        {
            Sum += Time;
        }
        Metrics->AverageResponseTime = Sum / ResponseHistory->Num();
    }

    // 計算資源利用率
    const FResourcePool* Pool = ResourcePools.Find(ResourceType);
    if (Pool && Pool->TotalCapacity > 0.0f)
    {
        Metrics->ResourceUtilization = (Pool->UtilizedCapacity / Pool->TotalCapacity) * 100.0f;
    }

    // 計算吞吐量 (每秒處理的請求數)
    if (Metrics->TotalRequestsProcessed > 0)
    {
        Metrics->Throughput = (float)Metrics->TotalRequestsProcessed / (FDateTime::Now() - FDateTime::MinValue()).GetTotalSeconds();
    }
}

FString UMingRTSResourceScheduler::GenerateAllocationID()
{
    return FString::Printf(TEXT("ALLOC_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSResourceScheduler::GenerateRequestID()
{
    return FString::Printf(TEXT("REQ_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSResourceScheduler::ValidateResourceRequest(const FResourceRequest& Request)
{
    if (Request.RequiredAmount <= 0.0f || Request.MaxAmount <= 0.0f)
    {
        return false;
    }

    if (Request.RequiredAmount > Request.MaxAmount)
    {
        return false;
    }

    if (Request.TimeoutSeconds <= 0.0f)
    {
        return false;
    }

    return true;
}

float UMingRTSResourceScheduler::GetPriorityWeight(EResourcePriority Priority)
{
    switch (Priority)
    {
    case EResourcePriority::Critical: return 100.0f;
    case EResourcePriority::High: return 75.0f;
    case EResourcePriority::Normal: return 50.0f;
    case EResourcePriority::Low: return 25.0f;
    case EResourcePriority::Background: return 10.0f;
    default: return 50.0f;
    }
}

void UMingRTSResourceScheduler::LogAllocationEvent(const FString& AllocationID, const FString& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Allocation Event [%s]: %s"), *AllocationID, *Event);
}

void UMingRTSResourceScheduler::RecordMetrics(EResourceType ResourceType, const FString& EventType, float Value)
{
    // 這裡可以實現更詳細的指標記錄
    UE_LOG(LogTemp, VeryVerbose, TEXT("Metrics [%d]: %s = %.2f"), (int32)ResourceType, *EventType, Value);
}
