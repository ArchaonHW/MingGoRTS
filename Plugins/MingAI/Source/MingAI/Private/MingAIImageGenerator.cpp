#include "MingAIImageGenerator.h"
#include "MingAIGeneratedContentSystem.h"

UMingAIImageGenerator::UMingAIImageGenerator()
{
}

void UMingAIImageGenerator::InitializeGenerator()
{
    InitializeDefaultPresets();
}

void UMingAIImageGenerator::InitializeDefaultPresets()
{
    // Character Portrait Preset
    FMingImageGenerationConfig CharacterConfig;
    CharacterConfig.Style = EMingImageStyle::Photorealistic;
    CharacterConfig.AspectRatio = EMingImageAspectRatio::Portrait;
    CharacterConfig.Steps = 40;
    CharacterConfig.CFGScale = 7.0f;
    CharacterConfig.Model = TEXT("SDXL");
    Presets.Add(TEXT("Character"), CharacterConfig);

    // Environment Preset
    FMingImageGenerationConfig EnvironmentConfig;
    EnvironmentConfig.Style = EMingImageStyle::Photorealistic;
    EnvironmentConfig.AspectRatio = EMingImageAspectRatio::Widescreen;
    EnvironmentConfig.Steps = 35;
    EnvironmentConfig.CFGScale = 7.5f;
    EnvironmentConfig.Model = TEXT("SDXL");
    Presets.Add(TEXT("Environment"), EnvironmentConfig);

    // Item Icon Preset
    FMingImageGenerationConfig IconConfig;
    IconConfig.Style = EMingImageStyle::Stylized;
    IconConfig.AspectRatio = EMingImageAspectRatio::Square;
    IconConfig.CustomWidth = 256;
    IconConfig.CustomHeight = 256;
    IconConfig.Steps = 25;
    IconConfig.CFGScale = 8.0f;
    Presets.Add(TEXT("Icon"), IconConfig);

    // UI Element Preset
    FMingImageGenerationConfig UIConfig;
    UIConfig.Style = EMingImageStyle::Stylized;
    UIConfig.AspectRatio = EMingImageAspectRatio::Square;
    UIConfig.Steps = 25;
    UIConfig.CFGScale = 7.5f;
    Presets.Add(TEXT("UI"), UIConfig);

    // Concept Art Preset
    FMingImageGenerationConfig ConceptConfig;
    ConceptConfig.Style = EMingImageStyle::Stylized;
    ConceptConfig.AspectRatio = EMingImageAspectRatio::Landscape;
    ConceptConfig.Steps = 35;
    ConceptConfig.CFGScale = 7.0f;
    Presets.Add(TEXT("Concept"), ConceptConfig);
}

FGuid UMingAIImageGenerator::GenerateCharacterPortrait(const FString& CharacterDescription, EMingImageStyle Style, int32 Resolution)
{
    FString Prompt = BuildCharacterPrompt(CharacterDescription, Style);
    FString NegativePrompt = GetNegativePrompt(Style);

    FMingImageGenerationConfig Config = GetPresetConfig(TEXT("Character"));
    Config.Style = Style;
    Config.CustomWidth = Resolution;
    Config.CustomHeight = static_cast<int32>(Resolution * 1.5f); // Portrait ratio

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.Width = Config.CustomWidth;
    Request.Height = Config.CustomHeight;
    Request.Steps = Config.Steps;
    Request.GuidanceScale = Config.CFGScale;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GenerateEnvironment(const FString& EnvironmentDescription, EMingImageStyle Style, EMingImageAspectRatio AspectRatio)
{
    FString Prompt = BuildEnvironmentPrompt(EnvironmentDescription, Style);
    FString NegativePrompt = GetNegativePrompt(Style);

    FMingImageGenerationConfig Config = GetPresetConfig(TEXT("Environment"));
    Config.Style = Style;
    Config.AspectRatio = AspectRatio;

    // Get dimensions based on aspect ratio
    FString Dimensions = GetAspectRatioDimensions(AspectRatio);
    TArray<FString> Parts;
    Dimensions.ParseIntoArray(Parts, TEXT("x"), true);
    int32 Width = FCString::Atoi(*Parts[0]);
    int32 Height = FCString::Atoi(*Parts[1]);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.Width = Width;
    Request.Height = Height;
    Request.Steps = Config.Steps;
    Request.GuidanceScale = Config.CFGScale;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GenerateItemIcon(const FString& ItemDescription, int32 Size)
{
    FString Prompt = FString::Printf(TEXT("Game item icon of %s, isolated on transparent background, detailed, high quality, game asset"), *ItemDescription);
    FString NegativePrompt = TEXT("background, scene, landscape, character, text, watermark, blurry, low quality");

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.Width = Size;
    Request.Height = Size;
    Request.Steps = 25;
    Request.GuidanceScale = 8.0f;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GenerateBuildingConcept(const FString& BuildingDescription, EMingImageStyle Style)
{
    FString StylePrompt = BuildStylePrompt(Style);
    FString Prompt = FString::Printf(TEXT("Architectural concept art of %s, %s, detailed structure, professional lighting, game concept art"),
        *BuildingDescription, *StylePrompt);
    FString NegativePrompt = GetNegativePrompt(Style);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.Width = 1024;
    Request.Height = 1024;
    Request.Steps = 35;
    Request.GuidanceScale = 7.5f;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GenerateTexture(const FString& TextureDescription, int32 Width, int32 Height)
{
    FString Prompt = FString::Printf(TEXT("Seamless texture of %s, tileable, high resolution, material texture, game asset, PBR texture"),
        *TextureDescription);
    FString NegativePrompt = TEXT("seams, visible edges, repeating pattern obvious, watermark, text, blurry");

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Texture;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.Width = Width;
    Request.Height = Height;
    Request.Steps = 30;
    Request.GuidanceScale = 7.5f;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GenerateUIElement(const FString& ElementDescription, int32 Width, int32 Height)
{
    FString Prompt = FString::Printf(TEXT("Game UI element: %s, user interface, clean design, game asset, high quality, stylized"),
        *ElementDescription);
    FString NegativePrompt = TEXT("cluttered, messy, unclear, text, watermark, blurry, low quality");

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::UI;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.Width = Width;
    Request.Height = Height;
    Request.Steps = 25;
    Request.GuidanceScale = 7.5f;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIImageGenerator::GeneratePromotionalArt(const FString& SceneDescription, EMingImageStyle Style, EMingImageAspectRatio AspectRatio)
{
    FString StylePrompt = BuildStylePrompt(Style);
    FString Prompt = FString::Printf(TEXT("Epic game promotional art of %s, %s, dramatic composition, marketing art, high detail, cinematic lighting, professional quality"),
        *SceneDescription, *StylePrompt);
    FString NegativePrompt = GetNegativePrompt(Style);

    FString Dimensions = GetAspectRatioDimensions(AspectRatio);
    TArray<FString> Parts;
    Dimensions.ParseIntoArray(Parts, TEXT("x"), true);
    int32 Width = FCString::Atoi(*Parts[0]);
    int32 Height = FCString::Atoi(*Parts[1]);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = EMingAIQualityLevel::Ultra;
    Request.Width = Width;
    Request.Height = Height;
    Request.Steps = 50;
    Request.GuidanceScale = 7.0f;

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

TArray<FGuid> UMingAIImageGenerator::GenerateCharacterPortraitsBatch(const TArray<FString>& CharacterDescriptions, EMingImageStyle Style)
{
    TArray<FGuid> Result;
    for (const FString& Description : CharacterDescriptions)
    {
        FGuid ID = GenerateCharacterPortrait(Description, Style, 1024);
        Result.Add(ID);
    }
    return Result;
}

TArray<FGuid> UMingAIImageGenerator::GenerateEnvironmentBatch(const TArray<FString>& EnvironmentDescriptions, EMingImageStyle Style)
{
    TArray<FGuid> Result;
    for (const FString& Description : EnvironmentDescriptions)
    {
        FGuid ID = GenerateEnvironment(Description, Style, EMingImageAspectRatio::Widescreen);
        Result.Add(ID);
    }
    return Result;
}

bool UMingAIImageGenerator::UpscaleImage(const FString& ImagePath, int32 TargetWidth, int32 TargetHeight)
{
    // In a real implementation, this would use an upscaling AI model
    // For now, return true as placeholder
    UE_LOG(LogTemp, Log, TEXT("Upscaling image %s to %dx%d"), *ImagePath, TargetWidth, TargetHeight);
    return true;
}

bool UMingAIImageGenerator::RemoveBackground(const FString& ImagePath)
{
    // In a real implementation, this would use a background removal AI model
    UE_LOG(LogTemp, Log, TEXT("Removing background from %s"), *ImagePath);
    return true;
}

bool UMingAIImageGenerator::ApplyStyleTransfer(const FString& SourceImagePath, const FString& StyleReferencePath)
{
    // In a real implementation, this would use style transfer AI
    UE_LOG(LogTemp, Log, TEXT("Applying style from %s to %s"), *StyleReferencePath, *SourceImagePath);
    return true;
}

FMingImageGenerationConfig UMingAIImageGenerator::GetPresetConfig(const FString& PresetName) const
{
    if (Presets.Contains(PresetName))
    {
        return Presets[PresetName];
    }
    return FMingImageGenerationConfig();
}

void UMingAIImageGenerator::SavePresetConfig(const FString& PresetName, const FMingImageGenerationConfig& Config)
{
    Presets.Add(PresetName, Config);
}

FString UMingAIImageGenerator::EnhancePrompt(const FString& BasePrompt, EMingImageStyle Style) const
{
    FString StyleKeywords = BuildStylePrompt(Style);
    return FString::Printf(TEXT("%s, %s, high quality, detailed, game asset"), *BasePrompt, *StyleKeywords);
}

FString UMingAIImageGenerator::GetNegativePrompt(EMingImageStyle Style) const
{
    FString BaseNegative = TEXT("low quality, blurry, deformed, ugly, duplicate, watermark, signature, text, bad anatomy, bad proportions, extra limbs, fused fingers, too many fingers, long neck, cross-eyed");

    switch (Style)
    {
    case EMingImageStyle::Photorealistic:
        return BaseNegative + TEXT(", cartoon, anime, painting, drawing, sketch, 3d render");
    case EMingImageStyle::Cartoon:
        return BaseNegative + TEXT(", realistic, photorealistic, 3d render");
    case EMingImageStyle::Anime:
        return BaseNegative + TEXT(", realistic, photorealistic, western cartoon");
    case EMingImageStyle::PixelArt:
        return BaseNegative + TEXT(", realistic, smooth, high resolution");
    default:
        return BaseNegative;
    }
}

FString UMingAIImageGenerator::GetAspectRatioDimensions(EMingImageAspectRatio AspectRatio) const
{
    switch (AspectRatio)
    {
    case EMingImageAspectRatio::Square:
        return TEXT("1024x1024");
    case EMingImageAspectRatio::Portrait:
        return TEXT("768x1152");
    case EMingImageAspectRatio::Landscape:
        return TEXT("1152x768");
    case EMingImageAspectRatio::Widescreen:
        return TEXT("1344x768");
    case EMingImageAspectRatio::Ultrawide:
        return TEXT("1792x768");
    default:
        return TEXT("1024x1024");
    }
}

FString UMingAIImageGenerator::BuildStylePrompt(EMingImageStyle Style) const
{
    switch (Style)
    {
    case EMingImageStyle::Photorealistic:
        return TEXT("photorealistic, realistic, 8k uhd, dslr, high quality, film grain, Fujifilm XT3");
    case EMingImageStyle::Stylized:
        return TEXT("stylized, artistic, game art, clean, vibrant colors");
    case EMingImageStyle::Cartoon:
        return TEXT("cartoon style, vibrant colors, clean lines, stylized");
    case EMingImageStyle::Watercolor:
        return TEXT("watercolor painting, artistic, soft colors, painted");
    case EMingImageStyle::Ink:
        return TEXT("ink painting, traditional Chinese art, ink wash, artistic");
    case EMingImageStyle::OilPainting:
        return TEXT("oil painting, artistic, classical art, detailed");
    case EMingImageStyle::PixelArt:
        return TEXT("pixel art, retro, 16-bit, game art");
    case EMingImageStyle::LowPoly:
        return TEXT("low poly, 3d render, stylized, geometric");
    case EMingImageStyle::Anime:
        return TEXT("anime style, manga, japanese art, detailed");
    default:
        return TEXT("high quality, detailed");
    }
}

FString UMingAIImageGenerator::BuildCharacterPrompt(const FString& Description, EMingImageStyle Style) const
{
    FString StylePrompt = BuildStylePrompt(Style);
    return FString::Printf(TEXT("Portrait of %s, %s, character design, detailed face, professional lighting, centered composition"),
        *Description, *StylePrompt);
}

FString UMingAIImageGenerator::BuildEnvironmentPrompt(const FString& Description, EMingImageStyle Style) const
{
    FString StylePrompt = BuildStylePrompt(Style);
    return FString::Printf(TEXT("Environment scene of %s, %s, landscape, atmospheric, detailed, game environment"),
        *Description, *StylePrompt);
}
