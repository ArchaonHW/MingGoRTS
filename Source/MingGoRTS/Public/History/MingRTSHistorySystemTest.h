// Copyright (c) 2026 MingGoRTS. All rights reserved.
// History System Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.h"
#include "MingRTSTimelineManager.h"
#include "MingRTSHistoricalNarrativeGenerator.h"
#include "MingRTSCausalityCalculator.h"
#include "MingRTSHistorySystemTest.generated.h"

/**
 * History System Test Suite - Comprehensive testing for the dynamic history system
 * 
 * This test suite validates all components of the real-time historical event
 * evolution mechanism, ensuring system reliability and performance.
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSHistorySystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSHistorySystemTest();

    // Test Execution
    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void RunBasicTests();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void RunPerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void RunIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void RunStressTests();

    // Test Results
    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool AreAllTestsPassed() const { return bAllTestsPassed; }

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    int32 GetTotalTestCount() const { return TotalTestCount; }

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    int32 GetPassedTestCount() const { return PassedTestCount; }

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    TArray<FString> GetFailedTestNames() const { return FailedTestNames; }

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    FString GetTestReport() const;

    // Individual Tests
    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestEventGeneration();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestTimelineManagement();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestCausalityCalculation();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestNarrativeGeneration();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestSystemIntegration();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestPerformanceUnderLoad();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestMemoryUsage();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    bool TestConcurrentOperations();

    // Utility Methods
    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void ResetTestResults();

    UFUNCTION(BlueprintCallable, Category = "History System Test")
    void LogTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage = FString());

protected:
    // Test Setup and Teardown
    void SetupTestEnvironment();
    void CleanupTestEnvironment();

    // Test Data Generation
    FHistoricalEventData GenerateTestEvent(const FString& EventName, EHistoricalEventType EventType);
    TArray<FHistoricalEventData> GenerateTestEventSet(int32 EventCount);
    FNarrativeGenerationRequest GenerateTestNarrativeRequest();

    // Performance Measurement
    double MeasureExecutionTime(TFunction<void()> TestFunction);
    void MeasureMemoryUsage(int32& UsedMemory, int32& PeakMemory);

    // Validation Methods
    bool ValidateEventData(const FHistoricalEventData& Event);
    bool ValidateTimelineData(const FTimelineBranch& Timeline);
    bool ValidateCausalityData(const FCausalityAnalysisResult& Causality);
    bool ValidateNarrativeData(const FNarrativeStructure& Narrative);

private:
    UPROPERTY()
    bool bAllTestsPassed;

    UPROPERTY()
    int32 TotalTestCount;

    UPROPERTY()
    int32 PassedTestCount;

    UPROPERTY()
    TArray<FString> FailedTestNames;

    UPROPERTY()
    TArray<FString> TestMessages;

    // Test components
    UPROPERTY()
    UMingRTSDynamicHistorySystem* HistorySystem;

    UPROPERTY()
    UMingRTSTimelineManager* TimelineManager;

    UPROPERTY()
    UMingRTSHistoricalNarrativeGenerator* NarrativeGenerator;

    UPROPERTY()
    UMingRTSCausalityCalculator* CausalityCalculator;

    // Performance metrics
    UPROPERTY()
    TArray<double> ExecutionTimes;

    UPROPERTY()
    TArray<int32> MemoryUsageData;

    // Helper methods
    void InitializeTestComponents();
    void CleanupTestComponents();
    FString GenerateTestReport() const;
    bool CompareEventSequences(const TArray<FString>& SequenceA, const TArray<FString>& SequenceB) const;
};
