#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSKernel.generated.h"

// 前向聲明至聖者指揮學擴展
class UMingRTSKernelSageCommandExtension;

UENUM(BlueprintType)
enum class EKernelState : uint8
{
    Uninitialized,      // 未初始化
    Initializing,       // 初始化中
    Running,           // 運行中
    ShuttingDown,      // 關閉中
    Stopped            // 已停止
};

UENUM(BlueprintType)
enum class ESystemCall : uint8
{
    // 進程管理系統調用
    CreateProcess,      // 創建進程
    TerminateProcess,   // 終止進程
    GetProcessInfo,     // 獲取進程信息
    WaitForProcess,     // 等待進程
    
    // 內存管理系統調用
    AllocateMemory,     // 分配內存
    FreeMemory,         // 釋放內存
    ProtectMemory,      // 保護內存
    MapMemory,          // 內存映射
    
    // 文件系統系統調用
    OpenFile,           // 打開文件
    ReadFile,           // 讀取文件
    WriteFile,          // 寫入文件
    CloseFile,          // 關閉文件
    DeleteFile,         // 刪除文件
    
    // 系統信息系統調用
    GetSystemInfo,      // 獲取系統信息
    GetTime,            // 獲取系統時間
    SetTime,            // 設置系統時間
    GetVersion,         // 獲取系統版本
    
    // 設備管理系統調用
    OpenDevice,         // 打開設備
    CloseDevice,        // 關閉設備
    ControlDevice,      // 控制設備
    ReadDevice,         // 讀取設備
    
    // 網絡系統調用
    CreateSocket,       // 創建套接字
    BindSocket,         // 綁定套接字
    ListenSocket,       // 監聽套接字
    AcceptSocket,       // 接受連接
    ConnectSocket,      // 連接套接字
    SendData,           // 發送數據
    ReceiveData,        // 接收數據
    
    // 至聖者指揮學系統調用
    SageCommand_CreateCharacter,       // 創建指揮者角色
    SageCommand_UseStrategy,          // 使用策略
    SageCommand_CoordinatePowers,     // 協調三權
    SageCommand_CheckFallStatus,      // 檢查墮落狀態
    SageCommand_StartWuXingCycle,     // 啟動五行循環
    SageCommand_AdvanceWuXingPhase,   // 推進五行階段
    SageCommand_CreateAtonementTask,  // 創建贖罪任務
    SageCommand_CompleteAtonementTask, // 完成贖罪任務
    SageCommand_GetStatus,            // 獲取指揮學系統狀態
    
    Custom              // 自定義系統調用
};

USTRUCT(BlueprintType)
struct FSystemCallRequest
{
    GENERATED_BODY()

    UPROPERTY()
    ESystemCall CallType;

    UPROPERTY()
    TArray<uint8> Parameters;

    UPROPERTY()
    FString RequesterID;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    int32 RequestID;

    FSystemCallRequest()
        : CallType(ESystemCall::Custom)
        , RequestID(0)
        , Timestamp(FDateTime::Now())
    {}
};

USTRUCT(BlueprintType)
struct FSystemCallResponse
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RequestID;

    UPROPERTY()
    bool bSuccess;

    UPROPERTY()
    TArray<uint8> ResultData;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    FDateTime Timestamp;

    FSystemCallResponse()
        : RequestID(0)
        , bSuccess(false)
        , Timestamp(FDateTime::Now())
    {}
};

USTRUCT(BlueprintType)
struct FKernelInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString KernelVersion;

    UPROPERTY()
    FString BuildNumber;

    UPROPERTY()
    FDateTime BuildTime;

    UPROPERTY()
    EKernelState CurrentState;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    float UptimeSeconds;

    UPROPERTY()
    int32 TotalProcesses;

    UPROPERTY()
    int32 ActiveProcesses;

    UPROPERTY()
    int32 TotalMemoryMB;

    UPROPERTY()
    int32 UsedMemoryMB;

    UPROPERTY()
    float CPUUsage;

    FKernelInfo()
        : KernelVersion(TEXT("1.0.0-SageCommand"))
        , BuildNumber(TEXT("20260323-Sage"))
        , CurrentState(EKernelState::Uninitialized)
        , UptimeSeconds(0.0f)
        , TotalProcesses(0)
        , ActiveProcesses(0)
        , TotalMemoryMB(0)
        , UsedMemoryMB(0)
        , CPUUsage(0.0f)
    {}
};

/**
 * MingGoRTS 作業系統內核
 * 提供核心的系統服務和管理功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSKernel : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSKernel();

    // 內核生命週期管理
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void Shutdown();

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void Restart();

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    EKernelState GetKernelState() const { return KernelState; }

    // 系統調用接口
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FSystemCallResponse MakeSystemCall(const FSystemCallRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    int32 MakeAsyncSystemCall(const FSystemCallRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FSystemCallResponse GetSystemCallResponse(int32 RequestID);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool IsSystemCallCompleted(int32 RequestID) const;

    // 內核信息查詢
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FKernelInfo GetKernelInfo() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FString GetSystemVersion() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FDateTime GetSystemTime() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool SetSystemTime(const FDateTime& NewTime);

    // 中斷處理
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool RegisterInterruptHandler(int32 InterruptNumber, UObject* Handler);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool UnregisterInterruptHandler(int32 InterruptNumber);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void TriggerInterrupt(int32 InterruptNumber);

    // 系統服務管理
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool RegisterSystemService(const FString& ServiceName, UObject* Service);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool UnregisterSystemService(const FString& ServiceName);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    UObject* GetSystemService(const FString& ServiceName);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    TArray<FString> GetRegisteredServices() const;

    // 內核配置
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void SetMaxSystemCalls(int32 MaxCalls);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void SetSystemCallTimeout(float TimeoutSeconds);

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    void EnableSystemCallLogging(bool bEnabled);

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    float GetSystemCallAverageTime() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    int32 GetSystemCallCount() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    TArray<FString> GetPerformanceMetrics() const;

    // 調試和診斷
    UFUNCTION(BlueprintCallable, Category = "Kernel")
    FString GenerateKernelDump() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool SaveKernelState(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Kernel")
    bool LoadKernelState(const FString& FilePath);

    // 至聖者指揮學擴展接口
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool InitializeSageCommandExtension();

    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool IsSageCommandExtensionEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FString GetSageCommandVersion() const;

    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    UObject* GetSageCommandExtension() const;

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKernelStateChanged, EKernelState, OldState, EKernelState, NewState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemCall, const FSystemCallRequest&, Request, const FSystemCallResponse&, Response);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInterrupt, int32, InterruptNumber);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemServiceRegistered, const FString&, ServiceName, UObject*, Service);

    UPROPERTY(BlueprintAssignable, Category = "Kernel")
    FOnKernelStateChanged OnKernelStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Kernel")
    FOnSystemCall OnSystemCall;

    UPROPERTY(BlueprintAssignable, Category = "Kernel")
    FOnInterrupt OnInterrupt;

    UPROPERTY(BlueprintAssignable, Category = "Kernel")
    FOnSystemServiceRegistered OnSystemServiceRegistered;

protected:
    virtual void BeginDestroy() override;

    // 內部系統調用處理
    virtual FSystemCallResponse HandleSystemCall(const FSystemCallRequest& Request);
    virtual void HandleAsyncSystemCall(const FSystemCallRequest& Request, int32 RequestID);

    // 具體系統調用實現
    virtual FSystemCallResponse HandleCreateProcess(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleTerminateProcess(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleAllocateMemory(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleFreeMemory(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleOpenFile(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleReadFile(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleWriteFile(const TArray<uint8>& Parameters);
    virtual FSystemCallResponse HandleGetSystemInfo(const TArray<uint8>& Parameters);

    // 中斷處理
    virtual void ProcessInterrupt(int32 InterruptNumber);

    // 內核狀態管理
    void SetKernelState(EKernelState NewState);
    void UpdateKernelInfo();

private:
    // 內核狀態
    UPROPERTY()
    EKernelState KernelState;

    UPROPERTY()
    FKernelInfo KernelInfo;

    // 系統調用管理
    UPROPERTY()
    TMap<int32, FSystemCallResponse> PendingSystemCalls;

    UPROPERTY()
    int32 NextSystemCallID;

    UPROPERTY()
    float SystemCallTimeout;

    UPROPERTY()
    bool bSystemCallLoggingEnabled;

    // 系統服務註冊表
    UPROPERTY()
    TMap<FString, UObject*> SystemServices;

    // 中斷處理器
    UPROPERTY()
    TMap<int32, UObject*> InterruptHandlers;

    // 性能統計
    UPROPERTY()
    TMap<ESystemCall, float> SystemCallTimes;

    UPROPERTY()
    TMap<ESystemCall, int32> SystemCallCounts;

    UPROPERTY()
    int32 TotalSystemCalls;

    // 配置
    UPROPERTY()
    int32 MaxSystemCallsPerSecond;

    UPROPERTY()
    FDateTime StartTime;

    // 至聖者指揮學擴展
    UPROPERTY()
    TObjectPtr<UMingRTSKernelSageCommandExtension> SageCommandExtension;
    
    UPROPERTY()
    bool bSageCommandExtensionEnabled;
};
