// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Test Coverage Analyzer - B3-2
// Provides comprehensive test coverage analysis and reporting

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingTestCoverageAnalyzer.generated.h"

UENUM(BlueprintType)
UENUM(BlueprintType)\nenum class ECoverageType : uint8\n{
    Line = 0, UMETA(DisplayName = "Line Coverage"),
    Branch, UMETA(DisplayName = "Branch Coverage"),
    Function, UMETA(DisplayName = "Function Coverage"),
    Statement, UMETA(DisplayName = "Statement Coverage"),
    Condition, UMETA(DisplayName = "Condition Coverage"),
    Path, UMETA(DisplayName = "Path Coverage"),
    Method, UMETA(DisplayName = "Method Coverage"),
    Class, UMETA(DisplayName = "Class Coverage"),
    Module, UMETA(DisplayName = "Module Coverage"),
    Overall UMETA(DisplayName = "Overall Coverage")
};

UENUM(BlueprintType)
enum class ETestType: uint8 {
    Unit = 0, UMETA(DisplayName = "Unit Test"),
    Integration, UMETA(DisplayName = "Integration Test"),
    System, UMETA(DisplayName = "System Test"),
    Performance, UMETA(DisplayName = "Performance Test"),
    Regression, UMETA(DisplayName = "Regression Test"),
    Smoke, UMETA(DisplayName = "Smoke Test"),
    Stress, UMETA(DisplayName = "Stress Test"),
    Load, UMETA(DisplayName = "Load Test"),
    Security, UMETA(DisplayName = "Security Test"),
    Compatibility, UMETA(DisplayName = "Compatibility Test")
};

UENUM(BlueprintType)
enum class ETestStatus: uint8 {
    NotRun = 0, UMETA(DisplayName = "Not Run"),
    Running, UMETA(DisplayName = "Running"),
    Passed, UMETA(DisplayName = "Passed"),
    Failed, UMETA(DisplayName = "Failed"),
    Skipped, UMETA(DisplayName = "Skipped"),
    Error, UMETA(DisplayName = "Error"),
    Timeout, UMETA(DisplayName = "Timeout"),
    Inconclusive UMETA(DisplayName = "Inconclusive")
};

USTRUCT(BlueprintType)
struct FCoverageData
{
    GENERATED_BODY()

    UPROPERTY()
    FString FilePath;

    UPROPERTY()
    ECoverageType Type;

    UPROPERTY()
    int32 TotalLines;

    UPROPERTY()
    int32 CoveredLines;

    UPROPERTY()
    int32 PartialLines;

    UPROPERTY()
    int32 UncoveredLines;

    UPROPERTY()
    float CoveragePercent;

    UPROPERTY()
    TArray<int32> CoveredLineNumbers;

    UPROPERTY()
    TArray<int32> UncoveredLineNumbers;

    UPROPERTY()
    TMap<FString, float> MethodCoverage;

    FCoverageData()
        : Type(ECoverageType::Line)
        , TotalLines(0)
        , CoveredLines(0)
        , PartialLines(0)
        , UncoveredLines(0)
        , CoveragePercent(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FTestResult
{
    GENERATED_BODY()

    UPROPERTY()
    FName TestID;

    UPROPERTY()
    FString TestName;

    UPROPERTY()
    ETestType Type;

    UPROPERTY()
    ETestStatus Status;

    UPROPERTY()
    FString FilePath;

    UPROPERTY()
    FString TestClass;

    UPROPERTY()
    float Duration;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    FString StackTrace;

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    int32 Assertions;

    UPROPERTY()
    int32 FailedAssertions;

    UPROPERTY()
    TArray<FString> Categories;

    FTestResult()
        : TestID(NAME_None)
        , Type(ETestType::Unit)
        , Status(ETestStatus::NotRun)
        , Duration(0.0f)
        , Timestamp(0)
        , Assertions(0)
        , FailedAssertions(0)
    {}
};

USTRUCT(BlueprintType)
struct FCoverageTarget
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coverage")
    ECoverageType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coverage")
    float TargetPercent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coverage")
    float MinimumPercent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coverage")
    TArray<FString> IncludedPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coverage")
    TArray<FString> ExcludedPaths;

    FCoverageTarget()
        : Type(ECoverageType::Line)
        , TargetPercent(85.0f)
        , MinimumPercent(70.0f)
    {}
};

USTRUCT(BlueprintType)
struct FCoverageReport
{
    GENERATED_BODY()

    UPROPERTY()
    FString ReportID;

    UPROPERTY()
    uint32 GeneratedTime;

    UPROPERTY()
    TArray<FCoverageData> FileCoverage;

    UPROPERTY()
    TMap<ECoverageType, float> OverallCoverage;

    UPROPERTY()
    int32 TotalFiles;

    UPROPERTY()
    int32 FilesMeetingTarget;

    UPROPERTY()
    int32 FilesBelowMinimum;

    UPROPERTY()
    TArray<FString> HighRiskFiles;

    UPROPERTY()
    TArray<FString> Recommendations;

    FCoverageReport()
        : GeneratedTime(0)
        , TotalFiles(0)
        , FilesMeetingTarget(0)
        , FilesBelowMinimum(0)
    {}
};

USTRUCT(BlueprintType)
struct FTestSuiteConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    FString SuiteName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    TArray<ETestType> IncludedTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    TArray<FString> TestPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    TArray<FString> ExcludedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    float TimeoutSeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    bool bParallelExecution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    int32 MaxParallelThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    bool bGenerateCoverage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite")
    bool bFailFast;

    FTestSuiteConfig()
        : TimeoutSeconds(30.0f)
        , bParallelExecution(true)
        , MaxParallelThreads(4)
        , bGenerateCoverage(true)
        , bFailFast(false)
    {}
};






/**
 * Test Coverage Analyzer
 * Provides comprehensive test coverage analysis and reporting
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingTestCoverageAnalyzer : public UObject
{
    GENERATED_BODY()

public:
    UMingTestCoverageAnalyzer();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void InitializeAnalyzer();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void ShutdownAnalyzer();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void SetCoverageTarget(const FCoverageTarget& Target);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    FCoverageData AnalyzeFileCoverage(const FString& FilePath, ECoverageType Type);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FCoverageData> AnalyzeProjectCoverage(const FString& ProjectPath);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    float CalculateCoveragePercent(const FCoverageData& Data) const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    bool RunTests(const FTestSuiteConfig& Config);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    bool RunTest(const FString& TestName, ETestType Type);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FTestResult> GetTestResults() const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FTestResult> GetFailedTests() const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FTestResult> GetTestsByType(ETestType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    FCoverageReport GenerateCoverageReport();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void ExportCoverageReport(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void ExportTestResults(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FString> IdentifyUntestedCode();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FString> IdentifyHighRiskAreas();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    TArray<FString> SuggestTestsForFile(const FString& FilePath);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    bool IsCoverageTargetMet(ECoverageType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    float GetCurrentCoverage(ECoverageType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void StartContinuousMonitoring();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void StopContinuousMonitoring();
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void SetCoverageThreshold(ECoverageType Type, float Target, float Minimum);
    UFUNCTION(BlueprintCallable, Category = "Test Coverage")
    void GenerateCoverageBadge();
    UFUNCTION(BlueprintPure, Category = "Test Coverage")
    int32 GetTotalTestsRun() const { return TestResults.Num(); }

    UFUNCTION(BlueprintPure, Category = "Test Coverage")
    int32 GetPassedTestsCount() const;

    UFUNCTION(BlueprintPure, Category = "Test Coverage")
    int32 GetFailedTestsCount() const;

    UPROPERTY(BlueprintAssignable, Category = "Test Coverage Events")
    FOnCoverageDataUpdated OnCoverageDataUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Test Coverage Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Test Coverage Events")
    FOnCoverageTargetMet OnCoverageTargetMet;

    UPROPERTY(BlueprintAssignable, Category = "Test Coverage Events")
    FOnCoverageBelowTarget OnCoverageBelowTarget;

    UPROPERTY(BlueprintAssignable, Category = "Test Coverage Events")
    FOnReportGenerated OnReportGenerated;

protected:
    UPROPERTY()
    TMap<ECoverageType, FCoverageTarget> CoverageTargets;

    UPROPERTY()
    TArray<FCoverageData> CoverageHistory;

    UPROPERTY()
    TArray<FTestResult> TestResults;

    UPROPERTY()
    FTimerHandle MonitoringTimer;

    UPROPERTY()
    bool bIsMonitoring;

    void StartMonitoring();
    void StopMonitoring();
    void PerformMonitoringCheck();
    FCoverageData ParseCoverageData(const FString& RawData, ECoverageType Type);
    void UpdateCoverageData(const FCoverageData& NewData);
    void CheckCoverageTargets();
    bool ExecuteTest(const FString& TestName, ETestType Type);
    TArray<FString> DiscoverTests(const FTestSuiteConfig& Config);
    FString GenerateReportID();
    void StoreTestResult(const FTestResult& Result);
    void LogCoverageActivity(const FString& Activity);
    float CalculateWeightedCoverage(const TArray<FCoverageData>& Data);
    TArray<FString> FindCriticalUncoveredLines(const FCoverageData& Data);
    void GenerateRecommendations(FCoverageReport& Report);
    static UMingTestCoverageAnalyzer* Get(UObject* WorldContextObject);
);
