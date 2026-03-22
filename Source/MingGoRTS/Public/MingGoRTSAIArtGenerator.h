#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MingGoRTSAIArtGenerator.generated.h"

UENUM(BlueprintType)
enum class EArtGenerationStatus : uint8
{
    Idle,
    Generating,
    Processing,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EArtStyle : uint8
{
    Realistic,         // ÂØ´ÂØ¶È¢®Ê†º
    Anime,            // ?ïÊº´È¢®Ê†º
    OilPainting,      // Ê≤πÁï´È¢®Ê†º
    Watercolor,       // Ê∞¥ÂΩ©È¢®Ê†º
    Sketch,           // Á¥†Ê?È¢®Ê†º
    TraditionalChinese, // ‰∏≠Â??≥Áµ±?´È¢®
    Military,         // Ëªç‰?È¢®Ê†º
    Historical,       // Ê≠∑Âè≤È¢®Ê†º
    ConceptArt,      // Ê¶ÇÂøµ?ùË?
    PixelArt         // ?èÁ??ùË?
};

UENUM(BlueprintType)
enum class EArtCategory : uint8
{
    Character,         // ËßíËâ≤Ë®≠Ë?
    Environment,       // ?∞Â†¥Ë®≠Ë?
    Prop,             // ?ìÂÖ∑Ë®≠Ë?
    Weapon,           // Ê≠¶Âô®Ë®≠Ë?
    Vehicle,          // ËºâÂÖ∑Ë®≠Ë?
    Architecture,     // Âª∫Á?Ë®≠Ë?
    UI,               // UIË®≠Ë?
    Icon,             // ?ñÊ?Ë®≠Ë?
    Texture,          // Á¥ãÁ?Ë®≠Ë?
    Concept           // Ê¶ÇÂøµË®≠Ë?
};

USTRUCT(BlueprintType)
struct FArtGenerationParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    EArtStyle Style = EArtStyle::Realistic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    EArtCategory Category = EArtCategory::Concept;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString Prompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString NegativePrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Width = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Height = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Steps = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    float CFGScale = 7.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString Sampler = "DPM++ 2M Karras";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Seed = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    bool bEnableControlNet = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    class UTexture2D* ControlImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString ControlNetModel = "canny";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    TArray<FString> AdditionalPrompts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    bool bEnhanceRepublicanEra = false;
};

USTRUCT(BlueprintType)
struct FArtPostProcessSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Brightness = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Contrast = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Saturation = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Gamma = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableSharpen = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float SharpenStrength = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableBlur = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float BlurRadius = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableNoise = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float NoiseStrength = 0.1f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtGenerated, class UTexture2D*, GeneratedArt};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArtGenerationCompleted, bool, bSuccess, const FString&, ErrorMessage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtGenerationProgress, float, Progress};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSAIArtGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAIArtGenerator(};

    // ‰∏ªË?Áπ™Â??üÊ??üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateArt(const FArtGenerationParameters& Parameters};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void StartArtGeneration(};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void StopArtGeneration(};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    EArtGenerationStatus GetGenerationStatus() const { return CurrentStatus; }

    // Stable Diffusion ?¥Â?
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    bool TestAPIConnection(};

    // ControlNet ?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void SetControlNetImage(UTexture2D* ControlImage, const FString& Model};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ClearControlNetImage(};

    // ?πÈ??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateArtBatch(const TArray<FArtGenerationParameters>& BatchParameters};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateVariations(UTexture2D* SourceImage, int32 VariationCount = 4};

    // ?ñÂ?ÂæåË???    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* PostProcessImage(UTexture2D* SourceImage, const FArtPostProcessSettings& Settings};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* ResizeImage(UTexture2D* SourceImage, int32 NewWidth, int32 NewHeight};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* CropImage(UTexture2D* SourceImage, int32 X, int32 Y, int32 Width, int32 Height};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* RotateImage(UTexture2D* SourceImage, float Angle};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* FlipImage(UTexture2D* SourceImage, bool bHorizontal, bool bVertical};

    // ?ùË?Ë≥áÁî¢ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void AddToArtLibrary(UTexture2D* Art, const FString& ArtName};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* GetArtFromLibrary(const FString& ArtName};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    TArray<FString> GetArtLibraryNames() const;

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ClearArtLibrary(};

    // ?êË≥™?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UMaterialInterface* GenerateMaterial(UTexture2D* BaseTexture, const FString& MaterialName};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateTextureSet(const FArtGenerationParameters& Parameters, TArray<UTexture2D*>& OutTextures};

    // ?êË®≠È¢®Ê†º
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetRepublicanEraCharacterStyle(};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetRepublicanEraEnvironmentStyle(};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetMilitaryWeaponStyle(};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetTraditionalChineseStyle(};

    // ?ñÂ?Â∞éÂá∫
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ExportArt(UTexture2D* Art, const FString& FilePath};

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ExportArtBatch(const TArray<UTexture2D*>& Arts, const FString& DirectoryPath};

    // ÂßîÊ?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerated OnArtGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerationCompleted OnArtGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerationProgress OnArtGenerationProgress;

protected:
    UPROPERTY()
    EArtGenerationStatus CurrentStatus;

    UPROPERTY()
    FString StableDiffusionEndpoint;

    UPROPERTY()
    FString StableDiffusionAPIKey;

    UPROPERTY()
    UTexture2D* CurrentGeneratedArt;

    UPROPERTY()
    UTexture2D* ControlNetImage;

    UPROPERTY()
    FString ControlNetModel;

    UPROPERTY()
    TMap<FString, UTexture2D*> ArtLibrary;

    UPROPERTY()
    TArray<UTexture2D*> GeneratedArts;

    UPROPERTY()
    bool bIsGenerating;

    UPROPERTY()
    float GenerationProgress;

    // Ticker delegates - not UPROPERTY as FTickerDelegate is not supported by UHT
    FTickerDelegate GenerationTicker;
    FDelegateHandle GenerationTickerHandle;

private:
    void ProcessArtGeneration(};
    bool OnGenerationTick(float DeltaTime};
    void SendStableDiffusionRequest(const FArtGenerationParameters& Parameters};
    void HandleGenerationResponse(bool bSuccess, const FString& ResponseData};
    FString BuildEnhancedPrompt(const FArtGenerationParameters& Parameters};
    FString GetStylePrompt(EArtStyle Style};
    FString GetCategoryPrompt(EArtCategory Category};
    UTexture2D* CreateTextureFromImageData(const TArray<uint8>& ImageData};
    void NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage = FString()};
    void NotifyGenerationProgress(float Progress};
    void ApplyPostProcessing(UTexture2D* Texture, const FArtPostProcessSettings& Settings};
    void InitializeDefaultStyles(};
};

