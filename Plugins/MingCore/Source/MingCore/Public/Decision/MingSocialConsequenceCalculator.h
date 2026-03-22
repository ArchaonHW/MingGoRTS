#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingSocialConsequenceCalculator.generated.h"

/**
 * ��??��??�p?X * �M??�p??�M??���??�h摧毀�v�T
 * ??�t����摧毀�סB��??í�w�ʡB?X?�v摧毀
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSocialConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingSocialConsequenceCalculator();

    // ��{����摧毀
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // �p??����摧毀??
    void CalculatePublicSupport(const FMingDecisionContext& Context, FMingSocialConsequence& Support);

    // �p??��??í?X
    void CalculateSocialStability(const FMingDecisionContext& Context, float& StabilityChange);

    // �p?X?�|����v�T
    void CalculateEducationLevel(const FMingDecisionContext& Context, float& EducationChange);

    // �p?X?�@??�d�v�T
    void CalculatePublicHealth(const FMingDecisionContext& Context, float& HealthChange);

    // �p??��?X???
    void CalculateSocialMovements(const FMingDecisionContext& Context, TArray<FString>& Movements);

    // ��?X?��p??
    void CalculateRepublicanEraSociety(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 故事重要性?�v摧毀??
    void CalculateNewCultureMovement(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // ??�|??���v�T�p??
    void CalculateEducationReform(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // ���k摧毀�v�T�p??
    void CalculateWomenLiberation(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 目標數量�v�T�p??
    void CalculateUrbanDevelopment(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

private:
    // �p?X??X
    static constexpr float CALCULATOR_WEIGHT = 0.6f;

    // ��?X???��?X??X?��
    static const TArray<FString> RepublicanEraMovements;

    // ??�|��??��??
    static const TArray<FString> EducationInstitutions;

    // �D?X??X?��
    static const TArray<FString> MajorCities;

    // ��?X?�h
    static const TArray<FString> SocialClasses;
);

