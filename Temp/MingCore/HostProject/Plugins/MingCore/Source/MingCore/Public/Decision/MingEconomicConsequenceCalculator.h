#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingEconomicConsequenceCalculator.generated.h"

/**
 * 經濟後果計算器
 * 專門計算決策對經濟層面的影響
 * 包含資源變化、經濟指標、貿易關係等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingEconomicConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建構子
    UMingEconomicConsequenceCalculator();

    // 實現介面方法
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計算經濟資源變化
    void CalculateEconomicResources(const FMingDecisionContext& Context, FMingEconomicConsequence& Resources);

    // 計算經濟指標變化
    void CalculateEconomicIndicators(const FMingDecisionContext& Context, FMingEconomicConsequence& Indicators);

    // 計算基礎設施變化
    void CalculateInfrastructure(const FMingDecisionContext& Context, TArray<FString>& InfrastructureChanges);

    // 計算貿易關係影響
    void CalculateTradeRelations(const FMingDecisionContext& Context, TMap<FString, int32>& TradeChanges);

    // 民國特色計算
    void CalculateRepublicanEraEconomy(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // 幣制改革影響計算
    void CalculateCurrencyReformImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // 工業化影響計算
    void CalculateIndustrializationImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // 農業問題影響計算
    void CalculateAgriculturalIssues(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // 外貿依賴影響計算
    void CalculateForeignTradeDependence(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

private:
    // 計算器權重
    static constexpr float CALCULATOR_WEIGHT = 0.7f;

    // 民國時期貨幣類型
    static const TArray<FString> RepublicanEraCurrencies;

    // 經濟資源類型
    static const TArray<FString> EconomicResourceTypes;

    // 基礎設施類型
    static const TArray<FString> InfrastructureTypes;

    // 貿易夥伴
    static const TArray<FString> TradePartners;
};
