// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統集成測試執行器 - 實際測試驗證實現

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSIntegrationTestSuite.h"
#include "MingRTSIntegrationTestExecutor.generated.h"

/**
 * 系統集成測試執行器
 * 負責執行實際的系統集成測試驗證
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestExecutor : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestExecutor();

    /** 執行完整的系統集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ExecuteFullIntegrationTest();

    /** 執行基礎集成測試驗證 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteBasicIntegrationValidation();

    /** 執行數據流測試驗證 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteDataFlowValidation();

    /** 執行性能集成測試驗證 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecutePerformanceIntegrationValidation();

    /** 執行用戶體驗測試驗證 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteUserExperienceValidation();

    /** 執行兼容性測試驗證 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteCompatibilityValidation();

    /** 執行最終驗證測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteFinalValidation();

    /** 獲取測試執行結果 */
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    TArray<FString> GetTestExecutionResults() const { return TestResults; }

    /** 獲取測試執行狀態 */
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    bool IsTestExecutionCompleted() const { return bTestExecutionCompleted; }

    /** 重置測試執行狀態 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ResetTestExecutionState();

public:
    /** 測試執行開始事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestExecutionStarted);

    /** 測試執行完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestExecutionCompleted, bool, bAllTestsPassed);

    /** 測試階段完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseExecutionCompleted, FString, PhaseName, bool, bPassed);

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionStarted OnTestExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionCompleted OnTestExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestPhaseExecutionCompleted OnTestPhaseExecutionCompleted;

protected:
    /** 記錄測試結果 */
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));

    /** 驗證系統連接 */
    bool ValidateSystemConnections();

    /** 驗證數據流 */
    bool ValidateDataFlow();

    /** 驗證性能指標 */
    bool ValidatePerformanceMetrics();

    /** 驗證用戶體驗 */
    bool ValidateUserExperience();

    /** 驗證兼容性 */
    bool ValidateCompatibility();

    /** 驗證系統完整性 */
    bool ValidateSystemIntegrity();

    /** 檢查RTS戰鬥系統 */
    bool CheckRTSCombatSystem();

    /** 檢查經濟系統 */
    bool CheckEconomicSystem();

    /** 檢查UI音頻系統 */
    bool CheckUIAudioSystem();

    /** 檢查本地化系統 */
    bool CheckLocalizationSystem();

    /** 檢查性能系統 */
    bool CheckPerformanceSystem();

    /** 檢查關係系統 */
    bool CheckRelationshipSystem();

    /** 檢查AI內容生成系統 */
    bool CheckAIContentSystem();

private:
    /** 測試結果列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    TArray<FString> TestResults;

    /** 測試執行是否完成 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    bool bTestExecutionCompleted;

    /** 當前測試階段 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    FString CurrentTestPhase;

    /** 通過的測試數量 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 PassedTestCount;

    /** 失敗的測試數量 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 FailedTestCount;

    /** 測試執行開始時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    double TestExecutionStartTime;
};
