#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIImageGenerator.generated.h"

UENUM(BlueprintType)
enum class EMingImageStyle : uint8
{
    None UMETA(DisplayName = "None"),
    Photorealistic UMETA(DisplayName = "Photorealistic"),
    Stylized UMETA(DisplayName = "Stylized"),
    Cartoon UMETA(DisplayName = "Cartoon"),
    Watercolor UMETA(DisplayName = "Watercolor"),
    Ink UMETA(DisplayName = "Ink Painting"),
    OilPainting UMETA(DisplayName = "Oil Painting"),
    PixelArt UMETA(DisplayName = "Pixel Art"),
    LowPoly UMETA(DisplayName = "Low Poly"),
    Anime UMETA(DisplayName = "Anime"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingImageAspectRatio : uint8
{
    Square UMETA(DisplayName = "1:1 Square"),
    Portrait UMETA(DisplayName = "2:3 Portrait"),
    Landscape UMETA(DisplayName = "3:2 Landscape"),
    Widescreen UMETA(DisplayName = "16:9 Widescreen"),
    Ultrawide UMETA(DisplayName = "21:9 Ultrawide"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingImageGenerationConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    EMingImageStyle Style;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    EMingImageAspectRatio AspectRatio;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    int32 CustomWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    int32 CustomHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    int32 Steps;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    float CFGScale;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    FString Sampler;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    FString Model;

    UPROPERTY(BlueprintReadWrite, Category = "Image Generation")
    int32 BatchSize;

    FMingImageGenerationConfig()
        : Style(EMingImageStyle::Photorealistic)
        , AspectRatio(EMingImageAspectRatio::Square)
        , CustomWidth(1024)
        , CustomHeight(1024)
        , Steps(30)
        , CFGScale(7.5f)
        , Sampler(TEXT("DPM++ 2M Karras"))
        , Model(TEXT("SDXL"))
        , BatchSize(1)
    {}
};

/**
 * AI Image Generator
 * Specialized system for generating images using AI
 */
UCLASS(ClassGroup = (AI, Content), Blueprintable)
class MINGAI_API UMingAIImageGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIImageGenerator(};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void InitializeGenerator(};

    // Image Generation Methods
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateCharacterPortrait(const FString& CharacterDescription, EMingImageStyle Style, int32 Resolution};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateEnvironment(const FString& EnvironmentDescription, EMingImageStyle Style, EMingImageAspectRatio AspectRatio};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateItemIcon(const FString& ItemDescription, int32 Size};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateBuildingConcept(const FString& BuildingDescription, EMingImageStyle Style};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateTexture(const FString& TextureDescription, int32 Width, int32 Height};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GenerateUIElement(const FString& ElementDescription, int32 Width, int32 Height};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FGuid GeneratePromotionalArt(const FString& SceneDescription, EMingImageStyle Style, EMingImageAspectRatio AspectRatio};

    // Batch Generation
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    TArray<FGuid> GenerateCharacterPortraitsBatch(const TArray<FString>& CharacterDescriptions, EMingImageStyle Style};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    TArray<FGuid> GenerateEnvironmentBatch(const TArray<FString>& EnvironmentDescriptions, EMingImageStyle Style};

    // Image Processing
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    bool UpscaleImage(const FString& ImagePath, int32 TargetWidth, int32 TargetHeight};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    bool RemoveBackground(const FString& ImagePath};

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    bool ApplyStyleTransfer(const FString& SourceImagePath, const FString& StyleReferencePath};

    // Presets
    UFUNCTION(BlueprintPure, Category = "AI Image Generation")
    FMingImageGenerationConfig GetPresetConfig(const FString& PresetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void SavePresetConfig(const FString& PresetName, const FMingImageGenerationConfig& Config};

    // Prompt Helpers
    UFUNCTION(BlueprintPure, Category = "AI Image Generation")
    FString EnhancePrompt(const FString& BasePrompt, EMingImageStyle Style) const;

    UFUNCTION(BlueprintPure, Category = "AI Image Generation")
    FString GetNegativePrompt(EMingImageStyle Style) const;

protected:
    UPROPERTY()
    TMap<FString, FMingImageGenerationConfig> Presets;

    void InitializeDefaultPresets(};
    FString GetAspectRatioDimensions(EMingImageAspectRatio AspectRatio) const;
    FString BuildStylePrompt(EMingImageStyle Style) const;
    FString BuildCharacterPrompt(const FString& Description, EMingImageStyle Style) const;
    FString BuildEnvironmentPrompt(const FString& Description, EMingImageStyle Style) const;
};

#endif // MINGAIIMAGEGENERATOR_H

