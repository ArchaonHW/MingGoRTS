// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization System Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSLocalizationSystemTest.generated.h"

class UMingRTSLocalizationSystem;
class UMingRTSCulturalAdaptationSystem;

/**
 * Test results structure
 */
USTRUCT(BlueprintType)
struct FLocalizationTestResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bPassed;
    
    UPROPERTY(BlueprintReadOnly)
    FString TestName;
    
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;
    
    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;
    
    FLocalizationTestResult()
        : bPassed(false)
        , ExecutionTime(0.0f)
    {}
};

/**
 * Comprehensive test suite for localization and cultural adaptation systems
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSLocalizationSystemTest : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSLocalizationSystemTest();
    
    /** Run all localization tests */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    TArray<FLocalizationTestResult> RunAllTests();
    
    /** Test basic localization functionality */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestBasicLocalization();
    
    /** Test language switching */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestLanguageSwitching();
    
    /** Test text direction support */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestTextDirection();
    
    /** Test cultural adaptation system */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestCulturalAdaptation();
    
    /** Test content variants */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestContentVariants();
    
    /** Test regional parameters */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestRegionalParameters();
    
    /** Test performance with large datasets */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestPerformance();
    
    /** Test memory management */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestMemoryManagement();
    
    /** Test fallback mechanisms */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestFallbackMechanisms();
    
    /** Test concurrent access */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FLocalizationTestResult TestConcurrentAccess();
    
    /** Generate test report */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    FString GenerateTestReport(const TArray<FLocalizationTestResult>& TestResults);
    
    /** Initialize test systems */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    void InitializeTestSystems();
    
    /** Cleanup test systems */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Test")
    void CleanupTestSystems();

private:
    /** Localization system instance */
    UPROPERTY()
    TObjectPtr<UMingRTSLocalizationSystem> LocalizationSystem;
    
    /** Cultural adaptation system instance */
    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> CulturalSystem;
    
    /** Helper method to create test result */
    FLocalizationTestResult CreateTestResult(bool bPassed, const FString& Name, 
        const FString& Error = FString(), float Time = 0.0f);
    
    /** Measure execution time */
    template<typename Func>
    float MeasureExecutionTime(Func&& Function);
    
    /** Validate text content */
    bool ValidateTextContent(const FString& Key, const FString& ExpectedLanguage);
    
    /** Test data generation */
    void GenerateTestData();
    
    /** Cleanup test data */
    void CleanupTestData();
};

