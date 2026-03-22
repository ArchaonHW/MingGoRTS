#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingPoliticalConsequenceCalculator.generated.h"

/**
 * ?�治後�?計�X * 專�?計�?決�?對政治層?��?影響
 * ?�含派系?��X�政治穩定性、�?交�?係�?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingPoliticalConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建�?�?    UMingPoliticalConsequenceCalculator(};

    // 實現介面?��?
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計�X�治影響
    void CalculatePoliticalImpact(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact};

    // 計�?派系?��?變�?
    void CalculateFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations};

    // 計�X�治穩�X    void CalculatePoliticalStability(const FMingDecisionContext& Context, float& StabilityChange};

    // 計�X�治?��?
    void CalculatePoliticalReputation(const FMingDecisionContext& Context, float& ReputationChange};

    // 計�X�部?�治壓�?
    void CalculateInternalPressure(const FMingDecisionContext& Context, float& PressureChange};

    // 民�X�色計�?
    void CalculateRepublicanEraPolitics(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact};

    // ?��?黨派系�?係�?�?    void CalculateKMTFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations};

    // 軍閥忠�?度�?�?    void CalculateWarlordLoyalty(const FMingDecisionContext& Context, TMap<FString, float>& Relations};

    // ?�共?��?計�?
    void CalculateKMTCCPRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations};

    // 外交壓�?計�?
    void CalculateDiplomaticPressure(const FMingDecisionContext& Context, float& PressureChange};

private:
    // 計�X��X    static constexpr float CALCULATOR_WEIGHT = 0.9f;

    // 民�X��?派系?�表
    static const TArray<FString> RepublicanEraFactions;

    // 軍閥?�表
    static const TArray<FString> WarlordFactions;

    // 外�X��X�表
    static const TArray<FString> ForeignPowers;
};

