// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSprint1Test.generated.h"

// Sprint 1 测试结果
USTRUCT(BlueprintType)
struct FSprint1TestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Test")
    FString TestName;

    UPROPERTY(BlueprintReadWrite, Category = "Test")
    bool bPassed = false;

    UPROPERTY(BlueprintReadWrite, Category = "Test")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Test")
    float ExecutionTime = 0.0f;
};

UCLASS(ClassGroup = (Test), meta = (DisplayName = "Sprint 1 - RTS Core Test Suite"))
class MINGRTS_API UMingRTSSprint1Test : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSprint1Test();

    // 测试套件初始化
    UFUNCTION(BlueprintCallable, Category = "Test")
    void InitializeTestSuite();

    // 运行所有测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    TArray<FSprint1TestResult> RunAllTests();

    // 单个测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestUnitSelection();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestBoxSelection();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestMoveCommand();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestGroupManagement();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestCombatDamage();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestUnitTypeAdvantages();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestMoraleSystem();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestAreaDamage();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestAIBehaviorStates();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestAIPerception();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestPatrolSystem();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint1TestResult TestAICombatReactions();

    // 测试报告
    UFUNCTION(BlueprintPure, Category = "Test")
    FString GenerateTestReport();

    UFUNCTION(BlueprintPure, Category = "Test")
    int32 GetTotalTestCount() const { return TestResults.Num(); }

    UFUNCTION(BlueprintPure, Category = "Test")
    int32 GetPassedTestCount() const;

    UFUNCTION(BlueprintPure, Category = "Test")
    int32 GetFailedTestCount() const;

    UFUNCTION(BlueprintPure, Category = "Test")
    float GetTestPassRate() const;

protected:
    UPROPERTY()
    TArray<FSprint1TestResult> TestResults;

    // 辅助函数
    void RecordTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage = TEXT(""), float ExecutionTime = 0.0f);
    void ClearResults();

public:
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, FSprint1TestResult, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTestsCompleted);

    UPROPERTY(BlueprintAssignable, Category = "Test|Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Test|Events")
    FOnAllTestsCompleted OnAllTestsCompleted;
};
