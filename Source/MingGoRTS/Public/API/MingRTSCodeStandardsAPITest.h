// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Standards API Test - 程式碼規範API測試

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCodeStandardsAPI.h"
#include "MingRTSCodeStandardsAPITest.generated.h"

USTRUCT(BlueprintType)
struct FTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    FTestResult()
    {
        TestName = TEXT("");
        bPassed = false;
        Timestamp = FDateTime::Now();
    }
};

/**
 * 程式碼規範API測試類
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCodeStandardsAPITest : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodeStandardsAPITest();

    /// 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void RunAllTests();

    /// 運行特定測試
    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestInitialization();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestNamingStandards();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestFormattingStandards();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestCodeAnalysis();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestAutoFix();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestReportGeneration();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestConfiguration();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestStatistics();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestTeamCollaboration();

    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    void TestCodeReview();

    /// 獲取測試結果
    UFUNCTION(BlueprintPure, Category = "Code Standards Test")
    TArray<FTestResult> GetTestResults() const { return TestResults; }

    /// 生成測試報告
    UFUNCTION(BlueprintCallable, Category = "Code Standards Test")
    FString GenerateTestReport();

    /// 檢查所有測試是否通過
    UFUNCTION(BlueprintPure, Category = "Code Standards Test")
    bool AreAllTestsPassed() const;

    /// 獲取通過的測試數量
    UFUNCTION(BlueprintPure, Category = "Code Standards Test")
    int32 GetPassedTestCount() const;

    /// 獲取失敗的測試數量
    UFUNCTION(BlueprintPure, Category = "Code Standards Test")
    int32 GetFailedTestCount() const;

    /// 獲取成功率
    UFUNCTION(BlueprintPure, Category = "Code Standards Test")
    float GetSuccessRate() const;

private:
    /// 測試環境管理
    void InitializeTestEnvironment();
    void CleanupTestEnvironment();
    void CreateTestFiles();
    void CleanupTestFiles();

    /// 測試記錄
    void RecordTestResult(const FString& TestName, bool bPassed);
    void OutputTestResults();

    /// 測試實例
    UPROPERTY()
    TObjectPtr<UMingRTSCodeStandardsAPI> CodeStandardsAPI;

    /// 測試結果
    UPROPERTY()
    TArray<FTestResult> TestResults;
};
