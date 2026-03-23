#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Generator Test Suite
// Comprehensive tests for AI image generation functionality

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingAIImageGenerator.h"
#include "MingAIImageGeneratorTest.generated.h"

UENUM(BlueprintType)
enum class EAIImageTestResult : uint8
{
    NotRun      UMETA(DisplayName = "Not Run"),
    Running     UMETA(DisplayName = "Running"),
    Passed      UMETA(DisplayName = "Passed"),
    Failed      UMETA(DisplayName = "Failed"),
    Skipped     UMETA(DisplayName = "Skipped")
};

USTRUCT(BlueprintType)
struct FAIImageTestCase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    EAIImageTestResult Result;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FDateTime ExecutionTimeStamp;

    FAIImageTestCase()
        : Result(EAIImageTestResult::NotRun)
        , ExecutionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAIImageTestSuiteResults
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    int32 TotalTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    int32 PassedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    int32 FailedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    int32 SkippedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    float TotalExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    TArray<FAIImageTestCase> TestCases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Results")
    FString Summary;

    FAIImageTestSuiteResults()
        : TotalTests(0)
        , PassedTests(0)
        , FailedTests(0)
        , SkippedTests(0)
        , TotalExecutionTime(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestCaseCompleted, FString, TestName, EAIImageTestResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, FAIImageTestSuiteResults, Results);

/**
 * AI Image Generator Test Suite
 * Comprehensive testing for AI image generation functionality
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAIImageGeneratorTest : public UObject
{
    GENERATED_BODY()

public:
    UMingAIImageGeneratorTest();

    // === Test Suite Management ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void InitializeTestSuite();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void RunSingleTest(const FString& TestName);

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void RunSpecificTests(const TArray<FString>& TestNames);

    UFUNCTION(BlueprintPure, Category = "AI Image Tests")
    FAIImageTestSuiteResults GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void ClearTestResults();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests")
    void ExportTestReport(const FString& FilePath);

    // === Individual Test Cases ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Core")
    FAIImageTestCase TestImageGeneratorCreation();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Core")
    FAIImageTestCase TestImageGeneratorInitialization();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Core")
    FAIImageTestCase TestParameterCreation();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Core")
    FAIImageTestCase TestStylePresets();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Core")
    FAIImageTestCase TestAspectRatioDimensions();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Generation")
    FAIImageTestCase TestImageGenerationAsync();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Generation")
    FAIImageTestCase TestImageGenerationSync();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Generation")
    FAIImageTestCase TestBatchGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Generation")
    FAIImageTestCase TestGenerationCancellation();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Generation")
    FAIImageTestCase TestProgressTracking();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Presets")
    FAIImageTestCase TestCharacterPortraitPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Presets")
    FAIImageTestCase TestEnvironmentPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Presets")
    FAIImageTestCase TestItemIconPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Presets")
    FAIImageTestCase TestUIElementPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Utilities")
    FAIImageTestCase TestPromptEnhancement();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Utilities")
    FAIImageTestCase TestQualityMetrics();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Utilities")
    FAIImageTestCase TestResultValidation();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Integration")
    FAIImageTestCase TestBlueprintLibraryAccess();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Integration")
    FAIImageTestCase TestQualityControllerIntegration();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Integration")
    FAIImageTestCase TestEventSystem();

    // === Performance Tests ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Performance")
    FAIImageTestCase TestGenerationPerformance();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Performance")
    FAIImageTestCase TestMemoryUsage();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Performance")
    FAIImageTestCase TestConcurrentGeneration();

    // === Stress Tests ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Stress")
    FAIImageTestCase TestRapidGenerationRequests();

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Stress")
    FAIImageTestCase TestLargeBatchGeneration();

    // === Events ===
    UPROPERTY(BlueprintAssignable, Category = "AI Image Tests Events")
    FOnTestCaseCompleted OnTestCaseCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Image Tests Events")
    FOnTestSuiteCompleted OnTestSuiteCompleted;

    // === Test Helpers ===
    UFUNCTION(BlueprintPure, Category = "AI Image Tests|Helpers")
    static FString GetTestResultString(EAIImageTestResult Result);

    UFUNCTION(BlueprintCallable, Category = "AI Image Tests|Helpers")
    static bool WasTestSuccessful(const FAIImageTestCase& TestCase);

    UFUNCTION(BlueprintPure, Category = "AI Image Tests|Helpers")
    static TArray<FString> GetAvailableTestNames();

protected:
    // === Internal Test Methods ===
    void ExecuteTest(FAIImageTestCase& TestCase, TFunction<void()> TestFunction);
    void RecordTestResult(FAIImageTestCase& TestCase, bool bPassed, const FString& Error = TEXT(""));
    
    bool ValidateImageParams(const FAIImageGenerationParams& Params);
    bool ValidateImageResult(const FAIImageGenerationResult& Result);
    
    UPROPERTY()
    UMingAIImageGenerator* TestGenerator;

    UPROPERTY()
    FAIImageTestSuiteResults TestResults;

    UPROPERTY()
    TArray<FString> AvailableTests;

    FCriticalSection TestLock;
};
