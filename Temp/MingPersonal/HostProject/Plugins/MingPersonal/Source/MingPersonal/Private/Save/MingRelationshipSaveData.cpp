#include "Save/MingRelationshipSaveData.h"
#include "Relationship/MingRelationshipManager.h"
#include "Engine/Engine.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UMingRelationshipSaveDataHandler::UMingRelationshipSaveDataHandler()
{
}

TArray<uint8> UMingRelationshipSaveDataHandler::SerializeRelationshipData(UMingRelationshipManager* RelationshipManager)
{
    TArray<uint8> OutData;
    
    if (!RelationshipManager)
    {
        return OutData;
    }
    
    // Export data to struct
    FMingRelationshipSaveData SaveData = ExportRelationshipData(RelationshipManager);
    
    // Serialize to binary
    SerializeToBinary(SaveData, OutData);
    
    return OutData;
}

bool UMingRelationshipSaveDataHandler::DeserializeRelationshipData(const TArray<uint8>& Data, UMingRelationshipManager* RelationshipManager)
{
    if (!RelationshipManager || Data.Num() == 0)
    {
        return false;
    }
    
    // Validate first
    if (!ValidateRelationshipData(Data))
    {
        return false;
    }
    
    // Deserialize from binary
    FMingRelationshipSaveData SaveData;
    if (!DeserializeFromBinary(Data, SaveData))
    {
        return false;
    }
    
    // Check version and migrate if needed
    if (SaveData.SaveVersion < GetCurrentSaveVersion())
    {
        if (!MigrateDataToCurrentVersion(SaveData))
        {
            return false;
        }
    }
    
    // Import data
    return ImportRelationshipData(SaveData, RelationshipManager);
}

FMingRelationshipSaveData UMingRelationshipSaveDataHandler::ExportRelationshipData(UMingRelationshipManager* RelationshipManager)
{
    FMingRelationshipSaveData SaveData;
    SaveData.SaveVersion = GetCurrentSaveVersion();
    SaveData.SaveTimestamp = FDateTime::Now();
    
    if (!RelationshipManager)
    {
        return SaveData;
    }
    
    // Get all character IDs
    TArray<FName> CharacterIDs = RelationshipManager->GetAllCharacterIDs();
    
    // Build entries for each character
    for (const FName& CharacterID : CharacterIDs)
    {
        FMingRelationshipEntry Entry;
        BuildRelationshipEntry(RelationshipManager, CharacterID, Entry);
        SaveData.RelationshipEntries.Add(Entry);
        
        // Update cache
        SaveData.RelationshipCache.Add(CharacterID, Entry.RelationshipValue);
        
        // Track quest givers
        if (Entry.bIsActiveQuestGiver)
        {
            SaveData.ActiveQuestGivers.Add(CharacterID.ToString());
        }
        
        // Update counts by type
        EMingRelationshipType Type = Entry.RelationshipType;
        FString TypeKey = FString::Printf(TEXT("Type_%d"), static_cast<int32>(Type));
        if (SaveData.RelationshipCountsByType.Contains(TypeKey))
        {
            SaveData.RelationshipCountsByType[TypeKey]++;
        }
        else
        {
            SaveData.RelationshipCountsByType.Add(TypeKey, 1);
        }
    }
    
    SaveData.TotalRelationships = SaveData.RelationshipEntries.Num();
    
    return SaveData;
}

bool UMingRelationshipSaveDataHandler::ImportRelationshipData(const FMingRelationshipSaveData& SaveData, UMingRelationshipManager* RelationshipManager)
{
    if (!RelationshipManager)
    {
        return false;
    }
    
    // Validate save data
    if (!ValidateRelationshipSaveData(SaveData))
    {
        return false;
    }
    
    // Clear existing data
    RelationshipManager->ClearAllRelationships();
    
    // Import each relationship entry
    for (const FMingRelationshipEntry& Entry : SaveData.RelationshipEntries)
    {
        ApplyRelationshipEntry(RelationshipManager, Entry);
    }
    
    return true;
}

bool UMingRelationshipSaveDataHandler::ValidateRelationshipData(const TArray<uint8>& Data) const
{
    ClearValidationErrors();
    
    if (Data.Num() == 0)
    {
        AddValidationError(TEXT("Data is empty"));
        return false;
    }
    
    // Check minimum size for header
    if (Data.Num() < sizeof(int32) * 2)
    {
        AddValidationError(TEXT("Data too small for header"));
        return false;
    }
    
    // Try to deserialize to validate structure
    FMingRelationshipSaveData TempData;
    if (!DeserializeFromBinary(Data, TempData))
    {
        AddValidationError(TEXT("Failed to deserialize data"));
        return false;
    }
    
    return ValidateRelationshipSaveData(TempData);
}

bool UMingRelationshipSaveDataHandler::ValidateRelationshipSaveData(const FMingRelationshipSaveData& SaveData) const
{
    ClearValidationErrors();
    
    bool bValid = true;
    
    // Check version
    if (SaveData.SaveVersion > GetCurrentSaveVersion())
    {
        AddValidationError(FString::Printf(TEXT("Version %d is newer than current version %d"),
            SaveData.SaveVersion, GetCurrentSaveVersion()));
        bValid = false;
    }
    
    // Check entries
    if (SaveData.RelationshipEntries.Num() == 0)
    {
        AddValidationError(TEXT("No relationship entries"));
        bValid = false;
    }
    
    // Validate each entry
    for (const FMingRelationshipEntry& Entry : SaveData.RelationshipEntries)
    {
        if (Entry.CharacterID.IsNone())
        {
            AddValidationError(TEXT("Entry has invalid CharacterID"));
            bValid = false;
        }
        
        if (Entry.RelationshipValue < 0.0f || Entry.RelationshipValue > 100.0f)
        {
            AddValidationError(FString::Printf(TEXT("Invalid relationship value %.2f for %s"),
                Entry.RelationshipValue, *Entry.CharacterID.ToString()));
            bValid = false;
        }
    }
    
    // Check cache consistency
    if (SaveData.RelationshipCache.Num() != SaveData.RelationshipEntries.Num())
    {
        AddValidationError(TEXT("Cache size mismatch with entries"));
        bValid = false;
    }
    
    // Check totals
    if (SaveData.TotalRelationships != SaveData.RelationshipEntries.Num())
    {
        AddValidationError(FString::Printf(TEXT("Total count mismatch: %d vs %d"),
            SaveData.TotalRelationships, SaveData.RelationshipEntries.Num()));
        bValid = false;
    }
    
    return bValid;
}

TArray<FString> UMingRelationshipSaveDataHandler::GetValidationErrors() const
{
    return ValidationErrors;
}

bool UMingRelationshipSaveDataHandler::MigrateDataToCurrentVersion(FMingRelationshipSaveData& SaveData) const
{
    // Version migration logic
    // Currently only version 1 exists, so no migration needed
    if (SaveData.SaveVersion == GetCurrentSaveVersion())
    {
        return true;
    }
    
    // Future migrations would go here
    // e.g., if (SaveData.SaveVersion == 1) { MigrateV1ToV2(SaveData); }
    
    SaveData.SaveVersion = GetCurrentSaveVersion();
    return true;
}

int32 UMingRelationshipSaveDataHandler::GetSerializedDataSize(const UMingRelationshipManager* RelationshipManager) const
{
    if (!RelationshipManager)
    {
        return 0;
    }
    
    // Estimate size based on relationship count
    int32 RelationshipCount = RelationshipManager->GetAllCharacterIDs().Num();
    
    // Base header + entry size * count
    int32 EstimatedSize = sizeof(FMingRelationshipSaveData) + (sizeof(FMingRelationshipEntry) * RelationshipCount);
    
    return EstimatedSize;
}

FString UMingRelationshipSaveDataHandler::GetSaveDataChecksum(const TArray<uint8>& Data) const
{
    // Calculate MD5 hash of data
    FMD5Hash Hash = FMD5Hash::HashData(Data.GetData(), Data.Num());
    return Hash.ToString();
}

TArray<uint8> UMingRelationshipSaveDataHandler::CompressRelationshipData(const TArray<uint8>& Data) const
{
    // Compression would be implemented here using Oodle or zlib
    // For now, return original data
    return Data;
}

TArray<uint8> UMingRelationshipSaveDataHandler::DecompressRelationshipData(const TArray<uint8>& CompressedData) const
{
    // Decompression would be implemented here
    return CompressedData;
}

bool UMingRelationshipSaveDataHandler::CompareRelationshipData(const FMingRelationshipSaveData& DataA, const FMingRelationshipSaveData& DataB) const
{
    // Quick checks first
    if (DataA.SaveVersion != DataB.SaveVersion)
    {
        return false;
    }
    
    if (DataA.TotalRelationships != DataB.TotalRelationships)
    {
        return false;
    }
    
    if (DataA.RelationshipEntries.Num() != DataB.RelationshipEntries.Num())
    {
        return false;
    }
    
    // Compare each entry
    for (const FMingRelationshipEntry& EntryA : DataA.RelationshipEntries)
    {
        bool bFoundMatch = false;
        
        for (const FMingRelationshipEntry& EntryB : DataB.RelationshipEntries)
        {
            if (EntryA.CharacterID == EntryB.CharacterID)
            {
                if (EntryA.RelationshipValue != EntryB.RelationshipValue ||
                    EntryA.RelationshipType != EntryB.RelationshipType ||
                    EntryA.RelationshipLevel != EntryB.RelationshipLevel)
                {
                    return false;
                }
                bFoundMatch = true;
                break;
            }
        }
        
        if (!bFoundMatch)
        {
            return false;
        }
    }
    
    return true;
}

TArray<FString> UMingRelationshipSaveDataHandler::GetDifferences(const FMingRelationshipSaveData& DataA, const FMingRelationshipSaveData& DataB) const
{
    TArray<FString> Differences;
    
    if (DataA.SaveVersion != DataB.SaveVersion)
    {
        Differences.Add(FString::Printf(TEXT("Version: %d vs %d"), DataA.SaveVersion, DataB.SaveVersion));
    }
    
    if (DataA.TotalRelationships != DataB.TotalRelationships)
    {
        Differences.Add(FString::Printf(TEXT("Count: %d vs %d"), DataA.TotalRelationships, DataB.TotalRelationships));
    }
    
    // Find added/removed/changed entries
    TSet<FName> IDsA;
    TSet<FName> IDsB;
    
    for (const FMingRelationshipEntry& Entry : DataA.RelationshipEntries)
    {
        IDsA.Add(Entry.CharacterID);
    }
    
    for (const FMingRelationshipEntry& Entry : DataB.RelationshipEntries)
    {
        IDsB.Add(Entry.CharacterID);
    }
    
    // Find added
    for (FName ID : IDsB)
    {
        if (!IDsA.Contains(ID))
        {
            Differences.Add(FString::Printf(TEXT("Added: %s"), *ID.ToString()));
        }
    }
    
    // Find removed
    for (FName ID : IDsA)
    {
        if (!IDsB.Contains(ID))
        {
            Differences.Add(FString::Printf(TEXT("Removed: %s"), *ID.ToString()));
        }
    }
    
    // Find changed
    for (const FMingRelationshipEntry& EntryA : DataA.RelationshipEntries)
    {
        for (const FMingRelationshipEntry& EntryB : DataB.RelationshipEntries)
        {
            if (EntryA.CharacterID == EntryB.CharacterID)
            {
                if (EntryA.RelationshipValue != EntryB.RelationshipValue)
                {
                    Differences.Add(FString::Printf(TEXT("Changed: %s (%.2f -> %.2f)"),
                        *EntryA.CharacterID.ToString(), EntryA.RelationshipValue, EntryB.RelationshipValue));
                }
                break;
            }
        }
    }
    
    return Differences;
}

int32 UMingRelationshipSaveDataHandler::GetRelationshipCount(const FMingRelationshipSaveData& SaveData) const
{
    return SaveData.RelationshipEntries.Num();
}

float UMingRelationshipSaveDataHandler::GetAverageRelationshipValue(const FMingRelationshipSaveData& SaveData) const
{
    if (SaveData.RelationshipEntries.Num() == 0)
    {
        return 0.0f;
    }
    
    float Total = 0.0f;
    for (const FMingRelationshipEntry& Entry : SaveData.RelationshipEntries)
    {
        Total += Entry.RelationshipValue;
    }
    
    return Total / SaveData.RelationshipEntries.Num();
}

TMap<EMingRelationshipType, int32> UMingRelationshipSaveDataHandler::GetRelationshipDistribution(const FMingRelationshipSaveData& SaveData) const
{
    TMap<EMingRelationshipType, int32> Distribution;
    
    for (const FMingRelationshipEntry& Entry : SaveData.RelationshipEntries)
    {
        EMingRelationshipType Type = Entry.RelationshipType;
        if (Distribution.Contains(Type))
        {
            Distribution[Type]++;
        }
        else
        {
            Distribution.Add(Type, 1);
        }
    }
    
    return Distribution;
}

bool UMingRelationshipSaveDataHandler::ExportToJson(const FMingRelationshipSaveData& SaveData, FString& OutJson)
{
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    
    // Add version info
    RootObject->SetNumberField(TEXT("version"), SaveData.SaveVersion);
    RootObject->SetStringField(TEXT("timestamp"), SaveData.SaveTimestamp.ToIso8601());
    RootObject->SetNumberField(TEXT("total_relationships"), SaveData.TotalRelationships);
    
    // Add entries
    TArray<TSharedPtr<FJsonValue>> EntriesArray;
    for (const FMingRelationshipEntry& Entry : SaveData.RelationshipEntries)
    {
        TSharedPtr<FJsonObject> EntryObject = MakeShared<FJsonObject>();
        EntryObject->SetStringField(TEXT("character_id"), Entry.CharacterID.ToString());
        EntryObject->SetNumberField(TEXT("relationship_value"), Entry.RelationshipValue);
        EntryObject->SetNumberField(TEXT("relationship_type"), static_cast<int32>(Entry.RelationshipType));
        EntryObject->SetNumberField(TEXT("relationship_level"), Entry.RelationshipLevel);
        EntryObject->SetBoolField(TEXT("is_quest_giver"), Entry.bIsActiveQuestGiver);
        EntryObject->SetBoolField(TEXT("can_interact"), Entry.bCanInteract);
        
        EntriesArray.Add(MakeShared<FJsonValueObject>(EntryObject));
    }
    RootObject->SetArrayField(TEXT("entries"), EntriesArray);
    
    // Serialize to string
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJson);
    bool bSuccess = FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return bSuccess;
}

bool UMingRelationshipSaveDataHandler::ImportFromJson(const FString& Json, FMingRelationshipSaveData& OutSaveData)
{
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
    
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        return false;
    }
    
    OutSaveData.SaveVersion = RootObject->GetIntegerField(TEXT("version"));
    OutSaveData.TotalRelationships = RootObject->GetIntegerField(TEXT("total_relationships"));
    
    FString TimestampStr;
    if (RootObject->TryGetStringField(TEXT("timestamp"), TimestampStr))
    {
        FDateTime::ParseIso8601(*TimestampStr, OutSaveData.SaveTimestamp);
    }
    
    const TArray<TSharedPtr<FJsonValue>>* EntriesArray;
    if (RootObject->TryGetArrayField(TEXT("entries"), EntriesArray))
    {
        for (const TSharedPtr<FJsonValue>& EntryValue : *EntriesArray)
        {
            TSharedPtr<FJsonObject> EntryObject = EntryValue->AsObject();
            if (EntryObject.IsValid())
            {
                FMingRelationshipEntry Entry;
                Entry.CharacterID = FName(*EntryObject->GetStringField(TEXT("character_id")));
                Entry.RelationshipValue = EntryObject->GetNumberField(TEXT("relationship_value"));
                Entry.RelationshipType = static_cast<EMingRelationshipType>(
                    static_cast<int32>(EntryObject->GetNumberField(TEXT("relationship_type"))));
                Entry.RelationshipLevel = EntryObject->GetIntegerField(TEXT("relationship_level"));
                Entry.bIsActiveQuestGiver = EntryObject->GetBoolField(TEXT("is_quest_giver"));
                Entry.bCanInteract = EntryObject->GetBoolField(TEXT("can_interact"));
                
                OutSaveData.RelationshipEntries.Add(Entry);
            }
        }
    }
    
    return true;
}

bool UMingRelationshipSaveDataHandler::ExportToFile(const FMingRelationshipSaveData& SaveData, const FString& FilePath)
{
    TArray<uint8> BinaryData;
    SerializeToBinary(SaveData, BinaryData);
    
    return FFileHelper::SaveArrayToFile(BinaryData, *FilePath);
}

bool UMingRelationshipSaveDataHandler::ImportFromFile(const FString& FilePath, FMingRelationshipSaveData& OutSaveData)
{
    TArray<uint8> BinaryData;
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
        return false;
    }
    
    return DeserializeFromBinary(BinaryData, OutSaveData);
}

void UMingRelationshipSaveDataHandler::BuildRelationshipEntry(UMingRelationshipManager* Manager, FName CharacterID, FMingRelationshipEntry& OutEntry)
{
    OutEntry.CharacterID = CharacterID;
    
    if (!Manager)
    {
        return;
    }
    
    OutEntry.RelationshipValue = Manager->GetRelationshipValue(CharacterID);
    OutEntry.RelationshipType = Manager->GetRelationshipType(CharacterID);
    OutEntry.RelationshipLevel = Manager->GetRelationshipLevel(CharacterID);
    OutEntry.bIsActiveQuestGiver = Manager->HasActiveQuest(CharacterID);
    OutEntry.bCanInteract = Manager->CanInteract(CharacterID);
    OutEntry.LastInteractionTime = Manager->GetLastInteractionTime(CharacterID);
    
    // Get interaction history (if available)
    // OutEntry.InteractionHistory = Manager->GetInteractionHistory(CharacterID);
    
    // Get modifiers
    // OutEntry.RelationshipModifiers = Manager->GetRelationshipModifiers(CharacterID);
}

void UMingRelationshipSaveDataHandler::ApplyRelationshipEntry(UMingRelationshipManager* Manager, const FMingRelationshipEntry& Entry)
{
    if (!Manager || Entry.CharacterID.IsNone())
    {
        return;
    }
    
    // Set relationship data
    Manager->SetRelationshipValue(Entry.CharacterID, Entry.RelationshipValue);
    Manager->SetRelationshipType(Entry.CharacterID, Entry.RelationshipType);
    Manager->SetRelationshipLevel(Entry.CharacterID, Entry.RelationshipLevel);
    
    // Restore last interaction time
    // Manager->SetLastInteractionTime(Entry.CharacterID, Entry.LastInteractionTime);
    
    // Restore modifiers
    // for (const auto& Modifier : Entry.RelationshipModifiers)
    // {
    //     Manager->SetRelationshipModifier(Entry.CharacterID, FName(*Modifier.Key), Modifier.Value);
    // }
}

void UMingRelationshipSaveDataHandler::SerializeToBinary(const FMingRelationshipSaveData& SaveData, TArray<uint8>& OutData)
{
    FMemoryWriter MemoryWriter(OutData, true);
    MemoryWriter << const_cast<FMingRelationshipSaveData&>(SaveData);
}

bool UMingRelationshipSaveDataHandler::DeserializeFromBinary(const TArray<uint8>& Data, FMingRelationshipSaveData& OutSaveData)
{
    if (Data.Num() == 0)
    {
        return false;
    }
    
    FMemoryReader MemoryReader(Data, true);
    MemoryReader << OutSaveData;
    
    return !MemoryReader.IsError();
}

void UMingRelationshipSaveDataHandler::ClearValidationErrors()
{
    ValidationErrors.Empty();
}

void UMingRelationshipSaveDataHandler::AddValidationError(const FString& Error)
{
    ValidationErrors.Add(Error);
}
