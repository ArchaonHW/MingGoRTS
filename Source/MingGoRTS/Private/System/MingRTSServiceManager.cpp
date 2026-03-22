#include "MingRTSServiceManager.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

// 靜態成員初始化
int32 UMingRTSServiceManager::ServiceIDCounter = 0;

UMingRTSServiceManager::UMingRTSServiceManager()
{
    // 初始化監控設置
    MonitoringInterval = 5.0f; // 5秒監控間隔
    bAutoMonitoringEnabled = true;
    bAutoRestartEnabled = true;
    LastMonitoringTime = FDateTime::Now();
}

void UMingRTSServiceManager::InitializeServiceManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingRTS Service Manager..."));
    
    // 清空現有服務
    ActiveServices.Empty();
    ServiceDependencies.Empty();
    ServiceDependents.Empty();
    
    // 重置計數器
    ServiceIDCounter = 0;
    
    // 初始化系統資源使用情況
    SystemResourceUsage.Empty();
    SystemResourceUsage.Add(TEXT("CPU"), 0.0f);
    SystemResourceUsage.Add(TEXT("Memory"), 0.0f);
    SystemResourceUsage.Add(TEXT("Network"), 0.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Service Manager initialized successfully"));
}

FString UMingRTSServiceManager::RegisterService(const FServiceRegistration& Registration)
{
    // 驗證註冊參數
    if (!ValidateServiceRegistration(Registration))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid service registration parameters"));
        return TEXT("");
    }
    
    // 檢查服務名稱是否已存在
    for (const auto& ServicePair : ActiveServices)
    {
        if (ServicePair.Value.ServiceName == Registration.ServiceName)
        {
            UE_LOG(LogTemp, Warning, TEXT("Service with name '%s' already exists"), *Registration.ServiceName);
            return TEXT("");
        }
    }
    
    // 生成唯一服務ID
    FString ServiceID = GenerateServiceID();
    
    // 創建服務信息
    FServiceInfo ServiceInfo;
    ServiceInfo.ServiceID = ServiceID;
    ServiceInfo.ServiceName = Registration.ServiceName;
    ServiceInfo.ServiceType = Registration.ServiceType;
    ServiceInfo.State = EServiceState::Uninitialized;
    ServiceInfo.Priority = Registration.Priority;
    ServiceInfo.Version = Registration.Version;
    ServiceInfo.Description = Registration.Description;
    ServiceInfo.Dependencies = Registration.Dependencies;
    ServiceInfo.ServiceData = Registration.InitialData;
    ServiceInfo.CPUUsage = 0.0f;
    ServiceInfo.MemoryUsage = 0.0f;
    ServiceInfo.LastUpdateTime = FDateTime::Now();
    ServiceInfo.bAutoRestart = Registration.bAutoRestart;
    
    // 檢查依賴關係
    if (!CheckServiceDependencies(Registration.Dependencies))
    {
        UE_LOG(LogTemp, Error, TEXT("Service '%s' has unmet dependencies"), *Registration.ServiceName);
        return TEXT("");
    }
    
    // 添加到活躍服務列表
    ActiveServices.Add(ServiceID, ServiceInfo);
    
    // 更新依賴關係圖
    ServiceDependencies.Add(ServiceID, Registration.Dependencies);
    
    // 更新依賴者關係圖
    for (const FString& DependencyID : Registration.Dependencies)
    {
        if (!ServiceDependents.Contains(DependencyID))
        {
            ServiceDependents.Add(DependencyID, TArray<FString>());
        }
        ServiceDependents[DependencyID].Add(ServiceID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' registered with ID: %s"), *Registration.ServiceName, *ServiceID);
    
    // 廣播服務註冊事件
    OnServiceRegistered.Broadcast(ServiceID, ServiceInfo);
    
    return ServiceID;
}

bool UMingRTSServiceManager::UnregisterService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo ServiceInfo = ActiveServices[ServiceID];
    
    // 檢查是否有其他服務依賴此服務
    if (ServiceDependents.Contains(ServiceID) && ServiceDependents[ServiceID].Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot unregister service '%s' - it has dependent services"), *ServiceInfo.ServiceName);
        return false;
    }
    
    // 停止服務（如果正在運行）
    if (ServiceInfo.State == EServiceState::Running)
    {
        StopService(ServiceID);
    }
    
    // 從依賴關係中移除
    if (ServiceDependencies.Contains(ServiceID))
    {
        for (const FString& DependencyID : ServiceDependencies[ServiceID])
        {
            if (ServiceDependents.Contains(DependencyID))
            {
                ServiceDependents[DependencyID].Remove(ServiceID);
            }
        }
        ServiceDependencies.Remove(ServiceID);
    }
    
    // 移除服務
    ActiveServices.Remove(ServiceID);
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' unregistered successfully"), *ServiceInfo.ServiceName);
    
    // 廣播服務註銷事件
    OnServiceUnregistered.Broadcast(ServiceID, true);
    
    return true;
}

bool UMingRTSServiceManager::StartService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    if (ServiceInfo.State == EServiceState::Running)
    {
        UE_LOG(LogTemp, Warning, TEXT("Service '%s' is already running"), *ServiceInfo.ServiceName);
        return true;
    }
    
    // 解決依賴關係並啟動依賴服務
    TArray<FString> Dependencies = ResolveServiceDependencies(ServiceID);
    for (const FString& DependencyID : Dependencies)
    {
        if (ActiveServices.Contains(DependencyID))
        {
            FServiceInfo& DependencyInfo = ActiveServices[DependencyID];
            if (DependencyInfo.State != EServiceState::Running)
            {
                if (!StartService(DependencyID))
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to start dependency service '%s' for '%s'"), 
                           *DependencyInfo.ServiceName, *ServiceInfo.ServiceName);
                    return false;
                }
            }
        }
    }
    
    // 啟動服務
    return StartServiceInternal(ServiceID);
}

bool UMingRTSServiceManager::StopService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    if (ServiceInfo.State != EServiceState::Running)
    {
        UE_LOG(LogTemp, Warning, TEXT("Service '%s' is not running"), *ServiceInfo.ServiceName);
        return true;
    }
    
    // 停止依賴此服務的其他服務
    if (ServiceDependents.Contains(ServiceID))
    {
        TArray<FString> Dependents = ServiceDependents[ServiceID];
        for (const FString& DependentID : Dependents)
        {
            if (ActiveServices.Contains(DependentID))
            {
                FServiceInfo& DependentInfo = ActiveServices[DependentID];
                if (DependentInfo.State == EServiceState::Running)
                {
                    StopService(DependentID);
                }
            }
        }
    }
    
    // 停止服務
    return StopServiceInternal(ServiceID);
}

bool UMingRTSServiceManager::RestartService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Restarting service with ID: %s"), *ServiceID);
    
    // 停止服務
    bool bStopResult = StopService(ServiceID);
    
    // 等待一小段時間確保完全停止
    FPlatformProcess::Sleep(0.1f);
    
    // 啟動服務
    bool bStartResult = StartService(ServiceID);
    
    return bStopResult && bStartResult;
}

bool UMingRTSServiceManager::SuspendService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    if (ServiceInfo.State != EServiceState::Running)
    {
        UE_LOG(LogTemp, Warning, TEXT("Service '%s' is not running"), *ServiceInfo.ServiceName);
        return false;
    }
    
    // 更新服務狀態
    UpdateServiceState(ServiceID, EServiceState::Suspended);
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' suspended"), *ServiceInfo.ServiceName);
    return true;
}

bool UMingRTSServiceManager::ResumeService(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    if (ServiceInfo.State != EServiceState::Suspended)
    {
        UE_LOG(LogTemp, Warning, TEXT("Service '%s' is not suspended"), *ServiceInfo.ServiceName);
        return false;
    }
    
    // 更新服務狀態
    UpdateServiceState(ServiceID, EServiceState::Running);
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' resumed"), *ServiceInfo.ServiceName);
    return true;
}

FServiceInfo UMingRTSServiceManager::GetServiceInfo(const FString& ServiceID) const
{
    if (ActiveServices.Contains(ServiceID))
    {
        return ActiveServices[ServiceID];
    }
    
    // 返回空的服務信息
    FServiceInfo EmptyInfo;
    return EmptyInfo;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetAllServices() const
{
    TArray<FServiceInfo> AllServices;
    
    for (const auto& ServicePair : ActiveServices)
    {
        AllServices.Add(ServicePair.Value);
    }
    
    return AllServices;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByType(EServiceType ServiceType) const
{
    TArray<FServiceInfo> ServicesByType;
    
    for (const auto& ServicePair : ActiveServices)
    {
        if (ServicePair.Value.ServiceType == ServiceType)
        {
            ServicesByType.Add(ServicePair.Value);
        }
    }
    
    return ServicesByType;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByState(EServiceState State) const
{
    TArray<FServiceInfo> ServicesByState;
    
    for (const auto& ServicePair : ActiveServices)
    {
        if (ServicePair.Value.State == State)
        {
            ServicesByState.Add(ServicePair.Value);
        }
    }
    
    return ServicesByState;
}

TArray<FServiceInfo> UMingRTSServiceManager::GetServicesByPriority(EServicePriority Priority) const
{
    TArray<FServiceInfo> ServicesByPriority;
    
    for (const auto& ServicePair : ActiveServices)
    {
        if (ServicePair.Value.Priority == Priority)
        {
            ServicesByPriority.Add(ServicePair.Value);
        }
    }
    
    return ServicesByPriority;
}

bool UMingRTSServiceManager::UpdateServiceState(const FString& ServiceID, EServiceState NewState)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    EServiceState OldState = ServiceInfo.State;
    ServiceInfo.State = NewState;
    ServiceInfo.LastUpdateTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' state changed from %d to %d"), 
           *ServiceInfo.ServiceName, (int32)OldState, (int32)NewState);
    
    // 廣播狀態變化事件
    OnServiceStateChanged.Broadcast(ServiceID, NewState);
    
    return true;
}

bool UMingRTSServiceManager::UpdateServiceData(const FString& ServiceID, const TMap<FString, FString>& NewData)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    // 更新服務數據
    for (const auto& DataPair : NewData)
    {
        ServiceInfo.ServiceData.Add(DataPair.Key, DataPair.Value);
    }
    
    ServiceInfo.LastUpdateTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' data updated"), *ServiceInfo.ServiceName);
    return true;
}

bool UMingRTSServiceManager::DoesServiceExist(const FString& ServiceID) const
{
    return ActiveServices.Contains(ServiceID);
}

TArray<FString> UMingRTSServiceManager::GetServiceDependencies(const FString& ServiceID) const
{
    if (ServiceDependencies.Contains(ServiceID))
    {
        return ServiceDependencies[ServiceID];
    }
    
    return TArray<FString>();
}

TArray<FString> UMingRTSServiceManager::GetServiceDependents(const FString& ServiceID) const
{
    if (ServiceDependents.Contains(ServiceID))
    {
        return ServiceDependents[ServiceID];
    }
    
    return TArray<FString>();
}

bool UMingRTSServiceManager::StartServiceDependencyChain(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    // 獲取依賴鏈
    TArray<FString> DependencyChain = ResolveServiceDependencies(ServiceID);
    
    // 按順序啟動依賴服務
    for (int32 i = DependencyChain.Num() - 1; i >= 0; i--)
    {
        const FString& DependencyID = DependencyChain[i];
        if (ActiveServices.Contains(DependencyID))
        {
            FServiceInfo& DependencyInfo = ActiveServices[DependencyID];
            if (DependencyInfo.State != EServiceState::Running)
            {
                if (!StartService(DependencyID))
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to start dependency service '%s'"), *DependencyInfo.ServiceName);
                    return false;
                }
            }
        }
    }
    
    // 最後啟動目標服務
    return StartService(ServiceID);
}

bool UMingRTSServiceManager::StopServiceDependencyChain(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    // 首先停止目標服務
    if (!StopService(ServiceID))
    {
        return false;
    }
    
    // 然後停止依賴此服務的其他服務
    if (ServiceDependents.Contains(ServiceID))
    {
        TArray<FString> Dependents = ServiceDependents[ServiceID];
        for (const FString& DependentID : Dependents)
        {
            if (ActiveServices.Contains(DependentID))
            {
                StopService(DependentID);
            }
        }
    }
    
    return true;
}

TMap<EServiceState, int32> UMingRTSServiceManager::GetSystemServiceStats() const
{
    TMap<EServiceState, int32> Stats;
    
    // 初始化所有狀態計數為0
    Stats.Add(EServiceState::Uninitialized, 0);
    Stats.Add(EServiceState::Initializing, 0);
    Stats.Add(EServiceState::Running, 0);
    Stats.Add(EServiceState::Suspended, 0);
    Stats.Add(EServiceState::Stopping, 0);
    Stats.Add(EServiceState::Stopped, 0);
    Stats.Add(EServiceState::Error, 0);
    
    // 統計各狀態服務數量
    for (const auto& ServicePair : ActiveServices)
    {
        EServiceState State = ServicePair.Value.State;
        if (Stats.Contains(State))
        {
            Stats[State]++;
        }
    }
    
    return Stats;
}

TMap<FString, float> UMingRTSServiceManager::GetServicePerformanceStats(const FString& ServiceID) const
{
    TMap<FString, float> PerformanceStats;
    
    if (!ActiveServices.Contains(ServiceID))
    {
        return PerformanceStats;
    }
    
    const FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    PerformanceStats.Add(TEXT("CPUUsage"), ServiceInfo.CPUUsage);
    PerformanceStats.Add(TEXT("MemoryUsage"), ServiceInfo.MemoryUsage);
    
    // 計算運行時間（秒）
    FTimespan Runtime = FDateTime::Now() - ServiceInfo.LastUpdateTime;
    PerformanceStats.Add(TEXT("RuntimeSeconds"), Runtime.GetTotalSeconds());
    
    return PerformanceStats;
}

bool UMingRTSServiceManager::SetServicePriority(const FString& ServiceID, EServicePriority NewPriority)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    ServiceInfo.Priority = NewPriority;
    ServiceInfo.LastUpdateTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' priority updated to %d"), *ServiceInfo.ServiceName, (int32)NewPriority);
    return true;
}

bool UMingRTSServiceManager::SetServiceAutoRestart(const FString& ServiceID, bool bAutoRestart)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service with ID '%s' not found"), *ServiceID);
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    ServiceInfo.bAutoRestart = bAutoRestart;
    ServiceInfo.LastUpdateTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' auto-restart set to %s"), 
           *ServiceInfo.ServiceName, bAutoRestart ? TEXT("true") : TEXT("false"));
    return true;
}

TMap<FString, bool> UMingRTSServiceManager::PerformServiceHealthCheck()
{
    TMap<FString, bool> HealthResults;
    
    for (const auto& ServicePair : ActiveServices)
    {
        const FString& ServiceID = ServicePair.Key;
        const FServiceInfo& ServiceInfo = ServicePair.Value;
        
        // 檢查服務健康狀態
        bool bHealthy = CheckServiceHealth(ServiceInfo);
        HealthResults.Add(ServiceID, bHealthy);
        
        if (!bHealthy)
        {
            UE_LOG(LogTemp, Warning, TEXT("Service '%s' failed health check"), *ServiceInfo.ServiceName);
            
            // 廣播錯誤事件
            OnServiceError.Broadcast(ServiceID);
            
            // 如果啟用自動重啟且服務配置了自動重啟
            if (bAutoRestartEnabled && ServiceInfo.bAutoRestart)
            {
                UE_LOG(LogTemp, Log, TEXT("Attempting auto-restart for service '%s'"), *ServiceInfo.ServiceName);
                RestartService(ServiceID);
            }
        }
    }
    
    return HealthResults;
}

TArray<FString> UMingRTSServiceManager::GetServiceStartupOrder()
{
    TArray<FString> StartupOrder;
    TSet<FString> Visited;
    TSet<FString> Visiting;
    
    // 對所有服務進行拓撲排序
    for (const auto& ServicePair : ActiveServices)
    {
        const FString& ServiceID = ServicePair.Key;
        if (!Visited.Contains(ServiceID))
        {
            TopologicalSort(ServiceID, Visited, Visiting, StartupOrder);
        }
    }
    
    return StartupOrder;
}

FString UMingRTSServiceManager::GenerateServiceReport() const
{
    FString Report;
    Report += TEXT("=== MingRTS Service Manager Report ===\n");
    Report += FString::Printf(TEXT("Generated: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Total Services: %d\n\n"), ActiveServices.Num());
    
    // 服務狀態統計
    TMap<EServiceState, int32> Stats = GetSystemServiceStats();
    Report += TEXT("=== Service Status Summary ===\n");
    for (const auto& StatPair : Stats)
    {
        FString StateName;
        switch (StatPair.Key)
        {
            case EServiceState::Uninitialized: StateName = TEXT("Uninitialized"); break;
            case EServiceState::Initializing: StateName = TEXT("Initializing"); break;
            case EServiceState::Running: StateName = TEXT("Running"); break;
            case EServiceState::Suspended: StateName = TEXT("Suspended"); break;
            case EServiceState::Stopping: StateName = TEXT("Stopping"); break;
            case EServiceState::Stopped: StateName = TEXT("Stopped"); break;
            case EServiceState::Error: StateName = TEXT("Error"); break;
        }
        Report += FString::Printf(TEXT("%s: %d\n"), *StateName, StatPair.Value);
    }
    
    Report += TEXT("\n=== Service Details ===\n");
    
    // 按優先級排序服務
    TArray<FServiceInfo> SortedServices = GetAllServices();
    SortedServices.Sort([](const FServiceInfo& A, const FServiceInfo& B)
    {
        return (int32)A.Priority < (int32)B.Priority;
    });
    
    for (const FServiceInfo& ServiceInfo : SortedServices)
    {
        Report += FString::Printf(TEXT("\nService: %s (ID: %s)\n"), *ServiceInfo.ServiceName, *ServiceInfo.ServiceID);
        Report += FString::Printf(TEXT("  Type: %d, Priority: %d\n"), (int32)ServiceInfo.ServiceType, (int32)ServiceInfo.Priority);
        Report += FString::Printf(TEXT("  State: %d, Version: %s\n"), (int32)ServiceInfo.State, *ServiceInfo.Version);
        Report += FString::Printf(TEXT("  CPU: %.2f%%, Memory: %.2fMB\n"), ServiceInfo.CPUUsage, ServiceInfo.MemoryUsage);
        Report += FString::Printf(TEXT("  Auto-Restart: %s\n"), ServiceInfo.bAutoRestart ? TEXT("Yes") : TEXT("No"));
        
        if (ServiceInfo.Dependencies.Num() > 0)
        {
            Report += TEXT("  Dependencies: ");
            for (const FString& DepID : ServiceInfo.Dependencies)
            {
                if (ActiveServices.Contains(DepID))
                {
                    Report += FString::Printf(TEXT("%s "), *ActiveServices[DepID].ServiceName);
                }
            }
            Report += TEXT("\n");
        }
    }
    
    // 系統資源使用情況
    Report += TEXT("\n=== System Resource Usage ===\n");
    for (const auto& ResourcePair : SystemResourceUsage)
    {
        Report += FString::Printf(TEXT("%s: %.2f%%\n"), *ResourcePair.Key, ResourcePair.Value);
    }
    
    Report += TEXT("\n=== End of Report ===\n");
    
    return Report;
}

// Protected Functions

FString UMingRTSServiceManager::GenerateServiceID() const
{
    return FString::Printf(TEXT("SRV_%08d"), ++ServiceIDCounter);
}

bool UMingRTSServiceManager::ValidateServiceRegistration(const FServiceRegistration& Registration) const
{
    if (Registration.ServiceName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Service name cannot be empty"));
        return false;
    }
    
    if (Registration.Version.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Service version cannot be empty"));
        return false;
    }
    
    // 檢查依賴服務是否存在
    for (const FString& DependencyID : Registration.Dependencies)
    {
        if (!ActiveServices.Contains(DependencyID))
        {
            UE_LOG(LogTemp, Error, TEXT("Dependency service '%s' not found"), *DependencyID);
            return false;
        }
    }
    
    return true;
}

bool UMingRTSServiceManager::CheckServiceDependencies(const TArray<FString>& Dependencies) const
{
    for (const FString& DependencyID : Dependencies)
    {
        if (!ActiveServices.Contains(DependencyID))
        {
            return false;
        }
    }
    return true;
}

TArray<FString> UMingRTSServiceManager::ResolveServiceDependencies(const FString& ServiceID) const
{
    TArray<FString> ResolvedDependencies;
    TSet<FString> Visited;
    
    ResolveDependenciesRecursive(ServiceID, ResolvedDependencies, Visited);
    
    return ResolvedDependencies;
}

void UMingRTSServiceManager::ResolveDependenciesRecursive(const FString& ServiceID, 
    TArray<FString>& ResolvedDependencies, TSet<FString>& Visited) const
{
    if (Visited.Contains(ServiceID))
    {
        return; // 避免循環依賴
    }
    
    Visited.Add(ServiceID);
    
    if (ServiceDependencies.Contains(ServiceID))
    {
        for (const FString& DependencyID : ServiceDependencies[ServiceID])
        {
            ResolveDependenciesRecursive(DependencyID, ResolvedDependencies, Visited);
            if (!ResolvedDependencies.Contains(DependencyID))
            {
                ResolvedDependencies.Add(DependencyID);
            }
        }
    }
}

bool UMingRTSServiceManager::StartServiceInternal(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    // 設置為初始化狀態
    UpdateServiceState(ServiceID, EServiceState::Initializing);
    
    // 模擬服務啟動延遲
    float StartupDelay = GetServiceStartupDelay(ServiceInfo.Priority);
    FPlatformProcess::Sleep(StartupDelay);
    
    // 設置為運行狀態
    UpdateServiceState(ServiceID, EServiceState::Running);
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' started successfully"), *ServiceInfo.ServiceName);
    return true;
}

bool UMingRTSServiceManager::StopServiceInternal(const FString& ServiceID)
{
    if (!ActiveServices.Contains(ServiceID))
    {
        return false;
    }
    
    FServiceInfo& ServiceInfo = ActiveServices[ServiceID];
    
    // 設置為停止中狀態
    UpdateServiceState(ServiceID, EServiceState::Stopping);
    
    // 模擬服務停止延遲
    FPlatformProcess::Sleep(0.5f);
    
    // 設置為已停止狀態
    UpdateServiceState(ServiceID, EServiceState::Stopped);
    
    UE_LOG(LogTemp, Log, TEXT("Service '%s' stopped successfully"), *ServiceInfo.ServiceName);
    return true;
}

void UMingRTSServiceManager::UpdateServicePerformanceData()
{
    for (auto& ServicePair : ActiveServices)
    {
        FServiceInfo& ServiceInfo = ServicePair.Value;
        
        // 模擬性能數據更新
        if (ServiceInfo.State == EServiceState::Running)
        {
            // 模擬CPU使用率變化
            ServiceInfo.CPUUsage = FMath::RandRange(5.0f, 25.0f);
            
            // 模擬內存使用變化
            ServiceInfo.MemoryUsage = FMath::RandRange(10.0f, 100.0f);
            
            ServiceInfo.LastUpdateTime = FDateTime::Now();
        }
        else
        {
            ServiceInfo.CPUUsage = 0.0f;
            ServiceInfo.MemoryUsage = 0.0f;
        }
    }
}

void UMingRTSServiceManager::PerformServiceMonitoring()
{
    if (!bAutoMonitoringEnabled)
    {
        return;
    }
    
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan TimeSinceLastMonitoring = CurrentTime - LastMonitoringTime;
    
    if (TimeSinceLastMonitoring.GetTotalSeconds() >= MonitoringInterval)
    {
        // 更新性能數據
        UpdateServicePerformanceData();
        
        // 執行健康檢查
        CheckServiceHealth();
        
        // 自動重啟失敗服務
        AutoRestartFailedServices();
        
        LastMonitoringTime = CurrentTime;
    }
}

void UMingRTSServiceManager::HandleServiceError(const FString& ServiceID, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Service error - ID: %s, Error: %s"), *ServiceID, *ErrorMessage);
    
    if (ActiveServices.Contains(ServiceID))
    {
        // 設置服務為錯誤狀態
        UpdateServiceState(ServiceID, EServiceState::Error);
        
        // 廣播錯誤事件
        OnServiceError.Broadcast(ServiceID);
    }
}

bool UMingRTSServiceManager::CheckServiceHealth(const FServiceInfo& ServiceInfo)
{
    // 基本健康檢查
    if (ServiceInfo.State == EServiceState::Error)
    {
        return false;
    }
    
    if (ServiceInfo.State == EServiceState::Running)
    {
        // 檢查性能指標是否在正常範圍內
        if (ServiceInfo.CPUUsage > 90.0f || ServiceInfo.MemoryUsage > 500.0f)
        {
            return false;
        }
    }
    
    return true;
}

void UMingRTSServiceManager::CheckServiceHealth()
{
    for (const auto& ServicePair : ActiveServices)
    {
        const FString& ServiceID = ServicePair.Key;
        const FServiceInfo& ServiceInfo = ServicePair.Value;
        
        if (!CheckServiceHealth(ServiceInfo))
        {
            HandleServiceError(ServiceID, TEXT("Health check failed"));
        }
    }
}

void UMingRTSServiceManager::AutoRestartFailedServices()
{
    if (!bAutoRestartEnabled)
    {
        return;
    }
    
    for (auto& ServicePair : ActiveServices)
    {
        const FString& ServiceID = ServicePair.Key;
        FServiceInfo& ServiceInfo = ServicePair.Value;
        
        if (ServiceInfo.State == EServiceState::Error && ServiceInfo.bAutoRestart)
        {
            UE_LOG(LogTemp, Log, TEXT("Auto-restarting failed service '%s'"), *ServiceInfo.ServiceName);
            RestartService(ServiceID);
        }
    }
}

float UMingRTSServiceManager::GetServiceStartupDelay(EServicePriority Priority) const
{
    switch (Priority)
    {
        case EServicePriority::Critical: return 0.1f;
        case EServicePriority::High: return 0.5f;
        case EServicePriority::Normal: return 1.0f;
        case EServicePriority::Low: return 2.0f;
        case EServicePriority::Background: return 3.0f;
        default: return 1.0f;
    }
}

void UMingRTSServiceManager::TopologicalSort(const FString& ServiceID, TSet<FString>& Visited, 
    TSet<FString>& Visiting, TArray<FString>& SortedList)
{
    if (Visiting.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Error, TEXT("Circular dependency detected involving service: %s"), *ServiceID);
        return;
    }
    
    if (Visited.Contains(ServiceID))
    {
        return;
    }
    
    Visiting.Add(ServiceID);
    
    // 訪問所有依賴
    if (ServiceDependencies.Contains(ServiceID))
    {
        for (const FString& DependencyID : ServiceDependencies[ServiceID])
        {
            TopologicalSort(DependencyID, Visited, Visiting, SortedList);
        }
    }
    
    Visiting.Remove(ServiceID);
    Visited.Add(ServiceID);
    SortedList.Add(ServiceID);
}
