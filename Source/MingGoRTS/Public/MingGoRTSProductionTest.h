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
    UMingGoRTSProductionTest(};

    // ?�產系統實�?
    UPROPERTY(BlueprintReadOnly, Category = "Production Test")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ?��X�測�?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void InitializeTest(};

    // 測試?�產系統?��X    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionSystemInitialization(};

    // 測試?�產線創�?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineCreation(};

    // 測試?�產?�方?�建
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionRecipeCreation(};

    // 測試?�產流�?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionFlow(};

    // 測試?�產?��?管�?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement(};

    // 測試?�產?��?計�?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation(};

    // 測試?�產?��X�復
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume(};

    // 測試?�產?��?�?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority(};

    // 測試?�產線�?�?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade(};

    // 測試?��X��X    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestAutomationFeatures(};

    // 測試民�X��X�色?�方
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestRepublicanEraRecipes(};

    // 測試?�產統�?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionStatistics(};

    // 測試?�產?�置
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionConfiguration(};

    // 測試?�產保�?載入
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestSaveAndLoad(};

    // ?��X�?�測�?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests(};

    // ?��?測試結�?
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // 清�?測試
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void CleanupTest(};

private:
    // 測試結�?
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試?��X��?
    int32 PassedTests;

    // 測試失�X��?
    int32 FailedTests;

    // 添�?測試結�?
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ?�建測試?�產�?    FProductionLine CreateTestProductionLine(const FString& LineName, const FString& BuildingID};

    // ?�建測試?�方
    FProductionRecipe CreateTestRecipe(const FString& RecipeName, EProductionCategory Category};

    // 驗�X�產線數X    bool VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName};

    // 驗�X�方?��?
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory};

    // 驗�X�產?�X    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus};

    // ?��X��X�方ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // ?�置測試計數X    void ResetTestCounters(};

    // ?�建測試?�置
    FProductionSystemConfig CreateTestConfig(};

    // 驗�X�置?�用
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig};

    // 等�X�產完�?
    bool WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f};

    // 模擬?��?流�?    void SimulateTimeProgress(float DeltaTime};

    // 驗�?資�?變�?
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges};
};

