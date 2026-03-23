#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Generator - C++ Implementation
// Provides AI-powered image generation capabilities for game assets

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "MingAIImageGenerator.generated.h"

// Forward declarations
class UAIContentQualityController;

UENUM(BlueprintType)
enum class EAIImageStyle : uint8
{
    Realistic       UMETA(DisplayName = "Realistic"),
    Stylized        UMETA(DisplayName = "Stylized"),
    Cartoon         UMETA(DisplayName = "Cartoon"),
    Watercolor      UMETA(DisplayName = "Watercolor"),
    InkPainting     UMETA(DisplayName = "Ink Painting"),
    OilPainting     UMETA(DisplayName = "Oil Painting"),
    PixelArt        UMETA(DisplayName = "Pixel Art"),
    LowPoly         UMETA(DisplayName = "Low Poly"),
    CelShaded       UMETA(DisplayName = "Cel Shaded"),
    Photorealistic  UMETA(DisplayName = "Photorealistic"),
    Anime           UMETA(DisplayName = "Anime"),
    Sketch          UMETA(DisplayName = "Sketch"),
    ConceptArt      UMETA(DisplayName = "Concept Art"),
    Historical      UMETA(DisplayName = "Historical"),
    Cyberpunk       UMETA(DisplayName = "Cyberpunk")
};

UENUM(BlueprintType)
enum class EAIImageAspectRatio : uint8
{
    Square          UMETA(DisplayName = "1:1 Square"),
    Portrait        UMETA(DisplayName = "3:4 Portrait"),
    Landscape       UMETA(DisplayName = "16:9 Landscape"),
    Widescreen      UMETA(DisplayName = "21:9 Widescreen"),
    MobilePortrait  UMETA(DisplayName = "9:16 Mobile Portrait"),
    Classic         UMETA(DisplayName = "4:3 Classic"),
    Panoramic       UMETA(DisplayName = "2:1 Panoramic"),
    Custom          UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EAIImageQuality : uint8
{
    Draft           UMETA(DisplayName = "Draft - Fast"),
    Standard        UMETA(DisplayName = "Standard - Balanced"),
    High            UMETA(DisplayName = "High Quality"),
    Ultra           UMETA(DisplayName = "Ultra High"),
    Cinematic       UMETA(DisplayName = "Cinematic - Best")
};

UENUM(BlueprintType)
enum class EAIImageProvider : uint8
{
    StableDiffusion UMETA(DisplayName = "Stable Diffusion"),
    DALLE3          UMETA(DisplayName = "DALL-E 3"),
    Midjourney      UMETA(DisplayName = "Midjourney"),
    Leonardo        UMETA(DisplayName = "Leonardo AI"),
    LocalModel      UMETA(DisplayName = "Local Model"),
    CustomAPI       UMETA(DisplayName = "Custom API")
};

UENUM(BlueprintType)
enum class EAIImageGenerationStatus : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Queued          UMETA(DisplayName = "Queued"),
    Generating      UMETA(DisplayName = "Generating"),
    PostProcessing  UMETA(DisplayName = "Post Processing"),
    Completed       UMETA(DisplayName = "Completed"),
    Failed          UMETA(DisplayName = "Failed"),
    Cancelled       UMETA(DisplayName = "Cancelled")
};

USTRUCT(BlueprintType)
struct FAIImageGenerationParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    FString Prompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    FString NegativePrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    EAIImageStyle Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    EAIImageAspectRatio AspectRatio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    EAIImageQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    EAIImageProvider Provider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    int32 Seed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    int32 Width;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    int32 Height;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    int32 Steps;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    float CFGScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    FString OutputPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    FString ReferenceImagePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    float ReferenceImageStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    bool bEnableUpscaling;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image Generation")
    bool bEnableQualityCheck;

    FAIImageGenerationParams()
        : Style(EAIImageStyle::Realistic)
        , AspectRatio(EAIImageAspectRatio::Square)
        , Quality(EAIImageQuality::Standard)
        , Provider(EAIImageProvider::StableDiffusion)
        , Seed(-1)
        , Width(512)
        , Height(512)
        , Steps(30)
        , CFGScale(7.5f)
        , ReferenceImageStrength(0.7f)
        , bEnableUpscaling(false)
        , bEnableQualityCheck(true)
    {}
};

USTRUCT(BlueprintType)
struct FAIImageGenerationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString GenerationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    EAIImageGenerationStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    UTexture2D* GeneratedTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FDateTime GenerationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    int32 GenerationDurationMs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    int32 ActualSeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    float QualityScore;

    FAIImageGenerationResult()
        : Status(EAIImageGenerationStatus::Idle)
        , GeneratedTexture(nullptr)
        , Progress(0.0f)
        , GenerationDurationMs(0)
        , ActualSeed(-1)
        , QualityScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAIImageBatchRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    FString BasePrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    TArray<FString> Variations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    int32 NumImages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    FAIImageGenerationParams BaseParams;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    bool bGenerateVariations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Request")
    float VariationStrength;

    FAIImageBatchRequest()
        : NumImages(1)
        , bGenerateVariations(false)
        , VariationStrength(0.5f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageGenerationProgress, FString, GenerationID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageGenerationCompleted, FString, GenerationID, FAIImageGenerationResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageGenerationFailed, FString, GenerationID, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchGenerationCompleted, FString, BatchID, TArray<FAIImageGenerationResult>, Results);

/**
 * AI Image Generator - Core C++ class for AI-powered image generation
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAIImageGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIImageGenerator();

    // === Initialization ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void InitializeImageGenerator();

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void ShutdownImageGenerator();

    UFUNCTION(BlueprintPure, Category = "AI Image Generation")
    bool IsInitialized() const { return bIsInitialized; }

    // === Core Image Generation ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation", meta = (DisplayName = "Generate Image"))
    FString GenerateImage(const FAIImageGenerationParams& Params);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void GenerateImageAsync(const FAIImageGenerationParams& Params);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    UTexture2D* GenerateImageSync(const FAIImageGenerationParams& Params);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FAIImageGenerationResult GetGenerationResult(const FString& GenerationID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    bool CancelGeneration(const FString& GenerationID);

    // === Batch Generation ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation", meta = (DisplayName = "Generate Image Batch"))
    FString GenerateImageBatch(const FAIImageBatchRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void GenerateImageBatchAsync(const FAIImageBatchRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    TArray<FAIImageGenerationResult> GetBatchResults(const FString& BatchID) const;

    // === Image Editing ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FString EditImage(const FString& ImagePath, const FString& EditPrompt, EAIImageProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FString InpaintImage(const FString& ImagePath, const TArray<FVector2D>& MaskPoints, const FString& FillPrompt);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FString OutpaintImage(const FString& ImagePath, EAIImageAspectRatio TargetRatio, const FString& ExtensionPrompt);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FString UpscaleImage(const FString& ImagePath, int32 TargetWidth, int32 TargetHeight);

    // === Style Transfer ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FString ApplyStyleTransfer(const FString& ContentImagePath, const FString& StyleImagePath, float StyleStrength);

    // === Utility Functions ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void SetAPIKey(EAIImageProvider Provider, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void SetLocalModelPath(const FString& ModelPath);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    TArray<FString> GetAvailableProviders() const;

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void SetDefaultOutputPath(const FString& OutputPath);

    UFUNCTION(BlueprintPure, Category = "AI Image Generation")
    FString GetDefaultOutputPath() const { return DefaultOutputPath; }

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    void ClearGenerationHistory();

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    TArray<FString> GetGenerationHistory() const;

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    bool ExportGeneratedImage(const FString& GenerationID, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    UTexture2D* LoadGeneratedTexture(const FString& GenerationID);

    // === Preset Templates ===
    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FAIImageGenerationParams GetCharacterPortraitPreset(const FString& CharacterDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FAIImageGenerationParams GetEnvironmentPreset(const FString& EnvironmentDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FAIImageGenerationParams GetItemIconPreset(const FString& ItemDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Image Generation")
    FAIImageGenerationParams GetUIElementPreset(const FString& UIDescription);

    // === Events ===
    UPROPERTY(BlueprintAssignable, Category = "AI Image Generation Events")
    FOnImageGenerationProgress OnGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "AI Image Generation Events")
    FOnImageGenerationCompleted OnGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Image Generation Events")
    FOnImageGenerationFailed OnGenerationFailed;

    UPROPERTY(BlueprintAssignable, Category = "AI Image Generation Events")
    FOnBatchGenerationCompleted OnBatchGenerationCompleted;

protected:
    // === Internal Implementation ===
    void ProcessImageGenerationRequest(const FString& GenerationID, const FAIImageGenerationParams& Params);
    void ProcessBatchGenerationRequest(const FString& BatchID, const FAIImageBatchRequest& Request);
    
    bool GenerateImageWithStableDiffusion(const FString& GenerationID, const FAIImageGenerationParams& Params);
    bool GenerateImageWithDALLE(const FString& GenerationID, const FAIImageGenerationParams& Params);
    bool GenerateImageWithLocalModel(const FString& GenerationID, const FAIImageGenerationParams& Params);
    
    UTexture2D* CreateTextureFromImageData(const TArray<uint8>& ImageData);
    bool SaveTextureToFile(UTexture2D* Texture, const FString& FilePath);
    
    FString GenerateUniqueID() const;
    void UpdateGenerationProgress(const FString& GenerationID, float Progress);
    void CompleteGeneration(const FString& GenerationID, const FAIImageGenerationResult& Result);
    void FailGeneration(const FString& GenerationID, const FString& ErrorMessage);
    
    FVector2D GetDimensionsFromAspectRatio(EAIImageAspectRatio AspectRatio) const;
    FString BuildEnhancedPrompt(const FString& BasePrompt, EAIImageStyle Style) const;
    FString GetProviderAPIToken(EAIImageProvider Provider) const;

    // === HTTP Request Handlers ===
    void OnImageGenerationResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString GenerationID);
    void OnBatchItemResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString GenerationID, FString BatchID);

private:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    UAIContentQualityController* QualityController;

    UPROPERTY()
    TMap<FString, FAIImageGenerationResult> GenerationResults;

    UPROPERTY()
    TMap<FString, TArray<FString>> BatchGenerationMap;

    UPROPERTY()
    TMap<EAIImageProvider, FString> ProviderAPIKeys;

    UPROPERTY()
    FString DefaultOutputPath;

    UPROPERTY()
    FString LocalModelPath;

    UPROPERTY()
    TArray<FString> GenerationHistory;

    FCriticalSection GenerationLock;
    
    static int32 GenerationCounter;
};
