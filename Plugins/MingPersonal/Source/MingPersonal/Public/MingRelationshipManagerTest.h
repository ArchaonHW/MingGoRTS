#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManagerTest.generated.h"

/**
 * ???X?羘??╰参代刚??︽竟
 * ???? Blueprint ??ㄓ??︽╰参代?? */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipTestRunner : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ???X?????玒?X???╰参代刚
     * ???X???秸刚??ㄏX     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunRelationshipTests();

    /**
     * ???X???代刚
     * @param TestIndex 代刚絪?? (1-8)
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunSpecificTest(int32 TestIndex);

    /**
     * 喷?X???╰参??セ??
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyRelationshipSystem();

    /**
     * 喷?X???╰参??セ??
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyReputationSystem();

    /**
     * ??代刚????
     * ??恶??代刚???X???玒?X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void CreateTestData();
);

