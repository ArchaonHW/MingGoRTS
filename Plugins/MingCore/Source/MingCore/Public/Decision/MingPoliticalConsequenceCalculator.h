#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingPoliticalConsequenceCalculator.generated.h"

/**
 * 政治後果計算器
 * 專門計算決策對政治層面的影響
 * 包含派系關係、政治穩定性、外交關係等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingPoliticalConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建構子
    UMingPoliticalConsequenceCalculator();

    // 實現介面方法
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計算政治影響
    void CalculatePoliticalImpact(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // 計算派系關係變化
    void CalculateFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // 計算政治穩定性
    void CalculatePoliticalStability(const FMingDecisionContext& Context, float& StabilityChange);

    // 計算政治聲望
    void CalculatePoliticalReputation(const FMingDecisionContext& Context, float& ReputationChange);

    // 計算內部政治壓力
    void CalculateInternalPressure(const FMingDecisionContext& Context, float& PressureChange);

    // 民國特色計算
    void CalculateRepublicanEraPolitics(const FMingDecisionContext& Context, FMingPoliticalConsequence& Impact);

    // 國民黨派系關係計算
    void CalculateKMTFactionRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // 軍閥忠誠度計算
    void CalculateWarlordLoyalty(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // 國共關係計算
    void CalculateKMTCCPRelations(const FMingDecisionContext& Context, TMap<FString, float>& Relations);

    // 外交壓力計算
    void CalculateDiplomaticPressure(const FMingDecisionContext& Context, float& PressureChange);

private:
    // 計算器權重
    static constexpr float CALCULATOR_WEIGHT = 0.9f;

    // 民國時期派系列表
    static const TArray<FString> RepublicanEraFactions;

    // 軍閥列表
    static const TArray<FString> WarlordFactions;

    // 外國勢力列表
    static const TArray<FString> ForeignPowers;
};
