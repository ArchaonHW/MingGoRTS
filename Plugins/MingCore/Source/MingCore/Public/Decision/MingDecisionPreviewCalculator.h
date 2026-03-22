#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingDecisionPreviewCalculator.generated.h"

// �M?X?��摧毀
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

// �M?X?����??
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
 * �M?X?���p?X * ??��摧毀�ѨM��?X??X?��??��?? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionPreviewCalculator : public UObject
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingDecisionPreviewCalculator();

    // �p??�M?X?��
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateDecisionPreview(const FMingDecisionContext& Context, FMingDecisionPreview& OutPreview);

    // �ֳt摧毀(??�D�n�vX
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateQuickPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    
    void CompareDecisionOptions(const TArray<FMingDecisionContext>& Options, TArray<FMingDecisionPreview>& OutComparisons);

protected:
    // ???X?�v�v�T??��
    void AnalyzePoliticalPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 摧毀�x??�v�T??��
    void AnalyzeMilitaryPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 摧毀�g??�v�T??��
    void AnalyzeEconomicPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // 摧毀��??�v�T??��
    void AnalyzeSocialPreview(const FMingDecisionContext& Context, TArray<FMingConsequencePreview>& OutPreviews);

    // �p??�`??�v�T??��
    float CalculateOverallImpactScore(const TArray<FMingConsequencePreview>& Previews);

    // �ѧO���I摧毀
    void IdentifyRiskFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutRiskFactors);

    // �ѧO��?X???
    void IdentifyOpportunityFactors(const TArray<FMingConsequencePreview>& Previews, TArray<FString>& OutOpportunityFactors);

    // 摧毀��ĳ�`??
    void GenerateRecommendationSummary(const FMingDecisionPreview& Preview, FString& OutSummary);

    // �p??�H??���
    float CalculateConfidenceLevel(const FMingDecisionContext& Context);

private:
    // ??���p??�v??
    static constexpr float POLITICAL_WEIGHT = 0.3f;
    static constexpr float MILITARY_WEIGHT = 0.3f;
    static constexpr float ECONOMIC_WEIGHT = 0.25f;
    static constexpr float SOCIAL_WEIGHT = 0.15f;
);

