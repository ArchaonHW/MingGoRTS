// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶?∑Ë???- ÂØ¶È?Ê∏¨Ë©¶È©óË?ÂØ¶Áèæ

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSIntegrationTestSuite.h"
#include "MingRTSIntegrationTestExecutor.generated.h"

/**
 * Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶?∑Ë??? * Ë≤†Ë≤¨?∑Ë?ÂØ¶È??ÑÁ≥ªÁµ±È??êÊ∏¨Ë©¶È?Ë≠? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestExecutor : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestExecutor();

    /** ?∑Ë?ÂÆåÊï¥?ÑÁ≥ªÁµ±È??êÊ∏¨Ë©?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ExecuteFullIntegrationTest();

    /** ?∑Ë??∫Á??ÜÊ?Ê∏¨Ë©¶È©óË? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteBasicIntegrationValidation();

    /** ?∑Ë??∏Ê?ÊµÅÊ∏¨Ë©¶È?Ë≠?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteDataFlowValidation();

    /** ?∑Ë??ßËÉΩ?ÜÊ?Ê∏¨Ë©¶È©óË? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecutePerformanceIntegrationValidation();

    /** ?∑Ë??®Êà∂È´îÈ?Ê∏¨Ë©¶È©óË? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteUserExperienceValidation();

    /** ?∑Ë??ºÂÆπ?ßÊ∏¨Ë©¶È?Ë≠?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteCompatibilityValidation();

    /** ?∑Ë??ÄÁµÇÈ?Ë≠âÊ∏¨Ë©?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    bool ExecuteFinalValidation();

    /** ?≤Â?Ê∏¨Ë©¶?∑Ë?ÁµêÊ? */
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    TArray<FString> GetTestExecutionResults() const { return TestResults; }

    /** ?≤Â?Ê∏¨Ë©¶?∑Ë??Ä??*/
    UFUNCTION(BlueprintPure, Category = "Integration Test Executor")
    bool IsTestExecutionCompleted() const { return bTestExecutionCompleted; }

    /** ?çÁΩÆÊ∏¨Ë©¶?∑Ë??Ä??*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test Executor")
    void ResetTestExecutionState();

public:
    /** Ê∏¨Ë©¶?∑Ë??ãÂ?‰∫ã‰ª∂ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestExecutionStarted);

    /** Ê∏¨Ë©¶?∑Ë?ÂÆåÊ?‰∫ã‰ª∂ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestExecutionCompleted, bool, bAllTestsPassed);

    /** Ê∏¨Ë©¶?éÊÆµÂÆåÊ?‰∫ã‰ª∂ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseExecutionCompleted, FString, PhaseName, bool, bPassed);

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionStarted OnTestExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestExecutionCompleted OnTestExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test Executor|Events")
    FOnTestPhaseExecutionCompleted OnTestPhaseExecutionCompleted;

protected:
    /** Ë®òÈ?Ê∏¨Ë©¶ÁµêÊ? */
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));

    /** È©óË?Á≥ªÁµ±??é• */
    bool ValidateSystemConnections();

    /** È©óË??∏Ê?Êµ?*/
    bool ValidateDataFlow();

    /** È©óË??ßËÉΩ?áÊ? */
    bool ValidatePerformanceMetrics();

    /** È©óË??®Êà∂È´îÈ? */
    bool ValidateUserExperience();

    /** È©óË??ºÂÆπ??*/
    bool ValidateCompatibility();

    /** È©óË?Á≥ªÁµ±ÂÆåÊï¥??*/
    bool ValidateSystemIntegrity();

    /** Ê™¢Êü•RTS?∞È¨•Á≥ªÁµ± */
    bool CheckRTSCombatSystem();

    /** Ê™¢Êü•Á∂ìÊ?Á≥ªÁµ± */
    bool CheckEconomicSystem();

    /** Ê™¢Êü•UI?≥È†ªÁ≥ªÁµ± */
    bool CheckUIAudioSystem();

    /** Ê™¢Êü•?¨Âú∞?ñÁ≥ªÁµ?*/
    bool CheckLocalizationSystem();

    /** Ê™¢Êü•?ßËÉΩÁ≥ªÁµ± */
    bool CheckPerformanceSystem();

    /** Ê™¢Êü•?ú‰?Á≥ªÁµ± */
    bool CheckRelationshipSystem();

    /** Ê™¢Êü•AI?ßÂÆπ?üÊ?Á≥ªÁµ± */
    bool CheckAIContentSystem();

private:
    /** Ê∏¨Ë©¶ÁµêÊ??óË°® */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    TArray<FString> TestResults;

    /** Ê∏¨Ë©¶?∑Ë??ØÂê¶ÂÆåÊ? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    bool bTestExecutionCompleted;

    /** ?∂Â?Ê∏¨Ë©¶?éÊÆµ */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    FString CurrentTestPhase;

    /** ?öÈ??ÑÊ∏¨Ë©¶Êï∏??*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 PassedTestCount;

    /** Â§±Ê??ÑÊ∏¨Ë©¶Êï∏??*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    int32 FailedTestCount;

    /** Ê∏¨Ë©¶?∑Ë??ãÂ??ÇÈ? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test Executor", meta = (AllowPrivateAccess = "true"))
    double TestExecutionStartTime;
};
