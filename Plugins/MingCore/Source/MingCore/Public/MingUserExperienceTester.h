#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUserExperienceTester.generated.h"

UENUM(BlueprintType)
enum class EMingUXTestType : uint8
{
    Navigation UMETA(DisplayName = "Navigation"),
    Interface UMETA(DisplayName = "Interface"),
    Feedback UMETA(DisplayName = "Feedback"),
    Accessibility UMETA(DisplayName = "Accessibility"),
    Performance UMETA(DisplayName = "Performance"),
    Consistency UMETA(DisplayName = "Consistency"),
    Intuitiveness UMETA(DisplayName = "Intuitiveness"),
    Responsiveness UMETA(DisplayName = "Responsiveness")
};

UENUM(BlueprintType)
enum class EMingUXRating : uint8
{
    Excellent UMETA(DisplayName = "Excellent"),
    Good UMETA(DisplayName = "Good"),
    Average UMETA(DisplayName = "Average"),
    Poor UMETA(DisplayName = "Poor"),
    Critical UMETA(DisplayName = "Critical")
};

USTRUCT(BlueprintType)
struct FMingUXTestResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUXTestType TestType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUXRating Rating;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Score;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Findings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Recommendations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Issues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Strengths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCritical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TestDuration;

    FMingUXTestResult()
        : TestType(EMingUXTestType::Navigation)
        , Rating(EMingUXRating::Average)
        , Score(50.0f)
        , bIsCritical(false)
        , TestDuration(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingUXMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NavigationEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InterfaceClarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FeedbackQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AccessibilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PerformanceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsistencyScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IntuitivenessScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResponsivenessScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallUXScore;

    FMingUXMetrics()
        : NavigationEfficiency(0.0f)
        , InterfaceClarity(0.0f)
        , FeedbackQuality(0.0f)
        , AccessibilityScore(0.0f)
        , PerformanceScore(0.0f)
        , ConsistencyScore(0.0f)
        , IntuitivenessScore(0.0f)
        , ResponsivenessScore(0.0f)
        , OverallUXScore(0.0f)
    {}
};

/**
 * User Experience Tester for MingGoRTS
 * Tests and evaluates user experience across all game interfaces
 */
UCLASS(ClassGroup = (Testing), Blueprintable)
class MINGGAMEFLOW_API UMingUserExperienceTester : public UObject
{
    GENERATED_BODY()

public:
    UMingUserExperienceTester();

    // UX Testing Control
    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteAllUXTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteUXTestType(EMingUXTestType TestType);

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteNavigationTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteInterfaceTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteFeedbackTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteAccessibilityTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecutePerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteConsistencyTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteIntuitivenessTests();

    UFUNCTION(BlueprintCallable, Category = "UX Testing")
    void ExecuteResponsivenessTests();

    // UX Simulation
    UFUNCTION(BlueprintCallable, Category = "UX Simulation")
    void SimulateUserJourney();

    UFUNCTION(BlueprintCallable, Category = "UX Simulation")
    void SimulateNewPlayerExperience();

    UFUNCTION(BlueprintCallable, Category = "UX Simulation")
    void SimulateExperiencedPlayerFlow();

    UFUNCTION(BlueprintCallable, Category = "UX Simulation")
    void SimulateAccessibilityScenarios();

    // UX Analysis
    UFUNCTION(BlueprintCallable, Category = "UX Analysis")
    void AnalyzeUserInterface();

    UFUNCTION(BlueprintCallable, Category = "UX Analysis")
    void AnalyzeNavigationFlow();

    UFUNCTION(BlueprintCallable, Category = "UX Analysis")
    void AnalyzeFeedbackSystems();

    UFUNCTION(BlueprintCallable, Category = "UX Analysis")
    void AnalyzeAccessibilityFeatures();

    // Results Evaluation
    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingUXTestResult> GetAllResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingUXTestResult> GetCriticalResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    FMingUXMetrics GetUXMetrics() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    float GetOverallUXScore() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    bool IsUXAcceptable() const;

    UFUNCTION(BlueprintCallable, Category = "Results")
    void GenerateUXReport();

    UFUNCTION(BlueprintCallable, Category = "Results")
    void ExportUXResults(const FString& FilePath);

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUXTestStarted, EMingUXTestType, TestType, const FString&, TestName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUXTestCompleted, const FMingUXTestResult&, Result, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllUXTestsCompleted, float, OverallScore);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUXTestStarted OnUXTestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUXTestCompleted OnUXTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAllUXTestsCompleted OnAllUXTestsCompleted;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetTestTypeName(EMingUXTestType TestType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetRatingName(EMingUXRating Rating);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveUXTestData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadUXTestData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingUXTestResult> UXTestResults;

    UPROPERTY()
    FMingUXMetrics CurrentMetrics;

    UPROPERTY()
    bool bIsTesting;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeUXTests();
    void SetupUXTestEnvironment();
    void CleanupUXTestEnvironment();

    // Test Execution
    void ExecuteNavigationTest(const FString& TestID);
    void ExecuteInterfaceTest(const FString& TestID);
    void ExecuteFeedbackTest(const FString& TestID);
    void ExecuteAccessibilityTest(const FString& TestID);
    void ExecutePerformanceTest(const FString& TestID);
    void ExecuteConsistencyTest(const FString& TestID);
    void ExecuteIntuitivenessTest(const FString& TestID);
    void ExecuteResponsivenessTest(const FString& TestID);

    // UX Simulation Functions
    void SimulateMainMenuNavigation();
    void SimulateCampaignNavigation();
    void SimulateInGameInterface();
    void SimulateSettingsInterface();
    void SimulateSaveLoadInterface();
    void SimulatePauseMenu();

    // User Journey Simulation
    void SimulateFirstTimePlayer();
    void SimulateReturningPlayer();
    void SimulatePowerUser();
    void SimulateCasualPlayer();

    // Analysis Functions
    void AnalyzeNavigationPatterns();
    void AnalyzeInterfaceElements();
    void AnalyzeFeedbackTiming();
    void AnalyzeAccessibilityCompliance();
    void AnalyzePerformanceImpact();
    void AnalyzeConsistencyIssues();
    void AnalyzeIntuitivenessFactors();
    void AnalyzeResponsivenessMetrics();

    // Metrics Calculation
    void CalculateNavigationEfficiency();
    void CalculateInterfaceClarity();
    void CalculateFeedbackQuality();
    void CalculateAccessibilityScore();
    void CalculatePerformanceScore();
    void CalculateConsistencyScore();
    void CalculateIntuitivenessScore();
    void CalculateResponsivenessScore();
    void CalculateOverallUXScore();

    // Results Processing
    void ProcessUXTestResults();
    void IdentifyUXIssues();
    void GenerateUXRecommendations();
    void CreateUXSummary();

    // Helpers
    FMingUXTestResult* FindUXTestResult(const FString& TestID);
    void AddUXTestResult(const FMingUXTestResult& Result);
    void LogUXTestEvent(const FString& Event, const FString& Details);
    void BroadcastUXTestProgress(const FString& TestName, float Progress);
    EMingUXRating CalculateRating(float Score) const;
};
