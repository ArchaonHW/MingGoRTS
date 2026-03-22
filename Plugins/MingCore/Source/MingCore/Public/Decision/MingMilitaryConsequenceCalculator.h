#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingMilitaryConsequenceCalculator.generated.h"

/**
 * 軍�?後�?計�X * 專�?計�?決�?對�?事層?��?影響
 * ?�含?��?變�X�戰?��?置、戰?��X��?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingMilitaryConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建�?�?    UMingMilitaryConsequenceCalculator(};

    // 實現介面?��?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計�?軍�?實�?變�?
    void CalculateMilitaryStrength(const FMingDecisionContext& Context, FMingMilitaryConsequence& Strength};

    // 計�X�略位置影響
    void CalculateStrategicPosition(const FMingDecisionContext& Context, TArray<FString>& PositionChanges};

    // 計�X�爭?�?��X    void CalculateWarStatus(const FMingDecisionContext& Context, TArray<FString>& WarStatusChanges};

    // 計�?軍�X��?變�?
    void CalculateMilitaryBudget(const FMingDecisionContext& Context, float& BudgetChange};

    // 計�?士氣變�?
    void CalculateMorale(const FMingDecisionContext& Context, float& MoraleChange};

    // 民�X�色計�?
    void CalculateRepublicanEraMilitary(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence};

    // ?��X�爭影響計�?
    void CalculateNorthernExpeditionImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence};

    // ?�日?�爭影響計�?
    void CalculateAntiJapaneseWarImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence};

    // 軍閥混戰影響計�?
    void CalculateWarlordConflictImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence};

    // 軍�X�代?�影?��?�?    void CalculateMilitaryModernization(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence};

private:
    // 計�X��X    static constexpr float CALCULATOR_WEIGHT = 0.8f;

    // 民�X��?主�X�爭?�表
    static const TArray<FString> RepublicanEraWars;

    // ?�略位置?�表
    static const TArray<FString> StrategicPositions;

    // 軍�?裝�?類�?
    static const TArray<FString> MilitaryEquipmentTypes;
};

