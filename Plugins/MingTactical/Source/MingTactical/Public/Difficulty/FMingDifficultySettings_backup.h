#pragma once

#include "CoreMinimal.h"
#include "Difficulty/EDifficultyLevel.h"
#include "FMingDifficultySettings.generated.h"

/**
 * ?æÂ∫¶?ÇÊï∞ÁªìÊ?
 * Â≠òÂÇ®?ï‰∏™?æÂ∫¶?ÇÊï∞?ÑÂ??çÂÄºÂ??ÉÂõ¥
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDifficultyParameter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyParameter ParameterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float CurrentValue = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float MinValue = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float MaxValue = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.01", ClampMax = "0.5"))
    float AdjustmentStep = 0.1f;

    FMingDifficultyParameter()
        : ParameterType(EDifficultyParameter::AIIntelligence)
        , CurrentValue(1.0f)
        , MinValue(0.1f)
        , MaxValue(3.0f)
        , AdjustmentStep(0.1f)
    {}

    explicit FMingDifficultyParameter(EDifficultyParameter InType, float InDefault = 1.0f)
        : ParameterType(InType)
        , CurrentValue(InDefault)
        , MinValue(0.1f)
        , MaxValue(3.0f)
        , AdjustmentStep(0.1f)
    {}

    /** Ë∞ÉÊï¥?ÇÊï∞?ºÔ?ËøîÂ??ØÂê¶ËææÂà∞ËæπÁ? */
    bool AdjustValue(float Delta};

    /** ËÆæÁΩÆ?ÇÊï∞?ºÔ?Â∏¶Ë??¥È??∂Ô? */
    void SetValue(float NewValue};
};

/**
 * È¢ÑËÆæ?æÂ∫¶?çÁΩÆÁªìÊ?
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingPresetDifficultyConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel Level = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString DisplayName = TEXT("?ÆÈÄ?};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString Description = TEXT("?áÂ?Ê∏∏Ê??æÂ∫¶"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficultyParameter, float> ParameterValues;

    FMingPresetDifficultyConfig()
    {
        InitializeDefaultValues(};
    }

    void InitializeDefaultValues(};
};

/**
 * ?æÂ∫¶ËÆæÁΩÆ‰∏ªÁ??? * ÁÆ°Á??Ä?âÈöæÂ∫¶Áõ∏?≥Á??çÁΩÆ
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDifficultySettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel CurrentLevel = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    TArray<FMingDifficultyParameter> DynamicParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    bool bEnableDynamicAdjustment = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "10.0", ClampMax = "300.0"))
    float EvaluationIntervalSeconds = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.01", ClampMax = "0.5"))
    float AdjustmentSmoothingFactor = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    bool bNotifyPlayerOnChange = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    bool bAllowMidGameChange = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "3", ClampMax = "10"))
    int32 MinEvaluationSamples = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float OscillationPreventionThreshold = 0.3f;

    FMingDifficultySettings(};

    /** ‰ªéÈ?ËÆæÈ?ÁΩÆÂ?ÂßãÂ? */
    void InitializeFromPreset(EDifficultyLevel Level};

    /** ?∑Â??áÂ?Á±ªÂ??ÑÂ???*/
    FMingDifficultyParameter* GetParameter(EDifficultyParameter Type};
    const FMingDifficultyParameter* GetParameter(EDifficultyParameter Type) const;

    /** ?∑Â??ÇÊï∞ÂΩìÂ???*/
    float GetParameterValue(EDifficultyParameter Type) const;

    /** ËÆæÁΩÆ?ÇÊï∞??*/
    void SetParameterValue(EDifficultyParameter Type, float Value};

    /** Ë∞ÉÊï¥?ÇÊï∞??*/
    bool AdjustParameter(EDifficultyParameter Type, float Delta};

    /** ?∑Â?È¢ÑËÆæ?çÁΩÆ?ôÊÄÅÊñπÊ≥?*/
    static FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level};
    static TArray<FMingPresetDifficultyConfig> GetAllPresetConfigs(};

private:
    void InitializeDefaultParameters(};
};

