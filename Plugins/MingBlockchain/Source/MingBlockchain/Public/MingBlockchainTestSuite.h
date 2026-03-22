#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/AutomationTest.h"
#include "MingBlockchainTestSuite.generated.h"

UENUM(BlueprintType)
enum class EBlockchainTestCategory: uint8 {
    MiningSystem,
    WalletSystem,
    PlayToEarnSystem,
    TransactionManager,
    NFTSystem,
    SettingsSystem,
    Integration,
    Performance,
    Security,
    EndToEnd
};

UENUM(BlueprintType)
enum class EBlockchainTestResult: uint8 {
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error,
    Timeout
};

USTRUCT(BlueprintType)
struct FBlockchainTestCase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString TestName;

    UPROPERTY(BlueprintReadWrite)
    EBlockchainTestCategory Category;

    UPROPERTY(BlueprintReadWrite)
    FString Description;

    UPROPERTY(BlueprintReadWrite)
    EBlockchainTestResult Result;

    UPROPERTY(BlueprintReadWrite)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(BlueprintReadWrite)
    bool bIsEnabled;

    FBlockchainTestCase()
    {
        TestName = TEXT("");
        Category = EBlockchainTestCategory::MiningSystem;
        Description = TEXT("");
        Result = EBlockchainTestResult::NotRun;
        ErrorMessage = TEXT("");
        ExecutionTime = 0.0f;
        bIsEnabled = true;
    }
};

USTRUCT(BlueprintType)
struct FBlockchainTestSuiteSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 TotalTests;

    UPROPERTY(BlueprintReadWrite)
    int32 PassedTests;

    UPROPERTY(BlueprintReadWrite)
    int32 FailedTests;

    UPROPERTY(BlueprintReadWrite)
    int32 SkippedTests;

    UPROPERTY(BlueprintReadWrite)
    int32 ErrorTests;

    UPROPERTY(BlueprintReadWrite)
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadWrite)
    TArray<FBlockchainTestCase> TestCases;

    UPROPERTY(BlueprintReadWrite)
    bool bAllTestsPassed;

    FBlockchainTestSuiteSummary()
    {
        TotalTests = 0;
        PassedTests = 0;
        FailedTests = 0;
        SkippedTests = 0;
        ErrorTests = 0;
        TotalExecutionTime = 0.0f;
        bAllTestsPassed = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteStarted, const FBlockchainTestSuiteSummary&, Summary);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestCaseCompleted, const FString&, TestName, EBlockchainTestResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FBlockchainTestSuiteSummary&, Summary);

/**
 * Comprehensive blockchain testing suite for MingGoRTS
 * Provides automated testing for all blockchain components
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (MingBlockchain))
class MINGBLOCKCHAIN_API UMingBlockchainTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingBlockchainTestSuite();

    // Test suite management
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void InitializeTestSuite();

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary RunTestCategory(EBlockchainTestCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    bool RunSingleTest(const FString& TestName);

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void StopTestSuite();

    // Test configuration
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void SetTestTimeout(float TimeoutSeconds);

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void EnableTestCategory(EBlockchainTestCategory Category, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void EnableTest(const FString& TestName, bool bEnabled);

    // Test results and reporting
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary GetLastTestSummary() const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    TArray<FBlockchainTestCase> GetTestResults() const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FString GenerateTestReport() const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    bool ExportTestResults(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void ClearTestResults();

    // Performance testing
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary RunPerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void SetPerformanceTestParameters(int32 Iterations, float DurationSeconds);

    // Security testing
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary RunSecurityTests();

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void EnableSecurityTestLevel(int32 Level);

    // Integration testing
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FBlockchainTestSuiteSummary RunIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    void SetIntegrationTestEnvironment(const FString& Environment);

    // Test utilities
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    bool IsTestSuiteRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    float GetTestProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Blockchain|Testing")
    FString GetCurrentTestName() const;

    // Events
    UPROPERTY(BlueprintAssignable)
    FOnTestSuiteStarted OnTestSuiteStarted;

    UPROPERTY(BlueprintAssignable)
    FOnTestCaseCompleted OnTestCaseCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTestSuiteCompleted OnTestSuiteCompleted;

protected:
    virtual void BeginPlay();
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

    // Test execution
    void ExecuteTest(FBlockchainTestCase& TestCase);
    bool ExecuteMiningTest(const FString& TestName);
    bool ExecuteWalletTest(const FString& TestName);
    bool ExecutePlayToEarnTest(const FString& TestName);
    bool ExecuteTransactionTest(const FString& TestName);
    bool ExecuteNFTTest(const FString& TestName);
    bool ExecuteSettingsTest(const FString& TestName);
    bool ExecuteIntegrationTest(const FString& TestName);
    bool ExecutePerformanceTest(const FString& TestName);
    bool ExecuteSecurityTest(const FString& TestName);
    bool ExecuteEndToEndTest(const FString& TestName);

    // Test setup and cleanup
    void SetupTestEnvironment();
    void CleanupTestEnvironment();
    void SetupMiningEnvironment();
    void SetupWalletEnvironment();
    void SetupPlayToEarnEnvironment();
    void SetupTransactionEnvironment();
    void SetupNFTEnvironment();
    void SetupSettingsEnvironment();

    // Test validation
    bool ValidateTestResults();
    bool ValidatePerformanceResults();
    bool ValidateSecurityResults();

    // Test reporting
    void UpdateTestSummary();
    void LogTestResults();
    void SaveTestResults();

    // Test helpers
    void LogTestMessage(const FString& Message, ELogVerbosity::Type Verbosity = ELogVerbosity::Log);
    void LogTestError(const FString& TestName, const FString& Error);
    void LogTestSuccess(const FString& TestName, float ExecutionTime);

private:
    // Test configuration
    UPROPERTY()
    float TestTimeout;

    UPROPERTY()
    int32 PerformanceTestIterations;

    UPROPERTY()
    float PerformanceTestDuration;

    UPROPERTY()
    int32 SecurityTestLevel;

    UPROPERTY()
    FString IntegrationTestEnvironment;

    // Test state
    UPROPERTY()
    bool bTestSuiteRunning;

    UPROPERTY()
    FString CurrentTestName;

    UPROPERTY()
    int32 CurrentTestIndex;

    UPROPERTY()
    float TestStartTime;

    // Test results
    UPROPERTY()
    FBlockchainTestSuiteSummary LastTestSummary;

    UPROPERTY()
    TArray<FBlockchainTestCase> AllTestCases;

    UPROPERTY()
    TMap<EBlockchainTestCategory, bool> CategoryEnabled;

    UPROPERTY()
    TMap<FString, bool> TestEnabled;

    // Test data
    UPROPERTY()
    TArray<FString> MiningTestNames;

    UPROPERTY()
    TArray<FString> WalletTestNames;

    UPROPERTY()
    TArray<FString> PlayToEarnTestNames;

    UPROPERTY()
    TArray<FString> TransactionTestNames;

    UPROPERTY()
    TArray<FString> NFTTestNames;

    UPROPERTY()
    TArray<FString> SettingsTestNames;

    UPROPERTY()
    TArray<FString> IntegrationTestNames;

    UPROPERTY()
    TArray<FString> PerformanceTestNames;

    UPROPERTY()
    TArray<FString> SecurityTestNames;

    UPROPERTY()
    TArray<FString> EndToEndTestNames;

    // Internal methods
    void InitializeTestCases();
    void InitializeTestCategoryArrays();
    FTestCase CreateTestCase(const FString& Name, EBlockchainTestCategory Category, const FString& Description);
    void MarkTestResult(const FString& TestName, EBlockchainTestResult Result, const FString& ErrorMessage = TEXT(""));
    float CalculateTestProgress() const;
    FString GetTestReportHeader() const;
    FString GetTestReportBody() const;
    FString GetTestReportFooter() const;
};
