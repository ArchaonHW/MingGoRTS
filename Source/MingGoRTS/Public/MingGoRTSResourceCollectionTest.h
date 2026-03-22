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
    UMingGoRTSResourceCollectionTest(};

    // Ë≥áÊ?Á≥ªÁµ±ÂØ¶‰?
    UPROPERTY(BlueprintReadOnly, Category = "Resource Test")
    UMingGoRTSResourceSystem* ResourceSystem;

    // ?ùÂ??ñÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void InitializeTest(};

    // Ê∏¨Ë©¶?∫Êú¨Ë≥áÊ??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestBasicResourceCollection(};

    // Ê∏¨Ë©¶Ë≥áÊ?Â≠òÂÑ≤
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceStorage(};

    // Ê∏¨Ë©¶Ë≥áÊ?ËΩâÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceConversion(};

    // Ê∏¨Ë©¶Ë≥áÊ??ãËº∏
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTransport(};

    // Ê∏¨Ë©¶Ë≥áÊ?‰∫§Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceTrade(};

    // Ê∏¨Ë©¶Ë≥áÊ?Ë°∞Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool TestResourceDecay(};

    // ?ãË??Ä?âÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    bool RunAllTests(};

    // ?≤Â?Ê∏¨Ë©¶ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Resource Test")
    FString GetTestResults() const;

    // Ê∏ÖÁ?Ê∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Resource Test")
    void CleanupTest(};

private:
    // Ê∏¨Ë©¶ÁµêÊ?
    UPROPERTY()
    TArray<FString> TestResults;

    // Ê∏¨Ë©¶?öÈ??∏È?
    int32 PassedTests;

    // Ê∏¨Ë©¶Â§±Ê??∏È?
    int32 FailedTests;

    // Ê∑ªÂ?Ê∏¨Ë©¶ÁµêÊ?
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ?µÂª∫Ê∏¨Ë©¶Â≠òÂÑ≤
    FString CreateTestStorage(const FString& StorageName, float Capacity};

    // ?µÂª∫Ê∏¨Ë©¶?∂È?
    FString CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method};

    // È©óË?Ë≥áÊ??∏È?
    bool VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity};

    // ?çÁΩÆÊ∏¨Ë©¶Ë®àÊï∏??    void ResetTestCounters(};
};

