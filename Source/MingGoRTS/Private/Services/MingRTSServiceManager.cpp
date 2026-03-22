#include "MingRTSServiceManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSServiceManager::UMingRTSServiceManager()
{
    bInitialized = false;
}

bool UMingRTSServiceManager::InitializeServiceManager(UMingRTSKernel* InKernel)
{
    if (!InKernel)
    {
        UE_LOG(LogTemp, Error, TEXT("Kernel reference is null"));
        return false;
    }

    Kernel = InKernel;

    // 初始化依賴圖
    if (!InitializeDependencyGraph())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize dependency graph"));
        return false;
    }

    // 加載所有服務配置
    if (!LoadAllConfigurations())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load some service configurations"));
    }

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Service Manager initialized successfully"));

    // 啟動健康檢查定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            // 定期健康檢查
            for (const auto& ServicePair : Services)
            {
                PerformHealthCheck(ServicePair.Key);
            }
        });
    }

    return true;
}

void UMingRTSServiceManager::ShutdownServiceManager()
{
    if (!bInitialized)
    {
        return;
    }

    // 停止所有運行中的服務
    StopAllServices(TEXT("Service Manager shutdown"));

    // 清理資源
    CleanupServices();
    CleanupConfigurations();
    CleanupDependencyGraph();

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("Service Manager shutdown completed"));
}

bool UMingRTSServiceManager::RegisterService(const FServiceMetadata& Metadata, UObject* ServiceInstance)
{
    if (!bInitialized || !ServiceInstance)
    {
        return false;
    }

    if (!ValidateServiceMetadata(Metadata))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid service metadata for %s"), *Metadata.ServiceID);
        return false;
    }

    if (!ValidateDependencies(Metadata))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid dependencies for service %s"), *Metadata.ServiceID);
        return false;
    }

    FScopeLock Lock(&ServiceCriticalSection);

    if (Services.Contains(Metadata.ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service %s already registered"), *Metadata.ServiceID);
        return false;
    }

    // 創建服務信息
    FServiceInfo ServiceInfo;
    ServiceInfo.Metadata = Metadata;
    ServiceInfo.State = EServiceState::Registering;

    // 添加到服務列表
    Services.Add(Metadata.ServiceID, ServiceInfo);
    ServiceInstances.Add(Metadata.ServiceID, ServiceInstance);

    // 更新依賴圖
    {
        FScopeLock DepLock(&DependencyCriticalSection);
        DependencyGraph.Add(Metadata.ServiceID, Metadata.Dependencies);
        
        for (const FString& Dependency : Metadata.Dependencies)
        {
            if (!ReverseDependencyGraph.Contains(Dependency))
            {
                ReverseDependencyGraph.Add(Dependency, TArray<FString>());
            }
            ReverseDependencyGraph[Dependency].Add(Metadata.ServiceID);
        }
    }

    // 註冊到內核
    if (!Kernel->RegisterSystemService(Metadata.ServiceID, ServiceInstance))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to register service %s with kernel"), *Metadata.ServiceID);
        Services.Remove(Metadata.ServiceID);
        ServiceInstances.Remove(Metadata.ServiceID);
        return false;
    }

    // 更新狀態
    UpdateServiceState(Metadata.ServiceID, EServiceState::Registered);

    // 加載配置
    LoadServiceConfiguration(Metadata.ServiceID);

    // 廣播事件
    OnServiceRegistered.Broadcast(Metadata.ServiceID, ServiceInfo);

    UE_LOG(LogTemp, Log, TEXT("Service %s registered successfully"), *Metadata.ServiceID);
    return true;
}

bool UMingRTSServiceManager::UnregisterService(const FString& ServiceID, const FString& Reason)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    
    // 檢查依賴關係
    TArray<FString> Dependents = GetServiceDependents(ServiceID);
    if (Dependents.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot unregister service %s: has %d dependents"), 
               *ServiceID, Dependents.Num());
        return false;
    }

    // 停止服務
    if (ServiceInfo.State == EServiceState::Running)
    {
        StopService(ServiceID, Reason);
    }

    // 從內核註銷
    Kernel->UnregisterSystemService(ServiceID);

    // 清理依賴圖
    {
        FScopeLock DepLock(&DependencyCriticalSection);
        DependencyGraph.Remove(ServiceID);
        ReverseDependencyGraph.Remove(ServiceID);
    }

    // 移除服務
    {
        FScopeLock Lock(&ServiceCriticalSection);
        Services.Remove(ServiceID);
        ServiceInstances.Remove(ServiceID);
        ServiceConfigurations.Remove(ServiceID);
    }

    // 廣播事件
    OnServiceUnregistered.Broadcast(ServiceID, Reason);

    UE_LOG(LogTemp, Log, TEXT("Service %s unregistered: %s"), *ServiceID, *Reason);
    return true;
}

bool UMingRTSServiceManager::IsServiceRegistered(const FString& ServiceID) const
{
    FScopeLock Lock(&ServiceCriticalSection);
    return Services.Contains(ServiceID);
}

TArray<FServiceInfo> UMingRTSServiceManager::GetAllRegisteredServices() const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    TArray<FServiceInfo> ServiceList;
    for (const auto& ServicePair : Services)
    {
        ServiceList.Add(ServicePair.Value);
    }
    
    return ServiceList;
}

FServiceInfo UMingRTSServiceManager::GetServiceInfo(const FString& ServiceID) const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    if (const FServiceInfo* ServiceInfo = Services.Find(ServiceID))
    {
        return *ServiceInfo;
    }
    
    return FServiceInfo();
}

bool UMingRTSServiceManager::StartService(const FString& ServiceID)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    if (!CanStartService(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start service %s: dependencies not met"), *ServiceID);
        return false;
    }

    StartServiceInternal(ServiceID);
    return true;
}

bool UMingRTSServiceManager::StopService(const FString& ServiceID, const FString& Reason)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    // 檢查是否有依賴此服務的其他服務
    TArray<FString> Dependents = GetServiceDependents(ServiceID);
    for (const FString& Dependent : Dependents)
    {
        FServiceInfo DependentInfo = GetServiceInfo(Dependent);
        if (DependentInfo.State == EServiceState::Running)
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot stop service %s: service %s depends on it"), 
                   *ServiceID, *Dependent);
            return false;
        }
    }

    StopServiceInternal(ServiceID, Reason);
    return true;
}

bool UMingRTSServiceManager::RestartService(const FString& ServiceID, const FString& Reason)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    EServiceState OriginalState = ServiceInfo.State;

    // 停止服務
    if (ServiceInfo.State == EServiceState::Running)
    {
        StopServiceInternal(ServiceID, Reason);
    }

    // 重啟服務
    RestartServiceInternal(ServiceID, Reason);

    return true;
}

bool UMingRTSServiceManager::PauseService(const FString& ServiceID)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FServiceInfo* ServiceInfo = Services.Find(ServiceID);
    if (ServiceInfo && ServiceInfo->State == EServiceState::Running)
    {
        UpdateServiceState(ServiceID, EServiceState::Stopped);
        
        // 通過內核掛起進程
        if (!ServiceInfo->ProcessID.IsEmpty())
        {
            Kernel->SuspendProcess(ServiceInfo->ProcessID);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Service %s paused"), *ServiceID);
        return true;
    }

    return false;
}

bool UMingRTSServiceManager::ResumeService(const FString& ServiceID)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FServiceInfo* ServiceInfo = Services.Find(ServiceID);
    if (ServiceInfo && ServiceInfo->State == EServiceState::Stopped)
    {
        UpdateServiceState(ServiceID, EServiceState::Running);
        
        // 通過內核恢復進程
        if (!ServiceInfo->ProcessID.IsEmpty())
        {
            Kernel->ResumeProcess(ServiceInfo->ProcessID);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Service %s resumed"), *ServiceID);
        return true;
    }

    return false;
}

bool UMingRTSServiceManager::StartAllServices()
{
    if (!bInitialized)
    {
        return false;
    }

    TArray<FString> StartupOrder = GetStartupOrder();
    bool bAllStarted = true;

    for (const FString& ServiceID : StartupOrder)
    {
        if (!StartService(ServiceID))
        {
            bAllStarted = false;
            UE_LOG(LogTemp, Error, TEXT("Failed to start service %s"), *ServiceID);
        }
    }

    return bAllStarted;
}

bool UMingRTSServiceManager::StopAllServices(const FString& Reason)
{
    if (!bInitialized)
    {
        return false;
    }

    // 按相反順序停止服務
    TArray<FString> StartupOrder = GetStartupOrder();
    bool bAllStopped = true;

    for (int32 i = StartupOrder.Num() - 1; i >= 0; --i)
    {
        const FString& ServiceID = StartupOrder[i];
        if (!StopService(ServiceID, Reason))
        {
            bAllStopped = false;
            UE_LOG(LogTemp, Error, TEXT("Failed to stop service %s"), *ServiceID);
        }
    }

    return bAllStopped;
}

bool UMingRTSServiceManager::RestartAllServices(const FString& Reason)
{
    if (!bInitialized)
    {
        return false;
    }

    bool bAllRestarted = StopAllServices(Reason);
    if (bAllRestarted)
    {
        bAllRestarted = StartAllServices();
    }

    return bAllRestarted;
}

TArray<FString> UMingRTSServiceManager::GetServiceDependencies(const FString& ServiceID) const
{
    FScopeLock Lock(&DependencyCriticalSection);
    
    if (const TArray<FString>* Dependencies = DependencyGraph.Find(ServiceID))
    {
        return *Dependencies;
    }
    
    return TArray<FString>();
}

TArray<FString> UMingRTSServiceManager::GetServiceDependents(const FString& ServiceID) const
{
    FScopeLock Lock(&DependencyCriticalSection);
    
    if (const TArray<FString>* Dependents = ReverseDependencyGraph.Find(ServiceID))
    {
        return *Dependents;
    }
    
    return TArray<FString>();
}

bool UMingRTSServiceManager::CheckDependencies(const FString& ServiceID) const
{
    TArray<FString> Dependencies = GetServiceDependencies(ServiceID);
    
    for (const FString& Dependency : Dependencies)
    {
        if (!IsServiceRegistered(Dependency))
        {
            return false;
        }
        
        FServiceInfo DependencyInfo = GetServiceInfo(Dependency);
        if (DependencyInfo.State != EServiceState::Running)
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRTSServiceManager::SetServiceConfiguration(const FString& ServiceID, const FServiceConfiguration& Config)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FScopeLock Lock(&ConfigCriticalSection);
    ServiceConfigurations.Add(ServiceID, Config);
    
    return SaveServiceConfiguration(ServiceID);
}

FServiceConfiguration UMingRTSServiceManager::GetServiceConfiguration(const FString& ServiceID) const
{
    FScopeLock Lock(&ConfigCriticalSection);
    
    if (const FServiceConfiguration* Config = ServiceConfigurations.Find(ServiceID))
    {
        return *Config;
    }
    
    return FServiceConfiguration();
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByType(EServiceType ServiceType) const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    TArray<FServiceInfo> FilteredServices;
    for (const auto& ServicePair : Services)
    {
        if (ServicePair.Value.Metadata.ServiceType == ServiceType)
        {
            FilteredServices.Add(ServicePair.Value);
        }
    }
    
    return FilteredServices;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByState(EServiceState State) const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    TArray<FServiceInfo> FilteredServices;
    for (const auto& ServicePair : Services)
    {
        if (ServicePair.Value.State == State)
        {
            FilteredServices.Add(ServicePair.Value);
        }
    }
    
    return FilteredServices;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByPriority(EServicePriority Priority) const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    TArray<FServiceInfo> FilteredServices;
    for (const auto& ServicePair : Services)
    {
        if (ServicePair.Value.Metadata.Priority == Priority)
        {
            FilteredServices.Add(ServicePair.Value);
        }
    }
    
    return FilteredServices;
}

int32 UMingRTSServiceManager::GetRunningServiceCount() const
{
    return GetServicesByState(EServiceState::Running).Num();
}

int32 UMingRTSServiceManager::GetStoppedServiceCount() const
{
    return GetServicesByState(EServiceState::Stopped).Num();
}

float UMingRTSServiceManager::GetAverageServiceUptime() const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    float TotalUptime = 0.0f;
    int32 RunningCount = 0;
    
    for (const auto& ServicePair : Services)
    {
        const FServiceInfo& ServiceInfo = ServicePair.Value;
        if (ServiceInfo.State == EServiceState::Running)
        {
            FTimespan Uptime = FDateTime::Now() - ServiceInfo.StartTime;
            TotalUptime += Uptime.GetTotalSeconds();
            RunningCount++;
        }
    }
    
    return RunningCount > 0 ? TotalUptime / RunningCount : 0.0f;
}

int32 UMingRTSServiceManager::GetTotalRestarts() const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    int32 TotalRestarts = 0;
    for (const auto& ServicePair : Services)
    {
        TotalRestarts += ServicePair.Value.RestartCount;
    }
    
    return TotalRestarts;
}

TArray<FString> UMingRTSServiceManager::GetFailedServices() const
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    TArray<FString> FailedServices;
    for (const auto& ServicePair : Services)
    {
        if (ServicePair.Value.State == EServiceState::Error)
        {
            FailedServices.Add(ServicePair.Key);
        }
    }
    
    return FailedServices;
}

bool UMingRTSServiceManager::ValidateServiceMetadata(const FServiceMetadata& Metadata) const
{
    if (Metadata.ServiceID.IsEmpty() || 
        Metadata.ServiceName.IsEmpty() || 
        Metadata.ServiceVersion.IsEmpty())
    {
        return false;
    }
    
    // 檢查服務ID格式
    if (!Metadata.ServiceID.StartsWith(TEXT("svc_")))
    {
        return false;
    }
    
    // 檢查版本格式
    if (!Metadata.ServiceVersion.Contains(TEXT(".")))
    {
        return false;
    }
    
    return true;
}

bool UMingRTSServiceManager::ValidateDependencies(const FServiceMetadata& Metadata) const
{
    // 檢查循環依賴
    TArray<FString> Visited;
    return !HasCircularDependency(Metadata.ServiceID, Visited);
}

bool UMingRTSServiceManager::CanStartService(const FString& ServiceID) const
{
    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    
    if (ServiceInfo.State != EServiceState::Registered && 
        ServiceInfo.State != EServiceState::Stopped)
    {
        return false;
    }
    
    return CheckDependencies(ServiceID);
}

void UMingRTSServiceManager::UpdateServiceState(const FString& ServiceID, EServiceState NewState)
{
    FScopeLock Lock(&ServiceCriticalSection);
    
    if (FServiceInfo* ServiceInfo = Services.Find(ServiceID))
    {
        EServiceState OldState = ServiceInfo->State;
        ServiceInfo->State = NewState;
        
        if (NewState == EServiceState::Running && OldState != EServiceState::Running)
        {
            ServiceInfo->StartTime = FDateTime::Now();
        }
        
        // 廣播狀態變化
        OnServiceStateChanged.Broadcast(ServiceID, NewState);
        
        UE_LOG(LogTemp, Log, TEXT("Service %s state changed from %d to %d"), 
               *ServiceID, (int32)OldState, (int32)NewState);
    }
}

void UMingRTSServiceManager::StartServiceInternal(const FString& ServiceID)
{
    UpdateServiceState(ServiceID, EServiceState::Starting);
    
    // 通過內核創建進程
    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    FString ProcessID = Kernel->CreateProcess(ServiceInfo.Metadata.ServiceName, 
                                           (int32)ServiceInfo.Metadata.Priority);
    
    if (ProcessID.IsEmpty())
    {
        UpdateServiceState(ServiceID, EServiceState::Error);
        HandleServiceError(ServiceID, TEXT("Failed to create process"));
        return;
    }
    
    // 更新進程ID
    {
        FScopeLock Lock(&ServiceCriticalSection);
        if (FServiceInfo* Info = Services.Find(ServiceID))
        {
            Info->ProcessID = ProcessID;
        }
    }
    
    UpdateServiceState(ServiceID, EServiceState::Running);
    
    UE_LOG(LogTemp, Log, TEXT("Service %s started successfully"), *ServiceID);
}

void UMingRTSServiceManager::StopServiceInternal(const FString& ServiceID, const FString& Reason)
{
    UpdateServiceState(ServiceID, EServiceState::Stopping);
    
    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    
    // 通過內核終止進程
    if (!ServiceInfo.ProcessID.IsEmpty())
    {
        Kernel->TerminateProcess(ServiceInfo.ProcessID);
    }
    
    UpdateServiceState(ServiceID, EServiceState::Stopped);
    
    UE_LOG(LogTemp, Log, TEXT("Service %s stopped: %s"), *ServiceID, *Reason);
}

void UMingRTSServiceManager::RestartServiceInternal(const FString& ServiceID, const FString& Reason)
{
    UpdateServiceState(ServiceID, EServiceState::Restarting);
    
    FServiceInfo* ServiceInfo = Services.Find(ServiceID);
    if (ServiceInfo)
    {
        ServiceInfo->RestartCount++;
    }
    
    // 延遲後重啟
    if (UWorld* World = GetWorld())
    {
        FServiceConfiguration Config = GetServiceConfiguration(ServiceID);
        World->GetTimerManager().SetTimerForNextTick([this, ServiceID, Reason]()
        {
            StartServiceInternal(ServiceID);
        });
    }
    
    UE_LOG(LogTemp, Log, TEXT("Service %s restarting: %s"), *ServiceID, *Reason);
}

void UMingRTSServiceManager::PerformHealthCheck(const FString& ServiceID)
{
    FServiceInfo ServiceInfo = GetServiceInfo(ServiceID);
    
    if (ServiceInfo.State != EServiceState::Running)
    {
        return;
    }
    
    FServiceConfiguration Config = GetServiceConfiguration(ServiceID);
    
    // 檢查心跳
    FTimespan TimeSinceLastHeartbeat = FDateTime::Now() - ServiceInfo.LastHeartbeat;
    if (TimeSinceLastHeartbeat.GetTotalSeconds() > Config.HeartbeatInterval / 1000.0f * 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Service %s heartbeat timeout"), *ServiceID);
        
        if (Config.bAutoRestart)
        {
            RestartService(ServiceID, TEXT("Heartbeat timeout"));
        }
        else
        {
            UpdateServiceState(ServiceID, EServiceState::Error);
            HandleServiceError(ServiceID, TEXT("Heartbeat timeout"));
        }
    }
    
    // 更新心跳時間
    {
        FScopeLock Lock(&ServiceCriticalSection);
        if (FServiceInfo* Info = Services.Find(ServiceID))
        {
            Info->LastHeartbeat = FDateTime::Now();
        }
    }
}

void UMingRTSServiceManager::UpdateServiceStatistics(const FString& ServiceID)
{
    FServiceInfo* ServiceInfo = Services.Find(ServiceID);
    if (!ServiceInfo || ServiceInfo->ProcessID.IsEmpty())
    {
        return;
    }
    
    // 從內核獲取進程信息
    FProcessInfo ProcessInfo = Kernel->GetProcessInfo(ServiceInfo->ProcessID);
    
    ServiceInfo->CPUUsage = ProcessInfo.CPUUsage;
    ServiceInfo->MemoryUsage = ProcessInfo.MemoryUsage;
}

void UMingRTSServiceManager::HandleServiceError(const FString& ServiceID, const FString& ErrorMessage)
{
    OnServiceError.Broadcast(ServiceID, ErrorMessage);
    UE_LOG(LogTemp, Error, TEXT("Service %s error: %s"), *ServiceID, *ErrorMessage);
}

TArray<FString> UMingRTSServiceManager::ResolveDependencies(const FString& ServiceID) const
{
    TArray<FString> Resolved;
    TArray<FString> Visited;
    
    ResolveDependenciesRecursive(ServiceID, Resolved, Visited);
    return Resolved;
}

bool UMingRTSServiceManager::HasCircularDependency(const FString& ServiceID, TArray<FString>& Visited) const
{
    if (Visited.Contains(ServiceID))
    {
        return true;
    }
    
    Visited.Add(ServiceID);
    
    TArray<FString> Dependencies = GetServiceDependencies(ServiceID);
    for (const FString& Dependency : Dependencies)
    {
        if (HasCircularDependency(Dependency, Visited))
        {
            return true;
        }
    }
    
    Visited.Remove(ServiceID);
    return false;
}

TArray<FString> UMingRTSServiceManager::GetStartupOrder() const
{
    TArray<FString> Order;
    TSet<FString> Processed;
    
    // 拓撲排序
    for (const auto& ServicePair : Services)
    {
        const FString& ServiceID = ServicePair.Key;
        if (!Processed.Contains(ServiceID))
        {
            TopologicalSort(ServiceID, Order, Processed);
        }
    }
    
    return Order;
}

bool UMingRTSServiceManager::LoadServiceConfiguration(const FString& ServiceID)
{
    FString ConfigPath = GetConfigurationFilePath(ServiceID);
    
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ConfigPath))
    {
        // 簡化的配置加載
        FServiceConfiguration Config;
        // 實際實現中應該從文件讀取配置
        
        FScopeLock Lock(&ConfigCriticalSection);
        ServiceConfigurations.Add(ServiceID, Config);
        
        return true;
    }
    
    return false;
}

bool UMingRTSServiceManager::SaveServiceConfiguration(const FString& ServiceID)
{
    FScopeLock Lock(&ConfigCriticalSection);
    
    if (const FServiceConfiguration* Config = ServiceConfigurations.Find(ServiceID))
    {
        FString ConfigPath = GetConfigurationFilePath(ServiceID);
        // 實際實現中應該將配置寫入文件
        return true;
    }
    
    return false;
}

FString UMingRTSServiceManager::GetConfigurationFilePath(const FString& ServiceID) const
{
    return FString::Printf(TEXT("Config/Services/%s.json"), *ServiceID);
}

bool UMingRTSServiceManager::InitializeDependencyGraph()
{
    FScopeLock Lock(&DependencyCriticalSection);
    DependencyGraph.Empty();
    ReverseDependencyGraph.Empty();
    return true;
}

bool UMingRTSServiceManager::LoadAllConfigurations()
{
    // 簡化實現，實際應該掃描配置目錄
    return true;
}

void UMingRTSServiceManager::CleanupServices()
{
    FScopeLock Lock(&ServiceCriticalSection);
    Services.Empty();
    ServiceInstances.Empty();
}

void UMingRTSServiceManager::CleanupConfigurations()
{
    FScopeLock Lock(&ConfigCriticalSection);
    ServiceConfigurations.Empty();
}

void UMingRTSServiceManager::CleanupDependencyGraph()
{
    FScopeLock Lock(&DependencyCriticalSection);
    DependencyGraph.Empty();
    ReverseDependencyGraph.Empty();
}

FString UMingRTSServiceManager::GenerateServiceID() const
{
    return FString::Printf(TEXT("svc_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSServiceManager::IsValidServiceID(const FString& ServiceID) const
{
    return !ServiceID.IsEmpty() && Services.Contains(ServiceID);
}

void UMingRTSServiceManager::LogServiceOperation(const FString& Operation, const FString& ServiceID, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("Service %s: %s - %s"), *Operation, *ServiceID, *Details);
}
