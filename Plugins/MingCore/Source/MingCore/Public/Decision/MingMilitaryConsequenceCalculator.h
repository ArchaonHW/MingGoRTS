#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingMilitaryConsequenceCalculator.generated.h"

/**
 * �x??��??�p?X * �M??�p??�M??��??�Ƽh摧毀�v�T
 * ??�t摧毀��?X?��摧毀�m�B��目標數量
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingMilitaryConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingMilitaryConsequenceCalculator();

    // ��{����摧毀
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // �p??�x??��??��??
    void CalculateMilitaryStrength(const FMingDecisionContext& Context, FMingMilitaryConsequence& Strength);

    // �p?X?����m�v�T
    void CalculateStrategicPosition(const FMingDecisionContext& Context, TArray<FString>& PositionChanges);

    // �p?X?��摧毀?X
    void CalculateWarStatus(const FMingDecisionContext& Context, TArray<FString>& WarStatusChanges);

    // �p??�x?X???��??
    void CalculateMilitaryBudget(const FMingDecisionContext& Context, float& BudgetChange);

    // �p??�h����??
    void CalculateMorale(const FMingDecisionContext& Context, float& MoraleChange);

    // ��?X?��p??
    void CalculateRepublicanEraMilitary(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // ???X?���v�T�p??
    void CalculateNorthernExpeditionImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // ??��??���v�T�p??
    void CalculateAntiJapaneseWarImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // �x�ֲV�Լv�T�p??
    void CalculateWarlordConflictImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // �x?X?�N??�v摧毀??
    void CalculateMilitaryModernization(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

private:
    // �p?X??X
    static constexpr float CALCULATOR_WEIGHT = 0.8f;

    // ��?X???�D?X?��??��
    static const TArray<FString> RepublicanEraWars;

    // ??����m??��
    static const TArray<FString> StrategicPositions;

    // �x??��??��??
    static const TArray<FString> MilitaryEquipmentTypes;
);

