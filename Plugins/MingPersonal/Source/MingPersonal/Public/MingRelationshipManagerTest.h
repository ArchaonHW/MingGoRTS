#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManagerTest.generated.h"

/**
 * ?ú‰??åËÅ≤?õÁ≥ªÁµ±Ê∏¨Ë©¶È?Ë°åÂô®
 * ?ê‰? Blueprint ?•Âè£‰æÜÈ?Ë°åÁ≥ªÁµ±Ê∏¨Ë©? */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipTestRunner : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ?ãË??Ä?âÈ?‰øÇÂ??≤Ê?Á≥ªÁµ±Ê∏¨Ë©¶
     * ?®È??ºÂ?Ë™øË©¶?Ç‰Ωø??     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunRelationshipTests(};

    /**
     * ?ãË??πÂ?Ê∏¨Ë©¶
     * @param TestIndex Ê∏¨Ë©¶Á∑®Ë? (1-8)
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunSpecificTest(int32 TestIndex};

    /**
     * È©óË??ú‰?Á≥ªÁµ±?∫Êú¨?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyRelationshipSystem(};

    /**
     * È©óË??≤Ê?Á≥ªÁµ±?∫Êú¨?üËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyReputationSystem(};

    /**
     * ?µÂª∫Ê∏¨Ë©¶?∏Ê?
     * ?®ÊñºÂ°´Â?Ê∏¨Ë©¶?∞Â??ÑÈ?‰øÇÂ??≤Ê??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void CreateTestData(};
};

