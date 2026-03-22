#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceCalculator.generated.h"

// ä»‹é¢é¡?- å¾Œæ?è¨ˆç???UINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceCalculator : public UInterface
{
    GENERATED_BODY()
};

/**
 * å¾Œæ?è¨ˆç??¨ä??? * ?€?‰å…·é«”è?ç®—å™¨?½é?è¦å¯¦?¾æ­¤ä»‹é¢
 */
class MINGCORE_API IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // è¨ˆç?å¾Œæ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result);

    // æª¢æŸ¥?¯å¦?¯ä»¥?•ç?è©²æ±ºç­–ä?ä¸‹æ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanHandle(const FMingDecisionContext& Context) const;

    // ?²å?è¨ˆç??¨æ???(?¨æ–¼?’å?)
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    float GetWeight() const;

    // ?²å?è¨ˆç??¨å?ç¨?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetCalculatorName() const;

    // ?²å?è¨ˆç??¨æ?è¿?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetCalculatorDescription() const;
};
