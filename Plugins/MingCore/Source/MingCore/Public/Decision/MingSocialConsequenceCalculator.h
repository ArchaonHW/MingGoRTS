#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingSocialConsequenceCalculator.generated.h"

/**
 * ç¤¾æ?å¾Œæ?è¨ˆç??? * å°ˆé?è¨ˆç?æ±ºç?å°ç¤¾?ƒå±¤?¢ç?å½±éŸ¿
 * ?…å«æ°‘çœ¾?¯æ?åº¦ã€ç¤¾?ƒç©©å®šæ€§ã€æ??–å½±?¿ç?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSocialConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingSocialConsequenceCalculator();

    // å¯¦ç¾ä»‹é¢?¹æ?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // è¨ˆç?æ°‘çœ¾?¯æ?åº?    void CalculatePublicSupport(const FMingDecisionContext& Context, FMingSocialConsequence& Support);

    // è¨ˆç?ç¤¾æ?ç©©å???    void CalculateSocialStability(const FMingDecisionContext& Context, float& StabilityChange);

    // è¨ˆç??™è‚²æ°´å¹³å½±éŸ¿
    void CalculateEducationLevel(const FMingDecisionContext& Context, float& EducationChange);

    // è¨ˆç??¬å…±?¥åº·å½±éŸ¿
    void CalculatePublicHealth(const FMingDecisionContext& Context, float& HealthChange);

    // è¨ˆç?ç¤¾æ??‹å?
    void CalculateSocialMovements(const FMingDecisionContext& Context, TArray<FString>& Movements);

    // æ°‘å??¹è‰²è¨ˆç?
    void CalculateRepublicanEraSociety(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // ?°æ??–é??•å½±?¿è?ç®?    void CalculateNewCultureMovement(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // ?™è‚²?¹é©å½±éŸ¿è¨ˆç?
    void CalculateEducationReform(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // å©¦å¥³è§?”¾å½±éŸ¿è¨ˆç?
    void CalculateWomenLiberation(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // ?å??¼å?å½±éŸ¿è¨ˆç?
    void CalculateUrbanDevelopment(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

private:
    // è¨ˆç??¨æ???    static constexpr float CALCULATOR_WEIGHT = 0.6f;

    // æ°‘å??‚æ?ç¤¾æ??‹å??—è¡¨
    static const TArray<FString> RepublicanEraMovements;

    // ?™è‚²æ©Ÿæ?é¡å?
    static const TArray<FString> EducationInstitutions;

    // ä¸»è??å??—è¡¨
    static const TArray<FString> MajorCities;

    // ç¤¾æ??å±¤
    static const TArray<FString> SocialClasses;
};
