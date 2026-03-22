#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * Integration Test Executor for MingGoRTS
 * Executes and manages comprehensive integration tests
 */
UCLASS(ClassGroup = (Testing), Blueprintable)
class MINGINTEGRATION_API UMingIntegrationTestExecutor : public UObject
{
    GENERATED_BODY()

public:
    UMingIntegrationTestExecutor();

    // Test Execution Control
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteAllTests();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteLoadOrderValidation();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteDependencyValidation();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecutePerformanceBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteCommunicationTests();

    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    void ExecuteHealthChecks();

    // Real-time Monitoring
    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void StartRealTimeMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void StopRealTimeMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Monitoring")
    void UpdateRealTimeMetrics();

    UFUNCTION(BlueprintPure, Category = "Monitoring")
    bool IsMonitoringActive() const;

    // Test Results
    UFUNCTION(BlueprintPure, Category = "Results")
    bool AreAllTestsPassed() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    float GetOverallSuccessRate() const;

    UFUNCTION(BlueprintCallable, Category = "Results")
    void GenerateTestReport();

    UFUNCTION(BlueprintCallable, Category = "Results")
    void ExportTestResults(const FString& FilePath);

protected:
    UPROPERTY()
    TObjectPtr<UMingIntegrationValidator> Validator;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeValidator();
    void SetupTestEnvironment();
    void CleanupTestEnvironment();

    // Test Execution Helpers
    void ExecuteTestWithTimeout(EMingValidationType ValidationType, float TimeoutSeconds);
    void MonitorTestProgress();
    void HandleTestCompletion(EMingValidationType ValidationType, bool bSuccess);

    // Real-time Monitoring
    void CollectRealTimeData();
    void UpdatePerformanceMetrics();
    void CheckSystemStability();
    void LogRealTimeStatus();

    // Results Processing
    void ProcessTestResults();
    void AnalyzeFailures();
    void GenerateRecommendations();
    void CreateDetailedReport();

    // Utility Functions
    void LogTestEvent(const FString& Event, const FString& Details);
    void BroadcastTestProgress(const FString& TestName, float Progress);
};

#include "MingIntegrationTestExecutor.generated.h"
