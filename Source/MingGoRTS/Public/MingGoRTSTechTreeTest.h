#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTreeTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeTest();

    // 科技樹系統實例
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree Test")
    UMingGoRTSTechTree* TechTreeSystem;

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void InitializeTest();

    // 測試科技樹初始化
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeInitialization();

    // 測試科技節點創建
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechNodeCreation();

    // 測試科技研發流程
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechResearchFlow();

    // 測試前置條件檢查
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestPrerequisiteChecking();

    // 測試科技效果應用
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechEffectApplication();

    // 測試科技解鎖機制
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechUnlocking();

    // 測試並行研發限制
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestConcurrentResearchLimit();

    // 測試科技樹配置
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeConfiguration();

    // 測試科技樹保存和載入
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestSaveAndLoad();

    // 測試民國時期特色科技
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestRepublicanEraTechs();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool RunAllTests();

    // 獲取測試結果
    UFUNCTION(BlueprintPure, Category = "Tech Tree Test")
    FString GetTestResults() const;

    // 清理測試
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void CleanupTest();

private:
    // 測試結果
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試通過數量
    int32 PassedTests;

    // 測試失敗數量
    int32 FailedTests;

    // 添加測試結果
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // 創建測試科技節點
    FTechNode CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category);

    // 驗證科技節點數據
    bool VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory);

    // 驗證研發狀態
    bool VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus);

    // 獲取隨機科技ID
    FString GetRandomTechID(ETechCategory Category) const;

    // 重置測試計數器
    void ResetTestCounters();

    // 創建測試配置
    FTechTreeConfig CreateTestConfig();

    // 驗證配置應用
    bool VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig);
};
