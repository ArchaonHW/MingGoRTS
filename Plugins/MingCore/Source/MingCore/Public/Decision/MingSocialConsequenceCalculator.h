#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/IMingConsequenceCalculator.h"
#include "MingSocialConsequenceCalculator.generated.h"

/**
 * 社會後果計算器
 * 專門計算決策對社會層面的影響
 * 包含民眾支持度、社會穩定性、文化影響等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSocialConsequenceCalculator : public UObject, public IMingConsequenceCalculator
{
    GENERATED_BODY()

public:
    // 建構子
    UMingSocialConsequenceCalculator();

    // 實現介面方法
    virtual void Calculate_Implementation(const FMingDecisionContext& Context, FMingConsequenceResult& Result) override;
    virtual bool CanHandle_Implementation(const FMingDecisionContext& Context) const override;
    virtual float GetWeight_Implementation() const override;
    virtual FString GetCalculatorName_Implementation() const override;
    virtual FString GetCalculatorDescription_Implementation() const override;

protected:
    // 計算民眾支持度
    void CalculatePublicSupport(const FMingDecisionContext& Context, FMingSocialConsequence& Support);

    // 計算社會穩定性
    void CalculateSocialStability(const FMingDecisionContext& Context, float& StabilityChange);

    // 計算教育水平影響
    void CalculateEducationLevel(const FMingDecisionContext& Context, float& EducationChange);

    // 計算公共健康影響
    void CalculatePublicHealth(const FMingDecisionContext& Context, float& HealthChange);

    // 計算社會運動
    void CalculateSocialMovements(const FMingDecisionContext& Context, TArray<FString>& Movements);

    // 民國特色計算
    void CalculateRepublicanEraSociety(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 新文化運動影響計算
    void CalculateNewCultureMovement(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 教育改革影響計算
    void CalculateEducationReform(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 婦女解放影響計算
    void CalculateWomenLiberation(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

    // 城市發展影響計算
    void CalculateUrbanDevelopment(const FMingDecisionContext& Context, FMingSocialConsequence& Consequence);

private:
    // 計算器權重
    static constexpr float CALCULATOR_WEIGHT = 0.6f;

    // 民國時期社會運動列表
    static const TArray<FString> RepublicanEraMovements;

    // 教育機構類型
    static const TArray<FString> EducationInstitutions;

    // 主要城市列表
    static const TArray<FString> MajorCities;

    // 社會階層
    static const TArray<FString> SocialClasses;
};
