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

    // ç§‘æ?æ¨¹ç³»çµ±å¯¦ä¾?    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree Test")
    UMingGoRTSTechTree* TechTreeSystem;

    // ?å??–æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void InitializeTest();

    // æ¸¬è©¦ç§‘æ?æ¨¹å?å§‹å?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeInitialization();

    // æ¸¬è©¦ç§‘æ?ç¯€é»å‰µå»?    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechNodeCreation();

    // æ¸¬è©¦ç§‘æ??”ç™¼æµç?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechResearchFlow();

    // æ¸¬è©¦?ç½®æ¢ä»¶æª¢æŸ¥
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestPrerequisiteChecking();

    // æ¸¬è©¦ç§‘æ??ˆæ??‰ç”¨
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechEffectApplication();

    // æ¸¬è©¦ç§‘æ?è§??æ©Ÿåˆ¶
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechUnlocking();

    // æ¸¬è©¦ä¸¦è??”ç™¼?åˆ¶
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestConcurrentResearchLimit();

    // æ¸¬è©¦ç§‘æ?æ¨¹é?ç½?    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeConfiguration();

    // æ¸¬è©¦ç§‘æ?æ¨¹ä?å­˜å?è¼‰å…¥
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestSaveAndLoad();

    // æ¸¬è©¦æ°‘å??‚æ??¹è‰²ç§‘æ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestRepublicanEraTechs();

    // ?‹è??€?‰æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool RunAllTests();

    // ?²å?æ¸¬è©¦çµæ?
    UFUNCTION(BlueprintPure, Category = "Tech Tree Test")
    FString GetTestResults() const;

    // æ¸…ç?æ¸¬è©¦
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
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

    // ?µå»ºæ¸¬è©¦ç§‘æ?ç¯€é»?    FTechNode CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category);

    // é©—è?ç§‘æ?ç¯€é»æ•¸??    bool VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory);

    // é©—è??”ç™¼?€??    bool VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus);

    // ?²å??¨æ?ç§‘æ?ID
    FString GetRandomTechID(ETechCategory Category) const;

    // ?ç½®æ¸¬è©¦è¨ˆæ•¸??    void ResetTestCounters();

    // ?µå»ºæ¸¬è©¦?ç½®
    FTechTreeConfig CreateTestConfig();

    // é©—è??ç½®?‰ç”¨
    bool VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig);
};
