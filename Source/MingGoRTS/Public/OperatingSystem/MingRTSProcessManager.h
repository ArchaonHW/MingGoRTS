#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSProcessManager.generated.h"

UENUM(BlueprintType)
enum class EProcessState : uint8
{
    Created,            // 已創建
    Ready,              // 就緒
    Running,            // 運行中
    Blocked,            // 阻塞
    Suspended,          // 掛起
    Terminated,         // 已終止
    Zombie             // 僵屍進程
};

UENUM(BlueprintType)
enum class ESchedulingPolicy : uint8
{
    FIFO,               // 先進先出
    Priority,           // 優先級調度
    RoundRobin,         // 時間片輪轉
    RealTime,           // 實時調度
    Adaptive            // 自適應調度
};

UENUM(BlueprintType)
enum class EProcessPriority : uint8
{
    Idle = 0,          // 空閒
    Low = 1,            // 低
    Normal = 2,         // 正常
    High = 3,           // 高
    Critical = 4        // 關鍵
};

USTRUCT(BlueprintType)
struct FProcessInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString ProcessID;

    UPROPERTY()
    FString ProcessName;

    UPROPERTY()
    FString ParentProcessID;

    UPROPERTY()
    EProcessState State;

    UPROPERTY()
    EProcessPriority Priority;

    UPROPERTY()
    ESchedulingPolicy SchedulingPolicy;

    UPROPERTY()
    int32 ThreadCount;

    UPROPERTY()
    size_t MemoryUsage;

    UPROPERTY()
    float CPUUsage;

    UPROPERTY()
    FDateTime CreationTime;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    float RunningTime;

    UPROPERTY()
    int32 ExitCode;

    UPROPERTY()
    FString ExecutablePath;

    UPROPERTY()
    TArray<FString> CommandArguments;

    UPROPERTY()
    TMap<FString, FString> Environment;

    UPROPERTY()
    FString WorkingDirectory;

    FProcessInfo()
        : State(EProcessState::Created)
        , Priority(EProcessPriority::Normal)
        , SchedulingPolicy(ESchedulingPolicy::Priority)
        , ThreadCount(1)
        , MemoryUsage(0)
        , CPUUsage(0.0f)
        , RunningTime(0.0f)
        , ExitCode(0)
    {}
};

USTRUCT(BlueprintType)
struct FProcessCreationRequest
{
    GENERATED_BODY()

    UPROPERTY()
    FString ProcessName;

    UPROPERTY()
    FString ExecutablePath;

    UPROPERTY()
    TArray<FString> Arguments;

    UPROPERTY()
    TMap<FString, FString> Environment;

    UPROPERTY()
    FString WorkingDirectory;

    UPROPERTY()
    EProcessPriority Priority;

    UPROPERTY()
    ESchedulingPolicy SchedulingPolicy;

    UPROPERTY()
    bool bSuspended;

    UPROPERTY()
    FString ParentProcessID;

    FProcessCreationRequest()
        : Priority(EProcessPriority::Normal)
        , SchedulingPolicy(ESchedulingPolicy::Priority)
        , bSuspended(false)
    {}
};

/**
 * MingGoRTS 進程管理器
 * 負責進程的創建、調度、終止和資源管理
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSProcessManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSProcessManager();

    // 進程管理
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString CreateProcess(const FProcessCreationRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool TerminateProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SuspendProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ResumeProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool WaitForProcess(const FString& ProcessID, float TimeoutSeconds = -1.0f);

    // 進程查詢
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FProcessInfo GetProcessInfo(const FString& ProcessID) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetAllProcesses() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByState(EProcessState State) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByPriority(EProcessPriority Priority) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FString> GetChildProcesses(const FString& ParentProcessID) const;

    // 進程控制
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SetProcessPriority(const FString& ProcessID, EProcessPriority Priority);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SetSchedulingPolicy(const FString& ProcessID, ESchedulingPolicy Policy);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SetProcessAffinity(const FString& ProcessID, const TArray<int32>& CPUAffinity);

    // 進程間通信 (IPC)
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SendMessage(const FString& FromProcessID, const FString& ToProcessID, const TArray<uint8>& Message);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<uint8> ReceiveMessage(const FString& ProcessID, float TimeoutSeconds = -1.0f);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool HasPendingMessages(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool CreateSharedMemory(const FString& Name, size_t Size);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void* MapSharedMemory(const FString& Name);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool UnmapSharedMemory(const FString& Name, void* Memory);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool DestroySharedMemory(const FString& Name);

    // 調度和性能
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetTimeSlice(float TimeSliceSeconds);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetSchedulingAlgorithm(ESchedulingPolicy Algorithm);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void EnablePreemptiveScheduling(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    float GetAverageResponseTime() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    float GetSystemThroughput() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetContextSwitches() const;

    // 監控和診斷
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FString> GetProcessDiagnostics(const FString& ProcessID) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString GenerateProcessReport(const FString& ProcessID) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString GenerateSystemReport() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SaveProcessSnapshot(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool LoadProcessSnapshot(const FString& FilePath);

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetMaxProcesses(int32 MaxProcesses);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetMaxMemoryPerProcess(size_t MaxMemory);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetProcessTimeout(float TimeoutSeconds);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void EnableProcessLogging(bool bEnabled);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessCreated, const FString&, ProcessID, const FProcessInfo&, ProcessInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessTerminated, const FString&, ProcessID, int32, ExitCode);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessStateChanged, const FString&, ProcessID, EProcessState, NewState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMessageReceived, const FString&, ProcessID, const FString&, FromProcessID, const TArray<uint8>&, Message);

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessCreated OnProcessCreated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessTerminated OnProcessTerminated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessStateChanged OnProcessStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnMessageReceived OnMessageReceived;

protected:
    virtual void BeginDestroy() override;

    // 內部調度邏輯
    virtual void ScheduleProcesses();
    virtual void UpdateProcessStates();
    virtual void PerformContextSwitch(const FString& FromProcessID, const FString& ToProcessID);

    // 調度算法實現
    virtual FString ScheduleFIFO();
    virtual FString SchedulePriority();
    virtual FString ScheduleRoundRobin();
    virtual FString ScheduleRealTime();
    virtual FString ScheduleAdaptive();

    // 進程狀態管理
    virtual void UpdateProcessState(const FString& ProcessID, EProcessState NewState);
    virtual void UpdateProcessStatistics(const FString& ProcessID);
    virtual void CleanupTerminatedProcesses();

    // IPC實現
    virtual void DeliverMessage(const FString& ToProcessID, const FString& FromProcessID, const TArray<uint8>& Message);

private:
    // 進程管理
    UPROPERTY()
    TMap<FString, FProcessInfo> Processes;

    UPROPERTY()
    FString CurrentProcessID;

    UPROPERTY()
    TArray<FString> ReadyQueue;

    UPROPERTY()
    TArray<FString> BlockedQueue;

    // 調度配置
    UPROPERTY()
    ESchedulingPolicy CurrentSchedulingPolicy;

    UPROPERTY()
    float TimeSlice;

    UPROPERTY()
    bool bPreemptiveSchedulingEnabled;

    UPROPERTY()
    int32 CurrentTimeSliceRemaining;

    // IPC機制
    UPROPERTY()
    TMap<FString, TArray<TTuple<FString, TArray<uint8>>>> MessageQueues;

    UPROPERTY()
    TMap<FString, void*> SharedMemoryRegions;

    UPROPERTY()
    TMap<FString, size_t> SharedMemorySizes;

    // 性能統計
    UPROPERTY()
    int32 ContextSwitchCount;

    UPROPERTY()
    float TotalResponseTime;

    UPROPERTY()
    int32 CompletedProcesses;

    UPROPERTY()
    float SystemUptime;

    // 配置
    UPROPERTY()
    int32 MaxConcurrentProcesses;

    UPROPERTY()
    size_t MaxMemoryPerProcess;

    UPROPERTY()
    float ProcessTimeout;

    UPROPERTY()
    bool bProcessLoggingEnabled;

    // 內部計時器
    UPROPERTY()
    FTimerHandle SchedulingTimer;

    UPROPERTY()
    FTimerHandle StatisticsTimer;

    // 進程ID生成
    UPROPERTY()
    int32 NextProcessID;

    // 內部狀態
    UPROPERTY()
    bool bSchedulingEnabled;
};
