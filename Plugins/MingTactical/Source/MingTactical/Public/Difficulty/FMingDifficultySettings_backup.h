#pragma once

#include "CoreMinimal.h"
#include "Difficulty/EDifficultyLevel.h"
#include "FMingDifficultySettings.generated.h"

/**
 * 摧毀摧毀
 * s摧毀摧毀摧毀X??X??
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

    /** 摧毀摧毀?X?_摧毀 */
    bool AdjustValue(float Delta);

    /** ?m摧毀摧毀?X??X??? */
    void SetValue(float NewValue);
);

/**
 * 摧毀??m???
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingPresetDifficultyConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel Level = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString DisplayName = TEXT("摧毀};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString Description = TEXT("摧毀?X?");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficultyParameter, float> ParameterValues;

    FMingPresetDifficultyConfig()
    {
        InitializeDefaultValues();
    }

    void InitializeDefaultValues();
};

/**
 * ???mD?X * ?X摧毀׬???X?m
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

    /** 摧毀??m??l?? */
    void InitializeFromPreset(EDifficultyLevel Level);

    /** 目標數量??X??X*/
    FMingDifficultyParameter* GetParameter(EDifficultyParameter Type);
    const FMingDifficultyParameter* GetParameter(EDifficultyParameter Type) const;

    /** 目標數量?X*/
    float GetParameterValue(EDifficultyParameter Type) const;

    /** ?m???X*/
    void SetParameterValue(EDifficultyParameter Type, float Value);

    /** 摧毀X*/
    bool AdjustParameter(EDifficultyParameter Type, float Delta);

    /** 摧毀摧毀m摧毀?*/
    static FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level);
    static TArray<FMingPresetDifficultyConfig> GetAllPresetConfigs();

private:
    void InitializeDefaultParameters();
};

