#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"

// 系統調用類型
UENUM(BlueprintType)
enum class ESystemCallType : uint8
{
    None,           // 無調用
    Process,        // 進程管理
    Memory,         // 內存管理
    FileSystem,     // 文件系統
    Network,        // 網絡
    Graphics,       // 圖形
    Audio,         // 音頻
    Input,         // 輸入
    Timer,         // 計時器
    Custom          // 自定義
};

// 進程狀態
UENUM(BlueprintType)
enum class EProcessState : uint8
{
    Created,        // 已創建
    Ready,          // 就緒
    Running,        // 運行中
    Waiting,        // 等待
    Suspended,      // 掛起
    Terminated,     // 已終止
    Error           // 錯誤
};

// 內核狀態
UENUM(BlueprintType)
enum class EKernelState : uint8
{
    Uninitialized,  // 未初始化
    Initializing,   // 初始化中
    Running,        // 運行中
    ShuttingDown,   // 關閉中
    Error           // 錯誤
};

// 系統調用參數
USTRUCT(BlueprintType)
struct FSystemCallParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "System Call")
    ESystemCallType CallType;

    UPROPERTY(BlueprintReadWrite, Category = "System Call")
    FString FunctionName;

    UPROPERTY(BlueprintReadWrite, Category = "System Call")
    TArray<FString> Parameters;

    UPROPERTY(BlueprintReadWrite, Category = "System Call")
    int32 Priority;

    FSystemCallParams()
    {
        CallType = ESystemCallType::None;
        Priority = 0;
    }
};

// 進程信息
USTRUCT(BlueprintType)
struct FProcessInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FString ProcessID;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FString ProcessName;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    EProcessState State;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    int32 Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    float CPUUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    int32 MemoryUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FDateTime CreationTime;

    FProcessInfo()
    {
        State = EProcessState::Created;
        Priority = 0;
        CPUUsage = 0.0f;
        MemoryUsage = 0;
    }
};

// 中斷類型
UENUM(BlueprintType)
enum class EInterruptType : uint8
{
    Timer,          // 計時器中斷
    Hardware,       // 硬件中斷
    Software,       // 軟件中斷
    System,         // 系統中斷
    User            // 用戶中斷
};

// 中斷處理器委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInterrupt, EInterruptType, InterruptType);

/**
 * MingGoRTS 內建作業系統內核
 * 提供系統調用接口、進程管理、中斷處理等核心功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSKernel : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSKernel();

    // 內核初始化和控制
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool InitializeKernel();

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void ShutdownKernel();

    UFUNCTION(BlueprintPure, Category = "Kernel")
    EKernelState GetKernelState() const { return KernelState; }

    // 系統調用接口
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FString ExecuteSystemCall(const FSystemCallParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool RegisterSystemService(const FString& ServiceName, class UObject* Service);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void UnregisterSystemService(const FString& ServiceName);

    // 進程管理
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FString CreateProcess(const FString& ProcessName, int32 Priority = 0);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool TerminateProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool SuspendProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool ResumeProcess(const FString& ProcessID);

    UFUNCTION(BlueprintPure, Category = "Kernel")
    TArray<FProcessInfo> GetAllProcesses() const;

    UFUNCTION(BlueprintPure, Category = "Kernel")
    FProcessInfo GetProcessInfo(const FString& ProcessID) const;

    // 中斷處理
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void RegisterInterruptHandler(EInterruptType InterruptType, const FOnInterrupt& Handler);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void TriggerInterrupt(EInterruptType InterruptType);

    // 內核統計
    UFUNCTION(BlueprintPure, Category = "Kernel")
    int32 GetProcessCount() const { return Processes.Num(); }

    UFUNCTION(BlueprintPure, Category = "Kernel")
    float GetCPUUsage() const { return TotalCPUUsage; }

    UFUNCTION(BlueprintPure, Category = "Kernel")
    int32 GetMemoryUsage() const { return TotalMemoryUsage; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Kernel Events")
    FOnInterrupt OnInterrupt;

protected:
    // 內核狀態
    UPROPERTY()
    EKernelState KernelState;

    // 進程管理
    UPROPERTY()
    TMap<FString, FProcessInfo> Processes;

    // 系統服務註冊
    UPROPERTY()
    TMap<FString, class UObject*> SystemServices;

    // 中斷處理器
    UPROPERTY()
    TMap<EInterruptType, FOnInterrupt> InterruptHandlers;

    // 系統調用隊列
    UPROPERTY()
    TQueue<FSystemCallParams> SystemCallQueue;

    // 同步機制
    mutable FCriticalSection ProcessCriticalSection;
    mutable FCriticalSection ServiceCriticalSection;
    mutable FCriticalSection InterruptCriticalSection;

    // 性能統計
    UPROPERTY()
    float TotalCPUUsage;

    UPROPERTY()
    int32 TotalMemoryUsage;

    // 內部方法
    void ProcessSystemCalls();
    void UpdateProcessStates();
    void HandleInterrupt(EInterruptType InterruptType);
    FString GenerateProcessID() const;
    bool IsValidProcessID(const FString& ProcessID) const;

private:
    // 內核初始化步驟
    bool InitializeProcessManager();
    bool InitializeMemoryManager();
    bool InitializeFileSystem();
    bool InitializeInterruptSystem();

    // 清理方法
    void CleanupProcesses();
    void CleanupServices();
    void CleanupInterrupts();
};
