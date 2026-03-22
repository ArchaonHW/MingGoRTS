#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSResourceCollectionTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSResourceCollectionTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSResourceCollectionTest();

    // 資??系統實??
    UPROPERTY(BlueprintReadOnly, Category = "Resource Test")
    UMingGoRTSResourceSystem* ResourceSystem;

    // ???X?測??    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void InitializeTest();

    // 測試??本資?X???
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestBasicResourceCollection();

    // 測試資??存儲
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceStorage();

    // 測試資??轉??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceConversion();

    // 測試資?X?輸
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTransport();

    // 測試資??交??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTrade();

    // 測試資??衰??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceDecay();

    // ???X???測??    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool RunAllTests();

    // ????測試結??
    UFUNCTION(BlueprintPure, Category = "Resource Test")
    FString GetTestResults() const;

    // 清??測試
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
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

    // ??建測試存儲
    FString CreateTestStorage(const FString& StorageName, float Capacity};

    // ??建測試????
    FString CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method};

    // 驗??資?X???
    bool VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity};

    // ??置測試計數X    void ResetTestCounters();
};

