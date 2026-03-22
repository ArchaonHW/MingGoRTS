#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceCalculator.generated.h"

// 介面??- 後??計?XUINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceCalculator : public UInterface
{
    GENERATED_BODY()
};

/**
 * 後??計?X??X * ????具體??算器????要實??此介面
 */
class MINGCORE_API IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 計??後??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result};

    // 檢查??否??以????該決策??下??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanHandle(const FMingDecisionContext& Context) const;

    // ????計?X??X(??於????)
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    float GetWeight() const;

    
    FString GetCalculatorName() const;

    
    FString GetCalculatorDescription() const;
};

