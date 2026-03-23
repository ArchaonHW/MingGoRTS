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
    UMingGoRTSResourceCollectionTest(};

    // ��動�t�ι�動
    UPROPERTY(BlueprintReadOnly, Category = "Resource Test")
    UMingGoRTSResourceSystem* ResourceSystem;

    // 動池��動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void InitializeTest(};

    // ����動����池動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestBasicResourceCollection(};

    // ���ո�動�s�x
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceStorage(};

    // ���ո�動��動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceConversion(};

    // ���ո�池��
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTransport(};

    // ���ո�動��動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTrade(};

    // ���ո�動�I動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceDecay(};

    // 目標數量��動
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool RunAllTests(};

    // ���յ�動
    UFUNCTION(BlueprintPure, Category = "Resource Test")
    FString GetTestResults() const;

    // �M動����
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
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

    // 動�ش��զs�x
    FString CreateTestStorage(const FString& StorageName, float Capacity};

    // 動�ش���
    FString CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method};

    // ��動��池動
    bool VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity};

    // 動�m���խp��X
    void ResetTestCounters(};
};

