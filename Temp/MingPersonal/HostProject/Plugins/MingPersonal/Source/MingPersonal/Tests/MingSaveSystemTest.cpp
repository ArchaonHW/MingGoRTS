#include "MingPersonal.h"
#include "Misc/AutomationTest.h"
#include "Save/MingSaveGame.h"
#include "Save/MingSaveGameManager.h"
#include "Save/MingRelationshipSaveData.h"
#include "Save/MingReputationSaveData.h"
#include "Save/MingAudioSaveData.h"
#include "Relationship/MingRelationshipManager.h"
#include "Reputation/MingReputationManager.h"
#include "Engine/World.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameBasicTest, "MingGoRTS.SaveSystem.Basic",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameBasicTest::RunTest(const FString& Parameters)
{
    // Test save game creation
    {
        UMingSaveGame* SaveGame = NewObject<UMingSaveGame>();
        TestNotNull(TEXT("SaveGame should be created"), SaveGame);
        
        if (SaveGame)
        {
            // Test default values
            TestEqual(TEXT("Default version should be 1"), SaveGame->GetCurrentSaveVersion(), 1);
            TestTrue(TEXT("Should have valid creation time"), SaveGame->GetCreationTime().IsValid());
            TestTrue(TEXT("Should have valid save time"), SaveGame->GetLastSaveTime().IsValid());
        }
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameManagerTest, "MingGoRTS.SaveSystem.Manager",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameManagerTest::RunTest(const FString& Parameters)
{
    UMingSaveGameManager* SaveManager = NewObject<UMingSaveGameManager>();
    TestNotNull(TEXT("SaveManager should be created"), SaveManager);
    
    if (SaveManager)
    {
        // Test initialization
        SaveManager->Initialize();
        TestEqual(TEXT("Max save slots should be 20"), SaveManager->GetMaxSaveSlots(), 20);
        TestEqual(TEXT("Autosave slot should be 0"), SaveManager->GetAutosaveSlot(), 0);
        TestEqual(TEXT("Quick save slot should be 1"), SaveManager->GetQuickSaveSlot(), 1);
        
        // Test autosave settings
        TestTrue(TEXT("Autosave should be enabled by default"), SaveManager->IsAutosaveEnabled());
        TestEqual(TEXT("Autosave interval should be 300 seconds"), SaveManager->GetAutosaveInterval(), 300.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameSlotManagementTest, "MingGoRTS.SaveSystem.SlotManagement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameSlotManagementTest::RunTest(const FString& Parameters)
{
    UMingSaveGameManager* SaveManager = NewObject<UMingSaveGameManager>();
    if (SaveManager)
    {
        SaveManager->Initialize();
        
        // Test empty slot detection
        FMingSaveGameSlotInfo EmptySlot;
        TestTrue(TEXT("Empty slot should be detected"), EmptySlot.bIsEmpty);
        
        // Test slot info
        FMingSaveGameSlotInfo FilledSlot;
        FilledSlot.SlotIndex = 5;
        FilledSlot.SaveName = TEXT("Test Save");
        FilledSlot.bIsEmpty = false;
        FilledSlot.PlayTimeSeconds = 3600;
        
        TestEqual(TEXT("Slot index should be 5"), FilledSlot.SlotIndex, 5);
        TestEqual(TEXT("Save name should match"), FilledSlot.SaveName, FString(TEXT("Test Save")));
        TestFalse(TEXT("Should not be empty"), FilledSlot.bIsEmpty);
        
        // Test play time formatting
        FString PlayTimeStr = FString::Printf(TEXT("%dh %dm"), FilledSlot.PlayTimeSeconds / 3600, (FilledSlot.PlayTimeSeconds % 3600) / 60);
        TestEqual(TEXT("Play time string should be 1h 0m"), PlayTimeStr, FString(TEXT("1h 0m")));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingRelationshipSaveDataTest, "MingGoRTS.SaveSystem.Relationship",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingRelationshipSaveDataTest::RunTest(const FString& Parameters)
{
    UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
    TestNotNull(TEXT("Relationship save handler should be created"), Handler);
    
    if (Handler)
    {
        // Test data structures
        FMingRelationshipEntry Entry;
        Entry.CharacterID = FName(TEXT("TestCharacter"));
        Entry.RelationshipValue = 75.0f;
        Entry.RelationshipType = EMingRelationshipType::Ally;
        Entry.RelationshipLevel = 7;
        
        TestEqual(TEXT("Character ID should match"), Entry.CharacterID, FName(TEXT("TestCharacter")));
        TestEqual(TEXT("Relationship value should be 75"), Entry.RelationshipValue, 75.0f);
        TestEqual(TEXT("Relationship type should be Ally"), Entry.RelationshipType, EMingRelationshipType::Ally);
        TestEqual(TEXT("Relationship level should be 7"), Entry.RelationshipLevel, 7);
        
        // Test save data
        FMingRelationshipSaveData SaveData;
        SaveData.SaveVersion = 1;
        SaveData.RelationshipEntries.Add(Entry);
        SaveData.TotalRelationships = 1;
        
        TestEqual(TEXT("Save version should be 1"), SaveData.SaveVersion, 1);
        TestEqual(TEXT("Should have 1 entry"), SaveData.RelationshipEntries.Num(), 1);
        TestEqual(TEXT("Total should be 1"), SaveData.TotalRelationships, 1);
        
        // Test validation
        TestTrue(TEXT("Valid save data should pass validation"), Handler->ValidateRelationshipSaveData(SaveData));
        
        // Test invalid data
        FMingRelationshipSaveData InvalidData;
        InvalidData.SaveVersion = 99; // Invalid version
        TestFalse(TEXT("Invalid version should fail validation"), Handler->ValidateRelationshipSaveData(InvalidData));
        
        // Test empty data
        FMingRelationshipSaveData EmptyData;
        TestFalse(TEXT("Empty data should fail validation"), Handler->ValidateRelationshipSaveData(EmptyData));
        
        // Test serialization size
        int32 EstimatedSize = Handler->GetSerializedDataSize(nullptr);
        TestEqual(TEXT("Size should be 0 for null manager"), EstimatedSize, 0);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingReputationSaveDataTest, "MingGoRTS.SaveSystem.Reputation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingReputationSaveDataTest::RunTest(const FString& Parameters)
{
    UMingReputationSaveDataHandler* Handler = NewObject<UMingReputationSaveDataHandler>();
    TestNotNull(TEXT("Reputation save handler should be created"), Handler);
    
    if (Handler)
    {
        // Test data structures
        FMingReputationEntry Entry;
        Entry.RegionID = FName(TEXT("TestRegion"));
        Entry.RegionName = TEXT("Test Region");
        Entry.ReputationValue = 85.0f;
        Entry.ReputationRank = 8;
        Entry.RankTitle = TEXT("Renowned");
        Entry.bIsAlly = true;
        Entry.bIsHostile = false;
        
        TestEqual(TEXT("Region ID should match"), Entry.RegionID, FName(TEXT("TestRegion")));
        TestEqual(TEXT("Region name should match"), Entry.RegionName, FString(TEXT("Test Region")));
        TestEqual(TEXT("Reputation value should be 85"), Entry.ReputationValue, 85.0f);
        TestTrue(TEXT("Should be ally"), Entry.bIsAlly);
        TestFalse(TEXT("Should not be hostile"), Entry.bIsHostile);
        
        // Test save data
        FMingReputationSaveData SaveData;
        SaveData.SaveVersion = 1;
        SaveData.ReputationEntries.Add(Entry);
        SaveData.MaxRankAchieved = 8;
        SaveData.AverageRank = 8;
        
        TestEqual(TEXT("Save version should be 1"), SaveData.SaveVersion, 1);
        TestEqual(TEXT("Should have 1 entry"), SaveData.ReputationEntries.Num(), 1);
        TestEqual(TEXT("Max rank should be 8"), SaveData.MaxRankAchieved, 8);
        
        // Test validation
        TestTrue(TEXT("Valid save data should pass validation"), Handler->ValidateReputationSaveData(SaveData));
        
        // Test region count
        int32 Count = Handler->GetTotalRegionCount(SaveData);
        TestEqual(TEXT("Region count should be 1"), Count, 1);
        
        // Test average reputation
        float Avg = Handler->GetAverageReputation(SaveData);
        TestEqual(TEXT("Average should be 85"), Avg, 85.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingAudioSaveDataTest, "MingGoRTS.SaveSystem.Audio",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingAudioSaveDataTest::RunTest(const FString& Parameters)
{
    UMingAudioSaveDataHandler* Handler = NewObject<UMingAudioSaveDataHandler>();
    TestNotNull(TEXT("Audio save handler should be created"), Handler);
    
    if (Handler)
    {
        // Test default settings
        FMingAudioSaveData DefaultSettings = Handler->GetDefaultAudioSettings();
        TestEqual(TEXT("Save version should be 1"), DefaultSettings.SaveVersion, 1);
        TestTrue(TEXT("Should have audio settings"), DefaultSettings.AudioSettings.Num() > 0);
        
        // Test category access
        float MasterVolume = Handler->GetCategoryVolume(DefaultSettings, EMingAudioCategory::Master);
        TestEqual(TEXT("Master volume should be 1.0"), MasterVolume, 1.0f);
        
        // Test muting
        TestFalse(TEXT("Master should not be muted by default"), Handler->IsCategoryMuted(DefaultSettings, EMingAudioCategory::Master));
        
        // Test modifying settings
        Handler->SetCategoryVolume(DefaultSettings, EMingAudioCategory::Music, 0.5f);
        float MusicVolume = Handler->GetCategoryVolume(DefaultSettings, EMingAudioCategory::Music);
        TestEqual(TEXT("Music volume should be 0.5"), MusicVolume, 0.5f);
        
        Handler->SetCategoryMuted(DefaultSettings, EMingAudioCategory::SFX, true);
        float SFXVolume = Handler->GetCategoryVolume(DefaultSettings, EMingAudioCategory::SFX);
        TestEqual(TEXT("SFX volume should be 0 when muted"), SFXVolume, 0.0f);
        
        // Test Republic Era settings
        FMingAudioSaveData EraSettings = Handler->GetRepublicEraAudioSettings();
        TestTrue(TEXT("Cultural music should be enabled"), EraSettings.RepublicEraPreferences.bEnableCulturalMusic);
        TestTrue(TEXT("Regional accents should be enabled"), EraSettings.RepublicEraPreferences.bEnableRegionalAccents);
        TestEqual(TEXT("Traditional instrument volume should be 0.9"), EraSettings.RepublicEraPreferences.TraditionalInstrumentVolume, 0.9f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameVersionTest, "MingGoRTS.SaveSystem.Version",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameVersionTest::RunTest(const FString& Parameters)
{
    // Test version consistency
    UMingSaveGame* SaveGame = NewObject<UMingSaveGame>();
    TestEqual(TEXT("SaveGame version should be 1"), SaveGame->GetCurrentSaveVersion(), 1);
    
    // Test handler versions
    UMingRelationshipSaveDataHandler* RelHandler = NewObject<UMingRelationshipSaveDataHandler>();
    TestEqual(TEXT("Relationship handler version should be 1"), RelHandler->GetCurrentSaveVersion(), 1);
    
    UMingReputationSaveDataHandler* RepHandler = NewObject<UMingReputationSaveDataHandler>();
    TestEqual(TEXT("Reputation handler version should be 1"), RepHandler->GetCurrentSaveVersion(), 1);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameSerializationTest, "MingGoRTS.SaveSystem.Serialization",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameSerializationTest::RunTest(const FString& Parameters)
{
    // Test relationship serialization
    {
        UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
        
        FMingRelationshipSaveData OriginalData;
        OriginalData.SaveVersion = 1;
        OriginalData.SaveTimestamp = FDateTime::Now();
        
        FMingRelationshipEntry Entry1;
        Entry1.CharacterID = FName(TEXT("Character1"));
        Entry1.RelationshipValue = 80.0f;
        Entry1.RelationshipType = EMingRelationshipType::Ally;
        
        FMingRelationshipEntry Entry2;
        Entry2.CharacterID = FName(TEXT("Character2"));
        Entry2.RelationshipValue = 30.0f;
        Entry2.RelationshipType = EMingRelationshipType::Rival;
        
        OriginalData.RelationshipEntries.Add(Entry1);
        OriginalData.RelationshipEntries.Add(Entry2);
        OriginalData.TotalRelationships = 2;
        
        // Export to JSON
        FString JsonString;
        bool bExportSuccess = Handler->ExportToJson(OriginalData, JsonString);
        TestTrue(TEXT("Export to JSON should succeed"), bExportSuccess);
        TestTrue(TEXT("JSON should not be empty"), !JsonString.IsEmpty());
        
        // Import from JSON
        FMingRelationshipSaveData ImportedData;
        bool bImportSuccess = Handler->ImportFromJson(JsonString, ImportedData);
        TestTrue(TEXT("Import from JSON should succeed"), bImportSuccess);
        TestEqual(TEXT("Version should match"), ImportedData.SaveVersion, OriginalData.SaveVersion);
        TestEqual(TEXT("Entry count should match"), ImportedData.RelationshipEntries.Num(), OriginalData.RelationshipEntries.Num());
        
        // Test data comparison
        TestTrue(TEXT("Data should be equal after round-trip"), Handler->CompareRelationshipData(OriginalData, ImportedData));
    }
    
    // Test audio serialization
    {
        UMingAudioSaveDataHandler* Handler = NewObject<UMingAudioSaveDataHandler>();
        FMingAudioSaveData OriginalData = Handler->GetDefaultAudioSettings();
        
        FString JsonString;
        bool bExportSuccess = Handler->ExportToJson(OriginalData, JsonString);
        TestTrue(TEXT("Audio export to JSON should succeed"), bExportSuccess);
        
        FMingAudioSaveData ImportedData;
        bool bImportSuccess = Handler->ImportFromJson(JsonString, ImportedData);
        TestTrue(TEXT("Audio import from JSON should succeed"), bImportSuccess);
        TestEqual(TEXT("Audio version should match"), ImportedData.SaveVersion, OriginalData.SaveVersion);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameValidationTest, "MingGoRTS.SaveSystem.Validation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameValidationTest::RunTest(const FString& Parameters)
{
    // Test relationship validation
    {
        UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
        
        // Valid data
        FMingRelationshipSaveData ValidData;
        ValidData.SaveVersion = 1;
        FMingRelationshipEntry Entry;
        Entry.CharacterID = FName(TEXT("Test"));
        Entry.RelationshipValue = 50.0f;
        ValidData.RelationshipEntries.Add(Entry);
        ValidData.TotalRelationships = 1;
        
        TestTrue(TEXT("Valid data should pass"), Handler->ValidateRelationshipSaveData(ValidData));
        
        // Invalid version
        FMingRelationshipSaveData BadVersion = ValidData;
        BadVersion.SaveVersion = 99;
        TestFalse(TEXT("Bad version should fail"), Handler->ValidateRelationshipSaveData(BadVersion));
        
        // Invalid relationship value
        FMingRelationshipSaveData BadValue = ValidData;
        BadValue.RelationshipEntries[0].RelationshipValue = -10.0f;
        TestFalse(TEXT("Invalid value should fail"), Handler->ValidateRelationshipSaveData(BadValue));
        
        // Empty character ID
        FMingRelationshipSaveData EmptyID = ValidData;
        EmptyID.RelationshipEntries[0].CharacterID = NAME_None;
        TestFalse(TEXT("Empty ID should fail"), Handler->ValidateRelationshipSaveData(EmptyID));
    }
    
    // Test reputation validation
    {
        UMingReputationSaveDataHandler* Handler = NewObject<UMingReputationSaveDataHandler>();
        
        // Valid data
        FMingReputationSaveData ValidData;
        ValidData.SaveVersion = 1;
        FMingReputationEntry Entry;
        Entry.RegionID = FName(TEXT("Test"));
        Entry.ReputationValue = 50.0f;
        ValidData.ReputationEntries.Add(Entry);
        
        TestTrue(TEXT("Valid reputation data should pass"), Handler->ValidateReputationSaveData(ValidData));
        
        // Invalid reputation value
        FMingReputationSaveData BadValue = ValidData;
        BadValue.ReputationEntries[0].ReputationValue = 150.0f;
        TestFalse(TEXT("Invalid reputation should fail"), Handler->ValidateReputationSaveData(BadValue));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameIntegrationTest, "MingGoRTS.SaveSystem.Integration",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FMingSaveGameIntegrationTest::RunTest(const FString& Parameters)
{
    // Test complete save system workflow
    UMingSaveGameManager* SaveManager = NewObject<UMingSaveGameManager>();
    if (SaveManager)
    {
        SaveManager->Initialize();
        
        // Test slot queries
        TArray<FMingSaveGameSlotInfo> Slots = SaveManager->GetAllSaveSlots();
        TestTrue(TEXT("Should get slot list"), Slots.Num() >= 0);
        
        // Test autosave properties
        TestTrue(TEXT("Autosave should be enabled"), SaveManager->IsAutosaveEnabled());
        TestTrue(TEXT("Autosave interval should be positive"), SaveManager->GetAutosaveInterval() > 0.0f);
        
        // Test save result types
        EMingSaveGameResult Results[] = {
            EMingSaveGameResult::Success,
            EMingSaveGameResult::Failed_InvalidSlot,
            EMingSaveGameResult::Failed_NoSpace,
            EMingSaveGameResult::Failed_CorruptData,
            EMingSaveGameResult::Failed_VersionMismatch
        };
        
        for (int32 i = 0; i < 5; i++)
        {
            TestTrue(TEXT("Save result should be valid enum"), 
                static_cast<int32>(Results[i]) >= 0 && static_cast<int32>(Results[i]) <= 5);
        }
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameStatisticsTest, "MingGoRTS.SaveSystem.Statistics",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameStatisticsTest::RunTest(const FString& Parameters)
{
    // Test relationship statistics
    {
        UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
        
        FMingRelationshipSaveData Data;
        
        // Add entries with different types
        FMingRelationshipEntry Ally;
        Ally.CharacterID = FName(TEXT("Ally1"));
        Ally.RelationshipValue = 80.0f;
        Ally.RelationshipType = EMingRelationshipType::Ally;
        
        FMingRelationshipEntry Enemy;
        Enemy.CharacterID = FName(TEXT("Enemy1"));
        Enemy.RelationshipValue = 20.0f;
        Enemy.RelationshipType = EMingRelationshipType::Enemy;
        
        FMingRelationshipEntry Neutral;
            Neutral.CharacterID = FName(TEXT("Neutral1"));
            Neutral.RelationshipValue = 50.0f;
            Neutral.RelationshipType = EMingRelationshipType::Acquaintance;
            
            Data.RelationshipEntries.Add(Ally);
            Data.RelationshipEntries.Add(Enemy);
            Data.RelationshipEntries.Add(Neutral);
            
            // Test count
            int32 Count = Handler->GetRelationshipCount(Data);
            TestEqual(TEXT("Should have 3 relationships"), Count, 3);
            
            // Test average
            float Avg = Handler->GetAverageRelationshipValue(Data);
            float ExpectedAvg = (80.0f + 20.0f + 50.0f) / 3.0f;
            TestEqual(TEXT("Average should be correct"), Avg, ExpectedAvg);
            
            // Test distribution
            auto Distribution = Handler->GetRelationshipDistribution(Data);
            TestEqual(TEXT("Should have 1 ally"), Distribution[EMingRelationshipType::Ally], 1);
            TestEqual(TEXT("Should have 1 enemy"), Distribution[EMingRelationshipType::Enemy], 1);
    }
    
    // Test reputation statistics
    {
        UMingReputationSaveDataHandler* Handler = NewObject<UMingReputationSaveDataHandler>();
        
        FMingReputationSaveData Data;
        
        FMingReputationEntry HighRep;
        HighRep.RegionID = FName(TEXT("High"));
        HighRep.ReputationValue = 90.0f;
        HighRep.ReputationRank = 9;
        
        FMingReputationEntry LowRep;
        LowRep.RegionID = FName(TEXT("Low"));
        LowRep.ReputationValue = 20.0f;
        LowRep.ReputationRank = 2;
        
        Data.ReputationEntries.Add(HighRep);
        Data.ReputationEntries.Add(LowRep);
        Data.MaxRankAchieved = 9;
        Data.AverageRank = 5;
        
        // Test region count
        int32 Count = Handler->GetTotalRegionCount(Data);
        TestEqual(TEXT("Should have 2 regions"), Count, 2);
        
        // Test average reputation
        float Avg = Handler->GetAverageReputation(Data);
        TestEqual(TEXT("Average should be 55"), Avg, 55.0f);
        
        // Test top regions
        auto TopRegions = Handler->GetTopRegions(Data, 1);
        TestEqual(TEXT("Should return 1 top region"), TopRegions.Num(), 1);
        TestEqual(TEXT("Top region should be High"), TopRegions[0].RegionID, FName(TEXT("High")));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameDataComparisonTest, "MingGoRTS.SaveSystem.Comparison",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameDataComparisonTest::RunTest(const FString& Parameters)
{
    UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
    
    // Create two identical datasets
    FMingRelationshipSaveData DataA;
    DataA.SaveVersion = 1;
    FMingRelationshipEntry EntryA;
    EntryA.CharacterID = FName(TEXT("Test"));
    EntryA.RelationshipValue = 50.0f;
    DataA.RelationshipEntries.Add(EntryA);
    
    FMingRelationshipSaveData DataB = DataA;
    
    // Should be equal
    TestTrue(TEXT("Identical data should be equal"), Handler->CompareRelationshipData(DataA, DataB));
    
    // Change version
    DataB.SaveVersion = 2;
    TestFalse(TEXT("Different version should not be equal"), Handler->CompareRelationshipData(DataA, DataB));
    DataB.SaveVersion = 1; // Restore
    
    // Change relationship value
    DataB.RelationshipEntries[0].RelationshipValue = 60.0f;
    TestFalse(TEXT("Different value should not be equal"), Handler->CompareRelationshipData(DataA, DataB));
    
    // Get differences
    auto Differences = Handler->GetDifferences(DataA, DataB);
    TestTrue(TEXT("Should have differences"), Differences.Num() > 0);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingSaveGameChecksumTest, "MingGoRTS.SaveSystem.Checksum",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingSaveGameChecksumTest::RunTest(const FString& Parameters)
{
    // Test relationship checksums
    {
        UMingRelationshipSaveDataHandler* Handler = NewObject<UMingRelationshipSaveDataHandler>();
        
        FMingRelationshipSaveData Data;
        Data.SaveVersion = 1;
        
        TArray<uint8> DataA;
        FMemoryWriter WriterA(DataA);
        WriterA << Data;
        
        FString ChecksumA = Handler->GetSaveDataChecksum(DataA);
        TestTrue(TEXT("Checksum A should not be empty"), !ChecksumA.IsEmpty());
        
        // Same data should produce same checksum
        TArray<uint8> DataB;
        FMemoryWriter WriterB(DataB);
        WriterB << Data;
        
        FString ChecksumB = Handler->GetSaveDataChecksum(DataB);
        TestEqual(TEXT("Same data should have same checksum"), ChecksumA, ChecksumB);
    }
    
    return true;
}
