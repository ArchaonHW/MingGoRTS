#include "Save/MingSaveGame.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"

UMingSaveGame::UMingSaveGame()
{
    Header.Version = EMingSaveGameVersion::Current;
    Header.SaveDateTime = FDateTime::Now();
}

bool UMingSaveGame::IsCompatibleVersion() const
{
    return Header.Version >= EMingSaveGameVersion::Initial &&
           Header.Version <= EMingSaveGameVersion::Current;
}

void UMingSaveGame::UpgradeToCurrentVersion()
{
    if (Header.Version == EMingSaveGameVersion::Current)
    {
        return;
    }

    // 執行版本遷移
    if (Header.Version == EMingSaveGameVersion::Initial)
    {
        // 從版本 1 遷移到版本 2
        Header.Version = EMingSaveGameVersion::Version_1_1;
    }

    if (Header.Version == EMingSaveGameVersion::Version_1_1)
    {
        // 從版本 2 遷移到版本 3
        Header.Version = EMingSaveGameVersion::Version_1_2;
    }

    // 更新到當前版本
    Header.Version = EMingSaveGameVersion::Current;
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
    if (const float* Value = AudioSettings.Find(SettingName))
    {
        return *Value;
    }
    return DefaultValue;
}

void UMingSaveGame::SetUISetting(const FString& SettingName, const FString& Value)
{
    UISettings.Add(SettingName, Value);
}

FString UMingSaveGame::GetUISetting(const FString& SettingName, const FString& DefaultValue) const
{
    if (const FString* Value = UISettings.Find(SettingName))
    {
        return *Value;
    }
    return DefaultValue;
}

void UMingSaveGame::SetGameStateData(const FString& Key, const FString& Value)
{
    GameStateData.Add(Key, Value);
}

FString UMingSaveGame::GetGameStateData(const FString& Key, const FString& DefaultValue) const
{
    if (const FString* Value = GameStateData.Find(Key))
    {
        return *Value;
    }
    return DefaultValue;
}

void UMingSaveGame::SetPlayerPreference(const FString& Key, const FString& Value)
{
    PlayerPreferences.Add(Key, Value);
}

FString UMingSaveGame::GetPlayerPreference(const FString& Key, const FString& DefaultValue) const
{
    if (const FString* Value = PlayerPreferences.Find(Key))
    {
        return *Value;
    }
    return DefaultValue;
}

void UMingSaveGame::SetMetadata(const FString& Key, const FString& Value)
{
    Header.Metadata.Add(Key, Value);
}

FString UMingSaveGame::GetMetadata(const FString& Key, const FString& DefaultValue) const
{
    if (const FString* Value = Header.Metadata.Find(Key))
    {
        return *Value;
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
    // 基本驗證
    if (Header.SaveGameName.IsEmpty())
    {
        return false;
    }

    // 版本驗證
    if (!IsCompatibleVersion())
    {
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

    return Errors;
}

void UMingSaveGame::CompressData()
{
    // 使用 FArchive 壓縮數據
    // 這裡是簡化實現，實際應用會使用更複雜的壓縮算法
    UE_LOG(LogTemp, Log, TEXT("Compressing save game data"));
}

void UMingSaveGame::DecompressData()
{
    UE_LOG(LogTemp, Log, TEXT("Decompressing save game data"));
}

void UMingSaveGame::EncryptData(const FString& EncryptionKey)
{
    // 加密數據
    // 實際實現會使用 AES 或其他加密算法
    UE_LOG(LogTemp, Log, TEXT("Encrypting save game data"));
}

void UMingSaveGame::DecryptData(const FString& EncryptionKey)
{
    UE_LOG(LogTemp, Log, TEXT("Decrypting save game data"));
}

UMingSaveGame* UMingSaveGame::CreateSaveGame(UObject* WorldContextObject, const FString& SaveName, int32 SlotIndex)
{
    UMingSaveGame* SaveGame = Cast<UMingSaveGame>(UGameplayStatics::CreateSaveGameObject(UMingSaveGame::StaticClass()));

    if (SaveGame)
    {
        SaveGame->Header.SaveGameName = SaveName;
        SaveGame->Header.SaveSlotIndex = SlotIndex;
        SaveGame->Header.SaveDateTime = FDateTime::Now();
        SaveGame->Header.Version = EMingSaveGameVersion::Current;
    }

    return SaveGame;
}

FString UMingSaveGame::GetSaveSlotName(int32 SlotIndex)
{
    return FString::Printf(TEXT("Slot_%02d"), SlotIndex);
}

int32 UMingSaveGame::GetMaxSaveSlots()
{
    return 100; // 最大100個存檔槽
}

bool UMingSaveGame::MigrateFromVersion1(const UMingSaveGame* OldSave, UMingSaveGame* NewSave)
{
    if (!OldSave || !NewSave)
    {
        return false;
    }

    // 遷移基本數據
    NewSave->Header = OldSave->Header;
    NewSave->Header.Version = EMingSaveGameVersion::Version_1_1;

    // 遷移關係數據
    NewSave->RelationshipData = OldSave->RelationshipData;
    NewSave->ReputationData = OldSave->ReputationData;
    NewSave->QuestData = OldSave->QuestData;

    // 遷移設置
    NewSave->AudioSettings = OldSave->AudioSettings;
    NewSave->UISettings = OldSave->UISettings;

    // 版本 1.1 新增的功能
    NewSave->GameStateData = OldSave->GameStateData;
    NewSave->PlayerPreferences = OldSave->PlayerPreferences;

    return true;
}

bool UMingSaveGame::MigrateFromVersion2(const UMingSaveGame* OldSave, UMingSaveGame* NewSave)
{
    if (!OldSave || !NewSave)
    {
        return false;
    }

    // 遷移所有數據
    NewSave->Header = OldSave->Header;
    NewSave->Header.Version = EMingSaveGameVersion::Current;

    NewSave->RelationshipData = OldSave->RelationshipData;
    NewSave->ReputationData = OldSave->ReputationData;
    NewSave->QuestData = OldSave->QuestData;
    NewSave->AudioSettings = OldSave->AudioSettings;
    NewSave->UISettings = OldSave->UISettings;
    NewSave->GameStateData = OldSave->GameStateData;
    NewSave->PlayerPreferences = OldSave->PlayerPreferences;

    return true;
}
