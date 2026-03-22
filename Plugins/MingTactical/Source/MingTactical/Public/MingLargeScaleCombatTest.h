#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLargeScaleCombatTest.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * �j??�Ҿ԰��ʯ����
 * ����1000+???X?�԰��ʯ�
 */
UCLASS()
class MINGTACTICAL_API UMingLargeScaleCombatTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLargeScaleCombatTest();

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void InitializeTest();

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CleanupTest();

    // ??�ش���摧毀
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CreateTestUnits(int32 UnitCount = 1000);

    // ???X?�����
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StartPerformanceTest();

    // ??��??�����
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StopPerformanceTest();

    // ???X?���??
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetAverageFPS() const { return AverageFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMinFPS() const { return MinFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMaxFPS() const { return MaxFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    int32 GetActiveUnitCount() const { return TestUnits.Num(); }

    // ���յ�??
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsPerformanceTestPass() const;

protected:
    virtual void Tick(float DeltaTime);

private:
    // ����摧毀
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> TestUnits;

    // ??��X??
    bool bIsTestRunning;
    float TestStartTime;
    float TotalFrameTime;
    int32 FrameCount;
    
    // FPS��??
    float AverageFPS;
    float MinFPS;
    float MaxFPS;
    float LastFrameTime;

    // 摧毀��?X
    UPROPERTY()
    TObjectPtr<UMingSelectionManager> SelectionManager;

    // ??��摧毀
    AMingTacticalUnit* CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType);

    // ??�s??���??
    void UpdatePerformanceStats(float DeltaTime);

    // ??�m??���??
    void ResetPerformanceStats();

    // ����摧毀�欰
    void SimulateUnitBehavior(float DeltaTime);

    // ����目標數量
    void TestUnitSelection();

    // ����摧毀��??
    void TestUnitMovement();

    // ����???X?��
    void TestUnitCombat();
};

