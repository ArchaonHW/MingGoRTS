#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSProcessManager.h"
#include "MingRTSServiceManager.generated.h"

UENUM(BlueprintType)
enum class EServiceType : uint8
{
    Core        UMETA(DisplayName = "Core"),
    AI          UMETA(DisplayName = "AI"),
    Audio       UMETA(DisplayName = "Audio"),
    UI          UMETA(DisplayName = "UI"),
    Network     UMETA(DisplayName = "Network"),
    SaveLoad    UMETA(DisplayName = "SaveLoad"),
    Input       UMETA(DisplayName = "Input"),
    Rendering   UMETA(DisplayName = "Rendering"),
    Physics     UMETA(DisplayName = "Physics"),
    Custom      UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EServiceState : uint8
{
    Uninitialized UMETA(DisplayName = "Uninitialized"),
    Initializing  UMETA(DisplayName = "Initializing"),
    Running       UMETA(DisplayName = "Running"),
    Suspended     UMETA(DisplayName = "Suspended"),
    Stopping      UMETA(DisplayName = "Stopping"),
    Stopped       UMETA(DisplayName = "Stopped"),
    Error         UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EServicePriority : uint8
{
    Critical     UMETA(DisplayName = "Critical"),
    High         UMETA(DisplayName = "High"),
    Normal       UMETA(DisplayName = "Normal"),
    Low          UMETA(DisplayName = "Low"),
    Background   UMETA(DisplayName = "Background")
};

USTRUCT(BlueprintType)
struct FServiceInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    FString ServiceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    FString ServiceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    EServiceType ServiceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    EServiceState State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    EServicePriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    TMap<FString, FString> ServiceData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    float CPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    FDateTime LastUpdateTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Info")
    bool bAutoRestart;
};

USTRUCT(BlueprintType)
struct FServiceRegistration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    FString ServiceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    EServiceType ServiceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    EServicePriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    bool bAutoRestart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Registration")
    TMap<FString, FString> InitialData;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceRegistered, const FString&, ServiceID, const FServiceInfo&, ServiceInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceStateChanged, const FString&, ServiceID, EServiceState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServiceUnregistered, const FString&, ServiceID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServiceError, const FString&, ServiceID);

/**
 * 內建作業系統服務管理器
 * 負責系統服務的註冊、管理、監控和協調
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSServiceManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSServiceManager();

    // 初始化服務管理器
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    void InitializeServiceManager();

    // 註冊服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    FString RegisterService(const FServiceRegistration& Registration);

    // 註銷服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool UnregisterService(const FString& ServiceID);

    // 啟動服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool StartService(const FString& ServiceID);

    // 停止服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool StopService(const FString& ServiceID);

    // 重啟服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool RestartService(const FString& ServiceID);

    // 掛起服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool SuspendService(const FString& ServiceID);

    // 恢復服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool ResumeService(const FString& ServiceID);

    // 獲取服務信息
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    FServiceInfo GetServiceInfo(const FString& ServiceID) const;

    // 獲取所有服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FServiceInfo> GetAllServices() const;

    // 按類型獲取服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FServiceInfo> GetServicesByType(EServiceType ServiceType) const;

    // 按狀態獲取服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FServiceInfo> GetServicesByState(EServiceState State) const;

    // 按優先級獲取服務
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FServiceInfo> GetServicesByPriority(EServicePriority Priority) const;

    // 更新服務狀態
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool UpdateServiceState(const FString& ServiceID, EServiceState NewState);

    // 更新服務數據
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool UpdateServiceData(const FString& ServiceID, const TMap<FString, FString>& NewData);

    // 檢查服務是否存在
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool DoesServiceExist(const FString& ServiceID) const;

    // 獲取服務依賴
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FString> GetServiceDependencies(const FString& ServiceID) const;

    // 獲取服務依賴者
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FString> GetServiceDependents(const FString& ServiceID) const;

    // 啟動服務依賴鏈
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool StartServiceDependencyChain(const FString& ServiceID);

    // 停止服務依賴鏈
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool StopServiceDependencyChain(const FString& ServiceID);

    // 獲取系統服務統計
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TMap<EServiceState, int32> GetSystemServiceStats() const;

    // 獲取服務性能統計
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TMap<FString, float> GetServicePerformanceStats(const FString& ServiceID) const;

    // 設置服務優先級
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool SetServicePriority(const FString& ServiceID, EServicePriority NewPriority);

    // 設置自動重啟
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    bool SetServiceAutoRestart(const FString& ServiceID, bool bAutoRestart);

    // 執行服務健康檢查
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TMap<FString, bool> PerformServiceHealthCheck();

    // 獲取服務啟動順序
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    TArray<FString> GetServiceStartupOrder();

    // 生成服務報告
    UFUNCTION(BlueprintCallable, Category = "Service Manager")
    FString GenerateServiceReport() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Service Manager")
    FOnServiceRegistered OnServiceRegistered;

    UPROPERTY(BlueprintAssignable, Category = "Service Manager")
    FOnServiceStateChanged OnServiceStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Service Manager")
    FOnServiceUnregistered OnServiceUnregistered;

    UPROPERTY(BlueprintAssignable, Category = "Service Manager")
    FOnServiceError OnServiceError;

protected:
    // 生成唯一服務ID
    FString GenerateServiceID() const;

    // 驗證服務註冊參數
    bool ValidateServiceRegistration(const FServiceRegistration& Registration) const;

    // 檢查服務依賴
    bool CheckServiceDependencies(const TArray<FString>& Dependencies) const;

    // 解決服務依賴
    TArray<FString> ResolveServiceDependencies(const FString& ServiceID) const;

    // 啟動服務內部實現
    bool StartServiceInternal(const FString& ServiceID);

    // 停止服務內部實現
    bool StopServiceInternal(const FString& ServiceID);

    // 更新服務性能數據
    void UpdateServicePerformanceData();

    // 執行服務監控
    void PerformServiceMonitoring();

    // 處理服務錯誤
    void HandleServiceError(const FString& ServiceID, const FString& ErrorMessage);

    // 檢查服務健康狀態
    void CheckServiceHealth();

    // 自動重啟失敗服務
    void AutoRestartFailedServices();

    // 獲取服務啟動延遲
    float GetServiceStartupDelay(EServicePriority Priority) const;

protected:
    // 活躍服務列表
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    TMap<FString, FServiceInfo> ActiveServices;

    // 服務依賴關係圖
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    TMap<FString, TArray<FString>> ServiceDependencies;

    // 服務依賴者關係圖
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    TMap<FString, TArray<FString>> ServiceDependents;

    // 服務ID計數器
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    static int32 ServiceIDCounter;

    // 最後監控時間
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    FDateTime LastMonitoringTime;

    // 監控間隔（秒）
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    float MonitoringInterval;

    // 是否啟用自動監控
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    bool bAutoMonitoringEnabled;

    // 是否啟用自動重啟
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    bool bAutoRestartEnabled;

    // 系統資源使用情況
    UPROPERTY(VisibleDefaultsOnly, Category = "Service Manager")
    TMap<FString, float> SystemResourceUsage;
};
