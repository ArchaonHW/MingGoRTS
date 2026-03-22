// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 8.1: Self-Learning System Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SelfLearning/MingRTSSelfLearningSystem.h"
#include "SelfLearning/MingRTSAILearningIntegration.h"
#include "MingRTSSelfLearningSystemTest.generated.h"

// Test result types
UENUM(BlueprintType)
enum class ETestResult : uint8
{
    NotRun,       // Test hasn't been executed
    Passed,       // Test passed successfully
    Failed,       // Test failed
    Skipped,      // Test was skipped
    Error         // Test encountered an error
};

// Test categories
UENUM(BlueprintType)
enum class ETestCategory : uint8
{
    SystemInitialization,    // System startup and initialization tests
    DataCollection,         // Data collection and storage tests
    ModelTraining,          // Model training and prediction tests
    AIIntegration,          // AI learning integration tests
    AdaptationMechanisms,   // System adaptation tests
    PerformanceMetrics,     // Performance and accuracy tests
    DataValidation,         // Data integrity and validation tests
    Concurrency,            // Multi-threading and concurrency tests
    ErrorHandling,          // Error handling and recovery tests
    Integration             // End-to-end integration tests
};

// Individual test result
USTRUCT(BlueprintType)
struct FTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    ETestCategory Category;

    UPROPERTY(BlueprintReadOnly)
    ETestResult Result;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> TestMetrics;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// Test suite summary
USTRUCT(BlueprintType)
struct FTestSuiteSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalTests;

    UPROPERTY(BlueprintReadOnly)
    int32 PassedTests;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedTests;

    UPROPERTY(BlueprintReadOnly)
    int32 SkippedTests;

    UPROPERTY(BlueprintReadOnly)
    int32 ErrorTests;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    TMap<ETestCategory, int32> CategoryResults;

    UPROPERTY(BlueprintReadOnly)
    TArray<FTestResult> AllTestResults;
};

/**
 * Self-Learning System Test Suite
 * 
 * Comprehensive test suite for validating the self-learning system functionality,
 * including data collection, model training, AI integration, and adaptation mechanisms.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (MingGoRTS))
class MINGGORTS_API UMingRTSSelfLearningSystemTest : public UObject
{
    GENERATED_BODY()

public:
    // Constructor and initialization
    UMingRTSSelfLearningSystemTest();
    virtual void BeginDestroy() override;

    // Test suite execution
    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test")
    void InitializeTestSuite(UMingRTSSelfLearningSystem* LearningSystem, UMingRTSAILearningIntegration* AIIntegration};

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test")
    FTestSuiteSummary RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test")
    FTestSuiteSummary RunTestCategory(ETestCategory Category};

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test")
    FTestResult RunSingleTest(const FString& TestName};

    // Individual test methods
    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|System")
    FTestResult TestSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|System")
    FTestResult TestSystemShutdown();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Data")
    FTestResult TestUserBehaviorCollection();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Data")
    FTestResult TestGameplayMetricsCollection();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Data")
    FTestResult TestCustomDataCollection();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Data")
    FTestResult TestDataValidation();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Models")
    FTestResult TestModelCreation();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Models")
    FTestResult TestModelTraining();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Models")
    FTestResult TestModelPrediction();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Models")
    FTestResult TestModelPersistence();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|AI")
    FTestResult TestAIIntegration();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|AI")
    FTestResult TestPlayerBehaviorAnalysis();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|AI")
    FTestResult TestStrategyPatternRecognition();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|AI")
    FTestResult TestAIAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Adaptation")
    FTestResult TestAdaptiveParameters();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Adaptation")
    FTestResult TestSystemAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Performance")
    FTestResult TestLearningAccuracy();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Performance")
    FTestResult TestPredictionPerformance();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Performance")
    FTestResult TestMemoryUsage();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Concurrency")
    FTestResult TestConcurrentDataCollection();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Concurrency")
    FTestResult TestConcurrentModelTraining();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Error")
    FTestResult TestInvalidDataHandling();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Error")
    FTestResult TestModelFailureRecovery();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Integration")
    FTestResult TestEndToEndLearning();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Integration")
    FTestResult TestCrossSystemIntegration();

    // Test utilities
    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Utilities")
    void GenerateTestData(int32 NumDataPoints};

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Utilities")
    void ClearTestData();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Utilities")
    FTestSuiteSummary GetLastTestResults() const;

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Utilities")
    void ExportTestResults(const FString& FilePath};

    // Performance benchmarks
    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Benchmarks")
    void RunPerformanceBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Self-Learning Test|Benchmarks")
    float GetBenchmarkResult(const FString& BenchmarkName) const;

    // Events and delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, const FTestResult&, TestResult};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FTestSuiteSummary&, Summary};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestProgress, int32, CurrentTest, int32, TotalTests};

    UPROPERTY(BlueprintAssignable)
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTestSuiteCompleted OnTestSuiteCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTestProgress OnTestProgress;

protected:
    // Test execution helpers
    FTestResult ExecuteTest(const FString& TestName, TFunction<void()> TestFunction};
    void RecordTestResult(const FTestResult& Result};
    void UpdateTestProgress();

    // Test data generators
    void GenerateUserBehaviorData(int32 Count};
    void GenerateGameplayMetricsData(int32 Count};
    void GenerateCustomData(ELearningDataType DataType, int32 Count};

    // Validation helpers
    bool ValidateLearningSystem() const;
    bool ValidateAIIntegration() const;
    bool ValidateTestData() const;

    // Performance measurement
    double MeasureExecutionTime(TFunction<void()> Function};
    void RecordBenchmark(const FString& Name, double Time};

    // Test assertion helpers
    bool AssertTrue(bool Condition, const FString& Message};
    bool AssertFalse(bool Condition, const FString& Message};
    bool AssertEqual(const FString& Expected, const FString& Actual, const FString& Message};
    bool AssertNotEqual(const FString& Expected, const FString& Actual, const FString& Message};
    bool AssertNotNull(const void* Pointer, const FString& Message};
    bool AssertNull(const void* Pointer, const FString& Message};

private:
    // System references
    UPROPERTY()
    TObjectPtr<UMingRTSSelfLearningSystem> TestLearningSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSAILearningIntegration> TestAIIntegration;

    // Test execution state
    UPROPERTY()
    TArray<FTestResult> TestResults;

    UPROPERTY()
    int32 CurrentTestIndex;

    UPROPERTY()
    bool bTestSuiteRunning;

    // Test data
    UPROPERTY()
    TArray<FUserBehaviorData> TestBehaviorData;

    UPROPERTY()
    TArray<FGameplayMetricsData> TestMetricsData;

    UPROPERTY()
    TMap<ELearningDataType, TArray<TMap<FString, FString>>> TestCustomData;

    // Performance benchmarks
    UPROPERTY()
    TMap<FString, double> BenchmarkResults;

    // Test configuration
    UPROPERTY()
    bool bVerboseLogging;

    UPROPERTY()
    bool bStopOnFirstFailure;

    UPROPERTY()
    int32 TestDataSize;

    // Test statistics
    UPROPERTY()
    double TotalTestTime;

    UPROPERTY()
    int32 TestsExecuted;

    UPROPERTY()
    int32 TestsPassed;

    UPROPERTY()
    int32 TestsFailed;
};

#endif // MINGRTSSELFSLEARNINGSYSTEMTEST_H

