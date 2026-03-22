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

    // ����??��????��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement();

    // ����??��????�p??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation();

    // ����??��???X?�_
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume();

    // ����??��??????
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority();

    // ����??���u????
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade();

    // ����???X??X
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

    // ???X???��??
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests();

    // ????���յ�??
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void CleanupTest();

private:
    // ���յ�??
    UPROPERTY()
    TArray<FString> TestResults;

    // ����???X???
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

    // ��?X?��????
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory);

    // ��?X?��??X
    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus);

    // ???X??X?��ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // ??�m���խp��X
    void ResetTestCounters();

    // ??�ش���??�m
    FProductionSystemConfig CreateTestConfig();

    // ��?X?�m??��
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig);

    // ��?X?����??
    bool WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f);

    // ����????�y??
    void SimulateTimeProgress(float DeltaTime);

    // ��??��??��??
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges);
);


