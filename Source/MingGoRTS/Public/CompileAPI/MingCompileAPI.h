#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "MingCompileAPI.generated.h"

UENUM(BlueprintType)
enum class EMingCompileMode : uint8
{
    Debug,
    Development,
    Shipping,
    Test
};

UENUM(BlueprintType)
enum class EMingCompileTarget : uint8
{
    Editor,
    Game,
    Server
};

UENUM(BlueprintType)
enum class EMingCompileStatus : uint8
{
    Idle,
    Compiling,
    Success,
    Failed,
    Cancelled
};

USTRUCT(BlueprintType)
struct FMingCompileConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    EMingCompileMode CompileMode;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    EMingCompileTarget Target;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    FString ProjectPath;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    FString OutputPath;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    bool bCleanBuild;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    int32 ParallelJobs;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Configuration")
    TArray<FString> AdditionalArguments;

    FMingCompileConfiguration()
    {
        CompileMode = EMingCompileMode::Development;
        Target = EMingCompileTarget::Game;
        ProjectPath = TEXT("");
        OutputPath = TEXT("");
        bCleanBuild = false;
        ParallelJobs = 4;
        AdditionalArguments.Empty();
    }
};

USTRUCT(BlueprintType)
struct FMingCompileResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    EMingCompileStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    float CompileTime;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    int32 WarningCount;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    int32 ErrorCount;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    FString OutputLog;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadWrite, Category = "Compile Result")
    FDateTime EndTime;

    FMingCompileResult()
    {
        Status = EMingCompileStatus::Idle;
        ErrorMessage = TEXT("");
        CompileTime = 0.0f;
        WarningCount = 0;
        ErrorCount = 0;
        OutputLog = TEXT("");
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FMingTestConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    bool bRunUnitTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    bool bRunIntegrationTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    bool bRunPerformanceTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    bool bRunStressTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    int32 TestTimeout;

    UPROPERTY(BlueprintReadWrite, Category = "Test Configuration")
    FString TestOutputPath;

    FMingTestConfiguration()
    {
        bRunUnitTests = true;
        bRunIntegrationTests = true;
        bRunPerformanceTests = false;
        bRunStressTests = false;
        TestTimeout = 300; // 5 minutes
        TestOutputPath = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    bool bSuccess;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    int32 TotalTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    int32 PassedTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    int32 FailedTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    int32 SkippedTests;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    float TestTime;

    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    FString TestReport;

    FMingTestResult()
    {
        bSuccess = false;
        TotalTests = 0;
        PassedTests = 0;
        FailedTests = 0;
        SkippedTests = 0;
        TestTime = 0.0f;
        TestReport = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompileStarted, const FMingCompileConfiguration&, Configuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompileProgress, float, Progress, const FString&, CurrentTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompileCompleted, const FMingCompileResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestStarted, const FMingTestConfiguration&, Configuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestProgress, float, Progress, const FString&, CurrentTest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, const FMingTestResult&, Result);

/**
 * MingGoRTS Compile API System
 * 
 * This system provides comprehensive compilation management and testing capabilities,
 * replacing the original PowerShell-based compilation scripts.
 * 
 * Features:
 * - Multiple compilation modes and targets
 * - Parallel compilation support
 * - Comprehensive testing framework
 * - Progress tracking and error handling
 * - C++ native performance
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Compile API")
class MINGGORTS_API UMingCompileAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingCompileAPI();

    // Initialize the compile API system
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void InitializeCompileAPI();

    // Compile project with configuration
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    FMingCompileResult CompileProject(const FMingCompileConfiguration& Configuration);

    // Compile project asynchronously
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void CompileProjectAsync(const FMingCompileConfiguration& Configuration);

    // Cancel current compilation
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void CancelCompilation();

    // Run tests with configuration
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    FMingTestResult RunTests(const FMingTestConfiguration& TestConfig);

    // Run tests asynchronously
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void RunTestsAsync(const FMingTestConfiguration& TestConfig);

    // Get current compile status
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    EMingCompileStatus GetCompileStatus();

    // Get compile configuration
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    FMingCompileConfiguration GetDefaultConfiguration();

    // Set compile configuration
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void SetDefaultConfiguration(const FMingCompileConfiguration& Configuration);

    // Generate compile report
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    bool GenerateCompileReport(const FMingCompileResult& Result, const FString& ReportPath);

    // Clean build artifacts
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    void CleanBuildArtifacts();

    // Validate project structure
    UFUNCTION(BlueprintCallable, Category = "Compile API")
    bool ValidateProjectStructure(const FString& ProjectPath);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnCompileStarted OnCompileStarted;

    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnCompileProgress OnCompileProgress;

    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnCompileCompleted OnCompileCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnTestStarted OnTestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnTestProgress OnTestProgress;

    UPROPERTY(BlueprintAssignable, Category = "Compile API")
    FOnTestCompleted OnTestCompleted;

protected:
    // Execute compilation command
    bool ExecuteCompilation(const FMingCompileConfiguration& Configuration, FMingCompileResult& OutResult);

    // Execute test command
    bool ExecuteTests(const FMingTestConfiguration& TestConfig, FMingTestResult& OutResult);

    // Parse compile log
    void ParseCompileLog(const FString& LogContent, FMingCompileResult& Result);

    // Parse test results
    void ParseTestResults(const FString& TestOutput, FMingTestResult& Result);

    // Write log entry
    void WriteLog(const FString& Message, const FString& Level = TEXT("INFO"));

    // Get Unreal Engine build command
    FString GetBuildCommand(const FMingCompileConfiguration& Configuration);

    // Get test command
    FString GetTestCommand(const FMingTestConfiguration& TestConfig);

private:
    // Current compile status
    UPROPERTY()
    EMingCompileStatus CurrentStatus;

    // Default configuration
    UPROPERTY()
    FMingCompileConfiguration DefaultConfiguration;

    // Current compilation process
    UPROPERTY()
    class FProcHandle* CurrentProcess;

    // Project paths
    FString ProjectRoot;
    FString BuildPath;
    FString LogPath;

    // System state
    bool bIsInitialized;
};
