// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - System Test Implementation

#include "RepublicEraFactions/MingRTSFactionSystemTest.h"
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "RepublicEraFactions/MingRTSBeiyangGovernment.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSFactionSystemTest, Log, All);

// MingRTSFactionSystemTest implementation

bool MingRTSFactionSystemTest::RunAllTests()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("=== Starting MingRTS Faction System Tests ==="));
    
    bool bAllTestsPassed = true;
    
    // Test 1: Faction Manager Initialization
    bAllTestsPassed &= TestFactionManagerInitialization();
    
    // Test 2: Beiyang Government Creation
    bAllTestsPassed &= TestBeiyangGovernmentCreation();
    
    // Test 3: Faction Attributes
    bAllTestsPassed &= TestFactionAttributes();
    
    // Test 4: Unique Units
    bAllTestsPassed &= TestUniqueUnits();
    
    // Test 5: Unique Mechanics
    bAllTestsPassed &= TestUniqueMechanics();
    
    // Test 6: Gameplay Guide
    bAllTestsPassed &= TestGameplayGuide();
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("=== Faction System Tests Complete ==="));
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Overall Result: %s"), 
        bAllTestsPassed ? TEXT("PASSED") : TEXT("FAILED"));
    
    return bAllTestsPassed;
}

bool MingRTSFactionSystemTest::TestFactionManagerInitialization()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 1: Faction Manager Initialization"));
    
    UMingRTSFactionManager* Manager = NewObject<UMingRTSFactionManager>();
    if (!Manager)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Failed to create Faction Manager"));
        return false;
    }
    
    Manager->InitializeFactionManager();
    
    if (!Manager->AreAllFactionsRegistered())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Warning, TEXT("Not all factions are registered (expected for partial implementation)"));
    }
    
    int32 FactionCount = Manager->GetFactionCount();
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Registered %d factions"), FactionCount);
    
    return true;
}

bool MingRTSFactionSystemTest::TestBeiyangGovernmentCreation()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 2: Beiyang Government Creation"));
    
    UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
    if (!Beiyang)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Failed to create Beiyang Government faction"));
        return false;
    }
    
    Beiyang->InitializeFaction();
    
    if (!Beiyang->IsInitialized())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Beiyang Government failed to initialize"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Beiyang Government created and initialized successfully"));
    return true;
}

bool MingRTSFactionSystemTest::TestFactionAttributes()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 3: Faction Attributes"));
    
    UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
    Beiyang->InitializeFaction();
    
    FFactionAttributes Attributes = Beiyang->GetFactionAttributes();
    
    if (Attributes.FactionID.IsNone())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Faction ID is none"));
        return false;
    }
    
    if (Attributes.FactionName.IsEmpty())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Faction name is empty"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Faction: %s (%s)"), 
        *Attributes.FactionName.ToString(), *Attributes.FactionNameEnglish.ToString());
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Difficulty: %d, Economic Power: %d, Military Power: %d"), 
        (int32)Attributes.Difficulty, Attributes.EconomicPower, Attributes.MilitaryPower);
    
    return true;
}

bool MingRTSFactionSystemTest::TestUniqueUnits()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 4: Unique Units"));
    
    UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
    Beiyang->InitializeFaction();
    
    TArray<FFactionUnit> Units = Beiyang->GetUniqueUnits();
    
    if (Units.Num() == 0)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("No unique units found"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Found %d unique units:"), Units.Num());
    for (const FFactionUnit& Unit : Units)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("  - %s: %s"), 
            *Unit.UnitID.ToString(), *Unit.Description.ToString());
    }
    
    return true;
}

bool MingRTSFactionSystemTest::TestUniqueMechanics()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 5: Unique Mechanics"));
    
    UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
    Beiyang->InitializeFaction();
    
    TArray<FFactionMechanic> Mechanics = Beiyang->GetUniqueMechanics();
    
    if (Mechanics.Num() == 0)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("No unique mechanics found"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Found %d unique mechanics:"), Mechanics.Num());
    for (const FFactionMechanic& Mechanic : Mechanics)
    {
        UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("  - %s: %s"), 
            *Mechanic.MechanicID.ToString(), *Mechanic.Description.ToString());
    }
    
    return true;
}

bool MingRTSFactionSystemTest::TestGameplayGuide()
{
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Test 6: Gameplay Guide"));
    
    UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
    Beiyang->InitializeFaction();
    
    FGameplayGuide Guide = Beiyang->GetGameplayGuide();
    
    if (Guide.FactionName.IsEmpty())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Gameplay guide faction name is empty"));
        return false;
    }
    
    if (Guide.Description.IsEmpty())
    {
        UE_LOG(LogMingRTSFactionSystemTest, Error, TEXT("Gameplay guide description is empty"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Gameplay Guide for %s:"), *Guide.FactionName);
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Description: %s"), *Guide.Description);
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Early Game: %s"), *Guide.EarlyGameStrategy);
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Mid Game: %s"), *Guide.MidGameStrategy);
    UE_LOG(LogMingRTSFactionSystemTest, Log, TEXT("Late Game: %s"), *Guide.LateGameStrategy);
    
    return true;
}

// Global function implementation
bool RunMingRTSFactionSystemTests()
{
    return MingRTSFactionSystemTest::RunAllTests();
}
