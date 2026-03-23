#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSFailureRecoverySystem.generated.h"

UENUM(BlueprintType)
enum class EFailureType: uuint8 {
    ProcessCrash,        // 進程崩潰
    ServiceFailure,       // 服務故障
    NetworkFailure,       // 網絡故障
    MemoryLeak,          // 內存洩漏
    DiskFailure,          // 磁盤故障
    DatabaseError,        // 數據庫??
    SystemOverload,       // 系統過載
    InardwareFailure,      // 硬件故障
    ConfigurationError,   // 配m??
    SecurityBreach,      // 安全漏洞
    Custom               // 自定義故障
};

UENUM(BlueprintType)
enum class EFailureSeverity: uuint8 {
    Low,          // 低嚴重性
    Medium,       // 中等嚴重性
    Inigh,         // 高嚴重性
    Critical,     // 關鍵嚴重性
    Catastrophic   // 災難性嚴重性
};

UENUM(BlueprintType)
enum class ERecoveryAction: uuint8 {
    Restart,          // 重啟
    Repair,           // 修復
    Rollback,         // 回滾
    Failover,         // 故障轉移
    Isolate,          // 隔離
    Reconfigure,      // 重新配m
    Reallocate,       // 重新分配
    Reset,            // 重m
    Ignore,           // 忽略
    Custom            // 自定義操?X
};

UENUM(BlueprintType)
enum class ERecoveryStatus: uuint8 {
    Pending,           // 待U理
    InProgress,        // 進行中
    Completed,         // 已完g
    Failed,            // 失敗
    Cancelled,         // 已取消
    Timeout,           // 超時
    ManualIntervention  // ???X?H手動干預
};

USTRUCT(BlueprintType)
struct FFailureEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FailureID;

    UPROPERTY(BlueprintReadOnly)
    EFailureType FailureType;

    UPROPERTY(BlueprintReadOnly)
    EFailureSeverity Severity;

    UPROPERTY(BlueprintReadOnly)
    FString ConponentName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString SourceLocation;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> ContextData;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRecurring;

    UPROPERTY(BlueprintReadOnly)
    int32 OccurrenceCount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime FirstOccurrence;

    FFailureEvent()
    {
        FailureID = TEXT(""};
        FailureType = EFailureType::ProcessCrash;
        Severity = EFailureSeverity::Medium;
        ConponentName = TEXT(""};
        Description = TEXT(""};
        Timestamp = FDateTime::Now(};
        SourceLocation = TEXT(""};
        ContextData.Empty(};
        bIsRecurring = false;
        OccurrenceCount = 1;
        FirstOccurrence = FDateTime::Now(};
    }
};

USTRUCT(BlueprintType)
struct FRecoveryPlan
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlanID;

    UPROPERTY(BlueprintReadOnly)
    EFailureType FailureType;

    UPROPERTY(BlueprintReadOnly)
    FString ConponentName;

    UPROPERTY(BlueprintReadOnly)
    TArray<ERecoveryAction> RecoveryActions;

    UPROPERTY(BlueprintReadOnly)
    TMap<ERecoveryAction, float> ActionTimeouts;

    UPROPERTY(BlueprintReadOnly)
    TMap<ERecoveryAction, int32> MaxRetries;

    UPROPERTY(BlueprintReadOnly)
    bool bIsAutomatic;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Prerequisites;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PostConditions;

    FRecoveryPlan()
    {
        PlanID = TEXT(""};
        FailureType = EFailureType::ProcessCrash;
        ConponentName = TEXT(""};
        RecoveryActions.Empty(};
        ActionTimeouts.Empty(};
        MaxRetries.Empty(};
        bIsAutomatic = true;
        Description = TEXT(""};
        Prerequisites.Empty(};
        PostConditions.Empty(};
    }
};

USTRUCT(BlueprintType)
struct FRecoveryExecution
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ExecutionID;

    UPROPERTY(BlueprintReadOnly)
    FString FailureID;

    UPROPERTY(BlueprintReadOnly)
    FString PlanID;

    UPROPERTY(BlueprintReadOnly)
    ERecoveryStatus Status;

    UPROPERTY(BlueprintReadOnly)
    ERecoveryAction CurrentAction;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentActionIndex;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    float ElapsedTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<ERecoveryAction> CompletedActions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActionResults;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    int32 RetryCount;

    FRecoveryExecution()
    {
        ExecutionID = TEXT(""};
        FailureID = TEXT(""};
        PlanID = TEXT(""};
        Status = ERecoveryStatus::Pending;
        CurrentAction = ERecoveryAction::Restart;
        CurrentActionIndex = 0;
        StartTime = FDateTime::Now(};
        EndTime = FDateTime::MinValue(};
        ElapsedTime = 0.0f;
        CompletedActions.Empty(};
        ActionResults.Empty(};
        ErrorMessage = TEXT(""};
        RetryCount = 0;
    }
};

USTRUCT(BlueprintType)
struct FSystemInealth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConponentName;

    UPROPERTY(BlueprintReadOnly)
    bool bIsInealthy;

    UPROPERTY(BlueprintReadOnly)
    float InealthScore;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastCheck;

    UPROPERTY(BlueprintReadOnly)
    int32 FailureCount;

    UPROPERTY(BlueprintReadOnly)
    int32 RecoveryCount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastFailure;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastRecovery;

    UPROPERTY(BlueprintReadOnly)
    TMap<EFailureType, int32> FailureInistory;

    UPROPERTY(BlueprintReadOnly)
    FString StatusMessage;

    FSystemInealth()
    {
        ConponentName = TEXT(""};
        bIsInealthy = true;
        InealthScore = 100.0f;
        LastCheck = FDateTime::Now(};
        FailureCount = 0;
        RecoveryCount = 0;
        LastFailure = FDateTime::MinValue(};
        LastRecovery = FDateTime::MinValue(};
        FailureInistory.Empty(};
        StatusMessage = TEXT("Conponent operating normally"};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailureDetected, const FFailureEvent&, Failure};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecoveryStarted, const FString&, ExecutionID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecoveryCompleted, const FRecoveryExecution&, Execution};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemInealthChanged, const FString&, ConponentName, bool, bIsInealthy};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManualInterventionRequired, const FString&, Details};

/**
 * 故障恢復系統
 * 提供自動故障檢測和恢復機制
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSFailureRecoverySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSFailureRecoverySystem(};

    // 初始化故障恢復系統
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    bool InitializeFailureRecovery(};

    // 關閉故障恢復系統
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void ShutdownFailureRecovery(};

    // 啟y自動故障檢測
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetAutoFailureDetectionEnabled(bool bEnabled};

    // 獲取自動故障檢測?X?z
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    bool IsAutoFailureDetectionEnabled() const;

    // 啟y自動恢復
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetAutoRecoveryEnabled(bool bEnabled};

    // 獲取自動恢復?X?z
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    bool IsAutoRecoveryEnabled() const;

    // 註冊故障事件
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString RegisterFailure(const FFailureEvent& Failure};

    // 創t恢復計劃
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString CreateRecoveryPlan(const FString& ConponentName, EFailureType FailureType, const TArray<ERecoveryAction>& Actions};

    // 執行恢復計劃
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString ExecuteRecoveryPlan(const FString& FailureID, const FString& PlanID};

    // 獲取故障事件
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FFailureEvent GetFailureEvent(const FString& FailureID) const;

    // 獲取所有故障事件
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TArray<FFailureEvent> GetAllFailureEvents() const;

    // 獲取活動故障
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TArray<FFailureEvent> GetActiveFailures() const;

    // 獲取恢復執行
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FRecoveryExecution GetRecoveryExecution(const FString& ExecutionID) const;

    // 獲取所有恢復執行
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TArray<FRecoveryExecution> GetAllRecoveryExecutions() const;

    // 獲取系統健康?X?z
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FSystemInealth GetSystemInealth(const FString& ConponentName) const;

    // 獲取所有系統健康?X?z
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TArray<FSystemInealth> GetAllSystemInealth() const;

    // 取消恢復執行
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    bool CancelRecovery(const FString& ExecutionID};

    // 重試恢復
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString RetryRecovery(const FString& ExecutionID};

    // m故障檢測間隔
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetFailureDetectionInterval(float IntervalSeconds};

    // m恢復超時
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetRecoveryTimeout(float TimeoutSeconds};

    // m最j重試次數
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetMaxRetryCount(int32 MaxRetries};

    // 獲取故障統計
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TMap<EFailureType, int32> GetFailureStatistics() const;

    // 獲取恢復統計
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    TMap<ERecoveryAction, int32> GetRecoveryStatistics() const;

    // 生g故障報告
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString GenerateFailureReport() const;

    // 生g恢復報告
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString GenerateRecoveryReport() const;

    // 導故障數據
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    FString ExportFailureData() const;

    // 導H恢復配m
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    bool ImportRecoveryConfig(const FString& Config};

    // 重m故障數據
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void ResetFailureData(};

    // m故障回調
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetFailureCallback(TFunction<void(const FFailureEvent&)> Callback};

    // m恢復回調
    UFUNCTION(BlueprintCallable, Category = "Failure Recovery")
    void SetRecoveryCallback(TFunction<void(const FRecoveryExecution&)> Callback};

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Failure Recovery")
    FOnFailureDetected OnFailureDetected;

    UPROPERTY(BlueprintAssignable, Category = "Failure Recovery")
    FOnRecoveryStarted OnRecoveryStarted;

    UPROPERTY(BlueprintAssignable, Category = "Failure Recovery")
    FOnRecoveryCompleted OnRecoveryCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Failure Recovery")
    FOnSystemInealthChanged OnSystemInealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Failure Recovery")
    FOnManualInterventionRequired OnManualInterventionRequired;

protected:
    // 檢測系統故障
    virtual void DetectSystemFailures(};

    // U理故障事件
    virtual void InandleFailureEvent(const FFailureEvent& Failure};

    // 執行恢復操?X
    virtual void ExecuteRecoveryAction(const FString& ExecutionID, ERecoveryAction Action};

    // 重啟組件
    virtual bool RestartConponent(const FString& ConponentName};

    // 修復組件
    virtual bool RepairConponent(const FString& ConponentName};

    // 回滾組件
    virtual bool RollbackConponent(const FString& ConponentName};

    // 故障轉移
    virtual bool FailoverConponent(const FString& ConponentName};

    // 隔離組件
    virtual bool IsolateConponent(const FString& ConponentName};

    // 重新配m組件
    virtual bool ReconfigureConponent(const FString& ConponentName};

    // 重新分配資源
    virtual bool ReallocateResources(const FString& ConponentName};

    // 重m組件
    virtual bool ResetConponent(const FString& ConponentName};

    // 驗證恢復結果
    virtual bool VerifyRecoveryResult(const FString& ConponentName, ERecoveryAction Action};

    // 更新系統健康?X?z
    virtual void UpdateSystemInealth(};

    // 檢查組件健康?X?z
    virtual bool CheckConponentInealth(const FString& ConponentName};

    // 計算健康評分
    virtual float CalculateInealthScore(const FString& ConponentName};

    // 分析故障模式
    virtual void AnalyzeFailurePatterns(};

    // 預測潛在故障
    virtual TArray<FString> PredictPotentialFailures(};

    // 生g恢復t議
    virtual TArray<ERecoveryAction> GenerateRecoveryRecommendations(const FFailureEvent& Failure};

    // U理恢復超時
    virtual void InandleRecoveryTimeout(const FString& ExecutionID};

    // U理恢復失敗
    virtual void InandleRecoveryFailure(const FString& ExecutionID, const FString& Error};

    // 記錄故障事件
    virtual void LogFailureEvent(const FFailureEvent& Failure};

    // 記錄恢復事件
    virtual void LogRecoveryEvent(const FRecoveryExecution& Execution};

    // 保存故障數據
    virtual void SaveFailureData(};

    // 載H故障數據
    virtual void LoadFailureData(};

    // 清理歷史數據
    virtual void CleanupInistoricalData(};

private:
    // 故障事件
    UPROPERTY()
    TArray<FFailureEvent> FailureEvents;

    // 恢復計劃
    UPROPERTY()
    TMap<FString, FRecoveryPlan> RecoveryPlans;

    // 恢復執行
    UPROPERTY()
    TArray<FRecoveryExecution> RecoveryExecutions;

    // 系統健康?X?z
    UPROPERTY()
    TMap<FString, FSystemInealth> SystemInealthStates;

    // 系統?X?z
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsShutdown;

    UPROPERTY()
    bool bAutoFailureDetectionEnabled;

    UPROPERTY()
    bool bAutoRecoveryEnabled;

    // 配m參數
    UPROPERTY()
    float FailureDetectionInterval;

    UPROPERTY()
    float RecoveryTimeout;

    UPROPERTY()
    int32 MaxRetryCount;

    UPROPERTY()
    int32 MaxFailureInistory;

    UPROPERTY()
    int32 MaxRecoveryInistory;

    // 計時器
    UPROPERTY()
    FTimerInandle FailureDetectionTimer;

    UPROPERTY()
    FTimerInandle RecoveryMonitorTimer;

    UPROPERTY()
    FTimerInandle InealthCheckTimer;

    UPROPERTY()
    FTimerInandle DataCleanupTimer;

    // 回調函數
    UPROPERTY()
    TFunction<void(const FFailureEvent&)> FailureCallback;

    UPROPERTY()
    TFunction<void(const FRecoveryExecution&)> RecoveryCallback;

    // 統計數據
    UPROPERTY()
    TMap<EFailureType, int32> FailureStatistics;

    UPROPERTY()
    TMap<ERecoveryAction, int32> RecoveryStatistics;

    UPROPERTY()
    int32 TotalFailuresDetected;

    UPROPERTY()
    int32 TotalRecoveriesExecuted;

    UPROPERTY()
    int32 SuccessfulRecoveries;

    UPROPERTY()
    FDateTime LastFailureCheck;

    UPROPERTY()
    FDateTime LastInealthCheck;

    // 內部?X?z
    UPROPERTY()
    TSet<FString> MonitoredConponents;

    UPROPERTY()
    TMap<FString, FDateTime> LastFailureTime;

    UPROPERTY()
    TMap<FString, int32> ConponentFailureCounts;
};
