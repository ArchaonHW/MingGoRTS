#include "MingAITaskIntegrationTest.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingAITaskIntegrationTest::UMingAITaskIntegrationTest()
{
}

void UMingAITaskIntegrationTest::InitializeTestSuite()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing AI Task Integration Test Suite..."));
    
    // Create test integration instance
    TestIntegration = NewObject<UMingAITaskIntegration>();
    
    if (TestIntegration)
    {
        TestIntegration->InitializeAITaskIntegration();
    }
    
    // Clear previous test results
    TestResults.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("AI Task Integration Test Suite initialized"));
}

TArray<FTestReport> UMingAITaskIntegrationTest::RunAllTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running all AI Task Integration tests..."));
    
    TestResults.Empty();
    
    // Run all individual tests
    TestResults.Add(TestTaskGeneratorInitialization());
    TestResults.Add(TestBehaviorPredictorInitialization());
    TestResults.Add(TestPersonalizedTaskGeneration());
    TestResults.Add(TestContextualTaskGeneration());
    TestResults.Add(TestPlayerActionRecording());
    TestResults.Add(TestBehaviorPrediction());
    TestResults.Add(TestPatternRecognition());
    TestResults.Add(TestContentPreload());
    TestResults.Add(TestPlayerInsightGeneration());
    TestResults.Add(TestTaskCompletionAnalysis());
    TestResults.Add(TestRealTimePrediction());
    TestResults.Add(TestCacheSystem());
    TestResults.Add(TestEventSystem());
    
    // Log summary
    FString Summary = GetTestSummary(TestResults);
    UE_LOG(LogTemp, Log, TEXT("All tests completed:\n%s"), *Summary);
    
    return TestResults;
}

TArray<FTestReport> UMingAITaskIntegrationTest::RunTestCategory(const FString& Category)
{
    TArray<FTestReport> CategoryResults;
    
    UE_LOG(LogTemp, Log, TEXT("Running tests for category: %s"), *Category);
    
    if (Category.Equals(TEXT("Initialization")))
    {
        CategoryResults.Add(TestTaskGeneratorInitialization());
        CategoryResults.Add(TestBehaviorPredictorInitialization());
    }
    else if (Category.Equals(TEXT("TaskGeneration")))
    {
        CategoryResults.Add(TestPersonalizedTaskGeneration());
        CategoryResults.Add(TestContextualTaskGeneration());
    }
    else if (Category.Equals(TEXT("BehaviorAnalysis")))
    {
        CategoryResults.Add(TestPlayerActionRecording());
        CategoryResults.Add(TestBehaviorPrediction());
        CategoryResults.Add(TestPatternRecognition());
    }
    else if (Category.Equals(TEXT("Integration")))
    {
        CategoryResults.Add(TestPlayerInsightGeneration());
        CategoryResults.Add(TestTaskCompletionAnalysis());
        CategoryResults.Add(TestRealTimePrediction());
        CategoryResults.Add(TestCacheSystem());
        CategoryResults.Add(TestEventSystem());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown test category: %s"), *Category);
    }
    
    return CategoryResults;
}

FTestReport UMingAITaskIntegrationTest::TestTaskGeneratorInitialization()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("TaskGeneratorInitialization"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Test task generator initialization
        FTaskGenerationRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.Context = TEXT("TestContext");
        Request.bForcePersonalization = false;
        Request.Priority = 1;
        
        FDynamicTask Task = TestIntegration->ProcessTaskGenerationRequest(Request);
        
        FString ErrorMessage;
        if (!AssertTaskValid(Task, ErrorMessage))
        {
            return CreateTestReport(TEXT("TaskGeneratorInitialization"), ETestResult::Failed, ErrorMessage);
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("TaskGeneratorInitialization"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("TaskGeneratorInitialization"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestBehaviorPredictorInitialization()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("BehaviorPredictorInitialization"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Test behavior predictor initialization
        FBehaviorAnalysisRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.AnalysisWindowMinutes = 10.0f;
        Request.bIncludePredictions = true;
        
        TArray<FPredictionResult> Predictions = TestIntegration->ProcessBehaviorAnalysisRequest(Request);
        
        // Should return at least one prediction even for new player
        if (Predictions.Num() == 0)
        {
            return CreateTestReport(TEXT("BehaviorPredictorInitialization"), ETestResult::Failed, TEXT("No predictions returned"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("BehaviorPredictorInitialization"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("BehaviorPredictorInitialization"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestPersonalizedTaskGeneration()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("PersonalizedTaskGeneration"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Update player profile first
        FPlayerBehaviorProfile Profile = CreateTestProfile();
        TestIntegration->UpdatePlayerBehaviorProfile(TEXT("TestPlayer"), Profile);
        
        // Generate personalized task
        FTaskGenerationRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.Context = TEXT("Combat");
        Request.bForcePersonalization = true;
        Request.Priority = 1;
        
        FDynamicTask Task = TestIntegration->ProcessTaskGenerationRequest(Request);
        
        FString ErrorMessage;
        if (!AssertTaskValid(Task, ErrorMessage))
        {
            return CreateTestReport(TEXT("PersonalizedTaskGeneration"), ETestResult::Failed, ErrorMessage);
        }
        
        if (!Task.bIsPersonalized)
        {
            return CreateTestReport(TEXT("PersonalizedTaskGeneration"), ETestResult::Failed, TEXT("Task is not personalized"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("PersonalizedTaskGeneration"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("PersonalizedTaskGeneration"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestContextualTaskGeneration()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("ContextualTaskGeneration"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Generate contextual task
        FTaskGenerationRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.Context = TEXT("battle");
        Request.bForcePersonalization = false;
        Request.Priority = 1;
        
        FDynamicTask Task = TestIntegration->ProcessTaskGenerationRequest(Request);
        
        FString ErrorMessage;
        if (!AssertTaskValid(Task, ErrorMessage))
        {
            return CreateTestReport(TEXT("ContextualTaskGeneration"), ETestResult::Failed, ErrorMessage);
        }
        
        if (Task.Type != ETaskType::Combat)
        {
            return CreateTestReport(TEXT("ContextualTaskGeneration"), ETestResult::Failed, TEXT("Task type doesn't match context"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("ContextualTaskGeneration"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("ContextualTaskGeneration"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestPlayerActionRecording()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("PlayerActionRecording"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Record multiple actions
        FPlayerAction Action1 = CreateTestAction(EPlayerActionType::MoveUnit);
        FPlayerAction Action2 = CreateTestAction(EPlayerActionType::Attack);
        FPlayerAction Action3 = CreateTestAction(EPlayerActionType::Build);
        
        TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action1);
        TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action2);
        TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action3);
        
        // Verify behavior analysis works
        FBehaviorAnalysisRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.AnalysisWindowMinutes = 10.0f;
        Request.bIncludePredictions = false;
        
        TArray<FPredictionResult> Predictions = TestIntegration->ProcessBehaviorAnalysisRequest(Request);
        
        if (Predictions.Num() == 0)
        {
            return CreateTestReport(TEXT("PlayerActionRecording"), ETestResult::Failed, TEXT("No predictions after recording actions"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("PlayerActionRecording"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("PlayerActionRecording"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestBehaviorPrediction()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("BehaviorPrediction"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Record a pattern of actions
        for (int32 i = 0; i < 10; ++i)
        {
            FPlayerAction Action = CreateTestAction(EPlayerActionType::MoveUnit);
            TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action);
        }
        
        // Get predictions
        TArray<FPredictionResult> Predictions = TestIntegration->GetRealTimePredictions(TEXT("TestPlayer"), 5.0f);
        
        if (Predictions.Num() == 0)
        {
            return CreateTestReport(TEXT("BehaviorPrediction"), ETestResult::Failed, TEXT("No predictions generated"));
        }
        
        // Check prediction validity
        FString ErrorMessage;
        if (!AssertPredictionValid(Predictions[0], ErrorMessage))
        {
            return CreateTestReport(TEXT("BehaviorPrediction"), ETestResult::Failed, ErrorMessage);
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("BehaviorPrediction"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("BehaviorPrediction"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestPatternRecognition()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("PatternRecognition"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Create a repeating pattern
        TArray<EPlayerActionType> Pattern = {EPlayerActionType::MoveUnit, EPlayerActionType::Attack, EPlayerActionType::Build};
        
        // Record pattern multiple times
        for (int32 i = 0; i < 5; ++i)
        {
            for (EPlayerActionType ActionType : Pattern)
            {
                FPlayerAction Action = CreateTestAction(ActionType);
                TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action);
            }
        }
        
        // Get predictions
        TArray<FPredictionResult> Predictions = TestIntegration->GetRealTimePredictions(TEXT("TestPlayer"), 5.0f);
        
        if (Predictions.Num() == 0)
        {
            return CreateTestReport(TEXT("PatternRecognition"), ETestResult::Failed, TEXT("No predictions from pattern"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("PatternRecognition"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("PatternRecognition"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestContentPreload()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("ContentPreload"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Record some actions
        FPlayerAction Action = CreateTestAction(EPlayerActionType::Combat);
        TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action);
        
        // Get preload requests
        TArray<FContentPreloadRequest> PreloadRequests = TestIntegration->GetContentPreloadRecommendations(TEXT("TestPlayer"));
        
        // Should have at least one request
        if (PreloadRequests.Num() == 0)
        {
            return CreateTestReport(TEXT("ContentPreload"), ETestResult::Failed, TEXT("No preload requests generated"));
        }
        
        // Check request validity
        for (const FContentPreloadRequest& Request : PreloadRequests)
        {
            if (Request.ContentType.IsEmpty() || Request.ContentID.IsEmpty())
            {
                return CreateTestReport(TEXT("ContentPreload"), ETestResult::Failed, TEXT("Invalid preload request"));
            }
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("ContentPreload"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("ContentPreload"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestPlayerInsightGeneration()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("PlayerInsightGeneration"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Get player insight
        FTaskBehaviorInsight Insight = TestIntegration->GetPlayerInsight(TEXT("TestPlayer"));
        
        FString ErrorMessage;
        if (!AssertInsightValid(Insight, ErrorMessage))
        {
            return CreateTestReport(TEXT("PlayerInsightGeneration"), ETestResult::Failed, ErrorMessage);
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("PlayerInsightGeneration"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("PlayerInsightGeneration"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestTaskCompletionAnalysis()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("TaskCompletionAnalysis"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Create a test task
        FDynamicTask Task;
        Task.TaskID = TEXT("TestTask");
        Task.Type = ETaskType::Combat;
        Task.Difficulty = ETaskDifficulty::Normal;
        
        // Analyze task completion
        TestIntegration->AnalyzeTaskCompletionImpact(TEXT("TestPlayer"), Task, 120.0f);
        
        // Get updated insight to verify analysis was applied
        FTaskBehaviorInsight Insight = TestIntegration->GetPlayerInsight(TEXT("TestPlayer"));
        
        if (Insight.PlayerID != TEXT("TestPlayer"))
        {
            return CreateTestReport(TEXT("TaskCompletionAnalysis"), ETestResult::Failed, TEXT("Insight not updated after task completion"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("TaskCompletionAnalysis"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("TaskCompletionAnalysis"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestRealTimePrediction()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("RealTimePrediction"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Enable real-time prediction
        // (This would require accessing private members, so we'll test the public interface)
        
        // Record action and get immediate prediction
        FPlayerAction Action = CreateTestAction(EPlayerActionType::MoveUnit);
        TestIntegration->RecordPlayerAction(TEXT("TestPlayer"), Action);
        
        TArray<FPredictionResult> Predictions = TestIntegration->GetRealTimePredictions(TEXT("TestPlayer"), 1.0f);
        
        if (Predictions.Num() == 0)
        {
            return CreateTestReport(TEXT("RealTimePrediction"), ETestResult::Failed, TEXT("No real-time predictions"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("RealTimePrediction"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("RealTimePrediction"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestCacheSystem()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("CacheSystem"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Generate insight
        FTaskBehaviorInsight Insight1 = TestIntegration->GetPlayerInsight(TEXT("TestPlayer"));
        
        // Generate insight again (should use cache)
        FTaskBehaviorInsight Insight2 = TestIntegration->GetPlayerInsight(TEXT("TestPlayer"));
        
        // Compare insights
        if (Insight1.PlayerID != Insight2.PlayerID || 
            Insight1.OverallEngagementScore != Insight2.OverallEngagementScore)
        {
            return CreateTestReport(TEXT("CacheSystem"), ETestResult::Failed, TEXT("Cache not working properly"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("CacheSystem"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("CacheSystem"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FTestReport UMingAITaskIntegrationTest::TestEventSystem()
{
    float StartTime = UKismetSystemLibrary::GetGameTimeInSeconds();
    
    try
    {
        if (!TestIntegration)
        {
            return CreateTestReport(TEXT("EventSystem"), ETestResult::Failed, TEXT("TestIntegration is null"));
        }
        
        // Test event binding
        bool bTaskGenerated = false;
        bool bBehaviorPredicted = false;
        bool bInsightGenerated = false;
        
        // Bind to events (simplified test)
        TestIntegration->OnTaskGenerated.AddLambda([&bTaskGenerated](const FDynamicTask& Task)
        {
            bTaskGenerated = true;
        });
        
        TestIntegration->OnBehaviorPredicted.AddLambda([&bBehaviorPredicted](const FPredictionResult& Prediction)
        {
            bBehaviorPredicted = true;
        });
        
        TestIntegration->OnInsightGenerated.AddLambda([&bInsightGenerated](const FTaskBehaviorInsight& Insight)
        {
            bInsightGenerated = true;
        });
        
        // Trigger events
        FTaskGenerationRequest Request;
        Request.PlayerID = TEXT("TestPlayer");
        Request.Context = TEXT("Test");
        Request.bForcePersonalization = false;
        Request.Priority = 1;
        
        FDynamicTask Task = TestIntegration->ProcessTaskGenerationRequest(Request);
        FTaskBehaviorInsight Insight = TestIntegration->GetPlayerInsight(TEXT("TestPlayer"));
        
        // Check if events were triggered
        if (!bTaskGenerated || !bInsightGenerated)
        {
            return CreateTestReport(TEXT("EventSystem"), ETestResult::Failed, TEXT("Events not triggered properly"));
        }
        
        float ExecutionTime = UKismetSystemLibrary::GetGameTimeInSeconds() - StartTime;
        return CreateTestReport(TEXT("EventSystem"), ETestResult::Passed, TEXT(""), ExecutionTime);
    }
    catch (...)
    {
        return CreateTestReport(TEXT("EventSystem"), ETestResult::Error, TEXT("Exception occurred"));
    }
}

FString UMingAITaskIntegrationTest::GetTestSummary(const TArray<FTestReport>& TestReports)
{
    int32 Passed = 0;
    int32 Failed = 0;
    int32 Skipped = 0;
    int32 Errors = 0;
    float TotalTime = 0.0f;
    
    for (const FTestReport& Report : TestReports)
    {
        switch (Report.Result)
        {
            case ETestResult::Passed:
                Passed++;
                break;
            case ETestResult::Failed:
                Failed++;
                break;
            case ETestResult::Skipped:
                Skipped++;
                break;
            case ETestResult::Error:
                Errors++;
                break;
            default:
                break;
        }
        
        TotalTime += Report.ExecutionTime;
    }
    
    FString Summary = FString::Printf(TEXT("Test Summary:\n"));
    Summary += FString::Printf(TEXT("Total Tests: %d\n"), TestReports.Num());
    Summary += FString::Printf(TEXT("Passed: %d\n"), Passed);
    Summary += FString::Printf(TEXT("Failed: %d\n"), Failed);
    Summary += FString::Printf(TEXT("Skipped: %d\n"), Skipped);
    Summary += FString::Printf(TEXT("Errors: %d\n"), Errors);
    Summary += FString::Printf(TEXT("Total Execution Time: %.3f seconds\n"), TotalTime);
    Summary += FString::Printf(TEXT("Success Rate: %.1f%%\n"), TestReports.Num() > 0 ? (float)Passed / TestReports.Num() * 100.0f : 0.0f);
    
    return Summary;
}

bool UMingAITaskIntegrationTest::AssertTaskValid(const FDynamicTask& Task, FString& ErrorMessage)
{
    if (Task.TaskID.IsEmpty())
    {
        ErrorMessage = TEXT("Task ID is empty");
        return false;
    }
    
    if (Task.Title.IsEmpty())
    {
        ErrorMessage = TEXT("Task title is empty");
        return false;
    }
    
    if (Task.Objectives.Num() == 0)
    {
        ErrorMessage = TEXT("Task has no objectives");
        return false;
    }
    
    return true;
}

bool UMingAITaskIntegrationTest::AssertPredictionValid(const FPredictionResult& Prediction, FString& ErrorMessage)
{
    if (Prediction.Probability < 0.0f || Prediction.Probability > 1.0f)
    {
        ErrorMessage = FString::Printf(TEXT("Invalid probability: %.2f"), Prediction.Probability);
        return false;
    }
    
    if (Prediction.PredictedTime == FDateTime::MinValue())
    {
        ErrorMessage = TEXT("Invalid predicted time");
        return false;
    }
    
    return true;
}

bool UMingAITaskIntegrationTest::AssertInsightValid(const FTaskBehaviorInsight& Insight, FString& ErrorMessage)
{
    if (Insight.PlayerID.IsEmpty())
    {
        ErrorMessage = TEXT("Player ID is empty");
        return false;
    }
    
    if (Insight.OverallEngagementScore < 0.0f || Insight.OverallEngagementScore > 1.0f)
    {
        ErrorMessage = FString::Printf(TEXT("Invalid engagement score: %.2f"), Insight.OverallEngagementScore);
        return false;
    }
    
    return true;
}

FPlayerAction UMingAITaskIntegrationTest::CreateTestAction(EPlayerActionType ActionType)
{
    FPlayerAction Action;
    Action.ActionType = ActionType;
    Action.ActionData = TEXT("TestAction");
    Action.Timestamp = FDateTime::Now();
    Action.Location = FVector2D(0.0f, 0.0f);
    Action.Duration = 1.0f;
    
    return Action;
}

FPlayerBehaviorProfile UMingAITaskIntegrationTest::CreateTestProfile()
{
    FPlayerBehaviorProfile Profile;
    Profile.CombatPreference = 0.8f;
    Profile.ExplorationPreference = 0.6f;
    Profile.ConstructionPreference = 0.4f;
    Profile.DiplomacyPreference = 0.3f;
    Profile.ResourceManagementPreference = 0.7f;
    Profile.StoryPreference = 0.5f;
    Profile.PlayerSkillLevel = 15;
    Profile.AverageCompletionTime = 180.0f;
    
    return Profile;
}

FTestReport UMingAITaskIntegrationTest::CreateTestReport(const FString& TestName, ETestResult Result, const FString& ErrorMessage, float ExecutionTime)
{
    FTestReport Report;
    Report.TestName = TestName;
    Report.Result = Result;
    Report.ErrorMessage = ErrorMessage;
    Report.ExecutionTime = ExecutionTime;
    
    LogTestResult(Report);
    
    return Report;
}

void UMingAITaskIntegrationTest::LogTestResult(const FTestReport& Report)
{
    FString ResultString;
    switch (Report.Result)
    {
        case ETestResult::Passed:
            ResultString = TEXT("PASSED");
            break;
        case ETestResult::Failed:
            ResultString = TEXT("FAILED");
            break;
        case ETestResult::Skipped:
            ResultString = TEXT("SKIPPED");
            break;
        case ETestResult::Error:
            ResultString = TEXT("ERROR");
            break;
        default:
            ResultString = TEXT("UNKNOWN");
            break;
    }
    
    if (Report.Result == ETestResult::Passed)
    {
        UE_LOG(LogTemp, Log, TEXT("[PASSED] %s (%.3fs)"), *Report.TestName, Report.ExecutionTime);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] %s (%.3fs): %s"), *ResultString, *Report.TestName, Report.ExecutionTime, *Report.ErrorMessage);
    }
}
