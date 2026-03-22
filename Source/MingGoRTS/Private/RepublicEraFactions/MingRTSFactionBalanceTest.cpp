// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Balance Test Implementation

#include "RepublicEraFactions/MingRTSFactionBalanceTest.h"
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFactionBalanceTest, Log, All);

UMingRTSFactionBalanceTest::UMingRTSFactionBalanceTest()
{
}

void UMingRTSFactionBalanceTest::InitializeBalanceTest()
{
    FactionManager = NewObject<UMingRTSFactionManager>(this);
    if (FactionManager)
    {
        FactionManager->InitializeFactionManager();
    }
    
    TestResults = FTestResults();
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Balance Test Suite initialized"));
}

bool UMingRTSFactionBalanceTest::RunAllBalanceTests()
{
    if (!FactionManager)
    {
        InitializeBalanceTest();
    }
    
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Starting all balance tests..."));
    
    bool bAllPassed = true;
    
    bAllPassed &= ValidateAllFactionsPresent();
    bAllPassed &= TestFactionAttributes();
    bAllPassed &= TestUnitBalance();
    bAllPassed &= TestMechanicsBalance();
    bAllPassed &= TestDifficultyDistribution();
    bAllPassed &= TestStartingConditions();
    bAllPassed &= TestFactionDiversity();
    
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Balance tests completed. Pass rate: %.1f%%"), 
        TestResults.GetPassRate() * 100.0f);
    
    return bAllPassed;
}

bool UMingRTSFactionBalanceTest::ValidateAllFactionsPresent() const
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: All 12 factions registered"));
    
    if (!FactionManager)
    {
        UE_LOG(LogFactionBalanceTest, Error, TEXT("Faction Manager not initialized"));
        return false;
    }
    
    bool bPassed = FactionManager->AreAllFactionsRegistered();
    
    if (bPassed)
    {
        const_cast<UMingRTSFactionBalanceTest*>(this)->TestResults.AddPass(TEXT("AllFactionsPresent"));
        UE_LOG(LogFactionBalanceTest, Log, TEXT("✓ All 12 factions are registered"));
    }
    else
    {
        const_cast<UMingRTSFactionBalanceTest*>(this)->TestResults.AddFail(
            TEXT("AllFactionsPresent"), 
            FString::Printf(TEXT("Expected 12 factions, found %d"), FactionManager->GetFactionCount()));
        UE_LOG(LogFactionBalanceTest, Error, TEXT("✗ Only %d factions registered"), FactionManager->GetFactionCount());
    }
    
    return bPassed;
}

bool UMingRTSFactionBalanceTest::TestFactionAttributes()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Faction Attributes"));
    
    bool bAllPassed = true;
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (!Faction) continue;
        
        FFactionAttributes Attrs = Faction->GetFactionAttributes();
        FString FactionName = Attrs.FactionName.ToString();
        
        // Test ID is valid
        if (Attrs.FactionID.IsNone())
        {
            TestResults.AddFail(TEXT("FactionAttributes_" + FactionName), TEXT("Faction ID is empty"));
            bAllPassed = false;
            continue;
        }
        
        // Test names are valid
        if (Attrs.FactionName.IsEmpty() || Attrs.FactionNameEnglish.IsEmpty())
        {
            TestResults.AddFail(TEXT("FactionAttributes_" + FactionName), TEXT("Faction names are empty"));
            bAllPassed = false;
            continue;
        }
        
        // Test starting position is valid
        if (Attrs.StartingPosition.CapitalCity.IsEmpty())
        {
            TestResults.AddFail(TEXT("FactionAttributes_" + FactionName), TEXT("Capital city is empty"));
            bAllPassed = false;
            continue;
        }
        
        if (Attrs.StartingPosition.StartingTerritories.Num() == 0)
        {
            TestResults.AddFail(TEXT("FactionAttributes_" + FactionName), TEXT("No starting territories"));
            bAllPassed = false;
            continue;
        }
        
        // Test traits are valid
        if (Attrs.Traits.Advantages.Num() == 0 || Attrs.Traits.Disadvantages.Num() == 0)
        {
            TestResults.AddFail(TEXT("FactionAttributes_" + FactionName), TEXT("Missing advantages or disadvantages"));
            bAllPassed = false;
            continue;
        }
        
        TestResults.AddPass(TEXT("FactionAttributes_" + FactionName));
        UE_LOG(LogFactionBalanceTest, Log, TEXT("✓ %s attributes validated"), *FactionName);
    }
    
    return bAllPassed;
}

bool UMingRTSFactionBalanceTest::TestUnitBalance()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Unit Balance"));
    
    bool bAllPassed = true;
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (!Faction) continue;
        
        TArray<FFactionUnit> Units = Faction->GetUniqueUnits();
        FString FactionName = Faction->GetFactionAttributes().FactionName.ToString();
        
        if (Units.Num() == 0)
        {
            TestResults.AddFail(TEXT("UnitBalance_" + FactionName), TEXT("No unique units"));
            bAllPassed = false;
            continue;
        }
        
        for (const FFactionUnit& Unit : Units)
        {
            if (!ValidateUnitStats(Unit, FactionName))
            {
                bAllPassed = false;
            }
        }
        
        TestResults.AddPass(TEXT("UnitBalance_" + FactionName));
    }
    
    return bAllPassed;
}

bool UMingRTSFactionBalanceTest::ValidateUnitStats(const FFactionUnit& Unit, const FString& FactionName)
{
    bool bValid = true;
    FString Context = FactionName + TEXT(".") + Unit.UnitID.ToString();
    
    // Check attack power (range: 40-180)
    bValid &= CheckStatRange(Unit.BaseStats.AttackPower, 40, 180, TEXT("AttackPower"), Context);
    
    // Check defense power (range: 40-160)
    bValid &= CheckStatRange(Unit.BaseStats.DefensePower, 40, 160, TEXT("DefensePower"), Context);
    
    // Check movement speed (range: 60-180)
    bValid &= CheckStatRange(Unit.BaseStats.MovementSpeed, 60, 180, TEXT("MovementSpeed"), Context);
    
    // Check health points (range: 50-200)
    bValid &= CheckStatRange(Unit.BaseStats.HealthPoints, 50, 200, TEXT("HealthPoints"), Context);
    
    // Check production cost (range: 40-250)
    bValid &= CheckStatRange(Unit.BaseStats.ProductionCost, 40, 250, TEXT("ProductionCost"), Context);
    
    // Check training time (range: 3-20 seconds)
    bValid &= CheckStatRange((int32)Unit.BaseStats.TrainingTime, 3, 20, TEXT("TrainingTime"), Context);
    
    if (bValid)
    {
        TestResults.AddPass(TEXT("UnitStats_" + Context));
    }
    
    return bValid;
}

bool UMingRTSFactionBalanceTest::CheckStatRange(int32 Value, int32 Min, int32 Max, 
    const FString& StatName, const FString& Context)
{
    if (Value < Min || Value > Max)
    {
        TestResults.AddFail(TEXT("StatRange_" + Context + TEXT("_") + StatName),
            FString::Printf(TEXT("%s = %d, expected range [%d, %d]"), *StatName, Value, Min, Max));
        return false;
    }
    return true;
}

bool UMingRTSFactionBalanceTest::TestMechanicsBalance()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Mechanics Balance"));
    
    bool bAllPassed = true;
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (!Faction) continue;
        
        TArray<FFactionMechanic> Mechanics = Faction->GetUniqueMechanics();
        FString FactionName = Faction->GetFactionAttributes().FactionName.ToString();
        
        if (Mechanics.Num() == 0)
        {
            TestResults.AddFail(TEXT("MechanicsBalance_" + FactionName), TEXT("No unique mechanics"));
            bAllPassed = false;
            continue;
        }
        
        for (const FFactionMechanic& Mechanic : Mechanics)
        {
            if (!ValidateMechanicValues(Mechanic, FactionName))
            {
                bAllPassed = false;
            }
        }
        
        TestResults.AddPass(TEXT("MechanicsBalance_" + FactionName));
    }
    
    return bAllPassed;
}

bool UMingRTSFactionBalanceTest::ValidateMechanicValues(const FFactionMechanic& Mechanic, 
    const FString& FactionName)
{
    bool bValid = true;
    FString Context = FactionName + TEXT(".") + Mechanic.MechanicID.ToString();
    
    // Check effect multiplier (range: 1.0 - 2.0)
    bValid &= CheckMultiplierRange(Mechanic.EffectMultiplier, 1.0f, 2.0f, TEXT("EffectMultiplier"), Context);
    
    if (bValid)
    {
        TestResults.AddPass(TEXT("Mechanic_" + Context));
    }
    
    return bValid;
}

bool UMingRTSFactionBalanceTest::CheckMultiplierRange(float Value, float Min, float Max, 
    const FString& StatName, const FString& Context)
{
    if (Value < Min || Value > Max)
    {
        TestResults.AddFail(TEXT("MultiplierRange_" + Context + TEXT("_") + StatName),
            FString::Printf(TEXT("%s = %.2f, expected range [%.1f, %.1f]"), *StatName, Value, Min, Max));
        return false;
    }
    return true;
}

bool UMingRTSFactionBalanceTest::TestDifficultyDistribution()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Difficulty Distribution"));
    
    if (!FactionManager)
    {
        return false;
    }
    
    bool bPassed = true;
    
    int32 VeryEasy = FactionManager->GetFactionsByDifficulty(EFactionDifficulty::VeryEasy).Num();
    int32 Easy = FactionManager->GetFactionsByDifficulty(EFactionDifficulty::Easy).Num();
    int32 Normal = FactionManager->GetFactionsByDifficulty(EFactionDifficulty::Normal).Num();
    int32 Hard = FactionManager->GetFactionsByDifficulty(EFactionDifficulty::Hard).Num();
    int32 VeryHard = FactionManager->GetFactionsByDifficulty(EFactionDifficulty::VeryHard).Num();
    
    // Expected distribution: 1 VeryEasy, 2 Easy, 4 Normal, 4 Hard, 1 VeryHard
    if (VeryEasy != 1)
    {
        TestResults.AddFail(TEXT("Difficulty_VeryEasy"), 
            FString::Printf(TEXT("Expected 1 Very Easy faction, found %d"), VeryEasy));
        bPassed = false;
    }
    
    if (Easy != 2)
    {
        TestResults.AddFail(TEXT("Difficulty_Easy"), 
            FString::Printf(TEXT("Expected 2 Easy factions, found %d"), Easy));
        bPassed = false;
    }
    
    if (Normal != 4)
    {
        TestResults.AddFail(TEXT("Difficulty_Normal"), 
            FString::Printf(TEXT("Expected 4 Normal factions, found %d"), Normal));
        bPassed = false;
    }
    
    if (Hard != 4)
    {
        TestResults.AddFail(TEXT("Difficulty_Hard"), 
            FString::Printf(TEXT("Expected 4 Hard factions, found %d"), Hard));
        bPassed = false;
    }
    
    if (VeryHard != 1)
    {
        TestResults.AddFail(TEXT("Difficulty_VeryHard"), 
            FString::Printf(TEXT("Expected 1 Very Hard faction, found %d"), VeryHard));
        bPassed = false;
    }
    
    if (bPassed)
    {
        TestResults.AddPass(TEXT("DifficultyDistribution"));
        UE_LOG(LogFactionBalanceTest, Log, TEXT("✓ Difficulty distribution: VeryEasy=%d, Easy=%d, Normal=%d, Hard=%d, VeryHard=%d"),
            VeryEasy, Easy, Normal, Hard, VeryHard);
    }
    
    return bPassed;
}

bool UMingRTSFactionBalanceTest::TestStartingConditions()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Starting Conditions"));
    
    bool bAllPassed = true;
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (!Faction) continue;
        
        FFactionAttributes Attrs = Faction->GetFactionAttributes();
        FString FactionName = Attrs.FactionName.ToString();
        
        // Gold range: 300-2000
        if (!CheckStatRange(Attrs.StartingPosition.InitialGold, 300, 2000, TEXT("InitialGold"), FactionName))
        {
            bAllPassed = false;
        }
        
        // Manpower range: 200-800
        if (!CheckStatRange(Attrs.StartingPosition.InitialManpower, 200, 800, TEXT("InitialManpower"), FactionName))
        {
            bAllPassed = false;
        }
        
        // Reputation range: 20-80
        if (!CheckStatRange(Attrs.StartingPosition.InitialReputation, 20, 80, TEXT("InitialReputation"), FactionName))
        {
            bAllPassed = false;
        }
        
        TestResults.AddPass(TEXT("StartingConditions_" + FactionName));
    }
    
    return bAllPassed;
}

bool UMingRTSFactionBalanceTest::TestFactionDiversity()
{
    UE_LOG(LogFactionBalanceTest, Log, TEXT("Testing: Faction Diversity"));
    
    bool bPassed = true;
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    // Check that all factions have unique IDs
    TSet<FName> UsedIDs;
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (!Faction) continue;
        
        FName FactionID = Faction->GetFactionAttributes().FactionID;
        if (UsedIDs.Contains(FactionID))
        {
            TestResults.AddFail(TEXT("FactionDiversity_UniqueIDs"), 
                FString::Printf(TEXT("Duplicate faction ID: %s"), *FactionID.ToString()));
            bPassed = false;
        }
        UsedIDs.Add(FactionID);
    }
    
    // Check that total unique units is reasonable (expected: ~48 units for 12 factions)
    int32 TotalUnits = FactionManager->GetTotalUniqueUnitsCount();
    if (TotalUnits < 40 || TotalUnits > 60)
    {
        TestResults.AddFail(TEXT("FactionDiversity_UnitCount"), 
            FString::Printf(TEXT("Total units = %d, expected range [40, 60]"), TotalUnits));
        bPassed = false;
    }
    
    // Check that total unique mechanics is reasonable (expected: ~24-36 mechanics for 12 factions)
    int32 TotalMechanics = FactionManager->GetTotalUniqueMechanicsCount();
    if (TotalMechanics < 20 || TotalMechanics > 40)
    {
        TestResults.AddFail(TEXT("FactionDiversity_MechanicCount"), 
            FString::Printf(TEXT("Total mechanics = %d, expected range [20, 40]"), TotalMechanics));
        bPassed = false;
    }
    
    if (bPassed)
    {
        TestResults.AddPass(TEXT("FactionDiversity"));
        UE_LOG(LogFactionBalanceTest, Log, TEXT("✓ Faction diversity: %d unique units, %d unique mechanics"),
            TotalUnits, TotalMechanics);
    }
    
    return bPassed;
}

FString UMingRTSFactionBalanceTest::GetTestResultsSummary() const
{
    FString Summary = TEXT("=== Faction Balance Test Results ===\n\n");
    Summary += FString::Printf(TEXT("Total Tests: %d\n"), TestResults.TotalTests);
    Summary += FString::Printf(TEXT("Passed: %d\n"), TestResults.PassedTests);
    Summary += FString::Printf(TEXT("Failed: %d\n"), TestResults.FailedTests);
    Summary += FString::Printf(TEXT("Pass Rate: %.1f%%\n\n"), TestResults.GetPassRate() * 100.0f);
    
    if (TestResults.ErrorMessages.Num() > 0)
    {
        Summary += TEXT("Errors:\n");
        for (const FString& Error : TestResults.ErrorMessages)
        {
            Summary += FString::Printf(TEXT("  - %s\n"), *Error);
        }
    }
    
    return Summary;
}

FString UMingRTSFactionBalanceTest::GenerateBalanceReport() const
{
    if (!FactionManager)
    {
        return TEXT("Faction Manager not initialized");
    }
    
    FString Report = TEXT("=== MingGoRTS Republic Era Factions Balance Report ===\n\n");
    Report += FactionManager->GetFactionBalanceReport();
    Report += TEXT("\n=== Test Results ===\n");
    Report += GetTestResultsSummary();
    
    return Report;
}
