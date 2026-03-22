#include "RTS/Test/MingRTSSprint3Test.h"
#include "RTS/MingRTSCampaignSystem.h"
#include "RTS/MingRTSAdvancedAI.h"

UMingRTSSprint3Test::UMingRTSSprint3Test()
{
    TotalTests = 0;
    PassedTests = 0;
    FailedTests = 0;
}

void UMingRTSSprint3Test::InitializeSprint3Test()
{
    TestResults.Empty();
    TotalTests = 0;
    PassedTests = 0;
    FailedTests = 0;
    
    UE_LOG(LogTemp, Log, TEXT("=== MingGoRTS Sprint 3 Test Suite Initialized ==="));
    UE_LOG(LogTemp, Log, TEXT("Testing: AI Campaign & Story Systems"));
}

void UMingRTSSprint3Test::RunAllSprint3Tests()
{
    InitializeSprint3Test();
    
    UE_LOG(LogTemp, Log, TEXT("=== Running Sprint 3 Tests ==="));
    
    // Campaign System Tests
    RecordTestResult(TestCampaignSystemCreation());
    RecordTestResult(TestCampaignStartAndCompletion());
    RecordTestResult(TestObjectiveManagement());
    RecordTestResult(TestChapterProgression());
    RecordTestResult(TestStoryEvents());
    
    // Advanced AI Tests
    RecordTestResult(TestAdvancedAICreation());
    RecordTestResult(TestAIBehaviorPatterns());
    RecordTestResult(TestAIPersonalitySystem());
    RecordTestResult(TestAIMemorySystem());
    RecordTestResult(TestAITacticalDecisions());
    RecordTestResult(TestAIStrategyPlanning());
    RecordTestResult(TestAIAdaptationLearning());
    RecordTestResult(TestTeamCoordination());
    
    // Integration Tests
    RecordTestResult(TestCampaignAIIntegration());
    
    LogTestSummary();
}

FSprint3TestResult UMingRTSSprint3Test::TestCampaignSystemCreation()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Campaign System Creation");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    if (!CampaignSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    
    if (CampaignSystem->GetCampaignState(TEXT("Test")) != ERTSCampaignState::NotStarted)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Campaign system initialization failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ Campaign System Creation test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestCampaignStartAndCompletion()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Campaign Start and Completion");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    if (!CampaignSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    
    FString TestCampaignID = TEXT("TestCampaign");
    CampaignSystem->StartCampaign(TestCampaignID, ERTSCampaignType::MainStory, ERTSCampaignDifficulty::Normal);
    
    if (CampaignSystem->GetCampaignState(TestCampaignID) != ERTSCampaignState::InProgress)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Campaign did not start correctly");
        return Result;
    }
    
    CampaignSystem->CompleteCampaign(TestCampaignID, true);
    
    if (CampaignSystem->GetCampaignState(TestCampaignID) != ERTSCampaignState::Completed)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Campaign completion failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ Campaign Start and Completion test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestObjectiveManagement()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Objective Management");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    if (!CampaignSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    
    // Create test objective
    FRTSCampaignObjective TestObjective;
    TestObjective.ObjectiveID = TEXT("TestObjective");
    TestObjective.ObjectiveName = FText::FromString(TEXT("Test Objective"));
    TestObjective.Description = FText::FromString(TEXT("Test Description"));
    TestObjective.ObjectiveType = ERTSObjectiveType::Destroy;
    TestObjective.bOptional = false;
    
    CampaignSystem->CompleteObjective(TestObjective.ObjectiveID);
    
    FRTSCampaignObjective RetrievedObjective = CampaignSystem->GetObjective(TestObjective.ObjectiveID);
    if (!RetrievedObjective.bCompleted)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Objective completion failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ Objective Management test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestChapterProgression()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Chapter Progression");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    if (!CampaignSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    
    FString TestCampaignID = TEXT("TestCampaign");
    
    // Create test chapters
    FRTSCampaignChapter Chapter1;
    Chapter1.ChapterID = TEXT("Chapter1");
    Chapter1.ChapterNumber = 1;
    Chapter1.ChapterName = FText::FromString(TEXT("Chapter 1"));
    
    FRTSCampaignChapter Chapter2;
    Chapter2.ChapterID = TEXT("Chapter2");
    Chapter2.ChapterNumber = 2;
    Chapter2.ChapterName = FText::FromString(TEXT("Chapter 2"));
    
    CampaignSystem->RegisterCampaignChapter(TestCampaignID, Chapter1);
    CampaignSystem->RegisterCampaignChapter(TestCampaignID, Chapter2);
    
    TArray<FRTSCampaignChapter> Chapters = CampaignSystem->GetCampaignChapters(TestCampaignID);
    if (Chapters.Num() != 2)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Chapter registration failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ Chapter Progression test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestStoryEvents()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Story Events");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    if (!CampaignSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    
    FRTSStoryEvent TestEvent;
    TestEvent.EventID = TEXT("TestEvent");
    TestEvent.EventName = FText::FromString(TEXT("Test Event"));
    TestEvent.Description = FText::FromString(TEXT("Test Description"));
    TestEvent.TriggerCondition = TEXT("TestCondition");
    
    CampaignSystem->RegisterStoryEvent(TestEvent);
    CampaignSystem->TriggerStoryEvent(TestEvent.EventID);
    
    UE_LOG(LogTemp, Log, TEXT("✓ Story Events test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAdvancedAICreation()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Advanced AI Creation");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    UE_LOG(LogTemp, Log, TEXT("✓ Advanced AI Creation test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAIBehaviorPatterns()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Behavior Patterns");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    
    // Test setting different behavior patterns
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Rush);
    if (AdvancedAI->GetAIBehaviorPattern(TestAIID) != ERTSAIBehaviorPattern::Rush)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Rush pattern setting failed");
        return Result;
    }
    
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Turtle);
    if (AdvancedAI->GetAIBehaviorPattern(TestAIID) != ERTSAIBehaviorPattern::Turtle)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Turtle pattern setting failed");
        return Result;
    }
    
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Adaptive);
    if (AdvancedAI->GetAIBehaviorPattern(TestAIID) != ERTSAIBehaviorPattern::Adaptive)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Adaptive pattern setting failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Behavior Patterns test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAIPersonalitySystem()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Personality System");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    
    FRTSAIPersonality TestPersonality;
    TestPersonality.Aggression = 0.8f;
    TestPersonality.Defensiveness = 0.3f;
    TestPersonality.Expansionism = 0.7f;
    TestPersonality.EconomicFocus = 0.6f;
    TestPersonality.TechFocus = 0.5f;
    TestPersonality.RiskTolerance = 0.9f;
    TestPersonality.Adaptability = 0.7f;
    
    AdvancedAI->AssignAIPersonality(TestAIID, TestPersonality);
    
    FRTSAIPersonality RetrievedPersonality = AdvancedAI->GetAIPersonality(TestAIID);
    
    if (FMath::Abs(RetrievedPersonality.Aggression - TestPersonality.Aggression) > 0.01f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Personality assignment failed - aggression mismatch");
        return Result;
    }
    
    if (FMath::Abs(RetrievedPersonality.RiskTolerance - TestPersonality.RiskTolerance) > 0.01f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Personality assignment failed - risk tolerance mismatch");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Personality System test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAIMemorySystem()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Memory System");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    
    // Add different types of memories
    FRTSAIMemory ShortTermMemory;
    ShortTermMemory.MemoryID = TEXT("ShortTerm1");
    ShortTermMemory.EventDescription = TEXT("Recent event");
    ShortTermMemory.MemoryType = ERTSAIMemoryType::ShortTerm;
    ShortTermMemory.Importance = 0.7f;
    ShortTermMemory.Timestamp = FPlatformTime::Seconds();
    ShortTermMemory.Location = FVector(1000.0f, 1000.0f, 0.0f);
    
    FRTSAIMemory LongTermMemory;
    LongTermMemory.MemoryID = TEXT("LongTerm1");
    LongTermMemory.EventDescription = TEXT("Important historical event");
    LongTermMemory.MemoryType = ERTSAIMemoryType::LongTerm;
    LongTermMemory.Importance = 0.9f;
    LongTermMemory.Timestamp = FPlatformTime::Seconds();
    LongTermMemory.Location = FVector(2000.0f, 2000.0f, 0.0f);
    
    AdvancedAI->AddAIMemory(TestAIID, ShortTermMemory);
    AdvancedAI->AddAIMemory(TestAIID, LongTermMemory);
    
    TArray<FRTSAIMemory> ShortTermMemories = AdvancedAI->GetAIMemories(TestAIID, ERTSAIMemoryType::ShortTerm);
    TArray<FRTSAIMemory> LongTermMemories = AdvancedAI->GetAIMemories(TestAIID, ERTSAIMemoryType::LongTerm);
    
    if (ShortTermMemories.Num() < 1)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Short term memory storage failed");
        return Result;
    }
    
    if (LongTermMemories.Num() < 1)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Long term memory storage failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Memory System test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAITacticalDecisions()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Tactical Decisions");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Balanced);
    
    FRTSTacticalDecision Decision = AdvancedAI->MakeTacticalDecision(TestAIID);
    
    if (Decision.DecisionID.IsEmpty())
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Tactical decision generation failed");
        return Result;
    }
    
    if (Decision.Confidence <= 0.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Tactical decision has invalid confidence");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Tactical Decisions test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAIStrategyPlanning()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Strategy Planning");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Boom);
    
    // Strategy plan is created automatically when behavior pattern is set
    // Execute the plan
    AdvancedAI->ExecuteStrategyPlan(TestAIID);
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Strategy Planning test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestAIAdaptationLearning()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("AI Adaptation and Learning");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    FString TestAIID = TEXT("TestAI");
    FString TestPlayerID = TEXT("TestPlayer");
    
    AdvancedAI->SetAIBehaviorPattern(TestAIID, ERTSAIBehaviorPattern::Balanced);
    
    // Test adaptation
    AdvancedAI->AdaptToPlayerStrategy(TestAIID, TestPlayerID);
    
    // Test learning from match
    AdvancedAI->LearnFromMatch(TestAIID, true, TEXT("TestMatch"));
    
    // Verify memories were created
    TArray<FRTSAIMemory> LongTermMemories = AdvancedAI->GetAIMemories(TestAIID, ERTSAIMemoryType::LongTerm);
    
    bool bFoundMatchMemory = false;
    for (const auto& Memory : LongTermMemories)
    {
        if (Memory.EventDescription == TEXT("Victory"))
        {
            bFoundMatchMemory = true;
            break;
        }
    }
    
    if (!bFoundMatchMemory)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Match result memory not created");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI Adaptation and Learning test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestTeamCoordination()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Team Coordination");
    Result.bPassed = true;
    
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    if (!AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create advanced AI");
        return Result;
    }
    
    AdvancedAI->InitializeAdvancedAI();
    
    int32 TeamID = 1;
    FString AI1ID = TEXT("AI_Team_1");
    FString AI2ID = TEXT("AI_Team_2");
    FString AI3ID = TEXT("AI_Team_3");
    
    // Add AIs to team
    AdvancedAI->SetTeamCooperation(AI1ID, TeamID, true);
    AdvancedAI->SetTeamCooperation(AI2ID, TeamID, true);
    AdvancedAI->SetTeamCooperation(AI3ID, TeamID, true);
    
    // Process team coordination
    AdvancedAI->ProcessTeamCoordination(TeamID);
    
    // Test leaving team
    AdvancedAI->SetTeamCooperation(AI2ID, TeamID, false);
    
    UE_LOG(LogTemp, Log, TEXT("✓ Team Coordination test passed"));
    return Result;
}

FSprint3TestResult UMingRTSSprint3Test::TestCampaignAIIntegration()
{
    FSprint3TestResult Result;
    Result.TestName = TEXT("Campaign AI Integration");
    Result.bPassed = true;
    
    UMingRTSCampaignSystem* CampaignSystem = NewObject<UMingRTSCampaignSystem>();
    UMingRTSAdvancedAI* AdvancedAI = NewObject<UMingRTSAdvancedAI>();
    
    if (!CampaignSystem || !AdvancedAI)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create campaign or AI system");
        return Result;
    }
    
    CampaignSystem->InitializeCampaignSystem();
    AdvancedAI->InitializeAdvancedAI();
    
    // Create AI for campaign
    FString EnemyAIID = TEXT("EnemyAI");
    AdvancedAI->SetAIBehaviorPattern(EnemyAIID, ERTSAIBehaviorPattern::Balanced);
    AdvancedAI->SetAITacticalRole(EnemyAIID, ERTSAITacticalRole::Commander);
    
    // Create campaign
    FString CampaignID = TEXT("IntegrationTest");
    CampaignSystem->StartCampaign(CampaignID, ERTSCampaignType::MainStory, ERTSCampaignDifficulty::Normal);
    
    // Create objectives that require defeating AI
    FRTSCampaignObjective DefeatEnemyObjective;
    DefeatEnemyObjective.ObjectiveID = TEXT("DefeatEnemy");
    DefeatEnemyObjective.ObjectiveName = FText::FromString(TEXT("Defeat Enemy AI"));
    DefeatEnemyObjective.ObjectiveType = ERTSObjectiveType::Destroy;
    DefeatEnemyObjective.bOptional = false;
    
    CampaignSystem->CompleteObjective(DefeatEnemyObjective.ObjectiveID);
    
    // Simulate AI making tactical decisions during campaign
    FRTSTacticalDecision AIDecision = AdvancedAI->MakeTacticalDecision(EnemyAIID);
    AdvancedAI->ExecuteTacticalDecision(EnemyAIID, AIDecision);
    
    // Complete campaign
    CampaignSystem->CompleteCampaign(CampaignID, true);
    
    if (CampaignSystem->GetCampaignState(CampaignID) != ERTSCampaignState::Completed)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Campaign completion in integration test failed");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ Campaign AI Integration test passed"));
    return Result;
}

void UMingRTSSprint3Test::RecordTestResult(const FSprint3TestResult& Result)
{
    TestResults.Add(Result);
    TotalTests++;
    
    if (Result.bPassed)
    {
        PassedTests++;
    }
    else
    {
        FailedTests++;
        UE_LOG(LogTemp, Warning, TEXT("✗ %s failed: %s"), *Result.TestName, *Result.ErrorMessage);
    }
}

void UMingRTSSprint3Test::LogTestSummary()
{
    UE_LOG(LogTemp, Log, TEXT("=== Sprint 3 Test Summary ==="));
    UE_LOG(LogTemp, Log, TEXT("Total Tests: %d"), TotalTests);
    UE_LOG(LogTemp, Log, TEXT("Passed: %d"), PassedTests);
    UE_LOG(LogTemp, Log, TEXT("Failed: %d"), FailedTests);
    UE_LOG(LogTemp, Log, TEXT("Coverage: %.1f%%"), GetTestCoverage());
    
    if (FailedTests == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("🎉 All Sprint 3 tests passed!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ %d tests failed. Review required."), FailedTests);
    }
}

int32 UMingRTSSprint3Test::GetTotalTests() const
{
    return TotalTests;
}

int32 UMingRTSSprint3Test::GetPassedTests() const
{
    return PassedTests;
}

int32 UMingRTSSprint3Test::GetFailedTests() const
{
    return FailedTests;
}

float UMingRTSSprint3Test::GetTestCoverage() const
{
    if (TotalTests == 0)
    {
        return 0.0f;
    }
    
    return (float)PassedTests / (float)TotalTests * 100.0f;
}

FString UMingRTSSprint3Test::GenerateTestReport() const
{
    FString Report = FString::Printf(TEXT("Sprint 3 Test Report\n"));
    Report += FString::Printf(TEXT("====================\n"));
    Report += FString::Printf(TEXT("Total Tests: %d\n"), TotalTests);
    Report += FString::Printf(TEXT("Passed: %d\n"), PassedTests);
    Report += FString::Printf(TEXT("Failed: %d\n"), FailedTests);
    Report += FString::Printf(TEXT("Coverage: %.1f%%\n\n"), GetTestCoverage());
    
    Report += TEXT("Test Details:\n");
    for (const auto& Result : TestResults)
    {
        Report += FString::Printf(TEXT("[%s] %s\n"), 
            Result.bPassed ? TEXT("PASS") : TEXT("FAIL"),
            *Result.TestName);
        
        if (!Result.bPassed && !Result.ErrorMessage.IsEmpty())
        {
            Report += FString::Printf(TEXT("  Error: %s\n"), *Result.ErrorMessage);
        }
    }
    
    return Report;
}
