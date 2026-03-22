#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingIntegrationManager.generated.h"

UENUM(BlueprintType)
enum class EMingIntegrationStatus : uint8
{
    NotStarted UMETA(DisplayName = "Not Started"),
    InProgress UMETA(DisplayName = "In Progress"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed"),
    Skipped UMETA(DisplayName = "Skipped")
};

UENUM(BlueprintType)
enum class EMingSystemType : uint8
{
    Core UMETA(DisplayName = "Core"),
    Tactical UMETA(DisplayName = "Tactical"),
    Building UMETA(DisplayName = "Building"),
    Strategic UMETA(DisplayName = "Strategic"),
    Personal UMETA(DisplayName = "Personal"),
    UI UMETA(DisplayName = "UI"),
    Audio UMETA(DisplayName = "Audio"),
    LevelDesigner UMETA(DisplayName = "Level Designer"),
    Mercenary UMETA(DisplayName = "Mercenary"),
    Achievements UMETA(DisplayName = "Achievements"),
    Platform UMETA(DisplayName = "Platform")
};

USTRUCT(BlueprintType)
struct FMingSystemInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSystemType SystemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SystemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ClassName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PluginName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLoaded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingIntegrationStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StatusMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LoadTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependants;

    FMingSystemInfo()
        : SystemType(EMingSystemType::Core)
        , bIsRequired(true)
        , bIsLoaded(false)
        , bIsInitialized(false)
        , Status(EMingIntegrationStatus::NotStarted)
        , LoadTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingIntegrationTest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCritical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPassed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TestSteps;

    FMingIntegrationTest()
        : bIsCritical(false)
        , bIsPassed(false)
        , ExecutionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingCorePerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FPS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FrameTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DrawCalls;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Triangles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NetworkLatency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DiskIO;

    FMingPerformanceMetrics()
        : FPS(60.0f)
        , FrameTime(16.67f)
        , MemoryUsage(0.0f)
        , CPUUsage(0.0f)
        , GPUUsage(0.0f)
        , DrawCalls(0)
        , Triangles(0)
        , NetworkLatency(0.0f)
        , DiskIO(0.0f)
    {}
};

/**
 * Integration Manager for MingGoRTS
 * Manages system integration, testing, and performance monitoring
 */
UCLASS(ClassGroup = (Integration), Blueprintable)
class MINGINTEGRATION_API UMingIntegrationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingIntegrationManager();

    // System Integration
    UFUNCTION(BlueprintCallable, Category = "Integration")
    void InitializeIntegration();

    UFUNCTION(BlueprintCallable, Category = "Integration")
    void LoadAllSystems();

    UFUNCTION(BlueprintCallable, Category = "Integration")
    void InitializeAllSystems();

    UFUNCTION(BlueprintCallable, Category = "Integration")
    void ShutdownAllSystems();

    UFUNCTION(BlueprintPure, Category = "Integration")
    TArray<FMingSystemInfo> GetAllSystems() const;

    UFUNCTION(BlueprintPure, Category = "Integration")
    TArray<FMingSystemInfo> GetLoadedSystems() const;

    UFUNCTION(BlueprintPure, Category = "Integration")
    TArray<FMingSystemInfo> GetFailedSystems() const;

    UFUNCTION(BlueprintPure, Category = "Integration")
    bool IsSystemLoaded(const FString& SystemName) const;

    UFUNCTION(BlueprintPure, Category = "Integration")
    bool IsAllSystemsLoaded() const;

    // Integration Testing
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void RunIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Testing")
    void RunSystemTest(const FString& TestName);

    UFUNCTION(BlueprintCallable, Category = "Testing")
    void RunPerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "Testing")
    void RunCompatibilityTests();

    UFUNCTION(BlueprintPure, Category = "Testing")
    TArray<FMingIntegrationTest> GetAllTests() const;

    UFUNCTION(BlueprintPure, Category = "Testing")
    TArray<FMingIntegrationTest> GetPassedTests() const;

    UFUNCTION(BlueprintPure, Category = "Testing")
    TArray<FMingIntegrationTest> GetFailedTests() const;

    UFUNCTION(BlueprintPure, Category = "Testing")
    bool AreAllTestsPassed() const;

    // Performance Monitoring
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void UpdatePerformanceMetrics();

    UFUNCTION(BlueprintPure, Category = "Performance")
    FMingPerformanceMetrics GetCurrentMetrics() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    TArray<FMingPerformanceMetrics> GetPerformanceHistory() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPerformanceTarget(float TargetFPS, float TargetMemory);

    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsPerformanceTargetMet() const;

    // System Communication
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void TestEventSystem();

    UFUNCTION(BlueprintCallable, Category = "Communication")
    void TestDataFlow();

    UFUNCTION(BlueprintCallable, Category = "Communication")
    void TestCrossSystemCommunication();

    UFUNCTION(BlueprintPure, Category = "Communication")
    bool IsEventSystemWorking() const;

    UFUNCTION(BlueprintPure, Category = "Communication")
    bool IsDataFlowWorking() const;

    // Health Check
    UFUNCTION(BlueprintCallable, Category = "Health")
    void RunHealthCheck();

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsSystemHealthy() const;

    UFUNCTION(BlueprintPure, Category = "Health")
    TArray<FString> GetHealthIssues() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void FixHealthIssue(const FString& Issue);

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemLoaded, const FString&, SystemName, float, LoadTime);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemFailed, const FString&, SystemName, const FString&, Error);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllSystemsLoaded, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationTestCompleted, bool, bAllPassed);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSystemLoaded OnSystemLoaded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSystemFailed OnSystemFailed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAllSystemsLoaded OnAllSystemsLoaded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnIntegrationTestCompleted OnIntegrationTestCompleted;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetSystemTypeName(EMingSystemType SystemType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetStatusName(EMingIntegrationStatus Status);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveIntegrationData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadIntegrationData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingSystemInfo> SystemRegistry;

    UPROPERTY()
    TArray<FMingIntegrationTest> IntegrationTests;

    UPROPERTY()
    TArray<FMingPerformanceMetrics> PerformanceHistory;

    UPROPERTY()
    FMingPerformanceMetrics CurrentMetrics;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    float TargetFPS;

    UPROPERTY()
    float TargetMemory;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeSystemRegistry();
    void CreateIntegrationTests();
    void SetupPerformanceMonitoring();

    // System Loading
    void LoadSystem(const FString& SystemName);
    void InitializeSystem(const FString& SystemName);
    void ShutdownSystem(const FString& SystemName);
    bool ValidateSystemDependencies(const FString& SystemName);

    // Testing Functions
    void RunCoreTests();
    void RunTacticalTests();
    void RunBuildingTests();
    void RunStrategicTests();
    void RunPersonalTests();
    void RunUITests();
    void RunAudioTests();
    void RunLevelDesignerTests();
    void RunMercenaryTests();
    void RunAchievementTests();
    void RunPlatformTests();

    // Performance Functions
    void CollectPerformanceMetrics();
    void AnalyzePerformanceData();
    void OptimizePerformance();

    // Communication Tests
    void TestCoreEventBus();
    void TestTacticalEventBus();
    void TestBuildingEventBus();
    void TestStrategicEventBus();
    void TestPersonalEventBus();

    // Health Check Functions
    void CheckMemoryUsage();
    void CheckCPUUsage();
    void CheckNetworkStatus();
    void CheckDiskSpace();

    // Helpers
    FMingSystemInfo* FindSystem(const FString& SystemName);
    FMingIntegrationTest* FindTest(const FString& TestName);
    void UpdateSystemStatus(const FString& SystemName, EMingIntegrationStatus Status, const FString& Message);
    void LogIntegrationEvent(const FString& Event, const FString& Details);
};
