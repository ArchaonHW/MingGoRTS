// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud Services System - Epic 10.1
//
// This system provides cloud-based save synchronization,
// AI computation offloading, and player analytics.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Misc/DateTime.h"
#include "MingCloudServices.generated.h"

// Cloud service types
UENUM(BlueprintType)
enum class ECloudServiceType : uint8
{
    SaveSync            UMETA(DisplayName = "Save Sync"),
    AIComputation       UMETA(DisplayName = "AI Computation"),
    Analytics           UMETA(DisplayName = "Analytics"),
    AssetStreaming      UMETA(DisplayName = "Asset Streaming"),
    Multiplayer         UMETA(DisplayName = "Multiplayer"),
    Leaderboard         UMETA(DisplayName = "Leaderboard"),
    Matchmaking         UMETA(DisplayName = "Matchmaking"),
    CrossPlatform       UMETA(DisplayName = "Cross Platform")
};

// Cloud connection states
UENUM(BlueprintType)
enum class ECloudConnectionState : uint8
{
    Disconnected        UMETA(DisplayName = "Disconnected"),
    Connecting          UMETA(DisplayName = "Connecting"),
    Connected           UMETA(DisplayName = "Connected"),
    Authenticating    UMETA(DisplayName = "Authenticating"),
    Authenticated       UMETA(DisplayName = "Authenticated"),
    Error               UMETA(DisplayName = "Error"),
    RateLimited         UMETA(DisplayName = "Rate Limited")
};

// Cloud save slot data
USTRUCT(BlueprintType)
struct FCloudSaveSlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SlotIndex = 0;

    UPROPERTY(BlueprintReadOnly)
    FString SlotName = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastModified;

    UPROPERTY(BlueprintReadOnly)
    int64 FileSize = 0;

    UPROPERTY(BlueprintReadOnly)
    FString CloudSaveId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    bool bIsSynced = false;

    UPROPERTY(BlueprintReadOnly)
    bool bHasConflict = false;
};

// Cloud save sync configuration
USTRUCT(BlueprintType)
struct FCloudSaveSyncConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAutoSyncEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AutoSyncInterval = 300.0f; // 5 minutes

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSyncOnSave = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bKeepLocalBackup = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxCloudSaves = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompressSaves = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEncryptSaves = true;
};

// Cloud analytics event
USTRUCT(BlueprintType)
struct FCloudAnalyticsEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventName = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString EventCategory = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString SessionId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString PlayerId = TEXT("");
};

// Cloud AI computation request
USTRUCT(BlueprintType)
struct FCloudAIRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString AIType = TEXT(""); // "Strategy", "Pathfinding", "Prediction"

    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> InputData;

    UPROPERTY(BlueprintReadOnly)
    float Priority = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxWaitTime = 30.0f;
};

// Cloud AI computation result
USTRUCT(BlueprintType)
struct FCloudAIResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess = false;

    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> OutputData;

    UPROPERTY(BlueprintReadOnly)
    float ComputationTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage = TEXT("");
};

// Cloud Services System - Main subsystem for cloud integration
UCLASS()
class MINGPERSONAL_API UMingCloudServices : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingCloudServices();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Connection management
    UFUNCTION(BlueprintCallable, Category = "Cloud|Connection")
    bool ConnectToCloud(const FString& ServerUrl, const FString& ApiKey);

    UFUNCTION(BlueprintCallable, Category = "Cloud|Connection")
    void DisconnectFromCloud();

    UFUNCTION(BlueprintCallable, Category = "Cloud|Connection")
    bool AuthenticatePlayer(const FString& PlayerId, const FString& AuthToken);

    UFUNCTION(BlueprintPure, Category = "Cloud|Connection")
    ECloudConnectionState GetConnectionState() const { return ConnectionState; }

    UFUNCTION(BlueprintPure, Category = "Cloud|Connection")
    bool IsConnected() const;

    // Save sync operations
    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void UploadSaveToCloud(int32 SlotIndex, const FString& SlotName);

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void DownloadSaveFromCloud(int32 SlotIndex, const FString& CloudSaveId);

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void DeleteCloudSave(const FString& CloudSaveId);

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void ListCloudSaves(TArray<FCloudSaveSlot>& OutSaveSlots);

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void SyncAllSaves();

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void ResolveSaveConflict(const FString& CloudSaveId, bool bUseCloudVersion);

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void SetSaveSyncConfig(const FCloudSaveSyncConfig& NewConfig);

    UFUNCTION(BlueprintPure, Category = "Cloud|SaveSync")
    FCloudSaveSyncConfig GetSaveSyncConfig() const { return SaveSyncConfig; }

    UFUNCTION(BlueprintCallable, Category = "Cloud|SaveSync")
    void EnableAutoSync(bool bEnable);

    UFUNCTION(BlueprintPure, Category = "Cloud|SaveSync")
    bool IsAutoSyncEnabled() const { return SaveSyncConfig.bAutoSyncEnabled; }

    // Analytics
    UFUNCTION(BlueprintCallable, Category = "Cloud|Analytics")
    void RecordEvent(const FString& EventName, const FString& EventCategory, const TMap<FString, FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Cloud|Analytics")
    void FlushAnalytics();

    UFUNCTION(BlueprintCallable, Category = "Cloud|Analytics")
    void StartAnalyticsSession(const FString& SessionName);

    UFUNCTION(BlueprintCallable, Category = "Cloud|Analytics")
    void EndAnalyticsSession();

    // AI computation
    UFUNCTION(BlueprintCallable, Category = "Cloud|AI")
    void SubmitAIComputation(const FCloudAIRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Cloud|AI")
    void CancelAIComputation(const FString& RequestId);

    UFUNCTION(BlueprintPure, Category = "Cloud|AI")
    bool IsAIComputationSupported() const;

    // Cross-platform
    UFUNCTION(BlueprintCallable, Category = "Cloud|CrossPlatform")
    void SyncPlayerProfile();

    UFUNCTION(BlueprintCallable, Category = "Cloud|CrossPlatform")
    void UploadAchievementProgress(const FString& AchievementId, float Progress);

    UFUNCTION(BlueprintCallable, Category = "Cloud|CrossPlatform")
    void DownloadAchievementProgress();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnCloudConnected OnCloudConnected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnCloudDisconnected OnCloudDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnSaveUploadComplete OnSaveUploadComplete;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnSaveDownloadComplete OnSaveDownloadComplete;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnSaveConflictDetected OnSaveConflictDetected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnAIComputationComplete OnAIComputationComplete;

    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnCloudError OnCloudError;

protected:
    // Internal state
    UPROPERTY()
    ECloudConnectionState ConnectionState = ECloudConnectionState::Disconnected;

    UPROPERTY()
    FString CurrentPlayerId;

    UPROPERTY()
    FString CurrentSessionId;

    UPROPERTY()
    FCloudSaveSyncConfig SaveSyncConfig;

    UPROPERTY()
    TArray<FCloudSaveSlot> CloudSaveCache;

    UPROPERTY()
    TArray<FCloudAnalyticsEvent> PendingAnalyticsEvents;

    UPROPERTY()
    TMap<FString, FCloudAIRequest> PendingAIRequests;

    UPROPERTY()
    float TimeSinceLastSync = 0.0f;

    UPROPERTY()
    bool bPendingSync = false;

    UPROPERTY()
    FString CloudServerUrl;

    UPROPERTY()
    FString CloudApiKey;

    // Internal methods
    void UpdateAutoSync(float DeltaTime);
    void PerformAutoSync();
    void SyncSaveSlot(int32 SlotIndex);
    void CompareLocalAndCloudSaves(int32 SlotIndex, const FCloudSaveSlot& CloudSave);
    void UploadSaveFile(int32 SlotIndex, const FString& LocalPath);
    void DownloadSaveFile(const FString& CloudSaveId, const FString& LocalPath);
    void SendAnalyticsBatch();
    void ProcessAIResponse(const FCloudAIResult& Result);
    void HandleConnectionError(const FString& ErrorMessage);
    void CacheCloudSaveList(const TArray<FCloudSaveSlot>& Saves);
    void LoadCloudConfig();
    void SaveCloudConfig();
    FString GenerateRequestId();
    FString GetLocalSavePath(int32 SlotIndex);
    void CompressAndUpload(const FString& LocalPath, const FString& CloudId);
    void DownloadAndDecompress(const FString& CloudId, const FString& LocalPath);
    void EncryptSaveData(TArray<uint8>& Data);
    void DecryptSaveData(TArray<uint8>& Data);

    // HTTP callbacks
    void OnUploadComplete(bool bSuccess, const FString& Response);
    void OnDownloadComplete(bool bSuccess, const TArray<uint8>& Data);
    void OnListComplete(bool bSuccess, const FString& Response);
    void OnDeleteComplete(bool bSuccess);
    void OnAIComputeComplete(bool bSuccess, const FCloudAIResult& Result);
};

// Event delegates
declare dynamic multicast delegate(FOnCloudConnected);
declare dynamic multicast delegate(FOnCloudDisconnected);
declare dynamic multicast delegate(FOnSaveUploadComplete, int32, SlotIndex, bool, bSuccess);
declare dynamic multicast delegate(FOnSaveDownloadComplete, int32, SlotIndex, bool, bSuccess);
declare dynamic multicast delegate(FOnSaveConflictDetected, const FString&, CloudSaveId, const FDateTime&, LocalTime, const FDateTime&, CloudTime);
declare dynamic multicast delegate(FOnAIComputationComplete, const FCloudAIResult&, Result);
declare dynamic multicast delegate(FOnCloudError, const FString&, ErrorMessage);
