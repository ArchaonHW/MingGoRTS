#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSTechTreeTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeTest(};

    // ��動��t�ι�動
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree Test")
    UMingGoRTSTechTree* TechTreeSystem;

    // 動池��動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void InitializeTest(};

    // ���լ�動��動�l動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeInitialization(};

    // ���լ�動�F事�I��動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechNodeCreation(};

    // ���լ�池�o�y動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechResearchFlow(};

    // ����動�m�����ˬd
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestPrerequisiteChecking(};

    // ���լ�池池��
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechEffectApplication(};

    // ���լ�動務����
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechUnlocking(};

    // ���ը�池�o動��
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestConcurrentResearchLimit(};

    // ���լ�動��
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeConfiguration(};

    // ���լ�動��動�s動���J
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestSaveAndLoad(};

    // ���ե�池池���動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestRepublicanEraTechs(};

    // 目標數量��動
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool RunAllTests(};

    // ���յ�動
    UFUNCTION(BlueprintPure, Category = "Tech Tree Test")
    FString GetTestResults() const;

    // �M動����
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
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

    // 動�ش��լ�動�F事動
    FTechNode CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category};

    // ��動��動�F事�I��X
    bool VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory};

    // ��池�o動X
    bool VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus};

    // 目標數量��動ID
    FString GetRandomTechID(ETechCategory Category) const;

    // 動�m���խp��X
    void ResetTestCounters(};

    // 動�ش���動�m
    FTechTreeConfig CreateTestConfig(};

    // ��池�m動��
    bool VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig};
};
};
