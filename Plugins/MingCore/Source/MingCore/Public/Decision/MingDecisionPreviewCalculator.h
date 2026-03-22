#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingDecisionPreviewCalculator.generated.h"

// Ê±∫Á??êË¶Ω?∏Ê?
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

// Ê±∫Á??êË¶ΩÁµêÊ?
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
 * Ê±∫Á??êË¶ΩË®àÁ??? * ?∫Áî®?∂Ê?‰æõÊ±∫Á≠ñÂ??úÁ??êÊ∏¨?åÂª∫Ë≠? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionPreviewCalculator : public UObject
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingDecisionPreviewCalculator();

    // Ë®àÁ?Ê±∫Á??êË¶Ω
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateDecisionPreview(const FMingDecisionContext& Context, FMingDecisionPreview& OutPreview);

    // Âø´ÈÄüÈ?Ë¶?(?Ö‰∏ªË¶ÅÂΩ±??
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateQuickPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // ÊØîË?Â§öÂÄãÊ±∫Á≠ñÈÅ∏??    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CompareDecisionOptions(const TArray<FMingDecisionContext>& Options, TArray<FMingDecisionPreview>& OutComparisons);

protected:
    // ?ÜÊ??øÊ≤ªÂΩ±Èüø?êË¶Ω
    void AnalyzePoliticalPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // ?ÜÊ?Ëªç‰?ÂΩ±Èüø?êË¶Ω
    void AnalyzeMilitaryPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // ?ÜÊ?Á∂ìÊ?ÂΩ±Èüø?êË¶Ω
    void AnalyzeEconomicPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // ?ÜÊ?Á§æÊ?ÂΩ±Èüø?êË¶Ω
    void AnalyzeSocialPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // Ë®àÁ?Á∏ΩÈ?ÂΩ±Èüø?ÜÊï∏
    float CalculateOverallImpactScore(const TArray<FMingConsequencePreview>& Previews);

    // Ë≠òÂà•È¢®Èö™?†Á?
    void IdentifyRiskFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutRiskFactors);

    // Ë≠òÂà•Ê©üÊ??†Á?
    void IdentifyOpportunityFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutOpportunityFactors);

    // ?üÊ?Âª∫Ë≠∞Á∏ΩÁ?
    void GenerateRecommendationSummary(const FMingDecisionPreview& Preview, FString& OutSummary);

    // Ë®àÁ?‰ø°Â?Ê∞¥Âπ≥
    float CalculateConfidenceLevel(const FMingDecisionContext& Context);

private:
    // ?êË¶ΩË®àÁ?Ê¨äÈ?
    static constexpr float POLITICAL_WEIGHT = 0.3f;
    static constexpr float MILITARY_WEIGHT = 0.3f;
    static constexpr float ECONOMIC_WEIGHT = 0.25f;
    static constexpr float SOCIAL_WEIGHT = 0.15f;
};
