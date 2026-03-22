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

    // ��??��t�ι�??
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree Test")
    UMingGoRTSTechTree* TechTreeSystem;

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void InitializeTest();

    // ���լ�??��??�l??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeInitialization();

    // ���լ�??�`�I��??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechNodeCreation();

    // ���լ�?X?�o�y??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechResearchFlow();

    // ����??�m�����ˬd
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestPrerequisiteChecking();

    // ���լ�?X??X?��
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechEffectApplication();

    // ���լ�???X����
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechUnlocking();

    // ���ը�?X?�o??��
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestConcurrentResearchLimit();

    // ���լ�??��????
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeConfiguration();

    // ���լ�??��??�s??���J
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestSaveAndLoad();

    // ���ե�?X??X?���??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestRepublicanEraTechs();

    // ???X???��??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool RunAllTests();

    // ????���յ�??
    UFUNCTION(BlueprintPure, Category = "Tech Tree Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
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

    // ??�ش��լ�??�`??
    FTechNode CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category);

    // ��??��??�`�I��X
    bool VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory);

    // ��?X?�o??X
    bool VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus);

    // ???X???��??ID
    FString GetRandomTechID(ETechCategory Category) const;

    // ??�m���խp��X
    void ResetTestCounters();

    // ??�ش���??�m
    FTechTreeConfig CreateTestConfig();

    // ��?X?�m??��
    bool VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig);
);
};
