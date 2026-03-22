#include "MingPerformanceTest.h"
#include "MingTacticalUnit.h"
#include "MingAIController.h"
#include "MingFormationSystem.h"
#include "MingCore/Source/MingCore/Public/MingCoreEventBus.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformMemory.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/GameEngine.h"

UMingPerformanceTest::UMingPerformanceTest()
{
    TargetUnitCount = 1000;
    TargetFPS = 60.0f;
    TestDuration = 60.0f; // 60 seconds test
    TestType = EMingPerformanceTestType::FullSimulation;
    bAutoStartOnBeginPlay = false;
    bShowDebugInfo = true;

    bIsRunning = false;
    bIsPaused = false;
    TestStartTime = 0.0f;
    TestElapsedTime = 0.0f;

    LastMetricsUpdateTime = 0.0f;
    MetricsUpdateInterval = 0.5f; // Update metrics every 0.5 seconds

    FrameCount = 0;
    TotalFrameTime = 0.0f;
    MinFrameTime = MAX_flt;
    MaxFrameTime = 0.0f;
}

void UMingPerformanceTest::StartTest()
{
    if (bIsRunning)
    {
        UE_LOG(LogTemp, Warning, TEXT("Performance test is already running"));
        return;
    }

    WorldContext = GetWorld();
    if (!WorldContext.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot start performance test: No valid world context"));
        return;
    }

    // Reset metrics
    CurrentMetrics = FMingPerformanceMetrics();
    MetricsHistory.Empty();
    TestUnits.Empty();

    // Reset timing
    FrameCount = 0;
    TotalFrameTime = 0.0f;
    MinFrameTime = MAX_flt;
    MaxFrameTime = 0.0f;

    TestStartTime = WorldContext->GetTimeSeconds();
    TestElapsedTime = 0.0f;
    bIsRunning = true;
    bIsPaused = false;

    UE_LOG(LogTemp, Log, TEXT("=== PERFORMANCE TEST STARTED ==="));
    UE_LOG(LogTemp, Log, TEXT("Target Units: %d | Target FPS: %.1f | Duration: %.1fs | Type: %d"),
           TargetUnitCount, TargetFPS, TestDuration, (int32)TestType);

    // Spawn initial test units
    SpawnTestUnits(TargetUnitCount);

    // Start periodic metrics update
    WorldContext->GetTimerManager().SetTimer(
        MetricsUpdateTimerHandle,
        FTimerDelegate::CreateUObject(this, &UMingPerformanceTest::UpdateMetricsTick),
        MetricsUpdateInterval,
        true
    );
}

void UMingPerformanceTest::StopTest()
{
    if (!bIsRunning)
    {
        return;
    }

    bIsRunning = false;
    bIsPaused = false;

    // Stop timer
    if (WorldContext.IsValid())
    {
        WorldContext->GetTimerManager().ClearTimer(MetricsUpdateTimerHandle);
    }

    // Final metrics update
    UpdateMetrics(0.0f);

    UE_LOG(LogTemp, Log, TEXT("=== PERFORMANCE TEST COMPLETED ==="));
    UE_LOG(LogTemp, Log, TEXT("Duration: %.2fs | Final FPS: %.1f | Units: %d"),
           TestElapsedTime, CurrentMetrics.AverageFPS, CurrentMetrics.ActiveUnitCount);

    // Generate and log report
    FString Report = GenerateReport();
    UE_LOG(LogTemp, Log, TEXT("Performance Report:\n%s"), *Report);

    // Auto-export results
    FString DefaultPath = FPaths::ProjectLogDir() / TEXT("PerformanceTest_Results.json");
    ExportResultsToFile(DefaultPath);
}

void UMingPerformanceTest::PauseTest()
{
    bIsPaused = true;
    UE_LOG(LogTemp, Log, TEXT("Performance test paused"));
}

void UMingPerformanceTest::ResumeTest()
{
    bIsPaused = false;
    UE_LOG(LogTemp, Log, TEXT("Performance test resumed"));
}

void UMingPerformanceTest::SpawnTestUnits(int32 Count)
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    int32 SpawnedCount = 0;
    for (int32 i = 0; i < Count; ++i)
    {
        FVector SpawnLocation = GetRandomSpawnLocation();
        AMingTacticalUnit* NewUnit = AMingTacticalUnit::CreateUnit(WorldContext.Get(), SpawnLocation);
        
        if (NewUnit)
        {
            TestUnits.Add(NewUnit);
            SpawnedCount++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Spawned %d test units (Total: %d)"), SpawnedCount, TestUnits.Num());
}

void UMingPerformanceTest::DestroyAllTestUnits()
{
    int32 DestroyedCount = 0;
    for (auto& WeakUnit : TestUnits)
    {
        if (WeakUnit.IsValid())
        {
            AMingTacticalUnit::DestroyUnit(WeakUnit.Get());
            DestroyedCount++;
        }
    }

    TestUnits.Empty();
    UE_LOG(LogTemp, Log, TEXT("Destroyed %d test units"), DestroyedCount);
}

void UMingPerformanceTest::StressTestSelection(int32 SelectionCount)
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    // Simulate rapid selection changes
    for (int32 i = 0; i < SelectionCount; ++i)
    {
        SelectRandomUnits(FMath::RandRange(1, FMath::Min(10, TestUnits.Num())));
    }

    UE_LOG(LogTemp, Verbose, TEXT("Selection stress test: %d operations"), SelectionCount);
}

void UMingPerformanceTest::StressTestMovement()
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    // Issue random move commands to all units
    for (auto& WeakUnit : TestUnits)
    {
        if (WeakUnit.IsValid())
        {
            if (AMingAIController* AIController = Cast<AMingAIController>(WeakUnit->GetController()))
            {
                FVector Destination = GetRandomDestination();
                AIController->IssueMoveCommand(Destination);
            }
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Movement stress test: Commands issued to %d units"), TestUnits.Num());
}

void UMingPerformanceTest::StressTestCombat()
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    // Create combat scenario
    int32 AttackersCount = FMath::Min(TestUnits.Num() / 2, 50);
    
    for (int32 i = 0; i < AttackersCount; ++i)
    {
        if (TestUnits[i].IsValid())
        {
            if (AMingAIController* AIController = Cast<AMingAIController>(TestUnits[i]->GetController()))
            {
                // Find random target
                int32 TargetIndex = FMath::RandRange(AttackersCount, TestUnits.Num() - 1);
                if (TestUnits[TargetIndex].IsValid())
                {
                    AIController->IssueAttackCommand(TestUnits[TargetIndex].Get());
                }
            }
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Combat stress test: %d attackers"), AttackersCount);
}

void UMingPerformanceTest::TestFormationSystem(int32 UnitCount, EMingFormationType FormationType)
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    // Create formation
    UMingFormationSystem* FormationSystem = NewObject<UMingFormationSystem>(this);
    FVector Center = WorldContext->GetFirstPlayerController() ? 
                     WorldContext->GetFirstPlayerController()->GetPawn()->GetActorLocation() : 
                     FVector::ZeroVector;
    
    FormationSystem->InitializeFormation(FormationType, Center, FVector::ForwardVector, UnitCount);

    // Assign units to formation
    int32 AssignedCount = 0;
    for (int32 i = 0; i < FMath::Min(UnitCount, TestUnits.Num()); ++i)
    {
        if (TestUnits[i].IsValid())
        {
            int32 SlotIndex = FormationSystem->FindBestSlotForUnit(TestUnits[i].Get());
            if (SlotIndex >= 0)
            {
                FormationSystem->AssignUnitToSlot(TestUnits[i].Get(), SlotIndex);
                AssignedCount++;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Formation test: %d units assigned to %s formation"),
           AssignedCount, *UEnum::GetValueAsString(FormationType));
}

void UMingPerformanceTest::UpdateMetricsTick()
{
    if (!bIsRunning || bIsPaused)
    {
        return;
    }

    // Calculate elapsed time
    if (WorldContext.IsValid())
    {
        TestElapsedTime = WorldContext->GetTimeSeconds() - TestStartTime;
    }

    // Check if test should auto-stop
    if (TestElapsedTime >= TestDuration)
    {
        StopTest();
        return;
    }

    // Update metrics
    UpdateMetrics(MetricsUpdateInterval);

    // Run test scenario based on type
    switch (TestType)
    {
    case EMingPerformanceTestType::MovementStress:
        RunMovementStressTest(MetricsUpdateInterval);
        break;
    case EMingPerformanceTestType::CombatStress:
        RunCombatStressTest(MetricsUpdateInterval);
        break;
    case EMingPerformanceTestType::SelectionStress:
        RunSelectionStressTest(MetricsUpdateInterval);
        break;
    case EMingPerformanceTestType::FullSimulation:
        RunFullSimulationTest(MetricsUpdateInterval);
        break;
    default:
        break;
    }

    // Store metrics history
    MetricsHistory.Add(CurrentMetrics);

    // Debug output
    if (bShowDebugInfo)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[%.1fs] FPS: %.1f | Units: %d | Memory: %.1f MB"),
               TestElapsedTime, CurrentMetrics.AverageFPS, 
               CurrentMetrics.ActiveUnitCount, CurrentMetrics.UsedMemoryMB);
    }
}

void UMingPerformanceTest::UpdateMetrics(float DeltaTime)
{
    if (!WorldContext.IsValid())
    {
        return;
    }

    // FPS Tracking
    TrackFPS(DeltaTime);

    // Memory Tracking
    TrackMemory();

    // Unit Count
    CurrentMetrics.ActiveUnitCount = 0;
    for (const auto& WeakUnit : TestUnits)
    {
        if (WeakUnit.IsValid() && WeakUnit->IsAlive())
        {
            CurrentMetrics.ActiveUnitCount++;
        }
    }

    // Event System Tracking
    TrackEventSystem();

    // AI Performance
    TrackAIPerformance();
}

void UMingPerformanceTest::TrackFPS(float DeltaTime)
{
    if (DeltaTime > 0.0f)
    {
        float CurrentFPS = 1.0f / DeltaTime;
        FrameCount++;
        TotalFrameTime += DeltaTime;

        CurrentMetrics.AverageFPS = FrameCount > 0 ? (1.0f / (TotalFrameTime / FrameCount)) : 0.0f;
        CurrentMetrics.MinFPS = FMath::Min(CurrentMetrics.MinFPS, CurrentFPS);
        CurrentMetrics.MaxFPS = FMath::Max(CurrentMetrics.MaxFPS, CurrentFPS);
    }
}

void UMingPerformanceTest::TrackMemory()
{
    // Get memory statistics
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    
    // Convert to MB
    CurrentMetrics.UsedMemoryMB = (MemoryStats.UsedVirtual - MemoryStats.AvailableVirtual) / (1024.0f * 1024.0f);
    CurrentMetrics.PeakMemoryMB = FMath::Max(CurrentMetrics.PeakMemoryMB, CurrentMetrics.UsedMemoryMB);
}

void UMingPerformanceTest::TrackEventSystem()
{
    // This would track event system metrics
    // For now, just a placeholder
    CurrentMetrics.EventsPerSecond = FMath::RandRange(100, 1000); // Simulated
    CurrentMetrics.AverageEventLatency = 0.0f; // Would require actual instrumentation
}

void UMingPerformanceTest::TrackAIPerformance()
{
    // Track AI update performance
    // This would require hooks into the AI controller update loop
    CurrentMetrics.AverageAIUpdateTime = 0.0f; // Placeholder
    CurrentMetrics.AIQueriesPerSecond = FMath::RandRange(50, 500); // Simulated
}

void UMingPerformanceTest::RunMovementStressTest(float DeltaTime)
{
    // Periodically issue random movement commands
    static float LastCommandTime = 0.0f;
    LastCommandTime += DeltaTime;

    if (LastCommandTime >= 2.0f) // Every 2 seconds
    {
        IssueRandomCommands();
        LastCommandTime = 0.0f;
    }
}

void UMingPerformanceTest::RunCombatStressTest(float DeltaTime)
{
    static float LastCombatTime = 0.0f;
    LastCombatTime += DeltaTime;

    if (LastCombatTime >= 3.0f) // Every 3 seconds
    {
        StressTestCombat();
        LastCombatTime = 0.0f;
    }
}

void UMingPerformanceTest::RunSelectionStressTest(float DeltaTime)
{
    static float LastSelectionTime = 0.0f;
    LastSelectionTime += DeltaTime;

    if (LastSelectionTime >= 1.0f) // Every second
    {
        StressTestSelection(5);
        LastSelectionTime = 0.0f;
    }
}

void UMingPerformanceTest::RunFullSimulationTest(float DeltaTime)
{
    // Combine all stress tests
    RunMovementStressTest(DeltaTime);
    
    static float LastCombatTime = 0.0f;
    LastCombatTime += DeltaTime;
    if (LastCombatTime >= 5.0f)
    {
        StressTestCombat();
        LastCombatTime = 0.0f;
    }
}

FVector UMingPerformanceTest::GetRandomSpawnLocation() const
{
    if (!WorldContext.IsValid())
    {
        return FVector::ZeroVector;
    }

    // Spawn in a 2000x2000 area around origin
    float X = FMath::RandRange(-1000.0f, 1000.0f);
    float Y = FMath::RandRange(-1000.0f, 1000.0f);
    float Z = 100.0f; // Above ground

    return FVector(X, Y, Z);
}

FVector UMingPerformanceTest::GetRandomDestination() const
{
    // Random point in test area
    float X = FMath::RandRange(-1500.0f, 1500.0f);
    float Y = FMath::RandRange(-1500.0f, 1500.0f);
    float Z = 100.0f;

    return FVector(X, Y, Z);
}

void UMingPerformanceTest::SelectRandomUnits(int32 Count)
{
    // This would integrate with the selection manager
    // For now, just a placeholder
}

void UMingPerformanceTest::IssueRandomCommands()
{
    // Issue random move commands
    for (auto& WeakUnit : TestUnits)
    {
        if (WeakUnit.IsValid() && FMath::RandBool())
        {
            if (AMingAIController* AIController = Cast<AMingAIController>(WeakUnit->GetController()))
            {
                FVector Destination = GetRandomDestination();
                AIController->IssueMoveCommand(Destination);
            }
        }
    }
}

float UMingPerformanceTest::GetTestProgress() const
{
    if (!bIsRunning || TestDuration <= 0.0f)
    {
        return 0.0f;
    }

    return FMath::Clamp(TestElapsedTime / TestDuration, 0.0f, 1.0f);
}

void UMingPerformanceTest::ExportResultsToFile(const FString& FilePath)
{
    // Create JSON output
    FString JsonString;
    JsonString += TEXT("{\n");
    JsonString += FString::Printf(TEXT("  \"test_type\": %d,\n"), (int32)TestType);
    JsonString += FString::Printf(TEXT("  \"target_units\": %d,\n"), TargetUnitCount);
    JsonString += FString::Printf(TEXT("  \"target_fps\": %.2f,\n"), TargetFPS);
    JsonString += FString::Printf(TEXT("  \"test_duration\": %.2f,\n"), TestDuration);
    JsonString += FString::Printf(TEXT("  \"elapsed_time\": %.2f,\n"), TestElapsedTime);
    JsonString += FString::Printf(TEXT("  \"average_fps\": %.2f,\n"), CurrentMetrics.AverageFPS);
    JsonString += FString::Printf(TEXT("  \"min_fps\": %.2f,\n"), CurrentMetrics.MinFPS);
    JsonString += FString::Printf(TEXT("  \"max_fps\": %.2f,\n"), CurrentMetrics.MaxFPS);
    JsonString += FString::Printf(TEXT("  \"used_memory_mb\": %.2f,\n"), CurrentMetrics.UsedMemoryMB);
    JsonString += FString::Printf(TEXT("  \"peak_memory_mb\": %.2f,\n"), CurrentMetrics.PeakMemoryMB);
    JsonString += FString::Printf(TEXT("  \"active_units\": %d,\n"), CurrentMetrics.ActiveUnitCount);
    JsonString += FString::Printf(TEXT("  \"passed_requirements\": %s\n"), DidPassAllRequirements() ? TEXT("true") : TEXT("false"));
    JsonString += TEXT("}\n");

    // Write to file
    FFileHelper::SaveStringToFile(JsonString, *FilePath);
    
    UE_LOG(LogTemp, Log, TEXT("Performance results exported to: %s"), *FilePath);
}

FString UMingPerformanceTest::GenerateReport() const
{
    FString Report;
    Report += FString::Printf(TEXT("=== MingGoRTS Performance Test Report ===\n\n"));
    Report += FString::Printf(TEXT("Test Type: %s\n"), *UEnum::GetValueAsString(TestType));
    Report += FString::Printf(TEXT("Target Units: %d\n"), TargetUnitCount);
    Report += FString::Printf(TEXT("Target FPS: %.1f\n\n"), TargetFPS);
    
    Report += FString::Printf(TEXT("--- Results ---\n"));
    Report += FString::Printf(TEXT("Test Duration: %.2f seconds\n"), TestElapsedTime);
    Report += FString::Printf(TEXT("Average FPS: %.2f\n"), CurrentMetrics.AverageFPS);
    Report += FString::Printf(TEXT("Min FPS: %.2f\n"), CurrentMetrics.MinFPS);
    Report += FString::Printf(TEXT("Max FPS: %.2f\n"), CurrentMetrics.MaxFPS);
    Report += FString::Printf(TEXT("Active Units: %d\n"), CurrentMetrics.ActiveUnitCount);
    Report += FString::Printf(TEXT("Memory Usage: %.2f MB (Peak: %.2f MB)\n\n"), 
                             CurrentMetrics.UsedMemoryMB, CurrentMetrics.PeakMemoryMB);
    
    Report += FString::Printf(TEXT("--- Validation ---\n"));
    Report += FString::Printf(TEXT("FPS Requirement: %s\n"), DidPassFPSRequirement() ? TEXT("PASS") : TEXT("FAIL"));
    Report += FString::Printf(TEXT("Memory Requirement: %s\n"), DidPassMemoryRequirement() ? TEXT("PASS") : TEXT("FAIL"));
    Report += FString::Printf(TEXT("Overall: %s\n"), DidPassAllRequirements() ? TEXT("PASS") : TEXT("FAIL"));

    return Report;
}

bool UMingPerformanceTest::DidPassFPSRequirement() const
{
    return CurrentMetrics.AverageFPS >= TargetFPS && CurrentMetrics.MinFPS >= (TargetFPS * 0.8f);
}

bool UMingPerformanceTest::DidPassMemoryRequirement() const
{
    // Assuming 4GB limit for large-scale battles
    return CurrentMetrics.PeakMemoryMB <= 4096.0f;
}

bool UMingPerformanceTest::DidPassAllRequirements() const
{
    return DidPassFPSRequirement() && DidPassMemoryRequirement();
}

void UMingPerformanceTest::EnableDebugVisualization(bool bEnable)
{
    bShowDebugInfo = bEnable;
}

// Static utility functions
float UMingPerformanceTest::GetCurrentFPS()
{
    if (GEngine)
    {
        return 1.0f / GEngine->GetWorld()->GetDeltaSeconds();
    }
    return 0.0f;
}

float UMingPerformanceTest::GetUsedMemoryMB()
{
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    return (MemoryStats.UsedVirtual - MemoryStats.AvailableVirtual) / (1024.0f * 1024.0f);
}

int32 UMingPerformanceTest::GetObjectCount(UWorld* World, TSubclassOf<AActor> ActorClass)
{
    if (!World)
    {
        return 0;
    }

    int32 Count = 0;
    for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
    {
        Count++;
    }
    return Count;
}
