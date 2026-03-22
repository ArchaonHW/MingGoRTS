#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAITaskIntegration.h"
#include "MingAITaskIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class ETestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error
};

USTRUCT(BlueprintType)
struct FTestReport
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETestResult Result;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;
};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAITaskIntegrationTest : public UObject
{
    GENERATED_BODY()

public:
    UMingAITaskIntegrationTest();

    // Initialize test suite
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    void InitializeTestSuite();

    // Run all tests
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    TArray<FTestReport> RunAllTests();

    // Run specific test category
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    TArray<FTestReport> RunTestCategory(const FString& Category);

    // Individual test methods
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestTaskGeneratorInitialization();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestBehaviorPredictorInitialization();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestPersonalizedTaskGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestContextualTaskGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestPlayerActionRecording();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestBehaviorPrediction();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestPatternRecognition();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestContentPreload();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestPlayerInsightGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestTaskCompletionAnalysis();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestRealTimePrediction();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestCacheSystem();

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FTestReport TestEventSystem();

    // Get test summary
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FString GetTestSummary(const TArray<FTestReport>& TestReports);

protected:
    // Test helper methods
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    bool AssertTaskValid(const FDynamicTask& Task, FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    bool AssertPredictionValid(const FPredictionResult& Prediction, FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    bool AssertInsightValid(const FTaskBehaviorInsight& Insight, FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FPlayerAction CreateTestAction(EPlayerActionType ActionType);

    UFUNCTION(BlueprintCallable, Category = "AI Task Integration Test")
    FPlayerBehaviorProfile CreateTestProfile();

private:
    UPROPERTY(Category = "AI Task Integration Test")
    TObjectPtr<UMingAITaskIntegration> TestIntegration;

    UPROPERTY(Category = "AI Task Integration Test")
    TArray<FTestReport> TestResults;

    // Helper methods
    FTestReport CreateTestReport(const FString& TestName, ETestResult Result, const FString& ErrorMessage = TEXT(""), float ExecutionTime = 0.0f);
    void LogTestResult(const FTestReport& Report);
};
