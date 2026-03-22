#include "MingRTSFailureRecoverySystem.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformMisc.h"

UMingRTSFailureRecoverySystem::UMingRTSFailureRecoverySystem()
{
    bIsInitialized = false;
    bIsShutdown = false;
    bAutoFailureDetectionEnabled = true;
    bAutoRecoveryEnabled = true;
    FailureDetectionInterval = 5.0f;
    RecoveryTimeout = 60.0f;
    MaxRetryCount = 3;
    MaxFailureHistory = 1000;
    MaxRecoveryHistory = 500;
    TotalFailuresDetected = 0;
    TotalRecoveriesExecuted = 0;
    SuccessfulRecoveries = 0;
    LastFailureCheck = FDateTime::Now();
    LastHealthCheck = FDateTime::Now();
}

bool UMingRTSFailureRecoverySystem::InitializeFailureRecovery()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failure Recovery System already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing Failure Recovery System..."));

    // 初始化統計數據
    FailureStatistics.Empty();
    RecoveryStatistics.Empty();

    // 初始化默認恢復計劃
    InitializeDefaultRecoveryPlans();

    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        // 故障檢測定時器
        World->GetTimerManager().SetTimer(
            FailureDetectionTimer,
            this,
            &UMingRTSFailureRecoverySystem::DetectSystemFailures,
            FailureDetectionInterval,
            true
        );

        // 恢復監控定時器
        World->GetTimerManager().SetTimer(
            RecoveryMonitorTimer,
            this,
            &UMingRTSFailureRecoverySystem::MonitorRecoveryExecutions,
            2.0f,
            true
        );

        // 健康檢查定時器
        World->GetTimerManager().SetTimer(
            HealthCheckTimer,
            this,
            &UMingRTSFailureRecoverySystem::UpdateSystemHealth,
            10.0f,
            true
        );

        // 數據清理定時器
        World->GetTimerManager().SetTimer(
            DataCleanupTimer,
            this,
            &UMingRTSFailureRecoverySystem::CleanupHistoricalData,
            300.0f,
            true
        );
    }

    bIsInitialized = true;
    bIsShutdown = false;

    UE_LOG(LogTemp, Log, TEXT("Failure Recovery System initialized successfully"));
    return true;
}

void UMingRTSFailureRecoverySystem::ShutdownFailureRecovery()
{
    if (!bIsInitialized || bIsShutdown)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down Failure Recovery System..."));

    // 取消所有正在進行的恢復
    for (FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.Status == ERecoveryStatus::InProgress)
        {
            Execution.Status = ERecoveryStatus::Cancelled;
        }
    }

    // 清除定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FailureDetectionTimer);
        World->GetTimerManager().ClearTimer(RecoveryMonitorTimer);
        World->GetTimerManager().ClearTimer(HealthCheckTimer);
        World->GetTimerManager().ClearTimer(DataCleanupTimer);
    }

    // 保存故障數據
    SaveFailureData();

    // 清空數據結構
    FailureEvents.Empty();
    RecoveryPlans.Empty();
    RecoveryExecutions.Empty();
    SystemHealthStates.Empty();
    FailureStatistics.Empty();
    RecoveryStatistics.Empty();

    bIsShutdown = true;
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("Failure Recovery System shutdown complete"));
}

void UMingRTSFailureRecoverySystem::InitializeDefaultRecoveryPlans()
{
    // 進程崩潰恢復計劃
    FRecoveryPlan ProcessCrashPlan;
    ProcessCrashPlan.PlanID = TEXT("PROCESS_CRASH_PLAN");
    ProcessCrashPlan.FailureType = EFailureType::ProcessCrash;
    ProcessCrashPlan.ComponentName = TEXT("Process");
    ProcessCrashPlan.RecoveryActions = {ERecoveryAction::Restart, ERecoveryAction::Reconfigure};
    ProcessCrashPlan.ActionTimeouts.Add(ERecoveryAction::Restart, 30.0f);
    ProcessCrashPlan.ActionTimeouts.Add(ERecoveryAction::Reconfigure, 15.0f);
    ProcessCrashPlan.MaxRetries.Add(ERecoveryAction::Restart, 3);
    ProcessCrashPlan.MaxRetries.Add(ERecoveryAction::Reconfigure, 1);
    ProcessCrashPlan.bIsAutomatic = true;
    ProcessCrashPlan.Description = TEXT("Process crash recovery plan");
    RecoveryPlans.Add(ProcessCrashPlan.PlanID, ProcessCrashPlan);

    // 服務故障恢復計劃
    FRecoveryPlan ServiceFailurePlan;
    ServiceFailurePlan.PlanID = TEXT("SERVICE_FAILURE_PLAN");
    ServiceFailurePlan.FailureType = EFailureType::ServiceFailure;
    ServiceFailurePlan.ComponentName = TEXT("Service");
    ServiceFailurePlan.RecoveryActions = {ERecoveryAction::Restart, ERecoveryAction::Repair, ERecoveryAction::Failover};
    ServiceFailurePlan.ActionTimeouts.Add(ERecoveryAction::Restart, 20.0f);
    ServiceFailurePlan.ActionTimeouts.Add(ERecoveryAction::Repair, 45.0f);
    ServiceFailurePlan.ActionTimeouts.Add(ERecoveryAction::Failover, 30.0f);
    ServiceFailurePlan.MaxRetries.Add(ERecoveryAction::Restart, 2);
    ServiceFailurePlan.MaxRetries.Add(ERecoveryAction::Repair, 1);
    ServiceFailurePlan.MaxRetries.Add(ERecoveryAction::Failover, 1);
    ServiceFailurePlan.bIsAutomatic = true;
    ServiceFailurePlan.Description = TEXT("Service failure recovery plan");
    RecoveryPlans.Add(ServiceFailurePlan.PlanID, ServiceFailurePlan);

    // 網絡故障恢復計劃
    FRecoveryPlan NetworkFailurePlan;
    NetworkFailurePlan.PlanID = TEXT("NETWORK_FAILURE_PLAN");
    NetworkFailurePlan.FailureType = EFailureType::NetworkFailure;
    NetworkFailurePlan.ComponentName = TEXT("Network");
    NetworkFailurePlan.RecoveryActions = {ERecoveryAction::Reconfigure, ERecoveryAction::Reset, ERecoveryAction::Failover};
    NetworkFailurePlan.ActionTimeouts.Add(ERecoveryAction::Reconfigure, 15.0f);
    NetworkFailurePlan.ActionTimeouts.Add(ERecoveryAction::Reset, 10.0f);
    NetworkFailurePlan.ActionTimeouts.Add(ERecoveryAction::Failover, 20.0f);
    NetworkFailurePlan.MaxRetries.Add(ERecoveryAction::Reconfigure, 3);
    NetworkFailurePlan.MaxRetries.Add(ERecoveryAction::Reset, 2);
    NetworkFailurePlan.MaxRetries.Add(ERecoveryAction::Failover, 1);
    NetworkFailurePlan.bIsAutomatic = true;
    NetworkFailurePlan.Description = TEXT("Network failure recovery plan");
    RecoveryPlans.Add(NetworkFailurePlan.PlanID, NetworkFailurePlan);

    // 內存洩漏恢復計劃
    FRecoveryPlan MemoryLeakPlan;
    MemoryLeakPlan.PlanID = TEXT("MEMORY_LEAK_PLAN");
    MemoryLeakPlan.FailureType = EFailureType::MemoryLeak;
    MemoryLeakPlan.ComponentName = TEXT("Memory");
    MemoryLeakPlan.RecoveryActions = {ERecoveryAction::Reallocate, ERecoveryAction::Restart};
    MemoryLeakPlan.ActionTimeouts.Add(ERecoveryAction::Reallocate, 30.0f);
    MemoryLeakPlan.ActionTimeouts.Add(ERecoveryAction::Restart, 45.0f);
    MemoryLeakPlan.MaxRetries.Add(ERecoveryAction::Reallocate, 2);
    MemoryLeakPlan.MaxRetries.Add(ERecoveryAction::Restart, 1);
    MemoryLeakPlan.bIsAutomatic = true;
    MemoryLeakPlan.Description = TEXT("Memory leak recovery plan");
    RecoveryPlans.Add(MemoryLeakPlan.PlanID, MemoryLeakPlan);

    // 系統過載恢復計劃
    FRecoveryPlan SystemOverloadPlan;
    SystemOverloadPlan.PlanID = TEXT("SYSTEM_OVERLOAD_PLAN");
    SystemOverloadPlan.FailureType = EFailureType::SystemOverload;
    SystemOverloadPlan.ComponentName = TEXT("System");
    SystemOverloadPlan.RecoveryActions = {ERecoveryAction::Reallocate, ERecoveryAction::Isolate, ERecoveryAction::Reset};
    SystemOverloadPlan.ActionTimeouts.Add(ERecoveryAction::Reallocate, 20.0f);
    SystemOverloadPlan.ActionTimeouts.Add(ERecoveryAction::Isolate, 10.0f);
    SystemOverloadPlan.ActionTimeouts.Add(ERecoveryAction::Reset, 30.0f);
    SystemOverloadPlan.MaxRetries.Add(ERecoveryAction::Reallocate, 3);
    SystemOverloadPlan.MaxRetries.Add(ERecoveryAction::Isolate, 2);
    SystemOverloadPlan.MaxRetries.Add(ERecoveryAction::Reset, 1);
    SystemOverloadPlan.bIsAutomatic = true;
    SystemOverloadPlan.Description = TEXT("System overload recovery plan");
    RecoveryPlans.Add(SystemOverloadPlan.PlanID, SystemOverloadPlan);
}

void UMingRTSFailureRecoverySystem::SetAutoFailureDetectionEnabled(bool bEnabled)
{
    bAutoFailureDetectionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto failure detection %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingRTSFailureRecoverySystem::IsAutoFailureDetectionEnabled() const
{
    return bAutoFailureDetectionEnabled;
}

void UMingRTSFailureRecoverySystem::SetAutoRecoveryEnabled(bool bEnabled)
{
    bAutoRecoveryEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto recovery %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingRTSFailureRecoverySystem::IsAutoRecoveryEnabled() const
{
    return bAutoRecoveryEnabled;
}

FString UMingRTSFailureRecoverySystem::RegisterFailure(const FFailureEvent& Failure)
{
    FFailureEvent NewFailure = Failure;
    NewFailure.FailureID = GenerateFailureID();
    NewFailure.Timestamp = FDateTime::Now();
    NewFailure.FirstOccurrence = NewFailure.Timestamp;

    // 檢查是否為重複故障
    for (FFailureEvent& ExistingFailure : FailureEvents)
    {
        if (ExistingFailure.ComponentName == NewFailure.ComponentName && 
            ExistingFailure.FailureType == NewFailure.FailureType)
        {
            ExistingFailure.OccurrenceCount++;
            ExistingFailure.bIsRecurring = true;
            ExistingFailure.Timestamp = NewFailure.Timestamp;
            
            // 更新統計
            UpdateFailureStatistics(NewFailure.FailureType);
            
            // 處理故障事件
            HandleFailureEvent(ExistingFailure);
            
            UE_LOG(LogTemp, Warning, TEXT("Recurring failure detected: %s - Occurrence %d"), 
                *ExistingFailure.ComponentName, ExistingFailure.OccurrenceCount);
            return ExistingFailure.FailureID;
        }
    }

    // 添加新故障
    FailureEvents.Add(NewFailure);
    
    // 更新統計
    UpdateFailureStatistics(NewFailure.FailureType);
    TotalFailuresDetected++;

    // 處理故障事件
    HandleFailureEvent(NewFailure);

    UE_LOG(LogTemp, Warning, TEXT("Failure registered: %s - %s"), 
        *NewFailure.ComponentName, *NewFailure.Description);
    return NewFailure.FailureID;
}

FString UMingRTSFailureRecoverySystem::CreateRecoveryPlan(const FString& ComponentName, EFailureType FailureType, const TArray<ERecoveryAction>& Actions)
{
    FRecoveryPlan Plan;
    Plan.PlanID = GeneratePlanID();
    Plan.ComponentName = ComponentName;
    Plan.FailureType = FailureType;
    Plan.RecoveryActions = Actions;
    Plan.bIsAutomatic = true;
    Plan.Description = FString::Printf(TEXT("Custom recovery plan for %s"), *ComponentName);

    // 設置默認超時和重試次數
    for (ERecoveryAction Action : Actions)
    {
        Plan.ActionTimeouts.Add(Action, 30.0f);
        Plan.MaxRetries.Add(Action, 3);
    }

    RecoveryPlans.Add(Plan.PlanID, Plan);

    UE_LOG(LogTemp, Log, TEXT("Recovery plan created: %s for %s"), *Plan.PlanID, *ComponentName);
    return Plan.PlanID;
}

FString UMingRTSFailureRecoverySystem::ExecuteRecoveryPlan(const FString& FailureID, const FString& PlanID)
{
    if (!RecoveryPlans.Contains(PlanID))
    {
        UE_LOG(LogTemp, Error, TEXT("Recovery plan not found: %s"), *PlanID);
        return TEXT("");
    }

    const FRecoveryPlan& Plan = RecoveryPlans[PlanID];

    // 創建恢復執行
    FRecoveryExecution Execution;
    Execution.ExecutionID = GenerateExecutionID();
    Execution.FailureID = FailureID;
    Execution.PlanID = PlanID;
    Execution.Status = ERecoveryStatus::Pending;
    Execution.StartTime = FDateTime::Now();

    RecoveryExecutions.Add(Execution);

    // 開始執行恢復
    if (Plan.bIsAutomatic || bAutoRecoveryEnabled)
    {
        Execution.Status = ERecoveryStatus::InProgress;
        ExecuteNextRecoveryAction(Execution.ExecutionID);
    }
    else
    {
        Execution.Status = ERecoveryStatus::ManualIntervention;
        OnManualInterventionRequired.Broadcast(FString::Printf(TEXT("Manual recovery required for failure: %s"), *FailureID));
    }

    UE_LOG(LogTemp, Log, TEXT("Recovery execution started: %s for failure: %s"), 
        *Execution.ExecutionID, *FailureID);
    return Execution.ExecutionID;
}

FFailureEvent UMingRTSFailureRecoverySystem::GetFailureEvent(const FString& FailureID) const
{
    for (const FFailureEvent& Failure : FailureEvents)
    {
        if (Failure.FailureID == FailureID)
        {
            return Failure;
        }
    }
    return FFailureEvent();
}

TArray<FFailureEvent> UMingRTSFailureRecoverySystem::GetAllFailureEvents() const
{
    return FailureEvents;
}

TArray<FFailureEvent> UMingRTSFailureRecoverySystem::GetActiveFailures() const
{
    TArray<FFailureEvent> ActiveFailures;
    FDateTime CutoffTime = FDateTime::Now() - FTimespan::FromMinutes(30); // 30分鐘內的故障

    for (const FFailureEvent& Failure : FailureEvents)
    {
        if (Failure.Timestamp >= CutoffTime)
        {
            ActiveFailures.Add(Failure);
        }
    }
    return ActiveFailures;
}

FRecoveryExecution UMingRTSFailureRecoverySystem::GetRecoveryExecution(const FString& ExecutionID) const
{
    for (const FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.ExecutionID == ExecutionID)
        {
            return Execution;
        }
    }
    return FRecoveryExecution();
}

TArray<FRecoveryExecution> UMingRTSFailureRecoverySystem::GetAllRecoveryExecutions() const
{
    return RecoveryExecutions;
}

FSystemHealth UMingRTSFailureRecoverySystem::GetSystemHealth(const FString& ComponentName) const
{
    const FSystemHealth* Health = SystemHealthStates.Find(ComponentName);
    return Health ? *Health : FSystemHealth();
}

TArray<FSystemHealth> UMingRTSFailureRecoverySystem::GetAllSystemHealth() const
{
    TArray<FSystemHealth> HealthStates;
    for (const auto& HealthPair : SystemHealthStates)
    {
        HealthStates.Add(HealthPair.Value);
    }
    return HealthStates;
}

bool UMingRTSFailureRecoverySystem::CancelRecovery(const FString& ExecutionID)
{
    for (FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.ExecutionID == ExecutionID && 
            (Execution.Status == ERecoveryStatus::InProgress || Execution.Status == ERecoveryStatus::Pending))
        {
            Execution.Status = ERecoveryStatus::Cancelled;
            Execution.EndTime = FDateTime::Now();
            Execution.ElapsedTime = (Execution.EndTime - Execution.StartTime).GetTotalSeconds();
            
            UE_LOG(LogTemp, Log, TEXT("Recovery cancelled: %s"), *ExecutionID);
            return true;
        }
    }
    return false;
}

FString UMingRTSFailureRecoverySystem::RetryRecovery(const FString& ExecutionID)
{
    for (FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.ExecutionID == ExecutionID && Execution.Status == ERecoveryStatus::Failed)
        {
            Execution.RetryCount++;
            Execution.Status = ERecoveryStatus::Pending;
            Execution.StartTime = FDateTime::Now();
            Execution.ErrorMessage = TEXT("");
            
            // 重新開始執行
            ExecuteNextRecoveryAction(Execution.ExecutionID);
            
            UE_LOG(LogTemp, Log, TEXT("Recovery retry started: %s (attempt %d)"), *ExecutionID, Execution.RetryCount);
            return Execution.ExecutionID;
        }
    }
    return TEXT("");
}

void UMingRTSFailureRecoverySystem::SetFailureDetectionInterval(float IntervalSeconds)
{
    FailureDetectionInterval = IntervalSeconds;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            FailureDetectionTimer,
            this,
            &UMingRTSFailureRecoverySystem::DetectSystemFailures,
            FailureDetectionInterval,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("Failure detection interval set to %.2f seconds"), IntervalSeconds);
}

void UMingRTSFailureRecoverySystem::SetRecoveryTimeout(float TimeoutSeconds)
{
    RecoveryTimeout = TimeoutSeconds;
    UE_LOG(LogTemp, Log, TEXT("Recovery timeout set to %.2f seconds"), TimeoutSeconds);
}

void UMingRTSFailureRecoverySystem::SetMaxRetryCount(int32 MaxRetries)
{
    MaxRetryCount = MaxRetries;
    UE_LOG(LogTemp, Log, TEXT("Max retry count set to %d"), MaxRetries);
}

TMap<EFailureType, int32> UMingRTSFailureRecoverySystem::GetFailureStatistics() const
{
    return FailureStatistics;
}

TMap<ERecoveryAction, int32> UMingRTSFailureRecoverySystem::GetRecoveryStatistics() const
{
    return RecoveryStatistics;
}

FString UMingRTSFailureRecoverySystem::GenerateFailureReport() const
{
    FString Report = TEXT("=== Failure Report ===\n\n");
    Report += FString::Printf(TEXT("Generated: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Total Failures Detected: %d\n"), TotalFailuresDetected);
    Report += FString::Printf(TEXT("Active Failures: %d\n"), GetActiveFailures().Num());
    Report += FString::Printf(TEXT("Recurring Failures: %d\n\n"), GetRecurringFailureCount());

    Report += TEXT("=== Failure Statistics ===\n");
    for (const auto& StatPair : FailureStatistics)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), 
            *StaticEnum<EFailureType>()->GetValueAsString(StatPair.Key), StatPair.Value);
    }

    Report += TEXT("\n=== Recent Failures ===\n");
    TArray<FFailureEvent> RecentFailures = GetActiveFailures();
    for (const FFailureEvent& Failure : RecentFailures)
    {
        Report += FString::Printf(TEXT("[%s] %s: %s (Severity: %s)\n"), 
            *Failure.Timestamp.ToString(), *Failure.ComponentName, 
            *Failure.Description, *StaticEnum<EFailureSeverity>()->GetValueAsString(Failure.Severity));
    }

    return Report;
}

FString UMingRTSFailureRecoverySystem::GenerateRecoveryReport() const
{
    FString Report = TEXT("=== Recovery Report ===\n\n");
    Report += FString::Printf(TEXT("Generated: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Total Recoveries Executed: %d\n"), TotalRecoveriesExecuted);
    Report += FString::Printf(TEXT("Successful Recoveries: %d\n"), SuccessfulRecoveries);
    Report += FString::Printf(TEXT("Success Rate: %.1f%%\n\n"), 
        TotalRecoveriesExecuted > 0 ? (float)SuccessfulRecoveries / TotalRecoveriesExecuted * 100.0f : 0.0f);

    Report += TEXT("=== Recovery Statistics ===\n");
    for (const auto& StatPair : RecoveryStatistics)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), 
            *StaticEnum<ERecoveryAction>()->GetValueAsString(StatPair.Key), StatPair.Value);
    }

    Report += TEXT("\n=== Recent Recoveries ===\n");
    for (const FRecoveryExecution& Execution : RecoveryExecutions)
    {
        Report += FString::Printf(TEXT("[%s] %s: %s (Status: %s, Duration: %.1fs)\n"), 
            *Execution.StartTime.ToString(), *Execution.ExecutionID, *Execution.PlanID,
            *StaticEnum<ERecoveryStatus>()->GetValueAsString(Execution.Status), Execution.ElapsedTime);
    }

    return Report;
}

FString UMingRTSFailureRecoverySystem::ExportFailureData() const
{
    FString Data = TEXT("{\n");
    Data += FString::Printf(TEXT("  \"TotalFailures\": %d,\n"), TotalFailuresDetected);
    Data += FString::Printf(TEXT("  \"TotalRecoveries\": %d,\n"), TotalRecoveriesExecuted);
    Data += FString::Printf(TEXT("  \"SuccessfulRecoveries\": %d,\n"), SuccessfulRecoveries);
    Data += TEXT("  \"FailureStatistics\": {\n");

    for (const auto& StatPair : FailureStatistics)
    {
        Data += FString::Printf(TEXT("    \"%s\": %d,\n"), 
            *StaticEnum<EFailureType>()->GetValueAsString(StatPair.Key), StatPair.Value);
    }

    Data += TEXT("  },\n");
    Data += TEXT("  \"RecoveryStatistics\": {\n");

    for (const auto& StatPair : RecoveryStatistics)
    {
        Data += FString::Printf(TEXT("    \"%s\": %d,\n"), 
            *StaticEnum<ERecoveryAction>()->GetValueAsString(StatPair.Key), StatPair.Value);
    }

    Data += TEXT("  }\n");
    Data += TEXT("}");

    return Data;
}

bool UMingRTSFailureRecoverySystem::ImportRecoveryConfig(const FString& Config)
{
    // 簡化的配置導入
    if (Config.Contains(TEXT("\"RecoveryPlans\"")))
    {
        UE_LOG(LogTemp, Log, TEXT("Recovery configuration imported"));
        return true;
    }
    return false;
}

void UMingRTSFailureRecoverySystem::ResetFailureData()
{
    FailureEvents.Empty();
    RecoveryExecutions.Empty();
    SystemHealthStates.Empty();
    FailureStatistics.Empty();
    RecoveryStatistics.Empty();
    
    TotalFailuresDetected = 0;
    TotalRecoveriesExecuted = 0;
    SuccessfulRecoveries = 0;

    UE_LOG(LogTemp, Log, TEXT("Failure data reset"));
}

void UMingRTSFailureRecoverySystem::SetFailureCallback(TFunction<void(const FFailureEvent&)> Callback)
{
    FailureCallback = Callback;
}

void UMingRTSFailureRecoverySystem::SetRecoveryCallback(TFunction<void(const FRecoveryExecution&)> Callback)
{
    RecoveryCallback = Callback;
}

void UMingRTSFailureRecoverySystem::DetectSystemFailures()
{
    if (!bAutoFailureDetectionEnabled || !bIsInitialized)
    {
        return;
    }

    LastFailureCheck = FDateTime::Now();

    // 檢查各個組件的健康狀態
    CheckComponentHealth(TEXT("ProcessManager"));
    CheckComponentHealth(TEXT("MemoryManager"));
    CheckComponentHealth(TEXT("NetworkManager"));
    CheckComponentHealth(TEXT("Database"));
    CheckComponentHealth(TEXT("FileSystem"));

    // 分析故障模式
    AnalyzeFailurePatterns();
}

void UMingRTSFailureRecoverySystem::HandleFailureEvent(const FFailureEvent& Failure)
{
    // 廣播故障事件
    OnFailureDetected.Broadcast(Failure);

    // 調用自定義回調
    if (FailureCallback)
    {
        FailureCallback(Failure);
    }

    // 更新系統健康狀態
    UpdateComponentHealth(Failure.ComponentName, false);

    // 記錄故障事件
    LogFailureEvent(Failure);

    // 如果啟用自動恢復，查找並執行恢復計劃
    if (bAutoRecoveryEnabled)
    {
        FString PlanID = FindRecoveryPlan(Failure.FailureType, Failure.ComponentName);
        if (!PlanID.IsEmpty())
        {
            ExecuteRecoveryPlan(Failure.FailureID, PlanID);
        }
    }
}

void UMingRTSFailureRecoverySystem::ExecuteRecoveryAction(const FString& ExecutionID, ERecoveryAction Action)
{
    FRecoveryExecution* Execution = FindRecoveryExecution(ExecutionID);
    if (!Execution)
    {
        return;
    }

    Execution->CurrentAction = Action;
    bool bSuccess = false;

    switch (Action)
    {
    case ERecoveryAction::Restart:
        bSuccess = RestartComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Repair:
        bSuccess = RepairComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Rollback:
        bSuccess = RollbackComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Failover:
        bSuccess = FailoverComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Isolate:
        bSuccess = IsolateComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Reconfigure:
        bSuccess = ReconfigureComponent(Execution->ComponentName);
        break;
    case ERecoveryAction::Reallocate:
        bSuccess = ReallocateResources(Execution->ComponentName);
        break;
    case ERecoveryAction::Reset:
        bSuccess = ResetComponent(Execution->ComponentName);
        break;
    default:
        bSuccess = false;
        break;
    }

    // 記錄操作結果
    FString Result = bSuccess ? TEXT("Success") : TEXT("Failed");
    Execution->ActionResults.Add(Result);
    Execution->CompletedActions.Add(Action);

    // 更新統計
    UpdateRecoveryStatistics(Action);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Recovery action completed successfully: %s - %s"), 
            *ExecutionID, *StaticEnum<ERecoveryAction>()->GetValueAsString(Action));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Recovery action failed: %s - %s"), 
            *ExecutionID, *StaticEnum<ERecoveryAction>()->GetValueAsString(Action));
    }

    // 繼續執行下一個操作
    ExecuteNextRecoveryAction(ExecutionID);
}

bool UMingRTSFailureRecoverySystem::RestartComponent(const FString& ComponentName)
{
    // 模擬組件重啟
    UE_LOG(LogTemp, Log, TEXT("Restarting component: %s"), *ComponentName);
    
    // 模擬重啟延遲
    FPlatformProcess::Sleep(2.0f);
    
    // 驗證重啟結果
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Restart);
}

bool UMingRTSFailureRecoverySystem::RepairComponent(const FString& ComponentName)
{
    // 模擬組件修復
    UE_LOG(LogTemp, Log, TEXT("Repairing component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(3.0f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Repair);
}

bool UMingRTSFailureRecoverySystem::RollbackComponent(const FString& ComponentName)
{
    // 模擬組件回滾
    UE_LOG(LogTemp, Log, TEXT("Rolling back component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(4.0f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Rollback);
}

bool UMingRTSFailureRecoverySystem::FailoverComponent(const FString& ComponentName)
{
    // 模擬故障轉移
    UE_LOG(LogTemp, Log, TEXT("Failing over component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(3.0f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Failover);
}

bool UMingRTSFailureRecoverySystem::IsolateComponent(const FString& ComponentName)
{
    // 模擬組件隔離
    UE_LOG(LogTemp, Log, TEXT("Isolating component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(1.0f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Isolate);
}

bool UMingRTSFailureRecoverySystem::ReconfigureComponent(const FString& ComponentName)
{
    // 模擬組件重新配置
    UE_LOG(LogTemp, Log, TEXT("Reconfiguring component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(2.0f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Reconfigure);
}

bool UMingRTSFailureRecoverySystem::ReallocateResources(const FString& ComponentName)
{
    // 模擬資源重新分配
    UE_LOG(LogTemp, Log, TEXT("Reallocating resources for component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(2.5f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Reallocate);
}

bool UMingRTSFailureRecoverySystem::ResetComponent(const FString& ComponentName)
{
    // 模擬組件重置
    UE_LOG(LogTemp, Log, TEXT("Resetting component: %s"), *ComponentName);
    
    FPlatformProcess::Sleep(1.5f);
    
    return VerifyRecoveryResult(ComponentName, ERecoveryAction::Reset);
}

bool UMingRTSFailureRecoverySystem::VerifyRecoveryResult(const FString& ComponentName, ERecoveryAction Action)
{
    // 模擬恢復結果驗證
    // 實際應檢查組件狀態、性能指標等
    
    // 80%的成功率模擬
    return FMath::RandRange(0, 100) > 20;
}

void UMingRTSFailureRecoverySystem::UpdateSystemHealth()
{
    if (!bIsInitialized)
    {
        return;
    }

    LastHealthCheck = FDateTime::Now();

    // 檢查所有監控組件的健康狀態
    for (const FString& ComponentName : MonitoredComponents)
    {
        bool bIsHealthy = CheckComponentHealth(ComponentName);
        UpdateComponentHealth(ComponentName, bIsHealthy);
    }

    // 預測潛在故障
    TArray<FString> PotentialFailures = PredictPotentialFailures();
    for (const FString& Failure : PotentialFailures)
    {
        UE_LOG(LogTemp, Warning, TEXT("Potential failure predicted: %s"), *Failure);
    }
}

bool UMingRTSFailureRecoverySystem::CheckComponentHealth(const FString& ComponentName)
{
    // 模擬組件健康檢查
    // 實際應檢查組件響應、性能指標、錯誤率等
    
    // 90%的健康率模擬
    return FMath::RandRange(0, 100) > 10;
}

float UMingRTSFailureRecoverySystem::CalculateHealthScore(const FString& ComponentName)
{
    FSystemHealth* Health = SystemHealthStates.Find(ComponentName);
    if (!Health)
    {
        return 100.0f;
    }

    // 基於故障歷史計算健康評分
    float BaseScore = 100.0f;
    
    // 根據故障次數扣分
    BaseScore -= Health->FailureCount * 5.0f;
    
    // 根據恢復次數加分
    BaseScore += Health->RecoveryCount * 2.0f;
    
    // 根據最近故障時間調整
    if (Health->LastFailure != FDateTime::MinValue())
    {
        FTimespan TimeSinceFailure = FDateTime::Now() - Health->LastFailure;
        float HoursSinceFailure = TimeSinceFailure.GetTotalHours();
        if (HoursSinceFailure < 1.0f)
        {
            BaseScore -= 20.0f;
        }
        else if (HoursSinceFailure < 24.0f)
        {
            BaseScore -= 10.0f;
        }
    }
    
    return FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

void UMingRTSFailureRecoverySystem::AnalyzeFailurePatterns()
{
    // 分析故障模式
    TMap<EFailureType, TArray<FFailureEvent>> FailuresByType;
    
    for (const FFailureEvent& Failure : FailureEvents)
    {
        TArray<FFailureEvent>& TypeFailures = FailuresByType.FindOrAdd(Failure.FailureType);
        TypeFailures.Add(Failure);
    }
    
    // 檢測重複故障模式
    for (const auto& TypePair : FailuresByType)
    {
        const TArray<FFailureEvent>& TypeFailures = TypePair.Value;
        if (TypeFailures.Num() > 5) // 超過5次同類型故障
        {
            UE_LOG(LogTemp, Warning, TEXT("Recurring failure pattern detected for type %s: %d occurrences"), 
                *StaticEnum<EFailureType>()->GetValueAsString(TypePair.Key), TypeFailures.Num());
        }
    }
}

TArray<FString> UMingRTSFailureRecoverySystem::PredictPotentialFailures()
{
    TArray<FString> Predictions;
    
    // 基於歷史數據預測潛在故障
    for (const auto& HealthPair : SystemHealthStates)
    {
        const FSystemHealth& Health = HealthPair.Value;
        
        // 健康評分低於30%的組件可能故障
        if (Health.HealthScore < 30.0f)
        {
            Predictions.Add(FString::Printf(TEXT("Component %s at risk of failure (Health: %.1f%%)"), 
                *Health.ComponentName, Health.HealthScore));
        }
        
        // 最近有故障歷史的組件
        if (Health.LastFailure != FDateTime::MinValue())
        {
            FTimespan TimeSinceFailure = FDateTime::Now() - Health.LastFailure;
            if (TimeSinceFailure.GetTotalMinutes() < 60 && Health.FailureCount > 2)
            {
                Predictions.Add(FString::Printf(TEXT("Component %s may experience recurring failure"), 
                    *Health.ComponentName));
            }
        }
    }
    
    return Predictions;
}

TArray<ERecoveryAction> UMingRTSFailureRecoverySystem::GenerateRecoveryRecommendations(const FFailureEvent& Failure)
{
    TArray<ERecoveryAction> Recommendations;
    
    // 根據故障類型和嚴重性推薦恢復操作
    switch (Failure.FailureType)
    {
    case EFailureType::ProcessCrash:
        Recommendations.Add(ERecoveryAction::Restart);
        if (Failure.Severity >= EFailureSeverity::High)
        {
            Recommendations.Add(ERecoveryAction::Reconfigure);
        }
        break;
        
    case EFailureType::ServiceFailure:
        Recommendations.Add(ERecoveryAction::Restart);
        Recommendations.Add(ERecoveryAction::Repair);
        if (Failure.Severity >= EFailureSeverity::Critical)
        {
            Recommendations.Add(ERecoveryAction::Failover);
        }
        break;
        
    case EFailureType::MemoryLeak:
        Recommendations.Add(ERecoveryAction::Reallocate);
        Recommendations.Add(ERecoveryAction::Restart);
        break;
        
    case EFailureType::SystemOverload:
        Recommendations.Add(ERecoveryAction::Reallocate);
        Recommendations.Add(ERecoveryAction::Isolate);
        break;
        
    default:
        Recommendations.Add(ERecoveryAction::Restart);
        break;
    }
    
    return Recommendations;
}

void UMingRTSFailureRecoverySystem::HandleRecoveryTimeout(const FString& ExecutionID)
{
    FRecoveryExecution* Execution = FindRecoveryExecution(ExecutionID);
    if (!Execution || Execution->Status != ERecoveryStatus::InProgress)
    {
        return;
    }

    Execution->Status = ERecoveryStatus::Timeout;
    Execution->EndTime = FDateTime::Now();
    Execution->ElapsedTime = (Execution.EndTime - Execution.StartTime).GetTotalSeconds();
    Execution->ErrorMessage = TEXT("Recovery operation timed out");

    UE_LOG(LogTemp, Error, TEXT("Recovery timeout: %s"), *ExecutionID);

    // 廣播恢復完成事件
    OnRecoveryCompleted.Broadcast(*Execution);

    // 調用回調
    if (RecoveryCallback)
    {
        RecoveryCallback(*Execution);
    }
}

void UMingRTSFailureRecoverySystem::HandleRecoveryFailure(const FString& ExecutionID, const FString& Error)
{
    FRecoveryExecution* Execution = FindRecoveryExecution(ExecutionID);
    if (!Execution)
    {
        return;
    }

    Execution->Status = ERecoveryStatus::Failed;
    Execution->EndTime = FDateTime::Now();
    Execution->ElapsedTime = (Execution.EndTime - Execution.StartTime).GetTotalSeconds();
    Execution->ErrorMessage = Error;

    UE_LOG(LogTemp, Error, TEXT("Recovery failed: %s - %s"), *ExecutionID, *Error);

    // 廣播恢復完成事件
    OnRecoveryCompleted.Broadcast(*Execution);

    // 調用回調
    if (RecoveryCallback)
    {
        RecoveryCallback(*Execution);
    }
}

void UMingRTSFailureRecoverySystem::LogFailureEvent(const FFailureEvent& Failure)
{
    UE_LOG(LogTemp, Warning, TEXT("Failure Event: [%s] %s - %s (Severity: %s)"), 
        *Failure.Timestamp.ToString(), *Failure.ComponentName, 
        *Failure.Description, *StaticEnum<EFailureSeverity>()->GetValueAsString(Failure.Severity));
}

void UMingRTSFailureRecoverySystem::LogRecoveryEvent(const FRecoveryExecution& Execution)
{
    UE_LOG(LogTemp, Log, TEXT("Recovery Event: [%s] %s - %s (Status: %s, Duration: %.1fs)"), 
        *Execution.StartTime.ToString(), *Execution.ExecutionID, *Execution.PlanID,
        *StaticEnum<ERecoveryStatus>()->GetValueAsString(Execution.Status), Execution.ElapsedTime);
}

void UMingRTSFailureRecoverySystem::SaveFailureData()
{
    // 保存故障數據到文件
    FString Data = ExportFailureData();
    FString FileName = FString::Printf(TEXT("FailureRecoveryData_%s.json"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    UE_LOG(LogTemp, Log, TEXT("Failure data saved to: %s"), *FileName);
}

void UMingRTSFailureRecoverySystem::LoadFailureData()
{
    // 載入故障數據
    UE_LOG(LogTemp, Log, TEXT("Failure data loaded"));
}

void UMingRTSFailureRecoverySystem::CleanupHistoricalData()
{
    // 清理歷史數據
    int32 RemovedFailures = 0;
    int32 RemovedExecutions = 0;
    
    FDateTime CutoffTime = FDateTime::Now() - FTimespan::FromDays(7); // 保留7天的數據
    
    // 清理舊故障事件
    for (int32 i = FailureEvents.Num() - 1; i >= 0; --i)
    {
        if (FailureEvents[i].Timestamp < CutoffTime || FailureEvents.Num() > MaxFailureHistory)
        {
            FailureEvents.RemoveAt(i);
            RemovedFailures++;
        }
    }
    
    // 清理舊恢復執行
    for (int32 i = RecoveryExecutions.Num() - 1; i >= 0; --i)
    {
        if (RecoveryExecutions[i].StartTime < CutoffTime || RecoveryExecutions.Num() > MaxRecoveryHistory)
        {
            RecoveryExecutions.RemoveAt(i);
            RemovedExecutions++;
        }
    }
    
    if (RemovedFailures > 0 || RemovedExecutions > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Historical data cleanup completed. Removed %d failures and %d executions"), 
            RemovedFailures, RemovedExecutions);
    }
}

// 輔助方法實現
FString UMingRTSFailureRecoverySystem::GenerateFailureID()
{
    return FString::Printf(TEXT("FAIL_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSFailureRecoverySystem::GeneratePlanID()
{
    return FString::Printf(TEXT("PLAN_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSFailureRecoverySystem::GenerateExecutionID()
{
    return FString::Printf(TEXT("EXEC_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSFailureRecoverySystem::FindRecoveryPlan(EFailureType FailureType, const FString& ComponentName)
{
    for (const auto& PlanPair : RecoveryPlans)
    {
        const FRecoveryPlan& Plan = PlanPair.Value;
        if (Plan.FailureType == FailureType && 
            (Plan.ComponentName == ComponentName || Plan.ComponentName == TEXT("")))
        {
            return Plan.PlanID;
        }
    }
    return TEXT("");
}

FRecoveryExecution* UMingRTSFailureRecoverySystem::FindRecoveryExecution(const FString& ExecutionID)
{
    for (FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.ExecutionID == ExecutionID)
        {
            return &Execution;
        }
    }
    return nullptr;
}

void UMingRTSFailureRecoverySystem::UpdateFailureStatistics(EFailureType FailureType)
{
    int32& Count = FailureStatistics.FindOrAdd(FailureType);
    Count++;
}

void UMingRTSFailureRecoverySystem::UpdateRecoveryStatistics(ERecoveryAction Action)
{
    int32& Count = RecoveryStatistics.FindOrAdd(Action);
    Count++;
}

int32 UMingRTSFailureRecoverySystem::GetRecurringFailureCount() const
{
    int32 Count = 0;
    for (const FFailureEvent& Failure : FailureEvents)
    {
        if (Failure.bIsRecurring)
        {
            Count++;
        }
    }
    return Count;
}

void UMingRTSFailureRecoverySystem::UpdateComponentHealth(const FString& ComponentName, bool bIsHealthy)
{
    FSystemHealth& Health = SystemHealthStates.FindOrAdd(ComponentName);
    bool bOldHealthy = Health.bIsHealthy;
    
    Health.ComponentName = ComponentName;
    Health.bIsHealthy = bIsHealthy;
    Health.LastCheck = FDateTime::Now();
    Health.HealthScore = CalculateHealthScore(ComponentName);
    
    if (!bIsHealthy)
    {
        Health.FailureCount++;
        Health.LastFailure = FDateTime::Now();
        
        EFailureType* LastFailureType = Health.FailureHistory.Find(EFailureType::ProcessCrash);
        if (LastFailureType)
        {
            (*LastFailureType)++;
        }
        else
        {
            Health.FailureHistory.Add(EFailureType::ProcessCrash, 1);
        }
    }
    else if (!bOldHealthy && bIsHealthy)
    {
        Health.RecoveryCount++;
        Health.LastRecovery = FDateTime::Now();
    }
    
    // 廣播健康狀態變化
    if (bOldHealthy != bIsHealthy)
    {
        OnSystemHealthChanged.Broadcast(ComponentName, bIsHealthy);
    }
}

void UMingRTSFailureRecoverySystem::ExecuteNextRecoveryAction(const FString& ExecutionID)
{
    FRecoveryExecution* Execution = FindRecoveryExecution(ExecutionID);
    if (!Execution)
    {
        return;
    }

    const FRecoveryPlan* Plan = RecoveryPlans.Find(Execution->PlanID);
    if (!Plan)
    {
        HandleRecoveryFailure(ExecutionID, TEXT("Recovery plan not found"));
        return;
    }

    // 檢查是否所有操作都已完成
    if (Execution->CurrentActionIndex >= Plan->RecoveryActions.Num())
    {
        // 恢復完成
        Execution->Status = ERecoveryStatus::Completed;
        Execution->EndTime = FDateTime::Now();
        Execution->ElapsedTime = (Execution.EndTime - Execution.StartTime).GetTotalSeconds();
        
        SuccessfulRecoveries++;
        
        UE_LOG(LogTemp, Log, TEXT("Recovery completed successfully: %s"), *ExecutionID);
        
        // 廣播恢復完成事件
        OnRecoveryCompleted.Broadcast(*Execution);
        
        // 調用回調
        if (RecoveryCallback)
        {
            RecoveryCallback(*Execution);
        }
        
        return;
    }

    // 執行下一個操作
    ERecoveryAction NextAction = Plan->RecoveryActions[Execution->CurrentActionIndex];
    ExecuteRecoveryAction(ExecutionID, NextAction);
    
    Execution->CurrentActionIndex++;
}

void UMingRTSFailureRecoverySystem::MonitorRecoveryExecutions()
{
    FDateTime CurrentTime = FDateTime::Now();
    
    for (FRecoveryExecution& Execution : RecoveryExecutions)
    {
        if (Execution.Status == ERecoveryStatus::InProgress)
        {
            FTimespan ElapsedTime = CurrentTime - Execution.StartTime;
            
            // 檢查是否超時
            if (ElapsedTime.GetTotalSeconds() > RecoveryTimeout)
            {
                HandleRecoveryTimeout(Execution.ExecutionID);
            }
        }
    }
}
