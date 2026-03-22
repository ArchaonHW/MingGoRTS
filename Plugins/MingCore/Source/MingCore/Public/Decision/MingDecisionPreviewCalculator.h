#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingDecisionPreviewCalculator.generated.h"

// 決?X?覽????
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

// 決?X?覽結??
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
 * 決?X?覽計?X * ??用????供決策?X??X?測??建?? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionPreviewCalculator : public UObject
{
    GENERATED_BODY()

public:
    // 建????    UMingDecisionPreviewCalculator();

    // 計??決?X?覽
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateDecisionPreview(const FMingDecisionContext& Context, FMingDecisionPreview& OutPreview};

    // 快速????(??主要影X
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateQuickPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews};

    // 比??多個決策選X    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CompareDecisionOptions(const TArray<FMingDecisionContext>& Options, TArray<FMingDecisionPreview>& OutComparisons};

protected:
    // ???X?治影響??覽
    void AnalyzePoliticalPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews};

    // ????軍??影響??覽
    void AnalyzeMilitaryPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews};

    // ????經??影響??覽
    void AnalyzeEconomicPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews};

    // ????社??影響??覽
    void AnalyzeSocialPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews};

    // 計??總??影響??數
    float CalculateOverallImpactScore(const TArray<FMingConsequencePreview>& Previews};

    // 識別風險????
    void IdentifyRiskFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutRiskFactors};

    // 識別機?X???
    void IdentifyOpportunityFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutOpportunityFactors};

    // ????建議總??
    void GenerateRecommendationSummary(const FMingDecisionPreview& Preview, FString& OutSummary};

    // 計??信??水平
    float CalculateConfidenceLevel(const FMingDecisionContext& Context};

private:
    // ??覽計??權??
    static constexpr float POLITICAL_WEIGHT = 0.3f;
    static constexpr float MILITARY_WEIGHT = 0.3f;
    static constexpr float ECONOMIC_WEIGHT = 0.25f;
    static constexpr float SOCIAL_WEIGHT = 0.15f;
};

