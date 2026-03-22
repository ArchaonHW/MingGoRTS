#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStressTestSuite.generated.h"

/**
 * ??ծM
 * ըt摧毀???X?íw?X?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingStressTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingStressTestSuite();

    // ========== ???X?íwʴ??==========
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Stability")
    void Start24HourStabilityTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Stability")
    void StopStabilityTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Stability")
    void StartLongTermMemoryTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Stability")
    void StopLongTermMemoryTest();
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Stability")
    bool IsStabilityTestPass() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Stability")
    float GetUptimeHours() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Stability")
    int32 GetCrashCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Stability")
    float GetAverageMemoryUsage() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Stability")
    bool HasMemoryLeak() const;

    // ========== j??ҩʯ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Performance")
    void StartMassiveUnitTest(int32 UnitCount = 10000);
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Performance")
    void StopMassiveUnitTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Performance")
    void StartLargeScaleBattleTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Performance")
    void StopLargeScaleBattleTest();
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Performance")
    bool IsMassiveTestPass() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Performance")
    float GetAverageFPS() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Performance")
    float GetMinFPS() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Performance")
    int32 GetActiveUnitCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Performance")
    float GetSimulationTime() const;

    // ========== õo?? ==========
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Concurrent")
    void StartConcurrentUserTest(int32 UserCount = 100);
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Concurrent")
    void StopConcurrentUserTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Concurrent")
    void StartNetworkStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Concurrent")
    void StopNetworkStressTest();
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Concurrent")
    bool IsConcurrentTestPass() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Concurrent")
    float GetAverageLatency() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Concurrent")
    int32 GetActiveUserCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Concurrent")
    float GetNetworkThroughput() const;

    // ========== 摧毀 ==========
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StartResourceStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StopResourceStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StartMemoryStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StopMemoryStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StartCPUStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test | Resource")
    void StopCPUStressTest();
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Resource")
    bool IsResourceTestPass() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Resource")
    float GetMemoryUsagePercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Resource")
    float GetCPUUsagePercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test | Resource")
    int32 GetResourceAllocations() const;

    // ========== 摧毀 ==========
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunAllStressTests();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunQuickStressTest();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunFullStressTest();
    
    UFUNCTION(BlueprintPure, Category = "Stress Test")
    bool AreAllStressTestsPassed() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test")
    int32 GetPassedTestCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Stress Test")
    int32 GetFailedTestCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    FString GenerateStressTestReport();
    
    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void ExportStressTestData(const FString& FilePath);

protected:
    virtual void Tick(float DeltaTime) override;

private:
    
    bool bStabilityTestRunning;
    
    UPROPERTY()
    bool bMassiveTestRunning;
    
    UPROPERTY()
    bool bConcurrentTestRunning;
    
    UPROPERTY()
    bool bResourceTestRunning;
    
    
    float TestStartTime;
    
    UPROPERTY()
    float TotalUptime;
    
    UPROPERTY()
    int32 CrashCount;
    
    UPROPERTY()
    float AverageMemoryUsage;
    
    UPROPERTY()
    bool bMemoryLeakDetected;
    
    // 摧毀??
    UPROPERTY()
    float AverageFPS;
    
    UPROPERTY()
    float MinFPS;
    
    UPROPERTY()
    float MaxFPS;
    
    UPROPERTY()
    int32 FrameCount;
    
    UPROPERTY()
    float TotalFrameTime;
    
    // õo摧毀
    UPROPERTY()
    float AverageLatency;
    
    UPROPERTY()
    int32 ActiveUserCount;
    
    UPROPERTY()
    float NetworkThroughput;
    
    // 摧毀??
    UPROPERTY()
    float MemoryUsagePercentage;
    
    UPROPERTY()
    float CPUUsagePercentage;
    
    UPROPERTY()
    int32 ResourceAllocations;
    
    // յ??
    UPROPERTY()
    int32 PassedTests;
    
    UPROPERTY()
    int32 FailedTests;
    
    UPROPERTY()
    TArray<FString> TestResults;

    // 摧毀
    void UpdateStabilityMetrics(float DeltaTime);
    void UpdatePerformanceMetrics(float DeltaTime);
    void UpdateConcurrentMetrics(float DeltaTime);
    void UpdateResourceMetrics(float DeltaTime);
    
    void MonitorSystemHealth();
    void CheckMemoryLeaks();
    void ValidateSystemIntegrity();
    void LogStressMetrics();
    
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};
    void ResetTestResults();
    void CalculateTestStatistics();
    
    // ??ջU
    void SimulateHighLoad();
    void GenerateTestLoad();
    void MonitorSystemResources();
    void ValidatePerformanceThresholds();
};

