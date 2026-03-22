// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud Save Sync System - C2-1
// Provides cloud save synchronization and cross-device save management

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingCloudSaveSyncSystem.generated.h"

UENUM(BlueprintType)
enum class ECloudProvider: uint8 {
    None = 0, UMETA(DisplayName = "None"),
    EpicOnlineServices, UMETA(DisplayName = "Epic Online Services"),
    SteamCloud, UMETA(DisplayName = "Steam Cloud"),
    XboxLive, UMETA(DisplayName = "Xbox Live"),
    PlayStationNetwork, UMETA(DisplayName = "PlayStation Network"),
    NintendoSwitch, UMETA(DisplayName = "Nintendo Switch"),
    GooglePlay, UMETA(DisplayName = "Google Play"),
    AppleGameCenter, UMETA(DisplayName = "Apple Game Center"),
    AWS, UMETA(DisplayName = "Amazon Web Services"),
    Azure, UMETA(DisplayName = "Microsoft Azure"),
    Custom UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ESyncStatus: uint8 {
    Idle = 0, UMETA(DisplayName = "Idle"),
    Syncing, UMETA(DisplayName = "Syncing"),
    Downloading, UMETA(DisplayName = "Downloading"),
    Uploading, UMETA(DisplayName = "Uploading"),
    ConflictResolution, UMETA(DisplayName = "Conflict Resolution"),
    Completed, UMETA(DisplayName = "Completed"),
    Failed, UMETA(DisplayName = "Failed"),
    Cancelled UMETA(DisplayName = "Cancelled")
};

UENUM(BlueprintType)
enum class EConflictResolutionStrategy: uint8 {
    PreferLocal = 0, UMETA(DisplayName = "Prefer Local"),
    PreferCloud, UMETA(DisplayName = "Prefer Cloud"),
    MostRecent, UMETA(DisplayName = "Most Recent"),
    AskUser, UMETA(DisplayName = "Ask User"),
    KeepBoth UMETA(DisplayName = "Keep Both")
};

USTRUCT(BlueprintType)
struct FCloudSaveMetadata
{
    GENERATED_BODY()

    UPROPERTY()
    FString SaveID;

    UPROPERTY()
    FString PlayerID;

    UPROPERTY()
    FString DeviceID;

    UPROPERTY()
    FString SaveName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    int32 SaveSlot;

    UPROPERTY()
    int32 FileSize;

    UPROPERTY()
    uint32 CreatedTimestamp;

    UPROPERTY()
    uint32 ModifiedTimestamp;

    UPROPERTY()
    uint32 SyncedTimestamp;

    UPROPERTY()
    FString Checksum;

    UPROPERTY()
    FString Version;

    UPROPERTY()
    TMap<FString, FString> CustomMetadata;

    FCloudSaveMetadata()
        : SaveSlot(0)
        , FileSize(0)
        , CreatedTimestamp(0)
        , ModifiedTimestamp(0)
        , SyncedTimestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FCloudSaveConflict
{
    GENERATED_BODY()

    UPROPERTY()
    FString SaveID;

    UPROPERTY()
    FCloudSaveMetadata LocalSave;

    UPROPERTY()
    FCloudSaveMetadata CloudSave;

    UPROPERTY()
    uint32 LocalTimestamp;

    UPROPERTY()
    uint32 CloudTimestamp;

    UPROPERTY()
    FString LocalChecksum;

    UPROPERTY()
    FString CloudChecksum;

    UPROPERTY()
    bool bDifferentContent;

    FCloudSaveConflict()
        : LocalTimestamp(0)
        , CloudTimestamp(0)
        , bDifferentContent(false)
    {}
};

USTRUCT(BlueprintType)
struct FCloudSyncConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    ECloudProvider Provider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bAutoSync;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    float AutoSyncInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnSave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    EConflictResolutionStrategy ConflictStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    int32 MaxCloudSaves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    int32 MaxSaveSizeMB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bEnableCompression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bEnableEncryption;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    FString EncryptionKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncOnWifiOnly;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud Sync")
    bool bSyncInBackground;

    FCloudSyncConfig()
        : Provider(ECloudProvider::None)
        , bAutoSync(true)
        , AutoSyncInterval(300.0f)
        , bSyncOnSave(true)
        , bSyncOnLoad(false)
        , ConflictStrategy(EConflictResolutionStrategy::MostRecent)
        , MaxCloudSaves(10)
        , MaxSaveSizeMB(100)
        , bEnableCompression(true)
        , bEnableEncryption(true)
        , bSyncOnWifiOnly(false)
        , bSyncInBackground(true)
    {}
};

USTRUCT(BlueprintType)
struct FDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString DeviceID;

    UPROPERTY()
    FString DeviceName;

    UPROPERTY()
    FString Platform;

    UPROPERTY()
    FString OSVersion;

    UPROPERTY()
    FString LastSyncTimestamp;

    UPROPERTY()
    bool bIsOnline;

    FDeviceInfo()
        : bIsOnline(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSyncStarted, ESyncStatus, SyncType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudSyncProgress, float, Progress, int32, BytesTransferred);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSyncCompleted, FCloudSaveMetadata, SaveMetadata);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSyncFailed, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudConflictDetected, FCloudSaveConflict, Conflict);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSaveDownloaded, FCloudSaveMetadata, SaveMetadata);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudSaveUploaded, FCloudSaveMetadata, SaveMetadata);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllDevicesSynced);

/**
 * Cloud Save Sync System
 * Provides cloud save synchronization and cross-device save management
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingCloudSaveSyncSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCloudSaveSyncSystem();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void InitializeCloudSystem(const FCloudSyncConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void ShutdownCloudSystem();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool ConnectToCloudProvider(ECloudProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void DisconnectFromCloud();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool IsConnectedToCloud() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    ECloudProvider GetConnectedProvider() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool UploadSaveToCloud(const FString& LocalSavePath, const FCloudSaveMetadata& Metadata);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool DownloadSaveFromCloud(const FString& SaveID, const FString& LocalDestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool SyncSaveWithCloud(int32 SaveSlot);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool SyncAllSaves();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    TArray<FCloudSaveMetadata> ListCloudSaves();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool DeleteCloudSave(const FString& SaveID);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool ResolveConflict(const FCloudSaveConflict& Conflict, EConflictResolutionStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    FCloudSaveMetadata GetCloudSaveMetadata(const FString& SaveID);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool CompareLocalAndCloud(const FString& SaveID, bool& OutIsDifferent);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void EnableAutoSync(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool IsAutoSyncEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void SetAutoSyncInterval(float IntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void ForceCloudSync();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void CancelCurrentSync();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    TArray<FDeviceInfo> GetSyncedDevices();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool IsSaveSyncedAcrossDevices(const FString& SaveID);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void SetConflictResolutionStrategy(EConflictResolutionStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool BackupAllSavesToCloud();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    bool RestoreSavesFromCloudBackup();

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void ExportCloudSavesToLocal(const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void ImportLocalSavesToCloud(const FString& ImportPath);

    UFUNCTION(BlueprintPure, Category = "Cloud Save")
    ESyncStatus GetCurrentSyncStatus() const;

    UFUNCTION(BlueprintPure, Category = "Cloud Save")
    float GetSyncProgress() const;

    UFUNCTION(BlueprintPure, Category = "Cloud Save")
    int32 GetCloudStorageUsed() const;

    UFUNCTION(BlueprintPure, Category = "Cloud Save")
    int32 GetCloudStorageTotal() const;

    UFUNCTION(BlueprintPure, Category = "Cloud Save")
    int32 GetRemainingCloudSaves() const;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSyncStarted OnCloudSyncStarted;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSyncProgress OnCloudSyncProgress;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSyncCompleted OnCloudSyncCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSyncFailed OnCloudSyncFailed;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudConflictDetected OnCloudConflictDetected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSaveDownloaded OnCloudSaveDownloaded;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnCloudSaveUploaded OnCloudSaveUploaded;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Events")
    FOnAllDevicesSynced OnAllDevicesSynced;

protected:
    UPROPERTY()
    FCloudSyncConfig Config;

    UPROPERTY()
    ECloudProvider CurrentProvider;

    UPROPERTY()
    ESyncStatus CurrentSyncStatus;

    UPROPERTY()
    float SyncProgress;

    UPROPERTY()
    bool bIsConnected;

    UPROPERTY()
    FTimerHandle AutoSyncTimer;

    UPROPERTY()
    TArray<FCloudSaveMetadata> CloudSaves;

    UPROPERTY()
    TArray<FDeviceInfo> SyncedDevices;

    void StartAutoSync();
    void StopAutoSync();
    void PerformAutoSync();
    bool UploadFileInternal(const FString& LocalPath, const FCloudSaveMetadata& Metadata);
    bool DownloadFileInternal(const FString& SaveID, const FString& DestinationPath);
    FCloudSaveMetadata ReadLocalSaveMetadata(const FString& SavePath);
    void WriteSaveMetadata(const FString& SavePath, const FCloudSaveMetadata& Metadata);
    bool CheckForConflicts(const FCloudSaveMetadata& Local, const FCloudSaveMetadata& Cloud);
    FCloudSaveConflict CreateConflict(const FCloudSaveMetadata& Local, const FCloudSaveMetadata& Cloud);
    bool ApplyConflictResolution(const FCloudSaveConflict& Conflict, EConflictResolutionStrategy Strategy);
    bool CompressSaveData(const FString& SourcePath, const FString& DestPath);
    bool DecompressSaveData(const FString& SourcePath, const FString& DestPath);
    bool EncryptSaveData(const FString& SourcePath, const FString& DestPath, const FString& Key);
    bool DecryptSaveData(const FString& SourcePath, const FString& DestPath, const FString& Key);
    FString CalculateChecksum(const FString& FilePath);
    void UpdateDeviceList();
    void NotifySyncProgress(float Progress, int32 BytesTransferred);
    void LogCloudActivity(const FString& Activity);
    static UMingCloudSaveSyncSystem* Get(UObject* WorldContextObject);
};
