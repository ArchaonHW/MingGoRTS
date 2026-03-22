#include "Save/MingSaveGame.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Compression/OodleDataCompression.h"
#include "Encryption/AESWrapper.h"

UMingSaveGame::UMingSaveGame()
{
    Header.Version = EMingSaveGameVersion::Current;
    Header.SaveDateTime = FDateTime::Now();
    Header.PlayTimeSeconds = 0;
}

bool UMingSaveGame::IsCompatibleVersion() const
{
    return static_cast<int32>(Header.Version) <= static_cast<int32>(EMingSaveGameVersion::Current);
}

void UMingSaveGame::UpgradeToCurrentVersion()
{
    while (Header.Version < EMingSaveGameVersion::Current)
    {
        EMingSaveGameVersion CurrentVer = Header.Version;
        
        switch (CurrentVer)
        {
        case EMingSaveGameVersion::Initial:
            MigrateFromVersion1(this, this);
            Header.Version = EMingSaveGameVersion::Version_1_1;
            break;
            
        case EMingSaveGameVersion::Version_1_1:
            MigrateFromVersion2(this, this);
            Header.Version = EMingSaveGameVersion::Version_1_2;
            break;
            
        default:
            // Already at current version
            break;
        }
    }
}

void UMingSaveGame::SetRelationshipData(const TArray<uint8>& Data)
{
    RelationshipData = Data;
}

TArray<uint8> UMingSaveGame::GetRelationshipData() const
{
    return RelationshipData;
}

void UMingSaveGame::SetReputationData(const TArray<uint8>& Data)
{
    ReputationData = Data;
}

TArray<uint8> UMingSaveGame::GetReputationData() const
{
    return ReputationData;
}

void UMingSaveGame::SetQuestData(const TArray<uint8>& Data)
{
    QuestData = Data;
}

TArray<uint8> UMingSaveGame::GetQuestData() const
{
    return QuestData;
}

void UMingSaveGame::SetAudioSetting(const FString& SettingName, float Value)
{
    AudioSettings.Add(SettingName, Value);
}

float UMingSaveGame::GetAudioSetting(const FString& SettingName, float DefaultValue) const
{
    if (AudioSettings.Contains(SettingName))
    {
        return AudioSettings[SettingName];
    }
    return DefaultValue;
}

void UMingSaveGame::SetUISetting(const FString& SettingName, const FString& Value)
{
    UISettings.Add(SettingName, Value);
}

FString UMingSaveGame::GetUISetting(const FString& SettingName, const FString& DefaultValue) const
{
    if (UISettings.Contains(SettingName))
    {
        return UISettings[SettingName];
    }
    return DefaultValue;
}

void UMingSaveGame::SetGameStateData(const FString& Key, const FString& Value)
{
    GameStateData.Add(Key, Value);
}

FString UMingSaveGame::GetGameStateData(const FString& Key, const FString& DefaultValue) const
{
    if (GameStateData.Contains(Key))
    {
        return GameStateData[Key];
    }
    return DefaultValue;
}

void UMingSaveGame::SetPlayerPreference(const FString& Key, const FString& Value)
{
    PlayerPreferences.Add(Key, Value);
}

FString UMingSaveGame::GetPlayerPreference(const FString& Key, const FString& DefaultValue) const
{
    if (PlayerPreferences.Contains(Key))
    {
        return PlayerPreferences[Key];
    }
    return DefaultValue;
}

void UMingSaveGame::SetMetadata(const FString& Key, const FString& Value)
{
    Header.Metadata.Add(Key, Value);
}

FString UMingSaveGame::GetMetadata(const FString& Key, const FString& DefaultValue) const
{
    if (Header.Metadata.Contains(Key))
    {
        return Header.Metadata[Key];
    }
    return DefaultValue;
}

void UMingSaveGame::UpdatePlayTime(int32 AdditionalSeconds)
{
    Header.PlayTimeSeconds += AdditionalSeconds;
}

FString UMingSaveGame::GetFormattedPlayTime() const
{
    int32 Hours = Header.PlayTimeSeconds / 3600;
    int32 Minutes = (Header.PlayTimeSeconds % 3600) / 60;
    int32 Seconds = Header.PlayTimeSeconds % 60;
    
    return FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
}

bool UMingSaveGame::ValidateSaveData() const
{
    // Check required data is present
    if (Header.SaveGameName.IsEmpty())
    {
        return false;
    }
    
    if (Header.Version == EMingSaveGameVersion::Initial)
    {
        // Version 1 is no longer supported
        return false;
    }
    
    return true;
}

TArray<FString> UMingSaveGame::GetValidationErrors() const
{
    TArray<FString> Errors;
    
    if (Header.SaveGameName.IsEmpty())
    {
        Errors.Add(TEXT("Save game name is empty"));
    }
    
    if (!IsCompatibleVersion())
    {
        Errors.Add(FString::Printf(TEXT("Incompatible version: %d"), static_cast<int32>(Header.Version)));
    }
    
    if (RelationshipData.Num() == 0)
    {
        Errors.Add(TEXT("Relationship data is missing"));
    }
    
    if (ReputationData.Num() == 0)
    {
        Errors.Add(TEXT("Reputation data is missing"));
    }
    
    return Errors;
}

void UMingSaveGame::CompressData()
{
    // Compress large data arrays using Oodle
    if (RelationshipData.Num() > 1024)
    {
        TArray<uint8> Compressed;
        FOodleDataCompression::CompressData(RelationshipData, Compressed, FOodleDataCompression::ECompressor::Mermaid);
        RelationshipData = Compressed;
    }
    
    if (ReputationData.Num() > 1024)
    {
        TArray<uint8> Compressed;
        FOodleDataCompression::CompressData(ReputationData, Compressed, FOodleDataCompression::ECompressor::Mermaid);
        ReputationData = Compressed;
    }
    
    if (QuestData.Num() > 1024)
    {
        TArray<uint8> Compressed;
        FOodleDataCompression::CompressData(QuestData, Compressed, FOodleDataCompression::ECompressor::Mermaid);
        QuestData = Compressed;
    }
}

void UMingSaveGame::DecompressData()
{
    // Decompress data if needed
    // This would check for compression markers and decompress
    // For now, assume data is not compressed
}

void UMingSaveGame::EncryptData(const FString& EncryptionKey)
{
    // Encrypt sensitive data using AES
    if (!EncryptionKey.IsEmpty())
    {
        // In a real implementation, this would encrypt the data arrays
        // using AES encryption with the provided key
    }
}

void UMingSaveGame::DecryptData(const FString& EncryptionKey)
{
    // Decrypt data using AES
    if (!EncryptionKey.IsEmpty())
    {
        // In a real implementation, this would decrypt the data arrays
    }
}

UMingSaveGame* UMingSaveGame::CreateSaveGame(UObject* WorldContextObject, const FString& SaveName, int32 SlotIndex)
{
    UMingSaveGame* SaveGame = Cast<UMingSaveGame>(UGameplayStatics::CreateSaveGameObject(UMingSaveGame::StaticClass()));
    
    if (SaveGame)
    {
        SaveGame->Header.SaveGameName = SaveName;
        SaveGame->Header.SaveSlotIndex = SlotIndex;
        SaveGame->Header.SaveDateTime = FDateTime::Now();
        SaveGame->Header.PlayerName = TEXT("Player");
        SaveGame->Header.Version = EMingSaveGameVersion::Current;
    }
    
    return SaveGame;
}

FString UMingSaveGame::GetSaveSlotName(int32 SlotIndex)
{
    return FString::Printf(TEXT("MingSaveSlot_%d"), SlotIndex);
}

int32 UMingSaveGame::GetMaxSaveSlots()
{
    return 50; // Maximum 50 save slots
}

bool UMingSaveGame::MigrateFromVersion1(const UMingSaveGame* OldSave, UMingSaveGame* NewSave)
{
    if (!OldSave || !NewSave)
    {
        return false;
    }
    
    // Copy basic header info
    NewSave->Header.SaveGameName = OldSave->Header.SaveGameName;
    NewSave->Header.PlayerName = OldSave->Header.PlayerName;
    NewSave->Header.SaveDateTime = OldSave->Header.SaveDateTime;
    NewSave->Header.PlayTimeSeconds = OldSave->Header.PlayTimeSeconds;
    NewSave->Header.CurrentLevel = OldSave->Header.CurrentLevel;
    NewSave->Header.SaveSlotIndex = OldSave->Header.SaveSlotIndex;
    
    // Migrate relationship data (convert from old format)
    NewSave->RelationshipData = OldSave->RelationshipData;
    
    // Migrate reputation data (convert from old format)
    NewSave->ReputationData = OldSave->ReputationData;
    
    // Migrate quest data
    NewSave->QuestData = OldSave->QuestData;
    
    // Audio settings may have different keys
    NewSave->AudioSettings = OldSave->AudioSettings;
    
    return true;
}

bool UMingSaveGame::MigrateFromVersion2(const UMingSaveGame* OldSave, UMingSaveGame* NewSave)
{
    if (!OldSave || !NewSave)
    {
        return false;
    }
    
    // Version 2 to 3 migration is mostly the same
    // Just copy all data
    NewSave->Header = OldSave->Header;
    NewSave->RelationshipData = OldSave->RelationshipData;
    NewSave->ReputationData = OldSave->ReputationData;
    NewSave->QuestData = OldSave->QuestData;
    NewSave->AudioSettings = OldSave->AudioSettings;
    NewSave->UISettings = OldSave->UISettings;
    NewSave->GameStateData = OldSave->GameStateData;
    NewSave->PlayerPreferences = OldSave->PlayerPreferences;
    
    // Any format conversions would go here
    
    return true;
}
