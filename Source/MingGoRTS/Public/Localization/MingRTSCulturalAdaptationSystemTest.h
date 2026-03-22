// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.2: Cultural Adaptation System Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "MingRTSCulturalAdaptationSystemTest.generated.h"

/**
 * Test suite for Cultural Adaptation System
 * Validates all cultural adaptation functionality including:
 * - Region detection and switching
 * - Content adaptation and caching
 * - Cultural preferences management
 * - Regional gameplay parameters
 * - Content rating and filtering
 */
UCLASS()
class MINGGORTS_API UMingRTSCulturalAdaptationSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCulturalAdaptationSystemTest();

    /** Run all cultural adaptation tests */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool RunAllTests();

    /** Test system initialization */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestSystemInitialization();

    /** Test region detection from system settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestRegionDetection();

    /** Test region switching and events */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestRegionSwitching();

    /** Test content adaptation and caching */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestContentAdaptation();

    /** Test cultural preferences management */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestCulturalPreferences();

    /** Test regional gameplay parameters */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestRegionalGameplayParams();

    /** Test content rating and filtering */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestContentRating();

    /** Test cache performance and management */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestCachePerformance();

    /** Test regional holidays and events */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural|Test")
    bool TestRegionalHolidays();

    /** Get comprehensive test results */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural|Test")
    FString GetTestResults() const { return TestResults; }

    /** Get test pass rate */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural|Test")
    float GetTestPassRate() const { return TestPassRate; }

    /** Get total tests run */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural|Test")
    int32 GetTotalTests() const { return TotalTests; }

    /** Get tests passed */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural|Test")
    int32 GetTestsPassed() const { return TestsPassed; }

private:
    /** Cultural adaptation system instance for testing */
    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> TestSystem;

    /** Test results tracking */
    FString TestResults;
    float TestPassRate;
    int32 TotalTests;
    int32 TestsPassed;
    int32 TestsFailed;

    /** Helper methods */
    void LogTest(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    void InitializeTestResults();
    void FinalizeTestResults();
    bool ValidateRegionContent(ECulturalRegion Region);
    bool TestCacheEfficiency();
    void SimulateCacheLoad();
};
