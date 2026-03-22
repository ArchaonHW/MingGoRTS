#include "MingTacticalBlueprintLibrary.h"
#include "MingLargeScaleCombatTest.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingLargeScaleCombatTest* UMingTacticalBlueprintLibrary::CreatePerformanceTest()
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create performance test: No valid world"));
        return nullptr;
    }

    UMingLargeScaleCombatTest* TestInstance = NewObject<UMingLargeScaleCombatTest>(World);
    if (TestInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("Performance test instance created"));
    }

    return TestInstance;
}

void UMingTacticalBlueprintLibrary::InitializePerformanceTest(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot initialize: Test instance is null"));
        return;
    }

    TestInstance->InitializeTest();
}

void UMingTacticalBlueprintLibrary::CreateTestUnits(UMingLargeScaleCombatTest* TestInstance, int32 UnitCount)
{
    if (!TestInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create units: Test instance is null"));
        return;
    }

    TestInstance->CreateTestUnits(UnitCount);
}

void UMingTacticalBlueprintLibrary::StartPerformanceTest(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot start test: Test instance is null"));
        return;
    }

    TestInstance->StartPerformanceTest();
}

void UMingTacticalBlueprintLibrary::StopPerformanceTest(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot stop test: Test instance is null"));
        return;
    }

    TestInstance->StopPerformanceTest();
}

bool UMingTacticalBlueprintLibrary::IsPerformanceTestPassing(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return false;
    }

    return TestInstance->IsPerformanceTestPass();
}

float UMingTacticalBlueprintLibrary::GetAverageFPS(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return 0.0f;
    }

    return TestInstance->GetAverageFPS();
}

float UMingTacticalBlueprintLibrary::GetMinFPS(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return 0.0f;
    }

    return TestInstance->GetMinFPS();
}

float UMingTacticalBlueprintLibrary::GetMaxFPS(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return 0.0f;
    }

    return TestInstance->GetMaxFPS();
}

int32 UMingTacticalBlueprintLibrary::GetActiveUnitCount(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return 0;
    }

    return TestInstance->GetActiveUnitCount();
}

void UMingTacticalBlueprintLibrary::CleanupPerformanceTest(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot cleanup: Test instance is null"));
        return;
    }

    TestInstance->CleanupTest();
}

FString UMingTacticalBlueprintLibrary::GetPerformanceStatusText(UMingLargeScaleCombatTest* TestInstance)
{
    if (!TestInstance)
    {
        return TEXT("Test instance is null");
    }

    const int32 UnitCount = TestInstance->GetActiveUnitCount();
    const float AvgFPS = TestInstance->GetAverageFPS();
    const float MinFPS = TestInstance->GetMinFPS();
    const float MaxFPS = TestInstance->GetMaxFPS();
    const bool bIsPassing = TestInstance->IsPerformanceTestPass();

    FString StatusText = FString::Printf(
        TEXT("Units: %d\nAvg FPS: %.1f\nMin FPS: %.1f\nMax FPS: %.1f\nStatus: %s"),
        UnitCount, AvgFPS, MinFPS, MaxFPS,
        bIsPassing ? TEXT("PASS") : TEXT("FAIL")
    );

    return StatusText;
}
