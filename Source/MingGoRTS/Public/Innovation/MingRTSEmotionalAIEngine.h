#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEmotionalAIEngine.generated.h"

UENUM(BlueprintType)
enum class EEmotionType : uint8
{
    Joy, UMETA(DisplayName = "Joy"),
    Sadness, UMETA(DisplayName = "Sadness"),
    Anger, UMETA(DisplayName = "Anger"),
    Fear, UMETA(DisplayName = "Fear"),
    Surprise, UMETA(DisplayName = "Surprise"),
    Disgust, UMETA(DisplayName = "Disgust"),
    Trust, UMETA(DisplayName = "Trust"),
    Anticipation, UMETA(DisplayName = "Anticipation"),
    Love, UMETA(DisplayName = "Love"),
    Optimism, UMETA(DisplayName = "Optimism"),
    Pessimism, UMETA(DisplayName = "Pessimism"),
    Anxiety, UMETA(DisplayName = "Anxiety"),
    Calm, UMETA(DisplayName = "Calm"),
    Excitement, UMETA(DisplayName = "Excitement"),
    Contentment, UMETA(DisplayName = "Contentment")
};

UENUM(BlueprintType)
enum class EEmotionIntensity : uint8
{
    VeryLow, UMETA(DisplayName = "Very Low"),
    Low, UMETA(DisplayName = "Low"),
    Medium, UMETA(DisplayName = "Medium"),
    High, UMETA(DisplayName = "High"),
    VeryHigh, UMETA(DisplayName = "Very High")
};

USTRUCT(BlueprintType)
struct FEmotionState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Emotion State")
    EEmotionType EmotionType;

    UPROPERTY(BlueprintReadOnly, Category = "Emotion State")
    EEmotionIntensity Intensity;

    UPROPERTY(BlueprintReadOnly, Category = "Emotion State")
    float Duration;

    UPROPERTY(BlueprintReadOnly, Category = "Emotion State")
    FText Description;

    FEmotionState()
        : EmotionType(EEmotionType::Calm)
        , Intensity(EEmotionIntensity::Medium)
        , Duration(0.0f)
        , Description(FText::GetEmpty())
    {}
};

/**
 * MingGoRTS Emotional AI Engine
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEmotionalAIEngine : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEmotionalAIEngine();

    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void InitializeEmotionalEngine();

    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void SetEmotionState(EEmotionType Emotion, EEmotionIntensity Intensity);

    UFUNCTION(BlueprintPure, Category = "Emotional AI")
    FEmotionState GetCurrentEmotionState() const;

    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void ProcessEmotionalInput(const FString& Input);

    UFUNCTION(BlueprintPure, Category = "Emotional AI")
    TArray<EEmotionType> GetDetectedEmotions() const;

protected:
    UPROPERTY()
    FEmotionState CurrentEmotionState;

    UPROPERTY()
    TArray<EEmotionType> DetectedEmotions;

    void UpdateEmotionState();
    float CalculateEmotionIntensity(const FString& Input);
    EEmotionType DetectEmotionType(const FString& Input);
};
