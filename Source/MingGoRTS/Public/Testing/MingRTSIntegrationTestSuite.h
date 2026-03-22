// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ç³»çµ±?†æ?æ¸¬è©¦?æ®µ?Ÿå? - ?ºç??†æ?æ¸¬è©¦

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Templates/Function.h"
#include "MingRTSIntegrationTestSuite.generated.h"

/** æ¸¬è©¦?æ®µ?šè? */
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

/** æ¸¬è©¦çµæ??šè? */
UENUM(BlueprintType)
enum class EIntegrationTestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error
};

/** æ¸¬è©¦?¨ä?çµæ? */
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

/** æ¸¬è©¦?æ®µçµæ? */
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
 * ç³»çµ±?†æ?æ¸¬è©¦?æ®µç®¡ç??? * è² è²¬?”èª¿?ŒåŸ·è¡Œæ??‰ç³»çµ±ç??†æ?æ¸¬è©¦
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestSuite();

    /** ?å??–é??æ¸¬è©¦å?ä»?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void InitializeIntegrationTestSuite();

    /** ?‹å?å®Œæ•´?†æ?æ¸¬è©¦ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFullIntegrationTest();

    /** ?‹å??ºç??†æ?æ¸¬è©¦ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartBasicIntegrationTest();

    /** ?‹å??¸æ?æµæ¸¬è©?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartDataFlowTest();

    /** ?‹å??§èƒ½?†æ?æ¸¬è©¦ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartPerformanceIntegrationTest();

    /** ?‹å??¨æˆ¶é«”é?æ¸¬è©¦ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartUserExperienceTest();

    /** ?‹å??¼å®¹?§æ¸¬è©?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartCompatibilityTest();

    /** ?‹å??€çµ‚é?è­‰æ¸¬è©?*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFinalValidationTest();

    /** ?²å??¶å?æ¸¬è©¦?æ®µ */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    ETestPhase GetCurrentTestPhase() const { return CurrentPhase; }

    /** ?²å?æ¸¬è©¦?²åº¦ */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    float GetTestProgress() const;

    /** ?²å?æ¸¬è©¦çµæ??±å? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    FString GenerateTestReport();

    /** ?²å??æ®µçµæ? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    TArray<FPhaseResult> GetAllPhaseResults() const { return PhaseResults; }

    /** æª¢æŸ¥æ¸¬è©¦?¯å¦å®Œæ? */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    bool IsTestCompleted() const { return CurrentPhase == ETestPhase::Completed; }

    /** ?ç½®æ¸¬è©¦?€??*/
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void ResetTestState();

public:
    /** æ¸¬è©¦?æ®µ?‹å?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestPhaseStarted, ETestPhase, Phase);

    /** æ¸¬è©¦?æ®µå®Œæ?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseCompleted, ETestPhase, Phase, ETestResult, Result);

    /** æ¸¬è©¦?¨ä?å®Œæ?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTestCaseCompleted, FString, TestCaseName, ETestResult, Result, float, ExecutionTime);

    /** å®Œæ•´æ¸¬è©¦å®Œæ?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFullTestCompleted, bool, bAllPassed);

    /** æ¸¬è©¦?²åº¦?´æ–°äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestProgressUpdated, float, Progress);

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
    /** ?·è?æ¸¬è©¦?¨ä? */
    void ExecuteTestCase(const FString& TestCaseName, TFunction<void()> TestFunction);

    /** ?´æ–°æ¸¬è©¦?²åº¦ */
    void UpdateTestProgress();

    /** è¨˜é?æ¸¬è©¦çµæ? */
    void RecordTestResult(const FString& TestCaseName, ETestResult Result, const FString& ErrorMessage = TEXT(""));

    /** å®Œæ??¶å??æ®µ */
    void CompleteCurrentPhase(ETestResult Result);

    /** ç§»å??°ä?ä¸€?æ®µ */
    void MoveToNextPhase();

private:
    /** ?¶å?æ¸¬è©¦?æ®µ */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    ETestPhase CurrentPhase;

    /** ?€?‰é?æ®µç???*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FPhaseResult> PhaseResults;

    /** ?¶å??æ®µ?„æ¸¬è©¦ç”¨ä¾?*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FTestCase> CurrentPhaseTestCases;

    /** ç¸½æ¸¬è©¦ç”¨ä¾‹æ•¸ */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 TotalTestCases;

    /** å·²å??æ¸¬è©¦ç”¨ä¾‹æ•¸ */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 CompletedTestCases;

    /** æ¸¬è©¦?‹å??‚é? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double TestStartTime;

    /** ?¶å?æ¸¬è©¦?¨ä??‹å??‚é? */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double CurrentTestCaseStartTime;

    /** ?¯å¦æ­?œ¨?‹è?æ¸¬è©¦ */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    bool bIsTestRunning;

    /** ?å??–åŸºç¤é??æ¸¬è©¦ç”¨ä¾?*/
    void InitializeBasicIntegrationTests();

    /** ?å??–æ•¸?šæ?æ¸¬è©¦?¨ä? */
    void InitializeDataFlowTests();

    /** ?å??–æ€§èƒ½?†æ?æ¸¬è©¦?¨ä? */
    void InitializePerformanceIntegrationTests();

    /** ?å??–ç”¨?¶é?é©—æ¸¬è©¦ç”¨ä¾?*/
    void InitializeUserExperienceTests();

    /** ?å??–å…¼å®¹æ€§æ¸¬è©¦ç”¨ä¾?*/
    void InitializeCompatibilityTests();

    /** ?å??–æ?çµ‚é?è­‰æ¸¬è©¦ç”¨ä¾?*/
    void InitializeFinalValidationTests();

    /** ?ºç??†æ?æ¸¬è©¦?·é?å¯¦ç¾ */
    void RunBasicIntegrationTests();

    /** ?¸æ?æµæ¸¬è©¦å…·é«”å¯¦??*/
    void RunDataFlowTests();

    /** ?§èƒ½?†æ?æ¸¬è©¦?·é?å¯¦ç¾ */
    void RunPerformanceIntegrationTests();

    /** ?¨æˆ¶é«”é?æ¸¬è©¦?·é?å¯¦ç¾ */
    void RunUserExperienceTests();

    /** ?¼å®¹?§æ¸¬è©¦å…·é«”å¯¦??*/
    void RunCompatibilityTests();

    /** ?€çµ‚é?è­‰æ¸¬è©¦å…·é«”å¯¦??*/
    void RunFinalValidationTests();
};
