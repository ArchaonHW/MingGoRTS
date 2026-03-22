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

    // 資源系統實例
    UPROPERTY(BlueprintReadOnly, Category = "Resource Test")
    UMingGoRTSResourceSystem* ResourceSystem;

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void InitializeTest();

    // 測試基本資源收集
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestBasicResourceCollection();

    // 測試資源存儲
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceStorage();

    // 測試資源轉換
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceConversion();

    // 測試資源運輸
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTransport();

    // 測試資源交易
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTrade();

    // 測試資源衰減
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceDecay();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool RunAllTests();

    // 獲取測試結果
    UFUNCTION(BlueprintPure, Category = "Resource Test")
    FString GetTestResults() const;

    // 清理測試
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void CleanupTest();

private:
    // 測試結果
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試通過數量
    int32 PassedTests;

    // 測試失敗數量
    int32 FailedTests;

    // 添加測試結果
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // 創建測試存儲
    FString CreateTestStorage(const FString& StorageName, float Capacity);

    // 創建測試收集
    FString CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method);

    // 驗證資源數量
    bool VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity);

    // 重置測試計數器
    void ResetTestCounters();
};
