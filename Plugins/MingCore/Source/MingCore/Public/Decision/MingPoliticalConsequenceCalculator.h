#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingPoliticalConsequenceCalculator.generated.h"

/**
 * ?¿æ²»å¾Œæ?è¨ˆç??? * å°ˆé?è¨ˆç?æ±ºç?å°æ”¿æ²»å±¤?¢ç?å½±éŸ¿
 * ?…å«æ´¾ç³»?œä??æ”¿æ²»ç©©å®šæ€§ã€å?äº¤é?ä¿‚ç?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingPoliticalConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingPoliticalConsequenceCalculator();

    // å¯¦ç¾ä»‹é¢?¹æ?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // è¨ˆç??¿æ²»å½±éŸ¿
    void CalculatePoliticalImpact(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // è¨ˆç?æ´¾ç³»?œä?è®Šå?
    void CalculateFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // è¨ˆç??¿æ²»ç©©å???    void CalculatePoliticalStability(const FMingDecisionContext& Context, float& StabilityChange);

    // è¨ˆç??¿æ²»?²æ?
    void CalculatePoliticalReputation(const FMingDecisionContext& Context, float& ReputationChange);

    // è¨ˆç??§éƒ¨?¿æ²»å£“å?
    void CalculateInternalPressure(const FMingDecisionContext& Context, float& PressureChange);

    // æ°‘å??¹è‰²è¨ˆç?
    void CalculateRepublicanEraPolitics(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // ?‹æ?é»¨æ´¾ç³»é?ä¿‚è?ç®?    void CalculateKMTFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // è»é–¥å¿ è?åº¦è?ç®?    void CalculateWarlordLoyalty(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // ?‹å…±?œä?è¨ˆç?
    void CalculateKMTCCPRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // å¤–äº¤å£“å?è¨ˆç?
    void CalculateDiplomaticPressure(const FMingDecisionContext& Context, float& PressureChange);

private:
    // è¨ˆç??¨æ???    static constexpr float CALCULATOR_WEIGHT = 0.9f;

    // æ°‘å??‚æ?æ´¾ç³»?—è¡¨
    static const TArray<FString> RepublicanEraFactions;

    // è»é–¥?—è¡¨
    static const TArray<FString> WarlordFactions;

    // å¤–å??¢å??—è¡¨
    static const TArray<FString> ForeignPowers;
};
