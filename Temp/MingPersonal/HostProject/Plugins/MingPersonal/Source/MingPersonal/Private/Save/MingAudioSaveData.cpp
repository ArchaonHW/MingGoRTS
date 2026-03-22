#include "Save/MingAudioSaveData.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"

UMingAudioSaveDataHandler::UMingAudioSaveDataHandler()
{
}

TArray<uint8> UMingAudioSaveDataHandler::SerializeAudioSettings(UMingMetaSoundsSystem* AudioSystem)
{
    TArray<uint8> OutData;
    
    if (!AudioSystem)
    {
        return OutData;
    }
    
    FMingAudioSaveData SaveData = ExportAudioSettings(AudioSystem);
    SerializeToBinary(SaveData, OutData);
    
    return OutData;
}

bool UMingAudioSaveDataHandler::DeserializeAudioSettings(const TArray<uint8>& Data, UMingMetaSoundsSystem* AudioSystem)
{
    if (!AudioSystem || Data.Num() == 0)
    {
        return false;
    }
    
    if (!ValidateAudioData(Data))
    {
        return false;
    }
    
    FMingAudioSaveData SaveData;
    if (!DeserializeFromBinary(Data, SaveData))
    {
        return false;
    }
    
    return ImportAudioSettings(SaveData, AudioSystem);
}

FMingAudioSaveData UMingAudioSaveDataHandler::ExportAudioSettings(UMingMetaSoundsSystem* AudioSystem)
{
    FMingAudioSaveData SaveData;
    SaveData.SaveVersion = 1;
    SaveData.SaveTimestamp = FDateTime::Now();
    
    if (!AudioSystem)
    {
        return SaveData;
    }
    
    BuildAudioSettings(AudioSystem, SaveData);
    
    return SaveData;
}

bool UMingAudioSaveDataHandler::ImportAudioSettings(const FMingAudioSaveData& SaveData, UMingMetaSoundsSystem* AudioSystem)
{
    if (!AudioSystem)
    {
        return false;
    }
    
    if (!ValidateAudioSaveData(SaveData))
    {
        return false;
    }
    
    ApplyAudioSettings(AudioSystem, SaveData);
    return true;
}

bool UMingAudioSaveDataHandler::ValidateAudioData(const TArray<uint8>& Data) const
{
    if (Data.Num() == 0)
    {
        return false;
    }
    
    FMingAudioSaveData TempData;
    if (!DeserializeFromBinary(Data, TempData))
    {
        return false;
    }
    
    return ValidateAudioSaveData(TempData);
}

bool UMingAudioSaveDataHandler::ValidateAudioSaveData(const FMingAudioSaveData& SaveData) const
{
    // Check version
    if (SaveData.SaveVersion != 1)
    {
        return false;
    }
    
    // Check settings array
    if (SaveData.AudioSettings.Num() == 0)
    {
        return false;
    }
    
    // Validate each setting
    for (const FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        if (Entry.Volume < 0.0f || Entry.Volume > 2.0f)
        {
            return false;
        }
    }
    
    return true;
}

void UMingAudioSaveDataHandler::ApplyDefaultSettings(UMingMetaSoundsSystem* AudioSystem)
{
    if (!AudioSystem)
    {
        return;
    }
    
    FMingAudioSaveData DefaultSettings = GetDefaultAudioSettings();
    ApplyAudioSettings(AudioSystem, DefaultSettings);
}

void UMingAudioSaveDataHandler::ApplyRepublicEraDefaults(UMingMetaSoundsSystem* AudioSystem)
{
    if (!AudioSystem)
    {
        return;
    }
    
    FMingAudioSaveData EraSettings = GetRepublicEraAudioSettings();
    ApplyAudioSettings(AudioSystem, EraSettings);
}

bool UMingAudioSaveDataHandler::AreSettingsEqual(const FMingAudioSaveData& DataA, const FMingAudioSaveData& DataB) const
{
    if (DataA.AudioSettings.Num() != DataB.AudioSettings.Num())
    {
        return false;
    }
    
    for (int32 i = 0; i < DataA.AudioSettings.Num(); i++)
    {
        if (DataA.AudioSettings[i].Volume != DataB.AudioSettings[i].Volume ||
            DataA.AudioSettings[i].bMuted != DataB.AudioSettings[i].bMuted)
        {
            return false;
        }
    }
    
    return true;
}

int32 UMingAudioSaveDataHandler::GetSerializedDataSize(const UMingMetaSoundsSystem* AudioSystem) const
{
    return sizeof(FMingAudioSaveData);
}

FString UMingAudioSaveDataHandler::GetSaveDataChecksum(const TArray<uint8>& Data) const
{
    FMD5Hash Hash = FMD5Hash::HashData(Data.GetData(), Data.Num());
    return Hash.ToString();
}

float UMingAudioSaveDataHandler::GetCategoryVolume(const FMingAudioSaveData& SaveData, EMingAudioCategory Category) const
{
    for (const FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        if (Entry.Category == Category)
        {
            return Entry.bMuted ? 0.0f : Entry.Volume;
        }
    }
    return 1.0f;
}

void UMingAudioSaveDataHandler::SetCategoryVolume(FMingAudioSaveData& SaveData, EMingAudioCategory Category, float Volume)
{
    for (FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        if (Entry.Category == Category)
        {
            Entry.Volume = FMath::Clamp(Volume, 0.0f, 2.0f);
            return;
        }
    }
    
    // Create new entry if not found
    FMingAudioSettingsEntry NewEntry;
    NewEntry.Category = Category;
    NewEntry.Volume = FMath::Clamp(Volume, 0.0f, 2.0f);
    SaveData.AudioSettings.Add(NewEntry);
}

bool UMingAudioSaveDataHandler::IsCategoryMuted(const FMingAudioSaveData& SaveData, EMingAudioCategory Category) const
{
    for (const FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        if (Entry.Category == Category)
        {
            return Entry.bMuted;
        }
    }
    return false;
}

void UMingAudioSaveDataHandler::SetCategoryMuted(FMingAudioSaveData& SaveData, EMingAudioCategory Category, bool bMuted)
{
    for (FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        if (Entry.Category == Category)
        {
            Entry.bMuted = bMuted;
            return;
        }
    }
}

bool UMingAudioSaveDataHandler::ExportToJson(const FMingAudioSaveData& SaveData, FString& OutJson)
{
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    
    RootObject->SetNumberField(TEXT("version"), SaveData.SaveVersion);
    RootObject->SetStringField(TEXT("timestamp"), SaveData.SaveTimestamp.ToIso8601());
    RootObject->SetBoolField(TEXT("was_playing"), SaveData.bWasPlayingAudio);
    
    // Audio settings
    TArray<TSharedPtr<FJsonValue>> SettingsArray;
    for (const FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        TSharedPtr<FJsonObject> EntryObject = MakeShared<FJsonObject>();
        EntryObject->SetStringField(TEXT("category"), AudioCategoryToString(Entry.Category));
        EntryObject->SetNumberField(TEXT("volume"), Entry.Volume);
        EntryObject->SetBoolField(TEXT("muted"), Entry.bMuted);
        EntryObject->SetBoolField(TEXT("enabled"), Entry.bEnabled);
        
        SettingsArray.Add(MakeShared<FJsonValueObject>(EntryObject));
    }
    RootObject->SetArrayField(TEXT("settings"), SettingsArray);
    
    // Preferences
    TSharedPtr<FJsonObject> PrefsObject = MakeShared<FJsonObject>();
    PrefsObject->SetBoolField(TEXT("subtitles_enabled"), SaveData.AudioPreferences.bSubtitlesEnabled);
    PrefsObject->SetBoolField(TEXT("subtitle_background"), SaveData.AudioPreferences.bSubtitleBackground);
    PrefsObject->SetNumberField(TEXT("subtitle_size"), SaveData.AudioPreferences.SubtitleSize);
    PrefsObject->SetStringField(TEXT("language"), SaveData.AudioPreferences.Language);
    PrefsObject->SetBoolField(TEXT("spatial_audio"), SaveData.AudioPreferences.bSpatialAudioEnabled);
    RootObject->SetObjectField(TEXT("preferences"), PrefsObject);
    
    // Republic Era preferences
    TSharedPtr<FJsonObject> EraObject = MakeShared<FJsonObject>();
    EraObject->SetBoolField(TEXT("cultural_music"), SaveData.RepublicEraPreferences.bEnableCulturalMusic);
    EraObject->SetBoolField(TEXT("regional_accents"), SaveData.RepublicEraPreferences.bEnableRegionalAccents);
    EraObject->SetNumberField(TEXT("traditional_volume"), SaveData.RepublicEraPreferences.TraditionalInstrumentVolume);
    RootObject->SetObjectField(TEXT("republic_era"), EraObject);
    
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJson);
    return FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
}

bool UMingAudioSaveDataHandler::ImportFromJson(const FString& Json, FMingAudioSaveData& OutSaveData)
{
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
    
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        return false;
    }
    
    OutSaveData.SaveVersion = RootObject->GetIntegerField(TEXT("version"));
    OutSaveData.bWasPlayingAudio = RootObject->GetBoolField(TEXT("was_playing"));
    
    FString TimestampStr;
    if (RootObject->TryGetStringField(TEXT("timestamp"), TimestampStr))
    {
        FDateTime::ParseIso8601(*TimestampStr, OutSaveData.SaveTimestamp);
    }
    
    const TArray<TSharedPtr<FJsonValue>>* SettingsArray;
    if (RootObject->TryGetArrayField(TEXT("settings"), SettingsArray))
    {
        for (const TSharedPtr<FJsonValue>& EntryValue : *SettingsArray)
        {
            TSharedPtr<FJsonObject> EntryObject = EntryValue->AsObject();
            if (EntryObject.IsValid())
            {
                FMingAudioSettingsEntry Entry;
                FString CategoryStr = EntryObject->GetStringField(TEXT("category"));
                Entry.Category = AudioCategoryFromString(CategoryStr);
                Entry.Volume = EntryObject->GetNumberField(TEXT("volume"));
                Entry.bMuted = EntryObject->GetBoolField(TEXT("muted"));
                Entry.bEnabled = EntryObject->GetBoolField(TEXT("enabled"));
                
                OutSaveData.AudioSettings.Add(Entry);
            }
        }
    }
    
    TSharedPtr<FJsonObject> PrefsObject;
    if (RootObject->TryGetObjectField(TEXT("preferences"), PrefsObject))
    {
        OutSaveData.AudioPreferences.bSubtitlesEnabled = PrefsObject->GetBoolField(TEXT("subtitles_enabled"));
        OutSaveData.AudioPreferences.bSubtitleBackground = PrefsObject->GetBoolField(TEXT("subtitle_background"));
        OutSaveData.AudioPreferences.SubtitleSize = PrefsObject->GetNumberField(TEXT("subtitle_size"));
        OutSaveData.AudioPreferences.Language = PrefsObject->GetStringField(TEXT("language"));
        OutSaveData.AudioPreferences.bSpatialAudioEnabled = PrefsObject->GetBoolField(TEXT("spatial_audio"));
    }
    
    TSharedPtr<FJsonObject> EraObject;
    if (RootObject->TryGetObjectField(TEXT("republic_era"), EraObject))
    {
        OutSaveData.RepublicEraPreferences.bEnableCulturalMusic = EraObject->GetBoolField(TEXT("cultural_music"));
        OutSaveData.RepublicEraPreferences.bEnableRegionalAccents = EraObject->GetBoolField(TEXT("regional_accents"));
        OutSaveData.RepublicEraPreferences.TraditionalInstrumentVolume = EraObject->GetNumberField(TEXT("traditional_volume"));
    }
    
    return true;
}

bool UMingAudioSaveDataHandler::ExportToFile(const FMingAudioSaveData& SaveData, const FString& FilePath)
{
    TArray<uint8> BinaryData;
    SerializeToBinary(SaveData, BinaryData);
    return FFileHelper::SaveArrayToFile(BinaryData, *FilePath);
}

bool UMingAudioSaveDataHandler::ImportFromFile(const FString& FilePath, FMingAudioSaveData& OutSaveData)
{
    TArray<uint8> BinaryData;
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
        return false;
    }
    
    return DeserializeFromBinary(BinaryData, OutSaveData);
}

void UMingAudioSaveDataHandler::BuildAudioSettings(UMingMetaSoundsSystem* AudioSystem, FMingAudioSaveData& OutSaveData)
{
    // Create entries for all categories
    for (int32 i = 0; i <= static_cast<int32>(EMingAudioCategory::UI); i++)
    {
        EMingAudioCategory Category = static_cast<EMingAudioCategory>(i);
        FMingAudioSettingsEntry Entry = CreateDefaultEntry(Category);
        
        // Get actual values from audio system
        // Entry.Volume = AudioSystem->GetCategoryVolume(Category);
        // Entry.bMuted = AudioSystem->IsCategoryMuted(Category);
        
        OutSaveData.AudioSettings.Add(Entry);
    }
    
    // Save preferences
    // OutSaveData.AudioPreferences.Language = AudioSystem->GetLanguage();
    // OutSaveData.AudioPreferences.bSubtitlesEnabled = AudioSystem->AreSubtitlesEnabled();
    
    // Save Republic Era preferences
    // OutSaveData.RepublicEraPreferences.bEnableCulturalMusic = AudioSystem->IsCulturalMusicEnabled();
}

void UMingAudioSaveDataHandler::ApplyAudioSettings(UMingMetaSoundsSystem* AudioSystem, const FMingAudioSaveData& SaveData)
{
    // Apply each category setting
    for (const FMingAudioSettingsEntry& Entry : SaveData.AudioSettings)
    {
        // AudioSystem->SetCategoryVolume(Entry.Category, Entry.bMuted ? 0.0f : Entry.Volume);
        // AudioSystem->SetCategoryMuted(Entry.Category, Entry.bMuted);
    }
    
    // Apply preferences
    // AudioSystem->SetLanguage(SaveData.AudioPreferences.Language);
    // AudioSystem->EnableSubtitles(SaveData.AudioPreferences.bSubtitlesEnabled);
    
    // Apply Republic Era preferences
    // AudioSystem->EnableCulturalMusic(SaveData.RepublicEraPreferences.bEnableCulturalMusic);
}

void UMingAudioSaveDataHandler::SerializeToBinary(const FMingAudioSaveData& SaveData, TArray<uint8>& OutData)
{
    FMemoryWriter MemoryWriter(OutData, true);
    MemoryWriter << const_cast<FMingAudioSaveData&>(SaveData);
}

bool UMingAudioSaveDataHandler::DeserializeFromBinary(const TArray<uint8>& Data, FMingAudioSaveData& OutSaveData)
{
    if (Data.Num() == 0)
    {
        return false;
    }
    
    FMemoryReader MemoryReader(Data, true);
    MemoryReader << OutSaveData;
    
    return !MemoryReader.IsError();
}

EMingAudioCategory UMingAudioSaveDataHandler::AudioCategoryFromString(const FString& CategoryName) const
{
    if (CategoryName == TEXT("Master")) return EMingAudioCategory::Master;
    if (CategoryName == TEXT("Music")) return EMingAudioCategory::Music;
    if (CategoryName == TEXT("SFX")) return EMingAudioCategory::SFX;
    if (CategoryName == TEXT("Dialogue")) return EMingAudioCategory::Dialogue;
    if (CategoryName == TEXT("Ambient")) return EMingAudioCategory::Ambient;
    if (CategoryName == TEXT("UI")) return EMingAudioCategory::UI;
    return EMingAudioCategory::Master;
}

FString UMingAudioSaveDataHandler::AudioCategoryToString(EMingAudioCategory Category) const
{
    switch (Category)
    {
    case EMingAudioCategory::Master: return TEXT("Master");
    case EMingAudioCategory::Music: return TEXT("Music");
    case EMingAudioCategory::SFX: return TEXT("SFX");
    case EMingAudioCategory::Dialogue: return TEXT("Dialogue");
    case EMingAudioCategory::Ambient: return TEXT("Ambient");
    case EMingAudioCategory::UI: return TEXT("UI");
    default: return TEXT("Master");
    }
}

FMingAudioSettingsEntry UMingAudioSaveDataHandler::CreateDefaultEntry(EMingAudioCategory Category) const
{
    FMingAudioSettingsEntry Entry;
    Entry.Category = Category;
    
    switch (Category)
    {
    case EMingAudioCategory::Master:
        Entry.Volume = 1.0f;
        break;
    case EMingAudioCategory::Music:
        Entry.Volume = 0.8f;
        break;
    case EMingAudioCategory::SFX:
        Entry.Volume = 1.0f;
        break;
    case EMingAudioCategory::Dialogue:
        Entry.Volume = 1.0f;
        break;
    case EMingAudioCategory::Ambient:
        Entry.Volume = 0.6f;
        break;
    case EMingAudioCategory::UI:
        Entry.Volume = 0.8f;
        break;
    default:
        Entry.Volume = 1.0f;
        break;
    }
    
    return Entry;
}

FMingAudioSaveData UMingAudioSaveDataHandler::GetDefaultAudioSettings()
{
    FMingAudioSaveData SaveData;
    SaveData.SaveVersion = 1;
    SaveData.SaveTimestamp = FDateTime::Now();
    
    // Add all categories with default values
    UMingAudioSaveDataHandler* Handler = NewObject<UMingAudioSaveDataHandler>();
    for (int32 i = 0; i <= static_cast<int32>(EMingAudioCategory::UI); i++)
    {
        EMingAudioCategory Category = static_cast<EMingAudioCategory>(i);
        SaveData.AudioSettings.Add(Handler->CreateDefaultEntry(Category));
    }
    
    return SaveData;
}

FMingAudioSaveData UMingAudioSaveDataHandler::GetRepublicEraAudioSettings()
{
    FMingAudioSaveData SaveData = GetDefaultAudioSettings();
    
    // Republic Era specific settings
    SaveData.RepublicEraPreferences.bEnableCulturalMusic = true;
    SaveData.RepublicEraPreferences.bEnableRegionalAccents = true;
    SaveData.RepublicEraPreferences.TraditionalInstrumentVolume = 0.9f;
    SaveData.RepublicEraPreferences.bUsePeriodAppropriateAudio = true;
    
    return SaveData;
}
