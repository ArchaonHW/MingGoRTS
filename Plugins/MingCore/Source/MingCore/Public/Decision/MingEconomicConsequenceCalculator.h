#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingEconomicConsequenceCalculator.generated.h"

/**
 * 經??後??計?X * 專??計??決??對??濟層????影響
 * ??含資??變?X???濟??標、貿????係??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingEconomicConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建????    UMingEconomicConsequenceCalculator();

    // 實現介面????
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計??經??資??變??
    void CalculateEconomicResources(const FMingDecisionContext& Context, FMingEconomicConsequence& Resources};

    // 計??經?X???變??
    void CalculateEconomicIndicators(const FMingDecisionContext& Context, FMingEconomicConsequence& Indicators};

    // 計?X???設施變??
    void CalculateInfrastructure(const FMingDecisionContext& Context, TArray<FString>& InfrastructureChanges};

    // 計??貿?X???影響
    void CalculateTradeRelations(const FMingDecisionContext& Context, TMap<FString, int32>& TradeChanges};

    // 民?X?色計??
    void CalculateRepublicanEraEconomy(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence};

    // ??????革影響計??
    void CalculateCurrencyReformImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence};

    // 工業??影??????    void CalculateIndustrializationImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence};

    // 農業????影響計??
    void CalculateAgriculturalIssues(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence};

    // 外貿依賴影響計??
    void CalculateForeignTradeDependence(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence};

private:
    // 計?X??X    static constexpr float CALCULATOR_WEIGHT = 0.7f;

    // 民?X???貨幣類??
    static const TArray<FString> RepublicanEraCurrencies;

    // 經??資??類??
    static const TArray<FString> EconomicResourceTypes;

    // ????設施類??
    static const TArray<FString> InfrastructureTypes;

    // 貿??夥伴
    static const TArray<FString> TradePartners;
};

