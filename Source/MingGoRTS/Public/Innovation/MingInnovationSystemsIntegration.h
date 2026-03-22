#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "MingCollaborativeAINetwork.h"
#include "MingEcologicalEnvironmentSystem.h"
#include "MingSocialDynamicsSystem.h"
#include "MingInnovationSystemsIntegration.generated.h"

// 系統集成狀態枚舉
UENUM(BlueprintType)
enum class EIntegrationState : uint8
{
    NotInitialized = 0,    // 未初始化
    Initializing = 1,       // 初始化中
    Ready = 2,             // 就緒
    Running = 3,           // 運行中
    Error = 4,             // 錯誤
    Stopped = 5             // 停止
};

// 數據同步類型枚舉
UENUM(BlueprintType)
enum class EDataSyncType : uint8
{
    RealTime = 0,           // 實時同步
    Batch = 1,              // 批量同步
    OnDemand = 2,           // 按需同步
    Scheduled = 3,          // 計劃同步
    EventDriven = 4,         // 事件驅動
    Manual = 5              // 手動同步
};

// 系統交互類型枚舉
UENUM(BlueprintType)
enum class ESystemInteractionType : uint8
{
    DataExchange = 0,       // 數據交換
    EventBroadcast = 1,      // 事件廣播
    ResourceSharing = 2,     // 資源共享
    ServiceCall = 3,         // 服務調用
    StateSync = 4,          // 狀態同步
    Configuration = 5        // 配置管理
};

// 集成配置數據結構
USTRUCT(BlueprintType)
struct FIntegrationConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SystemName;

    UPROPERTY(BlueprintReadOnly)
    FString SystemVersion;

    UPROPERTY(BlueprintReadOnly)
    FString SystemDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> ConnectionParameters;

    UPROPERTY(BlueprintReadOnly)
    EDataSyncType SyncType = EDataSyncType::RealTime;

    UPROPERTY(BlueprintReadOnly)
    float SyncInterval = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoStart = true;

    UPROPERTY(BlueprintReadOnly)
    bool bEnableLogging = true;

    UPROPERTY(BlueprintReadOnly)
    bool bEnableMetrics = true;

    UPROPERTY(BlueprintReadOnly)
    int32 Priority = 1;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CustomSettings;
};

// 系統交互數據結構
USTRUCT(BlueprintType)
struct FSystemInteraction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString InteractionID;

    UPROPERTY(BlueprintReadOnly)
    FString SourceSystem;

    UPROPERTY(BlueprintReadOnly)
    FString TargetSystem;

    UPROPERTY(BlueprintReadOnly)
    ESystemInteractionType InteractionType = ESystemInteractionType::DataExchange;

    UPROPERTY(BlueprintReadOnly)
    FString InteractionData;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    float Priority = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    bool bIsProcessed = false;

    UPROPERTY(BlueprintReadOnly)
    FString Result;

    UPROPERTY(BlueprintReadOnly)
    float ProcessingTime = 0.0f;
};

// 系統性能指標結構
USTRUCT(BlueprintType)
struct FSystemPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SystemID;

    UPROPERTY(BlueprintReadOnly)
    FString SystemName;

    UPROPERTY(BlueprintReadOnly)
    float CPUUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MemoryUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float NetworkLatency = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveConnections = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 ProcessedRequests = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedRequests = 0;

    UPROPERTY(BlueprintReadOnly)
    float AverageResponseTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float Throughput = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;

    UPROPERTY(BlueprintReadOnly)
    bool bIsHealthy = true;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HealthWarnings;
};

// 集成事件數據結構
USTRUCT(BlueprintType)
struct FIntegrationEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString SourceSystem;

    UPROPERTY(BlueprintReadOnly)
    FString TargetSystem;

    UPROPERTY(BlueprintReadOnly)
    FString EventData;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventParameters;

    UPROPERTY(BlueprintReadOnly)
    float Severity = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    bool bIsHandled = false;

    UPROPERTY(BlueprintReadOnly)
    FString HandlingResult;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemInitialized, const FString&, SystemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemShutdown, const FString&, SystemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionProcessed, const FSystemInteraction&, Interaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationEvent, const FIntegrationEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceAlert, const FSystemPerformanceMetrics&, Metrics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemStateChanged, const FString&, SystemID, EIntegrationState NewState);

/**
 * 創新系統集成接口
 * 提供所有創新系統的統一集成和管理功能
 */
UINTERFACE(BlueprintType)
class UInnovationSystemsIntegrationInterface : public UInterface
{
    GENERATED_BODY()
};

class IInnovationSystemsIntegrationInterface
{
    GENERATED_BODY()

public:
    // 初始化集成系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool InitializeIntegrationSystem();

    // 註冊創新系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool RegisterInnovationSystem(const FString& SystemID, const FIntegrationConfiguration& Configuration);

    // 啟動系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool StartSystem(const FString& SystemID);

    // 停止系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool StopSystem(const FString& SystemID);

    // 處理系統交互
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool ProcessSystemInteraction(const FSystemInteraction& Interaction);

    // 同步系統數據
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    bool SynchronizeSystemData(const FString& SystemID, EDataSyncType SyncType);

    // 獲取系統性能指標
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    FSystemPerformanceMetrics GetSystemMetrics(const FString& SystemID) const;

    // 獲取集成狀態
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    EIntegrationState GetIntegrationState(const FString& SystemID) const;

    // 獲取所有系統狀態
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Innovation Systems Integration")
    TMap<FString, EIntegrationState> GetAllSystemStates() const;
};

/**
 * 創新系統集成管理器
 * 實現所有創新系統的統一集成和管理
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Innovation))
class MINGRTS_API UMingInnovationSystemsIntegration : public UObject, public IInnovationSystemsIntegrationInterface
{
    GENERATED_BODY()

public:
    UMingInnovationSystemsIntegration();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool InitializeIntegrationSystem() override;

    // 註冊創新系統
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool RegisterInnovationSystem(const FString& SystemID, const FIntegrationConfiguration& Configuration) override;

    // 啟動系統
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool StartSystem(const FString& SystemID) override;

    // 停止系統
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool StopSystem(const FString& SystemID) override;

    // 處理系統交互
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool ProcessSystemInteraction(const FSystemInteraction& Interaction) override;

    // 同步系統數據
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool SynchronizeSystemData(const FString& SystemID, EDataSyncType SyncType) override;

    // 獲取系統性能指標
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    FSystemPerformanceMetrics GetSystemMetrics(const FString& SystemID) const override;

    // 獲取集成狀態
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    EIntegrationState GetIntegrationState(const FString& SystemID) const override;

    // 獲取所有系統狀態
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    TMap<FString, EIntegrationState> GetAllSystemStates() const override;

    // 高級功能
    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    TArray<FString> GetRegisteredSystems() const;

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    TArray<FSystemInteraction> GetPendingInteractions() const;

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    TArray<FIntegrationEvent> GetRecentEvents(int32 Count = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool ConfigureSystem(const FString& SystemID, const TMap<FString, FString>& Settings);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool UpdateSystemConfiguration(const FString& SystemID, const FIntegrationConfiguration& NewConfiguration);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool RestartSystem(const FString& SystemID);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool UnregisterSystem(const FString& SystemID);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool EnableSystemLogging(const FString& SystemID, bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool EnableSystemMetrics(const FString& SystemID, bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    float GetSystemHealthScore(const FString& SystemID) const;

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    TArray<FString> GetSystemDependencies(const FString& SystemID) const;

    UFUNCTION(BlueprintCallable, Category = "Innovation Systems Integration")
    bool ValidateSystemDependencies(const FString& SystemID) const;

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnSystemInitialized OnSystemInitialized;

    UPROPERTY(BlueprintAssignable)
    FOnSystemShutdown OnSystemShutdown;

    UPROPERTY(BlueprintAssignable)
    FOnInteractionProcessed OnInteractionProcessed;

    UPROPERTY(BlueprintAssignable)
    FOnIntegrationEvent OnIntegrationEvent;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceAlert OnPerformanceAlert;

    UPROPERTY(BlueprintAssignable)
    FOnSystemStateChanged OnSystemStateChanged;

protected:
    // 系統組件
    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TMap<FString, FIntegrationConfiguration> RegisteredSystems;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TMap<FString, EIntegrationState> SystemStates;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TArray<FSystemInteraction> InteractionQueue;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TMap<FString, FSystemPerformanceMetrics> SystemMetrics;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TArray<FIntegrationEvent> EventHistory;

    // 創新系統實例
    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TObjectPtr<UMingCollaborativeAINetwork> CollaborativeAINetwork;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TObjectPtr<UMingEcologicalEnvironmentSystem> EcologicalEnvironmentSystem;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    TObjectPtr<UMingSocialDynamicsSystem> SocialDynamicsSystem;

    // 配置參數
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    float IntegrationUpdateInterval = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    float MetricsUpdateInterval = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    float HealthCheckInterval = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    int32 MaxInteractionQueueSize = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    float PerformanceThreshold = 80.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    bool bEnableAutoRestart = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Innovation Systems Integration")
    bool bEnableHealthMonitoring = true;

    // 狀態變數
    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    bool bIsInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    int32 ActiveSystemCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    float OverallSystemHealth = 100.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Innovation Systems Integration")
    EIntegrationState OverallIntegrationState = EIntegrationState::NotInitialized;

private:
    // 內部方法
    void InitializeDefaultSystems();
    void ProcessInteractionQueue();
    void UpdateSystemMetrics();
    void PerformHealthChecks();
    void HandleSystemEvents();

    bool ValidateSystemRegistration(const FString& SystemID, const FIntegrationConfiguration& Configuration) const;
    bool ValidateInteraction(const FSystemInteraction& Interaction) const;
    bool CheckSystemDependencies(const FString& SystemID) const;

    void StartSystemInternal(const FString& SystemID);
    void StopSystemInternal(const FString& SystemID);
    void RestartSystemInternal(const FString& SystemID);

    FSystemPerformanceMetrics CalculateSystemMetrics(const FString& SystemID) const;
    float CalculateSystemHealth(const FSystemPerformanceMetrics& Metrics) const;
    void UpdateOverallSystemHealth();

    FString GenerateUniqueInteractionID() const;
    FString GenerateUniqueEventID() const;

    void LogIntegrationEvent(const FString& EventType, const FString& SystemID, const FString& Message, float Severity = 1.0f);
    void LogPerformanceMetrics(const FString& SystemID, const FSystemPerformanceMetrics& Metrics);

    // 系統特定的集成方法
    void IntegrateCollaborativeAI();
    void IntegrateEcologicalEnvironment();
    void IntegrateSocialDynamics();

    void SetupCollaborativeAIInteractions();
    void SetupEcologicalEnvironmentInteractions();
    void SetupSocialDynamicsInteractions();

    // 定時器
    FTimerHandle IntegrationUpdateTimerHandle;
    FTimerHandle MetricsUpdateTimerHandle;
    FTimerHandle HealthCheckTimerHandle;
    FTimerHandle InteractionProcessingTimerHandle;

    // 統計數據
    UPROPERTY()
    TMap<FString, float> IntegrationStats;

    UPROPERTY()
    float LastIntegrationUpdateTime = 0.0f;

    UPROPERTY()
    int32 TotalInteractionsProcessed = 0;

    UPROPERTY()
    int32 TotalEventsHandled = 0;

    UPROPERTY()
    int32 TotalSystemRestarts = 0;
};
