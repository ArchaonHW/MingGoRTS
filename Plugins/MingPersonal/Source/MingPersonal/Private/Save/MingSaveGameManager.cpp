#include "Save/MingSaveGameManager.h"
#include "Save/MingSaveGame.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "MingPersonalManager.h"

UMingSaveGameManager::UMingSaveGameManager()
    : bIsInitialized(false)
    , CurrentSaveSlot(0)
    , CurrentSaveGame(nullptr)
    , bAutosaveEnabled(true)
    , AutosaveIntervalMinutes(10.0f)
    , AutosaveTimer(0.0f)
    , bCompressionEnabled(true)
    , bEncryptionEnabled(false)
    , bDebugLogging(false)
{
}

void UMingSaveGameManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 確保保存目錄存在
    FString SaveDir = GetSaveDirectoryPath();
    if (!FPaths::DirectoryExists(SaveDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDir);
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Save Game Manager initialized"));
}

EMingSaveGameResult UMingSaveGameManager::SaveGame(int32 SlotIndex, const FString& SaveName, bool bIsAutosave)
{
    if (!bIsInitialized)
    {
        Initialize();
    }

    // 檢查磁盤空間
    if (GetAvailableDiskSpace() < 1024 * 1024) // 至少需要 1MB
    {
        LastError = TEXT("Not enough disk space");
        OnSaveGameComplete.Broadcast(EMingSaveGameResult::Failed_NoSpace, SlotIndex);
        return EMingSaveGameResult::Failed_NoSpace;
    }

    // 創建保存對象
    UMingSaveGame* SaveGame = UMingSaveGame::CreateSaveGame(this, SaveName, SlotIndex);
    if (!SaveGame)
    {
        LastError = TEXT("Failed to create save game object");
        OnSaveGameComplete.Broadcast(EMingSaveGameResult::Failed_Unknown, SlotIndex);
        return EMingSaveGameResult::Failed_Unknown;
    }

    // 收集數據
    CollectRelationshipData(SaveGame);
    CollectReputationData(SaveGame);
    CollectQuestData(SaveGame);
    CollectAudioSettings(SaveGame);
    CollectUISettings(SaveGame);
    CollectGameState(SaveGame);

    // 設置元數據
    SaveGame->SetMetadata(TEXT("PlayerName"), SaveGame->Header.PlayerName);
    SaveGame->SetMetadata(TEXT("GameVersion"), TEXT("1.0.0"));
    SaveGame->SetMetadata(TEXT("Platform"), FPlatformProperties::IniPlatformName());

    // 壓縮數據
    if (bCompressionEnabled)
    {
        SaveGame->CompressData();
    }

    // 加密數據
    if (bEncryptionEnabled && !EncryptionKey.IsEmpty())
    {
        SaveGame->EncryptData(EncryptionKey);
    }

    // 序列化數據
    TArray<uint8> SaveData;
    SerializeSaveGame(SaveGame, SaveData);

    // 寫入磁盤
    EMingSaveGameResult Result = WriteSaveToDisk(SlotIndex, SaveData);

    if (Result == EMingSaveGameResult::Success)
    {
        CurrentSaveGame = SaveGame;
        CurrentSaveSlot = SlotIndex;
        LogSaveEvent(TEXT("Saved"), SlotIndex);
    }

    OnSaveGameComplete.Broadcast(Result, SlotIndex);
    return Result;
}

EMingSaveGameResult UMingSaveGameManager::SaveGameWithMetadata(int32 SlotIndex, const FString& SaveName, const TMap<FString, FString>& Metadata)
{
    if (!bIsInitialized)
    {
        Initialize();
    }

    // 創建保存對象
    UMingSaveGame* SaveGame = UMingSaveGame::CreateSaveGame(this, SaveName, SlotIndex);
    if (!SaveGame)
    {
        LastError = TEXT("Failed to create save game object");
        return EMingSaveGameResult::Failed_Unknown;
    }

    // 添加元數據
    for (const auto& Pair : Metadata)
    {
        SaveGame->SetMetadata(Pair.Key, Pair.Value);
    }

    // 收集數據
    CollectRelationshipData(SaveGame);
    CollectReputationData(SaveGame);
    CollectQuestData(SaveGame);
    CollectAudioSettings(SaveGame);
    CollectUISettings(SaveGame);
    CollectGameState(SaveGame);

    // 壓縮和加密
    if (bCompressionEnabled)
    {
        SaveGame->CompressData();
    }

    if (bEncryptionEnabled && !EncryptionKey.IsEmpty())
    {
        SaveGame->EncryptData(EncryptionKey);
    }

    // 序列化並保存
    TArray<uint8> SaveData;
    SerializeSaveGame(SaveGame, SaveData);

    EMingSaveGameResult Result = WriteSaveToDisk(SlotIndex, SaveData);

    if (Result == EMingSaveGameResult::Success)
    {
        CurrentSaveGame = SaveGame;
        CurrentSaveSlot = SlotIndex;
    }

    OnSaveGameComplete.Broadcast(Result, SlotIndex);
    return Result;
}

EMingSaveGameResult UMingSaveGameManager::QuickSave()
{
    return SaveGame(GetQuickSaveSlot(), TEXT("Quick Save"), false);
}

void UMingSaveGameManager::SaveGameAsync(int32 SlotIndex, const FString& SaveName)
{
    // 異步保存實現
    // 在實際實現中，這會使用多線程或任務系統
    // 這裡簡化為同步調用
    SaveGame(SlotIndex, SaveName, false);
}

EMingSaveGameResult UMingSaveGameManager::LoadGame(int32 SlotIndex)
{
    if (!bIsInitialized)
    {
        Initialize();
    }

    // 讀取數據
    TArray<uint8> SaveData;
    EMingSaveGameResult Result = ReadSaveFromDisk(SlotIndex, SaveData);

    if (Result != EMingSaveGameResult::Success)
    {
        OnLoadGameComplete.Broadcast(Result, SlotIndex);
        return Result;
    }

    // 反序列化
    UMingSaveGame* SaveGame = NewObject<UMingSaveGame>(this);
    if (!DeserializeSaveGame(SaveData, SaveGame))
    {
        LastError = TEXT("Failed to deserialize save game");
        OnLoadGameComplete.Broadcast(EMingSaveGameResult::Failed_CorruptData, SlotIndex);
        return EMingSaveGameResult::Failed_CorruptData;
    }

    // 版本檢查
    if (!SaveGame->IsCompatibleVersion())
    {
        LastError = FString::Printf(TEXT("Incompatible save version: %d"), static_cast<int32>(SaveGame->Header.Version));
        OnLoadGameComplete.Broadcast(EMingSaveGameResult::Failed_VersionMismatch, SlotIndex);
        return EMingSaveGameResult::Failed_VersionMismatch;
    }

    // 解密數據
    if (bEncryptionEnabled && !EncryptionKey.IsEmpty())
    {
        SaveGame->DecryptData(EncryptionKey);
    }

    // 解壓數據
    if (bCompressionEnabled)
    {
        SaveGame->DecompressData();
    }

    // 恢復數據
    RestoreRelationshipData(SaveGame);
    RestoreReputationData(SaveGame);
    RestoreQuestData(SaveGame);
    RestoreAudioSettings(SaveGame);
    RestoreUISettings(SaveGame);
    RestoreGameState(SaveGame);

    CurrentSaveGame = SaveGame;
    CurrentSaveSlot = SlotIndex;

    LogSaveEvent(TEXT("Loaded"), SlotIndex);
    OnLoadGameComplete.Broadcast(EMingSaveGameResult::Success, SlotIndex);

    return EMingSaveGameResult::Success;
}

void UMingSaveGameManager::LoadGameAsync(int32 SlotIndex)
{
    // 異步加載實現
    // 實際應用中會使用多線程
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

    if (FPaths::FileExists(SavePath))
    {
        bool bSuccess = FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SavePath);
        OnDeleteGameComplete.Broadcast(bSuccess);
        return bSuccess;
    }

    OnDeleteGameComplete.Broadcast(false);
    return false;
}

bool UMingSaveGameManager::DeleteAllSaves()
{
    bool bAllDeleted = true;

    for (int32 SlotIndex = 0; SlotIndex < UMingSaveGame::GetMaxSaveSlots(); ++SlotIndex)
    {
        if (DoesSaveExist(SlotIndex))
        {
            if (!DeleteSaveGame(SlotIndex))
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

    for (int32 SlotIndex = 0; SlotIndex < UMingSaveGame::GetMaxSaveSlots(); ++SlotIndex)
    {
        FMingSaveGameSlotInfo SlotInfo = GetSaveSlotInfo(SlotIndex);
        if (!SlotInfo.bIsEmpty)
        {
            Slots.Add(SlotInfo);
        }
    }

    // 按時間排序（最新的在前）
    Slots.Sort([](const FMingSaveGameSlotInfo& A, const FMingSaveGameSlotInfo& B)
    {
        return A.SaveDateTime > B.SaveDateTime;
    });

    return Slots;
}

FMingSaveGameSlotInfo UMingSaveGameManager::GetSaveSlotInfo(int32 SlotIndex) const
{
    FMingSaveGameSlotInfo Info;
    Info.SlotIndex = SlotIndex;
    Info.bIsEmpty = true;

    FString SavePath = GetSaveGamePath(SlotIndex);
    if (!FPaths::FileExists(SavePath))
    {
        return Info;
    }

    // 讀取保存文件
    TArray<uint8> SaveData;
    if (!FFileHelper::LoadFileToArray(SaveData, *SavePath))
    {
        return Info;
    }

    // 嘗試反序列化頭部信息
    // 這裡簡化處理，實際應用會有更複雜的邏輯
    FBufferArchive Ar;
    Ar.Append(SaveData.GetData(), SaveData.Num());

    // 從文件信息填充
    FDateTime FileTime = FPlatformFileManager::Get().GetPlatformFile().GetTimeStamp(*SavePath);
    Info.SaveDateTime = FileTime;
    Info.bIsEmpty = false;
    Info.SaveName = FString::Printf(TEXT("Save Slot %d"), SlotIndex);

    return Info;
}

bool UMingSaveGameManager::DoesSaveExist(int32 SlotIndex) const
{
    FString SavePath = GetSaveGamePath(SlotIndex);
    return FPaths::FileExists(SavePath);
}

int32 UMingSaveGameManager::GetNextAvailableSlot() const
{
    for (int32 SlotIndex = 2; SlotIndex < UMingSaveGame::GetMaxSaveSlots(); ++SlotIndex)
    {
        if (!DoesSaveExist(SlotIndex))
        {
            return SlotIndex;
        }
    }
    return -1; // 沒有空槽
}

int32 UMingSaveGameManager::GetMostRecentSaveSlot() const
{
    FDateTime MostRecentTime;
    int32 MostRecentSlot = -1;

    for (int32 SlotIndex = 0; SlotIndex < UMingSaveGame::GetMaxSaveSlots(); ++SlotIndex)
    {
        if (DoesSaveExist(SlotIndex))
        {
            FString SavePath = GetSaveGamePath(SlotIndex);
            FDateTime FileTime = FPlatformFileManager::Get().GetPlatformFile().GetTimeStamp(*SavePath);

            if (MostRecentSlot == -1 || FileTime > MostRecentTime)
            {
                MostRecentTime = FileTime;
                MostRecentSlot = SlotIndex;
            }
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
}

void UMingSaveGameManager::SetAutosaveInterval(float IntervalMinutes)
{
    AutosaveIntervalMinutes = FMath::Max(1.0f, IntervalMinutes);
}

void UMingSaveGameManager::TriggerAutosave()
{
    if (bAutosaveEnabled)
    {
        SaveGame(GetAutoSaveSlot(), TEXT("Auto Save"), true);
        OnAutosaveTriggered.Broadcast(GetAutoSaveSlot());
    }
}

void UMingSaveGameManager::CheckAutosaveTimer(float DeltaTime)
{
    if (!bAutosaveEnabled)
    {
        return;
    }

    AutosaveTimer += DeltaTime;

    float IntervalSeconds = AutosaveIntervalMinutes * 60.0f;
    if (AutosaveTimer >= IntervalSeconds)
    {
        TriggerAutosave();
        AutosaveTimer = 0.0f;
    }
}

void UMingSaveGameManager::CollectRelationshipData(UMingSaveGame* SaveGame)
{
    // 從關係管理器收集數據
    if (PersonalManager)
    {
        // 實際實現會序列化關係數據
        // SaveGame->SetRelationshipData(...);
    }
}

void UMingSaveGameManager::CollectReputationData(UMingSaveGame* SaveGame)
{
    // 從聲望系統收集數據
    if (PersonalManager)
    {
        // SaveGame->SetReputationData(...);
    }
}

void UMingSaveGameManager::CollectQuestData(UMingSaveGame* SaveGame)
{
    // 收集任務數據
}

void UMingSaveGameManager::CollectAudioSettings(UMingSaveGame* SaveGame)
{
    // 收集音頻設置
    SaveGame->SetAudioSetting(TEXT("MasterVolume"), 1.0f);
    SaveGame->SetAudioSetting(TEXT("MusicVolume"), 0.8f);
    SaveGame->SetAudioSetting(TEXT("SFXVolume"), 1.0f);
}

void UMingSaveGameManager::CollectUISettings(UMingSaveGame* SaveGame)
{
    // 收集UI設置
    SaveGame->SetUISetting(TEXT("Theme"), TEXT("Default"));
    SaveGame->SetUISetting(TEXT("Language"), TEXT("zh-CN"));
}

void UMingSaveGameManager::CollectGameState(UMingSaveGame* SaveGame)
{
    // 收集遊戲狀態
    SaveGame->SetGameStateData(TEXT("CurrentLevel"), TEXT("Tutorial"));
    SaveGame->SetGameStateData(TEXT("PlayerLevel"), TEXT("1"));
    SaveGame->SetGameStateData(TEXT("Experience"), TEXT("0"));
}

void UMingSaveGameManager::RestoreRelationshipData(UMingSaveGame* SaveGame)
{
    // 恢復關係數據
}

void UMingSaveGameManager::RestoreReputationData(UMingSaveGame* SaveGame)
{
    // 恢復聲望數據
}

void UMingSaveGameManager::RestoreQuestData(UMingSaveGame* SaveGame)
{
    // 恢復任務數據
}

void UMingSaveGameManager::RestoreAudioSettings(UMingSaveGame* SaveGame)
{
    // 恢復音頻設置
}

void UMingSaveGameManager::RestoreUISettings(UMingSaveGame* SaveGame)
{
    // 恢復UI設置
}

void UMingSaveGameManager::RestoreGameState(UMingSaveGame* SaveGame)
{
    // 恢復遊戲狀態
}

void UMingSaveGameManager::CaptureSaveThumbnail(int32 SlotIndex)
{
    // 捕獲屏幕截圖作為縮略圖
    UE_LOG(LogTemp, Log, TEXT("Capturing thumbnail for slot %d"), SlotIndex);
}

UTexture2D* UMingSaveGameManager::LoadSaveThumbnail(int32 SlotIndex) const
{
    // 加載縮略圖
    return nullptr;
}

bool UMingSaveGameManager::ExportSaveToFile(int32 SlotIndex, const FString& FilePath)
{
    FString SavePath = GetSaveGamePath(SlotIndex);

    if (!FPaths::FileExists(SavePath))
    {
        return false;
    }

    // 複製文件到指定路徑
    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*FilePath, *SavePath);
}

bool UMingSaveGameManager::ImportSaveFromFile(const FString& FilePath, int32 TargetSlotIndex)
{
    if (!FPaths::FileExists(FilePath))
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

    FString SavePath = GetSaveGamePath(SlotIndex);
    return FPaths::FileExists(SavePath);
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
    FString SavePath = GetSaveGamePath(SlotIndex);
    FString BackupPath = GetBackupPath(SlotIndex);

    if (!FPaths::FileExists(SavePath))
    {
        return false;
    }

    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*BackupPath, *SavePath);
}

bool UMingSaveGameManager::RestoreFromBackup(int32 SlotIndex)
{
    FString SavePath = GetSaveGamePath(SlotIndex);
    FString BackupPath = GetBackupPath(SlotIndex);

    if (!FPaths::FileExists(BackupPath))
    {
        return false;
    }

    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*SavePath, *BackupPath);
}

void UMingSaveGameManager::EnableDebugLogging(bool bEnable)
{
    bDebugLogging = bEnable;
}

FString UMingSaveGameManager::GetDebugInfo() const
{
    FString Info;
    Info += FString::Printf(TEXT("Save Directory: %s\n"), *GetSaveDirectoryPath());
    Info += FString::Printf(TEXT("Total Save Size: %lld bytes\n"), GetSaveGameTotalSize());
    Info += FString::Printf(TEXT("Available Space: %lld bytes\n"), GetAvailableDiskSpace());
    Info += FString::Printf(TEXT("Max Save Slots: %d\n"), UMingSaveGame::GetMaxSaveSlots());
    Info += FString::Printf(TEXT("Current Slot: %d\n"), CurrentSaveSlot);
    Info += FString::Printf(TEXT("Autosave Enabled: %s\n"), bAutosaveEnabled ? TEXT("Yes") : TEXT("No"));
    Info += FString::Printf(TEXT("Compression Enabled: %s\n"), bCompressionEnabled ? TEXT("Yes") : TEXT("No"));
    Info += FString::Printf(TEXT("Encryption Enabled: %s\n"), bEncryptionEnabled ? TEXT("Yes") : TEXT("No"));
    return Info;
}

FString UMingSaveGameManager::GetSaveGamePath(int32 SlotIndex) const
{
    return GetSaveDirectoryPath() / FString::Printf(TEXT("SaveGame_%d.sav"), SlotIndex);
}

FString UMingSaveGameManager::GetThumbnailPath(int32 SlotIndex) const
{
    return GetSaveDirectoryPath() / FString::Printf(TEXT("Thumbnail_%d.png"), SlotIndex);
}

FString UMingSaveGameManager::GetBackupPath(int32 SlotIndex) const
{
    return GetSaveDirectoryPath() / FString::Printf(TEXT("Backup_%d.sav"), SlotIndex);
}

void UMingSaveGameManager::SerializeSaveGame(UMingSaveGame* SaveGame, TArray<uint8>& OutData)
{
    FBufferArchive Ar;
    SaveGame->Serialize(Ar);
    OutData = Ar;
}

bool UMingSaveGameManager::DeserializeSaveGame(const TArray<uint8>& Data, UMingSaveGame* OutSaveGame)
{
    FMemoryReader Ar(Data);
    OutSaveGame->Serialize(Ar);
    return true;
}

EMingSaveGameResult UMingSaveGameManager::WriteSaveToDisk(int32 SlotIndex, const TArray<uint8>& Data)
{
    FString SavePath = GetSaveGamePath(SlotIndex);

    if (FFileHelper::SaveArrayToFile(Data, *SavePath))
    {
        return EMingSaveGameResult::Success;
    }

    LastError = FString::Printf(TEXT("Failed to write save to: %s"), *SavePath);
    return EMingSaveGameResult::Failed_Unknown;
}

EMingSaveGameResult UMingSaveGameManager::ReadSaveFromDisk(int32 SlotIndex, TArray<uint8>& OutData)
{
    FString SavePath = GetSaveGamePath(SlotIndex);

    if (!FPaths::FileExists(SavePath))
    {
        LastError = FString::Printf(TEXT("Save file not found: %s"), *SavePath);
        return EMingSaveGameResult::Failed_Unknown;
    }

    if (FFileHelper::LoadFileToArray(OutData, *SavePath))
    {
        return EMingSaveGameResult::Success;
    }

    LastError = FString::Printf(TEXT("Failed to read save from: %s"), *SavePath);
    return EMingSaveGameResult::Failed_CorruptData;
}

void UMingSaveGameManager::UpdateAutosaveTimer(float DeltaTime)
{
    AutosaveTimer += DeltaTime;
}

void UMingSaveGameManager::OnAsyncSaveComplete(bool bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("Async save %s"), bSuccess ? TEXT("completed successfully") : TEXT("failed"));
}

void UMingSaveGameManager::OnAsyncLoadComplete(bool bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("Async load %s"), bSuccess ? TEXT("completed successfully") : TEXT("failed"));
}

void UMingSaveGameManager::LogSaveEvent(const FString& Event, int32 SlotIndex)
{
    if (bDebugLogging)
    {
        UE_LOG(LogTemp, Log, TEXT("[Save System] %s - Slot %d"), *Event, SlotIndex);
    }
}

UMingSaveGameManager* UMingSaveGameManager::GetSaveGameManager(UObject* WorldContextObject)
{
    if (WorldContextObject)
    {
        if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
        {
            // 從遊戲實例獲取或創建保存管理器
            // 實際實現會使用 GameInstance 的Subsystem
        }
    }

    return nullptr;
}

FString UMingSaveGameManager::GetSaveDirectoryPath()
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames");
}

int64 UMingSaveGameManager::GetAvailableDiskSpace()
{
    return FPlatformFileManager::Get().GetPlatformFile().GetFreeSpace(*GetSaveDirectoryPath());
}

int64 UMingSaveGameManager::GetSaveGameTotalSize()
{
    int64 TotalSize = 0;

    for (int32 SlotIndex = 0; SlotIndex < UMingSaveGame::GetMaxSaveSlots(); ++SlotIndex)
    {
        FString SavePath = GetSaveDirectoryPath() / FString::Printf(TEXT("SaveGame_%d.sav"), SlotIndex);
        if (FPaths::FileExists(SavePath))
        {
            TotalSize += FPlatformFileManager::Get().GetPlatformFile().FileSize(*SavePath);
        }
    }

    return TotalSize;
}
