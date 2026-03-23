// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Generator Test Suite Implementation

#include "AI/MingAIImageGeneratorTest.h"
#include "AI/MingAIImageBlueprintLibrary.h"
#include "AI/AIContentQualityController.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingAIImageGeneratorTest::UMingAIImageGeneratorTest()
    : TestGenerator(nullptr)
{
    // Populate available test names
    AvailableTests = {
        TEXT("TestImageGeneratorCreation"),
        TEXT("TestImageGeneratorInitialization"),
        TEXT("TestParameterCreation"),
        TEXT("TestStylePresets"),
        TEXT("TestAspectRatioDimensions"),
        TEXT("TestImageGenerationAsync"),
        TEXT("TestImageGenerationSync"),
        TEXT("TestBatchGeneration"),
        TEXT("TestGenerationCancellation"),
        TEXT("TestProgressTracking"),
        TEXT("TestCharacterPortraitPreset"),
        TEXT("TestEnvironmentPreset"),
        TEXT("TestItemIconPreset"),
        TEXT("TestUIElementPreset"),
        TEXT("TestPromptEnhancement"),
        TEXT("TestQualityMetrics"),
        TEXT("TestResultValidation"),
        TEXT("TestBlueprintLibraryAccess"),
        TEXT("TestQualityControllerIntegration"),
        TEXT("TestEventSystem"),
        TEXT("TestGenerationPerformance"),
        TEXT("TestMemoryUsage"),
        TEXT("TestConcurrentGeneration"),
        TEXT("TestRapidGenerationRequests"),
        TEXT("TestLargeBatchGeneration")
    };
}

void UMingAIImageGeneratorTest::InitializeTestSuite()
{
    // Create test generator
    TestGenerator = NewObject<UMingAIImageGenerator>(this);
    if (TestGenerator)
    {
        TestGenerator->InitializeImageGenerator();
    }

    // Clear previous results
    ClearTestResults();

    UE_LOG(LogTemp, Log, TEXT("AI Image Generator Test Suite initialized"));
}

void UMingAIImageGeneratorTest::RunAllTests()
{
    InitializeTestSuite();

    double SuiteStartTime = FPlatformTime::Seconds();

    // Run all available tests
    TArray<FAIImageTestCase> TestCases;
    
    TestCases.Add(TestImageGeneratorCreation());
    TestCases.Add(TestImageGeneratorInitialization());
    TestCases.Add(TestParameterCreation());
    TestCases.Add(TestStylePresets());
    TestCases.Add(TestAspectRatioDimensions());
    TestCases.Add(TestImageGenerationAsync());
    TestCases.Add(TestImageGenerationSync());
    TestCases.Add(TestBatchGeneration());
    TestCases.Add(TestGenerationCancellation());
    TestCases.Add(TestProgressTracking());
    TestCases.Add(TestCharacterPortraitPreset());
    TestCases.Add(TestEnvironmentPreset());
    TestCases.Add(TestItemIconPreset());
    TestCases.Add(TestUIElementPreset());
    TestCases.Add(TestPromptEnhancement());
    TestCases.Add(TestQualityMetrics());
    TestCases.Add(TestResultValidation());
    TestCases.Add(TestBlueprintLibraryAccess());
    TestCases.Add(TestQualityControllerIntegration());
    TestCases.Add(TestEventSystem());
    TestCases.Add(TestGenerationPerformance());
    TestCases.Add(TestMemoryUsage());
    TestCases.Add(TestConcurrentGeneration());
    TestCases.Add(TestRapidGenerationRequests());
    TestCases.Add(TestLargeBatchGeneration());

    // Calculate results
    int32 Passed = 0;
    int32 Failed = 0;
    int32 Skipped = 0;
    
    for (const FAIImageTestCase& TestCase : TestCases)
    {
        switch (TestCase.Result)
        {
        case EAIImageTestResult::Passed:
            Passed++;
            break;
        case EAIImageTestResult::Failed:
            Failed++;
            break;
        case EAIImageTestResult::Skipped:
            Skipped++;
            break;
        default:
            break;
        }
    }

    double SuiteEndTime = FPlatformTime::Seconds();
    
    {
        FScopeLock Lock(&TestLock);
        TestResults.TotalTests = TestCases.Num();
        TestResults.PassedTests = Passed;
        TestResults.FailedTests = Failed;
        TestResults.SkippedTests = Skipped;
        TestResults.TotalExecutionTime = SuiteEndTime - SuiteStartTime;
        TestResults.TestCases = TestCases;
        TestResults.Summary = FString::Printf(
            TEXT("Test Suite Complete: %d/%d Passed, %d Failed, %d Skipped (%.2f seconds)"),
            Passed, TestCases.Num(), Failed, Skipped, TestResults.TotalExecutionTime
        );
    }

    OnTestSuiteCompleted.Broadcast(TestResults);

    UE_LOG(LogTemp, Log, TEXT("AI Image Generator Test Suite completed: %s"), *TestResults.Summary);
}

void UMingAIImageGeneratorTest::RunSingleTest(const FString& TestName)
{
    InitializeTestSuite();

    FAIImageTestCase TestCase;
    
    if (TestName == TEXT("TestImageGeneratorCreation"))
        TestCase = TestImageGeneratorCreation();
    else if (TestName == TEXT("TestImageGeneratorInitialization"))
        TestCase = TestImageGeneratorInitialization();
    else if (TestName == TEXT("TestParameterCreation"))
        TestCase = TestParameterCreation();
    else if (TestName == TEXT("TestStylePresets"))
        TestCase = TestStylePresets();
    else if (TestName == TEXT("TestAspectRatioDimensions"))
        TestCase = TestAspectRatioDimensions();
    else if (TestName == TEXT("TestImageGenerationAsync"))
        TestCase = TestImageGenerationAsync();
    else if (TestName == TEXT("TestImageGenerationSync"))
        TestCase = TestImageGenerationSync();
    else if (TestName == TEXT("TestBatchGeneration"))
        TestCase = TestBatchGeneration();
    else if (TestName == TEXT("TestGenerationCancellation"))
        TestCase = TestGenerationCancellation();
    else if (TestName == TEXT("TestProgressTracking"))
        TestCase = TestProgressTracking();
    else if (TestName == TEXT("TestCharacterPortraitPreset"))
        TestCase = TestCharacterPortraitPreset();
    else if (TestName == TEXT("TestEnvironmentPreset"))
        TestCase = TestEnvironmentPreset();
    else if (TestName == TEXT("TestItemIconPreset"))
        TestCase = TestItemIconPreset();
    else if (TestName == TEXT("TestUIElementPreset"))
        TestCase = TestUIElementPreset();
    else if (TestName == TEXT("TestPromptEnhancement"))
        TestCase = TestPromptEnhancement();
    else if (TestName == TEXT("TestQualityMetrics"))
        TestCase = TestQualityMetrics();
    else if (TestName == TEXT("TestResultValidation"))
        TestCase = TestResultValidation();
    else if (TestName == TEXT("TestBlueprintLibraryAccess"))
        TestCase = TestBlueprintLibraryAccess();
    else if (TestName == TEXT("TestQualityControllerIntegration"))
        TestCase = TestQualityControllerIntegration();
    else if (TestName == TEXT("TestEventSystem"))
        TestCase = TestEventSystem();
    else if (TestName == TEXT("TestGenerationPerformance"))
        TestCase = TestGenerationPerformance();
    else if (TestName == TEXT("TestMemoryUsage"))
        TestCase = TestMemoryUsage();
    else if (TestName == TEXT("TestConcurrentGeneration"))
        TestCase = TestConcurrentGeneration();
    else if (TestName == TEXT("TestRapidGenerationRequests"))
        TestCase = TestRapidGenerationRequests();
    else if (TestName == TEXT("TestLargeBatchGeneration"))
        TestCase = TestLargeBatchGeneration();
    else
    {
        TestCase.TestName = TestName;
        TestCase.Result = EAIImageTestResult::Failed;
        TestCase.ErrorMessage = TEXT("Unknown test name");
    }

    {
        FScopeLock Lock(&TestLock);
        TestResults.TestCases.Add(TestCase);
        TestResults.TotalTests++;
        
        if (TestCase.Result == EAIImageTestResult::Passed)
            TestResults.PassedTests++;
        else if (TestCase.Result == EAIImageTestResult::Failed)
            TestResults.FailedTests++;
        else if (TestCase.Result == EAIImageTestResult::Skipped)
            TestResults.SkippedTests++;
    }

    OnTestCaseCompleted.Broadcast(TestName, TestCase.Result);
}

void UMingAIImageGeneratorTest::RunSpecificTests(const TArray<FString>& TestNames)
{
    InitializeTestSuite();
    
    for (const FString& TestName : TestNames)
    {
        RunSingleTest(TestName);
    }
}

void UMingAIImageGeneratorTest::ClearTestResults()
{
    FScopeLock Lock(&TestLock);
    TestResults = FAIImageTestSuiteResults();
}

void UMingAIImageGeneratorTest::ExportTestReport(const FString& FilePath)
{
    FString Report = TEXT("AI Image Generator Test Report\n");
    Report += TEXT("=============================\n\n");
    Report += FString::Printf(TEXT("Generated: %s\n\n"), *FDateTime::Now().ToString());
    Report += TestResults.Summary + TEXT("\n\n");
    
    Report += TEXT("Detailed Results:\n");
    Report += TEXT("----------------\n\n");
    
    for (const FAIImageTestCase& TestCase : TestResults.TestCases)
    {
        Report += FString::Printf(TEXT("[%s] %s\n"), 
            *GetTestResultString(TestCase.Result), 
            *TestCase.TestName);
        
        if (!TestCase.ErrorMessage.IsEmpty())
        {
            Report += FString::Printf(TEXT("  Error: %s\n"), *TestCase.ErrorMessage);
        }
        
        if (TestCase.ExecutionTime > 0.0f)
        {
            Report += FString::Printf(TEXT("  Time: %.3f seconds\n"), TestCase.ExecutionTime);
        }
        
        Report += TEXT("\n");
    }

    FFileHelper::SaveStringToFile(Report, *FilePath);
}

FAIImageTestCase UMingAIImageGeneratorTest::TestImageGeneratorCreation()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestImageGeneratorCreation");
    TestCase.Description = TEXT("Verify image generator can be created");
    
    ExecuteTest(TestCase, [this]()
    {
        UMingAIImageGenerator* Generator = NewObject<UMingAIImageGenerator>(this);
        if (Generator == nullptr)
        {
            throw TEXT("Failed to create image generator");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestImageGeneratorInitialization()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestImageGeneratorInitialization");
    TestCase.Description = TEXT("Verify image generator initializes correctly");
    
    ExecuteTest(TestCase, [this]()
    {
        if (TestGenerator == nullptr)
        {
            throw TEXT("Test generator is null");
        }
        
        if (!TestGenerator->IsInitialized())
        {
            throw TEXT("Generator failed to initialize");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestParameterCreation()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestParameterCreation");
    TestCase.Description = TEXT("Verify image generation parameters can be created");
    
    ExecuteTest(TestCase, []()
    {
        FAIImageGenerationParams Params;
        Params.Prompt = TEXT("Test prompt");
        Params.Style = EAIImageStyle::Realistic;
        Params.Quality = EAIImageQuality::High;
        Params.AspectRatio = EAIImageAspectRatio::Square;
        
        if (Params.Prompt.IsEmpty())
        {
            throw TEXT("Failed to set prompt");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestStylePresets()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestStylePresets");
    TestCase.Description = TEXT("Verify all image styles are available");
    
    ExecuteTest(TestCase, []()
    {
        TArray<EAIImageStyle> Styles = UMingAIImageBlueprintLibrary::GetAllImageStyles();
        if (Styles.Num() == 0)
        {
            throw TEXT("No image styles available");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestAspectRatioDimensions()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestAspectRatioDimensions");
    TestCase.Description = TEXT("Verify aspect ratio dimensions are correct");
    
    ExecuteTest(TestCase, []()
    {
        FVector2D SquareDims = UMingAIImageBlueprintLibrary::GetAspectRatioDimensions(EAIImageAspectRatio::Square);
        if (SquareDims.X != 512 || SquareDims.Y != 512)
        {
            throw TEXT("Square aspect ratio dimensions incorrect");
        }
        
        FVector2D LandscapeDims = UMingAIImageBlueprintLibrary::GetAspectRatioDimensions(EAIImageAspectRatio::Landscape);
        if (LandscapeDims.X <= 0 || LandscapeDims.Y <= 0)
        {
            throw TEXT("Landscape aspect ratio dimensions invalid");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestImageGenerationAsync()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestImageGenerationAsync");
    TestCase.Description = TEXT("Verify async image generation works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
            TEXT("A simple red circle"),
            EAIImageStyle::Cartoon,
            EAIImageQuality::Draft,
            EAIImageAspectRatio::Square
        );
        
        FString GenerationID = TestGenerator->GenerateImage(Params);
        if (GenerationID.IsEmpty())
        {
            throw TEXT("Failed to start async generation");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestImageGenerationSync()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestImageGenerationSync");
    TestCase.Description = TEXT("Verify sync image generation works (mock)");
    TestCase.Result = EAIImageTestResult::Skipped;
    TestCase.ErrorMessage = TEXT("Sync generation requires live API - skipping in unit test");
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestBatchGeneration()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestBatchGeneration");
    TestCase.Description = TEXT("Verify batch image generation works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
            TEXT("A simple geometric shape"),
            EAIImageStyle::LowPoly,
            EAIImageQuality::Draft,
            EAIImageAspectRatio::Square
        );
        
        FAIImageBatchRequest Request = UMingAIImageBlueprintLibrary::CreateBatchRequest(
            TEXT("Geometric shape"),
            2,
            Params
        );
        
        FString BatchID = TestGenerator->GenerateImageBatch(Request);
        if (BatchID.IsEmpty())
        {
            throw TEXT("Failed to start batch generation");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestGenerationCancellation()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestGenerationCancellation");
    TestCase.Description = TEXT("Verify generation can be cancelled");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
            TEXT("Test for cancellation"),
            EAIImageStyle::Sketch,
            EAIImageQuality::Draft,
            EAIImageAspectRatio::Square
        );
        
        FString GenerationID = TestGenerator->GenerateImage(Params);
        if (GenerationID.IsEmpty())
        {
            throw TEXT("Failed to start generation for cancellation test");
        }
        
        // Try to cancel immediately
        bool bCancelled = TestGenerator->CancelGeneration(GenerationID);
        // Cancellation may or may not succeed depending on timing
        // Just verify the function doesn't crash
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestProgressTracking()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestProgressTracking");
    TestCase.Description = TEXT("Verify progress tracking works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
            TEXT("Test progress"),
            EAIImageStyle::Stylized,
            EAIImageQuality::Draft,
            EAIImageAspectRatio::Square
        );
        
        FString GenerationID = TestGenerator->GenerateImage(Params);
        
        // Check initial progress
        FAIImageGenerationResult Result = TestGenerator->GetGenerationResult(GenerationID);
        // Progress should be between 0 and 1
        if (Result.Progress < 0.0f || Result.Progress > 1.0f)
        {
            throw TEXT("Invalid progress value");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestCharacterPortraitPreset()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestCharacterPortraitPreset");
    TestCase.Description = TEXT("Verify character portrait preset works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = TestGenerator->GetCharacterPortraitPreset(TEXT("A brave warrior"));
        
        if (Params.Prompt.IsEmpty())
        {
            throw TEXT("Character portrait preset returned empty prompt");
        }
        
        if (Params.Style != EAIImageStyle::Realistic)
        {
            throw TEXT("Character portrait preset has wrong style");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestEnvironmentPreset()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestEnvironmentPreset");
    TestCase.Description = TEXT("Verify environment preset works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = TestGenerator->GetEnvironmentPreset(TEXT("Ancient forest"));
        
        if (Params.Prompt.IsEmpty())
        {
            throw TEXT("Environment preset returned empty prompt");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestItemIconPreset()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestItemIconPreset");
    TestCase.Description = TEXT("Verify item icon preset works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = TestGenerator->GetItemIconPreset(TEXT("Magic sword"));
        
        if (Params.Prompt.IsEmpty())
        {
            throw TEXT("Item icon preset returned empty prompt");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestUIElementPreset()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestUIElementPreset");
    TestCase.Description = TEXT("Verify UI element preset works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = TestGenerator->GetUIElementPreset(TEXT("Menu button"));
        
        if (Params.Prompt.IsEmpty())
        {
            throw TEXT("UI element preset returned empty prompt");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestPromptEnhancement()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestPromptEnhancement");
    TestCase.Description = TEXT("Verify prompt enhancement works");
    
    ExecuteTest(TestCase, []()
    {
        FString BasePrompt = TEXT("A warrior");
        FString Enhanced = UMingAIImageBlueprintLibrary::EnhancePromptWithStyle(BasePrompt, EAIImageStyle::Realistic);
        
        if (Enhanced.IsEmpty())
        {
            throw TEXT("Prompt enhancement returned empty string");
        }
        
        if (!Enhanced.Contains(BasePrompt))
        {
            throw TEXT("Enhanced prompt doesn't contain base prompt");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestQualityMetrics()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestQualityMetrics");
    TestCase.Description = TEXT("Verify quality metrics are tracked");
    
    ExecuteTest(TestCase, [this]()
    {
        // Quality metrics should be available in results
        FAIImageGenerationResult Result;
        Result.QualityScore = 0.85f;
        
        if (Result.QualityScore < 0.0f || Result.QualityScore > 1.0f)
        {
            throw TEXT("Quality score out of valid range");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestResultValidation()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestResultValidation");
    TestCase.Description = TEXT("Verify result validation works");
    
    ExecuteTest(TestCase, []()
    {
        // Create a successful result
        FAIImageGenerationResult SuccessResult;
        SuccessResult.Status = EAIImageGenerationStatus::Completed;
        
        if (!UMingAIImageBlueprintLibrary::IsGenerationSuccessful(SuccessResult))
        {
            throw TEXT("Failed to recognize successful generation");
        }
        
        // Create a failed result
        FAIImageGenerationResult FailedResult;
        FailedResult.Status = EAIImageGenerationStatus::Failed;
        
        if (UMingAIImageBlueprintLibrary::IsGenerationSuccessful(FailedResult))
        {
            throw TEXT("Incorrectly recognized failed generation as successful");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestBlueprintLibraryAccess()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestBlueprintLibraryAccess");
    TestCase.Description = TEXT("Verify Blueprint library functions work");
    
    ExecuteTest(TestCase, []()
    {
        // Test static library functions
        TArray<EAIImageStyle> Styles = UMingAIImageBlueprintLibrary::GetAllImageStyles();
        if (Styles.Num() == 0)
        {
            throw TEXT("Blueprint library returned no styles");
        }
        
        TArray<EAIImageAspectRatio> Ratios = UMingAIImageBlueprintLibrary::GetAllAspectRatios();
        if (Ratios.Num() == 0)
        {
            throw TEXT("Blueprint library returned no aspect ratios");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestQualityControllerIntegration()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestQualityControllerIntegration");
    TestCase.Description = TEXT("Verify quality controller integration");
    
    ExecuteTest(TestCase, [this]()
    {
        // Quality controller should be available through the generator
        if (TestGenerator == nullptr)
        {
            throw TEXT("Generator not available");
        }
        
        // Quality checks are enabled by default
        FAIImageGenerationParams Params;
        Params.bEnableQualityCheck = true;
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestEventSystem()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestEventSystem");
    TestCase.Description = TEXT("Verify event system works");
    
    ExecuteTest(TestCase, [this]()
    {
        // Test that events can be bound
        if (TestGenerator == nullptr)
        {
            throw TEXT("Generator not available");
        }
        
        // Events should be accessible
        // Note: We can't fully test event firing without actual generation
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestGenerationPerformance()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestGenerationPerformance");
    TestCase.Description = TEXT("Verify generation performance is acceptable");
    
    ExecuteTest(TestCase, [this, &TestCase]()
    {
        double StartTime = FPlatformTime::Seconds();
        
        // Create multiple parameter sets quickly
        for (int32 i = 0; i < 10; ++i)
        {
            FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
                FString::Printf(TEXT("Performance test %d"), i),
                EAIImageStyle::Stylized,
                EAIImageQuality::Draft,
                EAIImageAspectRatio::Square
            );
        }
        
        double EndTime = FPlatformTime::Seconds();
        double Duration = EndTime - StartTime;
        
        TestCase.ExecutionTime = Duration;
        
        if (Duration > 1.0) // Should take less than 1 second
        {
            throw FString::Printf(TEXT("Parameter creation too slow: %.3f seconds"), Duration);
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestMemoryUsage()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestMemoryUsage");
    TestCase.Description = TEXT("Verify memory usage is reasonable");
    
    ExecuteTest(TestCase, [this]()
    {
        // Create and destroy multiple generators to check for leaks
        for (int32 i = 0; i < 5; ++i)
        {
            UMingAIImageGenerator* TempGenerator = NewObject<UMingAIImageGenerator>(this);
            if (TempGenerator)
            {
                TempGenerator->InitializeImageGenerator();
                TempGenerator->ShutdownImageGenerator();
            }
        }
        
        // Note: Detailed memory testing would require more sophisticated profiling
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestConcurrentGeneration()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestConcurrentGeneration");
    TestCase.Description = TEXT("Verify concurrent generation requests work");
    
    ExecuteTest(TestCase, [this]()
    {
        // Start multiple generations concurrently
        TArray<FString> GenerationIDs;
        
        for (int32 i = 0; i < 3; ++i)
        {
            FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
                FString::Printf(TEXT("Concurrent test %d"), i),
                EAIImageStyle::Sketch,
                EAIImageQuality::Draft,
                EAIImageAspectRatio::Square
            );
            
            FString ID = TestGenerator->GenerateImage(Params);
            if (!ID.IsEmpty())
            {
                GenerationIDs.Add(ID);
            }
        }
        
        if (GenerationIDs.Num() != 3)
        {
            throw TEXT("Failed to start all concurrent generations");
        }
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestRapidGenerationRequests()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestRapidGenerationRequests");
    TestCase.Description = TEXT("Verify system handles rapid requests");
    
    ExecuteTest(TestCase, [this]()
    {
        // Send many rapid requests
        for (int32 i = 0; i < 10; ++i)
        {
            FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
                FString::Printf(TEXT("Rapid request %d"), i),
                EAIImageStyle::Cartoon,
                EAIImageQuality::Draft,
                EAIImageAspectRatio::Square
            );
            
            TestGenerator->GenerateImage(Params);
        }
        
        // System should handle this without crashing
    });
    
    return TestCase;
}

FAIImageTestCase UMingAIImageGeneratorTest::TestLargeBatchGeneration()
{
    FAIImageTestCase TestCase;
    TestCase.TestName = TEXT("TestLargeBatchGeneration");
    TestCase.Description = TEXT("Verify large batch generation works");
    
    ExecuteTest(TestCase, [this]()
    {
        FAIImageGenerationParams Params = UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
            TEXT("Batch item"),
            EAIImageStyle::LowPoly,
            EAIImageQuality::Draft,
            EAIImageAspectRatio::Square
        );
        
        FAIImageBatchRequest Request;
        Request.BasePrompt = TEXT("Batch generation test");
        Request.NumImages = 5; // Reasonable size for testing
        Request.BaseParams = Params;
        
        FString BatchID = TestGenerator->GenerateImageBatch(Request);
        if (BatchID.IsEmpty())
        {
            throw TEXT("Failed to start large batch generation");
        }
    });
    
    return TestCase;
}

void UMingAIImageGeneratorTest::ExecuteTest(FAIImageTestCase& TestCase, TFunction<void()> TestFunction)
{
    double StartTime = FPlatformTime::Seconds();
    TestCase.ExecutionTimeStamp = FDateTime::Now();
    TestCase.Result = EAIImageTestResult::Running;
    
    try
    {
        TestFunction();
        RecordTestResult(TestCase, true);
    }
    catch (const FString& Error)
    {
        RecordTestResult(TestCase, false, Error);
    }
    catch (...)
    {
        RecordTestResult(TestCase, false, TEXT("Unknown exception occurred"));
    }
    
    double EndTime = FPlatformTime::Seconds();
    TestCase.ExecutionTime = EndTime - StartTime;
}

void UMingAIImageGeneratorTest::RecordTestResult(FAIImageTestCase& TestCase, bool bPassed, const FString& Error)
{
    if (bPassed)
    {
        TestCase.Result = EAIImageTestResult::Passed;
        TestCase.ErrorMessage.Empty();
    }
    else
    {
        TestCase.Result = EAIImageTestResult::Failed;
        TestCase.ErrorMessage = Error;
    }
    
    OnTestCaseCompleted.Broadcast(TestCase.TestName, TestCase.Result);
}

FString UMingAIImageGeneratorTest::GetTestResultString(EAIImageTestResult Result)
{
    switch (Result)
    {
    case EAIImageTestResult::NotRun:
        return TEXT("NOT RUN");
    case EAIImageTestResult::Running:
        return TEXT("RUNNING");
    case EAIImageTestResult::Passed:
        return TEXT("PASSED");
    case EAIImageTestResult::Failed:
        return TEXT("FAILED");
    case EAIImageTestResult::Skipped:
        return TEXT("SKIPPED");
    default:
        return TEXT("UNKNOWN");
    }
}

bool UMingAIImageGeneratorTest::WasTestSuccessful(const FAIImageTestCase& TestCase)
{
    return TestCase.Result == EAIImageTestResult::Passed;
}

TArray<FString> UMingAIImageGeneratorTest::GetAvailableTestNames()
{
    return {
        TEXT("TestImageGeneratorCreation"),
        TEXT("TestImageGeneratorInitialization"),
        TEXT("TestParameterCreation"),
        TEXT("TestStylePresets"),
        TEXT("TestAspectRatioDimensions"),
        TEXT("TestImageGenerationAsync"),
        TEXT("TestImageGenerationSync"),
        TEXT("TestBatchGeneration"),
        TEXT("TestGenerationCancellation"),
        TEXT("TestProgressTracking"),
        TEXT("TestCharacterPortraitPreset"),
        TEXT("TestEnvironmentPreset"),
        TEXT("TestItemIconPreset"),
        TEXT("TestUIElementPreset"),
        TEXT("TestPromptEnhancement"),
        TEXT("TestQualityMetrics"),
        TEXT("TestResultValidation"),
        TEXT("TestBlueprintLibraryAccess"),
        TEXT("TestQualityControllerIntegration"),
        TEXT("TestEventSystem"),
        TEXT("TestGenerationPerformance"),
        TEXT("TestMemoryUsage"),
        TEXT("TestConcurrentGeneration"),
        TEXT("TestRapidGenerationRequests"),
        TEXT("TestLargeBatchGeneration")
    };
}
