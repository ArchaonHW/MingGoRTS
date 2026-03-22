// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統?��?測試?��X- 實�?測試驗�?實現

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Test/MingRTSIntegrationTestSuite.h"
#include "MingRTSIntegrationTestExecutor.generated.h"

/**
 * 系統?��?測試?��X * 負責?��?實�X�系統�X�測試�?�? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestExecutor : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestExecutor();

    /** ?��?完整?�系統�X�測�?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ExecuteFullIntegrationTest();

    /** ?��X��X��?測試驗�? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteBasicIntegrationValidation();

    /** ?��X��?流測試�?�?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteDataFlowValidation();

    /** ?��X�能?��?測試驗�? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecutePerformanceIntegrationValidation();

    /** ?��X�戶體�?測試驗�? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteUserExperienceValidation();

    /** ?��X�容?�測試�?�?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteCompatibilityValidation();

    /** ?��X�終�?證測�?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteFinalValidation();

    /** ?��?測試?��?結�? */
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    TArray<FString> GetTestExecutionResults() const { return TestResults; }

    /** ?��?測試?��X�X*/
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    bool IsTestExecutionCompleted() const { return bTestExecutionCompleted; }

    /** ?�置測試?��X�X*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ResetTestExecutionState();

public:
    /** 測試?��X��?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestExecutionStarted);

    /** 測試?��?完�?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestExecutionCompleted, bool, bAllTestsPassed);

    /** 測試?�段完�?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseExecutionCompleted, FString, PhaseName, bool, bPassed);

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionStarted OnTestExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionCompleted OnTestExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestPhaseExecutionCompleted OnTestPhaseExecutionCompleted;

protected:
    /** 記�?測試結�? */
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};

    /** 驗�?系統X�� */
    bool ValidateSystemConnections();

    /** 驗�X��?�?*/
    bool ValidateDataFlow();

    /** 驗�X�能?��? */
    bool ValidatePerformanceMetrics();

    /** 驗�X�戶體�? */
    bool ValidateUserExperience();

    /** 驗�X�容X*/
    bool ValidateCompatibility();

    /** 驗�?系統完整X*/
    bool ValidateSystemIntegrity();

    /** 檢查RTS?�鬥系統 */
    bool CheckRTSCombatSystem();

    /** 檢查經�?系統 */
    bool CheckEconomicSystem();

    /** 檢查UI?�頻系統 */
    bool CheckUIAudioSystem();

    /** 檢查?�地?�系�?*/
    bool CheckLocalizationSystem();

    /** 檢查?�能系統 */
    bool CheckPerformanceSystem();

    /** 檢查?��?系統 */
    bool CheckRelationshipSystem();

    /** 檢查AI?�容?��?系統 */
    bool CheckAIContentSystem();

private:
    /** 測試結�X�表 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    TArray<FString> TestResults;

    /** 測試?��X�否完�? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    bool bTestExecutionCompleted;

    /** ?��?測試?�段 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    FString CurrentTestPhase;

    /** ?��X�測試數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 PassedTestCount;

    /** 失�X�測試數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 FailedTestCount;

    /** 測試?��X��X��? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    double TestExecutionStartTime;
};

