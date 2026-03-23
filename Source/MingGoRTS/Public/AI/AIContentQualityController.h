// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Content Quality Controller
// Manages quality assessment and control for AI-generated content

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIContentQualityController.generated.h"

UENUM(BlueprintType)
enum class EAIContentQualityLevel: uint8 {
    VeryLow         UMETA(DisplayName = "Very Low"),
    Low             UMETA(DisplayName = "Low"),
    Medium          UMETA(DisplayName = "Medium"),
    High            UMETA(DisplayName = "High"),
    VeryHigh        UMETA(DisplayName = "Very High"),
    Excellent       UMETA(DisplayName = "Excellent")
};

UENUM(BlueprintType)
enum class EAIContentType: uint8 {
    Image           UMETA(DisplayName = "Image"),
    Video           UMETA(DisplayName = "Video"),
    Audio           UMETA(DisplayName = "Audio"),
    Text            UMETA(DisplayName = "Text"),
    Model           UMETA(DisplayName = "3D Model"),
    Texture         UMETA(DisplayName = "Texture"),
    Animation       UMETA(DisplayName = "Animation"),
    Material        UMETA(DisplayName = "Material"),
    Effect          UMETA(DisplayName = "Visual Effect"),
    Music           UMETA(DisplayName = "Music"),
    Voice           UMETA(DisplayName = "Voice")
};

UENUM(BlueprintType)
enum class EAIProvider: uint8 {
    StableDiffusion UMETA(DisplayName = "Stable Diffusion"),
    DALLE           UMETA(DisplayName = "DALL-E"),
    Midjourney      UMETA(DisplayName = "Midjourney"),
    AIVA            UMETA(DisplayName = "AIVA"),
    ElevenLabs      UMETA(DisplayName = "ElevenLabs"),
    OpenAI          UMETA(DisplayName = "OpenAI"),
    LocalModel      UMETA(DisplayName = "Local Model"),
    CustomProvider  UMETA(DisplayName = "Custom Provider")
};

USTRUCT(BlueprintType)
struct FAIContentQualityMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float TechnicalQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float AestheticQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float RelevanceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float ConsistencyScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float OriginalityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float UsabilityScore;

    FAIContentQualityMetrics()
        : TechnicalQuality(0.0f)
        , AestheticQuality(0.0f)
        , RelevanceScore(0.0f)
        , ConsistencyScore(0.0f)
        , OriginalityScore(0.0f)
        , UsabilityScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAIContentEvaluation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    EAIContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    EAIProvider Provider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    FAIContentQualityMetrics Metrics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    EAIContentQualityLevel OverallQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    FString EvaluationNotes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    bool bPassedQualityCheck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
    FDateTime EvaluationTime;

    FAIContentEvaluation()
        : ContentType(EAIContentType::Image)
        , Provider(EAIProvider::StableDiffusion)
        , OverallQuality(EAIContentQualityLevel::Medium)
        , bPassedQualityCheck(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQualityCheckCompleted, FAIContentEvaluation, Evaluation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQualityThresholdExceeded, FString, ContentID, EAIContentQualityLevel, QualityLevel);

/**
 * AI Content Quality Controller
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UAIContentQualityController : public UObject
{
    GENERATED_BODY()

public:
    UAIContentQualityController();

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void InitializeController();

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    FAIContentEvaluation EvaluateContent(const FString& ContentID, EAIContentType ContentType, const TArray<uint8>& ContentData);

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    bool IsContentApproved(const FString& ContentID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void SetQualityThreshold(EAIContentType ContentType, float MinScore);

    UFUNCTION(BlueprintPure, Category = "AI Content Quality")
    float GetQualityThreshold(EAIContentType ContentType) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void RejectContent(const FString& ContentID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void ApproveContent(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    TArray<FAIContentEvaluation> GetEvaluationHistory() const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void ClearEvaluationHistory();

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    FAIContentQualityMetrics CalculateAverageMetrics(EAIContentType ContentType) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void ExportQualityReport(const FString& FilePath);

    UPROPERTY(BlueprintAssignable, Category = "AI Content Quality Events")
    FOnQualityCheckCompleted OnQualityCheckCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Quality Events")
    FOnQualityThresholdExceeded OnQualityThresholdExceeded;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    TMap<EAIContentType, float> QualityThresholds;

    UPROPERTY()
    TArray<FAIContentEvaluation> EvaluationHistory;

    UPROPERTY()
    TMap<FString, FAIContentEvaluation> ContentEvaluations;

    float EvaluateTechnicalQuality(const TArray<uint8>& ContentData, EAIContentType ContentType);
    float EvaluateAestheticQuality(const TArray<uint8>& ContentData, EAIContentType ContentType);
    float EvaluateRelevance(const TArray<uint8>& ContentData, const FString& Prompt);
    float EvaluateConsistency(const TArray<uint8>& ContentData, EAIContentType ContentType);
    float EvaluateOriginality(const TArray<uint8>& ContentData);
    float EvaluateUsability(const TArray<uint8>& ContentData, EAIContentType ContentType);
    EAIContentQualityLevel CalculateOverallQuality(const FAIContentQualityMetrics& Metrics);
};
