#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSProcessManager.generated.h"

UENUM(BlueprintType)
enum class EProcessState : uint8
{
    Created     UMETA(DisplayName = "Created"),
    Running     UMETA(DisplayName = "Running"),
    Suspended   UMETA(DisplayName = "Suspended"),
    Terminated  UMETA(DisplayName = "Terminated"),
    Crashed     UMETA(DisplayName = "Crashed")
};

UENUM(BlueprintType)
enum class EProcessPriority : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Low         UMETA(DisplayName = "Low"),
    Normal      UMETA(DisplayName = "Normal"),
    High        UMETA(DisplayName = "High"),
    Critical    UMETA(DisplayName = "Critical")
};

USTRUCT(BlueprintType)
struct FProcessInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    FString ProcessID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    FString ProcessName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    FString OwnerSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    EProcessState State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    EProcessPriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    float CPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    int32 ThreadCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    FDateTime CreationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    FDateTime LastActiveTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Info")
    TMap<FString, FString> ProcessData;
};

USTRUCT(BlueprintType)
struct FProcessCreationParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    FString ProcessName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    FString OwnerSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    EProcessPriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    float MemoryLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    float CPULimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    int32 MaxThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    TArray<FString> RequiredDependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Process Creation Params")
    TMap<FString, FString> InitialData;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessCreated, const FString&, ProcessID, const FProcessInfo&, ProcessInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessStateChanged, const FString&, ProcessID, EProcessState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessTerminated, const FString&, ProcessID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProcessCrashed, const FString&, ProcessID);

/**
 * 內建作業系統進程管理器
 * 負責進程的創建、調度、監控和終止
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSProcessManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSProcessManager();

    // 初始化進程管理器
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void InitializeProcessManager();

    // 創建進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString CreateProcess(const FProcessCreationParams& CreationParams);

    // 終止進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool TerminateProcess(const FString& ProcessID);

    // 掛起進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SuspendProcess(const FString& ProcessID);

    // 恢復進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ResumeProcess(const FString& ProcessID);

    // 獲取進程信息
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FProcessInfo GetProcessInfo(const FString& ProcessID) const;

    // 獲取所有進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetAllProcesses() const;

    // 按狀態獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByState(EProcessState State) const;

    // 按優先級獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByPriority(EProcessPriority Priority) const;

    // 按所有者獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByOwner(const FString& OwnerSystem) const;

    // 設置進程優先級
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SetProcessPriority(const FString& ProcessID, EProcessPriority NewPriority);

    // 更新進程狀態
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool UpdateProcessState(const FString& ProcessID, EProcessState NewState);

    // 檢查進程是否存在
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool DoesProcessExist(const FString& ProcessID) const;

    // 獲取進程性能統計
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TMap<FString, float> GetProcessPerformanceStats(const FString& ProcessID) const;

    // 獲取系統進程統計
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TMap<EProcessState, int32> GetSystemProcessStats() const;

    // 清理已終止的進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 CleanupTerminatedProcesses();

    // 設置最大進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetMaxProcessCount(int32 MaxCount);

    // 獲取最大進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetMaxProcessCount() const { return MaxProcessCount; }

    // 獲取當前進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetCurrentProcessCount() const;

    // 生成進程報告
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString GenerateProcessReport() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessCreated OnProcessCreated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessStateChanged OnProcessStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessTerminated OnProcessTerminated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager")
    FOnProcessCrashed OnProcessCrashed;

protected:
    // 生成唯一進程ID
    FString GenerateProcessID() const;

    // 驗證進程創建參數
    bool ValidateProcessCreationParams(const FProcessCreationParams& Params) const;

    // 檢查依賴關係
    bool CheckDependencies(const TArray<FString>& RequiredDependencies) const;

    // 分配系統資源
    bool AllocateSystemResources(const FString& ProcessID, const FProcessCreationParams& Params);

    // 釋放系統資源
    void ReleaseSystemResources(const FString& ProcessID);

    // 更新進程性能數據
    void UpdateProcessPerformanceData();

    // 執行進程調度
    void PerformProcessScheduling();

    // 檢查進程健康狀態
    void CheckProcessHealth();

    // 處理進程崩潰
    void HandleProcessCrash(const FString& ProcessID);

    // 計算進程執行時間
    float CalculateExecutionTime(const FProcessInfo& ProcessInfo) const;

private:
    // 活躍進程列表
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    TMap<FString, FProcessInfo> ActiveProcesses;

    // 已終止進程列表（用於清理）
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    TArray<FString> TerminatedProcesses;

    // 最大進程數量
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    int32 MaxProcessCount;

    // 進程調度間隔（秒）
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    float SchedulingInterval;

    // 最後調度時間
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    FDateTime LastSchedulingTime;

    // 進程ID計數器
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    static int32 ProcessIDCounter;

    // 系統資源使用情況
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    TMap<FString, float> SystemResourceUsage;

    // 是否啟用自動調度
    UPROPERTY(BlueprintReadOnly, Category = "Process Manager")
    bool bAutoSchedulingEnabled;
};
