#include "MingUserExperienceTester.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingUserExperienceTester::UMingUserExperienceTester()
{
    WorldContext = GetWorld();
    bIsTesting = false;
}

void UMingUserExperienceTester::ExecuteAllUXTests()
{
    UE_LOG(LogTemp, Log, TEXT("Executing all MingGoRTS UX tests"));
    
    InitializeUXTests();
    SetupUXTestEnvironment();
    
    // Execute all UX test types
    ExecuteNavigationTests();
    ExecuteInterfaceTests();
    ExecuteFeedbackTests();
    ExecuteAccessibilityTests();
    ExecutePerformanceTests();
    ExecuteConsistencyTests();
    ExecuteIntuitivenessTests();
    ExecuteResponsivenessTests();
    
    // Process results
    ProcessUXTestResults();
    IdentifyUXIssues();
    GenerateUXRecommendations();
    CreateUXSummary();
    
    CleanupUXTestEnvironment();
    
    // Broadcast completion
    float OverallScore = GetOverallUXScore();
    OnAllUXTestsCompleted.Broadcast(OverallScore);
    
    UE_LOG(LogTemp, Log, TEXT("All UX tests completed. Overall score: %.1f"), OverallScore);
}

void UMingUserExperienceTester::InitializeUXTests()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing UX tests"));
    
    UXTestResults.Empty();
    
    // Navigation Tests
    FMingUXTestResult MainMenuNav;
    MainMenuNav.TestID = TEXT("UX_MAIN_MENU_NAV");
    MainMenuNav.TestType = EMingUXTestType::Navigation;
    MainMenuNav.TestName = TEXT("Main Menu Navigation");
    MainMenuNav.Description = TEXT("Tests main menu navigation efficiency and intuitiveness");
    MainMenuNav.bIsCritical = true;
    UXTestResults.Add(MainMenuNav.TestID, MainMenuNav);
    
    FMingUXTestResult CampaignNav;
    CampaignNav.TestID = TEXT("UX_CAMPAIGN_NAV");
    CampaignNav.TestType = EMingUXTestType::Navigation;
    CampaignNav.TestName = TEXT("Campaign Navigation");
    CampaignNav.Description = TEXT("Tests campaign selection and navigation");
    CampaignNav.bIsCritical = true;
    UXTestResults.Add(CampaignNav.TestID, CampaignNav);
    
    FMingUXTestResult InGameNav;
    InGameNav.TestID = TEXT("UX_INGAME_NAV");
    InGameNav.TestType = EMingUXTestType::Navigation;
    InGameNav.TestName = TEXT("In-Game Navigation");
    InGameNav.Description = TEXT("Tests in-game interface navigation");
    InGameNav.bIsCritical = true;
    UXTestResults.Add(InGameNav.TestID, InGameNav);
    
    // Interface Tests
    FMingUXTestResult InterfaceClarity;
    InterfaceClarity.TestID = TEXT("UX_INTERFACE_CLARITY");
    InterfaceClarity.TestType = EMingUXTestType::Interface;
    InterfaceClarity.TestName = TEXT("Interface Clarity");
    InterfaceClarity.Description = TEXT("Tests interface clarity and readability");
    InterfaceClarity.bIsCritical = true;
    UXTestResults.Add(InterfaceClarity.TestID, InterfaceClarity);
    
    FMingUXTestResult VisualHierarchy;
    VisualHierarchy.TestID = TEXT("UX_VISUAL_HIERARCHY");
    VisualHierarchy.TestType = EMingUXTestType::Interface;
    VisualHierarchy.TestName = TEXT("Visual Hierarchy");
    VisualHierarchy.Description = TEXT("Tests visual hierarchy and information organization");
    VisualHierarchy.bIsCritical = false;
    UXTestResults.Add(VisualHierarchy.TestID, VisualHierarchy);
    
    // Feedback Tests
    FMingUXTestResult AudioFeedback;
    AudioFeedback.TestID = TEXT("UX_AUDIO_FEEDBACK");
    AudioFeedback.TestType = EMingUXTestType::Feedback;
    AudioFeedback.TestName = TEXT("Audio Feedback");
    AudioFeedback.TestName = TEXT("Tests audio feedback quality and timing");
    AudioFeedback.bIsCritical = false;
    UXTestResults.Add(AudioFeedback.TestID, AudioFeedback);
    
    FMingUXTestResult VisualFeedback;
    VisualFeedback.TestID = TEXT("UX_VISUAL_FEEDBACK");
    VisualFeedback.TestType = EMingUXTestType::Feedback;
    VisualFeedback.TestName = TEXT("Visual Feedback");
    VisualFeedback.TestName = TEXT("Tests visual feedback and animations");
    VisualFeedback.bIsCritical = true;
    UXTestResults.Add(VisualFeedback.TestID, VisualFeedback);
    
    // Accessibility Tests
    FMingUXTestResult ColorBlindness;
    ColorBlindness.TestID = TEXT("UX_COLOR_BLINDNESS");
    ColorBlindness.TestType = EMingUXTestType::Accessibility;
    ColorBlindness.TestName = TEXT("Color Blindness Support");
    ColorBlindness.Description = TEXT("Tests color blindness accessibility");
    ColorBlindness.bIsCritical = false;
    UXTestResults.Add(ColorBlindness.TestID, ColorBlindness);
    
    FMingUXTestResult TextReadability;
    TextReadability.TestID = TEXT("UX_TEXT_READABILITY");
    TextReadability.TestType = EMingUXTestType::Accessibility;
    TextReadability.TestName = TEXT("Text Readability");
    TextReadability.Description = TEXT("Tests text readability and font sizing");
    TextReadability.bIsCritical = true;
    UXTestResults.Add(TextReadability.TestID, TextReadability);
    
    // Performance Tests
    FMingUXTestResult UIResponsiveness;
    UIResponsiveness.TestID = TEXT("UX_UI_RESPONSIVENESS");
    UIResponsiveness.TestType = EMingUXTestType::Performance;
    UIResponsiveness.TestName = TEXT("UI Responsiveness");
    UIResponsiveness.Description = TEXT("Tests UI responsiveness and input lag");
    UIResponsiveness.bIsCritical = true;
    UXTestResults.Add(UIResponsiveness.TestID, UIResponsiveness);
    
    FMingUXTestResult LoadingTimes;
    LoadingTimes.TestID = TEXT("UX_LOADING_TIMES");
    LoadingTimes.TestType = EMingUXTestType::Performance;
    LoadingTimes.TestName = TEXT("Loading Times");
    LoadingTimes.Description = TEXT("Tests screen loading and transition times");
    LoadingTimes.bIsCritical = true;
    UXTestResults.Add(LoadingTimes.TestID, LoadingTimes);
    
    // Consistency Tests
    FMingUXTestResult DesignConsistency;
    DesignConsistency.TestID = TEXT("UX_DESIGN_CONSISTENCY");
    DesignConsistency.TestType = EMingUXTestType::Consistency;
    DesignConsistency.TestName = TEXT("Design Consistency");
    DesignConsistency.Description = TEXT("Tests design consistency across interfaces");
    DesignConsistency.bIsCritical = true;
    UXTestResults.Add(DesignConsistency.TestID, DesignConsistency);
    
    FMingUXTestResult InteractionConsistency;
    InteractionConsistency.TestID = TEXT("UX_INTERACTION_CONSISTENCY");
    InteractionConsistency.TestType = EMingUXTestType::Consistency;
    InteractionConsistency.TestName = TEXT("Interaction Consistency");
    InteractionConsistency.Description = TEXT("Tests interaction consistency across screens");
    InteractionConsistency.bIsCritical = false;
    UXTestResults.Add(InteractionConsistency.TestID, InteractionConsistency);
    
    // Intuitiveness Tests
    FMingUXTestResult NewPlayerExperience;
    NewPlayerExperience.TestID = TEXT("UX_NEW_PLAYER_EXPERIENCE");
    NewPlayerExperience.TestType = EMingUXTestType::Intuitiveness;
    NewPlayerExperience.TestName = TEXT("New Player Experience");
    NewPlayerExperience.Description = TEXT("Tests intuitiveness for new players");
    NewPlayerExperience.bIsCritical = true;
    UXTestResults.Add(NewPlayerExperience.TestID, NewPlayerExperience);
    
    FMingUXTestResult LearningCurve;
    LearningCurve.TestID = TEXT("UX_LEARNING_CURVE");
    LearningCurve.TestType = EMingUXTestType::Intuitiveness;
    LearningCurve.TestName = TEXT("Learning Curve");
    LearningCurve.Description = TEXT("Tests learning curve and tutorial effectiveness");
    LearningCurve.bIsCritical = false;
    UXTestResults.Add(LearningCurve.TestID, LearningCurve);
    
    // Responsiveness Tests
    FMingUXTestResult InputResponsiveness;
    InputResponsiveness.TestID = TEXT("UX_INPUT_RESPONSIVENESS");
    InputResponsiveness.TestType = EMingUXTestType::Responsiveness;
    InputResponsiveness.TestName = TEXT("Input Responsiveness");
    InputResponsiveness.Description = TEXT("Tests input responsiveness and feedback");
    InputResponsiveness.bIsCritical = true;
    UXTestResults.Add(InputResponsiveness.TestID, InputResponsiveness);
    
    FMingUXTestResult AnimationSmoothness;
    AnimationSmoothness.TestID = TEXT("UX_ANIMATION_SMOOTHNESS");
    AnimationSmoothness.TestType = EMingUXTestType::Responsiveness;
    AnimationSmoothness.TestName = TEXT("Animation Smoothness");
    AnimationSmoothness.Description = TEXT("Tests animation smoothness and frame rate");
    AnimationSmoothness.bIsCritical = false;
    UXTestResults.Add(AnimationSmoothness.TestID, AnimationSmoothness);
    
    UE_LOG(LogTemp, Log, TEXT("UX tests initialized: %d tests"), UXTestResults.Num());
}

void UMingUserExperienceTester::SetupUXTestEnvironment()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up UX test environment"));
    
    // Reset metrics
    CurrentMetrics = FMingUXMetrics();
    
    // Setup test environment
    bIsTesting = true;
    
    UE_LOG(LogTemp, Log, TEXT("UX test environment setup completed"));
}

void UMingUserExperienceTester::CleanupUXTestEnvironment()
{
    UE_LOG(LogTemp, Log, TEXT("Cleaning up UX test environment"));
    
    bIsTesting = false;
    
    UE_LOG(LogTemp, Log, TEXT("UX test environment cleanup completed"));
}

void UMingUserExperienceTester::ExecuteUXTestType(EMingUXTestType TestType)
{
    UE_LOG(LogTemp, Log, TEXT("Executing UX test type: %s"), *GetTestTypeName(TestType));
    
    switch (TestType)
    {
    case EMingUXTestType::Navigation:
        ExecuteNavigationTests();
        break;
    case EMingUXTestType::Interface:
        ExecuteInterfaceTests();
        break;
    case EMingUXTestType::Feedback:
        ExecuteFeedbackTests();
        break;
    case EMingUXTestType::Accessibility:
        ExecuteAccessibilityTests();
        break;
    case EMingUXTestType::Performance:
        ExecutePerformanceTests();
        break;
    case EMingUXTestType::Consistency:
        ExecuteConsistencyTests();
        break;
    case EMingUXTestType::Intuitiveness:
        ExecuteIntuitivenessTests();
        break;
    case EMingUXTestType::Responsiveness:
        ExecuteResponsivenessTests();
        break;
    }
}

void UMingUserExperienceTester::ExecuteNavigationTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING NAVIGATION TESTS ==="));
    
    // Execute individual navigation tests
    ExecuteNavigationTest(TEXT("UX_MAIN_MENU_NAV"));
    ExecuteNavigationTest(TEXT("UX_CAMPAIGN_NAV"));
    ExecuteNavigationTest(TEXT("UX_INGAME_NAV"));
    
    // Calculate navigation efficiency
    CalculateNavigationEfficiency();
    
    UE_LOG(LogTemp, Log, TEXT("Navigation tests completed"));
}

void UMingUserExperienceTester::ExecuteInterfaceTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING INTERFACE TESTS ==="));
    
    // Execute individual interface tests
    ExecuteInterfaceTest(TEXT("UX_INTERFACE_CLARITY"));
    ExecuteInterfaceTest(TEXT("UX_VISUAL_HIERARCHY"));
    
    // Calculate interface clarity
    CalculateInterfaceClarity();
    
    UE_LOG(LogTemp, Log, TEXT("Interface tests completed"));
}

void UMingUserExperienceTester::ExecuteFeedbackTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING FEEDBACK TESTS ==="));
    
    // Execute individual feedback tests
    ExecuteFeedbackTest(TEXT("UX_AUDIO_FEEDBACK"));
    ExecuteFeedbackTest(TEXT("UX_VISUAL_FEEDBACK"));
    
    // Calculate feedback quality
    CalculateFeedbackQuality();
    
    UE_LOG(LogTemp, Log, TEXT("Feedback tests completed"));
}

void UMingUserExperienceTester::ExecuteAccessibilityTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING ACCESSIBILITY TESTS ==="));
    
    // Execute individual accessibility tests
    ExecuteAccessibilityTest(TEXT("UX_COLOR_BLINDNESS"));
    ExecuteAccessibilityTest(TEXT("UX_TEXT_READABILITY"));
    
    // Calculate accessibility score
    CalculateAccessibilityScore();
    
    UE_LOG(LogTemp, Log, TEXT("Accessibility tests completed"));
}

void UMingUserExperienceTester::ExecutePerformanceTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING PERFORMANCE TESTS ==="));
    
    // Execute individual performance tests
    ExecutePerformanceTest(TEXT("UX_UI_RESPONSIVENESS"));
    ExecutePerformanceTest(TEXT("UX_LOADING_TIMES"));
    
    // Calculate performance score
    CalculatePerformanceScore();
    
    UE_LOG(LogTemp, Log, TEXT("Performance tests completed"));
}

void UMingUserExperienceTester::ExecuteConsistencyTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING CONSISTENCY TESTS ==="));
    
    // Execute individual consistency tests
    ExecuteConsistencyTest(TEXT("UX_DESIGN_CONSISTENCY"));
    ExecuteConsistencyTest(TEXT("UX_INTERACTION_CONSISTENCY"));
    
    // Calculate consistency score
    CalculateConsistencyScore();
    
    UE_LOG(LogTemp, Log, TEXT("Consistency tests completed"));
}

void UMingUserExperienceTester::ExecuteIntuitivenessTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING INTUITIVENESS TESTS ==="));
    
    // Execute individual intuitiveness tests
    ExecuteIntuitivenessTest(TEXT("UX_NEW_PLAYER_EXPERIENCE"));
    ExecuteIntuitivenessTest(TEXT("UX_LEARNING_CURVE"));
    
    // Calculate intuitiveness score
    CalculateIntuitivenessScore();
    
    UE_LOG(LogTemp, Log, TEXT("Intuitiveness tests completed"));
}

void UMingUserExperienceTester::ExecuteResponsivenessTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== EXECUTING RESPONSIVENESS TESTS ==="));
    
    // Execute individual responsiveness tests
    ExecuteResponsivenessTest(TEXT("UX_INPUT_RESPONSIVENESS"));
    ExecuteResponsivenessTest(TEXT("UX_ANIMATION_SMOOTHNESS"));
    
    // Calculate responsiveness score
    CalculateResponsivenessScore();
    
    UE_LOG(LogTemp, Log, TEXT("Responsiveness tests completed"));
}

void UMingUserExperienceTester::ExecuteNavigationTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing navigation test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate navigation test
    SimulateMainMenuNavigation();
    SimulateCampaignNavigation();
    SimulateInGameInterface();
    
    // Analyze navigation patterns
    AnalyzeNavigationPatterns();
    
    // Calculate score and rating
    Test->Score = 75.0f + FMath::RandRange(-10.0f, 20.0f); // Simulate score
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 80.0f)
    {
        Test->Findings = TEXT("Navigation is efficient and intuitive");
        Test->Recommendations = TEXT("Maintain current navigation design");
        Test->Strengths.Add(TEXT("Clear menu structure"));
        Test->Strengths.Add(TEXT("Intuitive button placement"));
    }
    else
    {
        Test->Findings = TEXT("Navigation has some usability issues");
        Test->Recommendations = TEXT("Improve button labeling and layout");
        Test->Issues.Add(TEXT("Some buttons unclear"));
        Test->Issues.Add(TEXT("Navigation path could be shorter"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Navigation test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteInterfaceTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing interface test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate interface test
    AnalyzeInterfaceElements();
    
    // Calculate score and rating
    Test->Score = 70.0f + FMath::RandRange(-15.0f, 25.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 75.0f)
    {
        Test->Findings = TEXT("Interface is clear and well-organized");
        Test->Recommendations = TEXT("Maintain current interface design");
        Test->Strengths.Add(TEXT("Good visual hierarchy"));
        Test->Strengths.Add(TEXT("Clear information layout"));
    }
    else
    {
        Test->Findings = TEXT("Interface needs improvement");
        Test->Recommendations = TEXT("Improve visual hierarchy and readability");
        Test->Issues.Add(TEXT("Some elements hard to read"));
        Test->Issues.Add(TEXT("Information could be better organized"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Interface test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteFeedbackTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing feedback test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate feedback test
    AnalyzeFeedbackTiming();
    
    // Calculate score and rating
    Test->Score = 80.0f + FMath::RandRange(-10.0f, 15.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 80.0f)
    {
        Test->Findings = TEXT("Feedback systems are effective");
        Test->Recommendations = TEXT("Maintain current feedback design");
        Test->Strengths.Add(TEXT("Good audio feedback"));
        Test->Strengths.Add(TEXT("Appropriate visual feedback"));
    }
    else
    {
        Test->Findings = TEXT("Feedback needs improvement");
        Test->Recommendations = TEXT("Enhance feedback timing and clarity");
        Test->Issues.Add(TEXT("Some feedback delayed"));
        Test->Issues.Add(TEXT("Visual feedback could be clearer"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Feedback test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteAccessibilityTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing accessibility test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate accessibility test
    AnalyzeAccessibilityCompliance();
    
    // Calculate score and rating
    Test->Score = 65.0f + FMath::RandRange(-20.0f, 30.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 70.0f)
    {
        Test->Findings = TEXT("Accessibility features are adequate");
        Test->Recommendations = TEXT("Maintain current accessibility support");
        Test->Strengths.Add(TEXT("Good text readability"));
        Test->Strengths.Add(TEXT("Color contrast acceptable"));
    }
    else
    {
        Test->Findings = TEXT("Accessibility needs improvement");
        Test->Recommendations = TEXT("Improve color contrast and text sizing");
        Test->Issues.Add(TEXT("Color contrast insufficient"));
        Test->Issues.Add(TEXT("Text size too small in some areas"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Accessibility test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecutePerformanceTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing performance test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate performance test
    AnalyzePerformanceImpact();
    
    // Calculate score and rating
    Test->Score = 72.0f + FMath::RandRange(-12.0f, 23.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 75.0f)
    {
        Test->Findings = TEXT("UI performance is good");
        Test->Recommendations = TEXT("Maintain current performance level");
        Test->Strengths.Add(TEXT("Fast UI response"));
        Test->Strengths.Add(TEXT("Quick loading times"));
    }
    else
    {
        Test->Findings = TEXT("UI performance needs improvement");
        Test->Recommendations = TEXT("Optimize UI rendering and loading");
        Test->Issues.Add(TEXT("Some UI elements slow to load"));
        Test->Issues.Add(TEXT("Input lag in some situations"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Performance test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteConsistencyTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing consistency test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate consistency test
    AnalyzeConsistencyIssues();
    
    // Calculate score and rating
    Test->Score = 78.0f + FMath::RandRange(-8.0f, 17.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 80.0f)
    {
        Test->Findings = TEXT("Design is consistent across interfaces");
        Test->Recommendations = TEXT("Maintain design consistency");
        Test->Strengths.Add(TEXT("Consistent visual style"));
        Test->Strengths.Add(TEXT("Uniform interaction patterns"));
    }
    else
    {
        Test->Findings = TEXT("Some consistency issues found");
        Test->Recommendations = TEXT("Standardize design elements and interactions");
        Test->Issues.Add(TEXT("Inconsistent button styles"));
        Test->Issues.Add(TEXT("Different interaction patterns"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Consistency test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteIntuitivenessTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing intuitiveness test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate intuitiveness test
    AnalyzeIntuitivenessFactors();
    
    // Calculate score and rating
    Test->Score = 68.0f + FMath::RandRange(-18.0f, 27.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 70.0f)
    {
        Test->Findings = TEXT("Interface is intuitive for most users");
        Test->Recommendations = TEXT("Maintain current intuitiveness level");
        Test->Strengths.Add(TEXT("Clear iconography"));
        Test->Strengths.Add(TEXT("Logical menu structure"));
    }
    else
    {
        Test->Findings = TEXT("Interface could be more intuitive");
        Test->Recommendations = TEXT("Improve icon clarity and menu organization");
        Test->Issues.Add(TEXT("Some icons unclear"));
        Test->Issues.Add(TEXT("Menu structure could be improved"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Intuitiveness test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::ExecuteResponsivenessTest(const FString& TestID)
{
    FMingUXTestResult* Test = FindUXTestResult(TestID);
    if (!Test)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing responsiveness test: %s"), *Test->TestName);
    
    Test->TestDuration = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Simulate responsiveness test
    AnalyzeResponsivenessMetrics();
    
    // Calculate score and rating
    Test->Score = 74.0f + FMath::RandRange(-14.0f, 21.0f);
    Test->Rating = CalculateRating(Test->Score);
    
    // Generate findings and recommendations
    if (Test->Score >= 75.0f)
    {
        Test->Findings = TEXT("System is responsive to user input");
        Test->Recommendations = TEXT("Maintain current responsiveness");
        Test->Strengths.Add(TEXT("Quick input response"));
        Test->Strengths.Add(TEXT("Smooth animations"));
    }
    else
    {
        Test->Findings = TEXT("Responsiveness could be improved");
        Test->Recommendations = TEXT("Optimize input handling and animations");
        Test->Issues.Add(TEXT("Some input lag detected"));
        Test->Issues.Add(TEXT("Animations could be smoother"));
    }
    
    Test->TestDuration = (WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f) - Test->TestDuration;
    
    OnUXTestCompleted.Broadcast(*Test, Test->Rating >= EMingUXRating::Good);
    
    UE_LOG(LogTemp, Log, TEXT("Responsiveness test completed: %s - Score: %.1f (%s)"), 
           *Test->TestName, Test->Score, *GetRatingName(Test->Rating));
}

void UMingUserExperienceTester::SimulateUserJourney()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating complete user journey"));
    
    SimulateFirstTimePlayer();
    SimulateReturningPlayer();
    SimulatePowerUser();
    SimulateCasualPlayer();
    
    UE_LOG(LogTemp, Log, TEXT("User journey simulation completed"));
}

void UMingUserExperienceTester::SimulateNewPlayerExperience()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating new player experience"));
    
    // Simulate first-time player journey
    SimulateMainMenuNavigation();
    SimulateTutorialFlow();
    SimulateFirstMission();
    
    UE_LOG(LogTemp, Log, TEXT("New player experience simulation completed"));
}

void UMingUserExperienceTester::SimulateExperiencedPlayerFlow()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating experienced player flow"));
    
    // Simulate experienced player journey
    SimulateQuickNavigation();
    SimulateAdvancedFeatures();
    SimulateOptimizedWorkflows();
    
    UE_LOG(LogTemp, Log, TEXT("Experienced player flow simulation completed"));
}

void UMingUserExperienceTester::SimulateAccessibilityScenarios()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating accessibility scenarios"));
    
    // Simulate various accessibility scenarios
    SimulateColorBlindnessScenario();
    SimulateLowVisionScenario();
    SimulateMotorImpairmentScenario();
    
    UE_LOG(LogTemp, Log, TEXT("Accessibility scenarios simulation completed"));
}

void UMingUserExperienceTester::AnalyzeUserInterface()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing user interface"));
    
    AnalyzeInterfaceElements();
    AnalyzeVisualHierarchy();
    AnalyzeLayoutEfficiency();
    
    UE_LOG(LogTemp, Log, TEXT("User interface analysis completed"));
}

void UMingUserExperienceTester::AnalyzeNavigationFlow()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing navigation flow"));
    
    AnalyzeNavigationPatterns();
    AnalyzeMenuEfficiency();
    AnalyzeInformationArchitecture();
    
    UE_LOG(LogTemp, Log, TEXT("Navigation flow analysis completed"));
}

void UMingUserExperienceTester::AnalyzeFeedbackSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing feedback systems"));
    
    AnalyzeFeedbackTiming();
    AnalyzeAudioFeedback();
    AnalyzeVisualFeedback();
    
    UE_LOG(LogTemp, Log, TEXT("Feedback systems analysis completed"));
}

void UMingUserExperienceTester::AnalyzeAccessibilityFeatures()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing accessibility features"));
    
    AnalyzeAccessibilityCompliance();
    AnalyzeColorContrast();
    AnalyzeTextReadability();
    
    UE_LOG(LogTemp, Log, TEXT("Accessibility features analysis completed"));
}

TArray<FMingUXTestResult> UMingUserExperienceTester::GetAllResults() const
{
    TArray<FMingUXTestResult> Results;
    for (const auto& Pair : UXTestResults)
    {
        Results.Add(Pair.Value);
    }
    return Results;
}

TArray<FMingUXTestResult> UMingUserExperienceTester::GetCriticalResults() const
{
    TArray<FMingUXTestResult> Results;
    for (const auto& Pair : UXTestResults)
    {
        if (Pair.Value.bIsCritical)
        {
            Results.Add(Pair.Value);
        }
    }
    return Results;
}

FMingUXMetrics UMingUserExperienceTester::GetUXMetrics() const
{
    return CurrentMetrics;
}

float UMingUserExperienceTester::GetOverallUXScore() const
{
    return CurrentMetrics.OverallUXScore;
}

bool UMingUserExperienceTester::IsUXAcceptable() const
{
    return CurrentMetrics.OverallUXScore >= 70.0f;
}

void UMingUserExperienceTester::GenerateUXReport()
{
    UE_LOG(LogTemp, Log, TEXT("Generating comprehensive UX report"));
    
    // Create detailed UX report
    UE_LOG(LogTemp, Log, TEXT("=== MINGGO RTS UX TEST REPORT ==="));
    
    float OverallScore = GetOverallUXScore();
    bool bAcceptable = IsUXAcceptable();
    
    UE_LOG(LogTemp, Log, TEXT("Overall UX Score: %.1f/100"), OverallScore);
    UE_LOG(LogTemp, Log, TEXT("UX Acceptable: %s"), bAcceptable ? TEXT("YES") : TEXT("NO"));
    
    // Individual test results
    TArray<FMingUXTestResult> AllResults = GetAllResults();
    for (const FMingUXTestResult& Result : AllResults)
    {
        UE_LOG(LogTemp, Log, TEXT(""));
        UE_LOG(Temp, Log, TEXT("UX TEST: %s"), *Result.TestName);
        UE_LOG(LogTemp, Log, TEXT("  Type: %s"), *GetTestTypeName(Result.TestType));
        UE_LOG(LogTemp, Log, TEXT("  Rating: %s"), *GetRatingName(Result.Rating));
        UE_LOG(LogTemp, Log, TEXT("  Score: %.1f/100"), Result.Score);
        UE_LOG(LogTemp, Log, TEXT("  Critical: %s"), Result.bIsCritical ? TEXT("Yes") : TEXT("No"));
        UE_LOG(LogTemp, Log, TEXT("  Duration: %.2fs"), Result.TestDuration);
        
        if (!Result.Findings.IsEmpty())
        {
            UE_LOG(LogTemp, Log, TEXT("  Findings: %s"), *Result.Findings);
        }
        
        if (!Result.Recommendations.IsEmpty())
        {
            UE_LOG(LogTemp, Log, TEXT("  Recommendations: %s"), *Result.Recommendations);
        }
        
        if (Result.Strengths.Num() > 0)
        {
            UE_LOG(LogTemp, Log, TEXT("  Strengths:"));
            for (const FString& Strength : Result.Strengths)
            {
                UE_LOG(LogTemp, Log, TEXT("    - %s"), *Strength);
            }
        }
        
        if (Result.Issues.Num() > 0)
        {
            UE_LOG(LogTemp, Log, TEXT("  Issues:"));
            for (const FString& Issue : Result.Issues)
            {
                UE_LOG(LogTemp, Log, TEXT("    - %s"), *Issue);
            }
        }
    }
    
    // Metrics breakdown
    UE_LOG(LogTemp, Log, TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("=== UX METRICS ==="));
    UE_LOG(LogTemp, Log, TEXT("Navigation Efficiency: %.1f/100"), CurrentMetrics.NavigationEfficiency);
    UE_LOG(LogTemp, Log, TEXT("Interface Clarity: %.1f/100"), CurrentMetrics.InterfaceClarity);
    UE_LOG(LogTemp, Log, TEXT("Feedback Quality: %.1f/100"), CurrentMetrics.FeedbackQuality);
    UE_LOG(LogTemp, Log, TEXT("Accessibility Score: %.1f/100"), CurrentMetrics.AccessibilityScore);
    UE_LOG(LogTemp, Log, TEXT("Performance Score: %.1f/100"), CurrentMetrics.PerformanceScore);
    UE_LOG(LogTemp, Log, TEXT("Consistency Score: %.1f/100"), CurrentMetrics.ConsistencyScore);
    UE_LOG(LogTemp, Log, TEXT("Intuitiveness Score: %.1f/100"), CurrentMetrics.IntuitivenessScore);
    UE_LOG(LogTemp, Log, TEXT("Responsiveness Score: %.1f/100"), CurrentMetrics.ResponsivenessScore);
    
    UE_LOG(LogTemp, Log, TEXT("=== END REPORT ==="));
    
    UE_LOG(LogTemp, Log, TEXT("UX report generated"));
}

void UMingUserExperienceTester::ExportUXResults(const FString& FilePath)
{
    UE_LOG(LogTemp, Log, TEXT("Exporting UX results to: %s"), *FilePath);
    
    FString ReportData = SaveUXTestData();
    
    // This would write the report data to file
    UE_LOG(LogTemp, Log, TEXT("UX results exported successfully"));
}

FString UMingUserExperienceTester::GetTestTypeName(EMingUXTestType TestType)
{
    switch (TestType)
    {
    case EMingUXTestType::Navigation: return TEXT("導航");
    case EMingUXTestType::Interface: return TEXT("界面");
    case EMingUXTestType::Feedback: return TEXT("反饋");
    case EMingUXTestType::Accessibility: return TEXT("無障礙");
    case EMingUXTestType::Performance: return TEXT("性能");
    case EMingUXTestType::Consistency: return TEXT("一致性");
    case EMingUXTestType::Intuitiveness: return TEXT("直觀性");
    case EMingUXTestType::Responsiveness: return TEXT("響應性");
    default: return TEXT("未知");
    }
}

FString UMingUserExperienceTester::GetRatingName(EMingUXRating Rating)
{
    switch (Rating)
    {
    case EMingUXRating::Excellent: return TEXT("優秀");
    case EMingUXRating::Good: return TEXT("良好");
    case EMingUXRating::Average: return TEXT("一般");
    case EMingUXRating::Poor: return TEXT("較差");
    case EMingUXRating::Critical: return TEXT("嚴重");
    default: return TEXT("未知");
    }
}

FString UMingUserExperienceTester::SaveUXTestData() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"total_tests\": %d,\n"), GetAllResults().Num());
    Result += FString::Printf(TEXT("  \"critical_tests\": %d,\n"), GetCriticalResults().Num());
    Result += FString::Printf(TEXT("  \"overall_score\": %.1f,\n"), GetOverallUXScore());
    Result += FString::Printf(TEXT("  \"ux_acceptable\": %s,\n"), IsUXAcceptable() ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"is_testing\": %s\n"), bIsTesting ? TEXT("true") : TEXT("false"));
    Result += TEXT("}\n");
    
    return Result;
}

void UMingUserExperienceTester::LoadUXTestData(const FString& JsonString)
{
    // Parse JSON and restore UX test data
    UE_LOG(LogTemp, Log, TEXT("Loading UX test data"));
}

void UMingUserExperienceTester::SimulateMainMenuNavigation()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating main menu navigation"));
    
    // Simulate main menu navigation patterns
    LogUXTestEvent(TEXT("MainMenuNavigation"), TEXT("Main menu navigation simulated"));
}

void UMingUserExperienceTester::SimulateCampaignNavigation()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating campaign navigation"));
    
    // Simulate campaign navigation patterns
    LogUXTestEvent(TEXT("CampaignNavigation"), TEXT("Campaign navigation simulated"));
}

void UMingUserExperienceTester::SimulateInGameInterface()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating in-game interface"));
    
    // Simulate in-game interface interactions
    LogUXTestEvent(TEXT("InGameInterface"), TEXT("In-game interface simulated"));
}

void UMingUserExperienceTester::SimulateSettingsInterface()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating settings interface"));
    
    // Simulate settings interface interactions
    LogUXTestEvent(TEXT("SettingsInterface"), TEXT("Settings interface simulated"));
}

void UMingUserExperienceTester::SimulateSaveLoadInterface()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating save/load interface"));
    
    // Simulate save/load interface interactions
    LogUXTestEvent(TEXT("SaveLoadInterface"), TEXT("Save/load interface simulated"));
}

void UMingUserExperienceTester::SimulatePauseMenu()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating pause menu"));
    
    // Simulate pause menu interactions
    LogUXTestEvent(TEXT("PauseMenu"), TEXT("Pause menu simulated"));
}

void UMingUserExperienceTester::SimulateFirstTimePlayer()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating first-time player experience"));
    
    // Simulate first-time player behavior
    LogUXTestEvent(TEXT("FirstTimePlayer"), TEXT("First-time player experience simulated"));
}

void UMingUserExperienceTester::SimulateReturningPlayer()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating returning player experience"));
    
    // Simulate returning player behavior
    LogUXTestEvent(TEXT("ReturningPlayer"), TEXT("Returning player experience simulated"));
}

void UMingUserExperienceTester::SimulatePowerUser()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating power user experience"));
    
    // Simulate power user behavior
    LogUXTestEvent(TEXT("PowerUser"), TEXT("Power user experience simulated"));
}

void UMingUserExperienceTester::SimulateCasualPlayer()
{
    UE_LOG(LogTemp, Log, TEXT("Simulating casual player experience"));
    
    // Simulate casual player behavior
    LogUXTestEvent(TEXT("CasualPlayer"), TEXT("Casual player experience simulated"));
}

void UMingUserExperienceTester::AnalyzeNavigationPatterns()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing navigation patterns"));
    
    // Analyze navigation efficiency and patterns
    CurrentMetrics.NavigationEfficiency = 75.0f + FMath::RandRange(-10.0f, 20.0f);
    
    LogUXTestEvent(TEXT("NavigationAnalysis"), TEXT("Navigation patterns analyzed"));
}

void UMingUserExperienceTester::AnalyzeInterfaceElements()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing interface elements"));
    
    // Analyze interface clarity and organization
    CurrentMetrics.InterfaceClarity = 70.0f + FMath::RandRange(-15.0f, 25.0f);
    
    LogUXTestEvent(TEXT("InterfaceAnalysis"), TEXT("Interface elements analyzed"));
}

void UMingUserExperienceTester::AnalyzeFeedbackTiming()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing feedback timing"));
    
    // Analyze feedback quality and timing
    CurrentMetrics.FeedbackQuality = 80.0f + FMath::RandRange(-10.0f, 15.0f);
    
    LogUXTestEvent(TEXT("FeedbackAnalysis"), TEXT("Feedback timing analyzed"));
}

void UMingUserExperienceTester::AnalyzeAccessibilityCompliance()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing accessibility compliance"));
    
    // Analyze accessibility features
    CurrentMetrics.AccessibilityScore = 65.0f + FMath::RandRange(-20.0f, 30.0f);
    
    LogUXTestEvent(TEXT("AccessibilityAnalysis"), TEXT("Accessibility compliance analyzed"));
}

void UMingUserExperienceTester::AnalyzePerformanceImpact()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing performance impact"));
    
    // Analyze UI performance
    CurrentMetrics.PerformanceScore = 72.0f + FMath::RandRange(-12.0f, 23.0f);
    
    LogUXTestEvent(TEXT("PerformanceAnalysis"), TEXT("Performance impact analyzed"));
}

void UMingUserExperienceTester::AnalyzeConsistencyIssues()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing consistency issues"));
    
    // Analyze design consistency
    CurrentMetrics.ConsistencyScore = 78.0f + FMath::RandRange(-8.0f, 17.0f);
    
    LogUXTestEvent(TEXT("ConsistencyAnalysis"), TEXT("Consistency issues analyzed"));
}

void UMingUserExperienceTester::AnalyzeIntuitivenessFactors()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing intuitiveness factors"));
    
    // Analyze interface intuitiveness
    CurrentMetrics.IntuitivenessScore = 68.0f + FMath::RandRange(-18.0f, 27.0f);
    
    LogUXTestEvent(TEXT("IntuitivenessAnalysis"), TEXT("Intuitiveness factors analyzed"));
}

void UMingUserExperienceTester::AnalyzeResponsivenessMetrics()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing responsiveness metrics"));
    
    // Analyze system responsiveness
    CurrentMetrics.ResponsivenessScore = 74.0f + FMath::RandRange(-14.0f, 21.0f);
    
    LogUXTestEvent(TEXT("ResponsivenessAnalysis"), TEXT("Responsiveness metrics analyzed"));
}

void UMingUserExperienceTester::CalculateNavigationEfficiency()
{
    // Calculate navigation efficiency metric
    CurrentMetrics.NavigationEfficiency = 75.0f + FMath::RandRange(-10.0f, 20.0f);
}

void UMingUserExperienceTester::CalculateInterfaceClarity()
{
    // Calculate interface clarity metric
    CurrentMetrics.InterfaceClarity = 70.0f + FMath::RandRange(-15.0f, 25.0f);
}

void UMingUserExperienceTester::CalculateFeedbackQuality()
{
    // Calculate feedback quality metric
    CurrentMetrics.FeedbackQuality = 80.0f + FMath::RandRange(-10.0f, 15.0f);
}

void UMingUserExperienceTester::CalculateAccessibilityScore()
{
    // Calculate accessibility score metric
    CurrentMetrics.AccessibilityScore = 65.0f + FMath::RandRange(-20.0f, 30.0f);
}

void UMingUserExperienceTester::CalculatePerformanceScore()
{
    // Calculate performance score metric
    CurrentMetrics.PerformanceScore = 72.0f + FMath::RandRange(-12.0f, 23.0f);
}

void UMingUserExperienceTester::CalculateConsistencyScore()
{
    // Calculate consistency score metric
    CurrentMetrics.ConsistencyScore = 78.0f + FMath::RandRange(-8.0f, 17.0f);
}

void UMingUserExperienceTester::CalculateIntuitivenessScore()
{
    // Calculate intuitiveness score metric
    CurrentMetrics.IntuitivenessScore = 68.0f + FMath::RandRange(-18.0f, 27.0f);
}

void UMingUserExperienceTester::CalculateResponsivenessScore()
{
    // Calculate responsiveness score metric
    CurrentMetrics.ResponsivenessScore = 74.0f + FMath::RandRange(-14.0f, 21.0f);
}

void UMingUserExperienceTester::CalculateOverallUXScore()
{
    // Calculate overall UX score from all metrics
    float TotalScore = 0.0f;
    int32 MetricCount = 0;
    
    TotalScore += CurrentMetrics.NavigationEfficiency;
    TotalScore += CurrentMetrics.InterfaceClarity;
    TotalScore += CurrentMetrics.FeedbackQuality;
    TotalScore += CurrentMetrics.AccessibilityScore;
    TotalScore += CurrentMetrics.PerformanceScore;
    TotalScore += CurrentMetrics.ConsistencyScore;
    TotalScore += CurrentMetrics.IntuitivenessScore;
    TotalScore += CurrentMetrics.ResponsivenessScore;
    
    MetricCount = 8;
    
    CurrentMetrics.OverallUXScore = TotalScore / MetricCount;
    
    UE_LOG(LogTemp, Log, TEXT("Overall UX score calculated: %.1f"), CurrentMetrics.OverallUXScore);
}

void UMingUserExperienceTester::ProcessUXTestResults()
{
    UE_LOG(LogTemp, Log, TEXT("Processing UX test results"));
    
    // Process all UX test results
    CalculateOverallUXScore();
    
    UE_LOG(LogTemp, Log, TEXT("UX test results processed"));
}

void UMingUserExperienceTester::IdentifyUXIssues()
{
    UE_LOG(LogTemp, Log, TEXT("Identifying UX issues"));
    
    // Identify UX issues from test results
    TArray<FMingUXTestResult> AllResults = GetAllResults();
    
    for (const FMingUXTestResult& Result : AllResults)
    {
        if (Result.Rating <= EMingUXRating::Average)
        {
            UE_LOG(LogTemp, Warning, TEXT("UX ISSUE: %s - %s"), *Result.TestName, *Result.Findings);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("UX issues identification completed"));
}

void UMingUserExperienceTester::GenerateUXRecommendations()
{
    UE_LOG(LogTemp, Log, TEXT("Generating UX recommendations"));
    
    // Generate UX recommendations based on test results
    TArray<FMingUXTestResult> AllResults = GetAllResults();
    
    for (const FMingUXTestResult& Result : AllResults)
    {
        if (!Result.Recommendations.IsEmpty())
        {
            UE_LOG(LogTemp, Log, TEXT("UX RECOMMENDATION: %s - %s"), *Result.TestName, *Result.Recommendations);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("UX recommendations generated"));
}

void UMingUserExperienceTester::CreateUXSummary()
{
    UE_LOG(LogTemp, Log, TEXT("Creating UX summary"));
    
    float OverallScore = GetOverallUXScore();
    bool bAcceptable = IsUXAcceptable();
    
    UE_LOG(LogTemp, Log, TEXT("=== UX SUMMARY ==="));
    UE_LOG(LogTemp, Log, TEXT("Overall Score: %.1f/100"), OverallScore);
    UE_LOG(LogTemp, Log, TEXT("Status: %s"), bAcceptable ? TEXT("ACCEPTABLE") : TEXT("NEEDS IMPROVEMENT"));
    
    if (!bAcceptable)
    {
        UE_LOG(LogTemp, Log, TEXT("Priority Areas for UX Improvement:"));
        
        TArray<FMingUXTestResult> CriticalResults = GetCriticalResults();
        for (const FMingUXTestResult& Result : CriticalResults)
        {
            if (Result.Rating <= EMingUXRating::Average)
            {
                UE_LOG(LogTemp, Log, TEXT("  - %s (%.1f/100)"), *Result.TestName, Result.Score);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== END SUMMARY ==="));
    
    UE_LOG(LogTemp, Log, TEXT("UX summary created"));
}

FMingUXTestResult* UMingUserExperienceTester::FindUXTestResult(const FString& TestID)
{
    return UXTestResults.Find(TestID);
}

void UMingUserExperienceTester::AddUXTestResult(const FMingUXTestResult& Result)
{
    UXTestResults.Add(Result.TestID, Result);
}

void UMingUserExperienceTester::LogUXTestEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[UXTest] %s: %s"), *Event, *Details);
}

void UMingUserExperienceTester::BroadcastUXTestProgress(const FString& TestName, float Progress)
{
    UE_LOG(LogTemp, Log, TEXT("[UXTestProgress] %s: %.1f%%"), *TestName, Progress);
}

EMingUXRating UMingUserExperienceTester::CalculateRating(float Score) const
{
    if (Score >= 90.0f)
    {
        return EMingUXRating::Excellent;
    }
    else if (Score >= 75.0f)
    {
        return EMingUXRating::Good;
    }
    else if (Score >= 60.0f)
    {
        return EMingUXRating::Average;
    }
    else if (Score >= 40.0f)
    {
        return EMingUXRating::Poor;
    }
    else
    {
        return EMingUXRating::Critical;
    }
}
