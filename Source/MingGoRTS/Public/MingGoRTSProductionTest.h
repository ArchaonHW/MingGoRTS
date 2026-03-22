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

    // ?Ÿç”¢ç³»çµ±å¯¦ä?
    UPROPERTY(BlueprintReadOnly, Category = "Production Test")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ?å??–æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void InitializeTest();

    // æ¸¬è©¦?Ÿç”¢ç³»çµ±?å???    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionSystemInitialization();

    // æ¸¬è©¦?Ÿç”¢ç·šå‰µå»?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineCreation();

    // æ¸¬è©¦?Ÿç”¢?æ–¹?µå»º
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionRecipeCreation();

    // æ¸¬è©¦?Ÿç”¢æµç?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionFlow();

    // æ¸¬è©¦?Ÿç”¢?Šå?ç®¡ç?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionQueueManagement();

    // æ¸¬è©¦?Ÿç”¢?ˆç?è¨ˆç?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionEfficiencyCalculation();

    // æ¸¬è©¦?Ÿç”¢?«å??¢å¾©
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPauseResume();

    // æ¸¬è©¦?Ÿç”¢?ªå?ç´?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionPriority();

    // æ¸¬è©¦?Ÿç”¢ç·šå?ç´?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionLineUpgrade();

    // æ¸¬è©¦?ªå??–å???    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestAutomationFeatures();

    // æ¸¬è©¦æ°‘å??‚æ??¹è‰²?æ–¹
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestRepublicanEraRecipes();

    // æ¸¬è©¦?Ÿç”¢çµ±è?
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionStatistics();

    // æ¸¬è©¦?Ÿç”¢?ç½®
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestProductionConfiguration();

    // æ¸¬è©¦?Ÿç”¢ä¿å?è¼‰å…¥
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool TestSaveAndLoad();

    // ?‹è??€?‰æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Production Test")
    bool RunAllTests();

    // ?²å?æ¸¬è©¦çµæ?
    UFUNCTION(BlueprintPure, Category = "Production Test")
    FString GetTestResults() const;

    // æ¸…ç?æ¸¬è©¦
    UFUNCTION(BlueprintCallable, Category = "Production Test")
    void CleanupTest();

private:
    // æ¸¬è©¦çµæ?
    UPROPERTY()
    TArray<FString> TestResults;

    // æ¸¬è©¦?šé??¸é?
    int32 PassedTests;

    // æ¸¬è©¦å¤±æ??¸é?
    int32 FailedTests;

    // æ·»å?æ¸¬è©¦çµæ?
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // ?µå»ºæ¸¬è©¦?Ÿç”¢ç·?    FProductionLine CreateTestProductionLine(const FString& LineName, const FString& BuildingID);

    // ?µå»ºæ¸¬è©¦?æ–¹
    FProductionRecipe CreateTestRecipe(const FString& RecipeName, EProductionCategory Category);

    // é©—è??Ÿç”¢ç·šæ•¸??    bool VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName);

    // é©—è??æ–¹?¸æ?
    bool VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory);

    // é©—è??Ÿç”¢?€??    bool VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus);

    // ?²å??¨æ??æ–¹ID
    FString GetRandomRecipeID(EProductionCategory Category) const;

    // ?ç½®æ¸¬è©¦è¨ˆæ•¸??    void ResetTestCounters();

    // ?µå»ºæ¸¬è©¦?ç½®
    FProductionSystemConfig CreateTestConfig();

    // é©—è??ç½®?‰ç”¨
    bool VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig);

    // ç­‰å??Ÿç”¢å®Œæ?
    bool WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds = 10.0f);

    // æ¨¡æ“¬?‚é?æµé€?    void SimulateTimeProgress(float DeltaTime);

    // é©—è?è³‡æ?è®Šå?
    bool VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges);
};
