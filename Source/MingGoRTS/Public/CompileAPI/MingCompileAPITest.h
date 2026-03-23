#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCompileAPI.h"
#include "MingCompileAPITest.generated.h"

/**
 * MingGoRTS Compile API Test Suite
 * 
 * Comprehensive test suite for the Compile API system.
 * Tests compilation, testing, and reporting functionality.
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Compile API Test")
class MINGGORTS_API UMingCompileAPITest : public UObject
{
    GENERATED_BODY()

public:
    UMingCompileAPITest();

    // Run all tests
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool RunAllTests();

    // Test compilation API initialization
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestCompileAPIInitialization();

    // Test default configuration
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestDefaultConfiguration();

    // Test project compilation
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestProjectCompilation();

    // Test asynchronous compilation
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestAsyncCompilation();

    // Test test execution
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestTestExecution();

    // Test report generation
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestReportGeneration();

    // Test project validation
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestProjectValidation();

    // Test build cleanup
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestBuildCleanup();

    // Test error handling
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestErrorHandling();

    // Test cancellation
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool TestCancellation();

    // Generate test report
    UFUNCTION(BlueprintCallable, Category = "Compile API Test")
    bool GenerateTestReport(const FString& ReportPath);

protected:
    // Log test result
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // Create test configuration
    FMingCompileConfiguration CreateTestConfiguration();

    // Create test configuration for tests
    FMingTestConfiguration CreateTestTestConfiguration();

private:
    // Test results
    UPROPERTY()
    TMap<FString, bool> TestResults;

    // Total tests
    int32 TotalTests;

    // Passed tests
    int32 PassedTests;

    // Failed tests
    int32 FailedTests;
};
