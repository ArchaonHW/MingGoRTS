#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"
#include "MingRTSServiceManager.h"
#include "MingRTSServiceRegistry.generated.h"

// 服務發現協議
UENUM(BlueprintType)
enum class EServiceDiscoveryProtocol : uint8
{
    Local,          // 本地發現
    Network,         // 網絡發現
    Cloud,           // 雲端發現
    Hybrid           // 混合發現
};

// 服務註冊狀態
UENUM(BlueprintType)
enum class EServiceRegistrationStatus : uint8
{
    Pending,         // 待處理
    Registered,      // 已註冊
    Unregistered,    // 未註冊
    Expired,         // 已過期
    Invalid          // 無效
};

// 服務端點信息
USTRUCT(BlueprintType)
struct FServiceEndpoint
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FString EndpointID;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FString ServiceID;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FString Host;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    int32 Port;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FString Protocol;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    TArray<FString> Capabilities;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FDateTime RegistrationTime;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    FDateTime LastHeartbeat;

    UPROPERTY(BlueprintReadOnly, Category = "Service Endpoint")
    bool bIsHealthy;

    FServiceEndpoint()
    {
        Port = 0;
        bIsHealthy = true;
        RegistrationTime = FDateTime::Now();
        LastHeartbeat = FDateTime::Now();
    }
};

// 服務查詢條件
USTRUCT(BlueprintType)
struct FServiceQuery
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    FString ServiceType;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    FString ServiceName;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    FString Version;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    TArray<FString> RequiredCapabilities;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    TMap<FString, FString> MetadataFilter;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    int32 MaxResults;

    UPROPERTY(BlueprintReadWrite, Category = "Service Query")
    bool bIncludeUnhealthy;

    FServiceQuery()
    {
        MaxResults = 100;
        bIncludeUnhealthy = false;
    }
};

// 服務註冊信息
USTRUCT(BlueprintType)
struct FServiceRegistration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Service Registration")
    FServiceMetadata Metadata;

    UPROPERTY(BlueprintReadWrite, Category = "Service Registration")
    FServiceEndpoint Endpoint;

    UPROPERTY(BlueprintReadWrite, Category = "Service Registration")
    int32 TTL; // 生存時間（秒）

    UPROPERTY(BlueprintReadWrite, Category = "Service Registration")
    bool bAutoRenew;

    UPROPERTY(BlueprintReadWrite, Category = "Service Registration")
    FString AuthToken;

    FServiceRegistration()
    {
        TTL = 3600; // 1小時
        bAutoRenew = true;
    }
};

// 服務發現事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceDiscovered, const FServiceEndpoint&, Endpoint, const FServiceMetadata&, Metadata);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceRegistered, const FString&, ServiceID, const FServiceEndpoint&, Endpoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceUnregistered, const FString&, ServiceID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceEndpointChanged, const FString&, ServiceID, const FServiceEndpoint&, NewEndpoint);

/**
 * 服務註冊中心
 * 提供服務註冊、發現、端點管理和健康檢查功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSServiceRegistry : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSServiceRegistry();

    // 初始化和控制
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool InitializeRegistry(UMingRTSServiceManager* ServiceManager);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void ShutdownRegistry();

    UFUNCTION(BlueprintPure, Category = "Service Registry")
    bool IsInitialized() const { return bInitialized; }

    // 服務註冊
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    FString RegisterService(const FServiceRegistration& Registration);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool UnregisterService(const FString& ServiceID, const FString& Reason = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool RenewRegistration(const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool UpdateServiceEndpoint(const FString& ServiceID, const FServiceEndpoint& NewEndpoint);

    // 服務發現
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FServiceEndpoint> DiscoverServices(const FServiceQuery& Query);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FServiceEndpoint> GetAllServices() const;

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    FServiceEndpoint GetServiceEndpoint(const FString& ServiceID) const;

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FServiceEndpoint> GetServicesByType(const FString& ServiceType) const;

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FServiceEndpoint> GetServicesByCapability(const FString& Capability) const;

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool IsServiceAvailable(const FString& ServiceID) const;

    // 端點管理
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool AddServiceEndpoint(const FString& ServiceID, const FServiceEndpoint& Endpoint);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool RemoveServiceEndpoint(const FString& ServiceID, const FString& EndpointID);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FServiceEndpoint> GetServiceEndpoints(const FString& ServiceID) const;

    // 健康檢查
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void EnableHealthCheck(bool bEnabled, int32 CheckInterval = 30000);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void SetHealthCheckTimeout(int32 TimeoutMs);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    bool IsServiceHealthy(const FString& ServiceID) const;

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    TArray<FString> GetUnhealthyServices() const;

    // 服務監控
    UFUNCTION(BlueprintPure, Category = "Service Registry")
    int32 GetRegisteredServiceCount() const;

    UFUNCTION(BlueprintPure, Category = "Service Registry")
    int32 GetHealthyServiceCount() const;

    UFUNCTION(BlueprintPure, Category = "Service Registry")
    int32 GetUnhealthyServiceCount() const;

    UFUNCTION(BlueprintPure, Category = "Service Registry")
    TArray<FString> GetExpiredServices() const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void SetDiscoveryProtocol(EServiceDiscoveryProtocol Protocol);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void SetDefaultTTL(int32 TTLSeconds);

    UFUNCTION(BlueprintCallable, Category = "Service Registry")
    void SetMaxRegistrations(int32 MaxCount);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Service Registry Events")
    FOnServiceDiscovered OnServiceDiscovered;

    UPROPERTY(BlueprintAssignable, Category = "Service Registry Events")
    FOnServiceRegistered OnServiceRegistered;

    UPROPERTY(BlueprintAssignable, Category = "Service Registry Events")
    FOnServiceUnregistered OnServiceUnregistered;

    UPROPERTY(BlueprintAssignable, Category = "Service Registry Events")
    FOnServiceEndpointChanged OnServiceEndpointChanged;

protected:
    // 服務管理器引用
    UPROPERTY()
    TObjectPtr<UMingRTSServiceManager> ServiceManager;

    // 服務註冊存儲
    UPROPERTY()
    TMap<FString, FServiceRegistration> ServiceRegistrations;

    UPROPERTY()
    TMap<FString, TArray<FServiceEndpoint>> ServiceEndpoints;

    // 服務索引
    UPROPERTY()
    TMap<FString, TArray<FString>> TypeIndex; // 服務類型 -> 服務ID列表

    UPROPERTY()
    TMap<FString, TArray<FString>> CapabilityIndex; // 能力 -> 服務ID列表

    // 配置
    UPROPERTY()
    EServiceDiscoveryProtocol DiscoveryProtocol;

    UPROPERTY()
    int32 DefaultTTL;

    UPROPERTY()
    int32 MaxRegistrations;

    UPROPERTY()
    bool bHealthCheckEnabled;

    UPROPERTY()
    int32 HealthCheckInterval;

    UPROPERTY()
    int32 HealthCheckTimeout;

    // 狀態
    UPROPERTY()
    bool bInitialized;

    // 同步機制
    mutable FCriticalSection RegistrationCriticalSection;
    mutable FCriticalSection EndpointCriticalSection;
    mutable FCriticalSection IndexCriticalSection;

    // 內部方法
    bool ValidateServiceRegistration(const FServiceRegistration& Registration) const;
    bool ValidateServiceEndpoint(const FServiceEndpoint& Endpoint) const;
    void UpdateServiceIndexes(const FString& ServiceID, const FServiceRegistration& Registration);
    void RemoveFromServiceIndexes(const FString& ServiceID);
    void PerformHealthCheck(const FString& ServiceID);
    void CleanupExpiredRegistrations();
    void RenewExpiredRegistrations();
    bool IsRegistrationExpired(const FServiceRegistration& Registration) const;
    void UpdateEndpointHealth(const FString& ServiceID, const FString& EndpointID, bool bHealthy);

    // 服務查詢
    TArray<FServiceEndpoint> QueryServices(const FServiceQuery& Query) const;
    bool MatchesQuery(const FServiceEndpoint& Endpoint, const FServiceQuery& Query) const;
    bool HasRequiredCapabilities(const FServiceEndpoint& Endpoint, const TArray<FString>& RequiredCapabilities) const;
    bool MatchesMetadataFilter(const FServiceEndpoint& Endpoint, const TMap<FString, FString>& Filter) const;

    // 網絡通信
    bool SendHeartbeat(const FString& ServiceID);
    bool SendServiceAdvertisement(const FServiceRegistration& Registration);
    bool SendServiceQuery(const FServiceQuery& Query);
    void HandleServiceDiscoveryResponse(const FString& Response);

private:
    // 初始化方法
    bool InitializeIndexes();
    bool StartHealthCheckTimer();

    // 清理方法
    void CleanupRegistrations();
    void CleanupEndpoints();
    void CleanupIndexes();

    // 輔助方法
    FString GenerateRegistrationID() const;
    FString GenerateEndpointID() const;
    bool IsValidServiceID(const FString& ServiceID) const;
    bool IsValidEndpointID(const FString& EndpointID) const;
    void LogRegistryOperation(const FString& Operation, const FString& ServiceID, const FString& Details = TEXT(""));
};
