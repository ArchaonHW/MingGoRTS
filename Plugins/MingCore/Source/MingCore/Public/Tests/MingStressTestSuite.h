#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStressTestSuite.generated.h"

UENUM(BlueprintType)
enum class EMingStressTestType : uint8
{
    Stability         UMETA(DisplayName = "Stability Test"),
    Performance       UMETA(DisplayName = "Performance Test"),
    Concurrent        UMETA(DisplayName = "Concurrent Test"),
    Resource          UMETA(DisplayName = "Resource Test"),
    Memory            UMETA(DisplayName = "Memory Test"),
    Network           UMETA(DisplayName = "Network Test"),
    All               UMETA(DisplayName = "All Tests")
};

UENUM(BlueprintType)
enum class EMingStressTestStatus : uint8
{
    Idle              UMETA(DisplayName = "Idle"),
    Running           UMETA(DisplayName = "Running"),
    Passed            UMETA(DisplayName = "Passed"),
    Failed            UMETA(DisplayName = "Failed"),
    Error             UMETA(DisplayName = "Error")
};

USTRUCT(BlueprintType)
struct FMingStressTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EMingStressTestType TestType;

    UPROPERTY(BlueprintReadWrite)
    EMingStressTestStatus Status;

    UPROPERTY(BlueprintReadWrite)
    float Duration;

    UPROPERTY(BlueprintReadWrite)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> Metrics;

    FMingStressTestResult()
    {
        TestType = EMingStressTestType::All;
        Status = EMingStressTestStatus::Idle;
        Duration = 0.0f;
        ErrorMessage = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStressTestStarted, EMingStressTestType, TestType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStressTestCompleted, EMingStressTestType, TestType, bool, bPassed};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStressTestFailed, EMingStressTestType, TestType, FString, ErrorMessage};

/**
 * Ming Stress Test Suite
 * Comprehensive stress testing system for MingGoRTS
 */
UCLASS(ClassGroup = (Tests), Blueprintable)
class MINGCORE_API UMingStressTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingStressTestSuite(const FObjectInitializer& ObjectInitializer};

    virtual void BeginDestroy() override;

    // ========== Initialization ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void InitializeStressTestSuite(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void ShutdownStressTestSuite(};

    UFUNCTION(BlueprintPure, Category = "Stress Test")
    bool IsInitialized() const { return bIsInitialized; }

    // ========== Stability Tests ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Stability")
    void Start24HourStabilityTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Stability")
    void StopStabilityTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Stability")
    void StartLongTermMemoryTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Stability")
    void StopLongTermMemoryTest(};

    UFUNCTION(BlueprintPure, Category = "Stress Test - Stability")
    bool IsStabilityTestPass() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Stability")
    float GetUptimeHours() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Stability")
    int32 GetCrashCount() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Stability")
    float GetAverageMemoryUsage() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Stability")
    bool HasMemoryLeak() const;

    // ========== Performance Tests ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Performance")
    void StartMassiveUnitTest(int32 UnitCount = 10000};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Performance")
    void StopMassiveUnitTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Performance")
    void StartLargeScaleBattleTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Performance")
    void StopLargeScaleBattleTest(};

    UFUNCTION(BlueprintPure, Category = "Stress Test - Performance")
    bool IsMassiveTestPass() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Performance")
    float GetAverageFPS() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Performance")
    float GetMinFPS() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Performance")
    int32 GetActiveUnitCount() const;

    // ========== Concurrent Tests ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Concurrent")
    void StartConcurrentUserTest(int32 UserCount = 100};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Concurrent")
    void StopConcurrentUserTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Concurrent")
    void StartNetworkStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Concurrent")
    void StopNetworkStressTest(};

    UFUNCTION(BlueprintPure, Category = "Stress Test - Concurrent")
    bool IsConcurrentTestPass() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Concurrent")
    float GetAverageLatency() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Concurrent")
    int32 GetActiveUserCount() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Concurrent")
    float GetNetworkThroughput() const;

    // ========== Resource Tests ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StartResourceStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StopResourceStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StartMemoryStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StopMemoryStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StartCPUStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test - Resource")
    void StopCPUStressTest(};

    UFUNCTION(BlueprintPure, Category = "Stress Test - Resource")
    bool IsResourceTestPass() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Resource")
    float GetMemoryUsagePercentage() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Resource")
    float GetCPUUsagePercentage() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test - Resource")
    int32 GetResourceAllocations() const;

    // ========== Test Management ==========

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunAllStressTests(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunQuickStressTest(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void RunFullStressTest(};

    UFUNCTION(BlueprintPure, Category = "Stress Test")
    bool AreAllStressTestsPassed() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test")
    int32 GetPassedTestCount() const;

    UFUNCTION(BlueprintPure, Category = "Stress Test")
    int32 GetFailedTestCount() const;

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    FString GenerateStressTestReport(};

    UFUNCTION(BlueprintCallable, Category = "Stress Test")
    void ResetTestResults(};

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStressTestStarted OnStressTestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStressTestCompleted OnStressTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStressTestFailed OnStressTestFailed;

protected:
    void CalculateTestStatistics(};

    // Internal simulation
    void SimulateHighLoad(};
    void GenerateTestLoad(};
    void MonitorSystemResources(};
    void ValidatePerformanceThresholds(};

private:
    bool bIsInitialized;

    // Test status tracking
    TMap<EMingStressTestType, EMingStressTestStatus> TestStatuses;
    TMap<EMingStressTestType, FMingStressTestResult> TestResults;

    // Stability test data
    float StabilityTestStartTime;
    int32 CrashCounter;
    bool bHasMemoryLeakDetected;

    // Performance test data
    float AverageFPS;
    float MinimumFPS;
    int32 CurrentActiveUnitCount;

    // Concurrent test data
    float AverageLatency;
    int32 CurrentActiveUserCount;
    float NetworkThroughputMBps;

    // Resource test data
    float MemoryUsagePercent;
    float CPUUsagePercent;
    int32 ResourceAllocationCount;
};
