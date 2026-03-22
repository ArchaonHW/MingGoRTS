#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingEconomicConsequenceCalculator.generated.h"

/**
 * ç¶“æ?å¾Œæ?è¨ˆç??? * å°ˆé?è¨ˆç?æ±ºç?å°ç?æ¿Ÿå±¤?¢ç?å½±éŸ¿
 * ?…å«è³‡æ?è®Šå??ç?æ¿Ÿæ?æ¨™ã€è²¿?“é?ä¿‚ç?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingEconomicConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingEconomicConsequenceCalculator();

    // å¯¦ç¾ä»‹é¢?¹æ?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // è¨ˆç?ç¶“æ?è³‡æ?è®Šå?
    void CalculateEconomicResources(const FMingDecisionContext& Context, FMingEconomicConsequence& Resources);

    // è¨ˆç?ç¶“æ??‡æ?è®Šå?
    void CalculateEconomicIndicators(const FMingDecisionContext& Context, FMingEconomicConsequence& Indicators);

    // è¨ˆç??ºç?è¨­æ–½è®Šå?
    void CalculateInfrastructure(const FMingDecisionContext& Context, TArray<FString>& InfrastructureChanges);

    // è¨ˆç?è²¿æ??œä?å½±éŸ¿
    void CalculateTradeRelations(const FMingDecisionContext& Context, TMap<FString, int32>& TradeChanges);

    // æ°‘å??¹è‰²è¨ˆç?
    void CalculateRepublicanEraEconomy(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // å¹?ˆ¶?¹é©å½±éŸ¿è¨ˆç?
    void CalculateCurrencyReformImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // å·¥æ¥­?–å½±?¿è?ç®?    void CalculateIndustrializationImpact(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // è¾²æ¥­?é?å½±éŸ¿è¨ˆç?
    void CalculateAgriculturalIssues(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

    // å¤–è²¿ä¾è³´å½±éŸ¿è¨ˆç?
    void CalculateForeignTradeDependence(const FMingDecisionContext& Context, FMingEconomicConsequence& Consequence);

private:
    // è¨ˆç??¨æ???    static constexpr float CALCULATOR_WEIGHT = 0.7f;

    // æ°‘å??‚æ?è²¨å¹£é¡å?
    static const TArray<FString> RepublicanEraCurrencies;

    // ç¶“æ?è³‡æ?é¡å?
    static const TArray<FString> EconomicResourceTypes;

    // ?ºç?è¨­æ–½é¡å?
    static const TArray<FString> InfrastructureTypes;

    // è²¿æ?å¤¥ä¼´
    static const TArray<FString> TradePartners;
};
