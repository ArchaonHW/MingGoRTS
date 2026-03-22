// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統集成測試階段啟動 - 基礎集成測試

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Templates/Function.h"
#include "MingRTSIntegrationTestSuite.generated.h"

/** 測試階段枚舉 */
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

/** 測試結果枚舉 */
UENUM(BlueprintType)
enum class ETestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error
};

/** 測試用例結構 */
USTRUCT(BlueprintType)
struct FTestCase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    FString TestCaseName;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    ETestResult Result;

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

/** 測試階段結果 */
USTRUCT(BlueprintType)
struct FPhaseResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    ETestPhase Phase;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    ETestResult OverallResult;

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
 * 系統集成測試階段管理器
 * 負責協調和執行所有系統的集成測試
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntegrationTestSuite();

    /** 初始化集成測試套件 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void InitializeIntegrationTestSuite();

    /** 開始完整集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFullIntegrationTest();

    /** 開始基礎集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartBasicIntegrationTest();

    /** 開始數據流測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartDataFlowTest();

    /** 開始性能集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartPerformanceIntegrationTest();

    /** 開始用戶體驗測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartUserExperienceTest();

    /** 開始兼容性測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartCompatibilityTest();

    /** 開始最終驗證測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartFinalValidationTest();

    /** 獲取當前測試階段 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    ETestPhase GetCurrentTestPhase() const { return CurrentPhase; }

    /** 獲取測試進度 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    float GetTestProgress() const;

    /** 獲取測試結果報告 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    FString GenerateTestReport();

    /** 獲取階段結果 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    TArray<FPhaseResult> GetAllPhaseResults() const { return PhaseResults; }

    /** 檢查測試是否完成 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    bool IsTestCompleted() const { return CurrentPhase == ETestPhase::Completed; }

    /** 重置測試狀態 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void ResetTestState();

public:
    /** 測試階段開始事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestPhaseStarted, ETestPhase, Phase);

    /** 測試階段完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestPhaseCompleted, ETestPhase, Phase, ETestResult, Result);

    /** 測試用例完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTestCaseCompleted, FString, TestCaseName, ETestResult, Result, float, ExecutionTime);

    /** 完整測試完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFullTestCompleted, bool, bAllPassed);

    /** 測試進度更新事件 */
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
    /** 執行測試用例 */
    void ExecuteTestCase(const FString& TestCaseName, TFunction<void()> TestFunction);

    /** 更新測試進度 */
    void UpdateTestProgress();

    /** 記錄測試結果 */
    void RecordTestResult(const FString& TestCaseName, ETestResult Result, const FString& ErrorMessage = TEXT(""));

    /** 完成當前階段 */
    void CompleteCurrentPhase(ETestResult Result);

    /** 移動到下一階段 */
    void MoveToNextPhase();

private:
    /** 當前測試階段 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    ETestPhase CurrentPhase;

    /** 所有階段結果 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FPhaseResult> PhaseResults;

    /** 當前階段的測試用例 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    TArray<FTestCase> CurrentPhaseTestCases;

    /** 總測試用例數 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 TotalTestCases;

    /** 已完成測試用例數 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    int32 CompletedTestCases;

    /** 測試開始時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double TestStartTime;

    /** 當前測試用例開始時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    double CurrentTestCaseStartTime;

    /** 是否正在運行測試 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test", meta = (AllowPrivateAccess = "true"))
    bool bIsTestRunning;

    /** 初始化基礎集成測試用例 */
    void InitializeBasicIntegrationTests();

    /** 初始化數據流測試用例 */
    void InitializeDataFlowTests();

    /** 初始化性能集成測試用例 */
    void InitializePerformanceIntegrationTests();

    /** 初始化用戶體驗測試用例 */
    void InitializeUserExperienceTests();

    /** 初始化兼容性測試用例 */
    void InitializeCompatibilityTests();

    /** 初始化最終驗證測試用例 */
    void InitializeFinalValidationTests();

    /** 基礎集成測試具體實現 */
    void RunBasicIntegrationTests();

    /** 數據流測試具體實現 */
    void RunDataFlowTests();

    /** 性能集成測試具體實現 */
    void RunPerformanceIntegrationTests();

    /** 用戶體驗測試具體實現 */
    void RunUserExperienceTests();

    /** 兼容性測試具體實現 */
    void RunCompatibilityTests();

    /** 最終驗證測試具體實現 */
    void RunFinalValidationTests();
};
