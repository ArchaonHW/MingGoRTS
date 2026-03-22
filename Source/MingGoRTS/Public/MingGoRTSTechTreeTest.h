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

    // 科??樹系統實??    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree Test")
    UMingGoRTSTechTree* TechTreeSystem;

    // ???X?測??    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void InitializeTest();

    // 測試科??樹??始??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeInitialization();

    // 測試科??節點創??    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechNodeCreation();

    // 測試科?X?發流??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechResearchFlow();

    // 測試??置條件檢查
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestPrerequisiteChecking();

    // 測試科?X??X?用
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechEffectApplication();

    // 測試科???X機制
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechUnlocking();

    // 測試並?X?發??制
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestConcurrentResearchLimit();

    // 測試科??樹????    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestTechTreeConfiguration();

    // 測試科??樹??存??載入
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestSaveAndLoad();

    // 測試民?X??X?色科??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool TestRepublicanEraTechs();

    // ???X???測??    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    bool RunAllTests();

    // ????測試結??
    UFUNCTION(BlueprintPure, Category = "Tech Tree Test")
    FString GetTestResults() const;

    // 清??測試
    UFUNCTION(BlueprintCallable, Category = "Tech Tree Test")
    void CleanupTest();

private:
    // 測試結??
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試???X???
    int32 PassedTests;

    // 測試失?X???
    int32 FailedTests;

    // 添??測試結??
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ??建測試科??節??    FTechNode CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category};

    // 驗??科??節點數X    bool VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory};

    // 驗?X?發??X    bool VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus};

    // ???X???科??ID
    FString GetRandomTechID(ETechCategory Category) const;

    // ??置測試計數X    void ResetTestCounters();

    // ??建測試??置
    FTechTreeConfig CreateTestConfig();

    // 驗?X?置??用
    bool VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig};
};

