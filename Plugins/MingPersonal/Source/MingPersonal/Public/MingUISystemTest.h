#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUISystemTest.generated.h"

/**
 * UIÁ≥ªÁµ±Ê∏¨Ë©¶?ãË??? * Ê∏¨Ë©¶?ã‰∫∫Á≥ªÁµ±UI?ÑÂ??ΩÂ??ßËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingUISystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ?ãË??Ä?âUIÁ≥ªÁµ±Ê∏¨Ë©¶
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void RunAllUITests();

    /**
     * Ê∏¨Ë©¶UIÁÆ°Á??®Â?ÂßãÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIManagerInitialization();

    /**
     * Ê∏¨Ë©¶?ú‰??¢Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestRelationshipPanelFunctionality();

    /**
     * Ê∏¨Ë©¶?≤Ê??¢Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestReputationPanelFunctionality();

    /**
     * Ê∏¨Ë©¶Â∞çË©±?¢Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestDialoguePanelFunctionality();

    /**
     * Ê∏¨Ë©¶‰ªªÂ??¢Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestQuestPanelFunctionality();

    /**
     * Ê∏¨Ë©¶?≥È†ª?¢Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestAudioPanelFunctionality();

    /**
     * Ê∏¨Ë©¶‰∏ªÂ?Ë°®Êùø?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestMainDashboardFunctionality();

    /**
     * Ê∏¨Ë©¶UI?ïÁï´Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIAnimationSystem();

    /**
     * Ê∏¨Ë©¶UI‰∫ã‰ª∂Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIEventSystem();

    /**
     * Ê∏¨Ë©¶UI?øÊ??ßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIPerformance();

    /**
     * Ê∏¨Ë©¶UI?∏Ê?Á∂ÅÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIDataBinding();

    /**
     * Ê∏¨Ë©¶UI?Ä?ãÁÆ°??     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIStateManagement();

    /**
     * Ê∏¨Ë©¶UI‰∏ªÈ?Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIThemeSystem();

    /**
     * Ê∏¨Ë©¶UI?¨Âú∞??     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUILocalization();

    /**
     * Ê∏¨Ë©¶UI?ØË®™?èÊÄ?     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIAccessibility();

    /**
     * Â£ìÂ?Ê∏¨Ë©¶ - Â§ßÈ?UI?ç‰?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIStressTest();

    /**
     * ?ÜÊ?Ê∏¨Ë©¶ - ÂÆåÊï¥?ÑUIÊµÅÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIIntegration();

    /**
     * ?µÂª∫Ê∏¨Ë©¶UI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void CreateTestUIData();

    /**
     * Ê∏ÖÁ?Ê∏¨Ë©¶UI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void CleanupTestUIData();

    /**
     * È©óË?UIÁ≥ªÁµ±?•Â∫∑?Ä??     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static bool VerifyUISystemHealth();

    /**
     * ?≤Â?UIÊ∏¨Ë©¶ÁµêÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static FString GetUITestReport();

    /**
     * ?üÊ?UI?ßËÉΩ?±Â?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static FString GenerateUIPerformanceReport();

    /**
     * Ê∏¨Ë©¶UI?®Êà∂È´îÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIUserExperience();

    /**
     * Ê∏¨Ë©¶UIË∑®Âπ≥?∞ÂÖºÂÆπÊÄ?     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUICrossPlatformCompatibility();

private:
    static bool bUITestPassed;
    static FString UITestReport;
    static int32 UITestsRun;
    static int32 UITestsPassed;

    static void AddUITestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    static void ResetUITestResults();
    static void MeasureUIPerformance(const FString& OperationName, TFunction<void()> Operation);
};
