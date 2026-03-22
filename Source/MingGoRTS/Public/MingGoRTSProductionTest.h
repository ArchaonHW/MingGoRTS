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
    UMingGoRTSProductionTest();

    // ??���t�ι�??
    UPROPERTY(BlueprintReadOnly, Category = "Production Test")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void InitializeTest();

    // ����??���t��???X
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionSystemInitialization();

    // ����??���u��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineCreation();

    // ����??��??��??��
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionRecipeCreation();

    // ����??���y??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionFlow();

    // ����??��摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement();

    // ����??��摧毀�p??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation();

    // ����??��???X?�_
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume();

    // ����??��摧毀??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority();

    // ����??���u摧毀
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade();

    // ����故事重要性
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestAutomationFeatures();

    // ���ե�?X??X?��??��
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestRepublicanEraRecipes();

    // ����??����??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionStatistics();

    // ����??��??�m
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionConfiguration();

    // ����??���O??���J
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestSaveAndLoad();

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests();

    // 摧毀���յ�??
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void CleanupTest();

private:
    // ���յ�??
    UPROPERTY()
    TArray<FString> TestResults;

    // ����目標數量
    int32 PassedTests;

    // ���ե�?X???
    int32 FailedTests;

    // �K??���յ�??
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ??�ش���??��??
    FProductionLine CreateTestProductionLine(const FString& LineName, const FString& BuildingID);

    // ??�ش���??��
    FProductionRecipe CreateTestRecipe(const FString& RecipeName, EProductionCategory Category);

    // ��?X?���u��X
    bool VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName);

    // ��?X?��摧毀
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory);

    // ��?X?��??X
    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus);

    // 故事重要性?��ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // ??�m���խp��X
    void ResetTestCounters();

    // ??�ش���??�m
    FProductionSystemConfig CreateTestConfig();

    // ��?X?�m??��
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig);

    // ��?X?����??
    bool WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f);

    // ����摧毀�y??
    void SimulateTimeProgress(float DeltaTime);

    // ��??��??��??
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges);
);


