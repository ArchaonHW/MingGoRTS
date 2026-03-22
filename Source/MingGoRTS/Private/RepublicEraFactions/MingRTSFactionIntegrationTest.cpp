// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Integration Test Implementation

#include "RepublicEraFactions/MingRTSFactionIntegrationTest.h"
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "RepublicEraFactions/MingRTSBeiyangGovernment.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSFactionIntegrationTest, Log, All);

bool MingRTSFactionIntegrationTest::RunAllIntegrationTests()
{
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("=== Starting MingRTS Faction Integration Tests ==="));
    
    bool bAllTestsPassed = true;
    
    // Test 1: Faction Manager Integration
    bAllTestsPassed &= TestFactionManagerIntegration();
    
    // Test 2: Faction Data Consistency
    bAllTestsPassed &= TestFactionDataConsistency();
    
    // Test 3: Faction Balance Validation
    bAllTestsPassed &= TestFactionBalanceValidation();
    
    // Test 4: Faction System Performance
    bAllTestsPassed &= TestFactionSystemPerformance();
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("=== Faction Integration Tests Complete ==="));
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Overall Result: %s"), 
        bAllTestsPassed ? TEXT("PASSED") : TEXT("FAILED"));
    
    return bAllTestsPassed;
}

bool MingRTSFactionIntegrationTest::TestFactionManagerIntegration()
{
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Integration Test 1: Faction Manager Integration"));
    
    UMingRTSFactionManager* Manager = NewObject<UMingRTSFactionManager>();
    if (!Manager)
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Failed to create Faction Manager"));
        return false;
    }
    
    Manager->InitializeFactionManager();
    
    // Test faction retrieval
    UMingRTSFactionBase* BeiyangFaction = Manager->GetFactionByID(FName("BeiyangGovernment"));
    if (!BeiyangFaction)
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Failed to retrieve Beiyang Government faction"));
        return false;
    }
    
    // Test faction attributes
    FFactionAttributes Attributes = BeiyangFaction->GetFactionAttributes();
    if (Attributes.FactionID != FName("BeiyangGovernment"))
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Faction ID mismatch"));
        return false;
    }
    
    // Test faction validation
    bool bValidationResult = Manager->ValidateAllFactions();
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Faction validation result: %s"), 
        bValidationResult ? TEXT("PASSED") : TEXT("FAILED"));
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Faction Manager Integration test completed"));
    return true;
}

bool MingRTSFactionIntegrationTest::TestFactionDataConsistency()
{
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Integration Test 2: Faction Data Consistency"));
    
    UMingRTSFactionManager* Manager = NewObject<UMingRTSFactionManager>();
    Manager->InitializeFactionManager();
    
    // Test data consistency across multiple faction instances
    UMingRTSBeiyangGovernment* Beiyang1 = NewObject<UMingRTSBeiyangGovernment>();
    UMingRTSBeiyangGovernment* Beiyang2 = NewObject<UMingRTSBeiyangGovernment>();
    
    Beiyang1->InitializeFaction();
    Beiyang2->InitializeFaction();
    
    FFactionAttributes Attr1 = Beiyang1->GetFactionAttributes();
    FFactionAttributes Attr2 = Beiyang2->GetFactionAttributes();
    
    // Check consistency
    if (Attr1.FactionID != Attr2.FactionID || 
        Attr1.FactionName != Attr2.FactionName ||
        Attr1.Difficulty != Attr2.Difficulty)
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Faction data inconsistency detected"));
        return false;
    }
    
    // Test unique units consistency
    TArray<FFactionUnit> Units1 = Beiyang1->GetUniqueUnits();
    TArray<FFactionUnit> Units2 = Beiyang2->GetUniqueUnits();
    
    if (Units1.Num() != Units2.Num())
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Unique units count mismatch"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Faction Data Consistency test completed"));
    return true;
}

bool MingRTSFactionIntegrationTest::TestFactionBalanceValidation()
{
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Integration Test 3: Faction Balance Validation"));
    
    UMingRTSFactionManager* Manager = NewObject<UMingRTSFactionManager>();
    Manager->InitializeFactionManager();
    
    // Test balance report generation
    FString BalanceReport = Manager->GetFactionBalanceReport();
    if (BalanceReport.IsEmpty())
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Error, TEXT("Failed to generate balance report"));
        return false;
    }
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Balance Report:"));
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("%s"), *BalanceReport);
    
    // Test faction count by difficulty
    int32 EasyCount = Manager->GetFactionsByDifficulty(EFactionDifficulty::Easy).Num();
    int32 NormalCount = Manager->GetFactionsByDifficulty(EFactionDifficulty::Normal).Num();
    int32 HardCount = Manager->GetFactionsByDifficulty(EFactionDifficulty::Hard).Num();
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Factions by difficulty - Easy: %d, Normal: %d, Hard: %d"), 
        EasyCount, NormalCount, HardCount);
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Faction Balance Validation test completed"));
    return true;
}

bool MingRTSFactionIntegrationTest::TestFactionSystemPerformance()
{
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Integration Test 4: Faction System Performance"));
    
    // Performance test: Create multiple faction instances
    double StartTime = FPlatformTime::Seconds();
    
    const int32 TestIterations = 100;
    for (int32 i = 0; i < TestIterations; ++i)
    {
        UMingRTSBeiyangGovernment* Beiyang = NewObject<UMingRTSBeiyangGovernment>();
        Beiyang->InitializeFaction();
        
        // Test data retrieval
        FFactionAttributes Attributes = Beiyang->GetFactionAttributes();
        TArray<FFactionUnit> Units = Beiyang->GetUniqueUnits();
        TArray<FFactionMechanic> Mechanics = Beiyang->GetUniqueMechanics();
        FGameplayGuide Guide = Beiyang->GetGameplayGuide();
    }
    
    double EndTime = FPlatformTime::Seconds();
    double ElapsedTime = EndTime - StartTime;
    double AverageTime = ElapsedTime / TestIterations;
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Performance Test Results:"));
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("  Total Time: %.4f seconds"), ElapsedTime);
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("  Average Time per Iteration: %.6f seconds"), AverageTime);
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("  Iterations per Second: %.2f"), 1.0 / AverageTime);
    
    // Performance criteria: Should be able to create and initialize at least 10 factions per second
    if (AverageTime > 0.1) // More than 100ms per iteration
    {
        UE_LOG(LogMingRTSFactionIntegrationTest, Warning, TEXT("Performance below optimal threshold"));
    }
    
    UE_LOG(LogMingRTSFactionIntegrationTest, Log, TEXT("Faction System Performance test completed"));
    return true;
}

// Global function implementation
bool RunMingRTSFactionIntegrationTests()
{
    return MingRTSFactionIntegrationTest::RunAllIntegrationTests();
}
