// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 10.1: Cloud Services Integration - Enhanced Cloud System

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Cloud/MingRTSCloudSystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "MingRTSCloudIntegrationSystem.generated.h"

/**
 * Cloud Service Providers
 */
UENUM(BlueprintType)
enum class ECloudServiceProvider : uint8 {
    None            UMETA(DisplayName = "None"),
    AWS             UMETA(DisplayName = "Amazon Web Services"),
    Azure           UMETA(DisplayName = "Microsoft Azure"),
    GoogleCloud     UMETA(DisplayName = "Google Cloud Platform"),
    AlibabaCloud    UMETA(DisplayName = "Alibaba Cloud"),
    TencentCloud    UMETA(DisplayName = "Tencent Cloud"),
    Custom          UMETA(DisplayName = "Custom Provider")
};

/**
 * Cloud Service Categories
 */
UENUM(BlueprintType)
enum class ECloudServiceCategory : uint8 {
    Storage         UMETA(DisplayName = "Storage"),
    Compute         UMETA(DisplayName = "Compute"),
    Database        UMETA(DisplayName = "Database"),
    Network         UMETA(DisplayName = "Network"),
    Analytics       UMETA(DisplayName = "Analytics"),
    AI_ML           UMETA(DisplayName = "AI & Machine Learning"),
    Security        UMETA(DisplayName = "Security"),
    Monitoring      UMETA(DisplayName = "Monitoring"),
    ContentDelivery UMETA(DisplayName = "Content Delivery"),
    Gaming          UMETA(DisplayName = "Gaming Services")
};

/**
 * Cloud Data Sync Strategies
 */
UENUM(BlueprintType)
enum class ECloudSyncStrategy : uint8 {
    Immediate       UMETA(DisplayName = "Immediate Sync"),
    Batch           UMETA(DisplayName = "Batch Sync"),
    Scheduled       UMETA(DisplayName = "Scheduled Sync"),
    EventDriven     UMETA(DisplayName = "Event Driven"),
    Hybrid          UMETA(DisplayName = "Hybrid Strategy")
};

/**
 * Cloud Security Levels
 */
UENUM(BlueprintType)
enum class ECloudSecurityLevel : uint8 {
    Basic           UMETA(DisplayName = "Basic"),
    Standard        UMETA(DisplayName = "Standard"),
    Enhanced        UMETA(DisplayName = "Enhanced"),
    Enterprise      UMETA(DisplayName = "Enterprise"),
    Military        UMETA(DisplayName = "Military Grade")
};

/**
 * Cloud Service Configuration
 */
USTRUCT(BlueprintType)
struct FCloudServiceConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    ECloudServiceProvider Provider = ECloudServiceProvider::AWS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    ECloudServiceCategory Category = ECloudServiceCategory::Storage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    FString ServiceEndpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    FString APIKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    FString SecretKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    FString Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    ECloudSecurityLevel SecurityLevel = ECloudSecurityLevel::Standard;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    bool bEnableEncryption = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    bool bEnableCompression = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    int32 TimeoutSeconds = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Service")
    int32 MaxRetries = 3;
};

/**
 * Cloud Sync Configuration
 */
USTRUCT(BlueprintType)
struct FCloudSyncConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    ECloudSyncStrategy SyncStrategy = ECloudSyncStrategy::Immediate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    float SyncIntervalMinutes = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bAutoSync = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnAppStart = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnAppClose = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnlyOnWiFi = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    int64 MaxSyncDataSize = 104857600; // 100MB

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    TArray<FString> SyncDataTypes;
};

/**
 * Cloud Performance Metrics
 */
USTRUCT(BlueprintType)
struct FCloudPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    float AverageLatency = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    float UploadSpeed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    float DownloadSpeed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    int32 SuccessfulRequests = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    int32 FailedRequests = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    int64 TotalBytesTransferred = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    float Uptime = 100.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    FDateTime LastSyncTime;

    UPROPERTY(BlueprintReadOnly, Category = "Cloud Performance")
    bool bIsHealthy = true;
};

/**
 * Cloud Analytics Data
 */
USTRUCT(BlueprintType)
struct FCloudAnalyticsData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    TMap<FString, FString> EventParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    FString UserID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Analytics")
    FString DeviceInfo;

    FCloudAnalyticsData()
    {
        Timestamp = FDateTime::Now();
    }
};

/**
 * Enhanced Cloud Integration System
 * Provides comprehensive cloud services integration with multiple providers
 */
UCLASS()
class MINGRTS_API UMingRTSCloudIntegrationSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    void InitializeCloudIntegration();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    void ShutdownCloudIntegration();

    // Service Configuration
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    void ConfigureCloudService(const FCloudServiceConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    FCloudServiceConfig GetCloudServiceConfig() const { return ServiceConfig; }

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    void SetSyncConfiguration(const FCloudSyncConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    FCloudSyncConfig GetSyncConfiguration() const { return SyncConfig; }

    // Connection Management
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    bool ConnectToCloudService();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    void DisconnectFromCloudService();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    bool IsCloudServiceConnected() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration")
    ECloudServiceProvider GetCurrentProvider() const { return CurrentProvider; }

    // Data Operations
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Data")
    void UploadData(const FString& DataKey, const FString& Data);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Data")
    void DownloadData(const FString& DataKey);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Data")
    void DeleteData(const FString& DataKey);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Data")
    TArray<FString> ListAvailableData();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Data")
    bool IsDataAvailable(const FString& DataKey);

    // Sync Operations
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Sync")
    void StartSync();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Sync")
    void StopSync();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Sync")
    void ForceSync();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Sync")
    bool IsSyncInProgress() const { return bSyncInProgress; }

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Sync")
    FDateTime GetLastSyncTime() const;

    // Analytics
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Analytics")
    void TrackEvent(const FCloudAnalyticsData& AnalyticsData);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Analytics")
    void TrackUserAction(const FString& Action, const TMap<FString, FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Analytics")
    void TrackPerformanceMetrics();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Analytics")
    void FlushAnalytics();

    // Performance Monitoring
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Performance")
    FCloudPerformanceMetrics GetPerformanceMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Performance")
    void ResetPerformanceMetrics();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Performance")
    bool IsServiceHealthy() const;

    // Multi-Provider Support
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Providers")
    void SetPrimaryProvider(ECloudServiceProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Providers")
    void AddBackupProvider(ECloudServiceProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Providers")
    void SwitchToBackupProvider();

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Providers")
    TArray<ECloudServiceProvider> GetAvailableProviders() const;

    // Security
    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Security")
    void SetSecurityLevel(ECloudSecurityLevel Level);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Security")
    ECloudSecurityLevel GetSecurityLevel() const { return CurrentSecurityLevel; }

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Security")
    void EnableEncryption(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Cloud Integration|Security")
    bool IsEncryptionEnabled() const { return bEncryptionEnabled; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudServiceConnected OnCloudServiceConnected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudServiceDisconnected OnCloudServiceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudDataUploaded OnCloudDataUploaded;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudDataDownloaded OnCloudDataDownloaded;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudSyncCompleted OnCloudSyncCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Integration|Events")
    FOnCloudError OnCloudError;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

protected:
    // Core Systems
    UPROPERTY()
    TObjectPtr<UMingRTSCloudSystem> BaseCloudSystem;

    // Configuration
    UPROPERTY()
    FCloudServiceConfig ServiceConfig;

    UPROPERTY()
    FCloudSyncConfig SyncConfig;

    UPROPERTY()
    ECloudServiceProvider CurrentProvider = ECloudServiceProvider::None;

    UPROPERTY()
    ECloudSecurityLevel CurrentSecurityLevel = ECloudSecurityLevel::Standard;

    UPROPERTY()
    bool bEncryptionEnabled = true;

    UPROPERTY()
    bool bCompressionEnabled = true;

    // State
    UPROPERTY()
    bool bIsConnected = false;

    UPROPERTY()
    bool bSyncInProgress = false;

    UPROPERTY()
    FCloudPerformanceMetrics PerformanceMetrics;

    // Provider Management
    UPROPERTY()
    TArray<ECloudServiceProvider> BackupProviders;

    UPROPERTY()
    ECloudServiceProvider ActiveProvider = ECloudServiceProvider::None;

    // Sync Management
    UPROPERTY()
    FTimerHandle SyncTimerHandle;

    UPROPERTY()
    FDateTime LastSuccessfulSync;

    // Analytics
    UPROPERTY()
    TArray<FCloudAnalyticsData> PendingAnalytics;

    UPROPERTY()
    FTimerHandle AnalyticsFlushTimer;

    // HTTP Request Management
    UPROPERTY()
    TArray<TSharedPtr<IHttpRequest>> PendingRequests;

    // Internal Methods
    void InitializeProvider();
    void SetupSyncTimer();
    void SetupAnalyticsTimer();
    void ProcessSyncQueue();
    void ProcessAnalyticsQueue();
    void UpdatePerformanceMetrics(float Latency, bool bSuccess, int64 BytesTransferred);
    FString EncryptData(const FString& Data);
    FString DecryptData(const FString& EncryptedData);
    FString CompressData(const FString& Data);
    FString DecompressData(const FString& CompressedData);
    void HandleHTTPResponse(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response);
    void OnSyncComplete(bool bSuccess);
    void OnDataOperationComplete(const FString& Operation, const FString& DataKey, bool bSuccess);
    void SwitchProvider(ECloudServiceProvider NewProvider);
    bool ValidateConfiguration();
    void LogCloudEvent(const FString& Event, const FString& Details);
    void CleanupPendingRequests();
};

// Event delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudServiceConnected, ECloudServiceProvider, Provider);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudServiceDisconnected, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudDataUploaded, const FString&, DataKey, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudDataDownloaded, const FString&, DataKey, const FString&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSyncCompleted, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudError, const FString&, ErrorType, const FString&, ErrorMessage);
