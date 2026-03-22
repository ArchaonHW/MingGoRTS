#include "Save/MingSaveGameManager.h"
#include "Save/MingSaveGame.h"
#include "MingPersonalManager.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"

UMingSaveGameManager::UMingSaveGameManager()
    : bIsInitialized(false)
    , CurrentSaveSlot(-1)
    , CurrentSaveGame(nullptr)
    , bAutosaveEnabled(true)
    , AutosaveIntervalMinutes(5.0f)
    , AutosaveTimer(0.0f)
    , bCompressionEnabled(true)
    , bEncryptionEnabled(false)
    , bDebugLogging(false)
    , PersonalManager(nullptr)
    , AudioSystem(nullptr)
{
}

void UMingSaveGameManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // Find Personal Manager
    if (UWorld* World = GetWorld())
    {
        // PersonalManager = ...; // Would get from game mode/state
    }

    bIsInitialized = true;
    LogSaveEvent(TEXT("Save Game Manager Initialized"), -1);
}

EMingSaveGameResult UMingSaveGameManager::SaveGame(int32 SlotIndex, const FString& SaveName, bool bIsAutosave)
{
    if (!bIsInitialized)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    // Create save game object
    UMingSaveGame* SaveGame = UMingSaveGame::CreateSaveGame(this, SaveName, SlotIndex);
    if (!SaveGame)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    // Collect all data
    CollectRelationshipData(SaveGame);
    CollectReputationData(SaveGame);
    CollectQuestData(SaveGame);
    CollectAudioSettings(SaveGame);
    CollectUISettings(SaveGame);
    CollectGameState(SaveGame);

    // Set autosave flag
    SaveGame->SetMetadata(TEXT("IsAutosave"), bIsAutosave ? TEXT("true") : TEXT("false"));

    // Compress if enabled
    if (bCompressionEnabled)
    {
        SaveGame->CompressData();
    }

    // Encrypt if enabled
    if (bEncryptionEnabled && !EncryptionKey.IsEmpty())
    {
        SaveGame->EncryptData(EncryptionKey);
    }

    // Validate before saving
    if (!SaveGame->ValidateSaveData())
    {
        LastError = FString::Join(SaveGame->GetValidationErrors(), TEXT("; "));
        return EMingSaveGameResult::Failed_CorruptData;
    }

    // Serialize and write to disk
    TArray<uint8> SaveData;
    SerializeSaveGame(SaveGame, SaveData);

    EMingSaveGameResult Result = WriteSaveToDisk(SlotIndex, SaveData);

    if (Result == EMingSaveGameResult::Success)
    {
        CurrentSaveSlot = SlotIndex;
        CurrentSaveGame = SaveGame;

        // Capture thumbnail
        CaptureSaveThumbnail(SlotIndex);

        LogSaveEvent(TEXT("Save successful"), SlotIndex);
    }
    else
    {
        LogSaveEvent(TEXT("Save failed"), SlotIndex);
    }

    OnSaveGameComplete.Broadcast(Result, SlotIndex);
    return Result;
}

EMingSaveGameResult UMingSaveGameManager::SaveGameWithMetadata(int32 SlotIndex, const FString& SaveName, const TMap<FString, FString>& Metadata)
{
    // Create save first
    EMingSaveGameResult Result = SaveGame(SlotIndex, SaveName, false);

    if (Result == EMingSaveGameResult::Success && CurrentSaveGame)
    {
        // Add metadata
        for (const auto& Pair : Metadata)
        {
            CurrentSaveGame->SetMetadata(Pair.Key, Pair.Value);
        }

        // Re-save with metadata
        TArray<uint8> SaveData;
        SerializeSaveGame(CurrentSaveGame, SaveData);
        Result = WriteSaveToDisk(SlotIndex, SaveData);
    }

    return Result;
}

EMingSaveGameResult UMingSaveGameManager::QuickSave()
{
    return SaveGame(GetQuickSaveSlot(), TEXT("Quick Save"), false);
}

void UMingSaveGameManager::SaveGameAsync(int32 SlotIndex, const FString& SaveName)
{
    // In a real implementation, this would use threading or async task
    // For now, just call the synchronous version
    SaveGame(SlotIndex, SaveName, false);
}

EMingSaveGameResult UMingSaveGameManager::LoadGame(int32 SlotIndex)
{
    if (!bIsInitialized)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    // Read from disk
    TArray<uint8> SaveData;
    EMingSaveGameResult Result = ReadSaveFromDisk(SlotIndex, SaveData);

    if (Result != EMingSaveGameResult::Success)
    {
        OnLoadGameComplete.Broadcast(Result, SlotIndex);
        return Result;
    }

    // Deserialize
    UMingSaveGame* LoadedSave = NewObject<UMingSaveGame>(this);
    if (!DeserializeSaveGame(SaveData, LoadedSave))
    {
        return EMingSaveGameResult::Failed_CorruptData;
    }

    // Check version compatibility
    if (!LoadedSave->IsCompatibleVersion())
    {
        // Try to upgrade
        LoadedSave->UpgradeToCurrentVersion();

        if (!LoadedSave->IsCompatibleVersion())
        {
            return EMingSaveGameResult::Failed_VersionMismatch;
        }
    }

    // Decrypt if needed
    if (bEncryptionEnabled && !EncryptionKey.IsEmpty())
    {
        LoadedSave->DecryptData(EncryptionKey);
    }

    // Decompress if needed
    LoadedSave->DecompressData();

    // Restore all data
    RestoreRelationshipData(LoadedSave);
    RestoreReputationData(LoadedSave);
    RestoreQuestData(LoadedSave);
    RestoreAudioSettings(LoadedSave);
    RestoreUISettings(LoadedSave);
    RestoreGameState(LoadedSave);

    CurrentSaveSlot = SlotIndex;
    CurrentSaveGame = LoadedSave;

    LogSaveEvent(TEXT("Load successful"), SlotIndex);
    OnLoadGameComplete.Broadcast(EMingSaveGameResult::Success, SlotIndex);

    return EMingSaveGameResult::Success;
}

void UMingSaveGameManager::LoadGameAsync(int32 SlotIndex)
{
    // Async version would use threading
    LoadGame(SlotIndex);
}

bool UMingSaveGameManager::LoadMostRecentSave()
{
    int32 MostRecentSlot = GetMostRecentSaveSlot();
    if (MostRecentSlot >= 0)
    {
        return LoadGame(MostRecentSlot) == EMingSaveGameResult::Success;
    }
    return false;
}

bool UMingSaveGameManager::DeleteSaveGame(int32 SlotIndex)
{
    FString SavePath = GetSaveGamePath(SlotIndex);
    FString ThumbnailPath = GetThumbnailPath(SlotIndex);

    bool bSaveDeleted = FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SavePath);
    bool bThumbDeleted = FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ThumbnailPath);

    bool bSuccess = bSaveDeleted;

    OnDeleteGameComplete.Broadcast(bSuccess);
    LogSaveEvent(TEXT("Delete save"), SlotIndex);

    return bSuccess;
}

bool UMingSaveGameManager::DeleteAllSaves()
{
    TArray<FMingSaveGameSlotInfo> Slots = GetAllSaveSlots();
    bool bAllDeleted = true;

    for (const FMingSaveGameSlotInfo& Slot : Slots)
    {
        if (!Slot.bIsEmpty)
        {
            if (!DeleteSaveGame(Slot.SlotIndex))
            {
                bAllDeleted = false;
            }
        }
    }

    return bAllDeleted;
}

TArray<FMingSaveGameSlotInfo> UMingSaveGameManager::GetAllSaveSlots() const
{
    TArray<FMingSaveGameSlotInfo> Slots;

    for (int32 i = 0; i < UMingSaveGame::GetMaxSaveSlots(); i++)
    {
        FMingSaveGameSlotInfo Info = GetSaveSlotInfo(i);
        Slots.Add(Info);
    }

    return Slots;
}

FMingSaveGameSlotInfo UMingSaveGameManager::GetSaveSlotInfo(int32 SlotIndex) const
{
    FMingSaveGameSlotInfo Info;
    Info.SlotIndex = SlotIndex;

    FString SavePath = GetSaveGamePath(SlotIndex);
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SavePath))
    {
        Info.bIsEmpty = true;
        return Info;
    }

    // Load header information
    TArray<uint8> SaveData;
    if (FFileHelper::LoadFileToArray(SaveData, *SavePath))
    {
        // Try to extract header info from save data
        // This would deserialize just the header
        Info.bIsEmpty = false;

        // Load thumbnail
        Info.ThumbnailImage = LoadSaveThumbnail(SlotIndex);
    }

    return Info;
}

bool UMingSaveGameManager::DoesSaveExist(int32 SlotIndex) const
{
    FString SavePath = GetSaveGamePath(SlotIndex);
    return FPlatformFileManager::Get().GetPlatformFile().FileExists(*SavePath);
}

int32 UMingSaveGameManager::GetNextAvailableSlot() const
{
    for (int32 i = 2; i < UMingSaveGame::GetMaxSaveSlots(); i++) // Start from 2 (skip quick and auto)
    {
        if (!DoesSaveExist(i))
        {
            return i;
        }
    }
    return -1;
}

int32 UMingSaveGameManager::GetMostRecentSaveSlot() const
{
    FDateTime MostRecentTime = FDateTime::MinValue();
    int32 MostRecentSlot = -1;

    for (int32 i = 0; i < UMingSaveGame::GetMaxSaveSlots(); i++)
    {
        FMingSaveGameSlotInfo Info = GetSaveSlotInfo(i);
        if (!Info.bIsEmpty && Info.SaveDateTime > MostRecentTime)
        {
            MostRecentTime = Info.SaveDateTime;
            MostRecentSlot = i;
        }
    }

    return MostRecentSlot;
}

void UMingSaveGameManager::SetCurrentSaveSlot(int32 SlotIndex)
{
    CurrentSaveSlot = SlotIndex;
}

void UMingSaveGameManager::SetAutosaveEnabled(bool bEnabled)
{
    bAutosaveEnabled = bEnabled;
    AutosaveTimer = 0.0f;
}

void UMingSaveGameManager::SetAutosaveInterval(float IntervalMinutes)
{
    AutosaveIntervalMinutes = FMath::Clamp(IntervalMinutes, 1.0f, 60.0f);
}

void UMingSaveGameManager::TriggerAutosave()
{
    if (!bAutosaveEnabled)
    {
        return;
    }

    int32 AutoSlot = GetAutoSaveSlot();
    EMingSaveGameResult Result = SaveGame(AutoSlot, TEXT("Auto Save"), true);

    if (Result == EMingSaveGameResult::Success)
    {
        OnAutosaveTriggered.Broadcast(AutoSlot);
    }

    AutosaveTimer = 0.0f;
}

void UMingSaveGameManager::CheckAutosaveTimer(float DeltaTime)
{
    if (!bAutosaveEnabled)
    {
        return;
    }

    UpdateAutosaveTimer(DeltaTime);
}

void UMingSaveGameManager::CollectRelationshipData(UMingSaveGame* SaveGame)
{
    if (!PersonalManager || !SaveGame)
    {
        return;
    }

    // Serialize relationship data
    // This would use the relationship manager's serialization
    // Placeholder implementation
    TArray<uint8> Data;
    // Data = PersonalManager->GetRelationshipManager()->SerializeData();
    SaveGame->SetRelationshipData(Data);
}

void UMingSaveGameManager::CollectReputationData(UMingSaveGame* SaveGame)
{
    if (!PersonalManager || !SaveGame)
    {
        return;
    }

    TArray<uint8> Data;
    // Data = PersonalManager->GetReputationManager()->SerializeData();
    SaveGame->SetReputationData(Data);
}

void UMingSaveGameManager::CollectQuestData(UMingSaveGame* SaveGame)
{
    // Collect quest progress data
    TArray<uint8> Data;
    SaveGame->SetQuestData(Data);
}

void UMingSaveGameManager::CollectAudioSettings(UMingSaveGame* SaveGame)
{
    if (!SaveGame)
    {
        return;
    }

    // Get current audio settings
    if (AudioSystem)
    {
        SaveGame->SetAudioSetting(TEXT("MasterVolume"), 1.0f); // Get from audio system
        SaveGame->SetAudioSetting(TEXT("MusicVolume"), 0.8f);
        SaveGame->SetAudioSetting(TEXT("SFXVolume"), 1.0f);
        SaveGame->SetAudioSetting(TEXT("DialogueVolume"), 1.0f);
    }
}

void UMingSaveGameManager::CollectUISettings(UMingSaveGame* SaveGame)
{
    if (!SaveGame)
    {
        return;
    }

    // Save UI preferences
    SaveGame->SetUISetting(TEXT("ShowQuickAccess"), TEXT("true"));
    SaveGame->SetUISetting(TEXT("NotificationDuration"), TEXT("5.0"));
    SaveGame->SetUISetting(TEXT("ShowTutorialTips"), TEXT("true"));
}

void UMingSaveGameManager::CollectGameState(UMingSaveGame* SaveGame)
{
    if (!SaveGame)
    {
        return;
    }

    // Save current game state
    if (UWorld* World = GetWorld())
    {
        FString CurrentLevelName = World->GetMapName();
        SaveGame->Header.CurrentLevel = CurrentLevelName;
    }

    // Save player position, inventory, etc.
}

void UMingSaveGameManager::RestoreRelationshipData(UMingSaveGame* SaveGame)
{
    if (!PersonalManager || !SaveGame)
    {
        return;
    }

    TArray<uint8> Data = SaveGame->GetRelationshipData();
    // PersonalManager->GetRelationshipManager()->DeserializeData(Data);
}

void UMingSaveGameManager::RestoreReputationData(UMingSaveGame* SaveGame)
{
    if (!PersonalManager || !SaveGame)
    {
        return;
    }

    TArray<uint8> Data = SaveGame->GetReputationData();
    // PersonalManager->GetReputationManager()->DeserializeData(Data);
}

void UMingSaveGameManager::RestoreQuestData(UMingSaveGame* SaveGame)
{
    // Restore quest progress
    TArray<uint8> Data = SaveGame->GetQuestData();
}

void UMingSaveGameManager::RestoreAudioSettings(UMingSaveGame* SaveGame)
{
    if (!AudioSystem || !SaveGame)
    {
        return;
    }

    float MasterVolume = SaveGame->GetAudioSetting(TEXT("MasterVolume"), 1.0f);
    float MusicVolume = SaveGame->GetAudioSetting(TEXT("MusicVolume"), 0.8f);
    float SFXVolume = SaveGame->GetAudioSetting(TEXT("SFXVolume"), 1.0f);

    // Apply to audio system
    // AudioSystem->SetMasterVolume(MasterVolume);
    // AudioSystem->SetMusicVolume(MusicVolume);
    // AudioSystem->SetSFXVolume(SFXVolume);
}

void UMingSaveGameManager::RestoreUISettings(UMingSaveGame* SaveGame)
{
    if (!SaveGame)
    {
        return;
    }

    FString ShowQuickAccess = SaveGame->GetUISetting(TEXT("ShowQuickAccess"), TEXT("true"));
    FString NotificationDuration = SaveGame->GetUISetting(TEXT("NotificationDuration"), TEXT("5.0"));

    // Apply UI settings
}

void UMingSaveGameManager::RestoreGameState(UMingSaveGame* SaveGame)
{
    if (!SaveGame)
    {
        return;
    }

    // Restore game state
    // Load level if different
    // Restore player position
    // Restore inventory
}

void UMingSaveGameManager::CaptureSaveThumbnail(int32 SlotIndex)
{
    // Capture screenshot for save thumbnail
    // This would use UE's screenshot functionality
}

UTexture2D* UMingSaveGameManager::LoadSaveThumbnail(int32 SlotIndex) const
{
    FString ThumbnailPath = GetThumbnailPath(SlotIndex);

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ThumbnailPath))
    {
        return nullptr;
    }

    // Load texture from file
    UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(ThumbnailPath);
    return Texture;
}

bool UMingSaveGameManager::ExportSaveToFile(int32 SlotIndex, const FString& FilePath)
{
    FString SourcePath = GetSaveGamePath(SlotIndex);
    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*FilePath, *SourcePath);
}

bool UMingSaveGameManager::ImportSaveFromFile(const FString& FilePath, int32 TargetSlotIndex)
{
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        return false;
    }

    FString TargetPath = GetSaveGamePath(TargetSlotIndex);
    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*TargetPath, *FilePath);
}

bool UMingSaveGameManager::ValidateSaveSlot(int32 SlotIndex) const
{
    if (SlotIndex < 0 || SlotIndex >= UMingSaveGame::GetMaxSaveSlots())
    {
        return false;
    }

    if (!DoesSaveExist(SlotIndex))
    {
        return false;
    }

    // Additional validation
    FMingSaveGameSlotInfo Info = GetSaveSlotInfo(SlotIndex);
    return !Info.bIsEmpty;
}

FString UMingSaveGameManager::GetSaveValidationError() const
{
    return LastError;
}

void UMingSaveGameManager::SetCompressionEnabled(bool bEnabled)
{
    bCompressionEnabled = bEnabled;
}

void UMingSaveGameManager::SetEncryptionEnabled(bool bEnabled, const FString& Key)
{
    bEncryptionEnabled = bEnabled;
    EncryptionKey = Key;
}

bool UMingSaveGameManager::CreateBackup(int32 SlotIndex)
{
    FString SourcePath = GetSaveGamePath(SlotIndex);
    FString BackupPath = GetBackupPath(SlotIndex);

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SourcePath))
    {
        return false;
    }

    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*BackupPath, *SourcePath);
}

bool UMingSaveGameManager::RestoreFromBackup(int32 SlotIndex)
{
    FString SourcePath = GetBackupPath(SlotIndex);
    FString TargetPath = GetSaveGamePath(SlotIndex);

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SourcePath))
    {
        return false;
    }

    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*TargetPath, *SourcePath);
}

FString UMingSaveGameManager::GetSaveGamePath(int32 SlotIndex) const
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames") / FString::Printf(TEXT("MingSave_%d.sav"), SlotIndex);
}

FString UMingSaveGameManager::GetThumbnailPath(int32 SlotIndex) const
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames") / FString::Printf(TEXT("MingSave_%d_thumb.png"), SlotIndex);
}

FString UMingSaveGameManager::GetBackupPath(int32 SlotIndex) const
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames") / FString::Printf(TEXT("MingSave_%d_backup.sav"), SlotIndex);
}

void UMingSaveGameManager::SerializeSaveGame(UMingSaveGame* SaveGame, TArray<uint8>& OutData)
{
    if (!SaveGame)
    {
        return;
    }

    FMemoryWriter MemoryWriter(OutData, true);
    FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
    SaveGame->Serialize(Ar);
}

bool UMingSaveGameManager::DeserializeSaveGame(const TArray<uint8>& Data, UMingSaveGame* OutSaveGame)
{
    if (!OutSaveGame || Data.Num() == 0)
    {
        return false;
    }

    FMemoryReader MemoryReader(Data, true);
    FObjectAndNameAsStringProxyArchive Ar(MemoryReader, false);
    OutSaveGame->Serialize(Ar);

    return true;
}

EMingSaveGameResult UMingSaveGameManager::WriteSaveToDisk(int32 SlotIndex, const TArray<uint8>& Data)
{
    FString SavePath = GetSaveGamePath(SlotIndex);
    FString SaveDir = FPaths::GetPath(SavePath);

    // Ensure directory exists
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SaveDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDir);
    }

    // Check available space
    int64 AvailableSpace = GetAvailableDiskSpace();
    if (AvailableSpace < Data.Num())
    {
        return EMingSaveGameResult::Failed_NoSpace;
    }

    // Write file
    if (FFileHelper::SaveArrayToFile(Data, *SavePath))
    {
        return EMingSaveGameResult::Success;
    }

    return EMingSaveGameResult::Failed_Unknown;
}

EMingSaveGameResult UMingSaveGameManager::ReadSaveFromDisk(int32 SlotIndex, TArray<uint8>& OutData)
{
    FString SavePath = GetSaveGamePath(SlotIndex);

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SavePath))
    {
        return EMingSaveGameResult::Failed_CorruptData;
    }

    if (FFileHelper::LoadFileToArray(OutData, *SavePath))
    {
        return EMingSaveGameResult::Success;
    }

    return EMingSaveGameResult::Failed_CorruptData;
}

void UMingSaveGameManager::UpdateAutosaveTimer(float DeltaTime)
{
    AutosaveTimer += DeltaTime;

    float IntervalSeconds = AutosaveIntervalMinutes * 60.0f;
    if (AutosaveTimer >= IntervalSeconds)
    {
        TriggerAutosave();
    }
}

void UMingSaveGameManager::OnAsyncSaveComplete(bool bSuccess)
{
    // Handle async save completion
}

void UMingSaveGameManager::OnAsyncLoadComplete(bool bSuccess)
{
    // Handle async load completion
}

void UMingSaveGameManager::LogSaveEvent(const FString& Event, int32 SlotIndex)
{
    if (bDebugLogging)
    {
        FString SlotStr = SlotIndex >= 0 ? FString::Printf(TEXT(" [Slot %d]"), SlotIndex) : TEXT("");
        UE_LOG(LogTemp, Log, TEXT("SaveGameManager: %s%s"), *Event, *SlotStr);
    }
}

void UMingSaveGameManager::EnableDebugLogging(bool bEnable)
{
    bDebugLogging = bEnable;
}

FString UMingSaveGameManager::GetDebugInfo() const
{
    FString DebugInfo;
    DebugInfo += FString::Printf(TEXT("Save Game Manager - %s\n"), bIsInitialized ? TEXT("Initialized") : TEXT("Not Initialized"));
    DebugInfo += FString::Printf(TEXT("Current Slot: %d\n"), CurrentSaveSlot);
    DebugInfo += FString::Printf(TEXT("Autosave: %s (%.1f min)\n"), bAutosaveEnabled ? TEXT("Enabled") : TEXT("Disabled"), AutosaveIntervalMinutes);
    DebugInfo += FString::Printf(TEXT("Compression: %s\n"), bCompressionEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    DebugInfo += FString::Printf(TEXT("Encryption: %s\n"), bEncryptionEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    DebugInfo += FString::Printf(TEXT("Save Directory: %s\n"), *GetSaveDirectoryPath());
    DebugInfo += FString::Printf(TEXT("Available Space: %lld MB\n"), GetAvailableDiskSpace() / (1024 * 1024));

    return DebugInfo;
}

UMingSaveGameManager* UMingSaveGameManager::GetSaveGameManager(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // In a real implementation, this would get the manager from game instance
    // For now, return nullptr
    return nullptr;
}

FString UMingSaveGameManager::GetSaveDirectoryPath()
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames");
}

int64 UMingSaveGameManager::GetAvailableDiskSpace()
{
    // Get available disk space in bytes
    // This is platform-specific
    return 1024 * 1024 * 1024; // Return 1GB as placeholder
}

int64 UMingSaveGameManager::GetSaveGameTotalSize()
{
    int64 TotalSize = 0;

    for (int32 i = 0; i < UMingSaveGame::GetMaxSaveSlots(); i++)
    {
        FString SavePath = FPaths::ProjectSavedDir() / TEXT("SaveGames") / FString::Printf(TEXT("MingSave_%d.sav"), i);
        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SavePath))
        {
            TotalSize += FPlatformFileManager::Get().GetPlatformFile().FileSize(*SavePath);
        }
    }

    return TotalSize;
}
