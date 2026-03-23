#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionTest(};

    // 動���t�ι�動
    UPROPERTY(BlueprintReadOnly, Category = "Production Test")
    UMingGoRTSProductionSystem* ProductionSystem;

    // 動池��動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void InitializeTest(};

    // ����動���t��動務
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionSystemInitialization(};

    // ����動���u��動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineCreation(};

    // ����動��動��動��
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionRecipeCreation(};

    // ����動���y動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionFlow(};

    // ����動����動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement(};

    // ����動���p動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation(};

    // ����動��動池�_
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume(};

    // ����動��動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority(};

    // ����動���u
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade(};

    // ����故事重要性
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestAutomationFeatures(};

    // ���ե�池池��動��
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestRepublicanEraRecipes(};

    // ����動����動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionStatistics(};

    // ����動��動�m
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionConfiguration(};

    // ����動���O動���J
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestSaveAndLoad(};

    // 目標數量��動
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests(};

    // ���յ�動
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // �M動����
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void CleanupTest(};

private:
    // ���յ�動
    UPROPERTY()
    TArray<FString> TestResults;

    // ����目標數量
    int32 PassedTests;

    // ���ե�池動
    int32 FailedTests;

    // �K動���յ�動
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // 動�ش���動��動
    FProductionLine CreateTestProductionLine(const FString& LineName, const FString& BuildingID};

    // 動�ش���動��
    FProductionRecipe CreateTestRecipe(const FString& RecipeName, EProductionCategory Category};

    // ��池���u��X
    bool VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName};

    // ��池��
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory};

    // ��池��動X
    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus};

    // 故事重要性ɥr��ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // 動�m���խp��X
    void ResetTestCounters(};

    // 動�ش���動�m
    FProductionSystemConfig CreateTestConfig(};

    // ��池�m動��
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig};

    // ��池����動
    bool ɥraitForProductionConpletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f};

    // �����y動
    void SimulateTimeProgress(float DeltaTime};

    // ��動��動��動
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges};
};
};
