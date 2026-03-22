#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingSocialConsequenceCalculator.generated.h"

/**
 * 社�?後�?計�X * 專�?計�?決�?對社?�層?��?影響
 * ?�含民眾?��?度、社?�穩定性、�X�影?��?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSocialConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建�?�?    UMingSocialConsequenceCalculator(};

    // 實現介面?��?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計�?民眾?��?�?    void CalculatePublicSupport(const FMingDecisionContext& Context, FMingSocialConsequence& Support};

    // 計�?社�?穩�X    void CalculateSocialStability(const FMingDecisionContext& Context, float& StabilityChange};

    // 計�X�育水平影響
    void CalculateEducationLevel(const FMingDecisionContext& Context, float& EducationChange};

    // 計�X�共?�康影響
    void CalculatePublicHealth(const FMingDecisionContext& Context, float& HealthChange};

    // 計�?社�X��?
    void CalculateSocialMovements(const FMingDecisionContext& Context, TArray<FString>& Movements};

    // 民�X�色計�?
    void CalculateRepublicanEraSociety(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence};

    // ?��X��X�影?��?�?    void CalculateNewCultureMovement(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence};

    // ?�育?�革影響計�?
    void CalculateEducationReform(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence};

    // 婦女�?��影響計�?
    void CalculateWomenLiberation(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence};

    // ?��X��?影響計�?
    void CalculateUrbanDevelopment(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence};

private:
    // 計�X��X    static constexpr float CALCULATOR_WEIGHT = 0.6f;

    // 民�X��?社�X��X�表
    static const TArray<FString> RepublicanEraMovements;

    // ?�育機�?類�?
    static const TArray<FString> EducationInstitutions;

    // 主�X��X�表
    static const TArray<FString> MajorCities;

    // 社�X�層
    static const TArray<FString> SocialClasses;
};

