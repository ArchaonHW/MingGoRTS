#pragma once

#include "CoreMinimal.h"
#include "Difficulty/EDifficultyLevel.h"
#include "FMingDifficultySettings.generated.h"

/**
 * ??度??????
 * 存??????度??????X?值?X??
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

    /** ?整???????返?X?否?到??? */
    bool AdjustValue(float Delta};

    /** ?置?????????X??X??? */
    void SetValue(float NewValue};
};

/**
 * ????度??置???
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingPresetDifficultyConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel Level = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString DisplayName = TEXT("????};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString Description = TEXT("????游?X?度"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficultyParameter, float> ParameterValues;

    FMingPresetDifficultyConfig()
    {
        InitializeDefaultValues();
    }

    void InitializeDefaultValues();
};

/**
 * ??度?置主?X * 管?X????度相???X?置
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

    FMingDifficultySettings();

    /** ??????置??始?? */
    void InitializeFromPreset(EDifficultyLevel Level};

    /** ???X?????X??X*/
    FMingDifficultyParameter* GetParameter(EDifficultyParameter Type};
    const FMingDifficultyParameter* GetParameter(EDifficultyParameter Type) const;

    /** ???X????X*/
    float GetParameterValue(EDifficultyParameter Type) const;

    /** ?置???X*/
    void SetParameterValue(EDifficultyParameter Type, float Value};

    /** ?整???X*/
    bool AdjustParameter(EDifficultyParameter Type, float Delta};

    /** ????????置???方??*/
    static FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level};
    static TArray<FMingPresetDifficultyConfig> GetAllPresetConfigs();

private:
    void InitializeDefaultParameters();
};

