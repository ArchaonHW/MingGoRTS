#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingMilitaryConsequenceCalculator.generated.h"

/**
 * 軍事後果計算器
 * 專門計算決策對軍事層面的影響
 * 包含兵力變化、戰略位置、戰爭狀態等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingMilitaryConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建構子
    UMingMilitaryConsequenceCalculator();

    // 實現介面方法
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計算軍事實力變化
    void CalculateMilitaryStrength(const FMingDecisionContext& Context, FMingMilitaryConsequence& Strength);

    // 計算戰略位置影響
    void CalculateStrategicPosition(const FMingDecisionContext& Context, TArray<FString>& PositionChanges);

    // 計算戰爭狀態變化
    void CalculateWarStatus(const FMingDecisionContext& Context, TArray<FString>& WarStatusChanges);

    // 計算軍事預算變化
    void CalculateMilitaryBudget(const FMingDecisionContext& Context, float& BudgetChange);

    // 計算士氣變化
    void CalculateMorale(const FMingDecisionContext& Context, float& MoraleChange);

    // 民國特色計算
    void CalculateRepublicanEraMilitary(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // 北伐戰爭影響計算
    void CalculateNorthernExpeditionImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // 抗日戰爭影響計算
    void CalculateAntiJapaneseWarImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // 軍閥混戰影響計算
    void CalculateWarlordConflictImpact(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

    // 軍事現代化影響計算
    void CalculateMilitaryModernization(const FMingDecisionContext& Context, FMingMilitaryConsequence& Consequence);

private:
    // 計算器權重
    static constexpr float CALCULATOR_WEIGHT = 0.8f;

    // 民國時期主要戰爭列表
    static const TArray<FString> RepublicanEraWars;

    // 戰略位置列表
    static const TArray<FString> StrategicPositions;

    // 軍事裝備類型
    static const TArray<FString> MilitaryEquipmentTypes;
};
