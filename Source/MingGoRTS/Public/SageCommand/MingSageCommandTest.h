#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingThreePowerSystem.h"
#include "MingMoralAuthority.h"
#include "MingStrategyAuthority.h"
#include "MingMilitaryAuthority.h"
#include "MingSageCommandTest.generated.h"

/**
 * 至聖者指揮學系統集成測試
 * 用於驗證三權系統的基本功能和集成
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Test")
class MINGGORTS_API UMingSageCommandTest : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 測試初始化
    // ============================================================================

    UMingSageCommandTest();

    /** 初始化測試環境 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool InitializeTestEnvironment();

    /** 重置測試環境 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    void ResetTestEnvironment();

    // ============================================================================
    // 三權系統測試
    // ============================================================================

    /** 測試三權系統初始化 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestThreePowerSystemInitialization();

    /** 測試權力分配功能 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestPowerAllocation();

    /** 測試權力平衡機制 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestPowerBalancing();

    /** 測試權力轉移系統 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestPowerTransfer();

    // ============================================================================
    // 道德權威測試
    // ============================================================================

    /** 測試道德權威初始化 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestMoralAuthorityInitialization();

    /** 測試墮落徵象檢測 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestCorruptionDetection();

    /** 測試道德邊界檢查 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestMoralBoundaryCheck();

    // ============================================================================
    // 策略權威測試
    // ============================================================================

    /** 測試策略權威初始化 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestStrategyAuthorityInitialization();

    /** 測試策略執行 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestStrategyExecution();

    /** 測試陰陽切換 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestYinYangSwitching();

    /** 測試五行節奏控制 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestWuXingRhythmControl();

    // ============================================================================
    // 軍事權威測試
    // ============================================================================

    /** 測試軍事權威初始化 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestMilitaryAuthorityInitialization();

    /** 測試命令發布 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestCommandIssuing();

    /** 測試命令執行 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestCommandExecution();

    /** 測試後果承擔機制 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestConsequenceBearing();

    // ============================================================================
    // 集成測試
    // ============================================================================

    /** 完整系統集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestFullSystemIntegration();

    /** 測試三權協調 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestThreePowerCoordination();

    /** 測試策略-軍事協作 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    bool TestStrategyMilitaryCoordination();

    // ============================================================================
    // 測試結果
    // ============================================================================

    /** 獲取測試結果 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    TArray<FString> GetTestResults() const;

    /** 獲取測試統計 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    void GetTestStatistics(int32& TotalTests, int32& PassedTests, int32& FailedTests) const;

    /** 清除測試結果 */
    UFUNCTION(BlueprintCallable, Category = "Sage Command Test")
    void ClearTestResults();

protected:
    // ============================================================================
    // 內部組件
    // ============================================================================

    /** 三權系統 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Components")
    TObjectPtr<UMingThreePowerSystem> ThreePowerSystem;

    /** 道德權威 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Components")
    TObjectPtr<UMingMoralAuthority> MoralAuthority;

    /** 策略權威 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Components")
    TObjectPtr<UMingStrategyAuthority> StrategyAuthority;

    /** 軍事權威 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Components")
    TObjectPtr<UMingMilitaryAuthority> MilitaryAuthority;

    // ============================================================================
    // 測試數據
    // ============================================================================

    /** 測試結果列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Data")
    TArray<FString> TestResults;

    /** 測試統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Data")
    int32 TotalTestCount;

    UPROPERTY(BlueprintReadOnly, Category = "Test Data")
    int32 PassedTestCount;

    UPROPERTY(BlueprintReadOnly, Category = "Test Data")
    int32 FailedTestCount;

    /** 測試狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Test Data")
    bool bTestEnvironmentInitialized;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 記錄測試結果 */
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    /** 創建測試組件 */
    bool CreateTestComponents();

    /** 驗證組件初始化 */
    bool ValidateComponentInitialization();

    /** 模擬基本場景 */
    void SimulateBasicScenario();

    /** 生成測試報告 */
    FString GenerateTestReport() const;
};
