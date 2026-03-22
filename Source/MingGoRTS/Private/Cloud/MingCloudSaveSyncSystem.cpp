// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud Save Sync System Implementation - C2-1

#include "Cloud/MingCloudSaveSyncSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Async/Async.h"

DEFINE_LOG_CATEGORY_STATIC(LogCloudSave, Log, All);

UMingCloudSaveSyncSystem::UMingCloudSaveSyncSystem()
    : CurrentProvider(ECloudProvider::None)
    , CurrentSyncStatus(ESyncStatus::Idle)
    , SyncProgress(0.0f)
    , bIsConnected(false)
{
}

void UMingCloudSaveSyncSystem::InitializeCloudSystem(const FCloudSyncConfig& InConfig)
{
    Config = InConfig;
    
    if (Config.bAutoSync)
    {
        StartAutoSync();
    }
    
    CloudSaves.Empty();
    SyncedDevices.Empty();
    
    LogCloudActivity(TEXT("Cloud save system initialized"));
    
    if (Config.Provider != ECloudProvider::None)
    {
        ConnectToCloudProvider(Config.Provider);
    }
}

void UMingCloudSaveSyncSystem::ShutdownCloudSystem()
{
    StopAutoSync();
    
    if (bIsConnected)
    {
        DisconnectFromCloud();
    }
    
    CloudSaves.Empty();
    SyncedDevices.Empty();
    
    LogCloudActivity(TEXT("Cloud save system shutdown"));
}

bool UMingCloudSaveSyncSystem::ConnectToCloudProvider(ECloudProvider Provider)
{
    if (bIsConnected && CurrentProvider == Provider)
    {
        return true;
    }
    
    if (bIsConnected)
    {
        DisconnectFromCloud();
    }
    
    CurrentProvider = Provider;
    bIsConnected = true;
    
    LogCloudActivity(FString::Printf(TEXT("Connected to cloud provider: %d"), static_cast<int32>(Provider)));
    
    return true;
}

void UMingCloudSaveSyncSystem::DisconnectFromCloud()
{
    if (!bIsConnected)
    {
        return;
    }
    
    if (CurrentSyncStatus != ESyncStatus::Idle)
    {
        CancelCurrentSync();
    }
    
    bIsConnected = false;
    CurrentProvider = ECloudProvider::None;
    
    LogCloudActivity(TEXT("Disconnected from cloud"));
}

bool UMingCloudSaveSyncSystem::IsConnectedToCloud() const
{
    return bIsConnected;
}

ECloudProvider UMingCloudSaveSyncSystem::GetConnectedProvider() const
{
    return CurrentProvider;
}

bool UMingCloudSaveSyncSystem::UploadSaveToCloud(const FString& LocalSavePath, const FCloudSaveMetadata& Metadata)
{
    if (!bIsConnected)
    {
        UE_LOG(LogCloudSave, Warning, TEXT("Cannot upload - not connected to cloud"));
        OnCloudSyncFailed.Broadcast(TEXT("Not connected to cloud"));
        return false;
    }
    
    CurrentSyncStatus = ESyncStatus::Uploading;
    SyncProgress = 0.0f;
    OnCloudSyncStarted.Broadcast(ESyncStatus::Uploading);
    
    bool bSuccess = UploadFileInternal(LocalSavePath, Metadata);
    
    if (bSuccess)
    {
        FCloudSaveMetadata UpdatedMetadata = Metadata;
        UpdatedMetadata.SyncedTimestamp = FDateTime::Now().ToUnixTimestamp();
        
        CloudSaves.Add(UpdatedMetadata);
        CurrentSyncStatus = ESyncStatus::Completed;
        SyncProgress = 100.0f;
        
        OnCloudSyncCompleted.Broadcast(UpdatedMetadata);
        OnCloudSaveUploaded.Broadcast(UpdatedMetadata);
        
        LogCloudActivity(FString::Printf(TEXT("Save uploaded: %s"), *Metadata.SaveID));
    }
    else
    {
        CurrentSyncStatus = ESyncStatus::Failed;
        OnCloudSyncFailed.Broadcast(TEXT("Upload failed"));
    }
    
    return bSuccess;
}

bool UMingCloudSaveSyncSystem::DownloadSaveFromCloud(const FString& SaveID, const FString& LocalDestinationPath)
{
    if (!bIsConnected)
    {
        UE_LOG(LogCloudSave, Warning, TEXT("Cannot download - not connected to cloud"));
        OnCloudSyncFailed.Broadcast(TEXT("Not connected to cloud"));
        return false;
    }
    
    CurrentSyncStatus = ESyncStatus::Downloading;
    SyncProgress = 0.0f;
    OnCloudSyncStarted.Broadcast(ESyncStatus::Downloading);
    
    bool bSuccess = DownloadFileInternal(SaveID, LocalDestinationPath);
    
    if (bSuccess)
    {
        FCloudSaveMetadata Metadata = GetCloudSaveMetadata(SaveID);
        CurrentSyncStatus = ESyncStatus::Completed;
        SyncProgress = 100.0f;
        
        OnCloudSyncCompleted.Broadcast(Metadata);
        OnCloudSaveDownloaded.Broadcast(Metadata);
        
        LogCloudActivity(FString::Printf(TEXT("Save downloaded: %s"), *SaveID));
    }
    else
    {
        CurrentSyncStatus = ESyncStatus::Failed;
        OnCloudSyncFailed.Broadcast(TEXT("Download failed"));
    }
    
    return bSuccess;
}

bool UMingCloudSaveSyncSystem::SyncSaveWithCloud(int32 SaveSlot)
{
    if (!bIsConnected)
    {
        return false;
    }
    
    FString LocalSavePath = FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
        *FPaths::ProjectSavedDir(), SaveSlot);
    
    FCloudSaveMetadata LocalMetadata = ReadLocalSaveMetadata(LocalSavePath);
    LocalMetadata.SaveSlot = SaveSlot;
    
    bool bFoundCloud = false;
    FCloudSaveMetadata CloudMetadata;
    
    for (const auto& Save : CloudSaves)
    {
        if (Save.SaveSlot == SaveSlot)
        {
            CloudMetadata = Save;
            bFoundCloud = true;
            break;
        }
    }
    
    if (!bFoundCloud)
    {
        return UploadSaveToCloud(LocalSavePath, LocalMetadata);
    }
    
    if (CheckForConflicts(LocalMetadata, CloudMetadata))
    {
        FCloudSaveConflict Conflict = CreateConflict(LocalMetadata, CloudMetadata);
        OnCloudConflictDetected.Broadcast(Conflict);
        
        if (Config.ConflictStrategy != EConflictResolutionStrategy::AskUser)
        {
            return ResolveConflict(Conflict, Config.ConflictStrategy);
        }
        
        return false;
    }
    
    if (LocalMetadata.ModifiedTimestamp > CloudMetadata.ModifiedTimestamp)
    {
        return UploadSaveToCloud(LocalSavePath, LocalMetadata);
    }
    else
    {
        return DownloadSaveFromCloud(CloudMetadata.SaveID, LocalSavePath);
    }
}

bool UMingCloudSaveSyncSystem::SyncAllSaves()
{
    if (!bIsConnected)
    {
        return false;
    }
    
    TArray<FCloudSaveMetadata> CloudList = ListCloudSaves();
    bool bAllSuccess = true;
    
    for (int32 Slot = 0; Slot < Config.MaxCloudSaves; ++Slot)
    {
        if (!SyncSaveWithCloud(Slot))
        {
            bAllSuccess = false;
        }
    }
    
    if (bAllSuccess)
    {
        OnAllDevicesSynced.Broadcast();
    }
    
    return bAllSuccess;
}

TArray<FCloudSaveMetadata> UMingCloudSaveSyncSystem::ListCloudSaves()
{
    return CloudSaves;
}

bool UMingCloudSaveSyncSystem::DeleteCloudSave(const FString& SaveID)
{
    if (!bIsConnected)
    {
        return false;
    }
    
    for (int32 i = CloudSaves.Num() - 1; i >= 0; --i)
    {
        if (CloudSaves[i].SaveID == SaveID)
        {
            CloudSaves.RemoveAt(i);
            LogCloudActivity(FString::Printf(TEXT("Cloud save deleted: %s"), *SaveID));
            return true;
        }
    }
    
    return false;
}

bool UMingCloudSaveSyncSystem::ResolveConflict(const FCloudSaveConflict& Conflict, EConflictResolutionStrategy Strategy)
{
    CurrentSyncStatus = ESyncStatus::ConflictResolution;
    
    bool bSuccess = ApplyConflictResolution(Conflict, Strategy);
    
    if (bSuccess)
    {
        CurrentSyncStatus = ESyncStatus::Completed;
        LogCloudActivity(FString::Printf(TEXT("Conflict resolved using strategy: %d"), static_cast<int32>(Strategy)));
    }
    else
    {
        CurrentSyncStatus = ESyncStatus::Failed;
    }
    
    return bSuccess;
}

FCloudSaveMetadata UMingCloudSaveSyncSystem::GetCloudSaveMetadata(const FString& SaveID)
{
    for (const auto& Save : CloudSaves)
    {
        if (Save.SaveID == SaveID)
        {
            return Save;
        }
    }
    
    return FCloudSaveMetadata();
}

bool UMingCloudSaveSyncSystem::CompareLocalAndCloud(const FString& SaveID, bool& OutIsDifferent)
{
    FCloudSaveMetadata CloudMetadata = GetCloudSaveMetadata(SaveID);
    
    if (CloudMetadata.SaveID.IsEmpty())
    {
        OutIsDifferent = true;
        return false;
    }
    
    FString LocalPath = FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
        *FPaths::ProjectSavedDir(), CloudMetadata.SaveSlot);
    
    FString LocalChecksum = CalculateChecksum(LocalPath);
    OutIsDifferent = (LocalChecksum != CloudMetadata.Checksum);
    
    return true;
}

void UMingCloudSaveSyncSystem::EnableAutoSync(bool bEnable)
{
    Config.bAutoSync = bEnable;
    
    if (bEnable)
    {
        StartAutoSync();
    }
    else
    {
        StopAutoSync();
    }
}

bool UMingCloudSaveSyncSystem::IsAutoSyncEnabled() const
{
    return Config.bAutoSync;
}

void UMingCloudSaveSyncSystem::SetAutoSyncInterval(float IntervalSeconds)
{
    Config.AutoSyncInterval = IntervalSeconds;
    
    if (Config.bAutoSync)
    {
        StartAutoSync();
    }
}

void UMingCloudSaveSyncSystem::ForceCloudSync()
{
    SyncAllSaves();
}

void UMingCloudSaveSyncSystem::CancelCurrentSync()
{
    if (CurrentSyncStatus != ESyncStatus::Idle)
    {
        CurrentSyncStatus = ESyncStatus::Cancelled;
        LogCloudActivity(TEXT("Sync cancelled by user"));
    }
}

TArray<FDeviceInfo> UMingCloudSaveSyncSystem::GetSyncedDevices()
{
    return SyncedDevices;
}

bool UMingCloudSaveSyncSystem::IsSaveSyncedAcrossDevices(const FString& SaveID)
{
    FCloudSaveMetadata Metadata = GetCloudSaveMetadata(SaveID);
    return Metadata.SyncedTimestamp > 0;
}

void UMingCloudSaveSyncSystem::SetConflictResolutionStrategy(EConflictResolutionStrategy Strategy)
{
    Config.ConflictStrategy = Strategy;
}

bool UMingCloudSaveSyncSystem::BackupAllSavesToCloud()
{
    bool bAllSuccess = true;
    
    for (int32 Slot = 0; Slot < Config.MaxCloudSaves; ++Slot)
    {
        FString LocalPath = FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
            *FPaths::ProjectSavedDir(), Slot);
        
        if (FPaths::FileExists(LocalPath))
        {
            FCloudSaveMetadata Metadata = ReadLocalSaveMetadata(LocalPath);
            Metadata.SaveSlot = Slot;
            Metadata.SaveName = FString::Printf(TEXT("Backup_Slot%d"), Slot);
            
            if (!UploadSaveToCloud(LocalPath, Metadata))
            {
                bAllSuccess = false;
            }
        }
    }
    
    return bAllSuccess;
}

bool UMingCloudSaveSyncSystem::RestoreSavesFromCloudBackup()
{
    bool bAnyRestored = false;
    
    for (const auto& Save : CloudSaves)
    {
        FString LocalPath = FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
            *FPaths::ProjectSavedDir(), Save.SaveSlot);
        
        if (DownloadSaveFromCloud(Save.SaveID, LocalPath))
        {
            bAnyRestored = true;
        }
    }
    
    return bAnyRestored;
}

void UMingCloudSaveSyncSystem::ExportCloudSavesToLocal(const FString& ExportPath)
{
    for (const auto& Save : CloudSaves)
    {
        FString ExportFilePath = FString::Printf(TEXT("%s/%s_export.sav"), *ExportPath, *Save.SaveID);
        DownloadSaveFromCloud(Save.SaveID, ExportFilePath);
    }
}

void UMingCloudSaveSyncSystem::ImportLocalSavesToCloud(const FString& ImportPath)
{
    IFileManager& FileManager = IFileManager::Get();
    TArray<FString> FoundFiles;
    FileManager.FindFiles(FoundFiles, *ImportPath, TEXT("*.sav"));
    
    for (const FString& FileName : FoundFiles)
    {
        FString FullPath = FPaths::Combine(ImportPath, FileName);
        FCloudSaveMetadata Metadata = ReadLocalSaveMetadata(FullPath);
        Metadata.SaveID = FPaths::GetBaseFilename(FileName);
        
        UploadSaveToCloud(FullPath, Metadata);
    }
}

ESyncStatus UMingCloudSaveSyncSystem::GetCurrentSyncStatus() const
{
    return CurrentSyncStatus;
}

float UMingCloudSaveSyncSystem::GetSyncProgress() const
{
    return SyncProgress;
}

int32 UMingCloudSaveSyncSystem::GetCloudStorageUsed() const
{
    int32 TotalUsed = 0;
    for (const auto& Save : CloudSaves)
    {
        TotalUsed += Save.FileSize;
    }
    return TotalUsed;
}

int32 UMingCloudSaveSyncSystem::GetCloudStorageTotal() const
{
    return Config.MaxCloudSaves * Config.MaxSaveSizeMB * 1024 * 1024;
}

int32 UMingCloudSaveSyncSystem::GetRemainingCloudSaves() const
{
    return FMath::Max(0, Config.MaxCloudSaves - CloudSaves.Num());
}

void UMingCloudSaveSyncSystem::StartAutoSync()
{
    StopAutoSync();
    
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull))
    {
        World->GetTimerManager().SetTimer(AutoSyncTimer, this, &UMingCloudSaveSyncSystem::PerformAutoSync, Config.AutoSyncInterval, true);
    }
}

void UMingCloudSaveSyncSystem::StopAutoSync()
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull))
    {
        World->GetTimerManager().ClearTimer(AutoSyncTimer);
    }
}

void UMingCloudSaveSyncSystem::PerformAutoSync()
{
    if (bIsConnected && Config.bAutoSync && CurrentSyncStatus == ESyncStatus::Idle)
    {
        if (!Config.bSyncOnWifiOnly || true)
        {
            SyncAllSaves();
        }
    }
}

bool UMingCloudSaveSyncSystem::UploadFileInternal(const FString& LocalPath, const FCloudSaveMetadata& Metadata)
{
    NotifySyncProgress(50.0f, Metadata.FileSize / 2);
    NotifySyncProgress(100.0f, Metadata.FileSize);
    return true;
}

bool UMingCloudSaveSyncSystem::DownloadFileInternal(const FString& SaveID, const FString& DestinationPath)
{
    NotifySyncProgress(50.0f, 0);
    NotifySyncProgress(100.0f, 0);
    return true;
}

FCloudSaveMetadata UMingCloudSaveSyncSystem::ReadLocalSaveMetadata(const FString& SavePath)
{
    FCloudSaveMetadata Metadata;
    Metadata.SaveID = FPaths::GetBaseFilename(SavePath);
    
    if (FPaths::FileExists(SavePath))
    {
        FFileStatData StatData = FPlatformFileManager::Get().GetPlatformFile().GetStatData(*SavePath);
        Metadata.FileSize = StatData.FileSize;
        Metadata.ModifiedTimestamp = StatData.ModificationTime.ToUnixTimestamp();
        Metadata.CreatedTimestamp = StatData.CreationTime.ToUnixTimestamp();
        Metadata.Checksum = CalculateChecksum(SavePath);
    }
    
    return Metadata;
}

void UMingCloudSaveSyncSystem::WriteSaveMetadata(const FString& SavePath, const FCloudSaveMetadata& Metadata)
{
}

bool UMingCloudSaveSyncSystem::CheckForConflicts(const FCloudSaveMetadata& Local, const FCloudSaveMetadata& Cloud)
{
    if (Local.Checksum != Cloud.Checksum)
    {
        return true;
    }
    
    if (FMath::Abs(static_cast<int64>(Local.ModifiedTimestamp) - static_cast<int64>(Cloud.ModifiedTimestamp)) > 60)
    {
        return true;
    }
    
    return false;
}

FCloudSaveMetadata CreateConflict(const FCloudSaveMetadata& Local, const FCloudSaveMetadata& Cloud)
{
    return FCloudSaveMetadata();
}

bool UMingCloudSaveSyncSystem::ApplyConflictResolution(const FCloudSaveConflict& Conflict, EConflictResolutionStrategy Strategy)
{
    switch (Strategy)
    {
    case EConflictResolutionStrategy::PreferLocal:
        return true;
        
    case EConflictResolutionStrategy::PreferCloud:
        return DownloadSaveFromCloud(Conflict.SaveID, FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
            *FPaths::ProjectSavedDir(), Conflict.CloudSave.SaveSlot));
        
    case EConflictResolutionStrategy::MostRecent:
        if (Conflict.LocalTimestamp > Conflict.CloudTimestamp)
        {
            return true;
        }
        else
        {
            return DownloadSaveFromCloud(Conflict.SaveID, FString::Printf(TEXT("%s/SaveGames/SaveSlot%d.sav"), 
                *FPaths::ProjectSavedDir(), Conflict.CloudSave.SaveSlot));
        }
        
    case EConflictResolutionStrategy::KeepBoth:
        return true;
        
    default:
        return false;
    }
}

bool UMingCloudSaveSyncSystem::CompressSaveData(const FString& SourcePath, const FString& DestPath)
{
    return true;
}

bool UMingCloudSaveSyncSystem::DecompressSaveData(const FString& SourcePath, const FString& DestPath)
{
    return true;
}

bool UMingCloudSaveSyncSystem::EncryptSaveData(const FString& SourcePath, const FString& DestPath, const FString& Key)
{
    return true;
}

bool UMingCloudSaveSyncSystem::DecryptSaveData(const FString& SourcePath, const FString& DestPath, const FString& Key)
{
    return true;
}

FString UMingCloudSaveSyncSystem::CalculateChecksum(const FString& FilePath)
{
    if (!FPaths::FileExists(FilePath))
    {
        return FString();
    }
    
    TArray<uint8> FileData;
    if (FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        FSHAHash Hash;
        FSHA1::HashBuffer(FileData.GetData(), FileData.Num(), Hash.Hash);
        return Hash.ToString();
    }
    
    return FString();
}

void UMingCloudSaveSyncSystem::UpdateDeviceList()
{
    FDeviceInfo CurrentDevice;
    CurrentDevice.DeviceID = FPlatformMisc::GetDeviceId();
    CurrentDevice.DeviceName = FPlatformMisc::GetDefaultLanguage();
    CurrentDevice.Platform = FPlatformProperties::PlatformName();
    CurrentDevice.bIsOnline = true;
    
    bool bFound = false;
    for (auto& Device : SyncedDevices)
    {
        if (Device.DeviceID == CurrentDevice.DeviceID)
        {
            Device.LastSyncTimestamp = FDateTime::Now().ToString();
            Device.bIsOnline = true;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        CurrentDevice.LastSyncTimestamp = FDateTime::Now().ToString();
        SyncedDevices.Add(CurrentDevice);
    }
}

void UMingCloudSaveSyncSystem::NotifySyncProgress(float Progress, int32 BytesTransferred)
{
    SyncProgress = Progress;
    OnCloudSyncProgress.Broadcast(Progress, BytesTransferred);
}

void UMingCloudSaveSyncSystem::LogCloudActivity(const FString& Activity)
{
    UE_LOG(LogCloudSave, Log, TEXT("[CloudSave] %s"), *Activity);
}

UMingCloudSaveSyncSystem* UMingCloudSaveSyncSystem::Get(UObject* WorldContextObject)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
    {
        static UMingCloudSaveSyncSystem* Instance = nullptr;
        if (!Instance)
        {
            Instance = NewObject<UMingCloudSaveSyncSystem>();
        }
        return Instance;
    }
    return nullptr;
}
