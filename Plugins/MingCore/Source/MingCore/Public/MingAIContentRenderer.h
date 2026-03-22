// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Content Rendering System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentRenderer.generated.h"

UENUM(BlueprintType)
enum class ERenderQuality: uint8 {
    Low UMETA(DisplayName = "Low"),
    Medium UMETA(DisplayName = "Medium"),
    High UMETA(DisplayName = "High"),
    Ultra UMETA(DisplayName = "Ultra"),
    Cinematic UMETA(DisplayName = "Cinematic")
};

UENUM(BlueprintType)
enum class ETimeOfDay: uint8 {
    Dawn UMETA(DisplayName = "Dawn"),
    Morning UMETA(DisplayName = "Morning"),
    Noon UMETA(DisplayName = "Noon"),
    Afternoon UMETA(DisplayName = "Afternoon"),
    Dusk UMETA(DisplayName = "Dusk"),
    Evening UMETA(DisplayName = "Evening"),
    Night UMETA(DisplayName = "Night")
};

UENUM(BlueprintType)
enum class EWeatherType: uint8 {
    Clear UMETA(DisplayName = "Clear"),
    Cloudy UMETA(DisplayName = "Cloudy"),
    Rainy UMETA(DisplayName = "Rainy"),
    Stormy UMETA(DisplayName = "Stormy"),
    Foggy UMETA(DisplayName = "Foggy"),
    Snowy UMETA(DisplayName = "Snowy")
};

USTRUCT(BlueprintType)
struct FRenderConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Configuration")
    ERenderQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Configuration")
    ETimeOfDay TimeOfDay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Configuration")
    EWeatherType Weather;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Configuration")
    float AmbientLightIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Configuration")
    FLinearColor SkyColor;

    FRenderConfiguration()
        : Quality(ERenderQuality::High)
        , TimeOfDay(ETimeOfDay::Morning)
        , Weather(EWeatherType::Clear)
        , AmbientLightIntensity(1.0f)
        , SkyColor(FLinearColor::White)
    {}
};

/**
 * MingGoRTS AI Content Renderer
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIContentRenderer : public UObject
{
    GENERATED_BODY()

public:
    UMingAIContentRenderer();

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void InitializeRenderer();

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetRenderConfiguration(const FRenderConfiguration& Config);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void RenderContent(const FString& ContentPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void RenderScene(const FString& SceneDescription);

    UFUNCTION(BlueprintPure, Category = "AI Content Renderer")
    FRenderConfiguration GetCurrentConfiguration() const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetQuality(ERenderQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetTimeOfDay(ETimeOfDay TimeOfDay);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetWeather(EWeatherType Weather);

protected:
    UPROPERTY()
    FRenderConfiguration CurrentConfig;

    UPROPERTY()
    bool bIsRendering;

    void UpdateEnvironmentalEffects(float DeltaTime);
    FLinearColor GetTimeOfDayColor(ETimeOfDay TimeOfDay) const;
    FVector GetLightDirection(ETimeOfDay TimeOfDay) const;
    float GetLightIntensity(ETimeOfDay TimeOfDay, EWeatherType Weather) const;
    FLinearColor GetFogColor(EWeatherType Weather) const;
    float GetFogDensity(EWeatherType Weather) const;
};
