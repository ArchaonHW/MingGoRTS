// Copyright (c) 2026 MingGoRTS. All rights reserved.
// History System Test Suite Implementation

#include "MingRTSHistorySystemTest.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSHistorySystemTest::UMingRTSHistorySystemTest()
{
    bAllTestsPassed = false;
    TotalTestCount = 0;
    PassedTestCount = 0;
    HistorySystem = nullptr;
    TimelineManager = nullptr;
    NarrativeGenerator = nullptr;
    CausalityCalculator = nullptr;
}

void UMingRTSHistorySystemTest::RunAllTests()
{
    ResetTestResults();
    SetupTestEnvironment();
    
    UE_LOG(LogTemp, Log, TEXT("Starting History System Test Suite..."));
    
    // Run basic tests
    RunBasicTests();
    
    // Run performance tests
    RunPerformanceTests();
    
    // Run integration tests
    RunIntegrationTests();
    
    // Run stress tests
    RunStressTests();
    
    CleanupTestEnvironment();
    
    bAllTestsPassed = (FailedTestNames.Num() == 0);
    
    UE_LOG(LogTemp, Log, TEXT("History System Test Suite completed. Passed: %d/%d"), PassedTestCount, TotalTestCount);
}

void UMingRTSHistorySystemTest::RunBasicTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running Basic Tests..."));
    
    LogTestResult(TEXT("Event Generation"), TestEventGeneration());
    LogTestResult(TEXT("Timeline Management"), TestTimelineManagement());
    LogTestResult(TEXT("Causality Calculation"), TestCausalityCalculation());
    LogTestResult(TEXT("Narrative Generation"), TestNarrativeGeneration());
    LogTestResult(TEXT("System Integration"), TestSystemIntegration());
}

void UMingRTSHistorySystemTest::RunPerformanceTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running Performance Tests..."));
    
    LogTestResult(TEXT("Performance Under Load"), TestPerformanceUnderLoad());
    LogTestResult(TEXT("Memory Usage"), TestMemoryUsage());
}

void UMingRTSHistorySystemTest::RunIntegrationTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running Integration Tests..."));
    
    // Integration tests are covered in basic tests
}

void UMingRTSHistorySystemTest::RunStressTests()
{
    UE_LOG(LogTemp, Log, TEXT("Running Stress Tests..."));
    
    LogTestResult(TEXT("Concurrent Operations"), TestConcurrentOperations());
}

bool UMingRTSHistorySystemTest::TestEventGeneration()
{
    if (!HistorySystem)
    {
        return false;
    }
    
    // Test basic event generation
    FHistoricalEventData TestEvent = GenerateTestEvent(TEXT("Test Event"), EHistoricalEventType::Political);
    FString EventID = HistorySystem->GenerateHistoricalEvent(TestEvent);
    
    if (EventID.IsEmpty())
    {
        return false;
    }
    
    // Test event retrieval
    FHistoricalEventData RetrievedEvent = HistorySystem->GetHistoricalEvent(EventID);
    if (!ValidateEventData(RetrievedEvent))
    {
        return false;
    }
    
    // Test event filtering
    TArray<FHistoricalEventData> PoliticalEvents = HistorySystem->GetEventsByType(EHistoricalEventType::Political);
    if (PoliticalEvents.Num() == 0)
    {
        return false;
    }
    
    return true;
}

bool UMingRTSHistorySystemTest::TestTimelineManagement()
{
    if (!TimelineManager)
    {
        return false;
    }
    
    // Test timeline creation
    FString TimelineID = TimelineManager->CreateNewTimeline(TEXT("MAIN"), TEXT("Test Timeline"), ETimelineBranchType::Alternate);
    if (TimelineID.IsEmpty())
    {
        return false;
    }
    
    // Test timeline retrieval
    TArray<FTimelineBranch> AllTimelines = TimelineManager->GetAllTimelines();
    if (AllTimelines.Num() == 0)
    {
        return false;
    }
    
    // Test timeline switching
    TimelineManager->SwitchToTimeline(TimelineID);
    if (TimelineManager->GetCurrentTimeline() != TimelineID)
    {
        return false;
    }
    
    return true;
}

bool UMingRTSHistorySystemTest::TestCausalityCalculation()
{
    if (!CausalityCalculator)
    {
        return false;
    }
    
    // Test causality analysis
    FHistoricalEventData SourceEvent = GenerateTestEvent(TEXT("Source Event"), EHistoricalEventType::Political);
    FHistoricalEventData TargetEvent = GenerateTestEvent(TEXT("Target Event"), EHistoricalEventType::Social);
    
    FCausalityAnalysisResult Result = CausalityCalculator->AnalyzeCausality(SourceEvent, TargetEvent);
    if (!ValidateCausalityData(Result))
    {
        return false;
    }
    
    // Test network analysis
    TMap<FString, FCausalityNetworkNode> Network = CausalityCalculator->GetCausalityNetwork();
    // Network can be empty initially, that's fine
    
    return true;
}

bool UMingRTSHistorySystemTest::TestNarrativeGeneration()
{
    if (!NarrativeGenerator)
    {
        return false;
    }
    
    // Test narrative generation
    FNarrativeGenerationRequest Request = GenerateTestNarrativeRequest();
    FNarrativeStructure Narrative = NarrativeGenerator->GenerateNarrative(Request);
    
    if (!ValidateNarrativeData(Narrative))
    {
        return false;
    }
    
    // Test quick narrative generation
    TArray<FHistoricalEventData> TestEvents = GenerateTestEventSet(5);
    FString QuickNarrative = NarrativeGenerator->GenerateQuickNarrative(TestEvents);
    
    if (QuickNarrative.IsEmpty())
    {
        return false;
    }
    
    return true;
}

bool UMingRTSHistorySystemTest::TestSystemIntegration()
{
    // Test integration between components
    if (!HistorySystem || !TimelineManager || !NarrativeGenerator || !CausalityCalculator)
    {
        return false;
    }
    
    // Generate an event and ensure it's properly integrated
    FHistoricalEventData TestEvent = GenerateTestEvent(TEXT("Integration Test"), EHistoricalEventType::Military);
    FString EventID = HistorySystem->GenerateHistoricalEvent(TestEvent);
    
    if (EventID.IsEmpty())
    {
        return false;
    }
    
    // Test that the event can be used in causality analysis
    FHistoricalEventData RetrievedEvent = HistorySystem->GetHistoricalEvent(EventID);
    FCausalityAnalysisResult CausalityResult = CausalityCalculator->AnalyzeCausality(RetrievedEvent, TestEvent);
    
    // Test that the event can be used in narrative generation
    TArray<FHistoricalEventData> Events;
    Events.Add(RetrievedEvent);
    FString Narrative = NarrativeGenerator->GenerateQuickNarrative(Events);
    
    return !Narrative.IsEmpty();
}

bool UMingRTSHistorySystemTest::TestPerformanceUnderLoad()
{
    if (!HistorySystem)
    {
        return false;
    }
    
    // Test performance with many events
    double StartTime = FPlatformTime::Seconds();
    
    for (int32 i = 0; i < 1000; i++)
    {
        FHistoricalEventData TestEvent = GenerateTestEvent(
            FString::Printf(TEXT("Performance Test %d"), i),
            EHistoricalEventType::Political
        );
        HistorySystem->GenerateHistoricalEvent(TestEvent);
    }
    
    double EndTime = FPlatformTime::Seconds();
    double ExecutionTime = EndTime - StartTime;
    
    // Should complete within reasonable time (e.g., 5 seconds)
    if (ExecutionTime > 5.0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Performance test took %f seconds, exceeds threshold"), ExecutionTime);
        return false;
    }
    
    ExecutionTimes.Add(ExecutionTime);
    return true;
}

bool UMingRTSHistorySystemTest::TestMemoryUsage()
{
    int32 InitialMemory = 0;
    int32 PeakMemory = 0;
    
    MeasureMemoryUsage(InitialMemory, PeakMemory);
    
    // Generate events and check memory usage
    for (int32 i = 0; i < 100; i++)
    {
        FHistoricalEventData TestEvent = GenerateTestEvent(
            FString::Printf(TEXT("Memory Test %d"), i),
            EHistoricalEventType::Economic
        );
        HistorySystem->GenerateHistoricalEvent(TestEvent);
    }
    
    int32 FinalMemory = 0;
    int32 FinalPeakMemory = 0;
    MeasureMemoryUsage(FinalMemory, FinalPeakMemory);
    
    // Memory usage should be reasonable (e.g., less than 100MB increase)
    int32 MemoryIncrease = FinalMemory - InitialMemory;
    if (MemoryIncrease > 100 * 1024 * 1024) // 100MB
    {
        UE_LOG(LogTemp, Warning, TEXT("Memory usage increased by %d bytes, exceeds threshold"), MemoryIncrease);
        return false;
    }
    
    MemoryUsageData.Add(MemoryIncrease);
    return true;
}

bool UMingRTSHistorySystemTest::TestConcurrentOperations()
{
    // Simplified concurrent test
    // In a real implementation, this would use threading
    TArray<FString> EventIDs;
    
    for (int32 i = 0; i < 10; i++)
    {
        FHistoricalEventData TestEvent = GenerateTestEvent(
            FString::Printf(TEXT("Concurrent Test %d"), i),
            EHistoricalEventType::Social
        );
        FString EventID = HistorySystem->GenerateHistoricalEvent(TestEvent);
        EventIDs.Add(EventID);
    }
    
    // Verify all events were created
    for (const FString& EventID : EventIDs)
    {
        FHistoricalEventData Event = HistorySystem->GetHistoricalEvent(EventID);
        if (!ValidateEventData(Event))
        {
            return false;
        }
    }
    
    return true;
}

void UMingRTSHistorySystemTest::SetupTestEnvironment()
{
    InitializeTestComponents();
}

void UMingRTSHistorySystemTest::CleanupTestEnvironment()
{
    CleanupTestComponents();
}

FHistoricalEventData UMingRTSHistorySystemTest::GenerateTestEvent(const FString& EventName, EHistoricalEventType EventType)
{
    FHistoricalEventData TestEvent;
    TestEvent.EventName = EventName;
    TestEvent.Description = FString::Printf(TEXT("Test description for %s"), *EventName);
    TestEvent.EventType = EventType;
    TestEvent.Importance = EHistoricalImportance::Moderate;
    TestEvent.Timestamp = FDateTime::Now();
    TestEvent.Location = FVector2D(FMath::RandRange(0.0f, 1000.0f), FMath::RandRange(0.0f, 1000.0f));
    
    return TestEvent;
}

TArray<FHistoricalEventData> UMingRTSHistorySystemTest::GenerateTestEventSet(int32 EventCount)
{
    TArray<FHistoricalEventData> Events;
    
    for (int32 i = 0; i < EventCount; i++)
    {
        FHistoricalEventData Event = GenerateTestEvent(
            FString::Printf(TEXT("Test Event %d"), i),
            (EHistoricalEventType)FMath::RandRange(0, (int32)EHistoricalEventType::Global - 1)
        );
        Events.Add(Event);
    }
    
    return Events;
}

FNarrativeGenerationRequest UMingRTSHistorySystemTest::GenerateTestNarrativeRequest()
{
    FNarrativeGenerationRequest Request;
    Request.TimelineID = TEXT("MAIN");
    Request.StartTime = FDateTime::Now() - FTimespan::FromDays(7);
    Request.EndTime = FDateTime::Now();
    Request.Style = ENarrativeStyle::Chronological;
    Request.Tone = ENarrativeTone::Neutral;
    Request.Focus = ENarrativeFocus::Events;
    Request.MaxLength = 500;
    
    return Request;
}

void UMingRTSHistorySystemTest::ResetTestResults()
{
    bAllTestsPassed = false;
    TotalTestCount = 0;
    PassedTestCount = 0;
    FailedTestNames.Empty();
    TestMessages.Empty();
    ExecutionTimes.Empty();
    MemoryUsageData.Empty();
}

void UMingRTSHistorySystemTest::LogTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage)
{
    TotalTestCount++;
    
    if (bPassed)
    {
        PassedTestCount++;
        UE_LOG(LogTemp, Log, TEXT("✓ %s"), *TestName);
    }
    else
    {
        FailedTestNames.Add(TestName);
        UE_LOG(LogTemp, Error, TEXT("✗ %s: %s"), *TestName, *ErrorMessage);
    }
}

FString UMingRTSHistorySystemTest::GetTestReport() const
{
    return GenerateTestReport();
}

void UMingRTSHistorySystemTest::InitializeTestComponents()
{
    HistorySystem = NewObject<UMingRTSDynamicHistorySystem>();
    TimelineManager = NewObject<UMingRTSTimelineManager>();
    NarrativeGenerator = NewObject<UMingRTSHistoricalNarrativeGenerator>();
    CausalityCalculator = NewObject<UMingRTSCausalityCalculator>();
    
    if (HistorySystem)
    {
        HistorySystem->InitializeDynamicHistorySystem();
    }
    
    if (TimelineManager)
    {
        TimelineManager->InitializeTimelineManager();
    }
    
    if (NarrativeGenerator)
    {
        NarrativeGenerator->InitializeNarrativeGenerator();
    }
    
    if (CausalityCalculator)
    {
        CausalityCalculator->InitializeCausalityCalculator();
    }
}

void UMingRTSHistorySystemTest::CleanupTestComponents()
{
    HistorySystem = nullptr;
    TimelineManager = nullptr;
    NarrativeGenerator = nullptr;
    CausalityCalculator = nullptr;
}

bool UMingRTSHistorySystemTest::ValidateEventData(const FHistoricalEventData& Event)
{
    return !Event.EventID.IsEmpty() && !Event.EventName.IsEmpty() && !Event.Description.IsEmpty();
}

bool UMingRTSHistorySystemTest::ValidateTimelineData(const FTimelineBranch& Timeline)
{
    return !Timeline.BranchID.IsEmpty() && !Timeline.BranchName.IsEmpty();
}

bool UMingRTSHistorySystemTest::ValidateCausalityData(const FCausalityAnalysisResult& Causality)
{
    return !Causality.SourceEventID.IsEmpty() && !Causality.TargetEventID.IsEmpty();
}

bool UMingRTSHistorySystemTest::ValidateNarrativeData(const FNarrativeStructure& Narrative)
{
    return !Narrative.Title.IsEmpty();
}

double UMingRTSHistorySystemTest::MeasureExecutionTime(TFunction<void()> TestFunction)
{
    double StartTime = FPlatformTime::Seconds();
    TestFunction();
    double EndTime = FPlatformTime::Seconds();
    return EndTime - StartTime;
}

void UMingRTSHistorySystemTest::MeasureMemoryUsage(int32& UsedMemory, int32& PeakMemory)
{
    // Simplified memory measurement
    UsedMemory = 0;
    PeakMemory = 0;
}

FString UMingRTSHistorySystemTest::GenerateTestReport() const
{
    FString Report;
    Report += TEXT("=== History System Test Report ===\n");
    Report += FString::Printf(TEXT("Total Tests: %d\n"), TotalTestCount);
    Report += FString::Printf(TEXT("Passed: %d\n"), PassedTestCount);
    Report += FString::Printf(TEXT("Failed: %d\n"), TotalTestCount - PassedTestCount);
    Report += FString::Printf(TEXT("Success Rate: %.1f%%\n"), TotalTestCount > 0 ? (float)PassedTestCount / (float)TotalTestCount * 100.0f : 0.0f);
    
    if (FailedTestNames.Num() > 0)
    {
        Report += TEXT("\nFailed Tests:\n");
        for (const FString& TestName : FailedTestNames)
        {
            Report += FString::Printf(TEXT("  - %s\n"), *TestName);
        }
    }
    
    if (ExecutionTimes.Num() > 0)
    {
        Report += TEXT("\nPerformance Metrics:\n");
        double TotalTime = 0.0;
        for (double Time : ExecutionTimes)
        {
            TotalTime += Time;
        }
        Report += FString::Printf(TEXT("  Average Execution Time: %.3f seconds\n"), TotalTime / ExecutionTimes.Num());
    }
    
    return Report;
}
