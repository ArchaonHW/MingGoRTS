#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingDecisionPreviewCalculator.generated.h"

// 決策預覽數據
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequencePreview
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceType;

    UPROPERTY(BlueprintReadOnly)
    float ImpactMagnitude;

    UPROPERTY(BlueprintReadOnly)
    FString ImpactDescription;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPositive;

    UPROPERTY(BlueprintReadOnly)
    bool bIsImmediate;

    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;
};

// 決策預覽結果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionPreview
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingConsequencePreview> ConsequencePreviews;

    UPROPERTY(BlueprintReadOnly)
    float OverallImpactScore;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskFactors;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> OpportunityFactors;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendationSummary;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;
};

/**
 * 決策預覽計算器
 * 為用戶提供決策後果的預測和建議
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionPreviewCalculator : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingDecisionPreviewCalculator();

    // 計算決策預覽
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateDecisionPreview(const FMingDecisionContext& Context, FMingDecisionPreview& OutPreview);

    // 快速預覽 (僅主要影響)
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateQuickPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 比較多個決策選項
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CompareDecisionOptions(const TArray<FMingDecisionContext>& Options, TArray<FMingDecisionPreview>& OutComparisons);

protected:
    // 分析政治影響預覽
    void AnalyzePoliticalPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 分析軍事影響預覽
    void AnalyzeMilitaryPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 分析經濟影響預覽
    void AnalyzeEconomicPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 分析社會影響預覽
    void AnalyzeSocialPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 計算總體影響分數
    float CalculateOverallImpactScore(const TArray<FMingConsequencePreview>& Previews);

    // 識別風險因素
    void IdentifyRiskFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutRiskFactors);

    // 識別機會因素
    void IdentifyOpportunityFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutOpportunityFactors);

    // 生成建議總結
    void GenerateRecommendationSummary(const FMingDecisionPreview& Preview, FString& OutSummary);

    // 計算信心水平
    float CalculateConfidenceLevel(const FMingDecisionContext& Context);

private:
    // 預覽計算權重
    static constexpr float POLITICAL_WEIGHT = 0.3f;
    static constexpr float MILITARY_WEIGHT = 0.3f;
    static constexpr float ECONOMIC_WEIGHT = 0.25f;
    static constexpr float SOCIAL_WEIGHT = 0.15f;
};
