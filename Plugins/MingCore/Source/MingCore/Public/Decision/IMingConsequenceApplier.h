#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceApplier.generated.h"

// ä»‹é¢é¡?- å¾Œæ??‰ç”¨??UINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceApplier : public UInterface
{
    GENERATED_BODY()
};

/**
 * å¾Œæ??‰ç”¨?¨ä??? * è² è²¬å°‡è?ç®—å‡º?„å??œæ??¨åˆ°?Šæˆ²ä¸–ç?
 */
class MINGCORE_API IMingConsequenceApplier
{
    GENERATED_BODY()

public:
    // ?‰ç”¨å¾Œæ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Apply(const FMingConsequenceResult& ConsequenceResult);

    // æª¢æŸ¥?¯å¦?¯ä»¥?‰ç”¨è©²å???    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanApply(const FMingConsequenceResult& ConsequenceResult) const;

    // ?¤éŠ·å¾Œæ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Undo(const FString& ConsequenceID);

    // ?²å??‰ç”¨?¨å?ç¨?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetApplierName() const;

    // ?²å??‰ç”¨?¨æ?è¿?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetApplierDescription() const;

    // ?²å??‰ç”¨?ªå?ç´?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    int32 GetPriority() const;
};
