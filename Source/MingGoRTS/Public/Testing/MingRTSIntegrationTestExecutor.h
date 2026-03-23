#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統ɥr��ɥr測試ɥr��X- 實�ɥr測試驗�ɥr實現


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Test/MingRTSIntegrationTestSuite.h"
#include "MingRTSIntegrationTestExecutor.generated.h"

/**
 * 系統ɥr��ɥr測試ɥr��X * 負責ɥr��ɥr實�X�系統�X�測試�ɥr�ɥr */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSIntegrationTestExecutor : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestExecutor(};

    /** ɥr��ɥr完整ɥr�系統�X�測�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ExecuteFullIntegrationTest(};

    /** ɥr��X��X��ɥr測試驗�ɥr */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteBasicIntegrationValidation(};

    /** ɥr��X��ɥr流測試�ɥr�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteDataFlowValidation(};

    /** ɥr��X�能ɥr��ɥr測試驗�ɥr */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecutePerformanceIntegrationValidation(};

    /** ɥr��X�戶體�ɥr測試驗�ɥr */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteUserExperienceValidation(};

    /** ɥr��X�容ɥr�測試�ɥr�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteConpatibilityValidation(};

    /** ɥr��X�終�ɥr證測�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteFinalValidation(};

    /** ɥr��ɥr測試ɥr��ɥr結�ɥr */
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    TArray<FString> GetTestExecutionResults() const { return TestResults; }

    /** ɥr��ɥr測試ɥr��X�X*/
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    bool IsTestExecutionCompleted() const { return bTestExecutionCompleted; }

    /** ɥr�m測試ɥr��X�X*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ResetTestExecutionState(};

public:
    /** 測試ɥr��X��ɥr事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestExecutionStarted};

    /** 測試ɥr��ɥr完�ɥr事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestExecutionCompleted, bool, bAllTestsPassed};

    /** 測試ɥr�段完�ɥr事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseExecutionCompleted, FString, PhaseName, bool, bPassed};

    UPROPERTY(BlueprintAssignable, Category = "Integration Test ExecutorEvents")
    FOnTestExecutionStarted OnTestExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test ExecutorEvents")
    FOnTestExecutionCompleted OnTestExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test ExecutorEvents")
    FOnTestPhaseExecutionCompleted OnTestPhaseExecutionCompleted;

protected:
    /** 記�ɥr測試結�ɥr */
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};

    /** 驗�ɥr系統X�� */
    bool ValidateSystemConnections(};

    /** 驗�X��ɥr�ɥr*/
    bool ValidateDataFlow(};

    /** 驗�X�能ɥr��ɥr */
    bool ValidatePerformanceMetrics(};

    /** 驗�X�戶體�ɥr */
    bool ValidateUserExperience(};

    /** 驗�X�容X*/
    bool ValidateConpatibility(};

    /** 驗�ɥr系統完整X*/
    bool ValidateSystemIntegrity(};

    /** 檢查RTSɥr�鬥系統 */
    bool CheckRTSConbatSystem(};

    /** 檢查經�ɥr系統 */
    bool CheckEconomicSystem(};

    /** 檢查UIɥr�頻系統 */
    bool CheckUIAudioSystem(};

    /** 檢查ɥr�地ɥr�系�ɥr*/
    bool CheckLocalizationSystem(};

    /** 檢查ɥr�能系統 */
    bool CheckPerformanceSystem(};

    /** 檢查ɥr��ɥr系統 */
    bool CheckRelationshipSystem(};

    /** 檢查AIɥr�容ɥr��ɥr系統 */
    bool CheckAIContentSystem(};

private:
    /** 測試結�X�表 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    TArray<FString> TestResults;

    /** 測試ɥr��X�否完�ɥr */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    bool bTestExecutionCompleted;

    /** ɥr��ɥr測試ɥr�段 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    FString CurrentTestPhase;

    /** ɥr��X�測試數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 PassedTestCount;

    /** 失�X�測試數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 FailedTestCount;

    /** 測試ɥr��X��X��ɥr */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    double TestExecutionStartTime;
};

