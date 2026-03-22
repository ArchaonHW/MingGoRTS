// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統????測試??段???? - ???X???測試

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Templates/Function.h"
#include "MingRTSIntegrationTestSuite.generated.h"

/** 測試??段???? */
UENUM(BlueprintType)
enum class ETestPhase : uint8
{
    NotStarted,
    BasicIntegration,
    DataFlow,
    Performance,
    UserExperience,
    Compatibility,
    FinalValidation,
    Completed
};

/** 測試結?X??? */
UENUM(BlueprintType)
enum class EIntegrationTestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error
};

/** 測試????結?? */
USTRUCT(BlueprintType)
struct FTestCase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    FString TestCaseName;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    EIntegrationTestResult Result;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    float ExecutionTime;

    FTestCase()
        : TestCaseName(TEXT(""))
        , Description(TEXT(""))
        , Result(ETestResult::NotRun)
        , ErrorMessage(TEXT(""))
        , ExecutionTime(0.0f)
    {
    }
};

/** 測試??段結?? */
USTRUCT(BlueprintType)
struct FPhaseResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    ETestPhase Phase;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    EIntegrationTestResult OverallResult;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    TArray<FTestCase> TestCases;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    int32 PassedCount;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    int32 FailedCount;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    int32 SkippedCount;

    FPhaseResult()
        : Phase(ETestPhase::NotStarted)
        , OverallResult(ETestResult::NotRun)
        , TotalExecutionTime(0.0f)
        , PassedCount(0)
        , FailedCount(0)
        , SkippedCount(0)
    {
    }
};

/**
 * 系統????測試??段管?X * 負責??調??執行?X?系統?X???測試
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestSuite(};

    /** ???X??X?測試????*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void InitializeIntegrationTestSuite(};

    /** ????完整????測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFullIntegrationTest(};

    /** ???X??X???測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartBasicIntegrationTest(};

    /** ???X???流測??*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartDataFlowTest(};

    /** ???X?能????測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartPerformanceIntegrationTest(};

    /** ???X?戶體??測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartUserExperienceTest(};

    /** ???X?容??測??*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartCompatibilityTest(};

    /** ???X?終??證測??*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFinalValidationTest(};

    /** ???X???測試??段 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    ETestPhase GetCurrentTestPhase() const { return CurrentPhase; }

    /** ????測試??度 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    float GetTestProgress() const;

    /** ????測試結?X??? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    FString GenerateTestReport(};

    /** ???X?段結?? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    TArray<FPhaseResult> GetAllPhaseResults() const { return PhaseResults; }

    /** 檢查測試??否完?? */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    bool IsTestCompleted() const { return CurrentPhase == ETestPhase::Completed; }

    /** ??置測試??X*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void ResetTestState(};

public:
    /** 測試??段????事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestPhaseStarted, ETestPhase, Phase};

    /** 測試??段完??事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseCompleted, ETestPhase, Phase, ETestResult, Result};

    /** 測試????完??事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTestCaseCompleted, FString, TestCaseName, ETestResult, Result, float, ExecutionTime};

    /** 完整測試完??事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFullTestCompleted, bool, bAllPassed};

    /** 測試??度??新事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestProgressUpdated, float, Progress};

    UPROPERTY(BlueprintAssignable, Category = "Integration Test|Events")
    FOnTestPhaseStarted OnTestPhaseStarted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test|Events")
    FOnTestPhaseCompleted OnTestPhaseCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test|Events")
    FOnTestCaseCompleted OnTestCaseCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test|Events")
    FOnFullTestCompleted OnFullTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Integration Test|Events")
    FOnTestProgressUpdated OnTestProgressUpdated;

protected:
    /** ????測試???? */
    void ExecuteTestCase(const FString& TestCaseName, TFunction<void()> TestFunction};

    /** ??新測試??度 */
    void UpdateTestProgress(};

    /** 記??測試結?? */
    void RecordTestResult(const FString& TestCaseName, ETestResult Result, const FString& ErrorMessage = TEXT("")};

    /** 完?X??X?段 */
    void CompleteCurrentPhase(ETestResult Result};

    /** 移?X???一??段 */
    void MoveToNextPhase(};

private:
    /** ????測試??段 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    ETestPhase CurrentPhase;

    /** ??????段?X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FPhaseResult> PhaseResults;

    /** ???X?段??測試用??*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FTestCase> CurrentPhaseTestCases;

    /** 總測試用例數 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 TotalTestCases;

    /** 已?X?測試用例數 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 CompletedTestCases;

    /** 測試???X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double TestStartTime;

    /** ????測試???X??X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double CurrentTestCaseStartTime;

    /** ??否????????測試 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    bool bIsTestRunning;

    /** ???X?基礎?X?測試用??*/
    void InitializeBasicIntegrationTests(};

    /** ???X?數????測試???? */
    void InitializeDataFlowTests(};

    /** ???X?性能????測試???? */
    void InitializePerformanceIntegrationTests(};

    /** ???X?用????驗測試用??*/
    void InitializeUserExperienceTests(};

    /** ???X?兼容性測試用??*/
    void InitializeCompatibilityTests(};

    /** ???X???終??證測試用??*/
    void InitializeFinalValidationTests(};

    /** ???X???測試????實現 */
    void RunBasicIntegrationTests(};

    /** ????流測試具體實X*/
    void RunDataFlowTests(};

    /** ??能????測試????實現 */
    void RunPerformanceIntegrationTests(};

    /** ??戶體??測試????實現 */
    void RunUserExperienceTests(};

    /** ??容??測試具體實X*/
    void RunCompatibilityTests(};

    /** ??終??證測試具體實X*/
    void RunFinalValidationTests(};
};