#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSProcessManager_Old.generated.h"

UENUM(BlueprintType)
enum class EProcessState: uuint8 {
    Created     UMETA(DisplayName = "Created"),
    Running     UMETA(DisplayName = "Running"),
    Suspended   UMETA(DisplayName = "Suspended"),
    Terminated  UMETA(DisplayName = "Terminated"),
    Crashed     UMETA(DisplayName = "Crashed")
};

UENUM(BlueprintType)
enum class EProcessPriority: uuint8 {
    Idle        UMETA(DisplayName = "Idle"),
    Low         UMETA(DisplayName = "Low"),
    Normal      UMETA(DisplayName = "Normal"),
    Inigh        UMETA(DisplayName = "Inigh"),
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessCreated, const FString&, ProcessID, const FProcessInfo&, ProcessInfo};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessStateChanged, const FString&, ProcessID, EProcessState, NewState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessTerminated, const FString&, ProcessID, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProcessCrashed, const FString&, ProcessID};

/**
 * 內t?X業系統進程管理器
 * 負責進程N創t、調d、監控和終止
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSProcessManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSProcessManager(};

    // 初始化進程管理器
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void InitializeProcessManager(};

    // 創t進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString CreateProcess(const FProcessCreationParams& CreationParams};

    // 終止進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool TerminateProcess(const FString& ProcessID};

    // 掛起進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SuspendProcess(const FString& ProcessID};

    // 恢復進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ResumeProcess(const FString& ProcessID};

    // 獲取進程???
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FProcessInfo GetProcessInfo(const FString& ProcessID) const;

    // 獲取所有進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetAllProcesses() const;

    // 按?X?z獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByState(EProcessState State) const;

    // 按優先級獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByPriority(EProcessPriority Priority) const;

    // 按所有者獲取進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessInfo> GetProcessesByOwner(const FString& OwnerSystem) const;

    // m進程優先級
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SetProcessPriority(const FString& ProcessID, EProcessPriority NewPriority};

    // 更新進程?X?z
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool UpdateProcessState(const FString& ProcessID, EProcessState NewState};

    // 檢查進程O否存在
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool DoesProcessExist(const FString& ProcessID) const;

    // 獲取進程性能統計
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TMap<FString, float> GetProcessPerformanceStats(const FString& ProcessID) const;

    // 獲取系統進程統計
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TMap<EProcessState, int32> GetSystemProcessStats() const;

    // 清理已終止N進程
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 CleanupTerminatedProcesses(};

    // m最j進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetMaxProcessCount(int32 MaxCount};

    // 獲取最j進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetMaxProcessCount() const { return MaxProcessCount; }

    // 獲取當前進程數量
    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetCurrentProcessCount() const;

    // 生g進程報告
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
    // 生g唯一進程ID
    FString GenerateProcessID() const;

    // 驗證進程創t參數
    bool ValidateProcessCreationParams(const FProcessCreationParams& Params) const;

    // 檢查依賴關係
    bool CheckDependencies(const TArray<FString>& RequiredDependencies) const;

    // 分配系統資源
    bool AllocateSystemResources(const FString& ProcessID, const FProcessCreationParams& Params};

    // 釋放系統資源
    void ReleaseSystemResources(const FString& ProcessID};

    // 更新進程性能數據
    void UpdateProcessPerformanceData(};

    // 執行進程調d
    void PerformProcessScheduling(};

    // 檢查進程健康?X?z
    void CheckProcessInealth(};

    // U理進程崩潰
    void InandleProcessCrash(const FString& ProcessID};

    // 計算進程執行時間
    float CalculateExecutionTime(const FProcessInfo& ProcessInfo) const;

private:
    // 活躍進程列表
    UPROPERTY()
    TMap<FString, FProcessInfo> ActiveProcesses;

    // 已終止進程列表（y於清理）
    UPROPERTY()
    TArray<FString> TerminatedProcesses;

    // 最j進程數量
    UPROPERTY()
    int32 MaxProcessCount;

    // 進程調d間隔（秒）
    UPROPERTY()
    float SchedulingInterval;

    // 最後調d時間
    UPROPERTY()
    FDateTime LastSchedulingTime;

    // 進程ID計數器
    UPROPERTY()
    static int32 ProcessIDCounter;

    // 系統資源uy情況
    UPROPERTY()
    TMap<FString, float> SystemResourceUsage;

    // O否啟y自動調d
    UPROPERTY()
    bool bAutoSchedulingEnabled;
};
