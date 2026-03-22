#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManagerTest.generated.h"

/**
 * ?��X�聲?�系統測試�?行器
 * ?��? Blueprint ?�口來�?行系統測�? */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipTestRunner : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ?��X�?��?係�X��?系統測試
     * ?��X��?調試?�使X     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunRelationshipTests(};

    /**
     * ?��X��?測試
     * @param TestIndex 測試編�? (1-8)
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunSpecificTest(int32 TestIndex};

    /**
     * 驗�X��?系統?�本?�能
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyRelationshipSystem(};

    /**
     * 驗�X��?系統?�本?�能
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyReputationSystem(};

    /**
     * ?�建測試?��?
     * ?�於填�?測試?��X��?係�X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void CreateTestData(};
};

