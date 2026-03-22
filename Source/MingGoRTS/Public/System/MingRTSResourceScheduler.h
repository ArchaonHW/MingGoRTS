#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSResourceScheduler.generated.h"

UENUM(BlueprintType)
enum class EResourceType: uint8 {
    CPU,
    Memory,
    GPU,
    Network,
    Storage,
    Custom
};

UENUM(BlueprintType)
enum class ESchedulingPolicy: uint8 {
    FIFO,           // First In, First Out
    Priority,       // Priority-based scheduling
    RoundRobin,     // Round-robin scheduling
    FairShare,      // Fair share scheduling
    RealTime,       // Real-time scheduling
    Adaptive        // Adaptive scheduling
};

UENUM(BlueprintType)
enum class EResourcePriority: uint8 {
    Critical,       // Critical system processes
    High,           // High priority processes
    Normal,         // Normal priority processes
    Low,            // Low priority processes
    Background      // Background processes
};

USTRUCT(BlueprintType)
struct FResourceRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestID;

    UPROPERTY(BlueprintReadOnly)
    EResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly)
    EResourcePriority Priority;

    UPROPERTY(BlueprintReadOnly)
    float RequiredAmount;

    UPROPERTY(BlueprintReadOnly)
    float MaxAmount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime RequestTime;

    UPROPERTY(BlueprintReadOnly)
    float TimeoutSeconds;

    UPROPERTY(BlueprintReadOnly)
    FString ProcessID;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPersistent;

    FResourceRequest()
    {
        RequestID = TEXT("");
        ResourceType = EResourceType::CPU;
        Priority = EResourcePriority::Normal;
        RequiredAmount = 0.0f;
        MaxAmount = 0.0f;
        RequestTime = FDateTime::Now();
        TimeoutSeconds = 30.0f;
        ProcessID = TEXT("");
        bIsPersistent = false;
    }
};

USTRUCT(BlueprintType)
struct FResourceAllocation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString AllocationID;

    UPROPERTY(BlueprintReadOnly)
    FString RequestID;

    UPROPERTY(BlueprintReadOnly)
    EResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly)
    float AllocatedAmount;

    UPROPERTY(BlueprintReadOnly)
    float UtilizedAmount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime AllocationTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdateTime;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    FResourceAllocation()
    {
        AllocationID = TEXT("");
        RequestID = TEXT("");
        ResourceType = EResourceType::CPU;
        AllocatedAmount = 0.0f;
        UtilizedAmount = 0.0f;
        AllocationTime = FDateTime::Now();
        LastUpdateTime = FDateTime::Now();
        bIsActive = true;
    }
};

USTRUCT(BlueprintType)
struct FResourcePool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly)
    float TotalCapacity;

    UPROPERTY(BlueprintReadOnly)
    float AvailableCapacity;

    UPROPERTY(BlueprintReadOnly)
    float ReservedCapacity;

    UPROPERTY(BlueprintReadOnly)
    float UtilizedCapacity;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAllocations;

    UPROPERTY(BlueprintReadOnly)
    int32 PendingRequests;

    FResourcePool()
    {
        ResourceType = EResourceType::CPU;
        TotalCapacity = 100.0f;
        AvailableCapacity = 100.0f;
        ReservedCapacity = 0.0f;
        UtilizedCapacity = 0.0f;
        ActiveAllocations = 0;
        PendingRequests = 0;
    }
};

USTRUCT(BlueprintType)
struct FSchedulingMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float AverageResponseTime;

    UPROPERTY(BlueprintReadOnly)
    float AverageWaitTime;

    UPROPERTY(BlueprintReadOnly)
    float ResourceUtilization;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalRequestsProcessed;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulAllocations;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedAllocations;

    UPROPERTY(BlueprintReadOnly)
    int32 Timeouts;

    UPROPERTY(BlueprintReadOnly)
    float Throughput;

    FSchedulingMetrics()
    {
        AverageResponseTime = 0.0f;
        AverageWaitTime = 0.0f;
        ResourceUtilization = 0.0f;
        TotalRequestsProcessed = 0;
        SuccessfulAllocations = 0;
        FailedAllocations = 0;
        Timeouts = 0;
        Throughput = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceAllocated, const FResourceAllocation&, Allocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceReleased, const FString&, AllocationID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceRequestFailed, const FString&, RequestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceUtilizationChanged, EResourceType, ResourceType, float, Utilization);

/**
 * 智能系統資源調度器
 * 提供CPU、內存、GPU等資源的智能分配和管理
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSResourceScheduler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSResourceScheduler();

    // 初始化資源調度器
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool InitializeResourceScheduler();

    // 關閉資源調度器
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void ShutdownResourceScheduler();

    // 請求資源分配
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    FString RequestResource(const FResourceRequest& Request);

    // 釋放資源分配
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool ReleaseResource(const FString& AllocationID);

    // 調整資源分配
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool AdjustResourceAllocation(const FString& AllocationID, float NewAmount);

    // 獲取資源分配信息
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    FResourceAllocation GetResourceAllocation(const FString& AllocationID) const;

    // 獲取資源池狀態
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    FResourcePool GetResourcePool(EResourceType ResourceType) const;

    // 獲取所有資源池狀態
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    TArray<FResourcePool> GetAllResourcePools() const;

    // 設置調度策略
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void SetSchedulingPolicy(EResourceType ResourceType, ESchedulingPolicy Policy);

    // 獲取調度策略
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    ESchedulingPolicy GetSchedulingPolicy(EResourceType ResourceType) const;

    // 設置資源池容量
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool SetResourcePoolCapacity(EResourceType ResourceType, float Capacity);

    // 預留資源
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool ReserveResource(EResourceType ResourceType, float Amount);

    // 釋放預留資源
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool ReleaseReservedResource(EResourceType ResourceType, float Amount);

    // 獲取調度指標
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    FSchedulingMetrics GetSchedulingMetrics(EResourceType ResourceType) const;

    // 獲取所有調度指標
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    TMap<EResourceType, FSchedulingMetrics> GetAllSchedulingMetrics() const;

    // 重置指標
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void ResetMetrics(EResourceType ResourceType);

    // 重置所有指標
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void ResetAllMetrics();

    // 強制垃圾回收
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void ForceGarbageCollection();

    // 獲取系統負載
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    float GetSystemLoad() const;

    // 獲取資源利用率
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    float GetResourceUtilization(EResourceType ResourceType) const;

    // 預測資源需求
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    float PredictResourceDemand(EResourceType ResourceType, float TimeHorizon);

    // 優化資源分配
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void OptimizeResourceAllocations();

    // 設置自動優化
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    void SetAutoOptimizationEnabled(bool bEnabled);

    // 獲取自動優化狀態
    UFUNCTION(BlueprintCallable, Category = "Resource Scheduler")
    bool IsAutoOptimizationEnabled() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Resource Scheduler")
    FOnResourceAllocated OnResourceAllocated;

    UPROPERTY(BlueprintAssignable, Category = "Resource Scheduler")
    FOnResourceReleased OnResourceReleased;

    UPROPERTY(BlueprintAssignable, Category = "Resource Scheduler")
    FOnResourceRequestFailed OnResourceRequestFailed;

    UPROPERTY(BlueprintAssignable, Category = "Resource Scheduler")
    FOnResourceUtilizationChanged OnResourceUtilizationChanged;

protected:
    // 處理資源請求
    virtual void ProcessResourceRequest(const FString& RequestID);

    // 檢查資源可用性
    virtual bool CheckResourceAvailability(EResourceType ResourceType, float Amount);

    // 執行資源分配
    virtual FString ExecuteResourceAllocation(const FResourceRequest& Request);

    // 執行資源釋放
    virtual bool ExecuteResourceRelease(const FString& AllocationID);

    // 更新資源利用率
    virtual void UpdateResourceUtilization();

    // 處理超時請求
    virtual void HandleTimeoutRequests();

    // 執行調度算法
    virtual FString ExecuteSchedulingAlgorithm(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // FIFO調度
    virtual FString ExecuteFIFOScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 優先級調度
    virtual FString ExecutePriorityScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 輪轉調度
    virtual FString ExecuteRoundRobinScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 公平分享調度
    virtual FString ExecuteFairShareScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 實時調度
    virtual FString ExecuteRealTimeScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 自適應調度
    virtual FString ExecuteAdaptiveScheduling(EResourceType ResourceType, const TArray<FResourceRequest>& PendingRequests);

    // 自動優化
    virtual void PerformAutoOptimization();

    // 計算調度指標
    virtual void CalculateSchedulingMetrics(EResourceType ResourceType);

    // 生成分配ID
    virtual FString GenerateAllocationID();

    // 生成請求ID
    virtual FString GenerateRequestID();

    // 驗證資源請求
    virtual bool ValidateResourceRequest(const FResourceRequest& Request);

    // 獲取優先級權重
    virtual float GetPriorityWeight(EResourcePriority Priority);

    // 記錄分配事件
    virtual void LogAllocationEvent(const FString& AllocationID, const FString& Event);

    // 記錄指標
    virtual void RecordMetrics(EResourceType ResourceType, const FString& EventType, float Value);

private:
    // 資源池
    UPROPERTY()
    TMap<EResourceType, FResourcePool> ResourcePools;

    // 資源分配
    UPROPERTY()
    TMap<FString, FResourceAllocation> ResourceAllocations;

    // 待處理請求
    UPROPERTY()
    TArray<FResourceRequest> PendingRequests;

    // 調度策略
    UPROPERTY()
    TMap<EResourceType, ESchedulingPolicy> SchedulingPolicies;

    // 調度指標
    UPROPERTY()
    TMap<EResourceType, FSchedulingMetrics> SchedulingMetrics;

    // 系統狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsShutdown;

    UPROPERTY()
    bool bAutoOptimizationEnabled;

    // 計時器
    UPROPERTY()
    FTimerHandle MetricsUpdateTimer;

    UPROPERTY()
    FTimerHandle TimeoutCheckTimer;

    UPROPERTY()
    FTimerHandle AutoOptimizationTimer;

    // 統計數據
    UPROPERTY()
    TMap<EResourceType, TArray<float>> ResponseTimeHistory;

    UPROPERTY()
    TMap<EResourceType, TArray<float>> UtilizationHistory;

    UPROPERTY()
    TMap<EResourceType, int32> RoundRobinCounters;
};
