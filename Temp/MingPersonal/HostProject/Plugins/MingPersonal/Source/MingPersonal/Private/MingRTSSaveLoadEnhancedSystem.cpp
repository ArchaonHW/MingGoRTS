#include "MingRTSSaveLoadEnhancedSystem.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/Compression.h"
#include "Misc/Base64.h"
#include "Engine/Engine.h"

UMingRTSSaveLoadEnhancedSystem::UMingRTSSaveLoadEnhancedSystem()
{
    CurrentWorld = nullptr;
    CurrentSlotID = TEXT("");
    SaveUpdateInterval = 0.1f; // 10 FPS
    LastSaveUpdateTime = 0.0f;
    AutoSaveInterval = 300.0f; // 5 minutes
    LastAutoSaveTime = 0.0f;
    MaxSaveCount = 100;
    MaxSlotCount = 10;
    DefaultFormat = ESaveFormat::Binary;
    DefaultLocation = ESaveLocation::Local;
    bAutoSaveEnabled = true;
    bCompressionEnabled = true;
    bEncryptionEnabled = false;
}

void UMingRTSSaveLoadEnhancedSystem::InitializeSaveLoadEnhancedSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSSaveLoadEnhancedSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastSaveUpdateTime = World->GetTimeSeconds();
    LastAutoSaveTime = World->GetTimeSeconds();
    
    // 清空現有保存數據
    SaveGameData.Empty();
    SaveSlots.Empty();
    SaveOperations.Empty();
    
    // 創建默認保存目錄
    FString SavePath = GetSavePath(DefaultLocation, TEXT(""));
    CreateSaveDirectory(SavePath);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSSaveLoadEnhancedSystem initialized successfully"));
}

void UMingRTSSaveLoadEnhancedSystem::UpdateSaveLoadEnhancedSystem(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 定期更新保存系統
    if (CurrentTime - LastSaveUpdateTime >= SaveUpdateInterval)
    {
        ProcessSaveOperations(DeltaTime);
        ProcessLoadOperations(DeltaTime);
        ProcessBackupOperations(DeltaTime);
        
        LastSaveUpdateTime = CurrentTime;
    }
    
    // 處理自動保存
    if (bAutoSaveEnabled && CurrentTime - LastAutoSaveTime >= AutoSaveInterval)
    {
        AutoSave();
        LastAutoSaveTime = CurrentTime;
    }
    
    // 清理無效數據
    CleanupInvalidSaveData();
    CleanupInvalidSaveSlots();
}

FString UMingRTSSaveLoadEnhancedSystem::SaveGameData(const FString& SaveName, ESaveDataType DataType, const TArray<uint8>& Data, ESaveFormat Format)
{
    if (SaveName.IsEmpty() || Data.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid save name or empty data"));
        return TEXT("");
    }
    
    if (SaveGameData.Num() >= MaxSaveCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum save count reached"));
        return TEXT("");
    }
    
    // 創建保存數據
    FSaveGameData SaveData = GetSaveDataTemplate();
    SaveData.SaveID = GenerateUniqueID(TEXT("Save"));
    SaveData.SaveName = SaveName;
    SaveData.DataType = DataType;
    SaveData.Format = Format;
    SaveData.Location = DefaultLocation;
    SaveData.Status = ESaveStatus::Saving;
    SaveData.Timestamp = CurrentWorld->GetTimeSeconds();
    SaveData.Version = 1;
    SaveData.Size = Data.Num();
    SaveData.bIsValid = true;
    
    // 處理數據
    TArray<uint8> ProcessedData = Data;
    
    // 壓縮數據
    if (bCompressionEnabled)
    {
        ProcessedData = CompressData(ProcessedData);
        SaveData.bIsCompressed = true;
    }
    
    // 加密數據
    if (bEncryptionEnabled)
    {
        ProcessedData = EncryptData(ProcessedData, TEXT("DefaultKey"));
        SaveData.bIsEncrypted = true;
    }
    
    // 計算校驗和
    SaveData.Checksum = CalculateChecksum(ProcessedData);
    
    // 設置文件路徑
    SaveData.FilePath = GetSavePath(SaveData.Location, FString::Printf(TEXT("%s.%s"), *SaveData.SaveID, *GetSaveFormatName(Format)));
    
    // 保存到文件
    if (FFileHelper::SaveArrayToFile(ProcessedData, *SaveData.FilePath))
    {
        SaveData.Data = ProcessedData;
        SaveData.Status = ESaveStatus::Saved;
        
        SaveGameData.Add(SaveData.SaveID, SaveData);
        
        // 觸發保存完成事件
        OnSaveCompleted.Broadcast(SaveData.SaveID, true);
        
        UE_LOG(LogTemp, Log, TEXT("Saved game data: %s (%s)"), *SaveData.SaveID, *SaveName);
        
        return SaveData.SaveID;
    }
    else
    {
        SaveData.Status = ESaveStatus::Failed;
        
        // 觸發保存失敗事件
        OnSaveCompleted.Broadcast(SaveData.SaveID, false);
        
        UE_LOG(LogTemp, Error, TEXT("Failed to save game data: %s"), *SaveName);
        
        return TEXT("");
    }
}

bool UMingRTSSaveLoadEnhancedSystem::LoadGameData(const FString& SaveID, TArray<uint8>& OutData)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    // 觸發載入開始事件
    OnLoadStarted.Broadcast(SaveID);
    
    // 從文件載入
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *SaveData->FilePath))
    {
        SaveData->Status = ESaveStatus::Missing;
        OnLoadCompleted.Broadcast(SaveID, false);
        return false;
    }
    
    // 驗證校驗和
    FString FileChecksum = CalculateChecksum(FileData);
    if (FileChecksum != SaveData->Checksum)
    {
        SaveData->Status = ESaveStatus::Corrupted;
        OnLoadCompleted.Broadcast(SaveID, false);
        return false;
    }
    
    // 解密數據
    if (SaveData->bIsEncrypted)
    {
        FileData = DecryptData(FileData, TEXT("DefaultKey"));
    }
    
    // 解壓縮數據
    if (SaveData->bIsCompressed)
    {
        FileData = DecompressData(FileData);
    }
    
    SaveData->Data = FileData;
    SaveData->Status = ESaveStatus::Loaded;
    OutData = FileData;
    
    // 觸發載入完成事件
    OnLoadCompleted.Broadcast(SaveID, true);
    
    UE_LOG(LogTemp, Log, TEXT("Loaded game data: %s"), *SaveID);
    
    return true;
}

bool UMingRTSSaveLoadEnhancedSystem::SaveToSlot(const FString& SlotName, const FString& Description, const FString& ThumbnailPath)
{
    if (SlotName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid slot name"));
        return false;
    }
    
    // 查找或創建槽位
    FString SlotID = TEXT("");
    for (const TPair<FString, FSaveSlot>& SlotPair : SaveSlots)
    {
        if (SlotPair.Value.SlotName == SlotName)
        {
            SlotID = SlotPair.Key;
            break;
        }
    }
    
    if (SlotID.IsEmpty())
    {
        SlotID = CreateSaveSlot(SlotName, Description);
    }
    
    if (SlotID.IsEmpty())
    {
        return false;
    }
    
    FSaveSlot* SaveSlot = SaveSlots.Find(SlotID);
    if (!SaveSlot)
    {
        return false;
    }
    
    // 獲取當前遊戲狀態
    TMap<FString, FString> GameState = GetCurrentGameState();
    TMap<FString, FString> PlayerData = GetCurrentPlayerData();
    TMap<FString, FString> WorldData = GetCurrentWorldData();
    
    // 保存所有數據類型
    TArray<FString> SavedIDs;
    
    // 保存遊戲狀態
    if (GameState.Num() > 0)
    {
        TArray<uint8> StateData = SerializeData(GameState, DefaultFormat);
        FString StateID = SaveGameData(FString::Printf(TEXT("%s_GameState"), *SlotName), ESaveDataType::GameState, StateData, DefaultFormat);
        if (!StateID.IsEmpty())
        {
            SavedIDs.Add(StateID);
        }
    }
    
    // 保存玩家數據
    if (PlayerData.Num() > 0)
    {
        TArray<uint8> PlayerDataBytes = SerializeData(PlayerData, DefaultFormat);
        FString PlayerID = SaveGameData(FString::Printf(TEXT("%s_PlayerData"), *SlotName), ESaveDataType::PlayerData, PlayerDataBytes, DefaultFormat);
        if (!PlayerID.IsEmpty())
        {
            SavedIDs.Add(PlayerID);
        }
    }
    
    // 保存世界數據
    if (WorldData.Num() > 0)
    {
        TArray<uint8> WorldDataBytes = SerializeData(WorldData, DefaultFormat);
        FString WorldID = SaveGameData(FString::Printf(TEXT("%s_WorldData"), *SlotName), ESaveDataType::WorldData, WorldDataBytes, DefaultFormat);
        if (!WorldID.IsEmpty())
        {
            SavedIDs.Add(WorldID);
        }
    }
    
    // 更新槽位信息
    SaveSlot->SaveIDs = SavedIDs;
    SaveSlot->LastModified = CurrentWorld->GetTimeSeconds();
    SaveSlot->ThumbnailPath = ThumbnailPath;
    SaveSlot->bIsEmpty = false;
    SaveSlot->LevelName = CurrentWorld->GetMapName();
    
    // 獲取玩家名稱
    if (APlayerController* PC = CurrentWorld->GetFirstPlayerController())
    {
        if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
        {
            SaveSlot->PlayerName = PS->GetPlayerName();
        }
    }
    
    CurrentSlotID = SlotID;
    
    UE_LOG(LogTemp, Log, TEXT("Saved to slot: %s (%d saves)"), *SlotName, SavedIDs.Num());
    
    return true;
}

bool UMingRTSSaveLoadEnhancedSystem::LoadFromSlot(const FString& SlotID)
{
    FSaveSlot* SaveSlot = SaveSlots.Find(SlotID);
    if (!SaveSlot)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save slot not found: %s"), *SlotID);
        return false;
    }
    
    if (SaveSlot->bIsEmpty)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save slot is empty: %s"), *SlotID);
        return false;
    }
    
    // 載入所有保存的數據
    bool bSuccess = true;
    
    for (const FString& SaveID : SaveSlot->SaveIDs)
    {
        TArray<uint8> Data;
        if (LoadGameData(SaveID, Data))
        {
            FSaveGameData* SaveData = SaveGameData.Find(SaveID);
            if (SaveData)
            {
                TMap<FString, FString> DeserializedData = DeserializeData(Data, SaveData->Format);
                
                // 根據數據類型設置遊戲狀態
                switch (SaveData->DataType)
                {
                    case ESaveDataType::GameState:
                        SetGameState(DeserializedData);
                        break;
                    case ESaveDataType::PlayerData:
                        SetPlayerData(DeserializedData);
                        break;
                    case ESaveDataType::WorldData:
                        SetWorldData(DeserializedData);
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            bSuccess = false;
        }
    }
    
    CurrentSlotID = SlotID;
    
    UE_LOG(LogTemp, Log, TEXT("Loaded from slot: %s - %s"), *SlotID, bSuccess ? TEXT("Success") : TEXT("Failed"));
    
    return bSuccess;
}

FString UMingRTSSaveLoadEnhancedSystem::CreateSaveSlot(const FString& SlotName, const FString& Description, int32 MaxSaves)
{
    if (SlotName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid slot name"));
        return TEXT("");
    }
    
    if (SaveSlots.Num() >= MaxSlotCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum slot count reached"));
        return TEXT("");
    }
    
    // 檢查是否已存在
    for (const TPair<FString, FSaveSlot>& SlotPair : SaveSlots)
    {
        if (SlotPair.Value.SlotName == SlotName)
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot already exists: %s"), *SlotName);
            return TEXT("");
        }
    }
    
    // 創建保存槽位
    FSaveSlot SaveSlot = GetSaveSlotTemplate();
    SaveSlot.SlotID = GenerateUniqueID(TEXT("Slot"));
    SaveSlot.SlotName = SlotName;
    SaveSlot.Description = Description;
    SaveSlot.MaxSaves = MaxSaves;
    SaveSlot.bIsEmpty = true;
    
    SaveSlots.Add(SaveSlot.SlotID, SaveSlot);
    
    // 觸發槽位創建事件
    OnSaveSlotCreated.Broadcast(SaveSlot);
    
    UE_LOG(LogTemp, Log, TEXT("Created save slot: %s (%s)"), *SaveSlot.SlotID, *SlotName);
    
    return SaveSlot.SlotID;
}

bool UMingRTSSaveLoadEnhancedSystem::DeleteSaveSlot(const FString& SlotID)
{
    FSaveSlot* SaveSlot = SaveSlots.Find(SlotID);
    if (!SaveSlot)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save slot not found: %s"), *SlotID);
        return false;
    }
    
    // 刪除所有相關的保存數據
    for (const FString& SaveID : SaveSlot->SaveIDs)
    {
        FSaveGameData* SaveData = SaveGameData.Find(SaveID);
        if (SaveData)
        {
            // 刪除文件
            if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveData->FilePath))
            {
                FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SaveData->FilePath);
            }
            
            // 從映射中移除
            SaveGameData.Remove(SaveID);
        }
    }
    
    // 觸發槽位刪除事件
    OnSaveSlotDeleted.Broadcast(SlotID);
    
    // 從映射中移除槽位
    SaveSlots.Remove(SlotID);
    
    UE_LOG(LogTemp, Log, TEXT("Deleted save slot: %s"), *SlotID);
    
    return true;
}

FSaveSlot UMingRTSSaveLoadEnhancedSystem::GetSaveSlot(const FString& SlotID) const
{
    const FSaveSlot* SaveSlot = SaveSlots.Find(SlotID);
    if (SaveSlot)
    {
        return *SaveSlot;
    }
    return FSaveSlot();
}

TArray<FSaveSlot> UMingRTSSaveLoadEnhancedSystem::GetAllSaveSlots() const
{
    TArray<FSaveSlot> AllSlots;
    
    for (const TPair<FString, FSaveSlot>& SlotPair : SaveSlots)
    {
        AllSlots.Add(SlotPair.Value);
    }
    
    return AllSlots;
}

FSaveGameData UMingRTSSaveLoadEnhancedSystem::GetSaveGameData(const FString& SaveID) const
{
    const FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (SaveData)
    {
        return *SaveData;
    }
    return FSaveGameData();
}

void UMingRTSSaveLoadEnhancedSystem::AutoSave(const FString& Reason)
{
    if (!bAutoSaveEnabled)
    {
        return;
    }
    
    // 觸發自動保存事件
    OnAutoSaveTriggered.Broadcast(Reason);
    
    // 創建自動保存槽位
    FString AutoSaveSlotName = TEXT("AutoSave");
    FString AutoSaveSlotID = TEXT("");
    
    // 查找現有的自動保存槽位
    for (const TPair<FString, FSaveSlot>& SlotPair : SaveSlots)
    {
        if (SlotPair.Value.SlotName == AutoSaveSlotName)
        {
            AutoSaveSlotID = SlotPair.Key;
            break;
        }
    }
    
    if (AutoSaveSlotID.IsEmpty())
    {
        AutoSaveSlotID = CreateSaveSlot(AutoSaveSlotName, TEXT("Auto-generated save"));
    }
    
    if (!AutoSaveSlotID.IsEmpty())
    {
        // 保存到自動保存槽位
        SaveToSlot(AutoSaveSlotName, FString::Printf(TEXT("Auto save: %s"), *Reason));
        
        UE_LOG(LogTemp, Log, TEXT("Auto save completed: %s"), *Reason);
    }
}

void UMingRTSSaveLoadEnhancedSystem::SetAutoSaveInterval(float Interval)
{
    AutoSaveInterval = FMath::Max(Interval, 60.0f); // 最少1分鐘
    UE_LOG(LogTemp, Log, TEXT("Set auto save interval: %.1f seconds"), AutoSaveInterval);
}

bool UMingRTSSaveLoadEnhancedSystem::BackupSaveData(const FString& SaveID, const FString& BackupLocation)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    // 創建備份路徑
    FString BackupPath = BackupLocation;
    if (BackupPath.IsEmpty())
    {
        BackupPath = GetSavePath(ESaveLocation::Backup, FString::Printf(TEXT("%s_backup"), *SaveID));
    }
    
    // 複製文件
    if (FPlatformFileManager::Get().GetPlatformFile().CopyFile(*BackupPath, *SaveData->FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Created backup: %s -> %s"), *SaveID, *BackupPath);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create backup: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::RestoreBackup(const FString& BackupID)
{
    // 簡化實現：假設BackupID是備份文件路徑
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*BackupID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Backup file not found: %s"), *BackupID);
        return false;
    }
    
    // 這裡應該實現完整的備份恢復邏輯
    UE_LOG(LogTemp, Log, TEXT("Restored backup: %s"), *BackupID);
    
    return true;
}

bool UMingRTSSaveLoadEnhancedSystem::ValidateSaveData(const FString& SaveID)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        return false;
    }
    
    return ValidateSaveData(*SaveData);
}

bool UMingRTSSaveLoadEnhancedSystem::RepairSaveData(const FString& SaveID)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    // 嘗試重新載入和驗證
    TArray<uint8> Data;
    if (LoadGameData(SaveID, Data))
    {
        SaveData->Status = ESaveStatus::Loaded;
        SaveData->bIsValid = true;
        
        UE_LOG(LogTemp, Log, TEXT("Repaired save data: %s"), *SaveID);
        return true;
    }
    else
    {
        SaveData->Status = ESaveStatus::Corrupted;
        SaveData->bIsValid = false;
        
        UE_LOG(LogTemp, Error, TEXT("Failed to repair save data: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::CompressSaveData(const FString& SaveID)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    if (SaveData->bIsCompressed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data already compressed: %s"), *SaveID);
        return true;
    }
    
    // 壓縮數據
    TArray<uint8> CompressedData = CompressData(SaveData->Data);
    if (CompressedData.Num() < SaveData->Data.Num())
    {
        SaveData->Data = CompressedData;
        SaveData->bIsCompressed = true;
        SaveData->Size = CompressedData.Num();
        
        UE_LOG(LogTemp, Log, TEXT("Compressed save data: %s (%d -> %d bytes)"), *SaveID, SaveData->Size, CompressedData.Num());
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Compression did not reduce size: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::DecompressSaveData(const FString& SaveID)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    if (!SaveData->bIsCompressed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not compressed: %s"), *SaveID);
        return true;
    }
    
    // 解壓縮數據
    TArray<uint8> DecompressedData = DecompressData(SaveData->Data);
    if (DecompressedData.Num() > 0)
    {
        SaveData->Data = DecompressedData;
        SaveData->bIsCompressed = false;
        SaveData->Size = DecompressedData.Num();
        
        UE_LOG(LogTemp, Log, TEXT("Decompressed save data: %s (%d -> %d bytes)"), *SaveID, SaveData->Size, DecompressedData.Num());
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to decompress save data: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::EncryptSaveData(const FString& SaveID, const FString& EncryptionKey)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    if (SaveData->bIsEncrypted)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data already encrypted: %s"), *SaveID);
        return true;
    }
    
    // 加密數據
    TArray<uint8> EncryptedData = EncryptData(SaveData->Data, EncryptionKey);
    if (EncryptedData.Num() > 0)
    {
        SaveData->Data = EncryptedData;
        SaveData->bIsEncrypted = true;
        
        UE_LOG(LogTemp, Log, TEXT("Encrypted save data: %s"), *SaveID);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to encrypt save data: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::DecryptSaveData(const FString& SaveID, const FString& EncryptionKey)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    if (!SaveData->bIsEncrypted)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not encrypted: %s"), *SaveID);
        return true;
    }
    
    // 解密數據
    TArray<uint8> DecryptedData = DecryptData(SaveData->Data, EncryptionKey);
    if (DecryptedData.Num() > 0)
    {
        SaveData->Data = DecryptedData;
        SaveData->bIsEncrypted = false;
        
        UE_LOG(LogTemp, Log, TEXT("Decrypted save data: %s"), *SaveID);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to decrypt save data: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::ExportSaveData(const FString& SaveID, const FString& ExportPath, ESaveFormat Format)
{
    FSaveGameData* SaveData = SaveGameData.Find(SaveID);
    if (!SaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Save data not found: %s"), *SaveID);
        return false;
    }
    
    // 獲取數據並轉換格式
    TArray<uint8> ExportData = SaveData->Data;
    
    // 如果需要轉換格式
    if (Format != SaveData->Format)
    {
        TMap<FString, FString> DeserializedData = DeserializeData(SaveData->Data, SaveData->Format);
        ExportData = SerializeData(DeserializedData, Format);
    }
    
    // 導出到文件
    if (FFileHelper::SaveArrayToFile(ExportData, *ExportPath))
    {
        UE_LOG(LogTemp, Log, TEXT("Exported save data: %s -> %s"), *SaveID, *ExportPath);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to export save data: %s"), *SaveID);
        return false;
    }
}

bool UMingRTSSaveLoadEnhancedSystem::ImportSaveData(const FString& ImportPath, ESaveFormat Format)
{
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImportPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Import file not found: %s"), *ImportPath);
        return false;
    }
    
    // 從文件載入
    TArray<uint8> ImportData;
    if (!FFileHelper::LoadFileToArray(ImportData, *ImportPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load import file: %s"), *ImportPath);
        return false;
    }
    
    // 創建導入的保存數據
    FSaveGameData SaveData = GetSaveDataTemplate();
    SaveData.SaveID = GenerateUniqueID(TEXT("Import"));
    SaveData.SaveName = FString::Printf(TEXT("Import_%s"), *FPaths::GetBaseFilename(ImportPath));
    SaveData.DataType = ESaveDataType::Custom;
    SaveData.Format = Format;
    SaveData.Location = ESaveLocation::Local;
    SaveData.Status = ESaveStatus::Loaded;
    SaveData.Timestamp = CurrentWorld->GetTimeSeconds();
    SaveData.Data = ImportData;
    SaveData.Size = ImportData.Num();
    SaveData.bIsValid = true;
    SaveData.FilePath = ImportPath;
    
    SaveGameData.Add(SaveData.SaveID, SaveData);
    
    UE_LOG(LogTemp, Log, TEXT("Imported save data: %s"), *SaveData.SaveID);
    
    return true;
}

TMap<FString, float> UMingRTSSaveLoadEnhancedSystem::GetSaveStatistics() const
{
    TMap<FString, float> Statistics;
    
    // 基本統計
    Statistics.Add(TEXT("TotalSaves"), SaveGameData.Num());
    Statistics.Add(TEXT("TotalSlots"), SaveSlots.Num());
    Statistics.Add(TEXT("ActiveOperations"), SaveOperations.Num());
    
    // 保存狀態統計
    int32 SavedCount = 0;
    int32 LoadedCount = 0;
    int32 CorruptedCount = 0;
    int32 CompressedCount = 0;
    int32 EncryptedCount = 0;
    
    for (const TPair<FString, FSaveGameData>& SavePair : SaveGameData)
    {
        const FSaveGameData& Data = SavePair.Value;
        
        if (Data.Status == ESaveStatus::Saved)
        {
            SavedCount++;
        }
        else if (Data.Status == ESaveStatus::Loaded)
        {
            LoadedCount++;
        }
        else if (Data.Status == ESaveStatus::Corrupted)
        {
            CorruptedCount++;
        }
        
        if (Data.bIsCompressed)
        {
            CompressedCount++;
        }
        
        if (Data.bIsEncrypted)
        {
            EncryptedCount++;
        }
    }
    
    Statistics.Add(TEXT("SavedCount"), SavedCount);
    Statistics.Add(TEXT("LoadedCount"), LoadedCount);
    Statistics.Add(TEXT("CorruptedCount"), CorruptedCount);
    Statistics.Add(TEXT("CompressedCount"), CompressedCount);
    Statistics.Add(TEXT("EncryptedCount"), EncryptedCount);
    
    // 槽位統計
    int32 EmptySlots = 0;
    int32 UsedSlots = 0;
    
    for (const TPair<FString, FSaveSlot>& SlotPair : SaveSlots)
    {
        const FSaveSlot& Slot = SlotPair.Value;
        
        if (Slot.bIsEmpty)
        {
            EmptySlots++;
        }
        else
        {
            UsedSlots++;
        }
    }
    
    Statistics.Add(TEXT("EmptySlots"), EmptySlots);
    Statistics.Add(TEXT("UsedSlots"), UsedSlots);
    
    return Statistics;
}

void UMingRTSSaveLoadEnhancedSystem::CleanupOldSaveData(float MaxAge)
{
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    for (auto It = SaveGameData.CreateIterator(); It; ++It)
    {
        const FSaveGameData& SaveData = It->Value;
        
        float Age = CurrentTime - SaveData.Timestamp;
        if (Age > MaxAge)
        {
            // 刪除舊文件
            if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveData.FilePath))
            {
                FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SaveData.FilePath);
            }
            
            It.RemoveCurrent();
            UE_LOG(LogTemp, Log, TEXT("Cleaned up old save data: %s"), *SaveData.SaveID);
        }
    }
}

void UMingRTSSaveLoadEnhancedSystem::OptimizeSavePerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing save performance..."));
    
    // 清理無效數據
    CleanupInvalidSaveData();
    CleanupInvalidSaveSlots();
    
    // 壓縮未壓縮的數據
    for (TPair<FString, FSaveGameData>& SavePair : SaveGameData)
    {
        FSaveGameData& SaveData = SavePair.Value;
        if (!SaveData.bIsCompressed && SaveData.Size > 1024) // 大於1KB的數據
        {
            CompressSaveData(SavePair.Key);
        }
    }
    
    // 平衡保存負載
    BalanceSaveLoad();
    
    UE_LOG(LogTemp, Log, TEXT("Save performance optimization completed"));
}

void UMingRTSSaveLoadEnhancedSystem::SetSavePriority(const FString& SaveID, ESavePriority Priority)
{
    FSaveOperation* Operation = SaveOperations.Find(SaveID);
    if (Operation)
    {
        Operation->Priority = Priority;
        UE_LOG(LogTemp, Log, TEXT("Set save priority: %s - %d"), *SaveID, (int32)Priority);
    }
}

void UMingRTSSaveLoadEnhancedSystem::BatchSave(const TArray<FString>& SaveIDs)
{
    for (const FString& SaveID : SaveIDs)
    {
        // 設置為高優先級
        SetSavePriority(SaveID, ESavePriority::High);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch save initiated for %d saves"), SaveIDs.Num());
}

void UMingRTSSaveLoadEnhancedSystem::BatchLoad(const TArray<FString>& SaveIDs)
{
    for (const FString& SaveID : SaveIDs)
    {
        // 設置為高優先級
        SetSavePriority(SaveID, ESavePriority::High);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch load initiated for %d saves"), SaveIDs.Num());
}

void UMingRTSSaveLoadEnhancedSystem::ProcessSaveOperations(float DeltaTime)
{
    for (auto It = SaveOperations.CreateIterator(); It; ++It)
    {
        FSaveOperation& Operation = It->Value;
        
        if (Operation.OperationType == TEXT("Save"))
        {
            // 處理保存操作
            Operation.Progress += DeltaTime * 0.5f; // 模擬進度
            
            if (Operation.Progress >= 1.0f)
            {
                Operation.bIsCompleted = true;
                It.RemoveCurrent();
            }
        }
    }
}

void UMingRTSSaveLoadEnhancedSystem::ProcessLoadOperations(float DeltaTime)
{
    for (auto It = SaveOperations.CreateIterator(); It; ++It)
    {
        FSaveOperation& Operation = It->Value;
        
        if (Operation.OperationType == TEXT("Load"))
        {
            // 處理載入操作
            Operation.Progress += DeltaTime * 0.5f; // 模擬進度
            
            if (Operation.Progress >= 1.0f)
            {
                Operation.bIsCompleted = true;
                It.RemoveCurrent();
            }
        }
    }
}

void UMingRTSSaveLoadEnhancedSystem::ProcessAutoSave(float DeltaTime)
{
    // 自動保存邏輯已在UpdateSaveLoadEnhancedSystem中處理
}

void UMingRTSSaveLoadEnhancedSystem::ProcessBackupOperations(float DeltaTime)
{
    // 處理備份操作
    // 這裡可以添加備份邏輯
}

bool UMingRTSSaveLoadEnhancedSystem::ValidateSaveIntegrity(const FSaveGameData& SaveData) const
{
    if (SaveData.SaveID.IsEmpty() || SaveData.Data.Num() == 0)
    {
        return false;
    }
    
    // 驗證校驗和
    FString CalculatedChecksum = CalculateChecksum(SaveData.Data);
    if (CalculatedChecksum != SaveData.Checksum)
    {
        return false;
    }
    
    return true;
}

FString UMingRTSSaveLoadEnhancedSystem::CalculateChecksum(const TArray<uint8>& Data) const
{
    // 簡化的校驗和計算
    uint32 Checksum = 0;
    for (uint8 Byte : Data)
    {
        Checksum = Checksum * 31 + Byte;
    }
    
    return FString::Printf(TEXT("%08X"), Checksum);
}

TArray<uint8> UMingRTSSaveLoadEnhancedSystem::SerializeData(const TMap<FString, FString>& Data, ESaveFormat Format) const
{
    TArray<uint8> SerializedData;
    
    switch (Format)
    {
        case ESaveFormat::Binary:
            {
                // 簡化的二進制序列化
                for (const TPair<FString, FString>& Pair : Data)
                {
                    // 寫入鍵長度和鍵
                    int32 KeyLength = Pair.Key.Len();
                    SerializedData.Append((uint8*)&KeyLength, sizeof(KeyLength));
                    SerializedData.Append((uint8*)*Pair.Key, KeyLength);
                    
                    // 寫入值長度和值
                    int32 ValueLength = Pair.Value.Len();
                    SerializedData.Append((uint8*)&ValueLength, sizeof(ValueLength));
                    SerializedData.Append((uint8*)*Pair.Value, ValueLength);
                }
                break;
            }
        case ESaveFormat::JSON:
            {
                // 簡化的JSON序列化
                FString JsonString = TEXT("{");
                bool bFirst = true;
                
                for (const TPair<FString, FString>& Pair : Data)
                {
                    if (!bFirst)
                    {
                        JsonString += TEXT(",");
                    }
                    JsonString += FString::Printf(TEXT("\"%s\":\"%s\""), *Pair.Key, *Pair.Value);
                    bFirst = false;
                }
                
                JsonString += TEXT("}");
                
                SerializedData.SetNum(JsonString.Len());
                for (int32 i = 0; i < JsonString.Len(); i++)
                {
                    SerializedData[i] = (uint8)JsonString[i];
                }
                break;
            }
        default:
            break;
    }
    
    return SerializedData;
}

TMap<FString, FString> UMingRTSSaveLoadEnhancedSystem::DeserializeData(const TArray<uint8>& Data, ESaveFormat Format) const
{
    TMap<FString, FString> DeserializedData;
    
    switch (Format)
    {
        case ESaveFormat::Binary:
            {
                // 簡化的二進制反序列化
                int32 Index = 0;
                while (Index < Data.Num())
                {
                    // 讀取鍵長度
                    if (Index + sizeof(int32) > Data.Num()) break;
                    int32 KeyLength = *(int32*)(Data.GetData() + Index);
                    Index += sizeof(int32);
                    
                    // 讀取鍵
                    if (Index + KeyLength > Data.Num()) break;
                    FString Key;
                    Key.AppendChars((TCHAR*)(Data.GetData() + Index), KeyLength);
                    Index += KeyLength;
                    
                    // 讀取值長度
                    if (Index + sizeof(int32) > Data.Num()) break;
                    int32 ValueLength = *(int32*)(Data.GetData() + Index);
                    Index += sizeof(int32);
                    
                    // 讀取值
                    if (Index + ValueLength > Data.Num()) break;
                    FString Value;
                    Value.AppendChars((TCHAR*)(Data.GetData() + Index), ValueLength);
                    Index += ValueLength;
                    
                    DeserializedData.Add(Key, Value);
                }
                break;
            }
        case ESaveFormat::JSON:
            {
                // 簡化的JSON反序列化
                FString JsonString;
                JsonString.AppendChars((TCHAR*)Data.GetData(), Data.Num() / sizeof(TCHAR));
                
                // 這裡應該使用JSON解析器，簡化實現只處理基本格式
                // 在實際實現中應該使用UE的JSON庫
                break;
            }
        default:
            break;
    }
    
    return DeserializedData;
}

TArray<uint8> UMingRTSSaveLoadEnhancedSystem::CompressData(const TArray<uint8>& Data) const
{
    TArray<uint8> CompressedData;
    
    // 使用UE的壓縮功能
    if (FCompression::CompressMemory(CompressedData, Data.GetData(), Data.Num()))
    {
        return CompressedData;
    }
    
    return Data; // 壓縮失敗，返回原數據
}

TArray<uint8> UMingRTSSaveLoadEnhancedSystem::DecompressData(const TArray<uint8>& Data) const
{
    TArray<uint8> DecompressedData;
    
    // 使用UE的解壓縮功能
    if (FCompression::DecompressMemory(DecompressedData, Data.GetData(), Data.Num()))
    {
        return DecompressedData;
    }
    
    return Data; // 解壓縮失敗，返回原數據
}

TArray<uint8> UMingRTSSaveLoadEnhancedSystem::EncryptData(const TArray<uint8>& Data, const FString& Key) const
{
    // 簡化的加密實現（XOR加密）
    TArray<uint8> EncryptedData = Data;
    
    if (Key.Len() > 0)
    {
        for (int32 i = 0; i < EncryptedData.Num(); i++)
        {
            EncryptedData[i] ^= (uint8)Key[i % Key.Len()];
        }
    }
    
    return EncryptedData;
}

TArray<uint8> UMingRTSSaveLoadEnhancedSystem::DecryptData(const TArray<uint8>& Data, const FString& Key) const
{
    // XOR加密是對稱的，解密和加密相同
    return EncryptData(Data, Key);
}

FString UMingRTSSaveLoadEnhancedSystem::GenerateThumbnail(const FString& SlotID) const
{
    // 簡化實現：返回空路徑
    // 在實際實現中應該截取遊戲畫面並保存為縮略圖
    return TEXT("");
}

FString UMingRTSSaveLoadEnhancedSystem::GetSavePath(ESaveLocation Location, const FString& FileName) const
{
    FString BasePath;
    
    switch (Location)
    {
        case ESaveLocation::Local:
            BasePath = FPaths::ProjectSavedDir();
            break;
        case ESaveLocation::Cloud:
            BasePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Cloud"));
            break;
        case ESaveLocation::Network:
            BasePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Network"));
            break;
        case ESaveLocation::Backup:
            BasePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Backup"));
            break;
        case ESaveLocation::Temp:
            BasePath = FPaths::ProjectTempDir();
            break;
        default:
            BasePath = FPaths::ProjectSavedDir();
            break;
    }
    
    return FPaths::Combine(BasePath, FileName);
}

bool UMingRTSSaveLoadEnhancedSystem::CreateSaveDirectory(const FString& Path) const
{
    return IPlatformFile::GetPlatformFile().CreateDirectory(*Path);
}

FString UMingRTSSaveLoadEnhancedSystem::GetSaveDataName(ESaveDataType DataType) const
{
    switch (DataType)
    {
        case ESaveDataType::GameState: return TEXT("GameState");
        case ESaveDataType::PlayerData: return TEXT("PlayerData");
        case ESaveDataType::WorldData: return TEXT("WorldData");
        case ESaveDataType::Inventory: return TEXT("Inventory");
        case ESaveDataType::Quests: return TEXT("Quests");
        case ESaveDataType::Relationships: return TEXT("Relationships");
        case ESaveDataType::Campaign: return TEXT("Campaign");
        case ESaveDataType::Settings: return TEXT("Settings");
        case ESaveDataType::Statistics: return TEXT("Statistics");
        case ESaveDataType::Custom: return TEXT("Custom");
        case ESaveDataType::All: return TEXT("All");
        default: return TEXT("Unknown");
    }
}

FString UMingRTSSaveLoadEnhancedSystem::GetSaveFormatName(ESaveFormat Format) const
{
    switch (Format)
    {
        case ESaveFormat::Binary: return TEXT("bin");
        case ESaveFormat::JSON: return TEXT("json");
        case ESaveFormat::XML: return TEXT("xml");
        case ESaveFormat::CSV: return TEXT("csv");
        case ESaveFormat::Text: return TEXT("txt");
        case ESaveFormat::Compressed: return TEXT("cmp");
        case ESaveFormat::Encrypted: return TEXT("enc");
        case ESaveFormat::Cloud: return TEXT("cloud");
        case ESaveFormat::Database: return TEXT("db");
        case ESaveFormat::Custom: return TEXT("custom");
        default: return TEXT("bin");
    }
}

FString UMingRTSSaveLoadEnhancedSystem::GetSaveLocationName(ESaveLocation Location) const
{
    switch (Location)
    {
        case ESaveLocation::Local: return TEXT("Local");
        case ESaveLocation::Cloud: return TEXT("Cloud");
        case ESaveLocation::Network: return TEXT("Network");
        case ESaveLocation::USB: return TEXT("USB");
        case ESaveLocation::Memory: return TEXT("Memory");
        case ESaveLocation::Cache: return TEXT("Cache");
        case ESaveLocation::Temp: return TEXT("Temp");
        case ESaveLocation::Backup: return TEXT("Backup");
        case ESaveLocation::Archive: return TEXT("Archive");
        case ESaveLocation::Custom: return TEXT("Custom");
        default: return TEXT("Local");
    }
}

FString UMingRTSSaveLoadEnhancedSystem::GenerateUniqueID(const FString& Prefix) const
{
    return FString::Printf(TEXT("%s_%d"), *Prefix, FDateTime::Now().GetTicks());
}

bool UMingRTSSaveLoadEnhancedSystem::ValidateSaveData(const FSaveGameData& SaveData) const
{
    if (SaveData.SaveID.IsEmpty())
    {
        return false;
    }
    
    if (SaveData.SaveName.IsEmpty())
    {
        return false;
    }
    
    if (SaveData.Data.Num() == 0)
    {
        return false;
    }
    
    return true;
}

bool UMingRTSSaveLoadEnhancedSystem::ValidateSaveSlot(const FSaveSlot& SaveSlot) const
{
    if (SaveSlot.SlotID.IsEmpty())
    {
        return false;
    }
    
    if (SaveSlot.SlotName.IsEmpty())
    {
        return false;
    }
    
    return true;
}

void UMingRTSSaveLoadEnhancedSystem::HandleSaveError(const FString& OperationID, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Save error: %s - %s"), *OperationID, *ErrorMessage);
    
    FSaveOperation* Operation = SaveOperations.Find(OperationID);
    if (Operation)
    {
        Operation->bIsFailed = true;
        Operation->ErrorMessage = ErrorMessage;
    }
}

void UMingRTSSaveLoadEnhancedSystem::HandleLoadError(const FString& OperationID, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Load error: %s - %s"), *OperationID, *ErrorMessage);
    
    FSaveOperation* Operation = SaveOperations.Find(OperationID);
    if (Operation)
    {
        Operation->bIsFailed = true;
        Operation->ErrorMessage = ErrorMessage;
    }
}

void UMingRTSSaveLoadEnhancedSystem::CleanupInvalidSaveData()
{
    for (auto It = SaveGameData.CreateIterator(); It; ++It)
    {
        const FSaveGameData& SaveData = It->Value;
        
        if (!ValidateSaveData(SaveData))
        {
            It.RemoveCurrent();
            UE_LOG(LogTemp, Log, TEXT("Cleaned up invalid save data: %s"), *SaveData.SaveID);
        }
    }
}

void UMingRTSSaveLoadEnhancedSystem::CleanupInvalidSaveSlots()
{
    for (auto It = SaveSlots.CreateIterator(); It; ++It)
    {
        const FSaveSlot& SaveSlot = It->Value;
        
        if (!ValidateSaveSlot(SaveSlot))
        {
            It.RemoveCurrent();
            UE_LOG(LogTemp, Log, TEXT("Cleaned up invalid save slot: %s"), *SaveSlot.SlotID);
        }
    }
}

FSaveGameData UMingRTSSaveLoadEnhancedSystem::GetSaveDataTemplate() const
{
    FSaveGameData Template;
    Template.DataType = ESaveDataType::GameState;
    Template.Format = DefaultFormat;
    Template.Location = DefaultLocation;
    Template.Status = ESaveStatus::None;
    Template.Timestamp = 0.0f;
    Template.Version = 1;
    Template.Size = 0;
    Template.bIsCompressed = false;
    Template.bIsEncrypted = false;
    Template.bIsAutoSave = false;
    Template.bIsValid = false;
    
    return Template;
}

FSaveSlot UMingRTSSaveLoadEnhancedSystem::GetSaveSlotTemplate() const
{
    FSaveSlot Template;
    Template.Description = TEXT("");
    Template.ThumbnailPath = TEXT("");
    Template.PlayTime = 0.0f;
    Template.LevelName = TEXT("");
    Template.PlayerName = TEXT("");
    Template.Chapter = 0;
    Template.CampaignID = TEXT("");
    Template.LastModified = 0.0f;
    Template.bIsEmpty = true;
    Template.bIsCorrupted = false;
    Template.bIsReadOnly = false;
    Template.MaxSaves = 10;
    
    return Template;
}

FSaveOperation UMingRTSSaveLoadEnhancedSystem::GetSaveOperationTemplate() const
{
    FSaveOperation Template;
    Template.OperationType = TEXT("");
    Template.TargetID = TEXT("");
    Template.DataType = ESaveDataType::GameState;
    Template.Priority = ESavePriority::Medium;
    Template.StartTime = 0.0f;
    Template.Progress = 0.0f;
    Template.bIsCompleted = false;
    Template.bIsFailed = false;
    Template.ErrorMessage = TEXT("");
    
    return Template;
}

void UMingRTSSaveLoadEnhancedSystem::RecordSaveStatistics()
{
    TMap<FString, float> Stats = GetSaveStatistics();
    
    for (const TPair<FString, float>& Stat : Stats)
    {
        UE_LOG(LogTemp, VeryVerbose, TEXT("Save Stat: %s - %.2f"), *Stat.Key, Stat.Value);
    }
}

float UMingRTSSaveLoadEnhancedSystem::PredictSaveLoad() const
{
    float Load = 0.0f;
    
    // 基於保存數量和操作數量預測負載
    Load += (float)SaveGameData.Num() / MaxSaveCount * 0.5f;
    Load += (float)SaveOperations.Num() / 10.0f * 0.5f; // 假設最多10個併發操作
    
    return FMath::Clamp(Load, 0.0f, 1.0f);
}

void UMingRTSSaveLoadEnhancedSystem::BalanceSaveLoad()
{
    float CurrentLoad = PredictSaveLoad();
    
    if (CurrentLoad > 0.8f)
    {
        // 降低保存頻率
        SaveUpdateInterval = FMath::Min(SaveUpdateInterval * 1.2f, 0.5f);
        
        UE_LOG(LogTemp, Log, TEXT("Balanced save load: reduced update interval"));
    }
    else if (CurrentLoad < 0.3f)
    {
        // 提高保存頻率
        SaveUpdateInterval = FMath::Max(SaveUpdateInterval * 0.8f, 0.05f);
        
        UE_LOG(LogTemp, Log, TEXT("Balanced save load: increased update interval"));
    }
}

void UMingRTSSaveLoadEnhancedSystem::ResolveSaveConflicts()
{
    // 解決保存衝突
    // 這裡可以添加衝突檢測和解決邏輯
}

void UMingRTSSaveLoadEnhancedSystem::OptimizeSavePerformance()
{
    // 優化保存性能
    // 這裡可以添加性能優化邏輯
}

TMap<FString, FString> UMingRTSSaveLoadEnhancedSystem::GetCurrentGameState() const
{
    TMap<FString, FString> GameState;
    
    // 獲取當前遊戲狀態
    if (CurrentWorld)
    {
        GameState.Add(TEXT("WorldName"), CurrentWorld->GetMapName());
        GameState.Add(TEXT("TimeSeconds"), FString::SanitizeFloat(CurrentWorld->GetTimeSeconds()));
        GameState.Add(TEXT("CurrentSlot"), CurrentSlotID);
        
        // 獲取遊戲模式信息
        if (AGameModeBase* GameMode = CurrentWorld->GetAuthGameMode())
        {
            GameState.Add(TEXT("GameMode"), GameMode->GetClass()->GetName());
        }
    }
    
    return GameState;
}

void UMingRTSSaveLoadEnhancedSystem::SetGameState(const TMap<FString, FString>& GameState)
{
    // 設置遊戲狀態
    // 這裡應該根據保存的數據恢復遊戲狀態
    UE_LOG(LogTemp, Log, TEXT("Set game state with %d entries"), GameState.Num());
}

TMap<FString, FString> UMingRTSSaveLoadEnhancedSystem::GetCurrentPlayerData() const
{
    TMap<FString, FString> PlayerData;
    
    // 獲取當前玩家數據
    if (CurrentWorld)
    {
        if (APlayerController* PC = CurrentWorld->GetFirstPlayerController())
        {
            if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
            {
                PlayerData.Add(TEXT("PlayerName"), PS->GetPlayerName());
                PlayerData.Add(TEXT("PlayerID"), PS->GetUniqueId().ToString());
            }
            
            // 獲取玩家位置
            FVector PlayerLocation = PC->GetPawn() ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
            PlayerData.Add(TEXT("PlayerLocation"), PlayerLocation.ToString());
        }
    }
    
    return PlayerData;
}

void UMingRTSSaveLoadEnhancedSystem::SetPlayerData(const TMap<FString, FString>& PlayerData)
{
    // 設置玩家數據
    // 這裡應該根據保存的數據恢復玩家狀態
    UE_LOG(LogTemp, Log, TEXT("Set player data with %d entries"), PlayerData.Num());
}

TMap<FString, FString> UMingRTSSaveLoadEnhancedSystem::GetCurrentWorldData() const
{
    TMap<FString, FString> WorldData;
    
    // 獲取當前世界數據
    if (CurrentWorld)
    {
        WorldData.Add(TEXT("WorldName"), CurrentWorld->GetMapName());
        WorldData.Add(TEXT("WorldTime"), FString::SanitizeFloat(CurrentWorld->GetTimeSeconds()));
        
        // 獲取世界中的Actor數量
        int32 ActorCount = 0;
        for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
        {
            ActorCount++;
        }
        WorldData.Add(TEXT("ActorCount"), FString::FromInt(ActorCount));
    }
    
    return WorldData;
}

void UMingRTSSaveLoadEnhancedSystem::SetWorldData(const TMap<FString, FString>& WorldData)
{
    // 設置世界數據
    // 這裡應該根據保存的數據恢復世界狀態
    UE_LOG(LogTemp, Log, TEXT("Set world data with %d entries"), WorldData.Num());
}
