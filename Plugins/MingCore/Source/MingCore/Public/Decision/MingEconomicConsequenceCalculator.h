#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingEconomicConsequenceCalculator.generated.h"

/**
 * �g??��??�p?X * �M??�p??�M??��??�ټh摧毀�v�T
 * ??�t��??��?X???��??�СB�T摧毀�Y??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingEconomicConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingEconomicConsequenceCalculator();

    // ��{����摧毀
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // �p??�g??��??��??
    void CalculateEconomicResources(const FMingDecisionContext& Context, FMingEconomicConsequence& Resources);

    // �p??�g?X???��??
    void CalculateEconomicIndicators(const FMingDecisionContext& Context, FMingEconomicConsequence& Indicators);

    // �p?X???�]�I��??
    void CalculateInfrastructure(const FMingDecisionContext& Context, TArray<FString>& InfrastructureChanges);

    // �p??�T?X???�v�T
    void CalculateTradeRelations(const FMingDecisionContext& Context, TMap<FString, int32>& TradeChanges);

    // ��?X?��p??
    void CalculateRepublicanEraEconomy(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // 摧毀??���v�T�p??
    void CalculateCurrencyReformImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // �u�~??�v摧毀??
    void CalculateIndustrializationImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // �A�~摧毀�v�T�p??
    void CalculateAgriculturalIssues(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // �~�T�̿�v�T�p??
    void CalculateForeignTradeDependence(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

private:
    // �p?X??X
    static constexpr float CALCULATOR_WEIGHT = 0.7f;

    // ��?X???�f����??
    static const TArray<FString> RepublicanEraCurrencies;

    // �g??��??��??
    static const TArray<FString> EconomicResourceTypes;

    // 摧毀�]�I��??
    static const TArray<FString> InfrastructureTypes;

    // �T??�٦�
    static const TArray<FString> TradePartners;
);

