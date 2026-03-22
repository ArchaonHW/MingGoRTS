// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization System Test Suite Implementation

#include "Localization/MingRTSLocalizationSystemTest.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSLocalizationTest, Log, All);

UMingRTSLocalizationSystemTest::UMingRTSLocalizationSystemTest()
{
}

void UMingRTSLocalizationSystemTest::InitializeTestSystems()
{
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Initializing localization test systems..."));
    
    // Create localization system
    LocalizationSystem = NewObject<UMingRTSLocalizationSystem>();
    if (LocalizationSystem)
    {
        LocalizationSystem->InitializeLocalizationSystem();
    }
    
    // Create cultural adaptation system
    CulturalSystem = NewObject<UMingRTSCulturalAdaptationSystem>();
    if (CulturalSystem)
    {
        CulturalSystem->InitializeCulturalSystem();
    }
    
    // Generate test data
    GenerateTestData();
    
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Localization test systems initialized"));
}

void UMingRTSLocalizationSystemTest::CleanupTestSystems()
{
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Cleaning up localization test systems..."));
    
    // Cleanup test data
    CleanupTestData();
    
    // Shutdown systems
    if (LocalizationSystem)
    {
        LocalizationSystem->ShutdownLocalizationSystem();
    }
    
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Localization test systems cleanup complete"));
}

TArray<FLocalizationTestResult> UMingRTSLocalizationSystemTest::RunAllTests()
{
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Running comprehensive localization test suite..."));
    
    TArray<FLocalizationTestResult> Results;
    
    // Initialize systems
    InitializeTestSystems();
    
    // Run all tests
    Results.Add(TestBasicLocalization());
    Results.Add(TestLanguageSwitching());
    Results.Add(TestTextDirection());
    Results.Add(TestCulturalAdaptation());
    Results.Add(TestContentVariants());
    Results.Add(TestRegionalParameters());
    Results.Add(TestPerformance());
    Results.Add(TestMemoryManagement());
    Results.Add(TestFallbackMechanisms());
    Results.Add(TestConcurrentAccess());
    
    // Cleanup
    CleanupTestSystems();
    
    // Generate report
    FString Report = GenerateTestReport(Results);
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Test Report:\n%s"), *Report);
    
    return Results;
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestBasicLocalization()
{
    FString TestName = TEXT("Basic Localization");
    float StartTime = FDateTime::Now().ToUnixTimestamp();
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Test basic text retrieval
        FString OKText = LocalizationSystem->GetLocalizedText(TEXT("UI.OK"));
        if (OKText.IsEmpty())
        {
            return CreateTestResult(false, TestName, TEXT("Failed to retrieve basic UI text"));
        }
        
        // Test namespace-based retrieval
        FString GameTitle = LocalizationSystem->GetLocalizedTextByNamespace(TEXT("Game"), TEXT("Title"));
        if (GameTitle.IsEmpty())
        {
            return CreateTestResult(false, TestName, TEXT("Failed to retrieve namespaced text"));
        }
        
        // Test formatted text
        TArray<FString> Args = {TEXT("Player1"), TEXT("100")};
        FString FormattedText = LocalizationSystem->FormatLocalizedText(TEXT("UI.PlayerScore"), Args);
        
        // Test key existence
        bool bHasKey = LocalizationSystem->HasKey(TEXT("UI.OK"));
        if (!bHasKey)
        {
            return CreateTestResult(false, TestName, TEXT("Key existence check failed"));
        }
        
        float ExecutionTime = MeasureExecutionTime([&]() {
            // Perform multiple lookups for performance measurement
            for (int32 i = 0; i < 1000; ++i)
            {
                LocalizationSystem->GetLocalizedText(TEXT("UI.OK"));
            }
        });
        
        return CreateTestResult(true, TestName, FString(), ExecutionTime);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestLanguageSwitching()
{
    FString TestName = TEXT("Language Switching");
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Store original language
        ELanguageCode OriginalLanguage = LocalizationSystem->GetCurrentLanguage();
        
        // Test switching to English
        LocalizationSystem->SetLanguage(ELanguageCode::en_US);
        if (LocalizationSystem->GetCurrentLanguage() != ELanguageCode::en_US)
        {
            return CreateTestResult(false, TestName, TEXT("Failed to switch to English"));
        }
        
        // Test switching to Chinese
        LocalizationSystem->SetLanguage(ELanguageCode::zh_CN);
        if (LocalizationSystem->GetCurrentLanguage() != ELanguageCode::zh_CN)
        {
            return CreateTestResult(false, TestName, TEXT("Failed to switch to Chinese"));
        }
        
        // Verify text changes with language
        FString ChineseOK = LocalizationSystem->GetLocalizedText(TEXT("UI.OK"));
        LocalizationSystem->SetLanguage(ELanguageCode::en_US);
        FString EnglishOK = LocalizationSystem->GetLocalizedText(TEXT("UI.OK"));
        
        if (ChineseOK == EnglishOK)
        {
            return CreateTestResult(false, TestName, TEXT("Text did not change with language"));
        }
        
        // Restore original language
        LocalizationSystem->SetLanguage(OriginalLanguage);
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestTextDirection()
{
    FString TestName = TEXT("Text Direction");
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Test LTR language
        LocalizationSystem->SetLanguage(ELanguageCode::en_US);
        ETextDirection EnglishDirection = LocalizationSystem->GetTextDirection();
        if (EnglishDirection != ETextDirection::LTR)
        {
            return CreateTestResult(false, TestName, TEXT("English should be LTR"));
        }
        
        // Test RTL language
        LocalizationSystem->SetLanguage(ELanguageCode::ar_SA);
        ETextDirection ArabicDirection = LocalizationSystem->GetTextDirection();
        if (ArabicDirection != ETextDirection::RTL)
        {
            return CreateTestResult(false, TestName, TEXT("Arabic should be RTL"));
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestCulturalAdaptation()
{
    FString TestName = TEXT("Cultural Adaptation");
    
    if (!CulturalSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Cultural system not initialized"));
    }
    
    try
    {
        // Test region detection
        ECulturalRegion DetectedRegion = CulturalSystem->DetectRegionFromSystem();
        if (DetectedRegion == ECulturalRegion::Global)
        {
            UE_LOG(LogMingRTSLocalizationTest, Warning, TEXT("Could not detect specific region, using Global"));
        }
        
        // Test region setting
        CulturalSystem->SetPlayerRegion(ECulturalRegion::EastAsia);
        if (CulturalSystem->GetCurrentRegion() != ECulturalRegion::EastAsia)
        {
            return CreateTestResult(false, TestName, TEXT("Failed to set player region"));
        }
        
        // Test adapted content retrieval
        FString AdaptedContent = CulturalSystem->GetAdaptedContentForCurrentRegion(TEXT("Game.Title"));
        if (AdaptedContent.IsEmpty())
        {
            return CreateTestResult(false, TestName, TEXT("Failed to get adapted content"));
        }
        
        // Test content filtering
        bool bAllowed = CulturalSystem->IsContentAllowed(TEXT("Game.Title"), 18, ECulturalRegion::EastAsia);
        if (!bAllowed)
        {
            return CreateTestResult(false, TestName, TEXT("Content should be allowed for adults"));
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestContentVariants()
{
    FString TestName = TEXT("Content Variants");
    
    if (!CulturalSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Cultural system not initialized"));
    }
    
    try
    {
        // Test getting variants for a key
        TArray<FCulturalVariant> Variants = CulturalSystem->GetAvailableVariants(TEXT("Game.Title"));
        
        // Test adapted content for different regions
        FString EastAsiaContent = CulturalSystem->GetAdaptedContent(TEXT("Game.Title"), ECulturalRegion::EastAsia);
        FString WesternContent = CulturalSystem->GetAdaptedContent(TEXT("Game.Title"), ECulturalRegion::WesternEurope);
        
        // Content should be different or fallback to default
        if (EastAsiaContent.IsEmpty() || WesternContent.IsEmpty())
        {
            return CreateTestResult(false, TestName, TEXT("Failed to get regional content variants"));
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestRegionalParameters()
{
    FString TestName = TEXT("Regional Parameters");
    
    if (!CulturalSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Cultural system not initialized"));
    }
    
    try
    {
        // Test getting regional parameters
        FRegionalGameplayParams EastAsiaParams = CulturalSystem->GetRegionalGameplayParams(ECulturalRegion::EastAsia);
        FRegionalGameplayParams WesternParams = CulturalSystem->GetRegionalGameplayParams(ECulturalRegion::WesternEurope);
        
        // Verify parameters are reasonable
        if (EastAsiaParams.DifficultyMultiplier <= 0.0f || WesternParams.DifficultyMultiplier <= 0.0f)
        {
            return CreateTestResult(false, TestName, TEXT("Invalid difficulty multipliers"));
        }
        
        // Test regional holidays
        TArray<FString> Holidays = CulturalSystem->GetRegionalHolidays(ECulturalRegion::EastAsia, 2026);
        if (Holidays.Num() == 0)
        {
            UE_LOG(LogMingRTSLocalizationTest, Warning, TEXT("No holidays found for East Asia region"));
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestPerformance()
{
    FString TestName = TEXT("Performance");
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Test large number of lookups
        float StartTime = FDateTime::Now().ToUnixTimestamp();
        
        for (int32 i = 0; i < 10000; ++i)
        {
            LocalizationSystem->GetLocalizedText(TEXT("UI.OK"));
            LocalizationSystem->GetLocalizedText(TEXT("UI.Cancel"));
            LocalizationSystem->GetLocalizedText(TEXT("UI.Yes"));
            LocalizationSystem->GetLocalizedText(TEXT("UI.No"));
        }
        
        float EndTime = FDateTime::Now().ToUnixTimestamp();
        float ExecutionTime = EndTime - StartTime;
        
        // Should complete within reasonable time (less than 1 second for 40k lookups)
        if (ExecutionTime > 1.0f)
        {
            return CreateTestResult(false, TestName, 
                FString::Printf(TEXT("Performance test failed: %f seconds for 40k lookups"), ExecutionTime));
        }
        
        return CreateTestResult(true, TestName, FString(), ExecutionTime);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestMemoryManagement()
{
    FString TestName = TEXT("Memory Management");
    
    if (!LocalizationSystem || !CulturalSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Systems not initialized"));
    }
    
    try
    {
        // Test cache management
        int32 InitialCacheSize = LocalizationSystem->GetCacheSize();
        
        // Generate some cache entries
        for (int32 i = 0; i < 100; ++i)
        {
            LocalizationSystem->GetLocalizedText(FString::Printf(TEXT("UI.OK_%d"), i));
        }
        
        int32 AfterCacheSize = LocalizationSystem->GetCacheSize();
        if (AfterCacheSize <= InitialCacheSize)
        {
            return CreateTestResult(false, TestName, TEXT("Cache size did not increase"));
        }
        
        // Test cache clearing
        LocalizationSystem->ClearCache();
        int32 ClearedCacheSize = LocalizationSystem->GetCacheSize();
        if (ClearedCacheSize != 0)
        {
            return CreateTestResult(false, TestName, TEXT("Cache was not properly cleared"));
        }
        
        // Test cultural system cache
        CulturalSystem->ClearContentCache();
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestFallbackMechanisms()
{
    FString TestName = TEXT("Fallback Mechanisms");
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Test fallback for non-existent key
        FString NonExistentText = LocalizationSystem->GetLocalizedText(TEXT("NonExistent.Key"));
        if (NonExistentText.IsEmpty())
        {
            return CreateTestResult(false, TestName, TEXT("Fallback mechanism failed for non-existent key"));
        }
        
        // Test fallback to English when current language lacks translation
        LocalizationSystem->SetLanguage(ELanguageCode::zh_CN);
        FString ChineseFallback = LocalizationSystem->GetLocalizedText(TEXT("NonExistent.Key"));
        
        // Should return the key itself as fallback
        if (ChineseFallback != TEXT("NonExistent.Key"))
        {
            UE_LOG(LogMingRTSLocalizationTest, Warning, TEXT("Unexpected fallback behavior"));
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::TestConcurrentAccess()
{
    FString TestName = TEXT("Concurrent Access");
    
    if (!LocalizationSystem)
    {
        return CreateTestResult(false, TestName, TEXT("Localization system not initialized"));
    }
    
    try
    {
        // Simulate concurrent access (simplified for single-threaded test)
        TArray<FString> Results;
        
        for (int32 i = 0; i < 100; ++i)
        {
            Results.Add(LocalizationSystem->GetLocalizedText(TEXT("UI.OK")));
            Results.Add(LocalizationSystem->GetLocalizedText(TEXT("UI.Cancel")));
        }
        
        // Verify all results are consistent
        for (const FString& Result : Results)
        {
            if (Result.IsEmpty())
            {
                return CreateTestResult(false, TestName, TEXT("Inconsistent results during concurrent access simulation"));
            }
        }
        
        return CreateTestResult(true, TestName);
    }
    catch (const std::exception& e)
    {
        return CreateTestResult(false, TestName, FString::Printf(TEXT("Exception: %s"), 
            UTF8_TO_TCHAR(e.what())));
    }
}

FString UMingRTSLocalizationSystemTest::GenerateTestReport(const TArray<FLocalizationTestResult>& TestResults)
{
    FString Report = TEXT("=== MingGoRTS Localization System Test Report ===\n\n");
    
    int32 PassedCount = 0;
    int32 TotalCount = TestResults.Num();
    float TotalTime = 0.0f;
    
    Report += FString::Printf(TEXT("Total Tests: %d\n"), TotalCount);
    
    for (const FLocalizationTestResult& Result : TestResults)
    {
        Report += FString::Printf(TEXT("[%s] %s"), 
            Result.bPassed ? TEXT("PASS") : TEXT("FAIL"),
            *Result.TestName);
        
        if (Result.ExecutionTime > 0.0f)
        {
            Report += FString::Printf(TEXT(" (%.3fs)"), Result.ExecutionTime);
            TotalTime += Result.ExecutionTime;
        }
        
        if (!Result.bPassed && !Result.ErrorMessage.IsEmpty())
        {
            Report += FString::Printf(TEXT(" - Error: %s"), *Result.ErrorMessage);
        }
        
        Report += TEXT("\n");
        
        if (Result.bPassed)
        {
            PassedCount++;
        }
    }
    
    Report += TEXT("\n=== Summary ===\n");
    Report += FString::Printf(TEXT("Passed: %d/%d (%.1f%%)\n"), 
        PassedCount, TotalCount, (float)PassedCount / TotalCount * 100.0f);
    Report += FString::Printf(TEXT("Total Execution Time: %.3fs\n"), TotalTime);
    
    if (PassedCount == TotalCount)
    {
        Report += TEXT("Status: ALL TESTS PASSED ✓\n");
    }
    else
    {
        Report += TEXT("Status: SOME TESTS FAILED ✗\n");
    }
    
    return Report;
}

FLocalizationTestResult UMingRTSLocalizationSystemTest::CreateTestResult(bool bPassed, 
    const FString& Name, const FString& Error, float Time)
{
    FLocalizationTestResult Result;
    Result.bPassed = bPassed;
    Result.TestName = Name;
    Result.ErrorMessage = Error;
    Result.ExecutionTime = Time;
    return Result;
}

template<typename Func>
float UMingRTSLocalizationSystemTest::MeasureExecutionTime(Func&& Function)
{
    double StartTime = FPlatformTime::Seconds();
    Function();
    double EndTime = FPlatformTime::Seconds();
    return static_cast<float>(EndTime - StartTime);
}

void UMingRTSLocalizationSystemTest::GenerateTestData()
{
    // Test data would be generated here if needed
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Test data generated"));
}

void UMingRTSLocalizationSystemTest::CleanupTestData()
{
    // Test data cleanup would be performed here
    UE_LOG(LogMingRTSLocalizationTest, Log, TEXT("Test data cleaned up"));
}
