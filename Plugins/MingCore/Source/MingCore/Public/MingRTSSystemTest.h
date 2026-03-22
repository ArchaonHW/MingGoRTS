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
 * RTS╰参代刚???X * 代刚????RTS????╰参???X??X?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ???X???RTS╰参代刚
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void RunAllRTSTests();

    /**
     * 代刚????恨?X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitManager();

    /**
     * 代刚??矮╰参
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatSystem();

    /**
     * 代刚AI??X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIController();

    /**
     * 代刚戈??恨?X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceManager();

    /**
     * 代刚??╰参
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingSystem();

    /**
     * 代刚竒??╰参
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicSystem();

    /**
     * 代刚╰参????
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestSystemIntegration();

    /**
     * ??代刚
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestPerformance();

    /**
     * 溃??代刚
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestStressTest();

    /**
     * ????代刚
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBoundaryConditions();

    /**
     * ??粇????代刚
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestErrorHandling();

    /**
     * ??代刚????
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CreateTestData();

    /**
     * 睲??代刚????
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CleanupTestData();

    /**
     * 喷??╰参??眃??X     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static bool VerifySystemHealth();

    /**
     * ????代刚????
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GetTestReport();

    /**
     * ???X?????
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GeneratePerformanceReport();

    /**
     * 代刚???X??X?簿X     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitSelectionAndMovement();

    /**
     * 代刚X╰参
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestFormationSystem();

    /**
     * 代刚??矮诀
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatMechanics();

    /**
     * 代刚AI︽
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIBehavior();

    /**
     * 代刚戈?X?玻
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceProduction();

    /**
     * 代刚??砞
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingConstruction();

    /**
     * 代刚竒??碻吏
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicCycle();

    /**
     * 代刚禩??╰参
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestTradeSystem();

    /**
     * 代刚カ初诀
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

