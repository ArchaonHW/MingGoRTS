#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceCalculator.generated.h"

// ??- ??p?XUINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceCalculator : public UInterface
{
    GENERATED_BODY()
};

/**
 * ??p?X??X * 摧毀??⾹摧毀n??
 */
class MINGCORE_API IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // p摧毀
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result);

    // ˬd??_??H摧毀ӨM??U??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanHandle(const FMingDecisionContext& Context) const;

    // 摧毀p?X??X(摧毀??)
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    float GetWeight() const;

    
    FString GetCalculatorName() const;

    
    FString GetCalculatorDescription() const;
};

