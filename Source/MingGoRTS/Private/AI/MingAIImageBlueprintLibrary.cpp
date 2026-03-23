// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Blueprint Library Implementation

#include "AI/MingAIImageBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

TWeakObjectPtr<UMingAIImageGenerator> UMingAIImageBlueprintLibrary::CachedGenerator = nullptr;

UMingAIImageGenerator* UMingAIImageBlueprintLibrary::GetImageGenerator(UObject* WorldContextObject)
{
    if (CachedGenerator.IsValid())
    {
        return CachedGenerator.Get();
    }

    // Create new generator
    UMingAIImageGenerator* Generator = NewObject<UMingAIImageGenerator>(GetTransientPackage());
    if (Generator)
    {
        Generator->InitializeImageGenerator();
        CachedGenerator = Generator;
    }

    return Generator;
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::CreateImageGenerationParams(
    const FString& Prompt,
    EAIImageStyle Style,
    EAIImageQuality Quality,
    EAIImageAspectRatio AspectRatio)
{
    FAIImageGenerationParams Params;
    Params.Prompt = Prompt;
    Params.Style = Style;
    Params.Quality = Quality;
    Params.AspectRatio = AspectRatio;
    Params.Provider = EAIImageProvider::StableDiffusion;
    Params.Seed = -1; // Random seed
    Params.Steps = 30;
    Params.CFGScale = 7.5f;
    
    // Set dimensions based on aspect ratio
    FVector2D Dimensions = GetAspectRatioDimensions(AspectRatio);
    Params.Width = Dimensions.X;
    Params.Height = Dimensions.Y;
    
    return Params;
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::CreateAdvancedImageParams(
    const FString& Prompt,
    const FString& NegativePrompt,
    EAIImageStyle Style,
    EAIImageQuality Quality,
    EAIImageAspectRatio AspectRatio,
    EAIImageProvider Provider,
    int32 Seed,
    int32 Steps,
    float CFGScale)
{
    FAIImageGenerationParams Params;
    Params.Prompt = Prompt;
    Params.NegativePrompt = NegativePrompt;
    Params.Style = Style;
    Params.Quality = Quality;
    Params.AspectRatio = AspectRatio;
    Params.Provider = Provider;
    Params.Seed = Seed;
    Params.Steps = Steps;
    Params.CFGScale = CFGScale;
    
    FVector2D Dimensions = GetAspectRatioDimensions(AspectRatio);
    Params.Width = Dimensions.X;
    Params.Height = Dimensions.Y;
    
    return Params;
}

FAIImageBatchRequest UMingAIImageBlueprintLibrary::CreateBatchRequest(
    const FString& BasePrompt,
    int32 NumImages,
    const FAIImageGenerationParams& BaseParams)
{
    FAIImageBatchRequest Request;
    Request.BasePrompt = BasePrompt;
    Request.NumImages = NumImages;
    Request.BaseParams = BaseParams;
    Request.bGenerateVariations = false;
    Request.VariationStrength = 0.5f;
    
    return Request;
}

FString UMingAIImageBlueprintLibrary::QuickGenerateImage(
    UObject* WorldContextObject,
    const FString& Prompt,
    EAIImageStyle Style,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get image generator"));
        return FString();
    }

    FAIImageGenerationParams Params = CreateImageGenerationParams(Prompt, Style, Quality, EAIImageAspectRatio::Square);
    return Generator->GenerateImage(Params);
}

UTexture2D* UMingAIImageBlueprintLibrary::QuickGenerateImageSync(
    UObject* WorldContextObject,
    const FString& Prompt,
    EAIImageStyle Style,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return nullptr;
    }

    FAIImageGenerationParams Params = CreateImageGenerationParams(Prompt, Style, Quality, EAIImageAspectRatio::Square);
    return Generator->GenerateImageSync(Params);
}

FString UMingAIImageBlueprintLibrary::GenerateCharacterPortrait(
    UObject* WorldContextObject,
    const FString& CharacterDescription,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return FString();
    }

    FAIImageGenerationParams Params = Generator->GetCharacterPortraitPreset(CharacterDescription);
    Params.Quality = Quality;
    return Generator->GenerateImage(Params);
}

FString UMingAIImageBlueprintLibrary::GenerateEnvironment(
    UObject* WorldContextObject,
    const FString& EnvironmentDescription,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return FString();
    }

    FAIImageGenerationParams Params = Generator->GetEnvironmentPreset(EnvironmentDescription);
    Params.Quality = Quality;
    return Generator->GenerateImage(Params);
}

FString UMingAIImageBlueprintLibrary::GenerateItemIcon(
    UObject* WorldContextObject,
    const FString& ItemDescription,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return FString();
    }

    FAIImageGenerationParams Params = Generator->GetItemIconPreset(ItemDescription);
    Params.Quality = Quality;
    return Generator->GenerateImage(Params);
}

FString UMingAIImageBlueprintLibrary::GenerateUIElement(
    UObject* WorldContextObject,
    const FString& UIDescription,
    EAIImageQuality Quality)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return FString();
    }

    FAIImageGenerationParams Params = Generator->GetUIElementPreset(UIDescription);
    Params.Quality = Quality;
    return Generator->GenerateImage(Params);
}

FString UMingAIImageBlueprintLibrary::GetStyleDisplayName(EAIImageStyle Style)
{
    const UEnum* EnumPtr = StaticEnum<EAIImageStyle>();
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue((int64)Style).ToString();
    }
    return FString(TEXT("Unknown"));
}

TArray<EAIImageStyle> UMingAIImageBlueprintLibrary::GetAllImageStyles()
{
    TArray<EAIImageStyle> Styles;
    const UEnum* EnumPtr = StaticEnum<EAIImageStyle>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            Styles.Add((EAIImageStyle)EnumPtr->GetValueByIndex(i));
        }
    }
    return Styles;
}

TArray<EAIImageAspectRatio> UMingAIImageBlueprintLibrary::GetAllAspectRatios()
{
    TArray<EAIImageAspectRatio> Ratios;
    const UEnum* EnumPtr = StaticEnum<EAIImageAspectRatio>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            Ratios.Add((EAIImageAspectRatio)EnumPtr->GetValueByIndex(i));
        }
    }
    return Ratios;
}

FVector2D UMingAIImageBlueprintLibrary::GetAspectRatioDimensions(EAIImageAspectRatio AspectRatio)
{
    switch (AspectRatio)
    {
    case EAIImageAspectRatio::Square:
        return FVector2D(512, 512);
    case EAIImageAspectRatio::Portrait:
        return FVector2D(384, 512);
    case EAIImageAspectRatio::Landscape:
        return FVector2D(768, 432);
    case EAIImageAspectRatio::Widescreen:
        return FVector2D(896, 384);
    case EAIImageAspectRatio::MobilePortrait:
        return FVector2D(288, 512);
    case EAIImageAspectRatio::Classic:
        return FVector2D(640, 480);
    case EAIImageAspectRatio::Panoramic:
        return FVector2D(1024, 512);
    case EAIImageAspectRatio::Custom:
        return FVector2D(512, 512);
    default:
        return FVector2D(512, 512);
    }
}

FString UMingAIImageBlueprintLibrary::GetAspectRatioDisplayName(EAIImageAspectRatio AspectRatio)
{
    const UEnum* EnumPtr = StaticEnum<EAIImageAspectRatio>();
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue((int64)AspectRatio).ToString();
    }
    return FString(TEXT("Unknown"));
}

TArray<EAIImageProvider> UMingAIImageBlueprintLibrary::GetAllImageProviders()
{
    TArray<EAIImageProvider> Providers;
    const UEnum* EnumPtr = StaticEnum<EAIImageProvider>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            Providers.Add((EAIImageProvider)EnumPtr->GetValueByIndex(i));
        }
    }
    return Providers;
}

FString UMingAIImageBlueprintLibrary::GetProviderDisplayName(EAIImageProvider Provider)
{
    const UEnum* EnumPtr = StaticEnum<EAIImageProvider>();
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue((int64)Provider).ToString();
    }
    return FString(TEXT("Unknown"));
}

void UMingAIImageBlueprintLibrary::SetAPIKeyForProvider(
    UObject* WorldContextObject,
    EAIImageProvider Provider,
    const FString& APIKey)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (Generator)
    {
        Generator->SetAPIKey(Provider, APIKey);
    }
}

FString UMingAIImageBlueprintLibrary::EnhancePromptWithStyle(const FString& BasePrompt, EAIImageStyle Style)
{
    FString StyleModifier;
    
    switch (Style)
    {
    case EAIImageStyle::Realistic:
        StyleModifier = TEXT("photorealistic, highly detailed, 8k, professional photography");
        break;
    case EAIImageStyle::Stylized:
        StyleModifier = TEXT("stylized, artistic, vibrant colors, game art");
        break;
    case EAIImageStyle::Cartoon:
        StyleModifier = TEXT("cartoon style, cute, colorful, animated");
        break;
    case EAIImageStyle::Watercolor:
        StyleModifier = TEXT("watercolor painting, artistic, soft colors, traditional art");
        break;
    case EAIImageStyle::InkPainting:
        StyleModifier = TEXT("Chinese ink painting, traditional art, black and white, artistic");
        break;
    case EAIImageStyle::OilPainting:
        StyleModifier = TEXT("oil painting, classical art, rich colors, textured");
        break;
    case EAIImageStyle::PixelArt:
        StyleModifier = TEXT("pixel art, retro, 16-bit style, game graphics");
        break;
    case EAIImageStyle::LowPoly:
        StyleModifier = TEXT("low poly, geometric, 3D render, minimalist");
        break;
    case EAIImageStyle::CelShaded:
        StyleModifier = TEXT("cel shaded, anime style, bold outlines, vibrant");
        break;
    case EAIImageStyle::Photorealistic:
        StyleModifier = TEXT("photorealistic, ultra detailed, 8k, ray tracing, cinematic lighting");
        break;
    case EAIImageStyle::Anime:
        StyleModifier = TEXT("anime style, Japanese animation, colorful, detailed");
        break;
    case EAIImageStyle::Sketch:
        StyleModifier = TEXT("pencil sketch, hand drawn, artistic, monochrome");
        break;
    case EAIImageStyle::ConceptArt:
        StyleModifier = TEXT("concept art, digital painting, detailed, professional");
        break;
    case EAIImageStyle::Historical:
        StyleModifier = TEXT("historical art, period accurate, detailed, realistic");
        break;
    case EAIImageStyle::Cyberpunk:
        StyleModifier = TEXT("cyberpunk, neon lights, futuristic, sci-fi, high tech");
        break;
    default:
        StyleModifier = TEXT("high quality, detailed");
        break;
    }

    return FString::Printf(TEXT("%s, %s"), *BasePrompt, *StyleModifier);
}

FString UMingAIImageBlueprintLibrary::AddNegativePrompt(const FString& BasePrompt, const TArray<FString>& ElementsToExclude)
{
    FString NegativePrompt;
    for (const FString& Element : ElementsToExclude)
    {
        if (!NegativePrompt.IsEmpty())
        {
            NegativePrompt += TEXT(", ");
        }
        NegativePrompt += Element;
    }
    return NegativePrompt;
}

bool UMingAIImageBlueprintLibrary::IsGenerationSuccessful(const FAIImageGenerationResult& Result)
{
    return Result.Status == EAIImageGenerationStatus::Completed && Result.GeneratedTexture != nullptr;
}

bool UMingAIImageBlueprintLibrary::IsGenerationInProgress(const FAIImageGenerationResult& Result)
{
    return Result.Status == EAIImageGenerationStatus::Queued ||
           Result.Status == EAIImageGenerationStatus::Generating ||
           Result.Status == EAIImageGenerationStatus::PostProcessing;
}

FString UMingAIImageBlueprintLibrary::GetGenerationErrorMessage(const FAIImageGenerationResult& Result)
{
    if (Result.Status == EAIImageGenerationStatus::Failed)
    {
        return Result.ErrorMessage;
    }
    return FString();
}

UTexture2D* UMingAIImageBlueprintLibrary::GetGeneratedTexture(const FAIImageGenerationResult& Result)
{
    return Result.GeneratedTexture;
}

FString UMingAIImageBlueprintLibrary::GetGeneratedFilePath(const FAIImageGenerationResult& Result)
{
    return Result.FilePath;
}

FString UMingAIImageBlueprintLibrary::GenerateImageBatch(
    UObject* WorldContextObject,
    const FAIImageBatchRequest& Request)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return FString();
    }

    return Generator->GenerateImageBatch(Request);
}

TArray<FAIImageGenerationResult> UMingAIImageBlueprintLibrary::GetBatchResults(
    UObject* WorldContextObject,
    const FString& BatchID)
{
    UMingAIImageGenerator* Generator = GetImageGenerator(WorldContextObject);
    if (!Generator)
    {
        return TArray<FAIImageGenerationResult>();
    }

    return Generator->GetBatchResults(BatchID);
}

int32 UMingAIImageBlueprintLibrary::CountSuccessfulGenerations(const TArray<FAIImageGenerationResult>& Results)
{
    int32 Count = 0;
    for (const FAIImageGenerationResult& Result : Results)
    {
        if (IsGenerationSuccessful(Result))
        {
            Count++;
        }
    }
    return Count;
}

FString UMingAIImageBlueprintLibrary::UpscaleImage(
    UObject* WorldContextObject,
    UTexture2D* SourceTexture,
    int32 TargetWidth,
    int32 TargetHeight)
{
    // TODO: Implement upscaling
    UE_LOG(LogTemp, Warning, TEXT("UpscaleImage not yet implemented"));
    return FString();
}

FString UMingAIImageBlueprintLibrary::ApplyStyleTransfer(
    UObject* WorldContextObject,
    UTexture2D* ContentImage,
    UTexture2D* StyleImage,
    float StyleStrength)
{
    // TODO: Implement style transfer
    UE_LOG(LogTemp, Warning, TEXT("ApplyStyleTransfer not yet implemented"));
    return FString();
}

float UMingAIImageBlueprintLibrary::GetGenerationQualityScore(const FAIImageGenerationResult& Result)
{
    return Result.QualityScore;
}

void UMingAIImageBlueprintLibrary::EnableQualityCheck(
    UObject* WorldContextObject,
    bool bEnable)
{
    // Quality check is enabled by default in the generator
    // This function can be used to toggle it globally if needed
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetCharacterPreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("character portrait of %s, detailed face, professional lighting"), *Description),
        EAIImageStyle::Realistic,
        EAIImageQuality::High,
        EAIImageAspectRatio::Portrait
    );
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetEnvironmentPreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("%s, environment concept art, detailed, atmospheric lighting"), *Description),
        EAIImageStyle::ConceptArt,
        EAIImageQuality::High,
        EAIImageAspectRatio::Landscape
    );
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetItemPreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("%s, item icon, game asset, centered, white background"), *Description),
        EAIImageStyle::Stylized,
        EAIImageQuality::Standard,
        EAIImageAspectRatio::Square
    );
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetUIPreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("%s, UI element, game interface, clean design, modern"), *Description),
        EAIImageStyle::Stylized,
        EAIImageQuality::High,
        EAIImageAspectRatio::Square
    );
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetConceptArtPreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("%s, concept art, digital painting, detailed, professional"), *Description),
        EAIImageStyle::ConceptArt,
        EAIImageQuality::High,
        EAIImageAspectRatio::Landscape
    );
}

FAIImageGenerationParams UMingAIImageBlueprintLibrary::GetTexturePreset(const FString& Description)
{
    return CreateImageGenerationParams(
        FString::Printf(TEXT("%s, seamless texture, tileable, high resolution"), *Description),
        EAIImageStyle::Realistic,
        EAIImageQuality::High,
        EAIImageAspectRatio::Square
    );
}
