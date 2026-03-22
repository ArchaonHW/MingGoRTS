// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統整合測試套件 - 頭文件

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/Function.h"
#include "MingRTSIntegrationTestSuite.generated.h"

UENUM(BlueprintType)
enum class EIntegrationTestType : uint8
{
    SystemInitialization      UMETA(DisplayName = "System Initialization"),
    CrossSystemDataFlow       UMETA(DisplayName = "Cross-System Data Flow"),
    EventPropagation          UMETA(DisplayName = "Event Propagation"),
    PerformanceStress         UMETA(DisplayName = "Performance Stress"),
    MemoryLeakDetection       UMETA(DisplayName = "Memory Leak Detection"),
    ConcurrentAccess          UMETA(DisplayName = "Concurrent Access"),
    ErrorRecovery             UMETA(DisplayName = "Error Recovery"),
    SaveLoadIntegration       UMETA(DisplayName = "Save/Load Integration"),
    NetworkSynchronization    UMETA(DisplayName = "Network Synchronization"),
    UISystemIntegration       UMETA(DisplayName = "UI System Integration"),
    AudioSystemIntegration    UMETA(DisplayName = "Audio System Integration"),
    AIBehaviorIntegration     UMETA(DisplayName = "AI Behavior Integration"),
    LocalizationIntegration   UMETA(DisplayName = "Localization Integration"),
    RegressionTest          UMETA(DisplayName = "Regression Test"),
    EndToEndScenario          UMETA(DisplayName = "End-to-End Scenario")
};

UENUM(BlueprintType)
enum class EIntegrationTestStatus : uint8
{
    Passed   UMETA(DisplayName = "Passed"),
    Failed   UMETA(DisplayName = "Failed"),
    Warning  UMETA(DisplayName = "Warning"),
    Skipped  UMETA(DisplayName = "Skipped"),
    Running  UMETA(DisplayName = "Running")
};

USTRUCT(BlueprintType)
struct FIntegrationTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    EIntegrationTestType TestType;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FString TestName;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FText Description;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    EIntegrationTestStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TArray<FString> ErrorMessages;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TArray<FString> WarningMessages;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    float ExecutionTimeSeconds;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TMap<FString, float> PerformanceData;

    FIntegrationTestResult()
        : TestType(EIntegrationTestType::SystemInitialization)
        , Status(EIntegrationTestStatus::Skipped)
        , ExecutionTimeSeconds(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FIntegrationTestSuiteConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    FString SuiteName;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    TArray<EIntegrationTestType> EnabledTests;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    bool bStopOnFirstFailure;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    bool bGenerateDetailedReport;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    bool bSaveResultsToFile;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    FString OutputPath;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test Suite")
    TArray<FString> SystemPairsToTest;

    FIntegrationTestSuiteConfig()
        : bStopOnFirstFailure(false)
        , bGenerateDetailedReport(true)
        , bSaveResultsToFile(true)
    {}
};

USTRUCT(BlueprintType)
struct FMemorySnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    FDateTime SnapshotTime;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int64 TotalPhysicalMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int64 AvailablePhysicalMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int64 TotalVirtualMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int64 AvailableVirtualMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory")
    int64 ProcessMemoryUsage;

    FMemorySnapshot()
        : TotalPhysicalMemory(0)
        , AvailablePhysicalMemory(0)
        , TotalVirtualMemory(0)
        , AvailableVirtualMemory(0)
        , ProcessMemoryUsage(0)
    {}
};

USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float AverageFPS;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MinFPS;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MaxFPS;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float AverageFrameTime;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MemoryUsageMB;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float CPUUsagePercent;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 ObjectCount;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float TextureMemoryMB;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MeshMemoryMB;

    FPerformanceMetrics()
        : AverageFPS(0.0f)
        , MinFPS(0.0f)
        , MaxFPS(0.0f)
        , AverageFrameTime(0.0f)
        , MemoryUsageMB(0.0f)
        , CPUUsagePercent(0.0f)
        , ObjectCount(0)
        , TextureMemoryMB(0.0f)
        , MeshMemoryMB(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FStressTestConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Stress Test")
    int32 ConcurrentUnitCount;

    UPROPERTY(BlueprintReadWrite, Category = "Stress Test")
    int32 SimulatedPlayerCount;

    UPROPERTY(BlueprintReadWrite, Category = "Stress Test")
    float TestDurationSeconds;

    UPROPERTY(BlueprintReadWrite, Category = "Stress Test")
    float TargetFPS;

    UPROPERTY(BlueprintReadWrite, Category = "Stress Test")
    float MaxMemoryUsageMB;

    FStressTestConfig()
        : ConcurrentUnitCount(100)
        , SimulatedPlayerCount(4)
        , TestDurationSeconds(60.0f)
        , TargetFPS(30.0f)
        , MaxMemoryUsageMB(2048.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSystemPairTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "System Pair")
    FString SystemA;

    UPROPERTY(BlueprintReadOnly, Category = "System Pair")
    FString SystemB;

    UPROPERTY(BlueprintReadOnly, Category = "System Pair")
    bool bCommunicationWorking;

    UPROPERTY(BlueprintReadOnly, Category = "System Pair")
    bool bDataTransferWorking;

    UPROPERTY(BlueprintReadOnly, Category = "System Pair")
    float DataTransferLatency;

    FSystemPairTestResult()
        : bCommunicationWorking(false)
        , bDataTransferWorking(false)
        , DataTransferLatency(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FIntegrationTestSuiteResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FString SuiteName;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    double TotalExecutionTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int32 TotalTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int32 PassedTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int32 FailedTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int32 WarningTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int32 SkippedTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    TArray<FIntegrationTestResult> TestResults;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    TArray<FSystemPairTestResult> SystemPairResults;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FString ReportFilePath;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FMemorySnapshot StartMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    FMemorySnapshot EndMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Test Suite Result")
    int64 MemoryDelta;

    FIntegrationTestSuiteResult()
        : TotalExecutionTime(0.0)
        , TotalTests(0)
        , PassedTests(0)
        , FailedTests(0)
        , WarningTests(0)
        , SkippedTests(0)
        , SuccessRate(0.0f)
        , MemoryDelta(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteStarted, const FString&, SuiteName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FIntegrationTestSuiteResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSingleTestCompleted, EIntegrationTestType, TestType, const FIntegrationTestResult&, Result);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestSuite();

    UFUNCTION(BlueprintCallable, Category = "Integration Test Suite")
    void InitializeTestSuite();

    UFUNCTION(BlueprintCallable, Category = "Integration Test Suite")
    FIntegrationTestSuiteResult RunFullTestSuite(const FIntegrationTestSuiteConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Integration Test Suite")
    FIntegrationTestResult RunSingleTest(EIntegrationTestType TestType);

    UFUNCTION(BlueprintCallable, Category = "Integration Test Suite")
    void CleanupTestResources();

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Suite Events")
    FOnTestSuiteStarted OnTestSuiteStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Suite Events")
    FOnTestSuiteCompleted OnTestSuiteCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Suite Events")
    FOnSingleTestCompleted OnSingleTestCompleted;

protected:
    UPROPERTY()
    bool bIsRunningTests;

    UPROPERTY()
    FIntegrationTestSuiteConfig CurrentConfig;

    UPROPERTY()
    TArray<FIntegrationTestSuiteResult> CachedResults;

    // Individual test implementations
    FIntegrationTestResult TestSystemInitialization();
    FIntegrationTestResult TestCrossSystemDataFlow();
    FIntegrationTestResult TestEventPropagation();
    FIntegrationTestResult RunPerformanceStressTest(const FStressTestConfig& Config);
    FIntegrationTestResult RunMemoryLeakDetection();
    FIntegrationTestResult TestConcurrentAccess();
    FIntegrationTestResult TestErrorRecovery();
    FIntegrationTestResult TestSaveLoadIntegration();
    FIntegrationTestResult TestNetworkSynchronization();
    FIntegrationTestResult TestUISystemIntegration();
    FIntegrationTestResult TestAudioSystemIntegration();
    FIntegrationTestResult TestAIBehaviorIntegration();
    FIntegrationTestResult TestLocalizationIntegration();
    FIntegrationTestResult RunRegressionTest();
    FIntegrationTestResult RunEndToEndScenarioTest();

    // Helper functions
    FSystemPairTestResult TestSystemPairCommunication(const FString& SystemA, const FString& SystemB);
    FMemorySnapshot CaptureMemorySnapshot();
    bool CompareMemorySnapshots(const FMemorySnapshot& Before, const FMemorySnapshot& After, int64& OutDelta);
    FPerformanceMetrics CollectPerformanceMetrics(float Duration);
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics, const FStressTestConfig& Config);
    void SimulateHighLoad(const FStressTestConfig& Config);
    float CalculateSuccessRate(const FIntegrationTestSuiteResult& Result) const;
    FString GenerateTestReport(const FIntegrationTestSuiteResult& Result, bool bAsHTML);
    FString GenerateHTMLReport(const FIntegrationTestSuiteResult& Result);
    FString GenerateTextReport(const FIntegrationTestSuiteResult& Result);
    void ExportResultsToJSON(const FIntegrationTestSuiteResult& Result, const FString& FilePath);
    void LogTestStart(const FString& TestName);
    void LogTestComplete(const FString& TestName, EIntegrationTestStatus Status, float Duration);
    FStressTestConfig GetDefaultStressConfig() const;
};
