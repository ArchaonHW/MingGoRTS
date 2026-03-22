#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingMilitaryConsequenceCalculator.generated.h"

/**
 * è»ä?å¾Œæ?è¨ˆç??? * å°ˆé?è¨ˆç?æ±ºç?å°è?äº‹å±¤?¢ç?å½±éŸ¿
 * ?…å«?µå?è®Šå??æˆ°?¥ä?ç½®ã€æˆ°?­ç??‹ç?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingMilitaryConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingMilitaryConsequenceCalculator();

    // å¯¦ç¾ä»‹é¢?¹æ?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // è¨ˆç?è»ä?å¯¦å?è®Šå?
    void CalculateMilitaryStrength(const FMingDecisionContext& Context, FMingMilitaryConsequence& Strength);

    // è¨ˆç??°ç•¥ä½ç½®å½±éŸ¿
    void CalculateStrategicPosition(const FMingDecisionContext& Context, TArray<FString>& PositionChanges);

    // è¨ˆç??°çˆ­?€?‹è???    void CalculateWarStatus(const FMingDecisionContext& Context, TArray<FString>& WarStatusChanges);

    // è¨ˆç?è»ä??ç?è®Šå?
    void CalculateMilitaryBudget(const FMingDecisionContext& Context, float& BudgetChange);

    // è¨ˆç?å£«æ°£è®Šå?
    void CalculateMorale(const FMingDecisionContext& Context, float& MoraleChange);

    // æ°‘å??¹è‰²è¨ˆç?
    void CalculateRepublicanEraMilitary(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // ?—ä??°çˆ­å½±éŸ¿è¨ˆç?
    void CalculateNorthernExpeditionImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // ?—æ—¥?°çˆ­å½±éŸ¿è¨ˆç?
    void CalculateAntiJapaneseWarImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // è»é–¥æ··æˆ°å½±éŸ¿è¨ˆç?
    void CalculateWarlordConflictImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // è»ä??¾ä»£?–å½±?¿è?ç®?    void CalculateMilitaryModernization(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

private:
    // è¨ˆç??¨æ???    static constexpr float CALCULATOR_WEIGHT = 0.8f;

    // æ°‘å??‚æ?ä¸»è??°çˆ­?—è¡¨
    static const TArray<FString> RepublicanEraWars;

    // ?°ç•¥ä½ç½®?—è¡¨
    static const TArray<FString> StrategicPositions;

    // è»ä?è£å?é¡å?
    static const TArray<FString> MilitaryEquipmentTypes;
};
