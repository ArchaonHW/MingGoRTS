#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Blueprint Library
// Provides Blueprint-accessible functions for AI image generation

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AI/MingAIImageGenerator.h"
#include "MingAIImageBlueprintLibrary.generated.h"

/**
 * Blueprint Function Library for AI Image Generation
 * Allows Blueprint users to easily access AI image generation capabilities
 */
UCLASS()
class MINGGORTS_API UMingAIImageBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // === Static Helper Functions ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Setup", meta = (CompactNodeTitle = "Create Image Params"))
    static FAIImageGenerationParams CreateImageGenerationParams(
        const FString& Prompt,
        EAIImageStyle Style,
        EAIImageQuality Quality,
        EAIImageAspectRatio AspectRatio
    );

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Setup", meta = (CompactNodeTitle = "Create Advanced Params"))
    static FAIImageGenerationParams CreateAdvancedImageParams(
        const FString& Prompt,
        const FString& NegativePrompt,
        EAIImageStyle Style,
        EAIImageQuality Quality,
        EAIImageAspectRatio AspectRatio,
        EAIImageProvider Provider,
        int32 Seed,
        int32 Steps,
        float CFGScale
    );

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Setup", meta = (CompactNodeTitle = "Create Batch Request"))
    static FAIImageBatchRequest CreateBatchRequest(
        const FString& BasePrompt,
        int32 NumImages,
        const FAIImageGenerationParams& BaseParams
    );

    // === Quick Generation Functions ===
    
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static FString QuickGenerateImage(
        UObject* WorldContextObject,
        const FString& Prompt,
        EAIImageStyle Style,
        EAIImageQuality Quality
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static UTexture2D* QuickGenerateImageSync(
        UObject* WorldContextObject,
        const FString& Prompt,
        EAIImageStyle Style,
        EAIImageQuality Quality
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static FString GenerateCharacterPortrait(
        UObject* WorldContextObject,
        const FString& CharacterDescription,
        EAIImageQuality Quality
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static FString GenerateEnvironment(
        UObject* WorldContextObject,
        const FString& EnvironmentDescription,
        EAIImageQuality Quality
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static FString GenerateItemIcon(
        UObject* WorldContextObject,
        const FString& ItemDescription,
        EAIImageQuality Quality
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quick", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
    static FString GenerateUIElement(
        UObject* WorldContextObject,
        const FString& UIDescription,
        EAIImageQuality Quality
    );

    // === Style Utilities ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static FString GetStyleDisplayName(EAIImageStyle Style);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static TArray<EAIImageStyle> GetAllImageStyles();

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static TArray<EAIImageAspectRatio> GetAllAspectRatios();

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static FVector2D GetAspectRatioDimensions(EAIImageAspectRatio AspectRatio);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static FString GetAspectRatioDisplayName(EAIImageAspectRatio AspectRatio);

    // === Provider Utilities ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static TArray<EAIImageProvider> GetAllImageProviders();

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static FString GetProviderDisplayName(EAIImageProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Setup", meta = (WorldContext = "WorldContextObject"))
    static void SetAPIKeyForProvider(
        UObject* WorldContextObject,
        EAIImageProvider Provider,
        const FString& APIKey
    );

    // === Prompt Enhancement ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities", meta = (CompactNodeTitle = "Enhance Prompt"))
    static FString EnhancePromptWithStyle(const FString& BasePrompt, EAIImageStyle Style);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Utilities")
    static FString AddNegativePrompt(const FString& BasePrompt, const TArray<FString>& ElementsToExclude);

    // === Result Handling ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Results")
    static bool IsGenerationSuccessful(const FAIImageGenerationResult& Result);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Results")
    static bool IsGenerationInProgress(const FAIImageGenerationResult& Result);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Results")
    static FString GetGenerationErrorMessage(const FAIImageGenerationResult& Result);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Results")
    static UTexture2D* GetGeneratedTexture(const FAIImageGenerationResult& Result);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Results")
    static FString GetGeneratedFilePath(const FAIImageGenerationResult& Result);

    // === Batch Operations ===
    
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Batch", meta = (WorldContext = "WorldContextObject"))
    static FString GenerateImageBatch(
        UObject* WorldContextObject,
        const FAIImageBatchRequest& Request
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Batch", meta = (WorldContext = "WorldContextObject"))
    static TArray<FAIImageGenerationResult> GetBatchResults(
        UObject* WorldContextObject,
        const FString& BatchID
    );

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Batch")
    static int32 CountSuccessfulGenerations(const TArray<FAIImageGenerationResult>& Results);

    // === Advanced Features ===
    
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Advanced", meta = (WorldContext = "WorldContextObject"))
    static FString UpscaleImage(
        UObject* WorldContextObject,
        UTexture2D* SourceTexture,
        int32 TargetWidth,
        int32 TargetHeight
    );

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Advanced", meta = (WorldContext = "WorldContextObject"))
    static FString ApplyStyleTransfer(
        UObject* WorldContextObject,
        UTexture2D* ContentImage,
        UTexture2D* StyleImage,
        float StyleStrength
    );

    // === Quality Control ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Quality")
    static float GetGenerationQualityScore(const FAIImageGenerationResult& Result);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation|Quality", meta = (WorldContext = "WorldContextObject"))
    static void EnableQualityCheck(
        UObject* WorldContextObject,
        bool bEnable
    );

    // === Preset Templates ===
    
    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "Character Preset"))
    static FAIImageGenerationParams GetCharacterPreset(const FString& Description);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "Environment Preset"))
    static FAIImageGenerationParams GetEnvironmentPreset(const FString& Description);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "Item Preset"))
    static FAIImageGenerationParams GetItemPreset(const FString& Description);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "UI Preset"))
    static FAIImageGenerationParams GetUIPreset(const FString& Description);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "Concept Art Preset"))
    static FAIImageGenerationParams GetConceptArtPreset(const FString& Description);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation|Presets", meta = (CompactNodeTitle = "Texture Preset"))
    static FAIImageGenerationParams GetTexturePreset(const FString& Description);

private:
    static UMingAIImageGenerator* GetImageGenerator(UObject* WorldContextObject);
    static TWeakObjectPtr<UMingAIImageGenerator> CachedGenerator;
};
