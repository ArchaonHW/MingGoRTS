#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSystemTest.generated.h"

class UMingRTSUnitManager;
class UMingRTSCombatSystem;
class AMingRTSAIController;
class UMingRTSResourceManager;
class UMingRTSBuildingSystem;
class UMingRTSEconomicSystem;

/**
 * RTS系統測試???X * 測試摧毀RTS摧毀系統故事重要性?能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 目標數量RTS系統測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void RunAllRTSTests();

    /**
     * 測試摧毀管?X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitManager();

    /**
     * 測試??鬥系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatSystem();

    /**
     * 測試AI??制X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIController();

    /**
     * 測試資??管?X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceManager();

    /**
     * 測試建??系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingSystem();

    /**
     * 測試經??系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicSystem();

    /**
     * 測試系統摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestSystemIntegration();

    /**
     * ??能測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestPerformance();

    /**
     * 壓??測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestStressTest();

    /**
     * 摧毀測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBoundaryConditions();

    /**
     * ??誤摧毀測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestErrorHandling();

    /**
     * ??建測試摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CreateTestData();

    /**
     * 清??測試摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CleanupTestData();

    /**
     * 驗??系統??康??X     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static bool VerifySystemHealth();

    /**
     * 摧毀測試摧毀
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GetTestReport();

    /**
     * ???X?能摧毀
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GeneratePerformanceReport();

    /**
     * 測試故事重要性?移X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitSelectionAndMovement();

    /**
     * 測試X系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestFormationSystem();

    /**
     * 測試??鬥機制
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatMechanics();

    /**
     * 測試AI行為
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIBehavior();

    /**
     * 測試資?X?產
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceProduction();

    /**
     * 測試建??建設
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingConstruction();

    /**
     * 測試經??循環
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicCycle();

    /**
     * 測試貿??系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestTradeSystem();

    /**
     * 測試市場機制
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestMarketMechanics();

private:
    static bool bTestPassed;
    static FString TestReport;
    static int32 TestsRun;
    static int32 TestsPassed;

    static void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};
    static void ResetTestResults();
    static void MeasurePerformance(const FString& OperationName, TFunction<void()> Operation};
};

