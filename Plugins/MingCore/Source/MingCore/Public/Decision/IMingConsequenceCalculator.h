#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceCalculator.generated.h"

// 介面類 - 後果計算器
UINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceCalculator : public UInterface
{
    GENERATED_BODY()
};

/**
 * 後果計算器介面
 * 所有具體計算器都需要實現此介面
 */
class MINGCORE_API IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 計算後果
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    void Calculate(const FMingDecisionContext& Context, FMingConsequenceResult& Result);

    // 檢查是否可以處理該決策上下文
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanHandle(const FMingDecisionContext& Context) const;

    // 獲取計算器權重 (用於排序)
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    float GetWeight() const;

    // 獲取計算器名稱
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetCalculatorName() const;

    // 獲取計算器描述
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetCalculatorDescription() const;
};
