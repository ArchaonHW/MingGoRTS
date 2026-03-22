#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceTest.generated.h"

USTRUCT(BlueprintType)
struct FMingPerformanceTestMetrics
{
    GENERATED_BODY()

    // FPS Metrics
    UPROPERTY(BlueprintReadOnly)
    float AverageFPS;

    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    UPROPERTY(BlueprintReadOnly)
    float MaxFPS;

    // Memory Metrics
    UPROPERTY(BlueprintReadOnly)
    float UsedMemoryMB;

    UPROPERTY(BlueprintReadOnly)
    float PeakMemoryMB;

    // Unit Metrics
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveUnitCount;

    UPROPERTY(BlueprintReadOnly)
    float AverageUnitUpdateTime;

    // Event System Metrics
    UPROPERTY(BlueprintReadOnly)
    int32 EventsPerSecond;

    UPROPERTY(BlueprintReadOnly)
    float AverageEventLatency;

    // Navigation Metrics
    UPROPERTY(BlueprintReadOnly)
    float PathfindingTime;

    UPROPERTY(BlueprintReadOnly)
    int32 ActivePathRequests;

    // AI Metrics
    UPROPERTY(BlueprintReadOnly)
    float AverageAIUpdateTime;

    UPROPERTY(BlueprintReadOnly)
    int32 AIQueriesPerSecond;

    FMingPerformanceMetrics()
        : AverageFPS(0.0f)
        , MinFPS(MAX_FLT)
        , MaxFPS(0.0f)
        , UsedMemoryMB(0.0f)
        , PeakMemoryMB(0.0f)
        , ActiveUnitCount(0)
        , AverageUnitUpdateTime(0.0f)
        , EventsPerSecond(0)
        , AverageEventLatency(0.0f)
        , PathfindingTime(0.0f)
        , ActivePathRequests(0)
        , AverageAIUpdateTime(0.0f)
        , AIQueriesPerSecond(0)
    {}
};

UENUM(BlueprintType)
enum class EMingPerformanceTestType : uint8
{
    UnitSpawning UMETA(DisplayName = "Unit Spawning"),
    MovementStress UMETA(DisplayName = "Movement Stress"),
    CombatStress UMETA(DisplayName = "Combat Stress"),
    SelectionStress UMETA(DisplayName = "Selection Stress"),
    FormationStress UMETA(DisplayName = "Formation Stress"),
    FullSimulation UMETA(DisplayName = "Full Simulation")
};

/**
 * Performance testing system for MingGoRTS
 * Validates 60fps target with 1000+ units
 */
UCLASS(ClassGroup = (Testing), Blueprintable)
class MINGTACTICAL_API UMingPerformanceTest : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceTest();

    // Test Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Config")
    int32 TargetUnitCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Config")
    float TargetFPS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Config")
    float TestDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Config")
    EMingPerformanceTestType TestType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Config")
    bool bAutoStartOnBeginPlay;

    // Test Control
    UFUNCTION(BlueprintCallable, Category = "Test Control")
    void StartTest();

    UFUNCTION(BlueprintCallable, Category = "Test Control")
    void StopTest();

    UFUNCTION(BlueprintCallable, Category = "Test Control")
    void PauseTest();

    UFUNCTION(BlueprintCallable, Category = "Test Control")
    void ResumeTest();

    // Test Actions
    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void SpawnTestUnits(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void DestroyAllTestUnits();

    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void StressTestSelection(int32 SelectionCount);

    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void StressTestMovement();

    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void StressTestCombat();

    UFUNCTION(BlueprintCallable, Category = "Test Actions")
    void TestFormationSystem(int32 UnitCount, EMingFormationType FormationType);

    // Results
    UFUNCTION(BlueprintPure, Category = "Results")
    const FMingPerformanceMetrics& GetCurrentMetrics() const { return CurrentMetrics; }

    UFUNCTION(BlueprintPure, Category = "Results")
    bool IsTestRunning() const { return bIsRunning; }

    UFUNCTION(BlueprintPure, Category = "Results")
    float GetTestProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Results")
    void ExportResultsToFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Results")
    FString GenerateReport() const;

    // Validation
    UFUNCTION(BlueprintPure, Category = "Validation")
    bool DidPassFPSRequirement() const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool DidPassMemoryRequirement() const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool DidPassAllRequirements() const;

    // Debug Visualization
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void EnableDebugVisualization(bool bEnable);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bShowDebugInfo;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsRunning;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsPaused;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    float TestStartTime;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    float TestElapsedTime;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    FMingPerformanceMetrics CurrentMetrics;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    TArray<FMingPerformanceMetrics> MetricsHistory;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    TArray<TWeakObjectPtr<class AMingTacticalUnit>> TestUnits;

    // Performance tracking
    void UpdateMetrics(float DeltaTime);
    void TrackFPS(float DeltaTime);
    void TrackMemory();
    void TrackUnitPerformance();
    void TrackEventSystem();
    void TrackAIPerformance();

    // Test scenarios
    void RunUnitSpawningTest(float DeltaTime);
    void RunMovementStressTest(float DeltaTime);
    void RunCombatStressTest(float DeltaTime);
    void RunSelectionStressTest(float DeltaTime);
    void RunFormationStressTest(float DeltaTime);
    void RunFullSimulationTest(float DeltaTime);

    // Helpers
    FVector GetRandomSpawnLocation() const;
    FVector GetRandomDestination() const;
    void SelectRandomUnits(int32 Count);
    void IssueRandomCommands();

    UPROPERTY()
    TWeakObjectPtr<class UWorld> WorldContext;

    // Timing
    float LastMetricsUpdateTime;
    float MetricsUpdateInterval;

    // Frame tracking
    int32 FrameCount;
    float TotalFrameTime;
    float MinFrameTime;
    float MaxFrameTime;

public:
    // Static utility functions
    UFUNCTION(BlueprintPure, Category = "Utilities", meta = (StaticFunction = "true"))
    static float GetCurrentFPS();

    UFUNCTION(BlueprintPure, Category = "Utilities", meta = (StaticFunction = "true"))
    static float GetUsedMemoryMB();

    UFUNCTION(BlueprintPure, Category = "Utilities", meta = (StaticFunction = "true"))
    static int32 GetObjectCount(UWorld* World, TSubclassOf<AActor> ActorClass);
};
