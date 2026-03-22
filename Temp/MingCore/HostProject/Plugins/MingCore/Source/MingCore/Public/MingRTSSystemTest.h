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
 * RTS系統測試運行器
 * 測試所有RTS核心系統的功能和性能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 運行所有RTS系統測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void RunAllRTSTests();

    /**
     * 測試單位管理器
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitManager();

    /**
     * 測試戰鬥系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatSystem();

    /**
     * 測試AI控制器
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIController();

    /**
     * 測試資源管理器
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceManager();

    /**
     * 測試建築系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingSystem();

    /**
     * 測試經濟系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicSystem();

    /**
     * 測試系統集成
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestSystemIntegration();

    /**
     * 性能測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestPerformance();

    /**
     * 壓力測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestStressTest();

    /**
     * 邊界測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBoundaryConditions();

    /**
     * 錯誤處理測試
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestErrorHandling();

    /**
     * 創建測試數據
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CreateTestData();

    /**
     * 清理測試數據
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CleanupTestData();

    /**
     * 驗證系統健康狀態
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static bool VerifySystemHealth();

    /**
     * 獲取測試報告
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GetTestReport();

    /**
     * 生成性能報告
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GeneratePerformanceReport();

    /**
     * 測試單位選擇和移動
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitSelectionAndMovement();

    /**
     * 測試陣型系統
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestFormationSystem();

    /**
     * 測試戰鬥機制
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatMechanics();

    /**
     * 測試AI行為
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIBehavior();

    /**
     * 測試資源生產
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceProduction();

    /**
     * 測試建築建設
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingConstruction();

    /**
     * 測試經濟循環
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicCycle();

    /**
     * 測試貿易系統
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

    static void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    static void ResetTestResults();
    static void MeasurePerformance(const FString& OperationName, TFunction<void()> Operation);
};
