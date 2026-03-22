#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUISystemTest.generated.h"

/**
 * UI系統測試運行器
 * 測試個人系統UI的功能和性能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingUISystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 運行所有UI系統測試
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void RunAllUITests();

    /**
     * 測試UI管理器初始化
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIManagerInitialization();

    /**
     * 測試關係面板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestRelationshipPanelFunctionality();

    /**
     * 測試聲望面板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestReputationPanelFunctionality();

    /**
     * 測試對話面板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestDialoguePanelFunctionality();

    /**
     * 測試任務面板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestQuestPanelFunctionality();

    /**
     * 測試音頻面板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestAudioPanelFunctionality();

    /**
     * 測試主儀表板功能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestMainDashboardFunctionality();

    /**
     * 測試UI動畫系統
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIAnimationSystem();

    /**
     * 測試UI事件系統
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIEventSystem();

    /**
     * 測試UI響應性能
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIPerformance();

    /**
     * 測試UI數據綁定
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIDataBinding();

    /**
     * 測試UI狀態管理
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIStateManagement();

    /**
     * 測試UI主題系統
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIThemeSystem();

    /**
     * 測試UI本地化
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUILocalization();

    /**
     * 測試UI可訪問性
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIAccessibility();

    /**
     * 壓力測試 - 大量UI操作
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIStressTest();

    /**
     * 集成測試 - 完整的UI流程
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIIntegration();

    /**
     * 創建測試UI數據
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void CreateTestUIData();

    /**
     * 清理測試UI數據
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static void CleanupTestUIData();

    /**
     * 驗證UI系統健康狀態
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static bool VerifyUISystemHealth();

    /**
     * 獲取UI測試結果報告
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static FString GetUITestReport();

    /**
     * 生成UI性能報告
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Test")
    static FString GenerateUIPerformanceReport();

    /**
     * 測試UI用戶體驗
     */
    UFUNCTION(BlueprintCallable, Category = "UI Test")
    static bool TestUIUserExperience();

    /**
     * 測試UI跨平台兼容性
     */
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
