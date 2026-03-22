#pragma once

#include "CoreMinimal.h"
#include "Difficulty/EDifficultyLevel.h"
#include "FMingDifficultySettings.generated.h"

/**
 * ?曉漲?蝏?
 * 摮?葵?曉漲????澆??
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

    /** 靚??潘?餈??臬颲曉颲寧? */
    bool AdjustValue(float Delta);

    /** 霈曄蔭??潘?撣西??湧??塚? */
    void SetValue(float NewValue);
};

/**
 * 憸挽?曉漲?蔭蝏?
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingPresetDifficultyConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel Level = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString DisplayName = TEXT("?桅?);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString Description = TEXT("??皜豢??曉漲");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficultyParameter, float> ParameterValues;

    FMingPresetDifficultyConfig()
    {
        InitializeDefaultValues();
    }

    void InitializeDefaultValues();
};

/**
 * ?曉漲霈曄蔭銝餌??? * 蝞∠???摨衣?喟??蔭
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

    /** 隞?霈暸?蝵桀?憪? */
    void InitializeFromPreset(EDifficultyLevel Level);

    /** ?瑕???蝐餃?????*/
    FMingDifficultyParameter* GetParameter(EDifficultyParameter Type);
    const FMingDifficultyParameter* GetParameter(EDifficultyParameter Type) const;

    /** ?瑕??敶???*/
    float GetParameterValue(EDifficultyParameter Type) const;

    /** 霈曄蔭???*/
    void SetParameterValue(EDifficultyParameter Type, float Value);

    /** 靚???*/
    bool AdjustParameter(EDifficultyParameter Type, float Delta);

    /** ?瑕?憸挽?蔭?瘜?*/
    static FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level);
    static TArray<FMingPresetDifficultyConfig> GetAllPresetConfigs();

private:
    void InitializeDefaultParameters();
};
