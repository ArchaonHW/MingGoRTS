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
 * RTSç³»çµ±æ¸¬è©¦?‹è??? * æ¸¬è©¦?€?‰RTS?¸å?ç³»çµ±?„å??½å??§èƒ½
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ?‹è??€?‰RTSç³»çµ±æ¸¬è©¦
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void RunAllRTSTests();

    /**
     * æ¸¬è©¦?®ä?ç®¡ç???     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitManager();

    /**
     * æ¸¬è©¦?°é¬¥ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatSystem();

    /**
     * æ¸¬è©¦AI?§åˆ¶??     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIController();

    /**
     * æ¸¬è©¦è³‡æ?ç®¡ç???     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceManager();

    /**
     * æ¸¬è©¦å»ºç?ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingSystem();

    /**
     * æ¸¬è©¦ç¶“æ?ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicSystem();

    /**
     * æ¸¬è©¦ç³»çµ±?†æ?
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestSystemIntegration();

    /**
     * ?§èƒ½æ¸¬è©¦
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestPerformance();

    /**
     * å£“å?æ¸¬è©¦
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestStressTest();

    /**
     * ?Šç?æ¸¬è©¦
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBoundaryConditions();

    /**
     * ?¯èª¤?•ç?æ¸¬è©¦
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestErrorHandling();

    /**
     * ?µå»ºæ¸¬è©¦?¸æ?
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CreateTestData();

    /**
     * æ¸…ç?æ¸¬è©¦?¸æ?
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static void CleanupTestData();

    /**
     * é©—è?ç³»çµ±?¥åº·?€??     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static bool VerifySystemHealth();

    /**
     * ?²å?æ¸¬è©¦?±å?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GetTestReport();

    /**
     * ?Ÿæ??§èƒ½?±å?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS System Test")
    static FString GeneratePerformanceReport();

    /**
     * æ¸¬è©¦?®ä??¸æ??Œç§»??     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestUnitSelectionAndMovement();

    /**
     * æ¸¬è©¦???ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestFormationSystem();

    /**
     * æ¸¬è©¦?°é¬¥æ©Ÿåˆ¶
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestCombatMechanics();

    /**
     * æ¸¬è©¦AIè¡Œç‚º
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestAIBehavior();

    /**
     * æ¸¬è©¦è³‡æ??Ÿç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestResourceProduction();

    /**
     * æ¸¬è©¦å»ºç?å»ºè¨­
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestBuildingConstruction();

    /**
     * æ¸¬è©¦ç¶“æ?å¾ªç’°
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestEconomicCycle();

    /**
     * æ¸¬è©¦è²¿æ?ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "RTS System Test")
    static bool TestTradeSystem();

    /**
     * æ¸¬è©¦å¸‚å ´æ©Ÿåˆ¶
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
