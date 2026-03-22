#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLargeScaleCombatTest.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * 大規模戰鬥性能測試
 * 測試1000+單位的戰鬥性能
 */
UCLASS()
class MINGTACTICAL_API UMingLargeScaleCombatTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLargeScaleCombatTest();

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void InitializeTest();

    // 清理測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CleanupTest();

    // 創建測試單位
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void CreateTestUnits(int32 UnitCount = 1000);

    // 開始性能測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StartPerformanceTest();

    // 停止性能測試
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    void StopPerformanceTest();

    // 獲取性能統計
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetAverageFPS() const { return AverageFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMinFPS() const { return MinFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    float GetMaxFPS() const { return MaxFPS; }

    UFUNCTION(BlueprintPure, Category = "Performance Test")
    int32 GetActiveUnitCount() const { return TestUnits.Num(); }

    // 測試結果
    UFUNCTION(BlueprintPure, Category = "Performance Test")
    bool IsPerformanceTestPass() const;

protected:
    virtual void Tick(float DeltaTime);

private:
    // 測試單位
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> TestUnits;

    // 性能監控
    bool bIsTestRunning;
    float TestStartTime;
    float TotalFrameTime;
    int32 FrameCount;
    
    // FPS統計
    float AverageFPS;
    float MinFPS;
    float MaxFPS;
    float LastFrameTime;

    // 選擇管理器
    UPROPERTY()
    TObjectPtr<UMingSelectionManager> SelectionManager;

    // 創建單位
    AMingTacticalUnit* CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType);

    // 更新性能統計
    void UpdatePerformanceStats(float DeltaTime);

    // 重置性能統計
    void ResetPerformanceStats();

    // 模擬單位行為
    void SimulateUnitBehavior(float DeltaTime);

    // 測試單位選擇
    void TestUnitSelection();

    // 測試單位移動
    void TestUnitMovement();

    // 測試單位戰鬥
    void TestUnitCombat();
};
