#include "MingRTSServiceRegistry.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSServiceRegistry::UMingRTSServiceRegistry()
{
    bInitialized = false;
    DiscoveryProtocol = EServiceDiscoveryProtocol::Local;
    DefaultTTL = 3600; // 1小時
    MaxRegistrations = 1000;
    bHealthCheckEnabled = true;
    HealthCheckInterval = 30000; // 30秒
    HealthCheckTimeout = 5000; // 5秒
}

bool UMingRTSServiceRegistry::InitializeRegistry(UMingRTSServiceManager* InServiceManager)
{
    if (!InServiceManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Service Manager reference is null"));
        return false;
    }

    ServiceManager = InServiceManager;

    // 初始化索引
    if (!InitializeIndexes())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize service indexes"));
        return false;
    }

    // 啟動健康檢查定時器
    if (!StartHealthCheckTimer())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to start health check timer"));
    }

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Service Registry initialized successfully"));
    return true;
}

void UMingRTSServiceRegistry::ShutdownRegistry()
{
    if (!bInitialized)
    {
        return;
    }

    // 清理所有註冊
    CleanupRegistrations();

    // 清理所有端點
    CleanupEndpoints();

    // 清理索引
    CleanupIndexes();

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("Service Registry shutdown completed"));
}

FString UMingRTSServiceRegistry::RegisterService(const FServiceRegistration& Registration)
{
    if (!bInitialized || !ValidateServiceRegistration(Registration))
    {
        return TEXT("");
    }

    FString RegistrationID = GenerateRegistrationID();
    FString ServiceID = Registration.Metadata.ServiceID;

    // 檢查服務是否已註冊
    if (ServiceRegistrations.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service %s already registered"), *ServiceID);
        return TEXT("");
    }

    // 檢查註冊數量限制
    if (ServiceRegistrations.Num() >= MaxRegistrations)
    {
        UE_LOG(LogTemp, Error, TEXT("Maximum registration limit reached"));
        return TEXT("");
    }

    FScopeLock Lock(&RegistrationCriticalSection);

    // 創建註冊副本
    FServiceRegistration NewRegistration = Registration;
    NewRegistration.Endpoint.EndpointID = GenerateEndpointID();
    NewRegistration.Endpoint.RegistrationTime = FDateTime::Now();
    NewRegistration.Endpoint.LastHeartbeat = FDateTime::Now();

    // 添加註冊
    ServiceRegistrations.Add(ServiceID, NewRegistration);

    // 添加端點
    {
        FScopeLock EndpointLock(&EndpointCriticalSection);
        if (!ServiceEndpoints.Contains(ServiceID))
        {
            ServiceEndpoints.Add(ServiceID, TArray<FServiceEndpoint>());
        }
        ServiceEndpoints[ServiceID].Add(NewRegistration.Endpoint);
    }

    // 更新索引
    UpdateServiceIndexes(ServiceID, NewRegistration);

    // 發送服務廣告
    SendServiceAdvertisement(NewRegistration);

    // 廣播事件
    OnServiceRegistered.Broadcast(ServiceID, NewRegistration.Endpoint);

    UE_LOG(LogTemp, Log, TEXT("Service %s registered successfully"), *ServiceID);
    return RegistrationID;
}

bool UMingRTSServiceRegistry::UnregisterService(const FString& ServiceID, const FString& Reason)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FScopeLock Lock(&RegistrationCriticalSection);

    if (!ServiceRegistrations.Contains(ServiceID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Service %s not registered"), *ServiceID);
        return false;
    }

    // 從索引中移除
    RemoveFromServiceIndexes(ServiceID);

    // 移除註冊
    ServiceRegistrations.Remove(ServiceID);

    // 移除端點
    {
        FScopeLock EndpointLock(&EndpointCriticalSection);
        ServiceEndpoints.Remove(ServiceID);
    }

    // 廣播事件
    OnServiceUnregistered.Broadcast(ServiceID, Reason);

    UE_LOG(LogTemp, Log, TEXT("Service %s unregistered: %s"), *ServiceID, *Reason);
    return true;
}

bool UMingRTSServiceRegistry::RenewRegistration(const FString& ServiceID)
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FScopeLock Lock(&RegistrationCriticalSection);

    if (FServiceRegistration* Registration = ServiceRegistrations.Find(ServiceID))
    {
        // 更新註冊時間
        Registration->Endpoint.RegistrationTime = FDateTime::Now();
        Registration->Endpoint.LastHeartbeat = FDateTime::Now();

        // 更新TTL
        if (Registration->TTL > 0)
        {
            // 實際實現中應該更新TTL
        }

        UE_LOG(LogTemp, Log, TEXT("Service %s registration renewed"), *ServiceID);
        return true;
    }

    return false;
}

bool UMingRTSServiceRegistry::UpdateServiceEndpoint(const FString& ServiceID, const FServiceEndpoint& NewEndpoint)
{
    if (!bInitialized || !IsValidServiceID(ServiceID) || !ValidateServiceEndpoint(NewEndpoint))
    {
        return false;
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        // 查找並更新端點
        for (int32 i = 0; i < Endpoints->Num(); ++i)
        {
            if ((*Endpoints)[i].EndpointID == NewEndpoint.EndpointID)
            {
                FServiceEndpoint OldEndpoint = (*Endpoints)[i];
                (*Endpoints)[i] = NewEndpoint;
                (*Endpoints)[i].LastHeartbeat = FDateTime::Now();

                // 廣播端點變化
                OnServiceEndpointChanged.Broadcast(ServiceID, NewEndpoint);

                UE_LOG(LogTemp, Log, TEXT("Service %s endpoint updated: %s"), *ServiceID, *NewEndpoint.EndpointID);
                return true;
            }
        }

        // 如果沒找到，添加新端點
        Endpoints->Add(NewEndpoint);
        OnServiceEndpointChanged.Broadcast(ServiceID, NewEndpoint);

        UE_LOG(LogTemp, Log, TEXT("Service %s new endpoint added: %s"), *ServiceID, *NewEndpoint.EndpointID);
        return true;
    }

    return false;
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::DiscoverServices(const FServiceQuery& Query)
{
    if (!bInitialized)
    {
        return TArray<FServiceEndpoint>();
    }

    return QueryServices(Query);
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::GetAllServices() const
{
    if (!bInitialized)
    {
        return TArray<FServiceEndpoint>();
    }

    TArray<FServiceEndpoint> AllEndpoints;
    FScopeLock Lock(&EndpointCriticalSection);

    for (const auto& EndpointPair : ServiceEndpoints)
    {
        AllEndpoints.Append(EndpointPair.Value);
    }

    return AllEndpoints;
}

FServiceEndpoint UMingRTSServiceRegistry::GetServiceEndpoint(const FString& ServiceID) const
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return FServiceEndpoint();
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (const TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        if (Endpoints->Num() > 0)
        {
            return (*Endpoints)[0]; // 返回第一個端點
        }
    }

    return FServiceEndpoint();
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::GetServicesByType(const FString& ServiceType) const
{
    if (!bInitialized)
    {
        return TArray<FServiceEndpoint>();
    }

    TArray<FServiceEndpoint> TypeEndpoints;
    FScopeLock Lock(&IndexCriticalSection);

    if (const TArray<FString>* ServiceIDs = TypeIndex.Find(ServiceType))
    {
        for (const FString& ServiceID : *ServiceIDs)
        {
            FServiceEndpoint Endpoint = GetServiceEndpoint(ServiceID);
            if (!Endpoint.ServiceID.IsEmpty())
            {
                TypeEndpoints.Add(Endpoint);
            }
        }
    }

    return TypeEndpoints;
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::GetServicesByCapability(const FString& Capability) const
{
    if (!bInitialized)
    {
        return TArray<FServiceEndpoint>();
    }

    TArray<FServiceEndpoint> CapabilityEndpoints;
    FScopeLock Lock(&IndexCriticalSection);

    if (const TArray<FString>* ServiceIDs = CapabilityIndex.Find(Capability))
    {
        for (const FString& ServiceID : *ServiceIDs)
        {
            FServiceEndpoint Endpoint = GetServiceEndpoint(ServiceID);
            if (!Endpoint.ServiceID.IsEmpty())
            {
                CapabilityEndpoints.Add(Endpoint);
            }
        }
    }

    return CapabilityEndpoints;
}

bool UMingRTSServiceRegistry::IsServiceAvailable(const FString& ServiceID) const
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FScopeLock Lock(&RegistrationCriticalSection);

    if (const FServiceRegistration* Registration = ServiceRegistrations.Find(ServiceID))
    {
        return !IsRegistrationExpired(*Registration) && IsServiceHealthy(ServiceID);
    }

    return false;
}

bool UMingRTSServiceRegistry::AddServiceEndpoint(const FString& ServiceID, const FServiceEndpoint& Endpoint)
{
    if (!bInitialized || !IsValidServiceID(ServiceID) || !ValidateServiceEndpoint(Endpoint))
    {
        return false;
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (!ServiceEndpoints.Contains(ServiceID))
    {
        ServiceEndpoints.Add(ServiceID, TArray<FServiceEndpoint>());
    }

    // 檢查端點是否已存在
    TArray<FServiceEndpoint>& Endpoints = ServiceEndpoints[ServiceID];
    for (const FServiceEndpoint& ExistingEndpoint : Endpoints)
    {
        if (ExistingEndpoint.EndpointID == Endpoint.EndpointID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Endpoint %s already exists for service %s"), 
                   *Endpoint.EndpointID, *ServiceID);
            return false;
        }
    }

    Endpoints.Add(Endpoint);
    UE_LOG(LogTemp, Log, TEXT("Endpoint %s added to service %s"), *Endpoint.EndpointID, *ServiceID);
    return true;
}

bool UMingRTSServiceRegistry::RemoveServiceEndpoint(const FString& ServiceID, const FString& EndpointID)
{
    if (!bInitialized || !IsValidServiceID(ServiceID) || EndpointID.IsEmpty())
    {
        return false;
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        for (int32 i = 0; i < Endpoints->Num(); ++i)
        {
            if ((*Endpoints)[i].EndpointID == EndpointID)
            {
                Endpoints->RemoveAt(i);
                UE_LOG(LogTemp, Log, TEXT("Endpoint %s removed from service %s"), *EndpointID, *ServiceID);
                return true;
            }
        }
    }

    return false;
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::GetServiceEndpoints(const FString& ServiceID) const
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return TArray<FServiceEndpoint>();
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (const TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        return *Endpoints;
    }

    return TArray<FServiceEndpoint>();
}

void UMingRTSServiceRegistry::EnableHealthCheck(bool bEnabled, int32 CheckInterval)
{
    bHealthCheckEnabled = bEnabled;
    HealthCheckInterval = CheckInterval;
    UE_LOG(LogTemp, Log, TEXT("Health check %s, interval: %d ms"), 
           bEnabled ? TEXT("enabled") : TEXT("disabled"), CheckInterval);
}

void UMingRTSServiceRegistry::SetHealthCheckTimeout(int32 TimeoutMs)
{
    HealthCheckTimeout = TimeoutMs;
    UE_LOG(LogTemp, Log, TEXT("Health check timeout set to %d ms"), TimeoutMs);
}

bool UMingRTSServiceRegistry::IsServiceHealthy(const FString& ServiceID) const
{
    if (!bInitialized || !IsValidServiceID(ServiceID))
    {
        return false;
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (const TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        for (const FServiceEndpoint& Endpoint : *Endpoints)
        {
            if (Endpoint.bIsHealthy)
            {
                return true;
            }
        }
    }

    return false;
}

TArray<FString> UMingRTSServiceRegistry::GetUnhealthyServices() const
{
    if (!bInitialized)
    {
        return TArray<FString>();
    }

    TArray<FString> UnhealthyServices;
    FScopeLock Lock(&EndpointCriticalSection);

    for (const auto& EndpointPair : ServiceEndpoints)
    {
        bool bHasHealthyEndpoint = false;
        for (const FServiceEndpoint& Endpoint : EndpointPair.Value)
        {
            if (Endpoint.bIsHealthy)
            {
                bHasHealthyEndpoint = true;
                break;
            }
        }

        if (!bHasHealthyEndpoint)
        {
            UnhealthyServices.Add(EndpointPair.Key);
        }
    }

    return UnhealthyServices;
}

int32 UMingRTSServiceRegistry::GetRegisteredServiceCount() const
{
    FScopeLock Lock(&RegistrationCriticalSection);
    return ServiceRegistrations.Num();
}

int32 UMingRTSServiceRegistry::GetHealthyServiceCount() const
{
    FScopeLock Lock(&EndpointCriticalSection);
    
    int32 HealthyCount = 0;
    for (const auto& EndpointPair : ServiceEndpoints)
    {
        for (const FServiceEndpoint& Endpoint : EndpointPair.Value)
        {
            if (Endpoint.bIsHealthy)
            {
                HealthyCount++;
                break;
            }
        }
    }
    
    return HealthyCount;
}

int32 UMingRTSServiceRegistry::GetUnhealthyServiceCount() const
{
    return GetUnhealthyServices().Num();
}

TArray<FString> UMingRTSServiceRegistry::GetExpiredServices() const
{
    if (!bInitialized)
    {
        return TArray<FString>();
    }

    TArray<FString> ExpiredServices;
    FScopeLock Lock(&RegistrationCriticalSection);

    for (const auto& RegistrationPair : ServiceRegistrations)
    {
        if (IsRegistrationExpired(RegistrationPair.Value))
        {
            ExpiredServices.Add(RegistrationPair.Key);
        }
    }

    return ExpiredServices;
}

void UMingRTSServiceRegistry::SetDiscoveryProtocol(EServiceDiscoveryProtocol Protocol)
{
    DiscoveryProtocol = Protocol;
    UE_LOG(LogTemp, Log, TEXT("Discovery protocol changed to %d"), (int32)Protocol);
}

void UMingRTSServiceRegistry::SetDefaultTTL(int32 TTLSeconds)
{
    DefaultTTL = TTLSeconds;
    UE_LOG(LogTemp, Log, TEXT("Default TTL set to %d seconds"), TTLSeconds);
}

void UMingRTSServiceRegistry::SetMaxRegistrations(int32 MaxCount)
{
    MaxRegistrations = MaxCount;
    UE_LOG(LogTemp, Log, TEXT("Max registrations set to %d"), MaxCount);
}

bool UMingRTSServiceRegistry::ValidateServiceRegistration(const FServiceRegistration& Registration) const
{
    if (Registration.Metadata.ServiceID.IsEmpty() || 
        Registration.Endpoint.Host.IsEmpty() || 
        Registration.Endpoint.Protocol.IsEmpty())
    {
        return false;
    }

    if (Registration.TTL < 0)
    {
        return false;
    }

    return true;
}

bool UMingRTSServiceRegistry::ValidateServiceEndpoint(const FServiceEndpoint& Endpoint) const
{
    if (Endpoint.ServiceID.IsEmpty() || 
        Endpoint.Host.IsEmpty() || 
        Endpoint.Protocol.IsEmpty())
    {
        return false;
    }

    if (Endpoint.Port < 0 || Endpoint.Port > 65535)
    {
        return false;
    }

    return true;
}

void UMingRTSServiceRegistry::UpdateServiceIndexes(const FString& ServiceID, const FServiceRegistration& Registration)
{
    FScopeLock Lock(&IndexCriticalSection);

    // 更新類型索引
    FString ServiceType = Registration.Metadata.ServiceName;
    if (!TypeIndex.Contains(ServiceType))
    {
        TypeIndex.Add(ServiceType, TArray<FString>());
    }
    TypeIndex[ServiceType].Add(ServiceID);

    // 更新能力索引
    for (const FString& Capability : Registration.Endpoint.Capabilities)
    {
        if (!CapabilityIndex.Contains(Capability))
        {
            CapabilityIndex.Add(Capability, TArray<FString>());
        }
        CapabilityIndex[Capability].Add(ServiceID);
    }
}

void UMingRTSServiceRegistry::RemoveFromServiceIndexes(const FString& ServiceID)
{
    FScopeLock Lock(&IndexCriticalSection);

    // 從類型索引中移除
    for (auto& TypePair : TypeIndex)
    {
        TypePair.Value.Remove(ServiceID);
    }

    // 從能力索引中移除
    for (auto& CapabilityPair : CapabilityIndex)
    {
        CapabilityPair.Value.Remove(ServiceID);
    }
}

void UMingRTSServiceRegistry::PerformHealthCheck(const FString& ServiceID)
{
    if (!bHealthCheckEnabled)
    {
        return;
    }

    FScopeLock Lock(&EndpointCriticalSection);

    if (TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        for (FServiceEndpoint& Endpoint : *Endpoints)
        {
            // 簡化的健康檢查：檢查心跳時間
            FTimespan TimeSinceLastHeartbeat = FDateTime::Now() - Endpoint.LastHeartbeat;
            bool bHealthy = TimeSinceLastHeartbeat.GetTotalSeconds() < (HealthCheckInterval / 1000.0f * 2);

            if (Endpoint.bIsHealthy != bHealthy)
            {
                Endpoint.bIsHealthy = bHealthy;
                UE_LOG(LogTemp, Log, TEXT("Service %s endpoint %s health changed to %s"), 
                       *ServiceID, *Endpoint.EndpointID, bHealthy ? TEXT("healthy") : TEXT("unhealthy"));
            }
        }
    }
}

void UMingRTSServiceRegistry::CleanupExpiredRegistrations()
{
    FScopeLock Lock(&RegistrationCriticalSection);

    TArray<FString> ExpiredServices;
    for (const auto& RegistrationPair : ServiceRegistrations)
    {
        if (IsRegistrationExpired(RegistrationPair.Value))
        {
            ExpiredServices.Add(RegistrationPair.Key);
        }
    }

    for (const FString& ServiceID : ExpiredServices)
    {
        if (FServiceRegistration* Registration = ServiceRegistrations.Find(ServiceID))
        {
            if (Registration->bAutoRenew)
            {
                // 自動續期
                RenewRegistration(ServiceID);
            }
            else
            {
                // 移除過期註冊
                UnregisterService(ServiceID, TEXT("Registration expired"));
            }
        }
    }
}

void UMingRTSServiceRegistry::RenewExpiredRegistrations()
{
    CleanupExpiredRegistrations();
}

bool UMingRTSServiceRegistry::IsRegistrationExpired(const FServiceRegistration& Registration) const
{
    if (Registration.TTL <= 0)
    {
        return false; // 無限TTL
    }

    FTimespan TimeSinceRegistration = FDateTime::Now() - Registration.Endpoint.RegistrationTime;
    return TimeSinceRegistration.GetTotalSeconds() > Registration.TTL;
}

void UMingRTSServiceRegistry::UpdateEndpointHealth(const FString& ServiceID, const FString& EndpointID, bool bHealthy)
{
    FScopeLock Lock(&EndpointCriticalSection);

    if (TArray<FServiceEndpoint>* Endpoints = ServiceEndpoints.Find(ServiceID))
    {
        for (FServiceEndpoint& Endpoint : *Endpoints)
        {
            if (Endpoint.EndpointID == EndpointID)
            {
                Endpoint.bIsHealthy = bHealthy;
                break;
            }
        }
    }
}

TArray<FServiceEndpoint> UMingRTSServiceRegistry::QueryServices(const FServiceQuery& Query) const
{
    TArray<FServiceEndpoint> MatchingEndpoints;
    TArray<FServiceEndpoint> AllEndpoints = GetAllServices();

    for (const FServiceEndpoint& Endpoint : AllEndpoints)
    {
        if (MatchesQuery(Endpoint, Query))
        {
            MatchingEndpoints.Add(Endpoint);
            
            // 檢查結果數量限制
            if (MatchingEndpoints.Num() >= Query.MaxResults)
            {
                break;
            }
        }
    }

    return MatchingEndpoints;
}

bool UMingRTSServiceRegistry::MatchesQuery(const FServiceEndpoint& Endpoint, const FServiceQuery& Query) const
{
    // 檢查服務類型
    if (!Query.ServiceType.IsEmpty())
    {
        FScopeLock Lock(&RegistrationCriticalSection);
        if (const FServiceRegistration* Registration = ServiceRegistrations.Find(Endpoint.ServiceID))
        {
            if (Registration->Metadata.ServiceName != Query.ServiceType)
            {
                return false;
            }
        }
    }

    // 檢查服務名稱
    if (!Query.ServiceName.IsEmpty())
    {
        FScopeLock Lock(&RegistrationCriticalSection);
        if (const FServiceRegistration* Registration = ServiceRegistrations.Find(Endpoint.ServiceID))
        {
            if (!Registration->Metadata.ServiceName.Contains(Query.ServiceName))
            {
                return false;
            }
        }
    }

    // 檢查版本
    if (!Query.Version.IsEmpty())
    {
        FScopeLock Lock(&RegistrationCriticalSection);
        if (const FServiceRegistration* Registration = ServiceRegistrations.Find(Endpoint.ServiceID))
        {
            if (Registration->Metadata.ServiceVersion != Query.Version)
            {
                return false;
            }
        }
    }

    // 檢查所需能力
    if (Query.RequiredCapabilities.Num() > 0)
    {
        if (!HasRequiredCapabilities(Endpoint, Query.RequiredCapabilities))
        {
            return false;
        }
    }

    // 檢查元數據過濾器
    if (Query.MetadataFilter.Num() > 0)
    {
        if (!MatchesMetadataFilter(Endpoint, Query.MetadataFilter))
        {
            return false;
        }
    }

    // 檢查健康狀態
    if (!Query.bIncludeUnhealthy && !Endpoint.bIsHealthy)
    {
        return false;
    }

    return true;
}

bool UMingRTSServiceRegistry::HasRequiredCapabilities(const FServiceEndpoint& Endpoint, const TArray<FString>& RequiredCapabilities) const
{
    for (const FString& RequiredCapability : RequiredCapabilities)
    {
        bool bFound = false;
        for (const FString& Capability : Endpoint.Capabilities)
        {
            if (Capability == RequiredCapability)
            {
                bFound = true;
                break;
            }
        }
        
        if (!bFound)
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRTSServiceRegistry::MatchesMetadataFilter(const FServiceEndpoint& Endpoint, const TMap<FString, FString>& Filter) const
{
    for (const auto& FilterPair : Filter)
    {
        const FString& Key = FilterPair.Key;
        const FString& Value = FilterPair.Value;
        
        if (const FString* MetadataValue = Endpoint.Metadata.Find(Key))
        {
            if (*MetadataValue != Value)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRTSServiceRegistry::SendHeartbeat(const FString& ServiceID)
{
    // 簡化實現，實際應該發送網絡心跳
    UE_LOG(LogTemp, Log, TEXT("Sending heartbeat for service %s"), *ServiceID);
    return true;
}

bool UMingRTSServiceRegistry::SendServiceAdvertisement(const FServiceRegistration& Registration)
{
    // 簡化實現，實際應該發送服務廣告
    UE_LOG(LogTemp, Log, TEXT("Sending advertisement for service %s"), *Registration.Metadata.ServiceID);
    return true;
}

bool UMingRTSServiceRegistry::SendServiceQuery(const FServiceQuery& Query)
{
    // 簡化實現，實際應該發送服務查詢
    UE_LOG(LogTemp, Log, TEXT("Sending service query"));
    return true;
}

void UMingRTSServiceRegistry::HandleServiceDiscoveryResponse(const FString& Response)
{
    // 處理服務發現響應
    UE_LOG(LogTemp, Log, TEXT("Handling service discovery response"));
}

bool UMingRTSServiceRegistry::InitializeIndexes()
{
    FScopeLock Lock(&IndexCriticalSection);
    TypeIndex.Empty();
    CapabilityIndex.Empty();
    return true;
}

bool UMingRTSServiceRegistry::StartHealthCheckTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            // 定期健康檢查
            for (const auto& RegistrationPair : ServiceRegistrations)
            {
                PerformHealthCheck(RegistrationPair.Key);
            }
            
            // 清理過期註冊
            CleanupExpiredRegistrations();
            
            // 繼續下一輪檢查
            if (bHealthCheckEnabled)
            {
                World->GetTimerManager().SetTimerForNextTick([this]()
                {
                    StartHealthCheckTimer();
                });
            }
        });
        
        return true;
    }
    
    return false;
}

void UMingRTSServiceRegistry::CleanupRegistrations()
{
    FScopeLock Lock(&RegistrationCriticalSection);
    ServiceRegistrations.Empty();
}

void UMingRTSServiceRegistry::CleanupEndpoints()
{
    FScopeLock Lock(&EndpointCriticalSection);
    ServiceEndpoints.Empty();
}

void UMingRTSServiceRegistry::CleanupIndexes()
{
    FScopeLock Lock(&IndexCriticalSection);
    TypeIndex.Empty();
    CapabilityIndex.Empty();
}

FString UMingRTSServiceRegistry::GenerateRegistrationID() const
{
    return FString::Printf(TEXT("reg_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSServiceRegistry::GenerateEndpointID() const
{
    return FString::Printf(TEXT("ep_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSServiceRegistry::IsValidServiceID(const FString& ServiceID) const
{
    return !ServiceID.IsEmpty() && ServiceRegistrations.Contains(ServiceID);
}

bool UMingRTSServiceRegistry::IsValidEndpointID(const FString& EndpointID) const
{
    return !EndpointID.IsEmpty();
}

void UMingRTSServiceRegistry::LogRegistryOperation(const FString& Operation, const FString& ServiceID, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("Registry %s: %s - %s"), *Operation, *ServiceID, *Details);
}
