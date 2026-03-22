#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionTest();

    // 生產系統實例
    UPROPERTY(BlueprintReadOnly, Category = "Production Test")
    UMingGoRTSProductionSystem* ProductionSystem;

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void InitializeTest();

    // 測試生產系統初始化
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionSystemInitialization();

    // 測試生產線創建
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineCreation();

    // 測試生產配方創建
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionRecipeCreation();

    // 測試生產流程
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionFlow();

    // 測試生產隊列管理
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement();

    // 測試生產效率計算
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation();

    // 測試生產暫停恢復
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume();

    // 測試生產優先級
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority();

    // 測試生產線升級
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade();

    // 測試自動化功能
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestAutomationFeatures();

    // 測試民國時期特色配方
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestRepublicanEraRecipes();

    // 測試生產統計
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionStatistics();

    // 測試生產配置
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionConfiguration();

    // 測試生產保存載入
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestSaveAndLoad();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests();

    // 獲取測試結果
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // 清理測試
    UFUNCTION(BlueprintCallable, Category = "Production Test")
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

    // 創建測試生產線
    FProductionLine CreateTestProductionLine(const FString& LineName, const FString& BuildingID);

    // 創建測試配方
    FProductionRecipe CreateTestRecipe(const FString& RecipeName, EProductionCategory Category);

    // 驗證生產線數據
    bool VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName);

    // 驗證配方數據
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory);

    // 驗證生產狀態
    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus);

    // 獲取隨機配方ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // 重置測試計數器
    void ResetTestCounters();

    // 創建測試配置
    FProductionSystemConfig CreateTestConfig();

    // 驗證配置應用
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig);

    // 等待生產完成
    bool WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f);

    // 模擬時間流逝
    void SimulateTimeProgress(float DeltaTime);

    // 驗證資源變化
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges);
};
