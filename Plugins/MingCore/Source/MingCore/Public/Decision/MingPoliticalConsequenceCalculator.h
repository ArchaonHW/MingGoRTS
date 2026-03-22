#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingPoliticalConsequenceCalculator.generated.h"

/**
 * ??�v��??�p?X * �M??�p??�M??��F�v�h摧毀�v�T
 * ??�t���t???X?�F�ví�w�ʡB??��??�Y??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingPoliticalConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingPoliticalConsequenceCalculator();

    // ��{����摧毀
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // �p?X?�v�v�T
    void CalculatePoliticalImpact(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // �p??���t摧毀��??
    void CalculateFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // �p?X?�ví?X
    void CalculatePoliticalStability(const FMingDecisionContext& Context, float& StabilityChange);

    // �p?X?�v摧毀
    void CalculatePoliticalReputation(const FMingDecisionContext& Context, float& ReputationChange);

    // �p?X?��??�v��??
    void CalculateInternalPressure(const FMingDecisionContext& Context, float& PressureChange);

    // ��?X?��p??
    void CalculateRepublicanEraPolitics(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // 摧毀�Ҭ��t??�Y摧毀
    void CalculateKMTFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // �x�֩�??��摧毀
    void CalculateWarlordLoyalty(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // ??�@摧毀�p??
    void CalculateKMTCCPRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // �~����??�p??
    void CalculateDiplomaticPressure(const FMingDecisionContext& Context, float& PressureChange);

private:
    // �p?X??X
    static constexpr float CALCULATOR_WEIGHT = 0.9f;

    // ��?X???���t??��
    static const TArray<FString> RepublicanEraFactions;

    // �x��??��
    static const TArray<FString> WarlordFactions;

    // �~?X??X?��
    static const TArray<FString> ForeignPowers;
);

