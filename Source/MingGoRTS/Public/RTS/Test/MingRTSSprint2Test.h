// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSprint2Test.generated.h"

// Sprint 2 测试结果
USTRUCT(BlueprintType)
struct FSprint2TestResult
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

UCLASS(ClassGroup = (Test), meta = (DisplayName = "Sprint 2 - Resource Building Economy Test Suite"))
class MINGGORTS_API UMingRTSSprint2Test : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSprint2Test();

    // 测试套件初始化
    UFUNCTION(BlueprintCallable, Category = "Test")
    void InitializeTestSuite();

    // 运行所有测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    TArray<FSprint2TestResult> RunAllTests();

    // 资源系统测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceNodeRegistration();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceGathering();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceInventory();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceCapacity();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceTrading();

    // 建筑系统测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingPlanning();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingConstruction();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingUpgrade();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingRepair();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingProduction();

    // 经济集成测试
    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestResourceToBuildingIntegration();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestEconomyCycle();

    UFUNCTION(BlueprintCallable, Category = "Test")
    FSprint2TestResult TestBuildingResourceGeneration();

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
    TArray<FSprint2TestResult> TestResults;

    void RecordTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage = TEXT(""), float ExecutionTime = 0.0f);
    void ClearResults();

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, FSprint2TestResult, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTestsCompleted);

    UPROPERTY(BlueprintAssignable, Category = "Test|Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Test|Events")
    FOnAllTestsCompleted OnAllTestsCompleted;
};
