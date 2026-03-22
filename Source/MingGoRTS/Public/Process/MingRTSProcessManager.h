#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "Kernel/MingRTSKernel.h"

// 進程調度算法
UENUM(BlueprintType)
enum class ESchedulingAlgorithm : uint8
{
    FIFO,           // 先進先出
    Priority,       // 優先級調度
    RoundRobin,     // 輪轉調度
    SJF,            // 最短作業優先
    MLFQ            // 多級反饋隊列
};

// 調度器狀態
UENUM(BlueprintType)
enum class ESchedulerState : uint8
{
    Idle,           // 空閒
    Running,        // 運行中
    Paused,         // 暫停
    Error           // 錯誤
};

// 進程隊列節點
USTRUCT(BlueprintType)
struct FProcessQueueNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    FString ProcessID;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    int32 Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    float ArrivalTime;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    float BurstTime;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    int32 TimeSlice;

    FProcessQueueNode()
    {
        Priority = 0;
        ArrivalTime = 0.0f;
        BurstTime = 0.0f;
        TimeSlice = 100; // 默認時間片100ms
    }
};

// 調度統計
USTRUCT(BlueprintType)
struct FSchedulingStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Scheduling Stats")
    int32 TotalProcessesScheduled;

    UPROPERTY(BlueprintReadOnly, Category = "Scheduling Stats")
    float AverageWaitingTime;

    UPROPERTY(BlueprintReadOnly, Category = "Scheduling Stats")
    float AverageTurnaroundTime;

    UPROPERTY(BlueprintReadOnly, Category = "Scheduling Stats")
    float CPUUtilization;

    UPROPERTY(BlueprintReadOnly, Category = "Scheduling Stats")
    int32 ContextSwitches;

    FSchedulingStatistics()
    {
        TotalProcessesScheduled = 0;
        AverageWaitingTime = 0.0f;
        AverageTurnaroundTime = 0.0f;
        CPUUtilization = 0.0f;
        ContextSwitches = 0;
    }
};

// 進程調度事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessScheduled, const FString&, ProcessID, ESchedulingAlgorithm, Algorithm);

/**
 * 進程管理器
 * 負責進程創建、調度、終止和資源管理
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSProcessManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSProcessManager();

    // 初始化和控制
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool InitializeProcessManager(UMingRTSKernel* InKernel);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void ShutdownProcessManager();

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    ESchedulerState GetSchedulerState() const { return SchedulerState; }

    // 進程管理
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString CreateProcess(const FString& ProcessName, int32 Priority = 0, float BurstTime = 1000.0f);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool TerminateProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SuspendProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ResumeProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ChangeProcessPriority(const FString& ProcessID, int32 NewPriority);

    // 調度控制
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetSchedulingAlgorithm(ESchedulingAlgorithm Algorithm);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetTimeSlice(int32 TimeSliceMs);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void StartScheduling();

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void StopScheduling();

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void PauseScheduling();

    // 進程查詢
    UFUNCTION(BlueprintPure, Category = "Process Manager")
    TArray<FString> GetReadyProcesses() const;

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    FString GetCurrentRunningProcess() const { return CurrentRunningProcess; }

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    TArray<FProcessInfo> GetAllProcesses() const;

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    FSchedulingStatistics GetSchedulingStatistics() const { return Statistics; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Process Manager Events")
    FOnProcessScheduled OnProcessScheduled;

protected:
    // 內核引用
    UPROPERTY()
    TObjectPtr<UMingRTSKernel> Kernel;

    // 調度器狀態
    UPROPERTY()
    ESchedulerState SchedulerState;

    // 調度算法
    UPROPERTY()
    ESchedulingAlgorithm SchedulingAlgorithm;

    // 時間片
    UPROPERTY()
    int32 TimeSlice;

    // 當前運行的進程
    UPROPERTY()
    FString CurrentRunningProcess;

    // 就緒隊列
    UPROPERTY()
    TQueue<FProcessQueueNode> ReadyQueue;

    // 等待隊列
    UPROPERTY()
    TMap<FString, FProcessQueueNode> WaitingProcesses;

    // 進程信息映射
    UPROPERTY()
    TMap<FString, FProcessQueueNode> ProcessNodes;

    // 調度統計
    UPROPERTY()
    FSchedulingStatistics Statistics;

    // 同步機制
    mutable FCriticalSection QueueCriticalSection;
    mutable FCriticalSection StatisticsCriticalSection;

    // 內部方法
    void ScheduleNextProcess();
    void PerformContextSwitch(const FString& NewProcessID);
    void UpdateStatistics();
    void ProcessTimeSlice();

    // 調度算法實現
    FString ScheduleFIFO();
    FString SchedulePriority();
    FString ScheduleRoundRobin();
    FString ScheduleSJF();
    FString ScheduleMLFQ();

    // 輔助方法
    bool IsProcessReady(const FString& ProcessID) const;
    bool IsProcessWaiting(const FString& ProcessID) const;
    void AddToReadyQueue(const FProcessQueueNode& ProcessNode);
    void AddToWaitingQueue(const FString& ProcessID, const FProcessQueueNode& ProcessNode);
    void RemoveFromQueue(const FString& ProcessID);
    float CalculateWaitingTime(const FString& ProcessID) const;
    float CalculateTurnaroundTime(const FString& ProcessID) const;

private:
    // 初始化方法
    bool InitializeScheduler();
    void ResetStatistics();

    // 清理方法
    void CleanupQueues();
    void CleanupStatistics();
};
