#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSResourceCollectionTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSResourceCollectionTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSResourceCollectionTest();

    // ��??�t�ι�??
    UPROPERTY(BlueprintReadOnly, Category = "Resource Test")
    UMingGoRTSResourceSystem* ResourceSystem;

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void InitializeTest();

    // ����??����?X???
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestBasicResourceCollection();

    // ���ո�??�s�x
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceStorage();

    // ���ո�??��??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceConversion();

    // ���ո�?X?��
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTransport();

    // ���ո�??��??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTrade();

    // ���ո�??�I??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceDecay();

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool RunAllTests();

    // 摧毀���յ�??
    UFUNCTION(BlueprintPure, Category = "Resource Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
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

    // ??�ش��զs�x
    FString CreateTestStorage(const FString& StorageName, float Capacity);

    // ??�ش���摧毀
    FString CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method);

    // ��??��?X???
    bool VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity);

    // ??�m���խp��X
    void ResetTestCounters();
};

